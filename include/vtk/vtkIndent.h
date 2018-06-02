
#ifndef vtkIndent_h
#define vtkIndent_h

#include <iostream>

using namespace std;

#define VTK_STD_INDENT 2
#define VTK_NUMBER_OF_BLANKS 40

static const char blanks[VTK_NUMBER_OF_BLANKS+1]="                                        ";

class vtkIndent
{
public:
  explicit vtkIndent(int ind=0) {this->Indent=ind;};
  /**
   * Determine the next indentation level. Keep indenting by two until the
   * max of forty.
   */
  vtkIndent GetNextIndent() 
  {
    int indent = this->Indent + VTK_STD_INDENT;
    if ( indent > VTK_NUMBER_OF_BLANKS )
    {
      indent = VTK_NUMBER_OF_BLANKS;
    }
    return vtkIndent(indent);
}

  
  int Indent;
public:

};

/**
 * Print out the indentation. Basically output a bunch of spaces.
 */
static inline std::ostream& operator<<(std::ostream& os, const vtkIndent& ind) 
{
  os << blanks + (VTK_NUMBER_OF_BLANKS-ind.Indent) ;
  return os;
}

template <typename T, typename Y> inline
void vPrintSelfImpl(T* a, Y* b, std::ostream& os, const vtkIndent& ind) {
  vtkIndent ind1 = ind;
  for (; (b = (b)->next); ) {
    b->PrintSelf(os, ind1);
  }
}

template <typename T> inline
void vPrintSelf(T* a, std::ostream& os, const vtkIndent& ind) {
  vPrintSelfImpl(a, a->first, os, ind);
}

#endif

// VTK-HeaderTest-Exclude: vtkIndent.h
