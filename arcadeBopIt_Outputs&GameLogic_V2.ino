#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

//lcd I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

// //MP3 setup
// // Use pins 2 and 3 to communicate with DFPlayer Mini
// static const uint8_t PIN_MP3_TX = 2; // Connects to module's RX 
// static const uint8_t PIN_MP3_RX = 3; // Connects to module's TX 
// SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

// const int wirePin = 4; 
// const int endPin = 5; 

// // Create the Player object
// DFRobotDFPlayerMini player;

//TODO
#define coinslotPin 9 //defining the pin for starting a new game to digital pin 9 (pin 15 of ATMega328P), this cannot
//be the reset pin or else the initial start of the game (when the device is first turned on) cannot occur

//inputs for the rotary encoder (turn right, turn left, and honk it actions)
#define CLK 7
#define DT 8
#define SW 4
#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin
int lastStateCLK; //value (0 or 1) of previous state of Output A of rotary encoder
//making this a global variable (outside of setup()) because it needs to be accessed by multiple functions
int roundNum = 0;
int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis
bool started = false;
//const int coinslotPin = , ; //should not be needed since this is set to a pin in #define
//since this is not a variable that needs to have values between -32768 and 32767 and instead just needs to be high or low

void setup() {
  // put your setup code here, to run once:
	
  //pulling CLK and DT high for default state of rotary encoder because the metal pads of the rotary encoder pull its output signal low (0)
  //when turning the rotary encoder
  pinMode(CLK,INPUT_PULLUP);
  pinMode(DT,INPUT_PULLUP);
  //read in the initial state of CLK (Output A) which should be 1 if it's not being turned due to its internal pullup resistor 
  double lastStateCLK = digitalRead(CLK);
  pinMode(9, INPUT_PULLUP);
  //setup LCD screen
  lcd.begin(16,2);//Defining 16 columns and 2 rows of lcd display
  lcd.backlight();//To Power ON the back light
  lcd.clear();//Clean the screen
  lcd.setCursor(0,0); //Defining positon to write from, first row,first column .
  lcd.print("Press to Start");
	
  //MP3 player
  // Init USB serial port for debugging
  Serial.begin(9600);
  // // Init serial port for DFPlayer Mini
  //softwareSerial.begin(9600);

  // // Start communication with DFPlayer Mini
  // if (player.begin(softwareSerial)) {
	// Serial.println("OK");

	// // Set volume to maximum (0 to 30).
	// player.volume(30);
	// // Play the first MP3 file on the SD card
	// player.play(1);
  // } else {
  // 	Serial.println("Connecting to DFPlayer Mini failed!");
  // }

}

void loop() {
  //Serial.println("Loop");
  // put your main code here, to run repeatedly:
    if(checkCoinslot()){
      started = true;
      Serial.println("Game Start");
    }
    if(started)
    {
      //4 actions (turn right, turn left, honk it, shift it)
      int action = random(1, 4);
      bool completed = 0; //checking if a proper action is done, initially this is 0 (false) 

      switch (action)
      {
        //0.05s * roundNum was changed to 0.045s * roundNum so that the fastest time (if someone gets to 99 successful attempts)
        //is 0.545s whereas with 0.05s * roundNum, at the 99th round, that is 0.05 s (very fast)
        case(1):
          completed = checkAction1(5 - (.045 * roundNum));
          break;
        case(2):
          completed = checkAction2(5 - (.045 * roundNum));
          break;
        case(3):
          completed = checkAction3(5 - (.045 * roundNum));
          break;
        case(4):
          completed = checkAction4(5 - (.045 * roundNum));
          break;
      }
      
      if(completed)//check if completed is true
      {
        //can add playVictorySound() if we have time, not required
        displayroundNumCount();
        roundNum++;
      }
      else{
        displayroundNumCount();
        roundNum = 0;
        started = false;
      }
    }
}


bool checkCoinslot()
//checks if the SPST switch has been pushed meaning a new game should begin 
{
    int coinslotState = digitalRead(coinslotPin);
    if(coinslotState == LOW)
    {
      return true;
    }
    else
    {
      return false;
    }
}

bool checkAction1(int time) 
//"Turn Right" steering wheel turn action
{
    Serial.println("Turn Right!");
    //play sound ("Turn Right" maybe?)
    //then check for input
    double startTime = millis(); //number of ms since Arduino program began running
    double endTime = startTime;
    int Turn = TurnIt();
    while((endTime - startTime) <= (time*1000)) //time is in milliseconds so multiply seconds (time) by 1000
    {

      // if(Turn == 1)
      // {
      //   Serial.println("Correct!");
      //   return true;
      // }
      // endTime = millis();
    }
    Serial.println("Out of Time!");
    return false;
}

