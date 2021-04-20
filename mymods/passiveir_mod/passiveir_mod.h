#pragma once

#include "wled.h"

/*
 * Usermods allow you to add own functionality to WLED more easily
 * See: https://github.com/Aircoookie/WLED/wiki/Add-own-functionality
 * 
 * This is an example for a v2 usermod.
 * v2 usermods are class inheritance based and can (but don't have to) implement more functions, each of them is shown in this example.
 * Multiple v2 usermods can be added to one compilation easily.
 * 
 * Creating a usermod:
 * This file serves as an example. If you want to create a usermod, it is recommended to use usermod_v2_empty.h from the usermods folder as a template.
 * Please remember to rename the class and file to a descriptive name.
 * You may also use multiple .h and .cpp files.
 * 
 * Using a usermod:
 * 1. Copy the usermod into the sketch folder (same folder as wled00.ino)
 * 2. Register the usermod by adding #include "usermod_filename.h" in the top and registerUsermod(new MyUsermodClass()) in the bottom of usermods_list.cpp
 */

#define PASSIVE_IR_PIN 27
#define CHECK_INTERVAL_MS 500

//class name. Use something descriptive and leave the ": public Usermod" part :)
class PassiveIRMod : public Usermod
{
private:
  //Private class members. You can declare variables and functions only accessible to your usermod here
  unsigned long _lastUpdateTs = 0;
  byte _PIRsensorPinState = LOW;
  byte _briLast = bri;
  byte _effectLast = effectCurrent;
  unsigned long _offTimeoutTs = 0;
  unsigned long _lastTurnedOffTs = 0;
  boolean _isOff = bri == 0;
  bool _inPirWindow = false;

  const byte NotifyUpdateMode = NOTIFIER_CALL_MODE_NO_NOTIFY;

  /**
     * return or change if new PIR sensor state is available
     */
  static volatile bool getOrSetPIRsensorState(bool changeState = false, bool newState = false);

  /**
     * PIR sensor state has changed
     */
  static void IRAM_ATTR ISR_PIRstateChange();

  /**
     * Set/get instance pointer
     */
  static PassiveIRMod *PassiveIRModInstance(PassiveIRMod *instance = nullptr, bool removeInstance = false);

public:
  static PassiveIRMod *GetInstance() { return PassiveIRModInstance(); }

  //Functions called by WLED

  /*
     * setup() is called once at boot. WiFi is not yet connected at this point.
     * You can use it to initialize variables, sensors or similar.
     */
  void setup()
  {
    pinMode(PASSIVE_IR_PIN, INPUT_PULLUP);

    // assign interrupt function and set CHANGE mode
    attachInterrupt(digitalPinToInterrupt(PASSIVE_IR_PIN), ISR_PIRstateChange, CHANGE);
  }

  void toggleStripPower(bool switchOn)
  {
    if (switchOn && bri == 0) {
      _briLast = bri;
      _effectLast = effectCurrent;
      applyPreset(pirActivationPreset);
      //bri = MAX(pirActivationBrightness, 50);
      effectCurrent = FX_MODE_STATIC;
      DEBUG_PRINTLN("PIR triggered ON");
      colorUpdated(NotifyUpdateMode);
    } else if (!switchOn && bri != 0) {
      briLast = bri;
      bri = 0;
      DEBUG_PRINTLN("PIR triggered OFF");
      colorUpdated(NotifyUpdateMode);
    }
  }

  bool isOff()
  {
    return bri == 0;
  }

  bool isOn() {
    return bri != 0;
  }

  bool updatePIRsensorState()
  {
    bool currentState = getOrSetPIRsensorState();
    if (currentState) {
      _PIRsensorPinState = digitalRead(PASSIVE_IR_PIN);

      // if the pir goes high and the strip is currently off, let's turn on      
      if (_PIRsensorPinState == HIGH && isOff() && (_lastTurnedOffTs == 0 || (millis() - _lastTurnedOffTs) > (pirActivationCooldownTimeSec*1000))) {
        _offTimeoutTs = 0;
        toggleStripPower(true);
      } else if (isOn()) {
        _offTimeoutTs = millis();
        //toggleStripPower(false);
        // start switch off timer
        //m_offTimerStart = millis();
      }
      getOrSetPIRsensorState(true, false);
      return true;
    }
    return false;
  }

  /*
     * loop() is called continuously. Here you can check for events, read sensors, etc.
     * 
     * Tips:
     * 1. You can use "if (WLED_CONNECTED)" to check for a successful network connection.
     *    Additionally, "if (WLED_MQTT_CONNECTED)" is available to check for a connection to an MQTT broker.
     * 
     * 2. Try to avoid using the delay() function. NEVER use delays longer than 10 milliseconds.
     *    Instead, use a timer check as shown here.
     */
  void loop()
  {
    // track when the lights were last turned off
    if (!_isOff && isOff()) {
      _isOff = true;
      _lastTurnedOffTs = millis();
      DEBUG_PRINTLN("PIR: LED Power off detected");
    } else if (_isOff && !isOff()) {
      _isOff = false;
      _lastTurnedOffTs = 0;
      DEBUG_PRINTLN("PIR: LED Power on detected");
    }

    if (!pirEnabled) {
      return;
    }

    // update our localtime
    updateLocalTime();    

    uint16_t curTimeCombined = hour(localTime) * 100 + minute(localTime); 
    _inPirWindow = (curTimeCombined >= (pirActiveFromHour * 100 + pirActiveFromMin)) && (curTimeCombined < (pirActiveToHour * 100 + pirActiveToMin));

    if (!_inPirWindow) {
      return;
    }

    if (millis() - _lastUpdateTs > 1000) {
      DEBUG_PRINT("PIR: ");
      DEBUG_PRINT(_PIRsensorPinState);      
      DEBUG_PRINT(" ");
      DEBUG_PRINTLN(digitalRead(PASSIVE_IR_PIN));
      _lastUpdateTs = millis();
    }

    if (!updatePIRsensorState()) {
      // if the preset is changed, then a user has changed the device settings, so we are done here
      if (currentPreset != pirActivationPreset) {
        _offTimeoutTs = 0;
      }

      // do we have an active off timer?
      if (_offTimeoutTs > 0 && millis() - _offTimeoutTs > (pirStayActivatedTimeSec * 1000)) {
        Serial.println("PIR Timeout");
        toggleStripPower(false);
        _offTimeoutTs = 0;
      }
    }
  }

