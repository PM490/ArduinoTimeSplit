/*
 TimeSplit v.01 - Time Counter and Threads for Arduino
 Copyright (c) 2015 Pablo Miliani  All right reserved.
 */

#include "TimeSplit.h"

// >>> SvcCall Methods
void SvcCall::svcAttach(void(*svcFunct)(void)){
  svcPointer = *svcFunct;	
};

void SvcCall::svcArm (void){
  svcNotFulfilled = 1;
};

void SvcCall::svcFulfill (void){
  if (svcArmed && svcNotFulfilled ) {
	  if (svcPointer != NULL){
	    svcPointer();
		}
	  svcNotFulfilled = 0;
    }	
};

void SvcCall::svcMakeActive (void){
  svcArmed = 1;	
};

void SvcCall::svcMakeInactive (void){
  svcArmed = 0;	
};

// >>> SvcStartCall Methods
void SvcStartCall::svcStartAttach(void(*svcFunct)(void)){
  svcStartPointer = *svcFunct;	
};

void SvcStartCall::svcStartArm (void){
  svcStartNotFulfilled = 1;
};

void SvcStartCall::svcStartFulfill (void){
  if (svcStartArmed && svcStartNotFulfilled ) {
	  if (svcStartPointer != NULL){
	    svcStartPointer();
		}
	  svcStartNotFulfilled = 0;
    }	
};

void SvcStartCall::svcStartMakeActive (void){
  svcStartArmed = 1;	
};

void SvcStartCall::svcStartMakeInactive (void){
  svcStartArmed = 0;	
};

//TimeBlock Definitions
long unsigned int TimeBlock::tKDiv[6] = {50, 1000, 60000, 3600000, 86400000, 604800000};
byte TimeBlock::tKOvf [6] = {20, 60, 60, 24, 7, 52};

// >>> TimeBlock Methods
void TimeBlock::timeSetBase (byte pbase){
  tKOvf[0] = pbase;
  tKDiv[0] = 1000/pbase;
}

void TimeBlock::timeSet ( byte pxsecs, byte psecs, byte pmins, byte phrs, byte pdays, byte pweeks  ) {
  timeKeeper [0] = pxsecs;
  timeKeeper [1] = psecs;
  timeKeeper [2] = pmins;
  timeKeeper [3] = phrs;
  timeKeeper [4] = pdays;
  timeKeeper [5] = pweeks;
  timeOvf( timeKeeper );
  }

void TimeBlock::timeInc ( byte pxsecs, byte psecs, byte pmins, byte phrs, byte pdays, byte pweeks ){
  timeKeeper [0] = timeKeeper [0] + pxsecs;
  timeKeeper [1] = timeKeeper [1] + psecs;
  timeKeeper [2] = timeKeeper [2] + pmins;
  timeKeeper [3] = timeKeeper [3] + phrs;
  timeKeeper [4] = timeKeeper [4] + pdays;
  timeKeeper [5] = timeKeeper [5] + pweeks;
  timeOvf( timeKeeper );
  }

byte TimeBlock::timeOvf ( byte timeArray [6] ) {		
  static int ovfCarry; 
  ovfCarry = 0;
  for ( int ovf_ctr = 0 ; ovf_ctr < 6; ovf_ctr++) {
    timeArray[ovf_ctr] = timeArray[ovf_ctr] + ovfCarry;
    ovfCarry = (int) (timeArray[ovf_ctr] / tKOvf [ovf_ctr]);
    timeArray[ovf_ctr] = (int) (timeArray[ovf_ctr] % tKOvf [ovf_ctr]);
    }
    return ovfCarry;
  }
  
