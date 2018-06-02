
#pragma comment(lib, "msimg32.lib") 
// CCoolEdit
const int nMargin = 5;
#define SHIFTED 0x8000
// 文本的字体
#define m_TxtFont UIResource_GetFont(ww->m_pResource, this->m_iFont) 
// CCoolEdit window

// 当前选择的文本
//#define GetSel( nStartChar, nEndChar )   (nStartChar = this->m_nSelectBegin, nEndChar = this->m_nSelectEnd)
#define GetSelText()         (ctr->m_sText + this->m_nSelectBegin)
#define GetSelTextLength()   (this->m_nSelectEnd - this->m_nSelectBegin)
#define strdel(s, start, len) strcpy((char*)(s)+start, (char*)(s)+start+len)

#define TxSetCaretPos(x, y)  SetCaretPos(x, y)
#define UICoolEdit_CtrFree CControlUI_CtrFree
#define UICoolEdit_SetPos CControlUI_SetPos
int UICoolEdit_CtrInit(CControlUI* ctr) {
  UICoolEdit* this = (UICoolEdit*)ctr;
  CControlUI_CtrInit(ctr);
  this->m_ptCursor.y = this->m_ptTxtBegin.y = 0;
  this->m_ptTxtBegin.x = this->m_ptCursor.x = nMargin;
  // GetSelText() = "";
  this->m_nCharPos = 0;
  this->m_nShowChar = 0;
  this->m_nMaxShowChars = 0;
  this->m_nSelectBegin = 0;
  this->m_nSelectEnd = 0;
  this->m_bLBtDown = false;
  this->m_ptFirst.x = this->m_ptFirst.y = 0; //this->m_ptSecond.x = this->m_ptSecond.y = 0;
  memset( &this->m_tm, 0, sizeof( TEXTMETRIC ) );
  //m_TxtFont.CreateFont( 15, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS,
  //                      CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "宋体" );
  this->m_nShiftBegin = -1;
  this->m_bDrawShade = FALSE;
  //SetBkColor(RGB( 255, 255, 255 ));
  //SetBkColor2(RGB( 255, 255, 255 ));
  //SetBkColor2(RGB( 148, 182, 225 ));
  //SetBkColor( RGB( 148, 182, 225 ) );
  //SetBkColor2( RGB( 46, 74, 137 ) );
  //SetTextColor( RGB( 253, 255, 247 ) );
  return 0;
}
//设置选择
//nStartChar 选择中的第一个字符的从零开始的索引
//nEndChar 选择中的最后一个字符的从零开始的索引
void UICoolEdit_SetSel( CControlUI* ctr, long nStartChar, long nEndChar ) {
  UICoolEdit* this = (UICoolEdit*)ctr;
  this->m_nSelectBegin = nStartChar;
  this->m_nSelectEnd = nEndChar;
  return ;
}

