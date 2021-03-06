/*  MonsterMotoShiel_test - Testing Monster Moto Shield for Arduino */
#define CW   1
#define CCW  2
#define CS_THRESHOLD 50
#define PWM_MAX 225
#define PWM_SLOW 50
#define RUNNING_TIME_MS 5000

/*  VNH2SP30 pin definitions */
int INA[2] = {7, 4};  // INA: Clockwise input
int INB[2] = {8, 9}; // INB: Counter-clockwise input
int PWM_out[2] = {5, 6}; // PWM output
int CS[2] = {2, 3}; // CS: Current sense analog input
int EN[2] = {0, 1}; // EN: Status of switches output (Analog pin)

void setup(){
  Serial.begin(9600);
  // Initialize digital pins as outputs
  for (int i=0; i<2; i++){
    pinMode(INA[i], OUTPUT);
    pinMode(INB[i], OUTPUT);
    pinMode(PWM_out[i], OUTPUT);
  }
  // Turn Off all motors
  for (int i=0; i<2; i++){
    motorOff(i);
  }
}// void setup()

void loop(){
  motorGo(0, CW, PWM_MAX);
  motorGo(1, CW, PWM_MAX);
  waitAndTurnMotorsOff(RUNNING_TIME_MS);
  motorGo(0, CCW, PWM_SLOW);
  motorGo(1, CCW, PWM_SLOW);
  waitAndTurnMotorsOff(RUNNING_TIME_MS);
  motorGo(0, CCW, PWM_MAX);
  motorGo(1, CCW, PWM_MAX);
  waitAndTurnMotorsOff(RUNNING_TIME_MS);
}// void loop()

void printCurrentSense(){
  int cs0 = analogRead(CS[0]);
  int cs1 = analogRead(CS[1]);
  Serial.println("cs0:"+String(cs0)+", cs1:"+String(cs1));
  if (cs0 > CS_THRESHOLD || cs1 > CS_THRESHOLD){
    Serial.println("OVERLOAD!!!");
  }
}// void printCurrentSense()

void waitAndTurnMotorsOff(int time_ms){
  delay(200);
  printCurrentSense();
  delay(time_ms);
  motorOff(0);
  motorOff(1);
  delay(2000);
}// void waitAndTurnMotorsOff(int time_ms)

void motorOff(int motor){
  for (int i=0; i<2; i++){
    digitalWrite(INA[i], LOW); // Brake to GND
    digitalWrite(INB[i], LOW); // Brake to GND
  }// for
  analogWrite(PWM_out[motor], 0);
}// void motorOff(int motor)

/* motorGo() will set a motor going in a specific direction
 - motors are labeled as 0 and 1
 - direct: Should be between 0 and 3, with the following result
 1: Clockwise
 2: CounterClockwise
 - PWM_out: should be a value between 0 and 255
 */
void motorGo(uint8_t motor, uint8_t direct, uint8_t m_pwm){
  if (motor <= 1 && m_pwm <=255){
      // Set inA[motor]
      if (direct == 1){
        digitalWrite(INA[motor], HIGH);
        digitalWrite(INB[motor], LOW);
      }
      else if (direct == 2){
        digitalWrite(INA[motor], LOW);
        digitalWrite(INB[motor], HIGH);
      }
      analogWrite(PWM_out[motor], m_pwm);
  }// if (motor <= 1 && m_PWM_out <=255)
}// void motorGo(uint8_t motor, uint8_t direct, uint8_t m_pwm)
