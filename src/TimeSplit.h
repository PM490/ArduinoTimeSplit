/*
 TimeSplit v.01 - Time Counter and Threads for Arduino
 Copyright (c) 2015 Pablo Miliani  All right reserved.
 */

#ifndef TimeSplit_h
#define TimeSplit_h

#include <Arduino.h>

class SvcCall {
  public:
    void svcMakeActive (void);
    void svcMakeInactive (void);
	void svcArm (void);
	void svcFulfill (void);
	void svcAttach(void(*svcFunct)(void));
	boolean Status = 0;
	protected:
      void ( *svcPointer ) (void);
	  boolean svcArmed = 0;
	  boolean svcNotFulfilled = 0;	
};

class SvcStartCall: public SvcCall {
  public:
    void svcStartMakeActive (void);
    void svcStartMakeInactive (void);
	void svcStartArm (void);
	void svcStartFulfill (void);
	void svcStartAttach(void(*svcFunct)(void));
	protected:
      void ( *svcStartPointer ) (void);
	  boolean svcStartArmed = 0;
	  boolean svcStartNotFulfilled = 0;	
};

class TimeBlock {
  public: 
	void timeSet ( byte pxsecs, byte psecs, byte pmins, byte phrs, byte pdays, byte pweeks );
	void timeInc ( byte pxsecs, byte psecs, byte pmins, byte phrs, byte pdays, byte pweeks );
	void timeDec ( byte pxsecs, byte psecs, byte pmins, byte phrs, byte pdays, byte pweeks );
    void timeStamp (void);
	void timeSetBase (byte pbase);
  protected:
    byte timeKeeper [6];
    static long unsigned int tKDiv [6];
    static byte tKOvf [6];
    byte timeOvf ( byte timeArray [6] );
    byte timeUnf ( byte timeArray [6] );
	void stampTimeArray( byte timeArray [6] );
};

class TimeRTClock : public TimeBlock {
  public:
	static byte xsecs;
    static byte secs;
    static byte mins;
    static byte hrs;
    static byte days;
    static byte weeks;
	void timeSettoRTC (void);
	void timeRTCmillis (void);  //Update RTC from Arduino Millis()
	void timeRTCUpdate (unsigned long int updateTimems);  //Update RTC from Parameter
	void timeStampRTC (void);
	boolean timeRTCSame (void);
  protected:
    static byte timeLastXsecs;
    static byte timeRTC [6];
    static unsigned int millisLastRead;
    static unsigned int millisPriorRead;
    static unsigned int millisChange;
    static unsigned long int leftms;
};

class TimeDownTimer : public SvcCall, public TimeBlock {
  public:
	void setDuration ( byte pxsecs, byte psecs, byte pmins, byte phrs, byte pdays, byte pweeks );
    boolean timeTimerFulfill (void);
    boolean timerRestart = 0;
  protected:
  	byte Duration [6];
};

class TimePeriod : public TimeRTClock, public SvcStartCall {
  public:
    TimePeriod (void);
	void setTimeEnd ( byte pxsecs, byte psecs, byte pmins, byte phrs, byte pdays, byte pweeks );
	void setDuration ( byte pxsecs, byte psecs, byte pmins, byte phrs, byte pdays, byte pweeks );
	void setRecurrency (byte pDigits);
	void timeStampEnd(void);
    boolean timePeriodFulfill (void);

    boolean updatePeriodGetStatus (void); // Update Event after after RTC was updated.
	boolean updateRTCPeriodGetStatus (void); // Update RTC from Millis first, then period.
	
  protected:
    static const byte arrayCompareKey [7];
  	byte timeEnd [6];
	byte timeDigitCompare;
	void periodCompare(byte pDigits);
};

class TimeThread : public TimeRTClock, public SvcCall {
  public:
	void setThread (byte pthreadPeriod, byte pthreadStart);
	boolean updateThreadGetStatus (void);
	boolean timeThreadFulfill (void);
  protected:
    void ( *threadFunction ) (void); 
    byte threadPeriod;
    byte lastXsecs;
	byte threadCtr;
	boolean threadStatus; // 1 when xsec corresponds to thread.
};

#if defined(AVR)
 #include "AVR/TimeSplitAVR.h"
#else
 #error "The Class StatusLED only supports AVR boards."
#endif

#endif
