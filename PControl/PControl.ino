#include <Servo.h>

#define PIN_IR A0
#define PIN_LED 9
#define PIN_SERVO 10

#define Size 5

#define _DUTY_MIN 1400
#define _DUTY_NEU 1500
#define _DUTY_MAX 1600

float Kp = 0.4;
float Ki = 0.1;
float Kd = 0.1;

float TargetDistance = 255;

float error;
float errorPrevious;

double PControl,IControl,DControl;
double Time = 0.005;
double PIDControl;

int buffer1[Size];
int pidbuf[Size];
float sum;
float psum;
float dist_Alpha = 0.4;
int EMA = 0;
int EMA_R =0;
int PMA = 0;
int PMA_R = 0;
Servo myservo;

int a = 60;
int b = 300;

void setup() {
  pinMode(PIN_LED,OUTPUT);
  digitalWrite(PIN_LED,1);
  myservo.attach(PIN_SERVO);
  Serial.begin(57600);
  myservo.writeMicroseconds(1470);
  
}

float ir_distance(void){ // return value unit: mm
  float val;
  float volt = float(analogRead(PIN_IR));
  val = ((6762.0/(volt-9.0))-4.0) * 10.0;
  return val;
}

void loop(){
  float raw_dist = ir_distance();

  EMA = int(raw_dist)*dist_Alpha + (1 - dist_Alpha)*EMA;
  sum -= buffer1[0];
  for(int i=0; i<Size-1;i++)
  {
    buffer1[i] = buffer1[i+1];
  }
  buffer1[Size-1] = EMA;
  sum+=buffer1[Size-1];
  EMA_R = sum /Size+2;
  float dist_cali=300/(b-a)*(EMA_R-a)-65;

  error = TargetDistance - dist_cali;

  PControl = Kp*error;
  IControl = Ki*error*Time;
  DControl = Kd*(error-errorPrevious)/Time;

  PIDControl = PControl + IControl + DControl;
  errorPrevious = error;


  Serial.print("target:255,dist:");
  Serial.print(raw_dist);
  Serial.print(",EMA_R:");
  Serial.print(EMA_R);
  Serial.print(",PID:");
  Serial.print(PIDControl);
  Serial.print(",dist_cali:");
  Serial.println(dist_cali);
  if(raw_dist > 156 && raw_dist <224) digitalWrite(PIN_LED, 0);
  else digitalWrite(PIN_LED, 255);

  if (PIDControl>=-25 && PIDControl<25)
  {
    myservo.writeMicroseconds(1500);
  }
  else if (PIDControl < -25)
  {
    myservo.writeMicroseconds(1300);
  }
  else if (PIDControl > 10)
  {
    myservo.writeMicroseconds(1600);
  }

  delay(20);
}
