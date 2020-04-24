#include <IRLib2.h>
/*
#include <IRLibAll.h>
#include <IRLibDecodeBase.h>
#include <IRLibGlobals.h>
#include <IRLibRecvBase.h>
#include <IRLibRecvLoop.h>
*/

#include <XInput.h>

#define SW 4

int looper;

int x_pos; //500 default/centered
int y_pos; //500 default/centered

int l_wd_button = 12; // Button
int r_wd_button = 11; // Button

int receiver_pin = 7;
IRrecvPCI myReceiver(receiver_pin);
IRdecode myDecoder;

int IRinput;

/* IR REMOTE CODES
 *  Power =     16753245
 *  Volume+ =   16736925
 *  Volume- =   16754775
 *  Func/Stop = 16769565
 *  Previous =  16720605
 *  Next =      16761405
 *  Play/Pause= 16712445
 *  Up =        16748655
 *  Down =      16769055
 *  EQ =        16750695
 *  ST/REPT =   16756815
 *  0 =         16738455
 *  1 =         16724175
 *  2 =         16718055
 *  3 =         16743045
 *  4 =         16716015
 *  5 =         16726215
 *  6 =         16734885
 *  7 =         16728765
 *  8 =         16730805
 *  9 =         16732845

 *  Held Input = 4294967295
 */

//Joystick Min = -32768 Max = 323767

void reset()
{
  XInput.setJoystick(JOY_LEFT, 0, 0);
  XInput.setJoystick(JOY_RIGHT, 0, 0);
  XInput.release(BUTTON_A);
  XInput.release(BUTTON_B);
  XInput.release(BUTTON_X);
  XInput.release(BUTTON_Y);
  XInput.release(BUTTON_LB);
  XInput.release(BUTTON_RB);
  XInput.release(BUTTON_L3);
  XInput.release(BUTTON_R3);
  XInput.release(BUTTON_START);
  XInput.release(BUTTON_BACK);
  XInput.setDpad(false, false, false, false);
  Serial.println("RESET");
}

void setup() {
  pinMode(l_wd_button, INPUT);
  pinMode(r_wd_button, INPUT);

  pinMode(SW,INPUT);
  digitalWrite(SW,HIGH);

  myReceiver.enableIRIn(); // Start the receiver
  
  XInput.begin();
  looper = 0;
  
  Serial.begin(9600);

}

