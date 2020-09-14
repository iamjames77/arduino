#define PIN_LED 7
unsigned int count, toggle;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200);
  while (!Serial) {
    ; 
  }
  for (int i =0;i<11;i++)
  {
    if(i==0)
    {
      digitalWrite(PIN_LED, i);
      delay(1000);
    }
    else
    {
      digitalWrite(PIN_LED, i%2);
      delay(100);
    }
  }
}

void loop() {
  while (1)
  {
    digitalWrite(PIN_LED, 1);
  }

}

int toggle_state(int toggle) {
  return toggle;
}
