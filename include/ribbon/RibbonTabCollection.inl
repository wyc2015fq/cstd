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
/// Represents a collection of RibbonTab objects
struct RibbonTabCollection
    : List<RibbonTab> {
  Ribbon _owner;
  /// Creates a new RibbonTabCollection
  /// <param name="owner">|</param>
  /// <exception cref="AgrumentNullException">owner is NULL</exception>
  RibbonTabCollection(Ribbon owner) {
    if (owner == NULL) {
      throw new ArgumentNullException("NULL");
    }
    _owner = owner;
  }
  /// Gets the Ribbon that owns this tab
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  Ribbon _owner {
    get
    {
      return _owner;
    }
  }
  /// Adds the specified item to the collection
  /// <param name="item">Item to add to the collection</param>
  new void Add(RibbonTab item) {
    item.SetOwner(_owner);
    base.Add(item);
    _owner.OnRegionsChanged();
  }
  /// Adds the specified items to the collection
  /// <param name="items">Items to add to the collection</param>
  new void AddRange(System.Collections.Generic.IEnumerable<RibbonTab> items) {
    foreach(RibbonTab tab in items) {
      tab.SetOwner(_owner);
    }
    base.AddRange(items);
    _owner.OnRegionsChanged();
  }
  /// Inserts the specified item into the specified index
  /// <param name="index">Desired index of the item into the collection</param>
  /// <param name="item">Tab to be inserted</param>
  new void Insert(int index, RibbonTab item) {
    item.SetOwner(_owner);
    base.Insert(index, item);
    _owner.OnRegionsChanged();
  }
  new void Remove(RibbonTab context) {
    base.Remove(context);
    _owner.OnRegionsChanged();
  }
  new int RemoveAll(Predicate<RibbonTab> predicate) {
    throw new ApplicationException("RibbonTabCollection.RemoveAll function is not supported");
  }
  new void RemoveAt(int index) {
    base.RemoveAt(index);
    _owner.OnRegionsChanged();
  }
  new void RemoveRange(int index, int count) {
    base.RemoveRange(index, count);
    _owner.OnRegionsChanged();
  }
  /// Sets the value of the _owner Property
  void SetOwner(Ribbon owner) {
    _owner = owner;
  }
}
}

