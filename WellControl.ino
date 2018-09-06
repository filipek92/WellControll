#include "HardwareSerialRS485.h"
#include "MessageReader.h"
#include <Bounce2.h>
#include "Build.h"
#include "App.h"

typedef HardwareSerialRS485_1   RS485Class;
typedef MessageReader<32, RS485Class> readerClass;
RS485Class& bus = RS485Class::ourSerialObject;

#define BTN 11
#define LED 13

Bounce debouncer = Bounce(); 

volatile bool measure = false;
volatile bool update = false;

char* myAddressList[]      = {(char*)"S*", 0};

readerClass reader(myAddressList, 20);

Application<readerClass, LED> app(reader);

void setup() { 

  while(!Serial & (millis()<3000));
  Serial1.begin(115200);
  Serial1.println("WellControll");

  bus.begin(9600);

  app.printAliveMessage(bus);
  app.printAliveMessage(Serial);

  initSonar();
  initTemperature();

  pinMode(6, OUTPUT);
  digitalWrite(6,0);
  pinMode(BTN, INPUT);
  debouncer.attach(BTN);
  debouncer.interval(5);
  pinMode(LED, OUTPUT);
}

uint32_t last_measure = 0;
unsigned long buttonPressTimeStamp;
void loop() {

  debouncer.update();
  if(debouncer.fell()){
    app.startMessage();
    bus.print("Learn");
    app.endMessage();
  }

  if(millis()-last_measure > 5000){
    last_measure = millis();
    //sonar_measure();
  }

  if(millis()-last_measure > 1500){
    measure=false;
  }

  if(update){
    app.startMessage();
    Serial1.print("Path: ");
    Serial1.print(path());
    Serial1.print(" cm");
    app.endMessage();
    update=false;
  }

  digitalWrite(LED, measure);

  app.communicate();
}
