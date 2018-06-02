// Copyright (C) 2002-2007 Nikolaus Gebhardt
// Copyright (C) 2007-2012 Christian Stehno
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_IRR_DEVICE_FB_H_INCLUDED__
#define __C_IRR_DEVICE_FB_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_FB_DEVICE_
//#include "CIrrDeviceStub.h"
//#include "SIrrCreationParameters.h"
//#include "IrrlichtDevice.h"
//#include "IImagePresenter.h"
//#include "ICursorControl.h"
#define KeySym s32
//#include <linux/fb.h>
//#include <linux/kd.h>
class CIrrDeviceFB : public CIrrDeviceStub, public IImagePresenter
{
public:
  //! constructor
  CIrrDeviceFB(const SIrrlichtCreationParameters& params);
  //! destructor
  ~CIrrDeviceFB();
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
  //! Minimizes window
  void minimizeWindow();
  //! Maximizes window
  void maximizeWindow();
  //! Restores original window size
  void restoreWindow();
  //! presents a surface in the client area
  bool present(IImage* surface, void* windowId = 0, IRECT* src = 0);
  //! notifies the device that it should close itself
  void closeDevice();
  //! Sets if the window should be resizeable in windowed mode.
  void setResizable(bool resize = false);
  //! Returns the type of this device
  E_DEVICE_TYPE getType() const;
private:
  //! create the driver
  void createDriver();
  bool createWindow(const dimension2du& windowSize, u32 bits);
  //! Implementation of the cursor control
  class CCursorControl : public ICursorControl
  {
  public:
    CCursorControl(CIrrDeviceFB* dev, bool null)
      : Device(dev), IsVisible(true), Null(null) {
      Device->grab();
    }
    ~CCursorControl() {
      Device->drop();
    }
    //! Changes the visible state of the mouse cursor.
    void setVisible(bool visible) {
      IsVisible = visible;
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
      setPosition((s32)(x * Device->CreationParams.WindowSize.Width), (s32)(y * Device->CreationParams.WindowSize.Height));
    }
    //! Sets the new position of the cursor.
    void setPosition(const position2di& pos) {
      setPosition(pos.X, pos.Y);
    }
    //! Sets the new position of the cursor.
    void setPosition(s32 x, s32 y) {
    }
    //! Returns the current position of the mouse cursor.
    const position2di& getPosition() {
      updateCursorPos();
      return CursorPos;
    }
    //! Returns the current position of the mouse cursor.
    position2df getRelativePosition() {
      updateCursorPos();
      return position2df(CursorPos.X / (f32)Device->CreationParams.WindowSize.Width,
          CursorPos.Y / (f32)Device->CreationParams.WindowSize.Height);
    }
    void setReferenceRect(IRECT* rect = 0) {
    }
  private:
    void updateCursorPos() {
    }
    position2di CursorPos;
    CIrrDeviceFB* Device;
    bool IsVisible;
    bool Null;
  };
  friend class CCursorControl;
  int Framebuffer;
  int EventDevice;
  int KeyboardDevice;
  struct fb_fix_screeninfo fbfixscreeninfo;
  struct fb_var_screeninfo fbscreeninfo;
  struct fb_var_screeninfo oldscreeninfo;
  long KeyboardMode;
  u8* SoftwareImage;
  u32 Pitch;
  ECOLOR_FORMAT FBColorFormat;
  bool Close;
  struct SKeyMap {
    SKeyMap() {}
    SKeyMap(s32 x11, s32 win32)
      : X11Key(x11), Win32Key(win32) {
    }
    KeySym X11Key;
    s32 Win32Key;
    bool operator<(const SKeyMap& o) const {
      return X11Key < o.X11Key;
    }
  };
  SKeyMap > KeyMap;
};
#endif // _IRR_USE_FB_DEVICE_
#endif // __C_IRR_DEVICE_FB_H_INCLUDED__

