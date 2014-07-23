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
 * The constructor performs the following tasks.
 *   - Allocate memory for the handler and function list
 *   - Reset all counters
 *   - Set rpc information
 *
 * The number of handlers and functions must not exceed the given number.
 * It's not possible to increase the limits.
 *
 * @param handler_count Maximum number of handlers
 * @param function_count Maximum number of functions
 */
ArduRPC::ArduRPC(uint8_t handler_count, uint8_t function_count)
{
  this->handlers = (rpc_handler_t *)malloc(sizeof(rpc_handler_t) * handler_count);
  this->functions = (rpc_function_t *)malloc(sizeof(rpc_function_t) * function_count);
  this->handler_infos = (rpc_handler_info_t *)malloc(sizeof(rpc_handler_info_t) * handler_count);
  this->handler_index = 0;
  this->function_index = 0;
  this->max_handler_count = handler_count;
  this->max_function_count = function_count;
  this->data.data = (uint8_t *)malloc(RPC_MAX_DATA_LENGTH);
#if RPC_SHARED_BUFFERS == 1
  this->result.data = this->data.data;
#else
  this->result.data = (uint8_t *)malloc(RPC_MAX_RESULT_LENGTH);
#endif
}

/**
 * Connect a function to the RPC processor.
 * @param function All information to call the function.
 * @return The internal function index
 */
uint8_t ArduRPC::connectFunction(rpc_function_t function)
{
  if(this->function_index >= this->max_function_count - 1)
    return 0xff;
  this->functions[this->function_index] = function;
  return this->function_index++;
}

/**
 * Use the given parameters to create the required data structures and
 * connect the function to the RPC processor.
 * @param type The type of the function. Have a look at the type list in the documentation.
 * @param callback The callback function to call
 * @param arguments A pointer to the arguments. It is passed to the callback function
 * @return The internal function index
 */
uint8_t ArduRPC::connectFunction(uint8_t type, void *callback, void *arguments)
{
  rpc_function_t function = {type, callback, arguments};
  return connectFunction(function);
}

/**
 * Connect a handler to the RPC processor.
 * @param handler All information to call the handler.
 * @return The internal handler index.
 */
uint8_t ArduRPC::connectHandler(rpc_handler_t handler)
{
  if(this->handler_index >= this->max_handler_count - 1)
    return 0xff;
  handlers[this->handler_index] = handler;
  return this->handler_index++;
}

/**
 * Connect a handler to the RPC processor.
 * @param handler Pointer to the handler class.
 * @return The internal handler index.
 */
uint8_t ArduRPC::connectHandler(void *handler)
{
  // Type cast
  ArduRPCHandler *h = (ArduRPCHandler *)handler;
  h->setRPC(this);
  rpc_handler_t handler_data = {h->type, handler};
  return connectHandler(handler_data);
}

/**
 * Copy external data into the internal processing buffer
 * @param src A pointer to the data to copy.
 * @param len Number of bytes to copy
 * @return 0 on success.
 */
uint8_t ArduRPC::copyData(uint8_t *src, uint8_t len)
{
  this->data.length = len;
  memcpy(this->data.data, src, len);
  return 0;
}

/**
 * Read a character from the current position in the parameter data and return it.
 * @return A character from the parameter data.
 */
char ArduRPC::getParam_char()
{
  char res = rpc_read_int8(&this->data.data[this->cur_data_read_pos]);
  this->cur_data_read_pos++;
  return res;
}

/**
 * Read a float value at the current position in the parameter data and return it.
 *
 * @return A float value.
 */
float ArduRPC::getParam_float()
{
  float result;
  uint8_t *v = (uint8_t *)&result;

  v[3] = this->getParam_uint8();
  v[2] = this->getParam_uint8();
  v[1] = this->getParam_uint8();
  v[0] = this->getParam_uint8();

  return result;
}

/**
 * Read a byte from the current position in the parameter data and return it.
 */
