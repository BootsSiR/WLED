#pragma once

#include "wled.h"
#include "../mcp3208/mcp3208_mod.h"
#include <deque>

class CurrentMonitoringMod : public Usermod
{
private:
    unsigned long _lastUpdateTs = 0;
    float _led1Current = 0.0f;
    float _led2Current = 0.0f;
public:

    /*
     * setup() is called once at boot. WiFi is not yet connected at this point.
     * You can use it to initialize variables, sensors or similar.
     */
    void setup()
    {
           
    }

    /*
     * connected() is called every time the WiFi is (re)connected
     * Use it to initialize network interfaces
     */
    void connected()
    {    

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
    
    //float cutOffLimit = 1.00;
    const float FACTOR = (60.0 * (3.3/5.0)) / 1000;
    // const float FACTOR = 60.0 / 1000;
    const float QOV = 0.12f * 3.3f;// set quiescent Output voltage for selected model
    //float cutOff = FACTOR/cutOffLimit;// convert current cut off to mV


    //uint16_t offset = 450;
    //float voltagePerMv = 3.3 / 4096;


int miliVoltsPerAmp = 60; //See the diagram for scale factors
int acOffset = 600;  //See the diagram for offset value
int rawSense = 0;
double voltageValue = 0;
double amp = 0;

    float analogToCurrent(uint16_t adcValue, float correctionValue) {
        //Robojax.com ACS758 Current Sensor 
        float voltage_raw =   (3.3 / 4095.0) * adcValue; // Read the voltage from sensor

        
        float voltage =  voltage_raw - QOV + correctionValue; // 0.007 is a value to make voltage zero when there is no current
        //if (voltage < 0) voltage = 0.0;
        return voltage / FACTOR;
        //if(abs(voltage) > cutOff ){
        //    Serial.println(QOV,4);// print voltage with 3 decimal places
        //    Serial.println(FACTOR,4);// print voltage with 3 decimal places
        //    Serial.println(voltage_raw,4);
        //    Serial.print("V: ");
        //   Serial.print(voltage,3);// print voltage with 3 decimal places
        //    Serial.print("V, I: ");
        //    Serial.print(current,2); // print the current with 2 decimal places
        //    Serial.println("A");
        //}
    }

    float analogToCurrent2(uint16_t adcValue) {
        voltageValue = (3.3 / 4095.0) * adcValue;
        return ((voltageValue - acOffset)/miliVoltsPerAmp);
    }

    void loop()
    {
        _led1Current = max(analogToCurrent(MCP3208Mod::getLED1Current(), 0.0371f), 0.0f);
        _led2Current = max(analogToCurrent(MCP3208Mod::getLED2Current(), -0.01337f), 0.0f);
    }

    /*
     * getId() allows you to optionally give your V2 usermod an unique ID (please define it in const.h!).
     * This could be used in the future for the system to determine whether your usermod is installed.
     */
    uint16_t getId()
    {
        return USERMOD_ID_LIGHTSENSOR;
    }


    /*
     * addToJsonInfo() can be used to add custom entries to the /json/info part of the JSON API.
     * Creating an "u" object allows you to add custom key/value pairs to the Info section of the WLED web UI.
     * Below it is shown how this could be used for e.g. a light sensor
     */
    
    void addToJsonInfo(JsonObject& root)
    {            
      char str[10];

      JsonObject user = root["u"];
      if (user.isNull()) user = root.createNestedObject("u");

      JsonArray tempArr = user.createNestedArray("LED1 Current"); //name
      sprintf(str, "%.2f", _led1Current);
      tempArr.add(str); //value
      tempArr.add(" A"); //unit

      JsonArray tempArr2 = user.createNestedArray("LED2 Current"); //name
      sprintf(str, "%.2f", _led2Current);
      tempArr2.add(str); //value
      tempArr2.add(" A"); //unit

      JsonArray tempArr3 = user.createNestedArray("LED Current (total)"); //name
      sprintf(str, "%.2f", _led1Current + _led2Current);
      tempArr3.add(str); //value
      tempArr3.add(" A"); //unit

      JsonArray tempArr4 = user.createNestedArray("LED Wattage (total)"); //name
      sprintf(str, "%d", (int)((_led1Current + _led2Current) * 5.0f));
      tempArr4.add(str); //value
      tempArr4.add(" W"); //unit
    }    

};