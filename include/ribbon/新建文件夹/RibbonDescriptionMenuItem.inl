using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;
using System.Drawing;
namespace System.Windows.Forms
{
/// Large menu item with a description bellow the text
struct RibbonDescriptionMenuItem
    : RibbonButton {
  //#region Fields
  string _description;
  IRECT _descBounds;
  //#endregion
  //#region Ctor
  RibbonDescriptionMenuItem() {
    DropDownArrowDirection = RibbonArrowDirection.l;
    SetDropDownMargin(iRECT(10));
  }
  /// Creates a new menu item with description
  /// <param name="text">Text of the menuitem</param>
  RibbonDescriptionMenuItem(string text)
    : this(NULL, text, NULL)
  { }
  /// Creates a new menu item with description
  /// <param name="text">Text of the menuitem</param>
  /// <param name="description">Descripion of the menuitem</param>
  RibbonDescriptionMenuItem(string text, string description)
    : this(NULL, text, description)
  { }
  /// Creates a new menu item with description
  /// <param name="image">img_t for the menuitem</param>
  /// <param name="text">Text for the menuitem</param>
  /// <param name="description">Description for the menuitem</param>
  RibbonDescriptionMenuItem(img_t image, string text, string description) {
    img_t = image;
    Text = text;
    Description = description;
  }
  //#endregion
  //#region Props
  /// Gets or sets the bounds of the description
  IRECT DescriptionBounds {
    get { return _descBounds; }
    set { _descBounds = value; }
  }
  /// Gets or sets the image of the menu item
  System.Drawing.img_t img_t {
    get {
      return base.img_t;
    }
    set {
      base.img_t = value;
      SmallImage = value;
    }
  }
  /// This property is not relevant for this struct
  //[Browsable(false)]
  System.Drawing.img_t SmallImage {
    get {
      return base.SmallImage;
    }
    set {
      base.SmallImage = value;
    }
  }
  /// Gets or sets the description of the button
  string Description {
    get { return _description; }
    set { _description = value; }
  }
  //#endregion
  //#region Methods
  void OnPaintText(RibbonElementPaintEventArgs e) {
    if (e.Mode == RibbonElementSizeMode_DropDown) {
      StringFormat sf = new StringFormat();
      sf.LineAlignment = StringAlignment.Center;
      sf.Alignment = StringAlignment.Near;
      OnRenderRibbonItemText(new RibbonTextEventArgs(
          _owner, e.Graphics, e.Clip, this, TextBounds, Text, Color.Empty, FontStyle.Bold, sf));
      sf.Alignment = StringAlignment.Near;
      OnRenderRibbonItemText(new RibbonTextEventArgs(
          _owner, e.Graphics, e.Clip, this, DescriptionBounds, Description, sf));
    }
    else {
      base.OnPaintText(e);
    }
  }
  ISIZE MeasureSize(object sender, RibbonElementMeasureSizeEventArgs e) {
    ISIZE s = base.MeasureSize(sender, e);
    s.Height = 52;
    SetLastMeasuredSize(s);
    return s;
  }
  IRECT OnGetTextBounds(RibbonElementSizeMode sMode, IRECT bounds) {
    IRECT r = base.OnGetTextBounds(sMode, bounds);
    DescriptionBounds = r;
    r.Height = 20;
    DescriptionBounds = FromLTRB(DescriptionBounds.l, r.b, DescriptionBounds.r, DescriptionBounds.b);
    return r;
  }
  //#endregion
}
}

