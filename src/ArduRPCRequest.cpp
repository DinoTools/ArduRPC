/**
 * Arduino Remote Procedure Calls - ArduRPC
 * Copyright (C) 2013-2016 DinoTools
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
 * The constructor performs the following tasks.
 *   - Allocate memory for the handler and function list
 *
 */
ArduRPCRequest::ArduRPCRequest()
{
  this->request.data = (uint8_t *)malloc(RPC_MAX_DATA_LENGTH);
#if RPC_SHARED_BUFFERS == 1
  this->result.data = this->request.data;
#else
  this->result.data = (uint8_t *)malloc(RPC_MAX_RESULT_LENGTH);
#endif
}

bool ArduRPCRequest::call(uint8_t handler_id, uint8_t cmd_id)
{
  // protocol version
  this->request.data[0] = 0x00;
  this->request.data[1] = handler_id;
  this->request.data[2] = cmd_id;
  this->request.data[3] = this->request.length - 4;
  ArduRPCRequestConnection *h = (ArduRPCRequestConnection *)this->handler;
  h->reset();
  h->send(this->request);
  h->waitResult();
  if (this->getError() == 0) {
    this->return_code = this->readResult_raw_uint8();
  }
}

uint8_t ArduRPCRequest::getConnectionError()
{
  // ToDo: Check if handler exists
  ArduRPCRequestConnection *h = (ArduRPCRequestConnection *)this->handler;
  return h->getError();
}

uint8_t ArduRPCRequest::getError()
{
  // ToDo: return better value
  if (this->error == 0 && this->getConnectionError() > 0) {
    return 1;
  }
  return this->error;
}

uint8_t ArduRPCRequest::getResultCurrentData(uint8_t **data)
{
  *data = &this->result.data[this->cur_result_read_pos];
  return this->result.length - this->cur_result_read_pos;
}

bool ArduRPCRequest::setHandler(void *handler)
{
  this->handler = handler;
}

uint8_t ArduRPCRequest::readResult_raw_uint8()
{
  if(this->error > 0) {
    return 0;
  }
  uint8_t res = rpc_read_uint8(&this->result.data[this->cur_result_read_pos]);
  this->cur_result_read_pos++;
  return res;
}

uint8_t ArduRPCRequest::readResult_type(uint8_t type)
{
  uint8_t res = this->readResult_raw_uint8();
  if (type == res) {
    return type;
  }
  this->error = 0x02;
  return 0xff;
}

int8_t ArduRPCRequest::readResult_int8()
{
  this->readResult_type(RPC_INT8);
  int8_t res = this->readResult_raw_uint8();
  return res;
}

int16_t ArduRPCRequest::readResult_int16()
{
  this->readResult_type(RPC_INT16);
  int16_t res = this->readResult_raw_uint8();
  res = res << 8;
  res = res | this->readResult_raw_uint8();
  return res;
}

int32_t ArduRPCRequest::readResult_int32()
{
  this->readResult_type(RPC_INT32);
  int32_t res = this->readResult_raw_uint8();
  res = res << 8;
  res = res | this->readResult_raw_uint8();
  res = res << 8;
  res = res | this->readResult_raw_uint8();
  res = res << 8;
  res = res | this->readResult_raw_uint8();
  return res;
}

uint8_t ArduRPCRequest::readResult_string(char *dst, uint8_t max_length)
{
  this->readResult_type(RPC_STRING);
  uint8_t length;
  uint8_t n;
  n = length = this->readResult_raw_uint8();
  if(n > max_length) {
    n = max_length;
  }

  memcpy(dst, &this->result.data[this->cur_result_read_pos], n);
  if(n < max_length) {
    dst[n] = '\0';
  }
  this->cur_result_read_pos += length;
  return n;
}

uint8_t ArduRPCRequest::readResult_uint8()
{
  this->readResult_type(RPC_UINT8);
  uint8_t res = this->readResult_raw_uint8();
  return res;
}

