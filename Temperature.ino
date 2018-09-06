#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_PIN 4

OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress[] = {
    {0x28,0x57,0xB1,0x21,0x05,0x00,0x00,0xFD}
  };

void initTemperature(){
  sensors.begin();
  int numberOfDevices = sensors.getDeviceCount();
  Serial.println(numberOfDevices);
  Serial.println("device(s) on bus");
}