void loop() {
    // Checks Wavedash Buttons
    int lwdbuttonState = digitalRead(l_wd_button);
    int rwdbuttonState = digitalRead(r_wd_button);

    if (myReceiver.getResults()) 
    {
      myDecoder.decode();           //Decode it
      //myDecoder.dumpResults(true);  //Now print results. Use false for less detail
      //Serial.println(myDecoder.value);
      if (myDecoder.value != 4294967295)
      {
        IRinput = myDecoder.value;
        Serial.println(IRinput);
      }
      else
      {
        Serial.println("HELD");
      }
      //---------------BASIC BUTTONS---------------           (Uses 2 Frame Delays After Inputs)
      if (IRinput == 26775)                      //0
      {
        XInput.press(BUTTON_A);
        delay(16*5);
        XInput.release(BUTTON_A);
      }
      else if (IRinput == 12495)                      //1
      {
        XInput.press(BUTTON_B);
        delay(16*2);
        XInput.release(BUTTON_B);
      }
      else if (IRinput == 6375)                      //2
      {
        XInput.press(BUTTON_X);
        delay(16*5);
        XInput.release(BUTTON_X);
      }
      else if (IRinput == 31365)                      //3
      {
        XInput.press(BUTTON_L3);
        delay(16);
      }
      else if (IRinput == 4335)                      //4    Left Smash
      {
        XInput.setJoystick(JOY_RIGHT, -32768, 0);
        delay(16*3);
        reset();
      }
      else if (IRinput == 14535)                      //5   Grab
      {
        XInput.press(BUTTON_RB);
        delay(16*3);
        reset();
      }
      else if (IRinput == 23205)                      //6   Right Smash
      {
        XInput.setJoystick(JOY_RIGHT, 32768, 0);
        delay(16*3);
        reset();
      }
      else if (IRinput == 17085)                      //7
      {
        XInput.setJoystick(JOY_LEFT, -32768, 0);
        delay(16*2);
        reset();
      }
      else if (IRinput == 19125)                      //8
      {
        XInput.setJoystick(JOY_LEFT, 0, -32768);
        XInput.press(BUTTON_L3);
        delay(16*2);
        reset();
      }
      else if (IRinput == 21165)                      //9
      {
        XInput.setJoystick(JOY_LEFT, 32768, 0);
        delay(16*2);
        reset();
      }
      else if (IRinput == 25245)                      //Vol+   C-Stick Up
      { 
        XInput.press(BUTTON_X);
        delay(16*2);
        XInput.setJoystick(JOY_RIGHT, 0, 32767);
        delay(17);
        reset();
      }
      else if (IRinput == -22441)                      //Vol-   C-Stick Down
      {
        XInput.setJoystick(JOY_RIGHT, 0, -32767);
        delay(17);
        reset();
      }
      else if (IRinput == -7651)                      //FUNC?STOP
      {
        XInput.press(BUTTON_START);
        delay(34);
        reset();
      }
  
      //---------------MACRO FUNCTIONS---------------
      else if (IRinput == -15811)//NEXT                  WAVEDASH RIGHT
      {
        XInput.press(BUTTON_X);
        XInput.setJoystick(JOY_LEFT, 32768, -32768);
        delay(16*3);                                  
        XInput.press(BUTTON_L3);
        delay(17);
        XInput.setJoystick(JOY_LEFT, 0, 0);
        XInput.release(BUTTON_X);
        XInput.release(BUTTON_L3);
        delay(17);
      }
      else if (IRinput == 8925)//PREVIOUS                 WAVEDASH LEFT
      {
        XInput.press(BUTTON_X);                               
        XInput.setJoystick(JOY_LEFT, -32768, -32768);
        delay(16*3);   
        XInput.press(BUTTON_L3);
        delay(17);
        XInput.setJoystick(JOY_LEFT, 0, 0);
        XInput.release(BUTTON_X);
        XInput.release(BUTTON_L3);
        delay(17);
      }
      else if (IRinput == 765)//PLAY/PAUSE                 MULTI-SHINE
      {
        XInput.press(BUTTON_X);
        delay(16*3);                              
        XInput.setJoystick(JOY_LEFT, 0, -32768);
        XInput.press(BUTTON_B);
        delay(16);
        reset();
        XInput.press(BUTTON_X);
        delay(16*3);                                 
        XInput.setJoystick(JOY_LEFT, 0, -32768);
        XInput.press(BUTTON_B);
        delay(16);
        reset();
      }
      else if (IRinput == -28561)// UP     Dash Dance Right          
      {
        XInput.setJoystick(JOY_LEFT, -32768, 0);
        delay(16*5);
        XInput.setJoystick(JOY_LEFT, 32768, 0);
        delay(16*5);
        reset();
      }
      else if (IRinput == -8161)// DOWN     Dash Dance Left          
      {
        XInput.setJoystick(JOY_LEFT, 32768, 0);
        delay(16*5);
        XInput.setJoystick(JOY_LEFT, -32768, 0);
        delay(16*5);
        reset();
        
      }
      else if (IRinput == -20401)// ST/REPT           
      {
        
      }
      else if (IRinput == -26521)//EQ               SHFFL-Drill-Shine
      {
        XInput.press(BUTTON_X);
        delay(16*2);
        XInput.release(BUTTON_X);
        delay(16*7);
        XInput.setJoystick(JOY_RIGHT, 0, -32768);   
        delay(16*3);   
        XInput.setJoystick(JOY_LEFT, 0, -32768);                          
        delay(16*3);
        XInput.press(BUTTON_L3);
        delay(16*3);
        XInput.release(BUTTON_L3);
        delay(16*11);
        XInput.press(BUTTON_B);
        delay(16);
        reset();
      }
      myReceiver.enableIRIn();      //Restart receiver
    }

    

    /**
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (lwdbuttonState == HIGH) 
    {
      //wavedash left
      XInput.press(BUTTON_L3);
      delay(100);
      reset();
    } 
    if (rwdbuttonState==HIGH)
    {
      //wavedash right
      XInput.press(BUTTON_A);
      delay(100);
      reset();
    }
    */

    x_pos = analogRead (A0) ;  
    y_pos = analogRead (A1) ;
    //Min = 0 Max=1023 

    int newx = ((x_pos-511)/511)*323767*5;
    int newy = ((y_pos-511)/-511)*323767*5;
    XInput.setJoystick(JOY_LEFT, newx, newy);

    //Frame Delay
    //delay(17);
    delay(16);
}
