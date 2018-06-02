#ifndef _DIAGRAMENTITY_H_
#define _DIAGRAMENTITY_H_

#define CMD_START   100
#define CMD_CUT     100
#define CMD_COPY    101
#define CMD_DUPLICATE   102
#define CMD_PROPERTIES    103
#define CMD_UP      104
#define CMD_DOWN    105
#define CMD_FRONT   106
#define CMD_BOTTOM    107
#define CMD_SELECT_GROUP  108
#define CMD_END     200

#define DEHT_NONE     0
#define DEHT_BODY     1
#define DEHT_TOPLEFT    2
#define DEHT_TOPMIDDLE    3
#define DEHT_TOPRIGHT   4
#define DEHT_BOTTOMLEFT   5
#define DEHT_BOTTOMMIDDLE   6
#define DEHT_BOTTOMRIGHT    7
#define DEHT_LEFTMIDDLE   8
#define DEHT_RIGHTMIDDLE    9

struct CDiagramPropertyDlg;

struct CDiagramEntity {
  RECT m_rc;
  BOOL  m_selected;
  // Data
  str_t m_type[1];
  str_t m_title[1];
  str_t m_name[1];

#include "DiagramEntity.inl"
};

#endif // _DIAGRAMENTITY_H_
