// Copyright (C) 2009-2012 Gaz Davidson
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_IRR_DEVICE_CONSOLE_H_INCLUDED__
#define __C_IRR_DEVICE_CONSOLE_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_CONSOLE_DEVICE_
//#define _IRR_USE_CONSOLE_FONT_
//#include "SIrrCreationParameters.h"
//#include "CIrrDeviceStub.h"
//#include "IImagePresenter.h"
// for console font
//#include "IGUIFont.h"
#ifdef _IRR_WINDOWS_API_
#define WIN32_LEAN_AND_MEAN
#if !defined(_IRR_XBOX_PLATFORM_)
//#include <windows.h>
#endif
#if(_WIN32_WINNT >= 0x0500)
#define _IRR_WINDOWS_NT_CONSOLE_
#endif
#else
//#include <time.h>
#endif
// for now we assume all other terminal types are VT100
#ifndef _IRR_WINDOWS_NT_CONSOLE_
#define _IRR_VT100_CONSOLE_
#endif
class CIrrDeviceConsole : public CIrrDeviceStub, IImagePresenter
{
public:
  //! constructor
  CIrrDeviceConsole(const SIrrlichtCreationParameters& params);
  //! destructor
  ~CIrrDeviceConsole();
  //! runs the device. Returns false if device wants to be deleted
  bool run();
  //! Cause the device to temporarily pause execution and let other processes to run
  // This should bring down processor usage without major performance loss for Irrlicht
  void yield();
  //! Pause execution and let other processes to run for a specified amount of time.
  void sleep(u32 timeMs, bool pauseTimer);
  //! sets the caption of the window
  void setWindowCaption(const char* text);
  //! returns if window is active. if not, nothing need to be drawn
  bool isWindowActive() const;
  //! returns if window has focus
  bool isWindowFocused() const;
  //! returns if window is minimized
  bool isWindowMinimized() const;
  //! presents a surface in the client area
  bool present(IImage* surface, void* windowId = 0, IRECT* src = 0);
  //! notifies the device that it should close itself
  void closeDevice();
  //! Sets if the window should be resizable in windowed mode.
  void setResizable(bool resize = false);
  //! Minimizes the window.
  void minimizeWindow();
  //! Maximizes the window.
  void maximizeWindow();
  //! Restores the window size.
  void restoreWindow();
  //! Get the device type
  E_DEVICE_TYPE getType() const {
    return EIDT_CONSOLE;
  }
  void addPostPresentText(s16 X, s16 Y, const char* text);
  //! Implementation of the win32 console mouse cursor
  class CCursorControl : public ICursorControl
  {
  public:
    CCursorControl(const dimension2du& wsize)
      : WindowSize(wsize), InvWindowSize(0.0f, 0.0f), IsVisible(true), UseReferenceRect(false) {
      if (WindowSize.Width != 0) {
        InvWindowSize.Width = 1.0f / WindowSize.Width;
      }
      if (WindowSize.Height != 0) {
        InvWindowSize.Height = 1.0f / WindowSize.Height;
      }
    }
    //! Changes the visible state of the mouse cursor.
    void setVisible(bool visible) {
      if (visible != IsVisible) {
        IsVisible = visible;
        setPosition(CursorPos.X, CursorPos.Y);
      }
    }
    //! Returns if the cursor is currently visible.
    bool isVisible() const {
      return IsVisible;
    }
    //! Sets the new position of the cursor.
    void setPosition(const position2df& pos) {
      setPosition(pos.X, pos.Y);
    }
    //! Sets the new position of the cursor.
    void setPosition(f32 x, f32 y) {
      if (!UseReferenceRect) {
        setPosition((s32)(x * WindowSize.Width), (s32)(y * WindowSize.Height));
      }
      else {
        setPosition((s32)(x * ReferenceRect.getWidth()), (s32)(y * ReferenceRect.getHeight()));
      }
    }
    //! Sets the new position of the cursor.
    void setPosition(const position2di& pos) {
      setPosition(pos.X, pos.Y);
    }
    //! Sets the new position of the cursor.
    void setPosition(s32 x, s32 y) {
      setInternalCursorPosition(position2di(x, y));
    }
    //! Returns the current position of the mouse cursor.
    const position2di& getPosition() {
      return CursorPos;
    }
    //! Returns the current position of the mouse cursor.
    position2df getRelativePosition() {
      if (!UseReferenceRect) {
        return position2df(CursorPos.X * InvWindowSize.Width,
            CursorPos.Y * InvWindowSize.Height);
      }
      return position2df(CursorPos.X / (f32)ReferenceRect.getWidth(),
          CursorPos.Y / (f32)ReferenceRect.getHeight());
    }
    //! Sets an absolute reference rect for calculating the cursor position.
    void setReferenceRect(IRECT* rect = 0) {
      if (rect) {
        ReferenceRect = *rect;
        UseReferenceRect = true;
        // prevent division through zero and uneven sizes
        if (!ReferenceRect.getHeight() || ReferenceRect.getHeight() % 2) {
          ReferenceRect.LowerRightCorner.Y += 1;
        }
        if (!ReferenceRect.getWidth() || ReferenceRect.getWidth() % 2) {
          ReferenceRect.LowerRightCorner.X += 1;
        }
      }
      else {
        UseReferenceRect = false;
      }
    }
    //! Updates the internal cursor position
    void setInternalCursorPosition(const position2di& pos) {
      CursorPos = pos;
      if (UseReferenceRect) {
        CursorPos -= ReferenceRect.UpperLeftCorner;
      }
    }
  private:
    position2di  CursorPos;
    dimension2du WindowSize;
    dimension2df InvWindowSize;
    bool                   IsVisible,
                           UseReferenceRect;
    IRECT        ReferenceRect;
  };
private:
  //! Set the position of the text caret
  void setTextCursorPos(s16 x, s16 y);
  // text to be added after drawing the screen
  struct SPostPresentText {
    position2d<s16> Pos;
    char*         Text;
  };
  bool IsWindowFocused;
  strv_t OutputBuffer;
  IGUIFont*  ConsoleFont;
  SPostPresentText > Text;
  FILE* OutFile;
#ifdef _IRR_WINDOWS_NT_CONSOLE_
  HANDLE WindowsSTDIn, WindowsSTDOut;
  u32 MouseButtonStates;
#endif
};
#ifdef _IRR_USE_CONSOLE_FONT_
class CGUIConsoleFont : public IGUIFont
{
public:
  CGUIConsoleFont(CIrrDeviceConsole* device) : Device(device) { }
  //! Draws some text and clips it to the specified rectangle if wanted.
  void draw(const char* text, const IRECT& position,
      SColor color, bool hcenter = false, bool vcenter = false,
      const IRECT* clip = 0) {
    IRECT Area = clip ? *clip : position;
    if (Area.UpperLeftCorner.X < 0) {
      Area.UpperLeftCorner.X = 0;
    }
    if (Area.UpperLeftCorner.Y < 0) {
      Area.UpperLeftCorner.Y = 0;
    }
    position2d<s16> pos;
    // centre vertically
    pos.Y = vcenter ? (position.UpperLeftCorner.Y + position.LowerRightCorner.Y) / 2 : position.UpperLeftCorner.Y;
    // nothing to display?
    if (pos.Y < Area.UpperLeftCorner.Y || pos.Y > Area.LowerRightCorner.Y) {
      return;
    }
    tempText = text;
    // centre horizontally
    pos.X = hcenter ? position.getCenter().X - (tempText_size / 2) : position.UpperLeftCorner.X;
    // clip
    u32 xlclip = 0,
        xrclip = 0;
    // get right clip
    if (pos.X + (s32)tempText_size > Area.LowerRightCorner.X) {
      xrclip = Area.LowerRightCorner.X - pos.X;
    }
    // get left clip
    if (pos.X < Area.UpperLeftCorner.X) {
      xlclip = Area.UpperLeftCorner.X - pos.X;
    }
    // totally clipped?
    if ((s32)tempText_size - xlclip - xrclip < 0) {
      return;
    }
    // null terminate the string
    if (xrclip > 0) {
      tempText[xrclip] = '\0';
    }
    Device->addPostPresentText(pos.X + xlclip, pos.Y, &(tempText[xlclip]));
  }
  //! Calculates the dimension of some text.
  dimension2du getDimension(const char* text) const {
    return dimension2du(wcslen(text), 1);
  }
  //! Calculates the index of the character in the text which is on a specific position.
  s32 getCharacterFromPos(const char* text, s32 pixel_x) const {
    return pixel_x;
  };
  //! No kerning
  void setKerningWidth(s32 kerning) { }
  void setKerningHeight(s32 kerning) { }
  s32 getKerningWidth(const char* thisLetter = 0, const char* previousLetter = 0) const {
    return 0;
  }
  s32 getKerningHeight() const  {
    return 0;
  }
  void setInvisibleCharacters(const char* s) { }
  // I guess this is an OS specific font
  EGUI_FONT_TYPE getType() const {
    return EGFT_OS;
  }
private:
  CIrrDeviceConsole* Device;
  char* tempText;
};
#endif // _IRR_USE_CONSOLE_FONT_
#endif // _IRR_COMPILE_WITH_CONSOLE_DEVICE_
#endif // __C_IRR_DEVICE_CONSOLE_H_INCLUDED__

