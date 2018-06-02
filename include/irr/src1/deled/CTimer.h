// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_IRR_C_TIMER_H_INCLUDED__
#define __C_IRR_C_TIMER_H_INCLUDED__
//#include "ITimer.h"
//#include "os.h"
//! Device independent implementation of the timer
class CTimer : public ITimer
{
public:
  CTimer(bool usePerformanceTimer = true) {
    os::Timer::initTimer(usePerformanceTimer);
  }
  //! Returns current real time in milliseconds of the system.
  /** This value does not start with 0 when the application starts.
  For example in one implementation the value returned could be the
  amount of milliseconds which have elapsed since the system was started. */
  u32 getRealTime() const {
    return os::Timer::getRealTime();
  }
  //! Get current time and date in calendar form
  RealTimeDate getRealTimeAndDate() const {
    return os::Timer::getRealTimeAndDate();
  }
  //! Returns current time in milliseconds.
  /** This value starts with 0 and can be manipulated using setTime(), stopTimer(),
  startTimer(), etc. This value depends on the set speed of the timer if the timer
  is stopped, etc. If you need the system time, use getRealTime() */
  u32 getTime() const {
    return os::Timer::getTime();
  }
  //! sets current time
  void setTime(u32 time) {
    os::Timer::setTime(time);
  }
  //! Stops the game timer.
  /** The timer is reference counted, which means everything which calls
  stopTimer() will also have to call startTimer(), otherwise the timer may not start/stop
  corretly again. */
  void stop() {
    os::Timer::stopTimer();
  }
  //! Starts the game timer.
  /** The timer is reference counted, which means everything which calls
  stopTimer() will also have to call startTimer(), otherwise the timer may not start/stop
  corretly again. */
  void start() {
    os::Timer::startTimer();
  }
  //! Sets the speed of the timer
  /** The speed is the factor with which the time is running faster or slower then the
  real system time. */
  void setSpeed(f32 speed = 1.0f) {
    os::Timer::setSpeed(speed);
  }
  //! Returns current speed of the timer
  /** The speed is the factor with which the time is running faster or slower then the
  real system time. */
  f32 getSpeed() const {
    return os::Timer::getSpeed();
  }
  //! Returns if game timer is currently stopped
  bool isStopped() const {
    bool ret = os::Timer::isStopped();
    _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
    return ret;
  }
  //! Advances the time
  /** Makes the timer update the time value based on the real time. This is
  called automaticly when calling IrrlichtDevice::run(), but you can call it manually
  if you don't use this method. */
  void tick() {
    os::Timer::tick();
  }
};
// end
#endif

