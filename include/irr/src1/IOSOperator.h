// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_OS_OPERATOR_H_INCLUDED__
#define __I_OS_OPERATOR_H_INCLUDED__
//#include "irrString.h"
//! The Operating system operator provides operation system specific methods and informations.
class IOSOperator
{
public:
  //! Get the current operation system version as string.
  const char* getOperatingSystemVersion() = 0;
  //! Get the current operation system version as string.
  /** \deprecated Use getOperatingSystemVersion instead. This method will be removed in Irrlicht 1.9. */
  _IRR_DEPRECATED_ const char* getOperationSystemVersion() const {
    return getOperatingSystemVersion();
  }
  //! Copies text to the clipboard
  void copyToClipboard(const c8* text) = 0;
  //! Get text from the clipboard
  /** \return Returns 0 if no string is in there. */
  const c8* getTextFromClipboard() = 0;
  //! Get the processor speed in megahertz
  /** \param MHz The integer variable to store the speed in.
  \return True if successful, false if not */
  bool getProcessorSpeedMHz(u32* MHz) = 0;
  //! Get the total and available system RAM
  /** \param Total: will contain the total system memory
  \param Avail: will contain the available memory
  \return True if successful, false if not */
  bool getSystemMemory(u32* Total, u32* Avail) = 0;
};
// end
#endif