#define GetString()  ctr->m_sText
char* strmid(const char* s, int start, int len) {
  char* d = (char*)malloc(len+1);
  memcpy(d, s+start, len); d[len]=0; return d;
}
// 通过指定的字符获得其x轴的位置
POINT PosFromChar( CWindowWnd* ww, CControlUI* ctr, UINT nChar ) {
  UICoolEdit* this = (UICoolEdit*)ctr;
  POINT pt;
  HDC dc = ww->m_hDcPaint;
  HFONT pOldFont = (HFONT)SelectObject( dc, m_TxtFont );
  SIZE TestSize = {0,0};
  int ret=GetTextExtentPoint32( dc, ctr->m_sText+this->m_nShowChar, nChar - this->m_nShowChar, &TestSize );
  if (0==ret) {
    MessageError();
  }
  SelectObject( dc, pOldFont );
  pt.x = ctr->m_rcItem.left + TestSize.cx + this->m_ptTxtBegin.x;
  pt.y = ctr->m_rcItem.top + this->m_ptTxtBegin.y;
  return pt;
}
// 通过给定的x轴的位置获得相应的字符
int CharFromPos( CWindowWnd* ww, CControlUI* ctr, int nPosX0 ) {
  UICoolEdit* this = (UICoolEdit*)ctr;
  const char* strTemp = ctr->m_sText+( this->m_nShowChar );
  //TRACE("%s=%s=%d\n", ctr->m_sText, strTemp, this->m_nShowChar);
  int nCharIndex = this->m_nShowChar;
  int nStringLen = (int)strlen(strTemp);
  char TempChar[ 4 ];
  int nBeginPos = nMargin;
  int nPosX = nPosX0 - ctr->m_rcItem.left-nMargin;
  SIZE TxtSize;
  const char *cText = ( const char * ) strTemp;
  HDC dc = ww->m_hDcPaint;
  HFONT pOldFont = (HFONT)SelectObject( dc, m_TxtFont );
  while ( nStringLen > 0 ) {
    if ( ( ( unsigned char ) * cText ) > 127 &&
      ( ( unsigned char ) * ( cText + 1 ) ) > 127 ) {
      strncpy( TempChar, cText, 2 );
      TempChar[ 2 ] = 0;
      cText += 2;
      nStringLen -= 2;
      nCharIndex += 2;
      GetTextExtentPoint32( dc, TempChar, 2, &TxtSize );
    } else {
      strncpy( TempChar, cText, 1 );
      TempChar[ 1 ] = 0;
      cText += 1;
      nStringLen -= 1;
      nCharIndex += 1;
      GetTextExtentPoint32( dc, TempChar, 1, &TxtSize );
    }
    if ( nBeginPos + TxtSize.cx > nPosX ) {
      if ( nPosX - nBeginPos < nBeginPos + TxtSize.cx - nPosX )
        nCharIndex -= (int)strlen( TempChar );
      break;
    }
    nBeginPos += TxtSize.cx;
  }
  SelectObject( dc, pOldFont );
  return nCharIndex;
}
// 处理键盘操作
BOOL OnKeyDown( CWindowWnd* ww, CControlUI* ctr, TCHAR wParam ) {
  UICoolEdit* this = (UICoolEdit*)ctr;
  const char *cText;
  switch ( wParam ) {
  case VK_UP:
  case VK_LEFT: {
      int nVirtKey = GetKeyState( VK_SHIFT );
      if ( nVirtKey & SHIFTED ) {
        if ( this->m_nShiftBegin < 0 ) {
          if ( GetSelTextLength() > 0 ) {
            if ( this->m_nCharPos < this->m_nSelectEnd )
              this->m_nShiftBegin = this->m_nSelectEnd;
            else
              this->m_nShiftBegin = this->m_nSelectBegin;
          } else {
            this->m_nShiftBegin = this->m_nCharPos;
          }
        }
      } else {
        this->m_nShiftBegin = -1;
        // GetSelText() = "";
      }
      HideCaret( ww->m_hWnd );
      cText = ( const char * ) ctr->m_sText;
      if ( this->m_nCharPos > 0 ) {
        if ( ( unsigned char ) cText[ this->m_nCharPos - 1 ] > 127 ) {
          this->m_nCharPos -= 2;
        } else
          this->m_nCharPos--;
      }
      while ( this->m_nCharPos - this->m_nShowChar < 0 ) {
        if ( ( unsigned char ) cText[ this->m_nShowChar - 1 ] > 127 )
          this->m_nShowChar -= 2;
        else
          this->m_nShowChar--;
      }
      if ( nVirtKey & SHIFTED ) {
        if ( this->m_nShiftBegin < this->m_nCharPos ) {
          this->m_nSelectBegin = this->m_nShiftBegin;
          this->m_nSelectEnd = this->m_nCharPos;
        } else {
          this->m_nSelectBegin = this->m_nCharPos;
          this->m_nSelectEnd = this->m_nShiftBegin;
        }
        // GetSelText() = ctr->m_sText.Mid( this->m_nSelectBegin, this->m_nSelectEnd - this->m_nSelectBegin );
      }
      this->m_ptCursor = PosFromChar( ww, ctr, this->m_nCharPos );
      Invalidate(ww->m_hWnd);
      TxSetCaretPos( this->m_ptCursor.x, this->m_ptCursor.y );
      ShowCaret( ww->m_hWnd );
      return TRUE;
    }
  case VK_DOWN:
  case VK_RIGHT: {
      int nVirtKey = GetKeyState( VK_SHIFT );
      if ( nVirtKey & SHIFTED ) {
        if ( this->m_nShiftBegin < 0 ) {
          if ( GetSelTextLength() > 0 ) {
            if ( this->m_nCharPos < this->m_nSelectEnd )
              this->m_nShiftBegin = this->m_nSelectEnd;
            else
              this->m_nShiftBegin = this->m_nSelectBegin;
          } else {
            this->m_nShiftBegin = this->m_nCharPos;
          }
        }
      } else {
        this->m_nShiftBegin = -1;
        // GetSelText() = "";
      }
      HideCaret( ww->m_hWnd );
      cText = ( const char * ) ctr->m_sText;
      if ( this->m_nCharPos < (int)strlen(ctr->m_sText) ) {
        if ( ( unsigned char ) cText[ this->m_nCharPos ] > 127 ) {
          this->m_nCharPos += 2;
        } else
          this->m_nCharPos++;
      }
      cText += this->m_nShowChar;
      while ( this->m_nCharPos - this->m_nShowChar > this->m_nMaxShowChars ) {
        if ( ( ( unsigned char ) * cText ) > 127 && ( ( unsigned char ) * ( cText + 1 ) ) > 127 ) {
          this->m_nShowChar += 2;
          cText += 2;
        } else {
          this->m_nShowChar++;
          cText++;
        }
      }
      if ( nVirtKey & SHIFTED ) {
        if ( this->m_nShiftBegin < this->m_nCharPos ) {
          this->m_nSelectBegin = this->m_nShiftBegin;
          this->m_nSelectEnd = this->m_nCharPos;
        } else {
          this->m_nSelectBegin = this->m_nCharPos;
          this->m_nSelectEnd = this->m_nShiftBegin;
        }
        // GetSelText() = ctr->m_sText.Mid( this->m_nSelectBegin, this->m_nSelectEnd - this->m_nSelectBegin );
      }
      this->m_ptCursor = PosFromChar( ww, ctr, this->m_nCharPos );
      Invalidate(ww->m_hWnd);
      ShowCaret( ww->m_hWnd );
      TxSetCaretPos( this->m_ptCursor.x, this->m_ptCursor.y );
      return TRUE;
    }
  case VK_HOME: {
      int nVirtKey = GetKeyState( VK_SHIFT );
      if ( nVirtKey & SHIFTED ) {
        if ( this->m_nShiftBegin < 0 ) {
          this->m_nSelectEnd = this->m_nCharPos;
          this->m_nShiftBegin = this->m_nCharPos;
        } else
          this->m_nSelectEnd = this->m_nShiftBegin;
        this->m_nSelectBegin = 0;
        // GetSelText() = ctr->m_sText.Mid( this->m_nSelectBegin, this->m_nSelectEnd - this->m_nSelectBegin );
      } else {
        this->m_nShiftBegin = -1;
        // GetSelText() = "";
        this->m_nSelectBegin = this->m_nSelectEnd = 0;
      }
      HideCaret( ww->m_hWnd );
      this->m_nCharPos = 0;
      this->m_nShowChar = 0;
      this->m_ptCursor = PosFromChar( ww, ctr, this->m_nCharPos );
      Invalidate(ww->m_hWnd);
      ShowCaret( ww->m_hWnd );
      TxSetCaretPos( this->m_ptCursor.x, this->m_ptCursor.y );
      return TRUE;
    }
  case VK_END: 
    {
      int nIndex, nShowChars;
      int nVirtKey = GetKeyState( VK_SHIFT );
      if ( nVirtKey & SHIFTED ) {
        if ( this->m_nShiftBegin < 0 ) {
          this->m_nSelectBegin = this->m_nCharPos;
          this->m_nShiftBegin = this->m_nCharPos;
        } else
          this->m_nSelectBegin = this->m_nShiftBegin;
        this->m_nSelectEnd = (int)strlen(ctr->m_sText);
        // GetSelText() = ctr->m_sText.Mid( this->m_nSelectBegin, this->m_nSelectEnd - this->m_nSelectBegin );
      } else {
        this->m_nShiftBegin = -1;
        // GetSelText() = "";
        this->m_nSelectBegin = this->m_nSelectEnd = 0;
      }
      HideCaret( ww->m_hWnd );
      this->m_nCharPos = (int)strlen(ctr->m_sText);
      cText = ( const char* ) ctr->m_sText;
      nIndex = this->m_nCharPos - 1;
      nShowChars = 0;
      while ( nShowChars < this->m_nMaxShowChars ) {
        if ( ( unsigned char ) cText[ nIndex ] > 127 ) {
          nIndex -= 2;
          nShowChars += 2;
        } else {
          nIndex -= 1;
          nShowChars += 1;
        }
      }
      this->m_nShowChar = this->m_nCharPos - nShowChars; //this->m_nMaxShowChars;
      if ( this->m_nShowChar < 0 )
        this->m_nShowChar = 0;
      this->m_ptCursor = PosFromChar( ww, ctr, this->m_nCharPos );
      Invalidate(ww->m_hWnd);
      ShowCaret( ww->m_hWnd );
      TxSetCaretPos( this->m_ptCursor.x, this->m_ptCursor.y );
      return TRUE;
    }
  case VK_DELETE:
    if (0==GetSelTextLength()) {
      int slen = strlen(ctr->m_sText);
      if ( slen > 0 && this->m_nCharPos < slen ) {
        const char * cText = ( const char * ) ctr->m_sText;
        int dellen = ( ( unsigned char ) cText[ this->m_nCharPos ] > 127 ) ? 2 : 1;
        strdel( ctr->m_sText, this->m_nCharPos, dellen );
        this->m_nSelectEnd = this->m_nSelectBegin;
      }
    } else {
      strdel( ctr->m_sText, this->m_nSelectBegin, GetSelTextLength() );
      this->m_nSelectEnd = this->m_nSelectBegin;
    }
    Invalidate(ww->m_hWnd);
    break;
  }
  return 0;
}
void OnLButtonDown( CWindowWnd* ww, CControlUI* ctr, UINT nFlags, POINT point ) {
  UICoolEdit* this = (UICoolEdit*)ctr;
  //SetFocus();
  // 从当前鼠标的位置获得对应字符的序号
  this->m_nCharPos = CharFromPos( ww, ctr, point.x );
  // 修正 this->m_nCharPos，确保其值不会超过字符串的长度
  if ( this->m_nCharPos > (int)strlen(ctr->m_sText) )
    this->m_nCharPos = (int)strlen(ctr->m_sText);
  this->m_ptCursor = PosFromChar( ww, ctr, this->m_nCharPos );  // 设置光标的位置
  TxSetCaretPos( this->m_ptCursor.x, this->m_ptCursor.y );
  this->m_bLBtDown = true;       // 设置右键按下标志位
  this->m_ptFirst = point;
  // GetSelText() = "";
  this->m_nSelectBegin = this->m_nSelectEnd = 0;
  this->m_nShiftBegin = this->m_nCharPos; //-1;
  Invalidate(ww->m_hWnd);
}
// 窗体获得焦点以后设置光标的位置
void OnSetFocus(CWindowWnd* ww, CControlUI* ctr) {
  UICoolEdit* this = (UICoolEdit*)ctr;
  CreateCaret( ww->m_hWnd, ( HBITMAP ) NULL, 0, this->m_tm.tmHeight );
  TxSetCaretPos( this->m_ptCursor.x, this->m_ptCursor.y );
  ShowCaret( ww->m_hWnd );
}
// 窗口失去焦点的时候销毁光标
void OnKillFocus(CWindowWnd* ww, CControlUI* ctr) {
  UICoolEdit* this = (UICoolEdit*)ctr;
  HideCaret( ww->m_hWnd );
  DestroyCaret();
}
#define cpystr(d, s, len) (memcpy(d, s, len),(d)[len]=0, (d))
// 拷贝字符串
void CopyString(CWindowWnd* ww, CControlUI* ctr) {
  UICoolEdit* this = (UICoolEdit*)ctr;
  if ( GetSelTextLength() <= 0 )
    return ;
  if ( OpenClipboard( ww->m_hWnd ) ) {
    HGLOBAL hClipboardData;
    char * pchData;
    EmptyClipboard();
    hClipboardData = GlobalAlloc( GMEM_DDESHARE, GetSelTextLength() + 1 );
    pchData = ( char* ) GlobalLock( hClipboardData );
    cpystr( pchData, (LPCSTR)( GetSelText() ), GetSelTextLength() );
    GlobalUnlock( hClipboardData );
    SetClipboardData( CF_TEXT, hClipboardData );
    CloseClipboard();
  }
}
char* memins(char* d, const char* s, int slen, int pos, int poslen, const char* s1, int s1len) {
  ASSERT(slen>=pos+poslen);
  if (d!=s) {memmove(d, s, pos);}
  memmove(d+pos+s1len, s+pos+poslen, slen-pos-poslen);
  memmove(d+pos, s1, s1len);
  return d;
}
char* strinsstr(const char* s, int pos, const char* s1) {
  int slen = strlen(s), s1len = strlen(s1);
  char* d = (char*)malloc(slen+s1len+1);
  memins(d, s, slen, pos, 0, s1, s1len);
  d[slen+s1len]=0;
  return d;
}
// 粘贴字符串
void PasteString(CWindowWnd* ww, CControlUI* ctr) {
  UICoolEdit* this = (UICoolEdit*)ctr;
  if ( OpenClipboard( ww->m_hWnd ) ) {
    if ( IsClipboardFormatAvailable( CF_TEXT ) ||
      IsClipboardFormatAvailable( CF_OEMTEXT ) ) {
      const char *cText;
      HANDLE hClipboardData;
      char *pchData;
      int len;
      HideCaret( ww->m_hWnd );
      hClipboardData = GetClipboardData( CF_TEXT );
      pchData = ( char* ) GlobalLock( hClipboardData );
      if ( GetSelTextLength() > 0 ) {
        strdel(ctr->m_sText, this->m_nSelectBegin, GetSelTextLength() );
        this->m_nCharPos = this->m_nSelectBegin;
      }
      this->m_nSelectEnd = this->m_nSelectBegin;
      GlobalUnlock( hClipboardData );
      cText = strinsstr( ctr->m_sText, this->m_nCharPos, pchData );
      free_s(ctr->m_sText); ctr->m_sText = cText;
      len = strlen(pchData);
      this->m_nCharPos += len;
      cText = ctr->m_sText;
      cText += this->m_nShowChar;
      while ( this->m_nCharPos - this->m_nShowChar > this->m_nMaxShowChars ) {
        if ( ( ( unsigned char ) * cText ) > 127 &&
          ( ( unsigned char ) * ( cText + 1 ) ) > 127 ) {
          this->m_nShowChar += 2;
          cText += 2;
        } else {
          this->m_nShowChar++;
          cText++;
        }
      }
      this->m_ptCursor = PosFromChar( ww, ctr, this->m_nCharPos );
      Invalidate(ww->m_hWnd);
      ShowCaret( ww->m_hWnd );
      TxSetCaretPos( this->m_ptCursor.x, this->m_ptCursor.y );
      // GetSelText() = "";
    }
    CloseClipboard();
  }
}
// 剪切字符串
void CutString(CWindowWnd* ww, CControlUI* ctr) {
  UICoolEdit* this = (UICoolEdit*)ctr;
  if ( GetSelTextLength() <= 0 )
    return ;
  strdel(ctr->m_sText, this->m_nSelectBegin, GetSelTextLength() );
  this->m_nSelectEnd = this->m_nSelectBegin;
  CopyString(ww, ctr);
  HideCaret( ww->m_hWnd );
  this->m_nCharPos = this->m_nSelectBegin;
  if ( this->m_nShowChar > 0 ) {
    this->m_nShowChar -= GetSelTextLength();
    if ( this->m_nShowChar < 0 )
      this->m_nShowChar = 0;
  }
  // GetSelText() = "";
  this->m_ptCursor = PosFromChar( ww, ctr, this->m_nCharPos );
  Invalidate(ww->m_hWnd);
  ShowCaret( ww->m_hWnd );
  TxSetCaretPos( this->m_ptCursor.x, this->m_ptCursor.y );
}
char* strinsch(const char* s, int pos, int ch) {
  int n = strlen(s)+1;
  char* new_s = malloc(n+1);
  memcpy(new_s, s, pos);
  memcpy(new_s+pos+1, s+pos, n-pos-1);
  new_s[pos] = ch;
  new_s[n] = 0;
  return new_s;
}
void OnChar( CWindowWnd* ww, CControlUI* ctr, UINT nChar, UINT nRepCnt, UINT nFlags ) {
  UICoolEdit* this = (UICoolEdit*)ctr;
  static bool bSpecial = false;
  HideCaret( ww->m_hWnd );
  switch ( nChar ) {
  case 8:                                    // 响应用户删除操作
    {
      this->m_ptCursor = PosFromChar( ww, ctr, this->m_nCharPos );
      if ( GetSelTextLength() > 0 ) {
        strdel( ctr->m_sText, this->m_nSelectBegin, this->m_nSelectEnd - this->m_nSelectBegin );
        // GetSelText() = "";
        this->m_nCharPos = this->m_nSelectBegin;
        this->m_ptCursor = PosFromChar( ww, ctr, this->m_nCharPos );
        this->m_nSelectBegin = this->m_nSelectEnd = 0;
        this->m_nShiftBegin = -1;
        Invalidate(ww->m_hWnd);
      } else if ( this->m_nCharPos > 0 && (int)strlen(ctr->m_sText) > 0 ) {
        const char * cText = ( const char * ) ctr->m_sText;
        if ( ( unsigned char ) cText[ this->m_nCharPos - 1 ] > 127 ) {
          strdel(ctr->m_sText, this->m_nCharPos - 2, 2 );
          this->m_nCharPos -= 2;
        } else {
          strdel(ctr->m_sText, this->m_nCharPos - 1, 1 );
          this->m_nCharPos--;
        }
        if ( this->m_nShowChar > 0 && ( unsigned char ) cText[ this->m_nShowChar - 1 ] > 127 )
          this->m_nShowChar -= 2;
        else
          this->m_nShowChar--;
        if ( this->m_nShowChar < 0 )
          this->m_nShowChar = 0;
        this->m_ptCursor = PosFromChar( ww, ctr, this->m_nCharPos );
        Invalidate(ww->m_hWnd);
      }
    }
    break;
  default: {
      if ( nChar < 32 ) {     // 此处处理Ctrl+C、Ctrl+V和Ctrl+X操作
        switch ( nChar ) {
        case 3: {
            CopyString(ww, ctr);
          }
          break;
        case 22: {
            PasteString(ww, ctr);
          }
          break;
        case 24: {
            CutString(ww, ctr);
          }
        }
      } else // 此处进行一般输入字符的处理
      {
        const char *cText;
        if ( GetSelTextLength() > 0 ) { // 如果当前有被选中的字符串，先进行删除操作
          strdel(ctr->m_sText, this->m_nSelectBegin, this->m_nSelectEnd - this->m_nSelectBegin );
          // GetSelText() = "";
          this->m_nCharPos = this->m_nSelectBegin;
        }
        this->m_nSelectBegin = this->m_nSelectEnd = 0;
        this->m_nShiftBegin = -1;
        cText = strinsch( ctr->m_sText, this->m_nCharPos, nChar );
        free_s(ctr->m_sText);
        ctr->m_sText = cText;
        this->m_nCharPos++;
        if ( nChar > 127 ) {
          if ( bSpecial ) {
            while ( this->m_nCharPos - this->m_nShowChar > this->m_nMaxShowChars )
              this->m_nShowChar++;
            this->m_ptCursor = PosFromChar( ww, ctr, this->m_nCharPos );
            Invalidate(ww->m_hWnd);
            bSpecial = false;
          } else {
            bSpecial = true;
          }
        } else {
          cText = ( char * ) ctr->m_sText;
          cText += this->m_nShowChar;
          while ( this->m_nCharPos - this->m_nShowChar > this->m_nMaxShowChars ) {
            if ( ( ( unsigned char ) * cText ) > 127 && ( ( unsigned char ) * ( cText + 1 ) ) > 127 ) {
              this->m_nShowChar += 2;
              cText += 2;
            } else {
              this->m_nShowChar++;
              cText++;
            }
          }
          this->m_ptCursor = PosFromChar( ww, ctr, this->m_nCharPos );
          Invalidate(ww->m_hWnd);
        }
      }
    }
    break;
  }
  ShowCaret( ww->m_hWnd );
  TxSetCaretPos( this->m_ptCursor.x, this->m_ptCursor.y );
}
#define rcHeight(rc)  ((rc).bottom-(rc).top)
#define rcWidth(rc)   ((rc).right-(rc).left)
void OnSize( CWindowWnd* ww, CControlUI* ctr, UINT nType, int cx, int cy ) {
  UICoolEdit* this = (UICoolEdit*)ctr;
  HDC dc = ww->m_hDcPaint;
  //GetClientRect(rcWnd);
  RECT rcWnd = ctr->m_rcItem;
  HFONT pOldFont = (HFONT)SelectObject(dc, m_TxtFont );
  GetTextMetrics( dc, &this->m_tm );
  this->m_ptTxtBegin.y = this->m_ptCursor.y = ( rcHeight(rcWnd) - this->m_tm.tmHeight ) / 2;
  SelectObject(dc, pOldFont );
  this->m_nMaxShowChars = ( rcWidth(rcWnd) - nMargin ) / this->m_tm.tmAveCharWidth;
}
void OnMouseMove( CWindowWnd* ww, CControlUI* ctr, UINT nFlags, POINT point ) {
  UICoolEdit* this = (UICoolEdit*)ctr;
  if ( this->m_bLBtDown ) {
    HideCaret( ww->m_hWnd );
    if ( this->m_ptFirst.x > point.x ) {
      this->m_nSelectBegin = CharFromPos( ww, ctr, point.x );
      this->m_nSelectEnd = CharFromPos( ww, ctr, this->m_ptFirst.x );
    } else {
      this->m_nSelectBegin = CharFromPos( ww, ctr, this->m_ptFirst.x );
      this->m_nSelectEnd = CharFromPos( ww, ctr, point.x );
    }
    if ( this->m_nSelectEnd > (int)strlen(ctr->m_sText) )
      this->m_nSelectEnd = (int)strlen(ctr->m_sText);
    else if ( this->m_nSelectEnd < 0 )
      this->m_nSelectEnd = 0;
    if ( this->m_nSelectBegin > (int)strlen(ctr->m_sText) )
      this->m_nSelectBegin = (int)strlen(ctr->m_sText);
    else if ( this->m_nSelectBegin < 0 )
      this->m_nSelectBegin = 0;
    if ( this->m_nSelectEnd - this->m_nSelectBegin > 0 )
      // GetSelText() = ctr->m_sText.Mid( this->m_nSelectBegin, this->m_nSelectEnd - this->m_nSelectBegin );
      this->m_nCharPos = CharFromPos( ww, ctr, point.x );
    if ( this->m_nCharPos < 0 )
      this->m_nCharPos = 0;
    else if ( this->m_nCharPos > (int)strlen(ctr->m_sText) )
      this->m_nCharPos = (int)strlen(ctr->m_sText);
    this->m_ptCursor = PosFromChar( ww, ctr, this->m_nCharPos );
    Invalidate(ww->m_hWnd);
    ShowCaret( ww->m_hWnd );
    TxSetCaretPos( this->m_ptCursor.x, this->m_ptCursor.y );
  }
}
void OnLButtonUp( CWindowWnd* ww, CControlUI* ctr, UINT nFlags, POINT point ) {
  UICoolEdit* this = (UICoolEdit*)ctr;
  this->m_bLBtDown = false;
  ReleaseCapture();
}
#define MoveTo( m_hDC, x, y )   MoveToEx( m_hDC, x, y, 0 )

