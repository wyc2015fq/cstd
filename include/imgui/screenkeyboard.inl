
typedef struct {
  char cNormal;
  char cShifted;
  int cWidth;
} KEYDEF;

//int pKeyWidth;
//int pKeyHeight;
// CTypedPtrList<CPtrList, KEYDEF*> cKeys;

#define WM_ICON_NOTIFY WM_USER + 10
#define TIMER_ID 101

#define TAB 0x02
#define CAPSLOCK 0x03
#define SKEY_SHIFT 0x04
#define CTRL 0x05
#define BSPACE 0x06
#define ENTER 0x07
#define ALT 0x08
#define WINDOW 0x9
#define LEFT 0x0A
#define RIGHT 0x0B
#define UP 0x0C
#define DOWN 0x0D
#define ESC 0x0E
#define PUP 0x0F
#define PDN 0x10
#define HOME 0x11
#define END 0x12
#define INS 0x13
#define DEL 0x14
#define WWW 0x15


#define SCREENKEYBOARD_ADDKEY(pNormal, pShifted, pWidth) (cKeys[nKeys].cNormal = pNormal, cKeys[nKeys].cShifted = pShifted, cKeys[nKeys].cWidth = pWidth, nKeys++)

int screenkeyboard_init(KEYDEF* cKeys)
{
  int nKeys = 0, fnkey;
  
  //control row
  SCREENKEYBOARD_ADDKEY(0x00,0x00,0); //signal row change
  SCREENKEYBOARD_ADDKEY(0x00,ESC,10);
  for(fnkey = 0x70; fnkey < 0x7C; fnkey++) {
    SCREENKEYBOARD_ADDKEY(0x00,fnkey & 0xFF,10);
  }
  if (0) {
    SCREENKEYBOARD_ADDKEY(0x00, 0x70,10);
    SCREENKEYBOARD_ADDKEY(0x00, 0x71,10);
    SCREENKEYBOARD_ADDKEY(0x00, 0x72,10);
    SCREENKEYBOARD_ADDKEY(0x00, 0x73,10);
    SCREENKEYBOARD_ADDKEY(0x00, 0x74,10);
    SCREENKEYBOARD_ADDKEY(0x00, 0x75,10);
    SCREENKEYBOARD_ADDKEY(0x00, 0x76,10);
    SCREENKEYBOARD_ADDKEY(0x00, 0x77,10);
    SCREENKEYBOARD_ADDKEY(0x00, 0x78,10);
    SCREENKEYBOARD_ADDKEY(0x00, 0x79,10);
    SCREENKEYBOARD_ADDKEY(0x00, 0x7a,10);
    SCREENKEYBOARD_ADDKEY(0x00, 0x7b,10);
    SCREENKEYBOARD_ADDKEY(0x00, 0x7c,10);
  }
  SCREENKEYBOARD_ADDKEY(0x00,INS,14);
  SCREENKEYBOARD_ADDKEY(0x00,DEL,-10);
  
  //first row
  SCREENKEYBOARD_ADDKEY(0x00,0x00,1); //signal row change
  SCREENKEYBOARD_ADDKEY('`','~',10);
  SCREENKEYBOARD_ADDKEY('1','!',10);
  SCREENKEYBOARD_ADDKEY('2','@',10);
  SCREENKEYBOARD_ADDKEY('3','#',10);
  SCREENKEYBOARD_ADDKEY('4','$',10);
  SCREENKEYBOARD_ADDKEY('5','%',10);
  SCREENKEYBOARD_ADDKEY('6','^',10);
  SCREENKEYBOARD_ADDKEY('7','&',10);
  SCREENKEYBOARD_ADDKEY('8','*',10);
  SCREENKEYBOARD_ADDKEY('9','(',10);
  SCREENKEYBOARD_ADDKEY('0',')',10);
  SCREENKEYBOARD_ADDKEY('-','_',10);
  SCREENKEYBOARD_ADDKEY('=','+',10);
  SCREENKEYBOARD_ADDKEY(0x00,BSPACE,14); //backspace
  SCREENKEYBOARD_ADDKEY(0x00,PUP,-12);
  
  //second row
  SCREENKEYBOARD_ADDKEY(0x00,0x00,2); //signal row change
  SCREENKEYBOARD_ADDKEY(0x00,TAB,14); //tab
  SCREENKEYBOARD_ADDKEY('q','Q',10);
  SCREENKEYBOARD_ADDKEY('w','W',10);
  SCREENKEYBOARD_ADDKEY('e','E',10);
  SCREENKEYBOARD_ADDKEY('r','R',10);
  SCREENKEYBOARD_ADDKEY('t','T',10);
  SCREENKEYBOARD_ADDKEY('y','Y',10);
  SCREENKEYBOARD_ADDKEY('u','U',10);
  SCREENKEYBOARD_ADDKEY('i','I',10);
  SCREENKEYBOARD_ADDKEY('o','O',10);
  SCREENKEYBOARD_ADDKEY('p','P',10);
  SCREENKEYBOARD_ADDKEY('[','{',10);
  SCREENKEYBOARD_ADDKEY(']','}',10);
  SCREENKEYBOARD_ADDKEY('\\','|',10);
  SCREENKEYBOARD_ADDKEY(0x00,PDN,-12);
  
  //third row
  SCREENKEYBOARD_ADDKEY(0x00,0x00,3); //signal row change
  SCREENKEYBOARD_ADDKEY(0x00,CAPSLOCK,18); //caps lock
  SCREENKEYBOARD_ADDKEY('a','A',10);
  SCREENKEYBOARD_ADDKEY('s','S',10);
  SCREENKEYBOARD_ADDKEY('d','D',10);
  SCREENKEYBOARD_ADDKEY('f','F',10);
  SCREENKEYBOARD_ADDKEY('g','G',10);
  SCREENKEYBOARD_ADDKEY('h','H',10);
  SCREENKEYBOARD_ADDKEY('j','J',10);
  SCREENKEYBOARD_ADDKEY('k','K',10);
  SCREENKEYBOARD_ADDKEY('l','L',10);
  SCREENKEYBOARD_ADDKEY(';',':',10);
  SCREENKEYBOARD_ADDKEY('\'','\"',10);
  SCREENKEYBOARD_ADDKEY(0x00,ENTER,16); //enter
  SCREENKEYBOARD_ADDKEY(0x00,HOME,-10);
  
  //fourth row
  SCREENKEYBOARD_ADDKEY(0x00,0x00,4); //signal row change
  SCREENKEYBOARD_ADDKEY(0x00,SKEY_SHIFT,22); //shift
  SCREENKEYBOARD_ADDKEY('z','Z',10);
  SCREENKEYBOARD_ADDKEY('x','X',10);
  SCREENKEYBOARD_ADDKEY('c','C',10);
  SCREENKEYBOARD_ADDKEY('v','V',10);
  SCREENKEYBOARD_ADDKEY('b','B',10);
  SCREENKEYBOARD_ADDKEY('n','N',10);
  SCREENKEYBOARD_ADDKEY('m','M',10);
  SCREENKEYBOARD_ADDKEY(',','<',10);
  SCREENKEYBOARD_ADDKEY('.','>',10);
  SCREENKEYBOARD_ADDKEY('/','?',10);
  SCREENKEYBOARD_ADDKEY(0x00,SKEY_SHIFT,22); //shift
  SCREENKEYBOARD_ADDKEY(0x00,END,-10);
  
  //fifth row
  SCREENKEYBOARD_ADDKEY(0x00,0x00,5); //signal row change
  SCREENKEYBOARD_ADDKEY(0x00,CTRL,12); //ctrl
  SCREENKEYBOARD_ADDKEY(0x00,WINDOW,12); //window
  SCREENKEYBOARD_ADDKEY(0x00,ALT,12); //alt
  SCREENKEYBOARD_ADDKEY(' ',' ',60);
  SCREENKEYBOARD_ADDKEY(0x00,LEFT,12); //left
  SCREENKEYBOARD_ADDKEY(0x00,UP,12); //up
  SCREENKEYBOARD_ADDKEY(0x00,DOWN,12); //down
  SCREENKEYBOARD_ADDKEY(0x00,RIGHT,12); //right
  SCREENKEYBOARD_ADDKEY(0x00,WWW,-10); //http://WWW
  //CalcWidthHeight();
  
  return nKeys;  // return TRUE  unless you set the focus to a control
}

