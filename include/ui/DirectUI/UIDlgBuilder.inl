
#include "Markup.inl"

CControlUI* _Parse( CMarkupNode* pRoot, CControlUI* pParent, UIResource* res ) {
  CControlUI * pDialogLayout = NULL;
  CControlUI* pReturn = NULL;
  CMarkupNode node = GetChildNode(pRoot, 0);
  for ( ; IsValidNode(&node); node = GetSiblingNode(&node) ) {
    LPCTSTR pstrClass = GetName(&node);
    SIZE_T cchLen;
    CControlUI* pControl = NULL;
    if ( _tcscmp( pstrClass, _T( "Image" ) ) == 0 || _tcscmp( pstrClass, _T( "Font" ) ) == 0 \
      || _tcscmp( pstrClass, _T( "Default" ) ) == 0 )
      continue;
    cchLen = _tcslen( pstrClass );
    pControl = CreateControl(pstrClass);
    
    ASSERT( pControl );
    if ( pControl == NULL )
      continue;
    // Add children
    if ( HasChildren(&node) ) {
      _Parse( &node, pControl, res );
    }
    // Attach to parent
    // 因为某些属性和父窗口相关，比如selected，必须先Add到父窗口
    if ( pParent != NULL ) {
      CControlUI_AddLast( pParent, pControl );
    }
    // Init default attributes
    if ( res ) {
      //pControl->SetManager( res, NULL, false );
      LPCTSTR pDefaultAttributes = UIResource_GetDefaultAttributeList( res, pstrClass );
      if ( pDefaultAttributes ) {
        CControlUI_ApplyAttributeList( res, pControl, pDefaultAttributes );
      }
    }
    // Process attributes
    if ( HasAttributes(&node) ) {
      TCHAR szValue[ 500 ] = { 0 };
      SIZE_T cchLen = lengthof( szValue ) - 1;
      // Set ordinary attributes
      int i, nAttributes = GetAttributeCount(&node);
      for ( i = 0; i < nAttributes; i++ ) {
        SetAttribute( res, pControl, GetAttributeName(&node, i ), GetAttributeValueByIndex( &node, i ) );
      }
      // Very custom attributes
      if ( GetAttributeValueByValue2( &node, _T( "stretch" ), szValue, cchLen ) ) {
        UINT uMode = 0;
        if ( pParent == NULL )
          continue;
        if ( pDialogLayout == NULL )
          pDialogLayout = pParent;
        ASSERT( pDialogLayout );
        if ( pDialogLayout == NULL )
          continue;
        if ( _tcsstr( szValue, _T( "move_x" ) ) != NULL )
          uMode |= UISTRETCH_MOVE_X;
        if ( _tcsstr( szValue, _T( "move_y" ) ) != NULL )
          uMode |= UISTRETCH_MOVE_Y;
        if ( _tcsstr( szValue, _T( "move_xy" ) ) != NULL )
          uMode |= UISTRETCH_MOVE_X | UISTRETCH_MOVE_Y;
        if ( _tcsstr( szValue, _T( "size_x" ) ) != NULL )
          uMode |= UISTRETCH_SIZE_X;
        if ( _tcsstr( szValue, _T( "size_y" ) ) != NULL )
          uMode |= UISTRETCH_SIZE_Y;
        if ( _tcsstr( szValue, _T( "size_xy" ) ) != NULL )
          uMode |= UISTRETCH_SIZE_X | UISTRETCH_SIZE_Y;
        if ( _tcsstr( szValue, _T( "group" ) ) != NULL )
          uMode |= UISTRETCH_NEWGROUP;
        if ( _tcsstr( szValue, _T( "line" ) ) != NULL )
          uMode |= UISTRETCH_NEWLINE;
        //pDialogLayout->SetStretchMode( pControl, uMode );
      }
    }
    if ( res ) {
     // pControl->SetManager( NULL, NULL, false );
    }
    // Return first item
    if ( pReturn == NULL )
      pReturn = pControl;
  }
  return pReturn;
}

