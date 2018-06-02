/** Example 022 Material Viewer
This example can be used to play around with material settings and watch the results.
Only the default non-shader materials are used in here.
You have two nodes to make it easier to see which difference your settings will make.
Additionally you have one lightscenenode and you can set the global ambient values.
*/
//#include "driverChoice.h"
#ifdef _MSC_VER
//#pragma comment(lib, "Irrlicht.lib")
#endif
/*
  Variables within the empty namespace are globals which are restricted to this file.
*/
const char* const DriverTypeNames[] = {
  "NUL",
  "SOFTWARE",
  "BURNINGSVIDEO",
  "DIRECT3D8",
  "DIRECT3D9",
  "OPENG",
  0,
};
// For the gui id's
enum EGUI_IDS {
  GUI_ID_OPEN_TEXTURE = 1,
  GUI_ID_QUIT,
  GUI_ID_MAX
};
// Name used in texture selection to clear the textures on the node
const char* CLEAR_TEXTURE = "CLEAR texture";
// some useful color constants
const SColor SCOL_BLACK     = _ARGB(255, 0,   0,   0);
const SColor SCOL_BLUE      = _ARGB(255, 0,   0,  255);
const SColor SCOL_CYAN      = _ARGB(255, 0,  255, 255);
const SColor SCOL_GRAY      = _ARGB(255, 128, 128, 128);
const SColor SCOL_GREEN     = _ARGB(255, 0,  255,  0);
const SColor SCOL_MAGENTA   = _ARGB(255, 255, 0,  255);
const SColor SCOL_RED       = _ARGB(255, 255, 0,   0);
const SColor SCOL_YELLOW    = _ARGB(255, 255, 255, 0);
const SColor SCOL_WHITE     = _ARGB(255, 255, 255, 255);
};  //
/*
  Returns a new unique number on each call.
*/
s32 makeUniqueId()
{
  static int unique = GUI_ID_MAX;
  ++unique;
  return unique;
}
/*
  Find out which vertex-type is needed for the given material type.
*/
E_VERTEX_TYPE getVertexTypeForMaterialType(E_MATERIAL_TYPE materialType)
{
  switch (materialType) {
  case EMT_SOLID:
    return EVT_STANDARD;
  case EMT_SOLID_2_LAYER:
    return EVT_STANDARD;
  case EMT_LIGHTMAP:
  case EMT_LIGHTMAP_ADD:
  case EMT_LIGHTMAP_M2:
  case EMT_LIGHTMAP_M4:
  case EMT_LIGHTMAP_LIGHTING:
  case EMT_LIGHTMAP_LIGHTING_M2:
  case EMT_LIGHTMAP_LIGHTING_M4:
    return EVT_2TCOORDS;
  case EMT_DETAIL_MAP:
    return EVT_2TCOORDS;
  case EMT_SPHERE_MAP:
    return EVT_STANDARD;
  case EMT_REFLECTION_2_LAYER:
    return EVT_2TCOORDS;
  case EMT_TRANSPARENT_ADD_COLOR:
    return EVT_STANDARD;
  case EMT_TRANSPARENT_ALPHA_CHANNEL:
    return EVT_STANDARD;
  case EMT_TRANSPARENT_ALPHA_CHANNEL_REF:
    return EVT_STANDARD;
  case EMT_TRANSPARENT_VERTEX_ALPHA:
    return EVT_STANDARD;
  case EMT_TRANSPARENT_REFLECTION_2_LAYER:
    return EVT_2TCOORDS;
  case EMT_NORMAL_MAP_SOLID:
  case EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR:
  case EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA:
  case EMT_PARALLAX_MAP_SOLID:
  case EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR:
  case EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA:
    return EVT_TANGENTS;
  case EMT_ONETEXTURE_BLEND:
    return EVT_STANDARD;
  case EMT_FORCE_32BIT:
    return EVT_STANDARD;
  }
  return EVT_STANDARD;
}
/*
  Custom GUI-control to edit colorvalues.
*/
class CColorControl : public IGUIElement
{
public:
  // Constructor
  CColorControl(IGUIEnvironment* guiEnv, const position2di& pos, const char* text, IGUIElement* parent, s32 id = -1)
    : IGUIElement(EGUIET_ELEMENT, guiEnv, parent, id, rect< s32 >(pos, pos + dimension2d<s32>(80, 75)))
    , DirtyFlag(true)
    , ColorStatic(0)
    , EditAlpha(0)
    , EditRed(0)
    , EditGreen(0)
    , EditBlue(0) {
    ButtonSetId = makeUniqueId();
    const rect< s32 > rectControls(0, 0, AbsoluteRect.getWidth(), AbsoluteRect.getHeight());
    IGUIStaticText* groupElement = guiEnv->addStaticText("", rectControls, true, false, this, -1, false);
    groupElement->setNotClipped(true);
    guiEnv->addStaticText(text, IRECT(0, 0, 80, 15), false, false, groupElement, -1, false);
    EditAlpha = addEditForNumbers(guiEnv, position2di(0, 15), "a", -1, groupElement);
    EditRed = addEditForNumbers(guiEnv, position2di(0, 30), "r", -1, groupElement);
    EditGreen = addEditForNumbers(guiEnv, position2di(0, 45), "g", -1, groupElement);
    EditBlue = addEditForNumbers(guiEnv, position2di(0, 60), "b", -1, groupElement);
    ColorStatic = guiEnv->addStaticText("", IRECT(60, 15, 80, 75), true, false, groupElement, -1, true);
    guiEnv->addButton(IRECT(60, 35, 80, 50), groupElement, ButtonSetId, "set");
    SetEditsFromColor(Color);
  }
  // event receiver
  bool OnEvent(const SEvent& event) {
    if (event.EventType != EET_GUI_EVENT) {
      return false;
    }
    if (event.GUIEvent.Caller->getID() == ButtonSetId && event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
      Color = GetColorFromEdits();
      SetEditsFromColor(Color);
    }
    return false;
  }
  // set the color values
  void setColor(const SColor& col) {
    DirtyFlag = true;
    Color = col;
    SetEditsFromColor(Color);
  }
  // get the color values
  const SColor& getColor() const {
    return Color;
  }
  // To reset the dirty flag
  void resetDirty() {
    DirtyFlag = false;
  }
  // when the color was changed the dirty flag is set
  bool isDirty() const {
    return DirtyFlag;
  };
protected:
  // Add a staticbox for a description + an editbox so users can enter numbers
  IGUIEditBox* addEditForNumbers(IGUIEnvironment* guiEnv, const position2di& pos, const char* text, s32 id, IGUIElement* parent) {
    rect< s32 > rect(pos, pos + dimension2d<s32>(10, 15));
    guiEnv->addStaticText(text, rect, false, false, parent, -1, false);
    rect += position2di(20, 0);
    rect.LowerRightCorner.X += 20;
    IGUIEditBox* edit = guiEnv->addEditBox("0", rect, true, parent, id);
    return edit;
  }
  // Get the color value from the editfields
  SColor GetColorFromEdits() {
    SColor col;
    if (EditAlpha) {
      u32 alpha = strtoul10(char * (EditAlpha->getText()));
      if (alpha > 255) {
        alpha = 255;
      }
      col.setAlpha(alpha);
    }
    if (EditRed) {
      u32 red = strtoul10(char * (EditRed->getText()));
      if (red > 255) {
        red = 255;
      }
      col.setRed(red);
    }
    if (EditGreen) {
      u32 green = strtoul10(char * (EditGreen->getText()));
      if (green > 255) {
        green = 255;
      }
      col.setGreen(green);
    }
    if (EditBlue) {
      u32 blue = strtoul10(char * (EditBlue->getText()));
      if (blue > 255) {
        blue = 255;
      }
      col.setBlue(blue);
    }
    return col;
  }
  // Fill the editfields with the value for the given color
  void SetEditsFromColor(SColor col) {
    DirtyFlag = true;
    if (EditAlpha) {
      EditAlpha->setText(char * (col.getAlpha()));
    }
    if (EditRed) {
      EditRed->setText(char * (col.getRed()));
    }
    if (EditGreen) {
      EditGreen->setText(char * (col.getGreen()));
    }
    if (EditBlue) {
      EditBlue->setText(char * (col.getBlue()));
    }
    if (ColorStatic) {
      ColorStatic->setBackgroundColor(col);
    }
  }
private:
  bool DirtyFlag;
  SColor Color;
  s32 ButtonSetId;
  IGUIStaticText* ColorStatic;
  IGUIEditBox* EditAlpha;
  IGUIEditBox* EditRed;
  IGUIEditBox* EditGreen;
  IGUIEditBox* EditBlue;
};
/*
  Custom GUI-control for to edit all colors typically used in materials and lights
*/
class CAllColorsControl : public IGUIElement
{
public:
  // Constructor
  CAllColorsControl(IGUIEnvironment* guiEnv, const position2di& pos, const char* description, bool hasEmissive, IGUIElement* parent, s32 id = -1)
    : IGUIElement(EGUIET_ELEMENT, guiEnv, parent, id, IRECT(pos, pos + dimension2d<s32>(60, 250)))
    , ControlAmbientColor(0), ControlDiffuseColor(0), ControlSpecularColor(0), ControlEmissiveColor(0) {
    IRECT rect(0, 0, 60, 15);
    guiEnv->addStaticText(description, rect, false, false, this, -1, false);
    createColorControls(guiEnv, position2di(0, 15), hasEmissive);
  }
  // Destructor
  ~CAllColorsControl() {
    ControlAmbientColor->drop();
    ControlDiffuseColor->drop();
    if (ControlEmissiveColor) {
      ControlEmissiveColor->drop();
    }
    ControlSpecularColor->drop();
  }
  // Set the color values to those within the material
  void setColorsToMaterialColors(const SMaterial& material) {
    ControlAmbientColor->setColor(material.AmbientColor);
    ControlDiffuseColor->setColor(material.DiffuseColor);
    ControlEmissiveColor->setColor(material.EmissiveColor);
    ControlSpecularColor->setColor(material.SpecularColor);
  }
  // Update all changed colors in the material
  void updateMaterialColors(SMaterial& material) {
    if (ControlAmbientColor->isDirty()) {
      material.AmbientColor = ControlAmbientColor->getColor();
    }
    if (ControlDiffuseColor->isDirty()) {
      material.DiffuseColor = ControlDiffuseColor->getColor();
    }
    if (ControlEmissiveColor->isDirty()) {
      material.EmissiveColor = ControlEmissiveColor->getColor();
    }
    if (ControlSpecularColor->isDirty()) {
      material.SpecularColor = ControlSpecularColor->getColor();
    }
  }
  // Set the color values to those from the light data
  void setColorsToLightDataColors(const SLight& lightData) {
    ControlAmbientColor->setColor(lightData.AmbientColor.to_ARGB());
    ControlAmbientColor->setColor(lightData.DiffuseColor.to_ARGB());
    ControlAmbientColor->setColor(lightData.SpecularColor.to_ARGB());
  }
  // Update all changed colors in the light data
  void updateLightColors(SLight& lightData) {
    if (ControlAmbientColor->isDirty()) {
      lightData.AmbientColor = SColorf(ControlAmbientColor->getColor());
    }
    if (ControlDiffuseColor->isDirty()) {
      lightData.DiffuseColor = SColorf(ControlDiffuseColor->getColor());
    }
    if (ControlSpecularColor->isDirty()) {
      lightData.SpecularColor = SColorf(ControlSpecularColor->getColor());
    }
  }
  // To reset the dirty flags
  void resetDirty() {
    ControlAmbientColor->resetDirty();
    ControlDiffuseColor->resetDirty();
    ControlSpecularColor->resetDirty();
    if (ControlEmissiveColor) {
      ControlEmissiveColor->resetDirty();
    }
  }
protected:
  void createColorControls(IGUIEnvironment* guiEnv, const position2di& pos, bool hasEmissive) {
    ControlAmbientColor = new CColorControl(guiEnv, pos, "ambient", this);
    ControlDiffuseColor = new CColorControl(guiEnv, pos + position2di(0, 75), "diffuse", this);
    ControlSpecularColor = new CColorControl(guiEnv, pos + position2di(0, 150), "specular", this);
    if (hasEmissive) {
      ControlEmissiveColor = new CColorControl(guiEnv, pos + position2di(0, 225), "emissive", this);
    }
  }
private:
  CColorControl*  ControlAmbientColor;
  CColorControl*  ControlDiffuseColor;
  CColorControl*  ControlSpecularColor;
  CColorControl*  ControlEmissiveColor;
};
/*
  GUI-Control to offer a selection of available textures.
*/
class CTextureControl : public IGUIElement
{
public:
  CTextureControl(IGUIEnvironment* guiEnv, IVideoDriver* driver, const position2di& pos, IGUIElement* parent, s32 id = -1)
    : IGUIElement(EGUIET_ELEMENT, guiEnv, parent, id, IRECT(pos, pos + dimension2d<s32>(100, 15)))
    , DirtyFlag(true), ComboTexture(0) {
    IRECT rectCombo(0, 0, AbsoluteRect.getWidth(), AbsoluteRect.getHeight());
    ComboTexture = guiEnv->addComboBox(rectCombo, this);
    updateTextures(driver);
  }
  bool OnEvent(const SEvent& event) {
    if (event.EventType != EET_GUI_EVENT) {
      return false;
    }
    if (event.GUIEvent.Caller == ComboTexture && event.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED) {
      DirtyFlag = true;
    }
    return false;
  }
  // Workaround for a problem with comboboxes.
  // We have to get in front when the combobox wants to get in front or combobox-list might be drawn below other elements.
  bool bringToFront(IGUIElement* element) {
    bool result = IGUIElement::bringToFront(element);
    if (Parent && element == ComboTexture) {
      result &= Parent->bringToFront(this);
    }
    return result;
  }
  // return selected texturename (if any, otherwise 0)
  const char* getSelectedTextureName() const {
    s32 selected = ComboTexture->getSelected();
    if (selected < 0) {
      return 0;
    }
    return ComboTexture->getItem(selected);
  }
  // reset the dirty flag
  void resetDirty() {
    DirtyFlag = false;
  }
  // when the texture was changed the dirty flag is set
  bool isDirty() const {
    return DirtyFlag;
  };
  // Put the names of all currently loaded textures in a combobox
  void updateTextures(IVideoDriver* driver) {
    s32 oldSelected = ComboTexture->getSelected();
    s32 selectNew = -1;
    char* oldTextureName;
    if (oldSelected >= 0) {
      oldTextureName = ComboTexture->getItem(oldSelected);
    }
    ComboTexture->clear();
    for (u32 i = 0; i < driver->getTextureCount(); ++i) {
      ITexture* texture = driver->getTextureByIndex(i);
      char* name(texture->getName());
      ComboTexture->addItem(name);
      if (!oldTextureName.empty() && selectNew < 0 && name == oldTextureName) {
        selectNew = i;
      }
    }
    // add another name which can be used to clear the texture
    ComboTexture->addItem(CLEAR_TEXTURE);
    if (CLEAR_TEXTURE == oldTextureName) {
      selectNew = ComboTexture->getItemCount() - 1;
    }
    if (selectNew >= 0) {
      ComboTexture->setSelected(selectNew);
    }
    DirtyFlag = true;
  }
private:
  bool DirtyFlag;
  IGUIComboBox* ComboTexture;
};
/*
  Control which allows setting some of the material values for a meshscenenode
*/
struct SMeshNodeControl {
  // constructor
  SMeshNodeControl()
    : Initialized(false), Driver(0), MeshManipulator(0), SceneNode(0), SceneNode2T(0), SceneNodeTangents(0)
    , AllColorsControl(0), ButtonLighting(0), InfoLighting(0), ComboMaterial(0), TextureControl1(0), TextureControl2(0), ControlVertexColors(0) {
  }
  // Destructor
  ~SMeshNodeControl() {
    if (TextureControl1) {
      TextureControl1->drop();
    }
    if (TextureControl2) {
      TextureControl2->drop();
    }
    if (ControlVertexColors) {
      ControlVertexColors->drop();
    }
    if (AllColorsControl) {
      AllColorsControl->drop();
    }
  }
  void init(IMeshSceneNode* node, IrrlichtDevice* device, const position2di& pos, const char* description) {
    if (Initialized || !node || !device) { // initializing twice or with invalid data not allowed
      return;
    }
    Driver = device->getVideoDriver();
    IGUIEnvironment* guiEnv = device->getGUIEnvironment();
    ISceneManager* smgr = device->getSceneManager();
    MeshManipulator = smgr->getMeshManipulator();
    SceneNode = node;
    IMeshManipulator* meshManip = smgr->getMeshManipulator();
    IMesh* mesh2T = meshManip->createMeshWith2TCoords(node->getMesh());
    SceneNode2T = smgr->addMeshSceneNode(mesh2T, 0, -1, SceneNode->getPosition(), SceneNode->getRotation(), SceneNode->getScale());
    mesh2T->drop();
    IMesh* meshTangents = meshManip->createMeshWithTangents(node->getMesh(), false, false, false);
    SceneNodeTangents = smgr->addMeshSceneNode(meshTangents, 0, -1
        , SceneNode->getPosition(), SceneNode->getRotation(), SceneNode->getScale());
    meshTangents->drop();
    SMaterial& material = SceneNode->getMaterial(0);
    material.Lighting = true;
    AllColorsControl = new CAllColorsControl(guiEnv, pos, description, true, guiEnv->getRootGUIElement());
    AllColorsControl->setColorsToMaterialColors(material);
    IRECT rectBtn(pos + position2di(0, 320), dimension2d<s32>(60, 15));
    ButtonLighting = guiEnv->addButton(rectBtn, 0, -1, "Lighting");
    ButtonLighting->setIsPushButton(true);
    ButtonLighting->setPressed(material.Lighting);
    IRECT rectInfo(rectBtn.LowerRightCorner.X, rectBtn.UpperLeftCorner.Y, rectBtn.LowerRightCorner.X + 40, rectBtn.UpperLeftCorner.Y + 15);
    InfoLighting = guiEnv->addStaticText("", rectInfo, true, false);
    InfoLighting->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
    IRECT rectCombo(pos.X, rectBtn.LowerRightCorner.Y, pos.X + 100, rectBtn.LowerRightCorner.Y + 15);
    ComboMaterial = guiEnv->addComboBox(rectCombo);
    for (int i = 0; i <= (int)EMT_ONETEXTURE_BLEND; ++i) {
      ComboMaterial->addItem(char * (sBuiltInMaterialTypeNames[i]));
    }
    ComboMaterial->setSelected((s32)material.MaterialType);
    position2di posTex(rectCombo.UpperLeftCorner.X, rectCombo.LowerRightCorner.Y);
    TextureControl1 = new CTextureControl(guiEnv, Driver, posTex, guiEnv->getRootGUIElement());
    posTex.Y += 15;
    TextureControl2 = new CTextureControl(guiEnv, Driver, posTex, guiEnv->getRootGUIElement());
    position2di posVertexColors(posTex.X, posTex.Y + 15);
    ControlVertexColors = new CColorControl(guiEnv, posVertexColors, "Vertex colors", guiEnv->getRootGUIElement());
    S3DVertex* vertices = (S3DVertex*)node->getMesh()->getMeshBuffer(0)->getVertices();
    if (vertices) {
      ControlVertexColors->setColor(vertices[0].Color);
    }
    Initialized = true;
  }
  void update() {
    if (!Initialized) {
      return;
    }
    SMaterial& material = SceneNode->getMaterial(0);
    SMaterial& material2T = SceneNode2T->getMaterial(0);
    SMaterial& materialTangents = SceneNodeTangents->getMaterial(0);
    s32 selectedMaterial = ComboMaterial->getSelected();
    if (selectedMaterial >= (s32)EMT_SOLID && selectedMaterial <= (s32)EMT_ONETEXTURE_BLEND) {
      E_VERTEX_TYPE vertexType = getVertexTypeForMaterialType((E_MATERIAL_TYPE)selectedMaterial);
      switch (vertexType) {
      case EVT_STANDARD:
        material.MaterialType = (E_MATERIAL_TYPE)selectedMaterial;
        SceneNode->setVisible(true);
        SceneNode2T->setVisible(false);
        SceneNodeTangents->setVisible(false);
        break;
      case EVT_2TCOORDS:
        material2T.MaterialType = (E_MATERIAL_TYPE)selectedMaterial;
        SceneNode->setVisible(false);
        SceneNode2T->setVisible(true);
        SceneNodeTangents->setVisible(false);
        break;
      case EVT_TANGENTS:
        materialTangents.MaterialType = (E_MATERIAL_TYPE)selectedMaterial;
        SceneNode->setVisible(false);
        SceneNode2T->setVisible(false);
        SceneNodeTangents->setVisible(true);
        break;
      }
    }
    updateMaterial(material);
    updateMaterial(material2T);
    updateMaterial(materialTangents);
    if (ButtonLighting->isPressed()) {
      InfoLighting->setText("on");
    }
    else {
      InfoLighting->setText("off");
    }
    AllColorsControl->resetDirty();
    TextureControl1->resetDirty();
    TextureControl2->resetDirty();
    ControlVertexColors->resetDirty();
  }
  void updateTextures() {
    TextureControl1->updateTextures(Driver);
    TextureControl2->updateTextures(Driver);
  }
protected:
  void updateMaterial(SMaterial& material) {
    AllColorsControl->updateMaterialColors(material);
    material.Lighting = ButtonLighting->isPressed();
    if (TextureControl1->isDirty()) {
      material.TextureLayer[0].Texture = Driver->getTexture(char * (TextureControl1->getSelectedTextureName()));
    }
    if (TextureControl2->isDirty()) {
      material.TextureLayer[1].Texture = Driver->getTexture(char * (TextureControl2->getSelectedTextureName()));
    }
    if (ControlVertexColors->isDirty()) {
      MeshManipulator->setVertexColors(SceneNode->getMesh(), ControlVertexColors->getColor());
      MeshManipulator->setVertexColors(SceneNode2T->getMesh(), ControlVertexColors->getColor());
      MeshManipulator->setVertexColors(SceneNodeTangents->getMesh(), ControlVertexColors->getColor());
    }
  }
  bool Initialized;
  IVideoDriver*     Driver;
  IMeshManipulator*  MeshManipulator;
  IMeshSceneNode*    SceneNode;
  IMeshSceneNode*    SceneNode2T;
  IMeshSceneNode*    SceneNodeTangents;
  CAllColorsControl*      AllColorsControl;
  IGUIButton*       ButtonLighting;
  IGUIStaticText*    InfoLighting;
  IGUIComboBox*     ComboMaterial;
  CTextureControl*      TextureControl1;
  CTextureControl*      TextureControl2;
  CColorControl*        ControlVertexColors;
};
/*
  Control to allow setting the color values of a lightscenenode.
*/
struct SLightNodeControl {
  // constructor
  SLightNodeControl() : Initialized(false), SceneNode(0), AllColorsControl(0) {
  }
  ~SLightNodeControl() {
    if (AllColorsControl) {
      AllColorsControl->drop();
    }
  }
  void init(ILightSceneNode* node, IGUIEnvironment* guiEnv, const position2di& pos, const char* description) {
    if (Initialized || !node || !guiEnv) { // initializing twice or with invalid data not allowed
      return;
    }
    SceneNode = node;
    AllColorsControl = new CAllColorsControl(guiEnv, pos, description, false, guiEnv->getRootGUIElement());
    const SLight& lightData = SceneNode->getLightData();
    AllColorsControl->setColorsToLightDataColors(lightData);
    Initialized = true;
  }
  void update() {
    if (!Initialized) {
      return;
    }
    SLight& lightData = SceneNode->getLightData();
    AllColorsControl->updateLightColors(lightData);
  }
protected:
  bool Initialized;
  ILightSceneNode* SceneNode;
  CAllColorsControl* AllColorsControl;
};
/*
  Application configuration
*/
struct SConfig {
  SConfig()
    : RenderInBackground(true)
    , DriverType(EDT_BURNINGSVIDEO)
    , ScreenSize(640, 480) {
  }
  bool RenderInBackground;
  E_DRIVER_TYPE DriverType;
  dimension2du ScreenSize;
};
/*
  Main application class
*/
class CApp : public IEventReceiver
{
  friend int main(int argc, char* argv[]);
public:
  // constructor
  CApp()
    : IsRunning(false)
    , Device(0)
    , Camera(0)
    , GlobalAmbient(0) {
  }
  // destructor
  ~CApp() {
  }
  // stop running - will quit at end of mainloop
  void stopApp() {
    IsRunning = false;
  }
  // Event handler
  bool OnEvent(const SEvent& event) {
    if (event.EventType == EET_GUI_EVENT) {
      IGUIEnvironment* env = Device->getGUIEnvironment();
      switch (event.GUIEvent.EventType) {
      case EGET_MENU_ITEM_SELECTED: {
        IGUIContextMenu* menu = (IGUIContextMenu*)event.GUIEvent.Caller;
        s32 id = menu->getItemCommandId(menu->getSelectedItem());
        switch (id) {
        case GUI_ID_OPEN_TEXTURE: // File -> Open Texture
          env->addFileOpenDialog("Please select a texture file to open");
          break;
        case GUI_ID_QUIT: // File -> Quit
          stopApp();
          break;
        }
      }
      break;
      case EGET_FILE_SELECTED: {
        // load the model file, selected in the file open dialog
        IGUIFileOpenDialog* dialog =
            (IGUIFileOpenDialog*)event.GUIEvent.Caller;
        loadTexture(char * (dialog->getFileName()));
      }
      break;
      default:
        break;
      }
    }
    return false;
  }
protected:
  // Application initialization
  // returns true when it was successful initialized, otherwise false.
  bool init(int argc, char* argv[]) {
    // ask user for driver
    Config.DriverType = driverChoiceConsole();
    if (Config.DriverType == EDT_COUNT) {
      return false;
    }
    // create the device with the settings from our config
    Device = createDevice(Config.DriverType, Config.ScreenSize);
    if (!Device) {
      return false;
    }
    Device->setWindowCaption(DriverTypeNames[Config.DriverType]);
    Device->setEventReceiver(this);
    ISceneManager* smgr = Device->getSceneManager();
    IVideoDriver* driver = Device->getVideoDriver();
    IGUIEnvironment* guiEnv = Device->getGUIEnvironment();
    // set a nicer font
    IGUISkin* skin = guiEnv->getSkin();
    IGUIFont* font = guiEnv->getFont("../../media/fonthaettenschweiler.bmp");
    if (font) {
      skin->setFont(font);
    }
    // remove some alpha value because it makes those menus harder to read otherwise
    SColor col3dHighLight(skin->getColor(EGDC_APP_WORKSPACE));
    col3dHighLight.setAlpha(255);
    SColor colHighLight(col3dHighLight);
    skin->setColor(EGDC_HIGH_LIGHT, colHighLight);
    skin->setColor(EGDC_3D_HIGH_LIGHT, col3dHighLight);
    // Add some textures which are useful to test material settings
    createDefaultTextures(driver);
    // create a menu
    IGUIContextMenu* menuBar = guiEnv->addMenu();
    menuBar->addItem("File", -1, true, true);
    IGUIContextMenu* subMenuFile = menuBar->getSubMenu(0);
    subMenuFile->addItem("Open texture ...", GUI_ID_OPEN_TEXTURE);
    subMenuFile->addSeparator();
    subMenuFile->addItem("Quit", GUI_ID_QUIT);
    // a static camera
    Camera = smgr->addCameraSceneNode(0, f3VECTOR(0, 0, 0),
        f3VECTOR(0, 0, 100),
        -1);
    // add the nodes which are used to show the materials
    IMeshSceneNode* nodeL = smgr->addCubeSceneNode(30.0f, 0, -1,
        f3VECTOR(-35, 0, 100),
        f3VECTOR(0, 0, 0),
        f3VECTOR(1.0f, 1.0f, 1.0f));
    NodeLeft.init(nodeL, Device, position2di(10, 20), "left node");
    IMeshSceneNode* nodeR = smgr->addCubeSceneNode(30.0f, 0, -1,
        f3VECTOR(35, 0, 100),
        f3VECTOR(0, 0, 0),
        f3VECTOR(1.0f, 1.0f, 1.0f));
    NodeRight.init(nodeR, Device, position2di(530, 20), "right node");
    // add one light
    ILightSceneNode* nodeLight = smgr->addLightSceneNode(0, f3VECTOR(0, 0, 0),
        SColorf(1.0f, 1.0f, 1.0f),
        100.0f);
    LightControl.init(nodeLight, guiEnv, position2di(270, 20), "light");
    // one large cube around everything. That's mainly to make the light more obvious.
    IMeshSceneNode* backgroundCube = smgr->addCubeSceneNode(200.0f, 0, -1, f3VECTOR(0, 0, 0),
        f3VECTOR(45, 0, 0),
        f3VECTOR(1.0f, 1.0f, 1.0f));
    backgroundCube->getMaterial(0).BackfaceCulling = false;     // we are within the cube, so we have to disable backface culling to see it
    backgroundCube->getMaterial(0).EmissiveColor.set(255, 50, 50, 50); // we keep some self lighting to keep texts visible
    // set the ambient light value
    GlobalAmbient = new CColorControl(guiEnv, position2di(270, 300), "global ambient", guiEnv->getRootGUIElement());
    GlobalAmbient->setColor(smgr->getAmbientLight().to_ARGB());
    return true;
  }
  // Update one frame
  bool update() {
    IVideoDriver* videoDriver =  Device->getVideoDriver();
    if (!Device->run()) {
      return false;
    }
    if (Device->isWindowActive() || Config.RenderInBackground) {
      IGUIEnvironment* guiEnv = Device->getGUIEnvironment();
      ISceneManager* smgr = Device->getSceneManager();
      IGUISkin* skin = guiEnv->getSkin();
      // update our controls
      NodeLeft.update();
      NodeRight.update();
      LightControl.update();
      // update ambient light settings
      if (GlobalAmbient->isDirty()) {
        smgr->setAmbientLight(GlobalAmbient->getColor());
        GlobalAmbient->resetDirty();
      }
      // draw everything
      SColor bkColor(skin->getColor(EGDC_APP_WORKSPACE));
      videoDriver->beginScene(true, true, bkColor);
      smgr->drawAll();
      guiEnv->drawAll();
      videoDriver->endScene();
    }
    return true;
  }
  // Run the application. Our main loop.
  void run() {
    IsRunning = true;
    if (!Device) {
      return;
    }
    // main application loop
    while (IsRunning) {
      if (!update()) {
        break;
      }
      Device->sleep(5);
    }
  }
  // Close down the application
  void quit() {
    IsRunning = false;
    GlobalAmbient->drop();
    GlobalAmbient = NULL;
    if (Device) {
      Device->closeDevice();
      Device->drop();
      Device = NULL;
    }
  }
  // Create some useful textures.
  // Note that the function put readability over speed, you shouldn't use setPixel at runtime but for initialization it's nice.
  void createDefaultTextures(IVideoDriver* driver) {
    const u32 width = 256;
    const u32 height = 256;
    IImage* imageA8R8G8B8 = driver->createImage(ECF_A8R8G8B8, dimension2du(width, height));
    if (!imageA8R8G8B8) {
      return;
    }
    const u32 pitch = imageA8R8G8B8->getPitch();
    // some nice square-pattern with 9 typical colors
    for (u32 y = 0; y < height; ++ y) {
      for (u32 x = 0; x < pitch; ++x) {
        if (y < height / 3) {
          if (x < width / 3) {
            imageA8R8G8B8->setPixel(x, y, SCOL_BLACK);
          }
          else if (x < 2 * width / 3) {
            imageA8R8G8B8->setPixel(x, y, SCOL_BLUE);
          }
          else {
            imageA8R8G8B8->setPixel(x, y, SCOL_CYAN);
          }
        }
        else if (y < 2 * height / 3) {
          if (x < width / 3) {
            imageA8R8G8B8->setPixel(x, y, SCOL_GRAY);
          }
          else if (x < 2 * width / 3) {
            imageA8R8G8B8->setPixel(x, y, SCOL_GREEN);
          }
          else {
            imageA8R8G8B8->setPixel(x, y, SCOL_MAGENTA);
          }
        }
        else {
          if (x < width / 3) {
            imageA8R8G8B8->setPixel(x, y, SCOL_RED);
          }
          else if (x < 2 * width / 3) {
            imageA8R8G8B8->setPixel(x, y, SCOL_YELLOW);
          }
          else {
            imageA8R8G8B8->setPixel(x, y, SCOL_WHITE);
          }
        }
      }
    }
    driver->addTexture(char * ("CARO_A8R8G8B8"), imageA8R8G8B8);
    // all white
    imageA8R8G8B8->fill(SCOL_WHITE);
    driver->addTexture(char * ("WHITE_A8R8G8B8"), imageA8R8G8B8);
    // all black
    imageA8R8G8B8->fill(SCOL_BLACK);
    driver->addTexture(char * ("BLACK_A8R8G8B8"), imageA8R8G8B8);
    // gray-scale
    for (u32 y = 0; y < height; ++ y) {
      for (u32 x = 0; x < pitch; ++x) {
        imageA8R8G8B8->setPixel(x, y, _ARGB(y, x, x, x));
      }
    }
    driver->addTexture(char * ("GRAYSCALE_A8R8G8B8"), imageA8R8G8B8);
    imageA8R8G8B8->drop();
  }
  // Load a texture and make sure nodes know it when more textures are available.
  void loadTexture(const char* name) {
    Device->getVideoDriver()->getTexture(name);
    NodeLeft.updateTextures();
    NodeRight.updateTextures();
  }
private:
  SConfig           Config;
  volatile bool       IsRunning;
  IrrlichtDevice*      Device;
  ICameraSceneNode*  Camera;
  SMeshNodeControl      NodeLeft;
  SMeshNodeControl      NodeRight;
  SLightNodeControl     LightControl;
  CColorControl*        GlobalAmbient;
};
/*
  A very short main as we do everything else in classes.
*/
int main(int argc, char* argv[])
{
  CApp APP;
  if (!APP.init(argc, argv)) {
    printf("init failed\n");
    return 1;
  }
  APP.run();
  APP.quit();
  return 0;
}
/*
**/

