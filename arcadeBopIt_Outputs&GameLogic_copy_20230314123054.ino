
bool started = false;
int round = 0;
//const int coinslotPin = , ;

void setup() {
  // put your setup code here, to run once:

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

}
