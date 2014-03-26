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

#include "ArduRPCEthernet.h"

/**
 * The constructor.
 *
 * @param udp: The UDP object
 * @param rpc: Specify the rpc handler to use
 *
 */
ArduRPCEthernetUDP::ArduRPCEthernetUDP(EthernetUDP &udp, ArduRPC &rpc)
{
  this->_udp = &udp;
  this->_rpc = &rpc;
}

/**
 * Replace the main loop.
 *
 * Can be used if the main loop() only calls readData().
 *
 * This function will never return.
 */
void ArduRPCEthernetUDP::loop()
{
  while(1)
    this->readData();
}

/**
 * Parse a packet and process the data.
 */
void ArduRPCEthernetUDP::readData()
{
  uint8_t length;
  int packet_size;
  rpc_data_t *data = this->_rpc->getRawData();
  rpc_result_t *result = this->_rpc->getRawResult();

  packet_size = this->_udp->parsePacket();
  this->_rpc->reset();
  if (packet_size <= 0)
    return;

#ifdef RPC_DEBUG
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    RPC_DEBUG_PRINT(this->_udp->remoteIP()[thisByte], DEC);
    RPC_DEBUG_PRINT(".");
  }
#endif

  RPC_DEBUG_PRINT("Packet size: ");
  RPC_DEBUG_PRINTLN(packet_size);
  this->_udp->read(data->data, RPC_MAX_DATA_LENGTH);
  data->length = packet_size;
    
  RPC_DEBUG_PRINTLN("Process data");
  this->_rpc->process();

  length = this->_rpc->getResultLength();
  if (length == 0)
    return;

  RPC_DEBUG_PRINTLN("Begin response");
  RPC_DEBUG_PRINT("Data length: ");
  RPC_DEBUG_PRINTLN(length);
  this->_udp->beginPacket(this->_udp->remoteIP(), this->_udp->remotePort());
  this->_udp->write(result->data, length);
  this->_udp->endPacket();
  RPC_DEBUG_PRINTLN("End response");
}
