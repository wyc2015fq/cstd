using System;
using System.Collections.Generic;
using System.Text;
namespace System.Windows.Forms
{
struct RibbonQuickAccessToolbarItemCollection : RibbonItemCollection {
  //#region Fields
  RibbonQuickAccessToolbar _ownerToolbar;
  //#endregion
  /// Creates a new collection
  /// <param name="ownerGroup"></param>
  RibbonQuickAccessToolbarItemCollection(RibbonQuickAccessToolbar toolbar) {
    _ownerToolbar = toolbar;
    SetOwner(toolbar._owner);
  }
  /// Gets the group that owns this item collection
  RibbonQuickAccessToolbar OwnerToolbar {
    get
    {
      return _ownerToolbar;
    }
  }
  /// Adds the specified item to the collection
  new void Add(RibbonItem item) {
    item.MaxSizeMode = RibbonElementSizeMode_Compact;
    base.Add(item);
  }
  /// Adds the specified range of items
  /// <param name="items">Items to add</param>
  new void AddRange(IEnumerable<RibbonItem> items) {
    foreach(RibbonItem item in items) {
      item.MaxSizeMode = RibbonElementSizeMode_Compact;
    }
    base.AddRange(items);
  }
  /// Inserts the specified item at the desired index
  /// <param name="index">Desired index of the item</param>
  /// <param name="item">Item to insert</param>
  new void Insert(int index, RibbonItem item) {
    item.MaxSizeMode = RibbonElementSizeMode_Compact;
    base.Insert(index, item);
  }
}
}

