// DockTest.c
//
// Simple demo of the Docking Toolwindow library (DockWnd.Dll)
//
// This must be linked with comctl32.lib and dockwnd.lib. The MSVC project
// file already has these two files added to the linker settings.
//
// When running DockTest.exe, the DockWnd.dll must be copied to the same
// directory as the exe, or copied to your system32 directory. If not,
// Windows will pop up a message box complaining that DockWnd.dll is
// missing.

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
//#include <afxres.h>
#include "resource.h"
#include "DockWnd.h"
#include "DockWnd.inl"

#pragma comment(lib, "comctl32.lib")
// Which menu heading number the "Window" menu is, referenced from 0
#define WINDOWMENUOFFSET 2

HINSTANCE InstanceHandle;
HWND    MainFrameWindow;
HWND    MainWindow;
//HMENU   FrameMenu=0;
//HMENU   ClientMenu=0;

const TCHAR szAppName[] = _T("Owner window");

const TCHAR MDIClientClass[] = _T("MDICLIENT");
const TCHAR ClientClassName[] = _T("MyMDIClient");

const TCHAR TreeParent1[] = _T("Parent 1");
const TCHAR TreeParent2[] = _T("Parent 2");
const TCHAR TreeItem1[] = _T("Item 1");
const TCHAR TreeItem2[] = _T("Item 2");
const TCHAR TreeItem3[] = _T("Item 3");

// Error messages
const TCHAR NoDockAPI[] = _T("Can't initialize Docking API!");
const TCHAR CantRegWindow[] = _T("Can't register window!");
const TCHAR CantCreateWindow[] = _T("Can't create window!");
const TCHAR NoMem[] = _T("Out of memory!");
const TCHAR CantLoadMenu[] = _T("Can't load menu!");













/*********************** myResize() ************************
 * Called by the docking library when the tool window that
 * hosts our edit and push button controls is resized. We
 * need to size and position those 2 controls here.
 *
 * dw =   DOCKINFO of the tool window.
 * rect = The area that we need to fill, relative to the
 *      tool window's client area.
 */

void WINAPI myResize(DOCKINFO* dw, RECT* rect)
{
  HWND  child;

  // Position the button above the bottom border
  child = GetDlgItem(dw->hwnd, IDC_MYBUTTON);
  SetWindowPos(child, 0, rect->left + 10, rect->bottom - 20, 50, 18, SWP_NOZORDER | SWP_NOACTIVATE);

  // Let the edit fill the remaining area
  child = GetDlgItem(dw->hwnd, IDC_MYEDIT);
  SetWindowPos(child, 0, rect->left, rect->top, rect->right - rect->left, (rect->bottom - rect->top) - 22, SWP_NOZORDER | SWP_NOACTIVATE);
}




/*********************** myEditCmds() ************************
 * Called by the docking library when the tool window that
 * hosts our edit and push button controls receives a message
 * that it doesn't handle, such as WM_COMMAND or WM_NOTIFY.
 *
 * dw =   DOCKINFO of the tool window.
 * message, wParam, lParam =  The standard parameters to a
 *                window procedure.
 */


LRESULT WINAPI myEditCmds(DOCKINFO* dw, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message) {
  case WM_COMMAND: {
    // Is it the edit control?
    if (LOWORD(wParam) == IDC_MYEDIT) {
      if (HIWORD(wParam) == EN_CHANGE) {
        // Here we would handle the EN_CHANGE, and then return
        // 0 to tell the docking library we handled it.
        return 0;
      }

      break;
    }

    // Is it the button control?
    if (LOWORD(wParam) == IDC_MYBUTTON) {
      if (HIWORD(wParam) == BN_CLICKED) {
        // Clear the edit control, and then return
        // 0 to tell the docking library we handled it.
        SendDlgItemMessage(dw->hwnd, IDC_MYEDIT, WM_SETTEXT, 0, (LPARAM)"");
        return 0;
      }

      break;
    }
  }
  }

  // Return -1 if we want the docking library to do default handling.
  return -1;
}





