/** Example 009 Mesh Viewer
This tutorial show how to create a more complex application with the engine.
We construct a simple mesh viewer using the user interface API and the
scene management of Irrlicht. The tutorial show how to create and use Buttons,
Windows, Toolbars, Menus, ComboBoxes, Tabcontrols, Editboxes, Images,
MessageBoxes, SkyBoxes, and how to parse XML files with the integrated XML
reader of the engine.
We start like in most other tutorials: Include all necessary header files, add
a comment to let the engine be linked with the right .lib file in Visual
Studio, and declare some global variables. We also add two 'using namespace'
statements, so we do not need to write the whole names of all classes. In this
tutorial, we use a lot stuff from the gui namespace.
*/
//#include "driverChoice.h"
#ifdef _MSC_VER
//#pragma comment(lib, "Irrlicht.lib")
#endif
/*
Some global variables used later on
*/
IrrlichtDevice* Device = 0;
char* StartUpModelFile;
char* MessageText;
char* Caption;
ISceneNode* Model = 0;
ISceneNode* SkyBox = 0;
bool isOctree = false;
bool isUseLight = false;
ICameraSceneNode* Camera[2] = {0, 0};
// Values used to identify individual GUI elements
enum {
  GUI_ID_DIALOG_ROOT_WINDOW  = 0x10000,
  GUI_ID_X_SCALE,
  GUI_ID_Y_SCALE,
  GUI_ID_Z_SCALE,
  GUI_ID_OPEN_MODEL,
  GUI_ID_SET_MODEL_ARCHIVE,
  GUI_ID_LOAD_AS_OCTREE,
  GUI_ID_SKY_BOX_VISIBLE,
  GUI_ID_TOGGLE_DEBUG_INFO,
  GUI_ID_DEBUG_OFF,
  GUI_ID_DEBUG_BOUNDING_BOX,
  GUI_ID_DEBUG_NORMALS,
  GUI_ID_DEBUG_SKELETON,
  GUI_ID_DEBUG_WIRE_OVERLAY,
  GUI_ID_DEBUG_HALF_TRANSPARENT,
  GUI_ID_DEBUG_BUFFERS_BOUNDING_BOXES,
  GUI_ID_DEBUG_ALL,
  GUI_ID_MODEL_MATERIAL_SOLID,
  GUI_ID_MODEL_MATERIAL_TRANSPARENT,
  GUI_ID_MODEL_MATERIAL_REFLECTION,
  GUI_ID_CAMERA_MAYA,
  GUI_ID_CAMERA_FIRST_PERSON,
  GUI_ID_POSITION_TEXT,
  GUI_ID_ABOUT,
  GUI_ID_QUIT,
  GUI_ID_TEXTUREFILTER,
  GUI_ID_SKIN_TRANSPARENCY,
  GUI_ID_SKIN_ANIMATION_FPS,
  GUI_ID_BUTTON_SET_SCALE,
  GUI_ID_BUTTON_SCALE_MUL10,
  GUI_ID_BUTTON_SCALE_DIV10,
  GUI_ID_BUTTON_OPEN_MODEL,
  GUI_ID_BUTTON_SHOW_ABOUT,
  GUI_ID_BUTTON_SHOW_TOOLBOX,
  GUI_ID_BUTTON_SELECT_ARCHIVE,
  GUI_ID_ANIMATION_INFO,
  // And some magic numbers
  MAX_FRAMERATE = 80,
  DEFAULT_FRAMERATE = 30
};
/*
Toggle between various cameras
*/
void setActiveCamera(ICameraSceneNode* newActive)
{
  if (0 == Device) {
    return;
  }
  ICameraSceneNode* active = Device->getSceneManager()->getActiveCamera();
  active->setInputReceiverEnabled(false);
  newActive->setInputReceiverEnabled(true);
  Device->getSceneManager()->setActiveCamera(newActive);
}
/*
  Set the skin transparency by changing the alpha values of all skin-colors
*/
void setSkinTransparency(s32 alpha, IGUISkin* skin)
{
  for (s32 i = 0; i < EGDC_COUNT ; ++i) {
    SColor col = skin->getColor((EGUI_DEFAULT_COLOR)i);
    col.setAlpha(alpha);
    skin->setColor((EGUI_DEFAULT_COLOR)i, col);
  }
}
/*
  Update the display of the model scaling
*/
void updateScaleInfo(ISceneNode* model)
{
  IGUIElement* toolboxWnd = Device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
  if (!toolboxWnd) {
    return;
  }
  if (!model) {
    toolboxWnd->getElementFromId(GUI_ID_X_SCALE, true)->setText("-");
    toolboxWnd->getElementFromId(GUI_ID_Y_SCALE, true)->setText("-");
    toolboxWnd->getElementFromId(GUI_ID_Z_SCALE, true)->setText("-");
  }
  else {
    vector3df scale = model->getScale();
    toolboxWnd->getElementFromId(GUI_ID_X_SCALE, true)->setText(char * (scale.X));
    toolboxWnd->getElementFromId(GUI_ID_Y_SCALE, true)->setText(char * (scale.Y));
    toolboxWnd->getElementFromId(GUI_ID_Z_SCALE, true)->setText(char * (scale.Z));
  }
}
/*
Function showAboutText() displays a messagebox with a caption and
a message text. The texts will be stored in the MessageText and Caption
variables at startup.
*/
void showAboutText()
{
  // create modal message box with the text
  // loaded from the xml file.
  Device->getGUIEnvironment()->addMessageBox(
      Caption, MessageText);
}
/*
Function loadModel() loads a model and displays it using an
addAnimatedMeshSceneNode and the scene manager. Nothing difficult. It also
displays a short message box, if the model could not be loaded.
*/
void loadModel(const c8* fn)
{
  // modify the name if it a .pk3 file
  char* filename(fn);
  char* extension;
  getFileNameExtension(extension, filename);
  extension.make_lower();
  // if a texture is loaded apply it to the current model..
  if (extension == ".jpg" || extension == ".pcx" ||
      extension == ".png" || extension == ".ppm" ||
      extension == ".pgm" || extension == ".pbm" ||
      extension == ".psd" || extension == ".tga" ||
      extension == ".bmp" || extension == ".wal" ||
      extension == ".rgb" || extension == ".rgba") {
    ITexture* texture =
        Device->getVideoDriver()->getTexture(filename);
    if (texture && Model) {
      // always reload texture
      Device->getVideoDriver()->removeTexture(texture);
      texture = Device->getVideoDriver()->getTexture(filename);
      Model->setMaterialTexture(0, texture);
    }
    return;
  }
  // if a archive is loaded add it to the FileArchive..
  else if (extension == ".pk3" || extension == ".zip" || extension == ".pak" || extension == ".npk") {
    Device->getFileSystem()->addFileArchive(filename);
    return;
  }
  // load a model into the engine
  if (Model) {
    Model->remove();
  }
  Model = 0;
  if (extension == ".irr") {
    ISceneNode** outNodes[100];
    int outNodes_size = 0;
    Device->getSceneManager()->loadScene(filename);
    outNodes_size = Device->getSceneManager()->getSceneNodesFromType(ESNT_ANIMATED_MESH, outNodes, 100);
    if (outNodes_size) {
      Model = outNodes[0];
    }
    return;
  }
  IAnimatedMesh* m = Device->getSceneManager()->getMesh(filename);
  if (!m) {
    // model could not be loaded
    if (StartUpModelFile != filename)
      Device->getGUIEnvironment()->addMessageBox(
          Caption, "The model could not be loaded. " \
          "Maybe it is not a supported file format.");
    return;
  }
  // set default material properties
  if (isOctree) {
    Model = Device->getSceneManager()->addOctreeSceneNode(m->getMesh(0));
  }
  else {
    IAnimatedMeshSceneNode* animModel = Device->getSceneManager()->addAnimatedMeshSceneNode(m);
    animModel->setAnimationSpeed(30);
    Model = animModel;
  }
  Model->setMaterialFlag(EMF_LIGHTING, isUseLight);
  Model->setMaterialFlag(EMF_NORMALIZE_NORMALS, isUseLight);
  //  Model->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
  Model->setDebugDataVisible(EDS_OFF);
  // we need to uncheck the menu entries. would be cool to fake a menu event, but
  // that's not so simple. so we do it brute force
  IGUIContextMenu* menu = (IGUIContextMenu*)Device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_TOGGLE_DEBUG_INFO, true);
  if (menu)
    for (int item = 1; item < 6; ++item) {
      menu->setItemChecked(item, false);
    }
  updateScaleInfo(Model);
}
/*
Function createToolBox() creates a toolbox window. In this simple mesh
viewer, this toolbox only contains a tab control with three edit boxes for
changing the scale of the displayed model.
*/
void createToolBox()
{
  // remove tool box if already there
  IGUIEnvironment* env = Device->getGUIEnvironment();
  IGUIElement* root = env->getRootGUIElement();
  IGUIElement* e = root->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
  if (e) {
    e->remove();
  }
  // create the toolbox window
  IGUIWindow* wnd = env->addWindow(IRECT(600, 45, 800, 480),
      false, "Toolset", 0, GUI_ID_DIALOG_ROOT_WINDOW);
  // create tab control and tabs
  IGUITabControl* tab = env->addTabControl(
      IRECT(2, 20, 800 - 602, 480 - 7), wnd, true, true);
  IGUITab* t1 = tab->addTab("Config");
  // add some edit boxes and a button to tab one
  env->addStaticText("Scale:",
      IRECT(10, 20, 60, 45), false, false, t1);
  env->addStaticText("X:", IRECT(22, 48, 40, 66), false, false, t1);
  env->addEditBox("1.0", IRECT(40, 46, 130, 66), true, t1, GUI_ID_X_SCALE);
  env->addStaticText("Y:", IRECT(22, 82, 40, 96), false, false, t1);
  env->addEditBox("1.0", IRECT(40, 76, 130, 96), true, t1, GUI_ID_Y_SCALE);
  env->addStaticText("Z:", IRECT(22, 108, 40, 126), false, false, t1);
  env->addEditBox("1.0", IRECT(40, 106, 130, 126), true, t1, GUI_ID_Z_SCALE);
  env->addButton(IRECT(10, 134, 85, 165), t1, GUI_ID_BUTTON_SET_SCALE, "Set");
  // quick scale buttons
  env->addButton(IRECT(65, 20, 95, 40), t1, GUI_ID_BUTTON_SCALE_MUL10, "* 10");
  env->addButton(IRECT(100, 20, 130, 40), t1, GUI_ID_BUTTON_SCALE_DIV10, "* 0.1");
  updateScaleInfo(Model);
  // add transparency control
  env->addStaticText("GUI Transparency Control:",
      IRECT(10, 200, 150, 225), true, false, t1);
  IGUIScrollBar* scrollbar = env->addScrollBar(true,
      IRECT(10, 225, 150, 240), t1, GUI_ID_SKIN_TRANSPARENCY);
  scrollbar->setMax(255);
  scrollbar->setPos(255);
  // add framerate control
  env->addStaticText(":", IRECT(10, 240, 150, 265), true, false, t1);
  env->addStaticText("Framerate:",
      IRECT(12, 240, 75, 265), false, false, t1);
  // current frame info
  env->addStaticText("", IRECT(75, 240, 200, 265), false, false, t1,
      GUI_ID_ANIMATION_INFO);
  scrollbar = env->addScrollBar(true,
      IRECT(10, 265, 150, 280), t1, GUI_ID_SKIN_ANIMATION_FPS);
  scrollbar->setMax(MAX_FRAMERATE);
  scrollbar->setMin(-MAX_FRAMERATE);
  scrollbar->setPos(DEFAULT_FRAMERATE);
  scrollbar->setSmallStep(1);
}
/*
Function updateToolBox() is called each frame to update dynamic information in
the toolbox.
*/
void updateToolBox()
{
  IGUIEnvironment* env = Device->getGUIEnvironment();
  IGUIElement* root = env->getRootGUIElement();
  IGUIElement* dlg = root->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
  if (!dlg) {
    return;
  }
  // update the info we have about the animation of the model
  IGUIStaticText*   aniInfo = (IGUIStaticText*)(dlg->getElementFromId(GUI_ID_ANIMATION_INFO, true));
  if (aniInfo) {
    if (Model && ESNT_ANIMATED_MESH == Model->getType()) {
      IAnimatedMeshSceneNode* animatedModel = (IAnimatedMeshSceneNode*)Model;
      char* str((s32)round_(animatedModel->getAnimationSpeed()));
      str += " Frame: ";
      str += char * ((s32)animatedModel->getFrameNr());
      aniInfo->setText(str);
    }
    else {
      aniInfo->setText("");
    }
  }
}
void onKillFocus()
{
  // Avoid that the FPS-camera continues moving when the user presses alt-tab while
  // moving the camera.
  const list<ISceneNodeAnimator*>& animators = Camera[1]->getAnimators();
  list<ISceneNodeAnimator*>::ConstIterator iter = animators.begin();
  while (iter != animators.end()) {
    if ((*iter)->getType() == ESNAT_CAMERA_FPS) {
      // we send a key-down event for all keys used by this animator
      ISceneNodeAnimatorCameraFPS* fpsAnimator = static_cast<ISceneNodeAnimatorCameraFPS*>(*iter);
      const SKeyMap* keyMap = fpsAnimator->getKeyMap();
      for (u32 i = 0; i < keyMap_size; ++i) {
        SEvent event;
        event.EventType = EET_KEY_INPUT_EVENT;
        event.KeyInput.Key = keyMap[i].KeyCode;
        event.KeyInput.PressedDown = false;
        fpsAnimator->OnEvent(event);
      }
    }
    ++iter;
  }
}
/*
Function hasModalDialog() checks if we currently have a modal dialog open.
*/
bool hasModalDialog()
{
  if (!Device) {
    return false;
  }
  IGUIEnvironment* env = Device->getGUIEnvironment();
  IGUIElement* focused = env->getFocus();
  while (focused) {
    if (focused->isVisible() && focused->hasType(EGUIET_MODAL_SCREEN)) {
      return true;
    }
    focused = focused->getParent();
  }
  return false;
}
/*
To get all the events sent by the GUI Elements, we need to create an event
receiver. This one is really simple. If an event occurs, it checks the id of
the caller and the event type, and starts an action based on these values. For
example, if a menu item with id GUI_ID_OPEN_MODEL was selected, it opens a file-open-dialog.
*/
class MyEventReceiver : public IEventReceiver
{
public:
  bool OnEvent(const SEvent& event) {
    // Escape swaps Camera Input
    if (event.EventType == EET_KEY_INPUT_EVENT &&
        event.KeyInput.PressedDown == false) {
      if (OnKeyUp(event.KeyInput.Key)) {
        return true;
      }
    }
    if (event.EventType == EET_GUI_EVENT) {
      s32 id = event.GUIEvent.Caller->getID();
      IGUIEnvironment* env = Device->getGUIEnvironment();
      switch (event.GUIEvent.EventType) {
      case EGET_MENU_ITEM_SELECTED:
        // a menu item was clicked
        OnMenuItemSelected((IGUIContextMenu*)event.GUIEvent.Caller);
        break;
      case EGET_FILE_SELECTED: {
        // load the model file, selected in the file open dialog
        IGUIFileOpenDialog* dialog =
            (IGUIFileOpenDialog*)event.GUIEvent.Caller;
        loadModel(char * (dialog->getFileName()));
      }
      break;
      case EGET_SCROLL_BAR_CHANGED:
        // control skin transparency
        if (id == GUI_ID_SKIN_TRANSPARENCY) {
          const s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
          setSkinTransparency(pos, env->getSkin());
        }
        // control animation speed
        else if (id == GUI_ID_SKIN_ANIMATION_FPS) {
          const s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
          if (ESNT_ANIMATED_MESH == Model->getType()) {
            ((IAnimatedMeshSceneNode*)Model)->setAnimationSpeed((f32)pos);
          }
        }
        break;
      case EGET_COMBO_BOX_CHANGED:
        // control anti-aliasing/filtering
        if (id == GUI_ID_TEXTUREFILTER) {
          OnTextureFilterSelected((IGUIComboBox*)event.GUIEvent.Caller);
        }
        break;
      case EGET_BUTTON_CLICKED:
        switch (id) {
        case GUI_ID_BUTTON_SET_SCALE: {
          // set scale
          IGUIElement* root = env->getRootGUIElement();
          vector3df scale;
          char* s;
          s = root->getElementFromId(GUI_ID_X_SCALE, true)->getText();
          scale.X = (f32)atof(s);
          s = root->getElementFromId(GUI_ID_Y_SCALE, true)->getText();
          scale.Y = (f32)atof(s);
          s = root->getElementFromId(GUI_ID_Z_SCALE, true)->getText();
          scale.Z = (f32)atof(s);
          if (Model) {
            Model->setScale(scale);
          }
          updateScaleInfo(Model);
        }
        break;
        case GUI_ID_BUTTON_SCALE_MUL10:
          if (Model) {
            Model->setScale(Model->getScale() * 10.f);
          }
          updateScaleInfo(Model);
          break;
        case GUI_ID_BUTTON_SCALE_DIV10:
          if (Model) {
            Model->setScale(Model->getScale() * 0.1f);
          }
          updateScaleInfo(Model);
          break;
        case GUI_ID_BUTTON_OPEN_MODEL:
          env->addFileOpenDialog("Please select a model file to open");
          break;
        case GUI_ID_BUTTON_SHOW_ABOUT:
          showAboutText();
          break;
        case GUI_ID_BUTTON_SHOW_TOOLBOX:
          createToolBox();
          break;
        case GUI_ID_BUTTON_SELECT_ARCHIVE:
          env->addFileOpenDialog("Please select your game archive/directory");
          break;
        }
        break;
      default:
        break;
      }
    }
    return false;
  }
  /*
    Handle key-up events
  */
  bool OnKeyUp(EKEY_CODE keyCode) {
    // Don't handle keys if we have a modal dialog open as it would lead
    // to unexpected application behaviour for the user.
    if (hasModalDialog()) {
      return false;
    }
    if (keyCode == KEY_ESCAPE) {
      if (Device) {
        ICameraSceneNode* camera =
            Device->getSceneManager()->getActiveCamera();
        if (camera) {
          camera->setInputReceiverEnabled(!camera->isInputReceiverEnabled());
        }
        return true;
      }
    }
    else if (keyCode == KEY_F1) {
      if (Device) {
        IGUIElement* elem = Device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_POSITION_TEXT);
        if (elem) {
          elem->setVisible(!elem->isVisible());
        }
      }
    }
    else if (keyCode == KEY_KEY_M) {
      if (Device) {
        Device->minimizeWindow();
      }
    }
    else if (keyCode == KEY_KEY_L) {
      isUseLight = !isUseLight;
      if (Model) {
        Model->setMaterialFlag(EMF_LIGHTING, isUseLight);
        Model->setMaterialFlag(EMF_NORMALIZE_NORMALS, isUseLight);
      }
    }
    return false;
  }
  /*
    Handle "menu item clicked" events.
  */
  void OnMenuItemSelected(IGUIContextMenu* menu) {
    s32 id = menu->getItemCommandId(menu->getSelectedItem());
    IGUIEnvironment* env = Device->getGUIEnvironment();
    switch (id) {
    case GUI_ID_OPEN_MODEL: // FilOnButtonSetScalinge -> Open Model
      env->addFileOpenDialog("Please select a model file to open");
      break;
    case GUI_ID_SET_MODEL_ARCHIVE: // File -> Set Model Archive
      env->addFileOpenDialog("Please select your game archive/directory");
      break;
    case GUI_ID_LOAD_AS_OCTREE: // File -> LoadAsOctree
      isOctree = !isOctree;
      menu->setItemChecked(menu->getSelectedItem(), isOctree);
      break;
    case GUI_ID_QUIT: // File -> Quit
      Device->closeDevice();
      break;
    case GUI_ID_SKY_BOX_VISIBLE: // View -> Skybox
      menu->setItemChecked(menu->getSelectedItem(), !menu->isItemChecked(menu->getSelectedItem()));
      SkyBox->setVisible(!SkyBox->isVisible());
      break;
    case GUI_ID_DEBUG_OFF: // View -> Debug Information
      menu->setItemChecked(menu->getSelectedItem() + 1, false);
      menu->setItemChecked(menu->getSelectedItem() + 2, false);
      menu->setItemChecked(menu->getSelectedItem() + 3, false);
      menu->setItemChecked(menu->getSelectedItem() + 4, false);
      menu->setItemChecked(menu->getSelectedItem() + 5, false);
      menu->setItemChecked(menu->getSelectedItem() + 6, false);
      if (Model) {
        Model->setDebugDataVisible(EDS_OFF);
      }
      break;
    case GUI_ID_DEBUG_BOUNDING_BOX: // View -> Debug Information
      menu->setItemChecked(menu->getSelectedItem(), !menu->isItemChecked(menu->getSelectedItem()));
      if (Model) {
        Model->setDebugDataVisible((E_DEBUG_SCENE_TYPE)(Model->isDebugDataVisible() ^ EDS_BBOX));
      }
      break;
    case GUI_ID_DEBUG_NORMALS: // View -> Debug Information
      menu->setItemChecked(menu->getSelectedItem(), !menu->isItemChecked(menu->getSelectedItem()));
      if (Model) {
        Model->setDebugDataVisible((E_DEBUG_SCENE_TYPE)(Model->isDebugDataVisible() ^ EDS_NORMALS));
      }
      break;
    case GUI_ID_DEBUG_SKELETON: // View -> Debug Information
      menu->setItemChecked(menu->getSelectedItem(), !menu->isItemChecked(menu->getSelectedItem()));
      if (Model) {
        Model->setDebugDataVisible((E_DEBUG_SCENE_TYPE)(Model->isDebugDataVisible() ^ EDS_SKELETON));
      }
      break;
    case GUI_ID_DEBUG_WIRE_OVERLAY: // View -> Debug Information
      menu->setItemChecked(menu->getSelectedItem(), !menu->isItemChecked(menu->getSelectedItem()));
      if (Model) {
        Model->setDebugDataVisible((E_DEBUG_SCENE_TYPE)(Model->isDebugDataVisible() ^ EDS_MESH_WIRE_OVERLAY));
      }
      break;
    case GUI_ID_DEBUG_HALF_TRANSPARENT: // View -> Debug Information
      menu->setItemChecked(menu->getSelectedItem(), !menu->isItemChecked(menu->getSelectedItem()));
      if (Model) {
        Model->setDebugDataVisible((E_DEBUG_SCENE_TYPE)(Model->isDebugDataVisible() ^ EDS_HALF_TRANSPARENCY));
      }
      break;
    case GUI_ID_DEBUG_BUFFERS_BOUNDING_BOXES: // View -> Debug Information
      menu->setItemChecked(menu->getSelectedItem(), !menu->isItemChecked(menu->getSelectedItem()));
      if (Model) {
        Model->setDebugDataVisible((E_DEBUG_SCENE_TYPE)(Model->isDebugDataVisible() ^ EDS_BBOX_BUFFERS));
      }
      break;
    case GUI_ID_DEBUG_ALL: // View -> Debug Information
      menu->setItemChecked(menu->getSelectedItem() - 1, true);
      menu->setItemChecked(menu->getSelectedItem() - 2, true);
      menu->setItemChecked(menu->getSelectedItem() - 3, true);
      menu->setItemChecked(menu->getSelectedItem() - 4, true);
      menu->setItemChecked(menu->getSelectedItem() - 5, true);
      menu->setItemChecked(menu->getSelectedItem() - 6, true);
      if (Model) {
        Model->setDebugDataVisible(EDS_FULL);
      }
      break;
    case GUI_ID_ABOUT: // Help->About
      showAboutText();
      break;
    case GUI_ID_MODEL_MATERIAL_SOLID: // View -> Material -> Solid
      if (Model) {
        Model->setMaterialType(EMT_SOLID);
      }
      break;
    case GUI_ID_MODEL_MATERIAL_TRANSPARENT: // View -> Material -> Transparent
      if (Model) {
        Model->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
      }
      break;
    case GUI_ID_MODEL_MATERIAL_REFLECTION: // View -> Material -> Reflection
      if (Model) {
        Model->setMaterialType(EMT_SPHERE_MAP);
      }
      break;
    case GUI_ID_CAMERA_MAYA:
      setActiveCamera(Camera[0]);
      break;
    case GUI_ID_CAMERA_FIRST_PERSON:
      setActiveCamera(Camera[1]);
      break;
    }
  }
  /*
    Handle the event that one of the texture-filters was selected in the corresponding combobox.
  */
  void OnTextureFilterSelected(IGUIComboBox* combo) {
    s32 pos = combo->getSelected();
    switch (pos) {
    case 0:
      if (Model) {
        Model->setMaterialFlag(EMF_BILINEAR_FILTER, false);
        Model->setMaterialFlag(EMF_TRILINEAR_FILTER, false);
        Model->setMaterialFlag(EMF_ANISOTROPIC_FILTER, false);
      }
      break;
    case 1:
      if (Model) {
        Model->setMaterialFlag(EMF_BILINEAR_FILTER, true);
        Model->setMaterialFlag(EMF_TRILINEAR_FILTER, false);
      }
      break;
    case 2:
      if (Model) {
        Model->setMaterialFlag(EMF_BILINEAR_FILTER, false);
        Model->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
      }
      break;
    case 3:
      if (Model) {
        Model->setMaterialFlag(EMF_ANISOTROPIC_FILTER, true);
      }
      break;
    case 4:
      if (Model) {
        Model->setMaterialFlag(EMF_ANISOTROPIC_FILTER, false);
      }
      break;
    }
  }
};
/*
Most of the hard work is done. We only need to create the Irrlicht Engine
device and all the buttons, menus and toolbars. We start up the engine as
usual, using createDevice(). To make our application catch events, we set our
eventreceiver as parameter. As you can see, there is also a call to
IrrlichtDevice::setResizeable(). This makes the render window resizeable, which
is quite useful for a mesh viewer.
*/
int main(int argc, char* argv[])
{
  // ask user for driver
  E_DRIVER_TYPE driverType = driverChoiceConsole();
  if (driverType == EDT_COUNT) {
    return 1;
  }
  // create device and exit if creation failed
  MyEventReceiver receiver;
  Device = createDevice(driverType, dimension2du(800, 600),
      16, false, false, false, &receiver);
  if (Device == 0) {
    return 1;  // could not create selected driver.
  }
  Device->setResizable(true);
  Device->setWindowCaption("Irrlicht Engine - Loading...");
  IVideoDriver* driver = Device->getVideoDriver();
  IGUIEnvironment* env = Device->getGUIEnvironment();
  ISceneManager* smgr = Device->getSceneManager();
  smgr->getParameters()->setAttribute(COLLADA_CREATE_SCENE_INSTANCES, true);
  driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);
  smgr->addLightSceneNode(0, f3VECTOR(200, 200, 200),
      SColorf(1.0f, 1.0f, 1.0f), 2000);
  smgr->setAmbientLight(SColorf(0.3f, 0.3f, 0.3f));
  // add our media directory as "search path"
  Device->getFileSystem()->addFileArchive("../../media/");
  /*
  The next step is to read the configuration file. It is stored in the xml
  format and looks a little bit like this:
  @verbatim
  <?xml version="1.0"?>
  <config>
    <startUpModel file="some filename" />
    <messageText caption="Irrlicht Engine Mesh Viewer">
      Hello!
    </messageText>
  </config>
  @endverbatim
  We need the data stored in there to be written into the global variables
  StartUpModelFile, MessageText and Caption. This is now done using the
  Irrlicht Engine integrated XML parser:
  */
  // read configuration from xml file
  IXMLReader* xml = Device->getFileSystem()->createXMLReader("config.xml");
  while (xml && xml->read()) {
    switch (xml->getNodeType()) {
    case EXN_TEXT:
      // in this xml file, the only text which occurs is the
      // messageText
      MessageText = xml->getNodeData();
      break;
    case EXN_ELEMENT: {
      if (char * ("startUpModel") == xml->getNodeName()) {
        StartUpModelFile = xml->getAttributeValue("file");
      }
      else if (char * ("messageText") == xml->getNodeName()) {
        Caption = xml->getAttributeValue("caption");
      }
    }
    break;
    default:
      break;
    }
  }
  if (xml) {
    xml->drop();  // don't forget to delete the xml reader
  }
  if (argc > 1) {
    StartUpModelFile = argv[1];
  }
  /*
  That wasn't difficult. Now we'll set a nicer font and create the Menu.
  It is possible to create submenus for every menu item. The call
  menu->addItem("File", -1, true, true); for example adds a new menu
  Item with the name "File" and the id -1. The following parameter says
  that the menu item should be enabled, and the last one says, that there
  should be a submenu. The submenu can now be accessed with
  menu->getSubMenu(0), because the "File" entry is the menu item with
  index 0.
  */
  // set a nicer font
  IGUISkin* skin = env->getSkin();
  IGUIFont* font = env->getFont("fonthaettenschweiler.bmp");
  if (font) {
    skin->setFont(font);
  }
  // create menu
  IGUIContextMenu* menu = env->addMenu();
  menu->addItem("File", -1, true, true);
  menu->addItem("View", -1, true, true);
  menu->addItem("Camera", -1, true, true);
  menu->addItem("Help", -1, true, true);
  IGUIContextMenu* submenu;
  submenu = menu->getSubMenu(0);
  submenu->addItem("Open Model File & Texture...", GUI_ID_OPEN_MODEL);
  submenu->addItem("Set Model Archive...", GUI_ID_SET_MODEL_ARCHIVE);
  submenu->addItem("Load as isOctree", GUI_ID_LOAD_AS_OCTREE);
  submenu->addSeparator();
  submenu->addItem("Quit", GUI_ID_QUIT);
  submenu = menu->getSubMenu(1);
  submenu->addItem("sky box visible", GUI_ID_SKY_BOX_VISIBLE, true, false, true);
  submenu->addItem("toggle model debug information", GUI_ID_TOGGLE_DEBUG_INFO, true, true);
  submenu->addItem("model material", -1, true, true);
  submenu = submenu->getSubMenu(1);
  submenu->addItem("Off", GUI_ID_DEBUG_OFF);
  submenu->addItem("Bounding Box", GUI_ID_DEBUG_BOUNDING_BOX);
  submenu->addItem("Normals", GUI_ID_DEBUG_NORMALS);
  submenu->addItem("Skeleton", GUI_ID_DEBUG_SKELETON);
  submenu->addItem("Wire overlay", GUI_ID_DEBUG_WIRE_OVERLAY);
  submenu->addItem("Half-Transparent", GUI_ID_DEBUG_HALF_TRANSPARENT);
  submenu->addItem("Buffers bounding boxes", GUI_ID_DEBUG_BUFFERS_BOUNDING_BOXES);
  submenu->addItem("All", GUI_ID_DEBUG_ALL);
  submenu = menu->getSubMenu(1)->getSubMenu(2);
  submenu->addItem("Solid", GUI_ID_MODEL_MATERIAL_SOLID);
  submenu->addItem("Transparent", GUI_ID_MODEL_MATERIAL_TRANSPARENT);
  submenu->addItem("Reflection", GUI_ID_MODEL_MATERIAL_REFLECTION);
  submenu = menu->getSubMenu(2);
  submenu->addItem("Maya Style", GUI_ID_CAMERA_MAYA);
  submenu->addItem("First Person", GUI_ID_CAMERA_FIRST_PERSON);
  submenu = menu->getSubMenu(3);
  submenu->addItem("About", GUI_ID_ABOUT);
  /*
  Below the menu we want a toolbar, onto which we can place colored
  buttons and important looking stuff like a senseless combobox.
  */
  // create toolbar
  IGUIToolBar* bar = env->addToolBar();
  ITexture* image = driver->getTexture("open.png");
  bar->addButton(GUI_ID_BUTTON_OPEN_MODEL, 0, "Open a model", image, 0, false, true);
  image = driver->getTexture("tools.png");
  bar->addButton(GUI_ID_BUTTON_SHOW_TOOLBOX, 0, "Open Toolset", image, 0, false, true);
  image = driver->getTexture("zip.png");
  bar->addButton(GUI_ID_BUTTON_SELECT_ARCHIVE, 0, "Set Model Archive", image, 0, false, true);
  image = driver->getTexture("help.png");
  bar->addButton(GUI_ID_BUTTON_SHOW_ABOUT, 0, "Open Help", image, 0, false, true);
  // create a combobox for texture filters
  IGUIComboBox* box = env->addComboBox(IRECT(250, 4, 350, 23), bar, GUI_ID_TEXTUREFILTER);
  box->addItem("No filtering");
  box->addItem("Bilinear");
  box->addItem("Trilinear");
  box->addItem("Anisotropic");
  box->addItem("Isotropic");
  /*
  To make the editor look a little bit better, we disable transparent gui
  elements, and add an Irrlicht Engine logo. In addition, a text showing
  the current frames per second value is created and the window caption is
  changed.
  */
  // disable alpha
  for (s32 i = 0; i < EGDC_COUNT ; ++i) {
    SColor col = env->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
    col.setAlpha(255);
    env->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
  }
  // add a tabcontrol
  createToolBox();
  // create fps text
  IGUIStaticText* fpstext = env->addStaticText("",
      IRECT(400, 4, 570, 23), true, false, bar);
  IGUIStaticText* postext = env->addStaticText("",
      IRECT(10, 50, 470, 80), false, false, 0, GUI_ID_POSITION_TEXT);
  postext->setVisible(false);
  // set window caption
  Caption += " - [";
  Caption += driver->getName();
  Caption += "]";
  Device->setWindowCaption(Caption);
  /*
  That's nearly the whole application. We simply show the about message
  box at start up, and load the first model. To make everything look
  better, a skybox is created and a user controlled camera, to make the
  application a little bit more interactive. Finally, everything is drawn
  in a standard drawing loop.
  */
  // show about message box and load default model
  if (argc == 1) {
    showAboutText();
  }
  loadModel(StartUpModelFile);
  // add skybox
  SkyBox = smgr->addSkyBoxSceneNode(
      driver->getTexture("irrlicht2_up.jpg"),
      driver->getTexture("irrlicht2_dn.jpg"),
      driver->getTexture("irrlicht2_lf.jpg"),
      driver->getTexture("irrlicht2_rt.jpg"),
      driver->getTexture("irrlicht2_ft.jpg"),
      driver->getTexture("irrlicht2_bk.jpg"));
  // add a camera scene node
  Camera[0] = smgr->addCameraSceneNodeMaya();
  Camera[0]->setFarValue(20000.f);
  // Maya cameras reposition themselves relative to their target, so target the location
  // where the mesh scene node is placed.
  Camera[0]->setTarget(f3VECTOR(0, 30, 0));
  Camera[1] = smgr->addCameraSceneNodeFPS();
  Camera[1]->setFarValue(20000.f);
  Camera[1]->setPosition(f3VECTOR(0, 0, -70));
  Camera[1]->setTarget(f3VECTOR(0, 30, 0));
  setActiveCamera(Camera[0]);
  // load the irrlicht engine logo
  IGUIImage* img =
      env->addImage(driver->getTexture("irrlichtlogo2.png"),
          position2di(10, driver->getScreenSize().Height - 128));
  // lock the logo's edges to the bottom left corner of the screen
  img->setAlignment(EGUIA_UPPERLEFT, EGUIA_UPPERLEFT,
      EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT);
  // remember state so we notice when the window does lose the focus
  bool hasFocus = Device->isWindowFocused();
  // draw everything
  while (Device->run() && driver) {
    // Catch focus changes (workaround until Irrlicht has events for this)
    bool focused = Device->isWindowFocused();
    if (hasFocus && !focused) {
      onKillFocus();
    }
    hasFocus = focused;
    if (Device->isWindowActive()) {
      driver->beginScene(true, true, _ARGB(150, 50, 50, 50));
      smgr->drawAll();
      env->drawAll();
      driver->endScene();
      // update information about current frame-rate
      char* str("FPS: ");
      str.append(char * (driver->getFPS()));
      str += " Tris: ";
      str.append(char * (driver->getPrimitiveCountDrawn()));
      fpstext->setText(str);
      // update information about the active camera
      ICameraSceneNode* cam = Device->getSceneManager()->getActiveCamera();
      str = "Pos: ";
      str.append(char * (cam->getPosition().X));
      str += " ";
      str.append(char * (cam->getPosition().Y));
      str += " ";
      str.append(char * (cam->getPosition().Z));
      str += " Tgt: ";
      str.append(char * (cam->getTarget().X));
      str += " ";
      str.append(char * (cam->getTarget().Y));
      str += " ";
      str.append(char * (cam->getTarget().Z));
      postext->setText(str);
      // update the tool dialog
      updateToolBox();
    }
    else {
      Device->yield();
    }
  }
  Device->drop();
  return 0;
}
/*
**/

