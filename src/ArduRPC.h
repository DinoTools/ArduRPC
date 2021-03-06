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

#ifndef ARDURPC_H
#define ARDURPC_H

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif

/* Config Start */

//! Set to 1 to share memory between data and result buffer
#define RPC_SHARED_BUFFERS 1

//! Maximum number of bytes available for data or shared buffer
#define RPC_MAX_DATA_LENGTH 256

//! Maximum number of bytes available for result buffer.
/*! Only change if RPC_SHARED_BUFFERS is set to 0 */
#define RPC_MAX_RESULT_LENGTH RPC_MAX_DATA_LENGTH

//! Maximum number characters available for each handler name
/*! Keep it as small as possible and don't waste memory */
#define RPC_MAX_NAME_LENGTH 16

// Uncomment to get debug information over serial
//#define RPC_DEBUG

/* Config end */

//! Major version
#define RPC_VERSION_MAJOR 0

//! Minor version
#define RPC_VERSION_MINOR 5

//! Patch level
#define RPC_VERSION_PATCH 0

//! Version
#define RPC_VERSION (RPC_VERSION_MAJOR * 10000 \
		   + RPC_VERSION_MINOR * 100 \
		   + RPC_VERSION_PATCH)


//! Datatype identifier
#define RPC_NONE 0x00
//! Datatype identifier
#define RPC_INT8 0x01
//! Datatype identifier
#define RPC_UINT8 0x02 // char
//! Datatype identifier
#define RPC_INT16 0x03
//! Datatype identifier
#define RPC_UINT16 0x04
//! Datatype identifier
#define RPC_INT32 0x05
//! Datatype identifier
#define RPC_UINT32 0x06
//! Datatype identifier
#define RPC_INT64 0x07
//! Datatype identifier
#define RPC_UINT64 0x08
//! Datatype identifier
#define RPC_FLOAT 0x09
//! Datatype identifier
#define RPC_ARRAY 0x10
//! Datatype identifier
#define RPC_STRING 0x11
//! Datatype identifier
#define RPC_MCARRAY 0x12  // 
//! Datatype identifier
#define RPC_VARRAY 0x13  // 

//! The command has been executed successfully
#define RPC_RETURN_SUCCESS 0
//! Error in the packet data
#define RPC_RETURN_INVALID_REQUEST 122
//! Error while parsing the header
#define RPC_RETURN_INVALID_HEADER 123
//! Unable determine the requested function
#define RPC_RETURN_FUNCTION_NOT_FOUND 124
//! Unable determine the requested handler
#define RPC_RETURN_HANDLER_NOT_FOUND 125
//! Unable determine the requested command
#define RPC_RETURN_COMMAND_NOT_FOUND 126
//! An error occurred 
#define RPC_RETURN_FAILURE 127

#ifdef RPC_DEBUG
#define RPC_DEBUG_CMD(...) (__VA_ARGS__)
#define RPC_DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
#define RPC_DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
#else /* defined RPC_DEBUG */
#define RPC_DEBUG_CMD(...)
#define RPC_DEBUG_PRINT(...)
#define RPC_DEBUG_PRINTLN(...)
#endif /* defined RPC_DEBUG */


//! Type is used for the processing buffer
typedef struct {
  //! The length of the data
  uint8_t length;
  //! The data of the buffer
  uint8_t *data;
} rpc_data_t;

//! Type is used for the result buffer
typedef struct {
  //! The length of the data
  uint8_t length;
  //! The data of the buffer
  uint8_t *data;
} rpc_result_t;

//! Used to store main information about a rpc function
typedef struct {
  //! The type of the rpc function. See documentation for more information.
  uint8_t type;
  //! A pointer to the callback function.
  void *callback;
  //! A pointer to the arguments passed to the callback.
  void *arguments;
} rpc_function_t;

//! Used to store main information about a rpc handler.
typedef struct {
  //! The type of the rpc handler. See documentation for more information.
  uint16_t type;
  //! A pointer to the callback function of the handler.
  void *handler;
} rpc_handler_t;

//! Additional information about a rpc handler. But not used for rpc functions.
typedef struct {
  //! The name of the handler
  char name[RPC_MAX_NAME_LENGTH];
  //! Internal address where data section starts in config
  uint16_t config_address;
  //! Internal address where data section starts in eeprom
  uint16_t eeprom_address;
} rpc_handler_info_t;

