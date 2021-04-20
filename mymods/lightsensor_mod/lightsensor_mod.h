#pragma once

#include "wled.h"
#include <deque>
//#include <SPI.h>
// good
//#include <Mcp320x.h>
  
#include "MCP_ADC.h"

#include "NTC_Thermistor.h"


#define PHOTORESISTOR_PIN   35
#define SPI_CS 	        5// SPI slave select
#define ADC_VREF           3300// 3.3V Vref
#define ADC_CLK             4000000// SPI clock 1.6MHz

#define CLOCK_PIN 18
#define MOSI_PIN 23
#define MISO_PIN 19
#define CS_PIN 5


#define THERMISTORNOMINAL 10000.0      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25.0   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950.0
// the value of the 'other' resistor
#define SERIESRESISTOR 10000.0    

class LightSensorMod : public Usermod
{
private:
    //good
    //MCP3208 adc;
    MCP3208 mcp1; 
    NTC_Thermistor *thermos1;
    unsigned long _lastUpdateTs = 0;
public:

    // good
    LightSensorMod() { //: therm1(&LightSensorMod::readThermistor, SERIESRESISTOR, THERMISTORNOMINAL, TEMPERATURENOMINAL, BCOEFFICIENT, 4095) {
        thermos1 = new NTC_Thermistor(&mcp1, SERIESRESISTOR, THERMISTORNOMINAL, TEMPERATURENOMINAL, BCOEFFICIENT, 4095);
    }

    uint16_t readThermistor() {
      return mcp1.analogRead(2);
    }

    /*
     * setup() is called once at boot. WiFi is not yet connected at this point.
     * You can use it to initialize variables, sensors or similar.
     */
    void setup()
    {
        pinMode(PHOTORESISTOR_PIN, INPUT); 
        

  //good
  // pinMode(SPI_CS, OUTPUT);
  // set initial PIN state
  //digitalWrite(SPI_CS, HIGH);
  //SPISettings settings(ADC_CLK, MSBFIRST, SPI_MODE0);
  //SPI.begin();
  //SPI.beginTransaction(settings);
       
       //adc.begin();

       mcp1.begin(SPI_CS);
 Serial.println();
  Serial.println("ADC\tCHAN\tMAXVALUE");
  Serial.print("mcp1\t");
  Serial.print(mcp1.channels());
  Serial.print("\t");
  Serial.println(mcp1.maxValue());

  mcp1.setSPIspeed(4000000);              
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
    
    float cutOffLimit = 1.00;
    const float FACTOR = (60.0 * (3.3/5.0)) /1000;
    const float QOV = 0.12 * 3.3;// set quiescent Output voltage for selected model
    float cutOff = FACTOR/cutOffLimit;// convert current cut off to mV


    uint16_t offset = 450;
    float voltagePerMv = 3.3 / 4096;


    void loop()
    {
        if (millis() - _lastUpdateTs > 2000) {
            uint16_t photoResValue = analogRead(PHOTORESISTOR_PIN);
            byte lightLevel = map(photoResValue, 0, 4095, 0, 100);

            DEBUG_PRINT("Ambient Light Reading: ");
            DEBUG_PRINT(photoResValue);
            DEBUG_PRINT(" ");
            DEBUG_PRINTLN(lightLevel);

            ambientLightLevel = lightLevel;

            _lastUpdateTs = millis();

Serial.print(millis());
  Serial.print("\tmcp1:\t");
  for (int channel = 0 ; channel < 2; channel++)
  {
    uint16_t val = mcp1.analogRead(channel);

    

  //Robojax.com ACS758 Current Sensor 
  float voltage_raw =   (3.3 / 4095.0)* val;// Read the voltage from sensor
  float voltage =  voltage_raw - QOV + 0.033;// 0.007 is a value to make voltage zero when there is no current
  //if (voltage < 0) voltage = 0.0;
  float current = voltage / FACTOR;
  //if(abs(voltage) > cutOff ){
    Serial.println(QOV,4);// print voltage with 3 decimal places
    Serial.println(FACTOR,4);// print voltage with 3 decimal places
    Serial.println(voltage_raw,4);
    Serial.print("V: ");
    Serial.print(voltage,3);// print voltage with 3 decimal places
    Serial.print("V, I: ");
    Serial.print(current,2); // print the current with 2 decimal places
    Serial.println("A");
  //}

    uint16_t therm1 = mcp1.analogRead(2);
    Serial.print("THERM: ");
    Serial.println(therm1);

  float averageTherm = therm1;
  float samplesTherm = 5;
     float steinhart;

  averageTherm = 4095.0 / ((float)therm1 - 1); //  4095 / averageTherm;
  steinhart = averageTherm / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert absolute temp to C
  
  Serial.print("Temperature "); 
  Serial.print(thermos1->readCelsius());
  Serial.println(" *C");
/*
    Serial.print(val);
    if (val > offset) val = val - offset;
    Serial.print(" corr: ");
    Serial.print(val);
    Serial.print("(");
    uint32_t mv = (static_cast<uint32_t>(val) * 3300) / (4096 - 1);
    Serial.print(mv);    
    Serial.print("mV)");

    float amps = mv / 60.0;
    Serial.print(" (");
    Serial.print(amps);
    Serial.print(" A)\t");
        // added so single reads are better visible on a scope
  }
  Serial.println();*/
            
            // good
            /*
                    uint32_t t1;
                    uint32_t t2;

                    // start sampling
                    Serial.println("Reading...");

                    t1 = micros();
                    uint16_t raw = adc.read(MCP3208::Channel::SINGLE_1);
                    t2 = micros();

                    // get analog value
                    uint16_t val = adc.toAnalog(raw);

                    // readed value
                    Serial.print("value: ");
                    Serial.print(raw);
                    Serial.print(" (");
                    Serial.print(val);
                    Serial.println(" mV)");

                    // sampling time
                    Serial.print("Sampling time: ");
                    Serial.print(static_cast<double>(t2 - t1) / 1000, 4);
                    Serial.println("ms");        

                    t1 = micros();
                    raw = adc.read(MCP3208::Channel::SINGLE_0);
                    t2 = micros();

                    // get analog value
                    val = adc.toAnalog(raw);

                    // readed value
                    Serial.print("value: ");
                    Serial.print(raw);
                    Serial.print(" (");
                    Serial.print(val);
                    Serial.println(" mV)");

                    // sampling time
                    Serial.print("Sampling time: ");
                    Serial.print(static_cast<double>(t2 - t1) / 1000, 4);
                    Serial.println("ms");                              
*/
            /* working

char temp[10];
for (int i = 0; i < 8; i++) {
		sprintf(temp, "%5d", adc.analogRead(i));
		Serial.print(temp);
	}
	Serial.print(" |");
	for (int i = 0; i < 4; i++) {
		sprintf(temp, "%5d", adc.analogReadDif(i));
		Serial.print(temp);
	}
	Serial.println();*/
        }}
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
      JsonObject user = root["u"];
      if (user.isNull()) user = root.createNestedObject("u");

      JsonArray tempArr = user.createNestedArray("Ambient Light"); //name
      tempArr.add(ambientLightLevel); //value
      tempArr.add(" %"); //unit
    }    

};