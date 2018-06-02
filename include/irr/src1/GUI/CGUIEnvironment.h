// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_ENVIRONMENT_H_INCLUDED__
#define __C_GUI_ENVIRONMENT_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUIEnvironment.h"
//#include "IGUIElement.h"
//#include "irrArray.h"
//#include "IFileSystem.h"
//#include "IOSOperator.h"
class IXMLWriter;
}
class CGUIEnvironment : public IGUIEnvironment, public IGUIElement
{
public:
  //! constructor
  CGUIEnvironment(IFileSystem* fs, IVideoDriver* driver, IOSOperator* op);
  //! destructor
  ~CGUIEnvironment();
  //! draws all gui elements
  void drawAll();
  //! returns the current video driver
  IVideoDriver* getVideoDriver() const;
  //! returns pointer to the filesystem
  IFileSystem* getFileSystem() const;
  //! returns a pointer to the OS operator
  IOSOperator* getOSOperator() const;
  //! posts an input event to the environment
  bool postEventFromUser(const SEvent& event);
  //! This sets a new event receiver for gui events. Usually you do not have to
  //! use this method, it is used by the internal engine.
  void setUserEventReceiver(IEventReceiver* evr);
  //! removes all elements from the environment
  void clear();
  //! called if an event happened.
  bool OnEvent(const SEvent& event);
  //! returns the current gui skin
  IGUISkin* getSkin() const;
  //! Sets a new GUI Skin
  void setSkin(IGUISkin* skin);
  //! Creates a new GUI Skin based on a template.
  /** \return Returns a pointer to the created skin.
  If you no longer need the skin, you should call IGUISkin::drop().
  See IReferenceCounted::drop() for more information. */
  IGUISkin* createSkin(EGUI_SKIN_TYPE type);
  //! Creates the image list from the given texture.
  IGUIImageList* createImageList(ITexture* texture,
      dimension2d<s32> imageSize, bool useAlphaChannel);
  //! returns the font
  IGUIFont* getFont(const char* filename);
  //! add an externally loaded font
  IGUIFont* addFont(const char* name, IGUIFont* font);
  //! remove loaded font
  void removeFont(IGUIFont* font);
  //! returns default font
  IGUIFont* getBuiltInFont() const;
  //! returns the sprite bank
  IGUISpriteBank* getSpriteBank(const char* filename);
  //! returns the sprite bank
  IGUISpriteBank* addEmptySpriteBank(const char* name);
  //! adds an button. The returned pointer must not be dropped.
  IGUIButton* addButton(const IRECT& rectangle, IGUIElement* parent = 0, s32 id = -1, const char* text = 0, const char* tooltiptext = 0);
  //! adds a window. The returned pointer must not be dropped.
  IGUIWindow* addWindow(const IRECT& rectangle, bool modal = false,
      const char* text = 0, IGUIElement* parent = 0, s32 id = -1);
  //! adds a modal screen. The returned pointer must not be dropped.
  IGUIElement* addModalScreen(IGUIElement* parent);
  //! Adds a message box.
  IGUIWindow* addMessageBox(const char* caption, const char* text = 0,
      bool modal = true, s32 flag = EMBF_OK, IGUIElement* parent = 0, s32 id = -1, ITexture* image = 0);
  //! adds a scrollbar. The returned pointer must not be dropped.
  IGUIScrollBar* addScrollBar(bool horizontal, const IRECT& rectangle,
      IGUIElement* parent = 0, s32 id = -1);
  //! Adds an image element.
  IGUIImage* addImage(ITexture* image, position2di pos,
      bool useAlphaChannel = true, IGUIElement* parent = 0, s32 id = -1, const char* text = 0);
  //! adds an image. The returned pointer must not be dropped.
  IGUIImage* addImage(const IRECT& rectangle,
      IGUIElement* parent = 0, s32 id = -1, const char* text = 0, bool useAlphaChannel = true);
  //! adds a checkbox
  IGUICheckBox* addCheckBox(bool checked, const IRECT& rectangle, IGUIElement* parent = 0, s32 id = -1, const char* text = 0);
  //! adds a list box
  IGUIListBox* addListBox(const IRECT& rectangle,
      IGUIElement* parent = 0, s32 id = -1, bool drawBackground = false);
  //! adds a tree view
  IGUITreeView* addTreeView(const IRECT& rectangle,
      IGUIElement* parent = 0, s32 id = -1, bool drawBackground = false,
      bool scrollBarVertical = true, bool scrollBarHorizontal = false);
  //! adds an mesh viewer. The returned pointer must not be dropped.
  IGUIMeshViewer* addMeshViewer(const IRECT& rectangle, IGUIElement* parent = 0, s32 id = -1, const char* text = 0);
  //! Adds a file open dialog.
  IGUIFileOpenDialog* addFileOpenDialog(const char* title = 0,
      bool modal = true, IGUIElement* parent = 0, s32 id = -1,
      bool restoreCWD = false, char*::char_type* startDir = 0);
  //! Adds a color select dialog.
  IGUIColorSelectDialog* addColorSelectDialog(const char* title = 0, bool modal = true, IGUIElement* parent = 0, s32 id = -1);
  //! adds a static text. The returned pointer must not be dropped.
  //IGUIStaticText* addStaticText(const char* text, const IRECT& rectangle, bool border = false, bool wordWrap = true, IGUIElement* parent = 0, s32 id = -1, bool drawBackground = false);
  //! Adds an edit box. The returned pointer must not be dropped.
  IGUIEditBox* addEditBox(const char* text, const IRECT& rectangle,
      bool border = false, IGUIElement* parent = 0, s32 id = -1);
  //! Adds a spin box to the environment
  IGUISpinBox* addSpinBox(const char* text, const IRECT& rectangle,
      bool border = false, IGUIElement* parent = 0, s32 id = -1);
  //! Adds a tab control to the environment.
  IGUITabControl* addTabControl(const IRECT& rectangle,
      IGUIElement* parent = 0, bool fillbackground = false, bool border = true, s32 id = -1);
  //! Adds tab to the environment.
  IGUITab* addTab(const IRECT& rectangle,
      IGUIElement* parent = 0, s32 id = -1);
  //! Adds a context menu to the environment.
  IGUIContextMenu* addContextMenu(const IRECT& rectangle,
      IGUIElement* parent = 0, s32 id = -1);
  //! Adds a menu to the environment.
  IGUIContextMenu* addMenu(IGUIElement* parent = 0, s32 id = -1);
  //! Adds a toolbar to the environment. It is like a menu is always placed on top
  //! in its parent, and contains buttons.
  IGUIToolBar* addToolBar(IGUIElement* parent = 0, s32 id = -1);
  //! Adds a combo box to the environment.
  IGUIComboBox* addComboBox(const IRECT& rectangle,
      IGUIElement* parent = 0, s32 id = -1);
  //! Adds a table element.
  IGUITable* addTable(const IRECT& rectangle,
      IGUIElement* parent = 0, s32 id = -1, bool drawBackground = false);
  //! sets the focus to an element
  bool setFocus(IGUIElement* element);
  //! removes the focus from an element
  bool removeFocus(IGUIElement* element);
  //! Returns if the element has focus
  bool hasFocus(IGUIElement* element) const;
  //! Returns the element with the focus
  IGUIElement* getFocus() const;
  //! Returns the element last known to be under the mouse
  IGUIElement* getHovered() const;
  //! Adds an element for fading in or out.
  IGUIInOutFader* addInOutFader(const IRECT* rectangle = 0, IGUIElement* parent = 0, s32 id = -1);
  //! Returns the root gui element.
  IGUIElement* getRootGUIElement();
  void OnPostRender(u32 time);
  //! Returns the default element factory which can create all built in elements
  IGUIElementFactory* getDefaultGUIElementFactory() const;
  //! Adds an element factory to the gui environment.
  /** Use this to extend the gui environment with new element types which it should be
  able to create automaticly, for example when loading data from xml files. */
  void registerGUIElementFactory(IGUIElementFactory* factoryToAdd);
  //! Returns amount of registered scene node factories.
  u32 getRegisteredGUIElementFactoryCount() const;
  //! Returns a scene node factory by index
  IGUIElementFactory* getGUIElementFactory(u32 index) const;
  //! Adds a GUI Element by its name
  IGUIElement* addGUIElement(const c8* elementName, IGUIElement* parent = 0);
  //! Saves the current gui into a file.
  /** \param filename: Name of the file.
  \param start: The element to start saving from.
  if not specified, the root element will be used */
  bool saveGUI(const char* filename, IGUIElement* start = 0);
  //! Saves the current gui into a file.
  /** \param file: The file to save the GUI to.
  \param start: The element to start saving from.
  if not specified, the root element will be used */
  bool saveGUI(IWriteFile* file, IGUIElement* start = 0);
  //! Loads the gui. Note that the current gui is not cleared before.
  /** \param filename: Name of the file.
  \param parent: The parent of all loaded GUI elements,
  if not specified, the root element will be used */
  bool loadGUI(const char* filename, IGUIElement* parent = 0);
  //! Loads the gui. Note that the current gui is not cleared before.
  /** \param file: IReadFile to load the GUI from
  \param parent: The parent of all loaded GUI elements,
  if not specified, the root element will be used */
  bool loadGUI(IReadFile* file, IGUIElement* parent = 0);
  //! Writes attributes of the environment
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;
  //! Reads attributes of the environment.
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);
  //! writes an element
  void writeGUIElement(IXMLWriter* writer, IGUIElement* node);
  //! reads an element
  void readGUIElement(IXMLReader* reader, IGUIElement* node);