  /*
     * addToJsonInfo() can be used to add custom entries to the /json/info part of the JSON API.
     * Creating an "u" object allows you to add custom key/value pairs to the Info section of the WLED web UI.
     * Below it is shown how this could be used for e.g. a light sensor
     */
  
    void addToJsonInfo(JsonObject& root)
    {
      char str[32];

      JsonObject user = root["u"];
      if (user.isNull()) user = root.createNestedObject("u");
     
      JsonArray tempArr = user.createNestedArray("PIR"); //name      
      tempArr.add(_PIRsensorPinState > 0 ? "Motion" : " No Motion"); //value      

      JsonArray tempArr2 = user.createNestedArray("PIR Active Time"); //name      
      tempArr2.add(_inPirWindow ? "Yes" : "No"); //value      

      JsonArray tempArr3 = user.createNestedArray("Time of Day"); //name      
      getTimeString(str);
      tempArr3.add(str); //value      
    }
    

  /*
     * addToJsonState() can be used to add custom entries to the /json/state part of the JSON API (state object).
     * Values in the state object may be modified by connected clients
     */
  void addToJsonState(JsonObject &root)
  {
    //root["pir_activation_brightness"] = _defaultActivationBrightness;
  }

  /*
     * readFromJsonState() can be used to receive data clients send to the /json/state part of the JSON API (state object).
     * Values in the state object may be modified by connected clients
     */
  void readFromJsonState(JsonObject &root)
  {
    //_defaultActivationBrightness = root["pir_activation_brightness"] | _defaultActivationBrightness; //if "user0" key exists in JSON, update, else keep old value
    //if (root["bri"] == 255) Serial.println(F("Don't burn down your garage!"));
  }

  /*
     * addToConfig() can be used to add custom persistent settings to the cfg.json file in the "um" (usermod) object.
     * It will be called by WLED when settings are actually saved (for example, LED settings are saved)
     * If you want to force saving the current state, use serializeConfig() in your loop().
     * 
     * CAUTION: serializeConfig() will initiate a filesystem write operation.
     * It might cause the LEDs to stutter and will cause flash wear if called too often.
     * Use it sparingly and always in the loop, never in network callbacks!
     * 
     * addToConfig() will also not yet add your setting to one of the settings pages automatically.
     * To make that work you still have to add the setting to the HTML, xml.cpp and set.cpp manually.
     * 
     * I highly recommend checking out the basics of ArduinoJson serialization and deserialization in order to use custom settings!
     */
  void addToConfig(JsonObject &root)
  {
    //JsonObject top = root.createNestedObject("passivepir_mod");
    //top["activation_brightness"] = _defaultActivationBrightness; //save this var persistently whenever settings are saved
  }

  /*
     * readFromConfig() can be used to read back the custom settings you added with addToConfig().
     * This is called by WLED when settings are loaded (currently this only happens once immediately after boot)
     * 
     * readFromConfig() is called BEFORE setup(). This means you can use your persistent values in setup() (e.g. pin assignments, buffer sizes),
     * but also that if you want to write persistent values to a dynamic buffer, you'd need to allocate it here instead of in setup.
     * If you don't know what that is, don't fret. It most likely doesn't affect your use case :)
     */
  void readFromConfig(JsonObject &root)
  {
    //JsonObject top = root["top"];
    //_defaultActivationBrightness = top["activation_brightness"] | DEFAULT_ACTIVATION_BRIGTNESS; //The value right of the pipe "|" is the default value in case your setting was not present in cfg.json (e.g. first boot)
  }

  /*
     * getId() allows you to optionally give your V2 usermod an unique ID (please define it in const.h!).
     * This could be used in the future for the system to determine whether your usermod is installed.
     */
  uint16_t getId()
  {
    return USERMOD_ID_PASSIVEPIR;
  }

  //More methods can be added in the future, this example will then be extended.
  //Your usermod will remain compatible as it does not need to implement all methods from the Usermod base class!
};

//////////////////////////////////////////////////////
// PIRsensorSwitch static method implementations

volatile bool PassiveIRMod::getOrSetPIRsensorState(bool changeState, bool newState)
{
  static volatile bool s_PIRsensorState = false;
  if (changeState) {
    s_PIRsensorState = newState;
  }
  return s_PIRsensorState;
}

void IRAM_ATTR PassiveIRMod::ISR_PIRstateChange()
{
  getOrSetPIRsensorState(true, true);
}

PassiveIRMod *PassiveIRMod::PassiveIRModInstance(PassiveIRMod *instance, bool removeInstance)
{
  static PassiveIRMod *thisInstance = nullptr;
  if (instance != nullptr || removeInstance) {
    thisInstance = instance;
  }
  return thisInstance;
}