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

Initialize all components
~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: c
    :linenos:

    #include <Adafruit_NeoPixel.h>
    #include <ArduRPC.h>
    #include <ArduRPC_Adafruit_NeoPixel.h>

    Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(14, 3, NEO_GRB + NEO_KHZ800);
    Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(14, 4, NEO_GRB + NEO_KHZ800);

    ArduRPC rpc = ArduRPC(2, 0);
    ArduRPCSerial rpc_serial = ArduRPCSerial(Serial, rpc);

**Line 1:**
    Include the Adafruit_NeoPixel library.

**Line 2:**
    Include the ArduRPC base library.

**Line 3:**
    Include the ArduRPC handler for the Adafruit_NeoPixel.

**Line 5:**
    Initialize the first strip.

**Line 6:**
    Initialize the second strip.

**Line 8:**
    Initialize the RPC manager with a limit of 2 handlers and 0 functions.

**Line 9:**
    Initialize the RPC data processor for a serial communication.


Connect the Handlers
~~~~~~~~~~~~~~~~~~~~

.. code-block:: c
    :linenos:

    void setup() {
      uint8_t handler_id;

      strip1.begin();
      strip1.show();

      handler_id = rpc.connectHandler(rpc_get_neopixel(&strip));
      rpc.setHandlerName(handler_id, "neopixel1");

      strip2.begin();
      strip2.show();

      handler_id = rpc.connectHandler(rpc_get_neopixel(&strip2));
      rpc.setHandlerName(handler_id, "neopixel2");
    }


**Line 2:**
    Define a variable to hold a handler ID.

**Line 4 and 5:**
    Initialize first strip.

**Line 7:**
    The rpc_get_neopixel() Function is provided by the handler and can be used to create the required data structures to connect the handler.
    The new handler can be registered by using the connectHandler() function. The ID of the connected handler is returned.

**Line 8:**
    Setting a name for a registered handler is optional but might help to identify the handler on the client site.

**Line 10 to 14:**
    Register the handler for the second strip.


Handle the incoming commands
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: c
    :linenos:

    void loop() {
      rpc_serial.loop();
    }

**Line 2:**
    Run the AruRPC processing loop.


Examples
--------

Some examples are already included in the main ArduRPC library. They can be used to get started.

.. **rfm12_node:**
    If you use a RFM12 module in combination with a microcontroller. This is an example that can be used as node. It uses the `RFM12 library <https://github.com/LowPowerLab/RFM12B>`_ provided by LowPowerLab.

.. **rfm12_gateway:**
    The gateway for the rfm12_node. It can be controller over a serial connection and submits the data and commands to a node.