// 生成窗口背景
void MakeBKGround( CWindowWnd* ww, CControlUI* ctr, HDC hDC, COLORREF clrBegin, COLORREF clrEnd, RECT rect ) {
  UICoolEdit* this = (UICoolEdit*)ctr;
  HDC pDC = hDC, MemDC;
  HBITMAP BmpBK, pOldBmp;  // 背景图片
  int r1, r2, g1, g2, b1, b2;
  int x1 = 0, y1 = 0;
  int x2 = 0, y2 = 0;
  if ( 0 == clrBegin ) {
    return ;
  }
  if ( 0 == clrEnd ) {
    clrEnd = clrBegin;
  }
  r1 = GetRValue( clrBegin ), g1 = GetGValue( clrBegin ), b1 = GetBValue( clrBegin );
  r2 = GetRValue( clrEnd ), g2 = GetGValue( clrEnd ), b2 = GetBValue( clrEnd );
  MemDC = CreateCompatibleDC( pDC );
  BmpBK = CreateCompatibleBitmap( pDC, rcWidth(rect), rcHeight(rect) );
  pOldBmp = (HBITMAP)SelectObject( MemDC, BmpBK );
  while ( x1 < rcWidth(rect) && y1 < rcHeight(rect) ) {
    HPEN p, oldpen;
    int r, g, b, i;
    if ( y1 < rcHeight(rect) - 1 )
      y1++;
    else
      x1++;
    if ( x2 < rcWidth(rect) - 1 )
      x2++;
    else
      y2++;
    i = x1 + y1;
    r = r1 + ( i * ( r2 - r1 ) / ( rcWidth(rect) + rcHeight(rect) ) );
    g = g1 + ( i * ( g2 - g1 ) / ( rcWidth(rect) + rcHeight(rect) ) );
    b = b1 + ( i * ( b2 - b1 ) / ( rcWidth(rect) + rcHeight(rect) ) );
    p = CreatePen( PS_SOLID, 1, RGB( r, g, b ) );
    oldpen = (HPEN)SelectObject( MemDC, p );
    MoveTo( MemDC, x1, y1 );
    LineTo( MemDC, x2, y2 );
    SelectObject( MemDC, oldpen );
    DeleteObject(p);
  }
  BitBlt( pDC, rect.left, rect.top, rcWidth(rect), rcHeight(rect), MemDC, 0, 0, SRCCOPY );
  SelectObject( MemDC, pOldBmp );
  DeleteObject(BmpBK);
  DeleteDC(MemDC);
}
void FillSolidRect( HDC m_hDC, int x, int y, int cx, int cy, COLORREF clr ) {
  RECT rect = { x, y, x + cx, y + cy };
  ASSERT( m_hDC != NULL );
  SetBkColor( m_hDC, clr );
  ExtTextOut( m_hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
}
void FillSolidRect2( HDC m_hDC, LPCRECT lpRect, COLORREF clr ) {
  ASSERT( m_hDC != NULL );
  SetBkColor( m_hDC, clr );
  ExtTextOut( m_hDC, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL );
}
int UICoolEdit_DoPaint( CWindowWnd* ww, CControlUI* ctr, HDC dc, RECT rcPaint ) {
  UICoolEdit* this = (UICoolEdit*)ctr;
  HPEN PenLine, pOldPen;
  HBRUSH OldBrush;
  HRGN DrawRgn;
  HDC BKdc;
  HBITMAP BmpMem, pOldBmp;
  HFONT pOldFont;
  RECT rcWnd = ctr->m_rcItem;
  int nOldMode;
  COLORREF clrOld;
  {
    HDC dc1 = dc;
    //GetClientRect(rcWnd);
    //rcWnd = ctr->m_rcItem;
    //CClientDC dc(this);
    HFONT pOldFont = (HFONT)SelectObject( dc1, m_TxtFont );
    GetTextMetrics( dc1, &this->m_tm );
    this->m_ptTxtBegin.y = this->m_ptCursor.y = ( rcHeight(rcWnd) - this->m_tm.tmHeight ) / 2;
    SelectObject( dc1, pOldFont );
    this->m_nMaxShowChars = ( rcWidth(rcWnd) - nMargin ) / this->m_tm.tmAveCharWidth;
  }
  rcWnd = cRect( 0, 0, rcWidth(rcWnd), rcHeight(rcWnd) );
  // 拷贝窗口所在区域的背景
  BKdc = CreateCompatibleDC( dc );
  BmpMem = CreateCompatibleBitmap( dc, rcWidth(rcWnd), rcHeight(rcWnd) );
  pOldBmp = (HBITMAP)SelectObject(BKdc, BmpMem );
  BitBlt( BKdc, 0, 0, rcWidth(rcWnd), rcHeight(rcWnd), dc, ctr->m_rcItem.left, ctr->m_rcItem.top, SRCCOPY );
  // 设置绘制的区域
  PenLine=CreatePen( PS_SOLID, 1, RGB( 135, 155, 200 ) );
  pOldPen = (HPEN)SelectObject(BKdc, PenLine );
  OldBrush = (HBRUSH) SelectObject( BKdc, GetStockObject( NULL_BRUSH ) );
  BeginPath(BKdc);
  RoundRect( BKdc, rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, this->m_iRoundCorner, this->m_iRoundCorner);
  EndPath(BKdc);
  DrawRgn=PathToRegion( BKdc );
  ExtSelectClipRgn( BKdc, DrawRgn, RGN_COPY );
  CControlUI_PaintBkImage( ww->m_pResource, ctr, BKdc );
  //MakeBKGround( ww, ctr, BKdc, ctr->m_dwBackColor, ctr->m_dwBackColor2, rcWnd );
  MakeBKGround( ww, ctr, BKdc, RGB( 148, 182, 225 ), RGB( 46, 74, 137 ), rcWnd );
  ExtSelectClipRgn( BKdc, NULL, RGN_COPY );  // 恢复DC现场
  DeleteObject(DrawRgn);     // 删除区域
  RoundRect( BKdc, rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, this->m_iRoundCorner, this->m_iRoundCorner ); // 绘制窗体的边框
  SelectObject(BKdc, OldBrush );
  SelectObject(BKdc, pOldPen );    // 恢复DC现场
  DeleteObject(PenLine);
  pOldFont = (HFONT)SelectObject(BKdc, m_TxtFont );
  nOldMode = SetBkMode( BKdc, TRANSPARENT );
  clrOld = SetTextColor( BKdc, RGB( 54, 73, 165 ) );
  if ( (int)strlen(ctr->m_sText) > 0 ) {
    // 绘制窗体中的文本信息
    const char* strTemp;
    int len;
    strTemp = ctr->m_sText+( this->m_nShowChar );
    len = strlen(strTemp);
    if ( this->m_bDrawShade ) {
      TextOut( BKdc, this->m_ptTxtBegin.x + 2, this->m_ptTxtBegin.y + 2, strTemp, len );
      SetTextColor( BKdc, RGB( 42, 56, 127 ) );
      TextOut( BKdc, this->m_ptTxtBegin.x + 1, this->m_ptTxtBegin.y + 1, strTemp, len );
    }
    SetTextColor( BKdc, RGB( 0, 0, 0 ) );
    TextOut( BKdc, this->m_ptTxtBegin.x, this->m_ptTxtBegin.y, strTemp, len );
  }
  if ( this->m_nSelectEnd > this->m_nSelectBegin ) {
    // 绘制被选中字符串
    HDC MemDC;
    HBITMAP BmpMask;
    HBITMAP pOldBmp;
    BLENDFUNCTION bf;
    int nBeginPosX, nEndPosX;
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.AlphaFormat = 0;
    bf.SourceConstantAlpha = 100;
    if ( this->m_nSelectBegin < this->m_nShowChar )
      nBeginPosX = PosFromChar( ww, ctr, this->m_nShowChar ).x-ctr->m_rcItem.left;
    else
      nBeginPosX = PosFromChar( ww, ctr, this->m_nSelectBegin ).x-ctr->m_rcItem.left;
    nEndPosX = PosFromChar( ww, ctr, this->m_nSelectEnd ).x-ctr->m_rcItem.left;
    MemDC = CreateCompatibleDC( dc );
    BmpMask = CreateCompatibleBitmap( dc, nEndPosX - nBeginPosX, this->m_tm.tmHeight );
    pOldBmp = (HBITMAP)SelectObject( MemDC, BmpMask );
    FillSolidRect( MemDC, 0, 0, nEndPosX, this->m_tm.tmHeight, RGB( 0, 0, 0 ) );
    AlphaBlend( BKdc, nBeginPosX, this->m_ptTxtBegin.y, nEndPosX - nBeginPosX, this->m_tm.tmHeight,
                MemDC, 0, 0, nEndPosX - nBeginPosX, this->m_tm.tmHeight, bf );
    SelectObject( MemDC, pOldBmp );
    DeleteDC(MemDC);
    DeleteObject(BmpMask);
  }
  BitBlt( dc, ctr->m_rcItem.left, ctr->m_rcItem.top, rcWidth(rcWnd), rcHeight(rcWnd), BKdc, 0, 0, SRCCOPY );
  SetTextColor( BKdc, clrOld );
  SetBkMode( BKdc, nOldMode );
  SelectObject(BKdc, pOldFont );
  SelectObject(BKdc, pOldBmp );
  DeleteObject(BmpMem);
  DeleteDC(BKdc);
  return 0;
}
int UICoolEdit_Event( CWindowWnd* ww, CControlUI* ctr, TEventUI* event ) {
  POINT pt = event->ptMouse;
  //pt.x = event->ptMouse.x-ctr->m_rcItem.left;
  //pt.y = event->ptMouse.y;
  if ( event->Type == UIEVENT_SETFOCUS ) {
    OnSetFocus(ww, ctr);
    return 0;
  }
  if ( event->Type == UIEVENT_KILLFOCUS ) {
    OnKillFocus(ww, ctr);
    return 0;
  }
  if ( event->Type == UIEVENT_MOUSEMOVE ) {
    OnMouseMove( ww, ctr, 0, event->ptMouse );
    return 0;
  }
  if ( event->Type == UIEVENT_BUTTONUP ) {
    OnLButtonUp( ww, ctr, 0, pt );
    return 0;
  }
  if ( event->Type == UIEVENT_DBLCLICK ) {
    UICoolEdit_SetSel( ctr, 0, (int)strlen(ctr->m_sText) );
    Invalidate(ww->m_hWnd);
    return 0;
  }
  if ( event->Type == UIEVENT_BUTTONDOWN || event->Type == UIEVENT_DBLCLICK ) {
    if ( PtInRect( &ctr->m_rcItem, pt ) && IsEnabled(ctr) ) {
      OnLButtonDown( ww, ctr, 0, pt );
      Invalidate(ww->m_hWnd);
    }
    return 0;
  }
  if ( event->Type == UIEVENT_KEYDOWN ) {
    OnKeyDown( ww, ctr, event->chKey );
    return 0;
  }
  if ( event->Type == UIEVENT_CHAR ) {
    OnChar( ww, ctr, event->chKey, 0, 0 );
    return 0;
  }
  CControlUI_Event( ww, ctr, event );
  return 0;
}
int UICoolEdit_SetAttribute( UIResource* res, CControlUI* ctr, LPCTSTR pstrName, LPCTSTR pstrValue ) {
  UICoolEdit* this = (UICoolEdit*)ctr;
  if ( _tcscmp( pstrName, _T( "corner" ) ) == 0 ) {
    this->m_iRoundCorner = _tcstol( pstrValue, 0, 10 );
  } else if ( _tcscmp( pstrName, _T( "drawshade" ) ) == 0 ) {
    this->m_bDrawShade = _tcscmp( pstrValue, _T( "true" ) ) == 0;
  } else {
    CControlUI_SetAttribute( res, ctr, pstrName, pstrValue );
  }
  return 0;
}

