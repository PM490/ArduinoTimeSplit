# Arduino - TimeSplit
##Arduino Library - Simple Thread and Time Counter

This library was created as a simple tool to develop applications which require counting long periods of time, and at the same time, as an alternative for splitting the arduino __loop()__ into timeblocks that are used as Threads.

Also included in the library is an LED indicator class to allow simple messaging using a blinking LED, which is an implementation of a Thread built in the library.

The library has the basic building blocks to create real time applications, without requiring RTC hardware, as long as the accuracy of the arduino clock is sufficient for the task.

Library timming is derived from __millis()__ and it does not use any hardware resources other than the pin for the LED indicator.

####Dividing Time
Time is divided in fractions, here called _xsecs_. The default time base is _20 xsecs = 1 second_. The library keeps time in _xsecs, secs, mins, hrs, days, weeks_.

It is possible to change the number of fractions corresponding to one second, which we call the _time base_. The time base for the library is applicable to all __TimeBlocks__.

![TimeSplit Diagram](https://raw.githubusercontent.com/PM490/ArduinoTimeSplit/master/Images/TimeSplit.png)

####Actions
The diagram shows the relationship between the classes part of the library. At the completion of each time segment _Thread or Block_, a function is called to _servicing that class_. __SvcCall__ Class provides the infrastructure for fulfilling services on time events (__TimeThread, TimeDownTimer, TimePeriod, and StatusLED__). Additionally, __TimePeriod__ is also serviced at the start of the period by __SvcStartCall__.

####indicatorLED
When working with Arduino, is always helpful to have a simple status LED. As part of the implementation of Threads, the library also includes a StatusLED Class. The service of __StatusLED__ is explicit, providing flexibility to where in the __loop()___ should take place.

The indicator works by shifting a value, providing a countable number of flashes.

----------- | ------- | ----------- | ------
 LED Code   |         | LED Code    |       
----------- | ------- | ----------- | ------
 Code 0x00  |  OFF    |  Code 0x10  | On    
 Code 0x01  |  On  1  |  Code 0x09  | Off 1 
 Code 0x02  |  On  2  |  Code 0x0A  | Off 2
 Code 0x03  |  On  3  |  Code 0x0B  | Off 3
 Code 0x04  |  On  4  |  Code 0x0C  | Off 4
 Code 0x05  |  On  5  |  Code 0x0D  | Off 5
 Code 0x06  |  On  6  |  Code 0x0E  | Off 6
 Code 0x07  |  On  7  |  Code 0x0F  | Off 7
 Code 0x08  |  Flash  |             |      

