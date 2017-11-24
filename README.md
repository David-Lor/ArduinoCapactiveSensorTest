# ArduinoCapactiveSensorTest

An Arduino sketch for trying several aluminum foil capacitive switches using the [CapacitiveSensor](https://playground.arduino.cc/Main/CapacitiveSensor?from=Main.CapSense) library, featuring:

* Define multiple switches in array
* Change the detection threshold via Serial
* Turn on a LED when any switch is pressed on
* Debouncing for each separate switch
* One counter per switch, which increases +1 on keydown
