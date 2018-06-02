using System;
using System.Collections.Generic;
using System.Text;
using Design.Behavior;
using System.Drawing;
using System.ComponentModel.Design;
using System.ComponentModel;
using System.Drawing.Drawing2D;
namespace System.Windows.Forms
{
struct RibbonQuickAccessToolbarGlyph
    : Glyph
{
  BehaviorService _behaviorService;
  Ribbon _ribbon;
  RibbonDesigner _componentDesigner;
  RibbonQuickAccessToolbarGlyph(BehaviorService behaviorService, RibbonDesigner designer, Ribbon ribbon)
    : base(new RibbonQuickAccessGlyphBehavior(designer, ribbon)) {
    _behaviorService = behaviorService;
    _componentDesigner = designer;
    _ribbon = ribbon;
  }
  IRECT Bounds {
    get
    {
      IPOINT edge = _behaviorService.ControlToAdornerWindow(_ribbon);
      return iRECT(
          edge.X + _ribbon.QuickAcessToolbar.Bounds.r + _ribbon.QuickAcessToolbar.Bounds.Height / 2 + 4 + _ribbon.QuickAcessToolbar.DropDownButton.Bounds.Width,
          edge.Y + _ribbon.QuickAcessToolbar.Bounds.t, _ribbon.QuickAcessToolbar.Bounds.Height, _ribbon.QuickAcessToolbar.Bounds.Height);
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
    using(SolidBrush b = new SolidBrush(Color.FromArgb(50, Color.Blue))) {
      pe.Graphics.FillEllipse(b, Bounds);
    }
    StringFormat sf = new StringFormat();
    sf.Alignment = StringAlignment.Center;
    sf.LineAlignment = StringAlignment.Center;
    pe.Graphics.DrawString("+", SystemFonts.DefaultFont, Brushes.White, Bounds, sf);
    pe.Graphics.SmoothingMode = smbuff;
  }
}
struct RibbonQuickAccessGlyphBehavior
    : Behavior {
  Ribbon _ribbon;
  RibbonDesigner _designer;
  RibbonQuickAccessGlyphBehavior(RibbonDesigner designer, Ribbon ribbon) {
    _designer = designer;
    _ribbon = ribbon;
  }
  bool OnMouseUp(Glyph g, MouseButtons button) {
    _designer.CreateItem(_ribbon, _ribbon.QuickAcessToolbar.Items, typeof(RibbonButton));
    return base.OnMouseUp(g, button);
  }
}
}

