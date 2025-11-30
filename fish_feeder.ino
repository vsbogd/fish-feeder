#define PIN_SERVO 11 // pin is driven via register and changing this define
                     // doesn't automatically change the pin, this constant is
                     // only for reference

#define FIRST_FEED_SECONDS 30   // delay of the first feed after powering on in seconds
#define FEEDING_PERIOD_HOURS 24 // feeding period, for example once in 24
                                // hours
#define NUMBER_OF_PORTIONS 1    // number of portions to drop when feeding

#define OPENED 0    // feeding position in degrees
#define CLOSED 180  // position between feedings in degrees

// Values below should be ok for SG90 servo which is used in original design
// One may need to change them for another model or specific instance of the drive 
#define MIN_ANGLE 0     // minimal angle for the servo
#define MAX_ANGLE 180   // maximum angle for the servo
#define MIN_SIGNAL 31   // the value to send on pin to get the minimal angle
#define MAX_SIGNAL 160  // the value to send on pin to get the maximum angle

unsigned long next_feed_ms = 0;

void setup() {
  init_pwm();
  set_position(CLOSED);
  next_feed_ms = millis() + FIRST_FEED_SECONDS * 1000;
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
    next_feed_ms += FEEDING_PERIOD_HOURS * 3600000;
    for (int i = 0; i < NUMBER_OF_PORTIONS; i++) {
      to_position(OPENED);
      delay(1000);
      to_position(CLOSED);
      delay(1000);
    }
  }
  delay(100);
}
