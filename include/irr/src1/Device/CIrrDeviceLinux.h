// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_IRR_DEVICE_LINUX_H_INCLUDED__
#define __C_IRR_DEVICE_LINUX_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_X11_DEVICE_
//#include "CIrrDeviceStub.h"
//#include "IrrlichtDevice.h"
//#include "IImagePresenter.h"
//#include "ICursorControl.h"
//#include "os.h"
#ifdef _IRR_COMPILE_WITH_X11_
#ifdef _IRR_COMPILE_WITH_OPENGL_
//#include <GL/gl.h>
#define GLX_GLXEXT_LEGACY 1
//#include <GL/glx.h>
#ifdef _IRR_OPENGL_USE_EXTPOINTER_
//#include "glxext.h"
#endif
#endif
//#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/cursorfont.h>
#ifdef _IRR_LINUX_X11_VIDMODE_
//#include <X11/extensions/xf86vmode.h>
#endif
#ifdef _IRR_LINUX_X11_RANDR_
//#include <X11/extensions/Xrandr.h>
#endif
//#include <X11/keysym.h>
#else
#define KeySym s32
#endif
class CIrrDeviceLinux : public CIrrDeviceStub, public IImagePresenter
{
public:
  //! constructor
  CIrrDeviceLinux(const SIrrlichtCreationParameters& param);
  //! destructor
  ~CIrrDeviceLinux();
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
  //! \return Returns a pointer to a list with all video modes
  //! supported by the gfx adapter.
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
  //! gets text from the clipboard
  //! \return Returns 0 if no string is in there.
  const c8* getTextFromClipboard() const;
  //! copies text to the clipboard
  //! This sets the clipboard selection and _not_ the primary selection which you have on X on the middle mouse button.
  void copyToClipboard(const c8* text) const;
  //! Remove all messages pending in the system message loop
  void clearSystemMessages();
  //! Get the device type
  E_DEVICE_TYPE getType() const {
    return EIDT_X11;
  }
#ifdef _IRR_COMPILE_WITH_X11_
  // convert an Irrlicht texture to a X11 cursor
  Cursor TextureToCursor(ITexture* tex, const IRECT& sourceRect, const position2di& hotspot);
  Cursor TextureToMonochromeCursor(ITexture* tex, const IRECT& sourceRect, const position2di& hotspot);
#ifdef _IRR_LINUX_XCURSOR_
  Cursor TextureToARGBCursor(ITexture* tex, const IRECT& sourceRect, const position2di& hotspot);
#endif
#endif
private:
  //! create the driver
  void createDriver();
  bool createWindow();
  void createKeyMap();
  void pollJoysticks();
  void initXAtoms();
  bool switchToFullscreen(bool reset = false);
  //! Implementation of the linux cursor control
  class CCursorControl : public ICursorControl
  {
  public:
    CCursorControl(CIrrDeviceLinux* dev, bool null);
    ~CCursorControl();
    //! Changes the visible state of the mouse cursor.
    void setVisible(bool visible) {
      if (visible == IsVisible) {
        return;
      }
      IsVisible = visible;
#ifdef _IRR_COMPILE_WITH_X11_
      if (!Null) {
        if (!IsVisible) {
          XDefineCursor(Device->display, Device->window, invisCursor);
        }
        else {
          XUndefineCursor(Device->display, Device->window);
        }
      }
#endif
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
#ifdef _IRR_COMPILE_WITH_X11_
      if (!Null) {
        if (UseReferenceRect) {
          XWarpPointer(Device->display,
              None,
              Device->window, 0, 0,
              Device->Width,
              Device->Height,
              ReferenceRect.UpperLeftCorner.X + x,
              ReferenceRect.UpperLeftCorner.Y + y);
        }
        else {
          XWarpPointer(Device->display,
              None,
              Device->window, 0, 0,
              Device->Width,
              Device->Height, x, y);
        }
        XFlush(Device->display);
      }
#endif
      CursorPos.X = x;
      CursorPos.Y = y;
    }
    //! Returns the current position of the mouse cursor.
    const position2di& getPosition() {
      updateCursorPos();
      return CursorPos;
    }
    //! Returns the current position of the mouse cursor.
    position2df getRelativePosition() {
      updateCursorPos();
      if (!UseReferenceRect) {
        return position2df(CursorPos.X / (f32)Device->Width,
            CursorPos.Y / (f32)Device->Height);
      }
      return position2df(CursorPos.X / (f32)ReferenceRect.getWidth(),
          CursorPos.Y / (f32)ReferenceRect.getHeight());
    }
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
    //! Sets the active cursor icon
    void setActiveIcon(ECURSOR_ICON iconId);
    //! Gets the currently active icon
    ECURSOR_ICON getActiveIcon() const {
      return ActiveIcon;
    }
    //! Add a custom sprite as cursor icon.
    ECURSOR_ICON addIcon(const SCursorSprite& icon);
    //! replace the given cursor icon.
    void changeIcon(ECURSOR_ICON iconId, const SCursorSprite& icon);
    //! Return a system-specific size which is supported for cursors. Larger icons will fail, smaller icons might work.
    dimension2di getSupportedIconSize() const;
#ifdef _IRR_COMPILE_WITH_X11_
    //! Set platform specific behavior flags.
    void setPlatformBehavior(ECURSOR_PLATFORM_BEHAVIOR behavior) {
      PlatformBehavior = behavior;
    }
    //! Return platform specific behavior.
    ECURSOR_PLATFORM_BEHAVIOR getPlatformBehavior() const {
      return PlatformBehavior;
    }
    void update();
    void clearCursors();
#endif
  private:
    void updateCursorPos() {
#ifdef _IRR_COMPILE_WITH_X11_
      if (Null) {
        return;
      }
      if (PlatformBehavior & ECPB_X11_CACHE_UPDATES && !os::Timer::isStopped()) {
        u32 now = os::Timer::getTime();
        if (now <= lastQuery) {
          return;
        }
        lastQuery = now;
      }
      Window tmp;
      int itmp1, itmp2;
      unsigned  int maskreturn;
      XQueryPointer(Device->display, Device->window,
          &tmp, &tmp,
          &itmp1, &itmp2,
          &CursorPos.X, &CursorPos.Y, &maskreturn);
      if (CursorPos.X < 0) {
        CursorPos.X = 0;
      }
      if (CursorPos.X > (s32) Device->Width) {
        CursorPos.X = Device->Width;
      }
      if (CursorPos.Y < 0) {
        CursorPos.Y = 0;
      }
      if (CursorPos.Y > (s32) Device->Height) {
        CursorPos.Y = Device->Height;
      }
#endif
    }
    CIrrDeviceLinux* Device;
    position2di CursorPos;
    IRECT ReferenceRect;
#ifdef _IRR_COMPILE_WITH_X11_
    ECURSOR_PLATFORM_BEHAVIOR PlatformBehavior;
    u32 lastQuery;
    Cursor invisCursor;
    struct CursorFrameX11 {
      CursorFrameX11() : IconHW(0) {}
      CursorFrameX11(Cursor icon) : IconHW(icon) {}
      Cursor IconHW;  // hardware cursor
    };
    struct CursorX11 {
      CursorX11() {}
      explicit CursorX11(Cursor iconHw, u32 frameTime = 0) : FrameTime(frameTime) {
        Frames.push_back(CursorFrameX11(iconHw));
      }
      CursorFrameX11 > Frames;
      u32 FrameTime;
    };
    CursorX11 > Cursors;
    void initCursors();
#endif
    bool IsVisible;
    bool Null;
    bool UseReferenceRect;
    ECURSOR_ICON ActiveIcon;
    u32 ActiveIconStartTime;
  };
  friend class CCursorControl;
#ifdef _IRR_COMPILE_WITH_X11_
  friend class COpenGLDriver;
  Display* display;
  XVisualInfo* visual;
  int screennr;
  Window window;
  XSetWindowAttributes attributes;
  XSizeHints* StdHints;
  XImage* SoftwareImage;
  mutable char* Clipboard;
#ifdef _IRR_LINUX_X11_VIDMODE_
  XF86VidModeModeInfo oldVideoMode;
#endif
#ifdef _IRR_LINUX_X11_RANDR_
  SizeID oldRandrMode;
  Rotation oldRandrRotation;
#endif
#ifdef _IRR_COMPILE_WITH_OPENGL_
  GLXWindow glxWin;
  GLXContext Context;
#endif
#endif
  u32 Width, Height;
  bool WindowHasFocus;
  bool WindowMinimized;
  bool UseXVidMode;
  bool UseXRandR;
  bool UseGLXWindow;
  bool ExternalWindow;
  int AutorepeatSupport;
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
#if defined(_IRR_COMPILE_WITH_JOYSTICK_EVENTS_)
  struct JoystickInfo {
    int fd;
    int axes;
    int buttons;
    SEvent persistentData;
    JoystickInfo() : fd(-1), axes(0), buttons(0) { }
  };
  JoystickInfo > ActiveJoysticks;
#endif
};
#endif // _IRR_COMPILE_WITH_X11_DEVICE_
#endif // __C_IRR_DEVICE_LINUX_H_INCLUDED__