int8_t ArduRPC::getParam_int8()
{
  int8_t res = rpc_read_int8(&this->data.data[this->cur_data_read_pos]);
  this->cur_data_read_pos++;
  return res;
}

/**
 * Read two bytes from the current position in the parameter data and return it.
 */
int16_t ArduRPC::getParam_int16()
{
  int16_t res;
  res = rpc_read_int16(&this->data.data[this->cur_data_read_pos]);
  this->cur_data_read_pos += 2;
  return res;
}

/**
 * Read a string from the current position in the parameter data.
 * @param dst Pointer to the destination.
 * @param max_length The maximum number of bytes to copy.
 * @return The number of copyed bytes.
 */
uint8_t ArduRPC::getParam_string(char *dst, uint8_t max_length)
{
  uint8_t length;
  uint8_t n;
  n = length = this->getParam_uint8();
  if(n > max_length) {
    n = max_length;
  }
  memcpy(dst, &this->data.data[this->cur_data_read_pos], n);
  this->cur_data_read_pos += length;
  return n;
}

/**
 * Read a byte from the current position in the parameter data and return it.
 */
uint8_t ArduRPC::getParam_uint8()
{
  uint8_t res = rpc_read_uint8(&this->data.data[this->cur_data_read_pos]);
  this->cur_data_read_pos++;
  return res;
}

/**
 * Read two bytes from the current position in the parameter data and return it.
 */
uint16_t ArduRPC::getParam_uint16()
{
  uint16_t res;
  res = rpc_read_uint16(&this->data.data[this->cur_data_read_pos]);
  this->cur_data_read_pos += 2;
  return res;
}

/**
 * Return pointer to raw data structure.
 *
 * @return Pointer to raw data.
 */
rpc_data_t *ArduRPC::getRawData()
{
  return &this->data;
}

/**
 * Return pointer to raw result data.
 *
 * @return Pointer to raw result data.
 */
rpc_result_t *ArduRPC::getRawResult()
{
  return &this->result;
}

/**
 * Return a pointer to the result data
 * @return A pointer to the result data
 */
uint8_t *ArduRPC::getResultData()
{
  return (uint8_t *)this->result.data;
}

/**
 * Get the length of the result data including the header.
 * @return The number of bytes of the result data
 */
uint8_t ArduRPC::getResultLength()
{
  return this->result.length + 1;
}

/**
 * Get the length of the result data.
 * @return The number of bytes of the result data
 */
uint8_t ArduRPC::getResultDataLength()
{
  return this->result.length;
}

/**
 * Handle all system calls.
 * @param cmd_id The ID of the called command.
 * @return The result of the command.
 */
uint8_t ArduRPC::handleSystemCalls(uint8_t cmd_id)
{
  uint8_t i;
  uint16_t handler_type;
  uint8_t handler_id;

  if (cmd_id == 0x01) {
    // get protocol version
    this->writeResult(RPC_UINT8);
    this->writeResult(0);
    return RPC_RETURN_SUCCESS;
  } else if (cmd_id == 0x02) {
    // get library version
    this->writeResult(RPC_ARRAY);
    this->writeResult(RPC_UINT8);
    this->writeResult(3);
    this->writeResult(RPC_VERSION_MAJOR);
    this->writeResult(RPC_VERSION_MINOR);
    this->writeResult(RPC_VERSION_PATCH);
    return RPC_RETURN_SUCCESS;
  } else if (cmd_id == 0x03) {
    this->writeResult_uint16(RPC_MAX_DATA_LENGTH);
    return RPC_RETURN_SUCCESS;
  } else if (cmd_id == 0x10) {
    this->writeResult(RPC_MCARRAY);
    this->writeResult(2);
    this->writeResult(RPC_UINT8);
    this->writeResult(RPC_UINT8);
    this->writeResult(function_index);
    for (i = 0; i < function_index; i++) {
      this->writeResult(i);
      this->writeResult(this->functions[i].type);
    }
    return RPC_RETURN_SUCCESS;
  } else if (cmd_id == 0x20) {
    this->writeResult(RPC_MCARRAY);
    this->writeResult(2);
    this->writeResult(RPC_UINT8);
    this->writeResult(RPC_UINT16);
    this->writeResult(handler_index);
    for (i = 0; i < handler_index; i++) {
      handler_type = this->handlers[i].type;
      this->writeResult(i);
      this->writeResult((((handler_type) >> 8) & 0xff));
      this->writeResult(((handler_type) & 0xff));
    }
    return RPC_RETURN_SUCCESS;
  } else if (cmd_id == 0x21) {
    handler_id = this->getParam_int8();
    if (handler_id < this->max_handler_count) {
      this->writeResult(RPC_STRING);
      this->writeResult(RPC_MAX_NAME_LENGTH);
      this->writeResult(this->handler_infos[handler_id].name, RPC_MAX_NAME_LENGTH);
    }
    return RPC_RETURN_SUCCESS;
  }
  return RPC_RETURN_COMMAND_NOT_FOUND;
}

