#define CMD_STR_CHAR '*'
#define CMD_END_CHAR '#'

#define LOCK_OPN_CHAR 'o'
#define LOCK_CLS_CHAR 'c'

#define LOCK_PIN 13

#define KEY_H0 A3
#define KEY_H1 A2
#define KEY_H2 A1
#define KEY_H3 A0

#define KEY_V0 9
#define KEY_V1 10
#define KEY_V2 11
#define KEY_V3 6

//Array Of Pins Giving Supply To Key Matrix
uint8_t KEY_H_OUT[] = {KEY_H0,KEY_H1,KEY_H2,KEY_H3};

//Array Of Pins Reading State Of Key Matrix
uint8_t KEY_V_IN[] = {KEY_V0,KEY_V1,KEY_V2,KEY_V3};

//Array Of Key Functionalities
uint8_t KEY_MAP[4][4] = {{1, 2, 3, 12}, {4, 5, 6, 13}, {7, 8, 9, 14}, {10, 0, 11, 15}};

//Set The Passcode
char passCode[6] = {0,3,1,2,6,2};

//Variables For User To Enter Code
char userCode[6] = {255,255,255,255,255,255};
uint8_t codeIndex = 0;

bool KEY_PRESSED_NOW = false;
bool KEY_PRESSED_PREV = false;

void unlock() {
  digitalWrite(LOCK_PIN, LOW);
  //Serial.println("Opening");
  delay(1000);
  digitalWrite(LOCK_PIN, HIGH);
  //Serial.println("Closing");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(LOCK_PIN, OUTPUT);
  digitalWrite(LOCK_PIN, HIGH);

  //Pin Config For KeyPad Matrix
  for (int i = 0; i<4; i++) {
    pinMode(KEY_H_OUT[i], OUTPUT);
    digitalWrite(KEY_H_OUT[i], HIGH);
  }
  for (int i = 0; i<4; i++) {
    pinMode(KEY_V_IN[i], INPUT_PULLUP);
  }
  
  //Serial.println("Ready");
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
          //Serial.println(KEY);
          if (KEY<10) {
            userCode[codeIndex] = KEY;
            codeIndex = codeIndex + 1;
          }
          else if (KEY==10) {
            Serial.print('*');
            for (int k = 0; k<5; k++) {
              Serial.print(userCode[k]);
              Serial.print(',');
            }
            Serial.print(userCode[5]);
            Serial.print('#');
            
            userCode[0] = 255;
            userCode[1] = 255;
            userCode[2] = 255;
            userCode[3] = 255;
            userCode[4] = 255;
            userCode[5] = 255;
            codeIndex = 0;
          }
          else if (KEY==11) {
            userCode[codeIndex] = 255;
            codeIndex = codeIndex - 1;
          }
        }
      }
    }
  }
  KEY_PRESSED_PREV = KEY_PRESSED_NOW;
  KEY_PRESSED_NOW = false;
  delay(50);  
}
