/** This example uses the Zumo's line sensors to detect the white
border around a sumo ring.  When the border is detected, it
backs up and turns. */

#include <Wire.h>
#include <Zumo32U4.h>

// This might need to be tuned for different lighting conditions,
// surfaces, etc.
#define QTR_THRESHOLD     1000  // microseconds

// These might need to be tuned for different motor types.
#define REVERSE_SPEED     200  // 0 is stopped, 400 is full speed
#define TURN_SPEED        200
#define FORWARD_SPEED     200
#define REVERSE_DURATION  200  // ms
#define TURN_DURATION     300  // ms


Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4Buzzer buzzer;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors botSensors;

#define NUM_SENSORS 3
unsigned int lineSensorValues[NUM_SENSORS];

int randVar = 0;
int songLoopNum = 0;
int toneTime = 0;

void loopSong(int songLoopNum, int toneTime)
{
  for(int i = 0; i < songLoopNum; i++)
  {
    for(int i = 0; i < 3; i++)
    {
      buzzer.playNote(NOTE_A(4), 2*toneTime, 15);
      delay(toneTime);
      buzzer.playNote(NOTE_E(4), toneTime, 15);
      delay(toneTime);
      buzzer.playNote(NOTE_G(4), toneTime, 15);
      delay(toneTime);
      buzzer.playNote(NOTE_A(4), toneTime, 15);
      delay(toneTime);
      buzzer.playNote(NOTE_C(5), toneTime, 15);
      delay(toneTime);
      buzzer.playNote(NOTE_B(4), toneTime, 15);
      delay(toneTime);
      buzzer.playNote(NOTE_G(4), toneTime, 15);
      delay(toneTime);
      buzzer.playNote(NOTE_A(4), toneTime, 15);
      delay(toneTime);
    }
  buzzer.playNote(NOTE_D(5), 2*toneTime, 15);
  delay(2*toneTime);
  buzzer.playNote(NOTE_C(5), toneTime, 15);
  delay(toneTime);
  buzzer.playNote(NOTE_B(4), 2*toneTime, 15);
  delay(2*toneTime);
  buzzer.playNote(NOTE_C(5), toneTime, 15);
  delay(toneTime);
  buzzer.playNote(NOTE_B(4), toneTime, 15);
  delay(toneTime);
  buzzer.playNote(NOTE_G(4), toneTime, 15);
  delay(toneTime);
  }
}

void waitForButtonAndCountDown()
{
  ledYellow(1);
  lcd.clear();
  lcd.print(F("Press A"));

  buttonA.waitForButton();

  ledYellow(0);
  lcd.clear();

  // Play audible countdown.
  for (int i = 0; i < 3; i++)
  {
    delay(1000);
    buzzer.playNote(NOTE_G(3), 200, 15);
  }
  delay(1000);
}

void setup()
{
  // Uncomment if necessary to correct motor directions:
  //motors.flipLeftMotor(true);
  //motors.flipRightMotor(true);

  lineSensors.initThreeSensors();
  botSensors.initThreeSensors();
  waitForButtonAndCountDown();
}

void loop()
{
  if (buttonA.isPressed())
  {
    // If button is pressed, stop and wait for another press to
    // go again.
    motors.setSpeeds(0, 0);
    buttonA.waitForRelease();
    waitForButtonAndCountDown();
  }

  lineSensors.read(lineSensorValues);

  botSensors.read();

  if (lineSensorValues[0] < QTR_THRESHOLD)
  {
    // If leftmost sensor detects line, reverse and turn to the
    // right.
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  else if (lineSensorValues[NUM_SENSORS - 1] < QTR_THRESHOLD)
  {
    // If rightmost sensor detects line, reverse and turn to the
    // left.
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  /*else
  {
    // Otherwise, go straight.
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }*/
  else if (botSensors.countsFrontWithLeftLeds() >= 2 || botSensors.countsFrontWithRightLeds() >= 2)
  {
    randVar = random(0,1);
    if(randVar == 0)
    {
      motors.setSpeeds(-2*FORWARD_SPEED, 0.5*FORWARD_SPEED);
      delay(100);
    }
    else if(randVar == 1)
    {
      motors.setSpeeds(-2*FORWARD_SPEED, 0.5*FORWARD_SPEED);
      delay(100);
    }
  }
  else if (botSensors.countsLeftWithLeftLeds() >= 2)
  {
    motors.setSpeeds(2*FORWARD_SPEED, FORWARD_SPEED);
    delay(100);
  }
  else if (botSensors.countsRightWithRightLeds() >= 2)
  {
    motors.setSpeeds(FORWARD_SPEED, 2*FORWARD_SPEED);
    delay(100);
  }
  else
  {
    randVar = random(0,1);
    if (randVar == 0)
    {
      motors.setSpeeds(FORWARD_SPEED, -FORWARD_SPEED);
    }
    else if (randVar == 1)
    {
      motors.setSpeeds(-FORWARD_SPEED, FORWARD_SPEED);
    }
    songLoopNum = 1;
    toneTime = 10; //insert 200 to play song properly
    loopSong(songLoopNum, toneTime);
  }
}
