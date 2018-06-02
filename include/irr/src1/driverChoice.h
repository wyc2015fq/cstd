// Copyright (C) 2009-2012 Christian Stehno
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __E_DRIVER_CHOICE_H_INCLUDED__
#define __E_DRIVER_CHOICE_H_INCLUDED__
////#include <iostream>
//#include <stdio.h>
//#include "IrrlichtDevice.h"
//! ask user for driver
static E_DRIVER_TYPE driverChoiceConsole(bool allDrivers = true)
{
  const char* const names[] = {"NullDriver", "Software Renderer", "Burning's Video", "Direct3D 8.1", "Direct3D 9.0c", "OpenGL 1.x/2.x/3.x"};
  printf("Please select the driver you want:\n");
  u32 i = 0;
  for (i = EDT_COUNT; i > 0; --i) {
    if (allDrivers || (IrrlichtDevice::isDriverSupported(E_DRIVER_TYPE(i - 1)))) {
      printf(" (%c) %s\n", 'a' + EDT_COUNT - i, names[i - 1]);
    }
  }
  char c;
  scanf("%c", &c);
  c = EDT_COUNT + 'a' - c;
  for (i = EDT_COUNT; i > 0; --i) {
    if (!(allDrivers || (IrrlichtDevice::isDriverSupported(E_DRIVER_TYPE(i - 1))))) {
      --c;
    }
    if ((char)i == c) {
      return E_DRIVER_TYPE(i - 1);
    }
  }
  return EDT_COUNT;
}
#endif

