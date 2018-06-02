// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_IRR_DEVICE_STUB_H_INCLUDED__
#define __C_IRR_DEVICE_STUB_H_INCLUDED__
//#include "IrrlichtDevice.h"
//#include "IImagePresenter.h"
//#include "SIrrCreationParameters.h"
//#include "CVideoModeList.h"
// lots of prototypes:
class IRandomizer;
class IGUIEnvironment;
IGUIEnvironment* createGUIEnvironment(IFileSystem* fs,
    IVideoDriver* Driver, IOSOperator* op);
}
ISceneManager* createSceneManager(IVideoDriver* driver,
    IFileSystem* fs, ICursorControl* cc, IGUIEnvironment* gui);
}
IFileSystem* createFileSystem();
}
IVideoDriver* createSoftwareDriver(const dimension2du& windowSize,
    bool fullscreen, IFileSystem* io,
    IImagePresenter* presenter);
IVideoDriver* createBurningVideoDriver(const SIrrlichtCreationParameters& params,
    IFileSystem* io, IImagePresenter* presenter);
IVideoDriver* createNullDriver(IFileSystem* io, const dimension2du& screenSize);
}
//! Stub for an Irrlicht Device implementation
class CIrrDeviceStub : public IrrlichtDevice
{
public:
  //! constructor
  CIrrDeviceStub(const SIrrlichtCreationParameters& param);
  //! destructor
  ~CIrrDeviceStub();
  //! returns the video driver
  IVideoDriver* getVideoDriver();
  //! return file system
  IFileSystem* getFileSystem();
  //! returns the gui environment
  IGUIEnvironment* getGUIEnvironment();
  //! returns the scene manager
  ISceneManager* getSceneManager();
  //! \return Returns a pointer to the mouse cursor control interface.
  ICursorControl* getCursorControl();
  //! Returns a pointer to a list with all video modes supported by the gfx adapter.
  IVideoModeList* getVideoModeList();
  //! Returns a pointer to the ITimer object. With it the current Time can be received.
  ITimer* getTimer();
  //! Returns the version of the engine.
  const char* getVersion() const;
  //! send the event to the right receiver
  bool postEventFromUser(const SEvent& event);
  //! Sets a new event receiver to receive events
  void setEventReceiver(IEventReceiver* receiver);
  //! Returns pointer to the current event receiver. Returns 0 if there is none.
  IEventReceiver* getEventReceiver();
  //! Sets the input receiving scene manager.
  /** If set to null, the main scene manager (returned by GetSceneManager()) will receive the input */
  void setInputReceivingSceneManager(ISceneManager* sceneManager);
  //! Returns a pointer to the logger.
  ILogger* getLogger();
  //! Provides access to the engine's currently set randomizer.
  IRandomizer* getRandomizer() const;
  //! Sets a new randomizer.
  void setRandomizer(IRandomizer* r);
  //! Creates a new default randomizer.
  IRandomizer* createDefaultRandomizer() const;
  //! Returns the operation system opertator object.
  IOSOperator* getOSOperator();
  //! Checks if the window is running in fullscreen mode.
  bool isFullscreen() const;
  //! get color format of the current window
  ECOLOR_FORMAT getColorFormat() const;
  //! Activate any joysticks, and generate events for them.
  bool activateJoysticks(SJoystickInfo > & joystickInfo);
  //! Set the current Gamma Value for the Display
  bool setGammaRamp(f32 red, f32 green, f32 blue, f32 brightness, f32 contrast);
  //! Get the current Gamma Value for the Display
  bool getGammaRamp(f32& red, f32& green, f32& blue, f32& brightness, f32& contrast);
  //! Set the maximal elapsed time between 2 clicks to generate doubleclicks for the mouse. It also affects tripleclick behavior.
  //! When set to 0 no double- and tripleclicks will be generated.
  void setDoubleClickTime(u32 timeMs);
  //! Get the maximal elapsed time between 2 clicks to generate double- and tripleclicks for the mouse.
  u32 getDoubleClickTime() const;
  //! Remove all messages pending in the system message loop
  void clearSystemMessages();
protected:
  void createGUIAndScene();
  //! checks version of SDK and prints warning if there might be a problem
  bool checkVersion(const char* version);
  //! Compares to the last call of this function to return double and triple clicks.
  //! \return Returns only 1,2 or 3. A 4th click will start with 1 again.
  u32 checkSuccessiveClicks(s32 mouseX, s32 mouseY, EMOUSE_INPUT_EVENT inputEvent);
  void calculateGammaRamp(u16* ramp, f32 gamma, f32 relativebrightness, f32 relativecontrast);
  void calculateGammaFromRamp(f32& gamma, const u16* ramp);
  IVideoDriver* VideoDriver;
  IGUIEnvironment* GUIEnvironment;
  ISceneManager* SceneManager;
  ITimer* Timer;
  ICursorControl* CursorControl;
  IEventReceiver* UserReceiver;
  CLogger* Logger;
  IOSOperator* Operator;
  IRandomizer* Randomizer;
  IFileSystem* FileSystem;
  ISceneManager* InputReceivingSceneManager;
  struct SMouseMultiClicks {
    SMouseMultiClicks()
      : DoubleClickTime(500), CountSuccessiveClicks(0), LastClickTime(0), LastMouseInputEvent(EMIE_COUNT)
    {}
    u32 DoubleClickTime;
    u32 CountSuccessiveClicks;
    u32 LastClickTime;
    position2di LastClick;
    EMOUSE_INPUT_EVENT LastMouseInputEvent;
  };
  SMouseMultiClicks MouseMultiClicks;
  CVideoModeList* VideoModeList;
  SIrrlichtCreationParameters CreationParams;
  bool Close;
};
#endif

