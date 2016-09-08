// This #include statement was automatically added by the Particle IDE.
#include "CE_BME280.h"

// This #include statement was automatically added by the Particle IDE.
#include "blynk/blynk.h"



// This #include statement was automatically added by the Particle IDE.
//#include "CE_BME280/CE_BME280.h"

/************************************************************************************
 * EnviroMon V1 connected to Blynk app
 * LDR for light level
 * BME280/BMP180 (temperature, humidity, pressure)
 *
 * Written: 19 May 2016
 * Modified: 17 July 2016
 * Peter Howells
 *
 * Blynk connections:
 * V11 = Light
 * V12 = Temperature
 * V13 = Humidity
 * V14 = Pressure
 * V15 = Dewpoint (calculated)
 *
 * V20 = WiFi Signal Strength
 *
 * ********************************************************************************/


SYSTEM_MODE(AUTOMATIC);



#define SEALEVELPRESSURE_HPA (1017.2)
CE_BME280 bme; // I2C


// Home - ADU Test Blynk Local-Server Auth
char auth[] = "7ab29481315f4f85ab4f9f781351f6f2";


IPAddress server_ip (192, 168, 2, 3);


float bmeTemperature;
float bmePressure;
float bmeHumidity;
float Light;
float dewpoint;
int wifiStrength;
float startTime;
float currentTime;
float sensorValue;
float Temp;


void setup()
{
    //Blynk.begin(auth, server_ip, 8442); // Local Server Version
    Blynk.begin(auth);
    delay(2000);

    if(!bme.begin()){
        String errorMessage;
        errorMessage = "Not found";
        Particle.publish("BME280 Error", errorMessage);
        //while(1);
        }

    startTime = millis();
}

void loop()
{
    Blynk.run();
    delay(200);
    currentTime = millis();
    if (currentTime > startTime + 2000){
        wifiStrength = WiFi.RSSI();
        Blynk.virtualWrite(V20, wifiStrength);
        getLight();
        getBME280Data();
        startTime = millis();
    }
}

void getLight(){
    sensorValue = analogRead(A0);
    sensorValue = 0;
    for(int x = 0; x <= 2; x++){
        sensorValue = sensorValue + analogRead(A0);
    }
    sensorValue = sensorValue / 3;
    Light = float(sensorValue) * 100 / 4095;
    Blynk.virtualWrite(V11, Light);
}

void getBME280Data(){
    bmeTemperature = bme.readTemperature() - 4;
    Blynk.virtualWrite(V12, bmeTemperature);

    bmeHumidity = bme.readHumidity();
    if (bmeHumidity > 1){
        Blynk.virtualWrite(V13, bmeHumidity);
    }

    bmePressure = (bme.readPressure()/100) - 3;
    Blynk.virtualWrite(V14, bmePressure);

    dewpoint = bmeTemperature - ((100 - bmeHumidity)/5);
    Blynk.virtualWrite(V15, dewpoint);
}