bool checkAction2(int time)
//turn left
{
    //play sound ("Turn Left" maybe?)
    //then check for input
    Serial.println("Turn Left!");
    double startTime = millis(); //number of ms since Arduino program began running
    double endTime = startTime;
    //variable to keep track if rotary encoder was turned
    int Turn;
    while((endTime - startTime) <= (time*1000)) //time is in milliseconds so multiply seconds (time) by 1000
    {
      //getting the value from TurnIt function to see if it is turned left (2)
      Turn = TurnIt();
      Serial.println(Turn);
      if(Turn == 2)
      {
        Serial.println("Correct!");
        return true;
      }
      endTime = millis();
    }
    Serial.println("Out of Time!");
    return false;
}

bool checkAction3(int time)
//honk horn
{
    Serial.println("Honk Horn!");
    //play sound

    //then check for input
    double startTime = millis();
    double endTime = startTime;

    double currentStateCLK = digitalRead(CLK);
    lastStateCLK = currentStateCLK;
    int btnState = digitalRead(SW);

    while((endTime - startTime) <= (time*1000)){ //time is in milliseconds
      if(btnState == HIGH){
        Serial.println("Correct!");
        return true;
      }
      endTime = millis();
    }
    Serial.println("Out of Time!");
    return false;
    
}

bool checkAction4(int time)
//move joystick
{
  Serial.println("Move Joystick!");
    //play sound

    //then check for input
    double startTime = millis();
    double endTime = startTime;
    int moved = JoystickMove();
    while((endTime - startTime) <= (time*1000)){ //time is in milliseconds
      if(moved == 1)
      {
        Serial.println("Correct!");
        return true;
      }
      endTime = millis();
    }
    //return false because the time ran out
    Serial.println("Out of Time!");
    return false;
    
}
void displayroundNumCount() {
  lcd.clear();//Clean the screen
  lcd.setCursor(0,0); //Defining positon to write from, first row,first column .
  lcd.print("round: ");
  delay(1000);
  lcd.print(roundNum);
  delay(2000);
  lcd.clear();  //clear again after use	
}

int TurnIt()
//function that checks if the steering wheel is turned left or right
//returns 1 if turned right, 2 if turned left, and 3 if no turn has been received
{
  int currentStateCLK;
  // Read the current state of CLK (output A)
	currentStateCLK = digitalRead(CLK);
  //if the state of Output A has changed (currentStateCLK != lastStateCLK) and output A is currently a 0 (currentStateCLK == 0) (so we are only checking when Output A is on a metal pad (0))
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 0)
  {
    // If the DT state is different than the CLK state and CLK state is 0 (low), and since DT != CLK at this time, 
    //that means we are CW because output A recevied a signal first meaning it touched a metal pad (pulling its value to ground, 0) first in the rotary encoder
    //and output A is closest to the metal pad in the clockwise direction (***And the outputs A and B are stationary, and we are rotating the metal pads of the rotary encoderthat can come into contact with outputs A and B)
		if (digitalRead(DT) != currentStateCLK) 
    {
      Serial.println("1");      
      //return 1 becasue rotary encoder has turned right (CW) 
			return 1;
    }
    //this means that we are rotating counterclockwise because when Output B is equal to Output A, and we already confirmed that output A is a 0, this means 
    //that we have rotated counterclockwise, and Output B has already gotten onto the conductive pad (became a 0) before output A, meaning Output B was pulled to 0 before output A
    else if (digitalRead(DT) == currentStateCLK)
    // return 2 becasue rotary encoder has turned left (CCW)
    Serial.println("2");
	  	return 2;
  }
  //return 3 because no turn was detected
  Serial.println("3");
  return 3;
}

bool Honk()
{
  int btnState = digitalRead(SW);
  if (btnState == HIGH) 
  {
  Serial.println("Button pressed!");
  //return 1 because button pressed
  return 1;
  }
  //delay for debouncing
  delay(300);
  //return 0 because no button press detected
  return 0;
}

bool JoystickMove()
{
  // read analog X and Y analog values
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);
  if (xValue <= 258 || yValue <= 252 || xValue >= 770 || yValue >= 779)
  {
    //return true because the joystick was moved properly  
    return true;
  }
//return false because the joystick was not moved enough
return false;
	//debouncing delay
  delay(100);	
}