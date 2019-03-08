// Declare pins
int LED01           = 3;
int LED02           = 5;
int LED03           = 6;
int LED04           = 9;
int LED05           = 11;

// Time variables
unsigned long currentTime;
unsigned long waitingTime[5];
unsigned long lastRound;

// LED Variables
int brightMin [5]   =       { 10,   10,   10,   10,     10     };    // Minimum brightness
int brightAvg [5]   =       { 200,  230,  210,  210,    235   };    // Average brightness
int brightMax[5]    =       { 240,  240,  240,  240,    240     };    // Maximum brightness
int timeWaitLow[5]  =       { 1000, 1000, 1000, 1000,   1000     };    // Waiting time
int timeWaitAvg[5]  =       { 5000, 5000, 5000, 5000,   5000     };    // Waiting time
int timeWaitMax[5]  =       { 4000, 3000, 3000, 3000,   3000     };    // Waiting time
int increment[5]    =       { 2,    5,    4,    5,      2     };    // Increment in brightness
int currentBright[5]=       { 200,  230,  210,  210,    235   };    // Current brightness

// LED Variables Possibilities
int brightMinPos[5] =       { 5,   20,    40,   5,      20    };    // Minimum brightness possibilities
int brightMaxPos[5] =       { 240,  250,  230,  225,    250   };    // Maximum brightness possibilities
int timeLowPos[5]   =       { 3000, 5000, 4000, 6000,   1000 };    // Waiting time on low brightness possibilities
int timeHighPos[5]  =       { 3000, 1000, 500,  2000,   4000  };    // Waiting time on high brightness possibilities
int timeAvgPos[5]   =       { 3000, 5000, 4000, 7000,   8000 };    // Waiting time on average brightness possibilities
int incrementPos[5] =       { 2,    4,    5,    3,      1     };    // increment in brightness possibilities

// Variables
bool startup = true;

// Directional variables
/* 0 = Waiting at Average
   1 = going Avg to Low
   2 = waiting Low
   3 = going Low to Avg
   4 = going Avg to Max
   5 = waiting at Max
   6 = going High to Avg
   7 = Updating variables*/
int statusLED[5]           =       { 0, 0, 0, 0, 0  };

int refreshRate            =    50;
bool statusWaiting[5]      =       { false, false, false, false,  false};  // Is the Led waiting?

// Random variables
long randomNumber;                                                    //  Long to store random number

// counters
int i = 0;                                                            // Counter for main loop




void setup() {
  Serial.begin (115200);                                              //  Start serial
  randomSeed(analogRead(A0));                                         //  Set randomseed 

// Declare outputs
  pinMode (LED01, OUTPUT);
  pinMode (LED02, OUTPUT);
  pinMode (LED03, OUTPUT);
  pinMode (LED04, OUTPUT);
  pinMode (LED05, OUTPUT);
}



void writeToLED(){
  analogWrite(LED01, currentBright[0]);
  analogWrite(LED02, currentBright[1]);
  analogWrite(LED03, currentBright[2]);
  analogWrite(LED04, currentBright[3]);
  analogWrite(LED05, currentBright[4]);
}






