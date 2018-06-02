// BoxManClass.cpp: implementation of the BoxManClass class.
//
//////////////////////////////////////////////////////////////////////
#include "BoxManClass.h"
#define BM_THREAD_BUFFER_SIZE 0x40000
struct PathStruct {
  char manx;
  char many;
  char boxx;
  char boxy;
};
const char WALLB = 0;
const char NULLWALLB = 1;
const char NULLB = 2;
const char BOXB = 3;
const char BOXATPOINTB = 4;
const char POINTB = 5;
const char MANB = 6;
const char MANATPOINTB = 7;
const char dx[4] = { 1, 0, -1, 0 };
const char dy[4] = { 0, -1, 0, 1 };
const char pausetimer = 50;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BoxManClass::BoxManClass()
{
  _ThreadCommand = BM_COMMAND_STOP_THREAD;
  _ThreadMessage = BM_MESSAGE_THINK_STOPED;
  m_size = 30;
  m_CanUndo = false;
  m_pmanmoveway = NULL;
  m_pDrawWnd = NULL;
  m_CanMoveToBadPoint = false;
  _ThinkingMap = 0;
  try {
    CFile fmap("Map.dat", CFile::modeRead);
    m_MapFileLength = fmap.GetLength();
    m_hmap = ::GlobalAlloc(GHND, m_MapFileLength);
    void* p = (void*)::GlobalLock(m_hmap);
    fmap.Read(p, m_MapFileLength);
    ::GlobalUnlock(m_hmap);
    fmap.Close();
  }
  catch (CFileException* fe) {
    switch (fe->m_cause) {
    case CFileException::fileNotFound:
      ::MessageBox(NULL, "没有找到文件\"Map.dat\",程序不能继续.", fe->m_strFileName,
          MB_OK | MB_ICONERROR);
      ::ExitProcess(1);
    default:
      ::MessageBox(NULL, "文件错误,程序不能继续.", fe->m_strFileName,
          MB_OK | MB_ICONERROR);
      ::ExitProcess(2);
    }
  }
  try {
    CFile fway("MoveWay.dat", CFile::modeRead);
    m_wayfilelength = fway.GetLength();
    m_hway = ::GlobalAlloc(GHND, m_wayfilelength);
    void* p = (void*)::GlobalLock(m_hway);
    fway.Read(p, m_wayfilelength);
    ::GlobalUnlock(m_hway);
    fway.Close();
  }
  catch (CFileException* fe) {
    ::MessageBeep(MB_ICONEXCLAMATION);
    switch (fe->m_cause) {
    case CFileException::fileNotFound:
      ::MessageBox(NULL, "没有找到文件\"MvoeWay.dat\",程序不能继续.", fe->m_strFileName ,
          MB_OK | MB_ICONERROR);
      ::ExitProcess(1);
    default:
      ::MessageBox(NULL, "文件错误,程序不能继续.", fe->m_strFileName ,
          MB_OK | MB_ICONERROR);
      ::ExitProcess(2);
    }
  }
  m_wayfilechanged = false;
  m_nowmap = 1;
  // ReadMap( m_nowmap );
}
BoxManClass::~BoxManClass()
{
  _ThreadCommand = BM_COMMAND_STOP_THREAD;
  if (m_wayfilechanged) {
    try {
      CFile fway("MoveWay.dat", CFile::modeWrite);
      fway.SetLength(m_wayfilelength);
      fway.SeekToBegin();
      void* p = (void*)::GlobalLock(m_hway);
      fway.Write(p, m_wayfilelength);
      ::GlobalUnlock(m_hway);
      fway.Close();
    }
    catch (CFileException* fe) {
      switch (fe->m_cause) {
      case CFileException::fileNotFound:
        ::MessageBox(NULL, "没有找到文件\"MvoeWay.dat\",走法不能纪录.", fe->m_strFileName ,
            MB_OK | MB_ICONERROR);
        ::ExitProcess(1);
      default:
        ::MessageBox(NULL, "文件错误,走法不能纪录.", fe->m_strFileName ,
            MB_OK | MB_ICONERROR);
        ::ExitProcess(2);
      }
    }
  }
  ::GlobalFree(m_hmap);
  ::GlobalFree(m_hway);
  DeleteAllPath();
  m_mapB.DeleteObject();
}
void BoxManClass::SetDrawWnd(CWnd* pWnd)
{
  m_pDrawWnd = pWnd;
  CClientDC dc(m_pDrawWnd);
  m_mapB.CreateCompatibleBitmap(&dc, MAXMAPSIZEX * m_size, MAXMAPSIZEY * m_size);
}
bool BoxManClass::ReadMap(unsigned int index)
{
  // if (index != m_nowmap ) m_BoxMoveOk = false;
  char* p = (char*)::GlobalLock(m_hmap);
  char str[256];
  bool ism = false;
  int i = 0;
  for (DWORD n = 0; n < m_MapFileLength; n++, p++) {
    if (ism) {
      if (*p != '\r') {
        str[i++] = *p;
      }
      else {
        str[i] = 0;
        unsigned int value = atoi(str);
        if (value == index) {
          //find the map
          if (index != m_nowmap) {
            _ThreadCommand = BM_COMMAND_STOP_THREAD;
            //never delete the {::Sleep(1);}
            for (int tt = 0; tt < 100 && _ThreadMessage == BM_MESSAGE_THINKING ; tt++) {
              ::Sleep(1);
            }
            //while ( _ThreadMessage == BM_MESSAGE_THINKING ) ::Sleep(2);
          }
          m_nowmap = index;
          p += 2;
          char* tp = p;
          m_x = 0;
          while (*tp != '\r') {
            m_x++;
            tp++;
          }
          m_y = 1;
          tp += 2;
          while ((*tp == '+') || (*tp == '-')) {
            m_y++;
            tp += (2 + m_x);
          }
          if (m_x > MAXMAPSIZEX || m_y > MAXMAPSIZEY) {
            return false;
          }
          m_NumberOfBox = 0;
          for (int y = 0; y < m_y; y++) {
            for (int x = 0; x < m_x; x++) {
              m_map[x][y] = *p++;
              switch (m_map[x][y]) {
              case '@':
                m_map[x][y] = '.';
                m_bnm[x][y] = '#';
                m_NumberOfBox++;
                break;
              case '$':
                m_map[x][y] = '.';
                m_bnm[x][y] = '^';
                break;
              case '^':
                m_map[x][y] = ' ';
                m_bnm[x][y] = '^';
                break;
              case '#':
                m_map[x][y] = ' ';
                m_bnm[x][y] = '#';
                m_NumberOfBox++;
                break;
              default:
                m_bnm[x][y] = 0;
              }
            }
            p += 2;
          }
          ::GlobalUnlock(m_hmap);
          if (m_x < 10) {
            m_mapsizex = 10;
          }
          else {
            m_mapsizex = m_x;
          }
          if (m_y < 8) {
            m_mapsizey = 8;
          }
          else {
            m_mapsizey = m_y;
          }
          DeleteAllPath();
          m_HaveOneWay = PrepareThinking(index);
          m_CanUndo = false;
          m_TheShowStep = 0;
          //可以开始线程
          if (!m_computerthinkout) {
            _ThreadCommand = BM_COMMAND_RUN_THREAD;
          }
          //else
          // _ThreadMessage = BM_MESSAGE_THINK_OUT;
          //////////
          return true;
        }
        i = 0;
        ism = false;
      }
    }
    else if (*p == 'M') {
      ism = true;
    }
  }
  ::GlobalUnlock(m_hmap);
  return false;
}
void BoxManClass::Paint()
{
  CClientDC dc(m_pDrawWnd);
  CDC mapDC;
  mapDC.CreateCompatibleDC(&dc);
  CBitmap* pOB = mapDC.SelectObject(&m_mapB);
  // dc.Ellipse(10,10,100,100);
  dc.BitBlt(0, 0, m_size * m_mapsizex, m_size * m_mapsizey, &mapDC, 0, 0, SRCCOPY);
  mapDC.SelectObject(pOB);
}
void BoxManClass::DrawIt(char x, char y)
{
  if ((x < 0) || (x >= m_x) || (y < 0) || (y >= m_y)) {
    return;
  }
  char index;
  switch (m_map[x][y]) {
  case '+':
    index = WALLB;
    break;
  case '-':
    index = NULLB;
    break;
  case ' ': {
    switch (m_bnm[x][y]) {
    case 0:
      index = NULLWALLB;
      break;
    case '^':
      index = MANB;
      m_mx = x;
      m_my = y;
      break;
    case '#':
      index = BOXB;
    }
  }
  break;
  case '*': {
    switch (m_bnm[x][y]) {
    case 0:
      index = NULLWALLB;
      break;
    case '^':
      index = MANB;
      m_mx = x;
      m_my = y;
      break;
    case '#':
      index = BOXB;
    }
  }
  break;
  case '.': {
    switch (m_bnm[x][y]) {
    case 0:
      index = POINTB;
      break;
    case '^':
      index = MANATPOINTB;
      m_mx = x;
      m_my = y;
      break;
    case '#':
      index = BOXATPOINTB;
    }
  }
  break;
  }
  x += (m_mapsizex - m_x) / 2;
  y += (m_mapsizey - m_y) / 2;
  CClientDC dc(m_pDrawWnd);
  //there must use CClientDC, can't use CPaintDC.
  //but I don't know why?
  CBitmap tB;
  tB.LoadBitmap(IDB_MAP);
  CDC tDC;
  tDC.CreateCompatibleDC(&dc);
  tDC.SelectObject(&tB);
  dc.BitBlt(x * m_size, y * m_size, m_size, m_size, &tDC, index * m_size, 0, SRCCOPY);
  CDC mapDC;
  mapDC.CreateCompatibleDC(&dc);
  CBitmap* pOB = mapDC.SelectObject(&m_mapB);
  mapDC.BitBlt(x * m_size, y * m_size, m_size, m_size, &tDC, index * m_size, 0, SRCCOPY);
  mapDC.SelectObject(pOB);
}
void BoxManClass::DrawAllMap()
{
  //TRACE("ASDFGHJKL;\n");
  if (m_mapsizex > m_x || m_mapsizey > m_y) {
    CClientDC dc(m_pDrawWnd);
    CBitmap tB;
    tB.LoadBitmap(IDB_MAP);
    CDC tDC;
    tDC.CreateCompatibleDC(&dc);
    tDC.SelectObject(&tB);
    CDC mapDC;
    mapDC.CreateCompatibleDC(&dc);
    CBitmap* pOB = mapDC.SelectObject(&m_mapB);
    for (char y = 0; y < m_mapsizey; y++) for (char x = 0; x < m_mapsizex; x++) {
        dc.BitBlt(x * m_size, y * m_size, m_size, m_size, &tDC, NULLB * m_size, 0, SRCCOPY);
        mapDC.BitBlt(x * m_size, y * m_size, m_size, m_size, &tDC, NULLB * m_size, 0, SRCCOPY);
      }
    mapDC.SelectObject(pOB);
  }
  for (char yy = 0; yy < m_y; yy++) for (char x = 0; x < m_x; x++) {
      DrawIt(x, yy);
    }
}
unsigned char BoxManClass::MoveTowards(char x, char y)
{
  unsigned char result = BM_ERROR_CANNOT_MOVE_THERE;
  if (x == 0 && y == 0) {
    return result;
  }
  char tx = m_mx + x;
  char ty = m_my + y;
  char ttx = tx + x;
  char tty = ty + y;
  if (m_map[tx][ty] != '+') {
    switch (m_bnm[tx][ty]) {
    case 0:
      result = BM_MAN_MOVED;
      break;
    case '#':
      if ((m_map[ttx][tty] != '+') && (m_bnm[ttx][tty] != '#')) {
        if (m_CanMoveToBadPoint) {
          result = BM_BOX_MOVED;
        }
        else if (m_map[ttx][tty] != '*') {
          result = BM_BOX_MOVED;
        }
        else {
          result = BM_MOVED_TO_BAD_POINT;
        }
      }
    }
  }
  if ((result == BM_MAN_MOVED) || (result == BM_BOX_MOVED)) {
    m_bnm[m_mx][m_my] = 0;
    DrawIt(m_mx , m_my);
    m_bnm[tx][ty] = '^';
    DrawIt(tx , ty);
    m_mx = tx;
    m_my = ty;
    if (result == BM_BOX_MOVED) {
      m_bnm[ttx][tty] = '#';
      DrawIt(ttx , tty);
      AddMapToPath(tx - x, ty - y, tx, ty);
    }
  }
  for (int j = 0; j < m_y; j++)for (int i = 0; i < m_x; i++)
      if ((m_map[i][j] == '.') && (m_bnm[i][j] != '#')) {
        return result;
      }
  BoxMoveOk(m_path);
  return BM_MOVED_OK;
}
void BoxManClass::BoxMoveOk(CPtrArray& record)
{
  // the map moved ok
  unsigned int boxmn = record.GetSize();
  char tc1 = '[';
  char tc2 = ']';
  if (&record == &m_path) {
    tc1 = '{';
    tc2 = '}';
  }
  //可以停止线程
  //_ThreadCommand = BM_COMMAND_STOP_THREAD;
  //////////
  char c[80];
  wsprintf(c, "%d", m_nowmap);
  CString str(c);
  if (m_noanswerrecoded) {
    //在MoveWay.dat的最后纪录走法
    m_wayfilechanged = true;
    ::GlobalReAlloc(m_hway, m_wayfilelength + 7 + str.GetLength() + boxmn * 4 , GHND);
    char* p = (char*)::GlobalLock(m_hway);
    p += m_wayfilelength;
    m_wayfilelength += 7 + str.GetLength() + boxmn * 4;
    *p++ = 'M';
    for (char stri = 0; stri < str.GetLength() ; stri++) {
      *p++ = str.GetAt(stri);
    }
    *p++ = '\r';
    *p++ = '\n';
    *p++ = tc1;
    for (unsigned int t = 0; t < boxmn; t++) {
      PathStruct* ps = (PathStruct*)record.GetAt(t);
      *p++ = ps->manx;
      *p++ = ps->many;
      *p++ = ps->boxx;
      *p++ = ps->boxy;
    }
    *p++ = tc2;
    *p++ = '\r';
    *p = '\n';
    ::GlobalUnlock(m_hway);
    m_boxmovestepnumber = boxmn;
  }
  else if (!m_computerthinkout && (boxmn < m_boxmovestepnumber || tc1 == '[')) {
    //改变已有的纪录,将其放在最后
    m_wayfilechanged = true;
    char* p = (char*)::GlobalLock(m_hway);
    char* MoveBufferEnd = p + m_wayfilelength;
    p += m_TheMapsMoveWaySavePoint;
    int delta = 7 + str.GetLength() + m_boxmovestepnumber * 4;
    for (char* tp = p + delta; tp < MoveBufferEnd; tp++, p++) {
      *p = *tp;
    }
    *p++ = 'M';
    for (char stri = 0; stri < str.GetLength() ; stri++) {
      *p++ = str.GetAt(stri);
    }
    *p++ = '\r';
    *p++ = '\n';
    *p++ = tc1;
    for (unsigned int t = 0; t < boxmn; t++) {
      PathStruct* ps = (PathStruct*)record.GetAt(t);
      *p++ = ps->manx;
      *p++ = ps->many;
      *p++ = ps->boxx;
      *p++ = ps->boxy;
    }
    *p++ = tc2;
    *p++ = '\r';
    *p = '\n';
    ::GlobalUnlock(m_hway);
    //一定是减小内存
    ::GlobalReAlloc(m_hway, m_wayfilelength - (m_boxmovestepnumber - boxmn) * 4, GHND);
    m_wayfilelength -= (m_boxmovestepnumber - boxmn) * 4; //::GlobalSize( m_hway );
    m_boxmovestepnumber = boxmn;
  }
}
void BoxManClass::AddMapToPath(char manx, char many, char boxx, char boxy)
{
  PathStruct* p = new PathStruct;
  p->manx = manx;
  p->many = many;
  p->boxx = boxx;
  p->boxy = boxy;
  m_path.Add(p);
  m_CanUndo = true;
}
void BoxManClass::DeleteAllPath()
{
  int n = m_path.GetSize();
  for (int i = 0; i < n; i++) {
    PathStruct* p = (PathStruct*)m_path.GetAt(i);
    delete p;
  }
  m_path.RemoveAll();
}
bool BoxManClass::Undo()
{
  if (!m_CanUndo) {
    return false;
  }
  int n = m_path.GetSize();
  PathStruct* p = (PathStruct*)m_path.GetAt(n - 1);
  m_path.RemoveAt(n - 1);
  if (n == 1) {
    m_CanUndo = false;
  }
  m_bnm[m_mx][m_my] = 0;
  m_bnm[p->boxx][p->boxy] = '#';
  m_bnm[p->manx][p->many] = '^';
  char tx = 2 * p->boxx - p->manx;
  char ty = 2 * p->boxy - p->many;
  m_bnm[tx][ty] = 0;
  DrawIt(m_mx, m_my);
  DrawIt(p->boxx, p->boxy);
  DrawIt(p->manx, p->many);
  DrawIt(tx, ty);
  m_mx = p->manx;
  m_my = p->many;
  return true;
}
char BoxManClass::PointAt(CPoint point)
{
  int tx = (m_mapsizex - m_x) / 2;
  int ty = (m_mapsizey - m_y) / 2;
  if ((point.x > (tx + m_x)*m_size) || (point.x < tx * m_size) || (point.y > (ty + m_y)*m_size) || (point.y < ty * m_size)) {
    return BM_ERROR_PIONT_OUT;
  }
  int tox = (point.x - tx * m_size) / m_size;
  int toy = (point.y - ty * m_size) / m_size;
  if (m_bnm[tox][toy] == '^') {
    return BM_ERROR_POINT_MAN;
  }
  if (m_map[tox][toy] == '+') {
    return BM_ERROR_POINT_WALL;
  }
  if (m_map[tox][toy] == '-') {
    return BM_ERROR_PIONT_OUT;
  }
  m_pmanmoveway = MoveManTo(tox, toy);
  if (m_pmanmoveway != NULL) {
    return DoManMove();
  }
  return BM_ERROR_CANNOT_MOVE_THERE;
}
CPtrArray* BoxManClass::MoveManTo(char x, char y)
{
  CPtrArray pos;
  CWordArray from;
  CPoint* pn = new CPoint;
  pn->x = x;//注意:这里是倒过来走的
  pn->y = y;
  pos.Add(pn);
  from.Add(0x7fff);
  // unsigned short head = 0;
  unsigned short now = 0;
  unsigned short end = 0;
  while (now <= end) {
    pn = (CPoint*)pos.GetAt(now);
    for (char d = 0; d < 4; d++) {
      char tx = (char)pn->x + dx[d];
      char ty = (char)pn->y + dy[d];
      if ((tx == m_mx) && (ty == m_my)) {
        CPtrArray* way = new CPtrArray;
        while (now != 0x7fff) {
          pn = (CPoint*)pos.GetAt(now);
          now = from.GetAt(now);
          CPoint* pe = new CPoint;
          pe->x = pn->x;
          pe->y = pn->y;
          way->Add(pe);
        }
        return way;
      }
      if ((m_map[tx][ty] != '+') && (m_bnm[tx][ty] != '#') && (m_bnm[tx][ty] != '^')) {
        bool have = false;
        for (unsigned short i = 0; !have && (i <= end); i++) {
          CPoint* tp = (CPoint*)pos.GetAt(i);
          if ((tp->x == tx) && (tp->y == ty)) {
            have = true;
          }
        }
        if (!have) {
          CPoint* pe = new CPoint;
          pe->x = tx;
          pe->y = ty;
          pos.Add(pe);
          from.Add(now);
          end++;
        }
      }
    }
    now++;
  }
  end = pos.GetSize();
  for (unsigned short i = 0; i < end; i++) {
    pn = (CPoint*)pos.GetAt(i);
    delete pn;
  }
  pos.RemoveAll();
  from.RemoveAll();
  return NULL;
}
unsigned char BoxManClass::DoManMove()
{
  unsigned char result;
  for (int i = 0; i < m_pmanmoveway->GetSize(); i++) {
    CPoint* p = (CPoint*)m_pmanmoveway->GetAt(i);
    char tx = p->x - m_mx;
    char ty = p->y - m_my;
    delete p;
    result = MoveTowards(tx, ty);
    ::Sleep(50);
  }
  m_pmanmoveway->RemoveAll();
  m_pmanmoveway = NULL;
  return result;
}
bool BoxManClass::PrepareThinking(unsigned int index)
{
  //优化地图
  char x = m_x - 1;
  char y = m_y - 1;
  for (char j = 1; j < y; j++) for (char i = 1; i < x; i++) {
      if (m_map[i][j] == ' ') {
        int u = 0;
        for (int d = 0; d < 4; d++) {
          if (m_map[i + dx[d]][j + dy[d]] == '+') {
            u++;
            bool canputbox = false;
            int td = (d + 3) % 4;
            int tx = i;
            int ty = j;
            while (!canputbox) {
              tx += dx[td];
              ty += dy[td];
              if (m_map[tx][ty] == '+') {
                break;
              }
              if (m_map[tx][ty] == '.') {
                canputbox = true;
              }
              else if (m_map[tx + dx[d]][ty + dy[d]] != '+') {
                canputbox = true;
              }
            }
            td = (d + 1) % 4;
            tx = i;
            ty = j;
            while (!canputbox) {
              tx += dx[td];
              ty += dy[td];
              if (m_map[tx][ty] == '+') {
                break;
              }
              if (m_map[tx][ty] == '.') {
                canputbox = true;
              }
              else if (m_map[tx + dx[d]][ty + dy[d]] != '+') {
                canputbox = true;
              }
            }
            if (!canputbox) {
              m_map[i][j] = '*';
            }
          }
          else { //endof if '+'
            if (u > 1) {
              m_map[i][j] = '*';
            }
            u = 0;
          }
        }//endof for (d~4)
        if ((u > 1) || ((u > 0) && (m_map[i + dx[0]][j + dy[0]] == '+'))) {
          m_map[i][j] = '*';
        }
      }//endof if map == ' '
    }//endof for (i , j)
  //分析moveway.dat文件
  char* p = (char*)::GlobalLock(m_hway);
  char str[256];
  bool ism = false;
  char i = 0;
  m_noanswerrecoded = false;
  for (DWORD n = 0; (n < m_wayfilelength); n++, p++) {
    if (ism) {
      if (*p != '\r') {
        str[i++] = *p;
      }
      else {
        str[i] = 0;
        unsigned int value = atoi(str);
        if (value == index) {
          //find the map way
          p += 2;
          if (*p == '[') {
            m_computerthinkout = true;
          }
          else {
            m_computerthinkout = false;
          }
          for (char* tp = p; (*tp != ']') && (*tp != '}'); tp++); //注:无循环体
          m_boxmovestepnumber = (tp - p) / 4;
          ::GlobalUnlock(m_hway);
          return true;
        }
        i = 0;
        m_TheMapsMoveWaySavePoint = -1;
        ism = false;
      }
    }
    else if (*p == 'M') {
      ism = true;
      m_TheMapsMoveWaySavePoint = n;
    }
  }
  ::GlobalUnlock(m_hway);
  m_noanswerrecoded = true;
  m_computerthinkout = false;
  return false;
}
void BoxManClass::ShowMoveWay()
{
  static CPtrArray path;
  static int TotalStep = 0;
  if (m_TheShowStep == 0) {
    for (int it = 0; it < path.GetSize(); it++) {
      PathStruct* ps = (PathStruct*)path.GetAt(it);
      delete ps;
    }
    path.RemoveAll();
    TotalStep = 0;
    if (m_CanUndo) {
      while (m_CanUndo) {
        Undo();
      }
      //::MessageBox( m_pDrawWnd->m_hWnd, "开始演示", "Message", 0 );
      PathStruct* ps1 = new PathStruct;
      ps1->manx = m_mx;
      ps1->many = m_my;
      ps1->boxx = m_mx;
      ps1->boxy = m_my;
      path.Add(ps1);
      TotalStep++;
    }
    char* p = (char*)::GlobalLock(m_hway);
    for (p += m_TheMapsMoveWaySavePoint; (*p != '{') && (*p != '['); p++); //no body
    p++;
    //TRACE( "%d\t", path.GetSize() );
    for (unsigned int i = 0; i < m_boxmovestepnumber; i++) {
      PathStruct* ps = new PathStruct;
      ps->manx = *p++;
      ps->many = *p++;
      ps->boxx = *p++;
      ps->boxy = *p++;
      path.Add(ps);
      TotalStep++;
    }
    //TRACE( "%d\n", path.GetSize() );
    ::GlobalUnlock(m_hway);
  }
  PathStruct* ps = (PathStruct*)path.GetAt(m_TheShowStep);
  if ((m_mx != ps->manx) || (m_my != ps->many)) {
    m_pmanmoveway = MoveManTo(ps->manx, ps->many);
    if (m_pmanmoveway != NULL) {
      DoManMove();
    }
  }
  ::Sleep(50);
  MoveTowards(ps->boxx - ps->manx, ps->boxy - ps->many);
  ::Sleep(50);
  m_RemainShewStep = TotalStep - ++m_TheShowStep;
  if (path.GetSize() == m_TheShowStep) {
    for (int i = 0; i < m_TheShowStep; i++) {
      PathStruct* ps = (PathStruct*)path.GetAt(i);
      delete ps;
    }
    path.RemoveAll();
    m_TheShowStep = m_RemainShewStep = 0;
    ::MessageBox(m_pDrawWnd->m_hWnd, "演示完毕!", "Message", MB_ICONASTERISK | MB_ICONINFORMATION);
  }
}
void BoxManClass::ThinkTheWay()
{
  if (_ThreadCommand == BM_COMMAND_STOP_THREAD) {
    _ThreadMessage = BM_MESSAGE_THINK_OUT;
    return;
  }
  _ThreadMessage = BM_MESSAGE_THINKING;
  unsigned char i, j;
  if (_ThinkingMap == m_nowmap) {
    return;
  }
  else {
    _ThinkingMap = m_nowmap;
  }
  _x = m_x - 1;
  _y = m_y - 1;
  _x2 = m_x - 2;
  _y2 = m_y - 2;
  _structSize = (m_NumberOfBox + 1) + 4;
  for (j = 0; j < m_y; j++) for (i = 0; i < m_x; i++) {
      _con[i][j] = m_bnm[i][j];
    }
  //there must use 0 to m_x, can't use 1 to _x, but i don't know why.
  //TRACE("
  _InitData();
  bool tb = _ThinkIt();
  ::GlobalFree(_head);
  if (_ThreadMessage == BM_MESSAGE_THINKING) {
    _ThreadMessage = 0;
  }
}
void BoxManClass::_InitData()
{
  unsigned char* p, **ap;
  unsigned char i, j;
  //定义一个计算用的内存
  p = (unsigned char*)GlobalAlloc(GPTR, _structSize * BM_THREAD_BUFFER_SIZE);
  _head = _now = p;
  _end = p + _structSize;
  ap = (unsigned char**)(_end - 4);
  *ap = NULL;
  for (j = 1; j < _y; j++) for (i = 1; i < _x; i++) {
      if (_con[i][j] == '#') {
        //*p++ = i;
        //*p++ = j;
        *p++ = (j - 1) * _x2 + (i - 1);
      }
    }
  //用'^'着色
  bool havechange;
  do {
    havechange = false;
    for (j = 1; j < _y; j++) for (i = 1; i < _x; i++) {
        if (m_map[i][j] != '+' && _con[i][j] == 0 &&
            (_con[i + 1][j] == '^' || _con[i - 1][j] == '^' || _con[i][j + 1] == '^' || _con[i][j - 1] == '^')) {
          _con[i][j] = '^';
          havechange = true;
        }
      }
    if (!havechange) {
      break;
    }
    havechange = false;
    for (i = _x - 1; i > 0; i--) for (j = _y - 1; j > 0; j--) {
        if (m_map[i][j] != '+' && _con[i][j] == 0 &&
            (_con[i + 1][j] == '^' || _con[i - 1][j] == '^' || _con[i][j + 1] == '^' || _con[i][j - 1] == '^')) {
          _con[i][j] = '^';
          havechange = true;
        }
      }
  }
  while (havechange);
  //纪录人的位置
  for (j = 1; j < _y; j++) for (i = 1; i < _x; i++) {
      if (_con[i][j] == '^') {
        //*p++ = i;
        //*p = j;
        *p = (j - 1) * _x2 + (i - 1);
        return;
      }
    }
  return;
}
inline void BoxManClass::_UnpackData(unsigned char* p)
{
  unsigned char i, j;
  //注意:这里一定是MAXMAPSIZEY*(_x-1)
  ::memset(MAXMAPSIZEY + (char*)_con, 0, MAXMAPSIZEY * (_x - 1));
  //for ( j=1; j<_y; j++ ) for ( i=1; i<_x; i++ ) _con[i][j] = 0;
  for (i = 0; i < m_NumberOfBox; i++, p++) {
    _con[ *p % _x2 + 1 ][ *p / _x2 + 1 ] = '#';//放'#'
  }
  //放'^'
  _con[ *p % _x2 + 1 ][ *p / _x2 + 1 ] = '^';
  //用'^'着色
  bool havechange;
  do {
    havechange = false;
    for (j = 1; j < _y; j++) for (i = 1; i < _x; i++) {
        if (m_map[i][j] != '+' && _con[i][j] == 0 &&
            (_con[i + 1][j] == '^' || _con[i - 1][j] == '^' || _con[i][j + 1] == '^' || _con[i][j - 1] == '^')) {
          _con[i][j] = '^';
          havechange = true;
        }
      }
    if (!havechange) {
      break;
    }
    havechange = false;
    for (i = _x - 1; i > 0; i--) for (j = _y - 1; j > 0; j--) {
        if (m_map[i][j] != '+' && _con[i][j] == 0 &&
            (_con[i + 1][j] == '^' || _con[i - 1][j] == '^' || _con[i][j + 1] == '^' || _con[i][j - 1] == '^')) {
          _con[i][j] = '^';
          havechange = true;
        }
      }
  }
  while (havechange);
}
inline bool BoxManClass::_IsOk(unsigned char* p)
{
  bool ok = true;
  for (int i = 0; ok && (i < m_NumberOfBox); i++ , p++) {
    ok = (m_map[ *p % _x2 + 1 ][ *p / _x2 + 1 ] == '.');
  }
  return ok;
}
bool BoxManClass::_ThinkIt()
{
  unsigned char* np, nx, ny, fx, fy, bx, by, k, d;
  int numberofnode = 0;
  bool conChanged;
  bool Ok;
  Ok = _IsOk(_end - _structSize);
  if (Ok) {
    //MsgBox( "ERROR", "The Map needn't think." );
    _ThreadMessage = BM_MESSAGE_THINK_OUT;
    ::MessageBeep(MB_OK);
    return true;
  }
  while (_now != _end && _ThreadCommand != BM_COMMAND_STOP_THREAD) {
    _UnpackData(_now);
    conChanged = false;
    //move
    np = _now;
    for (k = 0; k < m_NumberOfBox; k++, np++) {
      //nx = *np++;
      //ny = *np++;
      nx = *np % _x2 + 1;
      ny = *np / _x2 + 1;
      for (d = 0; d < 4; d++) {
        fx = nx + dx[d];
        fy = ny + dy[d];
        bx = nx - dx[d];
        by = ny - dy[d];
        if (_ThreadCommand == BM_COMMAND_STOP_THREAD) {
          _ThreadMessage = BM_MESSAGE_THINK_STOPED;
          return false;
        }
        if (conChanged) {
          _UnpackData(_now);
          conChanged = false;
        }
        if (m_map[fx][fy] != '+' && m_map[fx][fy] != '*' && _con[fx][fy] != '#' && _con[bx][by] == '^') {
          _con[fx][fy] = '#';
          _con[nx][ny] = '%';//移动后的人
          _con[bx][by] = '%';
          conChanged = true;
          _PackData();
          if (!_AlreadyHave()) {
            Ok = _IsOk(_end);
            _end += _structSize;
            if (BM_THREAD_BUFFER_SIZE < ++numberofnode) {
              _ThreadMessage = BM_MESSAGE_BUFFER_FULL;
              ::MessageBeep(MB_ICONHAND);
              return false;
            }
            if (Ok) {
              _OutputToFile();
              //char s[40];
              //wsprintf(s,"OK! 共产生了%d个节点",numberofnode);
              //MsgBox( "OK", s );
              _ThreadMessage = BM_MESSAGE_THINK_OUT;
              ::MessageBeep(MB_OK);
              return true;
            }
          }
        }//endof can move
      }//endof for d<4
    }//endof for k<m_NumberOfBox
    _now += _structSize;
  }//endof while !IsOk
  //UnpackData( head );
  //char s[40];
  //wsprintf(s,"No Answer! 共产生了%d个节点",numberofnode);
  //MsgBox( "ERROR", s );
  if (_now == _end) {
    _ThreadMessage = BM_MESSAGE_NO_ANSWER;
    ::MessageBeep(MB_ICONQUESTION);
  }
  else {
    _ThreadMessage = BM_MESSAGE_THINK_STOPED;
  }
  return false;
}
void BoxManClass::_OutputToFile()
{
  unsigned char* tp, **ap, *p;
  //将链表的方向颠倒
  p = _end - _structSize;
  tp = NULL;
  while (p != _head) {
    unsigned char* np = p;
    ap = (unsigned char**)(p + _structSize - 4);
    p = *ap;
    *ap = tp;
    tp = np;
  }
  ap = (unsigned char**)(p + _structSize - 4);
  *ap = tp;
  _now = _head;
  //将路径记入way中
  char pcon[MAXMAPSIZEX][MAXMAPSIZEY];
  unsigned char i, j;
  CPtrArray way;
  _UnpackData(_now);
  for (j = 1; j < _y; j++) for (i = 0; i < _x; i++) {
      pcon[i][j] = _con[i][j];
    }
  // char **
  ap = (unsigned char**)(_now + _structSize - 4);
  _now = *ap;
  while (_now != NULL) {
    _UnpackData(_now);
    PathStruct* ps = new PathStruct;
    for (j = 1; j < _y; j++) for (i = 1; i < _x; i++) {
        if (_con[i][j] == '^' && pcon[i][j] == '#') {
          ps->boxx = i;
          ps->boxy = j;
        }
      }
    for (j = 1; j < _y; j++) for (i = 1; i < _x; i++) {
        if (_con[i][j] == '#' && pcon[i][j] != '#') {
          ps->manx = ps->boxx * 2 - i;
          ps->many = ps->boxy * 2 - j;
        }
      }
    way.Add(ps);
    for (j = 1; j < _y; j++) for (i = 0; i < _x; i++) {
        pcon[i][j] = _con[i][j];
      }
    // char **
    ap = (unsigned char**)(_now + _structSize - 4);
    _now = *ap;
  }
  //纪录入内存中
  BoxMoveOk(way);
  //清除way;
  for (i = 0; i < way.GetSize(); i++) {
    PathStruct* ps = (PathStruct*)way.GetAt(i);
    delete ps;
  }
  way.RemoveAll();
  PrepareThinking(m_nowmap);
}
inline bool BoxManClass::_AlreadyHave()
{
  unsigned char* p = _head , *nextp, *temp, *ep;
  bool equal = false;
  while (!equal && (p != _end)) {
    nextp = p + _structSize;
    temp = nextp - 4;
    equal = true;
    for (ep = _end; equal && (p < temp); p++, ep++) {
      equal = (*p == *ep);
    }
    p = nextp;
  }
  return equal;
}
inline void BoxManClass::_PackData()
{
  //用'%'着色
  char i, j;
  bool havechange;
  do {
    havechange = false;
    for (j = 1; j < _y; j++) for (i = 1; i < _x; i++) {
        if (m_map[i][j] != '+' && (_con[i][j] == 0 || _con[i][j] == '^') &&
            (_con[i + 1][j] == '%' || _con[i - 1][j] == '%' || _con[i][j + 1] == '%' || _con[i][j - 1] == '%')) {
          _con[i][j] = '%';
          havechange = true;
        }
      }
    if (!havechange) {
      break;
    }
    havechange = false;
    for (i = _x - 1; i > 0; i--) for (j = _y - 1; j > 0; j--) {
        if (m_map[i][j] != '+' && (_con[i][j] == 0 || _con[i][j] == '^') &&
            (_con[i + 1][j] == '%' || _con[i - 1][j] == '%' || _con[i][j + 1] == '%' || _con[i][j - 1] == '%')) {
          _con[i][j] = '%';
          havechange = true;
        }
      }
  }
  while (havechange);
  //endof 用'%'着色
  //加入新节点
  unsigned char* p = _end, **ap;
  //char manY, manX=0;
  unsigned char manXY;
  for (j = 1; j < _y; j++) for (i = 1; i < _x; i++) {
      if (_con[i][j] == '#') {
        //*p++ = i;
        //*p++ = j;
        *p++ = (j - 1) * _x2 + (i - 1);
      }
      else if (_con[i][j] == '%') {
        //manX = i;
        //manY = j;
        manXY = (j - 1) * _x2 + (i - 1);
      }
    }
  *p++ = manXY;
  //*p++ = manY;
  ap = (unsigned char**)p;
  *ap = _now;
}