/*********************** myTreeCmds() ************************
 * Called by the docking library when the tool window that
 * hosts our treeview control receives a message that it
 * doesn't handle, such as WM_COMMAND or WM_NOTIFY.
 *
 * dw =   DOCKINFO of the tool window.
 * message, wParam, lParam =  The standard parameters to a
 *                window procedure.
 */

LRESULT WINAPI myTreeCmds(DOCKINFO* dw, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message) {
  case WM_NOTIFY: {
    // Here you would handle a WM_NOTIFY from the treeview and
    // return an appropriate value
  }
  }

  // Return -1 if we want the docking library to do default handling.
  return -1;
}





/*********************** myToolbarCmds() ************************
 * Called by the docking library when the tool window that
 * hosts our toolbar control receives a message that it
 * doesn't handle, such as WM_COMMAND or WM_NOTIFY.
 *
 * dw =   DOCKINFO of the tool window.
 * message, wParam, lParam =  The standard parameters to a
 *                window procedure.
 */

LRESULT WINAPI myToolbarCmds(DOCKINFO* dw, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message) {
  case WM_NOTIFY: {
    // Here you would handle a WM_NOTIFY from the toolbar and
    // return an appropriate value
  }
  }

  // Return -1 if we want the docking library to do default handling.
  return -1;
}




#define TOOLBARSTYLES WS_CHILD | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | CCS_NOPARENTALIGN |  CCS_NORESIZE| CCS_NODIVIDER

TBBUTTON tbb[] = {
  { STD_FILENEW,  0,  TBSTATE_ENABLED, TBSTYLE_BUTTON,  0, 0, 0, 0 },
  { STD_FILEOPEN, 0,  TBSTATE_ENABLED, TBSTYLE_BUTTON,  0, 0, 0, 1 },
  { STD_FILESAVE, 0,  TBSTATE_ENABLED, TBSTYLE_BUTTON,  0, 0, 0, 2 },
  { 0,        0,  TBSTATE_ENABLED, TBSTYLE_SEP,   0, 0, 0, 0 },
  { STD_CUT,    0,          0, TBSTYLE_BUTTON,  0, 0, 0, 3 },
  { STD_COPY,   0,          0, TBSTYLE_BUTTON,  0, 0, 0, 4 },
  { STD_PASTE,    0,  TBSTATE_ENABLED, TBSTYLE_BUTTON,  0, 0, 0, 5 },
  { 0,        0,  TBSTATE_ENABLED, TBSTYLE_SEP,   0, 0, 0, 0 },
  { STD_UNDO,   0,          0, TBSTYLE_BUTTON,  0, 0, 0, 6 },
  { STD_REDOW,    0,          0, TBSTYLE_BUTTON,  0, 0, 0, 7 },
  { STD_FIND,   0,  TBSTATE_ENABLED, TBSTYLE_BUTTON,  0, 0, 0, 8 },
};

char szTBStrings[] = "New\0Open\0Save\0Cut\0Paste\0Copy\0Undo\0Redo\0Find\0\0";

/*********************** createTB() ************************
 * Creates a toolbar.
 */

