
#ifdef _IRR_WINDOWS_
//#include <windows.h>
#if defined(_DEBUG) && !defined(__GNUWIN32__) && !defined(_WIN32_WCE)
//#include <crtdbg.h>
#endif // _DEBUG
#endif
//#include "irrlicht.h"
#ifdef _IRR_COMPILE_WITH_WINDOWS_DEVICE_
#include "CIrrDeviceWin32.h"
#include "CIrrDeviceWin32_impl.h"
#endif
#ifdef _IRR_COMPILE_WITH_OSX_DEVICE_
//#include "MacOSX/CIrrDeviceMacOSX.h"
#endif
#ifdef _IRR_COMPILE_WITH_WINDOWS_CE_DEVICE_
//#include "CIrrDeviceWinCE.h"
#endif
#ifdef _IRR_COMPILE_WITH_X11_DEVICE_
//#include "CIrrDeviceLinux.h"
#endif
#ifdef _IRR_COMPILE_WITH_SDL_DEVICE_
//#include "CIrrDeviceSDL.h"
#endif
#ifdef _IRR_COMPILE_WITH_FB_DEVICE_
//#include "CIrrDeviceFB.h"
#endif
#ifdef _IRR_COMPILE_WITH_CONSOLE_DEVICE_
//#include "CIrrDeviceConsole.h"
#endif
IrrlichtDevice* createDeviceEx(const SIrrlichtCreationParameters* params) {
  IrrlichtDevice* dev = 0;
#ifdef _IRR_COMPILE_WITH_WINDOWS_DEVICE_
  if (params->DeviceType == EIDT_WIN32 || (!dev && params->DeviceType == EIDT_BEST)) {
    dev = new CIrrDeviceWin32(params);
  }
#endif
#ifdef _IRR_COMPILE_WITH_OSX_DEVICE_
  if (params->DeviceType == EIDT_OSX || (!dev && params->DeviceType == EIDT_BEST)) {
    dev = new CIrrDeviceMacOSX(params);
  }
#endif
#ifdef _IRR_COMPILE_WITH_WINDOWS_CE_DEVICE_
  if (params->DeviceType == EIDT_WINCE || (!dev && params->DeviceType == EIDT_BEST)) {
    dev = new CIrrDeviceWinCE(params);
  }
#endif
#ifdef _IRR_COMPILE_WITH_X11_DEVICE_
  if (params->DeviceType == EIDT_X11 || (!dev && params->DeviceType == EIDT_BEST)) {
    dev = new CIrrDeviceLinux(params);
  }
#endif
#ifdef _IRR_COMPILE_WITH_SDL_DEVICE_
  if (params->DeviceType == EIDT_SDL || (!dev && params->DeviceType == EIDT_BEST)) {
    dev = new CIrrDeviceSDL(params);
  }
#endif
#ifdef _IRR_COMPILE_WITH_FB_DEVICE_
  if (params->DeviceType == EIDT_FRAMEBUFFER || (!dev && params->DeviceType == EIDT_BEST)) {
    dev = new CIrrDeviceFB(params);
  }
#endif
#ifdef _IRR_COMPILE_WITH_CONSOLE_DEVICE_
  if (params->DeviceType == EIDT_CONSOLE || (!dev && params->DeviceType == EIDT_BEST)) {
    dev = new CIrrDeviceConsole(params);
  }
#endif
  if (dev && !dev->getVideoDriver() && params->DriverType != EDT_NULL) {
    dev->closeDevice(); // destroy window
    dev->run(); // consume quit message
    dev->drop();
    dev = 0;
  }
  return dev;
}
//! stub for calling createDeviceEx
static IrrlichtDevice* createDevice(E_DRIVER_TYPE driverType, ISIZE windowSize, u32 bits, bool fullscreen, bool stencilbuffer, bool vsync, IEventReceiver* res)
{
}
const matrix4 IdentityMatrix(matrix4::EM4CONST_IDENTITY);

SMaterial IdentityMaterial;

#endif // defined(_IRR_WINDOWS_)

