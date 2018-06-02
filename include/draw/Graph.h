
//color definitions
#define RED        _RGB(255,0,0)
#define GREEN      _RGB(0,255,0)
#define BLUE      _RGB(0,0,255)
#define YELLOW      _RGB(255,255,0)
#define ORANGE      _RGB(255,153,51)
#define HOT_PINK    _RGB(255,51,153)
#define PURPLE      _RGB(153,0,204)
#define CYAN      _RGB(0,255,255)
#define BLACK      _RGB(0,0,0)
#define WHITE      _RGB(255,255,255)
#define LAVENDER    _RGB(199,177,255)
#define PEACH      _RGB(255,226,177)
#define SKY_BLUE    _RGB(142,255,255)
#define FOREST_GREEN  _RGB(0,192,0)
#define BROWN      _RGB(80,50,0)
#define TURQUOISE    _RGB(0,192,192)
#define ROYAL_BLUE    _RGB(0,0,192)
#define GREY      _RGB(192,192,192)
#define DARK_GREY    _RGB(128,128,128)
#define TAN        _RGB(255,198,107)
#define DARK_BLUE    _RGB(0,0,128)
#define MAROON      _RGB(128,0,0)
#define DUSK      _RGB(255,143,107)
#define LIGHT_GREY    _RGB(225,225,225)  //only for 3D graph lines

//for graph type definitions :
//single number = 2D type graph
//number in 20s = 2D type graph related to graph of single number
//number in 30s = 3D type graph related to graph of single number
//graph type definitions
#define BAR_GRAPH         0
#define LINE_GRAPH        1  //lines between plotted points
#define PIE_GRAPH         2
#define SCATTER_GRAPH      3
#define BOX_WHISKER_GRAPH    4
//#define RADAR_GRAPH        5
//specialty graphs
#define STACKED_BAR_GRAPH    20
#define XY_LINE_GRAPH      21  //straight line, no circles at data points
#define BAR_GRAPH_3D      30
#define LINE_GRAPH_3D      31  //looks like a "tape" graph
#define PIE_GRAPH_3D      32
#define STACKED_BAR_GRAPH_3D  320

#define VERTICAL_ALIGN    1
#define HORIZONTAL_ALIGN  0

/////////////////////////////////////////////////////////////////////////////
// CGraph window

typedef struct CGraphLegendSet {
  char* label;
  COLORREF color;

  RECT rcLabel;
  RECT rcColor;
} CGraphLegendSet;

struct CGraphPieLabel {
  int lineXStart;
  int lineYStart;
  int lineXEnd;
  int lineYEnd;
  int topLeftX;
  int topLeftY;
  int labelQuadrant;
  char pieLabel[32];  //i.e.  30% - actual percentile

};


struct CGraph {

  char** seriesLabel;
  int nseries;

  CGraphLegendSet* legend;
  int nlegend;

  int* datay;

  int graphType;  //0 - bar graph, 1 - line graph, 2 - pie

  char* axisYLabel;
  char* axisXLabel;

  BOOL graphHasLegend;

  char* graphTitle;
  BOOL graphHasGridLines;


#define STRLEN(s)  (int)strlen(s)

  int legendTitleFont;
  int legendFont;
  int titleFont;

#include "Graph.inl"


  void set(int type) {
    seriesLabel = 0;

    graphHasLegend = FALSE;

    graphType = type;

    graphHasGridLines = FALSE;

    legendTitle = "Legend";

    titleFont = font_create("Arial", 24, FONT_BOLD);
    legendTitleFont = font_create("Arial", 16, 0);
    legendFont = font_create("Arial", 12, 0);
    axisFont = font_create("Arial", 12, 0);
    sideFont = axisFont;
  }
  int axisFont;
  int sideFont;
  const char* legendTitle;

  RECT rcTitle;
  RECT rcLegend;
  RECT rcLegendTitle;
  RECT rcAxis;

  int hitrect(RECT rc) {
    RECT rc1;
    RECT graphRect = rc;
    SIZE szTitle;
    TSelectFont(titleFont);
    szTitle = FontGetTextExtent(graphTitle);
    RectCutTop(&graphRect, szTitle.cy + 10, 0, &rcTitle);
    rcAxis = graphRect;

    if (graphHasLegend) {
      int i;
      SIZE szLegend;
      SIZE szLlegendTitle;
      int magin = 2;
      int legendBarWidth = 20;
      SIZE szTextMax;

      TSelectFont(legendTitleFont);
      szLlegendTitle = FontGetTextExtent(legendTitle);
      TSelectFont(legendFont);

      szTextMax = cSIZE(0, 0);

      for (i = 0; i < nlegend; i++) {
        SIZE sz = FontGetTextExtent(legend[i].label);
        szTextMax = SZMAX(szTextMax, sz);
      }

      szTextMax.cx += 4, szTextMax.cy += 0;
      szLegend = cSIZE(szTextMax.cx + legendBarWidth + magin * 4, (szTextMax.cy + magin * 2) * nlegend + szLlegendTitle.cy + magin * 2);
      RectCutLeft(&rcAxis, -szLegend.cx - 20, 0, &rcLegend);
      RectAlign(rcLegend, szLegend.cx, szLegend.cy, DT_CENTER | DT_VCENTER, &rcLegend);
      rc1 = rcLegend;
      RectCutTop(&rc1, szLlegendTitle.cy + magin * 2, 0, &rcLegendTitle);

      for (i = 0; i < nlegend; i++) {
        RECT rcColor, rcLabel;
        RectCutTop(&rc1, szTextMax.cy + magin * 2, 0, &rcColor);
        RectCutLeft(&rcColor, magin * 2, 0, &rcLabel);
        RectCutLeft(&rcColor, szTextMax.cx, 0, &rcLabel);
        RectIn(rcColor, magin * 2, &rcColor);
        legend[i].rcLabel = rcLabel;
        legend[i].rcColor = rcColor;
      }
    }

    if (1) {
    }

    return 0;
  }



};

