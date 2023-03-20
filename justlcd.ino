#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

void setup() {
  //lcd code
  lcd.begin(16,2);//Defining 16 columns and 2 rows of lcd display
  lcd.backlight();//To Power ON the back light

}

void loop() {
  lcd.clear();//Clean the screen
  //lcd
  lcd.clear();//Clean the screen
  lcd.setCursor(0,0); //Defining positon to write from first row,first column .
  lcd.print(" ECE 1895 "); //You can write 16 Characters per line .
  delay(1000);//Delay used to give a dynamic effect
  lcd.setCursor(0,1);  //Defining positon to write from second row,first column .
  lcd.print("Junior Design");
  delay(2000); 

  //lcd
  lcd.clear();//Clean the screen
  lcd.setCursor(0,0); 
  lcd.print(" Project 2 ");
  //delay(1000);//Delay used to give a dynamic effect
  lcd.setCursor(0,1);
  lcd.print(" Bop-it ");
  delay(2000); 

}
