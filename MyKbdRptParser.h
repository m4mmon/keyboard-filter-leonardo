/*
  Copyright 2021, 2024 m4mmon

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef _MyKbdRptParser_h__
#define _MyKbdRptParser_h__
#include <arduino.h>
#include <HID-Project.h>
#include <HID-Settings.h>

#include <hidboot.h>

#define KBD_INPUT_TRACE 1

// -----------------------------------------------------------------------------

const uint8_t _pnEndSeqData[]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t _pnExecData[]    = {0x08, 0x00, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00}; // win+r
const uint8_t _pnCutData[]     = {0x01, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00}; // ctrl-x
const uint8_t _pnCopyData[]    = {0x01, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00}; // ctrl-c
const uint8_t _pnPasteData[]   = {0x01, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00}; // ctrl-v
const uint8_t _pnFindData[]    = {0x01, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00}; // ctrl-f
const uint8_t _pnUndoData[]    = {0x01, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00}; // ctrl-z
const uint8_t _pnRedoData[]    = {0x01, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00}; // ctrl-y
const uint8_t _pnMinMaxData[]  = {0x08, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00}; // win+d
const uint8_t _pnPropsData[]   = {0x08, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00}; // win+i
const uint8_t _pnLockData[]    = {0x08, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00}; // win+l
const uint8_t _pnTaskMgrData[] = {0x03, 0x00, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00}; // ctrl+shift+esc
const uint8_t _pnCtxMenuData[] = {0x00, 0x00, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00}; // context menu
const uint8_t _pnRCtlData[]    = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // right control

// -----------------------------------------------------------------------------

#define IsLControl(code)((code) & 0x01)
#define IsRControl(code)((code) & 0x10)
#define IsControl(code) ((code) & 0x11)
#define IsLShift(code)  ((code) & 0x02)
#define IsRShift(code)  ((code) & 0x20)
#define IsShift(code)   ((code) & 0x22)
#define IsLAlt(code)    ((code) & 0x04)
#define IsAlt IsLAlt
#define IsRAlt(code)    ((code) & 0x40)
#define IsAltGR IsRAlt
#define IsLGUI(code)    ((code) & 0x08)
#define IsRGUI(code)    ((code) & 0x80)
#define IsGUI(code)     ((code) & 0x88)

#define SendExec()        SendKeyboardReportToComputer(_pnExecData,    8)
#define SendCut()         SendKeyboardReportToComputer(_pnCutData,     8)
#define SendCopy()        SendKeyboardReportToComputer(_pnCopyData,    8)
#define SendPaste()       SendKeyboardReportToComputer(_pnPasteData,   8)
#define SendFind()        SendKeyboardReportToComputer(_pnFindData,    8)
#define SendUndo()        SendKeyboardReportToComputer(_pnUndoData,    8)
#define SendRedo()        SendKeyboardReportToComputer(_pnRedoData,    8)
#define SendMinMax()      SendKeyboardReportToComputer(_pnMinMaxData,  8)
#define SendProps()       SendKeyboardReportToComputer(_pnPropsData,   8)
#define SendLock()        SendKeyboardReportToComputer(_pnLockData,    8)
#define SendTaskMgr()     SendKeyboardReportToComputer(_pnTaskMgrData, 8)
#define SendContextMenu() SendKeyboardReportToComputer(_pnCtxMenuData, 8)
#define SendRControl()    SendKeyboardReportToComputer(_pnRCtlData,    8)

// -----------------------------------------------------------------------------

class MyKbdRptParser : public KeyboardReportParser
{
  public:
    MyKbdRptParser();

    // physical keyboard report handling.

    void Parse(USBHID * ipHid, bool bIsRptId, uint8_t inLen, uint8_t * ipData);

    // sets leds
    
    uint8_t InitLedStates(USBHID* ipHid, uint8_t inLeds);

  private:

    inline void StartErrorAnim()
    {
      _nErrLEDAnimation = 1;
      _nAnimStartTime = millis();
    }

    inline void EndErrorAnim()
    {
      _nErrLEDAnimation = 0;
    }

    inline void StartComposeAnim()
    {
      _nComposeBlink = 1;
      _nAnimStartTime = millis();
    }

    inline void EndComposeAnim()
    {
      _nComposeBlink = 0;
    }

    // blinks "compose" led.

    void BlinkLedCompose(USBHID* ipHid);

    // blinks all leds.

    void BlinkAllLeds(USBHID* ipHid);

    // handles "special" keys on Sun type 6/7 keyboards
    // returns 1 if something has been handled, else 0.

    int HandleSpecialSingleKeyPressed(USBHID * ipHid, uint8_t inLen, uint8_t * ipData);

    // checks if report coming from keyboard contains a single code, and if it is the case,
    // provide the code and its position in report data.

    int IsSingleKeyPressed(uint8_t inLen, uint8_t * ipData, uint8_t & onCode, int & onIdx);

    // set leds on plugged physical keyboard

    inline uint8_t SetKeyboardLeds(USBHID* ipHid, uint8_t inLeds)
    {
      uint8_t rc = 0;
      if (ipHid)
      {
        rc = ipHid->SetReport(0, 0, 2, 0, 1, &inLeds);
      }
      return rc;
    }

    // send keyboard report data to computer
    inline void SendKeyboardReportToComputer(uint8_t * ipData, uint8_t inLen)
    {
      HID().SendReport(HID_REPORTID_KEYBOARD, ipData, inLen);
    }

    // send consumer report data to computer.
    // we don't use ::write because it means that a key release report is sent.
    // we want the key to appear pressed as long as it is not released or another key
    // pressed (we will then send a releaseAll()).

    inline void SendConsumerReportToComputer(ConsumerKeycode iKey)
    {
      // send a report with the key

      Consumer.press(iKey);

      // remember we sent something

      _nConsumerKeySent = 1;
    }

    // send consumer key release report event to computer (all keys in fact)
    // but we don't care since we only send reports with single keys.

    inline void SendConsumerReleaseReportToComputer()
    {
      // if a key press report was sent

      if (_nConsumerKeySent)
      {
        // send a release report

        Consumer.releaseAll();

        // forget

        _nConsumerKeySent = 0;
      }
    }

    // flag indicating a consumer report with a key has been sent

    uint8_t _nConsumerKeySent;

    unsigned long _nAnimStartTime;
    uint8_t _nErrLEDAnimation;
    uint8_t _nComposeBlink;
};

// -----------------------------------------------------------------------------

#endif
