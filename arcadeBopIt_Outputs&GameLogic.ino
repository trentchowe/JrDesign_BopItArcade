#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//lcd I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

//do we need 7-segment code still?
//TODO



bool started = false;
int round = 0;
//const int coinslotPin = , ;

void setup() {
  // put your setup code here, to run once:

  //setup LCD screen
  lcd.begin(16,2);//Defining 16 columns and 2 rows of lcd display
  lcd.backlight();//To Power ON the back light

  //setup 7segment code if we use it
  //TODO

}

void loop() {
  // put your main code here, to run repeatedly:
    if(checkCoinslot()){
      started = true;
    }

    if(started)
    {
      int action = random(1, 3);

      switch(action):
        case(1):
          bool completed = checkAction1(5 - (.05 * round));
        case(2):
          bool completed = checkAction2(5 - (.05 * round));
        case(3):
          bool completed = checkAction3(5 - (.05 * round));
      
      if(completed){
        //can add playVictorySound() if we have time, not required
        displayRoundCount();
        round++;
      }
      else{
        displayRoundCount();
        round = 0;
        started = false;
      }
    }
}


bool checkCoinslot() {
    coinslotState = digitalRead(coinslotPin);
    if(coinslotState == HIGH)
    {
      return true;
    }
    else{
      return false;
    }
}

bool checkAction1(int time) {
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

bool checkAction2(int time) {
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

bool checkAction3(int time) {
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

void displayRoundCount() {
  lcd.clear();//Clean the screen
  lcd.setCursor(0,0); //Defining positon to write from first row,first column .
  lcd.print("Round: ");
  delay(1000);
  lcd.print(round);
  delay(2000);
  lcd.clear();  //clear again after use
}