private:
  IGUIElement* getNextElement(bool reverse = false, bool group = false);
  void updateHoveredElement(position2di mousePos);
  void loadBuiltInFont();
  struct SFont {
    SNamedPath NamedPath;
    IGUIFont* Font;
    bool operator < (const SFont& other) const {
      return (NamedPath < other.NamedPath);
    }
  };
  struct SSpriteBank {
    SNamedPath NamedPath;
    IGUISpriteBank* Bank;
    bool operator < (const SSpriteBank& other) const {
      return (NamedPath < other.NamedPath);
    }
  };
  struct SToolTip {
    IGUIStaticText* Element;
    u32 LastTime;
    u32 EnterTime;
    u32 LaunchTime;
    u32 RelaunchTime;
  };
  SToolTip ToolTip;
  IGUIElementFactory* > GUIElementFactoryList;
  SFont > Fonts;
  SSpriteBank > Banks;
  IVideoDriver* Driver;
  IGUIElement* Hovered;
  IGUIElement* HoveredNoSubelement; // subelements replaced by their parent, so you only have 'real' elements here
  IGUIElement* Focus;
  position2di LastHoveredMousePos;
  IGUISkin* CurrentSkin;
  IFileSystem* FileSystem;
  IEventReceiver* UserReceiver;
  IOSOperator* Operator;
  static const char* DefaultFontName;
};
#endif // _IRR_COMPILE_WITH_GUI_
#endif // __C_GUI_ENVIRONMENT_H_INCLUDED__