HWND createTB(HWND hwndParent)
{
  RECT  r;
  DWORD iNumButtons;
  HWND  hwndTB;

  iNumButtons = 12;

  if ((hwndTB = CreateToolbarEx(hwndParent,
      WS_CHILD | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_NODIVIDER,
      1, iNumButtons,
      HINST_COMMCTRL,
      IDB_STD_SMALL_COLOR,
      tbb,
      iNumButtons,
      55, 36, 36, 36,
      sizeof(TBBUTTON)))) {
    // Calculate toolbar height
    GetWindowRect(hwndTB, &r);

    // Add the strings to the toolbar
    SendMessage(hwndTB, TB_ADDSTRING, 0, (LPARAM)szTBStrings);
    SendMessage(hwndTB, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
    SendMessage(hwndTB, TB_AUTOSIZE, 0, 0);

    SetWindowText(hwndTB, _T("Toolbar"));
  }

  return(hwndTB);
}




/********************** createContentWindow() **********************
 * Creates one of three types of windows. We create this window as
 * the child of some Docking Frame (that we created with
 * DockingCreateFrame). It will fill the client area of the Docking
 * Frame, and therefore become the "contents" of some tool window.
 *
 * dw =     DOCKINFO for the tool window.
 * item =   How many docking frame windows we have open.
 *
 * RETURNS: 1 if success, or 0 if failure.
 */

unsigned createContentWindow(DOCKINFO* dw, DWORD item)
{
  // Create the contents based upon how many tool windows we have open.
  // This is just for demonstration (to present a variety of contents,
  // and would not be how you would normally determine your contents
  switch (item % 3) {
  case 0: {
    // Create a tool bar as the contents, and let the docking
    // library give it the focus and resize it.  Note that we
    // make the tool window its parent, and create the tool bar
    // with the WS_CHILD style
    if ((dw->focusWindow = createTB(dw->hwnd))) {
      // Provide a function that handles the WM_COMMAND (and other
      // message) sent by this control
      dw->DockMsg = myToolbarCmds;

      return(1);
    }

    break;
  }

  case 1: {
    // Create an EDIT control, and let the docking
    // library give it the focus
    if ((dw->focusWindow = CreateWindowEx(WS_EX_CLIENTEDGE,
        "EDIT", "Contents",
        ES_MULTILINE | WS_VSCROLL | WS_CHILD | WS_VISIBLE,
        0, 0, 0, 0,
        dw->hwnd,
        (HMENU)IDC_MYEDIT, InstanceHandle, 0))) {
      // Also create a push button. Note that we make the tool window
      // its parent too
      if (CreateWindow("BUTTON", "Clear",
          BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
          0, 0, 0, 0,
          dw->hwnd,
          (HMENU)IDC_MYBUTTON, InstanceHandle, 0)) {
        // Because we have more than one window as the contents of
        // our tool window, we need to provide a function to do the
        // sizing and positioning of these controls whenever the
        // tool window is resized. We'll call our function myResize()
        // and we stuff a pointer into the DOCKINFO DockResize field
        dw->DockResize = myResize;

        // Provide a function that handles the WM_COMMAND (and other
        // message) sent by these two controls
        dw->DockMsg = myEditCmds;

        // Success
        return(1);
      }
    }

    break;
  }

  default: {
    HWND      hwndTree;
    TV_INSERTSTRUCT insertStruct;

    if ((dw->focusWindow = hwndTree = CreateWindowEx(WS_EX_CLIENTEDGE,
        WC_TREEVIEW, "Tree",
        WS_VSCROLL | WS_CHILD | WS_VISIBLE | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT,
        0, 0, 0, 0,
        dw->hwnd,
        (HMENU)IDC_MYTREE, InstanceHandle, 0))) {
      insertStruct.hInsertAfter = TVI_LAST;
      insertStruct.hParent = 0;
      insertStruct.item.mask = TVIF_TEXT | TVIF_STATE;
      insertStruct.item.state = TVIS_EXPANDED | TVIS_SELECTED;
      insertStruct.item.pszText = (LPTSTR)TreeParent1;

      if ((insertStruct.hParent = TreeView_InsertItem(hwndTree, &insertStruct))) {
        insertStruct.item.mask = TVIF_TEXT;

        insertStruct.item.pszText = (LPTSTR)TreeItem1;
        TreeView_InsertItem(hwndTree, &insertStruct);
        insertStruct.item.pszText = (LPTSTR)TreeItem2;
        TreeView_InsertItem(hwndTree, &insertStruct);
        insertStruct.item.pszText = (LPTSTR)TreeItem3;
        TreeView_InsertItem(hwndTree, &insertStruct);

        insertStruct.hParent = 0;
        insertStruct.item.pszText = (LPTSTR)TreeParent2;

        if ((insertStruct.hParent = TreeView_InsertItem(hwndTree, &insertStruct))) {
          insertStruct.item.mask = TVIF_TEXT;
          insertStruct.item.pszText = (LPTSTR)TreeItem1;
          TreeView_InsertItem(hwndTree, &insertStruct);
          insertStruct.item.pszText = (LPTSTR)TreeItem2;
          TreeView_InsertItem(hwndTree, &insertStruct);
        }
      }

      // Provide a function that handles the WM_COMMAND (and other
      // message) sent by this control
      dw->DockMsg = myTreeCmds;

      return(1);
    }
  }
  }

  return(0);
}






/************************* clientWndProc() *************************
 * Window procedure for MDI client (document) windows.
 */

LRESULT CALLBACK clientWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg) {
  case WM_CREATE: {
    HWND    hwndEdit;

    // Create an edit control over the MDI child.
    hwndEdit = CreateWindow("edit", NULL, WS_CHILD | WS_HSCROLL | WS_MAXIMIZE |
        WS_VISIBLE | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL |
        ES_MULTILINE, 0, 0, 0, 0, hwnd, (HMENU)IDC_CLIENT, InstanceHandle,
        NULL);

    // Store the edit control's window handle in our client's USERDATA, so we can access it
    // later when we need to
    SetWindowLong(hwnd, GWL_USERDATA, (LONG)hwndEdit);

    // Give it the focus
    SetFocus(hwndEdit);

    // fall through
  }

  case WM_SIZE: {
    HWND    hwndEdit;

    hwndEdit = (HWND)GetWindowLong(hwnd, GWL_USERDATA);

    // Move/size the edit control to fill the MDI child's client area
    MoveWindow(hwndEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);

    break; // DefMDIChildProc must process WM_SIZE.
  }

  case WM_COMMAND: {
    if (lParam && LOWORD(wParam) == IDC_CLIENT) {
      HWND    hwndEdit;

      hwndEdit = (HWND)GetWindowLong(hwnd, GWL_USERDATA);

      switch (HIWORD(wParam)) {
      case EN_UPDATE:
      case EN_CHANGE: {
        // Edit control's contents changed. Here you typically
        // would fetch the contents of the EDIT control and
        // do something with it
        break;
      }
      }
    }

    return(0);
  }
  }

  return(DefMDIChildProc(hwnd, msg, wParam, lParam));
}






