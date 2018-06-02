
#include <tchar.h>
#include "Counting.inl"

int CountLines(const TCHAR* m_strComboFolder, const TCHAR* m_strComboExt);

int main(int argc, TCHAR* argv[]) {
  int nRetCode = 0;

  TCHAR* strComboExt = (argc>2) ? argv[1] : _T("*.c;*.cpp;*.h");
  TCHAR* strComboFolder = (argc>1) ? argv[1] : _T(".");
  CountLines(strComboFolder, strComboExt);

  return nRetCode;
}


