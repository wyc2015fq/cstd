
#include "cstd.h"
#include "str.h"
//#define NO_IRR_COMPILE_WITH_JPG_LOADER_
#define NO_IRR_COMPILE_WITH_PNG_LOADER_
#define NO_IRR_COMPILE_WITH_JPG_WRITER_
#define NO_IRR_COMPILE_WITH_PNG_WRITER_
//#define NO_IRR_COMPILE_WITH_ZLIB_
//#define NO_IRR_COMPILE_WITH_LZMA_
//#define NO__IRR_COMPILE_WITH_ZIP_ARCHIVE_LOADER_

//#include "IrrCompileConfig.h"
#include "irr.h"
#include "matrix4.h"
#include "DeviceStub.h"
#include "NullDriver.h"
#include "DeviceWin32.h"
#include "BurningSoftwareDriver.h"
#include "SoftwareDriver.h"

#include "MeshFile/test_MeshFile.inl"
#include "test/test_01_HelloWorld.h"
#if 1
#else
#include "octree.h"
#include "libIrrlicht.h"
#include "01_HelloWorld.h"
#include "02_Quake3Map.h"
#include "03_CustomSceneNode.h"
#include "04_Movement.h"
#include "05_UserInterface.h"
#include "06_2DGraphics.h"
#include "07_Collision.h"
#include "08_SpecialFX.h"
#include "09_Meshviewer.h"
#include "10_Shaders.h"
#include "11_PerPixelLighting.h"
#include "12_TerrainRendering.h"
#include "13_RenderToTexture.h"
#include "14_Win32Window.h"
#include "15_LoadIrrFile.h"
#include "16_Quake3MapShader.h"
#include "17_HelloWorld_Mobile.h"
#include "18_SplitScreen.h"
#include "19_MouseAndJoystick.h"
#include "20_ManagedLights.h"
#include "21_Quake3Explorer.h"
#include "22_MaterialViewer.h"
#include "23_SMeshHandling.h"
#include "24_CursorControl.h"
#include "25_XmlHandling.h"
#include "26_OcclusionQuery.h"

#include "21_Quake3Explorer.h"
#endif


#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

#ifdef TEST
void __cdecl bz_internal_error_1(int x)
{
  return ;
}

//#define test_Irrlicht main
int test_Irrlicht()
{

  return 0;
}
#endif
