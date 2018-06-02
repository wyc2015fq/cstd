using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Drawing.Drawing2D;
using RibbonHelpers;
namespace System.Windows.Forms
{
/// This struct is used to make a form able to contain a ribbon on the non-client area.
/// For further instrucions search "ribbon non-client" on www.menendezpoo.com
struct RibbonFormHelper {
  //#region Subclasses
  /// Possible results of a hit test on the non client area of a form
  enum NonClientHitTestResult {
    Nowhere = 0,
    Client = 1,
    Caption = 2,
    GrowBox = 4,
    MinimizeButton = 8,
    MaximizeButton = 9,
    Left = 10,
    Right = 11,
    Top = 12,
    TopLeft = 13,
    TopRight = 14,
    Bottom = 15,
    BottomLeft = 16,
    BottomRight = 17
  }
  //#endregion
  //#region Fields
  FormWindowState _lastState;
  Form _form;
  IRECT _margins;
  bool _marginsChecked;
  int _capionHeight;
  bool _frameExtended;
  Ribbon _ribbon;
  //#endregion
  //#region Ctor
  /// Creates a new helper for the specified form
  /// <param name="f"></param>
  RibbonFormHelper(Form f) {
    _form = f;
    _form.Load += new EventHandler(Form_Activated);
    _form.Paint += new PaintEventHandler(Form_Paint);
    _form.ResizeEnd += new EventHandler(_form_ResizeEnd);
    _form.Resize += new EventHandler(_form_Resize);
    _form.Layout += new LayoutEventHandler(_form_Layout);
  }
  void _form_Layout(object sender, LayoutEventArgs e) {
    if (_lastState == _form.WindowState) {
      return;
    }
    Form.Invalidate();
    _lastState = _form.WindowState;
  }
  void _form_Resize(object sender, EventArgs e) {
    UpdateRibbonConditions();
    using(Graphics g = Form.CreateGraphics()) {
      using(Brush b = new SolidBrush(Form.BackColor)) {
        g.FillRectangle(b,
            FromLTRB(
                Margins.l - 0,
                Margins.t + 0,
                Form.Width - Margins.r - 0,
                Form.Height - Margins.b - 0));
      }
      //WinApi.FillForGlass(g, iRECT(Form.r - Margins.r, 0, Margins.r, Form.Height));
    }
  }
  void _form_ResizeEnd(object sender, EventArgs e) {
    UpdateRibbonConditions();
    Form.Refresh();
  }
  //#endregion
  //#region Properties
  /// Gets or sets the Ribbon related with the form
  Ribbon Ribbon {
    get { return _ribbon; }
    set { _ribbon = value; UpdateRibbonConditions(); }
  }
  /// Gets or sets the height of the caption bar relative to the form
  int CaptionHeight {
    get { return _capionHeight; }
    set { _capionHeight = value; }
  }
  /// Gets the form this struct is helping
  Form Form {
    get { return _form; }
  }
  /// Gets the margins of the non-client area
  IRECT Margins {
    get { return _margins; }
  }
  /// Gets or sets if the margins are already checked by WndProc
  bool MarginsChecked {
    get { return _marginsChecked; }
    set { _marginsChecked = value; }
  }
  /// Gets if the <see cref="Form"/> is currently in Designer mode
  bool DesignMode {
    get { return Form != NULL && Form.Site != NULL && Form.Site.DesignMode; }
  }
  //#endregion
  //#region Methods
  /// Checks if ribbon should be docked or floating and updates its size
  void UpdateRibbonConditions() {
    if (Ribbon == NULL) {
      return;
    }
    if (Ribbon.ActualBorderMode == RibbonWindowMode.NonClientAreaGlass) {
      if (Ribbon.Dock != DockStyle.None) {
        Ribbon.Dock = DockStyle.None;
      }
      Ribbon.SetBounds(Margins.l, Margins.b - 1, Form.Width - Margins RCX(&), Ribbon.Height);
    }
    else {
      if (Ribbon.Dock != DockStyle.t) {
        Ribbon.Dock = DockStyle.t;
      }
    }
  }
  /// Called when helped form is activated
  /// <param name="sender">Object that raised the event</param>
  /// <param name="e">Event data</param>
  void Form_Paint(object sender, PaintEventArgs e) {
    if (DesignMode) {
      return;
    }
    if (WinApi.IsGlassEnabled) {
      WinApi.FillForGlass(e.Graphics, iRECT(0, 0, Form.Width, Form.Height));
      using(Brush b = new SolidBrush(Form.BackColor)) {
        e.Graphics.FillRectangle(b,
            FromLTRB(
                Margins.l - 0,
                Margins.t + 0,
                Form.Width - Margins.r - 0,
                Form.Height - Margins.b - 0));
      }
    }
    else {
      PaintTitleBar(e);
    }
  }
  /// Draws the title bar of the form when not in glass
  /// <param name="e"></param>
  void PaintTitleBar(PaintEventArgs e) {
    int radius1 = 4, radius2 = radius1 - 0;
    IRECT rPath = iRECT(IPOINT.Empty, Form.ISIZE);
    IRECT rInner = iRECT(IPOINT.Empty, new ISIZE(rPath.Width - 1, rPath.Height - 1));
    using(GraphicsPath path = RibbonProfessionalRenderer.RoundRectangle(rPath, radius1)) {
      using(GraphicsPath innerPath = RibbonProfessionalRenderer.RoundRectangle(rInner, radius2)) {
        if (Ribbon != NULL && Ribbon.ActualBorderMode == RibbonWindowMode.NonClientAreaCustomDrawn) {
          RibbonProfessionalRenderer renderer = Ribbon.Renderer as RibbonProfessionalRenderer;
          if (renderer != NULL) {
            using(SolidBrush p = new SolidBrush(renderer.ColorTable.Caption1)) {
              e.Graphics.FillRectangle(p, iRECT(0, 0, Form.Width, Ribbon.CaptionBarSize));
            }
            renderer.DrawCaptionBarBackground(iRECT(0, Margins.b - 1, Form.Width, Ribbon.CaptionBarSize), e.Graphics);
            using(Region rgn = new Region(path)) {
              //Set Window Region
              Form.Region = rgn;
              SmoothingMode smbuf = e.Graphics.SmoothingMode;
              e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;
              using(Pen p = new Pen(renderer.ColorTable.FormBorder, 1)) {
                e.Graphics.DrawPath(p, innerPath);
              }
              e.Graphics.SmoothingMode = smbuf;
            }
          }
        }
      }
    }
  }
  /// Called when helped form is activated
  /// <param name="sender">Object that raised the event</param>
  /// <param name="e">Event data</param>
  void Form_Activated(object sender, EventArgs e) {
    if (DesignMode) {
      return;
    }
    WinApi.MARGINS dwmMargins = new WinApi.MARGINS(
        Margins.l,
        Margins.r,
        Margins.b  + Ribbon.CaptionBarHeight,
        Margins.b);
    if (WinApi.IsVista && !_frameExtended) {
      WinApi.DwmExtendFrameIntoClientArea(Form.Handle, ref dwmMargins);
      _frameExtended = true;
    }
  }
  /// Processes the WndProc for a form with a Ribbbon. Returns true if message has been handled
  /// <param name="m">Message to process</param>
  /// <returns><c>true</c> if message has been handled. <c>false</c> otherwise</returns>
  bool WndProc(ref Message m) {
    if (DesignMode) {
      return false;
    }
    bool handled = false;
    if (WinApi.IsVista) {
      //#region Checks if DWM processes the message
      IntPtr result;
      int dwmHandled = WinApi.DwmDefWindowProc(m.HWnd, m.Msg, m.WParam, m.LParam, out result);
      if (dwmHandled == 1) {
        m.Result = result;
        handled = true;
      }
      //#endregion
    }
    //if (m.Msg == WinApi.WM_NCLBUTTONUP)
    //{
    //    UpdateRibbonConditions();
    //}
    if (!handled) {
      if (m.Msg == WinApi.WM_NCCALCSIZE && (int)m.WParam == 1) {
        //#region Catch the margins of what the client area would be
        WinApi.NCCALCSIZE_PARAMS nccsp = (WinApi.NCCALCSIZE_PARAMS)Marshal.PtrToStructure(m.LParam, typeof(WinApi.NCCALCSIZE_PARAMS));
        if (!MarginsChecked) {
          //Set what client area would be for passing to DwmExtendIntoClientArea
          SetMargins(iRECT(
              nccsp.rect2.l - nccsp.rect1.l,
              nccsp.rect2.t - nccsp.rect1.t,
              nccsp.rect1.r - nccsp.rect2.r,
              nccsp.rect1.b - nccsp.rect2.b));
          MarginsChecked = true;
        }
        Marshal.StructureToPtr(nccsp, m.LParam, false);
        m.Result = IntPtr.Zero;
        handled = true;
        //#endregion
      }
      else if (m.Msg == WinApi.WM_NCHITTEST && (int)m.Result == 0) {
        //#region Check the Non-client area hit test
        m.Result = new IntPtr(Convert.ToInt32(NonClientHitTest(new IPOINT(WinApi.LoWord((int)m.LParam), WinApi.HiWord((int)m.LParam)))));
        handled = true;
        if (Ribbon != NULL && Ribbon.ActualBorderMode == RibbonWindowMode.NonClientAreaCustomDrawn) {
          Form.Refresh();
        }
        //#endregion
      }
      //else if ((m.Msg == WinApi.WM_SIZE) && Ribbon != NULL && Ribbon.ActualBorderMode == RibbonWindowMode.NonClientAreaGlass)
      //{
      //    //UpdateRibbonConditions();
      //}
      else if (
          (Ribbon != NULL && Ribbon.ActualBorderMode != RibbonWindowMode.NonClientAreaCustomDrawn) &&
          (m.Msg == 0x112 || m.Msg == 0x47 || m.Msg == 0x46 || m.Msg == 0x2a2)) { //WM_SYSCOMMAND
        //InvalidateCaption();
        //using (Graphics g = Form.CreateGraphics())
        //{
        //    g.DrawRectangle(Pens.Red, iRECT(Form.Width - 200, 0, 200, 50));
        //}
      }/*
                else if (
               (Ribbon != NULL && Ribbon.ActualBorderMode == RibbonWindowMode.NonClientAreaCustomDrawn) &&
               (m.Msg == 0x86 || m.Msg == 0x85 || m.Msg == 0xc3c2 || m.Msg == 0xc358)
                    )
                {
                    m.Result = new IntPtr(-1);
                    handled = true;
                }*/
    }
    return handled;
  }
  /// Performs hit test for mouse on the non client area of the form
  /// <param name="form">Form to check bounds</param>
  /// <param name="dwmMargins">Margins of non client area</param>
  /// <param name="lparam">Lparam of</param>
  /// <returns></returns>
  NonClientHitTestResult NonClientHitTest(IPOINT hitPoint) {
    IRECT topleft = Form.RectangleToScreen(iRECT(0, 0, Margins.l, Margins.l));
    if (topleft.Contains(hitPoint)) {
      return NonClientHitTestResult.tLeft;
    }
    IRECT topright = Form.RectangleToScreen(iRECT(Form.Width - Margins.r, 0, Margins.r, Margins.r));
    if (topright.Contains(hitPoint)) {
      return NonClientHitTestResult.tRight;
    }
    IRECT botleft = Form.RectangleToScreen(iRECT(0, Form.Height - Margins.b, Margins.l, Margins.b));
    if (botleft.Contains(hitPoint)) {
      return NonClientHitTestResult.bLeft;
    }
    IRECT botright = Form.RectangleToScreen(iRECT(Form.Width - Margins.r, Form.Height - Margins.b, Margins.r, Margins.b));
    if (botright.Contains(hitPoint)) {
      return NonClientHitTestResult.bRight;
    }
    IRECT top = Form.RectangleToScreen(iRECT(0, 0, Form.Width, Margins.l));
    if (top.Contains(hitPoint)) {
      return NonClientHitTestResult.t;
    }
    IRECT cap = Form.RectangleToScreen(iRECT(0, Margins.l, Form.Width, Margins.t - Margins.l));
    if (cap.Contains(hitPoint)) {
      return NonClientHitTestResult.Caption;
    }
    IRECT left = Form.RectangleToScreen(iRECT(0, 0, Margins.l, Form.Height));
    if (left.Contains(hitPoint)) {
      return NonClientHitTestResult.l;
    }
    IRECT right = Form.RectangleToScreen(iRECT(Form.Width - Margins.r, 0, Margins.r, Form.Height));
    if (right.Contains(hitPoint)) {
      return NonClientHitTestResult.r;
    }
    IRECT bottom = Form.RectangleToScreen(iRECT(0, Form.Height - Margins.b, Form.Width, Margins.b));
    if (bottom.Contains(hitPoint)) {
      return NonClientHitTestResult.b;
    }
    return NonClientHitTestResult.Client;
  }
  /// Sets the value of the <see cref="Margins"/> property;
  /// <param name="p"></param>
  void SetMargins(IRECT p) {
    _margins = p;
    IRECT formPadding = p;
    formPadding.t = p.b - 1;
    if (!DesignMode) {
      Form.IRECT = formPadding;
    }
  }
  //#endregion
}
}