#if 0
void COnscreenKeyboardDlg::SendKey(KEYDEF * keydef)
{
  if(keydef->cNormal == 0x00)
  {
    int vk;
    BOOL uptoo = TRUE;
    if(keydef->cShifted > 0x6F) //is a function key
    {
      vk = keydef->cShifted;
    }
    else
    {
      switch(keydef->cShifted)
      {
      case TAB:    // 0x02
        vk = VkKeyScan(0x09) & 0xFF;
        break;
      case CAPSLOCK:    // 0x03
        vk = VK_CAPITAL;
        break;
      case SKEY_SHIFT:    // 0x04
        vk = VK_SHIFT;
        if(GetKeyState(vk) & 0xF000) return;
        uptoo = FALSE;
        break;
      case CTRL:    // 0x05
        vk = VK_CONTROL;
        if(GetKeyState(vk) & 0xF000) return;
        uptoo = FALSE;
        break;
      case BSPACE:    // 0x06
        vk = VK_BACK;
        break;
      case ENTER:    // 0x07
        vk = VkKeyScan(0x0D) & 0xFF;
        break;
      case ALT:    // 0x08
        vk = VK_MENU;
        break;
      case WINDOW:    // 0x9
        vk = VK_LWIN;
        break;
      case LEFT:    // 0x0A
        vk = VK_LEFT;
        break;
      case RIGHT:    // 0x0B
        vk = VK_RIGHT;
        break;
      case UP:    // 0x0C
        vk = VK_UP;
        break;
      case DOWN:    // 0x0D
        vk = VK_DOWN;
        break;
      case PUP: //0x0F
        vk = VK_PRIOR;
        break;
      case PDN: //0x10
        vk = VK_NEXT;
        break;
      case HOME: //0x11
        vk = VK_HOME;
        break;
      case END: //0x12
        vk = VK_END;
        break;
      case INS: //0x13
        vk = VK_INSERT;
        break;
      case DEL: //0x14
        vk = VK_DELETE;
        break;
      case WWW:
        KEYDEF key;
        key.cNormal = key.cShifted = 'H';
        SendKey(&key);
        key.cNormal = key.cShifted = 'T';
        SendKey(&key);
        SendKey(&key);
        key.cNormal = key.cShifted = 'P';
        SendKey(&key);
        key.cNormal = key.cShifted = ':';
        SendKey(&key);
        key.cNormal = key.cShifted = '/';
        SendKey(&key);
        SendKey(&key);
        key.cNormal = key.cShifted = 'W';
        SendKey(&key);
        SendKey(&key);
        SendKey(&key);
        vk = VkKeyScan('.') & 0xFF;
        break;
      default:
        ASSERT(FALSE);
        break;
      }
    }
    keybd_event(vk,0,0,0);
    if(uptoo)
    {
      keybd_event(vk,0,KEYEVENTF_KEYUP,0);
    }
  }
  else
  {
    char pChar = keydef->cNormal;
    SHORT ks = VkKeyScan(pChar);
    BYTE key = ks & 0xFF;
    
    keybd_event(key,0,0,0);
    keybd_event(key,0,KEYEVENTF_KEYUP,0);
    
    //turn off the control and shift if they were down
    unsigned char vk = VK_SHIFT;
    if(GetKeyState(vk) & 0xF000)
    {
      keybd_event(vk,0,KEYEVENTF_KEYUP,0);
    }
    vk = VK_CONTROL;
    if(GetKeyState(vk) & 0xF000) //allow toggle
    {
      keybd_event(vk,0,KEYEVENTF_KEYUP,0);
    }
  }
}
#endif

