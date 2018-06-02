
#ifndef vtkActor_h
#define vtkActor_h

#include "vtkProp3D.h"

class vtkRenderer;
class vtkTexture;
class vtkMapper;
class vtkProperty;

class VTKRENDERINGCORE_EXPORT vtkActor : public vtkProp3D
{
public:
  LISTDEF(vtkActor);
  void PrintSelf(ostream& os, vtkIndent indent);

  /**
   * Creates an actor with the following defaults: origin(0,0,0)
   * position=(0,0,0) scale=(1,1,1) visibility=1 pickable=1 dragable=1
   * orientation=(0,0,0). No user defined matrix and no texture map.
   */
  static vtkActor *New();

  /**
   * For some exporters and other other operations we must be
   * able to collect all the actors or volumes. These methods
   * are used in that process.
   */
  virtual void GetActors(vtkPropCollection *);

  //@{
  /**
   * Support the standard render methods.
   */
  virtual int RenderOpaqueGeometry(vtkViewport *viewport);
  virtual int RenderTranslucentPolygonalGeometry(vtkViewport *viewport);
  //@}

  /**
   * Does this prop have some translucent polygonal geometry?
   */
  virtual int HasTranslucentPolygonalGeometry();

  /**
   * This causes the actor to be rendered. It in turn will render the actor's
   * property, texture map and then mapper. If a property hasn't been
   * assigned, then the actor will create one automatically. Note that a side
   * effect of this method is that the pipeline will be updated.
   */
  virtual void Render(vtkRenderer *, vtkMapper *) {}

  /**
   * Shallow copy of an actor. Overloads the virtual vtkProp method.
   */
  void ShallowCopy(vtkProp *prop);

  /**
   * Release any graphics resources that are being consumed by this actor.
   * The parameter window could be used to determine which graphic
   * resources to release.
   */
  void ReleaseGraphicsResources(vtkWindow *);

  //@{
  /**
   * Set/Get the property object that controls this actors surface
   * properties.  This should be an instance of a vtkProperty object.  Every
   * actor must have a property associated with it.  If one isn't specified,
   * then one will be generated automatically. Multiple actors can share one
   * property object.
   */
  void SetProperty(vtkProperty *lut);
  vtkProperty *GetProperty();
  //@}

  /**
   * Create a new property suitable for use with this type of Actor.
   * For example, a vtkMesaActor should create a vtkMesaProperty
   * in this function.   The default is to just call vtkProperty::New.
   */
  virtual vtkProperty* MakeProperty();

  //@{
  /**
   * Set/Get the property object that controls this actors backface surface
   * properties.  This should be an instance of a vtkProperty object. If one
   * isn't specified, then the front face properties will be used.  Multiple
   * actors can share one property object.
   */
  void SetBackfaceProperty(vtkProperty *lut);
  vtkGetObjectMacro(BackfaceProperty,vtkProperty);
  //@}

  //@{
  /**
   * Set/Get the texture object to control rendering texture maps.  This will
   * be a vtkTexture object. An actor does not need to have an associated
   * texture map and multiple actors can share one texture.
   */
  virtual void SetTexture(vtkTexture*);
  vtkGetObjectMacro(Texture, vtkTexture);
  //@}

  /**
   * This is the method that is used to connect an actor to the end of a
   * visualization pipeline, i.e. the mapper. This should be a subclass
   * of vtkMapper. Typically vtkPolyDataMapper and vtkDataSetMapper will
   * be used.
   */
  virtual void SetMapper(vtkMapper *);

  //@{
  /**
   * Returns the Mapper that this actor is getting its data from.
   */
  vtkGetObjectMacro(Mapper, vtkMapper);
  //@}

  /**
   * Get the bounds for this Actor as (Xmin,Xmax,Ymin,Ymax,Zmin,Zmax). (The
   * method GetBounds(double bounds[6]) is available from the superclass.)
   */
  void GetBounds(double bounds[6]) {this->vtkProp3D::GetBounds( bounds );}
  double *GetBounds();

  /**
   * Apply the current properties to all parts that compose this actor.
   * This method is overloaded in vtkAssembly to apply the assemblies'
   * properties to all its parts in a recursive manner. Typically the
   * use of this method is to set the desired properties in the assembly,
   * and then push the properties down to the assemblies parts with
   * ApplyProperties().
   */
  virtual void ApplyProperties() {}

  /**
   * Get the actors mtime plus consider its properties and texture if set.
   */
  vtkMTimeType GetMTime();

  /**
   * Return the mtime of anything that would cause the rendered image to
   * appear differently. Usually this involves checking the mtime of the
   * prop plus anything else it depends on such as properties, textures,
   * etc.
   */
  virtual vtkMTimeType GetRedrawMTime();

  //@{
  /**
   * Force the actor to be treated as opaque or translucent
   */
  vtkGetMacro(ForceOpaque, bool);
  vtkSetMacro(ForceOpaque, bool);
  vtkBooleanMacro(ForceOpaque, bool);
  vtkGetMacro(ForceTranslucent, bool);
  vtkSetMacro(ForceTranslucent, bool);
  vtkBooleanMacro(ForceTranslucent, bool);
  //@}

  /**
   * WARNING: INTERNAL METHOD - NOT INTENDED FOR GENERAL USE
   * DO NOT USE THIS METHOD OUTSIDE OF THE RENDERING PROCESS
   * Used by vtkHardwareSelector to determine if the prop supports hardware
   * selection.
   */
  virtual bool GetSupportsSelection();

  vtkActor();
  ~vtkActor();

  // is this actor opaque
  int GetIsOpaque();
  bool ForceOpaque;
  bool ForceTranslucent;

  vtkProperty *Property;
  vtkProperty *BackfaceProperty;
  vtkTexture *Texture;
  vtkMapper *Mapper;

  // Bounds are cached in an actor - the MapperBounds are also cache to
  // help know when the Bounds need to be recomputed.
  double MapperBounds[6];
  vtkTimeStamp BoundsMTime;

private:
  vtkActor(const vtkActor&) VTK_DELETE_FUNCTION;
  void operator=(const vtkActor&) VTK_DELETE_FUNCTION;
};

#endif