/************************* frameWndProc() ***********************
 * Window procedure for main frame window.
 */

LRESULT CALLBACK frameWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  static int x = 200, y = 200;

  switch (msg) {
  case WM_CREATE: {
    CLIENTCREATESTRUCT  ccs;

    // Let the OS know where our "Window" menu is within our menus
    ccs.hWindowMenu = 0;
    ccs.idFirstChild = IDR_CLIENT;

    // Create an MDI client to host all of our client windows
    if ((MainWindow = CreateWindow(&MDIClientClass[0], 0, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
        0, 0, 0, 0, hwnd, (HMENU)1, InstanceHandle,
        (LPSTR)&ccs))) {
      return(0);
    }

    return(-1);
  }

  case WM_NCACTIVATE: {
    DOCKPARAMS  dockParams;

    dockParams.container = dockParams.hwnd = hwnd;
    dockParams.wParam = wParam;
    dockParams.lParam = lParam;
    return(DockingActivate(&dockParams));
  }

  case WM_ENABLE: {
    DOCKPARAMS  dockParams;

    dockParams.container = dockParams.hwnd = hwnd;
    dockParams.wParam = wParam;
    dockParams.lParam = lParam;
    return(DockingEnable(&dockParams));
  }

  case WM_DESTROY: {
    PostQuitMessage(0);
    return(0);
  }

  case WM_CLOSE: {
    DestroyWindow(hwnd);
    return(0);
  }

  case WM_SIZE: {
    HDWP  hdwp;
    RECT  rect;

    // Do the default handling of this message
    DefFrameProc(hwnd, MainWindow, msg, wParam, lParam);

    // Set the area where Docking Frame windows are allowed.
    // (Take into account any status bar, toolbar etc).
    rect.left = rect.top = 0;
    rect.right = LOWORD(lParam);
    rect.bottom = HIWORD(lParam);

    // Allocate enough space for all Docking Frames which are actually docked
    hdwp = BeginDeferWindowPos(DockingCountFrames(hwnd, 1) + 1);

    // Position the docked Docking Frame windows for this container
    // window. rect will be modified to contain the "inner" client
    // rectangle, where we can position an MDI client
    DockingArrangeWindows(hwnd, hdwp, &rect);

    // Here we resize our MDI client window so that it fits into the area
    // described by "rect". Do not let it extend outside of this
    // area or it (and the client windows inside of it) will be obscured
    // by docked toolwindows (or vice versa).
    DeferWindowPos(hdwp, MainWindow, 0, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOACTIVATE | SWP_NOZORDER);

    EndDeferWindowPos(hdwp);

    // Here, you really should figure out what state the contents of the MDI window is in.
    // For example, if some client window is maximized to fill the MDI client, then you
    // would simply leave things as above. On the other hand, if there are a bunch of
    // client windows visible, then you could rearrange them within the new (reduced)
    // area of the MDI client like so:
    //
    // SendMessage(MainWindow, WM_MDICASCADE, 0, 0);
    //
    // Not doing the above may leave some clients covered by a docking frame. But, the
    // user can remedy this situation himself by selecting the tile or cascade items
    // under the Window menu. So, it's not a big deal.

    return(0);
  }

  case WM_COMMAND: {
    switch (LOWORD(wParam)) {

    case IDM_VIEW_TOOLWINDOW: {
      DOCKINFO*  dw;
      HWND    frame = 0;
      TCHAR   szName[64];
      char    docked;

      // Set the window title for the Docking Frame when floating
      wParam = DockingCountFrames(hwnd, 0) + 1;
      wsprintf(szName, "Tool Window %d", wParam);

      // Vary the side to which it is docked, according to how many docking
      // frames we have open. This is just for demonstration purposes.
      switch (wParam % 5) {
      case 0:
        docked = (char)DWS_DOCKED_BOTTOM;
        break;

      case 1:
        docked = (char)DWS_DOCKED_TOP;
        break;

      case 2:
        docked = (char)DWS_DOCKED_LEFT;
        break;

      case 3:
        docked = (char)DWS_DOCKED_RIGHT;
        break;

      default:
        docked = (char)DWS_FLOATING;
        break;
      }

      // Allocate a DOCKINFO struct
      if ((dw = DockingAlloc(docked))) {
        // Prevent sizing. Or, you may wish to alter some other defaults set by DockingAlloc
        // dw->dwStyle |= DWS_NORESIZE;

        // Create a Docking Frame window. Note that the docking library fills
        // in DOCKINFO's hwnd field with the HWND of the tool window
        if (DockingCreateFrame(dw, hwnd, szName)) {
          // Create the child window(s) that will be hosted inside
          // of the Docking Frame window (ie, the contents of the
          // docking window's client area)
          if (createContentWindow(dw, wParam)) {
            // Show the Docking Frame. Note: The content window(s)
            // will be sized and positioned at this point
            DockingShowFrame(dw);
            break;
          }

          // NOTE: If any errors, the docking library will free the above
          // DOCKINFO, because we're using the default DockDestroy callback
          DestroyWindow(frame);
        }

        MessageBox(MainWindow, &CantCreateWindow[0], &szAppName[0], MB_OK);
      }
      else {
        MessageBox(MainWindow, &NoMem[0], &szAppName[0], MB_OK);
      }

      break;
    }

    case IDM_FILE_NEW: {
      MDICREATESTRUCT   mcs;
      HWND        child;

      // Create another client window
      ZeroMemory(&mcs, sizeof(MDICREATESTRUCT));
      mcs.szTitle = "Untitled";
      mcs.szClass = &ClientClassName[0];
      mcs.hOwner = InstanceHandle;
      mcs.x = mcs.cx = mcs.y = mcs.cy = CW_USEDEFAULT;
      //          mcs.style = 0;

      child = (HWND)SendMessage(MainWindow, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mcs);
      break;
    }

    }
  }
  }

  return(DefFrameProc(hwnd, MainWindow, msg, wParam, lParam));
}







