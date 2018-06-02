
#ifndef vtkHomogeneousTransform_h
#define vtkHomogeneousTransform_h

#include "vtkAbstractTransform.h"

class vtkMatrix4x4;

class vtkHomogeneousTransform : public vtkAbstractTransform
{
public:

  void PrintSelf(ostream& os, vtkIndent indent) VTK_OVERRIDE;

  /**
   * Apply the transformation to a series of points, and append the
   * results to outPts.
   */
  void TransformPoints(vtkPoints *inPts, vtkPoints *outPts) VTK_OVERRIDE;

  /**
   * Apply the transformation to a combination of points, normals
   * and vectors.
   */
  void TransformPointsNormalsVectors(vtkPoints *inPts,
                                     vtkPoints *outPts,
                                     vtkDataArray *inNms,
                                     vtkDataArray *outNms,
                                     vtkDataArray *inVrs,
                                     vtkDataArray *outVrs) VTK_OVERRIDE;

  /**
   * Get a copy of the internal transformation matrix.  The
   * transform is Updated first, to guarantee that the matrix
   * is valid.
   */
  void GetMatrix(vtkMatrix4x4 *m);

  /**
   * Get a pointer to an internal vtkMatrix4x4 that represents
   * the transformation.  An Update() is called on the transform
   * to ensure that the matrix is up-to-date when you get it.
   * You should not store the matrix pointer anywhere because it
   * might become stale.
   */
  vtkMatrix4x4 *GetMatrix() { this->Update(); return this->Matrix; };

  /**
   * Just like GetInverse(), but includes typecast to vtkHomogeneousTransform.
   */
  vtkHomogeneousTransform *GetHomogeneousInverse()
  {
      return static_cast<vtkHomogeneousTransform *>(this->GetInverse());
  }

  //@{
  /**
   * This will calculate the transformation without calling Update.
   * Meant for use only within other VTK classes.
   */
  void InternalTransformPoint(const float in[3], float out[3]) VTK_OVERRIDE;
  void InternalTransformPoint(const double in[3], double out[3]) VTK_OVERRIDE;
  //@}

  //@{
  /**
   * This will calculate the transformation as well as its derivative
   * without calling Update.  Meant for use only within other VTK
   * classes.
   */
  void InternalTransformDerivative(const float in[3], float out[3],
                                   float derivative[3][3]) VTK_OVERRIDE;
  void InternalTransformDerivative(const double in[3], double out[3],
                                   double derivative[3][3]) VTK_OVERRIDE;
  //@}

public:
  vtkHomogeneousTransform();
  ~vtkHomogeneousTransform() VTK_OVERRIDE;

  void InternalDeepCopy(vtkAbstractTransform *transform) VTK_OVERRIDE;

  vtkMatrix4x4 *Matrix;

private:
  vtkHomogeneousTransform(const vtkHomogeneousTransform&) VTK_DELETE_FUNCTION;
  void operator=(const vtkHomogeneousTransform&) VTK_DELETE_FUNCTION;
};

#endif





