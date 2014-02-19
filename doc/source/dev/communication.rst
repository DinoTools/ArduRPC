Communication
=============

Serial (Hex-Mode)
-----------------

* Use a serial port like UART (Arduino: Serial or SoftwareSerial)
* Read/Write data line by line
* Every package/line must start with a colon (':')
* Lines without a colon must be ignored
* Data is encoded as hex string

**Example:**

Line 1:
    The Request
Line 2:
    A comment or debug information
Line 3:
    The response

.. code-block:: text
    :linenos:

    :000302050110030001
    This is a comment or debug info
    :0110
