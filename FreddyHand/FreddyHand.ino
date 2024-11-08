#include <Servo.h>
#include <SoftwareSerial.h>

#define INDEX           (0)
#define MIDDLE          (1)
#define RING            (2)
#define PINKY           (3)
#define TxD             (10)
#define RxD             (11)

#define START           (0)
#define START2          (1)
#define START3          (2)
#define LOOP1           (3)
#define LOOP2           (4)
#define LOOP3           (5)
#define LOOP4           (6)
Servo servoIndex;  // create servo object to control a servo
Servo servoMiddle;
Servo servoRing;
Servo servoPinky;
// twelve servo objects can be created on most boards

SoftwareSerial mySerial(RxD, TxD); // RX, TX for Bluetooth

int pos[4] = {0, 0, 0, 0};    // variable to store the servo position
boolean terminateFlag = false;
int input = 3;
int counter = 0;
int waveState = 0;


void setup() {
  mySerial.begin(9600); // For Bluetooth
  servoIndex.attach(3);  // attaches the servo on pin 3 to the servo object
  servoMiddle.attach(5);
  servoRing.attach(6);
  servoPinky.attach(9);
}

void loop() 
{
  displaySelect();
}

void displaySelect()
{
  switch(input)
  {
    case 1:
      wave();
    break;
    case 2:
      shink();
    break;
    case 3:
      twitch();
    break;
    case 4:
    default:
      noAction();
    break;
  }
}

void wave()
{
  switch(waveState)
  {
    case START:
      pos[INDEX] ++;
      servoIndex.write(pos[INDEX]);
      if(counter>40)
      {
        waveState = START2;
        counter = 0;
      }
    break;
    case START2:
      pos[INDEX] ++;
      pos[MIDDLE] ++;
      servoIndex.write(pos[INDEX]);
      servoMiddle.write(pos[MIDDLE]);
      if(counter>40)
      {
        waveState = START3;
        counter = 0;
      }
    break;
    case START3:
      pos[INDEX] --;
      pos[MIDDLE] ++;
      pos[RING] ++;
      servoIndex.write(pos[INDEX]);
      servoMiddle.write(pos[MIDDLE]);
      servoRing.write(pos[RING]);
      if(counter>40)
      {
        waveState = LOOP1;
        counter = 0;
      }
    break;
    case LOOP1:
      pos[INDEX] --;
      pos[MIDDLE] --;
      pos[RING] ++;
      pos[PINKY] ++;
      servoIndex.write(pos[INDEX]);
      servoMiddle.write(pos[MIDDLE]);
      servoRing.write(pos[RING]);
      servoPinky.write(pos[PINKY]);
      if(counter>40)
      {
        waveState = LOOP2;
        counter = 0;
      }
    break;
    case LOOP2:
      pos[INDEX] ++;
      pos[MIDDLE] --;
      pos[RING] --;
      pos[PINKY] ++;
      servoIndex.write(pos[INDEX]);
      servoMiddle.write(pos[MIDDLE]);
      servoRing.write(pos[RING]);
      servoPinky.write(pos[PINKY]);
      if(counter>40)
      {
        waveState = LOOP3;
        counter = 0;
      }
    break;
    case LOOP3:
      pos[INDEX] ++;
      pos[MIDDLE] ++;
      pos[RING] --;
      pos[PINKY] --;
      servoIndex.write(pos[INDEX]);
      servoMiddle.write(pos[MIDDLE]);
      servoRing.write(pos[RING]);
      servoPinky.write(pos[PINKY]);
      if(counter>40)
      {
        waveState = LOOP4;
        counter = 0;
      }
    break;
    case LOOP4:
      pos[INDEX] --;
      pos[MIDDLE] ++;
      pos[RING] ++;
      pos[PINKY] --;
      servoIndex.write(pos[INDEX]);
      servoMiddle.write(pos[MIDDLE]);
      servoRing.write(pos[RING]);
      servoPinky.write(pos[PINKY]);
      if(counter>40)
      {
        waveState = LOOP1;
        counter = 0;
      }
    break;
    default:
    break;
  }

  waitTime(15);

  if(terminateFlag == true)
  {
    terminateFlag = false;
    waveState = START;
    counter = 0;
  }
  else
  {
    counter++;
  }
}
/*************************************************************
Funtion: shink
Purpose: hand slowly closes and rapidly opens
**************************************************************/
void shink()
{
  if(counter <90)
  {
    servoIndex.write(counter);
    servoMiddle.write(counter);
    servoRing.write(counter);
    servoPinky.write(counter);
  }
  else if(counter <91)
  {
    waitTime(3000);
  }
  else if(counter <92)
  {
    counter = 0;
    servoIndex.write(counter);
    servoMiddle.write(counter);
    servoRing.write(counter);
    servoPinky.write(counter);
    waitTime(3000);
  }
  else
  {
    
  }
  
  waitTime(30);
  if(terminateFlag == true)
  {
    terminateFlag = false;
    counter = 0;
  }
  else
  {
    counter++;
  }
}
/*************************************************************
Funtion: twitch
Purpose: random twitching per finger 
**************************************************************/
void twitch()
{
  int fingerTwitch = random(0,4);//0-3 range
  pos[fingerTwitch] = random(0,90);
  switch(fingerTwitch)
  {
    case INDEX:
      servoIndex.write(pos[INDEX]);
    break;
    case MIDDLE:
      servoMiddle.write(pos[MIDDLE]);
    break;
    case RING:
      servoRing.write(pos[RING]);
    break;
    case PINKY:
      servoPinky.write(pos[PINKY]);
    break;
    default:
    break;
  }
  
  waitTime(random(3000,10000));
  if(terminateFlag == true)
  {
    terminateFlag = false;
    counter = 0;
  }
  else
  {
    counter++;
  }
}
/*************************************************************
Funtion: waitTime
Purpose: time coordinating function for display functions
**************************************************************/
void waitTime (word timeDelay)
{
  unsigned long curTime=0;
  unsigned long startTime=0;
  
  startTime = millis();
  while(curTime < startTime+timeDelay)
  {
    curTime = millis ();
    if (mySerial.available()>0)
    {
      inputHandle();
      terminateFlag = true;
    }
  }
}
/*************************************************************
Funtion: inputHandle
Purpose: reads serial data from buffer
**************************************************************/
void inputHandle()
{
  goHome();
  input = mySerial.read();
  //displaySelect(); not needed as its looped
}

void goHome()
{
  counter = 0;
  pos[INDEX] = 0;
  pos[MIDDLE] = 0;
  pos[RING] = 0;
  pos[PINKY] = 0;
  servoIndex.write(0);
  servoMiddle.write(0);
  servoRing.write(0);
  servoPinky.write(0);
}
void noAction()
{

}