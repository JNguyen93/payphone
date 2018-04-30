/* Justin Nguyen
    3/18/2017
*/

#include <Keypad.h>
#include <Bounce2.h>
#define arr_len( x )  ( sizeof( x ) / sizeof( *x ) )

const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns0

//define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins
byte rowPins[ROWS] = {6, 7, 8, 9}; //connect to the row pinouts of the keypad
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = {2, 3, 4}; //connect to the column pinouts of the keypad
//Green is common
int quarter = 10; //white
int dime = 11; //red
int nickel = 12; //blue
int hookSwitch = 5;
int amountNeeded = 135;
int total = 0;
bool coinFlag = false;
char sol[8] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
char fun[4] = {'\0', '\0', '\0', '\0'};
char pass[3] = {'\0', '\0', '\0'};
int passCap = 2;
int cap = 7;
int funCap = 3;
String solution = "2902548";
String bypass = "52";
String restart = "8977296";
String funny = "911";
int solPointer = 0;
int funPointer = 0;
int passPointer = 0;
boolean dialing = false;
unsigned long lastDial;

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Bounce quart = Bounce();
Bounce dim = Bounce();
Bounce nick = Bounce();

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps for debugging
  //Serial.println("Payphone");
  pinMode(quarter, INPUT_PULLUP);
  pinMode(dime, INPUT_PULLUP);
  pinMode(nickel, INPUT_PULLUP);
  pinMode(hookSwitch, INPUT_PULLUP);

  customKeypad.addEventListener(keypadEvent);
  quart.attach(quarter);
  dim.attach(dime);
  nick.attach(nickel);
  quart.interval(10);
  dim.interval(10);
  nick.interval(10);

  lastDial = millis();
}

void loop() {
  int conn = digitalRead(hookSwitch);
  char customKey = customKeypad.getKey();

  while (!coinFlag) {
    quart.update();
    dim.update();
    nick.update();
    if (quart.fell()) {
      total += 25;
      //Serial.print("Total: ");
      //Serial.println(total);
    }
    if (dim.fell()) {
      total += 10;
      //Serial.print("Total: ");
      //Serial.println(total);
    }
    if (nick.fell()) {
      total += 5;
      //Serial.print("Total: ");
      //Serial.println(total);
    }
    if (total < amountNeeded) {
      coinFlag = false;
    }
    else if (total >= amountNeeded) {
      //Serial.println("Coinflag is true");
      coinFlag = true;
      total = 0;
    }
    
    char bypassKey = customKeypad.getKey();
    if (passPointer < passCap && bypassKey != '\0') {
      pass[passPointer] = bypassKey;
      passPointer++;
    }
    else if (passPointer == passCap) {
      passPointer = 0;
    }
    String passkey = String(pass);
    //Serial.println("Passkey = ");
    //Serial.println(passkey);
    if (passkey.equals(bypass)) {
      //Serial.println("Coinflag is true");
      coinFlag = true;
    }
  }
  if (conn) {
    reset(&solPointer, sol, cap);
    reset(&funPointer, fun, funCap);
    dialing = false;
  }
  else {
    if (!dialing) {
      if (millis() - lastDial >= 4000) {
        playSound(15);
        lastDial = millis();
      }
    }
    else {
      if (solPointer < cap && customKey != '\0') {
        sol[solPointer] = customKey;
        solPointer++;
      }
      else if (solPointer == cap) {
        solPointer = 0;
      }

      if (funPointer < funCap && customKey != '\0') {
        fun[funPointer] = customKey;
        funPointer++;
      }
      else if (funPointer == funCap) {
        funPointer = 0;
      }

      switch (customKey) {
        case '0':
          playSound(10);
          break;
        case '1':
          playSound(1);
          break;
        case '2':
          playSound(2);
          break;
        case '3':
          playSound(3);
          break;
        case '4':
          playSound(4);
          break;
        case '5':
          playSound(5);
          break;
        case '6':
          playSound(6);
          break;
        case '7':
          playSound(7);
          break;
        case '8':
          playSound(8);
          break;
        case '9':
          playSound(9);
          break;
        case '*':
          playSound(11);
          break;
        case '#':
          playSound(12);
      }

      String answer = String(sol);
      String police = String(fun);
      //Serial.println("Answer = ");
      //Serial.println(answer);
      //Serial.println("Police = ");
      //Serial.println(police);
      if (answer.equals(solution)) {
        playSound(14);
        playSound(13);
        reset(&solPointer, sol, cap);
        reset(&funPointer, fun, funCap);
      }
      else if (answer.equals(restart)) {
        //Serial.println("Payphone Reset");
        coinFlag = false;
        reset(&solPointer, sol, cap);
        reset(&funPointer, fun, funCap);
        reset(&passPointer, pass, passCap);
        dialing = false;
      }

      /*if (police.equals(funny)) {
        //playcomplete(ringer);
        //playcomplete(easterEgg);
        reset(&solPointer, sol, cap);
        reset(&funPointer, fun, funCap);
      }*/
      //delay(1000);
    }
  }
}

