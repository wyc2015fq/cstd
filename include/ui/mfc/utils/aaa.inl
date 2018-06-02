typedef struct DLGITEM_T {
  char m_strCaption[256];
  DLGITEMTEMPLATE m_dlgItemTemplate;
  WORD m_controltype;
} DLGITEM_T;
static void* memcpy_s(void* dst, size_t dstlen, const void* src, size_t srclen)
{
  if (srclen <= dstlen) {
    return memcpy(dst, src, srclen);
  }
  return NULL;
}
DLGTEMPLATE* getdlg(const DLGTEMPLATE* m_dlgTempl, const DLGITEM_T* m_rgDlgItem, int TOTALITEMS)
{
  // DLGTEMPLATE和DLGITEMTEMPLATE一般后面直接跟3个word数组，通常他们都是按照DWORD对齐的，
  // 三个数组分别表示menu、class和title，DLGITEMTEMPLATE是紧跟着DLGTEMPLATE出现的，
  // 可以有多个，每个表示DLGTEMPLATE中的一个控件。这个vc sample中的dlgtemp的示例中有介绍：
  // The first step is to allocate memory to define the dialog. The information to be
  // stored in the allocated buffer is the following:
  //
  // 1. DLGTEMPLATE structure
  // 2. 0x0000 (Word) indicating the dialog has no menu
  // 3. 0x0000 (Word) Let windows assign default class to the dialog
  // 4. (Caption) Null terminated unicode string
  // 5. 0x000B (size of the font to be used)
  // 6. "Arial" (name of the typeface to be used)
  // 7. DLGITEMTEMPLATE structure for the button (HAS TO BE DWORD ALIGNED)
  // 8. 0x0080 to indicate the control is a button
  // 9. (Title). Unicode null terminated string with the caption
  // 10. 0x0000 0 extra bytes of data for this control
  // 11. DLGITEMTEMPLATE structure for the Static Text (HAS TO BE DWORD ALIGNED)
  // 12. 0x0081 to indicate the control is static text
  // 13. (Title). Unicode null terminated string with the text
  // 14 0x0000. 0 extra bytes of data for this control
  // 15. DLGITEMTEMPLATE structure for the Edit Control (HAS TO BE DWORD ALIGNED)
  // 16. 0x0082 to indicate an Edit control
  // 17. (Text) - Null terminated unicode string to appear in the edit control
  // 18. 0x0000. 0 extra bytes of data for this control
  WCHAR szBoxCaption[] = L"Dialog Template Sample - PRESS <ESC> TO CLOSE THIS DIALOG";
  WCHAR szFontName[] = L"ARIAL";
  // will first convert the control captions to UNICODE
  int i;
  // catch memory exceptions and don't worry about allocation failures
  // The following expressions have unnecessary parenthesis trying to make the
  // comments more clear.
  int nBufferSize = sizeof(DLGTEMPLATE) + (2 * sizeof(WORD))/*menu and class*/ + sizeof(szBoxCaption);
  nBufferSize += sizeof(WORD) + sizeof(szFontName); /* font information*/
  nBufferSize = (nBufferSize + 3)& ~3; // adjust size to make first control DWORD aligned
  for (i = 0; i < TOTALITEMS; i++) {
    int nItemLength = sizeof(DLGITEMTEMPLATE) + 3 * sizeof(WORD);
    nItemLength += (strlen(m_rgDlgItem[i].m_strCaption) + 1) * sizeof(WCHAR);
    if (i != TOTALITEMS - 1) { // the last control does not need extra bytes
      nItemLength = (nItemLength + 3) & ~3; // take into account gap so next control is DWORD aligned
    }
    nBufferSize += nItemLength;
  }
  BYTE* pBuffer = (BYTE*)malloc(nBufferSize);
  if (pBuffer == NULL) {
    //printf("Memory allocation for dialog template failed. Demo aborted!");
    return NULL;
  }
  BYTE* pdest = pBuffer;
  BYTE* pdestEnd = pdest + nBufferSize;
  // transfer DLGTEMPLATE structure to the buffer
  memcpy_s(pdest, pdestEnd - pdest, &m_dlgTempl, sizeof(DLGTEMPLATE));
  pdest += sizeof(DLGTEMPLATE);
  *(WORD*)pdest = 0; // no menu
  *(WORD*)(pdest + sizeof(WORD)) = 0; // use default window class
  pdest += 2 * sizeof(WORD);
  memcpy_s(pdest, pdestEnd - pdest, szBoxCaption, sizeof(szBoxCaption));
  pdest += sizeof(szBoxCaption);
  *(WORD*)pdest = 11; // font size
  pdest += sizeof(WORD);
  memcpy_s(pdest, pdestEnd - pdest, szFontName, sizeof(szFontName));
  pdest += sizeof(szFontName);
  // will now transfer the information for each one of the item templates
  for (i = 0; i < TOTALITEMS; i++) {
    pdest = (BYTE*)(((DWORD_PTR)pdest + 3) & ~3); // make the pointer DWORD aligned
    memcpy_s(pdest, pdestEnd - pdest, (void*)&m_rgDlgItem[i].m_dlgItemTemplate, sizeof(DLGITEMTEMPLATE));
    pdest += sizeof(DLGITEMTEMPLATE);
    *(WORD*)pdest = 0xFFFF; // indicating atom value
    pdest += sizeof(WORD);
    *(WORD*)pdest = (WORD)m_rgDlgItem[i].m_controltype; // atom value for the control
    pdest += sizeof(WORD);
    // transfer the caption even when it is an empty string
    WCHAR* pchCaption;
    int nChars, nActualChars;
    nChars = strlen(m_rgDlgItem[i].m_strCaption) + 1;
    pchCaption = new WCHAR[nChars];
    nActualChars = MultiByteToWideChar(CP_ACP, 0, m_rgDlgItem[i].m_strCaption, -1, pchCaption, nChars);
    ASSERT(nActualChars > 0);
    memcpy_s(pdest, pdestEnd - pdest, pchCaption, nActualChars * sizeof(WCHAR));
    pdest += nActualChars * sizeof(WCHAR);
    delete [] pchCaption;
    *(WORD*)pdest = 0; // How many bytes in data for control
    pdest += sizeof(WORD);
  }
  ASSERT(pdest - pBuffer == nBufferSize); // just make sure we did not overrun the heap
  return (DLGTEMPLATE*)pBuffer;
}

