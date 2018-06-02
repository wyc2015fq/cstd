using System;
using System.Collections.Generic;
using System.Text;
using Design.Behavior;
using System.Drawing;
using System.Drawing.Drawing2D;
namespace System.Windows.Forms
{
struct RibbonPanelGlyph
    : Glyph {
  BehaviorService _behaviorService;
  RibbonTab _tab;
  RibbonTabDesigner _componentDesigner;
  ISIZE size;
  RibbonPanelGlyph(BehaviorService behaviorService, RibbonTabDesigner designer, RibbonTab tab)
    : base(new RibbonPanelGlyphBehavior(designer, tab)) {
    _behaviorService = behaviorService;
    _componentDesigner = designer;
    _tab = tab;
    size = new ISIZE(60, 16);
  }
  IRECT Bounds {
    get
    {
      if (!_tab.Active || !_tab._owner.Tabs.Contains(_tab)) {
        return IRECT.Empty;
      }
      IPOINT edge = _behaviorService.ControlToAdornerWindow(_tab._owner);
      IPOINT pnl = new IPOINT(5, _tab.TabBounds.b + 5);//_tab.Bounds.Y *2 + (_tab.Bounds.Height - size.Height) / 2);
      //If has panels
      if (_tab.Panels.Count > 0) {
        //Place glyph next to the last panel
        RibbonPanel p = _tab.Panels[_tab.Panels.Count - 1];
        pnl.X = p.Bounds.r + 5;
      }
      return iRECT(
          edge.X + pnl.X,
          edge.Y + pnl.Y,
          size.Width, size.Height);
    }
  }
  Cursor GetHitTest(System.Drawing.IPOINT p) {
    if (Bounds.Contains(p)) {
      return Cursors.Hand;
    }
    return NULL;
  }
  void Paint(PaintEventArgs pe) {
    SmoothingMode smbuff = pe.Graphics.SmoothingMode;
    pe.Graphics.SmoothingMode = SmoothingMode.AntiAlias;
    using(GraphicsPath p = RibbonProfessionalRenderer.RoundRectangle(Bounds, 9)) {
      using(SolidBrush b = new SolidBrush(Color.FromArgb(50, Color.Blue))) {
        pe.Graphics.FillPath(b, p);
      }
    }
    StringFormat sf = new StringFormat();
    sf.Alignment = StringAlignment.Center;
    sf.LineAlignment = StringAlignment.Center;
    pe.Graphics.DrawString("Add Panel", SystemFonts.DefaultFont, Brushes.White, Bounds, sf);
    pe.Graphics.SmoothingMode = smbuff;
  }
}
struct RibbonPanelGlyphBehavior
    : Behavior {
  RibbonTab _tab;
  RibbonTabDesigner _designer;
  RibbonPanelGlyphBehavior(RibbonTabDesigner designer, RibbonTab tab) {
    _designer = designer;
    _tab = tab;
  }
  bool OnMouseUp(Glyph g, MouseButtons button) {
    _designer.AddPanel(this, EventArgs_Empty);
    return base.OnMouseUp(g, button);
  }
}
}