/////////////////////////////////// HELPERS
/*
   print error message and halt
*/

void reset(int* pointer, char* solution, int capacity) {
  *pointer = 0;
  for (int k = 0; k < capacity; k++) {
    solution[*pointer] = '\0';
    *pointer = *pointer + 1;
  }
  *pointer = 0;
  dialing = false;
}

void keypadEvent(KeypadEvent key) {
  switch (customKeypad.getState()) {
    case PRESSED:
      dialing = true;
      //wave.stop();
      break;
    case RELEASED:
      break;
    case HOLD:
      break;
  }
}

void playSound(int num) {
  Serial.write(num);
  /*switch (num) {
    case 10:
      //0 or 10
      digitalWrite(bit0, LOW);
      digitalWrite(bit1, HIGH);
      digitalWrite(bit2, LOW);
      digitalWrite(bit3, HIGH);
      break;
    case 1:
      digitalWrite(bit0, HIGH);
      digitalWrite(bit1, LOW);
      digitalWrite(bit2, LOW);
      digitalWrite(bit3, LOW);
      break;
    case 2:
      digitalWrite(bit0, LOW);
      digitalWrite(bit1, HIGH);
      digitalWrite(bit2, LOW);
      digitalWrite(bit3, LOW);
      break;
    case 3:
      digitalWrite(bit0, HIGH);
      digitalWrite(bit1, HIGH);
      digitalWrite(bit2, LOW);
      digitalWrite(bit3, LOW);
      break;
    case 4:
      digitalWrite(bit0, LOW);
      digitalWrite(bit1, LOW);
      digitalWrite(bit2, HIGH);
      digitalWrite(bit3, LOW);
      break;
    case 5:
      digitalWrite(bit0, HIGH);
      digitalWrite(bit1, LOW);
      digitalWrite(bit2, HIGH);
      digitalWrite(bit3, LOW);
      break;
    case 6:
      digitalWrite(bit0, LOW);
      digitalWrite(bit1, HIGH);
      digitalWrite(bit2, HIGH);
      digitalWrite(bit3, LOW);
      break;
    case 7:
      digitalWrite(bit0, HIGH);
      digitalWrite(bit1, HIGH);
      digitalWrite(bit2, HIGH);
      digitalWrite(bit3, LOW);
      break;
    case 8:
      digitalWrite(bit0, LOW);
      digitalWrite(bit1, LOW);
      digitalWrite(bit2, LOW);
      digitalWrite(bit3, HIGH);
      break;
    case 9:
      digitalWrite(bit0, HIGH);
      digitalWrite(bit1, LOW);
      digitalWrite(bit2, LOW);
      digitalWrite(bit3, HIGH);
      break;
    case 11:
      //Star
      digitalWrite(bit0, HIGH);
      digitalWrite(bit1, HIGH);
      digitalWrite(bit2, LOW);
      digitalWrite(bit3, HIGH);
      break;
    case 12:
      //Pound
      digitalWrite(bit0, LOW);
      digitalWrite(bit1, LOW);
      digitalWrite(bit2, HIGH);
      digitalWrite(bit3, HIGH);
      break;
    case 13:
      //Answer, message
      digitalWrite(bit0, HIGH);
      digitalWrite(bit1, LOW);
      digitalWrite(bit2, HIGH);
      digitalWrite(bit3, HIGH);
      break;
    case 14:
      //Ringer
      digitalWrite(bit0, LOW);
      digitalWrite(bit1, HIGH);
      digitalWrite(bit2, HIGH);
      digitalWrite(bit3, HIGH);
      break;
    case 15:
      //Dialtone
      digitalWrite(bit0, HIGH);
      digitalWrite(bit1, HIGH);
      digitalWrite(bit2, HIGH);
      digitalWrite(bit3, HIGH);
  }
  delay(10);
  digitalWrite(bit0, LOW);
  digitalWrite(bit1, LOW);
  digitalWrite(bit2, LOW);
  digitalWrite(bit3, LOW);*/
}

