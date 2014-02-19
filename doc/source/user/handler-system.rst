Buildin system handler
======================

The system handler is available by default and is included in the base ArduRPC library.

Function overview
-----------------

+------+------------------------------+
| ID   | Function                     |
+======+==============================+
| 0x01 | :c:func:`getProtocolVersion` |
+------+------------------------------+
| 0x02 | :c:func:`getLibraryVersion`  |
+------+------------------------------+
| 0x03 | :c:func:`getMaxPacketSize`   |
+------+------------------------------+
| 0x10 | :c:func:`getFunctionList`    |
+------+------------------------------+
| 0x20 | :c:func:`getHandlerList`     |
+------+------------------------------+
| 0x21 | :c:func:`getHandlerName`     |
+------+------------------------------+


Function details
----------------

.. c:function:: uint8_t getProtocolVersion()

    Get the protocol version. At the moment this should be 0.

.. c:function:: RPC_ARRAY getLibraryVersion()

    Return the library version as RPC_ARRAY with three elements of uint8 type.

    1. Major version
    2. Minor version
    3. Patch level

.. c:function:: uint16_t getMaxPacketSize()

    Return the maximum packet size (header + data) in bytes.

.. c:function:: RPC_VARRAY getFunctionList()

    Get a list of all functions. The result has 2 columns.

    1. The first column is a unsigned char and represents the internal ID.
    2. The second column is a unsigned char and represents the type of the function.

.. c:function:: RPC_VARRAY getHandlerList()

    Get a list of all handlers. The result has 2 columns.

    1. The first column is a unsigned char and represents the internal ID of the handler.
    2. The second column is an unsigned short and represents the type of the handler.

.. c:function:: RPC_CARRAY getHandlerName(uint8_t handler_id)

    Get the handler name by a given ID.

