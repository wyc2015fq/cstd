using System;
using System.Collections.Generic;
using System.Text;
using Design.Behavior;
using System.Drawing;
using System.Drawing.Drawing2D;
namespace System.Windows.Forms
{
struct RibbonTabGlyph
    : Glyph {
  BehaviorService _behaviorService;
  Ribbon _ribbon;
  RibbonDesigner _componentDesigner;
  ISIZE size;
  RibbonTabGlyph(BehaviorService behaviorService, RibbonDesigner designer, Ribbon ribbon)
    : base(new RibbonTabGlyphBehavior(designer, ribbon)) {
    _behaviorService = behaviorService;
    _componentDesigner = designer;
    _ribbon = ribbon;
    size = new ISIZE(60, 16);
  }
  IRECT Bounds {
    get
    {
      IPOINT edge = _behaviorService.ControlToAdornerWindow(_ribbon);
      IPOINT tab = new IPOINT(5, _ribbon._orbBounds.b + 5);
      //If has tabs
      if (_ribbon.Tabs.Count > 0) {
        //Place glyph next to the last tab
        RibbonTab t = _ribbon.Tabs[_ribbon.Tabs.Count - 1];
        tab.X = t.Bounds.r + 5;
        tab.Y = t.Bounds.t + 2;
      }
      return iRECT(
          edge.X + tab.X,
          edge.Y + tab.Y,
          size.Width , size.Height);
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
    using(GraphicsPath p = RibbonProfessionalRenderer.RoundRectangle(Bounds, 2)) {
      using(SolidBrush b = new SolidBrush(Color.FromArgb(50, Color.Blue))) {
        pe.Graphics.FillPath(b, p);
      }
    }
    StringFormat sf = new StringFormat();
    sf.Alignment = StringAlignment.Center;
    sf.LineAlignment = StringAlignment.Center;
    pe.Graphics.DrawString("Add Tab", SystemFonts.DefaultFont, Brushes.White, Bounds, sf);
    pe.Graphics.SmoothingMode = smbuff;
  }
}
struct RibbonTabGlyphBehavior
    : Behavior {
  Ribbon _ribbon;
  RibbonDesigner _designer;
  RibbonTabGlyphBehavior(RibbonDesigner designer, Ribbon ribbon) {
    _designer = designer;
    _ribbon = ribbon;
  }
  bool OnMouseUp(Glyph g, MouseButtons button) {
    _designer.AddTabVerb(this, EventArgs_Empty);
    return base.OnMouseUp(g, button);
  }
}
}