int screenkeyboard_getrect(IRECT rect, const KEYDEF* cKeys, int nKeys, IRECT* prc) {
  int pKeyWidth = 0;
  int pKeyHeight = 0;
  int i;
  const KEYDEF* key;
  {
    long totalWidth = 0;
    long longest = 0;
    long rows = 0;
    for (i=0; i<nKeys; ++i) {
      key = cKeys + i;
      if((key->cNormal == 0x00) && (key->cShifted == 0x00))
      {
        rows++;
        if(totalWidth > longest)
        {
          longest = totalWidth;
        }
        totalWidth = 0;
      }
      else
      {
        totalWidth += abs(key->cWidth);
      }
    }
    pKeyHeight = RCH(&rect) / rows;
    pKeyWidth = (int)(((double)RCW(&rect)) / ((((double)(longest)) / 10)));
  }
  {
    int cx = rect.l;
    int cy = rect.t-pKeyHeight;
    for (i=0; i<nKeys; ++i) {
      IRECT rc = {0};
      key = cKeys + i;
      if((key->cNormal == 0x00) && (key->cShifted == 0x00)) {
        cx = 0;
        cy += pKeyHeight;
      } else {
        int width = ((abs(key->cWidth) * pKeyWidth) / 10);
        rc = iRECT2(cx,cy, width, pKeyHeight);
        if(key->cWidth < 0) {
          rc.r = rect.r;
        }
        cx += width;
      }
      prc[i] = rc;
    }
  }
  return 0;
}

