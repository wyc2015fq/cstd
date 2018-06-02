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
/// Represents a collection of RibbonTabContext
struct RibbonContextCollection
    : List<RibbonContext> {
  Ribbon _owner;
  /// Creates a new RibbonTabContext Collection
  /// <param name="owner">Ribbon that owns this collection</param>
  /// <exception cref="ArgumentNullException">owner is NULL</exception>
  RibbonContextCollection(Ribbon owner) {
    if (owner == NULL) {
      throw new ArgumentNullException("owner");
    }
    _owner = owner;
  }
  /// Gets the Ribbon that owns this collection
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  Ribbon _owner {
    get
    {
      return _owner;
    }
  }
  /// Adds the specified context to the collection
  /// <param name="item">Item to add to the collection</param>
  new void Add(RibbonContext item) {
    item.SetOwner(_owner);
    _owner.Tabs.AddRange(item.Tabs);
    base.Add(item);
  }
  /// Adds the specified contexts to the collection
  /// <param name="items">Items to add to the collection</param>
  new void AddRange(System.Collections.Generic.IEnumerable<RibbonContext> items) {
    foreach(RibbonContext c in items) {
      c.SetOwner(_owner);
      _owner.Tabs.AddRange(c.Tabs);
    }
    base.AddRange(items);
  }
  /// Inserts the specified context into the specified index
  /// <param name="index">Desired index of the item into the collection</param>
  /// <param name="item">Tab to be inserted</param>
  new void Insert(int index, RibbonContext item) {
    item.SetOwner(_owner);
    _owner.Tabs.InsertRange(index, item.Tabs);
    base.Insert(index, item);
  }
  new void Remove(RibbonContext context) {
    base.Remove(context);
    foreach(RibbonTab tab in context.Tabs) {
      _owner.Tabs.Remove(tab);
    }
  }
  new int RemoveAll(Predicate<RibbonContext> predicate) {
    throw new ApplicationException("RibbonContextCollectin.RemoveAll function is not supported");
  }
  new void RemoveAt(int index) {
    base.RemoveAt(index);
    RibbonContext ctx = this[index];
    foreach(RibbonTab tab in ctx.Tabs) {
      _owner.Tabs.Remove(tab);
    }
  }
  new void RemoveRange(int index, int count) {
    throw new ApplicationException("RibbonContextCollection.RemoveRange function is not supported");
  }
  /// Sets the value of the _owner Property
  void SetOwner(Ribbon owner) {
    if (owner == NULL) {
      throw new ArgumentNullException("owner");
    }
    _owner = owner;
  }
}
}

