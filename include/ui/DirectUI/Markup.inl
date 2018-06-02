
#include <windows.h>
#include <stdio.h>

#include <tchar.h>
#include "findfile.h"
#ifndef free_s
#define free_s(_p)   ((_p!=NULL)?(free((void*)(_p)), _p=0):(0))
#endif
#define resetbuf(xml, BUF, SIZE)  xml.buf = BUF, xml.bufused=0, xml.bufmaxsize=SIZE
#define bufmalloc(size)  (assert((int)size+xml->bufused<xml->bufmaxsize && "malloc error"), xml->bufused+=size, xml->buf+xml->bufused-size)
#define bufstrdup(s, l)  copystr(bufmalloc(l+1), s, l)
char* copystr(char* d, const char* s, size_t l) {memcpy(d, s, l); d[l]=0;return d;}
#define true 1
#define false 0

enum
{ XMLFILE_ENCODING_UTF8 = 0,
XMLFILE_ENCODING_UNICODE = 1,
XMLFILE_ENCODING_ASNI = 2,
};
typedef unsigned long ULONG;
//typedef int bool;
typedef char* LPTSTR;
typedef struct tagXMLELEMENT {
  ULONG iStart;
  ULONG iChild;
  ULONG iNext;
  ULONG iParent;
  ULONG iData;
} XMLELEMENT;
//typedef int BOOL;
typedef struct CMarkup {
  XMLELEMENT* m_pElements;
  LPTSTR m_pstrXML;
  ULONG m_nElements;
  ULONG m_nReservedElements;
  TCHAR m_szErrorMsg[ 100 ];
  TCHAR m_szErrorXML[ 50 ];
  BOOL m_bPreserveWhitespace;
}CMarkup;

typedef struct XMLATTRIBUTE {
  ULONG iName;
  ULONG iValue;
} XMLATTRIBUTE;
enum { MAX_XML_ATTRIBUTES = 64 };
typedef struct CMarkupNode {
  int m_iPos;
  int m_nAttributes;
  XMLATTRIBUTE m_aAttributes[ MAX_XML_ATTRIBUTES ];
  CMarkup* m_pOwner;
}CMarkupNode;

