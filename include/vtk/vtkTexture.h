
#ifndef vtkTexture_h
#define vtkTexture_h

#include "vtkImageAlgorithm.h"

class vtkImageData;
class vtkScalarsToColors;
class vtkRenderer;
//class vtkUnsignedCharArray;
class vtkWindow;
class vtkDataArray;
class vtkTransform;

#define VTK_TEXTURE_QUALITY_DEFAULT 0
#define VTK_TEXTURE_QUALITY_16BIT   16
#define VTK_TEXTURE_QUALITY_32BIT   32

class vtkTexture : public vtkImageAlgorithm
{
public:
  static vtkTexture* New();
  vtkTypeMacro(vtkTexture, vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  /**
   * Renders a texture map. It first checks the object's modified time
   * to make sure the texture maps Input is valid, then it invokes the
   * Load() method.
   */
  virtual void Render(vtkRenderer* ren);

  /**
   * Cleans up after the texture rendering to restore the state of the
   * graphics context.
   */
  virtual void PostRender(vtkRenderer*) {}

  /**
   * Release any graphics resources that are being consumed by this texture.
   * The parameter window could be used to determine which graphic
   * resources to release.
   */
  virtual void ReleaseGraphicsResources(vtkWindow*) {}

  /**
   * Abstract interface to renderer. Each concrete subclass of
   * vtkTexture will load its data into graphics system in response
   * to this method invocation.
   */
  virtual void Load(vtkRenderer*) {}

  //@{
  /**
   * Turn on/off the repetition of the texture map when the texture
   * coords extend beyond the [0,1] range.
   */
  vtkGetMacro(Repeat, int);
  vtkSetMacro(Repeat, int);
  vtkBooleanMacro(Repeat, int);
  //@}

  //@{
  /**
   * Turn on/off the clamping of the texture map when the texture
   * coords extend beyond the [0,1] range.
   * Only used when Repeat is off, and edge clamping is supported by
   * the graphics card.
   */
  vtkGetMacro(EdgeClamp, int);
  vtkSetMacro(EdgeClamp, int);
  vtkBooleanMacro(EdgeClamp, int);
  //@}

  //@{
  /**
   * Turn on/off linear interpolation of the texture map when rendering.
   */
  vtkGetMacro(Interpolate, int);
  vtkSetMacro(Interpolate, int);
  vtkBooleanMacro(Interpolate, int);
  //@}

  //@{
  /**
   * Force texture quality to 16-bit or 32-bit.
   * This might not be supported on all machines.
   */
  vtkSetMacro(Quality, int);
  vtkGetMacro(Quality, int);
  void SetQualityToDefault()
    { this->SetQuality(VTK_TEXTURE_QUALITY_DEFAULT); }
  void SetQualityTo16Bit()
    { this->SetQuality(VTK_TEXTURE_QUALITY_16BIT); }
  void SetQualityTo32Bit()
    { this->SetQuality(VTK_TEXTURE_QUALITY_32BIT); }
  //@}

  //@{
  /**
   * Turn on/off the mapping of color scalars through the lookup table.
   * The default is Off. If Off, unsigned char scalars will be used
   * directly as texture. If On, scalars will be mapped through the
   * lookup table to generate 4-component unsigned char scalars.
   * This ivar does not affect other scalars like unsigned short, float,
   * etc. These scalars are always mapped through lookup tables.
   */
  vtkGetMacro(MapColorScalarsThroughLookupTable, int);
  vtkSetMacro(MapColorScalarsThroughLookupTable, int);
  vtkBooleanMacro(MapColorScalarsThroughLookupTable, int);
  //@}

  /**
   * Get the input as a vtkImageData object.  This method is for
   * backwards compatibility.
   */
  vtkImageData* GetInput();

  //@{
  /**
   * Specify the lookup table to convert scalars if necessary
   */
  void SetLookupTable(vtkScalarsToColors *);
  vtkGetObjectMacro(LookupTable, vtkScalarsToColors);
  //@}

  //@{
  /**
   * Get Mapped Scalars
   */
  vtkGetObjectMacro(MappedScalars, vtkUnsignedCharArray);
  //@}

  /**
   * Map scalar values into color scalars.
   */
  unsigned char* MapScalarsToColors(vtkDataArray* scalars);

  //@{
  /**
   * Set a transform on the texture which allows one to scale,
   * rotate and translate the texture.
   */
  void SetTransform(vtkTransform *transform);
  vtkGetObjectMacro(Transform, vtkTransform);
  //@}

  /**
   * Used to specify how the texture will blend its RGB and Alpha values
   * with other textures and the fragment the texture is rendered upon.
   */
  enum VTKTextureBlendingMode
  {
    VTK_TEXTURE_BLENDING_MODE_NONE = 0,
    VTK_TEXTURE_BLENDING_MODE_REPLACE,
    VTK_TEXTURE_BLENDING_MODE_MODULATE,
    VTK_TEXTURE_BLENDING_MODE_ADD,
    VTK_TEXTURE_BLENDING_MODE_ADD_SIGNED,
    VTK_TEXTURE_BLENDING_MODE_INTERPOLATE,
    VTK_TEXTURE_BLENDING_MODE_SUBTRACT
  };

  //@{
  /**
   * Used to specify how the texture will blend its RGB and Alpha values
   * with other textures and the fragment the texture is rendered upon.
   */
  vtkGetMacro(BlendingMode, int);
  vtkSetMacro(BlendingMode, int);
  //@}

  //@{
  /**
   * Whether the texture colors are premultiplied by alpha.
   * Initial value is false.
   */
  vtkGetMacro(PremultipliedAlpha, bool);
  vtkSetMacro(PremultipliedAlpha, bool);
  vtkBooleanMacro(PremultipliedAlpha, bool);
  //@}

  //@{
  /**
   * When the texture is forced to be a power of 2, the default behavior is
   * for the "new" image's dimensions  to be greater than or equal to with
   * respects to the original.  Setting RestrictPowerOf2ImageSmaller to be
   * 1 (or ON) with force the new image's dimensions to be less than or equal
   * to with respects to the original.
   */
  vtkGetMacro(RestrictPowerOf2ImageSmaller, int);
  vtkSetMacro(RestrictPowerOf2ImageSmaller, int);
  vtkBooleanMacro(RestrictPowerOf2ImageSmaller, int);
  //@}

  /**
   * Is this Texture Translucent?
   * returns false (0) if the texture is either fully opaque or has
   * only fully transparent pixels and fully opaque pixels and the
   * Interpolate flag is turn off.
   */
  virtual int IsTranslucent();

  /**
   * Return the texture unit used for this texture
   */
  virtual int GetTextureUnit() { return 0; }

protected:
  vtkTexture();
  ~vtkTexture();

  // A texture is a sink, so there is no need to do anything.
  // This definition avoids a warning when doing Update() on a vtkTexture object.
  virtual void ExecuteData(vtkDataObject *)
  {
  }

  int Repeat;
  int EdgeClamp;
  int Interpolate;
  int Quality;
  int MapColorScalarsThroughLookupTable;
  vtkScalarsToColors* LookupTable;
  vtkUnsignedCharArray* MappedScalars;
  vtkTransform * Transform;

  int BlendingMode;
  int RestrictPowerOf2ImageSmaller;
  // this is to duplicated the previous behavior of SelfCreatedLookUpTable
  int SelfAdjustingTableRange;
  bool PremultipliedAlpha;

  // the result of HasTranslucentPolygonalGeometry is cached
  vtkTimeStamp TranslucentComputationTime;
  int TranslucentCachedResult;

private:
  vtkTexture(const vtkTexture&) VTK_DELETE_FUNCTION;
  void operator=(const vtkTexture&) VTK_DELETE_FUNCTION;
};

#endif