void TimeBlock::timeDec ( byte pxsecs, byte psecs, byte pmins, byte phrs, byte pdays, byte pweeks ){
  timeKeeper [0] = timeKeeper [0] - pxsecs;
  timeKeeper [1] = timeKeeper [1] - psecs; 
  timeKeeper [2] = timeKeeper [2] - pmins;
  timeKeeper [3] = timeKeeper [3] - phrs;
  timeKeeper [4] = timeKeeper [4] - pdays;
  timeKeeper [5] = timeKeeper [5] - pweeks;
  timeUnf( timeKeeper );

  }

byte TimeBlock::timeUnf ( byte timeArray [6] ) {		
  static byte unfCarry; 
  unfCarry = 0;
  for ( int unf_ctr = 0 ; unf_ctr < 6; unf_ctr++) {
   timeArray[unf_ctr] -= unfCarry;
   if ( timeArray[unf_ctr] >= 0x80 )   {                     // Result is Negative
       timeArray[unf_ctr] = ~timeArray[unf_ctr] + 1 ;        // Invert Result
       unfCarry =  ( ( timeArray[unf_ctr] -1 ) / tKOvf [unf_ctr]) + 1; // Calculate UnderCarry
       timeArray[unf_ctr]	= ( (byte) tKOvf [unf_ctr] - (byte) (timeArray[unf_ctr] % tKOvf [unf_ctr])) % tKOvf [unf_ctr];
	 }
	 else {
	   unfCarry = 0;
	   }
    }
	return unfCarry;
  }

/* 
// Working Time UnderFlow /KEEP THIS FOR REFERENCE
void TimeBlock::timeUnf ( byte timeArray [6] ) {		
  static byte unfCarry; 
  unfCarry = 0;
  for ( int unf_ctr = 0 ; unf_ctr < 6; unf_ctr++) {
   timeArray[unf_ctr] -= unfCarry;
   if ( timeArray[unf_ctr] >= 0x80 )   { //Result is Negative
       timeArray[unf_ctr] =    ~timeArray[unf_ctr] + 1 ; // Invert Result
       unfCarry =  ( (timeArray[unf_ctr] -1 ) / tKOvf [unf_ctr]) + 1; // Calculate UnderCarry
	   timeArray[unf_ctr] =  timeArray[unf_ctr] % tKOvf [unf_ctr];
	   timeArray[unf_ctr] = tKOvf [unf_ctr] - timeArray[unf_ctr];
       timeArray[unf_ctr]	= timeArray[unf_ctr] % tKOvf [unf_ctr];
	 }
	   else {
	     unfCarry = 0;
	   }
    }
  }
*/
 
void TimeBlock::stampTimeArray( byte timeArray [6] ) {
  // print realtime without CR/LF
  if (timeArray[5] < 10) {
    Serial.print ("Wk  ");
	} 
	else {
	 Serial.print ("Wk ");
	}  
  Serial.print (timeArray[5],DEC);
  Serial.print ("|Day ");
  Serial.print (timeArray[4],DEC);
  Serial.print ("| Time ");
  if (timeArray[3] < 10) {
    Serial.print ("0");
    }
   Serial.print (timeArray[3],DEC);
   Serial.print (":");
  if (timeArray[2] < 10) {
    Serial.print ("0");
    }
   Serial.print (timeArray[2],DEC);
   Serial.print (":");
  if (timeArray[1] < 10) {
    Serial.print ("0");
    }
   Serial.print (timeArray[1],DEC);
   Serial.print (":");
  if (timeArray[0] < 10) {
    Serial.print ("0");
    }
   Serial.print (timeArray[0],DEC);
   Serial.print (" | ");
   }

void TimeBlock::timeStamp (void) {
  Serial.print("* TIME|");
  stampTimeArray(timeKeeper);
}