/**
 * Process the data in the internal processing buffer.
 *   -# Check if the protocol version is supported.
 *   -# Extract the handler ID.
 *   -# Extract the command ID.
 *   -# Extract the length of the parameter data.
 *   -# Call the requested handler and function.
 */
void ArduRPC::process()
{
  uint8_t raw_data_length;

  // reset result
  this->result.length = 0;

  raw_data_length = this->data.length;

  // check for min packet size
  if (raw_data_length < 4) {
    this->setReturnCode(RPC_RETURN_INVALID_HEADER);
    this->writeResult(RPC_NONE);
    return;
  }

  // protocol version
  if (this->getParam_uint8() != 0x00) {
    return;
  }

  uint8_t handler_id = this->getParam_uint8();
  uint8_t command_id = this->getParam_uint8();
  uint8_t length = this->getParam_uint8();
  uint8_t res = RPC_RETURN_FAILURE;

  if (length != raw_data_length - 4) {
    this->setReturnCode(RPC_RETURN_INVALID_REQUEST);
    this->writeResult(RPC_NONE);
    return;
  }

  if(handler_id < this->handler_index) {
    rpc_handler_t *handler;
    handler = &handlers[handler_id];

    ArduRPCHandler *h = (ArduRPCHandler *)handler->handler;
    res = h->call(command_id);
  } else if (handler_id == 0xfe) {
    if (command_id >= this->function_index) {
      res = RPC_RETURN_FUNCTION_NOT_FOUND;
    } else {
      rpc_function_t *function;
      function = &functions[handler_id];
      rpc_callback_function_t callback_function = (rpc_callback_function_t)function->callback;
      res = callback_function(this, function->arguments);
    }
  } else if (handler_id == 0xff) {
    res = this->handleSystemCalls(command_id);
  } else {
    res = RPC_RETURN_HANDLER_NOT_FOUND;
  }

  this->setReturnCode(res);
  if (this->getResultDataLength() == 0) {
    this->writeResult(RPC_NONE);
  }
}

/**
 * Read and return the next byte from the result buffer.
 * @return The next byte from result buffer.
 */
uint8_t ArduRPC::readResult()
{
  return this->result.data[this->cur_result_read_pos++];
}

/**
 * Reset the internal processing and result buffer.
 * It does *not* remove connected handlers or functions.
 */
void ArduRPC::reset() {
  this->result.length = 0;
  this->data.length = 0;
  this->cur_data_read_pos = 0;
  this->cur_result_read_pos = 0;
}

/**
 * Set the name of a handler.
 * The length of the name must not exeed the number of bytes specified in RPC_MAX_NAME_LENGTH.
 * A name must only be used once.
 * @see RPC_MAX_NAME_LENGTH
 * @see connectHandler()
 * @param handler_id The internal handler ID returned while the handler was connected.
 * @param name The name of the handler.
 */
