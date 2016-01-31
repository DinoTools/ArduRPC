Changelog
=========

Version 0.5.0 (31.01.2016)
--------------------------

Lib:

* Remove debug print()
* Add uint32 and int32 result values
* Add uint32 and int32 parameter values
* Connect handler by ID
* Send RPC requests
* Change structure to comply with the Arduino 1.5 library

Docs:

* Add missing return codes


Version 0.4.0 (02.08.2014)
--------------------------

* Docs:     New handler type for temperature and humidity sensors
* Protocol: Support float types
* Feature:  Set max packet size to 256 byte
* Feature:  New ArduRPC_Ethernet lib
* Feature:  Handlers have to be a class
* Core:     Rename ArduRPCSerial to ArduRPC_Serial
* Docs:     New handler type for Arduino based boards

Version 0.3.0 (25.02.2014)
--------------------------

* Improvements to lower memory usage

  * Shared buffers

* Feature: ArduRPCSerial class
* Feature: Macros to get version information
* Feature: Functions to write result data with type identifier


Version 0.2.0 (no public release)
---------------------------------

* Protocol improvements
* Feature: Handler names
* Feature: Possibility to register functions
* Feature: None value
* Feature: Return codes


Version 0.1.0 (no public release)
---------------------------------

* Initial version
