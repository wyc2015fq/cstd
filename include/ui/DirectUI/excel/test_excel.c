
#include "debug.h"
#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <assert.h>

#define SelectPen(m_hDC, hPen) (HPEN)SelectObject(m_hDC, hPen)
#define SelectBrush(m_hDC, hBrush) (HBRUSH)SelectObject(m_hDC, hBrush)
#define SelectFont(m_hDC, hFont) (HFONT)SelectObject(m_hDC, hFont)
#define SelectBitmap(m_hDC, hBitmap) (HBITMAP)SelectObject(m_hDC, hBitmap)
#define SelectRgn(m_hDC, hRgn) (HRGN)SelectObject(m_hDC, hRgn)
#define MoveTo(m_hDC, x, y)  MoveToEx(m_hDC, x, y, 0)
//#define _ASSERT assert
typedef int bool;
#define false 0
#define true 1
#include "strid.h"
#include "excel.h"
#include "..\Markup.inl"
//#include "..\PtrArray.inl"
//#include "..\StringPtrMap.inl"
#include "XmlExcelBase.inl"
//#include "renderer.h"
#include "RgnLight.h"
#include "renderer.h"
#include "renderer.inl"
#include "htmlcodes.inl"
#include "htmlviewer.h"
#include "htmlviewer.inl"
#include "htmlcontrol.h"
#include "htmlcontrol.inl"

int test_xml() {
  CMarkup xml = {0};

  LoadFromFile(&xml, "example1.xml", XMLFILE_ENCODING_UTF8);
  //LoadFromFile(&xml, "test.xml", XMLFILE_ENCODING_UTF8);
  {
    int nAttributes = 0;
    LPCTSTR pstrName = NULL;
    LPCTSTR pstrValue = NULL;
    LPTSTR pstr = NULL;
    LPCTSTR pstrClass;
    CMarkupNode node, root;
    root = GetRoot(&xml);
    pstrClass = GetName(&root);
    node = GetChildNode(&root, 0);
    for ( ; IsValidNode(&node); node = GetSiblingNode(&node) ) {
      int i, na = GetAttributeCount(&node);
      pstrClass = GetName(&node);
      pstrValue = GetValue(&node);
      printf("%s = %s\n", pstrClass, pstrValue);
      for (i=0; i<na; ++i) {
        pstrClass = GetAttributeName(&node, i);
        pstrValue = GetAttributeValueByIndex(&node, i);
        printf("    %s = %s\n", pstrClass, pstrValue);
      }
    }
    Release(&xml);
  }
  return 0;
}

int test_excel() {
  Workbook workbook;
  LoadWorkbook(&workbook, "example2.xml");
  Free_Workbook(&workbook);
  clear_strtable();
  return 0;
}
int main() {
  MEM_CHECK_BEGIN();
  test_excel();
  MEM_CHECK_END();
  return 0;
}
