/*
 2008 Jos?Manuel Menéndez Poo
 *
 * Please give me credit if you use this code. It's all I ask.
 *
 * Contact me for more info: menendezpoo@gmail.com
 *
 */
using System;
using System.Collections.Generic;
using System.Text;
namespace System.Windows.Forms
{
/// Represents a collection of items that is hosted by the RibbonItemGroup
struct RibbonItemGroupItemCollection : RibbonItemCollection {
  RibbonItemGroup _ownerGroup;
  /// <param name="ownerGroup">Group that this collection belongs to</param>
  RibbonItemGroupItemCollection(RibbonItemGroup ownerGroup) {
    _ownerGroup = ownerGroup;
  }
  /// Gets the group that owns this item collection
  RibbonItemGroup OwnerGroup {
    get
    {
      return _ownerGroup;
    }
  }
  /// Adds the specified item to the collection
  new void Add(RibbonItem item) {
    item.MaxSizeMode = RibbonElementSizeMode_Compact;
    item.SetOwnerGroup(OwnerGroup);
    base.Add(item);
  }
  /// Adds the specified range of items
  /// <param name="items">Items to add</param>
  new void AddRange(IEnumerable<RibbonItem> items) {
    foreach(RibbonItem item in items) {
      item.MaxSizeMode = RibbonElementSizeMode_Compact;
      item.SetOwnerGroup(OwnerGroup);
    }
    base.AddRange(items);
  }
  /// Inserts the specified item at the desired index
  /// <param name="index">Desired index of the item</param>
  /// <param name="item">Item to insert</param>
  new void Insert(int index, RibbonItem item) {
    item.MaxSizeMode = RibbonElementSizeMode_Compact;
    item.SetOwnerGroup(OwnerGroup);
    base.Insert(index, item);
  }
}
}