#define TRACE printf
#define lengthof(x) (sizeof(x)/sizeof(*x))
int CMarkup_Release(CMarkup* mm) {
  free_s(mm->m_pstrXML);
  free_s(mm->m_pElements);
  return 0;
}
XMLELEMENT* _ReserveElement(CMarkup* mm) {
  if ( mm->m_nElements == 0 )
    mm->m_nReservedElements = 0;
  if ( mm->m_nElements >= mm->m_nReservedElements ) {
    mm->m_nReservedElements += ( mm->m_nReservedElements / 2 ) + 500;
    mm->m_pElements = (XMLELEMENT*)( realloc( mm->m_pElements, mm->m_nReservedElements * sizeof( XMLELEMENT ) ) );
  }
  return &mm->m_pElements[ mm->m_nElements++ ];
}
#define _SkipWhitespace(pstr)   {while ( *pstr > _T( '\0' ) && *pstr <= _T( ' ' ) )  pstr = CharNext( pstr );}
// 属性只能用英文，所以这样处理没有问题
#define _SkipIdentifier( pstr ) { while( *pstr != _T( '\0' ) && ( *pstr == _T( '_' ) || *pstr == _T( ':' ) || _istalnum( *pstr ) ) ) pstr = CharNext( pstr );}
#define CMarkup_ParseMetaChar(pstrText, pstrDest)  CMarkup_ParseMetaChar_(&pstrText, &pstrDest)
void CMarkup_ParseMetaChar_( LPTSTR* ppstrText, LPTSTR* ppstrDest ) {
  LPTSTR pstrText=*ppstrText, pstrDest=*ppstrDest;
  if ( pstrText[ 0 ] == _T( 'a' ) && pstrText[ 1 ] == _T( 'm' ) && pstrText[ 2 ] == _T( 'p' ) && pstrText[ 3 ] == _T( ';' ) ) {
    *pstrDest++ = _T( '&' );
    pstrText += 4;
  } else if ( pstrText[ 0 ] == _T( 'l' ) && pstrText[ 1 ] == _T( 't' ) && pstrText[ 2 ] == _T( ';' ) ) {
    *pstrDest++ = _T( '<' );
    pstrText += 3;
  } else if ( pstrText[ 0 ] == _T( 'g' ) && pstrText[ 1 ] == _T( 't' ) && pstrText[ 2 ] == _T( ';' ) ) {
    *pstrDest++ = _T( '>' );
    pstrText += 3;
  } else if ( pstrText[ 0 ] == _T( 'q' ) && pstrText[ 1 ] == _T( 'u' ) && pstrText[ 2 ] == _T( 'o' ) && pstrText[ 3 ] == _T( 't' ) && pstrText[ 4 ] == _T( ';' ) ) {
    *pstrDest++ = _T( '\"' );
    pstrText += 5;
  } else if ( pstrText[ 0 ] == _T( 'a' ) && pstrText[ 1 ] == _T( 'p' ) && pstrText[ 2 ] == _T( 'o' ) && pstrText[ 3 ] == _T( 's' ) && pstrText[ 4 ] == _T( ';' ) ) {
    *pstrDest++ = _T( '\'' );
    pstrText += 5;
  } else {
    *pstrDest++ = _T( '&' );
  }
  *ppstrText = pstrText, *ppstrDest = pstrDest;
}
#define CMarkup_ParseData(mm, pstrText, pstrDest, cEnd)  CMarkup_ParseData_( mm, &pstrText, &pstrDest, cEnd )
BOOL CMarkup_ParseData_( CMarkup* mm, LPTSTR* ppstrText, LPTSTR* ppstrDest, char cEnd ) {
  LPTSTR pstrFill, pstrText=*ppstrText, pstrDest=*ppstrDest;
  while ( *pstrText != _T( '\0' ) && *pstrText != cEnd ) {
    while ( *pstrText == _T( '&' ) ) {
      ++pstrText;
      CMarkup_ParseMetaChar( pstrText, pstrDest );
    }
    if ( *pstrText == _T( ' ' ) ) {
      *pstrDest++ = *pstrText++;
      if ( !mm->m_bPreserveWhitespace )
        _SkipWhitespace( pstrText );
    } else {
      LPTSTR pstrTemp = CharNext( pstrText );
      while ( pstrText < pstrTemp ) {
        *pstrDest++ = *pstrText++;
      }
    }
  }
  // Make sure that MapAttributes() works correctly when it parses
  // over a value that has been transformed.
  pstrFill = pstrDest + 1;
  while ( pstrFill < pstrText )
    * pstrFill++ = _T( ' ' );
  *ppstrText = pstrText, *ppstrDest = pstrDest;
  return true;
}
BOOL _Failed( CMarkup* mm, LPCTSTR pstrError, LPCTSTR pstrLocation) {
  // Register last error
  TRACE( _T( "XML Error: %s" ), pstrError );
  TRACE( pstrLocation );
  _tcsncpy( mm->m_szErrorMsg, pstrError, ( sizeof( mm->m_szErrorMsg ) / sizeof( mm->m_szErrorMsg[ 0 ] ) ) - 1 );
  _tcsncpy( mm->m_szErrorXML, pstrLocation != NULL ? pstrLocation : _T( "" ), lengthof( mm->m_szErrorXML ) - 1 );
  return false; // Always return 'false'
}
#define CMarkup_ParseAttributes(mm, pstrText) CMarkup_ParseAttributes_(mm, &pstrText)
BOOL CMarkup_ParseAttributes_( CMarkup* mm, LPTSTR* ppstrText ) {
#define pstrText (*ppstrText)
  LPTSTR pstrIdentifierEnd, pstrDest;
  if ( *pstrText == _T( '>' ) )
    return true;
  if ( pstrText[0] == _T( '/' ) && pstrText[1] == _T( '>' ) )
    return true;
  *pstrText++ = _T( '\0' );
  _SkipWhitespace( pstrText );
  while ( *pstrText != _T( '\0' ) && *pstrText != _T( '>' ) && *pstrText != _T( '/' ) ) {
    _SkipIdentifier( pstrText );
    pstrIdentifierEnd = pstrText;
    _SkipWhitespace( pstrText );
    if ( *pstrText != _T( '=' ) )
      return _Failed( mm, _T( "Error while parsing attributes" ), pstrText );
    *pstrText++ = _T( ' ' );
    *pstrIdentifierEnd = _T( '\0' );
    _SkipWhitespace( pstrText );
    if ( *pstrText++ != _T( '\"' ) )
      return _Failed( mm, _T( "Expected attribute value" ), pstrText );
    pstrDest = pstrText;
    if ( !CMarkup_ParseData( mm, pstrText, pstrDest, _T( '\"' ) ) )
      return false;
    if ( *pstrText == _T( '\0' ) )
      return _Failed( mm, _T( "Error while parsing attribute string" ), pstrText );
    *pstrDest = _T( '\0' );
    *pstrText++ = _T( '\0' );
    _SkipWhitespace( pstrText );
  }
  return true;
}
#define CMarkup_ParseImpl(mm, pstrText, iParent) CMarkup_ParseImpl_(mm, &pstrText, iParent)
BOOL CMarkup_ParseImpl_( CMarkup* mm, LPTSTR* ppstrText, ULONG iParent ) {
  LPCTSTR pstrName;
  LPTSTR pstrNameEnd;
  LPTSTR pstrDest;
  SIZE_T cchName;
  ULONG iPrevious = 0;
  XMLELEMENT* pEl;
  ULONG iPos;
  _SkipWhitespace( pstrText );
  for ( ; ; ) {
    if ( *pstrText == _T( '\0' ) && iParent <= 1 )
      return true;
    _SkipWhitespace( pstrText );
    if ( *pstrText != _T( '<' ) )
      return _Failed( mm, _T( "Expected start tag" ), pstrText );
    if ( pstrText[ 1 ] == _T( '/' ) )
      return true;
    *pstrText++ = _T( '\0' );
    _SkipWhitespace( pstrText );
    // Skip comment or processing directive
    if ( *pstrText == _T( '!' ) || *pstrText == _T( '?' ) ) {
      TCHAR ch = *pstrText;
      if ( *pstrText == _T( '!' ) )
        ch = _T( '-' );
      while ( *pstrText != _T( '\0' ) && !( *pstrText == ch && *( pstrText + 1 ) == _T( '>' ) ) )
        pstrText = CharNext( pstrText );
      if ( *pstrText != _T( '\0' ) )
        pstrText += 2;
      _SkipWhitespace( pstrText );
      continue;
    }
    _SkipWhitespace( pstrText );
    // Fill out element structure
    pEl = _ReserveElement(mm);
    iPos = pEl - mm->m_pElements;
    pEl->iStart = pstrText - mm->m_pstrXML;
    pEl->iParent = iParent;
    pEl->iNext = pEl->iChild = 0;
    if ( iPrevious != 0 )
      mm->m_pElements[ iPrevious ].iNext = iPos;
    else if ( iParent > 0 )
      mm->m_pElements[ iParent ].iChild = iPos;
    iPrevious = iPos;
    // Parse name
    pstrName = pstrText;
    _SkipIdentifier( pstrText );
    pstrNameEnd = pstrText;
    if ( *pstrText == _T( '\0' ) )
      return _Failed( mm, _T( "Error parsing element name" ), pstrText );
    // Parse attributes
    if ( !CMarkup_ParseAttributes( mm, pstrText ) )
      return false;
    _SkipWhitespace( pstrText );
    if ( pstrText[ 0 ] == _T( '/' ) && pstrText[ 1 ] == _T( '>' ) ) {
      pEl->iData = pstrText - mm->m_pstrXML;
      *pstrText = _T( '\0' );
      pstrText += 2;
    } else {
      if ( *pstrText != _T( '>' ) )
        return _Failed( mm, _T( "Expected start-tag closing" ), pstrText );
      // Parse node data
      pEl->iData = ++pstrText - mm->m_pstrXML;
      pstrDest = pstrText;
      if ( !CMarkup_ParseData( mm, pstrText, pstrDest, _T( '<' ) ) )
        return false;
      // Determine type of next element
      if ( *pstrText == _T( '\0' ) && iParent <= 1 )
        return true;
      if ( *pstrText != _T( '<' ) )
        return _Failed( mm, _T( "Expected end-tag start" ), pstrText );
      if ( pstrText[ 0 ] == _T( '<' ) && pstrText[ 1 ] != _T( '/' ) ) {
        if ( !CMarkup_ParseImpl( mm, pstrText, iPos ) )
          return false;
      }
      if ( pstrText[ 0 ] == _T( '<' ) && pstrText[ 1 ] == _T( '/' ) ) {
        *pstrDest = _T( '\0' );
        *pstrText = _T( '\0' );
        pstrText += 2;
        _SkipWhitespace( pstrText );
        cchName = pstrNameEnd - pstrName;
        if ( _tcsncmp( pstrText, pstrName, cchName ) != 0 )
          return _Failed( mm, _T( "Unmatched closing tag" ), pstrText );
        pstrText += cchName;
        _SkipWhitespace( pstrText );
        if ( *pstrText++ != _T( '>' ) )
          return _Failed( mm, _T( "Unmatched closing tag" ), pstrText );
      }
    }
    *pstrNameEnd = _T( '\0' );
    _SkipWhitespace( pstrText );
  }
#undef pstrText
}