bool ArduRPC::setHandlerName(uint8_t handler_id, char name[])
{
  if(handler_id < this->max_handler_count) { 
    strncpy(this->handler_infos[handler_id].name, name, RPC_MAX_NAME_LENGTH);
  }
}

/**
 * Set the return code.
 * @param code The return code
 */
void ArduRPC::setReturnCode(uint8_t code)
{
  this->result.data[0] = code;
}

/**
 * Write a byte into the data buffer.
 * @param c The byte to write.
 */
bool ArduRPC::writeData(uint8_t c)
{
  this->data.data[this->data.length] = c;
  this->data.length++;
}

/**
 * Write a byte into the result buffer.
 * @param c The byte to write.
 */
bool ArduRPC::writeResult(uint8_t c)
{
  this->result.length++;
  this->result.data[this->result.length] = c;
}

/**
 * Write a string into the result buffer.
 * @param string A pointer to the string.
 * @param length The length of the string to copy.
 */
bool ArduRPC::writeResult(char *string, uint8_t length)
{
  memcpy(&this->result.data[this->result.length + 1], string, RPC_MAX_NAME_LENGTH);
  this->result.length = this->result.length + length;
}

/**
 * Write a value of type FLOAT
 * @param value The value to write.
 */
bool ArduRPC::writeResult_float(float value)
{
  uint8_t *v = (uint8_t *)&value;
  this->writeResult(RPC_FLOAT);
  this->writeResult(v[3]);
  this->writeResult(v[2]);
  this->writeResult(v[1]);
  this->writeResult(v[0]);
}

/**
 * Write a value of type INT8
 * @param value The value to write.
 */
bool ArduRPC::writeResult_int8(int8_t value)
{
  this->writeResult(RPC_INT8);
  this->writeResult(value);
}

/**
 * Write a value of type INT16
 * @param value The value to write.
 */
bool ArduRPC::writeResult_int16(int16_t value)
{
  this->writeResult(RPC_INT16);
  this->writeResult((((value) >> 8) & 0xff));
  this->writeResult(((value) & 0xff));
}

/**
 * Write a value of type STRING
 * @param value The value to write.
 * @param length The string length.
 */
bool ArduRPC::writeResult_string(char *value, uint8_t length)
{
  this->writeResult(RPC_STRING);
  this->writeResult(length);
  this->writeResult(value, length);
}

/**
 * Write a value of type UINT8
 * @param value The value to write.
 */
bool ArduRPC::writeResult_uint8(uint8_t value)
{
  this->writeResult(RPC_UINT8);
  this->writeResult(value);
}

/**
 * Write a value of type UINT16
 * @param value The value to write.
 */
bool ArduRPC::writeResult_uint16(uint16_t value)
{
  this->writeResult(RPC_UINT16);
  this->writeResult((((value) >> 8) & 0xff));
  this->writeResult(((value) & 0xff));
}

/**
 * Constructor of the ArudRPCHandler class.
 */
ArduRPCHandler::ArduRPCHandler()
{
  this->_rpc = NULL;
  this->type = 0;
}

/**
 * Internal function to register the handler.
 *
 * This function must be called by the handler to connect itself with ArduRPC.
 *
 * @param rpc: The AarduRPC object
 * @param name: The name of the handler
 * @param handler: Pointer to the handler.
 */
void ArduRPCHandler::registerSelf(ArduRPC &rpc, char *name, void *handler)
{
  uint8_t handler_id;

  ArduRPC *r = &rpc;
  handler_id = r->connectHandler(handler);
  r->setHandlerName(handler_id, name);
}

/**
 * The the RPC object for the handler
 *
 * @param rpc: RPC object
 */
void ArduRPCHandler::setRPC(ArduRPC &rpc)
{
  this->_rpc = &rpc;
}

/**
 * The the RPC object for the handler
 *
 * @param rpc: Pointer to the RPC object
 */
void ArduRPCHandler::setRPC(ArduRPC *rpc)
{
  this->_rpc = rpc;
}
