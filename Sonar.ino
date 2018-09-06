#define MOUNTING_HEIGHT 418.0

#define ECHO 3   
#define TRIGGER 2

volatile uint32_t path_time = 0;

void initSonar(){
  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ECHO), echo, CHANGE);
}

float level(){
  float l = path();
  if(l==0){
    return 0;
  }
  return MOUNTING_HEIGHT - l;   
}

float path(){
  float l = path_time / 58.2;
  if(l>450) return 0;
  if(l<3) return 0;
  return l;
}

void echo(){
  static uint32_t start = 0;
  uint32_t now = micros();
  if(digitalRead(ECHO)){
    start = now;
  }else{
    path_time = now - start;
    measure= false;
    update = true;
  }
}

void sonar_measure(){
    //if(measure) return;
    measure=true;
    digitalWrite(TRIGGER, 0);
    delayMicroseconds(2);
    delay(5);
    digitalWrite(TRIGGER, 1);  
}
