// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
// This device code is based on the original SDL device implementation
// contributed by Shane Parker (sirshane).
#ifndef __C_IRR_DEVICE_SDL_H_INCLUDED__
#define __C_IRR_DEVICE_SDL_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_SDL_DEVICE_
//#include "IrrlichtDevice.h"
//#include "CIrrDeviceStub.h"
//#include "IImagePresenter.h"
//#include "ICursorControl.h"
//#include <SDL/SDL.h>
//#include <SDL/SDL_syswm.h>
class CIrrDeviceSDL : public CIrrDeviceStub, IImagePresenter
{
public:
  //! constructor
  CIrrDeviceSDL(const SIrrlichtCreationParameters& param);
  //! destructor
  ~CIrrDeviceSDL();
  //! runs the device. Returns false if device wants to be deleted
  bool run();
  //! pause execution temporarily
  void yield();
  //! pause execution for a specified time
  void sleep(u32 timeMs, bool pauseTimer);
  //! sets the caption of the window
  void setWindowCaption(const char* text);
  //! returns if window is active. if not, nothing need to be drawn
  bool isWindowActive() const;
  //! returns if window has focus.
  bool isWindowFocused() const;
  //! returns if window is minimized.
  bool isWindowMinimized() const;
  //! returns color format of the window.
  ECOLOR_FORMAT getColorFormat() const;
  //! presents a surface in the client area
  bool present(IImage* surface, void* windowId = 0, IRECT* src = 0);
  //! notifies the device that it should close itself
  void closeDevice();
  //! \return Returns a pointer to a list with all video modes supported
  IVideoModeList* getVideoModeList();
  //! Sets if the window should be resizable in windowed mode.
  void setResizable(bool resize = false);
  //! Minimizes the window.
  void minimizeWindow();
  //! Maximizes the window.
  void maximizeWindow();
  //! Restores the window size.
  void restoreWindow();
  //! Activate any joysticks, and generate events for them.
  bool activateJoysticks(SJoystickInfo > & joystickInfo);
  //! Set the current Gamma Value for the Display
  bool setGammaRamp(f32 red, f32 green, f32 blue, f32 brightness, f32 contrast);
  //! Get the current Gamma Value for the Display
  bool getGammaRamp(f32& red, f32& green, f32& blue, f32& brightness, f32& contrast);
  //! Get the device type
  E_DEVICE_TYPE getType() const {
    return EIDT_SDL;
  }
  //! Implementation of the linux cursor control
  class CCursorControl : public ICursorControl
  {
  public:
    CCursorControl(CIrrDeviceSDL* dev)
      : Device(dev), IsVisible(true) {
    }
    //! Changes the visible state of the mouse cursor.
    void setVisible(bool visible) {
      IsVisible = visible;
      if (visible) {
        SDL_ShowCursor(SDL_ENABLE);
      }
      else {
        SDL_ShowCursor(SDL_DISABLE);
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
      setPosition((s32)(x * Device->Width), (s32)(y * Device->Height));
    }
    //! Sets the new position of the cursor.
    void setPosition(const position2di& pos) {
      setPosition(pos.X, pos.Y);
    }
    //! Sets the new position of the cursor.
    void setPosition(s32 x, s32 y) {
      SDL_WarpMouse(x, y);
    }
    //! Returns the current position of the mouse cursor.
    const position2di& getPosition() {
      updateCursorPos();
      return CursorPos;
    }
    //! Returns the current position of the mouse cursor.
    position2df getRelativePosition() {
      updateCursorPos();
      return position2df(CursorPos.X / (f32)Device->Width,
          CursorPos.Y / (f32)Device->Height);
    }
    void setReferenceRect(IRECT* rect = 0) {
    }
  private:
    void updateCursorPos() {
      CursorPos.X = Device->MouseX;
      CursorPos.Y = Device->MouseY;
      if (CursorPos.X < 0) {
        CursorPos.X = 0;
      }
      if (CursorPos.X > (s32)Device->Width) {
        CursorPos.X = Device->Width;
      }
      if (CursorPos.Y < 0) {
        CursorPos.Y = 0;
      }
      if (CursorPos.Y > (s32)Device->Height) {
        CursorPos.Y = Device->Height;
      }
    }
    CIrrDeviceSDL* Device;
    position2di CursorPos;
    bool IsVisible;
  };
private:
  //! create the driver
  void createDriver();
  bool createWindow();
  void createKeyMap();
  SDL_Surface* Screen;
  int SDL_Flags;
#if defined(_IRR_COMPILE_WITH_JOYSTICK_EVENTS_)
  SDL_Joystick* > Joysticks;
#endif
  s32 MouseX, MouseY;
  u32 MouseButtonStates;
  u32 Width, Height;
  bool Resizable;
  bool WindowHasFocus;
  bool WindowMinimized;
  struct SKeyMap {
    SKeyMap() {}
    SKeyMap(s32 x11, s32 win32)
      : SDLKey(x11), Win32Key(win32) {
    }
    s32 SDLKey;
    s32 Win32Key;
    bool operator<(const SKeyMap& o) const {
      return SDLKey < o.SDLKey;
    }
  };
  SKeyMap > KeyMap;
  SDL_SysWMinfo Info;
};
#endif // _IRR_COMPILE_WITH_SDL_DEVICE_
#endif // __C_IRR_DEVICE_SDL_H_INCLUDED__

