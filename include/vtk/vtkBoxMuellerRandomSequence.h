
#ifndef vtkBoxMuellerRandomSequence_h
#define vtkBoxMuellerRandomSequence_h

#include "vtkGaussianRandomSequence.h"

class VTKCOMMONCORE_EXPORT vtkBoxMuellerRandomSequence: public vtkGaussianRandomSequence
{
public:
  vtkTypeMacro(vtkBoxMuellerRandomSequence,vtkGaussianRandomSequence);
  void PrintSelf(ostream& os, vtkIndent indent) VTK_OVERRIDE;

  static vtkBoxMuellerRandomSequence* New();

  /**
   * Current value.
   */
  double GetValue() VTK_OVERRIDE;

  /**
   * Move to the next number in the random sequence.
   */
  void Next() VTK_OVERRIDE;

  /**
   * Return the uniformly distributed sequence of random numbers.
   */
  vtkRandomSequence *GetUniformSequence();

  /**
   * Set the uniformly distributed sequence of random numbers.
   * Default is a .
   */
  void SetUniformSequence(vtkRandomSequence *uniformSequence);

  vtkBoxMuellerRandomSequence();
  ~vtkBoxMuellerRandomSequence() VTK_OVERRIDE;

  vtkRandomSequence *UniformSequence;
  double Value;
private:
  vtkBoxMuellerRandomSequence(const vtkBoxMuellerRandomSequence&) VTK_DELETE_FUNCTION;
  void operator=(const vtkBoxMuellerRandomSequence&) VTK_DELETE_FUNCTION;
};

#endif // #ifndef vtkBoxMuellerRandomSequence_h
