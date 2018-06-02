
#include <math.h>


typedef struct {

  int x;
  int y;
  BOOL hit;

} hitParams;

typedef struct {

  RECT rect;
  BOOL hit;

} hitParamsRect;

int GetNewGroup()
{
  static int m_sCurrentGroup = 0;

  m_sCurrentGroup++;
  return m_sCurrentGroup;

}

#include "DiagramEntity.h"

// Current mouse mode
#define MODE_NONE      0
#define MODE_RUBBERBANDING  1
#define MODE_MOVING      2
#define MODE_RESIZING    3
#define MODE_DRAWING    4
#define MODE_BGRESIZING    5

// Restraint modes
#define RESTRAINT_NONE    0
#define RESTRAINT_VIRTUAL  1
#define RESTRAINT_MARGIN  2

#define KEY_NONE          0
#define KEY_ARROW         1 // Arrow keys
#define KEY_PGUPDOWN      4  // Pg up & pg down
#define KEY_DELETE        8  // Delete key
#define KEY_ESCAPE       16  // Escape key
#define KEY_INSERT       32  // Insert key
#define KEY_PLUSMINUS    64  // Plus- and minus key
#define KEY_CTRL        128  // Ctrl+A,Z,X,C,V, Enter

#ifndef ID_EDIT_GROUP
#define ID_EDIT_GROUP    ID_EDIT_REDO + 1
#define ID_EDIT_UNGROUP    ID_EDIT_GROUP + 1
#endif

#define WHEEL_SCROLL  0
#define WHEEL_ZOOM    1

/////////////////////////////////////////////////////////////////////////////
// CDiagramEditor window

struct CDiagramEditor {
  CDiagramEntity*    m_objs;
  int m_objsNum;
  int        m_maxstacksize;
  SIZE      m_virtualSize;


  int      m_interactMode;    // Current mouse-mode
  int      m_subMode;      // Sub-mode for resizing (corner)
  RECT    m_selectionRect;  // Rect to draw for rubberbanding
  POINT    m_deltaPoint;    // Offset to object when moving
  BOOL    m_drawing;      // We are currently drawing
  BOOL    m_multiDraw;    // If the drawing mode is continued
  // after an object is added.

  CDiagramEntity* m_multiSelObj;  // Primary object when moving multiple

  // Properties
  COLORREF  m_bkgndCol;      // Background of paper area
  COLORREF  m_nonClientBkgndCol;// Background of non-paper area

  int      m_bgResizeZone;    // Size, in pixels, of resize zone
  BOOL    m_bgResize;      // TRUE if the paper can be resized
  BOOL    m_bgResizeSelected;  // TRUE if we are resizing the background

  BOOL    m_isSnapToGrid;        // TRUE if we should snap to grid
  BOOL    m_isGridVisible;        // TRUE if the background grid should be displayed
  SIZE    m_gridSize;      // Size of a grid cell
  COLORREF  m_gridCol;      // Color of the grid

  BOOL    m_isMarginVisible;      // TRUE if margins should be drawn
  COLORREF  m_marginColor;    // Color of the margin

  RECT m_rcMargin;  // margin in pixels

  int      m_restraint;    // Restraint mode ( none, virtual or margin )

  SIZE    m_markerSize;    // Size of selection marker

  UINT    m_keyInterface;    // Flags for the keys the editor will handle

#include "DiagramEditor.inl"
};
