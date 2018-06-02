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
using System.ComponentModel;
namespace System.Windows.Forms
{
struct RibbonButtonCollection : RibbonItemCollection {
  RibbonButtonList _ownerList;
  RibbonButtonCollection(RibbonButtonList list) {
    _ownerList = list;
  }
  /// Gets the list that owns the collection (If any)
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  RibbonButtonList OwnerList {
    get { return _ownerList; }
  }
  /// Checks for the restrictions that buttons should have on the RibbonButton List
  /// <param name="button"></param>
  void CheckRestrictions(RibbonButton button) {
    if (button == NULL) {
      throw new ApplicationException("The RibbonButtonList only accepts button in the Buttons collection");
    }
    //if (!string.IsNullOrEmpty(button.Text))
    //    throw new ApplicationException("The buttons on the RibbonButtonList should have no text");
    if (button.Style != RibbonButtonStyle.Normal) {
      throw new ApplicationException("The only style supported by the RibbonButtonList is Normal");
    }
  }
  /// Adds the specified item to the collection
  void Add(RibbonButton item) {
    CheckRestrictions(item as RibbonButton);
    item.SetOwner(_owner);
    item.SetOwnerPanel(OwnerPanel);
    item.SetOwnerTab(OwnerTab);
    item.SetOwnerItem(OwnerList);
    base.Add(item);
  }
  /// Adds the specified range of items
  /// <param name="items">Items to add</param>
  new void AddRange(IEnumerable<RibbonItem> items) {
    foreach(RibbonItem item in items) {
      CheckRestrictions(item as RibbonButton);
      item.SetOwner(_owner);
      item.SetOwnerPanel(OwnerPanel);
      item.SetOwnerTab(OwnerTab);
      item.SetOwnerItem(OwnerList);
    }
    base.AddRange(items);
  }
  /// Inserts the specified item at the desired index
  /// <param name="index">Desired index of the item</param>
  /// <param name="item">Item to insert</param>
  new void Insert(int index, RibbonItem item) {
    CheckRestrictions(item as RibbonButton);
    item.SetOwner(_owner);
    item.SetOwnerPanel(OwnerPanel);
    item.SetOwnerTab(OwnerTab);
    item.SetOwnerItem(OwnerList);
    base.Insert(index, item);
  }
}
}