void loop() {
  if (startup){
    lastRound = millis();                                                // Set lastRound for boot
    writeToLED();
    startup = false;                                                     // Turn off startup
  }
    
  currentTime = millis();                                                // Set current time


  if (currentTime - lastRound >= refreshRate ){ 
    for (i=0;i<5;i++){
      if (statusLED[i] == 7){                                                               // Updating Led
        randomNumber      = random(5);
        brightMin[i]      = brightMinPos[randomNumber];                                     // Set random value for minimum brightness
        randomNumber      = random(5);
        brightMax[i]      = brightMaxPos[randomNumber];                                     // Set random value for maximum brightness
        randomNumber      = random(5);
        timeWaitLow[i]    = timeLowPos[randomNumber];                                       // Set random value for low waiting time
        randomNumber      = random(5);
        timeWaitMax[i]    = timeHighPos[randomNumber];                                      // Set random value for high waiting time
        randomNumber      = random(5);
        timeWaitAvg[i]    = timeAvgPos[randomNumber];                                       // Set random value for average waiting time
        randomNumber      = random(5);
        increment[i]      = incrementPos[randomNumber];                                     // Set random value for increment value
        randomNumber      = random(2);
        if (randomNumber == 1){
          statusLED[i] = 1;
        }else{
          statusLED[i] = 4;
        }
        Serial.print("updated");
        Serial.print("\t");
        Serial.print(i);
        Serial.print("\t");
        Serial.print("to");
        Serial.print("\t");
        Serial.print(statusLED[i]);
        Serial.print("\n");
        
      }else if (statusLED[i] == 3){                                                     // Going Low to Avg
        if ( currentBright[i] >= brightAvg[i] ){
          statusLED[i] = 0;                                                                 // If at lowest point: Go to Waiting Avg
        }else{
          currentBright[i] = currentBright[i]+increment[i];                                 // Else: Higher Brightness
        }
        
      }else if (statusLED[i] == 2){                                                     // Waiting Low
        if ( !statusWaiting[i] ){                                                           // If not waiting:
          waitingTime[i] = millis();                                                        // Set waiting time
          statusWaiting[i] = true;                                                          // Start waiting
        }else if( statusWaiting[i] && currentTime-waitingTime[i] >= timeWaitLow[i] ){       // If waiting AND timeWaitAvg has passed:
          statusWaiting[i] = false;                                                         // Stop waiting
          statusLED[i] = 3;                                                                 // Set statusLed to Waiting for update
        }
        
      }else if (statusLED[i] == 1){                                                     // Going Avg to Low
        if ( currentBright[i] <= brightMin[i] ){
          statusLED[i] = 2;                                                                 // If at lowest point: Go to Waiting Low
        }else{
          currentBright[i] = currentBright[i]-increment[i];                                 // Else: Lower Brightness
        }
        
      }else if (statusLED[i] == 6){                                                     // Going Max to Avg
        if ( currentBright[i] <= brightAvg[i] ){
          statusLED[i] = 0;                                                                 // If at lowest point: Go to Waiting Low
        }else{
          currentBright[i] = currentBright[i]-increment[i];                                 // Else: Higher Brightness
        }
        
      }else if (statusLED[i] == 5){                                                     // Waiting Max
        if ( !statusWaiting[i] ){                                                         // If not waiting:
          waitingTime[i] = millis();                                                        // Set waiting time
          statusWaiting[i] = true;                                                          // Start waiting
        }else if( statusWaiting[i] && currentTime-waitingTime[i] >= timeWaitMax[i] ){     // If waiting AND timeWaitAvg has passed:
          statusWaiting[i] = false;                                                         // Stop waiting
          statusLED[i] = 6;                                                                 // Set statusLed to Waiting for update
        }
        
      }else if (statusLED[i] == 4){                                                       // Going Avg to Max
        if ( currentBright[i] >= brightMax[i] ){
          statusLED[i] = 5;                                                                 // If at lowest point: Go to Waiting High
        }else{
          currentBright[i] = currentBright[i]+increment[i];                                 // Else: Higher Brightness
        }
        
      }else if (statusLED[i] == 0){                                                       // Waiting at Average
        if ( !statusWaiting[i] ){                                                           // If not waiting:
          waitingTime[i] = millis();                                                        // Set waiting time
          statusWaiting[i] = true;                                                          // Start waiting
        }else if( statusWaiting[i] && currentTime-waitingTime[i] >= timeWaitAvg[i] ){     // If waiting AND timeWaitAvg has passed:
          statusWaiting[i] = false;                                                         // Stop waiting
          statusLED[i] = 7;                                                                 // Set statusLed to Waiting for update
        }
        
      }
    }
    i=0;
    lastRound = millis();
    writeToLED();
  }
}
