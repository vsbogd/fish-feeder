#include <Servo.h>

#define PIN_SERVO 11 // pin is driven via register and changing this define
                     // doesn't automatically change the pin

#define MIN_ANGLE 0
#define MAX_ANGLE 180
#define MIN_SIGNAL 31
#define MAX_SIGNAL 160

#define OPENED 0
#define CLOSED 180

unsigned long next_feed_ms = 0;

void setup() {
  init_pwm();
  set_position(CLOSED);
  next_feed_ms = millis() + 30 * 1000; // 30 seconds
}

void init_pwm() {
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22) | _BV(CS20); // prescaler 32, PWM frequency = 8_000_000 / 32 / 256 = 244Hz, 1 timer cycle = 4096us  
}

int angle_to_pwm(float angle) {
  return MIN_SIGNAL + (MAX_SIGNAL - MIN_SIGNAL) * angle / (MAX_ANGLE - MIN_ANGLE);
}

void set_position(float angle) {
  OCR2A = angle_to_pwm(angle);
  delay(200);
}

void to_position(float angle) {  
  int signal = angle_to_pwm(angle);
  int step = 1;
  if (OCR2A > signal) {
    step = -1;    
  }
  for (; OCR2A != signal; OCR2A += step) {    
    delay(20);
  }
}
 
void loop() {
  if (millis() > next_feed_ms) {
    next_feed_ms += 24 * 3600000; // 24 hours
    for (int i = 0; i < 3; i++) {
      to_position(OPENED);
      delay(1000);
      to_position(CLOSED);
      delay(1000);
    }
  }
  delay(100);
}
