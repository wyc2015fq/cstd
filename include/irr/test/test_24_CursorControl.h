/** Example 024 CursorControl
Show how to modify cursors and offer some useful tool-functions for creating cursors.
It can also be used for experiments with the mouse in general.
*/
//#include "driverChoice.h"
#ifdef _IRR_WINDOWS_
//#pragma comment(lib, "Irrlicht.lib")
#endif
const int DELAY_TIME = 3000;
enum ETimerAction {
  ETA_MOUSE_VISIBLE,
  ETA_MOUSE_INVISIBLE,
};
/*
  Structure to allow delayed execution of some actions.
*/
struct TimerAction {
  u32 TargetTime;
  ETimerAction Action;
};
/*
*/
struct SAppContext {
  SAppContext()
    : Device(0), InfoStatic(0), EventBox(0), CursorBox(0), SpriteBox(0)
    , ButtonSetVisible(0), ButtonSetInvisible(0), ButtonSimulateBadFps(0)
    , ButtonChangeIcon(0)
    , SimulateBadFps(false) {
  }
  void update() {
    if (!Device) {
      return;
    }
    u32 timeNow = Device->getTimer()->getTime();
    for (u32 i = 0; i < TimerActions_size; ++i) {
      if (timeNow >= TimerActions[i].TargetTime) {
        runTimerAction(TimerActions[i]);
        TimerActions.erase(i);
      }
      else {
        ++i;
      }
    }
  }
  void runTimerAction(const TimerAction& action) {
    if (ETA_MOUSE_VISIBLE == action.Action) {
      Device->getCursorControl()->setVisible(true);
      ButtonSetVisible->setEnabled(true);
    }
    else if (ETA_MOUSE_INVISIBLE == action.Action) {
      Device->getCursorControl()->setVisible(false);
      ButtonSetInvisible->setEnabled(true);
    }
  }
  /*
    Add another icon which the user can click and select as cursor later on.
  */
  void addIcon(const stringc& name, const SCursorSprite& sprite, bool addCursor = true) {
    // Sprites are just icons - not yet cursors. They can be displayed by Irrlicht sprite functions and be used to create cursors.
    SpriteBox->addItem(name, sprite.SpriteId);
    Sprites.push_back(sprite);
    // create the cursor together with the icon?
    if (addCursor) {
      /* Here we create a hardware cursor from a sprite */
      Device->getCursorControl()->addIcon(sprite);
      // ... and add it to the cursors selection listbox to the other system cursors.
      CursorBox->addItem(name);
    }
  }
  IrrlichtDevice* Device;
  IGUIStaticText* InfoStatic;
  IGUIListBox* EventBox;
  IGUIListBox* CursorBox;
  IGUIListBox* SpriteBox;
  IGUIButton* ButtonSetVisible;
  IGUIButton* ButtonSetInvisible;
  IGUIButton* ButtonSimulateBadFps;
  IGUIButton* ButtonChangeIcon;
  TimerAction* TimerActions;
  int TimerActions_size;
  bool SimulateBadFps;
  SCursorSprite* Sprites;
  int Sprites_size;
};
/*
  Helper function to print mouse event names into a stringc
*/
void PrintMouseEventName(const SEvent& event, stringc& result)
{
  switch (event.MouseInput.Event) {
  case EMIE_LMOUSE_PRESSED_DOWN:
    result += stringc("EMIE_LMOUSE_PRESSED_DOWN");
    break;
  case EMIE_RMOUSE_PRESSED_DOWN:
    result += stringc("EMIE_RMOUSE_PRESSED_DOWN");
    break;
  case EMIE_MMOUSE_PRESSED_DOWN:
    result += stringc("EMIE_MMOUSE_PRESSED_DOWN");
    break;
  case EMIE_LMOUSE_LEFT_UP:
    result += stringc("EMIE_LMOUSE_LEFT_UP");
    break;
  case EMIE_RMOUSE_LEFT_UP:
    result += stringc("EMIE_RMOUSE_LEFT_UP");
    break;
  case EMIE_MMOUSE_LEFT_UP:
    result += stringc("EMIE_MMOUSE_LEFT_UP");
    break;
  case EMIE_MOUSE_MOVED:
    result += stringc("EMIE_MOUSE_MOVED");
    break;
  case EMIE_MOUSE_WHEEL:
    result += stringc("EMIE_MOUSE_WHEE");
    break;
  case EMIE_LMOUSE_DOUBLE_CLICK:
    result += stringc("EMIE_LMOUSE_DOUBLE_CLICK");
    break;
  case EMIE_RMOUSE_DOUBLE_CLICK:
    result += stringc("EMIE_RMOUSE_DOUBLE_CLICK");
    break;
  case EMIE_MMOUSE_DOUBLE_CLICK:
    result += stringc("EMIE_MMOUSE_DOUBLE_CLICK");
    break;
  case EMIE_LMOUSE_TRIPLE_CLICK:
    result += stringc("EMIE_LMOUSE_TRIPLE_CLICK");
    break;
  case EMIE_RMOUSE_TRIPLE_CLICK:
    result += stringc("EMIE_RMOUSE_TRIPLE_CLICK");
    break;
  case EMIE_MMOUSE_TRIPLE_CLICK:
    result += stringc("EMIE_MMOUSE_TRIPLE_CLICK");
    break;
  default:
    break;
  }
}
/*
  Helper function to print all the state information which get from a mouse-event into a stringc
*/
void PrintMouseState(const SEvent& event, stringc& result)
{
  result += stringc("X: ");
  result += stringc(event.MouseInput.X);
  result += stringc("\n");
  result += stringc("Y: ");
  result += stringc(event.MouseInput.Y);
  result += stringc("\n");
  result += stringc("Wheel: ");
  result += stringc(event.MouseInput.Wheel);
  result += stringc("\n");
  result += stringc("Shift: ");
  if (event.MouseInput.Shift) {
    result += stringc("true\n");
  }
  else {
    result += stringc("false\n");
  }
  result += stringc("Control: ");
  if (event.MouseInput.Control) {
    result += stringc("true\n");
  }
  else {
    result += stringc("false\n");
  }
  result += stringc("ButtonStates: ");
  result += stringc(event.MouseInput.ButtonStates);
  result += stringc("\n");
  result += stringc("isLeftPressed: ");
  if (event.MouseInput.isLeftPressed()) {
    result += stringc("true\n");
  }
  else {
    result += stringc("false\n");
  }
  result += stringc("isRightPressed: ");
  if (event.MouseInput.isRightPressed()) {
    result += stringc("true\n");
  }
  else {
    result += stringc("false\n");
  }
  result += stringc("isMiddlePressed: ");
  if (event.MouseInput.isMiddlePressed()) {
    result += stringc("true\n");
  }
  else {
    result += stringc("false\n");
  }
  result += stringc("Event: ");
  PrintMouseEventName(event, result);
  result += stringc("\n");
}
/*
  A typical event receiver.
*/
class MyEventReceiver : public IEventReceiver
{
public:
  MyEventReceiver(SAppContext& context) : Context(context) { }
  bool OnEvent(const SEvent& event) {
    if (event.EventType == EET_GUI_EVENT) {
      switch (event.GUIEvent.EventType) {
      case EGET_BUTTON_CLICKED: {
        u32 timeNow = Context.Device->getTimer()->getTime();
        TimerAction action;
        action.TargetTime = timeNow + DELAY_TIME;
        if (event.GUIEvent.Caller == Context.ButtonSetVisible) {
          action.Action = ETA_MOUSE_VISIBLE;
          Context.TimerActions.push_back(action);
          Context.ButtonSetVisible->setEnabled(false);
        }
        else if (event.GUIEvent.Caller == Context.ButtonSetInvisible) {
          action.Action = ETA_MOUSE_INVISIBLE;
          Context.TimerActions.push_back(action);
          Context.ButtonSetInvisible->setEnabled(false);
        }
        else if (event.GUIEvent.Caller == Context.ButtonSimulateBadFps) {
          Context.SimulateBadFps = Context.ButtonSimulateBadFps->isPressed();
        }
        else if (event.GUIEvent.Caller == Context.ButtonChangeIcon) {
          /*
            Replace an existing cursor icon by another icon.
            The user has to select both - the icon which should be replaced and the icon which will replace it.
          */
          s32 selectedCursor = Context.CursorBox->getSelected();
          s32 selectedSprite = Context.SpriteBox->getSelected();
          if (selectedCursor >= 0 && selectedSprite >= 0) {
            /*
              This does replace the icon.
            */
            Context.Device->getCursorControl()->changeIcon((ECURSOR_ICON)selectedCursor, Context.Sprites[selectedSprite]);
            /*
              Do also show the new icon.
            */
            Context.Device->getCursorControl()->setActiveIcon(ECURSOR_ICON(selectedCursor));
          }
        }
      }
      break;
      case EGET_LISTBOX_CHANGED:
      case EGET_LISTBOX_SELECTED_AGAIN: {
        if (event.GUIEvent.Caller == Context.CursorBox) {
          /*
            Find out which cursor the user selected
          */
          s32 selected = Context.CursorBox->getSelected();
          if (selected >= 0) {
            /*
              Here we set the new cursor icon which will now be used within our window.
            */
            Context.Device->getCursorControl()->setActiveIcon(ECURSOR_ICON(selected));
          }
        }
      }
      break;
      default:
        break;
      }
    }
    if (event.EventType == EET_MOUSE_INPUT_EVENT) {
      stringc infoText;
      PrintMouseState(event, infoText);
      Context.InfoStatic->setText(infoText);
      if (event.MouseInput.Event != EMIE_MOUSE_MOVED && event.MouseInput.Event != EMIE_MOUSE_WHEEL) { // no spam
        infoText = "";
        PrintMouseEventName(event, infoText);
        Context.EventBox->insertItem(0, infoText, -1);
      }
    }
    return false;
  }
private:
  SAppContext& Context;
};
/*
  Use several imagefiles as animation frames for a sprite which can be used as cursor icon.
  The images in those files all need to have the same size.
  Return sprite index on success or -1 on failure
*/
s32 AddAnimatedIconToSpriteBank(IGUISpriteBank* spriteBank, IVideoDriver* driver,  const strv_t* files, u32 frameTime)
{
  if (!spriteBank || !driver || !files_size) {
    return -1;
  }
  ITexture* tex = driver->getTexture(files[0]);
  if (tex) {
    IRECT* spritePositions = spriteBank->getPositions();
    u32 idxRect = spritePositions_size;
    spritePositions.push_back(IRECT(0, 0, tex->getSize().Width, tex->getSize().Height));
    SGUISprite sprite;
    sprite.frameTime = frameTime;
    SGUISprite* sprites = spriteBank->getSprites();
    u32 startIdx = spriteBank->getTextureCount();
    for (u32 f = 0; f < files_size; ++f) {
      tex = driver->getTexture(files[f]);
      if (tex) {
        spriteBank->addTexture(driver->getTexture(files[f]));
        SGUISpriteFrame frame;
        frame.rectNumber = idxRect;
        frame.textureNumber = startIdx + f;
        sprite.Frames.push_back(frame);
      }
    }
    sprites.push_back(sprite);
    return sprites_size - 1;
  }
  return -1;
}
/*
  Use several images within one imagefile as animation frames for a sprite which can be used as cursor icon
  The sizes of the icons within that file all need to have the same size
  Return sprite index on success or -1 on failure
*/
s32 AddAnimatedIconToSpriteBank(IGUISpriteBank* spriteBank, IVideoDriver* driver,  const char* file, const IRECT* rects, int nrect, u32 frameTime)
{
  if (!spriteBank || !driver || !rects_size) {
    return -1;
  }
  ITexture* tex = driver->getTexture(file);
  if (tex) {
    IRECT* spritePositions = spriteBank->getPositions();
    u32 idxRect = spritePositions_size;
    u32 idxTex = spriteBank->getTextureCount();
    spriteBank->addTexture(tex);
    SGUISprite sprite;
    sprite.frameTime = frameTime;
    SGUISprite* sprites = spriteBank->getSprites();
    for (u32 i = 0; i < rects_size; ++i) {
      spritePositions.push_back(rects[i]);
      SGUISpriteFrame frame;
      frame.rectNumber = idxRect + i;
      frame.textureNumber = idxTex;
      sprite.Frames.push_back(frame);
    }
    sprites.push_back(sprite);
    return sprites_size - 1;
  }
  return -1;
}
/*
  Create a non-animated icon from the given file and position and put it into the spritebank.
  We can use this icon later on in a cursor.
*/
s32 AddIconToSpriteBank(IGUISpriteBank* spriteBank, IVideoDriver* driver,  const char* file, const IRECT& rect)
{
  if (!spriteBank || !driver) {
    return -1;
  }
  ITexture* tex = driver->getTexture(file);
  if (tex) {
    IRECT* spritePositions = spriteBank->getPositions();
    spritePositions.push_back(rect);
    SGUISprite* sprites = spriteBank->getSprites();
    spriteBank->addTexture(tex);
    SGUISpriteFrame frame;
    frame.rectNumber = spritePositions_size - 1;
    frame.textureNumber = spriteBank->getTextureCount() - 1;
    SGUISprite sprite;
    sprite.frameTime = 0;
    sprite.Frames.push_back(frame);
    sprites.push_back(sprite);
    return sprites_size - 1;
  }
  return -1;
}
int main()
{
  E_DRIVER_TYPE driverType = driverChoiceConsole();
  if (driverType == EDT_COUNT) {
    return 1;
  }
  IrrlichtDevice* device = createDevice(driverType, dimension2du(640, 480));
  if (device == 0) {
    return 1;  // could not create selected driver.
  }
  // It's sometimes of interest to know how the mouse behaves after a resize
  device->setResizable(true);
  device->setWindowCaption("Cursor control - Irrlicht engine tutorial");
  IVideoDriver* driver = device->getVideoDriver();
  IGUIEnvironment* env = device->getGUIEnvironment();
  IGUISpriteBank* SpriteBankIcons;
  SAppContext context;
  context.Device = device;
  rect< s32 > rectInfoStatic(10, 10, 200, 200);
  env->addStaticText("Cursor state information", rectInfoStatic, true, true);
  rectInfoStatic.UpperLeftCorner += dimension2di(0, 15);
  context.InfoStatic = env->addStaticText("", rectInfoStatic, true, true);
  rect< s32 > rectEventBox(10, 210, 200, 400);
  env->addStaticText("click events (new on top)", rectEventBox, true, true);
  rectEventBox.UpperLeftCorner += dimension2di(0, 15);
  context.EventBox = env->addListBox(rectEventBox);
  rect< s32 > rectCursorBox(210, 10, 400, 250);
  env->addStaticText("cursors, click to set the active one", rectCursorBox, true, true);
  rectCursorBox.UpperLeftCorner += dimension2di(0, 15);
  context.CursorBox = env->addListBox(rectCursorBox);
  rect< s32 > rectSpriteBox(210, 260, 400, 400);
  env->addStaticText("sprites", rectSpriteBox, true, true);
  rectSpriteBox.UpperLeftCorner += dimension2di(0, 15);
  context.SpriteBox = env->addListBox(rectSpriteBox);
  context.ButtonSetVisible = env->addButton(IRECT(410, 20, 560, 40), 0, -1, "set visible (delayed)");
  context.ButtonSetInvisible = env->addButton(IRECT(410, 50, 560, 70), 0, -1, "set invisible (delayed)");
  context.ButtonSimulateBadFps = env->addButton(IRECT(410, 80, 560, 100), 0, -1, "simulate bad FPS");
  context.ButtonSimulateBadFps->setIsPushButton(true);
  context.ButtonChangeIcon = env->addButton(IRECT(410, 140, 560, 160), 0, -1, "replace cursor icon\n(cursor+sprite must be selected)");
  // set the names for all the system cursors
  for (int i = 0; i < (int)ECI_COUNT; ++i) {
    context.CursorBox->addItem(stringc(GUICursorIconNames[i]));
  }
  /*
    Create sprites which then can be used as cursor icons.
   */
  SpriteBankIcons = env->addEmptySpriteBank(char * ("cursor_icons"));
  context.SpriteBox->setSpriteBank(SpriteBankIcons);
  // create one animated icon from several files
  strv_t files;
  files.push_back(char * ("../../media/icon_crosshairs16x16bw1.png"));
  files.push_back(char * ("../../media/icon_crosshairs16x16bw2.png"));
  files.push_back(char * ("../../media/icon_crosshairs16x16bw3.png"));
  files.push_back(char * ("../../media/icon_crosshairs16x16bw3.png"));
  files.push_back(char * ("../../media/icon_crosshairs16x16bw2.png"));
  SCursorSprite spriteBw; // the sprite + some additional information needed for cursors
  spriteBw.SpriteId = AddAnimatedIconToSpriteBank(SpriteBankIcons, driver, files, 200);
  spriteBw.SpriteBank = SpriteBankIcons;
  spriteBw.HotSpot = position2di(7, 7);
  context.addIcon("crosshair_bw", spriteBw);
  // create one animated icon from one file
  IRECT* iconRects;
  iconRects.push_back(IRECT(0, 0, 16, 16));
  iconRects.push_back(IRECT(16, 0, 32, 16));
  iconRects.push_back(IRECT(0, 16, 16, 32));
  iconRects.push_back(IRECT(0, 16, 16, 32));
  iconRects.push_back(IRECT(16, 0, 32, 16));
  SCursorSprite spriteCol;  // the sprite + some additional information needed for cursors
  spriteCol.SpriteId = AddAnimatedIconToSpriteBank(SpriteBankIcons, driver, char * ("../../media/icon_crosshairs16x16col.png"), iconRects, 200);
  spriteCol.HotSpot = position2di(7, 7);
  spriteCol.SpriteBank = SpriteBankIcons;
  context.addIcon("crosshair_colored", spriteCol);
  // Create some non-animated icons
  IRECT rectIcon;
  SCursorSprite spriteNonAnimated(SpriteBankIcons, 0, position2di(7, 7));
  rectIcon = IRECT(0, 0, 16, 16);
  spriteNonAnimated.SpriteId = AddIconToSpriteBank(SpriteBankIcons, driver, char * ("../../media/icon_crosshairs16x16col.png"), rectIcon);
  context.addIcon("crosshair_col1", spriteNonAnimated, false);
  rectIcon = IRECT(16, 0, 32, 16);
  spriteNonAnimated.SpriteId = AddIconToSpriteBank(SpriteBankIcons, driver, char * ("../../media/icon_crosshairs16x16col.png"), rectIcon);
  context.addIcon("crosshair_col2", spriteNonAnimated, false);
  rectIcon = IRECT(0, 16, 16, 32);
  spriteNonAnimated.SpriteId = AddIconToSpriteBank(SpriteBankIcons, driver, char * ("../../media/icon_crosshairs16x16col.png"), rectIcon);
  context.addIcon("crosshair_col3", spriteNonAnimated, false);
  MyEventReceiver receiver(context);
  device->setEventReceiver(&receiver);
  while (device->run() && driver) {
    // if (device->isWindowActive())
    {
      u32 realTimeNow = device->getTimer()->getRealTime();
      context.update();
      driver->beginScene(true, true, _ARGB(0, 200, 200, 200));
      env->drawAll();
      // draw custom sprite with Irrlicht functions for comparison. It should usually look the same as the cursors.
      if (context.SpriteBox) {
        s32 selectedSprite = context.SpriteBox->getSelected();
        if (selectedSprite >= 0 && context.Sprites[selectedSprite].SpriteId >= 0) {
          SpriteBankIcons->draw2DSprite(u32(context.Sprites[selectedSprite].SpriteId), position2di(580, 140), 0, _ARGB(255, 255, 255, 255), 0, realTimeNow);
        }
      }
      driver->endScene();
    }
    // By simulating bad fps we can find out if hardware-support for cursors works or not. If it works the cursor will move as usual,while it otherwise will just update with 2 fps now.
    if (context.SimulateBadFps) {
      device->sleep(500); // 2 fps
    }
    else {
      device->sleep(10);
    }
  }
  device->drop();
  return 0;
}
/*
**/

