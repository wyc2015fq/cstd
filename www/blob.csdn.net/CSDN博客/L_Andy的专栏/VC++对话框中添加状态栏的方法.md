# VC++对话框中添加状态栏的方法 - L_Andy的专栏 - CSDN博客

2013年02月27日 11:47:46[卡哥](https://me.csdn.net/L_Andy)阅读数：1236


方法一：

1.添加成员变量CStatusBarCtrl m_StatusBar;

2.在OnInitDialog()中加入：

                  m_StatusBar.Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW, CRect(0,0,0,0), this, 0);

                  int strPartDim[3]= {100, 200, -1}; //分割数量

                  m_StatusBar.SetParts(3, strPartDim);

                  //设置状态栏文本

                  m_StatusBar.SetText("分栏一", 0, 0);

                  m_StatusBar.SetText("分栏二", 1, 0);

                  m_StatusBar.SetText("分栏三", 2, 0);

                  //下面是在状态栏中加入图标

                  m_StatusBar.SetIcon(1, 

                                SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),

                                FALSE));//为第二个分栏中加的图标

方法二：

1.添加成员变量CStatusBar m_wndStatusBar;

2.在OnInitDialog()中加入：

                  static UINT indicators[] =

                  {

                                ID_SEPARATOR,                        // status line indicator

                                ID_INDICATOR_CAPS,              //CAP lock indicator.

                                ID_INDICATOR_NUM,               //NUM lock indicator.

                                ID_INDICATOR_SCRL,              //SCRL lock indicator.

                  };

                  if (!m_wndStatusBar.Create(this) ||

                                !m_wndStatusBar.SetIndicators(indicators,

                                  sizeof(indicators)/sizeof(UINT)))

                  {

                                TRACE0("Failed to create status barn");

                                return -1;         // fail to create

                  }

                  UINT nID;        //控制状态栏里面的分栏

                  m_wndStatusBar.SetPaneInfo(0,nID,SBPS_STRETCH|SBPS_NOBORDERS,100);      //返回值存nID中

                  m_wndStatusBar.SetPaneText(0,"就绪");

                  m_wndStatusBar.SetPaneInfo(1,nID,SBPS_NORMAL,100);

                  m_wndStatusBar.SetPaneText(1,"大写");

                  m_wndStatusBar.SetPaneInfo(2,nID,SBPS_POPOUT,100);

                  m_wndStatusBar.SetPaneText(2,"数字");

//                SetPaneInfo()函数的第三个参数的可选项如下：

//                The following indicator styles are supported:

//              SBPS_NOBORDERS     No 3-D border around the pane.

//              SBPS_POPOUT            Reverse border so that text "pops out."

//              SBPS_DISABLED         Do not draw text.

//              SBPS_STRETCH          Stretch pane to fill unused space. Only one pane per status bar can have this style.

//              SBPS_NORMAL            No stretch, borders, or pop-out.

//----------------让这个状态栏最终显示在对话框中-------------

                  RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);

