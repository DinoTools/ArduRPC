/**
 * Arduino Remote Procedure Calls - ArduRPC
 * Copyright (C) 2013-2015 DinoTools
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
ArduRPCRequest_Serial::ArduRPCRequest_Serial(ArduRPCRequest &rpc, Stream &serial) : ArduRPCRequestConnection()
{
  this->rpc = &rpc;
  this->rpc->handler = (void *) this;
  this->_serial = &serial;
  this->_state = 0;
  this->timeout = 5000;
}

void ArduRPCRequest_Serial::reset()
{
  this->error = 0;
  this->_serial->flush();
}


/**
 */
void ArduRPCRequest_Serial::send(rpc_data_t request)
{
  uint8_t i;
  uint8_t len;
  uint8_t *data;

  len = request.length;
  if (len == 0) {
    return;
  }

  this->_serial->print(":");
  data = request.data;
  for (i = 0; i < len; i++) {
    if (data[0] < 0x10)
      this->_serial->print('0');
    this->_serial->print(data[0], HEX);
    data++;
  }
  this->_serial->print('\n');
}

/**
 * Process serial data encoded in hex format.
 *
 * @param c: Character to process
 */
bool ArduRPCRequest_Serial::processDataHex(uint8_t c)
{
  if(c == '\r') {
    return false;
  }

  if(c == '\n') {
    return true;
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
    this->rpc->writeResult(this->_tmp_data);
    this->_tmp_data_part = 0;
  }
  return false;
}

/**
 */
bool ArduRPCRequest_Serial::waitResult()
{
  uint8_t c;
  bool result;
  unsigned long time_start, time_current;
  time_start = millis();
  while(1) {
    time_current = millis();
    if(time_current - time_start > this->timeout) {
      this->error = 1;
      return false;
    }
    if (this->_serial->available() < 1) {
      continue;
    }
  
    c = this->_serial->read();
  
    if (this->_state == 1) {
      result = processDataHex(c);
      if (result) {
        this->_state = 0;
        return true;
      }
    }
  
    if (this->_state == 0 && c == ':') {
      this->_state = 1;
      this->rpc->reset();
      this->_tmp_data_part = 0;
    }
  }
}

