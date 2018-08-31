#define CMD_STR_CHAR '*'
#define CMD_END_CHAR '#'

#define LOCK_OPN_CHAR 'o'
#define LOCK_CLS_CHAR 'c'

#define LOCK_PIN_A 3
#define LOCK_PIN_B 4

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
  Serial.println("Ready");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()<1) {
    return;
  }
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
