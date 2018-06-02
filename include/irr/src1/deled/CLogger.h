// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_LOGGER_H_INCLUDED__
#define __C_LOGGER_H_INCLUDED__
//#include "ILogger.h"
//#include "os.h"
//#include "irrString.h"
//#include "IEventReceiver.h"
// "CLogger_impl.h"
//! Class for logging messages, warnings and errors to stdout
class CLogger : public ILogger
{
public:
  CLogger(IEventReceiver* r);
  //! Returns the current set log level.
  ELOG_LEVEL getLogLevel() const;
  //! Sets a new log level. void setLogLevel(ELOG_LEVEL ll);
  void setLogLevel(ELOG_LEVEL ll);
  //! Prints out a text into the log
  void log(const c8* text, ELOG_LEVEL ll = ELL_INFORMATION);
  //! Prints out a text into the log
  void log(const c8* text, const c8* hint, ELOG_LEVEL ll = ELL_INFORMATION);
  //! Sets a new event receiver
  void setReceiver(IEventReceiver* r);
private:
  ELOG_LEVEL LogLevel;
  IEventReceiver* Receiver;
};
// end
#endif

