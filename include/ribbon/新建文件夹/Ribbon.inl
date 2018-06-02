//#include <string>
//#include <vector>
#define EventArgs_Empty NULL
struct EventArgs {
  //Empty,
};
struct SelectionTypes {
  enum {
    Primary,
  };
};
enum RibbonItemClass {
  RibbonItemClass_RibbonButton,
  RibbonItemClass_RibbonCaptionButton,
  RibbonItemClass_RibbonSeparator,
  RibbonItemClass_RibbonOrbOptionButton,
  RibbonItemClass_RibbonItemGroup,
  RibbonItemClass_RibbonButtonList,
  RibbonItemClass_RibbonTextBox,
};
enum RibbonButtonStyle {
  /// Simple clickable button
  RibbonButtonStyle_Normal,
  /// Button with a right side drop down
  RibbonButtonStyle_DropDown,
  /// Button with an optional dropdown attachment on the right
  RibbonButtonStyle_SplitDropDown,
};
/// Represents the size modes that a RibbonElement can be
enum RibbonElementSizeMode {
  /// The item is being shown on a dropdown
  RibbonElementSizeMode_DropDown = 5,
  /// Maximum size the element can reach
  RibbonElementSizeMode_Large = 4,
  /// A medium size for the element when not much space is available
  RibbonElementSizeMode_Medium = 3,
  /// The minimum size the element can be
  RibbonElementSizeMode_Compact = 2,
  /// The item doesn't fit as compact, so it must be shown as a single button
  RibbonElementSizeMode_Overflow = 1,
  /// No size mode specified
  RibbonElementSizeMode_None = 0,
};
/// Represents the directions that arrows can have
enum RibbonArrowDirection {
  /// The arrow points up
  RibbonArrowDirection_Up,
  /// The arrow points down
  RibbonArrowDirection_Down,
  /// The arrow points right
  RibbonArrowDirection_Right,
  /// The arrow points left
  RibbonArrowDirection_Left
};
enum Corners {
  Corners_None = 0,
  Corners_NorthWest = 2,
  Corners_NorthEast = 4,
  Corners_SouthEast = 8,
  Corners_SouthWest = 16,
  Corners_All = Corners_NorthWest | Corners_NorthEast | Corners_SouthEast | Corners_SouthWest,
  Corners_North = Corners_NorthWest | Corners_NorthEast,
  Corners_South = Corners_SouthEast | Corners_SouthWest,
  Corners_East = Corners_NorthEast | Corners_SouthEast,
  Corners_West = Corners_NorthWest | Corners_SouthWest
};
/// Represents possible flow directions of items on the panels
enum RibbonPanelFlowDirection {
  /// Layout of items flows to the left, then down
  RibbonPanelFlowDirection_r = 1,
  /// Layout of items flows to the bottom, then to the right
  RibbonPanelFlowDirection_b = 0,
};
enum ListScrollType {
  ListScrollType_UpDownButtons,
  ListScrollType_Scrollbar
};
/// Possible modes for the ribbon to be placed on the window
enum RibbonWindowMode {
  RibbonWindowMode_InsideWindow,
  RibbonWindowMode_NonClientAreaCustomDrawn,
  RibbonWindowMode_NonClientAreaGlass
};
#define throw_ArgumentNullException(x)  return 0;
static IRECT FromLTRB(int l, int t, int r, int b)
{
  IRECT rc;
  *(IRECT*)&rc = iRECT(l, t, r, b);
  return rc;
}
#include "gdi.inl"
struct RibbonItem {
  //#region Fields
  IRECT _bounds;
  ISIZE _lastMeasureSize;
  const char* _text;
  img_t* _image;
  const char* _tooltip;
  bool _checked;
  bool _selected;
  bool _pressed;
  bool _enabled;
  const char* _tooltipTitle;
  img_t* _tooltipImage;
  const char* _altKey;
  RibbonElementSizeMode _maxSize;
  RibbonElementSizeMode _minSize;
  RibbonItem* _ownerItem;
  int _sizeMode;
  RibbonItem* _items;
  int _items_count;
  int type;
  IRECT _contentBounds;
#if 0
  //#region Events
  event EventHandler DoubleClick;
  event EventHandler Click;
  event MouseEventHandler MouseUp;
  event MouseEventHandler MouseMove;
  event MouseEventHandler MouseDown;
  event MouseEventHandler MouseEnter;
  event MouseEventHandler MouseLeave;
  event EventHandler CanvasChanged;
  //#region Ctor
  RibbonItem() {
    _enabled = true;
    //Click += new EventHandler(RibbonItem_Click);
  }
  /// Selects the item when in a dropdown, in design mode
  /// <param name="sender"></param>
  void RibbonItem_Click(object sender, EventArgs e) {
    //RibbonDropDown dd = Canvas as RibbonDropDown;
    //if (dd != NULL && dd.SelectionService != NULL)
    {
      //dd.SelectionService.SetSelectedComponents( this, Primary);
    }
  }
  /// Called on every element when its time to draw itself
  /// <param name="g">Device to draw</param>
  /// <param name="sender">Object that is invoking the paint element</param>
  /// <param name="e">Paint event data</param>
  void OnPaint(Object sender, RibbonElementPaintEventArgs e) {}
#endif
  /// Gets the size in pixels needed for the element in the specified mode
  /// <param name="sender">Object that sends the measure message</param>
  /// <param name="e">Event data</param>
  ISIZE MeasureSize(int _sizeMode, const font_t* fo) {
    ISIZE sz = {0};
    return sz;
  }
  /// Called to make the element aware of its actual bounds on the control
  void SetBounds(IRECT bounds) {
    _bounds = bounds;
  }
#if 0
  /// Gets the bounds of the item
  /// Gets the bounds of the element relative to the Ribbon control
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  IRECT GetBounds() {
    return _bounds;
  }
  /// Used to extract all child components from RibbonItem objects
  Component* GetAllChildComponents(int* pCount) {
    return NULL;
  }
  //#region Props
  /// Gets the bounds of the item's content. (It takes the Ribbon._itemMargin)
  /// <remarks>
  /// Although this is the regular item content bounds, it depends on the logic of the item
  /// and how each item handles its own content.
  /// </remarks>
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  IRECT GetContentBounds() {
    return FromLTRB(
        _bounds.l + _owner->_itemMargin.l,
        _bounds.t + _owner->_itemMargin.t,
        _bounds.r - _owner->_itemMargin.r,
        _bounds.b - _owner->_itemMargin.b);
  }
  /// Gets the RibbonItemGroup that owns the item (If any)
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  RibbonItem* GetOwnerItem() {
    return _ownerItem;
  }
  /// Gets or sets the text that is to be displayed on the item
  //[DefaultValue("")]
  //[Localizable(true)]
  const char* GetText() {
    return _text;
  }
  int SetText(const char* value) {
    _text = value;
    NotifyOwnerRegionsChanged();
    return 0;
  }
  /// Gets or sets the image to be displayed on the item
  img_t* GetImage() {
    return _image;
  }
  int SetImage(img_t* value) {
    _image = value;
    NotifyOwnerRegionsChanged();
    return 0;
  }
  /// Gets or sets the tool tip text
  //[DefaultValue("")]
  //[Localizable(true)]
  const char* GetToolTip() {
    return _tooltip;
  }
  int SetToolTip(const char* value) {
    _tooltip = value;
    return 0;
  }
  /// Gets or sets a value indicating if the item is currently checked
  //[DefaultValue(false)]
  bool GetChecked() {
    return _checked;
  }
  int SetChecked(bool value) {
    _checked = value;
    NotifyOwnerRegionsChanged();
    return 0;
  }
  /// Gets the item's current _sizeMode
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  RibbonElementSizeMode GetSizeMode() {
    return _sizeMode;
  }
  /// Gets a value indicating whether the item is selected
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool GetSelected() {
    return _selected;
  }
  /// Gets a value indicating whether the state of the item is pressed
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool GetPressed() {
    return _pressed;
  }
  /// Gets the Ribbon owner of this item
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  Ribbon* GetOwner() {
    return _owner;
  }
  /// When implemented, must return an  enumerator to acces the items inside the type
  RibbonItem* GetItems(int* pCount) {
    return NULL;
  }
  /// When implemented, must return the bounds of the content where items are displayed
  /// <returns></returns>
  //virtual IRECT GetContentBounds() = 0;
  /// Gets or sets a value indicating if the item is currently enabled
  //[DefaultValue(true)]
  bool GetEnabled() {
    return _enabled;
  }
  int SetEnabled(bool value) {
    _enabled = value;
    int i, Count = 0;
    RibbonItem* item = GetItems(&Count);
    for (i = 0; i < Count; ++i) {
      item[i].SetEnabled(value);
    }
    NotifyOwnerRegionsChanged();
    return 0;
  }
  /// Gets or sets the tool tip title
  //[DefaultValue("")]
  const char* GetToolTipTitle() {
    return _tooltipTitle;
  }
  int SetToolTipTitle(const char* value) {
    _tooltipTitle = value;
    return 0;
  }
  /// Gets or sets the image of the tool tip
  //[DefaultValue("")]
  img_t* GetToolTipImage() {
    return _tooltipImage;
  }
  int SetToolTipImage(img_t* value) {
    _tooltipImage = value;
    return 0;
  }
  /// Gets or sets the object that contains data about the control
  object* GetTag() {
    return _tag;
  }
  int SetTag(object* value) {
    _tag = value;
    return 0;
  }
  /// Gets or sets the key combination that activates this element when the Alt key was pressed
  //[DefaultValue("")]
  const char* GetAltKey() {
    return _altKey;
  }
  int GetAltKey(const char* value) {
    _altKey = value;
    return 0;
  }
  /// Gets the RibbonTab that contains this item
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  RibbonTab* GetOwnerTab() {
    return _ownerTab;
  }
  /// Gets the RibbonPanel where this item is located
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  RibbonPanel* GetOwnerPanel() {
    return _ownerPanel;
  }
  /// Gets or sets the maximum size mode of the element
  //[DefaultValue(RibbonElementSizeMode_None)]
  RibbonElementSizeMode GetMaxSizeMode() {
    return _maxSize;
  }
  int SetMaxSizeMode(RibbonElementSizeMode value) {
    _maxSize = value;
    NotifyOwnerRegionsChanged();
    return 0;
  }
  /// Gets or sets the minimum size mode of the element
  //[DefaultValue(RibbonElementSizeMode_None)]
  RibbonElementSizeMode GetMinSizeMode() {
    return _minSize;
  }
  int SetMinSizeMode(RibbonElementSizeMode value) {
    _minSize = value;
    NotifyOwnerRegionsChanged();
    return 0;
  }
  /// Gets the last result of  MeasureSize
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  ISIZE GetLastMeasuredSize() {
    return _lastMeasureSize;
  }
  //#region Methods
  /// Gets if onwer dropdown must be closed when the item is clicked on the specified point
  /// <param name="p">IPOINT to test.</param>
  /// <returns></returns>
  bool ClosesDropDownAt(IPOINT p) {
    return true;
  }
  /// Forces the owner Ribbon to update its regions
  void NotifyOwnerRegionsChanged() {
#if 0
    if (_owner != NULL) {
      if (_owner == GetCanvas()) {
        _owner->OnRegionsChanged();
      }
      else if (GetCanvas() != NULL) {
        if (Canvas is RibbonOrbDropDown) {
          (Canvas as RibbonOrbDropDown).OnRegionsChanged();
        }
        else {
          Canvas.Invalidate(Bounds);
        }
      }
    }
#endif
  }
  /// Sets the value of the <see cref="OwnerItem"/> property
  /// <param name="item"></param>
  void SetOwnerItem(RibbonItem* item) {
    _ownerItem = item;
  }
  /// Sets the Ribbon that owns this item
  /// <param name="owner">Ribbon that owns this item</param>
  void SetOwner(Ribbon* owner) {
    _owner = owner;
  }
  /// Sets the value of the OwnerPanel property
  /// <param name="ownerPanel">RibbonPanel where this item is located</param>
  void SetOwnerPanel(RibbonPanel* ownerPanel) {
    _ownerPanel = ownerPanel;
  }
  /// Sets the value of the _selected property
  /// <param name="selected">Value that indicates if the element is selected</param>
  void SetSelected(bool selected) {
    if (!_enabled) {
      return;
    }
    _selected = selected;
  }
  /// Sets the value of the _pressed property
  /// <param name="pressed">Value that indicates if the element is pressed</param>
  void SetPressed(bool pressed) {
    _pressed = pressed;
  }
  /// Sets the value of the _ownerTab property
  /// <param name="ownerTab">RibbonTab where this item is located</param>
  void SetOwnerTab(RibbonTab* ownerTab) {
    _ownerTab = ownerTab;
  }
  /// Sets the value of the OwnerList property
  /// <param name="ownerList"></param>
  void SetOwnerGroup(RibbonItem* ownerGroup) {
    _ownerItem = ownerGroup;
  }
#endif
  /// Gets the size applying the rules of MaxSizeMode and MinSizeMode properties
  /// <param name="sizeMode">Suggested sizeMode</param>
  /// <returns>The nearest size to the specified one</returns>
  RibbonElementSizeMode GetNearestSize(int sizeMode) {
    int size = (int)sizeMode;
    int max = (int)_maxSize;
    int min = (int)_minSize;
    int result = (int)sizeMode;
    if (max > 0 && size > max) { //Max is specified and value exceeds max
      result = max;
    }
    if (min > 0 && size < min) { //Min is specified and value exceeds min
      result = min;
    }
    return (RibbonElementSizeMode)result;
  }
  /// Sets the value of the LastMeasuredSize property
  /// <param name="size">ISIZE to set to the property</param>
  void SetLastMeasuredSize(ISIZE size) {
    _lastMeasureSize = size;
  }
#if 0
  /// Sets the value of the _sizeMode property
  /// <param name="sizeMode"></param>
  void SetSizeMode(RibbonElementSizeMode sizeMode) {
    _sizeMode = GetNearestSize(sizeMode);
  }
  /// Raises the <see cref="CanvasChanged"/> event
  void OnCanvasChanged(EventArgs* e) {
#if 0
    if (CanvasChanged != NULL) {
      CanvasChanged(this, e);
    }
#endif
  }
  /// Raises the MouseEnter event
  /// <param name="e">Event data</param>
  void OnMouseEnter(MouseEventArgs e) {
    if (!_enabled) {
      return;
    }
#if 0
    if (MouseEnter != NULL) {
      MouseEnter(this, e);
    }
#endif
  }
  /// Raises the MouseDown event
  /// <param name="e">Event data</param>
  void OnMouseDown(MouseEventArgs e) {
    if (!_enabled) {
      return;
    }
#if 0
    if (MouseDown != NULL) {
      MouseDown(this, e);
    }
    RibbonPopup pop = Canvas as RibbonPopup;
    if (pop != NULL) {
      if (ClosesDropDownAt(e.Location)) {
        RibbonPopupManager.Dismiss(RibbonPopupManager.DismissReason.ItemClicked);
      }
      OnClick(EventArgs_Empty);
    }
    SetPressed(true);
#endif
  }
  /// Raises the MouseLeave event
  /// <param name="e">Event data</param>
  void OnMouseLeave(MouseEventArgs e) {
    if (!_enabled) {
      return;
    }
#if 0
    if (MouseLeave != NULL) {
      MouseLeave(this, e);
    }
#endif
  }
  /// Raises the MouseUp event
  /// <param name="e">Event data</param>
  void OnMouseUp(MouseEventArgs e) {
    if (!_enabled) {
      return;
    }
#if 0
    if (MouseUp != NULL) {
      MouseUp(this, e);
    }
    if (_pressed) {
      SetPressed(false);
      RedrawItem();
    }
#endif
  }
  /// Raises the MouseMove event
  /// <param name="e">Event data</param>
  void OnMouseMove(MouseEventArgs e) {
    if (!_enabled) {
      return;
    }
#if 0
    if (MouseMove != NULL) {
      MouseMove(this, e);
    }
#endif
  }
  /// Raises the Click event
  /// <param name="e">Event data</param>
  void OnClick(EventArgs e) {
    if (!_enabled) {
      return;
    }
#if 0
    if (Click != NULL) {
      Click(this, e);
    }
#endif
  }
  /// Raises the DoubleClick event
  /// <param name="e">Event data</param>
  void OnDoubleClick(EventArgs e) {
    if (!_enabled) {
      return;
    }
#if 0
    if (DoubleClick != NULL) {
      DoubleClick(this, e);
    }
#endif
  }
  /// Redraws the item area on the Onwer Ribbon
  void RedrawItem() {
#if 0
    if (Canvas != NULL) {
      Canvas.Invalidate(IRECT.Inflate(Bounds, 1, 1));
    }
#endif
  }
  /// Sets the canvas of the item
  /// <param name="canvas"></param>
  void SetCanvas(Control* canvas) {
    _canvas = canvas;
    SetCanvas(this, canvas);
    OnCanvasChanged(EventArgs_Empty);
  }
  /// Recurse on setting the canvas
  /// <param name="parent"></param>
  /// <param name="canvas"></param>
  void SetCanvas(RibbonItem* parent, Control* canvas) {
    int i, Count = 0;
    if (parent == NULL) {
      return;
    }
    RibbonItem* item = parent->GetItems(&Count);
    for (i = 0; i < Count; ++i) {
      item[i].SetCanvas(canvas);
    }
  }
#endif
};
//[Editor("RibbonItemCollectionEditor", typeof(UITypeEditor))]
//struct RibbonItemCollection : List<RibbonItem> {
#if 0
//#region Fields
Ribbon _owner;
RibbonTab _ownerTab;
RibbonPanel _ownerPanel;
//#endregion
//#region Ctor
/// Creates a new ribbon item collection
RibbonItemCollection()
{
}
//#endregion
//#region Properties
/// Gets the Ribbon owner of this collection
//[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
Ribbon _owner {
  get
  {
    return _owner;
  }
}
/// Gets the RibbonPanel where this item is located
//[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
RibbonPanel OwnerPanel {
  get
  {
    return _ownerPanel;
  }
}
/// Gets the RibbonTab that contains this item
//[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
RibbonTab _ownerTab {
  get
  {
    return _ownerTab;
  }
}
//#endregion
//#region Overrides
/// Adds the specified item to the collection
new void Add(RibbonItem item)
{
  item->SetOwner(_owner);
  item->SetOwnerPanel(OwnerPanel);
  item->SetOwnerTab(_ownerTab);
  base.Add(item);
}
/// Adds the specified range of items
/// <param name="items">_items to add</param>
new void AddRange(int count, RibbonItem* items)
{
  for (RibbonItem* item = items, *item_end = items + count; item < item_end; ++item) {
    item->SetOwner(_owner);
    item->SetOwnerPanel(OwnerPanel);
    item->SetOwnerTab(_ownerTab);
  }
  base.AddRange(count, items);
}
/// Inserts the specified item at the desired index
/// <param name="index">Desired index of the item</param>
/// <param name="item">Item to insert</param>
new void Insert(int index, RibbonItem item)
{
  item->SetOwner(_owner);
  item->SetOwnerPanel(OwnerPanel);
  item->SetOwnerTab(_ownerTab);
  base.Insert(index, item);
}
//#endregion
//#region Methods
#endif
/// Gets the left of items as a group of shapes
/// <returns></returns>
int GetItemsLeft(int count, RibbonItem* items)
{
  if (count == 0) {
    return 0;
  }
  int min = MAX_INT;
  for (RibbonItem* item = items, *item_end = items + count; item < item_end; ++item) {
    if (item->_bounds.l < min) {
      min = item->_bounds.l;
    }
  }
  return min;
}
/// Gets the right of items as a group of shapes
/// <returns></returns>
int GetItemsRight(int count, RibbonItem* items)
{
  if (count == 0) {
    return 0;
  }
  int max = MIN_INT;
  for (RibbonItem* item = items, *item_end = items + count; item < item_end; ++item) {
    if (item->_bounds.r > max) {
      max = item->_bounds.r;
    }
  }
  return max;
}
/// Gets the top of items as a group of shapes
/// <returns></returns>
int GetItemsTop(int count, RibbonItem* items)
{
  if (count == 0) {
    return 0;
  }
  int min = MAX_INT;
  for (RibbonItem* item = items, *item_end = items + count; item < item_end; ++item) {
    if (item->_bounds.t < min) {
      min = item->_bounds.t;
    }
  }
  return min;
}
/// Gets the bottom of items as a group of shapes
/// <returns></returns>
int GetItemsBottom(int count, RibbonItem* items)
{
  if (count == 0) {
    return 0;
  }
  int max = MIN_INT;
  for (RibbonItem* item = items, *item_end = items + count; item < item_end; ++item) {
    if (item->_bounds.b > max) {
      max = item->_bounds.b;
    }
  }
  return max;
}
/// Gets the width of items as a group of shapes
/// <returns></returns>
int GetItemsWidth(int count, RibbonItem* items)
{
  return GetItemsRight(count, items) - GetItemsLeft(count, items);
}
/// Gets the height of items as a group of shapes
/// <returns></returns>
int GetItemsHeight(int count, RibbonItem* items)
{
  return GetItemsBottom(count, items) - GetItemsTop(count, items);
}
/// Gets the bounds of items as a group of shapes
/// <returns></returns>
IRECT GetItemsBounds(int count, RibbonItem* items)
{
  return FromLTRB(GetItemsLeft(count, items), GetItemsTop(count, items), GetItemsRight(count, items), GetItemsBottom(count, items));
}
/// Moves the bounds of items as a group of shapes
/// <param name="p"></param>
void MoveTo(int count, RibbonItem* items, IPOINT p)
{
  IRECT oldBounds = GetItemsBounds(count, items);
  int i;
  for (i = 0; i < count; ++i) {
    RibbonItem* item = items + i;
    int dx = item->_bounds.l - oldBounds.l;
    int dy = item->_bounds.t - oldBounds.t;
    item->SetBounds(iRECT2(p.x + dx, p.y + dy, RCW(&item->_bounds), RCH(&item->_bounds)));
  }
}
#if 0
/// Centers the items on the specified rectangle
/// <param name="rectangle"></param>
void CenterItemsInto(IRECT rectangle)
{
  CenterItemsInto(this, rectangle);
}
/// Centers the items vertically on the specified rectangle
/// <param name="rectangle"></param>
void CenterItemsVerticallyInto(IRECT rectangle)
{
  CenterItemsVerticallyInto(this, rectangle);
}
/// Centers the items horizontally on the specified rectangle
/// <param name="rectangle"></param>
void CenterItemsHorizontallyInto(IRECT rectangle)
{
  CenterItemsHorizontallyInto(this, rectangle);
}
#endif
/// Centers the items on the specified rectangle
/// <param name="rectangle"></param>
void CenterItemsInto(int count, RibbonItem* items, IRECT rectangle)
{
  int x = rectangle.l + (RCW(&rectangle) - GetItemsWidth(count, items)) / 2;
  int y = rectangle.t + (RCH(&rectangle) - GetItemsHeight(count, items)) / 2;
  MoveTo(count, items, iPOINT(x, y));
}
/// Centers the items vertically on the specified rectangle
/// <param name="rectangle"></param>
void CenterItemsVerticallyInto(int count, RibbonItem* items, IRECT rectangle)
{
  int x = GetItemsLeft(count, items);
  int y = rectangle.t + (RCH(&rectangle) - GetItemsHeight(count, items)) / 2;
  MoveTo(count, items, iPOINT(x, y));
}
#if 0
/// Centers the items horizontally on the specified rectangle
/// <param name="rectangle"></param>
void CenterItemsHorizontallyInto(int count, RibbonItem* items, IRECT rectangle)
{
  int x = rectangle.l + (rectangle.Width - GetItemsWidth(count, items)) / 2;
  int y = GetItemsTop(count, items);
  MoveTo(items, new IPOINT(x, y));
}
/// Sets the owner Ribbon of the collection
/// <param name="owner"></param>
void SetOwner(Ribbon owner)
{
  _owner = owner;
  foreach(RibbonItem item in this) {
    item->SetOwner(owner);
  }
}
/// Sets the owner Tab of the collection
/// <param name="tab"></param>
void SetOwnerTab(RibbonTab tab)
{
  _ownerTab = tab;
  foreach(RibbonItem item in this) {
    item->SetOwnerTab(tab);
  }
}
/// Sets the owner panel of the collection
/// <param name="panel"></param>
void SetOwnerPanel(RibbonPanel panel)
{
  _ownerPanel = panel;
  foreach(RibbonItem item in this) {
    item->SetOwnerPanel(panel);
  }
}
#endif
//#endregion
//};
template <typename T> inline
int AddRange(T* a, const T* b)
{
  int i, n = b->size();
  for (i = 0; i < n; ++i) {
    a->push_back(b[i]);
  }
  return 0;
}
////[ToolboxItemAttribute(false)]
struct RibbonPopup {
  //#region Fields
  //RibbonWrappedDropDown _toolStripDropDown;
  int _borderRoundness;
  bool DoubleBuffered;
  bool _drawIconsBar;
  IRECT _bounds;
#if 0
  //#region Events
  event EventHandler Showed;
  /// Raised when the popup is closed
  event EventHandler Closed;
  /// Raised when the popup is about to be closed
  event ToolStripDropDownClosingEventHandler Closing;
  /// Raised when the Popup is about to be opened
  event CancelEventHandler Opening;
  //#region Ctor
  RibbonPopup() {
    SetStyle(ControlStyles.Opaque, true);
    SetStyle(ControlStyles.AllPaintingInWmPaint, true);
    SetStyle(ControlStyles.Selectable, false);
    _borderRoundness = 3;
  }
  //#region Props
  /// Gets or sets the roundness of the border
  //[Browsable(false)]
  int _borderRoundness {
    get { return _borderRoundness; }
    set { _borderRoundness = value; }
  }
  /// Gets the related ToolStripDropDown
  RibbonWrappedDropDown WrappedDropDown {
    get { return _toolStripDropDown; }
    set { _toolStripDropDown = value; }
  }
  //#region Methods
  /// Shows this Popup on the specified location of the screen
  /// <param name="screenLocation"></param>
  void Show(IPOINT screenLocation) {
    ToolStripControlHost host = new ToolStripControlHost(this);
    WrappedDropDown = new RibbonWrappedDropDown();
    WrappedDropDown.AutoClose = RibbonDesigner.Current != NULL;
    WrappedDropDown._items.Add(host);
    WrappedDropDown.IRECT = IRECT.Empty;
    WrappedDropDown.Margin = IRECT.Empty;
    host.IRECT = IRECT.Empty;
    host.Margin = IRECT.Empty;
    WrappedDropDown.Opening += new CancelEventHandler(ToolStripDropDown_Opening);
    WrappedDropDown.Closing += new ToolStripDropDownClosingEventHandler(ToolStripDropDown_Closing);
    WrappedDropDown.Closed += new ToolStripDropDownClosedEventHandler(ToolStripDropDown_Closed);
    WrappedDropDown.ISIZE = ISIZE;
    WrappedDropDown.Show(screenLocation);
    RibbonPopupManager.Register(this);
    OnShowed(EventArgs_Empty);
  }
  /// Handles the Opening event of the ToolStripDropDown
  /// <param name="sender"></param>
  void ToolStripDropDown_Opening(object sender, CancelEventArgs e) {
    OnOpening(e);
  }
  /// Called when pop-up is being opened
  void OnOpening(CancelEventArgs e) {
    if (Opening != NULL) {
      Opening(this, e);
    }
  }
  /// Handles the Closing event of the ToolStripDropDown
  /// <param name="sender"></param>
  void ToolStripDropDown_Closing(object sender, ToolStripDropDownClosingEventArgs e) {
    OnClosing(e);
  }
  /// Handles the closed event of the ToolStripDropDown
  /// <param name="sender"></param>
  void ToolStripDropDown_Closed(object sender, ToolStripDropDownClosedEventArgs e) {
    OnClosed(EventArgs_Empty);
  }
  /// Closes this popup.
  void Close() {
    if (WrappedDropDown != NULL) {
      WrappedDropDown.Close();
    }
  }
  /// Raises the <see cref="Closing"/> event
  void OnClosing(ToolStripDropDownClosingEventArgs e) {
    if (Closing != NULL) {
      Closing(this, e);
    }
  }
  /// Raises the <see cref="Closed"/> event
  void OnClosed(EventArgs e) {
    RibbonPopupManager.Unregister(this);
    if (Closed != NULL) {
      Closed(this, e);
    }
    //if (NextPopup != NULL)
    //{
    //    NextPopup.CloseForward();
    //    NextPopup = NULL;
    //}
    //if (PreviousPopup != NULL && PreviousPopup.NextPopup.Equals(this))
    //{
    //    PreviousPopup.NextPopup = NULL;
    //}
  }
  /// Raises the Showed event
  void OnShowed(EventArgs e) {
    if (Showed != NULL) {
      Showed(this, e);
    }
  }
  /// Overriden. Used to drop a shadow on the popup
  CreateParams CreateParams {
    get {
      CreateParams cp = base.CreateParams;
      if (WinApi.IsXP) {
        cp.ClassStyle |= WinApi.CS_DROPSHADOW;
      }
      return cp;
    }
  }
#endif
};
struct RibbonSeparator : RibbonItem {
#if 0
  RibbonSeparator() {
  }
  RibbonSeparator(string text) {
    _text = text;
  }
  void OnPaint(object sender, RibbonElementPaintEventArgs e) {
    OnRenderRibbonItem(new RibbonItemRenderEventArgs(
        _owner, e.Graphics, e.Clip, this));
    if (!string.IsNullOrEmpty(_text)) {
      OnRenderRibbonItemText(new RibbonTextEventArgs(
          _owner, e.Graphics, e.Clip, this,
          FromLTRB(
              Bounds.l + _owner._itemMargin.l,
              Bounds.t + _owner._itemMargin.t,
              Bounds.r - _owner._itemMargin.r,
              Bounds.b - _owner._itemMargin.b), _text, FontStyle.Bold));
    }
  }
  void SetBounds(IRECT bounds) {
    base.SetBounds(bounds);
  }
  ISIZE MeasureSize(object sender, RibbonElementMeasureSizeEventArgs e) {
    if (e._sizeMode == RibbonElementSizeMode_DropDown) {
      if (string.IsNullOrEmpty(_text)) {
        SetLastMeasuredSize(new ISIZE(1, 3));
      }
      else {
        ISIZE sz = e.Graphics.MeasureString(_text, new Font(_owner.Font, FontStyle.Bold)).ToSize();
        SetLastMeasuredSize(new ISIZE(sz.Width + _owner._itemMargin RCX(&), sz.Height + _owner._itemMargin RCY(&)));
      }
    }
    else {
      SetLastMeasuredSize(new ISIZE(2, OwnerPanel._contentBounds.Height - _owner._itemPadding RCY(&) - _owner._itemMargin RCY(&)));
    }
    return LastMeasuredSize;
  }
#endif
};
#if 0
/// Provides mouse functionality to RibbonTab, RibbonPanel and RibbonItem objects on a specified Control
struct RibbonMouseSensor { // : IDisposable
  //#region Fields
  Ribbon* _ribbon;
  RibbonTab** _tabs;
  RibbonPanel** _panels;
  RibbonItem** _items;
  RibbonTab* _tabLimit;
  RibbonPanel* _panelLimit;
  RibbonItem** _itemsLimit;
  bool _disposed;
  bool _suspended;
  RibbonTab* _hittedTab;
  RibbonPanel* _hittedPanel;
  RibbonItem* _hittedItem;
  RibbonItem* _hittedSubItem;
  bool _hittedTabScrollLeft;
  bool _hittedTabScrollRight;
  RibbonTab* _selectedTab;
  RibbonPanel* _selectedPanel;
  RibbonItem* _selectedItem;
  RibbonItem* _selectedSubItem;
#if 0
  //#region Constructor
  /// Initializes inner fields
  int init() {
    _tabs = new List<RibbonTab*>();
    _panels = new List<RibbonPanel*>();
    _items = new List<RibbonItem*>();
    return 0;
  }
  void AddHandlers() {}
  /// Creates a new Empty Sensor
  /// <param name="control">Control to listen mouse events</param>
  /// <param name="ribbon">Ribbon that will be affected</param>
  int init(Control* control, Ribbon* ribbon) {
    if (control == NULL) {
      throw_ArgumentNullException("control");
    }
    if (ribbon == NULL) {
      throw_ArgumentNullException("ribbon");
    }
    _control = control;
    _ribbon = ribbon;
    AddHandlers();
    return 0;
  }
  /// Creates a new Sensor for specified objects
  /// <param name="control">Control to listen mouse events</param>
  /// <param name="ribbon">Ribbon that will be affected</param>
  /// <param name="tabs">_tabs that will be sensed</param>
  /// <param name="panels">Panels that will be sensed</param>
  /// <param name="items">_items that will be sensed</param>
  int init(Control* control, Ribbon* ribbon, IEnumerable<RibbonTab*>* tabs, IEnumerable<RibbonPanel*>* panels, IEnumerable<RibbonItem*>* items) {
    init(control, ribbon);
    if (tabs != NULL) {
      AddRange(_tabs, tabs);
    }
    if (panels != NULL) {
      AddRange(_panels, panels);
    }
    if (items != NULL) {
      AddRange(_items, items);
    }
    return 0;
  }
  /// Creates a new Sensor for the specified RibbonTab
  /// <param name="control">Control to listen to mouse events</param>
  /// <param name="ribbon">Ribbon that will be affected</param>
  /// <param name="tab">Tab that will be sensed, from which all panels and items will be extracted to sensing also.</param>
  int init(Control* control, Ribbon* ribbon, RibbonTab* tab) {
    init(control, ribbon);
    _tabs->push_back(tab);
    AddRange(_panels, &tab->_panels);
    foreach(RibbonPanel panel in tab->_panels) {
      _items.AddRange(panel._items);
    }
    return 0;
  }
  /// Creates a new Sensor for only the specified items
  /// <param name="control">Control to listen to mouse events</param>
  /// <param name="ribbon">Ribbon that will be affected</param>
  /// <param name="items">_items that will be sensed</param>
  RibbonMouseSensor(Control control, Ribbon ribbon, IEnumerable<RibbonItem> itemsSource)
    : this(control, ribbon) {
    ItemsSource = itemsSource;
  }
  //#region Properties
  /// Gets the control where the sensor listens to mouse events
  Control Control {
    get { return _control; }
  }
  /// Gets if the sensor has already been
  bool Disposed {
    get { return _disposed; }
  }
  /// Gets the RibbonTab hitted by the last <see cref="HitTest"/>
  RibbonTab HittedTab {
    get { return _hittedTab; }
    set { _hittedTab = value; }
  }
  /// Gets if the test hit resulted on some scroll button of the hitted tab
  bool HittedTabScroll {
    get { return HittedTabScrollLeft || HittedTabScrollRight; }
  }
  /// Gets or sets if the last hit test resulted on the left scroll of the hitted tab
  bool HittedTabScrollLeft {
    get { return _hittedTabScrollLeft; }
    set { _hittedTabScrollLeft = value; }
  }
  /// Gets or sets if the last hit test resulted on the right scroll of the hitted tab
  bool HittedTabScrollRight {
    get { return _hittedTabScrollRight; }
    set { _hittedTabScrollRight = value; }
  }
  /// Gets the RibbonPanel hitted by the last <see cref="HitTest"/>
  RibbonPanel HittedPanel {
    get { return _hittedPanel; }
    set { _hittedPanel = value; }
  }
  /// Gets the RibbonItem hitted by the last <see cref="HitTest"/>
  RibbonItem HittedItem {
    get { return _hittedItem; }
    set { _hittedItem = value; }
  }
  /// Gets the RibbonItem (on other RibbonItem) hitted by the last <see cref="HitTest"/>
  RibbonItem HittedSubItem {
    get { return _hittedSubItem; }
    set { _hittedSubItem = value; }
  }
  /// Gets if the sensor is currently suspended
  bool IsSupsended {
    get { return _suspended; }
  }
  /// Gets or ests the source of items what limits the sensing.
  /// If collection is NULL, all items on the <see cref="_items"/> property will be sensed.
  IEnumerable<RibbonItem> ItemsSource {
    get { return _itemsLimit; }
    set { _itemsLimit = value; }
  }
  /// Gets the collection of items this sensor affects.
  /// Sensing can be limitated by the <see cref="ItemsLimit"/> property
  List<RibbonItem> _items {
    get { return _items; }
  }
  /// Gets or sets the Panel that will be the limit to be sensed.
  /// If set to NULL, all panels in the <see cref="Panels"/> property will be sensed.
  RibbonPanel PanelLimit {
    get { return _panelLimit; }
    set { _panelLimit = value; }
  }
  /// Gets the collection of panels this sensor affects.
  /// Sensing can be limitated by the <see cref="PanelLimit"/> property
  List<RibbonPanel> Panels {
    get { return _panels; }
  }
  /// Gets the ribbon this sensor responds to
  Ribbon Ribbon {
    get { return _ribbon; }
  }
  /// Gets or sets the last selected tab
  RibbonTab SelectedTab {
    get { return _selectedTab; }
    set { _selectedTab = value; }
  }
  /// Gets or sets the last selected panel
  RibbonPanel SelectedPanel {
    get { return _selectedPanel; }
    set { _selectedPanel = value; }
  }
  /// Gets or sets the last selected item
  RibbonItem SelectedItem {
    get { return _selectedItem; }
    set { _selectedItem = value; }
  }
  /// Gets or sets the last selected sub-item
  RibbonItem SelectedSubItem {
    get { return _selectedSubItem; }
    set { _selectedSubItem = value; }
  }
  /// Gets or sets the Tab that will be the only to be sensed.
  /// If set to NULL, all tabs in the <see cref="_tabs"/> property will be sensed.
  RibbonTab TabLimit {
    get { return _tabLimit; }
    set { _tabLimit = value; }
  }
  /// Gets the collection of tabs this sensor affects.
  /// Sensing can be limitated by the <see cref="TabLimit"/> property
  List<RibbonTab> _tabs {
    get { return _tabs; }
  }
  //#region Methods
  /// Adds the necessary handlers to the control
  void AddHandlers() {
    if (Control == NULL) {
      throw new ApplicationException("Control is Null, cant Add RibbonMouseSensor Handles");
    }
    Control.MouseMove += new MouseEventHandler(Control_MouseMove);
    Control.MouseLeave += new EventHandler(Control_MouseLeave);
    Control.MouseDown += new MouseEventHandler(Control_MouseDown);
    Control.MouseUp += new MouseEventHandler(Control_MouseUp);
    Control.MouseClick += new MouseEventHandler(Control_MouseClick);
    Control.MouseDoubleClick += new MouseEventHandler(Control_MouseDoubleClick);
  }
  void Control_MouseDoubleClick(object sender, MouseEventArgs e) {
    if (IsSupsended || Disposed) {
      return;
    }
    //#region Panel
    if (HittedPanel != NULL) {
      HittedPanel.OnDoubleClick(e);
    }
    //#region Item
    if (HittedItem != NULL) {
      HittedItem.OnDoubleClick(e);
    }
    //#region SubItem
    if (HittedSubItem != NULL) {
      HittedSubItem.OnDoubleClick(e);
    }
  }
  void Control_MouseClick(object sender, MouseEventArgs e) {
    if (IsSupsended || Disposed) {
      return;
    }
    //#region Panel
    if (HittedPanel != NULL) {
      HittedPanel.OnClick(e);
    }
    //#region Item
    if (HittedItem != NULL) {
      HittedItem.OnClick(e);
    }
    //#region SubItem
    if (HittedSubItem != NULL) {
      HittedSubItem.OnClick(e);
    }
  }
  /// Handles the MouseUp event on the control
  /// <param name="sender"></param>
  void Control_MouseUp(object sender, MouseEventArgs e) {
    if (IsSupsended || Disposed) {
      return;
    }
    //#region Tab Scrolls
    if (HittedTab != NULL) {
      if (HittedTab._scrollLeftVisible) {
        HittedTab.SetScrollLeftPressed(false);
        Control.Invalidate(HittedTab._scrollLeftBounds);
      }
      if (HittedTab._scrollRightVisible) {
        HittedTab.SetScrollRightPressed(false);
        Control.Invalidate(HittedTab._scrollRightBounds);
      }
    }
    //#region Panel
    if (HittedPanel != NULL) {
      HittedPanel.SetPressed(false);
      HittedPanel.OnMouseUp(e);
      Control.Invalidate(HittedPanel.Bounds);
    }
    //#region Item
    if (HittedItem != NULL) {
      HittedItem.SetPressed(false);
      HittedItem.OnMouseUp(e);
      Control.Invalidate(HittedItem.Bounds);
    }
    //#region SubItem
    if (HittedSubItem != NULL) {
      HittedSubItem.SetPressed(false);
      HittedSubItem.OnMouseUp(e);
      Control.Invalidate(IRECT.Intersect(HittedItem.Bounds, HittedSubItem.Bounds));
    }
  }
  /// Handles the MouseDown on the control
  /// <param name="sender"></param>
  void Control_MouseDown(object sender, MouseEventArgs e) {
    if (IsSupsended || Disposed) {
      return;
    }
    HitTest(e.Location);
    //#region Tab Scrolls
    if (HittedTab != NULL) {
      if (HittedTabScrollLeft) {
        HittedTab.SetScrollLeftPressed(true);
        Control.Invalidate(HittedTab._scrollLeftBounds);
      }
      if (HittedTabScrollRight) {
        HittedTab.SetScrollRightPressed(true);
        Control.Invalidate(HittedTab._scrollRightBounds);
      }
    }
    //#region Panel
    if (HittedPanel != NULL) {
      HittedPanel.SetPressed(true);
      HittedPanel.OnMouseDown(e);
      Control.Invalidate(HittedPanel.Bounds);
    }
    //#region Item
    if (HittedItem != NULL) {
      HittedItem.SetPressed(true);
      HittedItem.OnMouseDown(e);
      Control.Invalidate(HittedItem.Bounds);
    }
    //#region SubItem
    if (HittedSubItem != NULL) {
      HittedSubItem.SetPressed(true);
      HittedSubItem.OnMouseDown(e);
      Control.Invalidate(IRECT.Intersect(HittedItem.Bounds, HittedSubItem.Bounds));
    }
  }
  /// Handles the MouseLeave on the control
  /// <param name="sender"></param>
  void Control_MouseLeave(object sender, EventArgs e) {
    if (IsSupsended || Disposed) {
      return;
    }
  }
  /// Handles the MouseMove on the control
  /// <param name="sender"></param>
  void Control_MouseMove(object sender, MouseEventArgs e) {
    if (IsSupsended || Disposed) {
      return;
    }
    HitTest(e.Location);
    //#region _selected ones
    if (SelectedPanel != NULL && SelectedPanel != HittedPanel) {
      SelectedPanel.SetSelected(false);
      SelectedPanel.OnMouseLeave(e);
      Control.Invalidate(SelectedPanel.Bounds);
    }
    if (SelectedItem != NULL && SelectedItem != HittedItem) {
      SelectedItem.SetSelected(false);
      SelectedItem.OnMouseLeave(e);
      Control.Invalidate(SelectedItem.Bounds);
    }
    if (SelectedSubItem != NULL && SelectedSubItem != HittedSubItem) {
      SelectedSubItem.SetSelected(false);
      SelectedSubItem.OnMouseLeave(e);
      Control.Invalidate(IRECT.Intersect(SelectedItem.Bounds, SelectedSubItem.Bounds));
    }
    //#region Tab Scrolls
    if (HittedTab != NULL) {
      if (HittedTab._scrollLeftVisible) {
        HittedTab.SetScrollLeftSelected(HittedTabScrollLeft);
        Control.Invalidate(HittedTab._scrollLeftBounds);
      }
      if (HittedTab._scrollRightVisible) {
        HittedTab.SetScrollRightSelected(HittedTabScrollRight);
        Control.Invalidate(HittedTab._scrollRightBounds);
      }
    }
    //#region Panel
    if (HittedPanel != NULL) {
      if (HittedPanel == SelectedPanel) {
        HittedPanel.OnMouseMove(e);
      }
      else {
        HittedPanel.SetSelected(true);
        HittedPanel.OnMouseEnter(e);
        Control.Invalidate(HittedPanel.Bounds);
      }
    }
    //#region Item
    if (HittedItem != NULL) {
      if (HittedItem == SelectedItem) {
        HittedItem.OnMouseMove(e);
      }
      else {
        HittedItem.SetSelected(true);
        HittedItem.OnMouseEnter(e);
        Control.Invalidate(HittedItem.Bounds);
      }
    }
    //#region SubItem
    if (HittedSubItem != NULL) {
      if (HittedSubItem == SelectedSubItem) {
        HittedSubItem.OnMouseMove(e);
      }
      else {
        HittedSubItem.SetSelected(true);
        HittedSubItem.OnMouseEnter(e);
        Control.Invalidate(IRECT.Intersect(HittedItem.Bounds, HittedSubItem.Bounds));
      }
    }
  }
  /// Performs a hit-test and specifies hitted objects on properties: <see cref="HittedPanel"/>,
  /// <see cref="HittedTab"/>, <see cref="HittedItem"/> and <see cref="HittedSubItem"/>
  /// <param name="x"></param>
  /// <param name="y"></param>
  void HitTest(IPOINT p) {
    SelectedTab = HittedTab;
    SelectedPanel = HittedPanel;
    SelectedItem = HittedItem;
    SelectedSubItem = HittedSubItem;
    HittedTab = NULL;
    HittedTabScrollLeft = false;
    HittedTabScrollRight = false;
    HittedPanel = NULL;
    HittedItem = NULL;
    HittedSubItem = NULL;
    //#region _tabs
    if (TabLimit != NULL) {
      if (TabLimit.TabContentBounds.Contains(p)) {
        HittedTab = TabLimit;
      }
    }
    else {
      foreach(RibbonTab tab in _tabs) {
        if (tab.TabContentBounds.Contains(p)) {
          HittedTab = tab;
          break;
        }
      }
    }
    //#region TabScrolls
    if (HittedTab != NULL) {
      HittedTabScrollLeft = HittedTab._scrollLeftVisible && HittedTab._scrollLeftBounds.Contains(p);
      HittedTabScrollRight = HittedTab._scrollRightVisible && HittedTab._scrollRightBounds.Contains(p);
    }
    if (!HittedTabScroll) {
      //#region Panels
      if (PanelLimit != NULL) {
        if (PanelLimit.Bounds.Contains(p)) {
          HittedPanel = PanelLimit;
        }
      }
      else {
        foreach(RibbonPanel pnl in Panels) {
          if (pnl.Bounds.Contains(p)) {
            HittedPanel = pnl;
            break;
          }
        }
      }
      //#region Item
      IEnumerable<RibbonItem> items = _items;
      if (ItemsSource != NULL) {
        items = ItemsSource;
      }
      foreach(RibbonItem item in items) {
        if (item.OwnerPanel != NULL && item.OwnerPanel.OverflowMode && !(Control is RibbonPanelPopup)) {
          continue;
        }
        if (item.Bounds.Contains(p)) {
          HittedItem = item;
          break;
        }
      }
      //#region Subitem
      IContainsSelectableRibbonItems container = HittedItem as IContainsSelectableRibbonItems;
      IScrollableRibbonItem scrollable = HittedItem as IScrollableRibbonItem;
      if (container != NULL) {
        IRECT sensibleBounds = scrollable != NULL ? scrollable._contentBounds : HittedItem.Bounds;
        foreach(RibbonItem item in container.GetItems()) {
          IRECT actualBounds = item.Bounds;
          actualBounds.Intersect(sensibleBounds);
          if (actualBounds.Contains(p)) {
            HittedSubItem = item;
          }
        }
      }
    }
  }
  /// Removes the added handlers to the Control
  void RemoveHandlers() {
    foreach(RibbonItem item in _items) {
      item.SetSelected(false);
      item.SetPressed(false);
    }
    Control.MouseMove -= Control_MouseMove;
    Control.MouseLeave -= Control_MouseLeave;
    Control.MouseDown -= Control_MouseDown;
    Control.MouseUp -= Control_MouseUp;
  }
  /// Resumes the sensing after being suspended by <see cref="Suspend"/>
  void Resume() {
    _suspended = false;
  }
  /// Suspends sensing until <see cref="Resume"/> is called
  void Suspend() {
    _suspended = true;
  }
  //#region IDisposable Members
  void Dispose() {
    _disposed = true;
    RemoveHandlers();
  }
#endif
};
#endif
enum CaptionButton {
  CaptionButton_Minimize,
  CaptionButton_Maximize,
  CaptionButton_Restore,
  CaptionButton_Close
};
//[Designer(typeof(RibbonButtonDesigner))]
struct RibbonButton {
  //#region Fields
  enum { arrowWidth = 5 };
  RibbonItem c[1];
  int _style;
  int _corners;
  int _faceRounding;
  int _ddRounding;
  IRECT _dropDownBounds;
  IRECT _buttonFaceBounds;
  //RibbonItemCollection _dropDownItems;
  bool _dropDownPressed;
  bool _dropDownSelected;
  img_t* _smallImage;
  ISIZE _dropDownArrowSize;
  IRECT _dropDownMargin;
  bool _dropDownVisible;
  //RibbonDropDown _dropDown;
  IRECT _imageBounds;
  IRECT _textBounds;
  bool _dropDownResizable;
  bool _checkOnClick;
  IPOINT _lastMousePos;
  int _DropDownArrowDirection;
  int init() {
    _dropDownArrowSize = iSIZE(5, 3);
    _dropDownMargin = iRECT(6, 6, 6, 6);
    _DropDownArrowDirection = RibbonArrowDirection_Down;
#if 0
    img_t = CreateImage(32);
    _smallImage = CreateImage(16);
    _dropDownItems = new RibbonItemCollection();
#endif
    return 0;
  }
#if 0
  //#region Events
  /// Occurs when the dropdown is about to be displayed
  event EventHandler DropDownShowing;
  //#region Ctors
  /// Creates a new button
  /// <param name="image">img_t of the button (32 x 32 suggested)</param>
  /// <param name="smallImage">img_t of the button when in medium of compact mode (16 x 16 suggested)</param>
  /// <param name="style">_style of the button</param>
  /// <param name="text">_text of the button</param>
  RibbonButton(string text)
    : this() {
    _text = text;
  }
  RibbonButton(img_t smallImage)
    : this() {
    _smallImage = smallImage;
  }
  //#region Props
  /// Gets the DropDown of the button
  RibbonDropDown DropDown {
    get { return _dropDown; }
  }
  /// Gets or sets a value indicating if the <see cref="Checked"/> property should be toggled
  /// when button is clicked
  //[DefaultValue(false)]
  [Description("Toggles the Checked property of the button when clicked")]
  bool CheckOnClick {
    get { return _checkOnClick; }
    set { _checkOnClick = value; }
  }
  /// Gets or sets a value indicating if the DropDown should be resizable
  //[DefaultValue(false)]
  [Description("Makes the DropDown resizable with a grip on the corner")]
  bool DropDownResizable {
    get { return _dropDownResizable; }
    set { _dropDownResizable = value; }
  }
  /// Gets the bounds where the <see cref="img_t"/> or <see cref="_smallImage"/> will be drawn.
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  IRECT ImageBounds {
    get { return _imageBounds; }
  }
  /// Gets the bounds where the <see cref="_text"/> of the button will be drawn
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  IRECT TextBounds {
    get { return _textBounds; }
  }
  /// Gets if the DropDown is currently visible
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool DropDownVisible {
    get { return _dropDownVisible; }
  }
  /// Gets or sets the size of the dropdown arrow
  ISIZE DropDownArrowSize {
    get { return _dropDownArrowSize; }
    set { _dropDownArrowSize = value; NotifyOwnerRegionsChanged(); }
  }
  /// Gets or sets the direction where drop down's arrow should be pointing to
  RibbonArrowDirection DropDownArrowDirection {
    get { return _DropDownArrowDirection; }
    set { _DropDownArrowDirection = value; NotifyOwnerRegionsChanged(); }
  }
  /// Gets the style of the button
  RibbonButtonStyle _style {
    get
    {
      return _style;
    }
    set
    {
      _style = value;
      if (Canvas is RibbonPopup
      || (OwnerItem != NULL && OwnerItem.Canvas is RibbonPopup)) {
        DropDownArrowDirection = RibbonArrowDirection.l;
      }
      NotifyOwnerRegionsChanged();
    }
  }
  /// Gets the collection of items shown on the dropdown pop-up when _style allows it
  //[DesignerSerializationVisibility( DesignerSerializationVisibility.Content)]
  RibbonItemCollection DropDownItems {
    get
    {
      return _dropDownItems;
    }
  }
  /// Gets the bounds of the button face
  /// <remarks>When _style is different from SplitDropDown and SplitBottomDropDown, ButtonFaceBounds is the same area than DropDownBounds</remarks>
  //[DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
  IRECT ButtonFaceBounds {
    get
    {
      return _buttonFaceBounds;
    }
  }
  /// Gets the bounds of the dropdown button
  /// <remarks>When _style is different from SplitDropDown and SplitBottomDropDown, ButtonFaceBounds is the same area than DropDownBounds</remarks>
  //[DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
  IRECT DropDownBounds {
    get
    {
      return _dropDownBounds;
    }
  }
  /// Gets if the dropdown part of the button is selected
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool DropDownSelected {
    get
    {
      return _dropDownSelected;
    }
  }
  /// Gets if the dropdown part of the button is pressed
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool DropDownPressed {
    get
    {
      return _dropDownPressed;
    }
  }
  /// Gets or sets the image of the button when in compact of medium size mode
  //[DefaultValue(NULL)]
  img_t _smallImage {
    get
    {
      return _smallImage;
    }
    set
    {
      _smallImage = value;
      NotifyOwnerRegionsChanged();
    }
  }
  //#region Methods
  /// Sets the value of the <see cref="_dropDownMargin"/> property
  /// <param name="p"></param>
  void SetDropDownMargin(IRECT p) {
    _dropDownMargin = p;
  }
  /// Performs a click on the button
  void PerformClick() {
    OnClick(EventArgs_Empty);
  }
  /// Creates a new Transparent, empty image
  /// <param name="size"></param>
  /// <returns></returns>
  img_t CreateImage(int size) {
    Bitmap bmp = new Bitmap(size, size);
    //using (Graphics g = Graphics.FromImage(bmp))
    //{
    //    g.Clear(Color.FromArgb(50, Color.Navy));
    //}
    return bmp;
  }
  /// Creates the DropDown menu
  void CreateDropDown() {
    _dropDown = new RibbonDropDown(this, DropDownItems, _owner);
  }
  void SetPressed(bool pressed) {
    base.SetPressed(pressed);
  }
  void SetOwner(Ribbon owner) {
    base.SetOwner(owner);
    if (_dropDownItems != NULL) {
      _dropDownItems.SetOwner(owner);
    }
  }
  void SetOwnerPanel(RibbonPanel ownerPanel) {
    base.SetOwnerPanel(ownerPanel);
    if (_dropDownItems != NULL) {
      _dropDownItems.SetOwnerPanel(ownerPanel);
    }
  }
  void SetOwnerTab(RibbonTab ownerTab) {
    base.SetOwnerTab(ownerTab);
    if (_dropDownItems != NULL) {
      _dropDownItems.SetOwnerTab(ownerTab);
    }
  }
  /// Sets the bounds of the button
  /// <param name="bounds"></param>
  void SetBounds(IRECT bounds) {
    base.SetBounds(bounds);
    RibbonElementSizeMode sMode = GetNearestSize(_sizeMode);
    _imageBounds = OnGetImageBounds(sMode, bounds);
    _textBounds = OnGetTextBounds(sMode, bounds);
    if (_style == RibbonButtonStyle_SplitDropDown) {
      _dropDownBounds = OnGetDropDownBounds(sMode, bounds);
      _buttonFaceBounds = OnGetButtonFaceBounds(sMode, bounds);
    }
  }
#endif
  /// Sets the bounds of the image of the button when SetBounds is called.
  /// Override this method to change image bounds
  /// <param name="sMode">Mode which is being measured</param>
  /// <param name="bounds">Bounds of the button</param>
  /// <remarks>
  /// The measuring occours in the following order:
  /// <list type="">
  /// <item>OnSetImageBounds</item>
  /// <item>OnSetTextBounds</item>
  /// <item>OnSetDropDownBounds</item>
  /// <item>OnSetButtonFaceBounds</item>
  /// </list>
  /// </remarks>
  IRECT OnGetImageBounds(int sMode, IRECT _itemMargin) {
    IRECT Bounds = c->_contentBounds;
    if (sMode == RibbonElementSizeMode_Large) { // || this is RibbonOrbMenuItem)
      if (c->_image != NULL) {
        return iRECT2(Bounds.l + ((RCW(&Bounds) - c->_image->w) / 2), Bounds.t + _itemMargin.t, c->_image->w, c->_image->h);
      }
      else {
        return iRECT2(c->_contentBounds.l, c->_contentBounds.t, 32, 32);
      }
    }
    else {
      if (_smallImage != NULL) {
        return iRECT(Bounds.l + _itemMargin.l, Bounds.t + ((RCH(&Bounds) - _smallImage->h) / 2), _smallImage->w, _smallImage->h);
      }
      else {
        return iRECT(c->_contentBounds.l, c->_contentBounds.t, 0, 0);
      }
    }
  }
  /// Sets the bounds of the text of the button when SetBounds is called.
  /// Override this method to change image bounds
  /// <param name="sMode">Mode which is being measured</param>
  /// <param name="bounds">Bounds of the button</param>
  /// <remarks>
  /// The measuring occours in the following order:
  /// <list type="">
  /// <item>OnSetImageBounds</item>
  /// <item>OnSetTextBounds</item>
  /// <item>OnSetDropDownBounds</item>
  /// <item>OnSetButtonFaceBounds</item>
  /// </list>
  /// </remarks>
  IRECT OnGetTextBounds(int sMode, IRECT Bounds, IRECT _itemMargin) {
    int imgw = RCW(&_imageBounds);
    int imgh = RCH(&_imageBounds);
    if (sMode == RibbonElementSizeMode_Large) {
      return FromLTRB(
          Bounds.l + _itemMargin.l,
          Bounds.t + _itemMargin.t + imgh,
          Bounds.r - _itemMargin.r,
          Bounds.b - _itemMargin.b);
    }
    else {
      int ddw = _style != RibbonButtonStyle_Normal ? RCX(&_dropDownMargin) : 0;
      return FromLTRB(
          Bounds.l + imgw + RCX(&_itemMargin) + _itemMargin.l,
          Bounds.t + _itemMargin.t,
          Bounds.r - ddw,
          Bounds.b - _itemMargin.b);
    }
  }
  /// Sets the bounds of the dropdown part of the button when SetBounds is called.
  /// Override this method to change image bounds
  /// <param name="sMode">Mode which is being measured</param>
  /// <param name="bounds">Bounds of the button</param>
  /// <remarks>
  /// The measuring occours in the following order:
  /// <list type="">
  /// <item>OnSetImageBounds</item>
  /// <item>OnSetTextBounds</item>
  /// <item>OnSetDropDownBounds</item>
  /// <item>OnSetButtonFaceBounds</item>
  /// </list>
  /// </remarks>
  IRECT OnGetDropDownBounds(int sMode, IRECT bounds, IRECT _itemMargin) {
    IRECT rc = {0};
    IRECT sideBounds = FromLTRB(
        bounds.r -  RCX(&_dropDownMargin) - 2,
        bounds.t, bounds.r, bounds.b);
    switch (c->_sizeMode) {
    case RibbonElementSizeMode_Large:
    case RibbonElementSizeMode_Overflow:
      rc = FromLTRB(bounds.l,
          bounds.t + c->_image->h + RCY(&_itemMargin),
          bounds.r, bounds.b);
    case RibbonElementSizeMode_DropDown:
    case RibbonElementSizeMode_Medium:
    case RibbonElementSizeMode_Compact:
      rc = sideBounds;
    }
    return rc;
  }
#if 0
  /// Sets the bounds of the button face part of the button when SetBounds is called.
  /// Override this method to change image bounds
  /// <param name="sMode">Mode which is being measured</param>
  /// <param name="bounds">Bounds of the button</param>
  /// <remarks>
  /// The measuring occours in the following order:
  /// <list type="">
  /// <item>OnSetImageBounds</item>
  /// <item>OnSetTextBounds</item>
  /// <item>OnSetDropDownBounds</item>
  /// <item>OnSetButtonFaceBounds</item>
  /// </list>
  /// </remarks>
  IRECT OnGetButtonFaceBounds(RibbonElementSizeMode sMode, IRECT bounds) {
    IRECT sideBounds = FromLTRB(
        bounds.r - _dropDownMargin RCX(&) - 2,
        bounds.t, bounds.r, bounds.b);
    switch (_sizeMode) {
    case RibbonElementSizeMode_Large:
    case RibbonElementSizeMode_Overflow:
      return FromLTRB(bounds.l,
          bounds.t, bounds.r, _dropDownBounds.t);
    case RibbonElementSizeMode_DropDown:
    case RibbonElementSizeMode_Medium:
    case RibbonElementSizeMode_Compact:
      return FromLTRB(bounds.l, bounds.t,
          _dropDownBounds.l, bounds.b);
    }
    return IRECT.Empty;
  }
#endif
  /// Measures the string for the large size
  /// <param name="g"></param>
  /// <param name="text"></param>
  /// <param name="font"></param>
  /// <returns></returns>
  static ISIZE MeasureStringLargeSize(const char* text, const font_t* fo) {
    int i;
    ISIZE sz = {0};
    if (NULL == text) {
      return sz;
    }
    sz = font_text_size(fo, text, -1, 0);
    if (strchr(text, ' ')) {
      vstr_t words[1] = {0};
      strv_split_str(words, STR1(text), " ", "", 0);
      int longestWord = 0;
      int width = sz.w;
      str_t* ps = words->v + longestWord;
      for (i = 1; i < words->n; i++) {
        if (words->v[i].l > words->v[longestWord].l) {
          longestWord = i;
        }
      }
      width = font_text_size(fo, ps->s, ps->l, 0).w;
      width = MAX(sz.w / 2, width) + 1;
      sz.w = MIN(sz.w, width);
      strv_free(words);
    }
    return sz;
  }
  /// Measures the size of the button
  /// <param name="sender"></param>
  /// <returns></returns>
  ISIZE MeasureSize(int _sizeMode, const font_t* fo) {
    RibbonElementSizeMode theSize = c->GetNearestSize(_sizeMode);
    ISIZE sz = {0};
#if 0
    int widthSum = RCX(&_itemMargin);
    int heightSum = RCY(&_itemMargin);
    int largeHeight = OwnerPanel == NULL ? 0 : OwnerPanel._contentBounds.Height - _owner._itemPadding RCY(&);// -_owner._itemMargin RCY(&); //58;
    ISIZE simg = _smallImage != NULL ? _smallImage.ISIZE : ISIZE.Empty;
    ISIZE img = img_t != NULL ? img_t.ISIZE : ISIZE.Empty;
    ISIZE sz = ISIZE.Empty;
    switch (theSize) {
    case RibbonElementSizeMode_Large:
    case RibbonElementSizeMode_Overflow:
      sz = MeasureStringLargeSize(e.Graphics, _text, _owner.Font);
      if (!string.IsNullOrEmpty(_text)) {
        widthSum += MAX(sz.Width + 1, img.Width);
        heightSum = largeHeight;
      }
      else {
        widthSum += img.Width;
        heightSum += img.Height;
      }
      break;
    case RibbonElementSizeMode_DropDown:
    case RibbonElementSizeMode_Medium:
      sz = TextRenderer.MeasureText(_text, _owner.Font);
      if (!string.IsNullOrEmpty(_text)) {
        widthSum += sz.Width + 1;
      }
      widthSum += simg.Width + _owner._itemMargin RCX(&);
      heightSum += MAX(sz.Height, simg.Height);
      break;
    case RibbonElementSizeMode_Compact:
      widthSum += simg.Width;
      heightSum += simg.Height;
      break;
    default:
      throw new ApplicationException("_sizeMode not supported: " + e._sizeMode.ToString());
    }
    if (theSize == RibbonElementSizeMode_DropDown) {
      heightSum += 2;
    }
    if (_style == RibbonButtonStyle_DropDown) {
      widthSum += arrowWidth + _owner._itemMargin.r;
    }
    else if (_style == RibbonButtonStyle_SplitDropDown) {
      widthSum += arrowWidth + _owner._itemMargin RCX(&);
    }
    sz = iSIZE(widthSum, heightSum);
#endif
    return sz;
  }
#if 0
  /// Sets the value of the DropDownPressed property
  /// <param name="pressed">Value that indicates if the dropdown button is pressed</param>
  void SetDropDownPressed(bool pressed) {
    throw new System.NotImplementedException();
  }
  /// Sets the value of the DropDownSelected property
  /// <param name="selected">Value that indicates if the dropdown part of the button is selected</param>
  void SetDropDownSelected(bool selected) {
    //Dont use, an overflow occours
    throw new Exception();
  }
  /// Shows the drop down items of the button, as if the dropdown part has been clicked
  void ShowDropDown() {
    if (_style == RibbonButtonStyle_Normal || DropDownItems.Count == 0) {
      if (DropDown != NULL) {
        RibbonPopupManager.DismissChildren(DropDown, RibbonPopupManager.DismissReason.NewPopup);
      }
      return;
    }
    if (_style == RibbonButtonStyle_DropDown) {
      SetPressed(true);
    }
    else {
      _dropDownPressed = true;
    }
    CreateDropDown();
    DropDown.MouseEnter += new EventHandler(DropDown_MouseEnter);
    DropDown.Closed += new EventHandler(_dropDown_Closed);
    DropDown._showSizingGrip = DropDownResizable;
    RibbonPopup canvasdd = Canvas as RibbonPopup;
    IPOINT location = OnGetDropDownMenuLocation();
    ISIZE minsize = OnGetDropDownMenuSize();
    if (!minsize.IsEmpty) {
      DropDown._minimumSize = minsize;
    }
    OnDropDownShowing(EventArgs_Empty);
    SetDropDownVisible(true);
    DropDown.SelectionService = GetService(typeof(ISelectionService)) as ISelectionService;
    DropDown.Show(location);
  }
  void DropDown_MouseEnter(object sender, EventArgs e) {
    SetSelected(true);
    RedrawItem();
  }
  /// Gets the location where the dropdown menu will be shown
  /// <returns></returns>
  IPOINT OnGetDropDownMenuLocation() {
    IPOINT location = IPOINT.Empty;
    if (Canvas is RibbonDropDown) {
      location = Canvas.PointToScreen(new IPOINT(Bounds.r, Bounds.t));
    }
    else {
      location = Canvas.PointToScreen(new IPOINT(Bounds.l, Bounds.b));
    }
    return location;
  }
  /// Gets the size of the dropdown. If ISIZE.Empty is returned,
  /// size will be measured automatically
  /// <returns></returns>
  ISIZE OnGetDropDownMenuSize() {
    return ISIZE.Empty;
  }
  /// Handles the closing of the dropdown
  /// <param name="sender"></param>
  void _dropDown_Closed(object sender, EventArgs e) {
    SetPressed(false);
    _dropDownPressed = false;
    SetDropDownVisible(false);
    SetSelected(false);
    RedrawItem();
  }
  /// Ignores deactivation of canvas if it is a volatile window
  void IgnoreDeactivation() {
    if (Canvas is RibbonPanelPopup) {
      (Canvas as RibbonPanelPopup).IgnoreNextClickDeactivation();
    }
    if (Canvas is RibbonDropDown) {
      (Canvas as RibbonDropDown).IgnoreNextClickDeactivation();
    }
  }
  /// Closes the DropDown if opened
  void CloseDropDown() {
    if (DropDown != NULL) {
      RibbonPopupManager.Dismiss(DropDown, RibbonPopupManager.DismissReason.NewPopup);
    }
    SetDropDownVisible(false);
  }
  /// Overriden. Informs the button text
  /// <returns></returns>
  string ToString() {
    return string.Format("{1}: {0}", _text, GetType().Name);
  }
  /// Sets the value of DropDownVisible
  /// <param name="visible"></param>
  void SetDropDownVisible(bool visible) {
    _dropDownVisible = visible;
  }
  /// Raises the DropDownShowing event
  void OnDropDownShowing(EventArgs e) {
    if (DropDownShowing != NULL) {
      DropDownShowing(this, e);
    }
  }
  //#region Overrides
  void OnCanvasChanged(EventArgs e) {
    base.OnCanvasChanged(e);
    if (Canvas is RibbonDropDown) {
      DropDownArrowDirection = RibbonArrowDirection.l;
    }
  }
  bool ClosesDropDownAt(IPOINT p) {
    if (_style == RibbonButtonStyle_DropDown) {
      return false;
    }
    else if (_style == RibbonButtonStyle_SplitDropDown) {
      return ButtonFaceBounds.Contains(p);
    }
    else {
      return true;
    }
  }
  void SetSizeMode(RibbonElementSizeMode sizeMode) {
    if (sizeMode == RibbonElementSizeMode_Overflow) {
      base.SetSizeMode(RibbonElementSizeMode_Large);
    }
    else {
      base.SetSizeMode(sizeMode);
    }
  }
  void SetSelected(bool selected) {
    base.SetSelected(selected);
    SetPressed(false);
  }
  void OnMouseDown(MouseEventArgs e) {
    if (!_enabled) {
      return;
    }
    if ((DropDownSelected || _style == RibbonButtonStyle_DropDown) && DropDownItems.Count > 0) {
      _dropDownPressed = true;
      ShowDropDown();
    }
    base.OnMouseDown(e);
  }
  void OnMouseUp(MouseEventArgs e) {
    base.OnMouseUp(e);
  }
  void OnMouseMove(MouseEventArgs e) {
    if (!_enabled) {
      return;
    }
    //Detect mouse on dropdwon
    if (_style == RibbonButtonStyle_SplitDropDown) {
      bool lastState = _dropDownSelected;
      if (DropDownBounds.Contains(e.X, e.Y)) {
        _dropDownSelected = true;
      }
      else {
        _dropDownSelected = false;
      }
      if (lastState != _dropDownSelected) {
        RedrawItem();
      }
      lastState = _dropDownSelected;
    }
    _lastMousePos = new IPOINT(e.X, e.Y);
    base.OnMouseMove(e);
  }
  void OnMouseLeave(MouseEventArgs e) {
    base.OnMouseLeave(e);
    _dropDownSelected = false;
  }
  void OnClick(EventArgs e) {
    if (_style != RibbonButtonStyle_Normal && !ButtonFaceBounds.Contains(_lastMousePos)) {
      //Clicked the dropdown area, don't raise Click()
      return;
    }
    if (CheckOnClick) {
      Checked = !Checked;
    }
    base.OnClick(e);
  }
  //#region IContainsRibbonItems Members
  IEnumerable<RibbonItem> GetItems() {
    return DropDownItems;
  }
  //#region IContainsRibbonComponents Members
  IEnumerable<Component> GetAllChildComponents() {
    return DropDownItems.ToArray();
  }
#endif
  //#region Static
  /// Gets the character to render the specified button type
  /// <param name="type">type of button</param>
  /// <returns>Character to use with Marlett font in Windows, some other representative characters when in other O.S.</returns>
  static const char* GetCharFor(CaptionButton type) {
    if (1) {
      switch (type) {
      case CaptionButton_Minimize:
        return "0";
      case CaptionButton_Maximize:
        return "1";
      case CaptionButton_Restore:
        return "2";
      case CaptionButton_Close:
        return "r";
      default:
        return "?";
      }
    }
    else {
      switch (type) {
      case CaptionButton_Minimize:
        return "_";
      case CaptionButton_Maximize:
        return "+";
      case CaptionButton_Restore:
        return "^";
      case CaptionButton_Close:
        return "X";
      default:
        return "?";
      }
    }
  }
  //#region Fields
  int _captionButtonType;
#if 0
  //#region Ctor
  /// Creates a new CaptionButton
  /// <param name="buttonType"></param>
  RibbonCaptionButton(CaptionButton buttonType) {
    SetCaptionButtonType(buttonType);
  }
  //#region Prop
  /// Gets the type of caption button this is
  CaptionButton CaptionButtonType {
    get { return _captionButtonType; }
  }
  //#region Methods
  void OnClick() {
    if (f == NULL) {
      return;
    }
    switch (CaptionButtonType) {
    case CaptionButton_Minimize:
      //f.WindowState = FormWindowState._minimized;
      break;
    case CaptionButton_Maximize:
      //f.WindowState = FormWindowState.Maximized;
      break;
    case CaptionButton_Restore:
      //f.WindowState = FormWindowState.Normal;
      break;
    case CaptionButton_Close:
      //f.Close();
      break;
    default:
      break;
    }
  }
  /// Sets value to the type of caption button
  /// <param name="buttonType"></param>
  void SetCaptionButtonType(CaptionButton buttonType) {
    _text = GetCharFor(buttonType);
    _captionButtonType = buttonType;
  }
  IRECT OnGetTextBounds(RibbonElementSizeMode sMode, IRECT bounds) {
    IRECT r = bounds;
    r.X = bounds.l + 3;
    return r;
  }
#endif
};
//[ToolboxItem(false)]
struct ISelectionService {};
struct RibbonDropDown : RibbonPopup {
  //#region Static
  // static List<RibbonDropDown> registeredDds = new List<RibbonDropDown>();
  // static void RegisterDropDown(RibbonDropDown dropDown)
  //{
  //    registeredDds.Add(dropDown);
  //}
  // static void UnregisterDropDown(RibbonDropDown dropDown)
  //{
  //    registeredDds.Remove(dropDown);
  //}
  //internal static void DismissAll()
  //{
  //    for (int i = 0; i < registeredDds.Count; i++)
  //    {
  //        registeredDds[i].Close();
  //    }
  //    registeredDds.Clear();
  //}
  //
  ///// Closes all the dropdowns before the specified dropDown
  //
  ///// <param name="dropDown"></param>
  //internal static void DismissTo(RibbonDropDown dropDown)
  //{
  //    if (dropDown == NULL) throw new ArgumentNullException("dropDown");
  //    for (int i = registeredDds.Count - 1; i >= 0; i--)
  //    {
  //        if (i >= registeredDds.Count)
  //        {
  //            break;
  //        }
  //        if (registeredDds[i].Equals(dropDown))
  //        {
  //            break;
  //        }
  //        else
  //        {
  //            registeredDds[i].Close();
  //        }
  //    }
  //}
  //#region Fields
  RibbonItem* _items;
  bool _showSizingGrip;
  int _sizingGripHeight;
  //RibbonMouseSensor _sensor[1];
  RibbonItem* _parentItem;
  bool _ignoreNext;
  RibbonElementSizeMode _MeasuringSize;
  bool _resizing;
  IRECT _sizingGripBounds;
  IPOINT _resizeOrigin;
  ISIZE _resizeSize;
  ISelectionService _SelectionService;
  bool _drawIconsBar;
  //#region Ctor
#if 0
  RibbonDropDown() {
    //RegisterDropDown(this);
    DoubleBuffered = true;
    _drawIconsBar = true;
  }
  RibbonDropDown(RibbonItem* parentItem, IEnumerable<RibbonItem*> items, Ribbon* ownerRibbon) {
    init(parentItem, items, ownerRibbon, RibbonElementSizeMode_DropDown);
  }
  RibbonDropDown(RibbonItem* parentItem, IEnumerable<RibbonItem*> items, Ribbon* ownerRibbon, RibbonElementSizeMode measuringSize) {
    init(parentItem, items, ownerRibbon, measuringSize);
  }
  int init(RibbonItem* parentItem, IEnumerable<RibbonItem*> items, Ribbon* ownerRibbon, RibbonElementSizeMode measuringSize) {
    _items = items;
    _ownerRibbon = ownerRibbon;
    _sizingGripHeight = 12;
    _parentItem = parentItem;
    _sensor = new RibbonMouseSensor(this, OwnerRibbon, items);
    _MeasuringSize = measuringSize;
    if (_items != NULL)
      foreach(RibbonItem item in _items) {
      item.SetSizeMode(RibbonElementSizeMode_DropDown);
      item.SetCanvas(this);
    }
    UpdateSize();
    return 0;
  }
  //#region Properties
  /// Gets or sets if the icons bar should be drawn
  bool _drawIconsBar {
    get { return _drawIconsBar; }
    set { _drawIconsBar = value; }
  }
  void OnOpening(CancelEventArgs e) {
    base.OnOpening(e);
    UpdateItemsBounds();
  }
  /// Gets or sets the selection service for the dropdown
  ISelectionService SelectionService {
    get { return _SelectionService; }
    set { _SelectionService = value; }
  }
  /// Gets the bounds of the sizing grip
  IRECT SizingGripBounds {
    get { return _sizingGripBounds; }
  }
  /// Gets or sets the size for measuring items (by default is DropDown)
  RibbonElementSizeMode MeasuringSize {
    get { return _MeasuringSize; }
    set { _MeasuringSize = value; }
  }
  /// Gets the parent item of this dropdown
  RibbonItem ParentItem {
    get { return _parentItem; }
  }
  /// Gets the sennsor of this dropdown
  RibbonMouseSensor Sensor {
    get { return _sensor; }
  }
  /// Gets the Ribbon this DropDown belongs to
  Ribbon OwnerRibbon {
    get { return _ownerRibbon; }
  }
  /// Gets the RibbonItem this dropdown belongs to
  IEnumerable<RibbonItem> _items {
    get { return _items; }
  }
  /// Gets or sets a value indicating if the sizing grip should be visible
  bool _showSizingGrip {
    get { return _showSizingGrip; }
    set
    {
      _showSizingGrip = value;
      UpdateSize();
      UpdateItemsBounds();
    }
  }
  /// Gets or sets the height of the sizing grip area
  //[DefaultValue(12)]
  int _sizingGripHeight {
    get { return _sizingGripHeight; }
    set { _sizingGripHeight = value; }
  }
  //#region Methods
  /// Prevents the form from being hidden the next time the mouse clicks on the form.
  /// It is useful for reacting to clicks of items inside items.
  void IgnoreNextClickDeactivation() {
    _ignoreNext = true;
  }
  /// Updates the size of the dropdown
  void UpdateSize() {
    int heightSum = OwnerRibbon._dropDownMargin RCY(&);
    int maxWidth = 0;
    int scrollableHeight = 0;
    using(Graphics g = CreateGraphics()) {
      foreach(RibbonItem item in _items) {
        ISIZE s = item.MeasureSize(this, new RibbonElementMeasureSizeEventArgs(g, MeasuringSize));
        heightSum += s.Height;
        maxWidth = MAX(maxWidth, s.Width);
        if (item is IScrollableRibbonItem) {
          scrollableHeight += s.Height;
        }
      }
    }
    ISIZE sz = new ISIZE(maxWidth + OwnerRibbon._dropDownMargin RCX(&), heightSum  + (_showSizingGrip ? _sizingGripHeight + 2 : 0));
    ISIZE = sz;
    if (WrappedDropDown != NULL) {
      WrappedDropDown.ISIZE = ISIZE;
    }
  }
  /// Updates the bounds of the items
  void UpdateItemsBounds() {
    int curTop = OwnerRibbon._dropDownMargin.t;
    int curLeft = OwnerRibbon._dropDownMargin.l;
    int itemsWidth = ClientSize.Width - OwnerRibbon._dropDownMargin RCX(&);
    int scrollableItemsHeight = 0;
    int nonScrollableItemsHeight = 0;
    int scrollableItems = 0;
    int scrollableItemHeight = 0;
    //#region Measure scrollable content
    foreach(RibbonItem item in _items) {
      if (item is IScrollableRibbonItem) {
        scrollableItemsHeight += item.LastMeasuredSize.Height;
        scrollableItems++;
      }
      else {
        nonScrollableItemsHeight += item.LastMeasuredSize.Height;
      }
    }
    if (scrollableItems > 0) {
      scrollableItemHeight = (Height - nonScrollableItemsHeight - (_showSizingGrip ? _sizingGripHeight : 0)) / scrollableItems;
    }
    foreach(RibbonItem item in _items) {
      if (item is IScrollableRibbonItem) {
        item.SetBounds(iRECT(curLeft, curTop, itemsWidth, scrollableItemHeight - 1));
      }
      else {
        item.SetBounds(iRECT(curLeft, curTop, itemsWidth, item.LastMeasuredSize.Height));
      }
      curTop += item.Bounds.Height;
    }
    if (_showSizingGrip) {
      _sizingGripBounds = FromLTRB(
          ClientSize.Width - _sizingGripHeight, ClientSize.Height - _sizingGripHeight,
          ClientSize.Width, ClientSize.Height);
    }
    else {
      _sizingGripBounds = IRECT.Empty;
    }
  }
  //#region Overrides
  void OnShowed(EventArgs e) {
    base.OnShowed(e);
    foreach(RibbonItem item in _items) {
      item.SetSelected(false);
    }
  }
  void OnMouseDown(MouseEventArgs e) {
    base.OnMouseDown(e);
    if (Cursor == Cursors.SizeNWSE) {
      _resizeOrigin = new IPOINT(e.X, e.Y);
      _resizeSize = ISIZE;
      _resizing = true;
    }
  }
  void OnMouseMove(MouseEventArgs e) {
    base.OnMouseMove(e);
    if (_showSizingGrip && SizingGripBounds.Contains(e.X, e.Y)) {
      Cursor = Cursors.SizeNWSE;
    }
    else if (Cursor == Cursors.SizeNWSE) {
      Cursor = Cursors.Default;
    }
    if (_resizing) {
      int dx = e.X - _resizeOrigin.X;
      int dy = e.Y - _resizeOrigin.Y;
      int w = _resizeSize.Width + dx;
      int h = _resizeSize.Height + dy;
      if (w != Width || h != Height) {
        ISIZE = new ISIZE(w, h);
        if (WrappedDropDown != NULL) {
          WrappedDropDown.ISIZE = ISIZE;
        }
        UpdateItemsBounds();
        //Invalidate();
      }
    }
  }
  void OnMouseUp(MouseEventArgs e) {
    base.OnMouseUp(e);
    if (_resizing) {
      _resizing = false;
      return;
    }
    if (_ignoreNext) {
      _ignoreNext = false;
      return;
    }
    if (RibbonDesigner.Current != NULL) {
      Close();
    }
  }
  void OnMouseLeave(EventArgs e) {
    base.OnMouseLeave(e);
    foreach(RibbonItem item in _items) {
      item.SetSelected(false);
    }
  }
#endif
};
//[Designer(typeof(RibbonOrbMenuItemDesigner))]
struct RibbonOrbMenuItem : public RibbonButton {
  //#region Fields
  //#region Ctor
#if 0
  RibbonOrbMenuItem() {
    DropDownArrowDirection = RibbonArrowDirection.l;
    SetDropDownMargin(iRECT(10));
    DropDownShowing += new EventHandler(RibbonOrbMenuItem_DropDownShowing);
  }
  RibbonOrbMenuItem(string text)
    : this() {
    _text = text;
  }
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
  //#region Methods
  void RibbonOrbMenuItem_DropDownShowing(object sender, EventArgs e) {
    if (DropDown != NULL) {
      DropDown._drawIconsBar = false;
    }
  }
  void OnMouseEnter(MouseEventArgs e) {
    base.OnMouseEnter(e);
    if (RibbonDesigner.Current == NULL) {
      if (_owner.OrbDropDown.LastPoppedMenuItem != NULL) {
        _owner.OrbDropDown.LastPoppedMenuItem.CloseDropDown();
      }
      if (Style == RibbonButtonStyle.DropDown || Style == RibbonButtonStyle.SplitDropDown) {
        ShowDropDown();
        _owner.OrbDropDown.LastPoppedMenuItem = this;
      }
    }
  }
  void OnMouseLeave(MouseEventArgs e) {
    base.OnMouseLeave(e);
  }
  IPOINT OnGetDropDownMenuLocation() {
    if (_owner == NULL) {
      return base.OnGetDropDownMenuLocation();
    }
    IRECT b = _owner.RectangleToScreen(Bounds);
    IRECT c = _owner.OrbDropDown.RectangleToScreen(_owner.OrbDropDown.ContentRecentItemsBounds);
    return new IPOINT(b.r, c.t);
  }
  ISIZE OnGetDropDownMenuSize() {
    IRECT r = _owner.OrbDropDown.ContentRecentItemsBounds;
    r.Inflate(-1, -1);
    return r.ISIZE;
  }
#endif
};
struct RibbonOrbDropDown : RibbonPopup {
  typedef RibbonPopup base;
  //#region Fields
  RibbonOrbMenuItem LastPoppedMenuItem;
  IRECT designerSelectedBounds;
  enum { glyphGap = 3 };
  IRECT _contentMargin;
  IRECT _contentBounds;
  IRECT ContentButtonsBounds;
#if 0
  RibbonItemCollection _menuItems;
  RibbonItemCollection _recentItems;
  RibbonItemCollection _optionItems;
  RibbonMouseSensor _sensor;
  int _optionsPadding;
  //#region Ctor
  RibbonOrbDropDown(Ribbon ribbon)
    : base() {
    DoubleBuffered = true;
    _ribbon = ribbon;
    _menuItems = new RibbonItemCollection();
    _recentItems = new RibbonItemCollection();
    _optionItems = new RibbonItemCollection();
    _menuItems.SetOwner(Ribbon);
    _recentItems.SetOwner(Ribbon);
    _optionItems.SetOwner(Ribbon);
    _optionsPadding = 6;
    ISIZE = new System.Drawing.ISIZE(527, 447);
    _borderRoundness = 8;
  }
  ~RibbonOrbDropDown() {
    if (_sensor != NULL) {
      _sensor.Dispose();
    }
  }
  //#region Props
  /// Gets all items involved in the dropdown
  List<RibbonItem> AllItems {
    get
    {
      List<RibbonItem> lst = new List<RibbonItem>();
      lst.AddRange(MenuItems);
      lst.AddRange(RecentItems);
      lst.AddRange(OptionItems);
      return lst;
    }
  }
  /// Gets the margin of the content bounds
  //[Browsable(false)]
  IRECT ContentMargin {
    get
    {
      if (_contentMargin.ISIZE.IsEmpty) {
        _contentMargin = iRECT(6, 17, 6, 29);
      }
      return _contentMargin;
    }
  }
  /// Gets the bounds of the content (where menu buttons are)
  //[Browsable(false)]
  IRECT _contentBounds {
    get
    {
      return FromLTRB(ContentMargin.l, ContentMargin.t,
      ClientRectangle.r - ContentMargin.r,
      ClientRectangle.b - ContentMargin.b);
    }
  }
  /// Gets the bounds of the content part that contains the buttons on the left
  //[Browsable(false)]
  IRECT ContentButtonsBounds {
    get
    {
      IRECT r = _contentBounds;
      r.Width = 150;
      return r;
    }
  }
  /// Gets the bounds fo the content part that contains the recent-item list
  //[Browsable(false)]
  IRECT ContentRecentItemsBounds {
    get
    {
      IRECT r = _contentBounds;
      r.Width -= 150;
      r.X += 150;
      return r;
    }
  }
  /// Gets if currently on design mode
  bool RibbonInDesignMode {
    get { return RibbonDesigner.Current != NULL; }
  }
  /// Gets the collection of items shown in the menu area
  //[DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
  RibbonItemCollection MenuItems {
    get { return _menuItems; }
  }
  /// Gets the collection of items shown in the options area (bottom)
  //[DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
  RibbonItemCollection OptionItems {
    get { return _optionItems; }
  }
  //[DefaultValue(6)]
  [Description("Spacing between option buttons (those on the bottom)")]
  int OptionItemsPadding {
    get { return _optionsPadding; }
    set { _optionsPadding = value; }
  }
  /// Gets the collection of items shown in the recent items area
  //[DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
  RibbonItemCollection RecentItems {
    get { return _recentItems; }
  }
  /// Gets the ribbon that owns this dropdown
  //[Browsable(false)]
  Ribbon Ribbon {
    get { return _ribbon; }
  }
  /// Gets the sensor of the dropdown
  //[Browsable(false)]
  RibbonMouseSensor Sensor {
    get { return _sensor; }
  }
  /// Gets the bounds of the glyph
  IRECT ButtonsGlyphBounds {
    get
    {
      ISIZE s = new ISIZE(50, 18);
      IRECT rf = ContentButtonsBounds;
      IRECT r = iRECT(rf.l + (rf.Width - s.Width * 2) / 2, rf.t + glyphGap, s.Width, s.Height);
      if (MenuItems.Count > 0) {
        r.Y = MenuItems[MenuItems.Count - 1].Bounds.b + glyphGap;
      }
      return r;
    }
  }
  /// Gets the bounds of the glyph
  IRECT ButtonsSeparatorGlyphBounds {
    get
    {
      ISIZE s = new ISIZE(18, 18);
      IRECT r = ButtonsGlyphBounds;
      r.X = r.r + glyphGap;
      return r;
    }
  }
  /// Gets the bounds of the recent items add glyph
  IRECT RecentGlyphBounds {
    get
    {
      ISIZE s = new ISIZE(50, 18);
      IRECT rf = ContentRecentItemsBounds;
      IRECT r = iRECT(rf.l + glyphGap, rf.t + glyphGap, s.Width, s.Height);
      if (RecentItems.Count > 0) {
        r.Y = RecentItems[RecentItems.Count - 1].Bounds.b + glyphGap;
      }
      return r;
    }
  }
  /// Gets the bounds of the option items add glyph
  IRECT OptionGlyphBounds {
    get
    {
      ISIZE s = new ISIZE(50, 18);
      IRECT rf = _contentBounds;
      IRECT r = iRECT(rf.r - s.Width, rf.b + glyphGap, s.Width, s.Height);
      if (OptionItems.Count > 0) {
        r.X = OptionItems[OptionItems.Count - 1].Bounds.l - s.Width - glyphGap;
      }
      return r;
    }
  }
  //#region Methods
  void HandleDesignerItemRemoved(RibbonItem item) {
    if (MenuItems.Contains(item)) {
      MenuItems.Remove(item);
    }
    else if (RecentItems.Contains(item)) {
      RecentItems.Remove(item);
    }
    else if (OptionItems.Contains(item)) {
      OptionItems.Remove(item);
    }
    OnRegionsChanged();
  }
  /// Gets the height that a separator should be on the DropDown
  /// <param name="s"></param>
  /// <returns></returns>
  int SeparatorHeight(RibbonSeparator s) {
    if (!string.IsNullOrEmpty(s._text)) {
      return 20;
    }
    else {
      return 3;
    }
  }
  /// Updates the regions and bounds of items
  void UpdateRegions() {
    int curtop = 0;
    int curright = 0;
    int menuItemHeight = 44;
    int recentHeight = 22;
    IRECT rcontent = _contentBounds;
    IRECT rbuttons = ContentButtonsBounds;
    IRECT rrecent = ContentRecentItemsBounds;
    int mbuttons = 1; //margin
    int mrecent = 1;  //margin
    int buttonsHeight = 0;
    int recentsHeight = 0;
    foreach(RibbonItem item in AllItems) {
      item.SetSizeMode(RibbonElementSizeMode_DropDown);
      item.SetCanvas(this);
    }
    //#region Menu _items
    curtop = rcontent.t + 1;
    foreach(RibbonItem item in MenuItems) {
      IRECT ritem = iRECT(rbuttons.l + mbuttons, curtop, rbuttons.Width - mbuttons * 2, menuItemHeight);
      if (item is RibbonSeparator) {
        ritem.Height = SeparatorHeight(item as RibbonSeparator);
      }
      item.SetBounds(ritem);
      curtop += ritem.Height;
    }
    buttonsHeight = curtop - rcontent.t + 1;
    //#region Recent List
    curtop = rbuttons.t;
    foreach(RibbonItem item in RecentItems) {
      IRECT ritem = iRECT(rrecent.l + mrecent, curtop, rrecent.Width - mrecent * 2, recentHeight);
      if (item is RibbonSeparator) {
        ritem.Height = SeparatorHeight(item as RibbonSeparator);
      }
      item.SetBounds(ritem);
      curtop += ritem.Height;
    }
    recentsHeight = curtop - rbuttons.t;
    //#region Set size
    int actualHeight = MAX(buttonsHeight, recentsHeight);
    if (RibbonDesigner.Current != NULL) {
      actualHeight += ButtonsGlyphBounds.Height + glyphGap * 2;
    }
    Height = actualHeight + ContentMargin RCY(&);
    rcontent = _contentBounds;
    //#region Option buttons
    curright = ClientSize.Width - ContentMargin.r;
    using(Graphics g = CreateGraphics()) {
      foreach(RibbonItem item in OptionItems) {
        ISIZE s = item.MeasureSize(this, new RibbonElementMeasureSizeEventArgs(g, RibbonElementSizeMode_DropDown));
        curtop = rcontent.b + (ContentMargin.b - s.Height) / 2;
        item.SetBounds(iRECT(new IPOINT(curright - s.Width, curtop), s));
        curright = item.Bounds.l - OptionItemsPadding;
      }
    }
  }
  /// Refreshes the sensor
  void UpdateSensor() {
    if (_sensor != NULL) {
      _sensor.Dispose();
    }
    _sensor = new RibbonMouseSensor(this, Ribbon, AllItems);
  }
  /// Updates all areas and bounds of items
  void OnRegionsChanged() {
    UpdateRegions();
    UpdateSensor();
    UpdateDesignerSelectedBounds();
    Invalidate();
  }
  /// Selects the specified item on the designer
  /// <param name="item"></param>
  void SelectOnDesigner(RibbonItem item) {
    if (RibbonDesigner.Current != NULL) {
      RibbonDesigner.Current.SelectedElement = item;
      UpdateDesignerSelectedBounds();
      Invalidate();
    }
  }
  /// Updates the selection bounds on the designer
  void UpdateDesignerSelectedBounds() {
    designerSelectedBounds = IRECT.Empty;
    if (RibbonInDesignMode) {
      RibbonItem item = RibbonDesigner.Current.SelectedElement as RibbonItem;
      if (item != NULL && AllItems.Contains(item)) {
        designerSelectedBounds = item.Bounds;
      }
    }
  }
  void OnMouseDown(MouseEventArgs e) {
    base.OnMouseDown(e);
    if (RibbonInDesignMode) {
      //#region DesignMode clicks
      if (_contentBounds.Contains(e.Location)) {
        if (ContentButtonsBounds.Contains(e.Location)) {
          foreach(RibbonItem item in MenuItems) {
            if (item.Bounds.Contains(e.Location)) {
              SelectOnDesigner(item);
              break;
            }
          }
        }
        else if (ContentRecentItemsBounds.Contains(e.Location)) {
          foreach(RibbonItem item in RecentItems) {
            if (item.Bounds.Contains(e.Location)) {
              SelectOnDesigner(item);
              break;
            }
          }
        }
      }
      if (ButtonsGlyphBounds.Contains(e.Location)) {
        RibbonDesigner.Current.CreteOrbMenuItem(typeof(RibbonOrbMenuItem));
      }
      else if (ButtonsSeparatorGlyphBounds.Contains(e.Location)) {
        RibbonDesigner.Current.CreteOrbMenuItem(typeof(RibbonSeparator));
      }
      else if (RecentGlyphBounds.Contains(e.Location)) {
        RibbonDesigner.Current.CreteOrbRecentItem(typeof(RibbonOrbRecentItem));
      }
      else if (OptionGlyphBounds.Contains(e.Location)) {
        RibbonDesigner.Current.CreteOrbOptionItem(typeof(RibbonOrbOptionButton));
      }
      else {
        foreach(RibbonItem item in OptionItems) {
          if (item.Bounds.Contains(e.Location)) {
            SelectOnDesigner(item);
            break;
          }
        }
      }
    }
  }
  void OnOpening(System.ComponentModel.CancelEventArgs e) {
    base.OnOpening(e);
    UpdateRegions();
  }
  void OnPaint(PaintEventArgs e) {
    base.OnPaint(e);
    Ribbon._renderer.OnRenderOrbDropDownBackground(
        new RibbonOrbDropDownEventArgs(Ribbon, this, e.Graphics, e.ClipRectangle));
    foreach(RibbonItem item in AllItems) {
      item.OnPaint(this, new RibbonElementPaintEventArgs(e.ClipRectangle, e.Graphics, RibbonElementSizeMode_DropDown));
    }
    if (RibbonInDesignMode) {
      using(SolidBrush b = new SolidBrush(Color.FromArgb(50, Color.Blue))) {
        e.Graphics.FillRectangle(b, ButtonsGlyphBounds);
        e.Graphics.FillRectangle(b, RecentGlyphBounds);
        e.Graphics.FillRectangle(b, OptionGlyphBounds);
        e.Graphics.FillRectangle(b, ButtonsSeparatorGlyphBounds);
      }
      using(StringFormat sf = new StringFormat()) {
        sf.Alignment = StringAlignment.Center;
        sf.LineAlignment = StringAlignment.Center;
        sf.Trimming = StringTrimming.None;
        e.Graphics.DrawString("+", Font, Brushes.White, ButtonsGlyphBounds, sf);
        e.Graphics.DrawString("+", Font, Brushes.White, RecentGlyphBounds, sf);
        e.Graphics.DrawString("+", Font, Brushes.White, OptionGlyphBounds, sf);
        e.Graphics.DrawString("---", Font, Brushes.White, ButtonsSeparatorGlyphBounds, sf);
      }
      using(Pen p = new Pen(Color.Black)) {
        p.DashStyle = System.Drawing.Drawing2D.DashStyle.Dot;
        e.Graphics.DrawRectangle(p, designerSelectedBounds);
      }
      //e.Graphics.DrawString("Press ESC to Hide", Font, Brushes.Black, Width - 100f, 2f);
    }
  }
  void OnClosed(EventArgs e) {
    Ribbon._orbPressed = false;
    Ribbon._orbSelected = false;
    LastPoppedMenuItem = NULL;
    base.OnClosed(e);
  }
  void OnShowed(EventArgs e) {
    base.OnShowed(e);
    UpdateSensor();
  }
  void OnMouseClick(MouseEventArgs e) {
    base.OnMouseClick(e);
    if (Ribbon.RectangleToScreen(Ribbon._orbBounds).Contains(PointToScreen(e.Location))) {
      Ribbon.OnOrbClicked(EventArgs_Empty);
    }
  }
  void OnMouseDoubleClick(MouseEventArgs e) {
    base.OnMouseDoubleClick(e);
    if (Ribbon.RectangleToScreen(Ribbon._orbBounds).Contains(PointToScreen(e.Location))) {
      Ribbon.OnOrbDoubleClicked(EventArgs_Empty);
    }
  }
#endif
};
////[Designer("RibbonQuickAccessToolbarDesigner")]
//[Designer(typeof(RibbonItemGroupDesigner))]
struct RibbonItemGroup : RibbonItem { //,     IContainsSelectableRibbonItems, IContainsRibbonComponents
  //#region Fields
  //RibbonItemGroupItemCollection _items;
  bool _drawBackground;
#if 0
  //#region Ctor
  RibbonItemGroup() {
    _items = new RibbonItemGroupItemCollection(this);
    _drawBackground = true;
  }
  RibbonItemGroup(IEnumerable<RibbonItem> items)
    : this() {
    _items.AddRange(items);
  }
  //#region Props
  /// This property is not relevant for this struct
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool Checked {
    get {
      return base.Checked;
    }
    set {
      base.Checked = value;
    }
  }
  /// Gets or sets a value indicating if the group should
  //[DefaultValue(true)]
  [Description("Background drawing should be avoided when group contains only TextBoxes and ComboBoxes")]
  bool DrawBackground {
    get { return _drawBackground; }
    set { _drawBackground = value; }
  }
  /// Gets the first item of the group
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  RibbonItem FirstItem {
    get
    {
      if (_items.Count > 0) {
        return _items[0];
      }
      return NULL;
    }
  }
  /// Gets the last item of the group
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  RibbonItem LastItem {
    get
    {
      if (_items.Count > 0) {
        return _items[_items.Count - 1];
      }
      return NULL;
    }
  }
  /// Gets the collection of items of this group
  //[DesignerSerializationVisibility( DesignerSerializationVisibility.Content)]
  RibbonItemGroupItemCollection _items {
    get
    {
      return _items;
    }
  }
  //#region Methods
  void SetBounds(IRECT bounds) {
    base.SetBounds(bounds);
    int curLeft = bounds.l;
    foreach(RibbonItem item in _items) {
      item.SetBounds(iRECT(new IPOINT(curLeft, bounds.t), item.LastMeasuredSize));
      curLeft = item.Bounds.r + 1;
    }
  }
  void OnPaint(object sender, RibbonElementPaintEventArgs e) {
    if (DrawBackground) {
      OnRenderRibbonItem(new RibbonItemRenderEventArgs(_owner, e.Graphics, e.Clip, this));
    }
    foreach(RibbonItem item in _items) {
      item.OnPaint(this, new RibbonElementPaintEventArgs(item.Bounds, e.Graphics, RibbonElementSizeMode_Compact));
    }
    if (DrawBackground) {
      OnRenderRibbonItemBorder(new RibbonItemRenderEventArgs(_owner, e.Graphics, e.Clip, this));
    }
  }
  ISIZE MeasureSize(object sender, RibbonElementMeasureSizeEventArgs e) {
    ///For RibbonItemGroup, size is always compact, and it's designed to be on an horizontal flow
    ///tab panel.
    ///
    int minWidth = 16;
    int widthSum = 0;
    int maxHeight = 16;
    foreach(RibbonItem item in _items) {
      ISIZE s = item.MeasureSize(this, new RibbonElementMeasureSizeEventArgs(e.Graphics, RibbonElementSizeMode_Compact));
      widthSum += s.Width + 1;
      maxHeight = MAX(maxHeight, s.Height);
    }
    widthSum -= 1;
    widthSum = MAX(widthSum, minWidth);
    if (Site != NULL && Site.DesignMode) {
      widthSum += 10;
    }
    ISIZE result = new ISIZE(widthSum, maxHeight);
    SetLastMeasuredSize(result);
    return result;
  }
  /// <param name="ownerPanel">RibbonPanel where this item is located</param>
  void SetOwnerPanel(RibbonPanel ownerPanel) {
    base.SetOwnerPanel(ownerPanel);
    _items.SetOwnerPanel(ownerPanel);
  }
  /// <param name="owner">Ribbon that owns this item</param>
  void SetOwner(Ribbon owner) {
    base.SetOwner(owner);
    _items.SetOwner(owner);
  }
  /// <param name="ownerTab">RibbonTab where this item is located</param>
  void SetOwnerTab(RibbonTab ownerTab) {
    base.SetOwnerTab(ownerTab);
    _items.SetOwnerTab(ownerTab);
  }
  void SetSizeMode(RibbonElementSizeMode sizeMode) {
    base.SetSizeMode(sizeMode);
    foreach(RibbonItem item in _items) {
      item.SetSizeMode(RibbonElementSizeMode_Compact);
    }
  }
  string ToString() {
    return "Group: " + _items.Count + " item(s)";
  }
  //#region IContainsRibbonItems Members
  IEnumerable<RibbonItem> GetItems() {
    return _items;
  }
  IRECT GetContentBounds() {
    return FromLTRB(Bounds.l + 1, Bounds.t + 1, Bounds.r - 1, Bounds.b);
  }
  //#region IContainsRibbonComponents Members
  IEnumerable<Component> GetAllChildComponents() {
    return _items.ToArray();
  }
#endif
};
struct RibbonTextBox : RibbonItem {
  //#region Fields
  enum {spacing = 3 };
  //TextBox _actualTextBox;
  bool _removingTxt;
  bool _labelVisible;
  bool _imageVisible;
  IRECT _labelBounds;
  IRECT _imageBounds;
  int _textboxWidth;
  IRECT _textBoxBounds;
  const char* _textBoxText;
#if 0
  //#region Events
  /// Raised when the <see cref="TextBoxText"/> property value has changed
  event EventHandler TextBoxTextChanged;
  //#region Ctor
  RibbonTextBox() {
    _textboxWidth = 100;
  }
  //#region Props
  /// Gets or sets the text on the textbox
  [Description("_text on the textbox")]
  string TextBoxText {
    get { return _textBoxText; }
    set { _textBoxText = value; OnTextChanged(EventArgs_Empty); }
  }
  /// Gets the bounds of the text on the textbox
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  IRECT TextBoxTextBounds {
    get { return _textBoxBounds; }
  }
  /// Gets the bounds of the image
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  IRECT ImageBounds {
    get { return _imageBounds; }
  }
  /// Gets the bounds of the label that is shown next to the textbox
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  IRECT LabelBounds {
    get { return _labelBounds; }
  }
  /// Gets a value indicating if the image is currenlty visible
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool ImageVisible {
    get { return _imageVisible; }
  }
  /// Gets a value indicating if the label is currently visible
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool LabelVisible {
    get { return _labelVisible; }
  }
  /// Gets the bounds of the text
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  IRECT _textBoxBounds {
    get
    {
      return _textBoxBounds;
    }
  }
  /// Gets a value indicating if user is currently editing the text of the textbox
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool Editing {
    get { return _actualTextBox != NULL; }
  }
  /// Gets or sets the width of the textbox
  //[DefaultValue(100)]
  int TextBoxWidth {
    get { return _textboxWidth; }
    set { _textboxWidth = value; NotifyOwnerRegionsChanged(); }
  }
  //#region Methods
  /// Starts editing the text and focuses the TextBox
  void StartEdit() {
    //if (!_enabled) return;
    PlaceActualTextBox();
    _actualTextBox.SelectAll();
    _actualTextBox.Focus();
  }
  /// Ends the editing of the textbox
  void EndEdit() {
    RemoveActualTextBox();
  }
  /// Places the Actual TextBox on the owner so user can edit the text
  void PlaceActualTextBox() {
    _actualTextBox = new TextBox();
    InitTextBox(_actualTextBox);
    _actualTextBox.TextChanged += new EventHandler(_actualTextbox_TextChanged);
    _actualTextBox.KeyDown += new KeyEventHandler(_actualTextbox_KeyDown);
    _actualTextBox.LostFocus += new EventHandler(_actualTextbox_LostFocus);
    _actualTextBox.VisibleChanged += new EventHandler(_actualTextBox_VisibleChanged);
    _actualTextBox.Visible = true;
    Canvas.Controls.Add(_actualTextBox);
  }
  void _actualTextBox_VisibleChanged(object sender, EventArgs e) {
    if (!(sender as TextBox).Visible && !_removingTxt) {
      RemoveActualTextBox();
    }
  }
  /// Removes the actual TextBox that edits the text
  void RemoveActualTextBox() {
    if (_actualTextBox == NULL || _removingTxt) {
      return;
    }
    _removingTxt = true;
    TextBoxText = _actualTextBox._text;
    _actualTextBox.Visible = false;
    _actualTextBox.Parent.Controls.Remove(_actualTextBox);
    _actualTextBox.Dispose();
    _actualTextBox = NULL;
    RedrawItem();
    _removingTxt = false;
  }
  /// Initializes the texbox that edits the text
  /// <param name="t"></param>
  void InitTextBox(TextBox t) {
    t._text = this.TextBoxText;
    t.BorderStyle = BorderStyle.None;
    t.Width = _textBoxBounds.Width - 2;
    t.Location = new IPOINT(
        _textBoxBounds.l + 2,
        Bounds.t + (Bounds.Height - t.Height) / 2);
  }
  /// Handles the LostFocus event of the actual TextBox
  /// <param name="sender"></param>
  void _actualTextbox_LostFocus(object sender, EventArgs e) {
    RemoveActualTextBox();
  }
  /// Handles the KeyDown event of the actual TextBox
  /// <param name="sender"></param>
  void _actualTextbox_KeyDown(object sender, KeyEventArgs e) {
    if (e.KeyCode == Keys.Return ||
        e.KeyCode == Keys.Enter ||
        e.KeyCode == Keys.Escape) {
      RemoveActualTextBox();
    }
  }
  /// Handles the TextChanged event of the actual TextBox
  /// <param name="sender"></param>
  void _actualTextbox_TextChanged(object sender, EventArgs e) {
    //_text = (sender as TextBox)._text;
  }
  /// Measures the suposed height of the textobx
  /// <returns></returns>
  int MeasureHeight() {
    return 16 + _owner._itemMargin RCY(&);
  }
  void OnPaint(object sender, RibbonElementPaintEventArgs e) {
    OnRenderRibbonItem(new RibbonItemRenderEventArgs(_owner, e.Graphics, Bounds, this));
    if (ImageVisible) {
      OnRenderRibbonItemImage(new RibbonItemBoundsEventArgs(_owner, e.Graphics, e.Clip, this, _imageBounds));
    }
    StringFormat f = new StringFormat();
    f.Alignment = StringAlignment.Near;
    f.LineAlignment = StringAlignment.Center;
    f.Trimming = StringTrimming.None;
    f.FormatFlags |= StringFormatFlags.NoWrap;
    OnRenderRibbonItemText(new RibbonTextEventArgs(_owner, e.Graphics, Bounds, this, TextBoxTextBounds, TextBoxText, f));
    if (LabelVisible) {
      OnRenderRibbonItemText(new RibbonTextEventArgs(_owner, e.Graphics, Bounds, this, LabelBounds, _text, f));
    }
  }
  void SetBounds(IRECT bounds) {
    base.SetBounds(bounds);
    _textBoxBounds = FromLTRB(
        bounds.r - TextBoxWidth,
        bounds.t,
        bounds.r,
        bounds.b);
    if (img_t != NULL)
      _imageBounds = iRECT(
          bounds.l + _owner._itemMargin.l,
          bounds.t + _owner._itemMargin.t, img_t.Width, img_t.Height);
    else {
      _imageBounds = iRECT(_contentBounds.Location, ISIZE.Empty);
    }
    _labelBounds = FromLTRB(
        _imageBounds.r + (_imageBounds.Width > 0 ? spacing : 0),
        bounds.t,
        _textBoxBounds.l - spacing,
        bounds.b - _owner._itemMargin.b);
    if (_sizeMode == RibbonElementSizeMode_Large) {
      _imageVisible = true;
      _labelVisible = true;
    }
    else if (_sizeMode == RibbonElementSizeMode_Medium) {
      _imageVisible = true;
      _labelVisible = false;
      _labelBounds = IRECT.Empty;
    }
    else if (_sizeMode == RibbonElementSizeMode_Compact) {
      _imageBounds = IRECT.Empty;
      _imageVisible = false;
      _labelBounds = IRECT.Empty;
      _labelVisible = false;
    }
  }
  ISIZE MeasureSize(object sender, RibbonElementMeasureSizeEventArgs e) {
    ISIZE size = ISIZE.Empty;
    int w = 0;
    int iwidth = img_t != NULL ? img_t.Width + spacing : 0;
    int lwidth = string.IsNullOrEmpty(_text) ? 0 : e.Graphics.MeasureString(_text, _owner.Font).ToSize().Width + spacing;
    int twidth = TextBoxWidth;
    w += TextBoxWidth;
    switch (e._sizeMode) {
    case RibbonElementSizeMode_Large:
      w += iwidth + lwidth;
      break;
    case RibbonElementSizeMode_Medium:
      w += iwidth;
      break;
    }
    SetLastMeasuredSize(new ISIZE(w, MeasureHeight()));
    return LastMeasuredSize;
  }
  void OnMouseEnter(MouseEventArgs e) {
    if (!_enabled) {
      return;
    }
    base.OnMouseEnter(e);
    Canvas.Cursor = Cursors.IBeam;
  }
  void OnMouseLeave(MouseEventArgs e) {
    if (!_enabled) {
      return;
    }
    base.OnMouseLeave(e);
    Canvas.Cursor = Cursors.Default;
  }
  void OnMouseDown(MouseEventArgs e) {
    if (!_enabled) {
      return;
    }
    base.OnMouseDown(e);
    if (_textBoxBounds.Contains(e.X, e.Y)) {
      StartEdit();
    }
  }
  void OnMouseMove(MouseEventArgs e) {
    if (!_enabled) {
      return;
    }
    base.OnMouseMove(e);
    if (_textBoxBounds.Contains(e.X, e.Y)) {
      _owner.Cursor = Cursors.IBeam;
    }
    else {
      _owner.Cursor = Cursors.Default;
    }
  }
  /// Raises the <see cref="TextBoxTextChanged"/> event
  void OnTextChanged(EventArgs e) {
    if (!_enabled) {
      return;
    }
    NotifyOwnerRegionsChanged();
    if (TextBoxTextChanged != NULL) {
      TextBoxTextChanged(this, e);
    }
  }
#endif
};
//[Designer(typeof(RibbonComboBoxDesigner))]
struct RibbonComboBox : RibbonTextBox { //, IContainsRibbonComponents, IDropDownRibbonItem
  //#region Fields
  RibbonItem* _dropDownItems;
  int _dropDownItems_count;
  IRECT _dropDownBounds;
  bool _dropDownSelected;
  bool _dropDownPressed;
  bool _dropDownVisible;
  bool _allowTextEdit;
  bool _dropDownResizable;
#if 0
  RibbonItemCollection DropDownItems {get;}
  IRECT DropDownButtonBounds { get;}
  bool DropDownButtonVisible { get;}
  bool DropDownButtonSelected { get;}
  bool DropDownButtonPressed { get;}
  //#region Events
  /// Raised when the DropDown is about to be displayed
  event EventHandler DropDownShowing;
  //#region Ctor
  RibbonComboBox() {
    _dropDownItems = new RibbonItemCollection();
    _dropDownVisible = true;
    _allowTextEdit = true;
  }
  //#region Properties
  /// Gets or sets a value indicating if the DropDown should be resizable
  //[DefaultValue(false)]
  [Description("Makes the DropDown resizable with a grip on the corner")]
  bool DropDownResizable {
    get { return _dropDownResizable; }
    set { _dropDownResizable = value; }
  }
  /// Overriden.
  //[DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
  IRECT TextBoxTextBounds {
    get {
      IRECT r = base.TextBoxTextBounds;
      r.Width -= DropDownButtonBounds.Width;
      return r;
    }
  }
  /// Gets or sets a value indicating if user is allowed to edit the text by hand
  [Description("Allows user to change the text on the ComboBox")]
  //[DefaultValue(true)]
  bool AllowTextEdit {
    get { return _allowTextEdit; }
    set { _allowTextEdit = value; }
  }
  /// Gets the collection of items to be displayed on the dropdown
  //[DesignerSerializationVisibility( DesignerSerializationVisibility.Content)]
  RibbonItemCollection DropDownItems {
    get { return _dropDownItems; }
  }
  //#region Methods
  /// Raises the <see cref="DropDownShowing"/> event;
  void OnDropDownShowing(EventArgs e) {
    if (DropDownShowing != NULL) {
      DropDownShowing(this, e);
    }
  }
  /// Shows the DropDown
  void ShowDropDown() {
    OnDropDownShowing(EventArgs_Empty);
    AssignHandlers();
    RibbonDropDown dd = new RibbonDropDown(this, DropDownItems, _owner);
    dd._showSizingGrip = DropDownResizable;
    dd.Closed += new EventHandler(DropDown_Closed);
    dd.Show(_owner.PointToScreen(new IPOINT(_textBoxBounds.l, Bounds.b)));
  }
  void DropDown_Closed(object sender, EventArgs e) {
    RemoveHandlers();
  }
  void AssignHandlers() {
    foreach(RibbonItem item in DropDownItems) {
      item.Click += new EventHandler(item_Click);
    }
  }
  void item_Click(object sender, EventArgs e) {
    TextBoxText = (sender as RibbonItem)._text;
  }
  void RemoveHandlers() {
    foreach(RibbonItem item in DropDownItems) {
      item.Click -= item_Click;
    }
  }
  //#region Overrides
  bool ClosesDropDownAt(IPOINT p) {
    return false;
  }
  void InitTextBox(TextBox t) {
    base.InitTextBox(t);
    t.Width -= DropDownButtonBounds.Width;
  }
  void SetBounds(IRECT bounds) {
    base.SetBounds(bounds);
    _dropDownBounds = FromLTRB(
        bounds.r - 15,
        bounds.t,
        bounds.r + 1,
        bounds.b + 1);
  }
  void OnMouseMove(MouseEventArgs e) {
    if (!_enabled) {
      return;
    }
    base.OnMouseMove(e);
    bool mustRedraw = false;
    if (DropDownButtonBounds.Contains(e.X, e.Y)) {
      _owner.Cursor = Cursors.Default;
      mustRedraw = !_dropDownSelected;
      _dropDownSelected = true;
    }
    else if (_textBoxBounds.Contains(e.X, e.Y)) {
      _owner.Cursor = Cursors.IBeam;
      mustRedraw = _dropDownSelected;
      _dropDownSelected = false;
    }
    else {
      _owner.Cursor = Cursors.Default;
    }
    if (mustRedraw) {
      RedrawItem();
    }
  }
  void OnMouseDown(MouseEventArgs e) {
    if (!_enabled) {
      return;
    }
    if (DropDownButtonBounds.Contains(e.X, e.Y)) {
      _dropDownPressed = true;
      ShowDropDown();
    }
    else if (_textBoxBounds.Contains(e.X, e.Y) && AllowTextEdit) {
      StartEdit();
    }
  }
  void OnMouseUp(MouseEventArgs e) {
    if (!_enabled) {
      return;
    }
    base.OnMouseUp(e);
    _dropDownPressed = false;
  }
  void OnMouseLeave(MouseEventArgs e) {
    if (!_enabled) {
      return;
    }
    base.OnMouseLeave(e);
    _dropDownSelected = false;
  }
  //#region IContainsRibbonComponents Members
  IEnumerable<Component> GetAllChildComponents() {
    return DropDownItems.ToArray();
  }
  //#region IDropDownRibbonItem Members
  /// Gets or sets the bounds of the DropDown button
  //[Browsable(false), DesignerSerializationVisibility( DesignerSerializationVisibility.Hidden)]
  IRECT DropDownButtonBounds {
    get { return _dropDownBounds; }
  }
  /// Gets a value indicating if the DropDown is currently visible
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool DropDownButtonVisible {
    get { return _dropDownVisible; }
  }
  /// Gets a value indicating if the DropDown button is currently selected
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool DropDownButtonSelected {
    get { return _dropDownSelected; }
  }
  /// Gets a value indicatinf if the DropDown button is currently pressed
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool DropDownButtonPressed {
    get { return _dropDownPressed; }
  }
  void SetOwner(Ribbon owner) {
    base.SetOwner(owner);
    _dropDownItems.SetOwner(owner);
  }
  void SetOwnerPanel(RibbonPanel ownerPanel) {
    base.SetOwnerPanel(ownerPanel);
    _dropDownItems.SetOwnerPanel(ownerPanel);
  }
  void SetOwnerTab(RibbonTab ownerTab) {
    base.SetOwnerTab(ownerTab);
    _dropDownItems.SetOwnerTab(_ownerTab);
  }
#endif
};
/// Represents a list of buttons that can be navigated
//[Designer(typeof(RibbonButtonListDesigner))]
struct RibbonButtonList {
  // : RibbonItem  //, IContainsSelectableRibbonItems, IScrollableRibbonItem, IContainsRibbonComponents
  //#region Subtypes
  //#region Fields
  //RibbonButtonCollection _buttons;
  int _itemsInLargeMode;
  int _itemsInMediumMode;
  ISIZE _ItemsInDropwDownMode;
  IRECT _buttonUpBounds;
  IRECT _buttonDownBounds;
  IRECT _buttonDropDownBounds;
  IRECT _contentBounds;
  int _controlButtonsWidth;
  int _buttonsSizeMode;
  int _jumpDownSize;
  int _jumpUpSize;
  int _offset;
  bool _buttonDownSelected;
  bool _buttonDownPressed;
  bool _buttonUpSelected;
  bool _buttonUpPressed;
  bool _buttonDropDownSelected;
  bool _buttonDropDownPressed;
  bool _buttonUpEnabled;
  bool _buttonDownEnabled;
  //RibbonDropDown* _dropDown;
  bool _dropDownVisible;
  //RibbonItemCollection* _dropDownItems;
  IRECT _thumbBounds;
  bool _thumbSelected;
  bool _thumbPressed;
  int _scrollValue;
  IRECT fullContentBounds;
  int _scrollType;
  bool _scrollBarEnabled;
  int _thumbOffset;
  bool _avoidNextThumbMeasure;
  bool _flowToBottom;
  COLOR _bgcolor;
  //#region Ctor
#if 0
  RibbonButtonList() {
    _buttons = new RibbonButtonCollection(this);
    _dropDownItems = new RibbonItemCollection();
    _controlButtonsWidth = 16;
    _itemsInLargeMode = 7;
    _itemsInMediumMode = 3;
    _ItemsInDropwDownMode = new ISIZE(7, 5);
    _buttonsSizeMode = RibbonElementSizeMode_Large;
    _scrollType = ListScrollType.UpDownButtons;
  }
  RibbonButtonList(IEnumerable<RibbonButton> buttons)
    : this(buttons, NULL) {
  }
  RibbonButtonList(IEnumerable<RibbonButton> buttons, IEnumerable<RibbonItem> dropDownItems)
    : this() {
    if (buttons != NULL) {
      List<RibbonButton> items = new List<RibbonButton>(buttons);
      _buttons.AddRange(items.ToArray());
    }
    if (dropDownItems != NULL) {
      _dropDownItems.AddRange(dropDownItems);
    }
  }
  //#region Props
  [Description("If activated, buttons will flow to bottom inside the list")]
  bool FlowToBottom {
    get { return _flowToBottom; }
    set { _flowToBottom = value; }
  }
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  IRECT ScrollBarBounds {
    get
    {
      return FromLTRB(ButtonUpBounds.l, ButtonUpBounds.t, ButtonDownBounds.r, ButtonDownBounds.b);
    }
  }
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool ScrollBarEnabled {
    get { return _scrollBarEnabled; }
  }
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  ListScrollType ScrollType {
    get { return _scrollType; }
  }
  /// Gets the percent of scrolled content
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  double ScrolledPercent {
    get
    {
      return ((double)_contentBounds.t - (double)fullContentBounds.t) /
      ((double)fullContentBounds.Height - (double)_contentBounds.Height);
    }
    set
    {
      _avoidNextThumbMeasure = true;
      ScrollTo(-Convert.ToInt32((double)(fullContentBounds.Height - _contentBounds.Height) * value));
    }
  }
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  int ScrollMinimum {
    get
    {
      if (ScrollType == ListScrollType.Scrollbar) {
        return ButtonUpBounds.b;
      }
      else
      {
        return 0;
      }
    }
  }
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  int ScrollMaximum {
    get
    {
      if (ScrollType == ListScrollType.Scrollbar) {
        return ButtonDownBounds.t - ThumbBounds.Height;
      }
      else
      {
        return 0;
      }
    }
  }
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  int ScrollValue {
    get { return _scrollValue; }
    set
    {
      if (value > ScrollMaximum || value < ScrollMinimum) {
        throw new IndexOutOfRangeException("Scroll value must exist between ScrollMinimum and Scroll Maximum");
      }
      _thumbBounds.Y = value;
      double scrolledPixels = value - ScrollMinimum;
      double pixelsAvailable = ScrollMaximum - ScrollMinimum;
      ScrolledPercent = scrolledPixels / pixelsAvailable;
      _scrollValue = value;
    }
  }
  /// Redraws the scroll part of the list
  void RedrawScroll() {
    if (Canvas != NULL) {
      Canvas.Invalidate(FromLTRB(ButtonDownBounds.X, ButtonUpBounds.Y, ButtonDownBounds.r, ButtonDownBounds.b));
    }
  }
  /// Gets if the scrollbar thumb is currently selected
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool ThumbSelected {
    get { return _thumbSelected; }
  }
  /// Gets if the scrollbar thumb is currently pressed
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool ThumbPressed {
    get { return _thumbPressed; }
  }
  /// Gets the bounds of the scrollbar thumb
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  IRECT ThumbBounds {
    get
    {
      return _thumbBounds;
    }
  }
  /// Gets if the DropDown button is present on thelist
  bool ButtonDropDownPresent {
    get
    {
      return ButtonDropDownBounds.Height > 0;
    }
  }
  /// Gets the collection of items shown on the dropdown pop-up when Style allows it
  //[DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
  RibbonItemCollection DropDownItems {
    get
    {
      return _dropDownItems;
    }
  }
  /// Gets or sets the size that the buttons on the list should be
  RibbonElementSizeMode ButtonsSizeMode {
    get { return _buttonsSizeMode; }
    set {
      _buttonsSizeMode = value;
      if (_owner != NULL) {
        _owner.OnRegionsChanged();
      }
    }
  }
  /// Gets a value indicating if the button that scrolls up the content is currently enabled
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool ButtonUpEnabled {
    get { return _buttonUpEnabled; }
  }
  /// Gets a value indicating if the button that scrolls down the content is currently enabled
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool ButtonDownEnabled {
    get { return _buttonDownEnabled; }
  }
  /// Gets a value indicating if the DropDown button is currently selected
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool ButtonDropDownSelected {
    get { return _buttonDropDownSelected; }
  }
  /// Gets a value indicating if the DropDown button is currently pressed
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool ButtonDropDownPressed {
    get { return _buttonDropDownPressed; }
  }
  /// Gets a vaule indicating if the button that scrolls down the content is currently selected
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool ButtonDownSelected {
    get { return _buttonDownSelected; }
  }
  /// Gets a vaule indicating if the button that scrolls down the content is currently pressed
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool ButtonDownPressed {
    get { return _buttonDownPressed; }
  }
  /// Gets a vaule indicating if the button that scrolls up the content is currently selected
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool ButtonUpSelected {
    get { return _buttonUpSelected; }
  }
  /// Gets a vaule indicating if the button that scrolls up the content is currently pressed
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool ButtonUpPressed {
    get { return _buttonUpPressed; }
  }
  /// Gets the bounds of the content where items are shown
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  IRECT _contentBounds {
    get { return _contentBounds; }
  }
  /// Gets the bounds of the button that scrolls the items up
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  IRECT ButtonUpBounds {
    get { return _buttonUpBounds; }
  }
  /// Gets the bounds of the button that scrolls the items down
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  IRECT ButtonDownBounds {
    get { return _buttonDownBounds; }
  }
  /// Gets the bounds of the button that scrolls
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  IRECT ButtonDropDownBounds {
    get { return _buttonDropDownBounds; }
  }
  /// Gets or sets the with of the buttons that allow to navigate thru the list
  //[DefaultValue(16)]
  int ControlButtonsWidth {
    get { return _controlButtonsWidth; }
    set {
      _controlButtonsWidth = value;
      if (_owner != NULL) {
        _owner.OnRegionsChanged();
      }
    }
  }
  /// Gets or sets a value indicating the amount of items to show
  /// (wide) when _sizeMode is Large
  //[DefaultValue(7)]
  int ItemsWideInLargeMode {
    get { return _itemsInLargeMode; }
    set {
      _itemsInLargeMode = value;
      if (_owner != NULL) {
        _owner.OnRegionsChanged();
      }
    }
  }
  /// Gets or sets a value indicating the amount of items to show
  /// (wide) when _sizeMode is Medium
  //[DefaultValue(3)]
  int ItemsWideInMediumMode {
    get { return _itemsInMediumMode; }
    set {
      _itemsInMediumMode = value;
      if (_owner != NULL) {
        _owner.OnRegionsChanged();
      }
    }
  }
  /// Gets or sets a value indicating the amount of items to show
  /// (wide) when _sizeMode is Medium
  ISIZE ItemsSizeInDropwDownMode {
    get { return _ItemsInDropwDownMode; }
    set {
      _ItemsInDropwDownMode = value;
      if (_owner != NULL) {
        _owner.OnRegionsChanged();
      }
    }
  }
  /// Gets the collection of buttons of the list
  //[DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
  RibbonButtonCollection Buttons {
    get
    {
      return _buttons;
    }
  }
  //#region Methods
  /// Ignores deactivation of canvas if it is a volatile window
  void IgnoreDeactivation() {
    if (Canvas is RibbonPanelPopup) {
      (Canvas as RibbonPanelPopup).IgnoreNextClickDeactivation();
    }
    if (Canvas is RibbonDropDown) {
      (Canvas as RibbonDropDown).IgnoreNextClickDeactivation();
    }
  }
  /// Redraws the control buttons: up, down and dropdown
  void RedrawControlButtons() {
    if (Canvas != NULL) {
      if (ScrollType == ListScrollType.Scrollbar) {
        Canvas.Invalidate(ScrollBarBounds);
      }
      else {
        Canvas.Invalidate(FromLTRB(ButtonUpBounds.l, ButtonUpBounds.t, ButtonDropDownBounds.r, ButtonDropDownBounds.b));
      }
    }
  }
  /// Pushes the amount of _offset of the top of items
  /// <param name="amount"></param>
  void ScrollOffset(int amount) {
    ScrollTo(_offset + amount);
  }
  /// Scrolls the content to the specified offset
  /// <param name="offset"></param>
  void ScrollTo(int offset) {
    int minOffset = _contentBounds.Y - fullContentBounds.Height + _contentBounds.Height;
    if (offset < minOffset) {
      offset = minOffset;
    }
    _offset = offset;
    SetBounds(Bounds);
    RedrawItem();
  }
  /// Scrolls the list down
  void ScrollDown() {
    ScrollOffset(-(_jumpDownSize + 1));
  }
  /// Scrolls the list up
  void ScrollUp() {
    ScrollOffset(_jumpDownSize + 1);
  }
  /// Shows the drop down items of the button, as if the dropdown part has been clicked
  void ShowDropDown() {
    if (DropDownItems.Count == 0) {
      SetPressed(false);
      return;
    }
    IgnoreDeactivation();
    _dropDown = new RibbonDropDown(this, DropDownItems, _owner);
    //_dropDown.FormClosed += new FormClosedEventHandler(dropDown_FormClosed);
    //_dropDown.StartPosition = FormStartPosition.Manual;
    _dropDown._showSizingGrip = true;
    IPOINT location = Canvas.PointToScreen(new IPOINT(Bounds.l, Bounds.t));
    SetDropDownVisible(true);
    _dropDown.Show(location);
  }
  void dropDown_FormClosed(object sender, FormClosedEventArgs e) {
    SetDropDownVisible(false);
  }
  /// Closes the DropDown if opened
  void CloseDropDown() {
    if (_dropDown != NULL) {
      //RibbonDropDown.DismissTo(_dropDown);
    }
    SetDropDownVisible(false);
  }
  /// Sets the value of DropDownVisible
  /// <param name="visible"></param>
  void SetDropDownVisible(bool visible) {
    _dropDownVisible = visible;
  }
  //#region Overrides
  void OnCanvasChanged(EventArgs e) {
    base.OnCanvasChanged(e);
    if (Canvas is RibbonDropDown) {
      _scrollType = ListScrollType.Scrollbar;
    }
    else {
      _scrollType = ListScrollType.UpDownButtons;
    }
  }
  bool ClosesDropDownAt(IPOINT p) {
    return !(
        ButtonDropDownBounds.Contains(p) ||
        ButtonDownBounds.Contains(p) ||
        ButtonUpBounds.Contains(p) ||
        (ScrollType == ListScrollType.Scrollbar && ScrollBarBounds.Contains(p))
        );
  }
  void SetOwner(Ribbon owner) {
    base.SetOwner(owner);
    _buttons.SetOwner(owner);
    _dropDownItems.SetOwner(owner);
  }
  void SetOwnerPanel(RibbonPanel ownerPanel) {
    base.SetOwnerPanel(ownerPanel);
    _buttons.SetOwnerPanel(ownerPanel);
    _dropDownItems.SetOwnerPanel(ownerPanel);
  }
  void SetOwnerTab(RibbonTab ownerTab) {
    base.SetOwnerTab(ownerTab);
    _buttons.SetOwnerTab(ownerTab);
    _dropDownItems.SetOwnerTab(_ownerTab);
  }
  void OnPaint(object sender, RibbonElementPaintEventArgs e) {
    OnRenderRibbonItem(new RibbonItemRenderEventArgs(_owner, e.Graphics, e.Clip, this));
    if (e.Mode != RibbonElementSizeMode_Compact) {
      Region lastClip = e.Graphics.Clip;
      Region newClip = new Region(lastClip.GetBounds(e.Graphics));
      newClip.Intersect(_contentBounds);
      e.Graphics.SetClip(newClip.GetBounds(e.Graphics));
      foreach(RibbonButton button in Buttons) {
        if (!button.Bounds.IsEmpty) {
          button.OnPaint(this, new RibbonElementPaintEventArgs(button.Bounds, e.Graphics, ButtonsSizeMode));
        }
      }
      e.Graphics.SetClip(lastClip.GetBounds(e.Graphics));
    }
  }
  void SetBounds(IRECT bounds) {
    base.SetBounds(bounds);
    //#region Assign control buttons bounds
    if (ScrollType != ListScrollType.Scrollbar) {
      //#region Custom Buttons
      int cbtns = 3; // Canvas is RibbonDropDown ? 2 : 3;
      int buttonHeight = bounds.Height / cbtns;
      int buttonWidth = _controlButtonsWidth;
      _buttonUpBounds = FromLTRB(bounds.r - buttonWidth,
          bounds.t, bounds.r, bounds.t + buttonHeight);
      _buttonDownBounds = FromLTRB(_buttonUpBounds.l, _buttonUpBounds.b,
          bounds.r, _buttonUpBounds.b + buttonHeight);
      if (cbtns == 2) {
        _buttonDropDownBounds = IRECT.Empty;
      }
      else {
        _buttonDropDownBounds = FromLTRB(_buttonDownBounds.l, _buttonDownBounds.b,
            bounds.r, bounds.b + 1);
      }
      _thumbBounds.Location = IPOINT.Empty;
    }
    else {
      //#region Scrollbar
      int bwidth = ThumbBounds.Width;
      int bheight = ThumbBounds.Width;
      _buttonUpBounds = FromLTRB(bounds.r - bwidth,
          bounds.t, bounds.r, bounds.t + bheight);
      _buttonDownBounds = FromLTRB(_buttonUpBounds.l, bounds.Height - bheight,
          bounds.r, bounds.Height);
      _buttonDropDownBounds = IRECT.Empty;
      _thumbBounds.X = _buttonUpBounds.l;
    }
    _contentBounds = FromLTRB(bounds.l, bounds.t, _buttonUpBounds.l, bounds.b);
    //#region Assign buttons regions
    _buttonUpEnabled = _offset < 0;
    if (!_buttonUpEnabled) {
      _offset = 0;
    }
    _buttonDownEnabled = false;
    int curLeft = _contentBounds.l + 1;
    int curTop = _contentBounds.t + 1 + _offset;
    int maxBottom = curTop; // int.MinValue;
    int iniTop = curTop;
    foreach(RibbonItem item in Buttons) {
      item.SetBounds(IRECT.Empty);
    }
    for (int i = 0; i < Buttons.Count; i++) {
      RibbonButton button = Buttons[i] as RibbonButton;
      if (button == NULL) {
        break;
      }
      if (curLeft + button.LastMeasuredSize.Width > _contentBounds.r) {
        curLeft = _contentBounds.l + 1;
        curTop = maxBottom + 1;
      }
      button.SetBounds(iRECT(curLeft, curTop, button.LastMeasuredSize.Width, button.LastMeasuredSize.Height));
      curLeft = button.Bounds.r + 1;
      maxBottom = MAX(maxBottom, button.Bounds.b);
      if (button.Bounds.b > _contentBounds.b) {
        _buttonDownEnabled = true;
      }
      _jumpDownSize = button.Bounds.Height;
      _jumpUpSize = button.Bounds.Height;
    }
    //#region Adjust thumb size
    double contentHeight = maxBottom - iniTop;
    double viewHeight = bounds.Height;
    if (contentHeight > viewHeight && contentHeight != 0) {
      double viewPercent = contentHeight > viewHeight ? viewHeight / contentHeight : 0.0;
      double availHeight = ButtonDownBounds.t - ButtonUpBounds.b;
      double thumbHeight = Math.Ceiling(viewPercent * availHeight);
      if (thumbHeight < 30) {
        if (availHeight >= 30) {
          thumbHeight = 30;
        }
        else {
          thumbHeight = availHeight;
        }
      }
      _thumbBounds.Height = Convert.ToInt32(thumbHeight);
      fullContentBounds = FromLTRB(_contentBounds.l, iniTop, _contentBounds.r, maxBottom);
      _scrollBarEnabled = true;
      UpdateThumbPos();
    }
    else {
      _scrollBarEnabled = false;
    }
  }
  /// Updates the position of the scroll thumb depending on the current offset
  void UpdateThumbPos() {
    if (_avoidNextThumbMeasure) {
      _avoidNextThumbMeasure = false;
      return;
    }
    double scrolledp = ScrolledPercent;
    if (!double.IsInfinity(scrolledp)) {
      double availSpace = ScrollMaximum - ScrollMinimum;
      double scrolledSpace = Math.Ceiling(availSpace * ScrolledPercent);
      _thumbBounds.Y = ScrollMinimum + Convert.ToInt32(scrolledSpace);
    }
    else {
      _thumbBounds.Y = ScrollMinimum;
    }
    if (_thumbBounds.Y > ScrollMaximum) {
      _thumbBounds.Y = ScrollMaximum;
    }
  }
  ISIZE MeasureSize(object sender, RibbonElementMeasureSizeEventArgs e) {
    //#region Determine items
    int itemsWide = 0;
    switch (e._sizeMode) {
    case RibbonElementSizeMode_DropDown:
      itemsWide = ItemsSizeInDropwDownMode.Width;
      break;
    case RibbonElementSizeMode_Large:
      itemsWide = ItemsWideInLargeMode;
      break;
    case RibbonElementSizeMode_Medium:
      itemsWide = ItemsWideInMediumMode;
      break;
    case RibbonElementSizeMode_Compact:
      itemsWide = 0;
      break;
    }
    int height = OwnerPanel._contentBounds.Height - _owner._itemPadding RCY(&) - 4;
    int scannedItems = 0;
    int widthSum = 1;
    int buttonHeight = 0;
    int heightSum = 0;
    bool sumWidth = true;
    foreach(RibbonButton button in Buttons) {
      ISIZE s = button.MeasureSize(this,
          new RibbonElementMeasureSizeEventArgs(e.Graphics, this.ButtonsSizeMode));
      if (sumWidth) {
        widthSum += s.Width + 1;
      }
      buttonHeight = button.LastMeasuredSize.Height;
      heightSum += buttonHeight;
      if (++scannedItems == itemsWide) {
        sumWidth = false;
      }
    }
    if (e._sizeMode == RibbonElementSizeMode_DropDown) {
      height = buttonHeight * ItemsSizeInDropwDownMode.Height;
    }
    if (ScrollBarRenderer.IsSupported) {
      _thumbBounds = iRECT(IPOINT.Empty, ScrollBarRenderer.GetSizeBoxSize(e.Graphics, VisualStyles.ScrollBarState.Normal));
    }
    else {
      _thumbBounds = iRECT(IPOINT.Empty, new ISIZE(16, 16));
    }
    //if (height < 0)
    //{
    //    throw new Exception("???");
    //}
    SetLastMeasuredSize(new ISIZE(widthSum + ControlButtonsWidth, height));
    return LastMeasuredSize;
  }
  void SetSizeMode(RibbonElementSizeMode sizeMode) {
    base.SetSizeMode(sizeMode);
    foreach(RibbonItem item in Buttons) {
      item.SetSizeMode(ButtonsSizeMode);
    }
  }
  void OnMouseMove(MouseEventArgs e) {
    base.OnMouseMove(e);
    if (ButtonDownPressed && ButtonDownSelected && ButtonDownEnabled) {
      ScrollOffset(-1);
    }
    if (ButtonUpPressed && ButtonUpSelected && ButtonUpEnabled) {
      ScrollOffset(1);
    }
    bool upCache = _buttonUpSelected;
    bool downCache = _buttonDownSelected;
    bool dropCache = _buttonDropDownSelected;
    bool thumbCache = _thumbSelected;
    _buttonUpSelected = _buttonUpBounds.Contains(e.Location);
    _buttonDownSelected = _buttonDownBounds.Contains(e.Location);
    _buttonDropDownSelected = _buttonDropDownBounds.Contains(e.Location);
    _thumbSelected = _thumbBounds.Contains(e.Location) && ScrollType == ListScrollType.Scrollbar && ScrollBarEnabled;
    if ((upCache != _buttonUpSelected)
        || (downCache != _buttonDownSelected)
        || (dropCache != _buttonDropDownSelected)
        || (thumbCache != _thumbSelected)) {
      RedrawControlButtons();
    }
    if (ThumbPressed) {
      int newval = e.Y - _thumbOffset;
      if (newval < ScrollMinimum) {
        newval = ScrollMinimum;
      }
      else if (newval > ScrollMaximum) {
        newval = ScrollMaximum;
      }
      ScrollValue = newval;
      RedrawScroll();
    }
  }
  void OnMouseLeave(MouseEventArgs e) {
    base.OnMouseLeave(e);
    bool mustRedraw = _buttonUpSelected || _buttonDownSelected || _buttonDropDownSelected;
    _buttonUpSelected = false;
    _buttonDownSelected = false;
    _buttonDropDownSelected = false;
    if (mustRedraw) {
      RedrawControlButtons();
    }
  }
  void OnMouseDown(MouseEventArgs e) {
    base.OnMouseDown(e);
    if (ButtonDownSelected || ButtonUpSelected || ButtonDropDownSelected) {
      IgnoreDeactivation();
    }
    if (ButtonDownSelected && ButtonDownEnabled) {
      _buttonDownPressed = true;
      ScrollDown();
    }
    if (ButtonUpSelected && ButtonUpEnabled) {
      _buttonUpPressed = true;
      ScrollUp();
    }
    if (ButtonDropDownSelected) {
      _buttonDropDownPressed = true;
      ShowDropDown();
    }
    if (ThumbSelected) {
      _thumbPressed = true;
      _thumbOffset = e.Y - _thumbBounds.Y;
    }
    if (
        ScrollType == ListScrollType.Scrollbar &&
        ScrollBarBounds.Contains(e.Location) &&
        e.Y >= ButtonUpBounds.b && e.Y <= ButtonDownBounds.Y &&
        !ThumbBounds.Contains(e.Location) &&
        !ButtonDownBounds.Contains(e.Location) &&
        !ButtonUpBounds.Contains(e.Location)) {
      if (e.Y < ThumbBounds.Y) {
        ScrollUp();
      }
      else {
        ScrollDown();
      }
    }
  }
  void OnMouseUp(MouseEventArgs e) {
    base.OnMouseUp(e);
    _buttonDownPressed = false;
    _buttonUpPressed = false;
    _buttonDropDownPressed = false;
    _thumbPressed = false;
  }
  //#region IContainsRibbonItems Members
  IEnumerable<RibbonItem> GetItems() {
    return Buttons;
  }
  IRECT GetContentBounds() {
    return _contentBounds;
  }
  //#region IContainsRibbonComponents Members
  IEnumerable<Component> GetAllChildComponents() {
    List<Component> result = new List<Component>(Buttons.ToArray());
    result.AddRange(DropDownItems.ToArray());
    return result;
  }
#endif
};
//[DesignTimeVisible(false)]
//[Designer(typeof(RibbonPanelDesigner))]
struct RibbonPanel { // : Component, IRibbonElement, IContainsSelectableRibbonItems, IContainsRibbonComponents
  //#region Fields
  img_t* _image;
  RibbonItem* _items;
  int _items_count;
  const char* _text;
  IRECT _bounds;
  IRECT _contentBounds;
  IRECT _buttonMoreBounds;
  //RibbonTab* _ownerTab;
  RibbonElementSizeMode _sizeMode;
  RibbonPanelFlowDirection _flowsTo;
  bool _pressed;
  bool _enabled;
  bool _selected;
  bool _buttonMorePressed;
  bool _buttonMoreSelected;
  bool _buttonMoreVisible;
  bool _buttonMoreEnabled;
  IRECT _overflowBoundsBuffer;
  bool _popupShowed;
#if 0
  //#region Ctor
  /// Creates a new RibbonPanel
  RibbonPanel() {
    _items = new RibbonItemCollection();
    _sizeMode = RibbonElementSizeMode_None;
    _flowsTo = RibbonPanelFlowDirection_b;
    _buttonMoreEnabled = true;
    _buttonMoreVisible = true;
    _items.SetOwnerPanel(this);
    _enabled = true;
  }
  /// Creates a new RibbonPanel with the specified text and panel flow direction
  /// <param name="text">_text of the panel</param>
  /// <param name="flowsTo">Flow direction of the content items</param>
  RibbonPanel(const char* text, RibbonPanelFlowDirection flowsTo, IEnumerable<RibbonItem> items)
    : this() {
    _text = text;
    _flowsTo = flowsTo;
    _items.AddRange(items);
  }
  /// Gets if this panel is currenlty collapsed
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool Collapsed {
    get { return _sizeMode == RibbonElementSizeMode_Overflow; }
  }
#endif
  /// Gets if the panel is in overflow mode
  /// <remarks>Overflow mode is when the available space to draw the panel is not enough to draw components, so panel is drawn as a button that shows the full content of the panel in a pop-up window when clicked</remarks>
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool OverflowMode() {
    return _sizeMode == RibbonElementSizeMode_Overflow;
  }
#if 0
  ISIZE SwitchToSize(Control ctl, Graphics g, RibbonElementSizeMode size) {
    ISIZE s = MeasureSize(this, new RibbonElementMeasureSizeEventArgs(g, size));
    IRECT r = iRECT(0, 0, s.Width, s.Height);
    //if (!(ctl is Ribbon))
    //    r = boundsBuffer;
    //else
    //    r = iRECT(0, 0, 0, 0);
    SetBounds(r);
    UpdateItemsRegions(g, size);
    return s;
  }
#endif
  /// Sets the bounds of the panel
  /// <param name="bounds"></param>
  int SetBounds(IRECT bounds) {
    //bool trigger = _bounds != bounds;
    _bounds = bounds;
#if 0
    OnResize(EventArgs_Empty);
    if (_owner != NULL) {
      //Update contentBounds
      _contentBounds = FromLTRB(
          bounds.X + _owner._panelMargin.l + 0,
          bounds.Y + _owner._panelMargin.t + 0,
          bounds.r - _owner._panelMargin.r,
          bounds.b - _owner._panelMargin.b);
    }
    //"More" bounds
    if (_buttonMoreVisible) {
      SetMoreBounds(FromLTRB(bounds.r - 15, _contentBounds.b + 1, bounds.r, bounds.b));
    }
    else {
      SetMoreBounds(IRECT.Empty);
    }
#endif
    return 0;
  }
  /// Measures the size when flow direction is to right
  /// <param name="sender"></param>
  /// <returns></returns>
  ISIZE MeasureSizeFlowsToRight(IRECT _panelMargin, IRECT _itemPadding, int _sizeMode, const font_t* fo) {
    int widthSum = RCX(&_panelMargin);
    int maxWidth = 0;
    int maxHeight = 0;
    int dividedWidth = 0;
    int i;
    for (i = 0; i < _items_count; ++i) {
      RibbonItem* item = _items + i;
      ISIZE itemSize = item->MeasureSize(_sizeMode, fo);
      widthSum += itemSize.w + RCX(&_itemPadding) + 1;
      maxWidth = MAX(maxWidth, itemSize.w);
      maxHeight = MAX(maxHeight, itemSize.h);
    }
    switch (_sizeMode) {
    case RibbonElementSizeMode_Large:
      dividedWidth = widthSum / 1; //Show items on one row
      break;
    case RibbonElementSizeMode_Medium:
      dividedWidth = widthSum / 2; //Show items on two rows
      break;
    case RibbonElementSizeMode_Compact:
      dividedWidth = widthSum / 3; //Show items on three rows
      break;
    default:
      break;
    }
    //Add padding
    dividedWidth += RCX(&_panelMargin);
    return iSIZE(MAX(maxWidth, dividedWidth) + RCX(&_panelMargin), 0); //Height is provided by MeasureSize
  }
  /// Measures the size when flow direction is to bottom
  /// <param name="sender"></param>
  /// <returns></returns>
  ISIZE MeasureSizeFlowsToBottom(IRECT _panelMargin, IRECT _itemPadding, int availableHeight, int _sizeMode, const font_t* fo) {
    int curRight = _panelMargin.l + RCX(&_itemPadding);
    int curBottom = _contentBounds.t + RCY(&_itemPadding);
    int lastRight = 0;
    int lastBottom = 0;
    int maxRight = 0;
    int maxBottom = 0;
    int i;
    for (i = 0; i < _items_count; ++i) {
      RibbonItem* item = _items + i;
      ISIZE itemSize = item->MeasureSize(_sizeMode, fo);
      if (curBottom + itemSize.h > _contentBounds.b) {
        curBottom = _contentBounds.t + RCY(&_itemPadding) + 0;
        curRight = maxRight + RCX(&_itemPadding) + 0;
      }
      IRECT bounds = iRECT2(curRight, curBottom, itemSize.w, itemSize.h);
      lastRight = bounds.r;
      lastBottom = bounds.b;
      curBottom = bounds.b + RCY(&_itemPadding) + 1;
      maxRight = MAX(maxRight, lastRight);
      maxBottom = MAX(maxBottom, lastBottom);
    }
    return iSIZE(maxRight + _itemPadding.r + _panelMargin.r + 1, 0); //Height is provided by MeasureSize
  }
  /// Measures the size of the panel on the mode specified by the event object
  /// <param name="sender"></param>
  /// <returns></returns>
  ISIZE MeasureSize(IRECT TabContentBounds, IRECT TabContentMargin, IRECT _panelPadding, IRECT _panelMargin, IRECT _itemPadding, int _sizeMode, const font_t* fo) {
    ISIZE result = {0};
    ISIZE minSize = {0};
    int panelHeight = RCH(&TabContentBounds) - RCY(&_panelPadding);
    //#region Measure width of minSize
    minSize.w = font_text_size(fo, _text, -1, 0).w + RCX(&_panelMargin) + 1;
    if (_buttonMoreVisible) {
      minSize.w += RCW(&_buttonMoreBounds) + 3;
    }
    if (_sizeMode == RibbonElementSizeMode_Overflow) {
      ISIZE textSize = RibbonButton::MeasureStringLargeSize(_text, fo);
      return iSIZE(textSize.w + RCX(&_panelMargin), panelHeight);
    }
    switch (_flowsTo) {
    case RibbonPanelFlowDirection_r:
      result = MeasureSizeFlowsToRight(_panelMargin, _itemPadding, _sizeMode, fo);
      break;
    case RibbonPanelFlowDirection_b: {
      int availableHeight = RCH(&TabContentBounds) - RCY(&TabContentMargin) - RCY(&_panelPadding) - RCY(&_panelMargin);
      result = MeasureSizeFlowsToBottom(_panelMargin, _itemPadding, availableHeight, _sizeMode, fo);
    }
    break;
    default:
      break;
    }
    return iSIZE(MAX(result.w, minSize.w), panelHeight);
  }
#if 0
  /// Sets the value of the _owner Property
  void SetOwner(Ribbon owner) {
    _owner = owner;
    _items.SetOwner(owner);
  }
  /// Sets the value of the _selected property
  /// <param name="selected">Value that indicates if the element is selected</param>
  void SetSelected(bool selected) {
    _selected = selected;
  }
  //#region Methods
  /// Raises the <see cref="Resize"/> method
  void OnResize(EventArgs e) {
    if (Resize != NULL) {
      Resize(this, e);
    }
  }
  /// Shows the panel in a popup
  void ShowOverflowPopup() {
    IRECT b = _bounds;
    RibbonPanelPopup f = new RibbonPanelPopup(this);
    IPOINT p = _owner.PointToScreen(new IPOINT(b.l, b.b));
    _popupShowed = true;
    f.Show(p);
  }
#endif
  /// Sets the value of the _sizeMode property
  /// <param name="sizeMode"></param>
  void SetSizeMode(RibbonElementSizeMode sizeMode) {
    _sizeMode = sizeMode;
    int i;
    for (i = 0; i < _items_count; ++i) {
      RibbonItem* item = _items + i;
      //item->SetSizeMode(sizeMode);
    }
  }
  /// Sets the value of the _contentBounds property
  /// <param name="contentBounds">_bounds of the content on the panel</param>
  void SetContentBounds(IRECT contentBounds) {
    _contentBounds = contentBounds;
  }
  /// Updates the bounds of child elements when flow is to bottom
  void UpdateRegionsFlowsToBottom(IRECT _itemPadding, RibbonElementSizeMode mode) {
    int curRight = _contentBounds.l + RCX(&_itemPadding) + 0;
    int curBottom = _contentBounds.t + RCY(&_itemPadding) + 0;
    int lastRight = curRight;
    int lastBottom = 0;
    RibbonItem* lastColumn = _items;
    ///Iterate thru items on panel
    for (RibbonItem* item = _items, *item_end = _items + _items_count; item < item_end; ++item) {
      ///Gets the last measured size (to avoid re-measuring calculations)
      ISIZE itemSize = item->_lastMeasureSize;
      ///If not enough space available, reset curBottom and advance curRight
      if (curBottom + itemSize.h > _contentBounds.b) {
        curBottom = _contentBounds.t + RCY(&_itemPadding) + 0;
        curRight = lastRight + RCX(&_itemPadding) + 0;
        CenterItemsVerticallyInto(lastColumn - _items, lastColumn, _contentBounds);
        lastColumn = item;
      }
      ///Set the item's bounds
      item->SetBounds(iRECT2(curRight, curBottom, itemSize.w, itemSize.h));
      ///save last right and bottom
      lastRight = MAX(item->_bounds.r, lastRight);
      lastBottom = item->_bounds.b;
      ///update current bottom
      curBottom = item->_bounds.b + RCY(&_itemPadding) + 1;
      ///Add to the collection of items of the last column
      //lastColumn.Add(item);
    }
    ///Center the items vertically on the last column
    CenterItemsVerticallyInto(lastColumn - _items, _items, GetItemsBounds(_items_count, _items));
  }
  /// Updates the bounds of child elements when flow is to bottom
  void UpdateRegionsFlowsToRight(IRECT _itemPadding, RibbonElementSizeMode mode) {
    int curLeft = _contentBounds.l;
    int curTop = _contentBounds.t;
    int padding = mode == RibbonElementSizeMode_Medium ? 7 : 0;
    int maxBottom = 0;
    int i, j, list_Count;
    int index[100] = {0};
    //#region Sorts from larger to smaller
    RibbonItem* array = _items;
    for (i = 0; i < _items_count; ++i) {
      index[i] = i;
    }
    for (i = (_items_count - 1); i >= 0; i--) {
      for (j = 1; j <= i; j++) {
        if (array[index[j - 1]]._lastMeasureSize.w < array[index[j]]._lastMeasureSize.w) {
          int temp = index[j - 1];
          index[j - 1] = index[j];
          index[j] = temp;
        }
      }
    }
    int* list = index;
    list_Count = _items_count;
    ///Attend elements, deleting every attended element from the list
    while (list_Count > 0) {
      ///Extract item and delete it
      RibbonItem* item = array + (*list++);
      list_Count--;
      ///If not enough space left, reset left and advance top
      if (curLeft + item->_lastMeasureSize.w > _contentBounds.r) {
        curLeft = _contentBounds.l;
        curTop = maxBottom + RCY(&_itemPadding) + 1 + padding;
      }
      ///Set item's bounds
      item->SetBounds(iRECT2(curLeft, curTop, item->_lastMeasureSize.w, item->_lastMeasureSize.h));
      ///Increment reminders
      curLeft += RCW(&item->_bounds) + RCX(&_itemPadding);
      maxBottom = MAX(maxBottom, item->_bounds.b);
      ///Check available space after placing item
      int spaceAvailable = _contentBounds.r - curLeft;
      ///Check for elements that fit on available space
      for (i = 0; i < list_Count; i++) {
        ///If item fits on the available space
        if (list[i] >= 0) {
          RibbonItem* item1 = array + list[i];
          if (item1->_lastMeasureSize.w < spaceAvailable) {
            ///Place the item there and reset the counter to check for further items
            item1->SetBounds(iRECT2(curLeft, curTop, item1->_lastMeasureSize.w, item1->_lastMeasureSize.h));
            curLeft += RCW(&item1->_bounds) + RCX(&_itemPadding);
            maxBottom = MAX(maxBottom, item1->_bounds.b);
            spaceAvailable = _contentBounds.r - curLeft;
            list[i] = -1;;
            i = 0;
          }
        }
      }
      j = 0;
      for (i = 0; i < list_Count; i++) {
        if (list[i] >= 0) {
          index[j++] = list[i];
        }
      }
      list_Count = j;
      list = index;
    }
  }
  /// Updates the bounds of child elements
  void UpdateItemsRegions(IRECT _itemPadding, RibbonElementSizeMode mode) {
    switch (_flowsTo) {
    case RibbonPanelFlowDirection_r:
      UpdateRegionsFlowsToRight(_itemPadding, mode);
      break;
    case RibbonPanelFlowDirection_b:
      UpdateRegionsFlowsToBottom(_itemPadding, mode);
      break;
    }
    ///Center items on the panel
    CenterItems();
  }
  /// Centers the items on the tab conent
  void CenterItems() {
    CenterItemsInto(_items_count, _items, _contentBounds);
  }
  /// Sets the value of the _pressed property
  /// <param name="pressed"></param>
  void SetPressed(bool pressed) {
    _pressed = pressed;
  }
#if 0
  /// Overriden. Gives info about the panel as a const char*
  /// <returns></returns>
  const char* ToString() {
    return const char * .Format("Panel: {0} ({1})", _text, _sizeMode);
  }
  /// Sets the value of the _buttonMorePressed property
  /// <param name="bounds">property value</param>
  void SetMorePressed(bool pressed) {
    _buttonMorePressed = pressed;
  }
  /// Sets the value of the _buttonMoreSelected property
  /// <param name="bounds">property value</param>
  void SetMoreSelected(bool selected) {
    _buttonMoreSelected = selected;
  }
  /// Sets the value of the _buttonMoreBounds property
  /// <param name="bounds">property value</param>
  void SetMoreBounds(IRECT bounds) {
    _buttonMoreBounds = bounds;
  }
  /// Raised the <see cref="ButtonMoreClick"/> event
  void OnButtonMoreClick(EventArgs e) {
    if (ButtonMoreClick != NULL) {
      ButtonMoreClick(this, e);
    }
  }
  //#region IContainsRibbonItems Members
  IEnumerable<RibbonItem> GetItems() {
    return _items;
  }
  IRECT GetContentBounds() {
    return _contentBounds;
  }
  /// Raises the MouseEnter event
  /// <param name="e">Event data</param>
  void OnMouseEnter(MouseEventArgs e) {
    if (!_enabled) {
      return;
    }
    if (MouseEnter != NULL) {
      MouseEnter(this, e);
    }
  }
  /// Raises the MouseLeave event
  /// <param name="e">Event data</param>
  void OnMouseLeave(MouseEventArgs e) {
    if (!_enabled) {
      return;
    }
    if (MouseLeave != NULL) {
      MouseLeave(this, e);
    }
  }
  /// Raises the MouseMove event
  /// <param name="e">Event data</param>
  void OnMouseMove(MouseEventArgs e) {
    if (!_enabled) {
      return;
    }
    if (MouseMove != NULL) {
      MouseMove(this, e);
    }
    bool redraw = false;
    if (_buttonMoreEnabled && _buttonMoreVisible && _buttonMoreBounds.Contains(e.X, e.Y) && !Collapsed) {
      SetMoreSelected(true);
      redraw = true;
    }
    else {
      redraw = _buttonMoreSelected;
      SetMoreSelected(false);
    }
    if (redraw) {
      _owner.Invalidate(_bounds);
    }
  }
  /// Raises the Click event
  /// <param name="e">Event data</param>
  void OnClick(EventArgs e) {
    if (!_enabled) {
      return;
    }
    if (Click != NULL) {
      Click(this, e);
    }
    if (Collapsed && _popUp == NULL) {
      ShowOverflowPopup();
    }
  }
  /// Raises the DoubleClick event
  /// <param name="e">Event data</param>
  void OnDoubleClick(EventArgs e) {
    if (!_enabled) {
      return;
    }
    if (DoubleClick != NULL) {
      DoubleClick(this, e);
    }
  }
  /// Raises the MouseDown event
  /// <param name="e">Event data</param>
  void OnMouseDown(MouseEventArgs e) {
    if (!_enabled) {
      return;
    }
    if (MouseDown != NULL) {
      MouseDown(this, e);
    }
    SetPressed(true);
    bool redraw = false;
    if (_buttonMoreEnabled && _buttonMoreVisible && _buttonMoreBounds.Contains(e.X, e.Y) && !Collapsed) {
      SetMorePressed(true);
      redraw = true;
    }
    else {
      redraw = _buttonMoreSelected;
      SetMorePressed(false);
    }
    if (redraw) {
      _owner.Invalidate(_bounds);
    }
  }
  /// Raises the MouseUp event
  /// <param name="e">Event data</param>
  void OnMouseUp(MouseEventArgs e) {
    if (!_enabled) {
      return;
    }
    if (MouseUp != NULL) {
      MouseUp(this, e);
    }
    if (_buttonMoreEnabled && _buttonMoreVisible && _buttonMorePressed && !Collapsed) {
      OnButtonMoreClick(EventArgs_Empty);
    }
    SetPressed(false);
    SetMorePressed(false);
  }
  //#region IContainsRibbonComponents Members
  IEnumerable<Component> GetAllChildComponents() {
    return _items.ToArray();
  }
#endif
};
/// Represents a tab that can contain RibbonPanel objects
//[DesignTimeVisible(false)]
//[Designer(typeof(RibbonTabDesigner))]
struct RibbonTab {
  IRECT _bounds;
  IRECT _tabContentBounds;
  IRECT _scrollLeftBounds;
  IRECT _scrollRightBounds;
  const char* _text;
  RibbonPanel* _panels;
  int _panels_count;
  int _offset;
  bool _pressed;
  bool _selected;
  bool _active;
  bool _scrollLeftVisible;
  bool _scrollLeftSelected;
  bool _scrollLeftPressed;
  bool _scrollRightSelected;
  bool _scrollRightVisible;
  bool _scrollRightPressed;
  /// Measures the size of the tab. The tab content bounds is measured by the Ribbon control
  /// <param name="sender"></param>
  ISIZE MeasureSize(const font_t* fo) {
    ISIZE textSize = font_text_size(fo, _text, -1, 0);
    return textSize;
  }
  /// Sets the value of the _bounds property
  /// <param name="tabBounds">IRECT representing the bounds of the tab</param>
  void SetTabBounds(IRECT tabBounds) {
    //bool tigger = _bounds != tabBounds;
    _bounds = tabBounds;
    //OnTabBoundsChanged(EventArgs_Empty);
  }
#if 0
  /// Sets the value of the _tabContentBounds
  /// <param name="tabContentBounds">IRECT representing the bounds of the tab's content</param>
  void SetTabContentBounds(IRECT tabContentBounds) {
    bool trigger = _tabContentBounds != tabContentBounds;
    _tabContentBounds = tabContentBounds;
    OnTabContentBoundsChanged(EventArgs_Empty);
  }
#endif
  /// Gets the panel with the larger with and the specified size mode
  /// <param name="size">ISIZE mode of panel to search</param>
  /// <returns>Larger panel-> Null if none of the specified size mode</returns>
  RibbonPanel* GetLargerPanel(int size) {
    RibbonPanel* result = NULL;
    int i;
    for (i = 0; i < _panels_count; ++i) {
      RibbonPanel* panel = _panels + i;
      if (panel->_sizeMode != size) {
        continue;
      }
      if (result == NULL) {
        result = panel;
      }
      if (RCW(&panel->_bounds) > RCW(&result->_bounds)) {
        result = panel;
      }
    }
    return result;
  }
  /// Gets the panel with a larger size
  /// <returns></returns>
  RibbonPanel* GetLargerPanel() {
    RibbonPanel* largeLarger = GetLargerPanel(RibbonElementSizeMode_Large);
    if (largeLarger != NULL) {
      return largeLarger;
    }
    RibbonPanel* mediumLarger = GetLargerPanel(RibbonElementSizeMode_Medium);
    if (mediumLarger != NULL) {
      return mediumLarger;
    }
    RibbonPanel* compactLarger = GetLargerPanel(RibbonElementSizeMode_Compact);
    if (compactLarger != NULL) {
      return compactLarger;
    }
    RibbonPanel* overflowLarger = GetLargerPanel(RibbonElementSizeMode_Overflow);
    if (overflowLarger != NULL) {
      return overflowLarger;
    }
    return NULL;
  }
  bool AllPanelsOverflow() {
    int i;
    for (i = 0; i < _panels_count; ++i) {
      RibbonPanel* panel = _panels + i;
      if (panel->_sizeMode != RibbonElementSizeMode_Overflow) {
        return false;
      }
    }
    return true;
  }
  /// Updates the bounds of the scroll bounds
  void UpdateScrollBounds(IRECT ClientRectangle) {
#if 0
    int w = 13;
    bool scrBuffer = _scrollRightVisible;
    bool sclBuffer = _scrollLeftVisible;
    IRECT rrBuffer = _scrollRightBounds;
    IRECT rlBuffer = _scrollLeftBounds;
    if (_panels_count == 0) {
      return;
    }
    if (_panels[_panels_count - 1]._bounds.r > _tabContentBounds.r) {
      _scrollRightVisible = true;
    }
    else {
      _scrollRightVisible = false;
    }
    if (_scrollRightVisible != scrBuffer) {
      OnScrollRightVisibleChanged(EventArgs_Empty);
    }
    if (_offset < 0) {
      _scrollLeftVisible = true;
    }
    else {
      _scrollLeftVisible = false;
    }
    if (_scrollRightVisible != scrBuffer) {
      OnScrollLeftVisibleChanged(EventArgs_Empty);
    }
    if (_scrollLeftVisible || _scrollRightVisible) {
      _scrollRightBounds = FromLTRB(ClientRectangle.r - w, _tabContentBounds.t, ClientRectangle.r, _tabContentBounds.b);
      _scrollLeftBounds = FromLTRB(0, _tabContentBounds.t, w, _tabContentBounds.b);
      if (_scrollRightBounds != rrBuffer) {
        OnScrollRightBoundsChanged(EventArgs_Empty);
      }
      if (_scrollLeftBounds != rlBuffer) {
        OnScrollLeftBoundsChanged(EventArgs_Empty);
      }
    }
#endif
  }
  /// Updates the regions of the panels and its contents
  int UpdatePanelsRegions(IRECT _tabContentMargin, IRECT _panelPadding, IRECT _panelMargin, IRECT _itemPadding, int _panelSpacing, bool DesignMode, int _sizeMode, const font_t* fo) {
    int i;
    if (_panels_count == 0) {
      return 0;
    }
    bool dMode = DesignMode;
    //if(!dMode)  _offset = 0;
    int curRight = _tabContentBounds.l + _panelPadding.l + _offset;
    int panelsTop = _tabContentBounds.t + _panelPadding.t;
    {
      //Check all at full size
      for (i = 0; i < _panels_count; ++i) {
        RibbonPanel* panel = _panels + i;
        RibbonElementSizeMode sMode = panel->_flowsTo == RibbonPanelFlowDirection_r ? RibbonElementSizeMode_Medium : RibbonElementSizeMode_Large;
        //Set the bounds of the panel to let it know it's height
        panel->SetBounds(iRECT2(0, 0, 1, RCH(&_tabContentBounds) - RCY(&_panelPadding)));
        ///ISIZE of the panel
        ISIZE size = panel->MeasureSize(_tabContentBounds, _tabContentMargin, _panelPadding, _panelMargin, _itemPadding, sMode, fo);
        ///Creates the bounds of the panel
        IRECT bounds = iRECT2(curRight, panelsTop, size.w, size.h);
        ///Set the bouns of the panel
        panel->SetBounds(bounds);
        ///Let the panel know what size we have decided for it
        panel->_sizeMode = sMode;
        ///Update curLeft
        curRight = bounds.r + 1 + _panelSpacing;
      }
      if (!dMode) {
        while (curRight > _tabContentBounds.r && !AllPanelsOverflow()) {
          //#region Down grade the larger panel one position
          RibbonPanel* larger = GetLargerPanel();
          if (larger->_sizeMode == RibbonElementSizeMode_Large) {
            larger->SetSizeMode(RibbonElementSizeMode_Medium);
          }
          else if (larger->_sizeMode == RibbonElementSizeMode_Medium) {
            larger->SetSizeMode(RibbonElementSizeMode_Compact);
          }
          else if (larger->_sizeMode == RibbonElementSizeMode_Compact) {
            larger->SetSizeMode(RibbonElementSizeMode_Overflow);
          }
          ISIZE size = larger->MeasureSize(_tabContentBounds, _tabContentMargin, _panelPadding, _panelMargin, _itemPadding, _sizeMode, fo);
          larger->SetBounds(iRECT2(larger->_bounds.l, larger->_bounds.t, size.w + RCX(&_panelMargin), size.h));
          ///Reset x-axis reminder
          curRight = _tabContentBounds.l + _panelPadding.l;
          ///Re-arrange location because of the new bounds
          for (i = 0; i < _panels_count; ++i) {
            RibbonPanel* panel = _panels + i;
            ISIZE s = RCSZ(&panel->_bounds);
            panel->SetBounds(iRECT2(curRight, panelsTop, s.w, s.h));
            curRight += RCW(&panel->_bounds) + 1 + _panelSpacing;
          }
        }
      }
      ///Update regions of all panels
      for (i = 0; i < _panels_count; ++i) {
        RibbonPanel* panel = _panels + i;
        panel->UpdateItemsRegions(_itemPadding, panel->_sizeMode);
      }
    }
    UpdateScrollBounds(_tabContentBounds);
    return 0;
  }
#if 0
  /// Overriden. Returns a string representation of the tab
  /// <returns></returns>
  string ToString() {
    return string.Format("Tab: {0}", _text);
  }
  /// Raises the MouseEnter event
  /// <param name="e">Event data</param>
  void OnMouseEnter(MouseEventArgs e) {
    if (MouseEnter != NULL) {
      MouseEnter(this, e);
    }
  }
  /// Raises the MouseLeave event
  /// <param name="e">Event data</param>
  void OnMouseLeave(MouseEventArgs e) {
    if (MouseLeave != NULL) {
      MouseLeave(this, e);
    }
  }
  /// Raises the MouseMove event
  /// <param name="e">Event data</param>
  void OnMouseMove(MouseEventArgs e) {
    if (MouseMove != NULL) {
      MouseMove(this, e);
    }
  }
  /// Sets the value of the <see cref="_scrollLeftPressed"/>
  /// <param name="pressed"></param>
  void SetScrollLeftPressed(bool pressed) {
    _scrollLeftPressed = pressed;
    if (pressed) {
      ScrollLeft();
    }
    OnScrollLeftPressedChanged(EventArgs_Empty);
  }
  /// Sets the value of the <see cref="_scrollLeftSelected"/>
  /// <param name="selected"></param>
  void SetScrollLeftSelected(bool selected) {
    _scrollLeftSelected = selected;
    OnScrollLeftSelectedChanged(EventArgs_Empty);
  }
  /// Sets the value of the <see cref="_scrollRightPressed"/>
  /// <param name="pressed"></param>
  void SetScrollRightPressed(bool pressed) {
    _scrollRightPressed = pressed;
    if (pressed) {
      ScrollRight();
    }
    OnScrollRightPressedChanged(EventArgs_Empty);
  }
  /// Sets the value of the <see cref="_scrollRightSelected"/>
  /// <param name="selected"></param>
  void SetScrollRightSelected(bool selected) {
    _scrollRightSelected = selected;
    OnScrollRightSelectedChanged(EventArgs_Empty);
  }
  /// Presses the lef-scroll button
  void ScrollLeft() {
    ScrollOffset(50);
  }
  /// Presses the left-scroll button
  void ScrollRight() {
    ScrollOffset(-50);
  }
  void ScrollOffset(int amount) {
    _offset += amount;
    foreach(RibbonPanel p in _panels) {
      p.SetBounds(iRECT(p._bounds.l + amount, p._bounds.t, RCW(&p._bounds), RCH(&p._bounds)));
    }
    if (Site != NULL && Site.DesignMode) {
      UpdatePanelsRegions();
    }
    UpdateScrollBounds();
    _owner.Invalidate();
  }
  //#region IContainsRibbonComponents Members
  IEnumerable<Component> GetAllChildComponents() {
    return _panels.ToArray();
  }
#endif
};
/// Represents a quick access toolbar hosted on the Ribbon
struct RibbonQuickAccessToolbar { // : RibbonItem //        IContainsSelectableRibbonItems, IContainsRibbonComponents
  //#region Fields
  bool _menuButtonVisible;
  IRECT _bounds;
  IRECT _margin;
  IRECT _padding;
  //RibbonMouseSensor _sensor;
  RibbonButton* _dropDownButton;
  bool _dropDownButtonVisible;
  RibbonButton* _items;
  int _items_count;
  int init() {
#if 0
    _dropDownButton->init();
    _dropDownButton->_owner = ownerRibbon;
    _margin = iRECT(9, 9, 9, 9);
    _padding = iRECT(3, 0, 0, 0);
    _dropDownButtonVisible = true;
    _dropDownButton.SmallImage = CreateDropDownButtonImage();
    _items = new RibbonQuickAccessToolbarItemCollection(this);
    _sensor = new RibbonMouseSensor(ownerRibbon, ownerRibbon, _items);
#endif
    return 0;
  }
#if 0
  int CreateDropDownButtonImage(img_t* im) {
    Bitmap bmp = new Bitmap(7, 7);
    RibbonProfessionalRenderer renderer = _owner._renderer as RibbonProfessionalRenderer;
    COLOR dk = Color_Navy;
    COLOR lt = Color_White;
    if (renderer != NULL) {
      dk = renderer.ColorTable.Arrow;
      lt = renderer.ColorTable.ArrowLight;
    }
    using(Graphics g = Graphics.FromImage(bmp)) {
      DrawDropDownButtonArrow(g, lt, 0, 1);
      DrawDropDownButtonArrow(g, dk, 0, 0);
    }
    return 0;
  }
  RibbonQuickAccessToolbarItemCollection _items;
  //#region Ctor
  void DrawDropDownButtonArrow(Graphics g, COLOR c, int x, int y) {
    using(Pen p = new Pen(c)) {
      using(SolidBrush b = new SolidBrush(c)) {
        g.DrawLine(p, x, y, x + 4, y);
        g.FillPolygon(b, new IPOINT[] {
          new IPOINT(x, y + 3),
          new IPOINT(x + 5, y + 3),
          new IPOINT(x + 2, y + 6)
        });
      }
    }
  }
  //#region Properties
  [Description("Shows or hides the dropdown button of the toolbar")]
  //[DefaultValue(true)]
  bool DropDownButtonVisible {
    get { return _dropDownButtonVisible; }
    set { _dropDownButtonVisible = value; _owner.OnRegionsChanged(); }
  }
  /// Gets the bounds of the toolbar including the graphic adornments
  //[Browsable(false)]
  IRECT SuperBounds {
    get { return FromLTRB(Bounds.l - _padding RCX(&), Bounds.t, _dropDownButton.Bounds.r, Bounds.b); }
  }
  /// Gets the dropdown button
  //[Browsable(false)]
  //[DesignerSerializationVisibility( DesignerSerializationVisibility.Hidden)]
  RibbonButton _dropDownButton {
    get { return _dropDownButton; }
  }
  /// Gets or sets the padding (internal) of the toolbar
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  _padding _padding {
    get { return _padding; }
  }
  /// Gets or sets the margin (external) of the toolbar
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  _padding _margin {
    get { return _margin; }
  }
  /// Gets or sets a value indicating if the button that shows the menu of the
  /// QuickAccess toolbar should be visible
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool MenuButtonVisible {
    get { return _menuButtonVisible; }
    set { _menuButtonVisible = value; }
  }
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  RibbonMouseSensor Sensor {
    get { return _sensor; }
  }
  /// Gets the
  //[DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
  RibbonQuickAccessToolbarItemCollection _items {
    get
    {
      if (DropDownButtonVisible) {
        if (!_items.Contains(_dropDownButton)) {
          _items.Add(_dropDownButton);
        }
      }
      else
      {
        if (_items.Contains(_dropDownButton)) {
          _items.Remove(_dropDownButton);
        }
      }
      return _items;
    }
  }
  //#region Methods
#endif
  ISIZE MeasureSize(bool DesignMode, int sizeMode, const font_t* fo) {
    ISIZE result = {0};
    ///For RibbonItemGroup, size is always compact, and it's designed to be on an horizontal flow
    ///tab panel.
    ///
    int widthSum = RCX(&_padding);
    int maxHeight = 16;
    int i;
    for (i = 0; i < _items_count; ++i) {
      RibbonButton* item = _items + i;
      if (item == _dropDownButton) {
        continue;
      }
      ISIZE s = item->MeasureSize(RibbonElementSizeMode_Compact, fo);
      widthSum += s.w + 1;
      maxHeight = MAX(maxHeight, s.h);
    }
    widthSum -= 1;
    if (DesignMode) {
      widthSum += 16;
    }
    result = iSIZE(widthSum, maxHeight);
    //SetLastMeasuredSize(result);
    return result;
  }
  void SetBounds(IRECT bounds, const font_t* fo) {
    //base.SetBounds(bounds);
    int curLeft = bounds.l + _padding.l;
    int i;
    for (i = 0; i < _items_count; ++i) {
      RibbonButton* item = _items + i;
      ISIZE s = item->MeasureSize(RibbonElementSizeMode_Compact, fo);
      item->c->SetBounds(iRECT2(curLeft, bounds.t, s.w, s.h));
      curLeft = item->c->_bounds.r + 1;
    }
    _dropDownButton->c->SetBounds(iRECT2(bounds.r + RCH(&bounds) / 2 + 2, bounds.t, 12, RCH(&bounds)));
  }
#if 0
  //#region IContainsRibbonComponents Members
  IEnumerable<System.ComponentModel.Component> GetAllChildComponents() {
    return _items.ToArray();
  }
  //#region IContainsSelectableRibbonItems Members
  IEnumerable<RibbonItem> GetItems() {
    return _items;
  }
  IRECT GetContentBounds() {
    return Bounds;
  }
#endif
};
/// Provides a Ribbon toolbar
//[Designer(typeof(RibbonDesigner))]
struct Ribbon {
  //#region Static
  const font_t* fo;
  const char* _text;
  //#region Fields
  IRECT _tabsMargin;
  IRECT _tabsPadding;
  IRECT _tabContentMargin;
  IRECT _tabContentPadding;
  IRECT _panelPadding;
  IRECT _panelMargin;
  IRECT _itemMargin;
  IRECT _itemPadding;
  //RibbonMouseSensor* _sensor;
  IRECT _dropDownMargin;
  IRECT _tabTextMargin;
  IRECT _orbBounds;
  float _tabSum;
  bool ForceOrbMenu;
  bool _updatingSuspended;
  bool _orbSelected;
  bool _orbPressed;
  bool _orbVisible;
  bool _minimized;
  img_t* _orbImage;
  bool _quickAcessVisible;
  int _tabSpacing;
  int _panelSpacing;
  int _captionBarHeight;
  int _borderMode;
  int _actualBorderMode;
  ISIZE _lastSizeMeasured;
  bool _CaptionButtonsVisible;
  RibbonTab* _activeTab;
  RibbonTab* _lastSelectedTab;
  RibbonTab* _tabs;
  int _tabs_count;
  RibbonQuickAccessToolbar _quickAcessToolbar[1];
  RibbonOrbDropDown* _orbDropDown;
  RibbonButton _CloseButton[1];
  RibbonButton _MaximizeRestoreButton[1];
  RibbonButton _MinimizeButton[1];
  int _sizeMode;
#if 0
  RibbonRenderer* _renderer;
  RibbonContextCollection _contexts;
  GlobalHook _mouseHook;
  GlobalHook _keyboardHook;
  //#region Events
  /// Occours when the Orb is clicked
  event EventHandler OrbClicked;
  /// Occours when the Orb is double-clicked
  event EventHandler OrbDoubleClick;
  /// Occours when the <see cref="_activeTab"/> property value has changed
  event EventHandler ActiveTabChanged;
  /// Occours when the <see cref="ActualBorderMode"/> property has changed
  event EventHandler ActualBorderModeChanged;
  /// Occours when the <see cref="_CaptionButtonsVisible"/> property value has changed
  event EventHandler CaptionButtonsVisibleChanged;
  //#region Ctor
#endif
  /// Creates a new Ribbon control
  int init() {
    _captionBarHeight = 24;
    _tabsMargin = iRECT(12, 24 + 2, 20, 0);
    _tabTextMargin = iRECT(4, 2, 4, 2);
    _tabsPadding = iRECT(8, 5, 8, 3);
    _tabContentMargin = iRECT(1, 0, 1, 2);
    _panelPadding = iRECT(3, 3, 3, 3);
    _panelMargin = iRECT(3, 2, 3, 15);
    _panelSpacing = 3;
    _itemPadding = iRECT(1, 0, 1, 0);
    _itemMargin = iRECT(4, 2, 4, 2);
    _tabSpacing = 6;
    _dropDownMargin = iRECT(2, 2, 2, 2);
    _orbVisible = true;
    _quickAcessVisible = true;
    _borderMode = RibbonWindowMode_NonClientAreaGlass;
#if 0
    _quickAcessToolbar = new RibbonQuickAccessToolbar(this);
    _orbDropDown = new RibbonOrbDropDown(this);
    _renderer = new RibbonProfessionalRenderer();
    _MinimizeButton = new RibbonCaptionButton(RibbonCaptionButton.CaptionButton.Minimize);
    _MaximizeRestoreButton = new RibbonCaptionButton(RibbonCaptionButton.CaptionButton.Maximize);
    _CloseButton = new RibbonCaptionButton(RibbonCaptionButton.CaptionButton.Close);
    _MinimizeButton.SetOwner(this);
    _MaximizeRestoreButton.SetOwner(this);
    _CloseButton.SetOwner(this);
    Font = SystemFonts.CaptionFont;
    SetStyle(ControlStyles_ResizeRedraw, true);
    SetStyle(ControlStyles_Selectable, false);
    SetStyle(ControlStyles_UserPaint, true);
    SetStyle(ControlStyles_OptimizedDoubleBuffer, true);
    //DoubleBuffered = true;
    Dock = DockStyle.t;
    _tabs = new RibbonTabCollection(this);
    _contexts = new RibbonContextCollection(this);
    Disposed += new EventHandler(Ribbon_Disposed);
#endif
    return 0;
  }
#if 0
  ~Ribbon() {
    if (_mouseHook != NULL) {
      _mouseHook.Dispose();
    }
  }
  //#region Props
  IRECT CaptionTextBounds {
    get
    {
      int left = 0;
      if (OrbVisible) {
        left = _orbBounds.r;
      }
      if (QuickAccessVisible) {
        left = _quickAcessToolbar.Bounds.r + 20;
      }
      if (QuickAccessVisible && _quickAcessToolbar.DropDownButtonVisible) {
        left = _quickAcessToolbar.DropDownButton.Bounds.r;
      }
      IRECT r = FromLTRB(left, 0, Width - 100, CaptionBarSize);
      return r;
    }
  }
  /// Gets if the caption buttons are currently visible, according to the value specified in <see cref="BorderMode"/>
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool _CaptionButtonsVisible {
    get { return _CaptionButtonsVisible; }
  }
  /// Gets the Ribbon's close button
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  RibbonCaptionButton _CloseButton {
    get { return _CloseButton; }
  }
  /// Gets the Ribbon's maximize-restore button
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  RibbonCaptionButton MaximizeRestoreButton {
    get { return _MaximizeRestoreButton; }
  }
  /// Gets the Ribbon's minimize button
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  RibbonCaptionButton MinimizeButton {
    get { return _MinimizeButton; }
  }
  /// Gets or sets the RibbonFormHelper object if the parent form is IRibbonForm
  //[Browsable(false)]
  RibbonFormHelper FormHelper {
    get
    {
      IRibbonForm irf = Parent as IRibbonForm;
      if (irf != NULL) {
        return irf.Helper;
      }
      return NULL;
    }
  }
  /// Gets the actual <see cref="RibbonWindowMode"/> that the ribbon has.
  /// It's value may vary from <see cref="BorderMode"/>
  /// because of computer and operative system capabilities.
  //[Browsable(false)]
  RibbonWindowMode ActualBorderMode {
    get { return _actualBorderMode; }
  }
  /// Gets or sets the border mode of the ribbon relative to the window where it is contained
  //[DefaultValue(RibbonWindowMode_NonClientAreaGlass)]
  //[Browsable(true)]
  //[DesignerSerializationVisibility( DesignerSerializationVisibility.Visible)]
  [Description("Specifies how the Ribbon is placed on the window border and the non-client area")]
  RibbonWindowMode BorderMode {
    get { return _borderMode; }
    set
    {
      _borderMode = value;
      RibbonWindowMode actual = value;
      if (value == RibbonWindowMode_NonClientAreaGlass && !WinApi.IsGlassEnabled) {
        actual = RibbonWindowMode_NonClientAreaCustomDrawn;
      }
      if (FormHelper == NULL || (value == RibbonWindowMode_NonClientAreaCustomDrawn && Environment.OSVersion.Platform != PlatformID.Win32NT)) {
        actual = RibbonWindowMode_InsideWindow;
      }
      SetActualBorderMode(actual);
    }
  }
  /// Gets the Orb's DropDown
  //[DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
  //[Browsable(false)]
  RibbonOrbDropDown OrbDropDown {
    get { return _orbDropDown; }
  }
  /// Gets or sets a value indicating if the QuickAcess toolbar should be visible
  //[DefaultValue(true)]
  [Description("Shows or hides the QuickAccess toolbar")]
  //[DesignerSerializationVisibility(DesignerSerializationVisibility.Visible)]
  bool QuickAccessVisible {
    get { return _quickAcessVisible; }
    set { _quickAcessVisible = value; }
  }
  /// Gets  the _quickAcessToolbar
  //[DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
  RibbonQuickAccessToolbar _quickAcessToolbar {
    get { return _quickAcessToolbar; }
  }
  /// Gets or sets the img_t of the orb
  //[DesignerSerializationVisibility(DesignerSerializationVisibility.Visible)]
  img_t _orbImage {
    get { return _orbImage; }
    set { _orbImage = value; Invalidate(_orbBounds); }
  }
  /// Gets or sets if the Ribbon should show an orb on the corner
  //[DefaultValue(true)]
  //[DesignerSerializationVisibility(DesignerSerializationVisibility.Visible)]
  bool OrbVisible {
    get { return _orbVisible; }
    set { _orbVisible = value; OnRegionsChanged(); }
  }
  /// Gets or sets if the Orb is currently selected
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool _orbSelected {
    get { return _orbSelected; }
    set { _orbSelected = value; }
  }
  /// Gets or sets if the Orb is currently pressed
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  bool _orbPressed {
    get { return _orbPressed; }
    set { _orbPressed = value; Invalidate(_orbBounds); }
  }
  /// Gets the Height of the caption bar
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  int CaptionBarSize {
    get { return CaptionBarHeight; }
  }
  /// Gets the bounds of the orb
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  IRECT _orbBounds;
  {
    get {
      if (OrbVisible) {
        return iRECT(4, 4, 36, 36);
      }
      else
      {
        return iRECT(4, 4, 0, 0);
      }
    }
  }
  /// Gets the next tab to be activated
  /// <returns></returns>
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  RibbonTab NextTab {
    get {
      if (_activeTab == NULL || _tabs.Count == 0) {
        if (_tabs.Count == 0) {
          return NULL;
        }
        return _tabs[0];
      }
      int index = _tabs.IndexOf(_activeTab);
      if (index == _tabs.Count - 1) {
        return _activeTab;
      }
      else
      {
        return _tabs[index + 1];
      }
    }
  }
  /// Gets the next tab to be activated
  /// <returns></returns>
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  RibbonTab PreviousTab {
    get
    {
      if (_activeTab == NULL || _tabs.Count == 0) {
        if (_tabs.Count == 0) {
          return NULL;
        }
        return _tabs[0];
      }
      int index = _tabs.IndexOf(_activeTab);
      if (index == 0) {
        return _activeTab;
      }
      else
      {
        return _tabs[index - 1];
      }
    }
  }
  /// Overriden. The maximum size is fixed
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  ISIZE _maximumSize;//     return new System.Drawing.ISIZE(0, 138); //115 was the old one
  /// Overriden. The minimum size is fixed
  //[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
  ISIZE _minimumSize;//      return new System.Drawing.ISIZE(0, 138); //115);
  void _mouseHook_MouseDown(object sender, MouseEventArgs e) {
    if (!RectangleToScreen(_orbBounds).Contains(e.Location)) {
      RibbonPopupManager.FeedHookClick(e);
    }
  }
  /// Checks if MouseWheel should be raised
  /// <param name="sender"></param>
  void _mouseHook_MouseWheel(object sender, MouseEventArgs e) {
    if (!RibbonPopupManager.FeedMouseWheel(e)) {
      if (RectangleToScreen(
          iRECT(IPOINT.Empty, ISIZE)
          ).Contains(e.Location)) {
        OnMouseWheel(e);
      }
    }
  }
  /// Raises the OrbClicked event
  /// <param name="e">event data</param>
  void OnOrbClicked(EventArgs e) {
    if (_orbPressed) {
      OrbDropDown.Close();
    }
    else {
      ShowOrbDropDown();
    }
    if (OrbClicked != NULL) {
      OrbClicked(this, e);
    }
  }
  /// Raises the OrbDoubleClicked
  void OnOrbDoubleClicked(EventArgs e) {
    if (OrbDoubleClick != NULL) {
      OrbDoubleClick(this, e);
    }
  }
  //#region Methods
  /// Initializes hooks
  void SetUpHooks() {
    if (!(Site != NULL && Site.DesignMode)) {
      _mouseHook = new GlobalHook(GlobalHook.HookTypes.Mouse);
      _mouseHook.MouseWheel += new MouseEventHandler(_mouseHook_MouseWheel);
      _mouseHook.MouseDown += new MouseEventHandler(_mouseHook_MouseDown);
      _keyboardHook = new GlobalHook(GlobalHook.HookTypes.Keyboard);
      _keyboardHook.KeyDown += new KeyEventHandler(_keyboardHook_KeyDown);
    }
  }
  void _keyboardHook_KeyDown(object sender, KeyEventArgs e) {
    if (e.KeyCode == Keys.Escape) {
      RibbonPopupManager.Dismiss(RibbonPopupManager.DismissReason.EscapePressed);
    }
  }
  /// Catches the disposal of the ribbon
  /// <param name="sender"></param>
  void Ribbon_Disposed(object sender, EventArgs e) {
    if (_mouseHook != NULL) {
      _mouseHook.Dispose();
    }
    if (_keyboardHook != NULL) {
      _keyboardHook.Dispose();
    }
  }
  /// Shows the Orb's dropdown
  void ShowOrbDropDown() {
    _orbPressed = true;
    OrbDropDown.Show(PointToScreen(new IPOINT(_orbBounds.X - 4, _orbBounds.b - OrbDropDown.ContentMargin.t + 2)));
  }
#endif
  /// Drops out the old sensor and creates a new one
  void RenewSensor() {
#if 0
    if (_activeTab == NULL) {
      return;
    }
    if (Sensor != NULL) {
      Sensor.Dispose();
    }
    _sensor = new RibbonMouseSensor(this, this, _activeTab);
    if (_CaptionButtonsVisible) {
      Sensor._items.AddRange(new RibbonItem[] { _CloseButton, MaximizeRestoreButton, MinimizeButton });
    }
#endif
  }
#if 0
  /// Sets the value of the <see cref="BorderMode"/> property
  /// <param name="borderMode">Actual border mode accquired</param>
  void SetActualBorderMode(RibbonWindowMode borderMode) {
    bool trigger = _actualBorderMode != borderMode;
    _actualBorderMode = borderMode;
    if (trigger) {
      OnActualBorderModeChanged(EventArgs_Empty);
    }
    SetCaptionButtonsVisible(borderMode == RibbonWindowMode_NonClientAreaCustomDrawn);
  }
  /// Sets the value of the <see cref="_CaptionButtonsVisible"/> property
  /// <param name="visible">Value to set to the caption buttons</param>
  void SetCaptionButtonsVisible(bool visible) {
    bool trigger = _CaptionButtonsVisible != visible;
    _CaptionButtonsVisible = visible;
    if (trigger) {
      OnCaptionButtonsVisibleChanged(EventArgs_Empty);
    }
  }
  /// Suspends any drawing/regions update operation
  void SuspendUpdating() {
    _updatingSuspended = true;
  }
  /// Resumes any drawing/regions update operation
  /// <param name="update"></param>
  void ResumeUpdating() {
    ResumeUpdating(true);
  }
  /// Resumes any drawing/regions update operation
  /// <param name="update"></param>
  void ResumeUpdating(bool update) {
    _updatingSuspended = false;
    if (update) {
      OnRegionsChanged();
    }
  }
  /// Removes all helper controls placed by any reason.
  /// Contol's visibility is set to false before removed.
  void RemoveHelperControls() {
    RibbonPopupManager.Dismiss(RibbonPopupManager.DismissReason.AppClicked);
    while (Controls.Count > 0) {
      Control ctl = Controls[0];
      ctl.Visible = false;
      Controls.Remove(ctl);
    }
  }
  /// Hittest on tab
  /// <param name="x"></param>
  /// <param name="y"></param>
  /// <returns>true if a tab has been clicked</returns>
  bool TabHitTest(int x, int y) {
    if (FromLTRB(Right - 10, Bottom - 10, Right, Bottom).Contains(x, y)) {
      MessageBox.Show(cr);
    }
    //look for mouse on tabs
    foreach(RibbonTab tab in _tabs) {
      if (tab->_bounds.Contains(x, y)) {
        _activeTab = tab;
        return true;
      }
    }
    return false;
  }
  /// Updates the regions of the tabs and the tab content bounds of the active tab
  void UpdateRegions() {
    UpdateRegions(NULL);
  }
#endif
  /// Updates the regions of the tabs and the tab content bounds of the active tab
  int UpdateRegions(IRECT rc, bool DesignMode, const font_t* fo) {
    int i;
    IRECT ClientRectangle = rc;
    ISIZE ClientSize = iSIZE(RCW(&rc), RCH(&rc));
    if (_updatingSuspended) {
      return 0;
    }
    ///Graphics for measurement
    ///X coordinate reminder
    int curLeft = _tabsMargin.l + RCW(&_orbBounds);
    ///Saves the width of the larger tab
    int maxWidth = 0;
    ///Saves the bottom of the tabs
    int tabsBottom = 0;
    //#region Assign default tab bounds (best case)
    for (i = 0; i < _tabs_count; ++i) {
      RibbonTab* tab = _tabs + i;
      ISIZE tabSize = tab->MeasureSize(fo);
      IRECT bounds = iRECT(curLeft, _tabsMargin.t,
          _tabsPadding.l + tabSize.w + _tabsPadding.r,
          _tabsPadding.t + tabSize.h + _tabsPadding.b);
      tab->_bounds = bounds;
      curLeft = bounds.r + _tabSpacing;
      maxWidth = MAX(RCW(&bounds), maxWidth);
      tabsBottom = MAX(bounds.b, tabsBottom);
      tab->_tabContentBounds = FromLTRB(
          _tabContentMargin.l, tabsBottom + _tabContentMargin.t,
          ClientSize.w - 1 - _tabContentMargin.r, ClientSize.h - 1 - _tabContentMargin.b);
      if (tab->_active) {
        tab->UpdatePanelsRegions(_tabContentMargin, _panelPadding, _panelMargin, _itemPadding, _panelSpacing, DesignMode, _sizeMode, fo);
      }
    }
    //#region Reduce bounds of tabs if needed
    while (curLeft > ClientRectangle.r && maxWidth > 0) {
      int i;
      curLeft = _tabsMargin.l + RCW(&_orbBounds);
      maxWidth--;
      for (i = 0; i < _tabs_count; ++i) {
        RibbonTab* tab = _tabs + i;
        ISIZE sz = RCSZ(&tab->_bounds);
        if (sz.w >= maxWidth) {
          tab->SetTabBounds(iRECT2(curLeft, _tabsMargin.t, maxWidth, sz.h));
        }
        else {
          tab->SetTabBounds(iRECT(curLeft, _tabsMargin.t, sz.w, sz.h));
        }
        curLeft = tab->_bounds.r + _tabSpacing;
      }
    }
    //#region Update QuickAccess bounds
    ISIZE sz = _quickAcessToolbar->MeasureSize(DesignMode, RibbonElementSizeMode_Compact, fo);
    _quickAcessToolbar->SetBounds(iRECT2(_orbBounds.r + _quickAcessToolbar->_margin.l, _orbBounds.t - 2, sz.w, sz.h), fo);
    //#region Update Caption Buttons bounds
    if (_CaptionButtonsVisible) {
      ISIZE cbs = iSIZE(20, 20);
      int cbg = 2;
      _CloseButton->c->SetBounds(iRECT2(ClientRectangle.r - cbs.w - cbg, cbg, cbs.w, cbs.h));
      _MaximizeRestoreButton->c->SetBounds(iRECT2(_CloseButton->c->_bounds.l - cbs.w, cbg, cbs.w, cbs.h));
      _MinimizeButton->c->SetBounds(iRECT2(_MaximizeRestoreButton->c->_bounds.l - cbs.w, cbg, cbs.w, cbs.h));
    }
    //_lastSizeMeasured = ISIZE;
    RenewSensor();
    return 0;
  }
#if 0
  /// Called when the tabs collection has changed (_tabs has been added or removed)
  /// and region re-measuring is necessary
  /// <param name="tab">Added tab</param>
  void OnRegionsChanged() {
    if (_updatingSuspended) {
      return;
    }
    if (_tabs.Count == 1) {
      _activeTab = _tabs[0];
    }
    _lastSizeMeasured = ISIZE.Empty;
    Refresh();
  }
  /// Redraws the specified tab
  /// <param name="tab"></param>
  void RedrawTab(RibbonTab tab) {
    using(Graphics g = CreateGraphics()) {
      IRECT clip = FromLTRB(
          tab->_bounds.l,
          tab->_bounds.t,
          tab->_bounds.r,
          tab->_bounds.b);
      g.SetClip(clip);
      g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
      g.TextRenderingHint = System.Drawing._text.TextRenderingHint.AntiAlias;
      tab->OnPaint(this, new RibbonElementPaintEventArgs(tab->_bounds, g, RibbonElementSizeMode_None));
    }
  }
  /// Sets the currently selected tab
  /// <param name="tab"></param>
  void SetSelectedTab(RibbonTab tab) {
    if (tab == _lastSelectedTab) {
      return;
    }
    if (_lastSelectedTab != NULL) {
      _lastSelectedTab.SetSelected(false);
      RedrawTab(_lastSelectedTab);
    }
    if (tab != NULL) {
      tab->SetSelected(true);
      RedrawTab(tab);
    }
    _lastSelectedTab = tab;
  }
  /// Suspends the sensor activity
  void SuspendSensor() {
    if (Sensor != NULL) {
      Sensor.Suspend();
    }
  }
  /// Resumes the sensor activity
  void ResumeSensor() {
    Sensor.Resume();
  }
  /// Redraws the specified area on the sensor's control
  /// <param name="area"></param>
  void RedrawArea(IRECT area) {
    Sensor.Control.Invalidate(area);
  }
  /// Activates the next tab available
  void ActivateNextTab() {
    RibbonTab tab = NextTab;
    if (tab != NULL) {
      _activeTab = tab;
    }
  }
  /// Activates the previous tab available
  void ActivatePreviousTab() {
    RibbonTab tab = PreviousTab;
    if (tab != NULL) {
      _activeTab = tab;
    }
  }
  /// Handles the mouse down on the orb area
  void OrbMouseDown() {
    OnOrbClicked(EventArgs_Empty);
  }
  void WndProc(ref Message m) {
    bool bypassed = false;
    if (WinApi.IsWindows && (ActualBorderMode == RibbonWindowMode_NonClientAreaGlass || ActualBorderMode == RibbonWindowMode_NonClientAreaCustomDrawn)) {
      if (m.Msg == WinApi.WM_NCHITTEST) {
        Form f = FindForm();
        int captionLeft = QuickAccessVisible ? _quickAcessToolbar.Bounds.r : _orbBounds.r;
        if (QuickAccessVisible && _quickAcessToolbar.DropDownButtonVisible) {
          captionLeft = _quickAcessToolbar.DropDownButton.Bounds.r;
        }
        IRECT caption = FromLTRB(captionLeft, 0, Width, CaptionBarSize);
        IPOINT screenPoint = new IPOINT(WinApi.LoWord((int)m.LParam), WinApi.HiWord((int)m.LParam));
        IPOINT ribbonPoint = PointToClient(screenPoint);
        bool onCaptionButtons = false;
        if (_CaptionButtonsVisible) {
          onCaptionButtons = _CloseButton.Bounds.Contains(ribbonPoint) ||
              MinimizeButton.Bounds.Contains(ribbonPoint) ||
              MaximizeRestoreButton.Bounds.Contains(ribbonPoint);
        }
        if (RectangleToScreen(caption).Contains(screenPoint) && !onCaptionButtons) {
          IPOINT p = PointToScreen(screenPoint);
          WinApi.SendMessage(f.Handle, WinApi.WM_NCHITTEST, m.WParam, WinApi.MakeLParam(p.X, p.Y));
          m.Result = new IntPtr(-1);
          bypassed = true;
        }
      }
    }
    if (!bypassed) {
      base.WndProc(ref m);
    }
  }
#endif
#if 0
  /// Raises the <see cref="ActiveTabChanged"/> event
  /// <param name="e">Event data</param>
  void OnActiveTabChanged(EventArgs e) {
    if (ActiveTabChanged != NULL) {
      ActiveTabChanged(this, e);
    }
  }
  /// Raises the <see cref="ActualBorderMode"/> event
  void OnActualBorderModeChanged(EventArgs e) {
    if (ActualBorderModeChanged != NULL) {
      ActualBorderModeChanged(this, e);
    }
  }
  /// Overriden. Raises the MouseMove event and tunnels the message to child elements
  /// <param name="e">A <see cref="T:MouseEventArgs"></see> that contains the event data.</param>
  void OnMouseMove(MouseEventArgs e) {
    base.OnMouseMove(e);
    if (_activeTab == NULL) {
      return;
    }
    bool someTabHitted = false;
    //Check if mouse on tab
    if (_activeTab.TabContentBounds.Contains(e.X, e.Y)) {
      //Do nothing, everything is on the sensor
    }
    //Check if mouse on orb
    else if (OrbVisible && _orbBounds.Contains(e.Location) && !_orbSelected) {
      _orbSelected = true;
      Invalidate(_orbBounds);
    }
    //Check if mouse on QuickAccess toolbar
    else if (QuickAccessVisible && _quickAcessToolbar.Bounds.Contains(e.Location)) {
    }
    else {
      //look for mouse on tabs
      foreach(RibbonTab tab in _tabs) {
        if (tab->_bounds.Contains(e.X, e.Y)) {
          SetSelectedTab(tab);
          someTabHitted = true;
        }
      }
    }
    if (!someTabHitted) {
      SetSelectedTab(NULL);
    }
    if (_orbSelected && !_orbBounds.Contains(e.Location)) {
      _orbSelected = false;
      Invalidate(_orbBounds);
    }
  }
  /// Overriden. Raises the MouseUp event and tunnels the message to child elements
  /// <param name="e">A <see cref="T:MouseEventArgs"></see> that contains the event data.</param>
  void OnMouseUp(MouseEventArgs e) {
    base.OnMouseUp(e);
  }
  /// Overriden. Raises the MouseDown event and tunnels the message to child elements
  void OnMouseDown(MouseEventArgs e) {
    base.OnMouseDown(e);
    if (_orbBounds.Contains(e.Location)) {
      OrbMouseDown();
    }
    else {
      TabHitTest(e.X, e.Y);
    }
  }
  /// Handles the mouse wheel
  void OnMouseWheel(MouseEventArgs e) {
    base.OnMouseWheel(e);
    if (_tabs.Count == 0 || _activeTab == NULL) {
      return;
    }
    int index = _tabs.IndexOf(_activeTab);
    if (e.Delta < 0) {
      _tabSum += 0.4f;
    }
    else {
      _tabSum -= 0.4f;
    }
    int tabRounded = Convert.ToInt16(Math.Round(_tabSum));
    if (tabRounded != 0) {
      index += tabRounded;
      if (index < 0) {
        index = 0;
      }
      else if (index >= _tabs.Count - 1) {
        index = _tabs.Count - 1;
      }
      _activeTab = _tabs[index];
      _tabSum = 0f;
    }
  }
  /// Overriden. Raises the OnSizeChanged event and performs layout calculations
  /// <param name="e">An <see cref="T:System.EventArgs"></see> that contains the event data.</param>
  void OnSizeChanged(System.EventArgs e) {
    UpdateRegions();
    RemoveHelperControls();
    base.OnSizeChanged(e);
  }
  /// Handles when its parent has changed
  void OnParentChanged(EventArgs e) {
    base.OnParentChanged(e);
    if (!(Site != NULL && Site.DesignMode)) {
      BorderMode = BorderMode;
      if (Parent is IRibbonForm) {
        FormHelper.Ribbon = this;
      }
      SetUpHooks();
    }
  }
#endif
};

