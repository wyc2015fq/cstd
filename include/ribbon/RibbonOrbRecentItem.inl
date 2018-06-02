using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
namespace System.Windows.Forms
{
struct RibbonOrbRecentItem
    : RibbonButton {
  //#region Ctor
  RibbonOrbRecentItem()
    : base() {
  }
  RibbonOrbRecentItem(string text)
    : this() {
    Text = text;
  }
  //#endregion
  //#region Methods
  IRECT OnGetImageBounds(RibbonElementSizeMode sMode, IRECT bounds) {
    return IRECT.Empty;
  }
  IRECT OnGetTextBounds(RibbonElementSizeMode sMode, IRECT bounds) {
    IRECT r = base.OnGetTextBounds(sMode, bounds);
    r.X = Bounds.l + 3;
    return r;
  }
  //#endregion
}
}

