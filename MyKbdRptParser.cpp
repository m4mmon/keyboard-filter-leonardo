/*
Copyright 2021 m4mmon

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "MyKbdRptParser.h"

// -----------------------------------------------------------------------------

MyKbdRptParser::MyKbdRptParser():
  KeyboardReportParser(),
  _nConsumerKeySent(0)
{
  kbdLockingKeys.bLeds = -1;
}

// -----------------------------------------------------------------------------

void MyKbdRptParser::Parse(USBHID * ipHid, bool bIsRptId, uint8_t inLen, uint8_t * ipData)
{
  Serial.print("## KEYBOARD");
  for (int idx = 0; idx < inLen; idx++)
  {
    Serial.print(" ");
    PrintHex<uint8_t>(ipData[idx], 0x80);
  }
  Serial.println();

  // finish Consumer transaction if any

  SendConsumerReleaseReportToComputer();

  // On error - return
  if (ipData[2] == 1)
  {
    Serial.println("buffer error\r\n");
    return;
  }

  if (inLen != 8)
  {
    Serial.println("unhandled length");
    return;
  }

  // if no manipulation occurred

  if (!HandleSpecialSingleKeyPressed(ipHid, inLen, ipData))
  {
    // send report to computer

    SendKeyboardReportToComputer(ipData, inLen);
  }
}

// -----------------------------------------------------------------------------

void MyKbdRptParser::BlinkLedCompose(USBHID* ipHid)
{
  uint8_t nLeds = kbdLockingKeys.bLeds;
  SetKeyboardLeds(ipHid, kbdLockingKeys.bLeds | 0x08);
  delay(25);
  SetKeyboardLeds(ipHid, kbdLockingKeys.bLeds);
}

// -----------------------------------------------------------------------------

int MyKbdRptParser::HandleSpecialSingleKeyPressed(USBHID * ipHid, uint8_t inLen, uint8_t * ipData)
{
  int nHandled = 0;
  uint8_t nCode = 0;
  int nIdx = 0;

  // to avoid having to manage multiple reports to send etc., just perform
  // handling if a single (non-modifier) key is pressed.
  
  if (IsSingleKeyPressed(inLen, ipData, nCode, nIdx))
  {
    // assume we handle it

    nHandled = 1;

    switch (nCode)
    {
      case 0x66: // Keyboard Power
        SendLock();
        break;
      case 0x74: // Open
        SendExec();
        break;
      case 0x75: // Help
        SendConsumerReportToComputer((IsShift(ipData[0])) ? CONSUMER_BROWSER_BACK : CONSUMER_BROWSER_HOME);
        break;
      case 0x76: // Props
        SendProps();
        break;
      case 0x77: // Front
        SendMinMax();
        break;
      case 0x78: // Stop
        SendTaskMgr();
        break;
      case 0x79: // Again
        SendRedo();
        break;
      case 0x7a: // Undo
        SendUndo();
        break;
      case 0x7b: // Cut
        SendCut();
        break;
      case 0x7c: // Copy
        SendCopy();
        break;
      case 0x7d: // Paste
        SendPaste();
        break;
      case 0x7e: // Find
        SendFind();
        break;
      case 0x7f: // Keyboard Mute
        SendConsumerReportToComputer(MEDIA_VOLUME_MUTE);
        break;
      case 0x80: // Keyboard Volume Up / shift Brightness Up
        SendConsumerReportToComputer((IsShift(ipData[0])) ? CONSUMER_BRIGHTNESS_UP : MEDIA_VOLUME_UP);
        break;
      case 0x81: // Keyboard Volume Down / shift Brightness Down
        SendConsumerReportToComputer((IsShift(ipData[0])) ? CONSUMER_BRIGHTNESS_DOWN : MEDIA_VOLUME_DOWN);
        break;
      default:
        // oops, not handled
        nHandled = 0;
    }
  }

  // blink "compose" led if we did something

  if (nHandled)
  {
    BlinkLedCompose(ipHid);
  }

  return nHandled;
}

// -----------------------------------------------------------------------------

int MyKbdRptParser::IsSingleKeyPressed(uint8_t inLen, uint8_t * ipData, uint8_t & onCode, int & onIdx)
{
  onCode = 0;
  onIdx = 0;
  int isSingle = 0;

  // if buffer has correct length and first key code does not indicate error
  // has been checked in ::Parse

  // for each key "slot"

  for (int idx = 2; idx <= 7; idx++)
  {
    // read code

    uint8_t nCode = ipData[idx];

    // some data

    if (nCode != 0)
    {
      // another key code was previously detected

      if (isSingle != 0)
      {
        // not single then

        isSingle = 0;

        // forget previously found key info

        onCode = 0;
        onIdx = 0;

        // and don't bother looping again

        break;
      }

      // let's be optimistic

      else
      {
        isSingle = 1;
        onCode = nCode;
        onIdx = idx;
      }
    }
  }

  return isSingle;
}

// -----------------------------------------------------------------------------
