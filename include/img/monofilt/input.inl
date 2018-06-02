
char *KeyNames[] =
{
 "?",
 "Left Mouse Button", "Right Mouse Button", "?", "Middle Mouse Button",
 "?", "?", "?", "Backspace", "Tab", "?", "?", "?", "Enter", "?", "?",
 "Shift", "Ctrl", "Alt", "Pause", "Caps Lock", "?", "?", "?", "?", "?", "?",
 "Escape", "?", "?", "?", "?",
 "Space", "Page Up", "Page Down", "End", "Home",
 "Left Arrow", "Up Arrow", "Right Arrow", "Down Arrow",
 "?", "?", "?", "?", "Insert", "Delete", "?",
 "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
 "?", "?", "?", "?", "?", "?", "?",
 "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
 "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
 "Left Win", "Right Win", "Application", "?", "?",
 "NumPad 0", "NumPad 1", "NumPad 2", "NumPad 3", "NumPad 4",
 "NumPad 5", "NumPad 6", "NumPad 7", "NumPad 8", "NumPad 9",
 "Multiply", "Add", "?", "Subtract", "Decimal", "Divide",
 "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "Num Lock", "Scroll Lock",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "Semicolon", "Equals", "Comma", "Minus", "Period", "Slash", "Grave",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?",
 "Left bracket", "Backslash", "Right bracket", "Apostrophe",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?"
};


POINT Input_GetMousePos(HWND hWnd)
{
  POINT g_MousePos;
  GetCursorPos(&g_MousePos);
  //ClientToScreen(hWnd, &g_MousePos);
  ScreenToClient(hWnd, &g_MousePos);
  return g_MousePos;
}

int Input_SetMousePos(POINT pt)
{
  return SetCursorPos(pt.x,pt.y);
}

int Input_GetKeyState(int key)
{
  return ((GetKeyState(key) & 0x8000) != 0);
}

//int    bCaptured;
static char keyz[256];

int Input_KeyDown(int key)
{
  return (keyz[key] & 1) != 0;
}

int Input_KeyUp(int key)
{
  return (keyz[key] & 2) != 0;
}

char* Input_GetKeyName(int key)
{
  return KeyNames[key];
}

#define LOWORDINT(n) ((int)((signed short)(LOWORD(n))))
#define HIWORDINT(n) ((int)((signed short)(HIWORD(n))))

#define HGEINP_SHIFT       1
#define HGEINP_CTRL        2
#define HGEINP_ALT         4
#define HGEINP_CAPSLOCK    8
#define HGEINP_SCROLLLOCK 16
#define HGEINP_NUMLOCK    32
#define HGEINP_REPEAT     64

typedef struct _InputEvent
{
  int type;    // event type
  int key;     // key code
  int flags;   // event flags
  int chr;     // character code
  int wheel;   // wheel shift
  int x;       // mouse cursor x-coordinate
  int y;       // mouse cursor y-coordinate
}InputEvent;

int Input_BuildEvent(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam, InputEvent* event)
{
  unsigned char kbstate[256];
  POINT pt;
  int key=wParam;
  int scan=HIWORD(lParam) & 0xFF, flags=(lParam & 0x40000000) ? HGEINP_REPEAT:0;
  int x=LOWORDINT(lParam), y=HIWORDINT(lParam);
  //InputEvent event;
  pt.x=x; pt.y=y;
  event->type=nMsg;

  GetKeyboardState(kbstate);
  if(nMsg==WM_KEYDOWN)
  {
    if((flags & HGEINP_REPEAT) == 0) keyz[key] |= 1;
    ToAscii(key, scan, kbstate, (unsigned short *)&event->chr, 0);
  }
  if(nMsg==WM_KEYUP)
  {
    keyz[key] |= 2;
    ToAscii(key, scan, kbstate, (unsigned short *)&event->chr, 0);
  }
  if(nMsg==WM_MOUSEWHEEL)
  {
    event->key=0; event->wheel=key;
    ScreenToClient(hwnd,&pt);
  }
  else { event->key=key; event->wheel=0; }

  if(nMsg==WM_MBUTTONDOWN)
  {
    keyz[key] |= 1;
    //SetCapture(hwnd);
    //bCaptured=true;
  }
  if(nMsg==WM_MBUTTONUP)
  {
    keyz[key] |= 2;
    ReleaseCapture();
    //bCaptured=false;
  }
  
  if(kbstate[VK_SHIFT] & 0x80) flags|=HGEINP_SHIFT;
  if(kbstate[VK_CONTROL] & 0x80) flags|=HGEINP_CTRL;
  if(kbstate[VK_MENU] & 0x80) flags|=HGEINP_ALT;
  if(kbstate[VK_CAPITAL] & 0x1) flags|=HGEINP_CAPSLOCK;
  if(kbstate[VK_SCROLL] & 0x1) flags|=HGEINP_SCROLLLOCK;
  if(kbstate[VK_NUMLOCK] & 0x1) flags|=HGEINP_NUMLOCK;
  event->flags=flags;
  return 0;
}
