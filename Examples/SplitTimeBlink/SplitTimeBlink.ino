/*

	Example: Use Split Time to Create a Blinking Indicator


*/

#include <TimeSplit.h>
#define Pin_LED 13
#define LED_ON HIGH
#define LED_Speed 2

TimeRTClock simpleClock;

//Create LED Indicator
StatusLED indicatorLED (Pin_LED, HIGH,LED_Speed,0);

void setup() {
  Serial.begin(115200);
  simpleClock.timeSetBase(10); // Define Time Base - 10 Threads per Second
  
  //Start Real Time Counter at  8:59:50:0
  simpleClock.timeSet (0,50,59,8,0,0); // Define Time Start
  simpleClock.timeSettoRTC();  //Move Time to RTC

  indicatorLED.codeLED = 2;
 
}  

void loop() {
// TimeSplit Loop Structure
// 1. Wait while the time thread is the same
  do { simpleClock.timeRTCmillis();
// 2. Do Nothing, or execute items that are outside a Thread.

	 } while ( simpleClock.timeRTCSame() );

// 3. Space to create any changes conditional to time prior services	 
  if (simpleClock.xsecs == 0) {
    // Stamp RTC at Every Thread 0 (every second)
    simpleClock.timeStampRTC();
    Serial.println( "TimeSplit");
    }
	

  
// 4. Fulfill All Services
  indicatorLED.timeThreadFulfill();
// 5. Process any items that are dependent of service Fulfillment AFTER.


}

