
/// <summary>
/// Provides render functionallity for the Ribbon control
/// </summary>
/// <remarks></remarks>


/// <summary>
/// Multiplies the color matrix (Extracted from .NET Framework
/// </summary>
/// <param name="matrix1"></param>
/// <param name="matrix2"></param>
/// <returns></returns>
static void MultiplyColorMatrix(float* newColorMatrix, const float* matrix1, const float* matrix2)
{
  int j, k, m, n;
  enum { num = 5 };
  float numArray2[num];
  for (j = 0; j < num; j++)
  {
    for (k = 0; k < num; k++) {
      numArray2[k] = matrix1[k*5+j];
    }
    for (m = 0; m < num; m++) {
      const float* numArray3 = matrix2+m;
      float num6 = 0.f;
      for (n = 0; n < num; n++) {
        num6 += numArray3[n] * numArray2[n];
      }
      newColorMatrix[m*5+j] = num6;
    }
  }
}


/// <summary>
/// Gets the disabled image color matrix
/// </summary>
static float* GetDisabledImageColorMatrix()
{
  static float _disabledImageColorMatrix[25];
  static int inited = 0;
  if (!inited)
  {
    float numArray[25] = {
       0.2125f, 0.2125f, 0.2125f, 0.f, 0.f ,
       0.2577f, 0.2577f, 0.2577f, 0.f, 0.f ,
       0.0361f, 0.0361f, 0.0361f, 0.f, 0.f ,
       0.f, 0.f, 0.f, 1.f, 0.f,
       0.38f, 0.38f, 0.38f, 0.f, 1.f
    };
    float numArray2[25] = {
       1.f, 0.f, 0.f, 0.f, 0.f ,
       0.f, 1.f, 0.f, 0.f, 0.f ,
       0.f, 0.f, 1.f, 0.f, 0.f ,
       0.f, 0.f, 0.f, 0.7f, 0.f ,
       0.f, 0.f, 0.f, 0.f, 0.f ,
    };
    inited = 0;
    MultiplyColorMatrix(_disabledImageColorMatrix, numArray2, numArray);
  }
  return _disabledImageColorMatrix;
}
#if 0
/// <summary>
/// Creates the disabled image for the specified Image
/// </summary>
/// <param name="normalImage"></param>
/// <returns></returns>
static Image CreateDisabledImage(Image normalImage)
{
  ImageAttributes imageAttr = new ImageAttributes();
  imageAttr.ClearColorKey();
  imageAttr.SetColorMatrix(DisabledImageColorMatrix);
  Size size = normalImage.Size;
  Bitmap image = new Bitmap(size.Width, size.Height);
  Graphics graphics = Graphics.FromImage(image);
  graphics.DrawImage(normalImage, new Rectangle(0, 0, size.Width, size.Height), 0, 0, size.Width, size.Height, GraphicsUnit.Pixel, imageAttr);
  graphics.Dispose();
  return image;
}
#endif
