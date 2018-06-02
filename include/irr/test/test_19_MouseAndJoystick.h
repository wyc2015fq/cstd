/** Example 019 Mouse and Joystick
This tutorial builds on example 04.Movement which showed how to
handle keyboard events in Irrlicht.  Here we'll handle mouse events
and joystick events, if you have a joystick connected and a device
that supports joysticks.  These are currently Windows, Linux and SDL
devices.
*/
#ifdef _MSC_VER
// We'll define this to stop MSVC complaining about sprintf().
#define _CRT_SECURE_NO_WARNINGS
//#pragma comment(lib, "Irrlicht.lib")
#endif
//#include "driverChoice.h"
/*
Just as we did in example 04.Movement, we'll store the latest state of the
mouse and the first joystick, updating them as we receive events.
*/
class MyEventReceiver : public IEventReceiver
{
public:
  // We'll create a struct to record info on the mouse state
  struct SMouseState {
    position2di Position;
    bool LeftButtonDown;
    SMouseState() : LeftButtonDown(false) { }
  } MouseState;
  // This is the one method that we have to implement
  bool OnEvent(const SEvent& event) {
    // Remember the mouse state
    if (event.EventType == EET_MOUSE_INPUT_EVENT) {
      switch (event.MouseInput.Event) {
      case EMIE_LMOUSE_PRESSED_DOWN:
        MouseState.LeftButtonDown = true;
        break;
      case EMIE_LMOUSE_LEFT_UP:
        MouseState.LeftButtonDown = false;
        break;
      case EMIE_MOUSE_MOVED:
        MouseState.Position.X = event.MouseInput.X;
        MouseState.Position.Y = event.MouseInput.Y;
        break;
      default:
        // We won't use the wheel
        break;
      }
    }
    // The state of each connected joystick is sent to us
    // once every run() of the Irrlicht device.  Store the
    // state of the first joystick, ignoring other joysticks.
    // This is currently only supported on Windows and Linux.
    if (event.EventType == EET_JOYSTICK_INPUT_EVENT
        && event.JoystickEvent.Joystick == 0) {
      JoystickState = event.JoystickEvent;
    }
    return false;
  }
  const SEvent::SJoystickEvent& GetJoystickState(void) const {
    return JoystickState;
  }
  const SMouseState& GetMouseState(void) const {
    return MouseState;
  }
  MyEventReceiver() {
  }
private:
  SEvent::SJoystickEvent JoystickState;
};
/*
The event receiver for keeping the pressed keys is ready, the actual responses
will be made inside the render loop, right before drawing the scene. So lets
just create an IrrlichtDevice and the scene node we want to move. We also
create some other additional scene nodes, to show that there are also some
different possibilities to move and animate scene nodes.
*/
int main()
{
  // ask user for driver
  E_DRIVER_TYPE driverType = driverChoiceConsole();
  if (driverType == EDT_COUNT) {
    return 1;
  }
  // create device
  MyEventReceiver receiver;
  IrrlichtDevice* device = createDevice(driverType,
      dimension2du(640, 480), 16, false, false, false, &receiver);
  if (device == 0) {
    return 1;  // could not create selected driver.
  }
  SJoystickInfo joystickInfo[100];
  int joystickInfo_size = 0;
  if (joystickInfo = device->activateJoysticks(joystickInfo, 100)) {
    std::cout << "Joystick support is enabled and " << joystickInfo_size << " joystick(s) are present." << std::endl;
    for (u32 joystick = 0; joystick < joystickInfo_size; ++joystick) {
      std::cout << "Joystick " << joystick << ":" << std::endl;
      std::cout << "\tName: '" << joystickInfo[joystick].Name << "'" << std::endl;
      std::cout << "\tAxes: " << joystickInfo[joystick].Axes << std::endl;
      std::cout << "\tButtons: " << joystickInfo[joystick].Buttons << std::endl;
      std::cout << "\tHat is: ";
      switch (joystickInfo[joystick].PovHat) {
      case SJoystickInfo::POV_HAT_PRESENT:
        std::cout << "present" << std::endl;
        break;
      case SJoystickInfo::POV_HAT_ABSENT:
        std::cout << "absent" << std::endl;
        break;
      case SJoystickInfo::POV_HAT_UNKNOWN:
      default:
        std::cout << "unknown" << std::endl;
        break;
      }
    }
  }
  else {
    std::cout << "Joystick support is not enabled." << std::endl;
  }
  char* tmp = "Irrlicht Joystick Example (";
  tmp += joystickInfo_size;
  tmp += " joysticks)";
  device->setWindowCaption(tmp);
  IVideoDriver* driver = device->getVideoDriver();
  ISceneManager* smgr = device->getSceneManager();
  /*
  We'll create an arrow mesh and move it around either with the joystick axis/hat,
  or make it follow the mouse pointer. */
  ISceneNode* node = smgr->addMeshSceneNode(
      smgr->addArrowMesh("Arrow",
          _ARGB(255, 255, 0, 0),
          _ARGB(255, 0, 255, 0),
          16, 16,
          2.f, 1.3f,
          0.1f, 0.6f
                        )
      );
  node->setMaterialFlag(EMF_LIGHTING, false);
  ICameraSceneNode* camera = smgr->addCameraSceneNode();
  camera->setPosition(f3VECTOR(0, 0, -10));
  // As in example 04, we'll use framerate independent movement.
  u32 then = device->getTimer()->getTime();
  const f32 MOVEMENT_SPEED = 5.f;
  while (device->run()) {
    // Work out a frame delta time.
    const u32 now = device->getTimer()->getTime();
    const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
    then = now;
    bool movedWithJoystick = false;
    vector3df nodePosition = node->getPosition();
    if (joystickInfo_size > 0) {
      f32 moveHorizontal = 0.f; // Range is -1.f for full left to +1.f for full right
      f32 moveVertical = 0.f; // -1.f for full down to +1.f for full up.
      const SEvent::SJoystickEvent& joystickData = receiver.GetJoystickState();
      // We receive the full analog range of the axes, and so have to implement our
      // own dead zone.  This is an empirical value, since some joysticks have more
      // jitter or creep around the center point than others.  We'll use 5% of the
      // range as the dead zone, but generally you would want to give the user the
      // option to change this.
      const f32 DEAD_ZONE = 0.05f;
      moveHorizontal =
          (f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_X] / 32767.f;
      if (fabs(moveHorizontal) < DEAD_ZONE) {
        moveHorizontal = 0.f;
      }
      moveVertical =
          (f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_Y] / -32767.f;
      if (fabs(moveVertical) < DEAD_ZONE) {
        moveVertical = 0.f;
      }
      // POV hat info is only currently supported on Windows, but the value is
      // guaranteed to be 65535 if it's not supported, so we can check its range.
      const u16 povDegrees = joystickData.POV / 100;
      if (povDegrees < 360) {
        if (povDegrees > 0 && povDegrees < 180) {
          moveHorizontal = 1.f;
        }
        else if (povDegrees > 180) {
          moveHorizontal = -1.f;
        }
        if (povDegrees > 90 && povDegrees < 270) {
          moveVertical = -1.f;
        }
        else if (povDegrees > 270 || povDegrees < 90) {
          moveVertical = +1.f;
        }
      }
      if (!equals(moveHorizontal, 0.f) || !equals(moveVertical, 0.f)) {
        nodePosition.X += MOVEMENT_SPEED * frameDeltaTime * moveHorizontal;
        nodePosition.Y += MOVEMENT_SPEED * frameDeltaTime * moveVertical;
        movedWithJoystick = true;
      }
    }
    // If the arrow node isn't being moved with the joystick, then have it follow the mouse cursor.
    if (!movedWithJoystick) {
      // Create a ray through the mouse cursor.
      line3df ray = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(
          receiver.GetMouseState().Position, camera);
      // And intersect the ray with a plane around the node facing towards the camera.
      plane3df plane(nodePosition, f3VECTOR(0, 0, -1));
      vector3df mousePosition;
      if (plane.getIntersectionWithLine(ray.start, ray.getVector(), mousePosition)) {
        // We now have a mouse position in 3d space; move towards it.
        vector3df toMousePosition(mousePosition - nodePosition);
        const f32 availableMovement = MOVEMENT_SPEED * frameDeltaTime;
        if (toMousePosition.getLength() <= availableMovement) {
          nodePosition = mousePosition;  // Jump to the final position
        }
        else {
          nodePosition += toMousePosition.normalize() * availableMovement;  // Move towards it
        }
      }
    }
    node->setPosition(nodePosition);
    // Turn lighting on and off depending on whether the left mouse button is down.
    node->setMaterialFlag(EMF_LIGHTING, receiver.GetMouseState().LeftButtonDown);
    driver->beginScene(true, true, _ARGB(255, 113, 113, 133));
    smgr->drawAll(); // draw the 3d scene
    driver->endScene();
  }
  /*
  In the end, delete the Irrlicht device.
  */
  device->drop();
  return 0;
}
/*
**/

