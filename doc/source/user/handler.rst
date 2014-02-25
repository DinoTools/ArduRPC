Handler
=======

A handler provides one or more callable functions. Before the functions are accessible over the remote procedure call interface the handler must be connected to the system. A handler can be identified by the handler type.

Handler types
-------------

A handler type belongs always to a group with a base type. Functions defined in the base type must be available in all subtypes. This ensures that a client with support for a base type can at least access the basic functionality of a handler even if that specific subtype isn't supported.

To register a new handler type feel free to open a new request by using the `issue tracker <https://github.com/DinoTools/ardurpc/issues>`_ on github.

+--------+--------+------+-------------------------------------------------------------------------------+
| Start  | End    | Mask | Name                                                                          |
+========+========+======+===============================================================================+
| 0x0100 | 0x01FF |    8 | :ref:`Base Pixel Strip`                                                       |
+--------+--------+------+-------------------------------------------------------------------------------+
| 0x0180 | 0x01FF |    9 | :ref:`Extended Pixel Strip`                                                   |
+--------+--------+------+-------------------------------------------------------------------------------+
| 0x0181          |      | :doc:`Adafruit_NeoPixel </handler/ArduRPC_Adafruit_NeoPixel/doc/index>`       |
+--------+--------+------+-------------------------------------------------------------------------------+
| 0x0200 | 0x02FF |    8 | :ref:`Base Matrix`                                                            |
+--------+--------+------+-------------------------------------------------------------------------------+
| 0x0280 | 0x02FF |    9 | :ref:`Extended Matrix`                                                        |
+--------+--------+------+-------------------------------------------------------------------------------+
| 0x0281          |      | :ref:`Colorduino_GFX`                                                         |
+--------+--------+------+-------------------------------------------------------------------------------+
| 0xFF00 | 0xFFFF |    8 | :ref:`Custom handlers`                                                        |
+--------+--------+------+-------------------------------------------------------------------------------+

Base types
----------

.. _base pixel strip:

Base Pixel Strip
~~~~~~~~~~~~~~~~

+------+----------------------------------------------+
| ID   | Function                                     |
+======+==============================================+
| 0x01 | :cpp:func:`base_pixel_strip::getColorCount`  |
+------+----------------------------------------------+
| 0x02 | :cpp:func:`base_pixel_strip::getPixelCount`  |
+------+----------------------------------------------+
| 0x11 | :cpp:func:`base_pixel_strip::setPixelColor`  |
+------+----------------------------------------------+
| 0x12 | :cpp:func:`base_pixel_strip::setRangeColor`  |
+------+----------------------------------------------+

.. cpp:function:: uint8_t base_led_strip::getColorCount()

    Get the number of colors. Return value should be 1, 2 or 3.

    :return: Number of colors.

.. cpp:function:: uint16_t base_led_strip::getPixelCount()

    Get the number of available pixels.

    :return: Number of pixels

.. cpp:function:: void base_led_strip::setPixelColor(uint16_t n, uint8_t color1, uint8_t color2, uint8_t color3)

    Set the color of a pixel. All color values MUST be given and spare colors will be ignored by the device.

    :param n: The number of the LED. Range from 0 to pixel_count - 1
    :param color1: First color. Red if color_count = 3.
    :param color2: Second color. Green if color_count = 3.
    :param color3: Third color. Blue if color_count = 3.

.. cpp:function:: void base_led_strip::setRangeColor(uint16_t start, uint16_t end, uint8_t color1, uint8_t color2, uint8_t color3)

    Set the color of a range of pixels.

    :param start: The position to start. Range from 0 to pixel_count - 1
    :param end: The position to stop. Range from start to pixel_count - 1
    :param color1: First color. Red if color_count = 3.
    :param color2: Second color. Green if color_count = 3.
    :param color3: Third color. Blue if color_count = 3.


.. _extended pixel strip:

Extended Pixel Strip
~~~~~~~~~~~~~~~~~~~~

.. _Base Matrix:
.. _Extended Matrix:

Base/Extended Matrix
~~~~~~~~~~~~~~~~~~~~

The Base Matrix handler is inspired by the Adafruit_GFX library and it is intended to be used with libraries based on Adafruit_GFX.
But it might also be possible to wrap any other library.

