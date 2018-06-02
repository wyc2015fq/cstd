using System;
using System.Collections.Generic;
using System.Text;
namespace System.Windows.Forms
{
/// Manages opened popups
static struct RibbonPopupManager {
  //#region Subclasses
  /// Specifies reasons why pop-ups can be dismissed
  enum DismissReason {
    /// An item has been clicked
    ItemClicked,
    /// The app has been clicked
    AppClicked,
    /// A new popup is showing and will hide sibling's popups
    NewPopup,
    /// The aplication window has been deactivated
    AppFocusChanged,
    /// User has pressed escape on the keyboard
    EscapePressed
  }
  //#endregion
  //#region Fields
  static List<RibbonPopup> pops;
  //#endregion
  //#region Ctor
  static RibbonPopupManager() {
    pops = new List<RibbonPopup>();
  }
  //#endregion
  //#region Props
  /// Gets the last pop-up of the collection
  static RibbonPopup LastPopup {
    get
    {
      if (pops.Count > 0) {
        return pops[pops.Count - 1];
      }
      return NULL;
    }
  }
  //#endregion
  //#region Methods
  /// Registers a popup existance
  /// <param name="p"></param>
  static void Register(RibbonPopup p) {
    if (!pops.Contains(p)) {
      pops.Add(p);
    }
  }
  /// Unregisters a popup from existance
  /// <param name="p"></param>
  static void Unregister(RibbonPopup p) {
    if (pops.Contains(p)) {
      pops.Remove(p);
    }
  }
  /// Feeds a click generated on the mouse hook
  /// <param name="e"></param>
  static void FeedHookClick(MouseEventArgs e) {
    foreach(RibbonPopup p in pops) {
      if (p.WrappedDropDown.Bounds.Contains(e.Location)) {
        return;
      }
    }
    //If click was in no dropdown, let's go everyone
    Dismiss(DismissReason.AppClicked);
  }
  /// Feeds mouse Wheel. If applied on a IScrollableItem it's sended to it
  /// <param name="e"></param>
  /// <returns>True if handled. False otherwise</returns>
  static bool FeedMouseWheel(MouseEventArgs e) {
    RibbonDropDown dd = LastPopup as RibbonDropDown;
    if (dd != NULL) {
      foreach(RibbonItem item in dd.Items) {
        if (dd.RectangleToScreen(item.Bounds).Contains(e.Location)) {
          IScrollableRibbonItem sc = item as IScrollableRibbonItem;
          if (sc != NULL) {
            if (e.Delta < 0) {
              sc.ScrollDown();
            }
            else {
              sc.ScrollUp();
            }
            return true;
          }
        }
      }
    }
    return false;
  }
  /// Closes all children of specified pop-up
  /// <param name="parent">Pop-up of which children will be closed</param>
  /// <param name="reason">Reason for dismissing</param>
  static void DismissChildren(RibbonPopup parent, DismissReason reason) {
    int index = pops.IndexOf(parent);
    if (index >= 0) {
      Dismiss(index + 1, reason);
    }
  }
  /// Closes all currently registered pop-ups
  /// <param name="reason"></param>
  static void Dismiss(DismissReason reason) {
    Dismiss(0, reason);
  }
  /// Closes specified pop-up and all its descendants
  /// <param name="startPopup">Pop-up to close (and its descendants)</param>
  /// <param name="reason">Reason for closing</param>
  static void Dismiss(RibbonPopup startPopup, DismissReason reason) {
    int index = pops.IndexOf(startPopup);
    if (index >= 0) {
      Dismiss(index, reason);
    }
  }
  /// Closes pop-up of the specified index and all its descendants
  /// <param name="startPopup">Index of the pop-up to close</param>
  /// <param name="reason">Reason for closing</param>
  static void Dismiss(int startPopup, DismissReason reason) {
    for (int i = pops.Count - 1; i >= startPopup; i--) {
      pops[i].Close();
    }
  }
  //#endregion
}
}

