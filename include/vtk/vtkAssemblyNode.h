
#ifndef vtkAssemblyNode_h
#define vtkAssemblyNode_h

class vtkProp;
class vtkMatrix4x4;

class VTKRENDERINGCORE_EXPORT vtkAssemblyNode
{
public:
  LISTDEF(vtkAssemblyNode);
  /**
   * Create an assembly node.
   */
  static vtkAssemblyNode *New();

  vtkTypeMacro(vtkAssemblyNode, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  //@{
  /**
   * Set/Get the prop that this assembly node refers to.
   */
  virtual void SetViewProp(vtkProp* prop);
  vtkGetObjectMacro(ViewProp, vtkProp);
  //@}

  //@{
  /**
   * Specify a transformation matrix associated with the prop.
   * Note: if the prop is not a type of vtkProp3D, then the
   * transformation matrix is ignored (and expected to be NULL).
   * Also, internal to this object the matrix is copied because
   * the matrix is used for computation by vtkAssemblyPath.
   */
  void SetMatrix(vtkMatrix4x4 *matrix);
  vtkGetObjectMacro(Matrix, vtkMatrix4x4);
  //@}

  /**
   * Override the standard GetMTime() to check for the modified times
   * of the prop and matrix.
   */
  virtual vtkMTimeType GetMTime();

protected:
  vtkAssemblyNode();
  ~vtkAssemblyNode();

private:
  vtkProp *ViewProp; //reference to vtkProp
  vtkMatrix4x4 *Matrix; //associated matrix

private:
  void operator=(const vtkAssemblyNode&) VTK_DELETE_FUNCTION;
  vtkAssemblyNode(const vtkAssemblyNode&) VTK_DELETE_FUNCTION;
};

#endif
