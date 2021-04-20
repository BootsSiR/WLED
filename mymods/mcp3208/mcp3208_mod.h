#pragma once

#include "wled.h"
#include <deque>
#include "MCP_ADC.h"


#define SPI_CS 	        5// SPI slave select
#define ADC_VREF           3300// 3.3V Vref
#define ADC_CLK             4000000// SPI clock 1.6MHz

#define CLOCK_PIN 18
#define MOSI_PIN 23
#define MISO_PIN 19
#define CS_PIN 5


class MCP3208Mod : public Usermod
{
private:
    MCP3208 mcp1; 
    unsigned long _lastUpdateTs = 0;    
    static uint16_t _LED1current;
    static uint16_t _LED2current;
    static uint16_t _LED1mosfetTemp;
    static uint16_t _LED2mosfetTemp;
    static uint16_t _lightLevel;

public:
    
    static uint16_t getLED1Current() {
        return MCP3208Mod::_LED1current;
    }

    static uint16_t getLED2Current() {
        return MCP3208Mod::_LED2current;
    }

    void setup() {
        mcp1.begin(SPI_CS);
        
        Serial.println();
        Serial.println("ADC\tCHAN\tMAXVALUE");
        Serial.print("mcp1\t");
        Serial.print(mcp1.channels());
        Serial.print("\t");
        Serial.println(mcp1.maxValue());

        mcp1.setSPIspeed(4000000);                     
    };

    void loop() {
        // read all of our adc channels
        if (millis() - _lastUpdateTs > 100) {            
            MCP3208Mod::_LED1current = mcp1.analogRead(0);     
            MCP3208Mod::_LED2current = mcp1.analogRead(1);
            MCP3208Mod::_LED1mosfetTemp = mcp1.analogRead(2);
            MCP3208Mod::_LED2mosfetTemp = mcp1.analogRead(3);
            MCP3208Mod::_lightLevel = mcp1.analogRead(4);

            _lastUpdateTs = millis();
        }
    };

    void connected() {
        DEBUG_PRINTLN("XYZ");
    };  

    void addToJsonInfo(JsonObject& root) {
#ifdef WLED_DEBUG
      JsonObject user = root["u"];
      if (user.isNull()) user = root.createNestedObject("u");

      JsonArray tempArr = user.createNestedArray("ADC_LED1_current"); //name
      tempArr.add(MCP3208Mod::_LED1current); //value   

      JsonArray tempArr2 = user.createNestedArray("ADC_LED2_current"); //name
      tempArr2.add(MCP3208Mod::_LED2current); //value   

      JsonArray tempArr3 = user.createNestedArray("ADC_LED1_mosfet_temp"); //name
      tempArr3.add(MCP3208Mod::_LED1mosfetTemp); //value   

      JsonArray tempArr4 = user.createNestedArray("ADC_LED2_mosfet_temp"); //name
      tempArr4.add(MCP3208Mod::_LED1mosfetTemp); //value   

      JsonArray tempArr5 = user.createNestedArray("ADC_lightLevel"); //name
      tempArr5.add(MCP3208Mod::_lightLevel); //value   
#endif      
    };

    uint16_t getId() {
        return USERMOD_ID_MCP3208;   
    };
};

uint16_t MCP3208Mod::_LED1current;
uint16_t MCP3208Mod::_LED2current;
uint16_t MCP3208Mod::_LED1mosfetTemp;
uint16_t MCP3208Mod::_LED2mosfetTemp;
uint16_t MCP3208Mod::_lightLevel;