CControlUI* CreateControlByXml( STRINGorID xml, LPCTSTR type, UIResource* res ) {
  CMarkup m_xml = {0};
  CMarkupNode root;
  CControlUI* ctr;
  if ( HIWORD( xml.s ) != 0 ) {
    if ( *( xml.s ) == _T( '<' ) ) {
      if ( !Load( &m_xml, xml.s ) )
        return NULL;
    } else {
      if ( !LoadFromFile( &m_xml, xml.s, XMLFILE_ENCODING_UTF8 ) )
        return NULL;
    }
  } else {
    HGLOBAL hGlobal;
    HRSRC hResource = FindResource( GetResourceInstance(), xml.s, type );
    if ( hResource == NULL )
      return NULL;
    hGlobal = LoadResource( GetResourceInstance(), hResource );
    if ( hGlobal == NULL ) {
      FreeResource( hResource );
      return NULL;
    }
    if ( !LoadFromMem( &m_xml, ( BYTE* ) LockResource( hGlobal ), SizeofResource( GetResourceInstance(), hResource ), XMLFILE_ENCODING_UTF8 ) )
      return NULL;
    FreeResource( hResource );
  }
  root = GetRoot(&m_xml);
  if ( res ) {
    LPCTSTR pstrClass = GetName(&root);
    if ( _tcscmp( pstrClass, _T( "Window" ) ) == 0 ) {
      CMarkupNode node;
      int nAttributes = 0;
      LPCTSTR pstrName = NULL;
      LPCTSTR pstrValue = NULL;
      LPTSTR pstr = NULL;
      int i;
      node = GetChildNode(&root, 0);
      for ( ; IsValidNode(&node); node = GetSiblingNode(&node) ) {
        pstrClass = GetName(&node);
        if ( _tcscmp( pstrClass, _T( "Image" ) ) == 0 ) {
          LPCTSTR pImageName = NULL;
          LPCTSTR pImageResType = NULL;
          DWORD mask = 0;
          nAttributes = GetAttributeCount(&node);
          for ( i = 0; i < nAttributes; i++ ) {
            pstrName = GetAttributeName( &node, i );
            pstrValue = GetAttributeValueByIndex( &node, i );
            if ( _tcscmp( pstrName, _T( "name" ) ) == 0 ) {
              pImageName = pstrValue;
            } else if ( _tcscmp( pstrName, _T( "restype" ) ) == 0 ) {
              pImageResType = pstrValue;
            } else if ( _tcscmp( pstrName, _T( "mask" ) ) == 0 ) {
              if ( *pstrValue == _T( '#' ) )
                pstrValue = CharNext( pstrValue );
              mask = _tcstoul( pstrValue, &pstr, 16 );
            }
          }
          if ( pImageName )
            UIResource_AddImage( res, pImageName, pImageResType, mask );
        } else if ( _tcscmp( pstrClass, _T( "Font" ) ) == 0 ) {
          LPCTSTR pFontName = NULL;
          int size = 12;
          bool bold = false;
          bool underline = false;
          bool italic = false;
          bool defaultfont = false;
          bool defaultboldfont = false;
          bool defaultlinkfont = false;
          nAttributes = GetAttributeCount(&node);
          for ( i = 0; i < nAttributes; i++ ) {
            pstrName = GetAttributeName(&node, i );
            pstrValue = GetAttributeValueByIndex( &node, i );
            if ( _tcscmp( pstrName, _T( "name" ) ) == 0 ) {
              pFontName = pstrValue;
            } else if ( _tcscmp( pstrName, _T( "size" ) ) == 0 ) {
              size = _tcstol( pstrValue, &pstr, 10 );
            } else if ( _tcscmp( pstrName, _T( "bold" ) ) == 0 ) {
              bold = ( _tcscmp( pstrValue, _T( "true" ) ) == 0 );
            } else if ( _tcscmp( pstrName, _T( "underline" ) ) == 0 ) {
              underline = ( _tcscmp( pstrValue, _T( "true" ) ) == 0 );
            } else if ( _tcscmp( pstrName, _T( "italic" ) ) == 0 ) {
              italic = ( _tcscmp( pstrValue, _T( "true" ) ) == 0 );
            } else if ( _tcscmp( pstrName, _T( "default" ) ) == 0 ) {
              defaultfont = ( _tcscmp( pstrValue, _T( "true" ) ) == 0 );
            } else if ( _tcscmp( pstrName, _T( "defaultbold" ) ) == 0 ) {
              defaultboldfont = ( _tcscmp( pstrValue, _T( "true" ) ) == 0 );
            } else if ( _tcscmp( pstrName, _T( "defaultlink" ) ) == 0 ) {
              defaultlinkfont = ( _tcscmp( pstrValue, _T( "true" ) ) == 0 );
            }
          }
          if ( pFontName ) {
            HFONT hFont = UIResource_AddFontByName( res, pFontName, size, bold, underline, italic );
            if ( hFont != NULL ) {
              if ( defaultfont )
                UIResource_SetDefaultFont( res, hFont, 0xFF000000 );
              if ( defaultboldfont )
                UIResource_SetDefaultBoldFont( res, hFont, 0xFF000000 );
              if ( defaultlinkfont )
                UIResource_SetDefaultLinkFont( res, hFont, 0xFF0000FF, 0xFFD3215F );
            }
          }
        } else if ( _tcscmp( pstrClass, _T( "Default" ) ) == 0 ) {
          LPCTSTR pControlName = NULL;
          LPCTSTR pControlValue = NULL;
          nAttributes = GetAttributeCount(&node);
          for ( i = 0; i < nAttributes; i++ ) {
            pstrName = GetAttributeName(&node, i );
            pstrValue = GetAttributeValueByIndex( &node, i );
            if ( _tcscmp( pstrName, _T( "name" ) ) == 0 ) {
              pControlName = pstrValue;
            } else if ( _tcscmp( pstrName, _T( "value" ) ) == 0 ) {
              pControlValue = pstrValue;
            }
          }
          if ( pControlName ) {
            UIResource_AddDefaultAttributeList( res, pControlName, pControlValue );
          }
        }
      }
      if ( res ) {
        int cx, cy;
        LPTSTR pstr = NULL;
        int nAttributes = GetAttributeCount(&root);
        for ( i = 0; i < nAttributes; i++ ) {
          pstrName = GetAttributeName( &root, i );
          pstrValue = GetAttributeValueByIndex( &root, i );
          if ( _tcscmp( pstrName, _T( "size" ) ) == 0 ) {
            cx = _tcstol( pstrValue, &pstr, 10 );
            ASSERT( pstr );
            cy = _tcstol( pstr + 1, &pstr, 10 );
            ASSERT( pstr );
            //res->SetInitSize( cx, cy );
          } else if ( _tcscmp( pstrName, _T( "sizebox" ) ) == 0 ) {
            RECT rcSizeBox = { 0 };
            rcSizeBox.left = _tcstol( pstrValue, &pstr, 10 );
            ASSERT( pstr );
            rcSizeBox.top = _tcstol( pstr + 1, &pstr, 10 );
            ASSERT( pstr );
            rcSizeBox.right = _tcstol( pstr + 1, &pstr, 10 );
            ASSERT( pstr );
            rcSizeBox.bottom = _tcstol( pstr + 1, &pstr, 10 );
            ASSERT( pstr );
            //res->SetSizeBox( rcSizeBox );
          } else if ( _tcscmp( pstrName, _T( "caption" ) ) == 0 ) {
            RECT rcCaption = { 0 };
            rcCaption.left = _tcstol( pstrValue, &pstr, 10 );
            ASSERT( pstr );
            rcCaption.top = _tcstol( pstr + 1, &pstr, 10 );
            ASSERT( pstr );
            rcCaption.right = _tcstol( pstr + 1, &pstr, 10 );
            ASSERT( pstr );
            rcCaption.bottom = _tcstol( pstr + 1, &pstr, 10 );
            ASSERT( pstr );
            //res->SetCaptionRect( rcCaption );
          } else if ( _tcscmp( pstrName, _T( "roundcorner" ) ) == 0 ) {
            cx = _tcstol( pstrValue, &pstr, 10 );
            ASSERT( pstr );
            cy = _tcstol( pstr + 1, &pstr, 10 );
            ASSERT( pstr );
            //res->SetRoundCorner( cx, cy );
          } else if ( _tcscmp( pstrName, _T( "mininfo" ) ) == 0 ) {
            cx = _tcstol( pstrValue, &pstr, 10 );
            ASSERT( pstr );
            cy = _tcstol( pstr + 1, &pstr, 10 );
            ASSERT( pstr );
            //res->SetMinMaxInfo( cx, cy );
          } else if ( _tcscmp( pstrName, _T( "showdirty" ) ) == 0 ) {
            //res->SetShowUpdateRect( _tcscmp( pstrValue, _T( "true" ) ) == 0 );
          }
        }
      }
    }
  }
  ctr = _Parse( &root, NULL, res );
  CMarkup_Release(&m_xml);
  return ctr;
}
