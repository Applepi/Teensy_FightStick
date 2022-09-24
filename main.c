#include <Bounce.h>

const int numButtons = 14;
const int Buttons[numButtons] = {6,7,8,9,10,11,13,14,15,16,17,20,21,22};  
const int numJoy = 4;
const int Joy[numButtons] = {2,3,4,5}; 
const int stateToggle = 12;

Bounce pushbutton = Bounce(stateToggle, 10);

Bounce bouncer[numButtons] = {
  Bounce( Buttons[0],10 ),
  Bounce( Buttons[1],10 ),
  Bounce( Buttons[2],10 ),
  Bounce( Buttons[3],10 ),
  Bounce( Buttons[4],10 ),
  Bounce( Buttons[5],10 ),
  Bounce( Buttons[6],10 ),
  Bounce( Buttons[7],10 ),
  Bounce( Buttons[8],10 ),
  Bounce( Buttons[9],10 ),
  Bounce( Buttons[10],10 ),
  Bounce( Buttons[11],10 ),
  Bounce( Buttons[12],10 ),
  Bounce( Buttons[13],10 )
};

void setup() {
  // you can print to the serial monitor while the joystick is active!
  Serial.begin(9600);
  // configure the joystick to manual send mode.  This gives precise
  // control over when the computer receives updates, but it does
  // require you to manually call Joystick.send_now().
  Joystick.useManualSend(true);
  for (int i=0; i<numButtons; i++) {
    pinMode(Buttons[i], INPUT_PULLUP);
  }
  for (int i=0; i<numJoy; i++) {
    pinMode(Joy[i], INPUT_PULLUP);
  }
  pinMode(23, OUTPUT);
  digitalWrite(23, LOW);
  pinMode(stateToggle, INPUT_PULLUP);
  Serial.println("Begin Complete Joystick Test");
}

byte allButtons[numButtons];
byte prevButtons[numButtons];
byte allJoy[numJoy];
byte prevJoy[numJoy];
byte angleMask;
int angle=0;

unsigned int count = 0;            // how many times has it changed to low
unsigned long countAt = 0;         // when count changed
unsigned int countPrinted = 0;     // last count printed
unsigned int stateMax = 2;          //

void loop() {

  if (pushbutton.update()) {
    if (pushbutton.fallingEdge()) {
      count = count + 1;
      if (count > stateMax)
        count = 0;
      countAt = millis();
    }
  } else {
    if (count != countPrinted) {
      unsigned long nowMillis = millis();
      if (nowMillis - countAt > 100) {
        Serial.print("count: ");
        Serial.println(count);
        countPrinted = count;
      }
    }
  }

  if (count == 0){
    // read digital pins and use them for the buttons
    for (int i=0; i<numButtons; i++) {
      if (digitalRead(Buttons[i])) {
        // when a pin reads high, the button is not pressed
        // the pullup resistor creates the "on" signal
        allButtons[i] = 0;
      } else {
        // when a pin reads low, the button is connecting to ground.
        allButtons[i] = 1;
      }
      Joystick.button(i + 1, allButtons[i]);
      }
      angleMask = 8 * !digitalRead(5) + 4 * !digitalRead(3) 
                    + 2 * !digitalRead(4) + !digitalRead(2);
      switch (angleMask){
        case 1:
          angle = 90; 
          break;
        case 2:
          angle = 180; 
          break;
        case 3:
          angle = 135; 
          break;
        case 4:
          angle = 270; 
          break;
        case 6:
          angle = 225; 
          break;
        case 8:
          angle = 0; 
          break;
        case 9:
          angle = 45; 
          break;
        case 12:
          angle = 315; 
          break;
        default:
          angle = -1; 
          break;
      }
      
    Joystick.hat(angle);
    angleMask = 0;

    Joystick.send_now();
    
    // check to see if any button changed since last time
    boolean anyChange = false;
    for (int i=0; i<numButtons; i++) {
      if (allButtons[i] != prevButtons[i]) anyChange = true;
      prevButtons[i] = allButtons[i];
    }
    
    // if any button changed, print them to the serial monitor
    if (anyChange) {
      Serial.print("Buttons: ");
      for (int i=0; i<numButtons; i++) {
        Serial.print(allButtons[i], DEC);
      }
      Serial.println();
    }
  }
  else{
    for (int i = 0; i < numButtons; i++){
      bouncer[i].update();
    }
    if (bouncer[0].fallingEdge()){
      Keyboard.print("3");
    }
    if (bouncer[1].fallingEdge()){
      Keyboard.print("2");
    }
    if (bouncer[2].fallingEdge()){
      Keyboard.print("1");
    }
    if (bouncer[3].fallingEdge()){
      Keyboard.press(KEY_ENTER);
      Keyboard.release(KEY_ENTER);
    }
    if (bouncer[4].fallingEdge()){
      Keyboard.print(".");
    }
    if (bouncer[5].fallingEdge()){
      Keyboard.press(KEY_ESC);
      Keyboard.release(KEY_ESC);
    }
    if (bouncer[6].fallingEdge()){
      Keyboard.print("P");
    }
    if (bouncer[7].fallingEdge()){
      Keyboard.print(";");
    }
    if (bouncer[8].fallingEdge()){
      Keyboard.print("O");
    }
    if (bouncer[9].fallingEdge()){
      Keyboard.print("L");
    }
    if (bouncer[10].fallingEdge()){
      Keyboard.print("I");
    }
    if (bouncer[11].fallingEdge()){
      Keyboard.print("K");
    }
    if (bouncer[12].fallingEdge()){
      Keyboard.print("U");
    }
    if (bouncer[13].fallingEdge()){
      Keyboard.print("J");
    }
  }
 
  // a brief delay, so this runs "only" 200 times per second
  delay(5);
}
