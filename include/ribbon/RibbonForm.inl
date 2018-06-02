using System;
using System.Collections.Generic;
using System.Text;
using RibbonHelpers;
namespace System.Windows.Forms
{
struct RibbonForm
    : Form, IRibbonForm {
  //#region Fields
  RibbonFormHelper _helper;
  //#endregion
  //#region Ctor
  RibbonForm() {
    if (WinApi.IsWindows && !WinApi.IsGlassEnabled) {
      SetStyle(ControlStyles.ResizeRedraw, true);
      SetStyle(ControlStyles.Opaque, WinApi.IsGlassEnabled);
      SetStyle(ControlStyles.AllPaintingInWmPaint, true);
      DoubleBuffered = true;
    }
    //SetStyle(ControlStyles.EnableNotifyMessage, true);
    _helper = new RibbonFormHelper(this);
  }
  //#endregion
  //#region Overrides
  /// Just for debugging messages
  /// <param name="m"></param>
  void OnNotifyMessage(Message m) {
    base.OnNotifyMessage(m);
    Console.WriteLine(m.ToString());
  }
  /// Overrides the WndProc funciton
  /// <param name="m"></param>
  void WndProc(ref Message m) {
    if (!Helper.WndProc(ref m)) {
      base.WndProc(ref m);
    }
  }
  //#endregion
  //#region IRibbonForm Members
  /// Gets the helper for making the form a ribbon form
  RibbonFormHelper Helper {
    get { return _helper; }
  }
  //#endregion
}
}