//TimeRTClock Definitions
unsigned long int TimeRTClock::leftms = 0;
byte TimeRTClock::timeRTC [6] = {0,0,0,0,0,0}; 
byte TimeRTClock::timeLastXsecs = 0;
byte TimeRTClock::xsecs = 0;
byte TimeRTClock::secs = 0;
byte TimeRTClock::mins = 0;
byte TimeRTClock::hrs = 0;
byte TimeRTClock::days = 0;
byte TimeRTClock::weeks = 0;
unsigned int TimeRTClock::millisLastRead = 0;
unsigned int TimeRTClock::millisPriorRead = 0;
unsigned int TimeRTClock::millisChange = 0;

// >>> TimeRTClock Methods
void TimeRTClock::timeSettoRTC (void) {
  for (int x = 0; x<6 ; x++) {
    timeRTC[x] = timeKeeper [x];
    // Update Public Variables
    xsecs = timeRTC[0];
    secs  = timeRTC[1];
    mins  = timeRTC[2];
    hrs   = timeRTC[3];
    days  = timeRTC[4];
    weeks = timeRTC[5];
	}
	timeLastXsecs = xsecs;
}

void TimeRTClock::timeRTCUpdate (unsigned long int updateTimems) {
  unsigned long int timeinUnit;
  leftms += updateTimems;
  //Convert milliseconds to each unit
  timeinUnit = 0;
  for ( int div_ctr = 5 ; div_ctr >= 0; div_ctr--) {
    timeinUnit = (unsigned long int) ( leftms / tKDiv[div_ctr]);
    timeRTC[div_ctr] = timeRTC[div_ctr] + timeinUnit;
    leftms = leftms - (unsigned long int) ( timeinUnit * tKDiv [div_ctr]);
    }

  // Carry Overflows  
  timeOvf( timeRTC );
  
  // Update Public Variables
  xsecs = timeRTC[0];
  secs  = timeRTC[1];
  mins  = timeRTC[2];
  hrs   = timeRTC[3];
  days  = timeRTC[4];
  weeks = timeRTC[5];
}

void TimeRTClock::timeStampRTC (void) {
  Serial.print("*  RTC|");
  stampTimeArray (timeRTC);
}

void TimeRTClock::timeRTCmillis (void) {
  millisPriorRead = millisLastRead;
  millisLastRead = millis();  //Millis result gets intentionally truncated to 16 bit - unsigned int
 
  // Calculate elapsed millis since last pass
  millisChange = int (millisLastRead - millisPriorRead); //Typecast arithmetic handles overflows RT array
  timeRTCUpdate (millisChange); // Update RTC Array and Variables
}

boolean TimeRTClock::timeRTCSame (void){
  static boolean timeUnchanged;
  if (xsecs == timeLastXsecs) {
    timeUnchanged = 1;
    }
	else {
      timeLastXsecs = xsecs;
      timeUnchanged = 0;
	  }
  return timeUnchanged;
};

// >>> TimeThread Methods
void TimeThread::setThread (byte pthreadPeriod, byte pthreadStart){
  threadPeriod = pthreadPeriod;
  threadCtr = threadPeriod + pthreadStart;
  lastXsecs = xsecs;  
  Status = 0;
  svcNotFulfilled = 0;
  svcArmed = 1;
  };

boolean TimeThread::updateThreadGetStatus (void) {
    if (lastXsecs != xsecs) {
     lastXsecs = xsecs;
     threadCtr--;
	 if (threadCtr == 0) {
	   Status = 1;
	   threadCtr = threadPeriod;
	   svcNotFulfilled = 1;
	   }
	   else {
	     Status = 0;
	     }
     }
   return Status;
};

boolean TimeThread::timeThreadFulfill (void){
  if (updateThreadGetStatus() && svcArmed) {
	 svcFulfill();	   
  }
  return Status;
};

// TimePeriod Definitions
const byte TimePeriod::arrayCompareKey [7] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F }; 


TimePeriod::TimePeriod (void) {
  timeDigitCompare = 0x3F; // Default Period Compares all digits
}


