# Overview

Fish feeder firmware for the https://www.thingiverse.com/thing:6378659 based on
cheap Arduino Mini Pro Atmega168 8MHz. Link to my make:
https://www.thingiverse.com/make:1242906

It controls PWM pin 11 manually using Atmega168 registers. Thus changing pin in
this code is not trivial. Constant in the code is provided for reference only.

Other constants in the code allows you tuning it for the specific case:
- `FIRST_FEED_SECONDS` - delay of the first feed after powering on in seconds
  (default 30 seconds)
- `FEEDING_PERIOD_HOURS` - feeding period in hours (default once in 24 hours)
- `NUMBER_OF_PORTIONS` - number of portions to drop when feeding (default 1
  portion)

# Tuning

After flashing the microcontroller, turn on the empty feeder and wait for 30
seconds. You should see feeding disk going to the default position immediately
after turning on. After `FIRST_FEED_SECONDS` delay disk should go into the
feeding position and after a second go back. 

If the angle between feeding and default position is 180 degrees but positions
are not correct then unscrew the disk, turn it to the default position, screw
it again and repeat the test.

If the angle is not 180 degrees then set default position first as described
above, then try to change `MIN_SIGNAL` and `MAX_SIGNAL` in order to change the
angle. Default values should be ok for the SG90 servo drive but if you have
another servo the additional tuning may be required.


