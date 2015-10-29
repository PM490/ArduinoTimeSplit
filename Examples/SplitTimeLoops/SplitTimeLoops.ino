/*

	Example: Use Split Time to Equally Divide Loop into Threads


*/

#include <TimeSplit.h>

#define Pin_LED 13
#define LED_ON HIGH
#define LED_Speed 1

TimeRTClock simpleClock;

//Create LED Indicator
StatusLED indicatorLED (Pin_LED, HIGH,LED_Speed,0);

// Create Other Loops
TimeThread loop1Thread;
TimeThread loop2Thread;
TimeThread loop3Thread;


void setup() {
  Serial.begin(115200);
  simpleClock.timeSetBase(4); // Define Time Base - 4 Threads per Second
  
  //Start Real Time Counter at  8:59:50:0
  simpleClock.timeSet (0,50,59,8,0,0); // Define Time Start
  simpleClock.timeSettoRTC();  //Move Time to RTC

  indicatorLED.codeLED = 1;

  // Set Asynchronous Thread
  loop1Thread.setThread (3,1); //Every 3 xsecs, starting at xsec = 1
  loop1Thread.svcAttach( *loop1 );
  loop1Thread.svcMakeActive(); 

  // Set Asynchronous Thread
  loop2Thread.setThread (3,2);
  loop2Thread.svcAttach( *loop2 );
  loop2Thread.svcMakeActive(); 

  // Set Asynchronous Thread
  loop3Thread.setThread (3,3);
  loop3Thread.svcAttach( *loop3 );
  loop3Thread.svcMakeActive(); 
  
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
  loop1Thread.timeThreadFulfill();
  loop2Thread.timeThreadFulfill();
  loop3Thread.timeThreadFulfill();
  
  indicatorLED.timeThreadFulfill();
// 5. Process any items that are dependent of service Fulfillment AFTER.

}

// Fulfill Loops, these will execute Every 1/3 of a second
void loop1(){
  //Code will execute on Thread
  simpleClock.timeStampRTC();
  Serial.print("Thread1 at ");
  Serial.println(millis());
}

void loop2(){
  //Code will execute on Thread
  simpleClock.timeStampRTC();
  Serial.print("Thread2 at ");
  Serial.println(millis());
}

void loop3(){
  //Code will execute on Thread
  simpleClock.timeStampRTC();
  Serial.print("Thread3 at ");
  Serial.println(millis());

}