// >>> TimePeriod Methods
void TimePeriod::setTimeEnd ( byte pxsecs, byte psecs, byte pmins, byte phrs, byte pdays, byte pweeks ) {
  timeEnd [0] = pxsecs;
  timeEnd [1] = psecs;
  timeEnd [2] = pmins;
  timeEnd [3] = phrs;
  timeEnd [4] = pdays;
  timeEnd [5] = pweeks;
  timeOvf( timeEnd );
};

void TimePeriod::setDuration ( byte pxsecs, byte psecs, byte pmins, byte phrs, byte pdays, byte pweeks ) {
  timeEnd [0] = timeKeeper [0] + pxsecs;
  timeEnd [1] = timeKeeper [1] + psecs;
  timeEnd [2] = timeKeeper [2] + pmins;
  timeEnd [3] = timeKeeper [3] + phrs;
  timeEnd [4] = timeKeeper [4] + pdays;
  timeEnd [5] = timeKeeper [5] + pweeks;
  timeOvf( timeEnd );
}

void TimePeriod::setRecurrency (byte pDigits) {
  timeDigitCompare = arrayCompareKey[pDigits];
  Status = 0;
};

void TimePeriod::timeStampEnd(void){
  Serial.print("*  END|");
  stampTimeArray(timeEnd);
};

boolean TimePeriod::updateRTCPeriodGetStatus (void) {
  timeRTCmillis ();
  periodCompare(timeDigitCompare);
  return Status;  
 };

 boolean TimePeriod::updatePeriodGetStatus (void) {
  periodCompare(timeDigitCompare);
  return Status;  
 };
 
void TimePeriod::periodCompare(byte pDigits) {
  byte compReg;
  byte compBit;
  byte i = 0;
  byte timeStartCompare = 0;
  byte timeEndCompare = 0;
  compReg = pDigits;
  compBit = 1;
  
  while ((compReg != 0) && (B01000000 > compBit)) { 
    if( timeKeeper [i] == timeRTC[i] ) { timeStartCompare = timeStartCompare | compBit;}
	if( timeEnd [i] == timeRTC[i] )  {timeEndCompare = timeEndCompare | compBit;}
	compReg = compReg >> 1;
	compBit = compBit << 1;
	i++;
    }
  if( (timeStartCompare & pDigits) == pDigits) {
    svcStartArm();
	Status = 1;
	}
  if( (timeEndCompare & pDigits) == pDigits) {
    svcArm();
    Status = 0;
	}	
};

 boolean TimePeriod::timePeriodFulfill (void) {
  periodCompare(timeDigitCompare);
  svcStartFulfill();
  svcFulfill();
  return Status;  
 };

// >>> TimeDownTimer Methods
void TimeDownTimer::setDuration ( byte pxsecs, byte psecs, byte pmins, byte phrs, byte pdays, byte pweeks ){
  timeKeeper [0] = pxsecs;
  timeKeeper [1] = psecs;
  timeKeeper [2] = pmins;
  timeKeeper [3] = phrs;
  timeKeeper [4] = pdays;
  timeKeeper [5] = pweeks;
  timeOvf( timeKeeper );
  for (int x = 0; x<6 ; x++) {
    Duration [x] = timeKeeper [x];
    }
  Status = 1;
  svcMakeActive();
};

boolean TimeDownTimer::timeTimerFulfill (void){
  if (Status) {
    timeKeeper [0] = timeKeeper [0] - 1;
    if (timeUnf( timeKeeper ) != 0 ) {
	  // Timer Expired
	  svcArm();     
	  svcFulfill();
      if (timerRestart) {
        for (int x = 0; x<6 ; x++) {
          timeKeeper [x] = Duration [x];
		  timeUnf(timeKeeper);
          }
		Status = 1;
		}
        else {
		  for (int x = 0; x<6 ; x++) {
            timeKeeper [x] = 0;
		    timeUnf(timeKeeper);
            }
          Status = 0;  
		}
	  }
  }
  return Status;
};
 

// >>> Status LED

