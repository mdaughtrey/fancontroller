#include <Wire.h>
#include "Adafruit_APDS9960.h"

#define I2C_SCL 22
#define I2C_SDA 21
#define PIN_FAN 10
#define PIN_PIR 9

Adafruit_APDS9960 apds;
// 22 = SCL
// 21 = SDA
bool poweredOn = false;

void fanOn()
{
    // Turn fans on
    digitalWrite(PIN_FAN, HIGH);
}

void fanOff()
{
    // Turn fans off
    digitalWrite(PIN_FAN, LOW);
}

void handleGesture()
{
    switch (apds.readGesture()) {
        case APDS9960_UP:
            poweredOn = true;
            fanOn();
//            Serial.println("UP");
            break;

        case APDS9960_DOWN:
            poweredOn = false;
            fanOff(); 
//            Serial.println("DOWN");
            break;

        case APDS9960_LEFT:
//            Serial.println("LEFT");
            poweredOn = false;
            fanOff(); 
            break;

        case APDS9960_RIGHT:
//            Serial.println("RIGHT");
            poweredOn = true;
            fanOn();
            break;

        default:
            break;
    }
}

void initGestureSensor(void)
{
    if (!apds.begin())
    {
        while(1)
        {
           Serial.println("APDS9960 init failed");
            delay(5000);
        }
    }
    apds.enableProximity(true);
    apds.enableGesture(true);
}

#ifdef I2CSCAN
void i2cscan()
{
    byte error, address;
    int nDevices;
//    serout << "Scanning..." << endl;
    nDevices = 0;
    for(address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0) {
//            serout << "I2C device found at address  " << address << endl;
            nDevices++;
        }
        else if (error==4) {
//            serout  << "Unknown error at address " << address << endl;
        }    
    }
    if (nDevices == 0) {
//        serout << "No I2C devices found" << endl;
    }
    else {
//        serout << "Done" << endl;
    }
}

#endif // I2CSCAN

void setup(void)
{
    Serial.begin(115200);
#ifdef I2CSCAN
//    serout << "SDA" << I2C_SDA << endl;
//    serout << "SCL" << I2C_SCL << endl;
//    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.begin();
    while (true) {
        i2cscan();
        delay(5000);
}
#endif // I2CSCAN
    initGestureSensor();
    fanOff();
    pinMode(PIN_FAN, OUTPUT);
}

void loop(void)
{
    handleGesture();
    if (!poweredOn)
    {
        return;
    }
    if (digitalRead(PIN_PIR))
    {
        fanOn();
    }
    else
    {
        fanOff();
    }
    delay(100);
}
