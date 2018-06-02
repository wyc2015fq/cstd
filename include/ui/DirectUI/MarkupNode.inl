

CMarkupNode GetSiblingNode(CMarkupNode* mn) {
  ULONG iPos;
  if ( mn->m_pOwner == NULL )
    return cMarkupNodeNULL();
  iPos = mn->m_pOwner->m_pElements[ mn->m_iPos ].iNext;
  if ( iPos == 0 )
    return cMarkupNodeNULL();
  return cMarkupNode( mn->m_pOwner, iPos );
}
BOOL HasSiblings(CMarkupNode* mn) {
  ULONG iPos;
  if ( mn->m_pOwner == NULL )
    return false;
  iPos = mn->m_pOwner->m_pElements[ mn->m_iPos ].iNext;
  return iPos > 0;
}
CMarkupNode* GetChildNode2( CMarkupNode* mn, LPCTSTR pstrName, CMarkupNode* out ) {
  ULONG iPos;
  if ( mn->m_pOwner == NULL )
    return NULL;
  if (NULL==pstrName) {
    iPos = mn->m_pOwner->m_pElements[ mn->m_iPos ].iChild;
    if ( iPos == 0 )
      return NULL;
    return SetMarkupNode( out, mn->m_pOwner, iPos );
  }
  iPos = mn->m_pOwner->m_pElements[ mn->m_iPos ].iChild;
  while ( iPos != 0 ) {
    if ( _tcscmp( mn->m_pOwner->m_pstrXML + mn->m_pOwner->m_pElements[ iPos ].iStart, pstrName ) == 0 ) {
      return SetMarkupNode( out, mn->m_pOwner, iPos );
    }
    iPos = mn->m_pOwner->m_pElements[ iPos ].iNext;
  }
  return NULL;
}
CMarkupNode GetChildNode( CMarkupNode* mn, LPCTSTR pstrName ) {
  ULONG iPos;
  if ( mn->m_pOwner == NULL )
    return cMarkupNodeNULL();
  if (NULL==pstrName) {
    iPos = mn->m_pOwner->m_pElements[ mn->m_iPos ].iChild;
    if ( iPos == 0 )
      return cMarkupNodeNULL();
    return cMarkupNode( mn->m_pOwner, iPos );
  }
  iPos = mn->m_pOwner->m_pElements[ mn->m_iPos ].iChild;
  while ( iPos != 0 ) {
    if ( _tcscmp( mn->m_pOwner->m_pstrXML + mn->m_pOwner->m_pElements[ iPos ].iStart, pstrName ) == 0 ) {
      return cMarkupNode( mn->m_pOwner, iPos );
    }
    iPos = mn->m_pOwner->m_pElements[ iPos ].iNext;
  }
  return cMarkupNodeNULL();
}
BOOL HasChildren(CMarkupNode* mn) {
  if ( mn->m_pOwner == NULL )
    return false;
  return mn->m_pOwner->m_pElements[ mn->m_iPos ].iChild != 0;
}
CMarkupNode GetParentNode(CMarkupNode* mn) {
  ULONG iPos;
  if ( mn->m_pOwner == NULL )
    return cMarkupNodeNULL();
  iPos = mn->m_pOwner->m_pElements[ mn->m_iPos ].iParent;
  if ( iPos == 0 )
    return cMarkupNodeNULL();
  return cMarkupNode( mn->m_pOwner, iPos );
}
BOOL IsValidNode(CMarkupNode* mn) {
  return mn->m_pOwner != NULL;
}
LPCTSTR GetName(CMarkupNode* mn) {
  if ( mn->m_pOwner == NULL )
    return NULL;
  return mn->m_pOwner->m_pstrXML + mn->m_pOwner->m_pElements[ mn->m_iPos ].iStart;
}
LPCTSTR GetValue(CMarkupNode* mn) {
  if ( mn->m_pOwner == NULL )
    return NULL;
  return mn->m_pOwner->m_pstrXML + mn->m_pOwner->m_pElements[ mn->m_iPos ].iData;
}
void _MapAttributes(CMarkupNode* mn) {
  LPCTSTR pstr;
  LPCTSTR pstrEnd;
  mn->m_nAttributes = 0;
  pstr = mn->m_pOwner->m_pstrXML + mn->m_pOwner->m_pElements[ mn->m_iPos ].iStart;
  pstrEnd = mn->m_pOwner->m_pstrXML + mn->m_pOwner->m_pElements[ mn->m_iPos ].iData;
  pstr += _tcslen( pstr ) + 1;
  while ( pstr < pstrEnd ) {
    _SkipWhitespace( pstr );
    mn->m_aAttributes[ mn->m_nAttributes ].iName = pstr - mn->m_pOwner->m_pstrXML;
    pstr += _tcslen( pstr ) + 1;
    _SkipWhitespace( pstr );
    if ( *pstr++ != _T( '\"' ) )
      return ; // if( *pstr != _T('\"') ) { pstr = ::CharNext(pstr); return; }
    mn->m_aAttributes[ mn->m_nAttributes++ ].iValue = pstr - mn->m_pOwner->m_pstrXML;
    if ( mn->m_nAttributes >= MAX_XML_ATTRIBUTES )
      return ;
    pstr += _tcslen( pstr ) + 1;
  }
}
LPCTSTR GetAttributeName( CMarkupNode* mn, int iIndex ) {
  if ( mn->m_pOwner == NULL )
    return NULL;
  if ( mn->m_nAttributes == 0 )
    _MapAttributes(mn);
  if ( iIndex < 0 || iIndex >= mn->m_nAttributes )
    return _T( "" );
  return mn->m_pOwner->m_pstrXML + mn->m_aAttributes[ iIndex ].iName;
}
LPCTSTR GetAttributeValueByIndex( CMarkupNode* mn, int iIndex ) {
  if ( mn->m_pOwner == NULL )
    return NULL;
  if ( mn->m_nAttributes == 0 )
    _MapAttributes(mn);
  if ( iIndex < 0 || iIndex >= mn->m_nAttributes )
    return _T( "" );
  return mn->m_pOwner->m_pstrXML + mn->m_aAttributes[ iIndex ].iValue;
}
LPCTSTR GetAttributeValueByName( CMarkupNode* mn, LPCTSTR pstrName ) {
  int i;
  if ( mn->m_pOwner == NULL )
    return NULL;
  if ( mn->m_nAttributes == 0 )
    _MapAttributes(mn);
  for ( i = 0; i < mn->m_nAttributes; i++ ) {
    if ( _tcscmp( mn->m_pOwner->m_pstrXML + mn->m_aAttributes[ i ].iName, pstrName ) == 0 )
      return mn->m_pOwner->m_pstrXML + mn->m_aAttributes[ i ].iValue;
  }
  return NULL;
}
BOOL GetAttributeValueByValue( CMarkupNode* mn, int iIndex, LPTSTR pstrValue, SIZE_T cchMax ) {
  if ( mn->m_pOwner == NULL )
    return false;
  if ( mn->m_nAttributes == 0 )
    _MapAttributes(mn);
  if ( iIndex < 0 || iIndex >= mn->m_nAttributes )
    return false;
  _tcsncpy( pstrValue, mn->m_pOwner->m_pstrXML + mn->m_aAttributes[ iIndex ].iValue, cchMax );
  return true;
}
BOOL GetAttributeValueByValue2( CMarkupNode* mn, LPCTSTR pstrName, LPTSTR pstrValue, SIZE_T cchMax ) {
  int i;
  if ( mn->m_pOwner == NULL )
    return false;
  if ( mn->m_nAttributes == 0 )
    _MapAttributes(mn);
  for ( i = 0; i < mn->m_nAttributes; i++ ) {
    if ( _tcscmp( mn->m_pOwner->m_pstrXML + mn->m_aAttributes[ i ].iName, pstrName ) == 0 ) {
      _tcsncpy( pstrValue, mn->m_pOwner->m_pstrXML + mn->m_aAttributes[ i ].iValue, cchMax );
      return true;
    }
  }
  return false;
}
int GetAttributeCount(CMarkupNode* mn) {
  if ( mn->m_pOwner == NULL )
    return 0;
  if ( mn->m_nAttributes == 0 )
    _MapAttributes(mn);
  return mn->m_nAttributes;
}
BOOL HasAttributes(CMarkupNode* mn) {
  if ( mn->m_pOwner == NULL )
    return false;
  if ( mn->m_nAttributes == 0 )
    _MapAttributes(mn);
  return mn->m_nAttributes > 0;
}
BOOL HasAttribute( CMarkupNode* mn, LPCTSTR pstrName ) {
  int i;
  if ( mn->m_pOwner == NULL )
    return false;
  if ( mn->m_nAttributes == 0 )
    _MapAttributes(mn);
  for ( i = 0; i < mn->m_nAttributes; i++ ) {
    if ( _tcscmp( mn->m_pOwner->m_pstrXML + mn->m_aAttributes[ i ].iName, pstrName ) == 0 )
      return true;
  }
  return false;
}