const unsigned int StatusLED::menuItemLED [17] {
0x0000,  	//	Code 	0		All OFF
0x0001,  	//	Code 	1		Flash Once
0x0005,  	//	Code 	2		Flash Twice
0x0015,  	//	Code 	3		Flash Three times
0x0055,  	//	Code 	4		Flash Four times
0x0155,  	//	Code 	5		Flash Five times
0x0555,     //  Code    6       Flash Six Times
0x1555,     //  Code    7       Flash 7 Times
0x5555,     //  Code    8       Flash Continuously
0xFFFE,		//  Code    9       All ON, Flash Off Once
0xFFFA,		//  Code    10      All ON, Flash Off Twice
0xFFEA,		//  Code    11      All ON, Flash Off Three times
0xFFAA,		//  Code    12      All ON, Flash Off Four times
0xFEAA,		//  Code    13      All ON, Flash Off Five times
0xFAAA,		//  Code    14      All ON, Flash Off Six Times
0xEAAA,		//  Code    15      All ON, Flash Off Seven Times
0xFFFF      //	Code 	16      All ON
};

const unsigned int StatusLED::LED_State_Mask [16] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x100,0x200,0x400,0x800,0x1000,0x2000,0x4000,0x8000};	

// >>> StatusLED Methods
StatusLED::	StatusLED(uint8_t pinLED, boolean pinOnLED, byte pthreadPeriod, byte pthreadStart)
{
  uint8_t outPortLED;	
  errorStatus = 0;
  pinLogicLED = pinOnLED;

// get pin mapping and port for Led Pin
#ifdef NUM_DIGITAL_PINS
  if (pinLED >= NUM_DIGITAL_PINS) errorStatus = 1;
#endif

  LEDBit =  digitalPinToBitMask(pinLED);			// LED pin's ports and bitmask
  outPortLED = digitalPinToPort(pinLED);
  LEDReg = portModeRegister(outPortLED);
  LEDOut = portOutputRegister(outPortLED);			// get pointer to output register  

  if (!errorStatus) {
    *LEDReg |= LEDBit;						        // set LEDpin to OUTPUT 
    setLEDOFF();
  }
    
  //Thread
  threadPeriod = pthreadPeriod;
  threadCtr = threadPeriod + pthreadStart;
  lastXsecs = xsecs;  
  Status = 0;
  svcNotFulfilled = 0;
  svcArmed = 1;
};

void StatusLED::updateLED (void) {
    unsigned int led_state_bit = 0;
	led_state_bit =  ( menuItemLED[codeLED] & LED_State_Mask[ clockLED ] ) ;
	if (led_state_bit == 0 ) {
      setLEDOFF();
	  }
	  else {
        setLEDON();
		};
	clockLED = ++clockLED & 0x0F;
}

boolean StatusLED::timeThreadFulfill (void){
  if (updateThreadGetStatus() && svcNotFulfilled) {
	 if (svcArmed) {
		updateLED(); 
	   }
     svcNotFulfilled = 0;	   
  }
  return Status;
};

void StatusLED::svcMakeInactive( void ) {
  svcArmed = 0;
  setLEDOFF();
};

boolean StatusLED::setLEDON (void) {
  if (!errorStatus) {
    // Set LED On
    if ( pinLogicLED ) {
      *LEDOut |= LEDBit;         // set send Pin High 
      }
      else {
        *LEDOut &= ~LEDBit;        // set Send Pin Register low
        }
    return 0;
    }
	else {
	  return 1;
	  }
}

boolean StatusLED::setLEDOFF (void) {
  if (!errorStatus) {
    // Set LED OFF
    if ( pinLogicLED ) {
      *LEDOut &= ~LEDBit;        // set Send Pin Register low
      }
      else {
        *LEDOut |= LEDBit;         // set send Pin High 
        }	  
      return 0;
    }
	else {
	  return 1;	
	  }
}
