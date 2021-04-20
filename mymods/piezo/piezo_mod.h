#pragma once

#include "wled.h"
#include "arduino.h"
#include <deque>

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

#define PIEZO_PIN 16

static const uint16_t PIEZO_SCALE_UP[] = {NOTE_C, 50,
                                          NOTE_D, 50,
                                          NOTE_E, 50,
                                          NOTE_F, 50,
                                          NOTE_G, 50,
                                          NOTE_A, 50,
                                          NOTE_B, 50};

static const uint16_t PIEZO_DING_DONG[] = {NOTE_C, 50,
                                          NOTE_D, 50};                                          

//class name. Use something descriptive and leave the ": public Usermod" part :)
class PiezoMod : public Usermod
{
private:
    // setting PWM properties
    const int _freq = 2000;
    const int _ledChannel = 0;
    const int _resolution = 8;
    unsigned long _nextTs = 0;
    bool _isAttached = false;

    std::deque<std::pair<note_t, uint32_t>> _playData{};

public:

    /*
     * setup() is called once at boot. WiFi is not yet connected at this point.
     * You can use it to initialize variables, sensors or similar.
     */

    void noTone() {        
        ledcWriteTone(_ledChannel, 0);        
    }

    void tone(note_t note) {
        ledcWriteNote(_ledChannel, note, 5);
    }

    void scaleUp()
    {        
        for (int i = 0; i < sizeof(PIEZO_SCALE_UP) / sizeof(uint16_t); i += 2)
        {            
            _playData.push_back({(note_t)PIEZO_SCALE_UP[i], PIEZO_SCALE_UP[i + 1]});
        }
    }

    void scaleDown()
    {        
        for (int i = sizeof(PIEZO_SCALE_UP) / sizeof(uint16_t) - 1; i >= 2; i -= 2)
        {            
            _playData.push_back({(note_t)PIEZO_SCALE_UP[i - 1], PIEZO_SCALE_UP[i]});
        }
    }

    void dingDong()
    {        
        for (int i = sizeof(PIEZO_DING_DONG) / sizeof(uint16_t) - 1; i >= 2; i -= 2)
        {            
            _playData.push_back({(note_t)PIEZO_DING_DONG[i - 1], PIEZO_DING_DONG[i]});
        }
    }    

    void setup()
    {
        pinMode(PIEZO_PIN, OUTPUT);

        //Serial.println("Hello from my usermod!");
          // configure LED PWM functionalitites
        ledcSetup(_ledChannel, _freq, _resolution);

        ledcAttachPin(PIEZO_PIN, _ledChannel);        

        _isAttached = true;

        dingDong();
    }

    /*
     * connected() is called every time the WiFi is (re)connected
     * Use it to initialize network interfaces
     */
    void connected()
    {    
        scaleUp();
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
        if (millis() < _nextTs) {
            return;
        }
        
        if (!_playData.empty()) {               
            std::pair<note_t, uint32_t> current = _playData.front();
            _playData.pop_front();

            // play our tone
            tone(current.first);                

            // our stop time
            _nextTs = millis() + current.second;
        } else {                        
            noTone();
            _nextTs = 0;
        }
    }

    /*
     * getId() allows you to optionally give your V2 usermod an unique ID (please define it in const.h!).
     * This could be used in the future for the system to determine whether your usermod is installed.
     */
    uint16_t getId()
    {
        return USERMOD_ID_PIEZO;
    }
};