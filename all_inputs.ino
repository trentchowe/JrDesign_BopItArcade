#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

//lcd I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//MP3 setup
// Use pins 2 and 3 to communicate with DFPlayer Mini
//static const uint8_t PIN_MP3_TX = 1; // Connects to module's RX
//static const uint8_t PIN_MP3_RX = 0; // Connects to module's TX
//SoftwareSerial softwareSerial(0, 1);
SoftwareSerial mySerial(3, 2); //tx THEN RX

// const int wirePin = 4;
// const int endPin = 5;

// // Create the Player object
DFRobotDFPlayerMini player;
#define Start_Byte 0x7E
#define Version_Byte 0xFF
#define Command_Length 0x06
#define End_Byte 0xEF
#define Acknowledge 0x00
#define ACTIVATED LOW
 
//TODO
#define coinslotPin 9 //9 for pro mini d9 or 15 for atmega
//defining the pin for starting a new game to digital pin 9 (pin 15 of ATMega328P), this cannot
//be the reset pin or else the initial start of the game (when the device is first turned on) cannot occur

//inputs for the rotary encoder (turn right, turn left, and honk it actions)
#define CLK 7 //
#define DT 8
#define SW 6 //4 for pro micro 6 for atmega
#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin
int lastStateCLK; //value (0 or 1) of previous state of Output A of rotary encoder
//making this a global variable (outside of setup()) because it needs to be accessed by multiple functions
int currentStateCLK;
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
  pinMode(9, INPUT_PULLUP);


  mySerial.begin(9600);
  while (!player.begin(mySerial)) {  //Use softwareSerial to communicate with mp3.

  }
  //setup LCD screen
  lcd.begin(16,2);//Defining 16 columns and 2 rows of lcd display
  lcd.backlight();//To Power ON the back light
  lcd.clear();//Clean the screen
  lcd.setCursor(0,0); //Defining positon to write from, first row,first column .
  lcd.print(" Insert Coin ");

  //MP3 player
  // Init USB serial port for debugging
  Serial.begin(115200);
 
  player.volume(30);
  // Init serial port for DFPlayer Mini
  //softwareSerial.begin(9600);
  lastStateCLK = digitalRead(CLK);

  // // Start communication with DFPlayer Mini
  // if (player.begin(softwareSerial)) {
	// Serial.println("connecting to mp3");

	// // Set volume to maximum (0 to 30).
	// player.volume(30);
	// // Play the first MP3 file on the SD card
	// player.play(1);
  // } else {
  // 	Serial.println("Connecting to DFPlayer Mini failed!");
  // }
 // while (!mySerial.begin()
  //mySerial.begin (9600);
  //delay(1000);
  //playFirst();

}