+------+-----------------------------------------+-----------------+
| ID   | Function                                | Matrix Type     |
+------+-----------------------------------------+------+----------+
|      |                                         | Base | Extended |
+======+=========================================+======+==========+
| 0x01 | :cpp:func:`matrix_gfx::getColorCount`   | X    | X        |
+------+-----------------------------------------+------+----------+
| 0x02 | :cpp:func:`matrix_gfx::getWidth`        | X    | X        |
+------+-----------------------------------------+------+----------+
| 0x03 | :cpp:func:`matrix_gfx::getHeight`       | X    | X        |
+------+-----------------------------------------+------+----------+
| 0x10 | :cpp:func:`matrix_gfx::drawPixel`       | X    | X        |
+------+-----------------------------------------+------+----------+
| 0x20 | :cpp:func:`matrix_gfx::drawLine`        | X    | X        |
+------+-----------------------------------------+------+----------+
| 0x21 | :cpp:func:`matrix_gfx::drawFastVLine`   |      | X        |
+------+-----------------------------------------+------+----------+
| 0x22 | :cpp:func:`matrix_gfx::drawFastHLine`   |      | X        |
+------+-----------------------------------------+------+----------+
| 0x23 | :cpp:func:`matrix_gfx::drawRect`        |      | X        |
+------+-----------------------------------------+------+----------+
| 0x24 | :cpp:func:`matrix_gfx::fillRect`        |      | X        |
+------+-----------------------------------------+------+----------+
| 0x25 | :cpp:func:`matrix_gfx::fillScreen`      | X    | X        |
+------+-----------------------------------------+------+----------+
| 0x26 | :cpp:func:`matrix_gfx::invertDisplay`   |      | X        |
+------+-----------------------------------------+------+----------+
| 0x30 | :cpp:func:`matrix_gfx::drawCircle`      |      | X        |
+------+-----------------------------------------+------+----------+
| 0x31 | :cpp:func:`matrix_gfx::fillCircle`      |      | X        |
+------+-----------------------------------------+------+----------+
| 0x32 | :cpp:func:`matrix_gfx::drawTriangle`    |      | X        |
+------+-----------------------------------------+------+----------+
| 0x33 | :cpp:func:`matrix_gfx::fillTriangle`    |      | X        |
+------+-----------------------------------------+------+----------+
| 0x34 | :cpp:func:`matrix_gfx::drawRoundRect`   |      | X        |
+------+-----------------------------------------+------+----------+
| 0x35 | :cpp:func:`matrix_gfx::fillRoundRect`   |      | X        |
+------+-----------------------------------------+------+----------+
| 0x40 | :cpp:func:`matrix_gfx::drawChar`        |      | X        |
+------+-----------------------------------------+------+----------+
| 0x41 | :cpp:func:`matrix_gfx::setCursor`       |      | X        |
+------+-----------------------------------------+------+----------+
| 0x42 | :cpp:func:`matrix_gfx::setTextColor`    |      | X        |
+------+-----------------------------------------+------+----------+
| 0x43 | :cpp:func:`matrix_gfx::setTextColor`    |      | X        |
+------+-----------------------------------------+------+----------+
| 0x44 | :cpp:func:`matrix_gfx::setTextSize`     |      | X        |
+------+-----------------------------------------+------+----------+
| 0x45 | :cpp:func:`matrix_gfx::setTextWrap`     |      | X        |
+------+-----------------------------------------+------+----------+
| 0x46 | :cpp:func:`matrix_gfx::write`           |      | X        |
+------+-----------------------------------------+------+----------+
| 0x50 | :cpp:func:`matrix_gfx::setRotation`     |      | X        |
+------+-----------------------------------------+------+----------+
| 0x51 | :cpp:func:`matrix_gfx::drawBitmap`      |      | X        |
+------+-----------------------------------------+------+----------+


.. cpp:function:: uint8_t matrix_gfx::getColorCount()

    Get the number of colors. Return value should be 1, 2 or 3.

    :return: Number of colors.

.. cpp:function:: uint16_t matrix_gfx::getWidth()

    Get width in pixels.

    :return: Number of pixels

.. cpp:function:: uint16_t matrix_gfx::getHeight()

    Get height in pixels.

    :return: Number of pixels

