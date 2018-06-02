using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.ComponentModel;
using System.Drawing.Drawing2D;
namespace System.Windows.Forms
{
/// A RibbonButton that incorporates a <see cref="Color"/> property and
/// draws this color below the displaying <see cref="img_t"/> or <see cref="SmallImage"/>
struct RibbonColorChooser
    : RibbonButton {
  //#region Fields
  COLOR _color;
  int _imageColorHeight;
  int _smallImageColorHeight;
  //#endregion
  //#region Events
  /// Raised when the <see cref="Color"/> property has been changed
  event EventHandler ColorChanged;
  //#endregion
  //#region Ctor
  RibbonColorChooser() {
    _color = Color.Transparent;
    _imageColorHeight = 8;
    _smallImageColorHeight = 4;
  }
  //#endregion
  //#region Properties
  /// Gets or sets the height of the color preview on the <see cref="img_t"/>
  [Description("Height of the color preview on the large image")]
  //[DefaultValue(8)]
  int ImageColorHeight {
    get { return _imageColorHeight; }
    set { _imageColorHeight = value; }
  }
  /// Gets or sets the height of the color preview on the <see cref="SmallImage"/>
  [Description("Height of the color preview on the small image")]
  //[DefaultValue(4)]
  int SmallImageColorHeight {
    get { return _smallImageColorHeight; }
    set { _smallImageColorHeight = value; }
  }
  /// Gets or sets the currently chosen color
  COLOR Color {
    get { return _color; }
    set { _color = value; RedrawItem(); OnColorChanged(EventArgs_Empty); }
  }
  //#endregion
  //#region Methods
  img_t CreateColorBmp(Color c) {
    Bitmap b = new Bitmap(16, 16);
    using(Graphics g = Graphics.FromImage(b)) {
      using(SolidBrush br = new SolidBrush(c)) {
        g.FillRectangle(br, iRECT(0, 0, 15, 15));
      }
      g.DrawRectangle(Pens.DimGray, iRECT(0, 0, 15, 15));
    }
    return b;
  }
  /// Raises the <see cref="ColorChanged"/>
  /// <param name="e"></param>
  void OnColorChanged(EventArgs e) {
    if (ColorChanged != NULL) {
      ColorChanged(this, e);
    }
  }
  //#endregion
  //#region Overrides
  void OnPaint(object sender, RibbonElementPaintEventArgs e) {
    base.OnPaint(sender, e);
    COLOR c = this.Color.Equals(Color.Transparent) ? Color.White : Color;
    int h = e.Mode == RibbonElementSizeMode_Large ? ImageColorHeight : SmallImageColorHeight;
    IRECT colorFill = FromLTRB(
        ImageBounds.l,
        ImageBounds.b - h,
        ImageBounds.r,
        ImageBounds.b
        );
    SmoothingMode sm = e.Graphics.SmoothingMode;
    e.Graphics.SmoothingMode = SmoothingMode.None;
    using(SolidBrush b = new SolidBrush(c)) {
      e.Graphics.FillRectangle(b, colorFill);
    }
    if (this.Color.Equals(Color.Transparent)) {
      e.Graphics.DrawRectangle(Pens.DimGray, colorFill);
    }
    e.Graphics.SmoothingMode = sm;
  }
  //#endregion
}
}