/********************* createMainFrameWindow() *******************
 * Sets up and creates the main window.
 *
 * Registers the Window classes for the main application frame
 * window and the MDI child windows. Loads the 2 menus for the
 * MDI document and the main frame. Creates the main frame
 * window.
 */

BOOL createMainFrameWindow(void)
{
  WNDCLASSEX  wc;

  // Register the main frame class
  ZeroMemory(&wc, sizeof(WNDCLASSEX));

  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_CLASSDC | CS_PARENTDC;
  wc.lpfnWndProc = frameWndProc;
  //  wc.cbClsExtra = 0;
  wc.cbWndExtra = sizeof(DOCKINFO*);    // We'll keep the list of DOCKINFOs in the extra data area of the frame window
  wc.hInstance = InstanceHandle;
  wc.hIcon = LoadIcon(0, MAKEINTRESOURCE(IDR_MAINFRAME));
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  //  wc.lpszMenuName = 0;
  wc.lpszClassName = &szAppName[0];
  wc.hIconSm = LoadIcon(NULL, MAKEINTRESOURCE(IDR_MAINFRAME));

  if (RegisterClassEx(&wc)) {
    // Register the class for MDI child windows
    wc.lpfnWndProc = clientWndProc;
    wc.lpszClassName = &ClientClassName[0];
    wc.cbWndExtra = 0;
    wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDR_CLIENT));
    wc.hIconSm = LoadIcon(NULL, MAKEINTRESOURCE(IDR_CLIENT));

    if (RegisterClassEx(&wc)) {
      // Load the menu for the frame
      // Load the menu for a client window
      // Create the frame window
      if ((MainFrameWindow = CreateWindow(&szAppName[0], &szAppName[0],
          WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
          CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0,
          0, InstanceHandle, 0))) {
        // Success
        return(1);
      }

      MessageBox(0, &CantLoadMenu[0], &szAppName[0], MB_OK);
      goto bad;
    }
  }

  MessageBox(0, &CantRegWindow[0], &szAppName[0], MB_OK);