.. cpp:function:: void matrix_gfx::drawPixel(int16_t x, int16_t y, uint8_t color1, uint8_t color2, uint8_t color3)

    Draw a pixel.

    :param x: Pixel x position
    :param y: Pixel y position
    :param color1: First color. Red if color_count = 3.
    :param color2: Second color. Green if color_count = 3.
    :param color3: Third color. Blue if color_count = 3.

.. cpp:function:: void matrix_gfx::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color1, uint8_t color2, uint8_t color3)

    Draw a line.

.. cpp:function:: void matrix_gfx::drawFastVLine(int16_t x, int16_t y, int16_t h, uint8_t color1, uint8_t color2, uint8_t color3)

    Draw a vertical line.

.. cpp:function:: void matrix_gfx::drawFastHLine(int16_t x, int16_t y, int16_t w, uint8_t color1, uint8_t color2, uint8_t color3)

    Draw a horizontal line.

.. cpp:function:: void matrix_gfx::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color1, uint8_t color2, uint8_t color3)

    Draw the boarder of rectangle.

.. cpp:function:: void matrix_gfx::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color1, uint8_t color2, uint8_t color3)

    Draw a filled rectangle.

.. cpp:function:: void matrix_gfx::fillScreen(uint8_t color1, uint8_t color2, uint8_t color3)

    Fill the screen with the given color.

.. cpp:function:: void matrix_gfx::invertDisplay(boolean i)

    Invert the display.

.. cpp:function:: void matrix_gfx::drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color1, uint8_t color2, uint8_t color3)

    Draw the border of a circle.

.. cpp:function:: void matrix_gfx::fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color1, uint8_t color2, uint8_t color3)

    Draw a filled circle.

.. cpp:function:: void matrix_gfx::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color1, uint8_t color2, uint8_t color3)

    Draw the boarder of a triangle.

.. cpp:function:: void matrix_gfx::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color1, uint8_t color2, uint8_t color3)

    Draw a filled triangle.

.. cpp:function:: void matrix_gfx::drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint8_t color1, uint8_t color2, uint8_t color3)

    Draw the boarder of a round rectangle.

.. cpp:function:: void matrix_gfx::fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint8_t color1, uint8_t color2, uint8_t color3)

    Draw a filled round rectangle.

.. cpp:function:: void matrix_gfx::drawChar(int16_t x, int16_t y, unsigned char c, uint8_t color1, uint8_t color2, uint8_t color3, uint16_t bg, uint8_t size)

    Draw a character.

.. cpp:function:: void matrix_gfx::setCursor(int16_t x, int16_t y)

    Set the cursor position.

.. cpp:function:: void matrix_gfx::setTextColor(uint8_t color1, uint8_t color2, uint8_t color3)

    Set the text color.

.. cpp:function:: void matrix_gfx::setTextColor(uint8_t color1, uint8_t color2, uint8_t color3, uint8_t bg_red, uint8_t bg_green, uint8_t bg_blue)

    Set the text color.

.. cpp:function:: void matrix_gfx::setTextSize(uint8_t s)

    Set the text size.

.. cpp:function:: void matrix_gfx::setTextWrap(boolean w)

    Set the text wrap.

.. cpp:function:: void matrix_gfx::setRotation(uint8_t r)

    Set the rotation.

.. cpp:function:: void matrix_gfx::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t color1, uint8_t color2, uint8_t color3)

    Draw a Bitmap.


.. _Colorduino_GFX:

Colorduino_GFX
~~~~~~~~~~~~~~

This handler extends :ref:`Extended Matrix`.

+------+---------------------------------+
| ID   | Function                        |
+======+=================================+
| 0xA0 | :cpp:func:`swapBuffers`         |
+------+---------------------------------+
| 0xA1 | :cpp:func:`setAutoSwapBuffers`  |
+------+---------------------------------+

.. cpp:function:: uint8_t swapBuffers(uint8_t copy)

    :param copy: 0 = False | 1 = True

    Swap buffers and copy new front buffer into the back buffer.

.. cpp:function:: uint8_t setAutoSwapBuffers(uint8_t auto_swap)

    :param auto_swap: 0 = False | 1 = True

    Set option to swap buffers after each command.


.. _Custom handlers:

Custom handlers
~~~~~~~~~~~~~~~

This range of IDs is reserved for custom handlers e.g. for testing or prototyping purposes.
