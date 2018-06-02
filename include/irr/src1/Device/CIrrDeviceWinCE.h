// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_IRR_DEVICE_WINCE_H_INCLUDED__
#define __C_IRR_DEVICE_WINCE_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_WINDOWS_CE_DEVICE_
//#include "CIrrDeviceStub.h"
//#include "IrrlichtDevice.h"
//#include "IImagePresenter.h"
//#include <windows.h>
//#include <windowsx.h>
//#include <commctrl.h>
//#include <aygshell.h>
//#include <sipapi.h>
//#include <gx.h>
class CIrrDeviceWinCE : public CIrrDeviceStub, IImagePresenter
{
public:
  //! constructor
  CIrrDeviceWinCE(const SIrrlichtCreationParameters& params);
  //! destructor
  ~CIrrDeviceWinCE();
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
  //! \return Returns a pointer to a list with all video modes
  //! supported by the gfx adapter.
  IVideoModeList* getVideoModeList();
  //! Notifies the device, that it has been resized
  void OnResized();
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
    return EIDT_WINCE;
  }
  //! Implementation of the win32 cursor control
  class CCursorControl : public ICursorControl
  {
  public:
    CCursorControl(const dimension2du& wsize, HWND hwnd, bool fullscreen)
      : WindowSize(wsize), InvWindowSize(0.0f, 0.0f),
        HWnd(hwnd), BorderX(0), BorderY(0),
        UseReferenceRect(false), IsVisible(true) {
      if (WindowSize.Width != 0) {
        InvWindowSize.Width = 1.0f / WindowSize.Width;
      }
      if (WindowSize.Height != 0) {
        InvWindowSize.Height = 1.0f / WindowSize.Height;
      }
      if (!fullscreen) {
        BorderX = GetSystemMetrics(SM_CXDLGFRAME);
        BorderY = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYDLGFRAME);
      }
    }
    //! Changes the visible state of the mouse cursor.
    void setVisible(bool visible) {
      IsVisible = visible;
    }
    //! Returns if the cursor is currently visible.
    bool isVisible() const {
      _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
      return IsVisible;
    }
    //! Sets the new position of the cursor.
    void setPosition(const position2df& pos) {
      setPosition(pos.X, pos.Y);
    }
    //! Sets the new position of the cursor.
    void setPosition(f32 x, f32 y) {
      if (!UseReferenceRect) {
        setPosition(round32(x * WindowSize.Width), round32(y * WindowSize.Height));
      }
      else {
        setPosition(round32(x * ReferenceRect.getWidth()), round32(y * ReferenceRect.getHeight()));
      }
    }
    //! Sets the new position of the cursor.
    void setPosition(const position2di& pos) {
      setPosition(pos.X, pos.Y);
    }
    //! Sets the new position of the cursor.
    void setPosition(s32 x, s32 y) {
      RECT rect;
      if (UseReferenceRect) {
        SetCursorPos(ReferenceRect.UpperLeftCorner.X + x,
            ReferenceRect.UpperLeftCorner.Y + y);
      }
      else {
        if (GetWindowRect(HWnd, &rect)) {
          SetCursorPos(x + rect.left + BorderX, y + rect.top + BorderY);
        }
      }
      CursorPos.X = x;
      CursorPos.Y = y;
    }
    //! Returns the current position of the mouse cursor.
    const position2di& getPosition() {
      updateInternalCursorPosition();
      return CursorPos;
    }
    //! Returns the current position of the mouse cursor.
    position2df getRelativePosition() {
      updateInternalCursorPosition();
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
    /** Used to notify the cursor that the window was resized. */
    void OnResize(const dimension2du& size) {
      WindowSize = size;
      if (size.Width != 0) {
        InvWindowSize.Width = 1.0f / size.Width;
      }
      else {
        InvWindowSize.Width = 0.f;
      }
      if (size.Height != 0) {
        InvWindowSize.Height = 1.0f / size.Height;
      }
      else {
        InvWindowSize.Height = 0.f;
      }
    }
  private:
    //! Updates the internal cursor position
    void updateInternalCursorPosition() {
      POINT p;
      if (!GetCursorPos(&p)) {
        DWORD xy = GetMessagePos();
        p.x = GET_X_LPARAM(xy);
        p.y = GET_Y_LPARAM(xy);
      }
      if (UseReferenceRect) {
        CursorPos.X = p.x - ReferenceRect.UpperLeftCorner.X;
        CursorPos.Y = p.y - ReferenceRect.UpperLeftCorner.Y;
      }
      else {
        RECT rect;
        if (GetWindowRect(HWnd, &rect)) {
          CursorPos.X = p.x - rect.left - BorderX;
          CursorPos.Y = p.y - rect.top - BorderY;
        }
        else {
          // window seems not to be existent, so set cursor to
          // a negative value
          CursorPos.X = -1;
          CursorPos.Y = -1;
        }
      }
    }
    position2di CursorPos;
    dimension2d<s32> WindowSize;
    dimension2df InvWindowSize;
    HWND HWnd;
    s32 BorderX, BorderY;
    IRECT ReferenceRect;
    bool UseReferenceRect;
    bool IsVisible;
  };
  //! returns the win32 cursor control
  CCursorControl* getWin32CursorControl();
private:
  //! create the driver
  void createDriver();
  //! switchs to fullscreen
  bool switchToFullScreen();
  void getWindowsVersion(char* version);
  void resizeIfNecessary();
  HWND HWnd;
  CCursorControl* Win32CursorControl;
  bool ChangedToFullScreen;
  bool Resized;
  bool ExternalWindow;
};
#endif // _IRR_COMPILE_WITH_WINDOWS_CE_DEVICE_
#endif // __C_IRR_DEVICE_WINCE_H_INCLUDED__

