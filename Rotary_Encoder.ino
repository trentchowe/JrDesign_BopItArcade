// Rotary Encoder Inputs
#define CLK 2
#define DT 3
#define SW 4

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

void setup() {
        
	// Set encoder pins as inputs
	pinMode(CLK,INPUT_PULLUP);
	pinMode(DT,INPUT_PULLUP);
	//pinMode(SW, INPUT_PULLUP);

	// Setup Serial Monitor
	Serial.begin(9600);

	// Read the initial state of CLK
	lastStateCLK = !digitalRead(CLK);
	// lastStateCLK = 0;
  
}

void loop() {
  //Serial.print(lastStateCLK);
  // Serial.println(lastStateCLK); 
	// Read the current state of CLK
	currentStateCLK = !digitalRead(CLK);
  // Serial.println(!digitalRead(CLK));
  // Serial.println(!digitalRead(DT));

	// If last and current state of CLK are different, then pulse occurred
	// React to only 1 state change to avoid double count
	// if (currentStateCLK != lastStateCLK){
  //Serial.print("CLK = ");
  //if the state of Output A has changed (currentStateCLK != lastStateCLK) and output A is currently a 1 (currentStateCLK == 1)
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){  

		// If the DT state is different than the CLK state and CLK state is 1 (high), then
		// the encoder is rotating CW so increment
    Serial.println("inside first if");
    //if output B is different than output A (meaning output B is a 0 since Output A we already confirmed is a 1) then 
    //this means that we are rotating clockwise because Output A will enter the metal pad before output B (Output A will be a 1
    //when output B is a 0 since we are rotating clockwise)
		if (!digitalRead(DT) != currentStateCLK) {
      Serial.println("initial if");
      Serial.println(!digitalRead(CLK));
      Serial.println(!digitalRead(DT));
      //increment counter because we are rotating clockwise
			counter ++;
			currentDir ="CW";

    } else if (!digitalRead(DT) == currentStateCLK)
    //this means that we are rotating counterclockwise because when Output B is equal to Output A, and we already confirmed that output A is a 1, this means 
    //that we have rotated counterclockwise, and Output B has already gotten onto the conductive pad (became a 1) before output A
     {
      Serial.println("else if");
      Serial.println(!digitalRead(CLK));
      Serial.println(!digitalRead(DT));
			// Encoder is rotating CCW so decrement
			counter --;
			currentDir ="CCW";
		}
    // Remember last CLK state
    // lastStateCLK = currentStateCLK;

		Serial.print("Direction: ");
		Serial.print(currentDir);
		Serial.print(" | Counter: ");
		Serial.println(counter);
    //delay(100);
    Serial.println(!digitalRead(CLK));
    Serial.println(!digitalRead(DT));
	}

	// // Remember last CLK state
	lastStateCLK = currentStateCLK;

	// Read the button state
	int btnState = digitalRead(SW);
  //Serial.println(btnState);

	//If we detect LOW signal, button is pressed
	if (btnState == HIGH) {
    	Serial.println("Button pressed!");
		//if 50ms have passed since last LOW pulse, it means that the
		//button has been pressed, released and pressed again
		// if (millis() - lastButtonPress > 50) {
		// 	Serial.println("Button pressed!");
		// }
    delay(300);

		// Remember last button press event
		lastButtonPress = millis();
	}

	// Put in a slight delay to help debounce the reading
	delay(1); //delay by 1 ms
}