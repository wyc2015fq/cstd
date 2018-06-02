// BoxManClass.h: interface for the BoxManClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOXMANCLASS_H__A8DB6093_6EBD_11D3_9C7F_9D98C5BF25E9__INCLUDED_)
#define AFX_BOXMANCLASS_H__A8DB6093_6EBD_11D3_9C7F_9D98C5BF25E9__INCLUDED_

//my
#define BM_MOVED_TO_BAD_POINT 0
#define BM_ERROR_POINT_MAN 1
#define BM_ERROR_CANNOT_MOVE_THERE 2
#define BM_ERROR_POINT_WALL 3
#define BM_ERROR_PIONT_OUT 4
#define BM_MOVED_OK 5
#define BM_BOX_MOVED 6
#define BM_MAN_MOVED 7

#define BM_MESSAGE_THINK_STOPED 100
#define BM_MESSAGE_THINKING 101
#define BM_MESSAGE_NO_ANSWER 102
#define BM_MESSAGE_BUFFER_FULL 103
#define BM_MESSAGE_THINK_OUT 104

#define BM_COMMAND_RUN_THREAD 200
#define BM_COMMAND_STOP_THREAD 201


const char MAXMAPSIZEX = 20;
const char MAXMAPSIZEY = 16;
//end

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BoxManClass
{
public:
  BoxManClass();
  virtual ~BoxManClass();

  // Implementation
public:
  unsigned int _ThreadMessage;
  unsigned int m_nowmap;
  int m_RemainShewStep;
  unsigned char m_NumberOfBox;
  void ThinkTheWay();
  bool m_CanMoveToBadPoint;
  int m_TheShowStep;
  void ShowMoveWay();
  int m_TheMapsMoveWaySavePoint;
  bool m_HaveOneWay;
  bool PrepareThinking(unsigned int index);
  unsigned char DoManMove();
  IPOINT* MoveManTo(char x, char y);
  char PointAt(IPOINT point);
  unsigned char m_size;
  unsigned char m_mapsizex;
  unsigned char m_mapsizey;
  unsigned char m_x;
  unsigned char m_y;
  unsigned int m_boxmovestepnumber;
  bool m_noanswerrecoded;
  //  bool m_BoxMoveOk;
  bool m_CanUndo;
  bool Undo();
  void DeleteAllPath();
  void AddMapToPath(char manx, char many, char boxx, char boxy);
  void BoxMoveOk(CPtrArray& record);
  unsigned char MoveTowards(char x, char y);
  void DrawAllMap();
  void DrawIt(char x, char y);
  void Paint();
  bool ReadMap(unsigned int index);
  void SetDrawWnd(CWnd* pWnd);

  // Attributes
protected:
  bool m_computerthinkout;
  CPtrArray m_path;
  CWnd* m_pDrawWnd;
  //  unsigned char m_stepnumber;
  unsigned char m_mx;
  unsigned char m_my;
  unsigned char m_map[MAXMAPSIZEX][MAXMAPSIZEY];
  unsigned char m_bnm[MAXMAPSIZEX][MAXMAPSIZEY];
  //  bool m_manmoving;
  bool m_wayfilechanged;
  CPtrArray* m_pmanmoveway;
  GLOBALHANDLE m_hmap;
  GLOBALHANDLE m_hway;
  DWORD m_wayfilelength;
  DWORD m_MapFileLength;
  CBitmap m_mapB;
private:
  inline void _PackData();
  inline bool _AlreadyHave();
  inline bool _IsOk(unsigned char* p);
  inline void _UnpackData(unsigned char* p);
  void _OutputToFile();
  bool _ThinkIt();
  void _InitData();
  unsigned int _ThreadCommand;
  unsigned int _ThinkingMap;
  unsigned char* _now, *_head, *_end;
  unsigned char _x, _x2;
  unsigned char _y, _y2;
  unsigned char _structSize;
  char _con[MAXMAPSIZEX][MAXMAPSIZEY];
};

#endif // !defined(AFX_BOXMANCLASS_H__A8DB6093_6EBD_11D3_9C7F_9D98C5BF25E9__INCLUDED_)
