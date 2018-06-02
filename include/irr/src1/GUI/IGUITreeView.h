// written by Reinhard Ostermeier, reinhard@nospam.r-ostermeier.de
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_GUI_TREE_VIEW_H_INCLUDED__
#define __I_GUI_TREE_VIEW_H_INCLUDED__
//#include "IGUIElement.h"
//#include "IGUIImageList.h"
class IGUIFont;
class IGUITreeView;
//! Node for gui tree view
/** \par This element can create the following events of type EGUI_EVENT_TYPE:
\li EGET_TREEVIEW_NODE_EXPAND
\li EGET_TREEVIEW_NODE_COLLAPS
\li EGET_TREEVIEW_NODE_DESELECT
\li EGET_TREEVIEW_NODE_SELECT
*/
class IGUITreeViewNode
{
public:
  //! returns the owner (tree view) of this node
  IGUITreeView* getOwner() = 0;
  //! Returns the parent node of this node.
  /** For the root node this will return 0. */
  IGUITreeViewNode* getParent() = 0;
  //! returns the text of the node
  const char* getText() = 0;
  //! sets the text of the node
  void setText(const char* text) = 0;
  //! returns the icon text of the node
  const char* getIcon() = 0;
  //! sets the icon text of the node
  void setIcon(const char* icon) = 0;
  //! returns the image index of the node
  u32 getImageIndex() = 0;
  //! sets the image index of the node
  void setImageIndex(u32 imageIndex) = 0;
  //! returns the image index of the node
  u32 getSelectedImageIndex() = 0;
  //! sets the image index of the node
  void setSelectedImageIndex(u32 imageIndex) = 0;
  //! returns the user data (void*) of this node
  void* getData() = 0;
  //! sets the user data (void*) of this node
  void setData(void* data) = 0;
  //! returns the user data2 (IReferenceCounted) of this node
  IReferenceCounted* getData2() = 0;
  //! sets the user data2 (IReferenceCounted) of this node
  void setData2(IReferenceCounted* data) = 0;
  //! returns the child item count
  u32 getChildCount() = 0;
  //! removes all children (recursive) from this node
  void clearChildren() = 0;
  //! removes all children (recursive) from this node
  /** \deprecated Deprecated in 1.8, use clearChildren() instead.
  This method may be removed by Irrlicht 1.9 */
  _IRR_DEPRECATED_ void clearChilds() {
    clearChildren();
  }
  //! returns true if this node has child nodes
  bool hasChildren() = 0;
  //! returns true if this node has child nodes
  /** \deprecated Deprecated in 1.8, use hasChildren() instead.
  This method may be removed by Irrlicht 1.9 */
  _IRR_DEPRECATED_ bool hasChilds() const {
    return hasChildren();
  }
  //! Adds a new node behind the last child node.
  /** \param text text of the new node
  \param icon icon text of the new node
  \param imageIndex index of the image for the new node (-1 = none)
  \param selectedImageIndex index of the selected image for the new node (-1 = same as imageIndex)
  \param data user data (void*) of the new node
  \param data2 user data2 (IReferenceCounted*) of the new node
  \return The new node
  */
  IGUITreeViewNode* addChildBack(
      const char* text, const char* icon = 0,
      s32 imageIndex = -1, s32 selectedImageIndex = -1,
      void* data = 0, IReferenceCounted* data2 = 0) = 0;
  //! Adds a new node before the first child node.
  /** \param text text of the new node
  \param icon icon text of the new node
  \param imageIndex index of the image for the new node (-1 = none)
  \param selectedImageIndex index of the selected image for the new node (-1 = same as imageIndex)
  \param data user data (void*) of the new node
  \param data2 user data2 (IReferenceCounted*) of the new node
  \return The new node
  */
  IGUITreeViewNode* addChildFront(
      const char* text, const char* icon = 0,
      s32 imageIndex = -1, s32 selectedImageIndex = -1,
      void* data = 0, IReferenceCounted* data2 = 0) = 0;
  //! Adds a new node behind the other node.
  /** The other node has also te be a child node from this node.
  \param other Node to insert after
  \param text text of the new node
  \param icon icon text of the new node
  \param imageIndex index of the image for the new node (-1 = none)
  \param selectedImageIndex index of the selected image for the new node (-1 = same as imageIndex)
  \param data user data (void*) of the new node
  \param data2 user data2 (IReferenceCounted*) of the new node
  \return The new node or 0 if other is no child node from this
  */
  IGUITreeViewNode* insertChildAfter(
      IGUITreeViewNode* other,
      const char* text, const char* icon = 0,
      s32 imageIndex = -1, s32 selectedImageIndex = -1,
      void* data = 0, IReferenceCounted* data2 = 0) = 0;
  //! Adds a new node before the other node.
  /** The other node has also te be a child node from this node.
  \param other Node to insert before
  \param text text of the new node
  \param icon icon text of the new node
  \param imageIndex index of the image for the new node (-1 = none)
  \param selectedImageIndex index of the selected image for the new node (-1 = same as imageIndex)
  \param data user data (void*) of the new node
  \param data2 user data2 (IReferenceCounted*) of the new node
  \return The new node or 0 if other is no child node from this
  */
  IGUITreeViewNode* insertChildBefore(
      IGUITreeViewNode* other,
      const char* text, const char* icon = 0,
      s32 imageIndex = -1, s32 selectedImageIndex = -1,
      void* data = 0, IReferenceCounted* data2 = 0) = 0;
  //! Return the first child node from this node.
  /** \return The first child node or 0 if this node has no children. */
  IGUITreeViewNode* getFirstChild() = 0;
  //! Return the last child node from this node.
  /** \return The last child node or 0 if this node has no children. */
  IGUITreeViewNode* getLastChild() = 0;
  //! Returns the previous sibling node from this node.
  /** \return The previous sibling node from this node or 0 if this is
  the first node from the parent node.
  */
  IGUITreeViewNode* getPrevSibling() = 0;
  //! Returns the next sibling node from this node.
  /** \return The next sibling node from this node or 0 if this is
  the last node from the parent node.
  */
  IGUITreeViewNode* getNextSibling() = 0;
  //! Returns the next visible (expanded, may be out of scrolling) node from this node.
  /** \return The next visible node from this node or 0 if this is
  the last visible node. */
  IGUITreeViewNode* getNextVisible() = 0;
  //! Deletes a child node.
  /** \return Returns true if the node was found as a child and is deleted. */
  bool deleteChild(IGUITreeViewNode* child) = 0;
  //! Moves a child node one position up.
  /** \return True if the node was found as achild node and was not already the first child. */
  bool moveChildUp(IGUITreeViewNode* child) = 0;
  //! Moves a child node one position down.
  /** \return True if the node was found as achild node and was not already the last child. */
  bool moveChildDown(IGUITreeViewNode* child) = 0;
  //! Returns true if the node is expanded (children are visible).
  bool getExpanded() = 0;
  //! Sets if the node is expanded.
  void setExpanded(bool expanded) = 0;
  //! Returns true if the node is currently selected.
  bool getSelected() = 0;
  //! Sets this node as selected.
  void setSelected(bool selected) = 0;
  //! Returns true if this node is the root node.
  bool isRoot() = 0;
  //! Returns the level of this node.
  /** The root node has level 0. Direct children of the root has level 1 ... */
  s32 getLevel() = 0;
  //! Returns true if this node is visible (all parents are expanded).
  bool isVisible() = 0;
};
//! Default tree view GUI element.
/** Displays a windows like tree buttons to expand/collaps the child
nodes of an node and optional tree lines. Each node consits of an
text, an icon text and a void pointer for user data. */
class IGUITreeView : public IGUIElement
{
public:
  //! constructor
  IGUITreeView(IGUIEnvironment* environment, IGUIElement* parent,
      s32 id, IRECT rectangle)
    : IGUIElement(EGUIET_TREE_VIEW, environment, parent, id, rectangle) {}
  //! returns the root node (not visible) from the tree.
  IGUITreeViewNode* getRoot() = 0;
  //! returns the selected node of the tree or 0 if none is selected
  IGUITreeViewNode* getSelected() = 0;
  //! returns true if the tree lines are visible
  bool getLinesVisible() = 0;
  //! sets if the tree lines are visible
  /** \param visible true for visible, false for invisible */
  void setLinesVisible(bool visible) = 0;
  //! Sets the font which should be used as icon font.
  /** This font is set to the Irrlicht engine built-in-font by
  default. Icons can be displayed in front of every list item.
  An icon is a string, displayed with the icon font. When using
  the build-in-font of the Irrlicht engine as icon font, the icon
  strings defined in GUIIcons.h can be used.
  */
  void setIconFont(IGUIFont* font) = 0;
  //! Sets the image list which should be used for the image and selected image of every node.
  /** The default is 0 (no images). */
  void setImageList(IGUIImageList* imageList) = 0;
  //! Returns the image list which is used for the nodes.
  IGUIImageList* getImageList() = 0;
  //! Sets if the image is left of the icon. Default is true.
  void setImageLeftOfIcon(bool bLeftOf) = 0;
  //! Returns if the Image is left of the icon. Default is true.
  bool getImageLeftOfIcon() = 0;
  //! Returns the node which is associated to the last event.
  /** This pointer is only valid inside the OnEvent call! */
  IGUITreeViewNode* getLastEventNode() = 0;
};
#endif

