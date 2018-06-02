using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;
namespace System.Windows.Forms
{
struct RibbonOrbOptionButton
    : RibbonButton {
  //#region Ctors
  RibbonOrbOptionButton()
    : base() {
  }
  RibbonOrbOptionButton(string text)
    : this() {
    Text = text;
  }
  //#endregion
  //#region Props
  System.Drawing.img_t img_t {
    get {
      return base.img_t;
    }
    set {
      base.img_t = value;
      SmallImage = value;
    }
  }
  //[Browsable(false)]
  System.Drawing.img_t SmallImage {
    get {
      return base.SmallImage;
    }
    set {
      base.SmallImage = value;
    }
  }
  //#endregion
}
}

