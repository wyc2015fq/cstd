
#define _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
static bool SVideoMode_eq(const SVideoMode* this, const SVideoMode* other)
{
  return this->size.w == other->size.w && this->size.h == other->size.h && this->depth == other->depth;
}
static bool SVideoMode_less(const SVideoMode* this, const SVideoMode* other)
{
  return (this->size.w < other->size.w ||
    (this->size.w == other->size.w &&
    this->size.h < other->size.h) ||
    (this->size.w == other->size.w &&
    this->size.h == other->size.h &&
    this->depth < other->depth));
}
IFileSystem* createFileSystem()
{
  static IFileSystem s_FileSystem[1] = {0};
  return s_FileSystem;
}
IDevice* optDevice(void* findWindowId, IDevice* pDevAdd, IDevice* pDevDel, bool delall)
{
  IDevice* gDevice = NULL;
  if (delall) {
    IDevice* c = gDevice;
    for (; c;) {
      IDevice* c_next = c->next;
      c->closeDevice(c);
      free(c);
      c = c_next;
    }
    gDevice = NULL;
    return 0;
  }
  if (findWindowId) {
    IDevice* c = gDevice;
    for (; c; c = c->next) {
      if (c->WindowId == findWindowId) {
        return c;
      }
    }
  }
  if (pDevAdd) {
    pDevAdd->prev = NULL;
    pDevAdd->next = gDevice;
    gDevice = pDevAdd;
  }
  if (pDevDel) {
    if (pDevDel == gDevice) {
      gDevice = pDevDel->next;
      if (pDevDel->next) {
        pDevDel->next->prev = NULL;
      }
    }
    else {
      if (pDevDel->prev) {
        pDevDel->prev->next = pDevDel->next;
      }
      if (pDevDel->next) {
        pDevDel->next->prev = pDevDel->prev;
      }
    }
    pDevDel->next = NULL;
    pDevDel->prev = NULL;
  }
  return 0;
}
IDevice* getDeviceFromWindowId(void* findWindowId)
{
  return optDevice(findWindowId, NULL, NULL, false);
}
void* AddDevice(IDevice* pDevAdd)
{
  return optDevice(NULL, pDevAdd, NULL, false);
}
void* DelDevice(IDevice* pDevDel)
{
  return optDevice(NULL, NULL, pDevDel, false);
}
void sysexit()
{
  optDevice(NULL, NULL, NULL, true);
}
//! Compares to the last call of this function to return double and triple clicks.
u32 CDevice_checkSuccessiveClicks(IDevice* s, s32 mouseX, s32 mouseY, EMOUSE_INPUT_EVENT inputEvent)
{
  const s32 MAX_MOUSEMOVE = 3;
  u32 clickTime = getRealTime();
  if ((clickTime - s->MouseMultiClicks.LastClickTime) < s->MouseMultiClicks.DoubleClickTime
      && ABS(s->MouseMultiClicks.LastClick.x - mouseX) <= MAX_MOUSEMOVE
      && ABS(s->MouseMultiClicks.LastClick.y - mouseY) <= MAX_MOUSEMOVE
      && s->MouseMultiClicks.CountSuccessiveClicks < 3
      && s->MouseMultiClicks.LastMouseInputEvent == inputEvent
     ) {
    ++s->MouseMultiClicks.CountSuccessiveClicks;
  }
  else {
    s->MouseMultiClicks.CountSuccessiveClicks = 1;
  }
  s->MouseMultiClicks.LastMouseInputEvent = inputEvent;
  s->MouseMultiClicks.LastClickTime = clickTime;
  s->MouseMultiClicks.LastClick.x = mouseX;
  s->MouseMultiClicks.LastClick.y = mouseY;
  return s->MouseMultiClicks.CountSuccessiveClicks;
}
//! send the event to the right receiver
bool CDevice_postEventFromUser(IDevice* s, const SEvent* event)
{
  bool absorbed = false;
  ISceneManager* inputReceiver;
  if (s->UserReceiver) {
    absorbed = s->UserReceiver->OnEvent(event);
  }
  if (!absorbed && s->GUIEnvironment) {
    absorbed = s->GUIEnvironment->postEventFromUser(event);
  }
  inputReceiver = s->InputReceivingSceneManager;
  if (!inputReceiver) {
    inputReceiver = s->SceneManager;
  }
  if (!absorbed && inputReceiver) {
    absorbed = inputReceiver->postEventFromUser(event);
  }
  _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
  return absorbed;
}
void calculateGammaRamp(u16* ramp, f32 gamma, f32 relativebrightness, f32 relativecontrast)
{
  s32 i;
  s32 value;
  s32 rbright = (s32)(relativebrightness * (65535.f / 4));
  f32 rcontrast = 1.f / (255.f - (relativecontrast * 127.5f));
  gamma = gamma > 0.f ? 1.0f / gamma : 0.f;
  for (i = 0; i < 256; ++i) {
    value = (s32)(pow(rcontrast * i, gamma) * 65535.f + 0.5f);
    ramp[i] = (u16) s32_clamp(value + rbright, 0, 65535);
  }
}
static f32 calculateGammaFromRamp(const u16* ramp)
{
  /* The following is adapted from a post by Garrett Bass on OpenGL
  Gamedev list, March 4, 2000.
  */
  f32 sum = 0.0;
  f32 gamma;
  s32 i, count = 0;
  gamma = 1.0;
  for (i = 1; i < 256; ++i) {
    if ((ramp[i] != 0) && (ramp[i] != 65535)) {
      f32 B = (f32)i / 256.f;
      f32 A = ramp[i] / 65535.f;
      sum += (f32)(logf(A) / logf(B));
      count++;
    }
  }
  if (count && sum) {
    gamma = 1.0f / (sum / count);
  }
  return gamma;
}
static s32 getDesktopDepth()
{
  SVideoMode mode = {0};
  getVideoMode(-1, &mode);
  return mode.depth;
}
static ISIZE getDesktopResolution()
{
  SVideoMode mode = {0};
  getVideoMode(-1, &mode);
  return mode.size;
}
static s32 getVideoModeDepth(s32 modeNumber)
{
  SVideoMode mode = {0};
  getVideoMode(modeNumber, &mode);
  return mode.depth;
}
static ISIZE getVideoModeResolution(s32 modeNumber)
{
  SVideoMode mode = {0};
  getVideoMode(modeNumber, &mode);
  return mode.size;
}
s32 getVideoModeCount()
{
  return getVideoMode(0, NULL);
}
ISIZE getVideoModeResolution2(ISIZE minSize, ISIZE maxSize)
{
  SVideoMode mode = {0};
  int i, best, VideoModes_size;
  best = VideoModes_size = getVideoMode(0, NULL);
  // if only one or no mode
  if (best < 2) {
    getVideoMode(0, &mode);
    return mode.size;
  }
  for (i = 0; i < VideoModes_size; ++i) {
    getVideoMode(i, &mode);
    if (mode.size.w >= minSize.w &&
        mode.size.h >= minSize.h &&
        mode.size.w <= maxSize.w &&
        mode.size.h <= maxSize.h) {
      best = i;
    }
  }
  // we take the last one found, the largest one fitting
  if (best < VideoModes_size) {
    getVideoMode(best, &mode);
    return mode.size;
  }
  {
    const int minArea = minSize.w * minSize.h;
    const int maxArea = maxSize.w * maxSize.h;
    u32 minDist = 0xffffffff;
    best = 0;
    for (i = 0; i < VideoModes_size; ++i) {
      int area, dist;
      getVideoMode(i, &mode);
      area = mode.size.w * mode.size.h;
      dist = MIN(ABS((minArea - area)), ABS((maxArea - area)));
      if (dist < minDist) {
        minDist = dist;
        best = i;
      }
    }
  }
  getVideoMode(best, &mode);
  return mode.size;
}