BOOL CMarkup_Parse(CMarkup* mm) {
  LPTSTR pstrXML;
  _ReserveElement(mm); // Reserve index 0 for errors
  ZeroMemory( mm->m_szErrorMsg, sizeof( mm->m_szErrorMsg ) );
  ZeroMemory( mm->m_szErrorXML, sizeof( mm->m_szErrorXML ) );
  pstrXML = mm->m_pstrXML;
  return CMarkup_ParseImpl( mm, pstrXML, 0 );
}

void Release(CMarkup* mm) {
  if ( mm->m_pstrXML != NULL )
    free( mm->m_pstrXML );
  if ( mm->m_pElements != NULL )
    free( mm->m_pElements );
  mm->m_pstrXML = NULL;
  mm->m_pElements = NULL;
  mm->m_nElements;
}
BOOL IsValid(CMarkup* mm) {
  return mm->m_pElements != NULL;
}
void SetPreserveWhitespace( CMarkup* mm, BOOL bPreserve ) {
  mm->m_bPreserveWhitespace = bPreserve;
}
void GetLastErrorMessage( CMarkup* mm, LPTSTR pstrMessage, SIZE_T cchMax ) {
  _tcsncpy( pstrMessage, mm->m_szErrorMsg, cchMax );
}
void GetLastErrorLocation( CMarkup* mm, LPTSTR pstrSource, SIZE_T cchMax ) {
  _tcsncpy( pstrSource, mm->m_szErrorXML, cchMax );
}