/**
 * The main class to handle rpc on a microcontroller.
 *
 * It's possible to:
 *   - register handlers and functions
 *   - process incoming data
 *     - identify called handler and function
 *     - extract parameters
 *   - generate and return the result
 *   - query the system and get additional information
 *     - available handlers and functions
 *     - name of a handler
 */

class ArduRPC
{
  public:
    ArduRPC(uint8_t handler_count=8, uint8_t function_count=8);
    bool
      setHandlerName(uint8_t handler_id, char name[]),
      writeData(uint8_t c),
      writeResult(uint8_t c),
      writeResult(char *string, uint8_t length),
      writeResult_float(float value),
      writeResult_int8(int8_t value),
      writeResult_int16(int16_t value),
      writeResult_int32(int32_t value),
      writeResult_string(char *string, uint8_t length),
      writeResult_uint8(uint8_t value),
      writeResult_uint16(uint16_t value),
      writeResult_uint32(uint32_t value);
    uint8_t
      connectFunction(rpc_function_t function),
      connectFunction(uint8_t type, void *callback, void *arguments),
      connectHandler(rpc_handler_t handler),
      connectHandler(rpc_handler_t, uint8_t),
      connectHandler(void *handler),
      connectHandler(void *, uint8_t),
      readResult(),
      *getResultData(),
      copyData(uint8_t *src, uint8_t len),
      getRequestParamLength(),
      getResultLength(),
      getResultDataLength();
    void
      process(),
      reset(),
      setReturnCode(uint8_t code);
    // get params
    char
      getParam_char();
    float
      getParam_float();
    int8_t
      getParam_int8();
    int16_t
      getParam_int16();
    int32_t
      getParam_int32();
    uint8_t
      getParam_uint8(),
      getParam_string(char *dst, uint8_t max_length);
    uint16_t
      getParam_uint16();
    uint32_t
      getParam_uint32();
    rpc_data_t
      *getRawData();
    rpc_result_t
      *getRawResult();
  private:
    /* functions */
    uint8_t
      handleSystemCalls(uint8_t cmd_id);

    /* vars */
    rpc_handler_t
      //! List of connected rpc handlers
      *handlers;
    rpc_function_t
      //! List of connected rpc functions
      *functions;

    rpc_result_t
      //! Result buffer
      result;
    rpc_data_t
      //! Data buffer
      data;

    rpc_handler_info_t
      //! Additional information for connected handlers
      *handler_infos;

    // internal stuff
    uint8_t
      //! Current position in the data buffer while reading data
      cur_data_read_pos,
      //! Current position in the result buffer while reading data
      cur_result_read_pos,
      //! Number of connected handlers
      handler_index,
      //! Number of connected functions
      function_index,
      //! Maximum number of connected handlers
      max_handler_count,
      //! Maximum number of connected functions
      max_function_count;
};

/**
 * Prototype for all ArduRPC handlers
 */
class ArduRPCHandler
{
  public:
    ArduRPCHandler();
    uint8_t
      registerSelf(ArduRPC &rpc, char *name),
      registerSelf(ArduRPC &rpc, char *name, uint8_t),
      registerSelf(ArduRPC &rpc, char *name, void *handler);
    void
      setRPC(ArduRPC &rpc),
      setRPC(ArduRPC *rpc);
    virtual uint8_t
      call(uint8_t cmd_id) = 0;
    uint16_t
      //! Type of the handler
      type;
    ArduRPC
      //! Internal RPC object the handler has been connected to
      *_rpc;
};

/**
 * Handle serial communication.
 */
class ArduRPC_Serial
{
  public:
    ArduRPC_Serial(Stream &serial, ArduRPC &rpc);
    void loop();
    void processDataHex(uint8_t c);
    void readData();
  private:
    //! RPC handler to use
    ArduRPC *_rpc;
    //! Serial port to use
    Stream *_serial;
    //! Internal processing state
    uint8_t _state;
    //! Temporary data
    uint8_t _tmp_data;
    //! Data part for hex strings. 0 = part 1 (bits 7-4); 1 = part 2 (bits 3-0)
    uint8_t _tmp_data_part;
    void processResultHex();
};

