
//#include "MarkXML.h"
#include "Markup.inl"

int test_MarkXML() {
  {
    //1. Éú³ÉUserInfo.xml
    CMarkup xml = {0};
#define TESTFILE "UserInfo1.xml"
    int sz = filesize(TESTFILE);
    //char* buf;
    //loaddatafn(TESTFILE, &buf, sz, 0);
    //xml.m_pstrXML = buf;
    //_Parse(&xml);
    LoadFromFile(&xml, TESTFILE, XMLFILE_ENCODING_ASNI);
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
    //free(buf);
  }
  return 0;
}
int main(int argc, char** argv) {
  test_MarkXML();
  return 0;
}