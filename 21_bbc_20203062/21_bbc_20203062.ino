#include <Servo.h>

// Arduino pin assignment
#define PIN_IR A0
#define PIN_LED 9
#define SIZE 5
#define PIN_SERVO 10

int a, b; // unit: mm
int buffer[SIZE];
float sum;
int sensorValue;
float EMA = 0.4;
int EMA_S = 0;
int EMA_s =0;
Servo myservo;

void setup() {
// initialize GPIO pins
  pinMode(PIN_LED,OUTPUT);
  digitalWrite(PIN_LED, 1);
  myservo.attach(PIN_SERVO); 
// initialize serial port
  Serial.begin(57600);

  myservo.write(90);
  a = 60;
  b = 450;
  EMA_S = int(ir_distance());
}

float ir_distance(void){ // return value unit: mm
  float val;
  float volt = float(analogRead(PIN_IR));
  val = ((6762.0/(volt-9.0))-4.0) * 10.0;
  return val;
}

void loop() {
  float raw_dist = ir_distance();
  EMA_s = EMA * int(raw_dist) + (1-EMA)* EMA_s;
  sum-=buffer[0];
  for(int i=0;i<SIZE-1;i++)
  {
    buffer[i] = buffer[i+1];
  }
  buffer[SIZE-1]=EMA_s;
  sum+=buffer[SIZE-1];
  EMA_S=sum/SIZE+2;
  float dist_cali = 100 + 300.0 / (b - a) * (EMA_S - a);
  Serial.print("min:0,max:500,dist:");
  Serial.print(raw_dist);
  Serial.print(",EMA_S:");
  Serial.print(EMA_S);
  Serial.print(",dist_cali:");
  Serial.println(dist_cali);
  if(raw_dist > 156 && raw_dist <224) digitalWrite(PIN_LED, 0);
  else digitalWrite(PIN_LED, 255);
  delay(20);
  if(dist_cali>255)
  {
    myservo.writeMicroseconds(1300);
    delay(100);
  }
  else
  {
    myservo.writeMicroseconds(1700);
    delay(100);
  }
}