class ArduRPCRequest
{
  public:
    ArduRPCRequest();
    bool
      call(uint8_t, uint8_t),
      setHandler(void *),
      writeRequest(uint8_t c),
      writeRequest_float(float value),
      writeRequest_int8(int8_t value),
      writeRequest_int16(int16_t value),
      writeRequest_int32(int32_t value),
      writeRequest_string(char *),
      writeRequest_uint8(uint8_t value),
      writeRequest_uint16(uint16_t value),
      writeRequest_uint32(uint32_t value),
      writeResult(uint8_t c);
    int8_t
      readResult_int8();
    int16_t
      readResult_int16();
    int32_t
      readResult_int32();
    void
      reset();
    void
      *handler;
    uint8_t
      getConnectionError(),
      getError(),
      getResultCurrentData(uint8_t **);
    uint8_t
      readResult_raw_uint8(),
      readResult_string(char *, uint8_t),
      readResult_type(uint8_t),
      readResult_uint8();
    uint16_t
      readResult_uint16();
    uint32_t
      readResult_uint32();
      //readResult();
    uint8_t
      return_code;
  private:
    rpc_result_t
      //! Result buffer
      result;
    rpc_data_t
      //! Data buffer
      request;

    // internal stuff
    uint8_t
      error,
      //! Current position in the data buffer while reading data
      cur_request_read_pos,
      //! Current position in the result buffer while reading data
      cur_result_read_pos;
};

class ArduRPCRequestHandler
{
  public:
    ArduRPCRequestHandler();
    ArduRPCRequest *_rpc;
    uint8_t _handler_id;
};

class ArduRPCRequestConnection
{
  public:
    ArduRPCRequestConnection();
    virtual void reset() = 0;
    virtual void send(rpc_data_t ) = 0;
    virtual bool waitResult() = 0;
    uint8_t
      getError();
    uint8_t error;
    ArduRPCRequest *rpc;
    unsigned long timeout;
  private:
};

/**
 * Handle serial communication.
 */
class ArduRPCRequest_Serial : public ArduRPCRequestConnection
{
  public:
    ArduRPCRequest_Serial(ArduRPCRequest &rpc, Stream &serial);
    void reset();
    void send(rpc_data_t request);
    bool waitResult();
  private:
    bool processDataHex(uint8_t c);
    //! Serial port to use
    Stream *_serial;
    //! Internal processing state
    uint8_t _state;
    //! Temporary data
    uint8_t _tmp_data;
    //! Data part for hex strings. 0 = part 1 (bits 7-4); 1 = part 2 (bits 3-0)
    uint8_t _tmp_data_part;
    //void processResultHex();
};

//! Callback function for a rpc function
typedef uint8_t (*rpc_callback_function_t)(ArduRPC *rpc, void *);
//! Callback function for a rpc handler
typedef uint8_t (*rpc_callback_handler_t)(uint8_t, ArduRPC *rpc, void *);

/**
 * Extract one byte from given data
 * @param data array
 * @return Extracted data
 */
static inline int8_t rpc_read_char(void *data)
{
  char *d = (char *)data;
  return (char)d[0];
}

/**
 * Extract one byte from given data
 * @param data array
 * @return Extracted data
 */
static inline int8_t rpc_read_int8(void *data)
{
  int8_t *d = (int8_t *)data;
  return (int8_t)d[0];
}

/**
 * Extract two bytes from given data
 * @param data array
 * @return Extracted data
 */
static inline int16_t rpc_read_int16(void *data)
{
  int16_t res;
  uint8_t *d = (uint8_t *)data;
  res = d[0];
  res = res << 8;
  res = res | d[1];
  return res;
}

/**
 * Extract four bytes from given data
 * @param data array
 * @return Extracted data
 */
static inline int32_t rpc_read_int32(void *data)
{
  int32_t res;
  uint8_t *d = (uint8_t *)data;
  res = d[0];
  res = res << 8;
  res = res | d[1];
  res = res << 8;
  res = res | d[2];
  res = res << 8;
  res = res | d[3];
  return res;
}

/**
 * Extract one byte from given data
 * @param data array
 * @return Extracted data
 */
static inline uint8_t rpc_read_uint8(void *data)
{
  uint8_t *d = (uint8_t *)data;
  return (uint8_t)d[0];
}

/**
 * Extract two byte from given data
 * @param data array
 * @return Extracted data
 */
static inline uint16_t rpc_read_uint16(void *data)
{
  uint16_t res;
  uint8_t *d = (uint8_t *)data;
  res = d[0];
  res = res << 8;
  res = res | d[1];
  return res;
}

/**
 * Extract four byte from given data
 * @param data array
 * @return Extracted data
 */
static inline uint32_t rpc_read_uint32(void *data)
{
  uint32_t res;
  uint8_t *d = (uint8_t *)data;
  res = d[0];
  res = res << 8;
  res = res | d[1];
  res = res << 8;
  res = res | d[2];
  res = res << 8;
  res = res | d[3];
  return res;
}
#endif
