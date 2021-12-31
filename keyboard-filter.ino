/*
Copyright 2021 m4mmon

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "ArduinoKeyboard.h"
#include "MyKeyboard.h"
#include "MyUSB.h"

// -----------------------------------------------------------------------------

MyUSB usb;
MyKeyboard keyb(&usb);

// -----------------------------------------------------------------------------

void setup()
{
  // wait for serial to be ready

  Serial.begin(9600);
  uint8_t nb = 0;
  while (!Serial && nb++ < 30)
  {
    delay(100);
  }

  Serial.print("--> setup(tried #");
  Serial.print(nb, DEC);
  Serial.println(" times for serial)");

  // init USB

  if (usb.Init() == -1)
  {
    Serial.println("USB host shield did not start.");
  }

  // start arduino keyboard

  begin();

  Serial.println("<-- setup()");
}

// -----------------------------------------------------------------------------

void loop()
{
  usb.Task();
  keyb.Task();
}

// -----------------------------------------------------------------------------
