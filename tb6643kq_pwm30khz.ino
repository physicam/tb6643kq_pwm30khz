#define PIN_SW1   2 // Speed Up
#define PIN_SW2   3 // Speed Down
#define PIN_IN1   9 // Output Pin for IN1
#define PIN_IN2   10 // Output Pin for IN2
#define MAX_VALUE 256
#define LIMIT_VALUE (MAX_VALUE-1)
#define DELTA_SPEED (MAX_VALUE/8)

int prev_sw1 = HIGH;
int prev_sw2 = HIGH;

int value_speed = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("TB6643KQ PWM 30kHz");

  pinMode(PIN_SW1, INPUT);  // Speed Up
  pinMode(PIN_SW2, INPUT);  // Speed Down
  pinMode(PIN_IN1, OUTPUT);  // Output Pin for IN1
  pinMode(PIN_IN2, OUTPUT);  // Output Pin for IN2

  // PWM 30kHz
  TCCR1A = 0b00000001;
  TCCR1B = 0b00000001;
  
  analogWrite(PIN_IN1, value_speed);
  analogWrite(PIN_IN2, value_speed);
}

void loop() {
  // put your main code here, to run repeatedly:
  int curr_sw1, curr_sw2;
  curr_sw1 = digitalRead(PIN_SW1);
  curr_sw2 = digitalRead(PIN_SW2);
  if (curr_sw1 != prev_sw1) {
    if (curr_sw1 == LOW) {
      // これが押された判定
      Serial.println("sw1");
      if (value_speed == -LIMIT_VALUE) {
        value_speed = -MAX_VALUE;
      }
      value_speed += DELTA_SPEED;
      if (value_speed > LIMIT_VALUE) {
        value_speed = LIMIT_VALUE;
      }
      updateSpeed();
    }
    prev_sw1 = curr_sw1;
    delay(10);
  }
  if (curr_sw2 != prev_sw2) {
    if (curr_sw2 == LOW) {
      // これが押された判定
      Serial.println("sw2");
      if (value_speed == LIMIT_VALUE) {
        value_speed = MAX_VALUE;
      }
      value_speed -= DELTA_SPEED;
      if (value_speed < -LIMIT_VALUE) {
        value_speed = -LIMIT_VALUE;
      }
      updateSpeed();
    }
    prev_sw2 = curr_sw2;
    delay(10);
  }
}

void updateSpeed() {
  if (value_speed > 0) {
    Serial.print("(");
    Serial.print(value_speed);
    Serial.println(", 0)");
    analogWrite(PIN_IN2, 0);
    analogWrite(PIN_IN1, value_speed);
  } else if (value_speed < 0) {
    Serial.print("(0, ");
    Serial.print(value_speed);
    Serial.println(")");
    analogWrite(PIN_IN1, 0);
    analogWrite(PIN_IN2, -value_speed);
  } else {
    Serial.println("(0, 0)");
    analogWrite(PIN_IN1, 0);
    analogWrite(PIN_IN2, 0);
  }
}