bad:
  return(0);
}


/**************************** WinMain() ************************
 * Program entry point.
 *
 */

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iShowCmd)
int DockTest()
{
  MSG     msg;
  HACCEL    hAccelTable;

  InstanceHandle = GetModuleHandle(0);

  init(InstanceHandle, DLL_PROCESS_ATTACH);

  // Initialize use of DockWnd.dll
  if (DockingInitialize(0)) {
    MessageBox(0, &NoDockAPI[0], &szAppName[0], MB_OK);
#ifndef _DEBUG
    ExitProcess((unsigned) - 1);
    return;
#else
    return(-1);
#endif
  }

  // Register and create the main (container) window
  if (createMainFrameWindow()) {
    ShowWindow(MainFrameWindow, SW_SHOWDEFAULT);
    UpdateWindow(MainFrameWindow);

    // Load the accelerator table
    hAccelTable = LoadAccelerators(InstanceHandle, MAKEINTRESOURCE(IDR_MAINFRAME));

    // Do message loop
    while (GetMessage(&msg, 0, 0, 0) == 1) {
      if (!TranslateMDISysAccel(MainWindow, &msg) && !TranslateAccelerator(MainFrameWindow, hAccelTable, &msg)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
  }

  // Free docking library
  DockingUnInitialize();

  init(InstanceHandle, DLL_PROCESS_DETACH);
  // Exit
#ifndef _DEBUG
  ExitProcess(0);
  return;
#else
  return(0);
#endif
}
