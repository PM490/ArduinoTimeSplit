/*

	Example: Use Split Time to Create a Period


*/

#include <TimeSplit.h>
#define Pin_LED 13
#define LED_ON HIGH
#define LED_Speed 2

TimeRTClock simpleClock;
TimePeriod testPeriod;

//Create LED Indicator
StatusLED indicatorLED (Pin_LED, HIGH,LED_Speed,0);

void setup() {
  Serial.begin(115200);
  simpleClock.timeSetBase(25); // Define Time Base - 25 Threads per Second
  
  //Start Real Time Counter at  8:59:50:0
  simpleClock.timeSet (0,50,59,8,0,0); // Define Time Start
  simpleClock.timeSettoRTC();  //Move Time to RTC

  indicatorLED.codeLED = 1;
  
  //Set a Sample Period
  testPeriod.timeSet (5,0,0,0,0,0);    // Time Start - Period Start at xsecs 5
  testPeriod.setDuration(15,0,0,0,0,0); // Duration - Period Ends 15 xsecs later
  testPeriod.svcStartAttach ( *testServiceStart );
  testPeriod.svcStartMakeActive();
  testPeriod.svcAttach ( *testServiceEnd );
  testPeriod.svcMakeActive();
  testPeriod.setRecurrency(2); //Comparison will be valid every second
  /* How setRecurrency works
    The times defined by TimePeriod::timeSet and TimePeriod::timeEnd, or TimePeriod::timeDuration, are compared to determine the TimePeriod start and end.
	The comparison can be set for every second, minute, hour, day, week, or year. That allows periods to be repetitive (second, minute, hour, day, week) or not (year).
	For Example, If setRecurrency is set to 2 without changing the timeSet or setDuration, Period will instead repeat every minute, starting at second 0, xsec 5, and ending at second 0, xsec 20.
  */
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
    Serial.println(" simpleClock");
    }
	

  
// 4. Fulfill All Services
  indicatorLED.timeThreadFulfill();
  testPeriod.timePeriodFulfill();
// 5. Process any items that are dependent of service Fulfillment AFTER.

}

void testServiceStart(void) {
  // Execute at every testPeriod Start
  testPeriod.timeStampRTC();
  Serial.println("test Start");
}

void testServiceEnd(void) {
  // Execute at every testPeriod End
  simpleClock.timeStampRTC();
  Serial.println("test End");
  }