CMarkupNode cMarkupNode( CMarkup* pOwner, int iPos ) {
  CMarkupNode mn;
  memset(&mn, 0, sizeof(mn));
  mn.m_pOwner = ( pOwner ), mn.m_iPos = ( iPos ), mn.m_nAttributes = ( 0 ) ;
  return mn;
}
CMarkupNode* SetMarkupNode(CMarkupNode* pNode, CMarkup* pOwner, int iPos ) {
  memset(pNode, 0, sizeof(CMarkupNode));
  pNode->m_pOwner = ( pOwner ), pNode->m_iPos = ( iPos ), pNode->m_nAttributes = ( 0 ) ;
  return pNode;
}
BOOL Load( CMarkup* mm, LPCTSTR pstrXML ) {
  SIZE_T cchLen;
  BOOL bRes;
  Release(mm);
  cchLen = _tcslen( pstrXML ) + 1;
  mm->m_pstrXML = (LPTSTR)( malloc( cchLen * sizeof( TCHAR ) ) );
  CopyMemory( mm->m_pstrXML, pstrXML, cchLen * sizeof( TCHAR ) );
  bRes = CMarkup_Parse(mm);
  if ( !bRes )
    Release(mm);
  return bRes;
}
BOOL LoadFromMem( CMarkup* mm, BYTE* pByte, DWORD dwSize, int encoding ) {
  BOOL bRes;
#ifdef _UNICODE
  if ( encoding == XMLFILE_ENCODING_UTF8 ) {
    DWORD nWide = MultiByteToWideChar( CP_UTF8, 0, ( LPCSTR ) pByte, dwSize, NULL, 0 );
    mm->m_pstrXML = (LPTSTR)( malloc( ( nWide + 1 ) * sizeof( TCHAR ) ) );
    MultiByteToWideChar( CP_UTF8, 0, ( LPCSTR ) pByte, dwSize, mm->m_pstrXML, nWide );
    mm->m_pstrXML[ nWide ] = _T( '\0' );
  } else if ( encoding == XMLFILE_ENCODING_ASNI ) {
    DWORD nWide = MultiByteToWideChar( CP_ACP, 0, ( LPCSTR ) pByte, dwSize, NULL, 0 );
    mm->m_pstrXML = (LPTSTR)( malloc( ( nWide + 1 ) * sizeof( TCHAR ) ) );
    MultiByteToWideChar( CP_ACP, 0, ( LPCSTR ) pByte, dwSize, mm->m_pstrXML, nWide );
    mm->m_pstrXML[ nWide ] = _T( '\0' );
  } else {
    if ( dwSize >= 2 && ( ( pByte[ 0 ] == 0xFE && pByte[ 1 ] == 0xFF ) || ( pByte[ 0 ] == 0xFF && pByte[ 1 ] == 0xFE ) ) ) {
      dwSize = dwSize / 2 - 1;
      if ( pByte[ 0 ] == 0xFE && pByte[ 1 ] == 0xFF ) {
        pByte += 2;
        for ( DWORD nSwap = 0 ; nSwap < dwSize ; nSwap ++ ) {
          register CHAR nTemp = pByte[ ( nSwap << 1 ) + 0 ];
          pByte[ ( nSwap << 1 ) + 0 ] = pByte[ ( nSwap << 1 ) + 1 ];
          pByte[ ( nSwap << 1 ) + 1 ] = nTemp;
        }
      } else {
        pByte += 2;
      }
      mm->m_pstrXML = (LPTSTR)( malloc( ( dwSize + 1 ) * sizeof( TCHAR ) ) );
      CopyMemory( mm->m_pstrXML, pByte, dwSize * sizeof( TCHAR ) );
      mm->m_pstrXML[ dwSize ] = _T( '\0' );
      pByte -= 2;
    }
  }
#else // !_UNICODE
  if ( encoding == XMLFILE_ENCODING_UTF8 ) {
    DWORD wide, nWide;
    LPWSTR w_str;
    nWide = MultiByteToWideChar( CP_UTF8, 0, ( LPCSTR ) pByte, dwSize, NULL, 0 );
    w_str = (LPWSTR)( malloc( ( nWide + 1 ) * sizeof( WCHAR ) ) );
    MultiByteToWideChar( CP_UTF8, 0, ( LPCSTR ) pByte, dwSize, w_str, nWide );
    w_str[ nWide ] = L'\0';
    wide = WideCharToMultiByte( CP_ACP, 0, ( LPCWSTR ) w_str, nWide, NULL, 0, NULL, NULL );
    mm->m_pstrXML = (LPTSTR)( malloc( ( wide + 1 ) * sizeof( TCHAR ) ) );
    WideCharToMultiByte( CP_ACP, 0, ( LPCWSTR ) w_str, nWide, mm->m_pstrXML, wide, NULL, NULL );
    mm->m_pstrXML[ wide ] = _T( '\0' );
    free( w_str );
  } else if ( encoding == XMLFILE_ENCODING_UNICODE ) {
    if ( dwSize >= 2 && ( ( pByte[ 0 ] == 0xFE && pByte[ 1 ] == 0xFF ) || ( pByte[ 0 ] == 0xFF && pByte[ 1 ] == 0xFE ) ) ) {
      DWORD nWide;
      dwSize = dwSize / 2 - 1;
      if ( pByte[ 0 ] == 0xFE && pByte[ 1 ] == 0xFF ) {
        DWORD nSwap;
        pByte += 2;
        for ( nSwap = 0 ; nSwap < dwSize ; nSwap ++ ) {
          register CHAR nTemp = pByte[ ( nSwap << 1 ) + 0 ];
          pByte[ ( nSwap << 1 ) + 0 ] = pByte[ ( nSwap << 1 ) + 1 ];
          pByte[ ( nSwap << 1 ) + 1 ] = nTemp;
        }
      } else {
        pByte += 2;
      }
      nWide = WideCharToMultiByte( CP_ACP, 0, ( LPCWSTR ) pByte, dwSize, NULL, 0, NULL, NULL );
      mm->m_pstrXML = (LPTSTR)( malloc( ( nWide + 1 ) * sizeof( TCHAR ) ) );
      WideCharToMultiByte( CP_ACP, 0, ( LPCWSTR ) pByte, dwSize, mm->m_pstrXML, nWide, NULL, NULL );
      mm->m_pstrXML[ nWide ] = _T( '\0' );
      pByte -= 2;
    }
  } else {
    mm->m_pstrXML = (LPTSTR)( malloc( ( dwSize + 1 ) * sizeof( TCHAR ) ) );
    CopyMemory( mm->m_pstrXML, pByte, dwSize * sizeof( TCHAR ) );
    mm->m_pstrXML[ dwSize ] = _T( '\0' );
  }
#endif // _UNICODE
  bRes = CMarkup_Parse(mm);
  if ( !bRes )
    Release(mm);
  return bRes;
}
BOOL LoadFromFile( CMarkup* mm, LPCTSTR pstrFilename, int encoding ) {
  HANDLE hFile;
  DWORD dwSize;
  DWORD dwRead;
  BYTE* pByte;
  BOOL ret;
  Release(mm);
  hFile = CreateFile( pstrFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
  if ( hFile == INVALID_HANDLE_VALUE )
    return _Failed( mm, _T( "Error opening file" ), NULL );
  dwSize = GetFileSize( hFile, NULL );
  if ( dwSize == 0 )
    return _Failed( mm, _T( "File is empty" ), NULL );
  if ( dwSize > 4096 * 1024 )
    return _Failed( mm, _T( "File too large" ), NULL );
  dwRead = 0;
  pByte = (BYTE*)malloc(dwSize);
  ReadFile( hFile, pByte, dwSize, &dwRead, NULL );
  CloseHandle( hFile );
  if ( dwRead != dwSize ) {
    free(pByte);
    Release(mm);
    return _Failed( mm, _T( "Could not read file" ), NULL );
  }
  ret = LoadFromMem( mm, pByte, dwSize, encoding );
  free(pByte);
  return ret;
}

#define GetRoot(mm)       cMarkupNode( mm, 1 )
#define cMarkupNodeNULL() cMarkupNode( 0, 0 )

#include "MarkupNode.inl"
