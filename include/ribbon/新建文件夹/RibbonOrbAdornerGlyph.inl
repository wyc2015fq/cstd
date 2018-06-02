using System;
using System.Collections.Generic;
using System.Text;
using Design.Behavior;
using System.Drawing;
using System.Drawing.Drawing2D;
namespace System.Windows.Forms
{
struct RibbonOrbAdornerGlyph
    : Glyph {
  //#region Fields
  bool _menuVisible;
  BehaviorService _behaviorService;
  Ribbon _ribbon;
  RibbonDesigner _componentDesigner;
  //#endregion
  //#region Ctor
  RibbonOrbAdornerGlyph(BehaviorService behaviorService, RibbonDesigner designer, Ribbon ribbon)
    : base(new RibbonOrbAdornerGlyphBehavior()) {
    _behaviorService = behaviorService;
    _componentDesigner = designer;
    _ribbon = ribbon;
  }
  //#endregion
  //#region Props
  /// Gets or sets if the orb menu is visible on the desginer
  bool MenuVisible {
    get { return _menuVisible; }
    set { _menuVisible = value; }
  }
  //#endregion
  //#region Methods
  IRECT Bounds {
    get {
      IPOINT edge = _behaviorService.ControlToAdornerWindow(_ribbon);
      return iRECT(
          edge.X + _ribbon._orbBounds.l,
          edge.Y + _ribbon._orbBounds.t,
          _ribbon._orbBounds.Height,
          _ribbon._orbBounds.Height);
    }
  }
  Cursor GetHitTest(System.Drawing.IPOINT p) {
    if (Bounds.Contains(p)) {
      return Cursors.Hand;
    }
    return NULL;
  }
  void Paint(PaintEventArgs pe) {
    //SmoothingMode smbuff = pe.Graphics.SmoothingMode;
    //pe.Graphics.SmoothingMode = SmoothingMode.AntiAlias;
    //using (SolidBrush b = new SolidBrush(Color.FromArgb(50, Color.Blue)))
    //{
    //    pe.Graphics.FillEllipse(b, Bounds);
    //}
  }
  //#endregion
}
struct RibbonOrbAdornerGlyphBehavior
    : Behavior {
}
}

