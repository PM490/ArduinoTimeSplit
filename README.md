# Arduino - TimeSplit
##Arduino Library - Simple Thread and Time Counter

This library was created as a simple tool to develop applications which require counting long periods of time, and at the same time, as an alternative for splitting the arduino __loop()__ into timeblocks that are used as Threads.

Also included in the library is an LED indicator class to allow simple messaging using a blinking LED, which is an implementation of a Thread built in the library.

The library has the basic building blocks to create real time applications, without requiring RTC hardware, as long as the accuracy of the arduino clock is sufficient for the task.

Library timming is derived from __millis()__ and it does not use any hardware resources other than the pin for the LED indicator.

####Dividing Time
Time is divided in fractions, here called _xsecs_. The default time base is _20 xsecs = 1 second_. The library keeps time in _xsecs, secs, mins, hrs, days, weeks_.

It is possible to change the number of fractions corresponding to one second, which we call the _time base_. The time base for the library is applicable to all __TimeBlocks__.

<<<<<<< HEAD
![alt text](https://github.com/PM490/ArduinoTimeSplit/blob/d2ee10ae2f241f0e9f3c6ebfc2b10ca1ef2edc2d/Images/TimeSplit.png"Diagram")
=======
![alt text]("http://github.com/PM490/ArduinoTimeSplit/Images/TimeSplit.png"Diagram")
>>>>>>> origin/master