void DrawKey(gc_t* g, IRECT rc, const KEYDEF* key, BOOL hot, BOOL hit, font_t* font)
{
#if 0
  if(!cHilight)
  {
    rc = iRectDeflate2(rc, 1,1);
    gcSolidRect2(g, rc, _GetSysColor(CLR_3DFACE));
    g->gcDraw3dRect(rc,_GetSysColor(CLR_3DSHADOW),_GetSysColor(CLR_3DDKSHADOW));
    rc.DeflateRect(1,1);
    g->Draw3dRect(rc,_GetSysColor(CLR_3DLIGHT),_GetSysColor(CLR_3DSHADOW));
  }
  else
  {
    rc.DeflateRect(1,1);
    g->FillSolidRect(rc,_GetSysColor(CLR_3DLIGHT));
    g->Draw3dRect(rc,_GetSysColor(CLR_3DLIGHT),_GetSysColor(CLR_3DSHADOW));
    rc.DeflateRect(1,1);
    g->Draw3dRect(rc,_GetSysColor(CLR_3DSHADOW),_GetSysColor(CLR_3DDKSHADOW));
  }
  rc.DeflateRect(3,1);
#endif
  
  if (1) {
    COLOR clrFace = myGetSysColor(CLR_BTNFACE);
    gcSolidRect2(g, rc, clrFace);
    gcEdgeRectSys(g, rc, hit ? BDS_SUNKEN : BDS_HIGH, 15, clrFace);
    if (hot) {
      gcEdgeRect1(g, rc, _rgba(1, 1, 1, 0.2), 0, 0);
    }
  }
  if(key->cNormal == 0x00)
  {
    const char* label = " ";
    BOOL special = FALSE;
    if(key->cShifted >= 0x70) //is a function
    {
      int fkeynum = key->cShifted - 0x70;
      static const char* ff[] = {"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12"};
      ASSERT(fkeynum<12);
      label = ff[fkeynum];
    }
    else
    {
      switch(key->cShifted)
      {
      case TAB:    // 0x02
        label = "Tab->";
        break;
      case CAPSLOCK:    // 0x03
        label = "Caps";
        break;
      case SKEY_SHIFT:    // 0x04
        label = "Shift";
        break;
      case CTRL:    // 0x05
        label = "Ctrl";
        break;
      case BSPACE:    // 0x06
        label = "<-Bspc";
        break;
      case ENTER:    // 0x07
        label = "Enter";
        break;
      case ALT:    // 0x08
        label = "Alt";
        break;
      case WINDOW:    // 0x9
        label = "Wnd";
        break;
      case ESC:
        label = "Esc";
        break;
      case LEFT:    // 0x0A
        special = TRUE;
        {
          double cx = (rc.l + rc.r)/2;
          double cy = (rc.t + rc.b)/2;
          double siz =  RCW(&rc)/3;
          gcDrawLine3(g, cx+siz, cy, cx-siz, cy, cx-siz + (siz*4)/5, cy - siz/2);
          gcDrawLine(g, cx-siz, cy, cx-siz + (siz*4)/5, cy + siz/2);
        }
        break;
      case RIGHT:    // 0x0B
        special = TRUE;
        {
          double cx = (rc.l + rc.r)/2;
          double cy = (rc.t + rc.b)/2;
          double siz = RCW(&rc)/3;
          gcDrawLine3(g, cx-siz, cy, cx+siz, cy, cx+siz - (siz*4)/5, cy - siz/2);
          gcDrawLine(g, cx+siz, cy, cx+siz - (siz*4)/5, cy + siz/2);
        }
        break;
      case UP:    // 0x0C
        special = TRUE;
        {
          double cx = (rc.l + rc.r)/2;
          double cy = (rc.t + rc.b)/2;
          double siz = RCW(&rc)/3;
          gcDrawLine3(g, cx, cy+siz, cx, cy-siz, cx - siz/2, cy-siz + (siz*4)/5);
          gcDrawLine(g, cx, cy-siz, cx + siz/2, cy-siz + (siz*4)/5);
        }
        break;
      case DOWN:    // 0x0D
        special = TRUE;
        {
          double cx = (rc.l + rc.r)/2;
          double cy = (rc.t + rc.b)/2;
          double siz = RCW(&rc)/3;
          gcDrawLine5(g, cx, cy-siz, cx, cy+siz, cx - siz/2, cy+siz - (siz*4)/5, cx, cy+siz, cx + siz/2, cy+siz - (siz*4)/5);
        }
        break;
      case PUP: //0x0F
        label = "PgUp";
        break;
      case PDN: //0x10
        label = "PgDn";
        break;
      case HOME: //0x11
        label = "Home";
        break;
      case END: //0x12
        label = "End";
        break;
      case INS: //0x13
        label = "Ins";
        break;
      case DEL: //0x14
        label = "Del";
        break;
      case WWW:
        label = "WWW";
        break;
      default:
        ASSERT(FALSE);
        label = "#ERR#";
        break;
      }
    }
    if(!special)
    {
      gcSolidText(g, rc, label, NULL, font, 0, TF_CENTER|TF_VCENTER, ColorBlack);
    }
  }
  else
  {
    int fmt = TF_CENTER|TF_VCENTER;
    char chr[4] = {0};
    if((key->cShifted >= 'A') && (key->cShifted <= 'Z')) {
      chr[0] = key->cShifted;
      gcSolidText(g, rc, chr, NULL, font, 0, fmt, ColorBlack);
    } else {
      int w2 = RCW(&rc)/2, h2 = RCH(&rc)/2;
      chr[0] = key->cShifted;
      gcSolidText(g, iRECT2(rc.l, rc.t, w2, h2), chr, NULL, font, 0, fmt, ColorBlack);
      chr[0] = key->cNormal;
      gcSolidText(g, iRECT2(rc.l+w2, rc.t+h2, w2, h2), chr, NULL, font, 0, fmt, ColorBlack);
    }
  }
}

