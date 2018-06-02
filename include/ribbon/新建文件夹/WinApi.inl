using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Drawing;
using VisualStyles;
namespace RibbonHelpers
{
/// Provides Windows Operative System specific functionallity.
static struct WinApi {
  //#region Constants
  const int WM_MOUSEFIRST = 0x0200;
  const int WM_MOUSEMOVE = 0x0200;
  const int WM_LBUTTONDOWN = 0x0201;
  const int WM_LBUTTONUP = 0x0202;
  const int WM_LBUTTONDBLCLK = 0x0203;
  const int WM_RBUTTONDOWN = 0x0204;
  const int WM_RBUTTONUP = 0x0205;
  const int WM_RBUTTONDBLCLK = 0x0206;
  const int WM_MBUTTONDOWN = 0x0207;
  const int WM_MBUTTONUP = 0x0208;
  const int WM_MBUTTONDBLCLK = 0x0209;
  const int WM_MOUSEWHEEL = 0x020A;
  const int WM_XBUTTONDOWN = 0x020B;
  const int WM_XBUTTONUP = 0x020C;
  const int WM_XBUTTONDBLCLK = 0x020D;
  const int WM_MOUSELAST = 0x020D;
  const int WM_KEYDOWN = 0x100;
  const int WM_KEYUP = 0x101;
  const int WM_SYSKEYDOWN = 0x104;
  const int WM_SYSKEYUP = 0x105;
  const byte VK_SHIFT = 0x10;
  const byte VK_CAPITAL = 0x14;
  const byte VK_NUMLOCK = 0x90;
  const int DTT_COMPOSITED = (int)(1UL << 13);
  const int DTT_GLOWSIZE = (int)(1UL << 11);
  const int DT_SINGLELINE = 0x00000020;
  const int DT_CENTER = 0x00000001;
  const int DT_VCENTER = 0x00000004;
  const int DT_NOPREFIX = 0x00000800;
  /// Enables the drop shadow effect on a window
  const int CS_DROPSHADOW = 0x00020000;
  /// Windows NT/2000/XP: Installs a hook procedure that monitors low-level mouse input events.
  const int WH_MOUSE_LL = 14;
  /// Windows NT/2000/XP: Installs a hook procedure that monitors low-level keyboard  input events.
  const int WH_KEYBOARD_LL = 13;
  /// Installs a hook procedure that monitors mouse messages.
  const int WH_MOUSE = 7;
  /// Installs a hook procedure that monitors keystroke messages.
  const int WH_KEYBOARD = 2;
  /// The WM_NCLBUTTONUP message is posted when the user releases the left mouse button while the cursor is within the nonclient area of a window.
  const int WM_NCLBUTTONUP = 0x00A2;
  /// The WM_SIZE message is sent to a window after its size has changed.
  const int WM_SIZE = 0x5;
  /// The WM_ERASEBKGND message is sent when the window background must be erased (for example, when a window is resized).
  const int WM_ERASEBKGND = 0x14;
  /// The WM_NCCALCSIZE message is sent when the size and position of a window's client area must be calculated.
  const int WM_NCCALCSIZE = 0x83;
  /// The WM_NCHITTEST message is sent to a window when the cursor moves, or when a mouse button is pressed or released.
  const int WM_NCHITTEST = 0x84;
  /// The WM_NCMOUSEMOVE message is posted to a window when the cursor is moved within the nonclient area of the window.
  const int WM_NCMOUSEMOVE = 0x00A0;
  /// The WM_NCMOUSELEAVE message is posted to a window when the cursor leaves the nonclient area of the window specified in a prior call to TrackMouseEvent.
  const int WM_NCMOUSELEAVE = 0x2a2;
  /// An uncompressed format.
  const int BI_RGB = 0;
  /// The BITMAPINFO structure contains an array of literal RGB values.
  const int DIB_RGB_COLORS = 0;
  /// Copies the source rectangle directly to the destination rectangle.
  const int SRCCOPY = 0x00CC0020;
  //#endregion
  //#region Dll Imports
  [DllImport("user32")]
  static extern bool GetCursorPos(out POINT lpPoint);
  /// The ToAscii function translates the specified virtual-key code and keyboard state to the corresponding character or characters.
  /// <param name="uVirtKey"></param>
  /// <param name="uScanCode"></param>
  /// <param name="lpbKeyState"></param>
  /// <param name="lpwTransKey"></param>
  /// <param name="fuState"></param>
  /// <returns></returns>
  [DllImport("user32")]
  static extern int ToAscii(int uVirtKey, int uScanCode, byte[] lpbKeyState, byte[] lpwTransKey, int fuState);
  /// The GetKeyboardState function copies the status of the 256 keys to the specified buffer.
  /// <param name="pbKeyState"></param>
  /// <returns></returns>
  [DllImport("user32")]
  static extern int GetKeyboardState(byte[] pbKeyState);
  /// This function retrieves the status of the specified key. The status specifies whether the key is up, down, or toggled on or off ¡ª alternating each time the key is pressed.
  /// <param name="vKey"></param>
  /// <returns></returns>
  [DllImport("user32.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.StdCall)]
  static extern short GetKeyState(int vKey);
  /// Installs an application-defined hook procedure into a hook chain
  /// <param name="idHook"></param>
  /// <param name="lpfn"></param>
  /// <param name="hInstance"></param>
  /// <param name="threadId"></param>
  /// <returns></returns>
  [DllImport("user32.dll")]
  static extern int SetWindowsHookEx(int idHook, GlobalHook.HookProcCallBack lpfn, IntPtr hInstance, int threadId);
  /// Removes a hook procedure installed in a hook chain by the SetWindowsHookEx function.
  /// <param name="idHook"></param>
  /// <returns></returns>
  [DllImport("user32.dll")]
  static extern bool UnhookWindowsHookEx(int idHook);
  /// Passes the hook information to the next hook procedure in the current hook chain
  /// <param name="idHook"></param>
  /// <param name="nCode"></param>
  /// <param name="wParam"></param>
  /// <param name="lParam"></param>
  /// <returns></returns>
  [DllImport("user32.dll")]
  static extern int CallNextHookEx(int idHook, int nCode, IntPtr wParam, IntPtr lParam);
  /// This function retrieves a handle to a display device context (DC) for the client area of the specified window.
  /// <param name="hdc"></param>
  /// <returns></returns>
  [DllImport("user32.dll")]
  static extern IntPtr GetDC(IntPtr hdc);
  /// The SaveDC function saves the current state of the specified device context (DC) by copying data describing selected objects and graphic modes
  /// <param name="hdc"></param>
  /// <returns></returns>
  [DllImport("gdi32.dll")]
  static extern int SaveDC(IntPtr hdc);
  /// This function releases a device context (DC), freeing it for use by other applications. The effect of ReleaseDC depends on the type of device context.
  /// <param name="hdc"></param>
  /// <param name="state"></param>
  /// <returns></returns>
  [DllImport("user32.dll")]
  static extern int ReleaseDC(IntPtr hdc, int state);
  /// Draws text using the color and font defined by the visual style. Extends DrawThemeText by allowing additional text format options.
  /// <param name="hTheme"></param>
  /// <param name="hdc"></param>
  /// <param name="iPartId"></param>
  /// <param name="iStateId"></param>
  /// <param name="text"></param>
  /// <param name="iCharCount"></param>
  /// <param name="dwFlags"></param>
  /// <param name="pRect"></param>
  /// <param name="pOptions"></param>
  /// <returns></returns>
  [DllImport("UxTheme.dll", CharSet = CharSet.Unicode)]
  static extern int DrawThemeTextEx(IntPtr hTheme, IntPtr hdc, int iPartId, int iStateId, string text, int iCharCount, int dwFlags, ref RECT pRect, ref DTTOPTS pOptions);
  /// Draws text using the color and font defined by the visual style.
  /// <param name="hTheme"></param>
  /// <param name="hdc"></param>
  /// <param name="iPartId"></param>
  /// <param name="iStateId"></param>
  /// <param name="text"></param>
  /// <param name="iCharCount"></param>
  /// <param name="dwFlags1"></param>
  /// <param name="dwFlags2"></param>
  /// <param name="pRect"></param>
  /// <returns></returns>
  [DllImport("UxTheme.dll")]
  static extern int DrawThemeText(IntPtr hTheme, IntPtr hdc, int iPartId, int iStateId, string text, int iCharCount, int dwFlags1, int dwFlags2, ref RECT pRect);
  /// The CreateDIBSection function creates a DIB that applications can write to directly.
  /// <param name="hdc"></param>
  /// <param name="pbmi"></param>
  /// <param name="iUsage"></param>
  /// <param name="ppvBits"></param>
  /// <param name="hSection"></param>
  /// <param name="dwOffset"></param>
  /// <returns></returns>
  [DllImport("gdi32.dll")]
  static extern IntPtr CreateDIBSection(IntPtr hdc, ref BITMAPINFO pbmi, uint iUsage, int ppvBits, IntPtr hSection, uint dwOffset);
  /// This function transfers pixels from a specified source rectangle to a specified destination rectangle, altering the pixels according to the selected raster operation (ROP) code.
  /// <param name="hdc"></param>
  /// <param name="nXDest"></param>
  /// <param name="nYDest"></param>
  /// <param name="nWidth"></param>
  /// <param name="nHeight"></param>
  /// <param name="hdcSrc"></param>
  /// <param name="nXSrc"></param>
  /// <param name="nYSrc"></param>
  /// <param name="dwRop"></param>
  /// <returns></returns>
  [DllImport("gdi32.dll")]
  static extern bool BitBlt(IntPtr hdc, int nXDest, int nYDest, int nWidth, int nHeight, IntPtr hdcSrc, int nXSrc, int nYSrc, uint dwRop);
  /// This function creates a memory device context (DC) compatible with the specified device.
  /// <param name="hDC"></param>
  /// <returns></returns>
  [DllImport("gdi32.dll")]
  static extern IntPtr CreateCompatibleDC(IntPtr hDC);
  /// This function selects an object into a specified device context. The new object replaces the previous object of the same type.
  /// <param name="hDC"></param>
  /// <param name="hObject"></param>
  /// <returns></returns>
  [DllImport("gdi32.dll")]
  static extern IntPtr SelectObject(IntPtr hDC, IntPtr hObject);
  /// The DeleteObject function deletes a logical pen, brush, font, bitmap, region, or palette, freeing all system resources associated with the object. After the object is deleted, the specified handle is no longer valid.
  /// <param name="hObject"></param>
  /// <returns></returns>
  [DllImport("gdi32.dll")]
  static extern bool DeleteObject(IntPtr hObject);
  /// The DeleteDC function deletes the specified device context (DC).
  /// <param name="hdc"></param>
  /// <returns></returns>
  [DllImport("gdi32.dll")]
  static extern bool DeleteDC(IntPtr hdc);
  /// Extends the window frame behind the client area.
  /// <param name="hdc"></param>
  /// <param name="marInset"></param>
  /// <returns></returns>
  [DllImport("dwmapi.dll")]
  static extern int DwmExtendFrameIntoClientArea(IntPtr hdc, ref MARGINS marInset);
  /// Default window procedure for Desktop Window Manager (DWM) hit-testing within the non-client area.
  /// <param name="hwnd"></param>
  /// <param name="msg"></param>
  /// <param name="wParam"></param>
  /// <param name="lParam"></param>
  /// <param name="result"></param>
  /// <returns></returns>
  [DllImport("dwmapi.dll")]
  static extern int DwmDefWindowProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, out IntPtr result);
  /// Obtains a value that indicates whether Desktop Window Manager (DWM) composition is enabled.
  /// <param name="pfEnabled"></param>
  /// <returns></returns>
  [DllImport("dwmapi.dll")]
  static extern int DwmIsCompositionEnabled(ref int pfEnabled);
  /// Sends the specified message to a window or windows
  /// <param name="hWnd"></param>
  /// <param name="Msg"></param>
  /// <param name="wParam"></param>
  /// <param name="lParam"></param>
  /// <returns></returns>
  [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = false)]
  static extern IntPtr SendMessage(IntPtr hWnd, Int32 Msg, IntPtr wParam, IntPtr lParam);
  //#endregion
  //#region Structs
  /// Contains information about a mouse event passed to a WH_MOUSE hook procedure
  [StructLayout(LayoutKind.Sequential)]
  struct MouseLLHookStruct {
    POINT pt;
    int mouseData;
    int flags;
    int time;
    int extraInfo;
  }
  /// Contains information about a low-level keyboard input event.
  [StructLayout(LayoutKind.Sequential)]
  struct KeyboardLLHookStruct {
    int vkCode;
    int scanCode;
    int flags;
    int time;
    int dwExtraInfo;
  }
  /// Contains information about a mouse event passed to a WH_MOUSE hook procedure
  [StructLayout(LayoutKind.Sequential)]
  struct MouseHookStruct {
    POINT pt;
    int hwnd;
    int wHitTestCode;
    int dwExtraInfo;
  }
  /// Represents a point
  struct POINT {
    int x;
    int y;
  }
  /// Defines the options for the DrawThemeTextEx function.
  [StructLayout(LayoutKind.Sequential)]
  struct DTTOPTS {
    uint dwSize;
    uint dwFlags;
    uint crText;
    uint crBorder;
    uint crShadow;
    int iTextShadowType;
    POINT ptShadowOffset;
    int iBorderSize;
    int iFontPropId;
    int iColorPropId;
    int iStateId;
    int fApplyOverlay;
    int iGlowSize;
    IntPtr pfnDrawTextCallback;
    int lParam;
  }
  /// This structure describes a color consisting of relative intensities of red, green, and blue.
  [StructLayout(LayoutKind.Sequential)]
  struct RGBQUAD {
    byte rgbBlue;
    byte rgbGreen;
    byte rgbRed;
    byte rgbReserved;
  }
  /// This structure contains information about the dimensions and color format of a device-independent bitmap (DIB).
  [StructLayout(LayoutKind.Sequential)]
  struct BITMAPINFOHEADER {
    int biSize;
    int biWidth;
    int biHeight;
    short biPlanes;
    short biBitCount;
    int biCompression;
    int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    int biClrUsed;
    int biClrImportant;
  }
  /// This structure defines the dimensions and color information of a Windows-based device-independent bitmap (DIB).
  [StructLayout(LayoutKind.Sequential)]
  struct BITMAPINFO {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors;
  }
  /// Describes the width, height, and location of a rectangle.
  [StructLayout(LayoutKind.Sequential)]
  struct RECT {
    int Left;
    int Top;
    int Right;
    int Bottom;
  }
  /// The NCCALCSIZE_PARAMS structure contains information that an application can use
  /// while processing the WM_NCCALCSIZE message to calculate the size, position, and
  /// valid contents of the client area of a window.
  [StructLayout(LayoutKind.Sequential)]
  struct NCCALCSIZE_PARAMS {
    RECT rect0, rect1, rect2;                    // Can't use an array here so simulate one
    IntPtr lppos;
  }
  /// Used to specify margins of a window
  [StructLayout(LayoutKind.Sequential)]
  struct MARGINS {
    int cxLeftWidth;
    int cxRightWidth;
    int cyTopHeight;
    int cyBottomHeight;
    MARGINS(int Left, int Right, int Top, int Bottom) {
      this.cxLeftWidth = Left;
      this.cxRightWidth = Right;
      this.cyTopHeight = Top;
      this.cyBottomHeight = Bottom;
    }
  }
  //#endregion
  //#region Properties
  /// Gets if the current OS is Windows NT or later
  static bool IsWindows {
    get { return Environment.OSVersion.Platform == PlatformID.Win32NT; }
  }
  /// Gets a value indicating if operating system is vista or higher
  static bool IsVista {
    get { return IsWindows && Environment.OSVersion.Version.Major >= 6; }
  }
  /// Gets a value indicating if operating system is xp or higher
  static bool IsXP {
    get { return IsWindows && Environment.OSVersion.Version.Major >= 5; }
  }
  /// Gets if computer is glass capable and enabled
  static bool IsGlassEnabled {
    get
    {
      //Check is windows vista
      if (IsVista) {
        //Check what DWM says about composition
        int enabled = 0;
        int response = DwmIsCompositionEnabled(ref enabled);
        return enabled > 0;
      }
      return false;
    }
  }
  //#endregion
  //#region Methods
  /// Equivalent to the HiWord C Macro
  /// <param name="dwValue"></param>
  /// <returns></returns>
  static int HiWord(int dwValue) {
    return (dwValue >> 16) & 0xFFFF;
  }
  /// Equivalent to the LoWord C Macro
  /// <param name="dwValue"></param>
  /// <returns></returns>
  static int LoWord(int dwValue) {
    return dwValue & 0xFFFF;
  }
  /// Equivalent to the MakeLParam C Macro
  /// <param name="LoWord"></param>
  /// <param name="HiWord"></param>
  /// <returns></returns>
  static IntPtr MakeLParam(int LoWord, int HiWord) {
    return new IntPtr((HiWord << 16) | (LoWord & 0xffff));
  }
  /// Fills an area for glass rendering
  /// <param name="gph"></param>
  /// <param name="rgn"></param>
  static void FillForGlass(Graphics g, IRECT r) {
    RECT rc = new RECT();
    rc.l = r.l;
    rc.r = r.r;
    rc.t = r.t;
    rc.b = r.b;
    IntPtr destdc = g.GetHdc();    //hwnd must be the handle of form,not control
    IntPtr Memdc = CreateCompatibleDC(destdc);
    IntPtr bitmap;
    IntPtr bitmapOld = IntPtr.Zero;
    BITMAPINFO dib = new BITMAPINFO();
    dib.bmiHeader.biHeight = -(rc.b - rc.t);
    dib.bmiHeader.biWidth = rc.r - rc.l;
    dib.bmiHeader.biPlanes = 1;
    dib.bmiHeader.biSize = Marshal.SizeOf(typeof(BITMAPINFOHEADER));
    dib.bmiHeader.biBitCount = 32;
    dib.bmiHeader.biCompression = BI_RGB;
    if (!(SaveDC(Memdc) == 0)) {
      bitmap = CreateDIBSection(Memdc, ref dib, DIB_RGB_COLORS, 0, IntPtr.Zero, 0);
      if (!(bitmap == IntPtr.Zero)) {
        bitmapOld = SelectObject(Memdc, bitmap);
        BitBlt(destdc, rc.l, rc.t, rc.r - rc.l, rc.b - rc.t, Memdc, 0, 0, SRCCOPY);
      }
      //Remember to clean up
      SelectObject(Memdc, bitmapOld);
      DeleteObject(bitmap);
      ReleaseDC(Memdc, -1);
      DeleteDC(Memdc);
    }
    g.ReleaseHdc();
  }
  /// Draws theme text on glass.
  /// <param name="hwnd"></param>
  /// <param name="text"></param>
  /// <param name="font"></param>
  /// <param name="ctlrct"></param>
  /// <param name="iglowSize"></param>
  /// <remarks>This method is courtesy of °æÈ¨ËùÓÐ (I hope the name's right)</remarks>
  static void DrawTextOnGlass(IntPtr hwnd, String text, Font font, IRECT ctlrct, int iglowSize) {
    if (IsGlassEnabled) {
      RECT rc = new RECT();
      RECT rc2 = new RECT();
      rc.l = ctlrct.l;
      rc.r = ctlrct.r;// +2 * iglowSize;  //make it larger to contain the glow effect
      rc.t = ctlrct.t;
      rc.b = ctlrct.b;// +2 * iglowSize;
      //Just the same rect with rc,but (0,0) at the lefttop
      rc2.l = 0;
      rc2.t = 0;
      rc2.r = rc.r - rc.l;
      rc2.b = rc.b - rc.t;
      IntPtr destdc = GetDC(hwnd);    //hwnd must be the handle of form,not control
      IntPtr Memdc = CreateCompatibleDC(destdc);   // Set up a memory DC where we'll draw the text.
      IntPtr bitmap;
      IntPtr bitmapOld = IntPtr.Zero;
      IntPtr logfnotOld;
      int uFormat = DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOPREFIX;   //text format
      BITMAPINFO dib = new BITMAPINFO();
      dib.bmiHeader.biHeight = -(rc.b - rc.t);         // negative because DrawThemeTextEx() uses a top-down DIB
      dib.bmiHeader.biWidth = rc.r - rc.l;
      dib.bmiHeader.biPlanes = 1;
      dib.bmiHeader.biSize = Marshal.SizeOf(typeof(BITMAPINFOHEADER));
      dib.bmiHeader.biBitCount = 32;
      dib.bmiHeader.biCompression = BI_RGB;
      if (!(SaveDC(Memdc) == 0)) {
        bitmap = CreateDIBSection(Memdc, ref dib, DIB_RGB_COLORS, 0, IntPtr.Zero, 0);   // Create a 32-bit bmp for use in offscreen drawing when glass is on
        if (!(bitmap == IntPtr.Zero)) {
          bitmapOld = SelectObject(Memdc, bitmap);
          IntPtr hFont = font.ToHfont();
          logfnotOld = SelectObject(Memdc, hFont);
          try {
            VisualStyleRenderer renderer = new VisualStyleRenderer(VisualStyleElement.Window.Caption.Active);
            DTTOPTS dttOpts = new DTTOPTS();
            dttOpts.dwSize = (uint)Marshal.SizeOf(typeof(DTTOPTS));
            dttOpts.dwFlags = DTT_COMPOSITED | DTT_GLOWSIZE;
            dttOpts.iGlowSize = iglowSize;
            int dtter = DrawThemeTextEx(renderer.Handle, Memdc, 0, 0, text, -1, uFormat, ref rc2, ref dttOpts);
            bool bbr = BitBlt(destdc, rc.l, rc.t, rc.r - rc.l, rc.b - rc.t, Memdc, 0, 0, SRCCOPY);
            if (!bbr) {
              //throw new Exception("???");
            }
          }
          catch (Exception e) {
            Console.WriteLine(e.ToString());
            //throw new Exception("???");
          }
          //Remember to clean up
          SelectObject(Memdc, bitmapOld);
          SelectObject(Memdc, logfnotOld);
          DeleteObject(bitmap);
          DeleteObject(hFont);
          ReleaseDC(Memdc, -1);
          DeleteDC(Memdc);
        }
        else {
          //throw new Exception("???");
        }
      }
      else {
        //throw new Exception("???");
      }
    }
  }
  //#endregion
}
}

