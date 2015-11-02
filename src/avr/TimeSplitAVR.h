/*
 TimeSplit v.01 - Time Counter and Threads for Arduino
 Copyright (c) 2015 Pablo Miliani  All right reserved.
 */


class StatusLED : public TimeThread {
  public:
	StatusLED (uint8_t pinLED, boolean pinOnLED, byte pthreadPeriod, byte pthreadStart); //Constructor
	byte codeLED;
	void updateLED(void);
	boolean setLEDON (void);
	boolean setLEDOFF (void);
	boolean timeThreadFulfill (void);
	void svcMakeInactive (void); //Different to include LED OFF
  private:
    static const unsigned int menuItemLED [17];
    static const unsigned int LED_State_Mask [16];	
    boolean errorStatus;
	uint8_t LEDBit;   // set pin's ports and bitmask
	volatile uint8_t *LEDReg;
	volatile uint8_t *LEDOut;
	byte clockLED;
	boolean pinLogicLED;
};
