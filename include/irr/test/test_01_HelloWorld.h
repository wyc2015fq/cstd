
int test_01_HelloWorld()
{
  /*
  The most important function of the engine is the createDevice()
  function. The IrrlichtDevice is created by it, which is the root
  object for doing anything with the engine. createDevice() has 7
  parameters:
  - deviceType: Type of the device. This can currently be the Null-device,
     one of the two software renderers, D3D8, D3D9, or OpenGL. In this
     example we use EDT_SOFTWARE, but to try out, you might want to
     change it to EDT_BURNINGSVIDEO, EDT_NULL, EDT_DIRECT3D8,
     EDT_DIRECT3D9, or EDT_OPENGL.
  - windowSize: Size of the Window or screen in FullScreenMode to be
     created. In this example we use 640x480.
  - bits: Amount of color bits per pixel. This should be 16 or 32. The
     parameter is often ignored when running in windowed mode.
  - fullscreen: Specifies if we want the device to run in fullscreen mode
     or not.
  - stencilbuffer: Specifies if we want to use the stencil buffer (for
     drawing shadows).
  - vsync: Specifies if we want to have vsync enabled, this is only useful
     in fullscreen mode.
  - eventReceiver: An object to receive events. We do not want to use this
     parameter here, and set it to 0.
  Always check the return value to cope with unsupported drivers,
  dimensions, etc.
  */
  IVideoDriver* driver = NULL;
  ISceneManager* smgr = NULL;
  IGUIEnvironment* guienv = NULL;
  IAnimatedMesh* mesh = NULL;
  IAnimatedMeshSceneNode* node = NULL;
  IDevice* device = NULL;
  E_DRIVER_TYPE type;
  type = EDT_SOFTWARE;
  type = EDT_OPENGL;
  device = createDevice(type, iSIZE(640, 480), 16, false, false, false, 0);
  if (!device) {
    return 1;
  }
  /*
  Set the caption of the window to some nice text. Note that there is an
  'L' in front of the string. The Irrlicht Engine uses wide character
  strings when displaying text.
  */
  device->setWindowCaption(device, "Hello World! - Irrlicht Engine Demo");

  /*
  Get a pointer to the VideoDriver, the SceneManager and the graphical
  user interface environment, so that we do not always have to write
  device->getVideoDriver(), device->getSceneManager(), or
  device->getGUIEnvironment().
  */
  driver = device->VideoDriver;
  smgr = device->SceneManager;
  guienv = device->GUIEnvironment;
  /*
  We add a hello world label to the window, using the GUI environment.
  The text is placed at the position (10,10) as top left corner and
  (260,22) as lower right corner.
  */
  //guienv->addStaticText(guienv, "Hello World! This is the Irrlicht Software renderer!", iRECT2(10, 10, 260, 22), true, true, NULL, -1, false);

  /*
  To show something interesting, we load a Quake 2 model and display it.
  We only have to get the Mesh from the Scene Manager with getMesh() and add
  a SceneNode to display the mesh with addAnimatedMeshSceneNode(). We
  check the return value of getMesh() to become aware of loading problems
  and other errors.
  Instead of writing the filename sydney.md2, it would also be possible
  to load a Maya object file (.obj), a complete Quake3 map (.bsp) or any
  other supported file format. By the way, that cool Quake 2 model
  called sydney was modelled by Brian Collins.
  */
#if 0
  mesh = getMesh(smgr, "../../media/sydney.md2");
  if (!mesh) {
    return 1;
  }
  node = smgr->addAnimatedMeshSceneNode(mesh, 0, -1, f3ZERO, f3ZERO, f3ONE, 0);
  /*
  To let the mesh look a little bit nicer, we change its material. We
  disable lighting because we do not have a dynamic light in here, and
  the mesh would be totally black otherwise. Then we set the frame loop,
  such that the predefined STAND animation is used. And last, we apply a
  texture to the mesh. Without it the mesh would be drawn using only a
  color.
  */
  if (node) {
    node->setMaterialFlag(node, EMF_LIGHTING, false);
    node->setMD2Animation(node, EMAT_STAND);
    node->setMaterialTexture(node, 0, driver->getTextureFile(driver, "../../media/sydney.bmp"));
  }
  /*
  To look at the mesh, we place a camera into 3d space at the position
  (0, 30, -40). The camera looks from there to (0,5,0), which is
  approximately the place where our md2 model is.
  */
  smgr->addCameraSceneNode(smgr, 0, f3VECTOR(0, 30, -40), f3VECTOR(0, 5, 0), -1, true);
  /*
  Ok, now we have set up the scene, lets draw everything: We run the
  device in a while() loop, until the device does not want to run any
  more. This would be when the user closes the window or presses ALT+F4
  (or whatever keycode closes a window).
  */
#endif
  //while (device->run(device)) {
  while (1) {
    IImage im[1] = {0};
    CDeviceWin32_handleSystemMessages(device);
    IImage_setsize(im, ECF_R8G8B8, iSIZE(100, 100));
    memset(im->Data, 255, 100*100);
#if 0
    driver->beginScene(driver, true, true, _ARGB(255, 100, 101, 140), NULL, NULL);
    smgr->drawAll(smgr);
    guienv->drawAll(driver);
    driver->endScene(driver);
#endif
    CDeviceWin32_present(device, im, device->WindowId, 0);
    IImage_free(im);
  }
  /*
  After we are done with the render loop, we have to delete the Irrlicht
  Device created before with createDevice(). In the Irrlicht Engine, you
  have to delete all objects you created with a method or function which
  starts with 'create'. The object is simply deleted by calling ->drop().
  See the documentation at IReferenceCounted::drop() for more
  information.
  */
  //device->done();SetWindowLongPtrA
  return 0;
}
/*
That's it. Compile and run.
**/

