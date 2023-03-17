#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin

int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis
// int xValue = analogRead(VRX_PIN);
// int yValue = analogRead(VRY_PIN);
void setup() {
  Serial.begin(9600) ;
}

void loop() {
  // read analog X and Y analog values
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);
  Serial.println(xValue); //about 516 is the stationary value seen
  Serial.println(yValue); //about 504 is the stationary value seen
  // print data to Serial Monitor on Arduino IDE
  // Serial.print("x = ");
  // Serial.print(xValue);
  // Serial.print(", y = ");
  // Serial.println(yValue);

  if (xValue <= 258 || yValue <= 252 || xValue >= 770 || yValue >= 779)
  {
    Serial.println("Shift");
  }
  delay(100);
}