void loop() {
  //Serial.println("Loop");
  // put your main code here, to run repeatedly:
  
    if(checkCoinslot()){
      started = true;
      Serial.println("Game Start");
      //play start noise
    }
    if(started)
    {
      //lcd.clear();//Clean the screen
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
      }

      if(completed)//check if completed is true
      {
        //can add playVictorySound() if we have time, not required
        Serial.println("You succeeded: Calling display round num count function.");
        roundNum++;
        if (roundNum == 100){
          lcd.clear();//Clean the screen
          lcd.setCursor(0,0); //Defining positon to write from, first row,first column .
          lcd.print(" You Won! ");
          //play win noise
        }
        displayroundNumCount();
      }
      else{
        lcd.clear();//Clean the screen
        lcd.setCursor(0,0); //Defining positon to write from, first row,first column .
        lcd.print(" Good Try! ");
        player.play(5);
        //play lose noise
        Serial.println("You failed: Calling display round num count function.");
        delay(1000);
        displayroundNumCount();
        roundNum = 0;
        started = false;
        lcd.setCursor(0,0); //Defining positon to write from, first row,first column .
        lcd.print("Coin to Restart");
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
//"Turn It" steering wheel turn action
{
    Serial.println("Turn It!");
    lcd.clear();//Clean the screen
    lcd.setCursor(0,0); //Defining positon to write from, first row,first column .
    lcd.print(" Turn It! ");
    //play sound ("Turn Right" maybe?)
    //playSpecific(4);
    player.play(1);
    
    //then check for input
    double startTime = millis(); //number of ms since Arduino program began running
    double endTime = startTime;
    int Turn;
    lastStateCLK = digitalRead(CLK);
    while((endTime - startTime) <= (time*1000)) //time is in milliseconds so multiply seconds (time) by 1000
    {
       Turn = TurnIt();
       Serial.print("turn outside loop");
       Serial.println(Turn);
       if(Turn == 1 || Turn == 2)
       {
         Serial.println("Correct!");
         Serial.println(Turn);
         return true;
       }
       endTime = millis();
    }
    Serial.println("Out of Time!");
    lcd.clear();//Clean the screen
    lcd.setCursor(0,0); //Defining positon to write from, first row,first column .
    lcd.print(" Wrong move! ");
    return false;
}

bool checkAction2(int time)
//honk horn
{
    Serial.println("Honk Horn!");
    lcd.clear();//Clean the screen
    lcd.setCursor(0,0); //Defining positon to write from, first row,first column .
    lcd.print(" Honk It! ");
    //play sound
    //playSpecific(2);
    player.play(2);
    
    //then check for input
    double startTime = millis();
    //double endTime = startTime;
    //double currentStateCLK = digitalRead(CLK);
    //lastStateCLK = currentStateCLK;
    int btnState = digitalRead(SW);

    while((millis() - startTime) <= (time*1000)){ //time is in milliseconds
      btnState = digitalRead(SW);
      if(btnState == HIGH){
        Serial.println("Correct!");
        //playSpecific(1);
        player.play(4);
        return true;
      }
      //endTime = millis();
    }
    Serial.println("Out of Time!");
    lcd.clear();//Clean the screen
    lcd.setCursor(0,0); //Defining positon to write from, first row,first column .
    lcd.print(" Wrong move! ");
    return false;

}

bool checkAction3(int time)
//move joystick
{
  Serial.println("Move Joystick!");
  lcd.clear();//Clean the screen
  lcd.setCursor(0,0); //Defining positon to write from, first row,first column .
  lcd.print("Shift It!");
  //play sound
  //playSpecific(3);
  player.play(3);
  //  delay(500);
  //then check for input
  double startTime = millis();
  //double endTime = startTime;
  int moved = JoystickMove();
  while((millis() - startTime) <= (time*1000)){ //time is in milliseconds
    moved = JoystickMove();
    if(moved == 1)
    {
      Serial.println("Correct!");
      return true;
    }
    //endTime = millis();
  }
  //return false because the time ran out
  Serial.println("Out of Time!");
  lcd.clear();//Clean the screen
  lcd.setCursor(0,0); //Defining positon to write from, first row,first column .
  lcd.print(" Wrong move! ");
  return false;

}

void displayroundNumCount() {
  Serial.println("Display round num count");
  lcd.clear();//Clean the screen
  lcd.setCursor(0,0); //Defining positon to write from, first row,first column .
  lcd.print("round: ");
  delay(500);
  lcd.print(roundNum);
  delay(1000);
  lcd.clear();  //clear again after use
}

int TurnIt()
//function that checks if the steering wheel is turned left or right
//returns 1 if turned right, 2 if turned left, and 3 if no turn has been received
{
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
      //Serial.println("1");
      //return 1 becasue rotary encoder has turned right (CW)
			return 1;
    }
    //this means that we are rotating counterclockwise because when Output B is equal to Output A, and we already confirmed that output A is a 0, this means
    //that we have rotated counterclockwise, and Output B has already gotten onto the conductive pad (became a 0) before output A, meaning Output B was pulled to 0 before output A
    else if (digitalRead(DT) == currentStateCLK)
    // return 2 becasue rotary encoder has turned left (CCW)
    //Serial.println("2");
	  	return 2;
  }
  //return 3 because no turn was detected
  //Serial.println("3");
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

// void playSpecific(int num)
// {
//   execute_CMD(0x3F, 0, 0);
//   delay(1);
//   setVolume(20);
//   delay(500);
//   execute_CMD(0x03,0, num); 
//   delay(500);
// }

// void setVolume(int volume)
// {
//   execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
//   delay(1);
// }

// void execute_CMD(byte CMD, byte Par1, byte Par2)
// // Excecute the command and parameters
// {
// // Calculate the checksum (2 bytes)
// word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
// // Build the command line
// byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
// Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
// //Send the command line to the module
// for (byte k=0; k<10; k++)
// {
// mySerial.write( Command_line[k]);
// }
// }