int screenkeyboard_ctrl(ctrl_t* c) {
  SYSIO;
  int presskey = 0, hot = 0, hit = 0, i;
  IRECT rc = c->rc;
  static KEYDEF cKeys[110];
  IRECT rcKeys[countof(cKeys)];
  static int nKeys = 0;
  if (1) {
    c->sz = iSIZE(154, 70);
  }
  if (0==nKeys) {
    nKeys = screenkeyboard_init(cKeys);
  }
  screenkeyboard_getrect(rc, cKeys, nKeys, rcKeys);
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rc, x, y);
    if (hot) {
      hot = iPtInRects(nKeys, rcKeys, x, y);
    }
    switch (io->msg) {
    case MSG_LBUTTONDOWN:
      if (hot>0) {
        hit = hot;
        presskey = cKeys[hit].cNormal;
      }
    }
  }
  
  if (1) {
    //color_background_ctrl(c->rc, _RGB(222, 222, 222), _RGB(122, 122, 122), true, true);
    for (i=0; i<nKeys; ++i) {
      const KEYDEF* key = cKeys + i;
      if((key->cNormal == 0x00) && (key->cShifted == 0x00)) {
      }
      else {
        DrawKey(g, rcKeys[i], key, hot==i, hit==i, io->font);
      }
    }
  }
  return presskey;
}

int test_screenkeyboard_ctrl(ctrl_t* c) {
  char ch;
  if (ch = screenkeyboard_ctrl(c)) {
    printf("%02x\n", ch);
  }
  return 0;
}

#undef WM_ICON_NOTIFY
#undef TIMER_ID
#undef TAB
#undef CAPSLOCK
#undef SKEY_SHIFT
#undef CTRL
#undef BSPACE
#undef ENTER
#undef ALT
#undef WINDOW
#undef LEFT
#undef RIGHT
#undef UP
#undef DOWN
#undef ESC
#undef PUP
#undef PDN
#undef HOME
#undef END
#undef INS
#undef DEL
#undef WWW
#undef SCREENKEYBOARD_ADDKEY
