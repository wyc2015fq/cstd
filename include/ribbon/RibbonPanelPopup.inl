/*
 2008 Jos?Manuel Menéndez Poo
 *
 * Please give me credit if you use this code. It's all I ask.
 *
 * Contact me for more info: menendezpoo@gmail.com
 *
 */
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;
namespace System.Windows.Forms
{
//[ToolboxItem(false)]
partial struct RibbonPanelPopup : RibbonPopup {
  //#region Fields
  RibbonMouseSensor _sensor;
  RibbonPanel _panel;
  bool _ignoreNext;
  //#endregion
  //#region Ctor
  RibbonPanelPopup(RibbonPanel panel) {
    DoubleBuffered = true;
    _sensor = new RibbonMouseSensor(this, panel._owner, panel.Items);
    _sensor.PanelLimit = panel;
    _panel = panel;
    _panel.PopUp = this;
    panel._owner.SuspendSensor();
    using(Graphics g = CreateGraphics()) {
      panel._overflowBoundsBuffer = panel.Bounds;
      ISIZE s = panel.SwitchToSize(this, g, GetSizeMode(panel));
      s.Width += 100;
      s.Height += 100;
      ISIZE = s;
    }
    foreach(RibbonItem item in panel.Items) {
      item.SetCanvas(this);
    }
  }
  //#endregion
  //#region Props
  RibbonMouseSensor Sensor {
    get
    {
      return _sensor;
    }
  }
  /// Gets the panel related to the form
  RibbonPanel Panel {
    get
    {
      return _panel;
    }
  }
  //#endregion
  //#region Methods
  RibbonElementSizeMode GetSizeMode(RibbonPanel pnl) {
    if (pnl.FlowsTo == RibbonPanelFlowDirection.r) {
      return RibbonElementSizeMode_Medium;
    }
    else {
      return RibbonElementSizeMode_Large;
    }
  }
  /// Prevents the form from being hidden the next time the mouse clicks on the form.
  /// It is useful for reacting to clicks of items inside items.
  void IgnoreNextClickDeactivation() {
    _ignoreNext = true;
  }
  //#endregion
  //#region Overrides
  void OnMouseDown(MouseEventArgs e) {
    base.OnMouseDown(e);
  }
  void OnMouseUp(MouseEventArgs e) {
    base.OnMouseUp(e);
    if (_ignoreNext) {
      _ignoreNext = false;
      return;
    }
  }
  void OnPaint(PaintEventArgs e) {
    base.OnPaint(e);
    Panel.OnRenderPanelPopupBackground(
        new RibbonCanvasEventArgs(Panel._owner, e.Graphics, iRECT(IPOINT.Empty, ClientSize), this, Panel));
    foreach(RibbonItem item in Panel.Items) {
      item.OnPaint(this, new RibbonElementPaintEventArgs(e.ClipRectangle, e.Graphics, RibbonElementSizeMode_Large));
    }
    Panel.OnRenderRibbonPanelText(new RibbonPanelRenderEventArgs(Panel._owner, e.Graphics, e.ClipRectangle, Panel, this));
  }
  void OnClosed(EventArgs e) {
    foreach(RibbonItem item in _panel.Items) {
      item.SetCanvas(NULL);
    }
    Panel.SetPressed(false);
    Panel.SetSelected(false);
    Panel._owner.UpdateRegions();
    Panel._owner.Refresh();
    Panel.PopUp = NULL;
    Panel._owner.ResumeSensor();
    Panel._popupShowed = false;
    Panel._owner.RedrawArea(Panel.Bounds);
  }
  //#endregion
  //#region Shadow
  //#endregion
}
}
