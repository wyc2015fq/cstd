// This file is part of the "Irrlicht Engine".
// written by Reinhard Ostermeier, reinhard@nospam.r-ostermeier.de
#ifndef __C_GUI_TREE_VIEW_H_INCLUDED__
#define __C_GUI_TREE_VIEW_H_INCLUDED__
//#include "IGUITreeView.h"
//#include "irrList.h"
// forward declarations
class IGUIFont;
class IGUIScrollBar;
class CGUITreeView;
//! Node for gui tree view
class CGUITreeViewNode : public IGUITreeViewNode
{
  friend class CGUITreeView;
public:
  //! constructor
  CGUITreeViewNode(CGUITreeView* owner, CGUITreeViewNode* parent);
  //! destructor
  ~CGUITreeViewNode();
  //! returns the owner (tree view) of this node
  IGUITreeView* getOwner() const;
  //! Returns the parent node of this node.
  IGUITreeViewNode* getParent() const;
  //! returns the text of the node
  const char* getText() const {
    return Text;
  }
  //! sets the text of the node
  void setText(const char* text);
  //! returns the icon text of the node
  const char* getIcon() const {
    return Icon;
  }
  //! sets the icon text of the node
  void setIcon(const char* icon);
  //! returns the image index of the node
  u32 getImageIndex() const {
    return ImageIndex;
  }
  //! sets the image index of the node
  void setImageIndex(u32 imageIndex) {
    ImageIndex = imageIndex;
  }
  //! returns the image index of the node
  u32 getSelectedImageIndex() const {
    return SelectedImageIndex;
  }
  //! sets the image index of the node
  void setSelectedImageIndex(u32 imageIndex) {
    SelectedImageIndex = imageIndex;
  }
  //! returns the user data (void*) of this node
  void* getData() const {
    return Data;
  }
  //! sets the user data (void*) of this node
  void setData(void* data) {
    Data = data;
  }
  //! returns the user data2 (IReferenceCounted) of this node
  IReferenceCounted* getData2() const {
    return Data2;
  }
  //! sets the user data2 (IReferenceCounted) of this node
  void setData2(IReferenceCounted* data) {
    if (Data2) {
      Data2->drop();
    }
    Data2 = data;
    if (Data2) {
      Data2->grab();
    }
  }
  //! returns the child item count
  u32 getChildCount() const {
    return Children.getSize();
  }
  //! removes all children (recursive) from this node
  void clearChildren();
  //! returns true if this node has child nodes
  bool hasChildren() const {
    return !Children.empty();
  }
  //! Adds a new node behind the last child node.
  //! \param text text of the new node
  //! \param icon icon text of the new node
  //! \param imageIndex index of the image for the new node (-1 = none)
  //! \param selectedImageIndex index of the selected image for the new node (-1 = same as imageIndex)
  //! \param data user data (void*) of the new node
  //! \param data2 user data2 (IReferenceCounted*) of the new node
  //! \return
  //! returns the new node
  IGUITreeViewNode* addChildBack(
      const char* text,
      const char* icon = 0,
      s32 imageIndex = -1,
      s32 selectedImageIndex = -1,
      void* data = 0,
      IReferenceCounted* data2 = 0);
  //! Adds a new node before the first child node.
  //! \param text text of the new node
  //! \param icon icon text of the new node
  //! \param imageIndex index of the image for the new node (-1 = none)
  //! \param selectedImageIndex index of the selected image for the new node (-1 = same as imageIndex)
  //! \param data user data (void*) of the new node
  //! \param data2 user data2 (IReferenceCounted*) of the new node
  //! \return
  //! returns the new node
  IGUITreeViewNode* addChildFront(
      const char*    text,
      const char*    icon = 0,
      s32         imageIndex = -1,
      s32         selectedImageIndex = -1,
      void*         data = 0,
      IReferenceCounted*      data2 = 0);
  //! Adds a new node behind the other node.
  //! The other node has also te be a child node from this node.
  //! \param text text of the new node
  //! \param icon icon text of the new node
  //! \param imageIndex index of the image for the new node (-1 = none)
  //! \param selectedImageIndex index of the selected image for the new node (-1 = same as imageIndex)
  //! \param data user data (void*) of the new node
  //! \param data2 user data2 (IReferenceCounted*) of the new node
  //! \return
  //! returns the new node or 0 if other is no child node from this
  IGUITreeViewNode* insertChildAfter(
      IGUITreeViewNode* other,
      const char*    text,
      const char*    icon = 0,
      s32         imageIndex = -1,
      s32         selectedImageIndex = -1,
      void*         data = 0,
      IReferenceCounted*      data2 = 0);
  //! Adds a new node before the other node.
  //! The other node has also te be a child node from this node.
  //! \param text text of the new node
  //! \param icon icon text of the new node
  //! \param imageIndex index of the image for the new node (-1 = none)
  //! \param selectedImageIndex index of the selected image for the new node (-1 = same as imageIndex)
  //! \param data user data (void*) of the new node
  //! \param data2 user data2 (IReferenceCounted*) of the new node
  //! \return
  //! returns the new node or 0 if other is no child node from this
  IGUITreeViewNode* insertChildBefore(
      IGUITreeViewNode* other,
      const char*    text,
      const char*    icon = 0,
      s32         imageIndex = -1,
      s32         selectedImageIndex = -1,
      void*         data = 0,
      IReferenceCounted*      data2 = 0);
  //! Return the first child note from this node.
  IGUITreeViewNode* getFirstChild() const;
  //! Return the last child note from this node.
  IGUITreeViewNode* getLastChild() const;
  //! Returns the preverse sibling node from this node.
  IGUITreeViewNode* getPrevSibling() const;
  //! Returns the next sibling node from this node.
  IGUITreeViewNode* getNextSibling() const;
  //! Returns the next visible (expanded, may be out of scrolling) node from this node.
  IGUITreeViewNode* getNextVisible() const;
  //! Deletes a child node.
  bool deleteChild(IGUITreeViewNode* child);
  //! Moves a child node one position up.
  bool moveChildUp(IGUITreeViewNode* child);
  //! Moves a child node one position down.
  bool moveChildDown(IGUITreeViewNode* child);
  //! Returns true if the node is expanded (children are visible).
  bool getExpanded() const {
    return Expanded;
  }
  //! Sets if the node is expanded.
  void setExpanded(bool expanded);
  //! Returns true if the node is currently selected.
  bool getSelected() const;
  //! Sets this node as selected.
  void setSelected(bool selected);
  //! Returns true if this node is the root node.
  bool isRoot() const;
  //! Returns the level of this node.
  s32 getLevel() const;
  //! Returns true if this node is visible (all parents are expanded).
  bool isVisible() const;
private:
  CGUITreeView*     Owner;
  CGUITreeViewNode*   Parent;
  char*     Text;
  char*     Icon;
  s32       ImageIndex;
  s32       SelectedImageIndex;
  void*       Data;
  IReferenceCounted*    Data2;
  bool        Expanded;
  list<CGUITreeViewNode*> Children;
};
//! Default tree view GUI element.
class CGUITreeView : public IGUITreeView
{
  friend class CGUITreeViewNode;
public:
  //! constructor
  CGUITreeView(IGUIEnvironment* environment, IGUIElement* parent,
      s32 id, IRECT rectangle, bool clip = true,
      bool drawBack = false, bool scrollBarVertical = true, bool scrollBarHorizontal = true);
  //! destructor
  ~CGUITreeView();
  //! returns the root node (not visible) from the tree.
  IGUITreeViewNode* getRoot() const {
    return Root;
  }
  //! returns the selected node of the tree or 0 if none is selected
  IGUITreeViewNode* getSelected() const {
    return Selected;
  }
  //! returns true if the tree lines are visible
  bool getLinesVisible() const {
    return LinesVisible;
  }
  //! sets if the tree lines are visible
  void setLinesVisible(bool visible) {
    LinesVisible = visible;
  }
  //! called if an event happened.
  bool OnEvent(const SEvent& event);
  //! draws the element and its children
  void draw();
  //! Sets the font which should be used as icon font. This font is set to the Irrlicht engine
  //! built-in-font by default. Icons can be displayed in front of every list item.
  //! An icon is a string, displayed with the icon font. When using the build-in-font of the
  //! Irrlicht engine as icon font, the icon strings defined in GUIIcons.h can be used.
  void setIconFont(IGUIFont* font);
  //! Sets the image list which should be used for the image and selected image of every node.
  //! The default is 0 (no images).
  void setImageList(IGUIImageList* imageList);
  //! Returns the image list which is used for the nodes.
  IGUIImageList* getImageList() const {
    return ImageList;
  }
  //! Sets if the image is left of the icon. Default is true.
  void setImageLeftOfIcon(bool bLeftOf) {
    ImageLeftOfIcon = bLeftOf;
  }
  //! Returns if the Image is left of the icon. Default is true.
  bool getImageLeftOfIcon() const {
    return ImageLeftOfIcon;
  }
  //! Returns the node which is associated to the last event.
  IGUITreeViewNode* getLastEventNode() const {
    return LastEventNode;
  }
private:
  //! calculates the heigth of an node and of all visible nodes.
  void recalculateItemHeight();
  //! executes an mouse action (like selectNew of CGUIListBox)
  void mouseAction(s32 xpos, s32 ypos, bool onlyHover = false);
  CGUITreeViewNode* Root;
  IGUITreeViewNode* Selected;
  s32     ItemHeight;
  s32     IndentWidth;
  s32     TotalItemHeight;
  s32     TotalItemWidth;
  IGUIFont*   Font;
  IGUIFont*   IconFont;
  IGUIScrollBar*    ScrollBarH;
  IGUIScrollBar*    ScrollBarV;
  IGUIImageList*    ImageList;
  IGUITreeViewNode* LastEventNode;
  bool      LinesVisible;
  bool      Selecting;
  bool      Clip;
  bool      DrawBack;
  bool      ImageLeftOfIcon;
};
#endif

