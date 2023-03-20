#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//lcd I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

//TODO
#define coinslotPin 9 //defining the pin for starting a new game to digital pin 9 (pin 15 of ATMega328P), this cannot
//be the reset pin or else the initial start of the game (when the device is first turned on) cannot occur

//inputs for the rotary encoder (turn right, turn left, and honk it actions)
#define CLK 2
#define DT 3
#define SW 4
//value (0 or 1) of previous state of Output A of rotary encoder
//making this a global variable (outside of setup()) because it needs to be accessed by multiple functions
int lastStateCLK;
bool started = false;
int roundNum = 0;
//const int coinslotPin = , ; //should not be needed since this is set to a pin in #define
//since this is not a variable that needs to have values between -32768 and 32767 and instead just needs to be high or low

void setup() {
  // put your setup code here, to run once:
  //pulling CLK and DT high for default state of rotary encoder because the metal pads of the rotary encoder pull its output signal low (0)
  //when turning the rotary encoder
  pinMode(CLK,INPUT_PULLUP);
	pinMode(DT,INPUT_PULLUP);
  //read in the initial state of CLK (Output A) which should be 1 if it's not being turned due to its internal pullup resistor 
  lastStateCLK = digitalRead(CLK);
  //setup LCD screen
  lcd.begin(16,2);//Defining 16 columns and 2 rows of lcd display
  lcd.backlight();//To Power ON the back light

}

void loop() {
  // put your main code here, to run repeatedly:
    if(checkCoinslot()){
      started = true;
    }

    if(started)
    {
      //4 actions (turn right, turn left, honk it, shift it)
      int action = random(1, 4);
      bool completed = 0; //checking if a proper action is done 

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
      
      if(completed){
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
    //play sound ("Turn Right" maybe?)
    //then check for input
    double startTime = millis(); //number of ms since Arduino program began running
    double endTime = startTime;
    int Turn = TurnIt();
    while((endTime - startTime) <= (time*1000)) //time is in milliseconds so multiply seconds (time) by 1000
    {
      if(Turn == 1)
      {
        Serial.println("")
        return true;
      }
      endtime = millis();
    }
    return false;
}

bool checkAction2(int time)
//turn left
{
    //play sound

    //then check for input
    startTime = millis();
    endTime = startTime;
    while((endTime - startTime) <= (time*1000)){ //time is in milliseconds
      if(/*pinLogic*/){
        return true;
      }
      endtime = millis();
    }
    return false;
}

bool checkAction3(int time)
//honk horn
{
    //play sound

    //then check for input
    startTime = millis();
    endTime = startTime;
    while((endTime - startTime) <= (time*1000)){ //time is in milliseconds
      if(/*pinLogic*/){
        return true;
      }
      endtime = millis();
    }
    return false;
    
}

bool checkAction4(int time)
//move joystick
{
    //play sound

    //then check for input
    startTime = millis();
    endTime = startTime;
    while((endTime - startTime) <= (time*1000)){ //time is in milliseconds
      if(/*pinLogic*/){
        return true;
      }
      endtime = millis();
    }
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
      //return 1 becasue rotary encoder has turned right (CW) 
			return 1;
    }
    //this means that we are rotating counterclockwise because when Output B is equal to Output A, and we already confirmed that output A is a 0, this means 
    //that we have rotated counterclockwise, and Output B has already gotten onto the conductive pad (became a 0) before output A, meaning Output B was pulled to 0 before output A
    else if (digitalRead(DT) == currentStateCLK)
    // return 2 becasue rotary encoder has turned left (CCW)
	  	return 2;
  }
  //return 3 because no turn was detected
  return 3;

}