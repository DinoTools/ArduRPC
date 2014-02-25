/**
 * Control your Adafruit NeoPixel or a WS2812 LED-Strip with ArduRPC
 *
 * Requirements:
 * - Adafruit_NeoPixel - https://github.com/adafruit/Adafruit_NeoPixel
 * - ArduRPC
 * - ArduRPC_Adafruit_NeoPixel - https://github.com/DinoTools/ArduRPC-handlers 
 */
#include <Adafruit_NeoPixel.h>

#include <ArduRPC.h>
#include <ArduRPC_Adafruit_NeoPixel.h>

#define RPC_NUM_HANDLERS 3
#define RPC_NUM_FUNCTIONS 0

// the serial port, it's also possible to use software serial
#define RPC_SERIAL_PORT Serial
#define RPC_SERIAL_BAUD 9600

ArduRPC rpc = ArduRPC(RPC_NUM_HANDLERS, RPC_NUM_FUNCTIONS);
ArduRPCSerial rpc_serial = ArduRPCSerial(RPC_SERIAL_PORT, rpc);

#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  uint8_t handler_id;

  RPC_SERIAL_PORT.begin(RPC_SERIAL_BAUD);

  strip.begin();
  strip.show();

  handler_id = rpc.connectHandler(get_ArduRPC_Adafruit_NeoPixel_Wrapper(strip));
  rpc.setHandlerName(handler_id, "neopixel");
}

void loop() {
  rpc_serial.readData();
}