uint16_t ArduRPCRequest::readResult_uint16()
{
  this->readResult_type(RPC_UINT16);
  uint16_t res = this->readResult_raw_uint8();
  res = res << 8;
  res = res | this->readResult_raw_uint8();
  return res;
}

uint32_t ArduRPCRequest::readResult_uint32()
{
  this->readResult_type(RPC_UINT32);
  uint32_t res = this->readResult_raw_uint8();
  res = res << 8;
  res = res | this->readResult_raw_uint8();
  res = res << 8;
  res = res | this->readResult_raw_uint8();
  res = res << 8;
  res = res | this->readResult_raw_uint8();
  return res;
}

void ArduRPCRequest::reset() {
  this->result.length = 0;
  this->request.length = 4;
  this->cur_request_read_pos = 0;
  this->cur_result_read_pos = 0;
  this->error = 0;
  ArduRPCRequestConnection *h = (ArduRPCRequestConnection *)this->handler;
  h->reset();
}

/**
 * Write a byte into the result buffer.
 * @param c The byte to write.
 */
bool ArduRPCRequest::writeRequest(uint8_t c)
{
  this->request.data[this->request.length] = c;
  this->request.length++;
}

/**
 * Write a value of type FLOAT
 * @param value The value to write.
 */
bool ArduRPCRequest::writeRequest_float(float value)
{
  uint8_t *v = (uint8_t *)&value;
  this->writeRequest(v[3]);
  this->writeRequest(v[2]);
  this->writeRequest(v[1]);
  this->writeRequest(v[0]);
}

/**
 * Write a value of type INT8
 * @param value The value to write.
 */
bool ArduRPCRequest::writeRequest_int8(int8_t value)
{
  this->writeRequest(value);
}

/**
 * Write a value of type INT16
 * @param value The value to write.
 */
bool ArduRPCRequest::writeRequest_int16(int16_t value)
{
  this->writeRequest((((value) >> 8) & 0xff));
  this->writeRequest(((value) & 0xff));
}

/**
 * Write a value of type INT32
 * @param value The value to write.
 */
bool ArduRPCRequest::writeRequest_int32(int32_t value)
{
  this->writeRequest((((value) >> 24) & 0xff));
  this->writeRequest((((value) >> 16) & 0xff));
  this->writeRequest((((value) >> 8) & 0xff));
  this->writeRequest(((value) & 0xff));
}

/**
 * Write a value of type string
 * @param value The value to write.
 */
bool ArduRPCRequest::writeRequest_string(char *s)
{
  uint8_t length;
  length = strlen(s);
  this->writeRequest_uint8(length);
  memcpy(&this->request.data[this->request.length], s, length);
  this->request.length += length;
}

/**
 * Write a value of type UINT8
 * @param value The value to write.
 */
bool ArduRPCRequest::writeRequest_uint8(uint8_t value)
{
  this->writeRequest(value);
}

/**
 * Write a value of type UINT16
 * @param value The value to write.
 */
bool ArduRPCRequest::writeRequest_uint16(uint16_t value)
{
  this->writeRequest((((value) >> 8) & 0xff));
  this->writeRequest(((value) & 0xff));
}

/**
 * Write a value of type UINT32
 * @param value The value to write.
 */
bool ArduRPCRequest::writeRequest_uint32(uint32_t value)
{
  this->writeRequest((((value) >> 24) & 0xff));
  this->writeRequest((((value) >> 16) & 0xff));
  this->writeRequest((((value) >> 8) & 0xff));
  this->writeRequest(((value) & 0xff));
}

bool ArduRPCRequest::writeResult(uint8_t c)
{
  this->result.data[this->result.length] = c;
  this->result.length++;
}

/**
 * Constructor of the ArudRPCRequestHandler class.
 */
ArduRPCRequestHandler::ArduRPCRequestHandler()
{
}

/**
 * Constructor of the ArudRPCRequestConnection class.
 */
ArduRPCRequestConnection::ArduRPCRequestConnection()
{
}

uint8_t ArduRPCRequestConnection::getError()
{
  return this->error;
}
