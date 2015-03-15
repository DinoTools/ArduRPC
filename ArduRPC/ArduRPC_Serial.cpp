/**
 * Arduino Remote Procedure Calls - ArduRPC
 * Copyright (C) 2013-2014 DinoTools
 *
 * This file is part of ArduRPC.
 *
 * ArduRPC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * ArduRPC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ArduRPC.h"

/**
 * The constructor.
 *
 * @param serial: Specify the serial port to use
 * @param rpc: Specify the rpc handler to use
 *
 */
ArduRPC_Serial::ArduRPC_Serial(Stream &serial, ArduRPC &rpc)
{
  this->_serial = &serial;
  this->_rpc = &rpc;
  this->_state = 0;
}

/**
 * Replace the main loop.
 *
 * Can be used if the main loop() only calls readData().
 *
 * This function will never return.
 */
void ArduRPC_Serial::loop()
{
  while(1) {
    readData();
  }
}

/**
 * Process serial data encoded in hex format.
 *
 * @param c: Character to process
 */
void ArduRPC_Serial::processDataHex(uint8_t c)
{
  if(c == '\r') {
    return;
  }

  if(c == '\n') {
    this->_rpc->process();
    this->processResultHex();
    this->_state = 0;
    return;
  }

  if(c >= 97) {
    c -= 87;
  } else if(c >= 65) {
    c -= 55;
  } else {
    c -= 48;
  }

  if(this->_tmp_data_part == 0) {
    c = c << 4;
    this->_tmp_data = c;
    this->_tmp_data_part = 1;
  } else {
    this->_tmp_data |= c;
    this->_rpc->writeData(this->_tmp_data);
    this->_tmp_data_part = 0;
  }
}

/**
 * Process the result and write it to the serial port using the HEX encoding.
 */
void ArduRPC_Serial::processResultHex()
{
    uint8_t i;
    uint8_t len;
    uint8_t *data;

    len = this->_rpc->getResultLength();
    if (len == 0) {
      return;
    }

    this->_serial->println(len);
    this->_serial->print(":");
    data = this->_rpc->getResultData();
    for (i = 0; i < len; i++) {
      if (data[0] < 0x10)
        this->_serial->print('0');
      this->_serial->print(data[0], HEX);
      data++;
    }
    this->_serial->print('\n');
}

/**
 * Read and process data from the serial port specified
 */
void ArduRPC_Serial::readData()
{
  uint8_t c;

  if (this->_serial->available() < 1) {
    return;
  }

  c = this->_serial->read();

  if (this->_state == 1) {
    processDataHex(c);
  }

  if (this->_state == 0 && c == ':') {
    this->_state = 1;
    this->_rpc->reset();
    this->_tmp_data_part = 0;
  }
}
