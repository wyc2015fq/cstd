
#include <windows.h>

int test_disp_trans() {
  DEVMODE dm = {0};
  
  if (0 != EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm))
  {
    // swap width and height 
    int temp = dm.dmPelsHeight;
    dm.dmPelsHeight = dm.dmPelsWidth;
    dm.dmPelsWidth = temp;
    
    // determine new orientation
    if (0) {
      switch(dm.dmDisplayOrientation) 
      { 
      case DMDO_DEFAULT:
        dm.dmDisplayOrientation = DMDO_270;
        break;
      case DMDO_270: 
        dm.dmDisplayOrientation = DMDO_180;
        break;
      case DMDO_180: 
        dm.dmDisplayOrientation = DMDO_90;
        break;
      case DMDO_90: 
        dm.dmDisplayOrientation = DMDO_DEFAULT;
        break;
      default: 
        // unknown orientation value 
        // add exception handling here 
        break;
      } 
    } else {
      switch(dm.dmDisplayOrientation) 
      { 
      case DMDO_DEFAULT:
        dm.dmDisplayOrientation = DMDO_270;
        break;
      case DMDO_270:
      case DMDO_180:
      case DMDO_90: 
        dm.dmDisplayOrientation = DMDO_DEFAULT;
        break;
      default: 
        // unknown orientation value 
        // add exception handling here 
        break;
      } 
    }
    
    int iRet = ChangeDisplaySettings(&dm, 0); 
    if (DISP_CHANGE_SUCCESSFUL != iRet) 
    {
      // add exception handling here 
    }
  }
  return 0;
}
