#define CMD_STR_CHAR '*'
#define CMD_END_CHAR '#'

#define LOCK_OPN_CHAR 'o'
#define LOCK_CLS_CHAR 'c'

#define LOCK_PIN_A 3
#define LOCK_PIN_B 4

#define KEY_H0 12
#define KEY_H1 11
#define KEY_H2 10
#define KEY_H3 9

#define KEY_V0 8
#define KEY_V1 7
#define KEY_V2 6
#define KEY_V3 5

//Array Of Pins Giving Supply To Key Matrix
uint8_t KEY_H_OUT[] = {KEY_H0,KEY_H1,KEY_H2,KEY_H3};

//Array Of Pins Reading State Of Key Matrix
uint8_t KEY_V_IN[] = {KEY_V0,KEY_V1,KEY_V2,KEY_V3};

//Array Of Key Functionalities
uint8_t KEY_MAP[4][4] = {{1, 2, 3, 12}, {4, 5, 6, 13}, {7, 8, 9, 14}, {10, 0, 11, 15}};

//Set The Passcode
uint64_t passCode = 1033;

//Variable For User To Enter Code
uint64_t userCode = 0;

bool KEY_PRESSED_NOW = false;
bool KEY_PRESSED_PREV = false;

void lock() {
  digitalWrite(LOCK_PIN_A, LOW);
  digitalWrite(LOCK_PIN_B, LOW);
  Serial.println("Closing");
}

void unlock() {
  digitalWrite(LOCK_PIN_A, HIGH);
  digitalWrite(LOCK_PIN_B, HIGH);
  Serial.println("Opening");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(LOCK_PIN_A, OUTPUT);
  pinMode(LOCK_PIN_B, OUTPUT);

  //Pin Config For KeyPad Matrix
  for (int i = 0; i<4; i++) {
    pinMode(KEY_H_OUT[i], OUTPUT);
    digitalWrite(KEY_H_OUT[i], HIGH);
  }
  for (int i = 0; i<4; i++) {
    pinMode(KEY_V_IN[i], INPUT_PULLUP);
  }
  
  Serial.println("Ready");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    if (Serial.read()!=CMD_STR_CHAR) {
      return;
    }
    delay(50);
    char CMD_MID_CHAR;
    CMD_MID_CHAR = Serial.read();
    if (CMD_MID_CHAR == LOCK_OPN_CHAR) {
      unlock();
    }
    else if (CMD_MID_CHAR == LOCK_CLS_CHAR) {
      lock();
    }
  }

  
  for (int i = 0; i<4; i++) {
    for (int j = 0; j<4; j++) {
       digitalWrite(KEY_H_OUT[j], HIGH);
    }
    digitalWrite(KEY_H_OUT[i], LOW);
    for (int j = 0; j<4; j++) {
      if (!digitalRead(KEY_V_IN[j])) {
        KEY_PRESSED_NOW = true;
        if (!KEY_PRESSED_PREV) {
          uint8_t KEY = KEY_MAP[i][j];
          if (KEY<=10) {
            userCode = userCode*10 + KEY;
          }
          else if (KEY==10) {
            if (userCode==passCode) {
              unlock();
            }
            else {
              userCode = 0;
            }
          }
          else if (KEY==11) {
            userCode = userCode/10;
          }
          else if (KEY==12) {
            lock();
          }
        }
      }
    }
  }
}
