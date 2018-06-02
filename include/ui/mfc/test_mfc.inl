
#include "mfc/mfc.inl"
//#include "bcg/bcg.inl"

//#include "test_mfcDlg.cpp"
//#include "test_mfc.inl"
//#include "testmdi/testmdi.inl"
//#include "testsdi/testsdi.inl"
#include "testdlg/testdlg.inl"
//#include "BCG/BCG.inl"

//#include "BCGCBPro/libbcg.inl"

// kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib 
int test_mfc() {
  CString aa;
  aa = "adf";
  CString bb(aa);
	// AfxGetModuleState
  AfxWinMain(GetModuleHandle(0), 0, 0, 1);
  return 0;
}

