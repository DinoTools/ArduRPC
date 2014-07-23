Getting started
===============

Setup
-----

First of all download the base library from the `ArduRPC repository <https://github.com/DinoTools/ArduRPC>`_ and extract the ArduRPC directory into your Arduino library path.

After that you can download additional handlers from the `ArduRPC handler repository <https://github.com/DinoTools/ArduRPC-handlers>`_ or from any other source. Copy the handler files to your Arduino library path. Make sure you have downloaded and installed all dependencies before using a handler.

**Example:** Adafruit_NeoPixel

1. Download `base library <https://github.com/DinoTools/ArduRPC>`_

  * Extract the files
  * Copy the ArduRPC directory to your Arduino library path

2. Download the `ArduRPC handler repository <https://github.com/DinoTools/ArduRPC-handlers>`_

  * Extract the files
  * Copy the ArduRPC_Adafruit_NeoPixel directory to your Arduino library path

3. Download the `Adafruit_NeoPixel library <https://github.com/adafruit/Adafruit_NeoPixel>`_

  * Extract the files
  * Copy the Adafruit_NeoPixel directory to you Arduino library path.

Usage
-----

The easiest way to get started is to use one of the available examples and to modify only the required parts.

To use the Adafruit_NeoPixel library with ArduRPC follow the examples below.

.. code-block:: c
    :linenos:

    #include <Adafruit_NeoPixel.h>
    #include <ArduRPC.h>
    #include <ArduRPC_Adafruit_NeoPixel.h>

    Adafruit_NeoPixel strip = Adafruit_NeoPixel(14, 6, NEO_GRB + NEO_KHZ800);

    ArduRPC rpc = ArduRPC(2, 0);
    ArduRPC_Serial rpc_serial = ArduRPC_Serial(Serial, rpc);

    ArduRPC_Adafruit_Neopixel Strip_Wrapper(rpc, "strip", strip);

    void setup() {
      Serial.begin(9600);

      strip.begin();
      strip.show();
    }

    void loop() {
      rpc_serial.loop();
    }

**Line 1:**
    Include the Adafruit_NeoPixel library.

**Line 2:**
    Include the ArduRPC base library.

**Line 3:**
    Include the ArduRPC handler for the Adafruit_NeoPixel.

**Line 5:**
    Initialize the NeoPixel strip.

**Line 7:**
    Initialize the RPC manager with a limit of 2 handlers and 0 functions.

**Line 8:**
    Initialize the RPC data processor for a serial communication.

**Line 10:**
    Initialize the handler for the strip and name it 'strip'.

**Line 13:**
    Initialize the serial port and set baud to 9600.

**Line 15 and 16:**
    Setup the strips.

**Line 18:**
    Run the ArduRPC processing loop.

Additional examples
-------------------

At least one example should be included with every library/handler.


.. **rfm12_node:**
    If you use a RFM12 module in combination with a microcontroller. This is an example that can be used as node. It uses the `RFM12 library <https://github.com/LowPowerLab/RFM12B>`_ provided by LowPowerLab.

.. **rfm12_gateway:**
    The gateway for the rfm12_node. It can be controller over a serial connection and submits the data and commands to a node.

