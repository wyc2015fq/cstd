
#include "String.hpp"
#include "TimeSpan.hpp"
#include "Time.hpp"
#if 0
#include "File.hpp"
#include "Archive.hpp"
#include "FileFind.hpp"
#include "ByteArray.hpp"
#include "WordArray.hpp"
#include "DWordArray.hpp"
#include "UIntArray.hpp"
#include "PtrArray.hpp"
#include "StringArray.hpp"
#include "PtrList.hpp"
#include "StringList.hpp"
#include "MapWordToPtr.hpp"
#include "MapPtrToWord.hpp"
#include "MapPtrToPtr.hpp"
#include "MapStringToPtr.hpp"
#include "String.hpp"

#include "Map.hpp"
#include "List.hpp"
#include "Array.hpp"
//#include "mfcwin.h"


int test_StringList() {
  // 初始化：
  CStringList listFileName;
  listFileName.RemoveAll();
  
  CString szFullPathName = "asdf";
  CString strFileName = "asdf";
  //添加操作：
  listFileName.AddTail(szFullPathName);
  
  //遍历操作：
  
  POSITION rPos;
  rPos = listFileName.GetHeadPosition();
  CString strLog;
  while (rPos != NULL)
  {
    strFileName = listFileName.GetNext(rPos);
    strLog += "/r/n" + strFileName;
  }
  return 0;
}
#endif



int test_std() {
  //test_StringList();
  return 0;
}

