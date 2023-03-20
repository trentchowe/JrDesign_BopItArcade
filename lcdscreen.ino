/* www.learningbuz.com */
/*Impport following Libraries*/
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//lcd I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

//7 segment code
int inputs[4] = {5, 6, 7, 8}; // A,B,C,D inputs
byte BCD[16][4] ={{0,0,0,0},
{1,0,0,0},
{0,1,0,0},
{1,1,0,0},
{0,0,1,0},
{1,0,1,0},
{0,1,1,0},
{1,1,1,0},
{0,0,0,1},
{1,0,0,1},
{0,1,0,1},
{1,1,0,1},
{0,0,1,1},
{1,0,1,1},
{0,1,1,1},
{1,1,1,1}}; //BCD code
int number1 = 0; //which number in BCD code do you want to send

//button code
const int buttonPin = 9;  // the number of the pushbutton pin
int buttonState = 0;  // variable for reading the pushbutton status
//bool bPress = false;
//const int buttonPin = 9;

//Variables will change
//int buttonPushCounter = 0;
//int buttonState = 0;
//int lastButtonState = 0;


void setup() 
{
//lcd code
lcd.begin(16,2);//Defining 16 columns and 2 rows of lcd display
lcd.backlight();//To Power ON the back light
//lcd.backlight();// To Power OFF the back light

//7 segemt code
for(int a = 0; a < 4; a++){
pinMode(inputs[a], OUTPUT);} //set outputs

//button code
// initialize the pushbutton pin as an input:
//pinMode(buttonPin, INPUT);
pinMode(buttonPin,INPUT_PULLUP);
//Serial.begin(9600);
//displayDigit(buttonPushCounter);
}

void loop() 
{
// read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  //lcd.clear();//Clean the screen
/*
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    //7 seg
    for(int c = 0; c < 4; c++){
    digitalWrite(inputs[c], BCD[number1][c]);
    
  }
  } else {
 
  }

*/
  number1 = number1+1;
  //lcd
  lcd.clear();//Clean the screen
  lcd.setCursor(0,0); //Defining positon to write from first row,first column .
  lcd.print(" ECE 1895 "); //You can write 16 Characters per line .
  delay(1000);//Delay used to give a dynamic effect
  lcd.setCursor(0,1);  //Defining positon to write from second row,first column .
  lcd.print("Junior Design");
  delay(2000); 


  //7 seg
  for(int c = 0; c < 4; c++){
  digitalWrite(inputs[c], BCD[number1][c]);
  }
  //lcd
  lcd.clear();//Clean the screen
  lcd.setCursor(0,0); 
  lcd.print(" Project 2 ");
  //delay(1000);//Delay used to give a dynamic effect
  lcd.setCursor(0,1);
  lcd.print(" Bop-it ");
  delay(2000); 
}

void buttonPressed() {

  
}