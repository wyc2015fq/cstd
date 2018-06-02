#include <imm.h>
#pragma comment(lib, "imm32.lib") 
// C4711: 为内联展开选定了函数“function”：虽然给定函数未标记为内联，但编译
// 器在该函数上执行了内联
//#pragma warning(disable : 4711)
// 当你使用了CSplitterWnd并且由该CSplitterWnd提供水平滚动条时，定义该变量
// #define _SYNC_SCROLLING
// 绘制换行符的最小宽度
const LONG PFX_EC_MIN_LINE_BREAK_WIDTH = 4;
// 文本将相对于文本区左边缘缩进该逻辑单元数
const LONG PFX_EC_MIN_TEXT_INDENT_LEFT = 5;
// 文本将相对于文本区上边缘缩进该逻辑单元数
const LONG PFX_EC_MIN_TEXT_INDENT_TOP = 3;
// 最小的行号区宽度
const LONG PFX_EC_LINE_NUMBER_MIN_WIDTH = 20;
// 行号文字与视图左边框间的间隔
const LONG PFX_EC_LINE_NUMBER_INDENT_LEFT = 5;
// 行号文字相对于行号区右边的缩进
const LONG PFX_EC_LINE_NUMBER_INDENT_RIGHT = 0;
// 最小的标尺高度
const LONG PFX_EC_RULER_MIN_HEIGHT = 16;
// 标尺刻度中凡是该数值的整数倍将是主刻度
const LONG PFX_EC_RULER_RESOLUTION = 10;
// 标尺刻度线宽度
const LONG PFX_EC_RULER_SCALE_WIDTH = 1;
// 打印时正文和标题的距离
const LONG PFX_EC_DEF_SPACING_HEADER_AND_TEXT = 20;
// 打印时正文和脚注的距离
const LONG PFX_EC_DEF_SPACING_FOOTER_AND_TEXT = 10;
// 标题脚注正文和水平分隔线的距离
const LONG PFX_EC_DEF_SPACING_HEADER_FOOTER_TEXT_AND_SEPARATOR = 1;
inline BOOL CPfxEditView::IsLeadByte( UINT c ) {
  return PFX_EC_IS_LEAD_BYTE( c );
}
inline BOOL CPfxEditView::IsWhitespace( WCHAR wc ) const {
  return ( wc == PFX_EC_CHR_SPACE ) || ( wc == PFX_EC_CHR_TAB );
}
inline BOOL CPfxEditView::IsDelayRecalcWrapping() const {
  return m_bDelayRecalcWrapping;
}
inline BOOL CPfxEditView::IsFirstSiblingView() const {
  ASSERT( m_pdb != NULL );
  return ( this == m_pdb->m_arrViews[ 0 ] );
}
inline BOOL CPfxEditView::IsWrapping() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->IsWrapping();
}
inline BOOL CPfxEditView::IsWrapAtWindowEdge() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->IsWrapAtWindowEdge();
}
inline BOOL CPfxEditView::IsWrapAfterColumns() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->IsWrapAfterColumns();
}
inline BOOL CPfxEditView::IsDrawRuler() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->IsDrawRuler();
}
inline BOOL CPfxEditView::IsDrawLineNumber() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->IsDrawLineNumber();
}
inline BOOL CPfxEditView::IsDrawWhitespace() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->IsDrawWhitespace();
}
inline BOOL CPfxEditView::IsDrawLineBreak() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->IsDrawLineBreak();
}
inline BOOL CPfxEditView::IsDrawHilighted() const {
  ASSERT( m_pti != NULL );
  return m_pti->IsDrawHilighted();
}
inline BOOL CPfxEditView::IsCanColumnSelect() const {
  return !IsWrapping();
}
inline BOOL CPfxEditView::IsCanColumnPaste() const {
  return ( IsCanPaste() && !IsWrapping() );
}
inline BOOL CPfxEditView::IsCanDelete() const {
  ASSERT( m_pdb != NULL );
  return !IsReadOnly() && ( m_pdb->GetDocSize() > 0 );
}
inline BOOL CPfxEditView::IsCanCopy() const {
  return IsTextSelected();
}
inline BOOL CPfxEditView::IsCanCut() const {
  return !IsReadOnly() && IsTextSelected();
}
inline BOOL CPfxEditView::IsCanPaste() const {
  return !IsReadOnly() && ::IsClipboardFormatAvailable( CF_TEXT );
}
inline BOOL CPfxEditView::IsCanUndo() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->IsCanUndo();
}
inline BOOL CPfxEditView::IsCanRedo() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->IsCanRedo();
}
inline BOOL CPfxEditView::IsColumnSelect() const {
  ASSERT( m_pdb != NULL );
  return ( m_nSelMode == PFX_EC_SM_COLUMN ) && ( m_pdb->GetDocSize() > 0 );
}
inline BOOL CPfxEditView::IsOverride() const {
  return m_bOverride;
}
inline BOOL CPfxEditView::IsReadOnly() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->IsReadOnly();
}
inline BOOL CPfxEditView::IsTracking() const {
  return m_bTrackingMouse;
}
inline BOOL CPfxEditView::IsColumnSelectTracking() const {
  return m_bTrackingColSelect;
}
inline BOOL CPfxEditView::IsDragging() const {
  return m_bDragging;
}
inline BOOL CPfxEditView::IsDragDropHandled() const {
  return m_bDragDropHandled;
}
inline BOOL CPfxEditView::IsTextSelected() const {
  return m_nSelLineFrom != PFX_EC_INVALID_LINE;
}
inline BOOL CPfxEditView::IsDragDropEnabled() const {
  ASSERT( m_pti != NULL );
  return m_pti->m_bEnableDragDrop;
}
inline BOOL CPfxEditView::IsExpandTabToSpaceChars() const {
  ASSERT( m_pti != NULL );
  return m_pti->m_bExpandTabToSpaceChars;
}
inline BOOL CPfxEditView::IsHomeKeyGotoFirstNonWhitespaceChar() const {
  ASSERT( m_pti != NULL );
  return m_pti->m_bHomeKeyGotoFirstNonWhitespaceChar;
}
inline BOOL CPfxEditView::IsSyncActiveLineBkColorWithTextBkColor() const {
  ASSERT( m_pti != NULL );
  return m_pti->m_bSyncActiveLineBkColorWithTextBkColor;
}
inline BOOL CPfxEditView::IsSyncTextBkColorWithSystem() const {
  ASSERT( m_pti != NULL );
  return m_pti->m_bSyncTextBkColorWithSystem;
}
inline LONG CPfxEditView::GetCaretReferPoint() const {
  return m_ptCaretRefer.x;
}
inline LONG CPfxEditView::GetFirstVisibleLine() const {
  return m_ptFirstVisible.y;
}
inline LONG CPfxEditView::GetLastVisibleLine() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->TextVerifyLine( m_ptFirstVisible.y + m_sizeVisible.cy - 1 );
}
inline LONG CPfxEditView::GetUndoName() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetUndoName();
}
inline LONG CPfxEditView::GetRedoName() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetRedoName();
}
inline LONG CPfxEditView::GetDocSize() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetDocSize();
}
inline LONG CPfxEditView::GetTabSize() const {
  ASSERT( m_pti != NULL );
  return m_pti->m_nTabSize;
}
inline LONG CPfxEditView::GetLineCount() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetLineCount();
}
inline LONG CPfxEditView::GetLineUpperBound() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetLineUpperBound();
}
inline LONG CPfxEditView::GetLineLength( LONG nLine ) const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetLineLength( nLine );
}
inline LONG CPfxEditView::GetLineData( LONG nLine, LPTSTR pBuffer, LONG nBufMax ) const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetLineData( nLine, pBuffer, nBufMax );
}
inline LONG CPfxEditView::GetWrapStyle() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetWrapStyle();
}
inline LONG CPfxEditView::GetWrapWidth() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetWrapWidth();
}
inline LONG CPfxEditView::GetLineBreakSize() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetLineBreakSize();
}
inline LONG CPfxEditView::GetLineBreakType() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetLineBreakType();
}
inline HFONT CPfxEditView::GetFont() {
  return PFX_EC_DATA.m_hFontText;
}
inline LONG CPfxEditView::GetLineHeight() {
  return PFX_EC_DATA.m_nLineHeight;
}
inline LONG CPfxEditView::GetSpacingLines() {
  return PFX_EC_DATA.m_nSpacingLines;
}
inline LONG CPfxEditView::GetSpacingLineNumberAndText() {
  return PFX_EC_DATA.m_nSpacingLineNumberAndText;
}
inline LONG CPfxEditView::GetLineTextVertOffset() const {
  return GetSpacingLines() / 2;
}
inline void CPfxEditView::SetColumnSelect() {
  SetColumnSelectTracking( true );
}
inline void CPfxEditView::SetColumnSelectTracking( BOOL bOn ) {
  m_bTrackingColSelect = bOn;
}
inline void CPfxEditView::SetDragging( BOOL bOn ) {
  m_bDragging = bOn;
}
inline void CPfxEditView::SetDragDropHandled( BOOL bOn ) {
  m_bDragDropHandled = bOn;
}
inline void CPfxEditView::SetDelayRecalcWrapping( BOOL bOn ) {
  m_bDelayRecalcWrapping = bOn;
}
inline void CPfxEditView::SetOverride( BOOL bOn ) {
  m_bOverride = bOn;
}
inline void CPfxEditView::SetReadOnly( BOOL bOn ) {
  ASSERT( m_pdb != NULL );
  m_pdb->SetReadOnly( bOn );
}
inline void CPfxEditView::SetModifiedFlag( BOOL bChanged ) {
  ASSERT( m_pdb != NULL );
  m_pdb->SetModifiedFlag( bChanged );
}
inline void CPfxEditView::SetLineBreakType( LONG nType ) {
  ASSERT( m_pdb != NULL );
  m_pdb->SetLineBreakType( nType );
}
inline void CPfxEditView::SetLockBufferCompact( BOOL bOn ) {
  ASSERT( m_pdb != NULL );
  m_pdb->SetLockBufferCompact( bOn );
}
inline void CPfxEditView::SetLockWordWrapEvent( BOOL bOn ) {
  ASSERT( m_pdb != NULL );
  m_pdb->SetLockWordWrapEvent( bOn );
}
inline void CPfxEditView::SetUndoGroup( BOOL bBegin, WORD fUndoName ) {
  ASSERT( m_pdb != NULL );
  m_pdb->UndoSetGroup( bBegin, fUndoName );
}
inline LONG CPfxEditView::BookmarkGetCount() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetBookmarkCount();
}
inline BOOL CPfxEditView::ParserIsEscape( WCHAR wc ) const {
  ASSERT( m_pti != NULL );
  return m_pti->IsEscape( wc );
}
inline BOOL CPfxEditView::ParserIsKeyword( LPCTSTR pData, LONG cch ) const {
  ASSERT( m_pti != NULL );
  return ( m_pti->m_mapKeywords.Lookup( pData, cch ) != -1 );
}
// CPfxEditData...
CPfxEditData PFX_EC_DATA;
CPfxEditData::CPfxEditData() {
  // 初始化参数
  m_nCharHeight = 0;
  m_nCharWidthMax = 0;
  m_nCharWidthAve = 0;
  m_nCharWidthMaxNum = 0;
  m_nCharWidthTab = 0;
  m_nCharWidthSpace = 0;
  m_nCharWidthLineBreak = 0;
  m_nRulerCharHeight = 0;
  m_nRulerCharWidthAve = 0;
  m_nRulerHeight = 0;
  m_nSpacingLines = 0;
  m_nSpacingLineNumberAndText = PFX_EC_MIN_TEXT_INDENT_LEFT;
  m_sizeCaret.cx = 0;
  m_sizeCaret.cy = 0;
  m_hFontText = NULL;
  m_hCurStandard = ::LoadCursor( NULL, IDC_ARROW );
  m_hCurText = ::LoadCursor( NULL, IDC_IBEAM );
  m_hCurColSelect = ::LoadCursor( NULL, IDC_CROSS );
  m_hDataDragDropCopied = NULL;
  m_hDataClipboardCopied = NULL;
  m_nDataClipboardCopied = 0;
  ASSERT( PFX_EC_SIZEOF( m_arrLeadByte ) >= UCHAR_MAX );
#ifdef _UNICODE
  for ( UCHAR c = 0; c < UCHAR_MAX; c++ ) {
    m_arrLeadByte[ c ] = false;
  }
#else
  // BLOCK：记录系统所用代码页中的“LeadByte”字符
  // 不要用CRT的_isleadbyte()因为这时CRT还没有初试化
  for ( UCHAR c = 0; c < UCHAR_MAX; c++ ) {
    m_arrLeadByte[ c ] = ::IsDBCSLeadByte( c );
  }
#endif
}
CPfxEditData::~CPfxEditData() {
  // 释放光标资源
  HCURSOR arrHCursor[ 3 ];
  arrHCursor[ 0 ] = m_hCurStandard;
  arrHCursor[ 1 ] = m_hCurText;
  arrHCursor[ 2 ] = m_hCurColSelect;
  for ( LONG i = 0; i < PFX_EC_SIZEOF( arrHCursor ); i++ ) {
    if ( arrHCursor[ i ] != NULL ) {
      ::DestroyCursor( arrHCursor[ i ] );
    }
  }
}
// CPfxEditAlloc...
CPfxEditAlloc::CAllocator::CAllocator( size_t nUnitSize, size_t nMaxUnits ) {
  ASSERT( nUnitSize >= sizeof( CNode ) );
  ASSERT( nMaxUnits > 1 );
  m_nUnitSize = nUnitSize;
  m_nMaxUnits = nMaxUnits;
  m_nBlockSize = m_nUnitSize * m_nMaxUnits + sizeof( CPlex );
  m_nBlocks = 0;
  m_pBlocks = NULL;
}
void* CPfxEditAlloc::CAllocator::Alloc() {
  CNode * pNode;
  CPlex* pBlock = m_pBlocks;
  while ( pBlock != NULL ) {
    if ( pBlock->pNodeFree != NULL ) {
      pNode = pBlock->pNodeFree;
      pBlock->pNodeFree = pBlock->pNodeFree->pNext;
      pBlock->nUsed++;
      ASSERT( ( pBlock->nUsed >= 0 ) && ( pBlock->nUsed <= m_nMaxUnits ) );
      return ( void* ) pNode;
    }
    pBlock = pBlock->pNext;
  }
  pNode = NULL;
  pBlock = ( CPlex* ) new BYTE[ m_nBlockSize ];
  if ( pBlock != NULL ) {
    pBlock->pNext = m_pBlocks;
    pBlock->pPrev = NULL;
    pBlock->nUsed = 1;
    if ( m_pBlocks != NULL ) {
      m_pBlocks->pPrev = pBlock;
    }
    m_pBlocks = pBlock;
    m_nBlocks++;
    pNode = ( CNode* ) pBlock->data();
    ( BYTE*& ) pNode += ( m_nUnitSize * m_nMaxUnits ) - m_nUnitSize;
    CNode* pNodeFree = NULL;
    for ( int i = m_nMaxUnits - 1; i >= 0; i--, ( BYTE*& ) pNode -= m_nUnitSize ) {
      pNode->pNext = pNodeFree;
      pNodeFree = pNode;
    }
    pNode = pNodeFree;
    pBlock->pNodeFree = pNodeFree->pNext;
  }
  return ( void* ) pNode;
}
void CPfxEditAlloc::CAllocator::Free( void* lp ) {
#ifdef _DEBUG
  BOOL bHit = false;
#endif
  CPlex* pBlock = m_pBlocks;
  size_t cbBlock = m_nBlockSize;
  while ( pBlock != NULL ) {
    // 这里的遍历不会太花时间，您应该控制m_nBlocks的数目，使它
    // 不要过大，一般几块或十多块就OK了
    if ( ( ( BYTE* ) lp <= ( BYTE* ) pBlock )
         || ( ( BYTE* ) lp >= ( BYTE* ) ( ( BYTE* ) pBlock + cbBlock ) ) ) {
      pBlock = pBlock->pNext;
      continue;
    }
#ifdef _DEBUG
    bHit = true;
#endif
    pBlock->nUsed--;
    ASSERT( ( pBlock->nUsed >= 0 ) && ( pBlock->nUsed <= m_nMaxUnits ) );
    // 这一块已经没有人使用就可以释放掉
    if ( pBlock->nUsed > 0 ) {
      ASSERT( m_nBlocks > 0 );
      ASSERT( ( ( BYTE* ) lp >= ( ( BYTE* ) pBlock + sizeof( CPlex ) ) ) );
      CNode* pNode = ( CNode* ) lp;
      pNode->pNext = pBlock->pNodeFree;
      pBlock->pNodeFree = pNode;
    } else {
      CPlex* pNext = pBlock->pNext;
      CPlex* pPrev = pBlock->pPrev;
      if ( pBlock == m_pBlocks ) {
        m_pBlocks = pNext;
        if ( pNext != NULL ) {
          pNext->pPrev = NULL;
        }
      } else {
        pPrev->pNext = pNext;
        if ( pNext != NULL ) {
          pNext->pPrev = pPrev;
        }
      }
      m_nBlocks--;
      delete[] ( BYTE* ) pBlock;
    }
    break;
  }
#ifdef _DEBUG
  ASSERT( bHit );
#endif
}
void CPfxEditAlloc::CAllocator::FreeAll() {
  CPlex * pBlock = m_pBlocks;
  while ( pBlock != NULL ) {
    BYTE * bytes = ( BYTE* ) pBlock;
    pBlock = pBlock->pNext;
    delete[] bytes;
  }
  m_nBlocks = 0;
  m_pBlocks = NULL;
}
const int PFX_EC_ALLOC_SIZE_1 = ( 8 * sizeof( TCHAR ) );
const int PFX_EC_ALLOC_SIZE_2 = ( 16 * sizeof( TCHAR ) );
const int PFX_EC_ALLOC_SIZE_3 = ( 24 * sizeof( TCHAR ) );
const int PFX_EC_ALLOC_SIZE_4 = ( 32 * sizeof( TCHAR ) );
const int PFX_EC_ALLOC_SIZE_5 = ( 64 * sizeof( TCHAR ) );
const int PFX_EC_ALLOC_SIZE_6 = ( 128 * sizeof( TCHAR ) );
const int PFX_EC_ALLOC_SIZE_7 = ( 256 * sizeof( TCHAR ) );
const int PFX_EC_ALLOC_SIZE_8 = ( 512 * sizeof( TCHAR ) );
const int PFX_EC_BLOCK_SIZE_1 = ( PFX_EC_ROUND4( PFX_EC_ALLOC_SIZE_1 + sizeof( int ) ) );
const int PFX_EC_BLOCK_SIZE_2 = ( PFX_EC_ROUND4( PFX_EC_ALLOC_SIZE_2 + sizeof( int ) ) );
const int PFX_EC_BLOCK_SIZE_3 = ( PFX_EC_ROUND4( PFX_EC_ALLOC_SIZE_3 + sizeof( int ) ) );
const int PFX_EC_BLOCK_SIZE_4 = ( PFX_EC_ROUND4( PFX_EC_ALLOC_SIZE_4 + sizeof( int ) ) );
const int PFX_EC_BLOCK_SIZE_5 = ( PFX_EC_ROUND4( PFX_EC_ALLOC_SIZE_5 + sizeof( int ) ) );
const int PFX_EC_BLOCK_SIZE_6 = ( PFX_EC_ROUND4( PFX_EC_ALLOC_SIZE_6 + sizeof( int ) ) );
const int PFX_EC_BLOCK_SIZE_7 = ( PFX_EC_ROUND4( PFX_EC_ALLOC_SIZE_7 + sizeof( int ) ) );
const int PFX_EC_BLOCK_SIZE_8 = ( PFX_EC_ROUND4( PFX_EC_ALLOC_SIZE_8 + sizeof( int ) ) );
CPfxEditAlloc::CPfxEditAlloc() : m_b1( PFX_EC_BLOCK_SIZE_1, ( ( 4 * 1024 ) / PFX_EC_BLOCK_SIZE_1 ) ), m_b2( PFX_EC_BLOCK_SIZE_2, ( ( 4 * 1024 ) / PFX_EC_BLOCK_SIZE_2 ) ), m_b3( PFX_EC_BLOCK_SIZE_3, ( ( 4 * 1024 ) / PFX_EC_BLOCK_SIZE_3 ) ), m_b4( PFX_EC_BLOCK_SIZE_4, ( ( 4 * 1024 ) / PFX_EC_BLOCK_SIZE_4 ) ), m_b5( PFX_EC_BLOCK_SIZE_5, ( ( 4 * 1024 ) / PFX_EC_BLOCK_SIZE_5 ) ), m_b6( PFX_EC_BLOCK_SIZE_6, ( ( 4 * 1024 ) / PFX_EC_BLOCK_SIZE_6 ) ), m_b7( PFX_EC_BLOCK_SIZE_7, ( ( 4 * 1024 ) / PFX_EC_BLOCK_SIZE_7 ) ), m_b8( PFX_EC_BLOCK_SIZE_8, ( ( 4 * 1024 ) / PFX_EC_BLOCK_SIZE_8 ) ) {}
CPfxEditAlloc::~CPfxEditAlloc() {}
void* CPfxEditAlloc::Alloc( size_t cb ) {
  int * pData;
  if ( cb <= PFX_EC_ALLOC_SIZE_1 ) {
    pData = ( int* ) m_b1.Alloc();
    *pData = PFX_EC_ALLOC_SIZE_1;
    pData++;
    return ( void* ) pData;
  }
  if ( cb <= PFX_EC_ALLOC_SIZE_2 ) {
    pData = ( int* ) m_b2.Alloc();
    *pData = PFX_EC_ALLOC_SIZE_2;
    pData++;
    return ( void* ) pData;
  }
  if ( cb <= PFX_EC_ALLOC_SIZE_3 ) {
    pData = ( int* ) m_b3.Alloc();
    *pData = PFX_EC_ALLOC_SIZE_3;
    pData++;
    return ( void* ) pData;
  }
  if ( cb <= PFX_EC_ALLOC_SIZE_4 ) {
    pData = ( int* ) m_b4.Alloc();
    *pData = PFX_EC_ALLOC_SIZE_4;
    pData++;
    return ( void* ) pData;
  }
  if ( cb <= PFX_EC_ALLOC_SIZE_5 ) {
    pData = ( int* ) m_b5.Alloc();
    *pData = PFX_EC_ALLOC_SIZE_5;
    pData++;
    return ( void* ) pData;
  }
  if ( cb <= PFX_EC_ALLOC_SIZE_6 ) {
    pData = ( int* ) m_b6.Alloc();
    *pData = PFX_EC_ALLOC_SIZE_6;
    pData++;
    return ( void* ) pData;
  }
  if ( cb <= PFX_EC_ALLOC_SIZE_7 ) {
    pData = ( int* ) m_b7.Alloc();
    *pData = PFX_EC_ALLOC_SIZE_7;
    pData++;
    return ( void* ) pData;
  }
  if ( cb <= PFX_EC_ALLOC_SIZE_6 ) {
    pData = ( int* ) m_b6.Alloc();
    *pData = PFX_EC_ALLOC_SIZE_6;
    pData++;
    return ( void* ) pData;
  }
  return new BYTE[ cb ];
}
void CPfxEditAlloc::Free( void* lp ) {
  if ( lp == NULL ) {
    return ;
  }
  int* pData = ( ( int* ) lp - 1 );
  switch ( *pData ) {
  default:
    delete lp;
    break;
  case PFX_EC_ALLOC_SIZE_1:
    m_b1.Free( pData );
    break;
  case PFX_EC_ALLOC_SIZE_2:
    m_b2.Free( pData );
    break;
  case PFX_EC_ALLOC_SIZE_3:
    m_b3.Free( pData );
    break;
  case PFX_EC_ALLOC_SIZE_4:
    m_b4.Free( pData );
    break;
  case PFX_EC_ALLOC_SIZE_5:
    m_b5.Free( pData );
    break;
  case PFX_EC_ALLOC_SIZE_6:
    m_b6.Free( pData );
    break;
  case PFX_EC_ALLOC_SIZE_7:
    m_b7.Free( pData );
    break;
  case PFX_EC_ALLOC_SIZE_8:
    m_b8.Free( pData );
    break;
  }
}
void CPfxEditAlloc::FreeAll() {
  m_b1.FreeAll();
  m_b2.FreeAll();
  m_b3.FreeAll();
  m_b4.FreeAll();
  m_b5.FreeAll();
  m_b6.FreeAll();
}
// CPfxEditHeap...
void* CPfxEditHeap::CAllocator::Alloc( size_t cb ) {
  CNode * pNode = NULL;
  for ( CNode * pNodeT = m_pNodes; pNodeT != NULL; pNodeT = pNodeT->pNext ) {
    if ( ( pNodeT->nRef == 0 ) && ( pNodeT->nSize >= cb ) ) {
      pNode = pNodeT;
      break;
    }
  }
  if ( pNode == NULL ) {
    size_t nAllocSize = PFX_EC_ROUND( cb, PFX_EC_TASKMEM_ALLOC_SIZE );
    size_t nBlockSize = sizeof( CNode ) + nAllocSize;
    pNode = reinterpret_cast<CNode*>( new BYTE[ nBlockSize ] );
    pNode->nRef = 0;
    pNode->nSize = nAllocSize;
    pNode->pPrev = NULL;
    pNode->pNext = m_pNodes;
    if ( m_pNodes != NULL ) {
      m_pNodes->pPrev = pNode;
    }
    m_nNodes++;
    m_pNodes = pNode;
  }
  pNode->nRef = 1;
  return pNode->data();
}
void CPfxEditHeap::CAllocator::Free( void* lp ) {
  if ( lp == NULL ) {
    return ;
  }
  TRACE( "CPfxEditHeap::CAllocator::Free(): Current blocks are: %d\n", m_nNodes );
  CNode* pNode = reinterpret_cast<CNode*>( reinterpret_cast<CNode*>( lp ) - 1 );
  if ( ( m_nNodes <= 3 ) && ( pNode->nSize <= PFX_EC_TASKMEM_CONST_SIZE ) ) {
    pNode->nRef = 0;
    return ;
  }
  CNode* pPrev = pNode->pPrev;
  CNode* pNext = pNode->pNext;
  if ( pNode == m_pNodes ) {
    m_pNodes = pNext;
    if ( pNext != NULL ) {
      pNext->pPrev = NULL;
    }
  } else {
    pPrev->pNext = pNext;
    if ( pNext != NULL ) {
      pNext->pPrev = pPrev;
    }
  }
  m_nNodes--;
  delete pNode;
}
void CPfxEditHeap::CAllocator::FreeAll() {
  for ( CNode * pNodeH = m_pNodes; pNodeH != NULL; ) {
    CNode* pNodeT = pNodeH;
    pNodeH = pNodeH->pNext;
    delete pNodeT;
  }
  m_nNodes = 0;
  m_pNodes = NULL;
}
void CPfxEditHeap::Free() {
  if ( m_pBuffer != NULL ) {
    PFX_EC_DATA.m_heap.Free( m_pBuffer );
    m_pBuffer = NULL;
  }
}
void* CPfxEditHeap::Alloc( size_t cb ) {
  Free();
  m_pBuffer = PFX_EC_DATA.m_heap.Alloc( cb );
  m_nBufferSize = m_nDataSize = ( ( m_pBuffer != NULL ) ? cb : 0 );
  return m_pBuffer;
}
void* CPfxEditHeap::ReAlloc( size_t cb ) {
  if ( m_pBuffer == NULL ) {
    return Alloc( cb );
  }
  if ( cb <= m_nBufferSize ) {
    m_nDataSize = cb;
    return m_pBuffer;
  }
  void* pBuffer = PFX_EC_DATA.m_heap.Alloc( cb );
  if ( pBuffer != NULL ) {
    memcpy( pBuffer, m_pBuffer, m_nDataSize );
    PFX_EC_DATA.m_heap.Free( m_pBuffer );
    m_pBuffer = pBuffer;
    m_nBufferSize = m_nDataSize = cb;
  }
  return pBuffer;
}
// CPfxEditHash...
CPfxEditHash::CPfxEditHash() {
  memset( m_arrHashTable, 0, sizeof( m_arrHashTable ) );
  m_nCount = 0;
  m_bMatchCase = true;
}
CPfxEditHash::~CPfxEditHash() {
  RemoveAll();
}
void CPfxEditHash::RemoveAll() {
  if ( GetCount() <= 0 ) {
    return ;
  }
  for ( int i = 0; i < PFX_EC_SIZEOF( m_arrHashTable ); i++ ) {
    CAssoc* pAssoc = m_arrHashTable[ i ];
    while ( pAssoc != NULL ) {
      CAssoc * pNext = pAssoc->pNext;
      PFX_EC_DATA.MemFree( pAssoc );
      pAssoc = pNext;
    }
  }
  m_nCount = 0;
  memset( m_arrHashTable, 0, sizeof( m_arrHashTable ) );
}
void CPfxEditHash::RemoveAt( LPCTSTR pszKey ) {
  CAssocInfo i;
  if ( GetAssocAt( pszKey, -1, i ) ) {
    m_arrHashTable[ i.nHash ] = i.pAssoc->pNext;
    PFX_EC_DATA.MemFree( i.pAssoc );
  }
}
void CPfxEditHash::SetAt( LONG nKeyClass, LPCTSTR pszKey ) {
  if ( ( pszKey == NULL ) || ( *pszKey == '\0' ) ) {
    return ;
  }
  CAssocInfo i;
  if ( GetAssocAt( pszKey, -1, i ) )           //      覆盖已有匹配项
  {
    i.pAssoc->nKeyClass = nKeyClass;
  } else // 没有匹配项则插入新项
  {
    // sizeof(CAssoc)中已经包含了容纳NULL字符的空间
    CAssoc* pAssoc = static_cast<CAssoc*>( PFX_EC_DATA.MemAlloc( sizeof( CAssoc ) + ( i.cchKey * sizeof( TCHAR ) ) ) );
    ASSERT( pAssoc != NULL );
    ASSERT( AfxIsValidAddress( pAssoc, sizeof( CAssoc ), true ) );
    memcpy( pAssoc->szKey, i.pszKey, ( i.cchKey * sizeof( TCHAR ) ) );
    pAssoc->cchKey = i.cchKey;
    pAssoc->szKey[ i.cchKey ] = '\0';
    pAssoc->nKeyClass = nKeyClass;
    pAssoc->pNext = m_arrHashTable[ i.nHash ];
    m_arrHashTable[ i.nHash ] = pAssoc;
    m_nCount++;
  }
}
// LPCTSTR pszKey     - 关键字，该字符不一定要NULL结尾，这时由cch指定长度
// LONG cch           - 如果等于-1则假定字符串为NULL结尾，长度自动计算
// CAssocInfo& retVal - 返回m_arrHashTable的散列值和CAssoc*指针
// 函数返回值：匹配返回TRUE否则返回FALSE
BOOL CPfxEditHash::GetAssocAt( LPCTSTR pszKey, LONG cch, CAssocInfo& retVal ) {
  if ( pszKey == NULL ) {
    return false;
  }
  // 不能处理过长的关键字
  if ( cch == -1 ) {
    cch = _tcslen( pszKey );
  }
  LONG cchKey = min( cch, PFX_EC_KEYWORD_MAP_MAX_KEYWORD_LEN );
  // 忽略大小写比较需要先转换成小写，加上一是用来容纳NULL结尾
  if ( !m_bMatchCase ) {
    memcpy( m_szKeyLower, pszKey, cchKey * sizeof( TCHAR ) );
    CharLowerBuff( m_szKeyLower, cchKey );
    m_szKeyLower[ cchKey ] = '\0';
    pszKey = m_szKeyLower;
  }
  retVal.pszKey = pszKey;
  retVal.cchKey = cchKey;
  retVal.nHash = HashKey( pszKey, cchKey );
  for ( CAssoc * pAssoc = m_arrHashTable[ retVal.nHash ]; pAssoc != NULL; pAssoc = pAssoc->pNext ) {
    // 长度不相等就不需要继续比较
    if ( cch != pAssoc->cchKey ) {
      continue;
    }
    // 比较关键字
    LPCTSTR s1 = pAssoc->szKey;
    LPCTSTR s2 = pszKey;
    while ( *s1 && ( *s1 == *s2 ) ) {
      s1++;
      s2++;
    }
    if ( *s1 == '\0' ) {
      retVal.pAssoc = pAssoc;
      return true;
    }
  }
  return false;
}
// CKeyClassArray
CKeyClassArray::CKeyClassArray() {
  // 设置预定义项为有效，以免被其他自定义项占用地方
  ASSERT( kcPredefinedKeyClassNumber < PFX_EC_SIZEOF( m_arrClasses ) );
  for ( int i = 0; i < kcPredefinedKeyClassNumber; i++ ) {
    m_arrClasses[ i ].m_bEmpty = false;
  }
}
CKeyClassArray::~CKeyClassArray() {
  for ( int i = 0; i < PFX_EC_SIZEOF( m_arrClasses ); i++ ) {
    RemoveClass( i );
  }
}
CKeyClassArray&
CKeyClassArray::operator=( const CKeyClassArray& src ) {
  for ( int i = 0; i < PFX_EC_SIZEOF( m_arrClasses ); i++ ) {
    m_arrClasses[ i ] = src.m_arrClasses[ i ];
  }
  return *this;
}
// 获取关键字分组信息
POSITION CKeyClassArray::GetClassFirst() const {
  return ( !m_arrClasses[ 0 ].IsEmpty() ? ( POSITION ) - 1 : NULL );
}
void CKeyClassArray::GetClassNext( POSITION& rPosition, CPfxEditKeyClass& theClass ) const {
  int i = ( int ) rPosition;
  if ( i == -1 )           //      要求第一个单元
  {
    i = 0;
  }
  if ( !IsValidClass( i ) ) {
    theClass.SetEmpty();
  } else {
    theClass = m_arrClasses[ i ];
  }
  i++;
  if ( !IsValidClass( i ) ) {
    i = NULL;
  }
  rPosition = ( POSITION ) i;
}
LPCTSTR CKeyClassArray::GetClassName( LONG nIndex ) const {
  return ( IsValidClass( nIndex ) ? ( LPCTSTR ) ( m_arrClasses[ nIndex ].m_strClassName ) : NULL );
}
void CKeyClassArray::SetClassName( LONG nIndex, LPCTSTR pszClassName ) {
  if ( IsValidClass( nIndex ) ) {
    m_arrClasses[ nIndex ].m_strClassName = ( pszClassName != NULL ) ? pszClassName : _T( "" );
  }
}
// 添加关键字-颜色定义对，返回已保存的索引号
LONG CKeyClassArray::SetClass( const CPfxEditKeyClass& theClass ) {
  for ( LONG i = 0; i < PFX_EC_SIZEOF( m_arrClasses ); i++ ) {
    CPfxEditKeyClass& item = m_arrClasses[ i ];
    // 如果遇到空的单元，表明前面没有与theClass相同的项
    if ( item.IsEmpty() ) {
      item = theClass;
      item.m_bEmpty = false;
      break;
    }
    // 如果遇到相同的类名（不区分大小写）
    if ( item.IsClass( theClass.m_strClassName ) ) {
      item.m_color = theClass.m_color;
      item.m_bEmpty = false;
      break;
    }
  }
  return ( ( i < PFX_EC_SIZEOF( m_arrClasses ) ) ? i : -1 );
}
void CKeyClassArray::RemoveClass( LONG nIndex ) {
  if ( IsValidClass( nIndex ) ) {
    m_arrClasses[ nIndex ].SetEmpty();
  }
}
void CKeyClassArray::RemoveClass( LPCTSTR pszClassName ) {
  if ( pszClassName == NULL ) {
    return ;
  }
  for ( int i = 0; i < PFX_EC_SIZEOF( m_arrClasses ); i++ ) {
    const CPfxEditKeyClass& item = m_arrClasses[ i ];
    if ( item.IsClass( pszClassName ) ) {
      RemoveClass( i );
      break;
    }
  }
}
// CMarkerPairArr
CMarkerPairArr::CMarkerPairArr() {
  for ( int i = 0; i < PFX_EC_SIZEOF( m_arrMarkers ); i++ ) {
    m_arrMarkers[ i ] = NULL;
  }
}
CMarkerPairArr::~CMarkerPairArr() {
  RemoveAll();
}
void CMarkerPairArr::RemoveAll() {
  for ( int i = 0; i < PFX_EC_SIZEOF( m_arrMarkers ); i++ ) {
    FreeMarkerChain( m_arrMarkers[ i ] );
    m_arrMarkers[ i ] = NULL;
  }
}
void CMarkerPairArr::FreeMarkerChain( CPfxEditMarkerPair* pMarker ) {
  while ( pMarker != NULL ) {
    CPfxEditMarkerPair * pNext = pMarker->pNext;
    PFX_EC_DATA.MemFree( pMarker );
    pMarker = pNext;
  }
}
CPfxEditMarkerPair* CMarkerPairArr::CopyMarkerChain( CPfxEditMarkerPair* pMarker ) {
  CPfxEditMarkerPair * pReturn = NULL;
  if ( pMarker != NULL ) {
    pReturn = CreateMarker( pMarker->pszOn, pMarker->pszOff );
    pMarker = pMarker->pNext;
    CPfxEditMarkerPair* pPrev = pReturn;
    while ( pMarker != NULL ) {
      CPfxEditMarkerPair * pNext = CreateMarker( pMarker->pszOn, pMarker->pszOff );
      if ( pNext == NULL ) {
        break;
      }
      pPrev->pNext = pNext;
      pNext->pPrev = pPrev;
      pPrev = pNext;
      pMarker = pMarker->pNext;
    }
  }
  return pReturn;
}
CPfxEditMarkerPair* CMarkerPairArr::CreateMarker( LPCTSTR pszOn, LPCTSTR pszOff ) {
  LONG cchOn = ( ( pszOn != NULL ) ? _tcslen( pszOn ) : 0 );
  LONG cchOff = ( ( pszOff != NULL ) ? _tcslen( pszOff ) : 0 );
  if ( ( cchOn == 0 ) && ( cchOff == 0 ) ) {
    return NULL;
  }
  CPfxEditMarkerPair* pMarker = static_cast<CPfxEditMarkerPair*>( PFX_EC_DATA.MemAlloc( sizeof( CPfxEditMarkerPair ) + ( ( cchOn + cchOff ) * sizeof( TCHAR ) ) ) );
  if ( pMarker == NULL ) {
    return NULL;
  }
  pMarker->pNext = NULL;
  pMarker->pPrev = NULL;
  if ( cchOn <= 0 ) {
    pMarker->pszOn = NULL;
    pMarker->cchOn = 0;
    pMarker->wLowerOn = 0;
    pMarker->wUpperOn = 0;
  } else {
    LPTSTR lp = &pMarker->szKey[ 0 ];
    _tcscpy( lp, pszOn );
    WCHAR wc = ( !CPfxEditView::IsLeadByte( *lp ) ? ( ( WCHAR ) * lp ) : ( *( ( LPWSTR ) lp ) ) );
    pMarker->pszOn = lp;
    pMarker->cchOn = cchOn;
    pMarker->wLowerOn = wc;
    pMarker->wUpperOn = wc;
    CharLowerBuff( ( LPTSTR ) & pMarker->wLowerOn, sizeof( pMarker->wLowerOn ) );
    CharUpperBuff( ( LPTSTR ) & pMarker->wUpperOn, sizeof( pMarker->wUpperOn ) );
  }
  if ( cchOff <= 0 ) {
    pMarker->pszOff = NULL;
    pMarker->cchOff = 0;
    pMarker->wLowerOff = 0;
    pMarker->wUpperOff = 0;
  } else {
    LPTSTR lp = &pMarker->szKey[ cchOn + 1 ];
    _tcscpy( lp, pszOff );
    WCHAR wc = ( !CPfxEditView::IsLeadByte( *lp ) ? ( ( WCHAR ) * lp ) : ( *( ( LPWSTR ) lp ) ) );
    pMarker->pszOff = lp;
    pMarker->cchOff = cchOff;
    pMarker->wLowerOff = wc;
    pMarker->wUpperOff = wc;
    CharLowerBuff( ( LPTSTR ) & pMarker->wLowerOff, sizeof( pMarker->wLowerOff ) );
    CharUpperBuff( ( LPTSTR ) & pMarker->wUpperOff, sizeof( pMarker->wUpperOff ) );
  }
  return pMarker;
}
CMarkerPairArr&
CMarkerPairArr::operator=( const CMarkerPairArr& src ) {
  RemoveAll();
  for ( int i = 0; i < PFX_EC_SIZEOF( m_arrMarkers ); i++ ) {
    m_arrMarkers[ i ] = CopyMarkerChain( src.m_arrMarkers[ i ] );
  }
  return *this;
}
// CPfxEditTypeInfo...
CPfxEditTypeInfo::CPfxEditTypeInfo() {
  m_bDrawHilighted = false;
  m_bDrawRuler = false;
  m_bDrawLineNumber = true;
  m_bDrawLineBreak = true;
  m_bDrawWhitespace = true;
  m_bEnableDragDrop = true;
  m_bExpandTabToSpaceChars = false;
  m_bSyncTextBkColorWithSystem = true;
  m_bHomeKeyGotoFirstNonWhitespaceChar = true;
  m_bSyncActiveLineBkColorWithTextBkColor = false;
  m_nTabSize = 8;
  m_nIndentSize = 8;
  m_nWrapStyle = PFX_EC_WRAP_NONE;
  m_nWrapWidth = 0;
  m_wEscape = 0; // 转义字符
  SetMatchCase( true );
  // 创建缺省文件类型（*.txt; *.*）的缺省参数
  SetDelimiters( NULL );
  // 预定义颜色
  COLORREF clrDefault = PFX_EC_CLR_TEXT;
  SetKeyClassColor( kcText, clrDefault );
  SetKeyClassColor( kcTextSelected, PFX_EC_CLR_TEXT_SELECTED );
  SetKeyClassColor( kcRuler, PFX_EC_CLR_RULER );
  SetKeyClassColor( kcLineNumber, PFX_EC_CLR_LINE_NUMBER );
  SetKeyClassColor( kcWhitespace, PFX_EC_CLR_WHITESPACE );
  SetKeyClassColor( kcBkText, PFX_EC_CLR_BK_TEXT );
  SetKeyClassColor( kcBkTextSelected, PFX_EC_CLR_BK_TEXT_SELECTED );
  SetKeyClassColor( kcBkRuler, PFX_EC_CLR_BK_RULER );
  SetKeyClassColor( kcBkLineNumber, PFX_EC_CLR_BK_LINE_NUMBER );
  SetKeyClassColor( kcBkActiveLine, PFX_EC_CLR_BK_ACTIVE_LINE );
  SetKeyClassColor( kcBkBookmark, PFX_EC_CLR_BK_BOOKMARK );
  SetKeyClassColor( kcHeaderAndFooter, clrDefault );
  SetKeyClassColor( kcQuotedString1, clrDefault );
  SetKeyClassColor( kcQuotedString2, clrDefault );
  SetKeyClassColor( kcLineComment1, clrDefault );
  SetKeyClassColor( kcLineComment2, clrDefault );
  SetKeyClassColor( kcBlockComment1, clrDefault );
  SetKeyClassColor( kcBlockComment2, clrDefault );
}
CPfxEditTypeInfo::~CPfxEditTypeInfo() {}
CString CPfxEditTypeInfo::GetDelimiters() const {
#ifdef _UNICODE
  return m_arrDelimiters;
#else
  // 在非_UNICODE模式下ANSI字符被扩展成16位存放！现在要将16位的ANSI字
  // 符还原成8位
  // 假设每个单词定界符转换成MBS字符时都需要PFX_EC_MAX_MB_LEN个字节
  char sz[ PFX_EC_SIZEOF( m_arrDelimiters ) * PFX_EC_MAX_MB_LEN ];
  memset( sz, 0, sizeof( sz ) );
  // 统计共有单词定界符数
  LPSTR p = sz;
  for ( int i = 0; i < PFX_EC_SIZEOF( m_arrDelimiters ); i++ ) {
    WCHAR wc = m_arrDelimiters[ i ];
    if ( wc == '\0' ) {
      break;
    }
    // 如果它是ANSI字符则wc的高八位是零
    if ( ( wc & 0xFF00 ) == 0 ) {
      *p = ( char ) wc;
      p++;
    }
    // 否则它是MBCS字符
    else {
      *( LPWSTR ) p = wc;
      p += sizeof( wc );
    }
  }
  return sz;
#endif
}
void CPfxEditTypeInfo::SetDelimiters( LPCTSTR pszDelimiters ) {
  memset( m_arrDelimiters, 0, sizeof( m_arrDelimiters ) );
  if ( pszDelimiters == NULL ) {
    pszDelimiters = GetDelimitersDefault();
  }
#ifdef _UNICODE
  for ( LONG i = 0; i < ( PFX_EC_SIZEOF( m_arrDelimiters ) - 1 ); i++ ) {
    WCHAR wc = *pszDelimiters++;
    if ( wc == '\0' ) {
      break;
    }
    m_arrDelimiters[ i ] = wc;
  }
#else
  for ( LONG i = 0; i < ( PFX_EC_SIZEOF( m_arrDelimiters ) - 1 ); i++ ) {
    TCHAR tc = *pszDelimiters++;
    if ( tc == '\0' ) {
      break;
    }
    WCHAR wc = tc;
    if ( CPfxEditView::IsLeadByte( tc ) && ( *pszDelimiters != '\0' ) ) {
      WCHAR ch = *pszDelimiters++;
      wc |= ( ch << 0x08 );
    }
    m_arrDelimiters[ i ] = wc;
  }
#endif
}
void CPfxEditTypeInfo::SetEscape( LPCTSTR pszEscape ) {
  WCHAR wc = 0;
  if ( pszEscape != NULL ) {
    if ( !CPfxEditView::IsLeadByte( *pszEscape ) ) {
      wc = ( ( WCHAR ) * pszEscape );
    } else {
      wc = ( *( LPCWSTR ) pszEscape );
    }
  }
  m_wEscape = wc;
}
void CPfxEditTypeInfo::SetMarkerPair( EPfxEditMarkerPair id, LPCTSTR pszOn, LPCTSTR pszOff ) {
  if ( !m_arrMarkerPair.IsValidMarker( id ) ) {
    return ;
  }
  m_arrMarkerPair.FreeMarkerChain( m_arrMarkerPair.m_arrMarkers[ id ] );
  m_arrMarkerPair.m_arrMarkers[ id ] = m_arrMarkerPair.CreateMarker( pszOn, pszOff );
}
void CPfxEditTypeInfo::AppendMarkerPair( EPfxEditMarkerPair id, LPCTSTR pszOn, LPCTSTR pszOff ) {
  if ( !m_arrMarkerPair.IsValidMarker( id ) ) {
    return ;
  }
  CPfxEditMarkerPair* pMarkerNew = m_arrMarkerPair.CreateMarker( pszOn, pszOff );
  CPfxEditMarkerPair* pMarkerOld = m_arrMarkerPair.m_arrMarkers[ id ];
  if ( pMarkerOld != NULL ) {
    pMarkerOld->pPrev = pMarkerNew;
  }
  pMarkerNew->pNext = pMarkerOld;
  m_arrMarkerPair.m_arrMarkers[ id ] = pMarkerNew;
}
// CPfxEditPrintInfo...
// C4244: conversion from 'float' to 'long', possible loss of data
#pragma warning (disable: 4244)
CPfxEditPrintInfo::CPfxEditPrintInfo() {
  m_fRatioX = 0;
  m_fRatioY = 0;
}
CPfxEditPrintInfo::~CPfxEditPrintInfo() {}
void CPfxEditPrintInfo::PrinterLogicUnitFromDisplay( CRect& rect ) {
  ASSERT( m_fRatioX != 0 );
  ASSERT( m_fRatioY != 0 );
  rect.left *= m_fRatioX;
  rect.top *= m_fRatioY;
  rect.right *= m_fRatioX;
  rect.bottom *= m_fRatioY;
}
void CPfxEditPrintInfo::PrinterLogicUnitFromDisplay( CPoint& rpt ) {
  ASSERT( m_fRatioX != 0 );
  ASSERT( m_fRatioY != 0 );
  rpt.x *= m_fRatioX;
  rpt.y *= m_fRatioY;
}
BOOL CPfxEditPrintInfo::SetMapDisplayFont() {
  ASSERT( m_pFontDisplay != NULL );
  LOGFONT lf;
  GetLogFont( m_pFontDisplay, &lf );
  lf.lfHeight = PrinterLogicUnitFromDisplayY( lf.lfHeight );
  lf.lfWidth = 0;
  if ( m_fontText != NULL ) {
    m_fontText.DeleteObject();
  }
  return m_fontText.CreateFontIndirect( &lf ) != NULL;
}
BOOL CPfxEditPrintInfo::SetPrinterFont( HFONT pFont ) {
  UNUSED( pFont );
  return false;
}
void CPfxEditPrintInfo::SetDC( HDC hdcDisplay, HDC hdcPrinter ) {
  CDCH pdcDisplay = hdcDisplay;
  CDCH pdcPrinter = hdcPrinter;
  LONG nOldMapMode = pdcPrinter.SetMapMode( MM_TEXT );
  FLOAT fLogPixelsXDisplay = ( FLOAT ) pdcDisplay.GetDeviceCaps( LOGPIXELSX );
  FLOAT fLogPixelsYDisplay = ( FLOAT ) pdcDisplay.GetDeviceCaps( LOGPIXELSY );
  FLOAT fLogPixelsXPrinter = ( FLOAT ) pdcPrinter.GetDeviceCaps( LOGPIXELSX );
  FLOAT fLogPixelsYPrinter = ( FLOAT ) pdcPrinter.GetDeviceCaps( LOGPIXELSY );
  // 还原DC映射模式
  pdcPrinter.SetMapMode( nOldMapMode );
  m_pdcPrinter = pdcPrinter;
  m_pdcDisplay = pdcDisplay;
  // 显示DC中的逻辑单元和打印DC中的逻辑单元比例系数
  m_fRatioX = fLogPixelsXPrinter / fLogPixelsXDisplay;
  m_fRatioY = fLogPixelsYPrinter / fLogPixelsYDisplay;
  // 计算打印DC每水平和垂直英寸所包含的逻辑单元数
  m_fLogUnitsPerInchX = fLogPixelsXPrinter;
  m_fLogUnitsPerInchY = fLogPixelsYPrinter;
  // 计算打印DC每水平和垂直厘米所包含的逻辑单元数,一英寸等于2.54厘米
  m_fLogUnitsPerCmX = m_fLogUnitsPerInchX / 2.54;
  m_fLogUnitsPerCmY = m_fLogUnitsPerInchX / 2.54;
}
// conversion from 'float' to 'long', possible loss of data
#pragma warning (default: 4244) 
// CPfxEditDoc
// 如果某行的选择长度等于该行长度+该常量，则表明该行的选择包括换行符
const LONG PFX_EC_VIRTUAL_LINE_BREAK_SIZE = 1;
const LONG PFX_EC_LINE_LINEINFO_ARRAY_GROWBY = 2048;
const LONG PFX_EC_LINE_MEMBLOCK_ALLOC_LENGTH = 65520;
const LONG PFX_EC_LINE_MEMBLOCK_UNUSED_PERCENT = 10;
const LONG PFX_EC_UNDO_UNDOINFO_ARRAY_MAXLEN = ( ( 512 * 1024 ) / sizeof( CPfxEditUndoInfo ) );
const LONG PFX_EC_UNDO_UNDOINFO_ARRAY_GROWBY = ( PFX_EC_UNDO_UNDOINFO_ARRAY_MAXLEN / 8 );
const LONG PFX_EC_UNDO_MEMBLOCK_ALLOC_LENGTH = 32768;
const LONG PFX_EC_UNDO_MEMBLOCK_UNUSED_PERCENT = 10;
// UNICODE文本文件标识，一般的UNICODE文本文件的开头都存放这个标识
const WORD PFX_EC_UNICODE_FILE_SIGNATURE = 0xFEFF;
const WORD PFX_EC_UNICODE_FILE_BIG_ENDIAN_SIGNATURE = 0xFFFE;
#define PFX_EC_ULE_TO_UBE(c)  (MAKEWORD(HIBYTE(c), LOBYTE(c)))
#define PFX_EC_UBE_TO_ULE(c)  (MAKEWORD(HIBYTE(c), LOBYTE(c))) 
// CPfxEditDocFile
CPfxEditDocFile::CPfxEditDocFile() {
  m_hMapping = NULL;
  m_nMappingSize = 0;
  m_pMappingAddress = NULL;
}
CPfxEditDocFile::~CPfxEditDocFile() {
  Close();
}
BOOL CPfxEditDocFile::MapFile( HANDLE hFile, LONG nSize ) {
  ASSERT( m_hMapping == NULL );
  ASSERT( m_nMappingSize == 0 );
  ASSERT( m_pMappingAddress == NULL );
  ASSERT( nSize > 0 );
  if ( nSize <= 0 ) {
    return false;
  }
  DWORD dwProtect;
  DWORD dwMappingSize;
  DWORD dwDesiredAccess;
  if ( hFile != ( HANDLE ) 0xFFFFFFFF ) {
    dwProtect = PAGE_READONLY | SEC_COMMIT;
    dwMappingSize = nSize;
    dwDesiredAccess = FILE_MAP_READ;
  } else {
    dwProtect = PAGE_READWRITE | SEC_COMMIT;
    dwMappingSize = PFX_EC_ROUND( nSize, 4096 );
    dwDesiredAccess = FILE_MAP_WRITE;
  }
  m_hMapping = ::CreateFileMapping( hFile, NULL, dwProtect, 0, dwMappingSize, NULL );
  if ( m_hMapping == NULL ) {
    AfxThrowMemoryException();
    return false;
  }
  m_pMappingAddress = ::MapViewOfFile( m_hMapping, dwDesiredAccess, 0, 0, dwMappingSize );
  if ( m_pMappingAddress == NULL ) {
    AfxThrowMemoryException();
    return false;
  }
  m_nMappingSize = nSize;
  return true;
}
void CPfxEditDocFile::Close() {
  if ( m_pMappingAddress != NULL ) {
    ::UnmapViewOfFile( m_pMappingAddress );
    m_nMappingSize = 0;
    m_pMappingAddress = NULL;
  }
  if ( m_hMapping != NULL ) {
    ::CloseHandle( m_hMapping );
    m_hMapping = NULL;
  }
}
// 检测给定的缓冲区是否是正确的UTF8编码数据，该函数只确认由UCS2转换来的数据
// 由于ASCII字符也是合法的UTF8字符，所以当缓冲区全部数据都是ASCII字符时函数
// 会反回TRUE，如果使用者认为所有字符都是ASCII字符的情况下不应该是UTF8，那么
// 他自己有责任去检测是否所有的字符都是ASCII这种情况
BOOL CPfxEditDocUtf8Converter::IsUtf8FromUnicode( LPCSTR pUtf8, LONG cch ) const {
  if ( ( pUtf8 == NULL ) || ( cch <= 0 ) ) {
    return false;
  }
  LPCSTR pCur = pUtf8;
  LPCSTR pEnd = pUtf8 + cch;
  while ( pCur < pEnd ) {
    LONG nSize = Utf8CharSize( static_cast<BYTE>( *pCur ) );
    if ( ( nSize == 0 ) || ( ( pCur + nSize ) > pEnd ) ) {
      return false;
    }
    // 确定了UTF8的字符长度以后还要检查后面的字节是否合法
    // 其它字节的合法范围
    // 10xx-xxxx
    // 1000-0000->1011-1111 = 0x80->0xBF
    LPCSTR pNextChar = pCur + nSize;
    while ( ++pCur < pNextChar ) {
      if ( ( ( ( BYTE ) * pCur ) < 0x80 ) || ( ( ( BYTE ) * pCur ) > 0xBF ) ) {
        return false;
      }
    }
  }
  return true;
}
// 将UTF8字符转换为UNICODE字符或者计算需要多大的输出缓冲区
LONG CPfxEditDocUtf8Converter::Utf8ToUnicode( LPCSTR pUtf8, LONG cchUtf8, LPWSTR pUnicode, LONG cchUnicode ) const {
  if ( ( pUtf8 == NULL )
       || ( cchUtf8 <= 0 )
       || ( ( cchUnicode != 0 ) && ( pUnicode == NULL ) ) ) {
    return 0;
  }
  LPWSTR pUnicodeCur = pUnicode;
  LPWSTR pUnicodeEnd = pUnicode + cchUnicode;
  const BYTE* pUtf8Cur = reinterpret_cast<const BYTE*>( pUtf8 );
  const BYTE* pUtf8End = reinterpret_cast<const BYTE*>( pUtf8 ) + cchUtf8;
  LONG nSizeInUnicode = 0;
  // 请求计算需要多大的输出缓冲区
  if ( cchUnicode == 0 ) {
    while ( pUtf8Cur < pUtf8End ) {
      LONG nSize = Utf8CharSize( *pUtf8Cur );
      // 如果遇到无效字符！Utf8CharSize()返回零，这时候强
      // 迫当作一单字符处理
      pUtf8Cur += max( 1, nSize );
      // 如果不是一完整的UTF8字符，就将其拆散
      if ( pUtf8Cur <= pUtf8End ) {
        nSizeInUnicode++;
      } else {
        nSizeInUnicode += ( max( 1, nSize ) );
        nSizeInUnicode -= ( pUtf8Cur - pUtf8End );
      }
    }
    return nSizeInUnicode;
  }
  // 开始执行UTF8转换成UNICODE的操作
  while ( ( pUtf8Cur < pUtf8End ) && ( pUnicodeCur < pUnicodeEnd ) ) {
    BYTE b3;
    BYTE b2;
    BYTE b1 = *pUtf8Cur++;
    nSizeInUnicode++;
    switch ( Utf8CharSize( b1 ) ) {
    default:
    case 0:           //      无效，当一独立ASCII处理
    case 1:           //      U+00000000-U+0000007F: 0xxxxxxx
      // x1111111 = 0x7F
      *pUnicodeCur++ = ( WORD ) b1;
      break;
    case 2:           //      U+00000080-U+000007FF: 110xxxxx 10xxxxxx
      // xxx11111 = 0x1F, xx111111 = 0x3F
      {
        b1 &= 0x1F;
        b2 = 0;
      }
      if ( pUtf8Cur < pUtf8End ) {
        b2 = *pUtf8Cur++;
        b2 &= 0x3F;
      }
      *pUnicodeCur++ = ( WORD ) ( MAKEWORD( ( b2 << 2 ), b1 ) >> 2 );
      break;
    case 3:           //      U+00000800-U+0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
      // xxxx1111 = 0x0F, xx111111 = 0x3F, xx111111 = 0x3F
      {
        b1 &= 0x0F;
        b2 = 0;
        b3 = 0;
      }
      if ( pUtf8Cur < pUtf8End ) {
        b2 = *pUtf8Cur++;
        b2 &= 0x3F;
      }
      if ( pUtf8Cur < pUtf8End ) {
        b3 = *pUtf8Cur++;
        b3 &= 0x3F;
      }
      *pUnicodeCur++ = ( WORD ) ( ( ( ( WORD ) b1 ) << 12 ) | ( ( ( WORD ) b2 ) << 6 ) | ( ( WORD ) b3 ) );
      break;
    }
  }
  return nSizeInUnicode;
}
// 将UNICODE字符转换为UTF8字符或者计算需要多大的输出缓冲区
LONG CPfxEditDocUtf8Converter::UnicodeToUtf8( LPCWSTR pUnicode, LONG cchUnicode, LPSTR pUtf8, LONG cchUtf8 ) const {
  if ( ( pUnicode == NULL )
       || ( cchUnicode <= 0 )
       || ( ( cchUtf8 != 0 ) && ( pUtf8 == NULL ) ) ) {
    return 0;
  }
  LPCWSTR pUnicodeCur = pUnicode;
  LPCWSTR pUnicodeEnd = pUnicode + cchUnicode;
  LPBYTE pUtf8Cur = reinterpret_cast<LPBYTE>( pUtf8 );
  LPBYTE pUtf8End = reinterpret_cast<LPBYTE>( pUtf8 ) + cchUtf8;
  LONG nSizeInUtf8 = 0;
  // 请求计算输出缓冲区的大小
  if ( cchUtf8 == 0 ) {
    while ( pUnicodeCur < pUnicodeEnd ) {
      nSizeInUtf8 += UnicodeToUtf8CharSize( *pUnicodeCur );
      pUnicodeCur++;
    }
    return nSizeInUtf8;
  }
  // 开始执行UNICODE转换成UTF8的操作
  while ( ( pUnicodeCur < pUnicodeEnd ) && ( pUtf8Cur < pUtf8End ) ) {
    WORD w = *pUnicodeCur++;
    switch ( UnicodeToUtf8CharSize( w ) ) {
    default:
    case 1:           //      U+00000000-U+0000007F: 0xxxxxxx
      // 0xxxxxxx = 0x7F
      {
        *pUtf8Cur++ = ( BYTE ) ( LOBYTE( w ) );
        nSizeInUtf8++;
      }
      break;
    case 2:           //      U+00000080-U+000007FF: 110xxxxx 10xxxxxx
      // 0x07FF共11bit，b1取5bit，b2取6bit
      // 110xxxxx = 0xC0, 10xxxxxx = 0x80, xx111111 = 0x3F
      // b1 = 0xC0 | (HIBYTE((w << 2)))
      // b2 = 0x80 | (LOBYTE(w) & 0x03F)
      {
        *pUtf8Cur++ = ( BYTE ) ( 0xC0 | ( HIBYTE( w << 2 ) ) );
        nSizeInUtf8++;
      }
      if ( pUtf8Cur < pUtf8End ) {
        *pUtf8Cur++ = ( BYTE ) ( 0x80 | ( LOBYTE( w ) & 0x03F ) );
        nSizeInUtf8++;
      }
      break;
    case 3:           //      U+00000800-U+0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
      // 0xFFFF共16bit，b1取4bit, b2取6bit, b3取6bit
      // 1110xxxx = 0xE0, 10xxxxxx = 0x80, 10xxxxxx = 0x80
      // b1 = 0xE0 | ((HIBYTE(w) >> 4)
      // b2 = 0x80 | ((w & 0x0FFF) << 2)
      // b3 = 0x80 | (LOBYTE(w) & 0x3F)
      {
        *pUtf8Cur++ = ( BYTE ) ( 0xE0 | ( HIBYTE( w ) >> 4 ) );
        nSizeInUtf8++;
      }
      if ( pUtf8Cur < pUtf8End ) {
        //  0000xxxx-xx000000->00000000-00xxxxxx=shr 6 bit
        *pUtf8Cur++ = ( BYTE ) ( 0x80 | ( ( w & 0x0FFF ) >> 6 ) );
        nSizeInUtf8++;
      }
      if ( pUtf8Cur < pUtf8End ) {
        *pUtf8Cur++ = ( BYTE ) ( 0x80 | ( LOBYTE( w ) & 0x3F ) );
        nSizeInUtf8++;
      }
      break;
    }
  }
  return nSizeInUtf8;
}
CPfxEditDoc::CPfxEditDoc() {
  m_bDrawRuler = false;
  m_bDrawLineNumber = true;
  m_bDrawLineBreak = true;
  m_bDrawWhitespace = true;
  m_bReadOnly = false;
  m_bLockedBufferCompact = false;
  m_bLockedWordWrapEvent = false;
  m_nWrapStyle = PFX_EC_WRAP_NONE;
  m_nWrapWidth = 0;
  m_nLineBreakType = PFX_EC_LINE_BREAK_TYPE_DOS;
  m_nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_DOS;
  m_nOpenConverter = PFX_EC_FILE_CONVERTER_AUTO_DETECT;
  m_nSaveConverter = PFX_EC_FILE_CONVERTER_ANSI;
  m_nBookmarks = 0;
  m_nDocSize = 0;
  m_nDocLineCount = 1;
  m_rectIndent.left = m_rectIndent.top = 0;
  m_rectIndent.right = m_rectIndent.bottom = 0;
  m_clsLine.nli = 0;
  m_clsLine.nliRemain = 0;
  m_clsLine.nliGrowBy = PFX_EC_LINE_LINEINFO_ARRAY_GROWBY;
  m_clsLine.pli = NULL;
  m_clsLine.pmb = NULL;
  m_clsUndo.nui = 0;
  m_clsUndo.nuiRemain = 0;
  m_clsUndo.nuiGrowBy = PFX_EC_UNDO_UNDOINFO_ARRAY_GROWBY;
  m_clsUndo.nuiMax = PFX_EC_UNDO_UNDOINFO_ARRAY_MAXLEN;
  m_clsUndo.nPosCur = 0;
  m_clsUndo.nPosUnmodified = 0;
  m_clsUndo.pui = NULL;
  m_clsUndo.pmb = NULL;
  m_clsUndo.nGroupName = PFX_EC_UNDO_UNKNOWN;
  m_clsUndo.bGroupBegin = false;
  m_clsUndo.nGroupLockCount = 0;
  m_arrViews[ 0 ] = m_arrViews[ 1 ] = NULL;
  OnNewDocument();
}
CPfxEditDoc::~CPfxEditDoc() {
  DeleteContents();
}
// CPfxEditDoc serialization
void CPfxEditDoc::Serialize( CArchive& ar ) {
  if ( ar.IsStoring() ) {
    // 写入文件时不应该使用映射文件机制，因为使用映射文件机制写
    // 入远程文件时不安全，系统无法保证文件与其它程序同步
    switch ( GetFileConverter() ) {
    default:
    case PFX_EC_FILE_CONVERTER_ANSI:
      FileSaveAnsi( ar );
      break;
    case PFX_EC_FILE_CONVERTER_UTF8:
      FileSaveUtf8( ar );
      break;
    case PFX_EC_FILE_CONVERTER_UNICODE:
      FileSaveUnicode( ar );
      break;
    case PFX_EC_FILE_CONVERTER_UNICODE_BIG_ENDIAN:
      FileSaveUnicodeBigEndian( ar );
      break;
    }
  } else {
    CPfxEditDocFile file;
    if ( !file.MapFile( reinterpret_cast<HANDLE>( ar.GetFile() ->m_hFile ), static_cast<LONG>( ar.GetFile() ->GetLength() ) ) ) {
      return ;
    }
    LONG nFileConverter = GetOpenConverter();
    if ( nFileConverter == PFX_EC_FILE_CONVERTER_AUTO_DETECT ) {
      nFileConverter = FileGetConverterType( static_cast<LPCTSTR>( file.GetBuffer() ), file.GetSize() );
    }
    // 保存文件所用的转换器与打开文件所用的转换器一样
    SetFileConverter( nFileConverter );
    ASSERT( nFileConverter != PFX_EC_FILE_CONVERTER_AUTO_DETECT );
    switch ( nFileConverter ) {
    default:
    case PFX_EC_FILE_CONVERTER_ANSI:
      FileLoadAnsi( file );
      break;
    case PFX_EC_FILE_CONVERTER_UTF8:
      FileLoadUtf8( file );
      break;
    case PFX_EC_FILE_CONVERTER_UNICODE:
      FileLoadUnicode( file );
      break;
    case PFX_EC_FILE_CONVERTER_UNICODE_BIG_ENDIAN:
      FileLoadUnicodeBigEndian( file );
      break;
    }
  }
}
// 指示文档是否已经被修改
// BOOL bModified - TRUE等于已修改，FALSE表示文档未被修改
// 从CPfxEditDoc派生的类一定要调用基类的SetModifiedFlag()
void CPfxEditDoc::SetModifiedFlag( BOOL bModified ) {
  if ( !bModified ) {
    UndoSetUnmodified();
  }
  CDocument::SetModifiedFlag( bModified );
}
void CPfxEditDoc::DeleteContents() {
  BufferLineCleanup();
  BufferUndoCleanup();
  m_nBookmarks = 0;
  m_nDocSize = 0;
  m_nDocLineCount = 1;
  m_nLineLastParsed = m_nLineLastParsedButCanceled = -1;
  m_bLockedBufferCompact = false;
  m_bLockedWordWrapEvent = false;
}
void CPfxEditDoc::OnChangedViewList() {
  ASSERT( PFX_EC_SIZEOF( m_arrViews ) == 2 );
  m_arrViews[ 0 ] = m_arrViews[ 1 ] = NULL;
  POSITION pos = GetFirstViewPosition();
  for ( LONG i = 0; ( pos != NULL ) && ( i < PFX_EC_SIZEOF( m_arrViews ) ); ) {
    // 不能认定一定是CPfxEditView，有可能是CPreviewView
    CView* pView = GetNextView( pos );
    // if ( pView->IsKindOf( RUNTIME_CLASS( CPfxEditView ) ) )
    {
      m_arrViews[ i ] = static_cast<CPfxEditView*>( pView );
      i++;
    }
  }
  // 一定不要将它放在前面，因为它可能会“delete this”从而令到上面的代码非法操作
  CDocument::OnChangedViewList();
}
BOOL CPfxEditDoc::OnNewDocument() {
  if ( CDocument::OnNewDocument() ) {
    return OnInitDocument( NULL );
  }
  return false;
}
BOOL CPfxEditDoc::OnOpenDocument( LPCTSTR pszPathName ) {
  if ( CDocument::OnOpenDocument( pszPathName ) ) {
    return OnInitDocument( pszPathName );
  }
  return false;
}
BOOL CPfxEditDoc::OnSaveDocument( LPCTSTR pszPathName ) {
  BOOL bSaveAs = IsReadOnly() || ( GetPathName() != pszPathName );
  BOOL bResult = CDocument::OnSaveDocument( pszPathName );
  if ( bResult && bSaveAs ) {
    bResult = OnInitDocument( pszPathName );
  }
  return bResult;
}
// 在文档被装入或正创建一空文档时用于初始化文档的各种参数，例如根据文档
// 的扩展名来决定使用哪种语法配置方案等
// LPCTSTR pszPathName - 文档文件的完整路径名，如果它等于NULL则表明这是
//                    一个新建的空文档（例如使用“新建”命令）
// 成功返回TRUE，失败返回FALSE
// 派生类一定要调用基类的本函数
BOOL CPfxEditDoc::OnInitDocument( LPCTSTR pszPathName ) {
  UNUSED( pszPathName );
  // 建立文档时必须初始化文本缓冲区，否则会非法操作
  if ( TextGetLineCount() == 0 ) {
    TextInsertLine( 0, NULL, 0, PFX_EC_LF_NONE );
  }
  m_bDrawRuler = true;
  m_bDrawLineNumber = true;
  m_bDrawLineBreak = true;
  m_bDrawWhitespace = true;
  m_nWrapStyle = PFX_EC_WRAP_NONE;
  m_nWrapWidth = 0;
  return true;
}
// 处理自定义事件
// UINT nCode    - 用户自定义消息
// LPARAM lParam - 与该消息相关的参数
// 自定义的消息处理结果
LONG CPfxEditDoc::OnEvent( UINT nCode, LPARAM lParam ) {
  UNUSED( nCode );
  UNUSED( lParam );
  return 0;
}
// 发送事件。该事件将被OnEvent()事件处理器接收
// UINT nCode    - 你自定义的消息
// LPARAM lParam - 与该消息相关的参数
// OnEvent()对该消息的处理结果
LONG CPfxEditDoc::EventSendMsg( UINT nCode, LPARAM lParam ) {
  if ( nCode == PFX_EN_WORD_WRAPPING ) {
    // 只有当文档体积大于64KTCHAR时才发送进度信息
    if ( IsLockedWordWrapEvent()
         || ( GetDocSize() < ( 64 * 1024 ) ) ) {
      return 0;
    }
  }
  return OnEvent( nCode, lParam );
}
// 重新从源文件读取数据
// 成功返回TRUE，失败返回FALSE
BOOL CPfxEditDoc::Reload() {
  if ( !IsCanReload() ) {
    return false;
  }
  BOOL bOldReadOnly = IsReadOnly();
  LONG nOldWrapStyle = GetWrapStyle();
  LONG nOldWrapWidth = GetWrapWidth();
  // 保存各个视图当前的状态以便在执行操作后恢复这些设置
  struct CState {
    CPoint ptCurrent;
    CPfxEditView* pView;
  };
  CState state[ PFX_EC_SIZEOF( m_arrViews ) ];
  for ( LONG i = 0; i < PFX_EC_SIZEOF( state ); i++ ) {
    CState& rState = state[ i ];
    rState.pView = m_arrViews[ i ];
    if ( rState.pView == NULL ) {
      continue;
    }
    rState.ptCurrent = rState.pView->GetCurrentPosition();
    rState.pView->OnInitialize();
  }
  // 尝试重新载入文档数据
  CString strPathName = GetPathName();
  if ( !OnOpenDocument( strPathName ) ) {
    return false;
  }
  // 首先必须重新初始化各个视图
  m_nWrapWidth = 0;
  m_nWrapStyle = PFX_EC_WRAP_NONE;
  SendInitialUpdate();
  // 如果原来是只读，则重载后也应该是只读
  SetReadOnly( IsReadOnly() || bOldReadOnly );
  // 试图恢复到以前的位置，如果不能这样做就将当前位置设置到文档开头
  for ( i = 0; i < PFX_EC_SIZEOF( m_arrViews ); i++ ) {
    CState& rState = state[ i ];
    if ( rState.pView == NULL ) {
      continue;
    }
    CPoint cpNew( 0, 0 );
    if ( GetLineCount() > rState.ptCurrent.y ) {
      cpNew.y = rState.ptCurrent.y;
      cpNew.x = min( rState.ptCurrent.x, GetLineLength( cpNew.y ) );
    }
    rState.pView->SetCurrentPosition( cpNew.y, cpNew.x );
  }
  CPfxEditView* pFirstView = m_arrViews[ 0 ];
  if ( pFirstView != NULL ) {
    pFirstView->SetViewWrapping( nOldWrapStyle, nOldWrapWidth );
  }
  return true;
}
// 更新与本文档关联的所有CPfxEditView
// BOOL bRecalcWrap - 是否重新执行自动换行
void CPfxEditDoc::UpdateAllViews( BOOL bRecalcWrap ) {
  CPfxEditView * pView = m_arrViews[ 0 ];
  if ( pView == NULL ) {
    return ;
  }
  // 强制刷新如果当前自动换行方式是PFX_EC_WRAP_WINDOWEDGE
  if ( !bRecalcWrap || !IsWrapAtWindowEdge() ) {
    pView->UpdateAllViews( PFX_EC_UT_ALL, 0, 0 );
  } else {
    pView->UpdateLayout();
    // CPfxEditView::SetViewWrapping()会判断文本区的宽度是否已经
    // 改变，如果没有改变就不执行换行，所以要将自动换行风格设置
    // 成PFX_EC_WRAP_NONE然后再让它执行换行操作
    m_nWrapStyle = PFX_EC_WRAP_NONE;
    m_nWrapWidth = 0;
    pView->SetViewWrapping( PFX_EC_WRAP_WINDOWEDGE, 0 );
  }
}
BOOL CPfxEditDoc::IsValidLine( LONG nLine ) const {
  return ( nLine >= 0 ) && ( nLine < m_nDocLineCount );
}
BOOL CPfxEditDoc::IsValidChar( LONG nLine, LONG nChar ) const {
  CPoint pt( nChar, nLine );
  if ( ( pt.y >= 0 ) && ( pt.y < GetLineCount() ) && ( pt.x >= 0 ) ) {
    TextAbsLineToMainLine( pt );
    if ( pt.x <= TextGetLineAt( pt.y ) ->nActLen ) {
      return true;
    }
  }
  return false;
}
LONG CPfxEditDoc::GetValidLine( LONG nLine ) const {
  if ( nLine >= GetLineCount() ) {
    nLine = GetLineUpperBound();
  }
  return max( 0, nLine );
}
LONG CPfxEditDoc::GetValidChar( LONG nLine, LONG nChar ) const {
  ASSERT( ( nLine >= 0 ) && ( nLine < GetLineCount() ) );
  CPoint pt( 0, GetValidLine( nLine ) );
  TextAbsLineToMainLine( pt );
  LONG cch = TextGetLineAt( pt.y ) ->nActLen;
  if ( nChar > cch ) {
    nChar = cch;
  }
  return max( 0, nChar );
}
// 获取当前正在使用的CPfxEditTypeInfo对象的指针。如果你从来没有设置过该
// 对象（使用SetTypeInfoPtr()），则返回一内部默认的对象的指针
// 如果当前没有视图就返回NULL，否则返回一对象指针
const CPfxEditTypeInfo* CPfxEditDoc::GetTypeInfoPtr() const {
  return ( ( m_arrViews[ 0 ] != NULL ) ? m_arrViews[ 0 ] ->m_pti : NULL );
}
// BOOL SetTypeInfoPtr(const CPfxEditTypeInfo* pTypeInfo)
// 设置文档工作参数。例如设置语法高亮：
#if 0
void setSyntaxHilight() {
  static CPfxEditTypeInfo ti;
  static const LPCTSTR arrKeywords[] = {
                                         _T( "int" ), _T( "float" ), _T( "char" ), _T( "unsigned" ), _T( "do" ), _T( "while" ), _T( "for" ), _T( "inline" ), _T( "const" ),
                                       }
                                       LONG nClass = ti.SetKeyClass( _T( "保留字" ), RGB( 0, 0, 255 ) );
  for ( int i = 0; i < PFX_EC_SIZEOF( arrKeywords ); i++ ) {
    ti.SetKeyword( arrKeywords[ i ], nClass );
  }
  static const LPCTSTR arrOperators[] = {
                                          _T( "+" ), _T( "-" ), _T( "*" ), _T( "/" ), _T( ">>" ), _T( "<<" ), _T( "->" ), _T( "||" ), _T( "&&" ), _T( "!" ),
                                        }
                                        nClass = ti.SetKeyClass( _T( "操作符" ), RGB( 0, 255, 0 ) );
  for ( int i = 0; i < PFX_EC_SIZEOF( arrOperators ); i++ ) {
    ti.SetKeyword( arrOperators[ i ], nClass );
  }
  SetTypeInfoPtr( &ti );
}
#endif 
// const CPfxEditTypeInfo* pTypeInfo - CPfxEditTypeInfo指针。该指针必须
//      指向一恒定的对象，即是说一但调用本函数，该指针所指向的对象就
//      必须一直存在，直到所有使用该指针的 CPfxEditDoc被销毁。因此不
//      能在堆栈构建 CPfxEditTypeInfo对象，而应该使用new运算符创建或
//      使用静态对象。如果你使用 new运算符创建该对象，那么请记住在程
//      序结束时用delete删除该对象，CPfxEditDoc不会自动为你删除它
//      正常情况下，你可以使用一CList来存放这些对象集，因为你的语法
//      编辑器可能同时支持很多种语言的编辑。例如可以这样定义：
//      typedef CList<CPfxEditTypeInfo, CPfxEditTypeInfo&>CListTypeInfo;
BOOL CPfxEditDoc::SetTypeInfoPtr( CPfxEditTypeInfo* pTypeInfo ) {
  if ( pTypeInfo == NULL ) {
    pTypeInfo = &PFX_EC_DATA.m_defTypeInfo;
  }
  for ( LONG i = 0; i < PFX_EC_SIZEOF( m_arrViews ); i++ ) {
    if ( m_arrViews[ i ] != NULL ) {
      m_arrViews[ i ] ->m_pti = pTypeInfo;
    }
  }
  m_bDrawRuler = pTypeInfo->m_bDrawRuler;
  m_bDrawLineNumber = pTypeInfo->m_bDrawLineNumber;
  m_bDrawLineBreak = pTypeInfo->m_bDrawLineBreak;
  m_bDrawWhitespace = pTypeInfo->m_bDrawWhitespace;
  CPfxEditView* pView = m_arrViews[ 0 ];
  if ( pView != NULL ) {
    pView->UpdateAllViews( PFX_EC_UT_ALL, 0, 0 );
    // 新的自动换行设置只会影响当前已经处于自动换行的文档，如果
    // 某文档当前不是自动换行状态，它将会忽略新的自动换行指示
    if ( IsWrapping() ) {
      pView->SetViewWrapping(
        pTypeInfo->m_nWrapStyle, pTypeInfo->m_nWrapWidth );
    }
  }
  return true;
}
// 获取UNDO名称
LONG CPfxEditDoc::GetUndoName() const {
  const CPfxEditUndoInfo * pui = UndoGetInfo();
  return ( ( pui != NULL ) ? pui->fUndoName : PFX_EC_UNDO_UNKNOWN );
}
// 获取REDO名称
LONG CPfxEditDoc::GetRedoName() const {
  const CPfxEditUndoInfo * pui = UndoGetRedoInfo();
  return ( ( pui != NULL ) ? pui->fUndoName : PFX_EC_UNDO_UNKNOWN );
}
void CPfxEditDoc::SetLineBreakType( LONG nType ) {
  ASSERT( ( nType == PFX_EC_LINE_BREAK_TYPE_DOS ) || ( nType == PFX_EC_LINE_BREAK_TYPE_UNIX ) || ( nType == PFX_EC_LINE_BREAK_TYPE_MAC ) );
  switch ( nType ) {
  case PFX_EC_LINE_BREAK_TYPE_DOS:
  case PFX_EC_LINE_BREAK_TYPE_UNIX:
  case PFX_EC_LINE_BREAK_TYPE_MAC:
    break;
  default:
    nType = PFX_EC_LINE_BREAK_TYPE_DOS;
    break;
  }
  if ( GetLineBreakType() == nType )           //      没有改变，不必更新
  {
    return ;
  }
  // 更新文档大小信息
  LONG nDocSize = 0;
  LONG nLineBreakSize = GetLineBreakSizeFromType( nType );
  for ( LONG i = 0, nCount = TextGetLineCount(); i < nCount; i++ ) {
    const CPfxEditLineInfo* pli = TextGetLineAt( i );
    if ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      nDocSize += ( pli->nLen + nLineBreakSize );
    } else {
      nDocSize += pli->nLen;
    }
  }
  // 更新选择文本大小
  for ( i = 0; i < PFX_EC_SIZEOF( m_arrViews ); i++ ) {
    CPfxEditView* pView = m_arrViews[ i ];
    if ( ( pView != NULL ) && pView->IsTextSelected() ) {
      pView->m_nSelSize = pView->GetSelSize( nType, false );
    }
  }
  m_nDocSize = nDocSize;
  m_nLineBreakType = nType;
  m_nLineBreakSize = GetLineBreakSizeFromType( nType );
}
// 获取指定行的长度
// LONG nLine - 以零为基值的行号
// 返回行的长度，如果行号无效返回-1，否则返回实际长度，单位：TCHAR
LONG CPfxEditDoc::GetLineLength( LONG nLine ) const {
  LONG nLength = -1;
  if ( IsValidLine( nLine ) ) {
    CPoint pt( 0, nLine );
    TextAbsLineToMainLine( pt );
    nLength = TextGetLineAt( pt.y ) ->nActLen;
  }
  return nLength;
}
// 获取某行数据
// LONG nLine     - 行号，以零为基值
// LPTSTR pBuffer - 接收数据的缓冲区
// LONG nBufMax   - 缓冲区包括NULL字符的长度
// 返回实际复制的字符数，不包括NULL字符，单位：TCHAR
LONG CPfxEditDoc::GetLineData( LONG nLine, LPTSTR pBuffer, LONG nBufMax ) const {
  ASSERT( AfxIsValidAddress( pBuffer, ( nBufMax * sizeof( TCHAR ) ), true ) );
  if ( ( pBuffer == NULL ) || ( nBufMax <= 0 ) ) {
    return 0;
  }
  LONG cch = 0;
  if ( IsValidLine( nLine ) ) {
    CPfxEditView * pView = m_arrViews[ 0 ];
    ASSERT( pView != NULL );
    CPoint pt( 0, nLine );
    TextAbsLineToMainLine( pt );
    const CPfxEditLineInfo* pli = TextGetLineAt( pt.y );
    cch = pli->nActLen;
    // 为NULL字符保留
    nBufMax--;
    if ( nBufMax < cch ) {
      cch = pView->CalcCharsToCopy( pli->pData, nBufMax );
    }
    if ( cch > 0 ) {
      memcpy( pBuffer, pli->pData, ( cch * sizeof( TCHAR ) ) );
      pBuffer += cch;
    }
  }
  *pBuffer = '\0';
  return cch;
}
LPCSTR CPfxEditDoc::GetLineBreakDataFromTypeA( LONG nType ) const {
  ASSERT( ( nType == PFX_EC_LINE_BREAK_TYPE_DOS )
          || ( nType == PFX_EC_LINE_BREAK_TYPE_UNIX )
          || ( nType == PFX_EC_LINE_BREAK_TYPE_MAC ) );
  // 换行符类型
  static const CHAR szLineBreakDos[] = "\r\n"; // Dos, Windows
  static const CHAR szLineBreakMac[] = "\r"; // Macintosh
  static const CHAR szLineBreakUnix[] = "\n"; // Unix
  LPCSTR pszLineBreakData;
  switch ( nType ) {
  default:           // 贯通
  case PFX_EC_LINE_BREAK_TYPE_DOS:
    pszLineBreakData = szLineBreakDos;
    break;
  case PFX_EC_LINE_BREAK_TYPE_UNIX:
    pszLineBreakData = szLineBreakUnix;
    break;
  case PFX_EC_LINE_BREAK_TYPE_MAC:
    pszLineBreakData = szLineBreakMac;
    break;
  }
  return pszLineBreakData;
}
LONG CPfxEditDoc::GetLineBreakSizeFromTypeA( LONG nType ) const {
  ASSERT( ( nType == PFX_EC_LINE_BREAK_TYPE_DOS )
          || ( nType == PFX_EC_LINE_BREAK_TYPE_UNIX )
          || ( nType == PFX_EC_LINE_BREAK_TYPE_MAC ) );
  LONG nLineBreakSize;
  switch ( nType ) {
  default:           // 贯通
  case PFX_EC_LINE_BREAK_TYPE_DOS:
    nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_DOS;
    break;
  case PFX_EC_LINE_BREAK_TYPE_UNIX:
    nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_UNIX;
    break;
  case PFX_EC_LINE_BREAK_TYPE_MAC:
    nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_MAC;
    break;
  }
  return nLineBreakSize;
}
LPCWSTR CPfxEditDoc::GetLineBreakDataFromTypeW( LONG nType ) const {
  ASSERT( ( nType == PFX_EC_LINE_BREAK_TYPE_DOS )
          || ( nType == PFX_EC_LINE_BREAK_TYPE_UNIX )
          || ( nType == PFX_EC_LINE_BREAK_TYPE_MAC ) );
  // 换行符类型
  static const WCHAR szLineBreakDos[] = L"\r\n"; // Dos, Windows
  static const WCHAR szLineBreakMac[] = L"\r"; // Macintosh
  static const WCHAR szLineBreakUnix[] = L"\n"; // Unix
  LPCWSTR pszLineBreakData;
  switch ( nType ) {
  default:           // 贯通
  case PFX_EC_LINE_BREAK_TYPE_DOS:
    pszLineBreakData = szLineBreakDos;
    break;
  case PFX_EC_LINE_BREAK_TYPE_UNIX:
    pszLineBreakData = szLineBreakUnix;
    break;
  case PFX_EC_LINE_BREAK_TYPE_MAC:
    pszLineBreakData = szLineBreakMac;
    break;
  }
  return pszLineBreakData;
}
LONG CPfxEditDoc::GetLineBreakSizeFromTypeW( LONG nType ) const {
  ASSERT( ( nType == PFX_EC_LINE_BREAK_TYPE_DOS )
          || ( nType == PFX_EC_LINE_BREAK_TYPE_UNIX )
          || ( nType == PFX_EC_LINE_BREAK_TYPE_MAC ) );
  LONG nLineBreakSize;
  switch ( nType ) {
  default:           // 贯通
  case PFX_EC_LINE_BREAK_TYPE_DOS:
    nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_DOS;
    break;
  case PFX_EC_LINE_BREAK_TYPE_UNIX:
    nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_UNIX;
    break;
  case PFX_EC_LINE_BREAK_TYPE_MAC:
    nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_MAC;
    break;
  }
  return nLineBreakSize;
}
// 获取需要指定长度的缓冲区
void CPfxEditDoc::BufferLineGetSpace( LONG nLen, TCHAR** ppBuffer, CPfxEditDocMemBlock** ppMemBlock ) {
  ASSERT( nLen > 0 );
  // 搜索已有的内存块，看看有没有可用的空间，如果有就在可用的空间内分
  // 配，否则要申请新的内存块
  for ( CPfxEditDocMemBlock * pMemBlock = m_clsLine.pmb; pMemBlock != NULL; pMemBlock = pMemBlock->pNext ) {
    if ( pMemBlock->nRemain >= nLen ) {
      break;
    }
  }
  // 没有可用的空间满足要求，要申请一新的内存块
  if ( pMemBlock == NULL ) {
    pMemBlock = BufferLineAllocBlock( nLen );
  }
  *ppBuffer = pMemBlock->pRemain;
  *ppMemBlock = pMemBlock;
  pMemBlock->nRemain -= nLen;
  pMemBlock->pRemain += nLen;
  ASSERT( pMemBlock->nRemain >= 0 );
}
// 对某个内存块执行垃圾回收
void CPfxEditDoc::BufferLineCompact( CPfxEditDocMemBlock* pCurMemBlock, LONG nNewUnused ) {
  ASSERT( pCurMemBlock != NULL );
  ASSERT( nNewUnused >= 0 );
  // 如果该内存块已经弃置无法使用的空间总和达到某个比例，就要重新调整
  // 该内存块。方法是重新申请另外一块，然后把旧块的数据搬到新块去，并
  // 重新调整其他与旧块相关的已经分配出去的缓冲区的信息
  pCurMemBlock->nUnused += nNewUnused;
  LONG nActualUsed = pCurMemBlock->nLength - ( pCurMemBlock->nUnused + pCurMemBlock->nRemain );
  if ( ( pCurMemBlock->nUnused < pCurMemBlock->nMaxUnused ) && ( nActualUsed > 0 ) ) {
    return ;
  }
  ASSERT( ( nActualUsed >= 0 ) && ( nActualUsed <= pCurMemBlock->nLength ) );
  TRACE( "CPfxEditDoc::BufferLineCompact() Begin dump of mem block 0x%X:\n", pCurMemBlock );
  TRACE( "\tOld block size %d TCHAR(s)\n", pCurMemBlock->nLength );
  TRACE( "\tOld block unused size %d TCHAR(s)\n", pCurMemBlock->nUnused );
  TRACE( "\tOld block remain size %d TCHAR(s)\n", pCurMemBlock->nRemain );
  // 如果已经弃置的空间加上剩余的空间等于该内存块数据区的总大小，则表
  // 明已经没有人使用这个内存块，可以释放掉，并且不需要重新申请空间
  if ( nActualUsed > 0 ) {
    if ( m_bLockedBufferCompact ) {
      return ;
    }
    CPfxEditDocMemBlock* pNewMemBlock = BufferLineAllocBlock( nActualUsed );
    // 扫描整个行信息链数组，凡是与旧块关联的行都被重新调整其缓
    // 冲区指针，使之指向新块
    LONG nRemain = pNewMemBlock->nRemain;
    LPTSTR pRemain = pNewMemBlock->pRemain;
    CPfxEditLineInfo* pli = m_clsLine.pli;
    CPfxEditLineInfo* pliEnd = m_clsLine.pli + m_clsLine.nli;
    for ( ; pli < pliEnd; pli++ ) {
      if ( pCurMemBlock != pli->pMemBlock ) {
        continue;
      }
      ASSERT( pli->nLen > 0 );
      ASSERT( pli->pData != NULL );
      ASSERT( !PFX_EC_IS_SUBLINE( pli ) );
      LONG cch = pli->nActLen;
      LPTSTR pData = pRemain;
      memcpy( pData, pli->pData, cch * sizeof( TCHAR ) );
      pli->pData = pData;
      pli->pMemBlock = pNewMemBlock;
      // 跳过子行，最后一行也是没有PFX_EC_LF_HARDRETURN
      // 标志的，所以必须用(pli < pliEnd)来限制
      if ( !PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
        pData += pli->nLen;
        while ( ( ++pli ) < pliEnd ) {
          ASSERT( PFX_EC_IS_SUBLINE( pli ) );
          ASSERT( pli->nLen > 0 );
          ASSERT( pli->pData != NULL );
          ASSERT( pli->pMemBlock == NULL );
          pli->pData = pData;
          if ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
            break;
          }
          pData += pli->nLen;
        }
      }
      nRemain -= cch;
      pRemain += cch;
    }
    pNewMemBlock->nRemain = nRemain;
    pNewMemBlock->pRemain = pRemain;
    ASSERT( nRemain >= 0 );
    TRACE( "\tNew block handle 0x%X\n", pNewMemBlock );
    TRACE( "\tNew block size %d\n", pNewMemBlock->nLength );
    TRACE( "\tNew block remain size %d\n", pNewMemBlock->nRemain );
  }
  // 删除旧的内存块
  BufferLineRemoveBlock( pCurMemBlock );
  TRACE( "Deleting block 0x%X\n", pCurMemBlock );
  TRACE( "Line memory block chain: %X\n", m_clsLine.pmb );
  TRACE( "End dump CPfxEditDoc::BufferLineCompact()\n" );
}
CPfxEditDocMemBlock* CPfxEditDoc::BufferLineAllocBlock( LONG nBlockSize ) {
  ASSERT( nBlockSize > 0 );
  // 申请一新的内存块，新的内存块结构如下：
  // |=========================|
  // | CPfxEditDocMemBlock结构 | （用来跟踪该内存块的使用情况）
  // |-------------------------|
  // |       数据区            |
  // |=========================|
  // 附加的结构大小
  LONG nExtraSize = sizeof( CPfxEditDocMemBlock );
  // 整个内存块的大小是（附加的结构大小 + 数据区大小）
  LONG nAllocSize = PFX_EC_ROUND( ( nBlockSize + nExtraSize ), PFX_EC_LINE_MEMBLOCK_ALLOC_LENGTH );
  // 整个内存块的大小完整到PFX_EC_LINE_MEMBLOCK_ALLOC_LENGTH的倍数，因此数据
  // 区的大小是（整个内存块的大小 - 附加的结构大小）
  CPfxEditDocMemBlock* pMemBlock = ( CPfxEditDocMemBlock* ) new TCHAR[ nAllocSize ];
  memset( pMemBlock, 0, sizeof( TCHAR ) * nAllocSize );
  // 保存新的内存块指针，总是放在链头，因为新的缓冲块通常有很多可用空
  // 间，这样可优化查找
  pMemBlock->pNext = m_clsLine.pmb;
  pMemBlock->pPrev = NULL;
  if ( m_clsLine.pmb != NULL ) {
    m_clsLine.pmb->pPrev = pMemBlock;
  }
  m_clsLine.pmb = pMemBlock;
  // 设置该新内存块的信息
  pMemBlock->nLength = nAllocSize - nExtraSize;
  pMemBlock->nRemain = pMemBlock->nLength;
  pMemBlock->pRemain = ( LPTSTR ) ( ( LPTSTR ) pMemBlock + nExtraSize );
  pMemBlock->nUnused = 0;
  pMemBlock->nMaxUnused = pMemBlock->nLength / PFX_EC_LINE_MEMBLOCK_UNUSED_PERCENT;
  return pMemBlock;
}
void CPfxEditDoc::BufferLineRemoveBlock( CPfxEditDocMemBlock* pMemBlock ) {
  ASSERT( pMemBlock != NULL );
  ASSERT( AfxIsValidAddress( pMemBlock, sizeof( CPfxEditDocMemBlock ) ) );
  if ( pMemBlock == NULL ) {
    return ;
  }
  CPfxEditDocMemBlock* pNext = pMemBlock->pNext;
  CPfxEditDocMemBlock* pPrev = pMemBlock->pPrev;
  if ( pMemBlock == m_clsLine.pmb ) {
    m_clsLine.pmb = pNext;
    if ( pNext != NULL ) {
      pNext->pPrev = NULL;
    }
  } else {
    pPrev->pNext = pNext;
    if ( pNext != NULL ) {
      pNext->pPrev = pPrev;
    }
  }
  delete pMemBlock;
}
// 释放所有行信息资源
void CPfxEditDoc::BufferLineCleanup() {
  // 清除行缓冲区
  if ( m_clsLine.pli != NULL ) {
    delete[] m_clsLine.pli;
  }
  CPfxEditDocMemBlock* pNodeHead = m_clsLine.pmb;
  while ( pNodeHead != NULL ) {
    CPfxEditDocMemBlock * pNodeTemp = pNodeHead;
    pNodeHead = pNodeHead->pNext;
    delete pNodeTemp;
  }
  m_clsLine.pli = NULL;
  m_clsLine.nli = m_clsLine.nliRemain = 0;
  m_clsLine.pmb = NULL;
}
void CPfxEditDoc::BufferUndoGetSpace( LONG nLen, TCHAR** ppBuffer, CPfxEditDocMemBlock** ppMemBlock ) {
  ASSERT( nLen > 0 );
  // 搜索已有的内存块，看看有没有可用的空间，如果有就在可用的空间内分
  // 配，否则要申请新的内存块
  CPfxEditDocMemBlock* pMemBlock = m_clsUndo.pmb;
  for ( ; pMemBlock != NULL; pMemBlock = pMemBlock->pNext ) {
    if ( pMemBlock->nRemain >= nLen ) {
      break;
    }
  }
  // 没有可用的空间满足要求，要申请一新的内存块
  if ( pMemBlock == NULL ) {
    pMemBlock = BufferUndoAllocBlock( nLen );
  }
  *ppBuffer = pMemBlock->pRemain;
  *ppMemBlock = pMemBlock;
  pMemBlock->nRemain -= nLen;
  pMemBlock->pRemain += nLen;
  ASSERT( pMemBlock->nRemain >= 0 );
}
// 压缩指定的UNDO内存块，重新调整资源以避免浪费内存空间
void CPfxEditDoc::BufferUndoCompact( CPfxEditDocMemBlock* pCurMemBlock, LONG nNewUnused ) {
  ASSERT( nNewUnused >= 0 );
  ASSERT( pCurMemBlock != NULL );
  // 如果该内存块已经弃置无法使用的空间总和达到某个比例，就要重新调整
  // 该内存块。方法是重新申请另外一块，然后把旧块的数据搬到新块去，并
  // 重新调整其他与旧块相关的已经分配出去的缓冲区的信息
  pCurMemBlock->nUnused += nNewUnused;
  LONG nActualUsed = pCurMemBlock->nLength - ( pCurMemBlock->nUnused + pCurMemBlock->nRemain );
  if ( ( pCurMemBlock->nUnused < pCurMemBlock->nMaxUnused ) && ( nActualUsed > 0 ) ) {
    return ;
  }
  ASSERT( ( nActualUsed >= 0 ) && ( nActualUsed <= pCurMemBlock->nLength ) );
  TRACE( "\n\n\n" );
  TRACE( "CPfxEditDoc::BufferUndoCompact() Begin dump of mem block 0x%X:\n", pCurMemBlock );
  TRACE( "\tOld block size %d TCHAR(s)\n", pCurMemBlock->nLength );
  TRACE( "\tOld block unused size %d TCHAR(s)\n", pCurMemBlock->nUnused );
  TRACE( "\tOld block remain size %d TCHAR(s)\n", pCurMemBlock->nRemain );
  // 如果已经弃置的空间加上剩余的空间等于该内存块数据区的总大小，则表
  // 明已经没有人使用这个内存块，可以释放掉，并且不需要重新申请空间
  if ( nActualUsed > 0 ) {
    if ( m_bLockedBufferCompact ) {
      return ;
    }
    CPfxEditDocMemBlock* pNewMemBlock = BufferUndoAllocBlock( nActualUsed );
    // 扫描整个UNDO信息数组，凡是与旧块关联的缓冲区都被调整到新
    // 块中去
    LONG nRemain = pNewMemBlock->nRemain;
    LPTSTR pRemain = pNewMemBlock->pRemain;
    CPfxEditUndoInfo* pui = m_clsUndo.pui;
    CPfxEditUndoInfo* puiEnd = m_clsUndo.pui + m_clsUndo.nui;
    for ( ; pui < puiEnd; pui++ ) {
      if ( pCurMemBlock == pui->pMemBlock ) {
        ASSERT( pui->nLen > 0 );
        ASSERT( pui->pData != NULL );
        LONG cch = pui->nLen;
        LPTSTR pData = pRemain;
        memcpy( pData, pui->pData, ( cch * sizeof( TCHAR ) ) );
        pui->pData = pData;
        pui->pMemBlock = pNewMemBlock;
        nRemain -= cch;
        pRemain += cch;
      }
    }
    ASSERT( nRemain >= 0 );
    pNewMemBlock->nRemain = nRemain;
    pNewMemBlock->pRemain = pRemain;
    TRACE( "\tNew block handle 0x%X\n", pNewMemBlock );
    TRACE( "\tNew block size %d\n", pNewMemBlock->nLength );
    TRACE( "\tNew block remain size %d\n", pNewMemBlock->nRemain );
  }
  // 删除旧的内存块
  BufferUndoRemoveBlock( pCurMemBlock );
  TRACE( "Deleting block 0x%X\n", pCurMemBlock );
  TRACE( "Undo memory block chain: %X\n", m_clsUndo.pmb );
  TRACE( "End dump of CPfxEditDoc::BufferUndoCompact()\n" );
}
// 申请一UNDO内存块
CPfxEditDocMemBlock* CPfxEditDoc::BufferUndoAllocBlock( LONG nBlockSize ) {
  ASSERT( nBlockSize > 0 );
  // 申请一新的内存块，新的内存块结构如下：
  // |=========================|
  // | CPfxEditDocMemBlock结构 | （这个结构用来跟踪该内存块的使用情况）
  // |-------------------------|
  // |       数据区            |
  // |=========================|
  // 附加的结构大小
  LONG nExtraSize = sizeof( CPfxEditDocMemBlock );
  // 整个内存块的大小是（附加的结构大小 + 数据区大小）
  LONG nAllocSize = PFX_EC_ROUND( ( nBlockSize + nExtraSize ), PFX_EC_UNDO_MEMBLOCK_ALLOC_LENGTH );
  // 整个内存块的大小完整到PFX_EC_UNDO_MEMBLOCK_ALLOC_LENGTH的倍数，因此
  // 数据区的大小是（整个内存块的大小 - 附加的结构大小）
  CPfxEditDocMemBlock* pMemBlock = ( CPfxEditDocMemBlock* ) new TCHAR[ nAllocSize ];
  // 保存新的内存块指针，总是放在链头，因为新的缓冲块通常有很多可用空
  // 间，这样可优化查找
  pMemBlock->pNext = m_clsUndo.pmb;
  pMemBlock->pPrev = NULL;
  if ( m_clsUndo.pmb != NULL ) {
    m_clsUndo.pmb->pPrev = pMemBlock;
  }
  m_clsUndo.pmb = pMemBlock;
  // 设置该新内存块的信息
  pMemBlock->nLength = nAllocSize - nExtraSize;
  pMemBlock->nRemain = pMemBlock->nLength;
  pMemBlock->pRemain = ( LPTSTR ) ( ( LPTSTR ) pMemBlock + nExtraSize );
  pMemBlock->nUnused = 0;
  pMemBlock->nMaxUnused = pMemBlock->nLength / PFX_EC_UNDO_MEMBLOCK_UNUSED_PERCENT;
  return pMemBlock;
}
// 删除一UNDO内存块
void CPfxEditDoc::BufferUndoRemoveBlock( CPfxEditDocMemBlock* pMemBlock ) {
  ASSERT( pMemBlock != NULL );
  ASSERT( AfxIsValidAddress( pMemBlock, sizeof( CPfxEditDocMemBlock ) ) );
  if ( pMemBlock == NULL ) {
    return ;
  }
  CPfxEditDocMemBlock* pNext = pMemBlock->pNext;
  CPfxEditDocMemBlock* pPrev = pMemBlock->pPrev;
  if ( pMemBlock == m_clsUndo.pmb ) {
    m_clsUndo.pmb = pNext;
    if ( m_clsUndo.pmb != NULL ) {
      m_clsUndo.pmb->pPrev = NULL;
    }
  } else {
    pPrev->pNext = pNext;
    if ( pNext != NULL ) {
      pNext->pPrev = pPrev;
    }
  }
  delete pMemBlock;
}
// 释放所有UNDO，REDO资源
void CPfxEditDoc::BufferUndoCleanup() {
  // 清除UNDO缓冲区
  if ( m_clsUndo.pui != NULL ) {
    delete[] m_clsUndo.pui;
  }
  CPfxEditDocMemBlock* pNodeHead = m_clsUndo.pmb;
  while ( pNodeHead != NULL ) {
    CPfxEditDocMemBlock * pNodeTemp = pNodeHead;
    pNodeHead = pNodeHead->pNext;
    delete pNodeTemp;
  }
  m_clsUndo.pui = NULL;
  m_clsUndo.nui = m_clsUndo.nuiRemain = 0;
  m_clsUndo.pmb = NULL;
  m_clsUndo.nPosCur = m_clsUndo.nPosUnmodified = 0;
  m_clsUndo.nGroupName = PFX_EC_UNDO_UNKNOWN;
  m_clsUndo.bGroupBegin = false;
  m_clsUndo.nGroupLockCount = 0;
}
// 检查一给定行序号是否合法，如果不合法就调整成合法
LONG CPfxEditDoc::TextVerifyLine( LONG nLine ) const {
  if ( nLine >= TextGetLineCount() ) {
    nLine = TextGetLineUpperBound();
  }
  return max( 0, nLine );
}
// 检查某行中所指定的列位置是否合法，如果不合法则调整成合法
LONG CPfxEditDoc::TextVerifyChar( LONG nLine, LONG nChar ) const {
  ASSERT( TextIsValidLine( nLine ) );
  if ( nChar > TextGetLineAt( nLine ) ->nLen ) {
    nChar = TextGetLineAt( nLine ) ->nLen;
  }
  return max( 0, nChar );
}
// 在指定的行前插入指定数目的新行，所有新行被初始化成空行
// 第一行的指针
// 本函数会影响行信息链表，因此如果之前使用 TextGetLineAt()获得某些指针
// 而且这些指针还在使用的话，应该在调用 TextInsertLine()后再次使用
// LineGetAt()刷新那些指针，否则可能会发生意想不到的结果
CPfxEditLineInfo* CPfxEditDoc::TextInsertLine( LONG nIndex, LONG nCount ) {
  ASSERT( nIndex >= 0 && nIndex <= TextGetLineCount() );
  ASSERT( nCount > 0 );
  // 如果数组里的空单元不足就要增加数组的尺寸，数组的总长度为已用单元
  // ＋剩余单元。本数组总是“充实的”一维线性数组，由第一个有效单元到
  // 最后单元中间没有空的无效单元
  if ( m_clsLine.nliRemain < nCount ) {
    LONG nNewSize = ( m_clsLine.nli + nCount + PFX_EC_LINE_LINEINFO_ARRAY_GROWBY );
    CPfxEditLineInfo* pli = new CPfxEditLineInfo[ nNewSize ];
    if ( m_clsLine.pli != NULL ) {
      memcpy( pli, m_clsLine.pli, m_clsLine.nli * sizeof( CPfxEditLineInfo ) );
      delete[] m_clsLine.pli;
    }
    m_clsLine.pli = pli;
    m_clsLine.nliRemain = nNewSize - m_clsLine.nli;
  }
  CPfxEditLineInfo* pli = m_clsLine.pli + nIndex;
  LONG nli = m_clsLine.nli - nIndex;
  if ( nli > 0 ) {
    memmove( pli + nCount, pli, nli * sizeof( CPfxEditLineInfo ) );
  }
  m_clsLine.nli += nCount;
  m_clsLine.nliRemain -= nCount;
  return pli;
}
// 删除从指定位置开始指定数目的行
// 调用本函数后将会影响行信息链表，因此凡是之前使用 TextGetLineAt()或得
// 的且当前还在使用的指针都应该再次调用LineGetAt()刷新
void CPfxEditDoc::TextRemoveLine( LONG nIndex, LONG nCount ) {
  ASSERT( nIndex >= 0 && ( nIndex + nCount ) <= TextGetLineCount() );
  ASSERT( nCount > 0 );
  LONG nli = m_clsLine.nli - ( nIndex + nCount );
  CPfxEditLineInfo* pli = m_clsLine.pli + nIndex;
  if ( nli > 0 ) {
    memmove( pli, pli + nCount, nli * sizeof( CPfxEditLineInfo ) );
  }
  // 链表里最后一个有效单元是靠指针来指示，因此该指针后面的空单元是否
  // 为零并不重要
  m_clsLine.nli -= nCount;
  m_clsLine.nliRemain += nCount;
  // 如果链表里有多于PFX_EC_LINE_LINEINFO_ARRAY_GROWBY 125%个空单元时就要
  // 将数组的尺寸缩减，这样可以减少内存占用
  if ( m_clsLine.nliRemain > ( PFX_EC_LINE_LINEINFO_ARRAY_GROWBY * 5 / 4 ) ) {
    LONG nNewSize = ( m_clsLine.nli + PFX_EC_LINE_LINEINFO_ARRAY_GROWBY );
    pli = new CPfxEditLineInfo[ nNewSize ];
    memmove( pli, m_clsLine.pli, m_clsLine.nli * sizeof( CPfxEditLineInfo ) );
    delete[] m_clsLine.pli;
    m_clsLine.pli = pli;
    m_clsLine.nliRemain = PFX_EC_LINE_LINEINFO_ARRAY_GROWBY;
  }
}
void CPfxEditDoc::TextInsertLine( LONG nIndex, LPCTSTR pLine, LONG nLen, DWORD dwFlags ) {
  ASSERT( nLen >= 0 );
  CPfxEditLineInfo* pli = TextInsertLine( nIndex, 1 );
  if ( nLen > 0 ) {
    ASSERT( pLine != NULL );
    LPTSTR pBuffer = NULL;
    CPfxEditDocMemBlock* pMemBlock = NULL;
    BufferLineGetSpace( nLen, &pBuffer, &pMemBlock );
    memcpy( pBuffer, pLine, nLen * sizeof( TCHAR ) );
    pli->dwFlags = dwFlags;
    pli->nActLen = pli->nLen = nLen;
    pli->pData = pBuffer;
    pli->pMemBlock = pMemBlock;
  } else {
    pli->dwFlags = dwFlags;
    pli->nActLen = pli->nLen = 0;
    pli->pData = NULL;
    pli->pMemBlock = NULL;
  }
  pli->nFirstChar = 0;
  pli->nIndex = PFX_EC_INVALID_LINE;
  pli->nSelFrom = pli->nSelTo = PFX_EC_INVALID_SELECT;
}
void CPfxEditDoc::TextRemoveLine( LONG nIndex ) {
  ASSERT( false );
  ASSERT( !PFX_EC_IS_SUBLINE( TextGetLineAt( nIndex ) ) );
  CPfxEditLineInfo* pli = TextGetLineAt( nIndex );
  CPfxEditDocMemBlock* pMemBlock;
  pMemBlock = static_cast<CPfxEditDocMemBlock*>( pli->pMemBlock );
  LONG nLen = pli->nActLen;
  TextRemoveLine( nIndex, 1 );
  BufferLineCompact( pMemBlock, nLen );
}
// 将物理位置的主行信息转换为相对位置的子行信息
void CPfxEditDoc::TextAbsLineToSubLine( CPoint& rpt ) const {
  if ( !IsValidChar( rpt.y, rpt.x ) ) {
    rpt.y = GetValidLine( rpt.y );
    rpt.x = GetValidChar( rpt.y, rpt.x );
  }
  TextAbsLineToMainLine( rpt );
  TextMainLineToSubLine( rpt );
}
void CPfxEditDoc::TextAbsLineToMainLine( CPoint& rpt ) const {
  if ( ( rpt.y < 0 ) || ( rpt.y >= GetLineCount() ) ) {
    rpt = CPoint( 0, 0 );
    return ;
  }
  if ( !IsWrapping() ) {
    return ;
  }
  // 下面使用折半查找方法查找指定的物理行
  for ( LONG nMin = rpt.y, nMax = TextGetLineCount(), nTarget = 0; /* empty */; /* empty */ ) {
    LONG nCur = nMin + ( nMax - nMin ) / 2;
    LONG nLine = TextGetThisMainLine( nCur );
    LONG nIndex = TextGetLineAt( nLine ) ->nIndex;
    // |---------------|-V--------------|
    if ( nIndex > rpt.y ) {
      nMax = nCur - 1;
    }
    // |-------------V-|----------------|
    else if ( nIndex < rpt.y ) {
      nMin = nCur + 1;
    } else {
      nTarget = nLine;
      break;
    }
  }
  rpt.y = nTarget;
}
LONG CPfxEditDoc::TextAbsLineToOffset( const CPoint& rpt ) const {
  CPoint pt = rpt;
  TextAbsLineToSubLine( pt );
  return TextSubLineToOffset( pt );
}
// 将相对主行的行序号转换成物理主行的行序号。如果当前不是处于自动换行模
// 式下则什么也不做
// CPoint& pt - [输入]指出主行行号
//           [输出]返回物理行号
void CPfxEditDoc::TextMainLineToAbsLine( CPoint& rpt ) const {
  ASSERT( !PFX_EC_IS_SUBLINE( TextGetLineAt( rpt.y ) ) );
  if ( IsWrapping() ) {
    rpt.y = TextGetLineAt( rpt.y ) ->nIndex;
  }
}
// 将相对于主行行首的偏移量转换为相对于子行的偏移量。例如：第2行有3个子
// 行，各子行长度分别为10, 20, 15，而给定的相对于主行的偏移量是12，则返
// 回值是：行3，偏移量2
// CPoint& rpt - X等于相对于主行开始处的偏移量，Y等于主行行序号
void CPfxEditDoc::TextMainLineToSubLine( CPoint& rpt ) const {
  ASSERT( TextIsValidLine( rpt.y ) );
  ASSERT( !PFX_EC_IS_SUBLINE( TextGetLineAt( rpt.y ) ) );
  ASSERT( ( rpt.x >= 0 ) && ( rpt.x <= TextGetLineAt( rpt.y ) ->nActLen ) );
  if ( IsWrapping() ) {
    for ( LONG nLineUpperBound = TextGetLineUpperBound(); rpt.y < nLineUpperBound; rpt.y++ ) {
      const CPfxEditLineInfo* pli = TextGetLineAt( rpt.y );
      rpt.x -= pli->nLen;
      if ( rpt.x <= 0 ) {
        rpt.x += pli->nLen;
        break;
      }
    }
  }
}
// 将相对位置的子行信息转换为物理位置的主行信息
void CPfxEditDoc::TextSubLineToAbsLine( CPoint& rpt ) const {
  ASSERT( TextIsValidLine( rpt.y ) );
  ASSERT( TextIsValidChar( rpt.y, rpt.x ) );
  TextSubLineToMainLine( rpt );
  TextMainLineToAbsLine( rpt );
}
// 相对子行位置转换成相对主行位置
void CPfxEditDoc::TextSubLineToMainLine( CPoint& rpt ) const {
  ASSERT( TextIsValidLine( rpt.y ) );
  ASSERT( TextIsValidChar( rpt.y, rpt.x ) );
  const CPfxEditLineInfo* pli = TextGetLineAt( rpt.y );
  if ( PFX_EC_IS_SUBLINE( pli ) ) {
    rpt.y -= pli->nActLen;
    rpt.x += pli->pData - TextGetLineAt( rpt.y ) ->pData;
  }
}
// 计算某点到文档开头的位置
// CPoint& rpt - 该点都文档开头的位置（相对的子行位置）
// 返回该点到文档开头的偏移量
LONG CPfxEditDoc::TextSubLineToOffset( const CPoint& rpt ) const {
  if ( !TextIsValidChar( rpt.y, rpt.x ) ) {
    return 0;
  }
  for ( LONG nOffset = rpt.x, i = 0; i < rpt.y; i++ ) {
    const CPfxEditLineInfo* pli = TextGetLineAt( i );
    if ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      nOffset += ( pli->nLen + PFX_EC_VIRTUAL_LINE_BREAK_SIZE );
    } else {
      nOffset += pli->nLen;
    }
  }
  return nOffset;
}
// LONG nOffset - 从文档开始的偏移量
// CPoint& rpt  - 返回计算结果
// 返回数值在rpt中
void CPfxEditDoc::TextOffsetToSubLine( LONG nOffset, CPoint& rpt ) const {
  LONG nLineCount = TextGetLineCount();
  for ( rpt = CPoint( 0, 0 ); rpt.y < nLineCount; rpt.y++ ) {
    const CPfxEditLineInfo* pli = TextGetLineAt( rpt.y );
    nOffset -= pli->nLen;
    if ( nOffset <= 0 ) {
      rpt.x = pli->nLen + nOffset;
      break;
    }
    if ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      nOffset -= PFX_EC_VIRTUAL_LINE_BREAK_SIZE;
    }
    ASSERT( nOffset >= 0 );
  }
  if ( nOffset > 0 ) {
    rpt.y = TextGetLineUpperBound();
    rpt.x = TextGetLineLength( rpt.y );
  }
}
// 获取当前行所在的主行行号
// LONG nLine - 当前行行号，它可以是主行也可以是子行
// 返回当前行以零为基值的主行行号
LONG CPfxEditDoc::TextGetThisMainLine( LONG nLine ) const {
  ASSERT( TextIsValidLine( nLine ) );
  ASSERT( ( nLine >= 0 ) && ( nLine < TextGetLineCount() ) );
  // 如果是子行则nActLen是该行到主行的行偏移量，否则是整行的长度
  const CPfxEditLineInfo* pli = TextGetLineAt( nLine );
  return ( !PFX_EC_IS_SUBLINE( pli ) ? nLine : ( nLine - pli->nActLen ) );
}
// 如果当前行已经是第一个一主行内的行反回PFX_EC_INVALID_LINE，否则返回上一主行行序号
LONG CPfxEditDoc::TextGetPrevMainLine( LONG nLine ) const {
  ASSERT( TextIsValidLine( nLine ) );
  const CPfxEditLineInfo* pli = TextGetLineAt( nLine );
  if ( PFX_EC_IS_SUBLINE( pli ) ) {
    // 跳过本身所在主行
    nLine -= pli->nActLen;
  }
  // 现在nLine应该指向一主行
  ASSERT( !PFX_EC_IS_SUBLINE( TextGetLineAt( nLine ) ) );
  if ( nLine <= 0 ) {
    return PFX_EC_INVALID_LINE;
  }
  nLine--;
  pli = TextGetLineAt( nLine );
  return ( !PFX_EC_IS_SUBLINE( pli ) ? nLine : ( nLine - pli->nActLen ) );
}
// 如果当前行已经是最后一主行内的行反回PFX_EC_INVALID_LINE，否则返回下一主行行序号
LONG CPfxEditDoc::TextGetNextMainLine( LONG nLine ) const {
  ASSERT( TextIsValidLine( nLine ) );
  LONG nLineCount = TextGetLineCount();
  for ( nLine++; nLine < nLineCount; nLine++ ) {
    if ( !PFX_EC_IS_SUBLINE( TextGetLineAt( nLine ) ) ) {
      return nLine;
    }
  }
  return PFX_EC_INVALID_LINE;
}
// 获取一行中包括主行和子行在内的总行数
// LONG nLine - 行序号可以是主行行号，也可以是子行行号
// 返回的数值是包含主行在内
LONG CPfxEditDoc::TextGetLineSubLines( LONG nLine ) const {
  ASSERT( TextIsValidLine( nLine ) );
  // 如果是子行就转换成所在的主行行号
  if ( PFX_EC_IS_SUBLINE( TextGetLineAt( nLine ) ) ) {
    nLine = TextGetThisMainLine( nLine );
  }
  LONG nSubLines = 1; // 至少有一主行
  LONG nLineCount = TextGetLineCount();
  while ( ( ++nLine < nLineCount )
          && ( PFX_EC_IS_SUBLINE( TextGetLineAt( nLine ) ) ) ) {
    nSubLines++;
  }
  return nSubLines;
}
// 获取从某行某列开始到某行某列结束的块的大小。当调用TextGetBlockData()
// 前可以使用本函数计算需要的缓冲区大小
// CPoint& rptFrom     - 起点，单位是相对子行位置
// CPoint& rptTo       - 终点，单位是相对子行位置
// LONG nLineBreakType - 换行符按哪种类型计算。可选类型有：
//                    PFX_EC_LINE_BREAK_TYPE_DOS
//                    PFX_EC_LINE_BREAK_TYPE_UNIX
//                    PFX_EC_LINE_BREAK_TYPE_MAC
LONG CPfxEditDoc::TextGetBlockSize( const CPoint& rptFrom, const CPoint& rptTo, LONG nLineBreakType ) {
  ASSERT( rptFrom.y <= rptTo.y );
  ASSERT( TextIsValidChar( rptFrom.y, rptFrom.x ) );
  ASSERT( TextIsValidChar( rptTo.y, rptTo.x ) );
  if ( rptFrom.y == rptTo.y ) {
    return rptTo.x - rptFrom.x;
  }
  LONG nLineBreakSize = GetLineBreakSizeFromType( nLineBreakType );
  // rptFrom.y行取rptFrom.x以后部分字符
  const CPfxEditLineInfo* pli = TextGetLineAt( rptFrom.y );
  LONG nSize = pli->nLen - rptFrom.x;
  if ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
    nSize += nLineBreakSize;
  }
  // 中间部分取整行
  for ( LONG i = rptFrom.y + 1; i < rptTo.y; i++ ) {
    pli = TextGetLineAt( i );
    nSize += ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ? ( pli->nLen + nLineBreakSize ) : pli->nLen );
  }
  // rptTo.y行取rptTo.x以前部分字符
  nSize += rptTo.x;
  ASSERT( nSize > 0 );
  return nSize;
}
// 获取从某行某列开始到某行某列结束的块的数据
LONG CPfxEditDoc::TextGetBlockData( const CPoint& rptFrom, const CPoint& rptTo, LPTSTR pBuffer, LONG nBufMax, LONG nLineBreakType ) {
  ASSERT( rptFrom.y <= rptTo.y );
  ASSERT( TextIsValidChar( rptFrom.y, rptFrom.x ) );
  ASSERT( TextIsValidChar( rptTo.y, rptTo.x ) );
  LPTSTR pCur = pBuffer;
  CPoint ptFrom = rptFrom;
  CPoint ptTo = rptTo;
  // 如果操作范围是在同一行内
  if ( ptFrom.y == ptTo.y ) {
    ASSERT( ptFrom.x <= ptTo.x );
    LONG nLen = min( nBufMax, ( ptTo.x - ptFrom.x ) );
    if ( nLen > 0 ) {
      const CPfxEditLineInfo * pli = TextGetLineAt( ptFrom.y );
      memcpy( pCur, pli->pData + ptFrom.x, nLen * sizeof( TCHAR ) );
      pCur += nLen;
    }
  } else {
    LONG nLineBreakSize = GetLineBreakSizeFromType( nLineBreakType );
    LPCTSTR pLineBreakData = GetLineBreakDataFromType( nLineBreakType );
    // 首先获取第一行ptFrom.x后面的字符
    const CPfxEditLineInfo* pli = TextGetLineAt( ptFrom.y );
    LONG nLen = pli->nLen - ptFrom.x;
    if ( nLen > 0 ) {
      nLen = min( nBufMax, nLen );
      memcpy( pCur, pli->pData + ptFrom.x, nLen * sizeof( TCHAR ) );
      pCur += nLen;
      nBufMax -= nLen;
    }
    if ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      nBufMax -= nLineBreakSize;
      if ( nBufMax >= 0 ) {
        memcpy( pCur, pLineBreakData, nLineBreakSize * sizeof( TCHAR ) );
        pCur += nLineBreakSize;
      }
    }
    // 获取中间行的数据
    for ( ptFrom.y++; ( ptFrom.y < ptTo.y ) && ( nBufMax > 0 ); ptFrom.y++ ) {
      pli = TextGetLineAt( ptFrom.y );
      nLen = min( nBufMax, pli->nLen );
      if ( nLen > 0 ) {
        memcpy( pCur, pli->pData, ( nLen * sizeof( TCHAR ) ) );
        pCur += nLen;
        nBufMax -= nLen;
      }
      if ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
        nBufMax -= nLineBreakSize;
        if ( nBufMax >= 0 ) {
          memcpy( pCur, pLineBreakData, nLineBreakSize * sizeof( TCHAR ) );
          pCur += nLineBreakSize;
        }
      }
    }
    // 获取最后行的数据
    if ( ( nBufMax > 0 ) && ( ptTo.x > 0 ) ) {
      nLen = min( nBufMax, ptTo.x );
      memcpy( pCur, TextGetLineAt( ptTo.y ) ->pData, nLen * sizeof( TCHAR ) );
      pCur += nLen;
    }
  }
  return ( pCur - pBuffer );
}
void CPfxEditDoc::TextUpdateDocSize( LONG cchChanged ) {
  m_nDocSize += ( cchChanged * sizeof( TCHAR ) );
  ASSERT( m_nDocSize >= 0 );
  SetModifiedFlag( true );
}
void CPfxEditDoc::TextUpdateLineIndex( LONG nFromLine, LONG nBaseIndex ) {
  ASSERT( ( nFromLine >= 0 ) && ( nFromLine < TextGetLineCount() ) );
  ASSERT( !PFX_EC_IS_SUBLINE( TextGetLineAt( nFromLine ) ) );
  ASSERT( nBaseIndex <= nFromLine );
  if ( !IsWrapping() ) {
    ASSERT( nBaseIndex == -1 );
    // 更新主行行数
    m_nDocLineCount = TextGetLineCount();
  } else {
    ASSERT( ( nBaseIndex >= 0 ) && ( nBaseIndex < TextGetLineCount() ) );
    CPfxEditLineInfo* pli = TextGetLineAt( nFromLine );
    CPfxEditLineInfo* pliEnd = TextGetLineAt( TextGetLineUpperBound() );
    for ( ; pli <= pliEnd; pli++ ) {
      if ( !PFX_EC_IS_SUBLINE( pli ) ) {
        pli->nIndex = nBaseIndex++;
      }
    }
    // 更新主行行数
    m_nDocLineCount = nBaseIndex;
  }
  ASSERT( m_nDocLineCount >= 1 );
}
// 一个行被修改了以后，需重新计算它的子行信息，本函数根据一指定的主行以
// 及相对于该主行开头的某一偏移量数值计算新的当前位置（插入符对应的字符
// 位置），并且把一定数量的行信息写回到行信息链中
// CPfxEditLineInfo* pliHead - 存储了已经计算好的行信息的缓冲区，它的首地
//                          址指针被看成是nFirstModifyLine行信息的指针
// LONG nFirstModifyLine     - 将被修改的第一行的行序号，修改从这一行开始
// LONG nModifiedLines       - 有这个数量的行被修改了，修改后变成nAdjustedLines行
// LONG nAdjustedLines       - 参见LONG ModifiedLines的解释
// CPoint& rptCurPos         - 输入：新的主行和主行偏移量位置
//                          返回：返回计算好的新当前位置（子行位置）
// CPfxEditLineInfo* pliHead里面的数据应该被CPfxEditView::WrapLineSingle()
// 或WrapLineMultiple()函数处理过
void CPfxEditDoc::TextUpdateLineInfo( CPfxEditLineInfo* pliHead, LONG nFirstModifiedLine, LONG nModifiedLines, LONG nAdjustedLines, CPoint& rptCurPos ) {
  ASSERT( rptCurPos.y >= nFirstModifiedLine );
  ASSERT( rptCurPos.x >= 0 );
  ASSERT( ( nFirstModifiedLine + nAdjustedLines ) > rptCurPos.y );
  CPfxEditLineInfo* pli = ( pliHead + ( rptCurPos.y - nFirstModifiedLine ) );
  LPCTSTR lp = pli->pData + rptCurPos.x;
  rptCurPos.x = 0;
  for ( LONG nLastModifyLine = nFirstModifiedLine + nAdjustedLines; rptCurPos.y < nLastModifyLine; ++rptCurPos.y, ++pli ) {
    if ( lp <= ( pli->pData + pli->nLen ) ) {
      rptCurPos.x = lp - pli->pData;
      break;
    }
  }
  LONG nLines = nAdjustedLines - nModifiedLines;
  if ( nLines > 0 )           //      修改后行数大于修改前行数，增加一些行
  {
    // 在最后增加行可能不需要移动大量数据
    TextInsertLine( ( nFirstModifiedLine + nModifiedLines ), nLines );
  } else if ( nLines < 0 )           //      否则删除一些行
  {
    TextRemoveLine( nLastModifyLine, -nLines );
  }
  memcpy( TextGetLineAt( nFirstModifiedLine ), pliHead, nAdjustedLines * sizeof( CPfxEditLineInfo ) );
  // 如果这次修改了文档的最后一行，则一定要清除最后一行的PFX_EC_LF_HARDRETURN
  // 标志，因为最后一行肯定没有该标志
  if ( TextGetLineCount() == nLastModifyLine ) {
    TextGetLineAt( TextGetLineUpperBound() ) ->dwFlags &= ~PFX_EC_LF_HARDRETURN;
  }
}
void CPfxEditDoc::TextConstructEmptyLineInfo( CPfxEditLineInfo* pli, LONG nCount ) const {
  ASSERT( nCount > 0 );
  for ( LONG i = 0; i < nCount; i++, pli++ ) {
    pli->dwFlags = PFX_EC_LF_HARDRETURN;
    pli->nIndex = PFX_EC_INVALID_LINE;
    pli->nFirstChar = 0;
    pli->nActLen = pli->nLen = 0;
    pli->nSelFrom = pli->nSelTo = PFX_EC_INVALID_SELECT;
    pli->pData = NULL;
    pli->pMemBlock = NULL;
  }
}
// 获取一缓冲区来存放一行的数据
// CPfxEditLineInfo* pli - 指向一行信息结构，该结构应该是TextGetLineAt()
//                      所返回的
// LONG nNewLen          - 新的缓冲区长度，该数值可取[0->需要的长度]
// 1）本函数有缓冲区压缩功能（垃圾回收），请参见BufferLineCompact()
// 2）由于调用本函数后可能导致执行垃圾回收操作，而一旦执行了垃圾回收操作
// 后，所有的行数据缓冲区都可能会改变，所以一但调用本函数后请务必记住
// 重新调用TextGetLineAt()函数获取某行的新的行信息，例如：
// void AppendTextToLine(LONG nLineIndex, LPCTSTR pTextToAppend)
// {
//       CPfxEditLineInfo* pli = TextGetLineAt(TextGetThisMainLine(nLineIndex));
//       CPfxEditLineInfo* pli2 = TextGetLineAt(0); // 示范用！
//       LONG nLenOfTextToAppend = lstrlen(pTextToAppend);
//       LONG nOldLen = TextGetLineLength(nLineIndex);
//       LONG nNewLen = nOldLen + nLenOfTextToAppend;
//       调用本函数获取需要的缓冲区
//       TextGetBuffer(pli, nNewLen);
////   记住一定要刷新所有已获取的CPfxEditLineInfo指针
//       pli = TextGetLineAt(nLineIndex);
//       pli2 = TextGetLineAt(0);
//       LPTSTR pData = pli->pData;
//       LPTSTR pInsertPoint = pData + pli->nActLen;
//       memcpy(pInsertPoint, pTextToAppend, nLenOfTextToAppend * sizeof(TCHAR));
// }
// 以上例子只是示范，实际上要插入文本应该调用TextInsertChars()，要删除
// 文本应该使用TextRemoveChars()
void CPfxEditDoc::TextGetBuffer( CPfxEditLineInfo* pli, LONG nNewLen ) {
  ASSERT( ( pli >= TextGetLineAt( 0 ) ) && ( pli <= TextGetLineAt( TextGetLineUpperBound() ) ) );
  ASSERT( !PFX_EC_IS_SUBLINE( pli ) );
  ASSERT( nNewLen >= 0 );
  LPTSTR pBuffer;
  CPfxEditDocMemBlock* pNewMemBlock;
  CPfxEditDocMemBlock* pCurMemBlock;
  LONG nOldLen = pli->nActLen;
  if ( nOldLen == nNewLen ) {
    return ;
  }
  // 原来是空行，现在申请新空间
  if ( 0 == nOldLen ) {
    pBuffer = NULL;
    pNewMemBlock = NULL;
    if ( nNewLen > 0 ) {
      BufferLineGetSpace( nNewLen, &pBuffer, &pNewMemBlock );
    }
    pli->nActLen = pli->nLen = nNewLen;
    pli->pData = pBuffer;
    pli->pMemBlock = pNewMemBlock;
    return ;
  }
  pCurMemBlock = static_cast<CPfxEditDocMemBlock*>( pli->pMemBlock );
  ASSERT( pCurMemBlock != NULL );
  if ( 0 == nNewLen ) {
    pli->nActLen = pli->nLen = 0;
    pli->pData = NULL;
    pli->pMemBlock = NULL;
    BufferLineCompact( pCurMemBlock, nOldLen );
    return ;
  }
  // 原来的行是该块内存块的最后一行，现在需要调整尺寸，这种情况可以动态调
  // 整该内存块的空闲区域指针来达到目的，以使操作更有效率
  if ( ( ( pli->pData + nOldLen ) == pCurMemBlock->pRemain )
       && ( ( pCurMemBlock->nRemain + nOldLen ) >= nNewLen ) ) {
    ASSERT( nOldLen > 0 );
    ASSERT( pli->pData != NULL );
    pli->nActLen = pli->nLen = nNewLen;
    nOldLen -= nNewLen;
    pCurMemBlock->nRemain += nOldLen;
    pCurMemBlock->pRemain -= nOldLen;
    return ;
  } else {
    // 如果是要求缩小缓冲区，只需要记下缩减的尺寸并返回原来的缓
    // 冲就可以
    if ( nNewLen < nOldLen ) {
      nOldLen -= nNewLen;
      pli->nActLen = pli->nLen = nNewLen;
    } else {
      BufferLineGetSpace( nNewLen, &pBuffer, &pNewMemBlock );
      memcpy( pBuffer, pli->pData, ( nOldLen * sizeof( TCHAR ) ) );
      pli->nActLen = pli->nLen = nNewLen;
      pli->pData = pBuffer;
      pli->pMemBlock = pNewMemBlock;
    }
    BufferLineCompact( pCurMemBlock, nOldLen );
    return ;
  }
}
// 在指定位置插入文本（该文本数据可以包含任意个换行符，但只限于DOS型的
// “回车/换行”即\r\n）
// CPoint& rpt         - 在该位置插入文本，它是相对子行位置
// LPCTSTR pData       - 要插入的文本指针
// LONG cch            - 要插入的文本的长度
// CPfxEditView* pView - 在哪个视图中执行本操作。如果该参数为NULL，则不
//                    触发刷新操作，这需要在以后执行一次全面刷新。当
//                    需要完成一次批操作时指定为NULL可以获得更高的执
//                    行速度
void CPfxEditDoc::TextInsertChars( CPoint& rpt, LPCTSTR pData, LONG cch, CPfxEditView* pView ) {
  ASSERT( cch > 0 );
  ASSERT( TextIsValidChar( rpt.y, rpt.x ) );
  CPfxEditHeap Heap;
  LONG cchChanged;
  LONG nAdjustedLines;
  LONG nModifiedLines;
  CPoint ptNewPos;
  // 首先判断要插入的字符中包含有多少行
  LPCTSTR pCur = pData;
  LPCTSTR pEnd = pCur + cch;
  LONG nLinesToInsert = 1;
  while ( pCur < pEnd ) {
    // 要插入的字符中换行符只能是DOS型的CRLF，即'\r\n'
    if ( ( PFX_EC_CHR_CR == *pCur++ )
         && ( pCur < pEnd )
         && ( PFX_EC_CHR_LF == *pCur ) ) {
      nLinesToInsert++;
      pCur++;
    }
  }
  CPoint ptCurPos = rpt;
  TextSubLineToMainLine( ptCurPos );
  nModifiedLines = TextGetLineSubLines( ptCurPos.y );
  // 如果要插入的字符中没有换行符，问题就简单得多
  if ( nLinesToInsert == 1 ) {
    CPfxEditLineInfo * pli = TextGetLineAt( ptCurPos.y );
    LONG cchOldLine = pli->nActLen;
    LONG cchNewLine = cchOldLine + cch;
    TextGetBuffer( pli, cchNewLine );
    // 移动插入点后面的字符以腾出空间存放插入的字符
    LPCTSTR pStart = pli->pData;
    pCur = pStart + ptCurPos.x;
    if ( cchOldLine > ptCurPos.x ) {
      memmove( const_cast<LPTSTR>( ( pCur + cch ) ), pCur, ( cchOldLine - ptCurPos.x ) * sizeof( TCHAR ) );
    }
    // 保存插入的字符
    memcpy( const_cast<LPTSTR>( pCur ), pData, ( cch * sizeof( TCHAR ) ) );
    pCur += cch;
    // 重新调整行信息
    nAdjustedLines = m_arrViews[ 0 ] ->WrapLineSingle( Heap, pli );
    LONG nFirstModifiedLine = ptCurPos.y;
    ptNewPos.x = pCur - pStart;
    ptNewPos.y = ptCurPos.y;
    TextUpdateLineInfo( reinterpret_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ), nFirstModifiedLine, nModifiedLines, nAdjustedLines, ptNewPos );
    cchChanged = cch;
  }
  // 如果要插入的字符包含有多行，则要分两步进行，第一步找出要插入的行
  // 数，然后在主行信息链中分配足够的行信息空间，再为每一行分配缓冲区，
  // 第二步再为自动换行而调整新插入的行
  else {
    LONG nModifiedLinesBaseIndex = TextGetLineAt( ptCurPos.y ) ->nIndex;
    nModifiedLines += nLinesToInsert;
    CPfxEditLineInfo* pli = TextInsertLine( ptCurPos.y, nLinesToInsert );
    TextConstructEmptyLineInfo( pli, nLinesToInsert );
    LONG nOldMainLine = ptCurPos.y + nLinesToInsert;
    CPfxEditLineInfo* pliOld = TextGetLineAt( nOldMainLine );
    LONG cchOldLine = pliOld->nActLen;
    // |<-修改行插入点前部分数据->|<-要插入的字符-|<-修改行插入点后部分数据->|
    // |                          |               |                          |
    // |abc123abc123abc123abc123ab|abc123\r\nabc12|abc123abc123abc123abc123ab|
    // |                          |    |     |    |                          |
    // |<-新的第一行--------------|    |<-|->|    |<-新的最后行------------->|
    //                                   |
    //                               新的中间行
    // 首先将原来行插入点的前半部分与插入字符第一个换行符前面的
    // 字符合并起来组成第一行
    LPCTSTR pStart = pCur = pData;
    pEnd = pStart + cch;
    while ( pCur < pEnd ) {
      if ( ( PFX_EC_CHR_CR == *pCur++ )
           && ( pCur < pEnd )
           && ( PFX_EC_CHR_LF == *pCur ) ) {
        pCur--;
        break;
      }
    }
    pCur = min( pCur, pEnd );
    // 第一行的长度
    LONG nLen1 = ptCurPos.x;
    LONG nLen2 = pCur - pStart;
    LONG cchNewLine = nLen1 + nLen2;
    LONG nLineIndex = ptCurPos.y;
    pli = TextGetLineAt( nLineIndex );
    if ( cchNewLine > 0 ) {
      TextGetBuffer( pli, cchNewLine );
    }
    // 复制前半部分字符
    if ( nLen1 > 0 ) {
      memcpy( pli->pData, pliOld->pData, ( nLen1 * sizeof( TCHAR ) ) );
    }
    if ( nLen2 > 0 ) {
      memcpy( ( pli->pData + nLen1 ), pStart, ( nLen2 * sizeof( TCHAR ) ) );
    }
    // 复制第一行和最后一行中的中间行的数据，这些数据在插入的字
    // 符中都有一个换行符来分隔
    pCur += PFX_EC_LINE_BREAK_SIZE_DOS;
    pStart = pCur;
    while ( pCur < pEnd ) {
      if ( ( PFX_EC_CHR_CR == *pCur++ )
           && ( pCur < pEnd )
           && ( PFX_EC_CHR_LF == *pCur ) ) {
        nLineIndex++;
        cchNewLine = ( pCur - pStart - 1 );
        if ( cchNewLine > 0 ) {
          pli = TextGetLineAt( nLineIndex );
          TextGetBuffer( pli, cchNewLine );
          memcpy( pli->pData, pStart, cchNewLine * sizeof( TCHAR ) );
        }
        pStart = ++pCur;
      }
    }
    pCur = min( pCur, pEnd );
    pStart = min( pStart, pEnd );
    // 最后一行的数据
    nLen1 = pCur - pStart;
    nLen2 = cchOldLine - ptCurPos.x;
    cchNewLine = nLen1 + nLen2;
    nLineIndex++;
    pli = TextGetLineAt( nLineIndex );
    if ( cchNewLine > 0 ) {
      TextGetBuffer( pli, cchNewLine );
    }
    if ( nLen1 > 0 ) {
      memcpy( pli->pData, pStart, ( nLen1 * sizeof( TCHAR ) ) );
    }
    if ( nLen2 > 0 ) {
      memcpy( pli->pData + nLen1, pliOld->pData + ptCurPos.x, nLen2 * sizeof( TCHAR ) );
    }
    // 释放被修改行原来的缓冲区
    TextGetBuffer( pliOld, 0 );
    // 重新调整被修改了的行信息
    nLineIndex = nLinesToInsert - 1;
    pli = TextGetLineAt( ptCurPos.y );
    // 恢复第一个被修改行的原来的标志（书签...等）
    pli->dwFlags = pliOld->dwFlags | PFX_EC_LF_HARDRETURN;
    nAdjustedLines = m_arrViews[ 0 ] ->WrapLineMultiple( Heap, pli, nLinesToInsert, nLineIndex );
    LONG nFirstModifiedLine = ptCurPos.y;
    ptNewPos.x = nLen1;
    ptNewPos.y = ptCurPos.y + nLineIndex;
    TextUpdateLineInfo( reinterpret_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ), nFirstModifiedLine, nModifiedLines, nAdjustedLines, ptNewPos );
    TextUpdateLineIndex( ptCurPos.y, nModifiedLinesBaseIndex );
    cchChanged = cch - ( nLinesToInsert * ( PFX_EC_LINE_BREAK_SIZE_DOS - GetLineBreakSize() ) );
  }
  if ( pView != NULL ) {
    DWORD dwFlags;
    LONG nFirstUpdateLine;
    LONG nLastUpdateLine;
    if ( nModifiedLines != nAdjustedLines ) {
      dwFlags = PFX_EC_UT_VERT;
      nFirstUpdateLine = ptCurPos.y;
      nLastUpdateLine = -1; // 更新直至窗口底边
    } else {
      dwFlags = PFX_EC_UT_HORZ;
      nFirstUpdateLine = ptCurPos.y;
      nLastUpdateLine = ptCurPos.y + nAdjustedLines - 1;
    }
    // 如果修改后前面的行的内容没有改变，只是当前编辑行改变了，
    // 那么就试图计算改变了的字符的宽度，而不是从编辑行的行首开始计算
    LONG xTextPos;
    if ( ( rpt.y == ptNewPos.y )
         && ( rpt.x < ptNewPos.x )
         && ( rpt.x == pView->m_ptEditPos.x )
         && ( rpt.y == pView->m_ptEditPos.y ) ) {
      pCur = TextGetLineAt( ptNewPos.y ) ->pData + rpt.x;
      xTextPos = pView->m_ptTextPos.x + pView->CalcStringWidth( pCur, ptNewPos.x - rpt.x, pView->m_ptLogiPos.x, NULL );
    } else {
      xTextPos = pView->TransCharPosToTextPos( ptNewPos );
    }
    pView->m_ptEditPos = ptNewPos;
    pView->UpdateAllViews( dwFlags, nFirstUpdateLine, nLastUpdateLine );
    pView->EnsureVisible( pView->m_ptEditPos.y, xTextPos );
    pView->UpdateCurrentPosition( xTextPos, true, true );
    // UpdateAllViews()可能会引起OnSize()被调用，这是因为当文档
    // 的行数变化有可能导致垂直滚动条由显示变隐藏或由隐藏变显示，
    // 这将会导致视图的客户区的宽度起变化，因而引起OnSize()被调
    // 用。如果出现这种情况并且当前处于根据窗口宽度而自动换行的
    // 模式时，自动换行后ptNewPos可能会成为无效位置，而
    // pView->m_ptEditPos是由此至终都被跟踪的，也即使说它的值一
    // 直都是正确的，所以要用它的值来从新设置ptNewPos的值
    ptNewPos = pView->m_ptEditPos;
  }
  TextUpdateDocSize( + cchChanged );
  rpt = ptNewPos; // 返回数值
  ASSERT( TextIsValidLine( ptNewPos.y ) );
  ASSERT( TextIsValidChar( ptNewPos.y, ptNewPos.x ) );
}
// 删除从指定点rptFrom到rptTo之间的文本
// CPoint& rptFrom     - 从这点开始删除，它是相对子行位置
// CPoint& rptTo       - 删除到这一点，它是相对子行位置
// CPfxEditView* pView - 在哪个视图(view)中执行删除，因为要刷新该视图
void CPfxEditDoc::TextRemoveChars( CPoint& rptFrom, CPoint& rptTo, CPfxEditView* pView ) {
  ASSERT( rptFrom.y <= rptTo.y );
  ASSERT( TextIsValidChar( rptFrom.y, rptFrom.x ) );
  ASSERT( TextIsValidChar( rptTo.y, rptTo.x ) );
  CPfxEditHeap Heap;
  LONG cchChanged;
  LONG nModifiedLines;
  LONG nAdjustedLines;
  CPoint ptNewPos;
  // 转换为主行和主行偏移量
  CPoint ptMainFirst = rptFrom;
  TextSubLineToMainLine( ptMainFirst );
  CPoint ptMainLast = rptTo;
  TextSubLineToMainLine( ptMainLast );
  if ( ptMainLast.y != ptMainFirst.y ) {
    LONG nLineBreakSize = GetLineBreakSize();
    // 计算有第一行到最后一行中共有多少行会被修改
    nModifiedLines = ( ptMainLast.y + TextGetLineSubLines( ptMainLast.y ) ) - ptMainFirst.y;
    ASSERT( nModifiedLines > 0 );
    // 操作将会把头一行和最后一行的未删除部分合并起来，中间行将
    // 被删除，新行的行标志(dwFlags)将会取原头一行的标志
    CPfxEditLineInfo* pliMainFirst = TextGetLineAt( ptMainFirst.y );
    CPfxEditLineInfo* pliMainLast = TextGetLineAt( ptMainLast.y );
    LONG nModifiedLinesBaseIndex = pliMainFirst->nIndex;
    LONG cchMainFirst = pliMainFirst->nActLen;
    LONG cchMainLast = pliMainLast->nActLen;
    LONG cchNewLine = ptMainFirst.x + ( cchMainLast - ptMainLast.x );
    // 重新分配第一行的缓冲区
    TextGetBuffer( pliMainFirst, cchNewLine );
    // 将最后行的未被删除部分接到第一行未被删除部分的尾部
    if ( !PFX_EC_IS_EMPTY_LINE( pliMainFirst )
         && !PFX_EC_IS_EMPTY_LINE( pliMainLast ) ) {
      memcpy( pliMainFirst->pData + ptMainFirst.x, pliMainLast->pData + ptMainLast.x, ( cchMainLast - ptMainLast.x ) * sizeof( TCHAR ) );
    }
    // 完成数据合并，下面开始删除第一行以下包括最后一行在内的所
    // 有行，首先释放它们所使用的缓冲区
    cchChanged = cchMainFirst;
    for ( LONG i = TextGetNextMainLine( ptMainFirst.y ); ( i != PFX_EC_INVALID_LINE ) && ( i <= ptMainLast.y ); i = TextGetNextMainLine( i ) ) {
      CPfxEditLineInfo * pli = TextGetLineAt( i );
      ASSERT( !PFX_EC_IS_SUBLINE( pli ) );
      cchChanged += ( pli->nActLen + nLineBreakSize );
      if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
        TextGetBuffer( pli, 0 );
      }
    }
    // 重新调整合并后的行，删除多余的行信息
    nAdjustedLines = m_arrViews[ 0 ] ->WrapLineSingle( Heap, pliMainFirst );
    LONG nFirstModifiedLine = ptMainFirst.y;
    ptNewPos = ptMainFirst;
    TextUpdateLineInfo( reinterpret_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ), ptMainFirst.y, nModifiedLines, nAdjustedLines, ptNewPos );
    TextUpdateLineIndex( ptMainFirst.y, nModifiedLinesBaseIndex );
    // 这一次操作被删除了的字符数
    cchChanged -= cchNewLine;
  } else // 删除操作将在一行中进行，以简单方法处理
  {
    nModifiedLines = TextGetLineSubLines( ptMainFirst.y );
    CPfxEditLineInfo* pliMainFirst = TextGetLineAt( ptMainFirst.y );
    LONG cchMainFirst = pliMainFirst->nActLen;
    cchChanged = ptMainLast.x - ptMainFirst.x;
    ASSERT( cchChanged >= 0 );
    LONG cchNewLine = cchMainFirst - cchChanged;
    LONG cchLastPart = cchMainFirst - ptMainLast.x;
    if ( cchLastPart > 0 ) {
      memmove( pliMainFirst->pData + ptMainFirst.x, pliMainFirst->pData + ptMainLast.x, cchLastPart * sizeof( TCHAR ) );
    }
    // 注：进行任何调整行缓冲区的操作后，都必须刷新指向该缓冲区
    // 的指针，因为缓冲区地址可能已经改变
    TextGetBuffer( pliMainFirst, cchNewLine );
    // 重新调整合并后的行，删除多余的行信息
    nAdjustedLines = m_arrViews[ 0 ] ->WrapLineSingle( Heap, pliMainFirst );
    LONG nFirstModifiedLine = ptMainFirst.y;
    ptNewPos = ptMainFirst;
    TextUpdateLineInfo( reinterpret_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ), ptMainFirst.y, nModifiedLines, nAdjustedLines, ptNewPos );
  }
  if ( pView != NULL ) {
    DWORD dwFlags;
    LONG nFirstUpdateLine;
    LONG nLastUpdateLine;
    if ( ( nModifiedLines != nAdjustedLines )
         || ( ptMainFirst.y != ptMainLast.y ) ) {
      dwFlags = PFX_EC_UT_VERT;
      nFirstUpdateLine = ptMainFirst.y;
      nLastUpdateLine = -1; // 更新直至窗口底边
    } else {
      dwFlags = PFX_EC_UT_HORZ;
      nFirstUpdateLine = ptMainFirst.y;
      nLastUpdateLine = ptMainFirst.y + nAdjustedLines - 1;
    }
    if ( ptNewPos != pView->m_ptEditPos ) {
      pView->m_ptEditPos = ptNewPos;
      pView->UpdateAllViews( dwFlags, nFirstUpdateLine, nLastUpdateLine );
      pView->m_ptTextPos.x = pView->TransCharPosToTextPos( pView->m_ptEditPos );
      pView->EnsureVisible( pView->m_ptEditPos.y, pView->m_ptTextPos.x );
      pView->UpdateCurrentPosition( pView->m_ptTextPos.x, true, true );
    } else {
      pView->m_ptEditPos = ptNewPos;
      pView->UpdateAllViews( dwFlags, nFirstUpdateLine, nLastUpdateLine );
      pView->EnsureVisible( pView->m_ptEditPos.y, pView->m_ptTextPos.x );
      pView->UpdateCurrentPosition( pView->m_ptTextPos.x, false, true );
    }
    // UpdateAllViews()可能会引起OnSize()被调用，这是因为当文档
    // 的行数变化有可能导致垂直滚动条由显示变隐藏或由隐藏变显示，
    // 这将会导致视图的客户区的宽度起变化，因而引起OnSize()被调
    // 用。如果出现这种情况并且当前处于根据窗口宽度而自动换行的
    // 模式时，自动换行后ptNewPos可能会成为无效位置，而
    // pView->m_ptEditPos是由此至终都被跟踪的，也即使说它的值一
    // 直都是正确的，所以要用它的值来从新设置ptNewPos的值
    ptNewPos = pView->m_ptEditPos;
  }
  // 数据减少了，所以是负数
  TextUpdateDocSize( -cchChanged );
  rptFrom = ptNewPos; // 返回数值
  ASSERT( TextIsValidLine( ptNewPos.y ) );
  ASSERT( TextIsValidChar( ptNewPos.y, ptNewPos.x ) );
}
// 为UNDO记录提供缓冲区
// CPfxEditUndoInfo* pui - 指向一行信息结构，该结构应该是UndoGetInfo()
//                      所返回的
// LONG nLen             - 新的缓冲区长度，该数值可取[0->需要的长度]
// 重要提示：请参考TextGetBuffer()
void CPfxEditDoc::UndoGetBuffer( CPfxEditUndoInfo* pui, LONG nLen ) {
  ASSERT( pui != NULL );
  LONG i;
  LPTSTR pBuffer;
  CPfxEditDocMemBlock* pNewMemBlock;
  CPfxEditDocMemBlock* pCurMemBlock;
  // 如果原来没有申请缓冲区
  if ( PFX_EC_IS_EMPTY_UNDO( pui ) ) {
    pBuffer = NULL;
    pNewMemBlock = NULL;
    if ( nLen > 0 ) {
      BufferUndoGetSpace( nLen, &pBuffer, &pNewMemBlock );
    }
    pui->nLen = nLen;
    pui->pData = pBuffer;
    pui->pMemBlock = pNewMemBlock;
    return ;
  }
  i = pui->nLen;
  pCurMemBlock = static_cast<CPfxEditDocMemBlock*>( pui->pMemBlock );
  ASSERT( pCurMemBlock != NULL );
  // 原来已申请缓冲区，现在申请NULL缓冲区
  if ( nLen == 0 ) {
    pui->nLen = 0;
    pui->pData = NULL;
    pui->pMemBlock = NULL;
    BufferUndoCompact( pCurMemBlock, i );
    return ;
  }
  // 原来的缓冲区是该块内存块的最后一块分配出去的块，现在需要调整尺寸，
  // 这种情况可以动态调整该内存块的空闲区域指针来达到目的，以使操作更
  // 有效率
  if ( ( ( pui->pData + i ) == pCurMemBlock->pRemain )
       && ( ( pCurMemBlock->nRemain + i ) >= nLen ) ) {
    ASSERT( i > 0 );
    ASSERT( pui->pData != NULL );
    pui->nLen = nLen;
    i -= nLen;
    pCurMemBlock->nRemain += i;
    pCurMemBlock->pRemain -= i;
    return ;
  } else {
    BufferUndoGetSpace( nLen, &pBuffer, &pNewMemBlock );
    memcpy( pBuffer, pui->pData, ( min( nLen, i ) * sizeof( TCHAR ) ) );
    pui->nLen = nLen;
    pui->pData = pBuffer;
    pui->pMemBlock = pNewMemBlock;
    BufferUndoCompact( pCurMemBlock, i );
    return ;
  }
}
void CPfxEditDoc::UndoInsert( CPfxEditUndoInfo* pui, CPoint& rptRetPos, BOOL bRedo ) {
  ASSERT( pui != NULL );
  ASSERT( PFX_EC_IS_EMPTY_UNDO( pui ) );
  ASSERT( pui->ptFrom.y != PFX_EC_INVALID_LINE );
  ASSERT( pui->ptTo.y != PFX_EC_INVALID_LINE );
  // 原来是插入操作，UNDO是删除操作，REDO是插入操作
  CPoint ptFrom = pui->ptFrom;
  TextAbsLineToMainLine( ptFrom );
  TextMainLineToSubLine( ptFrom );
  CPoint ptTo = pui->ptTo;
  TextAbsLineToMainLine( ptTo );
  TextMainLineToSubLine( ptTo );
  // 删除之前保存要删除的数据，以便REDO时使用
  UndoGetBuffer( pui, TextGetBlockSize( ptFrom, ptTo, PFX_EC_LINE_BREAK_TYPE_DOS ) );
  ASSERT( !PFX_EC_IS_EMPTY_UNDO( pui ) );
  TextGetBlockData( ptFrom, ptTo, pui->pData, pui->nLen, PFX_EC_LINE_BREAK_TYPE_DOS );
  TextRemoveChars( ptFrom, ptTo, NULL );
  rptRetPos = ptFrom;
  m_clsUndo.nPosCur += ( !bRedo ? -1 : + 1 );
  ASSERT( m_clsUndo.nPosCur >= 0 );
  if ( m_clsUndo.nPosCur == m_clsUndo.nPosUnmodified ) {
    SetModifiedFlag( false );
  }
}
void CPfxEditDoc::UndoRemove( CPfxEditUndoInfo* pui, CPoint& rptRetPos, BOOL bRedo ) {
  ASSERT( pui != NULL );
  ASSERT( !PFX_EC_IS_EMPTY_UNDO( pui ) );
  ASSERT( pui->ptFrom.y != PFX_EC_INVALID_LINE );
  // 原来是删除操作，UNDO是插入操作，REDO是删除操作
  CPoint pt = pui->ptFrom;
  TextAbsLineToMainLine( pt );
  TextMainLineToSubLine( pt );
  TextInsertChars( pt, pui->pData, pui->nLen, NULL );
  rptRetPos = pt;
  TextSubLineToMainLine( pt );
  pui->ptTo = pt;
  // 释放UNDO缓冲区
  UndoGetBuffer( pui, 0 );
  ASSERT( PFX_EC_IS_EMPTY_UNDO( pui ) );
  ASSERT( pui->ptFrom.y != PFX_EC_INVALID_LINE );
  ASSERT( pui->ptTo.y != PFX_EC_INVALID_LINE );
  m_clsUndo.nPosCur += ( !bRedo ? -1 : + 1 );
  ASSERT( m_clsUndo.nPosCur >= 0 );
  if ( m_clsUndo.nPosCur == m_clsUndo.nPosUnmodified ) {
    SetModifiedFlag( false );
  }
}
// 由字符插入操作而调用本函数
// CPoint ptFrom - 开始位置，X等于插入操作的第一个字符位置，该数值相对于
//              主行行首，Y等于插入操作的第一行行号，该行一定要是主行
// CPoint ptTo   - X等于本行最后一个字符的位置，该值是相对于ptTo.y而言，
//              Y等于最后一行行号，该行可以是主行，也可以是子行
void CPfxEditDoc::UndoAddInsert( CPoint ptFrom, CPoint ptTo ) {
  ASSERT( ( ptFrom.y >= 0 ) && ( ptFrom.y < TextGetLineCount() ) );
  ASSERT( !PFX_EC_IS_SUBLINE( TextGetLineAt( ptFrom.y ) ) );
  ASSERT( ( ptFrom.x >= 0 ) && ( ptFrom.x <= TextGetLineAt( ptFrom.y ) ->nActLen ) );
  ASSERT( TextIsValidChar( ptTo.y, ptTo.x ) );
  TextMainLineToAbsLine( ptFrom );
  TextSubLineToMainLine( ptTo );
  TextMainLineToAbsLine( ptTo );
  // 如果在一行中连续的键入字符，就将其合并为一个UNDO记录
  if ( ( m_clsUndo.nGroupName == PFX_EC_UNDO_TYPING ) && ( ptFrom.y == ptTo.y ) ) {
    CPfxEditUndoInfo * pui = UndoGetInfo();
    if ( ( pui != NULL )
         && ( pui->fUndoName == m_clsUndo.nGroupName )
         && ( pui->ptFrom.y == ptFrom.y )
         && ( pui->ptFrom.y == pui->ptTo.y )
         && ( pui->ptTo.x == ptFrom.x ) ) {
      pui->ptTo.x = ptTo.x;
      return ;
    }
  }
  CPfxEditUndoInfo* pui = UndoAddRecord();
  pui->fAction = UndoGetFlags( PFX_EC_UF_INSERT );
  pui->fUndoName = m_clsUndo.nGroupName;
  pui->ptFrom = ptFrom;
  pui->ptTo = ptTo;
  pui->nLen = 0;
  pui->pData = NULL;
  pui->pMemBlock = NULL;
  ASSERT( pui->ptFrom.y != PFX_EC_INVALID_LINE );
  ASSERT( pui->ptTo.y != PFX_EC_INVALID_LINE );
}
// 当删除字符前，调用本函数来保存UNDO信息
// CPoint ptFrom - 起点的子行位置
// CPoint ptTo   - 终点的子行位置
void CPfxEditDoc::UndoAddRemove( CPoint ptFrom, CPoint ptTo ) {
  ASSERT( ptFrom.y <= ptTo.y );
  ASSERT( TextIsValidChar( ptFrom.y, ptFrom.x ) );
  ASSERT( TextIsValidChar( ptTo.y, ptTo.x ) );
  CPfxEditUndoInfo* pui = UndoAddRecord();
  ASSERT( pui != NULL );
  UndoGetBuffer( pui, TextGetBlockSize( ptFrom, ptTo, PFX_EC_LINE_BREAK_TYPE_DOS ) );
  ASSERT( !PFX_EC_IS_EMPTY_UNDO( pui ) );
  TextGetBlockData( ptFrom, ptTo, pui->pData, pui->nLen, PFX_EC_LINE_BREAK_TYPE_DOS );
  TextSubLineToMainLine( ptFrom );
  TextMainLineToAbsLine( ptFrom );
  TextSubLineToMainLine( ptTo );
  TextMainLineToAbsLine( ptTo );
  pui->fAction = UndoGetFlags( PFX_EC_UF_REMOVE );
  pui->fUndoName = m_clsUndo.nGroupName;
  pui->ptFrom = ptFrom;
  pui->ptTo = ptTo;
  ASSERT( pui->ptFrom.y != PFX_EC_INVALID_LINE );
  ASSERT( pui->ptTo.y != PFX_EC_INVALID_LINE );
}
// 每当要添加一个新的UNDO记录时调用本函数来开辟一记录空间
CPfxEditUndoInfo* CPfxEditDoc::UndoAddRecord() {
  // 是否有REDO信息？如果有就删除所有REDO信息，并释放其中的缓冲区
  if ( m_clsUndo.nPosCur < m_clsUndo.nui ) {
    // m_clsUndo.nPosCur所指向的就是最近一个REDO信息
    LONG nCurPos = m_clsUndo.nPosCur;
    CPfxEditUndoInfo* pui = m_clsUndo.pui + nCurPos;
    for ( ; nCurPos < m_clsUndo.nui; nCurPos++, pui++ ) {
      if ( !PFX_EC_IS_EMPTY_UNDO( pui ) ) {
        BufferUndoCompact( static_cast<CPfxEditDocMemBlock*>( pui->pMemBlock ), pui->nLen );
      }
    }
    m_clsUndo.nuiRemain += ( m_clsUndo.nui - m_clsUndo.nPosCur );
    m_clsUndo.nui = m_clsUndo.nPosCur;
  }
  // 如果空间不足就要重新调整数组尺寸
  if ( m_clsUndo.nuiRemain == 0 ) {
    // 如果当前还没有达到最大UNDO次数就增加UNDO缓冲区长度
    if ( m_clsUndo.nui < m_clsUndo.nuiMax ) {
      LONG nGrowBy = min( m_clsUndo.nuiGrowBy, ( m_clsUndo.nuiMax - m_clsUndo.nui ) );
      LONG nNewLen = m_clsUndo.nui + nGrowBy;
      CPfxEditUndoInfo* pui = new CPfxEditUndoInfo[ nNewLen ];
      if ( m_clsUndo.pui != NULL ) {
        memcpy( pui, m_clsUndo.pui, m_clsUndo.nui * sizeof( CPfxEditUndoInfo ) );
        delete[] m_clsUndo.pui;
      }
      m_clsUndo.pui = pui;
      m_clsUndo.nuiRemain = nNewLen - m_clsUndo.nui;
    }
    // 否则UNDO缓冲已经无法增长，解决的方法是删除最早的UNDO记录
    // 而且是至少一半的数量，以腾出空间容纳新的记录
    else {
      LONG nDelCount = m_clsUndo.nui / 2;
      CPfxEditUndoInfo* pui = m_clsUndo.pui;
      CPfxEditUndoInfo* puiEnd = m_clsUndo.pui + nDelCount;
      for ( ; pui < puiEnd; pui++ ) {
        if ( !PFX_EC_IS_EMPTY_UNDO( pui ) ) {
          BufferUndoCompact( static_cast<CPfxEditDocMemBlock*>( pui->pMemBlock ), pui->nLen );
        }
      }
      pui->fAction |= PFX_EC_UF_BEGIN_GROUP;
      m_clsUndo.nui -= nDelCount;
      m_clsUndo.nuiRemain += nDelCount;
      m_clsUndo.nPosCur = m_clsUndo.nui;
      m_clsUndo.nPosUnmodified -= nDelCount;
      memmove( m_clsUndo.pui, pui, m_clsUndo.nui * sizeof( CPfxEditUndoInfo ) );
    }
  }
  CPfxEditUndoInfo* pui = m_clsUndo.pui + m_clsUndo.nPosCur;
  m_clsUndo.nui++;
  m_clsUndo.nuiRemain--;
  m_clsUndo.nPosCur++;
  pui->fAction = PFX_EC_UF_UNKNOWN;
  pui->ptFrom.x = pui->ptTo.x = PFX_EC_INVALID_CHAR;
  pui->ptFrom.y = pui->ptTo.y = PFX_EC_INVALID_LINE;
  pui->nLen = 0;
  pui->pData = NULL;
  pui->pMemBlock = NULL;
  return pui;
}
// 当多个UNDO操作需要组成为组时调用本函数，完成操作后也必须调用本函数
// BOOL bBegin - TRUE等于标识一组的开始，FALSE表示结束一组操作
void CPfxEditDoc::UndoSetGroup( BOOL bBegin, WORD fUndoName ) {
  if ( bBegin ) {
    // 防止在一个GROUP结束之前又开始BEGIN_GROUP
    if ( m_clsUndo.nGroupLockCount == 0 ) {
      m_clsUndo.bGroupBegin = true;
      m_clsUndo.nGroupName = fUndoName;
    }
    m_clsUndo.nGroupLockCount++;
  } else {
    ASSERT( m_clsUndo.nGroupLockCount > 0 );
    if ( m_clsUndo.nGroupLockCount == 1 ) {
      m_clsUndo.bGroupBegin = false;
      m_clsUndo.nGroupName = PFX_EC_UNDO_UNKNOWN;
    }
    m_clsUndo.nGroupLockCount--;
  }
}
WORD CPfxEditDoc::UndoGetFlags( WORD wDefaultFlags ) {
  if ( m_clsUndo.nGroupLockCount > 0 ) {
    wDefaultFlags |= PFX_EC_UF_GROUP;
  }
  if ( m_clsUndo.bGroupBegin ) {
    wDefaultFlags |= PFX_EC_UF_BEGIN_GROUP;
    m_clsUndo.bGroupBegin = false;
  }
  return wDefaultFlags;
}
CPfxEditUndoInfo* CPfxEditDoc::UndoGetInfo() const {
  if ( UndoGetCount() <= 0 ) {
    return NULL;
  }
  return ( m_clsUndo.pui + ( m_clsUndo.nPosCur - 1 ) );
}
CPfxEditUndoInfo* CPfxEditDoc::UndoGetRedoInfo() const {
  if ( UndoGetRedoCount() <= 0 ) {
    return NULL;
  }
  return ( m_clsUndo.pui + m_clsUndo.nPosCur );
}
BOOL CPfxEditDoc::FileIsAnsi( LPCSTR pData, LONG cch ) const {
  UNUSED( pData );
  UNUSED( cch );
  return true; // 总是返回TRUE
}
BOOL CPfxEditDoc::FileIsUtf8( LPCSTR pData, LONG cch ) const {
  if ( ( pData == NULL ) || ( cch <= 0 ) ) {
    return false;
  }
  // 我们按下面的方法检测：
  // 1. 找到一含有非ASCII字符的行进行检测
  //  a) 如果它含有非法字符（例如0）就肯定不是UTF8编码
  //  b) 从这行的第一个字符开始解码，判断每个字符的长度并移动读写指
  // 针，如果最终读写指针与换行符的位置重合就表明这是正确的UTF8
  // 编码
  // 2. 当所有文件数据都是ASCII字符时，它既是ANSI编码也是UTF8编码，如
  //  果是这样的话，我们就把它当作ANSI处理
  LPCSTR pCur = pData;
  const LPCSTR pEnd = pData + cch;
  LPCSTR pTargetLine = NULL;
  while ( ( pCur < pEnd ) && ( pTargetLine == NULL ) ) {
    // 扫描换行符所在地址
    // 注：在UTF8中"\r", "\n"不是某个多字节字符的一部分
    LPCSTR pLineStart = pCur;
    while ( pCur < pEnd ) {
      CHAR c = *pCur++;
      if ( !__isascii( c ) || ( c == 0 ) )           //      目标行找到！
      {
        pTargetLine = pLineStart;
        break;
      }
      if ( ( PFX_EC_CHR_CR == c ) || ( PFX_EC_CHR_LF == c ) ) {
        if ( ( PFX_EC_CHR_CR == c )           //      MSDOS型的换行符
             && ( pCur < pEnd )
             && ( PFX_EC_CHR_LF == *pCur ) ) {
          pCur++; // 越过PFX_EC_CHR_LF
        }
        break;
      }
    }
  }
  if ( pTargetLine == NULL )           //      如果所有字符都是ASCII字符就当ANSI处理
  {
    return false;
  }
  // 现在已经找到一包含有非ASCII字符的行，下面确定这行的行末位置
  const LPCSTR pLineStart = pTargetLine;
  LPCSTR pLineEnd = pEnd;
  for ( pCur = pLineStart; pCur < pEnd; pCur++ ) {
    if ( ( PFX_EC_CHR_CR == *pCur ) || ( PFX_EC_CHR_LF == *pCur ) ) {
      pLineEnd = pCur;
      break;
    }
  }
  CPfxEditDocUtf8Converter u8;
  return u8.IsUtf8FromUnicode( pLineStart, ( pLineEnd - pLineStart ) );
}
BOOL CPfxEditDoc::FileIsUnicode( LPCSTR pData, LONG cch ) const {
  // 1. 如果文件为空，它可以是任何类型的文档
  // 2. 如果文件的数据不能被sizeof(WCHAR)整除，它也不是UNICODE文件
  if ( ( pData == NULL )
       || ( cch < sizeof( WCHAR ) )
       || ( cch % sizeof( WCHAR ) != 0 ) ) {
    return false;
  }
  LONG cchUnicode = cch / sizeof( WCHAR );
  LPCWSTR pUnicode = reinterpret_cast<LPCWSTR>( pData );
  // 如果文件开头是UNICODE文件的标志就略去该标志
  if ( *pUnicode != PFX_EC_UNICODE_FILE_SIGNATURE ) {
    return false;
  } else {
    pUnicode++;
    cchUnicode--;
  }
  // 如果下面的字符中含有无效UNICODE字符就算是无效
  LPCWSTR pStart = pUnicode;
  LPCWSTR pEnd = pStart + cchUnicode;
  while ( ( pStart < pEnd )
          && ( *pStart != PFX_EC_UNICODE_FILE_SIGNATURE )
          && ( *pStart != 0 ) ) {
    pStart++;
  }
  return ( pStart >= pEnd );
}
BOOL CPfxEditDoc::FileIsUnicodeBigEndian( LPCSTR pData, LONG cch ) const {
  // 1. 如果文件为空，它可以是任何类型的文档
  // 2. 如果文件的数据不能被sizeof(WCHAR)整除，它也不是UNICODE文件
  if ( ( pData == NULL )
       || ( cch < sizeof( WCHAR ) )
       || ( cch % sizeof( WCHAR ) != 0 ) ) {
    return false;
  }
  LONG cchUnicode = cch / sizeof( WCHAR );
  LPCWSTR pUnicode = reinterpret_cast<LPCWSTR>( pData );
  // 如果文件开头是UNICODE文件的标志就略去该标志
  if ( *pUnicode != PFX_EC_UNICODE_FILE_BIG_ENDIAN_SIGNATURE ) {
    return false;
  } else {
    pUnicode++;
    cchUnicode--;
  }
  // 如果下面的字符中含有无效UNICODE字符就算是无效
  LPCWSTR pStart = pUnicode;
  LPCWSTR pEnd = pStart + cchUnicode;
  while ( ( pStart < pEnd )
          && ( *pStart != PFX_EC_UNICODE_FILE_BIG_ENDIAN_SIGNATURE )
          && ( *pStart != 0 ) ) {
    pStart++;
  }
  return ( pStart >= pEnd );
}
LONG CPfxEditDoc::FileGetConverterType( LPCSTR pData, LONG cch ) const {
  if ( FileIsUnicode( pData, cch ) ) {
    return PFX_EC_FILE_CONVERTER_UNICODE;
  }
  if ( FileIsUtf8( pData, cch ) ) {
    return PFX_EC_FILE_CONVERTER_UTF8;
  }
  if ( FileIsUnicodeBigEndian( pData, cch ) ) {
    return PFX_EC_FILE_CONVERTER_UNICODE_BIG_ENDIAN;
  } else {
    return PFX_EC_FILE_CONVERTER_ANSI;
  }
}
void CPfxEditDoc::FileLoadAnsi( CPfxEditDocFile& file ) {
  LONG cch = file.GetSize();
  LPCTSTR pData = static_cast<LPCTSTR>( file.GetBuffer() );
  if ( ( pData == NULL ) || ( cch < 0 ) ) {
    return ;
  }
  LPCTSTR pStart = pData;
  LPCTSTR pCur = pData;
  const LPCTSTR pEnd = pData + cch;
  LONG nDocSize = 0;
  LONG nLineIndex = 0;
  LONG nLineBreakTypeDos = 0;
  LONG nLineBreakTypeMac = 0;
  LONG nLineBreakTypeUnix = 0;
  m_nDocSize = 0;
  m_nDocLineCount = 1;
  m_nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_DOS;
  m_nLineBreakType = PFX_EC_LINE_BREAK_TYPE_DOS;
  while ( pCur < pEnd ) {
    TCHAR c = *pCur++;
    if ( ( PFX_EC_CHR_CR != c ) && ( PFX_EC_CHR_LF != c ) ) {
      continue;
    }
    // PFX_EC_LINE_BREAK_TYPE_DOS
    if ( ( PFX_EC_CHR_CR == c ) && ( pCur < pEnd ) && ( PFX_EC_CHR_LF == *pCur ) ) {
      pCur++; // 越过PFX_EC_CHR_LF
      cch = ( pCur - pStart );
      nDocSize += cch;
      cch -= PFX_EC_LINE_BREAK_SIZE_DOS;
      nLineBreakTypeDos++;
    }
    // PFX_EC_LINE_BREAK_TYPE_UNIX
    else if ( PFX_EC_CHR_LF == c ) {
      cch = ( pCur - pStart );
      nDocSize += cch;
      cch -= PFX_EC_LINE_BREAK_SIZE_UNIX;
      nLineBreakTypeUnix++;
    }
    // PFX_EC_LINE_BREAK_TYPE_MAC
    else {
      cch = ( pCur - pStart );
      nDocSize += cch;
      cch -= PFX_EC_LINE_BREAK_SIZE_MAC;
      nLineBreakTypeMac++;
    }
    TextInsertLine( nLineIndex, pStart, cch, PFX_EC_LF_NONE | PFX_EC_LF_HARDRETURN );
    nLineIndex++;
    pStart = pCur;
  }
  cch = pEnd - pStart;
  TextInsertLine( nLineIndex, pStart, cch, PFX_EC_LF_NONE );
  m_nDocSize = nDocSize + cch;
  m_nDocLineCount = nLineIndex + 1;
  // 由各个换行符的多少来决定文档是属于哪种类型，在同样多的情
  // 况下，以DOS型换行符优先
  if ( ( nLineBreakTypeUnix > nLineBreakTypeDos )
       && ( nLineBreakTypeUnix >= nLineBreakTypeMac ) ) {
    m_nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_UNIX;
    m_nLineBreakType = PFX_EC_LINE_BREAK_TYPE_UNIX;
  } else if ( ( nLineBreakTypeMac > nLineBreakTypeDos ) && ( nLineBreakTypeMac >= nLineBreakTypeUnix ) ) {
    m_nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_MAC;
    m_nLineBreakType = PFX_EC_LINE_BREAK_TYPE_MAC;
  } else {
    m_nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_DOS;
    m_nLineBreakType = PFX_EC_LINE_BREAK_TYPE_DOS;
  }
}
void CPfxEditDoc::FileLoadUtf8( CPfxEditDocFile& file ) {
  LONG cchUtf8 = file.GetSize();
  LPCSTR pUtf8 = static_cast<LPCSTR>( file.GetBuffer() );
  // 首先将UTF8数据转换成UNICODE，现在需要计算出存放解码后的数据需要的
  // 缓冲区长度
  CPfxEditDocUtf8Converter u8;
  LONG cchUnicode = u8.Utf8ToUnicode( pUtf8, cchUtf8, NULL, 0 );
  if ( cchUnicode == 0 ) {
    EventSendMsg( PFX_EE_CONVERT_ERR_ON_LOADING, 0 );
    return ;
  }
  CPfxEditDocFile file2;
  if ( !file2.MapMemory( cchUnicode * sizeof( WCHAR ) ) ) {
    EventSendMsg( PFX_EE_OUT_OF_MEMORY, 0 );
    return ;
  }
  // 执行转换操作
  cchUnicode = static_cast<LONG>( u8.Utf8ToUnicode( pUtf8, cchUtf8, static_cast<LPWSTR>( file2.GetBuffer() ), cchUnicode ) );
  if ( cchUnicode == 0 ) {
    EventSendMsg( PFX_EE_CONVERT_ERR_ON_LOADING, 0 );
  } else {
    file.Close();
    file2.SetSize( cchUnicode * sizeof( WCHAR ) );
    // 如果需要就要将BIG ENDIAN格式转换成LITTLE ENDIAN格式
    if ( PFX_EC_UNICODE_FILE_BIG_ENDIAN_SIGNATURE
         == ( *static_cast<LPCWSTR>( file2.GetBuffer() ) ) ) {
      LPWSTR pCur = static_cast<LPWSTR>( file2.GetBuffer() );
      const LPWSTR pEnd = pCur + cchUnicode;
      while ( pCur < pEnd ) {
        WCHAR ch = *pCur;
        *pCur++ = PFX_EC_UBE_TO_ULE( ch );
      }
    }
    FileLoadUnicode( file2 );
  }
}
void CPfxEditDoc::FileLoadUnicode( CPfxEditDocFile& file ) {
  // 截短到WCHAR单位
  LONG cchUnicode = ( file.GetSize() / sizeof( WCHAR ) );
  LPCWSTR pUnicode = static_cast<LPCWSTR>( file.GetBuffer() );
  // 如果文件开头是UNICODE文件的标志就略去该标志
  if ( ( cchUnicode > 0 )
       && ( *pUnicode == PFX_EC_UNICODE_FILE_SIGNATURE ) ) {
    pUnicode++;
    cchUnicode--;
  }
  if ( cchUnicode <= 0 )           //      等于零并不表示出错，仅表示这是个空文件
  {
    return ;
  }
  // 计算将整个文件转换成MBCS字符所需要的缓冲区空间
  LONG cchMultiByte = static_cast<LONG>( ::WideCharToMultiByte( CP_ACP, 0, pUnicode, static_cast<int>( cchUnicode ), NULL, 0, NULL, NULL ) );
  if ( cchMultiByte == 0 )           //      出错！
  {
    EventSendMsg( PFX_EE_CONVERT_ERR_ON_LOADING, 0 );
    return ;
  }
  CPfxEditDocFile file2; // 在虚拟内存中申请缓冲区
  if ( !file2.MapMemory( cchMultiByte ) ) {
    EventSendMsg( PFX_EE_OUT_OF_MEMORY, 0 );
    return ;
  }
  cchMultiByte = static_cast<LONG>( ::WideCharToMultiByte( CP_ACP, 0, pUnicode, static_cast<int>( cchUnicode ), static_cast<LPSTR>( file2.GetBuffer() ), cchMultiByte, NULL, NULL ) );
  if ( cchMultiByte == 0 ) {
    EventSendMsg( PFX_EE_CONVERT_ERR_ON_LOADING, 0 );
  } else {
    file.Close(); // 首先关闭原来的映射以节省内存空间
    file2.SetSize( cchMultiByte );
    FileLoadAnsi( file2 );
  }
}
void CPfxEditDoc::FileLoadUnicodeBigEndian( CPfxEditDocFile& file ) {
  // 由于原来的映射是只读的，所以必须创建一可写的缓冲区并将文件数据转
  // 换成LITTLE ENDIAN格式
  CPfxEditDocFile file2;
  if ( !file2.MapMemory( file.GetSize() ) ) {
    EventSendMsg( PFX_EE_OUT_OF_MEMORY, 0 );
    return ;
  }
  LPCWSTR pSrc = static_cast<LPCWSTR>( file.GetBuffer() );
  LPWSTR pDst = static_cast<LPWSTR>( file2.GetBuffer() );
  // 截短到WCHAR单位
  const LPWSTR pEnd = pDst + ( file2.GetSize() / sizeof( WCHAR ) );
  // 将BIG ENDIAN格式转换成LITTLE ENDIAN格式
  while ( pDst < pEnd ) {
    WCHAR ch = *pSrc++;
    *pDst++ = PFX_EC_UBE_TO_ULE( ch );
  }
  file.Close(); // 首先关闭原来的映射以节省内存空间
  FileLoadUnicode( file2 );
}
void CPfxEditDoc::FileSaveAnsi( CArchive& ar ) {
  // 写入文件时不应该使用映射文件机制，因为使用映射文件机制写入远程文
  // 件时不安全，系统无法保证文件与其它程序同步
  LONG nLineUpperBound = TextGetLineUpperBound();
  LONG nLineBreakSize = GetLineBreakSizeFromTypeA( m_nLineBreakType );
  LPCSTR pLineBreakData = GetLineBreakDataFromTypeA( m_nLineBreakType );
  for ( LONG nLineIndex = 0; nLineIndex <= nLineUpperBound; nLineIndex++ ) {
    const CPfxEditLineInfo* pli = TextGetLineAt( nLineIndex );
    if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
      ASSERT( !PFX_EC_IS_SUBLINE( pli ) );
      ar.Write( pli->pData, pli->nActLen );
    }
    if ( !PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      while ( ( ++nLineIndex < nLineUpperBound ) && !PFX_EC_IS_DRAW_HARDRETURN( TextGetLineAt( nLineIndex ) ) ) {
        //  do nothing
      }
    }
    if ( nLineIndex < nLineUpperBound ) {
      ar.Write( pLineBreakData, nLineBreakSize );
    }
  }
}
void CPfxEditDoc::FileSaveUtf8( CArchive& ar ) {
  // 找出转换成宽字符格式时需要的最大缓冲区长度，以免频繁改变缓冲区
  LONG nMaxBufferSizeUnicode = 0;
  LONG nLineUpperBound = TextGetLineUpperBound();
  for ( LONG nLineIndex = 0; nLineIndex <= nLineUpperBound; nLineIndex++ ) {
    const CPfxEditLineInfo* pli = TextGetLineAt( nLineIndex );
    if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
      LONG cch = static_cast<LONG>( ::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pli->pData, pli->nActLen, NULL, 0 ) );
      if ( cch == 0 ) {
        EventSendMsg( PFX_EE_CONVERT_ERR_ON_SAVING, 0 );
        return ;
      }
      nMaxBufferSizeUnicode = max( nMaxBufferSizeUnicode, cch );
    }
    // 跳过子行
    if ( !PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      while ( ( ++nLineIndex < nLineUpperBound ) && !PFX_EC_IS_DRAW_HARDRETURN( TextGetLineAt( nLineIndex ) ) ) {
        //  do nothing
      }
    }
  }
  // 写入UNICODE文件标志
  CHAR szUtf8[ 16 ];
  WORD wSignature = PFX_EC_UNICODE_FILE_SIGNATURE;
  CPfxEditDocUtf8Converter u8;
  ar.Write( szUtf8, u8.UnicodeToUtf8( ( LPCWSTR ) & wSignature, 1, szUtf8, sizeof( szUtf8 ) ) );
  if ( nMaxBufferSizeUnicode == 0 )           //      文档为空，这不是错误
  {
    return ;
  }
  // 假设每个UNICODE字符转换成UTF8时需要最大的缓冲区，即3个字节，现在
  // 可以求出需要的最大缓冲区长度为：
  LONG nMaxBufferSizeUtf8 = nMaxBufferSizeUnicode * 3;
  // 申请缓冲区进行转换操作
  CPfxEditDocFile file;
  if ( !file.MapMemory( ( nMaxBufferSizeUnicode * sizeof( WCHAR ) ) + nMaxBufferSizeUtf8 + 16 ) ) {
    EventSendMsg( PFX_EE_OUT_OF_MEMORY, 0 );
    return ;
  }
  CHAR szLineBreakData[ 32 ];
  LPWSTR pUnicode = static_cast<LPWSTR>( file.GetBuffer() );
  LPSTR pUtf8 = reinterpret_cast<LPSTR>( pUnicode + nMaxBufferSizeUnicode );
  LPCSTR pLineBreakData = szLineBreakData;
  UINT nLineBreakSize = u8.UnicodeToUtf8( GetLineBreakDataFromTypeW( m_nLineBreakType ), GetLineBreakSizeFromTypeW( m_nLineBreakType ), szLineBreakData, sizeof( szLineBreakData ) );
  // 开始转换
  for ( nLineIndex = 0; nLineIndex <= nLineUpperBound; nLineIndex++ ) {
    const CPfxEditLineInfo* pli = TextGetLineAt( nLineIndex );
    if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
      ASSERT( !PFX_EC_IS_SUBLINE( pli ) );
      UINT cchUnicode = static_cast<UINT>( ::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pli->pData, pli->nActLen, pUnicode, nMaxBufferSizeUnicode ) );
      if ( cchUnicode != 0 ) {
        UINT cchUtf8 = u8.UnicodeToUtf8( pUnicode, cchUnicode, pUtf8, nMaxBufferSizeUtf8 );
        ar.Write( pUtf8, cchUtf8 );
      }
    }
    if ( !PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      while ( ( ++nLineIndex < nLineUpperBound ) && !PFX_EC_IS_DRAW_HARDRETURN( TextGetLineAt( nLineIndex ) ) ) {
        //  do nothing
      }
    }
    if ( nLineIndex < nLineUpperBound ) {
      ar.Write( pLineBreakData, nLineBreakSize );
    }
  }
}
void CPfxEditDoc::FileSaveUnicode( CArchive& ar ) {
  // 找出转换成宽字符格式时需要的最大缓冲区长度，以免频繁改变缓冲区
  LONG nMaxBufferSize = 0;
  LONG nLineUpperBound = TextGetLineUpperBound();
  for ( LONG nLineIndex = 0; nLineIndex <= nLineUpperBound; nLineIndex++ ) {
    const CPfxEditLineInfo* pli = TextGetLineAt( nLineIndex );
    if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
      LONG cch = static_cast<LONG>( ::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pli->pData, pli->nActLen, NULL, 0 ) );
      if ( cch == 0 ) {
        EventSendMsg( PFX_EE_CONVERT_ERR_ON_SAVING, 0 );
        return ;
      }
      nMaxBufferSize = max( nMaxBufferSize, cch );
    }
    // 跳过子行
    if ( !PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      while ( ( ++nLineIndex < nLineUpperBound ) && !PFX_EC_IS_DRAW_HARDRETURN( TextGetLineAt( nLineIndex ) ) ) {
        //  do nothing
      }
    }
  }
  // 写入UNICODE文件标志
  WCHAR wSignature = PFX_EC_UNICODE_FILE_SIGNATURE;
  ar.Write( &wSignature, sizeof( wSignature ) );
  if ( nMaxBufferSize == 0 )           //      文档为空，这不是错误
  {
    return ;
  }
  // 申请缓冲区进行转换操作
  CPfxEditDocFile file;
  if ( !file.MapMemory( nMaxBufferSize * sizeof( WCHAR ) ) ) {
    EventSendMsg( PFX_EE_OUT_OF_MEMORY, 0 );
    return ;
  }
  LPWSTR pBuffer = static_cast<LPWSTR>( file.GetBuffer() );
  LONG nLineBreakSize = GetLineBreakSizeFromTypeW( m_nLineBreakType );
  LPCWSTR pLineBreakData = GetLineBreakDataFromTypeW( m_nLineBreakType );
  // 开始转换
  for ( nLineIndex = 0; nLineIndex <= nLineUpperBound; nLineIndex++ ) {
    const CPfxEditLineInfo* pli = TextGetLineAt( nLineIndex );
    if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
      ASSERT( !PFX_EC_IS_SUBLINE( pli ) );
      LONG cch = static_cast<LONG>( ::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pli->pData, pli->nActLen, pBuffer, nMaxBufferSize ) );
      if ( cch != 0 ) {
        ar.Write( pBuffer, cch * sizeof( WCHAR ) );
      }
    }
    if ( !PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      while ( ( ++nLineIndex < nLineUpperBound ) && !PFX_EC_IS_DRAW_HARDRETURN( TextGetLineAt( nLineIndex ) ) ) {
        //  do nothing
      }
    }
    if ( nLineIndex < nLineUpperBound ) {
      ar.Write( pLineBreakData, nLineBreakSize * sizeof( WCHAR ) );
    }
  }
}
void CPfxEditDoc::FileSaveUnicodeBigEndian( CArchive& ar ) {
  // 找出转换成宽字符格式时需要的最大缓冲区长度，以免频繁改变缓冲区
  LONG nMaxBufferSize = 0;
  LONG nLineUpperBound = TextGetLineUpperBound();
  for ( LONG nLineIndex = 0; nLineIndex <= nLineUpperBound; nLineIndex++ ) {
    const CPfxEditLineInfo* pli = TextGetLineAt( nLineIndex );
    if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
      LONG cch = static_cast<LONG>( ::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pli->pData, pli->nActLen, NULL, 0 ) );
      if ( cch == 0 ) {
        EventSendMsg( PFX_EE_CONVERT_ERR_ON_SAVING, 0 );
        return ;
      }
      nMaxBufferSize = max( nMaxBufferSize, cch );
    }
    // 跳过子行
    if ( !PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      while ( ( ++nLineIndex < nLineUpperBound ) && !PFX_EC_IS_DRAW_HARDRETURN( TextGetLineAt( nLineIndex ) ) ) {
        //  do nothing
      }
    }
  }
  // 写入UNICODE BIG ENDIAN文件标志
  WCHAR wSignature = PFX_EC_UNICODE_FILE_BIG_ENDIAN_SIGNATURE;
  ar.Write( &wSignature, sizeof( wSignature ) );
  if ( nMaxBufferSize == 0 )           //      文档为空，这不是错误
  {
    return ;
  }
  // 申请缓冲区进行转换操作
  CPfxEditDocFile file;
  if ( !file.MapMemory( nMaxBufferSize * sizeof( WCHAR ) ) ) {
    EventSendMsg( PFX_EE_OUT_OF_MEMORY, 0 );
    return ;
  }
  WCHAR szLineBreakData[ 16 ];
  LPWSTR pBuffer = static_cast<LPWSTR>( file.GetBuffer() );
  LONG nLineBreakSize = GetLineBreakSizeFromTypeW( m_nLineBreakType );
  LPCWSTR pLineBreakData = szLineBreakData;
  memcpy( szLineBreakData, GetLineBreakDataFromTypeW( m_nLineBreakType ), nLineBreakSize * sizeof( WCHAR ) );
  // 反转字节顺序
  for ( LONG i = 0; i < nLineBreakSize; i++ ) {
    WCHAR w = szLineBreakData[ i ];
    szLineBreakData[ i ] = PFX_EC_ULE_TO_UBE( w );
  }
  // 开始转换
  for ( nLineIndex = 0; nLineIndex <= nLineUpperBound; nLineIndex++ ) {
    const CPfxEditLineInfo* pli = TextGetLineAt( nLineIndex );
    if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
      ASSERT( !PFX_EC_IS_SUBLINE( pli ) );
      LONG cch = static_cast<LONG>( ::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pli->pData, pli->nActLen, pBuffer, nMaxBufferSize ) );
      if ( cch != 0 ) {
        //  反转字节顺序
        for ( LPWSTR s1 = pBuffer, s2 = pBuffer + cch; s1 < s2; s1++ ) {
          WCHAR w = *s1;
          *s1 = PFX_EC_ULE_TO_UBE( w );
        }
        ar.Write( pBuffer, cch * sizeof( WCHAR ) );
      }
    }
    if ( !PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      while ( ( ++nLineIndex < nLineUpperBound ) && !PFX_EC_IS_DRAW_HARDRETURN( TextGetLineAt( nLineIndex ) ) ) {
        //  do nothing
      }
    }
    if ( nLineIndex < nLineUpperBound ) {
      ar.Write( pLineBreakData, nLineBreakSize * sizeof( WCHAR ) );
    }
  }
}
CPfxEditView::CPfxEditViewClipboard::CPfxEditViewClipboard( CControlUI* pWndOwner, UINT uFormat, BOOL bGet ) {
  m_hCBData = NULL;
  m_bOpened = false;
  m_bForGet = bGet;
  m_uFormat = uFormat;
  m_hWndOwner = pWndOwner;
  ASSERT( m_hWndOwner );
}
CPfxEditView::CPfxEditViewClipboard::~CPfxEditViewClipboard() {
  Close();
}
BOOL CPfxEditView::CPfxEditViewClipboard::Attach( HGLOBAL hGlobal ) {
  if ( ( hGlobal != NULL )
       && ( ::GlobalFlags( hGlobal ) != GMEM_INVALID_HANDLE ) ) {
    Close();
    m_hCBData = hGlobal;
  }
  return ( hGlobal == m_hCBData );
}
HGLOBAL CPfxEditView::CPfxEditViewClipboard::Detach() {
  HGLOBAL hGlobal = m_hCBData;
  m_hCBData = NULL;
  if ( hGlobal != NULL ) {
    UnLock( hGlobal );
  }
  return hGlobal;
}
BOOL CPfxEditView::CPfxEditViewClipboard::Open() {
  if ( !m_bOpened ) {
    m_bOpened = ::OpenClipboard( m_hWndOwner->M_HWND );
  }
  return m_bOpened;
}
void CPfxEditView::CPfxEditViewClipboard::Close() {
  // 这次是用于获取剪贴板数据
  if ( m_bForGet ) {
    if ( m_hCBData != NULL ) {
      UnLock( m_hCBData );
      m_hCBData = NULL;
    }
  }
  // 否则这次是用来设置剪贴板数据
  else {
    if ( m_hCBData != NULL ) {
      VERIFY( ::GlobalFree( m_hCBData ) == NULL );
      m_hCBData = NULL;
    }
  }
  if ( m_bOpened ) {
    m_bOpened = !::CloseClipboard();
  }
  ASSERT( !m_bOpened );
  ASSERT( m_hCBData == NULL );
}
BOOL CPfxEditView::CPfxEditViewClipboard::UnLock( HGLOBAL hGlobal ) {
  UINT uFlags = ::GlobalFlags( hGlobal );
  if ( uFlags == GMEM_INVALID_HANDLE ) {
    return false;
  }
  // 如果据柄已经被锁定，现在需要解锁
  for ( UINT uLockCount = ( uFlags & GMEM_LOCKCOUNT ); uLockCount > 0; uLockCount-- ) {
    ::GlobalUnlock( hGlobal );
  }
  return true;
}
LPCTSTR CPfxEditView::CPfxEditViewClipboard::GetClipboardData() {
  ASSERT( m_bForGet );
  ASSERT( m_hCBData == NULL );
  LPCTSTR pData = NULL;
  if ( Open() ) {
    m_hCBData = ::GetClipboardData( m_uFormat );
    if ( m_hCBData != NULL ) {
      pData = Lock( m_hCBData );
    }
  }
  return pData;
}
LPTSTR CPfxEditView::CPfxEditViewClipboard::GetBuffer( LONG nBufLen, BOOL bAppend ) {
  ASSERT( !m_bForGet );
  ASSERT( m_hCBData == NULL );
  LPTSTR pBuffer = NULL;
  if ( !bAppend )           //      追加复制
  {
    // 申请一缓冲区，用来存放已有的和新的剪贴板数据
    m_hCBData = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, ( ( nBufLen + 1 ) * sizeof( TCHAR ) ) );
    // 复制已有的剪贴板数据
    if ( m_hCBData != NULL ) {
      pBuffer = Lock( m_hCBData );
    }
  }
  else {
    // 打开剪贴板，查询数据长度并获取数据
    HANDLE hData = NULL;
    LONG nDataLen = 0;
    LPCTSTR pData = NULL;
    if ( Open() ) {
      hData = ::GetClipboardData( m_uFormat );
      if ( hData != NULL ) {
        pData = Lock( hData );
      }
      if ( pData != NULL ) {
        nDataLen = _tcslen( pData );
      }
    }
    // 申请一缓冲区，用来存放已有的和新的剪贴板数据
    m_hCBData = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, ( ( nBufLen + nDataLen + 1 ) * sizeof( TCHAR ) ) );
    // 复制已有的剪贴板数据
    if ( m_hCBData != NULL ) {
      pBuffer = Lock( m_hCBData );
      if ( ( pBuffer != NULL ) && ( pData != NULL ) ) {
        memcpy( pBuffer, pData, nDataLen * sizeof( TCHAR ) );
        pBuffer += nDataLen;
      }
    }
    // 打开的剪贴板会在对象析构的时候关闭
    UnLock( hData );
  }
  return pBuffer;
}
BOOL CPfxEditView::CPfxEditViewClipboard::SetClipboardData() {
  ASSERT( !m_bForGet );
  ASSERT( m_hCBData != NULL );
  if ( !UnLock( m_hCBData ) || !Open() ) {
    return false;
  }
  // 清空剪贴板原来的数据
  ::EmptyClipboard();
  if ( ::SetClipboardData( m_uFormat, m_hCBData ) == m_hCBData ) {
    m_hCBData = NULL;
  }
  return ( m_hCBData == NULL );
}
// CPfxEditViewDataSource...
CPfxEditView::CPfxEditViewDataSource::~CPfxEditViewDataSource() {
  m_pOwner->SetDragging( false );
}
BOOL CPfxEditView::CPfxEditViewDataSource::OnRenderGlobalData( LPFORMATETC lpFormatEtc, HGLOBAL* phGlobal ) {
  UNUSED( lpFormatEtc );
  if ( *phGlobal != NULL ) {
    return false;
  }
  *phGlobal = m_pOwner->DragDropRender( true, false );
  return ( *phGlobal != NULL );
}
// CPfxEditView...
#if 0
IMPLEMENT_DYNCREATE( CPfxEditView, CView )
BEGIN_MESSAGE_MAP( CPfxEditView, CView )
//{{AFX_MSG_MAP(CPfxEditView)
ON_WM_MOUSEMOVE()
ON_WM_MOUSEWHEEL()
ON_WM_HSCROLL()
ON_WM_VSCROLL()
ON_WM_PAINT()
ON_WM_SIZE()
ON_WM_CHAR()
ON_WM_KEYDOWN()
ON_WM_SETFOCUS()
ON_WM_KILLFOCUS()
ON_WM_CAPTURECHANGED()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_LBUTTONDBLCLK()
ON_WM_CREATE()
ON_WM_RBUTTONDOWN()
ON_WM_SETCURSOR()
ON_WM_DESTROY()
//}}AFX_MSG_MAP
// MBCS support!
ON_MESSAGE( WM_IME_COMPOSITION, OnIMEComposition )
// Standard printing commands
ON_COMMAND( ID_FILE_PRINT, CView::OnFilePrint )
ON_COMMAND( ID_FILE_PRINT_DIRECT, CView::OnFilePrint )
ON_COMMAND( ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview )
END_MESSAGE_MAP()
#endif
CPfxEditView::CPfxEditView() {
  m_pdb = NULL;
  m_ppc = NULL;
  m_pti = &PFX_EC_DATA.m_defTypeInfo;
  OnInitialize();
  m_pdb = new CPfxEditDoc;
  m_pdb->m_arrViews[ 0 ] = this;
  OnCreate();
}
CPfxEditView::~CPfxEditView() {
  delete m_pdb;
}
// OLE拖放支持
DROPEFFECT CPfxEditView::OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint pt ) {
  if ( IsReadOnly() ) {
    return DROPEFFECT_NONE;
  }
  DestroyCaret();
  CreateSolidCaret( ( GetSystemMetrics( SM_CXBORDER ) * 2 ), PFX_EC_DATA.m_sizeCaret.cy );
  ShowCaret();
  return OnDragOver( pDataObject, dwKeyState, pt );
}
DROPEFFECT CPfxEditView::OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint pt ) {
  if ( IsReadOnly() || !pDataObject->IsDataAvailable( CF_TEXT ) ) {
    return DROPEFFECT_NONE;
  }
  CPoint ptTextPos;
  ptTextPos.x = TransViewPosToTextPosX( pt.x );
  ptTextPos.y = TransViewPosToTextPosY( pt.y );
  CPoint ptEditPos;
  ptEditPos.y = TransTextPosToLinePos( ptTextPos.y );
  ptEditPos.x = TransTextPosToCharPos( ptEditPos.y, ptTextPos.x );
  // 光标位置对齐到行的顶部
  ptTextPos.y = TransLinePosToTextPos( ptEditPos.y );
  CaretSetPos( ptTextPos );
  return ( ( dwKeyState & MK_CONTROL ) ? DROPEFFECT_COPY : DROPEFFECT_MOVE );
}
void CPfxEditView::OnDragLeave() {
  if ( !IsReadOnly() ) {
    DestroyCaret();
  }
}
BOOL CPfxEditView::OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint pt ) {
  DestroyCaret(); // 删除插入符，它并不是正常插入符
  PFX_EC_DATA.m_hDataDragDropCopied = NULL;
  if ( IsReadOnly() || !pDataObject->IsDataAvailable( CF_TEXT ) ) {
    return false;
  }
  CPfxEditView* pSiblingView = GetSiblingView();
  if ( ( pSiblingView != NULL ) && pSiblingView->IsDragging() ) {
    // 将本视图的坐标转换成文本坐标，然后在兄弟视图中再由文本坐
    // 标转换成视图坐标
    pt.y = TransViewPosToTextPosY( pt.y );
    pt.x = TransViewPosToTextPosX( pt.x );
    pt.y = pSiblingView->TransTextPosToViewPosY( pt.y );
    pt.x = pSiblingView->TransTextPosToViewPosX( pt.x );
    return pSiblingView->OnDrop( pDataObject, dropEffect, pt );
  }
  // 如果“放”在选择范围内，并且数据源提供者是自己（在自己的选择范围
  // 内用鼠标拖放）则不必继续操作
  if ( IsInsideSelect( pt ) && IsDragging() ) {
    return false;
  }
  HANDLE hGlobal = pDataObject->GetGlobalData( CF_TEXT );
  if ( hGlobal == NULL ) {
    return false;
  }
  CPoint ptTextPos;
  ptTextPos.x = TransViewPosToTextPosX( pt.x );
  ptTextPos.y = TransViewPosToTextPosY( pt.y );
  CPoint ptEditPos;
  ptEditPos.y = TransTextPosToLinePos( ptTextPos.y );
  ptEditPos.x = TransTextPosToCharPos( ptEditPos.y, ptTextPos.x );
  CPfxEditViewUndoManager um( this, ( ( dropEffect & DROPEFFECT_MOVE ) ? PFX_EC_UNDO_DRAGDROP_MOVE : PFX_EC_UNDO_DRAGDROP_COPY ) );
  if ( ( ( dropEffect & DROPEFFECT_MOVE ) == DROPEFFECT_MOVE ) && IsDragging() ) {
    DragDropRemove( ptEditPos );
  }
  // 告诉处理数据源的函数“数据被自己接收”
  SetDragDropHandled( IsDragging() );
  BOOL bResult = DragDropPaste( hGlobal, ptEditPos );
  // 由于数据源可能是自己也可能是其他窗口，而落点是随机的，所以最好的
  // 刷新方法是整个窗口一起刷新
  UpdateAllViews( PFX_EC_UT_LAYOUT, 0, 0 );
  return bResult;
}
void CPfxEditView::OnInitialize() {
  m_bOverride = false;
  m_bTrackingColSelect = false;
  m_bTrackingMouse = false;
  m_bDragging = false;
  m_bDelayRecalcWrapping = false;
  m_ptEditPos.x = m_ptEditPos.y = 0;
  m_ptLogiPos.x = m_ptLogiPos.y = 0;
  m_ptTextPos.x = m_ptTextPos.y = 0;
  m_ptCaretRefer.x = m_ptCaretRefer.y = 0;
  m_ptFirstVisible.x = m_ptFirstVisible.y = 0;
  m_sizeVisible.cx = m_sizeVisible.cy = 0;
  m_nSelSize = 0;
  m_nSelLineFrom = m_nSelLineTo = PFX_EC_INVALID_SELECT;
  m_nSelMode = PFX_EC_SM_NORMAL;
}
void CPfxEditView::OnInitialUpdate() {
  OnInitialize();
  UpdateLayout();
  UpdateCurrentPosition( m_ptTextPos.x, true, true );
  if ( IsFirstSiblingView() ) {
    SetViewWrapping( GetWrapStyle(), GetWrapWidth() );
  }
#ifdef _SYNC_SCROLLING
  // 当使用同步滚动功能时，如果已经有兄弟视图，当前的水平位置必须与兄
  // 弟视图一致，所以就不需要更新水平滚动条
  CPfxEditView* pSiblingView = GetSiblingView();
  if ( pSiblingView == NULL ) {
    ScrollBarUpdate( SB_HORZ );
    ScrollBarSetPos( SB_HORZ, 0 );
  } else {
    m_ptFirstVisible.x = pSiblingView->m_ptFirstVisible.x;
  }
#else
  ScrollBarUpdate( SB_HORZ );
  ScrollBarSetPos( SB_HORZ, 0 );
#endif
  ScrollBarUpdate( SB_VERT );
  ScrollBarSetPos( SB_VERT, 0 );
  // CView::OnInitialUpdate();
}
void CPfxEditView::SetSpacingLines( LONG nSpacing ) {
  ASSERT( nSpacing >= 0 );
  PFX_EC_DATA.m_nSpacingLines = nSpacing;
  PFX_EC_DATA.m_nLineHeight = PFX_EC_DATA.m_nCharHeight + nSpacing;
}
void CPfxEditView::SetSpacingLineNumberAndText( LONG nSpacing ) {
  ASSERT( nSpacing >= 0 );
  PFX_EC_DATA.m_nSpacingLineNumberAndText = nSpacing;
}
// BOOL SetFont(HFONT hFont)
// 设置控件的显示字体
// HFONT hFont - 新字体据柄
// hFont是共享资源，控件并不负责删除它，属主应负责删除
// 调用者必须给当前视窗口重新设置输入焦点以更新插入符位置
BOOL CPfxEditView::SetFont( HFONT hFont ) {
  // 如果下面的代码修改了视图的私有数据，无论新的字体据柄是否与旧的相同都要更新
  if ( hFont == NULL ) {
    hFont = ( HFONT ) ::GetStockObject( SYSTEM_FONT );
  }
  if ( hFont == PFX_EC_DATA.m_hFontText ) {
    return true;
  }
  // BLOCK：获取字体的相关属性
  LONG i;
  CSize size;
  // CControlUI* pWndDesktop = GetDesktopWindow();
  CWindowDC dc( 0 );
  HGDIOBJ hFontOld = dc.SelectObject( ( HGDIOBJ ) hFont );
  TEXTMETRIC tm;
  dc.GetTextMetrics( &tm );
  for ( PFX_EC_DATA.m_nCharWidthMaxNum = 0, i = '0'; i <= '9'; i++ ) {
    size = dc.GetTextExtent( ( LPCTSTR ) & i, 1 );
    PFX_EC_DATA.m_nCharWidthMaxNum = max( PFX_EC_DATA.m_nCharWidthMaxNum, size.cx );
  }
  i = PFX_EC_CHR_SPACE;
  size = dc.GetTextExtent( ( LPCTSTR ) & i, 1 );
  PFX_EC_DATA.m_nCharWidthSpace = size.cx;
  i = PFX_EC_CHR_TAB;
  size = dc.GetTextExtent( ( LPCTSTR ) & i, 1 );
  PFX_EC_DATA.m_nCharWidthTab = size.cx;
  dc.SelectObject( hFontOld );
  PFX_EC_DATA.m_hFontText = hFont;
  PFX_EC_DATA.m_sizeCaret.cy = tm.tmHeight;
  PFX_EC_DATA.m_sizeCaret.cx = 2 * GetSystemMetrics( SM_CXBORDER );
  PFX_EC_DATA.m_nCharHeight = tm.tmHeight;
  PFX_EC_DATA.m_nCharWidthAve = tm.tmAveCharWidth;
  PFX_EC_DATA.m_nCharWidthMax = max( tm.tmMaxCharWidth, ( tm.tmAveCharWidth * 2 ) );
  PFX_EC_DATA.m_nCharWidthLineBreak = max( PFX_EC_MIN_LINE_BREAK_WIDTH, ( PFX_EC_DATA.m_nCharHeight * 2 / 3 ) );
  PFX_EC_DATA.m_nLineHeight = PFX_EC_DATA.m_nCharHeight + PFX_EC_DATA.m_nSpacingLines;
  // 设置标尺尺寸
  PFX_EC_DATA.m_nRulerCharHeight = ( tm.tmHeight - tm.tmInternalLeading );
  PFX_EC_DATA.m_nRulerCharWidthAve = tm.tmAveCharWidth;
  PFX_EC_DATA.m_nRulerHeight = max( PFX_EC_DATA.m_nRulerCharHeight, PFX_EC_RULER_MIN_HEIGHT );
  return true;
}
// CPfxEditView 打印处理
void CPfxEditView::OnPrepareDC( HDC hDC, CPrintInfo* pInfo ) {
#if 0
  ASSERT( pDC.IsPrinting() );
  // 如果不是预览，并且只打印奇数页或偶数页，就要映射相应页
  if ( ( m_ppc != NULL ) && !pInfo->m_bPreview ) {
    switch ( m_ppc->m_nPrintRange ) {
    case PFX_EC_PRINT_PAGES_ODD:
      pInfo->m_nCurPage |= 1;
      break;
    case PFX_EC_PRINT_PAGES_EVEN:
      pInfo->m_nCurPage += ( pInfo->m_nCurPage % 2 );
      break;
    case PFX_EC_PRINT_PAGES_ALL:
    default:           // 贯通
      break;
    }
    // 需要停止打印？
    if ( pInfo->m_nCurPage > ( UINT ) m_ppc->m_nToPage ) {
      pInfo->m_bContinuePrinting = false;
    }
  }
#endif
}
void CPfxEditView::OnPrint( HDC hDC, CPrintInfo* pInfo ) {
#if 0
  if ( m_ppc == NULL ) {
    return ;
  }
  CPfxEditViewPrintContext& pc = *m_ppc;
  // 计数应该从零开始
  LONG nCurPage = max( 0, ( pInfo->m_nCurPage - 1 ) );
  nCurPage = min( nCurPage, ( pc.m_nToPage - 1 ) );
  // 计算每页的第一行和最后一行
  LONG nCurPageFirstLine = nCurPage * pc.m_nLinesPerPage;
  LONG nCurPageLastLine = nCurPageFirstLine + pc.m_nLinesPerPage;
  pc.m_nCurPage = nCurPage;
  pc.m_nCurPageFirstLine = min( nCurPageFirstLine, pc.m_nMaxLines );
  pc.m_nCurPageLastLine = min( nCurPageLastLine, pc.m_nMaxLines );
  // 设置DC属性
  CFontH pFontOld = pDC.GetCurrentFont();
  CPen* pPenOld = pDC.GetCurrentPen();
  LONG nMapMode = pDC.SetMapMode( MM_TEXT );
  LONG nBkMode = pDC.SetBkMode( TRANSPARENT );
  UINT nTextAlign = pDC.GetTextAlign();
  COLORREF clrText = pDC.GetTextColor();
  COLORREF clrTextBk = pDC.GetBkColor();
  TRY
  {
    if ( pc.IsPrintHeader() || pc.IsPrintFooter() ) {
    pDC.SelectObject( ( pc.FontHeaderAndFooter() == NULL ) ? pc.GetFontText() : pc.GetFontHeaderAndFooter() );
      // 绘制标题
      if ( pc.IsPrintHeader() ) {
        PrintHeader( pDC, pc );
      }
      // 绘制脚注
      if ( pc.IsPrintFooter() ) {
        PrintFooter( pDC, pc );
      }
    }
    // 绘制文本
    pDC.SelectObject( pc.GetFontText() );
    if ( pc.IsPrintHilighted() && IsDrawHilighted() ) {
    PrintHilighted( pDC, pc );
    } else {
      PrintPlainText( pDC, pc );
    }
    // 绘制边框线
    PrintMargin( pInfo->m_rectDraw, pDC, pc );
  }
  CATCH_ALL( e ) {
    LONG nCode = m_pdb->EventCodeFromException( e );
    e->Delete();
    m_pdb->EventSendMsg( nCode, 0 );
  }
  END_CATCH_ALL
  // 恢复DC属性
  pDC.SelectObject( pFontOld );
  pDC.SelectObject( pPenOld );
  pDC.SetMapMode( nMapMode );
  pDC.SetBkMode( nBkMode );
  pDC.SetTextAlign( nTextAlign );
  pDC.SetTextColor( clrText );
  pDC.SetBkColor( clrTextBk );
#endif
}
void CPfxEditView::OnBeginPrinting( HDC hDC, CPrintInfo* pInfo ) {
#if 0
  // 动态申请一缓冲区--CPfxEditViewPrintContext
  PrintReleaseResource();
  m_ppc = new CPfxEditViewPrintContext;
  if ( m_ppc == NULL ) {
    return ;
  }
  CPfxEditViewPrintContext& pc = *m_ppc;
  CClientDC1 dc( 0 );
  HDC hdcDisplay = dc;
  HDC hdcPrinter = pDC;
  // 初始化默认数值
  pc.SetDC( pdcDisplay, pdcPrinter );
  pc.m_pFontDisplay = ( GetFont() );
  pc.m_bPreview = pInfo->m_bPreview;
  pc.m_bPrintHilighted = IsDrawHilighted();
  pc.m_bPrintLineNumber = IsDrawLineNumber();
  pc.m_bPrintHeaderSeparator = true;
  pc.m_bPrintFooterSeparator = true;
  // 默认地全部打印（全部是指用户指定的范围内的所有页数）
  pc.m_nPrintRange = PFX_EC_PRINT_PAGES_ALL;
  pc.m_nFromPage = ( LONG ) ( pInfo->GetFromPage() );
  pc.m_nToPage = ( LONG ) ( pInfo->GetToPage() );
  // 先将颜色表复制过来
  for ( int i = 0; i < PFX_EC_SIZEOF( pc.m_arrKeyClasses ); i++ ) {
    pc.m_arrKeyClasses[ i ].m_color = m_pti->m_arrKeyClasses.m_arrClasses[ i ].m_color;
    pc.m_arrKeyClasses[ i ].m_bEmpty = m_pti->m_arrKeyClasses.m_arrClasses[ i ].m_bEmpty;
  }
  // 调整颜色
  PrintCheckColors( pc );
  // 制表宽度，与屏幕显示相同
  pc.m_nTabSize = GetTabSize();
  // 默认地自动将超出右边框的内容折回显示在下一行
  pc.m_nWrapStyle = GetWrapStyle();
  // 正文与页眉距离，不打印页眉时自动为零
  pc.m_nSpacingHeaderAndText = pc.PrinterLogicUnitFromDisplayY( PFX_EC_DEF_SPACING_HEADER_AND_TEXT );
  // 正文与页脚距离，不打印页脚时自动为零
  pc.m_nSpacingFooterAndText = pc.PrinterLogicUnitFromDisplayY( PFX_EC_DEF_SPACING_FOOTER_AND_TEXT );
  // 获取文档完整的路径名，如果没有路径名则表明文档还没有命名，这时使
  // 用标题名
  pc.m_strPathName = GetDocument() ->GetPathName();
  if ( pc.m_strPathName.IsEmpty() ) {
    pc.m_strPathName = GetDocument() ->GetTitle();
  }
  // 时间
  ::GetLocalTime( &pc.m_time );
  // 计算可打印范围
  pc.m_rectDraw.SetRect( 0, 0, pdcPrinter.GetDeviceCaps( HORZRES ), pdcPrinter.GetDeviceCaps( VERTRES ) );
  pdcPrinter.DPtoLP( &pc.m_rectDraw );
  // 初始化边框
  pc.m_rectMargin.SetRectEmpty();
  // 初始化失败，打印不再继续
  if ( !OnSetupPrintInfo( m_ppc ) ) {
    pInfo->m_bContinuePrinting = false;
    PrintReleaseResource();
    return ;
  }
  // 如果派生类没有指定打印字体，就映射显示字体而创建打印字体
  if ( pc.m_fontText == NULL ) {
    pc.SetMapDisplayFont();
  }
  // 派生类可能会改变颜色等设置，再次调整颜色
  PrintCheckColors( pc );
  // 设置好打印DC的属性
  CFontH pFontOldPrinter = pdcPrinter.SelectObject( pc.GetFontText() );
  CFontH pFontOldDisplay = pdcDisplay.GetCurrentFont();
  TRY
  {
    // 计算页面格式、布局
    PrintCalcLayout( pdcPrinter, pc );
    // 格式化文本数据，这种格式化只是获取主缓冲区行信息的一个副
    // 本，不会修改主缓冲区数据
    PrintFormatText( pdcPrinter, pc );
    // 设置页范围
    pc.m_nLineLastParsed = -1;
    // 页总数
    LONG nLinesPerPage = max( 1, pc.m_nLinesPerPage );
    pc.m_nTotalPages = ( pc.m_nMaxLines + ( nLinesPerPage - 1 ) ) / nLinesPerPage;
    // 设置最大最小页范围
    UINT nFromPage = ( UINT ) pc.m_nFromPage;
    UINT nToPage = ( UINT ) pc.m_nToPage;
    UINT nMaxPages = ( UINT ) pc.m_nTotalPages;
    nFromPage = max( nFromPage, 0 );
    nFromPage = min( nFromPage, nMaxPages );
    nToPage = min( nToPage, nMaxPages );
    nToPage = max( nToPage, 0 );
    if ( nFromPage > nToPage ) {
    LONG i = nFromPage;
    nFromPage = nToPage;
    nToPage = i;
  }
  pc.m_nFromPage = nFromPage;
  pc.m_nToPage = nToPage;
  pInfo->SetMinPage( nFromPage );
  pInfo->SetMaxPage( nToPage );
  pInfo->m_pPD->m_pd.nFromPage = ( USHORT ) nFromPage;
  pInfo->m_pPD->m_pd.nToPage = ( USHORT ) nToPage;
}
CATCH_ALL( e ) {
    LONG nCode = m_pdb->EventCodeFromException( e );
    e->Delete();
    m_pdb->EventSendMsg( nCode, 0 );
  }
  END_CATCH_ALL
  // 恢复DC属性
  pdcPrinter.SelectObject( pFontOldPrinter );
  pdcDisplay.SelectObject( pFontOldDisplay );
  // 修正一些不需要的参数，以免错误使用
  pc.m_pdcDisplay = NULL;
  pc.m_pdcPrinter = NULL;
#endif
}
void CPfxEditView::OnEndPrinting( HDC /*pDC*/, CPrintInfo* pInfo ) {
  ASSERT( pInfo != NULL );
  UNUSED( pInfo );
  PrintReleaseResource();
}
// CPfxEditView 窗口消息处理
INT CPfxEditView::OnCreate() {
  m_pdb = static_cast<CPfxEditDoc*>( GetDocument() );
  ASSERT_KINDOF( CPfxEditDoc, m_pdb );
  // 如果本视图是文档的第一个视图，那么GetTypeInfoPtr()应该返回NULL，
  // 这时候需要给它赋与默认的设置，但如果本视图不是文档的第一个视图，
  // 则GetTypeInfoPtr()不应该返回NULL，这时候文档的所有视图都使用同一
  // 种设置以保持一致的表现
  m_pti = const_cast<CPfxEditTypeInfo*>( m_pdb->GetTypeInfoPtr() );
  if ( m_pti == NULL ) {
    m_pti = &PFX_EC_DATA.m_defTypeInfo;
  }
  // 用户还没有使用SetFont()设置字体，用系统默认字体作显示字体
  if ( GetFont() == NULL ) {
    SetFont( ( HFONT ) NULL );
  }
  // 注册OLE拖放
  // m_odt.Register( this );
  return 0;
}
void CPfxEditView::OnDestroy() {
  // MFC7中打印预览时不一定会调用OnEndPrinting()（在MDI程序中，MFC7打印
  // 预览时HOOK住的不是主框架的窗口过程，而是MDI子窗口的窗口过程，当用
  // 户直接点击主窗口的关闭按钮时执行的是关闭程序，而不是关闭打印预览窗
  // 口），所以要在最后释放打印资源
  PrintReleaseResource();
}
void CPfxEditView::DoPaint( HDC hDC, const RECT& rcPaint ) {
  // if ( !GetUpdateRect( NULL ) ) {    return ;  }
  // 如果不加上一，当最后一可见行底部被剪切时可能导致这一行不会被正确刷新
  if ( IsDrawHilighted() && ParserPreParse( ( GetLastVisibleLine() + 1 ), m_pdb->TextGetLineUpperBound(), m_pdb->m_nLineLastParsedButCanceled, m_pdb->m_nLineLastParsed, m_pdb->TextGetLineAt( 0 ) ) ) {
    Invalidate( false );
  }
  CDCH dcSrc = hDC; // device context for painting
  CDC dcMem;
  CBitmap bitmap;
  CPfxEditViewPaintContext pc;
  CPoint pt( 0, m_rectText.top );
  CRect rectUpdate;
  dcSrc.GetClipBox( &rectUpdate );
  GetClientRect( rectUpdate );
  pc.m_nFontHeight = PFX_EC_DATA.m_nCharHeight;
  pc.m_nLineHeight = PFX_EC_DATA.m_nLineHeight;
  pc.m_nSpacingLines = GetLineTextVertOffset();
  // 目前只定义了LEFT成员
  pc.m_rectIndent.left = m_pdb->m_rectIndent.left;
  LONG nLinesOfsTop = ( ( rectUpdate.top < pt.y ) ? 0 : ( ( rectUpdate.top - pt.y ) / pc.m_nLineHeight ) );
  LONG nLinesOfsBottom = ( ( rectUpdate.bottom < pt.y ) ? -1 : ( ( rectUpdate.bottom - pt.y ) / pc.m_nLineHeight ) );
  pc.m_nFirstUpdateLine = m_ptFirstVisible.y + nLinesOfsTop;
  pc.m_nLastUpdateLine = min( ( m_ptFirstVisible.y + nLinesOfsBottom ), m_pdb->TextGetLineUpperBound() );
  pc.m_sizeBitmap.cx = m_rectClient.Width();
  pc.m_sizeBitmap.cy = pc.m_nLineHeight;
  pc.m_bDrawLineNumber = IsDrawLineNumber();
  pc.m_bDrawRuler = IsDrawRuler();
  pc.m_bDrawLineBreak = IsDrawLineBreak();
  pc.m_bDrawWhitespace = IsDrawWhitespace();
  pc.m_nTextHorzOffset = m_ptFirstVisible.x * PFX_EC_DATA.m_nCharWidthAve;
  pc.m_nCharWidthSpace = PFX_EC_DATA.m_nCharWidthSpace;
  pc.m_nCharWidthLineBreak = PFX_EC_DATA.m_nCharWidthLineBreak;
  pc.m_nCharWidthTab = PFX_EC_DATA.m_nCharWidthTab;
  pc.m_nCharWidthMax = PFX_EC_DATA.m_nCharWidthMax;
  pc.m_nTabSize = GetTabSize();
  // 基本颜色
  for ( int i = 0; i < PFX_EC_SIZEOF( pc.m_arrColors ); i++ ) {
    pc.m_arrColors[ i ] = m_pti->m_arrKeyClasses.m_arrClasses[ i ].m_color;
  }
  pc.m_arrColors[ kcBkText ] = ( !IsSyncTextBkColorWithSystem() ? pc.COLOR( kcBkText ) : ::GetSysColor( COLOR_WINDOW ) );
  pc.m_arrColors[ kcBkActiveLine ] = ( !IsSyncActiveLineBkColorWithTextBkColor() ? pc.COLOR( kcBkActiveLine ) : pc.COLOR( kcBkText ) );
  // 更新限定区域，超出该区域的操作系统会自动剪裁
  pc.m_rectUpdate.left = pt.x;
  pc.m_rectUpdate.top = pt.y + nLinesOfsTop * pc.m_nLineHeight;
  pc.m_rectUpdate.right = rectUpdate.right;
  pc.m_rectUpdate.bottom = rectUpdate.bottom;
  // 标尺区域
  pc.m_rectRuler.top = 0;
  pc.m_rectRuler.right = m_rectRuler.right;
  pc.m_rectRuler.bottom = m_rectRuler.bottom;
  pc.m_rectRuler.left = pc.m_rectLineNumber.left = 0;
  // 行号和正文区域
  pc.m_rectLineNumber.top = pc.m_rectText.top = 0;
  pc.m_rectLineNumber.right = pc.m_rectText.left = m_rectText.left;
  pc.m_rectLineNumber.bottom = pc.m_rectText.bottom = pc.m_nLineHeight;
  pc.m_rectText.right = m_rectText.right;
  pc.m_rectColSelect.top = PFX_EC_INVALID_LINE;
  pc.m_rectColSelect.bottom = PFX_EC_INVALID_LINE;
  pc.m_pPenWhitespace = NULL;
  // 为列块选择操作支持
  if ( IsColumnSelect() || ( IsTracking() && IsColumnSelectTracking() ) ) {
    // 该矩形内的坐标是文本坐标，现在将左右边转换成窗口坐标
    pc.m_rectColSelect.left = TransTextPosToViewPosX( m_rectColSelect.left );
    pc.m_rectColSelect.right = TransTextPosToViewPosX( m_rectColSelect.right );
    if ( !IsColumnSelect() ) {
      pc.m_rectColSelect.top = TransTextPosToLinePos( m_rectColSelect.top );
      pc.m_rectColSelect.bottom = TransTextPosToLinePos( m_rectColSelect.bottom );
    }
    pc.m_rectColSelect.NormalizeRect();
    // 剪裁超出文本区水平方向的左右区域
    pc.m_rectColSelect.left = max( pc.m_rectText.left, pc.m_rectColSelect.left );
    pc.m_rectColSelect.right = min( pc.m_rectText.right, pc.m_rectColSelect.right );
  }
  if ( pc.m_bDrawWhitespace || pc.m_bDrawLineBreak ) {
    pc.m_penNormal.CreatePen( PS_SOLID, 1, pc.COLOR( kcWhitespace ) );
    if ( IsTextSelected() ) {
      pc.m_penSelect.CreatePen( PS_SOLID, 1, pc.COLOR( kcTextSelected ) );
    }
  }
  dcMem.CreateCompatibleDC( dcSrc );
  dcMem.SetBkMode( TRANSPARENT );
  bitmap.CreateCompatibleBitmap( dcSrc, m_rcItem.Width(), m_rcItem.Height() );
  dcMem.SelectObject( bitmap );
  dcMem.SelectObject( PFX_EC_DATA.m_hFontText );
  DrawBk( dcMem, pc );
  // 更新区域落入标尺范围内就要刷新标尺
  if ( rectUpdate.top <= pc.m_rectRuler.bottom ) {
    DrawRuler( dcMem, pc );
  }
  if ( IsDrawHilighted() ) {
    DrawHilighted( dcMem, pc );
  } else {
    DrawPlainText( dcMem, pc );
  }
  BitBlt( dcSrc, m_rcItem.left, m_rcItem.top, m_rcItem.Width(), m_rcItem.Height(), dcMem, 0, 0, SRCCOPY );
}
void CPfxEditView::DrawBk( HDC hDC, const CPfxEditViewPaintContext& pc ) {
  // 填充未绘制的剩余屏幕部分
  if ( pc.m_rectUpdate.top < pc.m_rectUpdate.bottom ) {
    CRect rect;
    // 填充行号区背景
    int i = ( pc.m_rectLineNumber.right - pc.m_rectIndent.left );
    if ( i > 0 )           //      如果不显示行号就不需要填充
    {
      ::SetBkColor( hDC, pc.COLOR( kcBkLineNumber ) );
      ::SetRect( &rect, 0, pc.m_rectUpdate.top, i, pc.m_rectUpdate.bottom );
      ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
    }
    // 填充文本区背景
    ::SetBkColor( hDC, pc.COLOR( kcBkText ) );
    ::SetRect( &rect, i, pc.m_rectUpdate.top, pc.m_rectText.right, pc.m_rectUpdate.bottom );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
  }
}
void CPfxEditView::OnSize( UINT nType, INT cx, INT cy ) {
  UNUSED( nType );
  m_rectClient.SetRect( 0, 0, cx, cy );
  UpdateLayout();
  BOOL bInitialized = ( m_pdb->TextGetLineCount() > 0 );
  // 如果需要重新计算自动换行，这个任务将由第一视图来完成
  if ( bInitialized && IsWrapAtWindowEdge() && IsFirstSiblingView() ) {
    SetDelayRecalcWrapping( true );
    // if ( static_cast<CFrameWnd*>( AfxGetMainWnd() ) ->GetActiveFrame() == GetParentFrame() ) {
    // SetViewWrapping( PFX_EC_WRAP_WINDOWEDGE, 0 );
    // }
  }
  // 如果当前窗口大小已经能够容纳所有的行，就使第一行在窗口的顶部显示
  if ( bInitialized && ( m_pdb->TextGetLineCount() < m_sizeVisible.cy ) ) {
    EnsureVisible( 0, m_ptTextPos.x );
  }
  // 即使未有完成初始化也更新滚动条
  {
    ScrollBarUpdate( SB_HORZ );
    ScrollBarUpdate( SB_VERT );
  }
  // 重新定位插入符位置，WinXP系统与Win2000系统的表现有些不同，所以这
  // 是最安全的做法
  if ( bInitialized ) {
    UpdateCurrentPosition( m_ptTextPos.x, false, false );
  }
}
void CPfxEditView::OnMouseMove( UINT nFlags, CPoint pt ) {
  UNUSED( nFlags );
  UpdateCursor( pt );
  if ( !IsTracking() ) {
    return ;
  }
  CPoint ptTextPos;
  ptTextPos.x = TransViewPosToTextPosX( pt.x );
  ptTextPos.y = TransViewPosToTextPosY( pt.y );
  if ( IsColumnSelectTracking() ) {
    if ( EnsureVisible( TransTextPosToLinePos( ptTextPos.y ), ptTextPos.x ) ) {
      UpdateWindow();
    }
    CPoint ptOrg;
    ptOrg.x = TransTextPosToViewPosX( m_rectColSelect.left );
    ptOrg.y = TransTextPosToViewPosY( TransLinePosToTextPos( TransTextPosToLinePos( m_rectColSelect.top ) ) );
    CPoint ptOld;
    ptOld.x = TransTextPosToViewPosX( m_rectColSelect.right );
    ptOld.y = TransTextPosToViewPosY( TransLinePosToTextPos( TransTextPosToLinePos( m_rectColSelect.bottom ) ) );
    CPoint ptNew;
    ptNew.x = TransTextPosToViewPosX( ptTextPos.x );
    ptNew.y = TransTextPosToViewPosY( TransLinePosToTextPos( TransTextPosToLinePos( ptTextPos.y ) ) );
    CRect rectOrg;
    rectOrg.SetRect( ptOrg.x, ptOrg.y, ptOld.x, ( ptOrg.y + PFX_EC_DATA.m_nLineHeight ) );
    rectOrg.NormalizeRect();
    CRect rectOld;
    rectOld.SetRect( ptOrg.x, ptOld.y, ptOld.x, ( ptOld.y + PFX_EC_DATA.m_nLineHeight ) );
    rectOld.NormalizeRect();
    rectOld.UnionRect( &rectOld, rectOrg );
    rectOld.IntersectRect( &rectOld, &m_rectText );
    rectOrg.SetRect( ptOrg.x, ptOrg.y, ptNew.x, ( ptOrg.y + PFX_EC_DATA.m_nLineHeight ) );
    rectOrg.NormalizeRect();
    CRect rectNew;
    rectNew.SetRect( ptOrg.x, ptNew.y, ptNew.x, ptNew.y + PFX_EC_DATA.m_nLineHeight );
    rectNew.NormalizeRect();
    rectNew.UnionRect( &rectOrg, rectNew );
    rectNew.IntersectRect( &rectNew, &m_rectText );
    CRect rectIntersect;
    rectIntersect.IntersectRect( &rectOld, &rectNew );
    rectIntersect.NormalizeRect();
    CClientDC1 dc( this );
    if ( !rectOld.EqualRect( &rectIntersect ) ) {
      CRgn rgnOld;
      rgnOld.CreateRectRgn( rectOld.left, rectOld.top, rectOld.right, rectOld.bottom );
      dc.SelectClipRgn( rgnOld );
      dc.ExcludeClipRect( &rectIntersect );
      dc.InvertRect( &rectOld );
    }
    if ( !rectNew.EqualRect( &rectIntersect ) ) {
      CRgn rgnNew;
      rgnNew.CreateRectRgn( rectNew.left, rectNew.top, rectNew.right, rectNew.bottom );
      dc.SelectClipRgn( rgnNew );
      dc.ExcludeClipRect( &rectIntersect );
      dc.InvertRect( &rectNew );
    }
    m_rectColSelect.right = ptTextPos.x;
    m_rectColSelect.bottom = ptTextPos.y;
  } else if ( PFX_EC_SM_NORMAL == m_nSelMode ) {
    CPoint ptEditPos;
    ptEditPos.y = TransTextPosToLinePos( ptTextPos.y );
    ptEditPos.x = TransTextPosToCharPos( ptEditPos.y, ptTextPos.x );
    if ( ptEditPos == m_ptEditPos ) {
      return ;
    }
    CPoint ptSel = SelGetParam();
    m_ptEditPos = ptEditPos;
    SelRange( ptSel );
    EnsureVisible( m_ptEditPos.y, ptTextPos.x );
    UpdateCurrentPosition( ptTextPos.x, true, true );
  } else if ( PFX_EC_SM_WORD == m_nSelMode ) {
    CPoint ptEditPos;
    ptEditPos.y = TransTextPosToLinePos( ptTextPos.y );
    ptEditPos.x = TransTextPosToCharPos( ptEditPos.y, ptTextPos.x );
    if ( ptEditPos == m_ptEditPos ) {
      return ;
    }
    CPoint ptTestA = ptEditPos;
    m_pdb->TextSubLineToAbsLine( ptTestA );
    CPoint ptTestM = ptEditPos;
    m_pdb->TextSubLineToMainLine( ptTestM );
    CPoint ptBase1 = m_rectSelWordModeBaseRange.TopLeft();
    CPoint ptBase2 = m_rectSelWordModeBaseRange.BottomRight();
    const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( ptTestM.y );
    const LPCTSTR pStart = pli->pData;
    const LPCTSTR pEnd = pStart + pli->nActLen;
    LPCTSTR pCur = pStart + ptTestM.x;
    CPoint ptSelStartPos;
    if ( ( ptTestA.y < ptBase1.y )
         || ( ( ptTestA.y == ptBase1.y ) && ( ptTestA.x <= ptBase1.x ) ) ) {
      // 取前面部分
      ptSelStartPos = ptBase2;
      if ( ( pCur < pEnd )
           && !IsWhitespace( *pCur )
           && !IsDelimiter( pCur, pEnd ) ) {
        pCur = GetPrevDelimiterPos( pCur, pStart );
      }
    } else {
      // 取后面部分
      ptSelStartPos = ptBase1;
      if ( ( pCur < pEnd )
           && !IsWhitespace( *pCur )
           && !IsDelimiter( pCur, pEnd ) ) {
        pCur = GetNextDelimiterPos( pCur, pEnd );
      }
    }
    m_ptEditPos.x = pCur - pStart;
    m_ptEditPos.y = ptTestM.y;
    m_pdb->TextAbsLineToSubLine( ptSelStartPos );
    m_pdb->TextMainLineToSubLine( m_ptEditPos );
    SelRange( ptSelStartPos );
    EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  } else {
    ASSERT( false );
  }
}
BOOL CPfxEditView::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt ) {
  UNUSED( nFlags );
  UNUSED( pt );
  // Return Nonzero if mouse wheel scrolling is enabled; otherwise 0.
  UINT nSBCode;
  UINT nScrollLines;
#define SPI_GETWHEELSCROLLLINES 0x0068
  if ( !SystemParametersInfo( SPI_GETWHEELSCROLLLINES, 0, &nScrollLines, 0 ) ) {
    nScrollLines = 3; // default;
  }
  if ( zDelta > 0 ) {
    nSBCode = MAKEWPARAM( SB_LINEUP, 0 );
  } else {
    nSBCode = MAKEWPARAM( SB_LINEDOWN, 0 );
  }
  for ( UINT i = 0; i < nScrollLines; i++ ) {
    m_ptFirstVisible.y = ScrollVert( nSBCode );
  }
  UpdateCurrentPosition( m_ptTextPos.x, false, false );
  return true;
}
void CPfxEditView::OnSetFocus() {
  if ( IsDelayRecalcWrapping() && IsWrapAtWindowEdge() ) {
    SetViewWrapping( PFX_EC_WRAP_WINDOWEDGE, 0 );
  }
  // 在获得输入焦点时要检查是否有兄弟视图，如果有兄弟视图且兄弟视图当
  // 前有选择文本就要取消兄弟视图当前的选择文本，否则会有矛盾，即是说
  // 在任一时刻两个兄弟视图中只能有一个是激活的且只有激活的视图可以有
  // 选择文本
  CPfxEditView* pSiblingView = GetSiblingView();
  if ( pSiblingView != NULL ) {
    if ( pSiblingView->IsTextSelected() ) {
      pSiblingView->SelNone();
    }
  }
  // 由于兄弟视图可能已经修改过当前行的数据，使下面这些参数无效，所以
  // 要重新校验
  VerifyCurrentPosition();
  CaretRebuild();
  UpdateCurrentPosition( TransCharPosToTextPos( m_ptEditPos ), true, false );
}
void CPfxEditView::OnKillFocus() {
  // CView::OnKillFocus( pNewWnd );
  CaretDestroy();
}
void CPfxEditView::OnLButtonDown( UINT nFlags, CPoint pt ) {
  UNUSED( nFlags );
  BOOL bColSelectTracking = IsColumnSelectTracking();
  SetColumnSelectTracking( false );
  // 将鼠标的当前位置转换为文本位置
  CPoint ptTextPos;
  ptTextPos.x = TransViewPosToTextPosX( pt.x );
  ptTextPos.y = TransViewPosToTextPosY( pt.y );
  CPoint ptEditPos;
  ptEditPos.y = TransTextPosToLinePos( ptTextPos.y );
  ptEditPos.x = TransTextPosToCharPos( ptEditPos.y, ptTextPos.x );
  // 保存当前行以便用于更新操作
  CPoint cpOld = m_ptEditPos;
  // 如果SHIFT键被按下，就执行附加选择
  if ( PFX_EC_IS_KEY_DOWN( VK_SHIFT ) ) {
    CPoint ptSel = SelGetParam();
    m_ptEditPos = ptEditPos;
    SelRange( ptSel );
    // 标志鼠标左键已经按下，并进入鼠标选择状态
    SetTracking( true );
  }
  // 如果光标落在行号内，则与光标位置对应的行全行被选择，而新插入符位置将
  // 会移动到该被选择行的下一行的行首
  else if ( GetHitTestCode( pt ) == PFX_EC_HT_LINE_NUMBER ) {
    SelLine( ptEditPos.y );
    ptTextPos.x = TransCharPosToTextPos( m_ptEditPos );
    // 标志鼠标左键已经按下，并进入鼠标选择状态
    SetTracking( true );
  }
  // 如果有选择文本就为拖放编辑做准备
  else if ( IsDragDropEnabled() && IsInsideSelect( pt ) ) {
    m_ptEditPos = ptEditPos;
    UpdateLines( cpOld.y, m_ptEditPos.y );
    UpdateCurrentPosition( ptTextPos.x, true, true );
    DROPEFFECT dropEffect = DROPEFFECT_COPY | DROPEFFECT_MOVE;
    // 这对括号可以让dataSrc对象尽可能快的销毁
    {
      CPfxEditViewDataSource dataSrc( this );
      dataSrc.DelayRenderData( CF_TEXT );
      dropEffect = dataSrc.DoDragDrop( dropEffect );
    }
    if ( dropEffect == DROPEFFECT_NONE ) {
      SelNone();
    } else if ( ( dropEffect & DROPEFFECT_COPY ) && !IsDragDropHandled() ) {
      SelNone();
    }
    // 如果数据DROP在其它窗口，且返回数值指示删除操作，则执行删
    // 除已选择部分
    else if ( ( dropEffect & DROPEFFECT_MOVE ) && !IsDragDropHandled() ) {
      if ( IsReadOnly() ) {
        SelNone();
      } else {
        CPfxEditViewUndoManager um( this, PFX_EC_UNDO_DRAGDROP_MOVE );
        DragDropRemove( m_ptEditPos );
      }
    }
    ptTextPos.x = m_ptTextPos.x;
    CaretRebuild();
  }
  // 如果鼠标左键按钮下时键盘 ALT键处于按下状态则执行列块选择操作
  else if ( ( bColSelectTracking || PFX_EC_IS_KEY_DOWN( VK_MENU ) )
            && IsCanColumnSelect() ) {
    m_ptEditPos = ptEditPos;
    if ( IsTextSelected() ) {
      SelNone();
    }
    m_rectColSelect.SetRect( ptTextPos.x, ptTextPos.y, ptTextPos.x, ptTextPos.y );
    SetColumnSelectTracking( true );
    // 标志鼠标左键已经按下，并进入鼠标选择状态
    SetTracking( true );
  } else {
    m_ptEditPos = ptEditPos;
    if ( IsTextSelected() ) {
      SelNone();
    }
    // 标志鼠标左键已经按下，并进入鼠标选择状态
    SetTracking( true );
  }
  EnsureVisibleAndUpdateLines( m_ptEditPos.y, ptTextPos.x, cpOld.y, m_ptEditPos.y );
  UpdateCurrentPosition( ptTextPos.x, true, true );
}
void CPfxEditView::OnRButtonDown( UINT nFlags, CPoint pt ) {
  // 取消列块选择范围选定操作
  SetColumnSelectTracking( false );
  // 如果当前有选择内容并且鼠标没有击中选择范围，则当前位置重新设置并
  // 取消当前选择，否则什么也不做
  if ( !IsInsideSelect( pt ) ) {
    // 将鼠标的当前位置转换为文本位置
    CPoint ptTextPos;
    ptTextPos.x = TransViewPosToTextPosX( pt.x );
    ptTextPos.y = TransViewPosToTextPosY( pt.y );
    CPoint ptEditPos;
    ptEditPos.y = TransTextPosToLinePos( ptTextPos.y );
    ptEditPos.x = TransTextPosToCharPos( ptEditPos.y, ptTextPos.x );
    // 保存当前行以便用于更新操作
    CPoint cpOld = m_ptEditPos;
    SelNone();
    m_ptEditPos = ptEditPos;
    EnsureVisibleAndUpdateLines( m_ptEditPos.y, ptTextPos.x, cpOld.y, m_ptEditPos.y );
    UpdateCurrentPosition( ptTextPos.x, true, true );
  }
  // 让WM_CONTEXTMENU触发
  // CView::OnRButtonDown( nFlags, pt );
}
void CPfxEditView::OnLButtonUp( UINT nFlags, CPoint pt ) {
  UNUSED( nFlags );
  UNUSED( pt );
  SetTracking( false );
}
void CPfxEditView::OnLButtonDblClk( UINT nFlags, CPoint pt ) {
  UNUSED( nFlags );
  switch ( GetHitTestCode( pt ) ) {
  case PFX_EC_HT_RULER:
  case PFX_EC_HT_LINE_NUMBER:
    return ;
  default:
    break;
  }
  CPoint ptTextPos;
  ptTextPos.y = TransViewPosToTextPosY( pt.y );
  ptTextPos.x = TransViewPosToTextPosX( pt.x );
  CPoint ptEditPos;
  ptEditPos.y = TransTextPosToLinePos( ptTextPos.y );
  ptEditPos.x = TransTextPosToCharPos( ptEditPos.y, ptTextPos.x );
  // 更新原来的当前行，因为当活动行背景和非活动行背景色不同的
  // 时候就必须更新，否则原来的活动行仍然显示为当前活动行
  if ( ptEditPos.y != m_ptEditPos.y ) {
    LONG i = m_ptEditPos.y;
    m_ptEditPos.y = ptEditPos.y;
    UpdateLines( i, i );
  }
  SelWord( ptEditPos );
  EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  CPoint ptLeftTop = SelGetParam();
  m_pdb->TextSubLineToAbsLine( ptLeftTop );
  CPoint ptRightBottom = m_ptEditPos;
  m_pdb->TextSubLineToAbsLine( ptRightBottom );
  m_nSelMode = PFX_EC_SM_WORD;
  m_rectSelWordModeBaseRange.SetRect( ptLeftTop, ptRightBottom );
  SetTracking( true );
}
void CPfxEditView::OnCaptureChanged( CControlUI *pWnd ) {
  UNUSED( pWnd );
  if ( IsTracking() && IsColumnSelectTracking() ) {
    SelRange( m_rectColSelect );
  } else if ( m_nSelMode == PFX_EC_SM_WORD ) {
    m_nSelMode = PFX_EC_SM_NORMAL;
  }
  m_bTrackingMouse = false;
  SetColumnSelectTracking( false );
}
void CPfxEditView::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags ) {
  BOOL bCtrlDown = PFX_EC_IS_KEY_DOWN( VK_CONTROL );
  BOOL bShiftDown = PFX_EC_IS_KEY_DOWN( VK_SHIFT );
  BOOL bCancelTracking = true;
  switch ( nChar ) {
  case VK_BACK:
    // bCtrlDown = bWord
    OnKeyBackSpace( bCtrlDown );
    break;
  case VK_DELETE:
    // bCtrlDown = bWord
    OnKeyDelete( bCtrlDown );
    break;
  case VK_LEFT:
    // bShiftDown = bSelect
    // bCtrlDown = bWord
    OnKeyMoveLeft( bShiftDown, bCtrlDown );
    break;
  case VK_RIGHT:
    // bShiftDown = bSelect
    // bCtrlDown = bWord
    OnKeyMoveRight( bShiftDown, bCtrlDown );
    break;
  case VK_UP:
    // bShiftDown = bSelect
    // bCtrlDown = bScroll
    OnKeyMoveUp( bShiftDown, bCtrlDown );
    break;
  case VK_DOWN:
    // bShiftDown = bSelect
    // bCtrlDown = bScroll
    OnKeyMoveDown( bShiftDown, bCtrlDown );
    break;
  case VK_HOME:
    // bShiftDown = bSelect
    // bCtrlDown = bGotoSOF
    OnKeyHome( bShiftDown, bCtrlDown );
    break;
  case VK_END:
    // bShiftDown = bSelect
    // bCtrlDown = bGotoEOF
    OnKeyEnd( bShiftDown, bCtrlDown );
    break;
  case VK_PRIOR:
    // bShiftDown = bSelect
    OnKeyPageUp( bShiftDown );
    break;
  case VK_NEXT:
    // bShiftDown = bSelect
    OnKeyPageDown( bShiftDown );
    break;
  case VK_RETURN:
    OnKeyReturn();
    break;
  case VK_TAB:
    if ( !IsTracking() ) {
      OnKeyTab();
    }
    bCancelTracking = false;
    break;
  case VK_INSERT:
    OnKeyInsert();
    break;
  case VK_ESCAPE:
    SelNone();
    break;
  default:
    // CView::OnKeyDown( nChar, nRepCnt, nFlags );
    bCancelTracking = false;
    break;
  }
  // 如果当前有选择文本但使用某些键删除了选择部分就不应该再有选择出现，
  // 下面阻止WM_MOUSEMOVE处理器更新选择范围
  if ( bCancelTracking && IsTracking() ) {
    SetTracking( false );
  }
}
void CPfxEditView::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags ) {
  UNUSED( nFlags );
  UNUSED( nRepCnt );
#ifdef _UNICODE
  if ( !IsTracking() && _istprint( nChar ) ) {
    OnKeyChar( ( LPCTSTR ) & nChar, 1 );
  }
#else
  if ( IsTracking() ) {
    return ;
  }
  // 有些输入法（例如全拼输入法）在输入全角的标点符号时不会发送平时的
  // WM_IME_XXX消息，而是发送两次WM_CHAR，第一次是LEAD BYTE，第二次是
  // TRAIL BYTE，所以要特殊处理
  static UINT l_s_nLeadByte = 0;
  static HWND l_s_hWndOwner = NULL;
  if ( l_s_nLeadByte == 0 ) {
    if ( !_istprint( nChar ) ) {
      if ( _ismbblead( nChar ) ) {
        l_s_nLeadByte = nChar;
        l_s_hWndOwner = M_HWND;
      }
      return ;
    }
    OnKeyChar( ( LPCTSTR ) & nChar, 1 );
  } else {
    nChar = ( ( nChar << CHAR_BIT ) | l_s_nLeadByte );
    l_s_nLeadByte = 0;
    if ( ( l_s_hWndOwner != M_HWND ) || !_istlegal( nChar ) ) {
      return ;
    }
    OnKeyChar( ( LPCTSTR ) & nChar, PFX_EC_MAX_MB_LEN );
  }
#endif // #ifdef _UNICODE
}
LRESULT CPfxEditView::OnIMEComposition( WPARAM wParam, LPARAM lParam ) {
  if ( ( lParam & GCS_RESULTSTR ) == 0 ) {
    return 0; //DefWindowProc( WM_IME_COMPOSITION, wParam, lParam );
  }
  HIMC hIMC = ::ImmGetContext( M_HWND );
  if ( hIMC != NULL ) {
    CHAR sz[ MAX_PATH ];
    LONG cch = ::ImmGetCompositionString( hIMC, GCS_RESULTSTR, sz, PFX_EC_SIZEOF( sz ) );
    ::ImmReleaseContext( M_HWND, hIMC );
    // 经过测试，有些输入法可能会返回零
    if ( ( cch > 0 ) && ( cch != IMM_ERROR_NODATA ) && ( cch != IMM_ERROR_GENERAL ) ) {
      OnKeyChar( static_cast<LPCTSTR>( sz ), cch );
    }
  }
  return 0;
}
void CPfxEditView::OnKeyChar( LPCTSTR pData, LONG cch ) {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return ;
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_TYPING );
  if ( IsTextSelected() ) {
    SelNoneAndRemoveSelected();
  }
  if ( !IsOverride() ) {
    CharsInsert( m_ptEditPos, pData, cch, true );
    ASSERT( m_pdb->TextIsValidLine( m_ptEditPos.y ) );
    ASSERT( m_pdb->TextIsValidChar( m_ptEditPos.y, m_ptEditPos.x ) );
  } else {
    // 计算要插入的字符串中有多少个字符
    LPCTSTR pCur = pData;
    LPCTSTR pEnd = pData + cch;
    for ( LONG nCharsToInsert = 0; pCur < pEnd; nCharsToInsert++ ) {
      pCur += ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
    }
    // 获取从当前位置起到nCharsToInsert后的字符位置
    CPoint cpOld = m_ptEditPos;
    m_pdb->TextSubLineToMainLine( cpOld );
    const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( cpOld.y );
    const LPCTSTR pStart = pli->pData;
    for ( pCur = pStart + cpOld.x, pEnd = pStart + pli->nActLen; ( pCur < pEnd ) && ( nCharsToInsert > 0 ); nCharsToInsert-- ) {
      pCur += ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
    }
    cpOld.x = min( pCur, pEnd ) - pStart;
    m_pdb->TextMainLineToSubLine( cpOld );
    CharsReplace( m_ptEditPos, cpOld, pData, cch, true );
    ASSERT( m_pdb->TextIsValidLine( m_ptEditPos.y ) );
    ASSERT( m_pdb->TextIsValidChar( m_ptEditPos.y, m_ptEditPos.x ) );
  }
}
// void OnKeyTab()
void CPfxEditView::OnKeyTab() {
  LONG cch;
  TCHAR sz[ PFX_EC_MAX_TAB_SIZE ];
  if ( !IsExpandTabToSpaceChars() ) {
    cch = 1;
    sz[ 0 ] = PFX_EC_CHR_TAB;
  } else {
    ASSERT( GetTabSize() <= PFX_EC_MAX_TAB_SIZE );
    cch = GetTabSize() - m_ptLogiPos.x % GetTabSize();
    for ( LONG i = 0; i < cch; i++ ) {
      sz[ i ] = PFX_EC_CHR_SPACE;
    }
  }
  OnKeyChar( sz, cch );
}
// void OnKeyInsert()
void CPfxEditView::OnKeyInsert() {
  ASSERT_VALID( this );
  SetOverride( !IsOverride() );
}
// void OnKeyReturn()
void CPfxEditView::OnKeyReturn() {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return ;
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_TYPING );
  if ( IsTextSelected() ) {
    SelNoneAndRemoveSelected();
  }
  // 如果当前处于OVERRIDE模式，按下回车键只会将编辑点移到下一行的行首
  if ( IsOverride() ) {
    if ( !m_pdb->TextIsLastLine( m_ptEditPos.y ) ) {
      m_ptEditPos.x = 0;
      m_ptEditPos.y++;
      UpdateAllViews( PFX_EC_UT_HORZ, m_ptEditPos.y - 1, m_ptEditPos.y );
      EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
    }
  } else {
    // 计算正常对准时所需要的空白字符数
    LONG nLeadingSpaces = CalcLeadingWhitespaces( m_ptEditPos );
    LONG nLeadingSpacesBufferSize =
      FillBufferWithWhitespacesCalcBufferSize( 0, nLeadingSpaces );
    LONG nBufferSize = nLeadingSpacesBufferSize + PFX_EC_LINE_BREAK_SIZE_DOS;
    CPfxEditHeap heap;
    LPTSTR pBuffer = reinterpret_cast<LPTSTR>( heap.Alloc( nBufferSize * sizeof( TCHAR ) ) );
    memcpy( pBuffer, m_pdb->GetLineBreakDataFromType( PFX_EC_LINE_BREAK_TYPE_DOS ), PFX_EC_LINE_BREAK_SIZE_DOS * sizeof( TCHAR ) );
    FillBufferWithWhitespacesDoFill( 0, nLeadingSpaces, pBuffer + PFX_EC_LINE_BREAK_SIZE_DOS, nLeadingSpacesBufferSize );
    CharsInsert( m_ptEditPos, pBuffer, nBufferSize, true );
  }
}
// void OnKeyBackSpace(BOOL bWord)
void CPfxEditView::OnKeyBackSpace( BOOL bWord ) {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return ;
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_REMOVE );
  if ( IsTextSelected() ) {
    SelNoneAndRemoveSelected();
    return ;
  }
  if ( m_pdb->TextIsSOF( m_ptEditPos ) ) {
    return ;
  }
  CPoint cpNew;
  CPoint cpOld = m_ptEditPos;
  MoveLeft( cpNew, bWord, false );
  CharsRemove( cpNew, cpOld, true );
  ASSERT( m_pdb->TextIsValidLine( m_ptEditPos.y ) );
  ASSERT( m_pdb->TextIsValidChar( m_ptEditPos.y, m_ptEditPos.x ) );
}
// void OnKeyDelete(BOOL bWord)
void CPfxEditView::OnKeyDelete( BOOL bWord ) {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return ;
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_REMOVE );
  if ( IsTextSelected() ) {
    SelNoneAndRemoveSelected();
    return ;
  }
  if ( m_pdb->TextIsEOF( m_ptEditPos ) ) {
    return ;
  }
  CPoint cpNew;
  CPoint cpOld = m_ptEditPos;
  MoveRight( cpNew, bWord, false );
  CharsRemove( cpOld, cpNew, true );
  ASSERT( m_pdb->TextIsValidLine( m_ptEditPos.y ) );
  ASSERT( m_pdb->TextIsValidChar( m_ptEditPos.y, m_ptEditPos.x ) );
}
// void OnKeyMoveLeft(BOOL bSelect, BOOL bWord)
void CPfxEditView::OnKeyMoveLeft( BOOL bSelect, BOOL bWord ) {
  LONG xTextPos = 0;
  if ( bSelect ) {
    CPoint ptSel = SelGetParam();
    xTextPos = MoveLeft( m_ptEditPos, bWord );
    SelRange( ptSel );
    EnsureVisible( m_ptEditPos.y, xTextPos );
  } else {
    if ( IsTextSelected() ) {
      SelNoneAndUpdateCurrentPosition( VK_LEFT );
      xTextPos = m_ptTextPos.x; // 该数值已经被更新
      EnsureVisible( m_ptEditPos.y, xTextPos );
    } else {
      CPoint cpOld = m_ptEditPos;
      xTextPos = MoveLeft( m_ptEditPos, bWord );
      if ( !EnsureVisible( m_ptEditPos.y, xTextPos )
           && ( cpOld.y != m_ptEditPos.y ) ) {
        UpdateLines( cpOld.y, m_ptEditPos.y );
      }
    }
  }
  UpdateCurrentPosition( xTextPos, true, true );
}
// void OnKeyMoveRight(BOOL bSelect, BOOL bWord)
void CPfxEditView::OnKeyMoveRight( BOOL bSelect, BOOL bWord ) {
  LONG xTextPos = 0;
  if ( bSelect ) {
    CPoint ptSel = SelGetParam();
    xTextPos = MoveRight( m_ptEditPos, bWord );
    SelRange( ptSel );
    EnsureVisible( m_ptEditPos.y, xTextPos );
  } else {
    if ( IsTextSelected() ) {
      SelNoneAndUpdateCurrentPosition( VK_RIGHT );
      xTextPos = m_ptTextPos.x; // 该数值已经被更新
      EnsureVisible( m_ptEditPos.y, xTextPos );
    } else {
      CPoint cpOld = m_ptEditPos;
      xTextPos = MoveRight( m_ptEditPos, bWord );
      if ( !EnsureVisible( m_ptEditPos.y, xTextPos )
           && ( cpOld.y != m_ptEditPos.y ) ) {
        UpdateLines( cpOld.y, m_ptEditPos.y );
      }
    }
  }
  UpdateCurrentPosition( xTextPos, true, true );
}
// void OnKeyMoveUp(BOOL bSelect, BOOL bScroll)
// 1)  如果SHIFT键按下，执行选择操作
// 2)  如果CTRL键按下，执行滚屏操作：
//  在滚屏过程中当前行始终可见，即如果当前行要滚入不可见范围则第一可
//  见行要成为新的当前行
// 3)  如果SHIFT和CTRL键同时按下则SHIFT优先
void CPfxEditView::OnKeyMoveUp( BOOL bSelect, BOOL bScroll ) {
  if ( bSelect )           //      执行选择操作
  {
    if ( m_pdb->TextIsFirstLine( m_ptEditPos.y ) ) {
      // 如果当前位置不在可见范围内，要将其放到可见范围内
      EnsureVisibleAndUpdateCurrentPosition();
    } else {
      // 设定选择范围
      CPoint ptSel = SelGetParam();
      // 如果新的当前行将不可见，就要执行向上文本滚动操作
      if ( m_ptEditPos.y != m_ptFirstVisible.y ) {
        m_ptEditPos.y--;
      } else {
        m_ptEditPos.y = m_ptFirstVisible.y = ScrollVert( SB_LINEUP );
      }
      LONG xTextPos = GetCaretReferPoint();
      m_ptEditPos.x = TransTextPosToCharPos( m_ptEditPos.y, xTextPos );
      SelRange( ptSel );
      EnsureVisible( m_ptEditPos.y, xTextPos );
      UpdateCurrentPosition( xTextPos, true, false );
    }
  }
  else if ( bScroll )           //      VK_UP执行向上滚屏操作
  {
    if ( m_pdb->TextIsFirstLine( m_ptFirstVisible.y ) ) {
      // 如果当前位置不在可见范围内，要将其放到可见范围内
      if ( !IsTextSelected() ) {
        EnsureVisibleAndUpdateCurrentPosition();
      }
    } else {
      m_ptFirstVisible.y = ScrollVert( SB_LINEUP );
      LONG xTextPos = m_ptTextPos.x;
      // 如果当前有选择文本则不能使当行前离开选择文本区，
      // 这时候当前行只能滚动到不可见范围
      if ( ( m_ptEditPos.y > GetLastVisibleLine() ) && !IsTextSelected() ) {
        xTextPos = GetCaretReferPoint();
        m_ptEditPos.y = GetLastVisibleLine();
        m_ptEditPos.x = TransTextPosToCharPos( m_ptEditPos.y, xTextPos );
        EnsureVisibleAndUpdateLines( m_ptEditPos.y, xTextPos, m_ptEditPos.y, m_ptEditPos.y + 1 );
      }
      UpdateCurrentPosition( xTextPos, true, false );
    }
  }
  else // VK_UP执行更改当前行操作
  {
    if ( IsTextSelected() ) {
      SelNoneAndUpdateCurrentPosition( VK_UP );
    }
    if ( m_pdb->TextIsFirstLine( m_ptEditPos.y ) ) {
      // 如果当前位置不在可见范围内，要将其放到可见范围内
      EnsureVisibleAndUpdateCurrentPosition();
    } else {
      // 如果新的当前行已经不在可见范围，就要执行向上文本
      // 滚动操作
      if ( m_ptEditPos.y != m_ptFirstVisible.y ) {
        m_ptEditPos.y--;
      } else {
        m_ptEditPos.y = m_ptFirstVisible.y = ScrollVert( SB_LINEUP );
      }
      LONG xTextPos = GetCaretReferPoint();
      m_ptEditPos.x = TransTextPosToCharPos( m_ptEditPos.y, xTextPos );
      // 刷新改变的地方
      EnsureVisibleAndUpdateLines( m_ptEditPos.y, xTextPos, m_ptEditPos.y, m_ptEditPos.y + 1 );
      UpdateCurrentPosition( xTextPos, true, false );
    }
  }
}
// void OnKeyMoveDown(BOOL bSelect, BOOL bScroll)
void CPfxEditView::OnKeyMoveDown( BOOL bSelect, BOOL bScroll ) {
  if ( bSelect )           //      执行选择操作
  {
    if ( m_pdb->TextIsLastLine( m_ptEditPos.y ) ) {
      // 如果当前位置不在可见范围内，要将其放到可见范围内
      EnsureVisibleAndUpdateCurrentPosition();
    } else {
      // 设定选择范围
      CPoint ptSel = SelGetParam();
      // 如果新的当前行已经不在可见范围，就要执行向下文本
      // 滚动操作
      if ( m_ptEditPos.y < GetLastVisibleLine() ) {
        m_ptEditPos.y++;
      } else {
        m_ptFirstVisible.y = ScrollVert( SB_LINEDOWN );
        m_ptEditPos.y = GetLastVisibleLine();
      }
      LONG xTextPos = GetCaretReferPoint();
      m_ptEditPos.x = TransTextPosToCharPos( m_ptEditPos.y, xTextPos );
      SelRange( ptSel );
      // 刷新改变的地方
      EnsureVisible( m_ptEditPos.y, xTextPos );
      UpdateCurrentPosition( xTextPos, true, false );
    }
  }
  // 执行滚屏操作：
  // 当前行与文本内容同步滚动，结果好象使用鼠标拖动滚动条滑块，但当前
  // 行始终在可见范围
  else if ( bScroll ) {
    if ( !m_pdb->TextIsValidLine( m_ptFirstVisible.y + m_sizeVisible.cy ) ) {
      // 如果当前位置不在可见范围内，要将其放到可见范围内
      if ( !IsTextSelected() ) {
        EnsureVisibleAndUpdateCurrentPosition();
      }
    } else {
      LONG xTextPos = m_ptTextPos.x;
      m_ptFirstVisible.y = ScrollVert( SB_LINEDOWN );
      // 如果当前有选择文本则不能使当行前离开选择文本区，
      // 这时候当前行只能滚动到不可见范围
      if ( ( m_ptEditPos.y < m_ptFirstVisible.y ) && !IsTextSelected() ) {
        xTextPos = GetCaretReferPoint();
        m_ptEditPos.y = m_ptFirstVisible.y;
        m_ptEditPos.x = TransTextPosToCharPos( m_ptEditPos.y, xTextPos );
        //  刷新改变的地方
        EnsureVisibleAndUpdateLines( m_ptEditPos.y, xTextPos, m_ptEditPos.y, m_ptEditPos.y );
      }
      UpdateCurrentPosition( xTextPos, true, false );
    }
  } else // 执行更改当前行操作：
  {
    if ( IsTextSelected() ) {
      SelNoneAndUpdateCurrentPosition( VK_DOWN );
    }
    if ( m_pdb->TextIsLastLine( m_ptEditPos.y ) ) {
      // 如果当前位置不在可见范围内，要将其放到可见范围内
      EnsureVisibleAndUpdateCurrentPosition();
    } else {
      // 如果新的当前行已经不在可见范围，就要执行向下文本滚动操作
      if ( m_ptEditPos.y < GetLastVisibleLine() ) {
        m_ptEditPos.y++;
      } else {
        m_ptFirstVisible.y = ScrollVert( SB_LINEDOWN );
        m_ptEditPos.y = GetLastVisibleLine();
      }
      LONG xTextPos = GetCaretReferPoint();
      m_ptEditPos.x = TransTextPosToCharPos( m_ptEditPos.y, xTextPos );
      // 确保当前位置可见
      EnsureVisibleAndUpdateLines( m_ptEditPos.y, xTextPos, m_ptEditPos.y, m_ptEditPos.y - 1 );
      // 重定位插入符
      UpdateCurrentPosition( xTextPos, true, false );
    }
  }
}
// void OnKeyHome(BOOL bSelect, BOOL bGotoSOF)
// 1)  如果CTRL键按下则移动到文档的第一行适当的位置
// 2)  如果CTRL键没有按下：
//  a)  如果被设置成“HOME键移动到首个非空白字符“，则当前位置移动首个
//      非空白字符，否则当前位置移动到行首
//  b)  如果一行中没有非空白字符当前位置移动到行首
// 3)  如果SHIFT键同时被按下则移动当前位置的同时执行选择操作
void CPfxEditView::OnKeyHome( BOOL bSelect, BOOL bGotoSOF ) {
  if ( bSelect ) {
    if ( bGotoSOF ) {
      if ( m_pdb->TextIsSOF( m_ptEditPos ) ) {
        //  如果当前位置不在可见范围内，要将其放到可见范围内
        EnsureVisibleAndUpdateCurrentPosition();
      } else {
        //  确定选择范围
        CPoint ptSel = SelGetParam();
        m_ptEditPos = CPoint( 0, 0 );
        SelRange( ptSel );
        EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
      }
    } else {
      LONG nNewCharPos = 0; // 等于零表示移动到行首
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( m_ptEditPos.y );
      if ( IsHomeKeyGotoFirstNonWhitespaceChar() && !PFX_EC_IS_EMPTY_LINE( pli ) ) {
        const LPCTSTR pCur = pli->pData;
        const LPCTSTR pEnd = pCur + pli->nLen;
        LONG nFirstNonWhitespaceCharPos = GetNextNonWhitespacePos( pCur, pEnd ) - pCur;
        if ( m_ptEditPos.x != nFirstNonWhitespaceCharPos ) {
          nNewCharPos = nFirstNonWhitespaceCharPos;
        }
      }
      if ( nNewCharPos == m_ptEditPos.x ) {
        EnsureVisibleAndUpdateCurrentPosition();
      } else {
        CPoint ptSel = SelGetParam();
        m_ptEditPos.x = nNewCharPos;
        SelRange( ptSel );
        EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
      }
    }
  } else {
    if ( IsTextSelected() ) {
      SelNoneAndUpdateCurrentPosition( VK_HOME );
    }
    if ( bGotoSOF ) {
      if ( m_pdb->TextIsSOF( m_ptEditPos ) ) {
        //  如果当前位置不在可见范围内，要将其放到可见范围内
        EnsureVisibleAndUpdateCurrentPosition();
      } else {
        CPoint cp( 0, 0 );
        EnsureVisibleAndSetAsCurrentPosition( cp );
      }
    } else {
      LONG nNewCharPos = 0; // 等于零表示移动到行首
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( m_ptEditPos.y );
      if ( IsHomeKeyGotoFirstNonWhitespaceChar() && !PFX_EC_IS_EMPTY_LINE( pli ) ) {
        const LPCTSTR pCur = pli->pData;
        const LPCTSTR pEnd = pCur + pli->nLen;
        LONG nFirstNonWhitespaceCharPos = GetNextNonWhitespacePos( pCur, pEnd ) - pCur;
        if ( m_ptEditPos.x != nFirstNonWhitespaceCharPos ) {
          nNewCharPos = nFirstNonWhitespaceCharPos;
        }
      }
      if ( nNewCharPos == m_ptEditPos.x ) {
        EnsureVisibleAndUpdateCurrentPosition();
      } else {
        CPoint cp( nNewCharPos, m_ptEditPos.y );
        EnsureVisibleAndSetAsCurrentPosition( cp );
      }
    }
  }
}
// void OnKeyEnd(BOOL bSelect, BOOL bGotoEOF)
// 1)  如果同时按下CTRL键移动到文档的最后一行行末
// 2)  如果CTRL键没有按下则当前位置移动到行末
// 3)  如果SHIFT键同时被按下则移动当前位置的同时执行选择操作
void CPfxEditView::OnKeyEnd( BOOL bSelect, BOOL bGotoEOF ) {
  if ( bSelect ) {
    if ( bGotoEOF ) {
      if ( m_pdb->TextIsEOF( m_ptEditPos ) ) {
        //  如果当前位置不在可见范围内，要将其放到可
        //  见范围内
        EnsureVisibleAndUpdateCurrentPosition();
      } else {
        CPoint ptSel = SelGetParam();
        m_ptEditPos.y = m_pdb->TextGetLineUpperBound();
        m_ptEditPos.x = m_pdb->TextGetLineLength( m_ptEditPos.y );
        SelRange( ptSel );
        EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
      }
    } else {
      if ( m_pdb->TextIsEOL( m_ptEditPos ) ) {
        //  如果当前位置不在可见范围内，要将其放到可
        //  见范围内
        EnsureVisibleAndUpdateCurrentPosition();
      } else {
        CPoint ptSel = SelGetParam();
        m_ptEditPos.y = m_ptEditPos.y;
        m_ptEditPos.x = m_pdb->TextGetLineLength( m_ptEditPos.y );
        SelRange( ptSel );
        EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
      }
    }
  } else {
    if ( IsTextSelected() ) {
      SelNoneAndUpdateCurrentPosition( VK_END );
    }
    if ( bGotoEOF ) {
      if ( m_pdb->TextIsEOF( m_ptEditPos ) ) {
        //  如果当前位置不在可见范围内，要将其放到可
        //  见范围内
        EnsureVisibleAndUpdateCurrentPosition();
      } else {
        CPoint cp;
        cp.y = m_pdb->TextGetLineUpperBound();
        cp.x = m_pdb->TextGetLineLength( cp.y );
        EnsureVisibleAndSetAsCurrentPosition( cp );
      }
    } else {
      if ( m_pdb->TextIsEOL( m_ptEditPos ) ) {
        //  如果当前位置不在可见范围内，要将其放到可
        //  见范围内
        EnsureVisibleAndUpdateCurrentPosition();
      } else {
        CPoint cp;
        cp.y = m_ptEditPos.y;
        cp.x = m_pdb->TextGetLineLength( cp.y );
        EnsureVisibleAndSetAsCurrentPosition( cp );
      }
    }
  }
}
// void OnKeyPageUp(BOOL bSelect)
// 1)  CTRL键被忽略：执行向上滚一屏的操作
//  a)  如果已经有选择文本，取消已选择内容，插入符移动到已选择内容的
//      上面一行
//  b)  当前行也随同滚动相应的行数
// 2)  如果SHIFT键按下：执行向上滚一屏的操作，并选择这一屏滚过的内容
// 3)  所有滚动操作必须重定位插入符
void CPfxEditView::OnKeyPageUp( BOOL bSelect ) {
  if ( bSelect ) {
    if ( m_pdb->TextIsFirstLine( m_ptEditPos.y ) ) {
      EnsureVisibleAndUpdateCurrentPosition();
    } else {
      // 设定选择范围
      CPoint ptSel = SelGetParam();
      // 确定新的当前行、第一可见行
      if ( m_pdb->TextIsFirstLine( m_ptFirstVisible.y ) ) {
        m_ptEditPos.y = 0;
      } else {
        LONG nFirstVisibleLine = ScrollVert( SB_PAGEUP );
        m_ptEditPos.y = m_pdb->TextVerifyLine( ( m_ptEditPos.y - ( m_ptFirstVisible.y - nFirstVisibleLine ) ) );
        m_ptFirstVisible.y = nFirstVisibleLine;
      }
      // 重新计算当前列
      LONG xTextPos = GetCaretReferPoint();
      m_ptEditPos.x = TransTextPosToCharPos( m_ptEditPos.y, xTextPos );
      SelRange( ptSel );
      EnsureVisible( m_ptEditPos.y, xTextPos );
      UpdateCurrentPosition( xTextPos, true, false );
    }
  } else {
    if ( IsTextSelected() ) {
      SelNoneAndUpdateCurrentPosition( VK_PRIOR );
    }
    if ( m_pdb->TextIsFirstLine( m_ptEditPos.y ) ) {
      EnsureVisibleAndUpdateCurrentPosition();
    } else {
      CPoint cpOld = m_ptEditPos;
      if ( m_pdb->TextIsFirstLine( m_ptFirstVisible.y ) ) {
        m_ptEditPos.y = 0;
      } else {
        LONG nFirstVisibleLine = ScrollVert( SB_PAGEUP );
        m_ptEditPos.y = m_pdb->TextVerifyLine( ( m_ptEditPos.y - ( m_ptFirstVisible.y - nFirstVisibleLine ) ) );
        m_ptFirstVisible.y = nFirstVisibleLine;
      }
      LONG xTextPos = GetCaretReferPoint();
      m_ptEditPos.x = TransTextPosToCharPos( m_ptEditPos.y, xTextPos );
      EnsureVisibleAndUpdateLines( m_ptEditPos.y, xTextPos, m_ptEditPos.y, cpOld.y );
      UpdateCurrentPosition( xTextPos, true, false );
    }
  }
}
// void OnKeyPageDown(BOOL bSelect)
void CPfxEditView::OnKeyPageDown( BOOL bSelect ) {
  if ( bSelect ) {
    if ( m_pdb->TextIsLastLine( m_ptEditPos.y ) ) {
      EnsureVisibleAndUpdateCurrentPosition();
    } else {
      // 设定选择范围
      CPoint ptSel = SelGetParam();
      // 确定新的当前行、第一可见行
      if ( m_pdb->TextIsLastLine( GetLastVisibleLine() ) ) {
        m_ptEditPos.y = m_pdb->TextGetLineUpperBound();
      } else {
        LONG nFirstVisibleLine = ScrollVert( SB_PAGEDOWN );
        m_ptEditPos.y = m_pdb->TextVerifyLine( ( m_ptEditPos.y + ( nFirstVisibleLine - m_ptFirstVisible.y ) ) );
        m_ptFirstVisible.y = nFirstVisibleLine;
      }
      // 重新计算当前列
      LONG xTextPos = GetCaretReferPoint();
      m_ptEditPos.x = TransTextPosToCharPos( m_ptEditPos.y, xTextPos );
      SelRange( ptSel );
      EnsureVisible( m_ptEditPos.y, xTextPos );
      UpdateCurrentPosition( xTextPos, true, false );
    }
  } else // VK_NEXT(PAGEDOWN)
  {
    if ( IsTextSelected() ) {
      SelNoneAndUpdateCurrentPosition( VK_NEXT );
    }
    if ( m_pdb->TextIsLastLine( m_ptEditPos.y ) ) {
      EnsureVisibleAndUpdateCurrentPosition();
    } else {
      CPoint cpOld = m_ptEditPos;
      // 确定新的当前行、第一可见行
      if ( m_pdb->TextIsLastLine( GetLastVisibleLine() ) ) {
        m_ptEditPos.y = m_pdb->TextGetLineUpperBound();
      } else {
        LONG nFirstVisibleLine = ScrollVert( SB_PAGEDOWN );
        m_ptEditPos.y = m_pdb->TextVerifyLine( ( m_ptEditPos.y + ( nFirstVisibleLine - m_ptFirstVisible.y ) ) );
        m_ptFirstVisible.y = nFirstVisibleLine;
      }
      // 重新计算当前列
      LONG xTextPos = GetCaretReferPoint();
      m_ptEditPos.x = TransTextPosToCharPos( m_ptEditPos.y, xTextPos );
      EnsureVisibleAndUpdateLines( m_ptEditPos.y, xTextPos, cpOld.y, m_ptEditPos.y );
      UpdateCurrentPosition( xTextPos, true, false );
    }
  }
}
// CPfxEditView 绘制
void CPfxEditView::DrawVisualEOL( HDC hDC, LONG x, LONG y, LONG cx, LONG cy ) {
#ifndef LINE_BREAK_LOOKING_USE_CASE_2
  // 回车符的外观（方案1）
  //           |p3(x2, y1)-------|--------
  //           |                 | |
  //           |                 | |
  //           |                 | 2dx
  //           |                 | |
  // p0(x1, y2) |\   |   /| p2(x3, y2)-|--------
  //      | \  |  / |            | |
  //      |  \ | /  |            | dx
  //      |   \|/   |            | |
  //      |    V p1(x2, y3)------|--------
  //      |         |
  //   ---|---2dx---|---
  LONG dx = ( min( cx, cy ) + 3 ) / 4;
  LONG xc = x + 2 * dx;
  LONG x1 = xc - dx - 1;
  LONG x2 = xc - 1;
  LONG x3 = xc + dx;
  LONG y1 = y + 1 * dx;
  LONG y2 = y + 3 * dx;
  LONG y3 = y + 4 * dx;
  // HDC hDC = pDC;
  ::MoveToEx( hDC, x1, y2, NULL );
  ::LineTo( hDC, x2, y3 );
  ::LineTo( hDC, x3, y2 - 1 );
  ::MoveToEx( hDC, x2, y1, NULL );
  ::LineTo( hDC, x2, y3 );
#else
  // 回车符的外观（方案2）
  //              |
  //             |=|----------------|---
  //   (x2, y1)p2|||p3((x4, y1)     |
  //             |||                |
  //             |||                |2dx
  //             |||                |
  // ---------(x2, y2)p1|+|p4(x4, y2)------+--------------
  // (x1, y2)p0|\==|||==/|p5(x5, y2)--|---
  // (x1, y2)p7| \  |  / |            |
  //         |  \ | /  |            |dx
  //         |    v----+------------|---
  //    (x3, y3)p6|    |
  //         |    |    |
  //       --|---2dx---|--
  POINT pt[ 8 ];
  LONG dx = ( min( cx, cy ) + 3 ) / 4;
  LONG xc = x + 2 * dx;
  LONG x1 = xc - dx;
  LONG x2 = xc - dx / 2;
  LONG x3 = xc;
  LONG x4 = xc + dx / 2;
  LONG x5 = xc + dx;
  LONG y1 = y + 1 * dx;
  LONG y2 = y + 3 * dx;
  LONG y3 = y + 4 * dx;
  pt[ 0 ].x = pt[ 7 ].x = x1;
  pt[ 0 ].y = pt[ 7 ].y = pt[ 1 ].y = pt[ 4 ].y = pt[ 5 ].y = y2;
  pt[ 1 ].x = pt[ 2 ].x = x2;
  pt[ 2 ].y = pt[ 3 ].y = y1;
  pt[ 3 ].x = pt[ 4 ].x = x4;
  pt[ 5 ].x = x5;
  pt[ 6 ].x = x3;
  pt[ 6 ].y = y3;
  ::Polyline( hDC, pt, PFX_EC_SIZEOF( pt ) );
#endif
}
// void DrawVisualEOF(HDC hDC, LONG x, LONG y, LONG cx, LONG cy)
// HDC hDC - 用这个DC来绘制
// LONG x - 左上角x坐标
// LONG y - 左上角y坐标
// LONG cx - 宽度（注：该宽度几乎可以忽略）
// LONG cy - 高度，一般等于字体高度而不是行高度
// 由于文档结束标志是在文档最后一行的最后，而且绝对不会象换行符那样被选
// 择，所以几乎可以忽略cx而只使用cy
void CPfxEditView::DrawVisualEOF( HDC hDC, LONG x, LONG y, LONG cx, LONG cy ) {
#ifndef LINE_BREAK_LOOKING_USE_CASE_2
  // 文档结束标志的外观（方案1）
  //            p0(x2, y1)
  //          /-----------------------|---
  //        /              p3(x3, y2) |
  // p1(x1, y2) <=================|=        |
  //      | \               |         | 2dx
  //      |   \ p2(x2, y3)  |         |
  //      |----|------------|---------|---
  //      | dx |     2dx    |
  // 下面的计算式子是为了和DrawEndOfLine()中的一致，使到绘制出
  // 来的箭头和DrawEndOfLine()中的有一致的外观
  LONG dx = ( min( cx, cy ) + 3 ) / 4;
  LONG yc = y + cy / 2;
  LONG x1 = x + dx;
  LONG x2 = x + 2 * dx;
  LONG x3 = x + 4 * dx;
  LONG y1 = yc - dx;
  LONG y2 = yc;
  LONG y3 = yc + dx;
  // HDC hDC = hDC;
  ::MoveToEx( hDC, x2, y1, NULL );
  ::LineTo( hDC, x1, y2 );
  ::LineTo( hDC, x2 + 1, y3 + 1 );
  ::MoveToEx( hDC, x1, y2, NULL );
  ::LineTo( hDC, x3, y2 );
#else
  // 文档结束标志的外观（方案2）
  //                     |Y
  //                     |
  // (x2, y1) p1|--------|----------------------|---
  //           /|(x2, y2)|            (x3, y2)  |
  // p0(x1, y3)    /  |p2======|===========|p3        |1dy
  // -----------|<-------------+-----------|----------|------X
  // p7(x1, y3) |  \  |p5======|=|=========|p4        |1dy
  //      |    \|(x2, y4)| |         |(x3, y4)  |
  // (x2, y5) p6|--------+-+---------+----------|---
  //      |     |        | |         |
  // ------------|-1dy-|---1dy--|-|---1dy---|----
  //                     |
  //                     |
  POINT pt[ 8 ];
  // 下面的计算式子是为了和DrawEndOfLine()中的一致，使到绘制出
  // 来的箭头和DrawEndOfLine()中的有一致的外观
  LONG dx = ( min( cx, cy ) + 3 ) / 4;
  LONG yc = y + cy / 2;
  LONG x1 = x + dx;
  LONG x2 = x + 2 * dx;
  LONG x3 = x + 4 * dx;
  LONG y1 = yc - dx;
  LONG y2 = yc - dx / 2;
  LONG y3 = yc;
  LONG y4 = yc + dx / 2;
  LONG y5 = yc + dx;
  pt[ 0 ].x = pt[ 7 ].x = x1;
  pt[ 0 ].y = pt[ 7 ].y = y3;
  pt[ 1 ].x = pt[ 2 ].x = pt[ 5 ].x = pt[ 6 ].x = x2;
  pt[ 1 ].y = y1;
  pt[ 2 ].y = pt[ 3 ].y = y2;
  pt[ 3 ].x = pt[ 4 ].x = x3;
  pt[ 5 ].y = pt[ 4 ].y = y4;
  pt[ 6 ].y = y5;
  ::Polyline( hDC, pt, PFX_EC_SIZEOF( pt ) );
#endif
}
void CPfxEditView::DrawVisualTab( HDC hDC, LONG x, LONG y, LONG cx, LONG cy ) {
  // HDC hDC = hDC;
  LONG x2 = x + cx - 2;
  y += ( cy / 2 );
  ::MoveToEx( hDC, x, y, NULL );
  ::LineTo( hDC, x2, y );
  ::MoveToEx( hDC, x2, y - 2, NULL );
  ::LineTo( hDC, x2, y + 3 );
}
// 本函数被调用时DC的属性被设置成TA_UPDATECP | TA_TOP | TA_LEFT，在返回时应
// 确保这些属性未被改变，但函数可以更改当前位置
void CPfxEditView::DrawVisualSpace( HDC hDC, LONG x, LONG y, LONG cx, LONG cy ) {
  // HDC hDC = hDC;
  y += ( cy / 2 );
  ::MoveToEx( hDC, x + ( cx / 4 ), y, NULL );
  ::LineTo( hDC, x + cx - ( cx / 4 ), y );
}
void CPfxEditView::DrawRuler( HDC hdcSrc, const CPfxEditViewPaintContext& pc ) {
  CRect rect;
  CSize size( pc.m_rectRuler.Width(), pc.m_rectRuler.Height() );
  // 如果不绘制标尺，则标尺所在的区域实际上是一空白区域，它的作用是使
  // 窗口上边与正文有一间隔，以避免过分紧密
  if ( !pc.m_bDrawRuler ) {
    HDC hDC = hdcSrc;
    ::SetRect( &rect, 0, 0, pc.m_rectLineNumber.right - pc.m_rectIndent.left, size.cy );
    ::SetBkColor( hDC, pc.COLOR( kcBkLineNumber ) );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
    ::SetRect( &rect, rect.right, 0, size.cx, size.cy );
    ::SetBkColor( hDC, pc.COLOR( kcBkText ) );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
  } else {
    // 以下绘制标尺，使用内存DC缓冲输出
    CDC dcMem;
    dcMem.CreateCompatibleDC( hdcSrc );
    dcMem.SetBkMode( TRANSPARENT );
    HDC hDC = dcMem;
    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap( hdcSrc, size.cx, size.cy );
    HGDIOBJ hBitmapOld = ::SelectObject( hDC, bitmap );
    ::SetTextAlign( hDC, TA_TOP | TA_LEFT | TA_NOUPDATECP );
    // 两个刻度之间的距离是正文字体的平均宽度
    LONG nRulerScale = PFX_EC_DATA.m_nCharWidthAve;
    // 第一个要更新的刻度值
    LONG nCount = pc.m_nTextHorzOffset / nRulerScale;
    nCount /= PFX_EC_RULER_RESOLUTION;
    nCount *= PFX_EC_RULER_RESOLUTION;
    LONG x1 = pc.m_rectText.left;
    LONG x2 = x1 + nCount * nRulerScale - pc.m_nTextHorzOffset;
    LONG yMinor = size.cy - size.cy / 3;
    LONG yMajor = yMinor - 2;
    // 填充标尺背景
    ::SetRect( &rect, 0, 0, size.cx, size.cy );
    ::SetBkColor( hDC, pc.COLOR( kcBkRuler ) );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
    // 绘制标尺刻度
    ::SetTextColor( hDC, pc.COLOR( kcRuler ) );
    ::SetBkColor( hDC, pc.COLOR( kcRuler ) );
    for ( ; x2 <= pc.m_rectRuler.right; x2 += nRulerScale, nCount++ ) {
      // 绘制主刻度
      if ( ( nCount % PFX_EC_RULER_RESOLUTION ) == 0 ) {
        TCHAR sz[ 16 ];
        int szlen = _stprintf( sz, _T( "%d" ), ( nCount / PFX_EC_RULER_RESOLUTION + 1 ) );
        ::SetRect( &rect, x2, yMajor, x2 + PFX_EC_RULER_SCALE_WIDTH, size.cy );
        ::ExtTextOut( hDC, x2, 0, ETO_OPAQUE, &rect, sz, szlen, NULL );
      }
      // 绘制副刻度
      else {
        ::SetRect( &rect, x2, yMinor, x2 + PFX_EC_RULER_SCALE_WIDTH, size.cy );
        ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
      }
    }
    // 剪裁超出范围的文字
    ::SetRect( &rect, 0, 0, x1, size.cy );
    ::SetBkColor( hDC, pc.COLOR( kcBkRuler ) );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
    // 绘制标尺底部的水平线条
    ::SetRect( &rect, 0, ( size.cy - PFX_EC_RULER_SCALE_WIDTH ), size.cx, size.cy );
    ::SetBkColor( hDC, pc.COLOR( kcRuler ) );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
    // 输出标尺
    ::BitBlt( hdcSrc, pc.m_rectRuler.left, pc.m_rectRuler.top, size.cx, size.cy, hDC, 0, 0, SRCCOPY );
    ::SelectObject( hDC, hBitmapOld );
  }
}
// void DrawLineNumber(LONG nLine,HDC hDC,const CPfxEditViewPaintContext& pc)
void CPfxEditView::DrawLineNumber( LONG nLine, HDC hDC, const CPfxEditViewPaintContext& pc ) {
  // HDC hDC = hDC;
  LONG x = pc.m_rectLineNumber.right - pc.m_rectIndent.left;
  LONG y = pc.m_rectLineNumber.top;
  CRect rect;
  // 填充行号与正文之间的间隔
  ::SetRect( &rect, x, y, pc.m_rectLineNumber.right, pc.m_rectLineNumber.bottom );
  ::SetBkColor( hDC, pc.COLOR( kcBkText ) );
  ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, NULL, NULL );
  if ( pc.m_bDrawLineNumber ) {
    TCHAR sz[ 32 ];
    LONG cch;
    if ( nLine != PFX_EC_INVALID_LINE ) {
      cch = _stprintf( sz, _T( "%d" ), ( nLine + 1 ) );
    } else {
      cch = 0;
    }
    ::SetRect( &rect, pc.m_rectLineNumber.left, y, x, pc.m_rectLineNumber.bottom );
    ::SetTextAlign( hDC, TA_RIGHT | TA_TOP | TA_NOUPDATECP );
    ::SetTextColor( hDC, pc.COLOR( kcLineNumber ) );
    ::SetBkColor( hDC, pc.COLOR( kcBkLineNumber ) );
    ::ExtTextOut( hDC, ( x - PFX_EC_LINE_NUMBER_INDENT_RIGHT ), ( y + pc.m_nSpacingLines ), ETO_CLIPPED | ETO_OPAQUE, &rect, sz, cch, NULL );
  }
}
void CPfxEditView::DrawLineBreak( LONG nLine, const CPfxEditLineInfo* pli, HDC hDC, CPfxEditViewPaintContext& pc ) {
  if ( pc.m_ptCurPos.x >= pc.m_rectText.right ) {
    return ;
  }
  if ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
    CPenH pPen;
    if ( IsTextSelected()
         && ( nLine <= m_nSelLineTo )
         && ( nLine >= m_nSelLineFrom )
         && ( pli->nSelTo > pli->nLen ) ) {
      pPen = pc.m_penSelect;
    } else {
      pPen = pc.m_penNormal;
    }
    if ( pc.m_pPenWhitespace != pPen ) {
      ::SelectObject( hDC, pPen );
      pc.m_pPenWhitespace = pPen;
    }
    DrawVisualEOL( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, pc.m_nCharWidthLineBreak, pc.m_nFontHeight );
  } else if ( nLine == m_pdb->TextGetLineUpperBound() ) {
    if ( pc.m_pPenWhitespace != pc.m_penNormal ) {
      ::SelectObject( hDC, pc.m_penNormal );
      pc.m_pPenWhitespace = pc.m_penNormal;
    }
    DrawVisualEOF( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, pc.m_nCharWidthLineBreak, pc.m_nFontHeight );
  }
}
void CPfxEditView::DrawLineBkgnd( LONG nLine, const CPfxEditLineInfo* pli, LONG x, HDC hDC, const CPfxEditViewPaintContext& pc ) {
  CRect rect;
  // HDC hDC = hDC;
  ::SetRect( &rect, x, pc.m_rectText.top, pc.m_rectText.right, pc.m_rectText.bottom );
  if ( nLine == m_ptEditPos.y ) {
    ::SetBkColor( hDC, pc.COLOR( kcBkActiveLine ) );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
    if ( PFX_EC_IS_DRAW_BOOKMARK( pli ) ) {
      ::SetBkColor( hDC, pc.COLOR( kcBkBookmark ) );
      ::SetRect( &rect, x, pc.m_rectText.top, pc.m_rectText.right + 1, pc.m_rectText.top + 1 );
      ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
      ::SetRect( &rect, x, pc.m_rectText.bottom, pc.m_rectText.right + 1, pc.m_rectText.bottom - 1 );
      ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
    }
  } else if ( PFX_EC_IS_DRAW_BOOKMARK( pli ) ) {
    ::SetBkColor( hDC, pc.COLOR( kcBkBookmark ) );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
  } else {
    ::SetBkColor( hDC, pc.COLOR( kcBkText ) );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
  }
}
// void DrawPlainTextNormal(LPCTSTR pData,LONG nLen,LONG& rnLogChar,HDC hDC,CPfxEditViewPaintContext& pc)
void CPfxEditView::DrawPlainTextNormal( LPCTSTR pData, LONG nLen, LONG& rnLogChar, HDC hDC, CPfxEditViewPaintContext& pc ) {
  LONG x2 = pc.m_rectText.right;
  if ( pc.m_ptCurPos.x >= x2 ) {
    return ;
  }
  // 撤换当前用于绘制不可见字符的画笔
  if ( pc.m_bDrawWhitespace && ( pc.m_pPenWhitespace != pc.m_penNormal ) ) {
    ::SelectObject( hDC, pc.m_penNormal );
    pc.m_pPenWhitespace = pc.m_penNormal;
  }
  ::SetTextColor( hDC, pc.COLOR( kcText ) );
  LPCTSTR pTmp = pData;
  LPCTSTR pCur = pData;
  const LPCTSTR pEnd = pData + nLen;
  while ( ( pCur < pEnd ) && ( x2 > pc.m_ptCurPos.x ) ) {
    TCHAR tc = *pCur;
    if ( PFX_EC_IS_LEAD_BYTE( tc ) ) {
      rnLogChar += PFX_EC_MAX_MB_LEN;
      pCur += PFX_EC_MAX_MB_LEN;
      continue;
    }
    if ( tc != PFX_EC_CHR_TAB ) {
      // 如果要绘制空白字符
      if ( ( tc != PFX_EC_CHR_SPACE ) || !pc.m_bDrawWhitespace ) {
        rnLogChar++;
        pCur++;
      } else {
        if ( pCur > pTmp ) {
          ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( pCur - pTmp ), NULL );
          ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
        }
        DrawVisualSpace( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, pc.m_nCharWidthSpace, pc.m_nFontHeight );
        rnLogChar++;
        pTmp = ++pCur;
        pc.m_ptCurPos.x += pc.m_nCharWidthSpace;
        ::MoveToEx( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, NULL );
      }
    } else {
      if ( pCur > pTmp ) {
        ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( pCur - pTmp ), NULL );
        ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
      }
      LONG nTabSize = pc.m_nTabSize - ( rnLogChar % pc.m_nTabSize );
      LONG nTabWidth = nTabSize * pc.m_nCharWidthSpace;
      if ( pc.m_bDrawWhitespace ) {
        DrawVisualTab( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, nTabWidth, pc.m_nFontHeight );
      }
      rnLogChar += nTabSize;
      pTmp = ++pCur;
      pc.m_ptCurPos.x += nTabWidth;
      ::MoveToEx( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, NULL );
    }
  }
  if ( ( pCur > pTmp ) && ( x2 > pc.m_ptCurPos.x ) ) {
    int nTmpLen = min( pEnd, pCur ) - pTmp;
    ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, nTmpLen, NULL );
    ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
  }
}
void CPfxEditView::DrawPlainTextSelection( LONG nLine, const CPfxEditLineInfo* pli, LONG& rnLogChar, HDC hDC, CPfxEditViewPaintContext& pc ) {
  LONG x2 = pc.m_rectText.right;
  // 如果起始绘制点已经在更新区域的右边就不需要进行绘制
  if ( pc.m_ptCurPos.x >= x2 ) {
    return ;
  }
  // 填充背景
  ::SetBkColor( hDC, pc.COLOR( kcBkTextSelected ) );
  ::SetTextColor( hDC, pc.COLOR( kcTextSelected ) );
  CRect rect;
  ::SetRect( &rect, pc.m_ptCurPos.x, pc.m_rectText.top, pc.m_rectText.right, pc.m_rectText.bottom );
  ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
  // 撤换当前用于绘制不可见字符的画笔
  if ( pc.m_bDrawWhitespace && ( pc.m_pPenWhitespace != pc.m_penSelect ) ) {
    ::SelectObject( hDC, pc.m_penSelect );
    pc.m_pPenWhitespace = pc.m_penSelect;
  }
  LPCTSTR pCur = pli->pData + pli->nSelFrom;
  LPCTSTR pTmp = pCur;
  const LPCTSTR pEnd = pli->pData + min( pli->nSelTo, pli->nLen );
  while ( ( pCur < pEnd ) && ( x2 > pc.m_ptCurPos.x ) ) {
    TCHAR tc = *pCur;
    if ( PFX_EC_IS_LEAD_BYTE( tc ) ) {
      rnLogChar += PFX_EC_MAX_MB_LEN;
      pCur += PFX_EC_MAX_MB_LEN;
      continue;
    }
    if ( tc != PFX_EC_CHR_TAB ) {
      // 如果要绘制空白字符
      if ( ( tc != PFX_EC_CHR_SPACE ) || !pc.m_bDrawWhitespace ) {
        rnLogChar++;
        pCur++;
      } else {
        if ( pCur > pTmp ) {
          ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( pCur - pTmp ), NULL );
          ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
        }
        DrawVisualSpace( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, pc.m_nCharWidthSpace, pc.m_nFontHeight );
        rnLogChar++;
        pTmp = ++pCur;
        pc.m_ptCurPos.x += pc.m_nCharWidthSpace;
        ::MoveToEx( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, NULL );
      }
    } else {
      if ( pCur > pTmp ) {
        ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( pCur - pTmp ), NULL );
        ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
      }
      LONG nTabSize = pc.m_nTabSize - ( rnLogChar % pc.m_nTabSize );
      rnLogChar += nTabSize;
      nTabSize *= pc.m_nCharWidthSpace;
      if ( pc.m_bDrawWhitespace ) {
        DrawVisualTab( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, nTabSize, pc.m_nFontHeight );
      }
      pTmp = ++pCur;
      pc.m_ptCurPos.x += nTabSize;
      ::MoveToEx( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, NULL );
    }
  }
  if ( ( pCur > pTmp ) && ( x2 > pc.m_ptCurPos.x ) ) {
    ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( min( pEnd, pCur ) - pTmp ), NULL );
    ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
  }
  // 当前位置不能改变，因为可能要绘制换行符
  LONG xFill = pc.m_ptCurPos.x;
  // 当pli->nSelTo大于pli->nLen时表明这一行的选择包括换行符
  if ( pli->nSelTo > pli->nLen ) {
    xFill += pc.m_nCharWidthLineBreak;
  }
  // 填充后面的背景
  if ( x2 > xFill ) {
    DrawLineBkgnd( nLine, pli, xFill, hDC, pc );
  }
  // 如果是列块选择并且选择内容已经是本行最后的字符，以及选择内容右边
  // 比列块范围的右边小，就填充选择内容最后一个字符与列块范围右边的间
  // 隙，以使看起来是一整块
  if ( IsColumnSelect() && ( x2 > xFill ) && ( xFill < pc.m_rectColSelect.right ) && ( pli->nSelTo >= pli->nLen ) ) {
    ::SetRect( &rect, max( pc.m_rectColSelect.left, xFill ), pc.m_rectText.top, pc.m_rectColSelect.right, pc.m_rectText.bottom );
    ::SetBkColor( hDC, pc.COLOR( kcBkTextSelected ) );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
  }
}
void CPfxEditView::DrawPlainText( HDC hdcSrc, CPfxEditViewPaintContext& pc ) {
  CRect rect;
  BOOL bWrapping = IsWrapping();
  CPoint pt;
  CDC hdcMem;
  hdcMem.CreateCompatibleDC( hdcSrc );
  hdcMem.SetBkMode( TRANSPARENT );
  CBitmap bitmap;
  bitmap.CreateCompatibleBitmap( hdcSrc, pc.m_rectUpdate.Width(), pc.m_rectUpdate.Height() );
  hdcMem.SelectBitmap( bitmap );
  HDC hDC = hdcMem;
  pt.x = ( pc.m_rectText.left - pc.m_nTextHorzOffset );
  pt.y = ( pc.m_rectText.top + pc.m_nSpacingLines );
  for ( LONG i = pc.m_nFirstUpdateLine; i <= pc.m_nLastUpdateLine; i++ ) {
    const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( i );
    LPCTSTR pData = pli->pData;
    LONG nLogChar = pli->nFirstChar;
    // 输出一行文本
    pc.m_ptCurPos.x = pt.x;
    pc.m_ptCurPos.y = pt.y;
    ::SetTextAlign( hDC, TA_LEFT | TA_TOP | TA_UPDATECP );
    ::MoveToEx( hDC, pt.x, pt.y, NULL );
    // 如果这行在选择范围内，就要特别处理
    if ( ( i <= m_nSelLineTo ) && ( i >= m_nSelLineFrom ) ) {
      if ( pli->nSelFrom > 0 ) {
        DrawLineBkgnd( i, pli, pc.m_rectText.left, hdcMem, pc );
        DrawPlainTextNormal( pData, pli->nSelFrom, nLogChar, hdcMem, pc );
      }
      DrawPlainTextSelection( i, pli, nLogChar, hdcMem, pc );
      if ( pli->nSelTo < pli->nLen ) {
        DrawPlainTextNormal( ( pData + pli->nSelTo ), ( pli->nLen - pli->nSelTo ), nLogChar, hdcMem, pc );
      }
    } else {
      DrawLineBkgnd( i, pli, pc.m_rectText.left, hdcMem, pc );
      // 如果这一行没有选择文本，一次过绘制以加快速度
      if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
        DrawPlainTextNormal( pData, pli->nLen, nLogChar, hdcMem, pc );
      }
    }
    // 绘制回车符
    if ( pc.m_bDrawLineBreak ) {
      DrawLineBreak( i, pli, hdcMem, pc );
    }
    // 绘制行号
    DrawLineNumber( ( ( !PFX_EC_IS_SUBLINE( pli ) && !bWrapping ) ? i : pli->nIndex ), hdcMem, pc );
    // 为列块选择操作支持
    if ( ( i <= pc.m_rectColSelect.bottom ) && ( i >= pc.m_rectColSelect.top ) ) {
      ::SetRect( &rect, pc.m_rectColSelect.left, pc.m_rectText.top, pc.m_rectColSelect.right, pc.m_rectText.bottom );
      ::InvertRect( hDC, &rect );
    }
    // 输出到屏幕
    ::BitBlt( hdcSrc, pc.m_rectUpdate.left, pc.m_rectUpdate.top, pc.m_sizeBitmap.cx, pc.m_sizeBitmap.cy, hDC, 0, 0, SRCCOPY );
    // 下一行
    pc.m_rectUpdate.top += pc.m_sizeBitmap.cy;
  }
}
// 分析一给定的字符串，如果它是关键字就用相应的颜色输出，否则用普通的文本颜色输出
// LPCTSTR pData - 字符串地址
// LONG nLen     - 字符串长度
// HDC hDC       - 用于输出的CDC指针
// CPfxEditViewPaintContext& pc - 与显示相关的参数
// 该字符串内不得含有空白字符
void CPfxEditView::DrawHilightedString1( LPCTSTR pData, LONG nLen, HDC hDC, CPfxEditViewPaintContext& pc ) {
  if ( ( pData == NULL ) || ( nLen <= 0 ) ) {
    return ;
  }
  LONG i = m_pti->m_mapKeywords.Lookup( pData, nLen );
  COLORREF clr = pc.COLOR( ( i != -1 ) ? i : kcText );
  if ( clr != pc.m_clrCurText ) {
    pc.m_clrCurText = clr;
    ::SetTextColor( hDC, clr );
  }
  ::ExtTextOut( hDC, 0, 0, 0, NULL, pData, nLen, NULL );
  ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
}
// 输出一指定的字符串，该字符串将会用同一种颜色输出
// LPCTSTR pData    - 字符串的地址
// LONG nLen        - 字符串的长度
// LONG& rnLogChar  - 与字符串第一个字符对应的逻辑字符数
// COLORREF color   - 输出的文本颜色
// HDC hDC          - 用于输出的CDC指针
// CPfxEditViewPaintContext& pc - 与显示相关的参数
void CPfxEditView::DrawHilightedString2( LPCTSTR pData, LONG nLen, LONG& rnLogChar, COLORREF color, HDC hDC, CPfxEditViewPaintContext& pc ) {
  if ( ( pData == NULL ) || ( nLen == 0 ) ) {
    return ;
  }
  CDCH pDC = hDC;
  LONG x2 = pc.m_rectText.right;
  pc.m_clrCurText = color;
  ::SetTextColor( hDC, pc.m_clrCurText );
  LPCTSTR pCur = pData;
  LPCTSTR pTmp = pCur;
  const LPCTSTR pEnd = pData + nLen;
  while ( ( pCur < pEnd ) && ( x2 > pc.m_ptCurPos.x ) ) {
    TCHAR tc = *pCur;
#ifdef _MBCS
    if ( PFX_EC_IS_LEAD_BYTE( tc ) ) {
      rnLogChar += PFX_EC_MAX_MB_LEN;
      pCur += PFX_EC_MAX_MB_LEN;
      continue;
    }
#endif
    if ( tc != PFX_EC_CHR_TAB ) {
      // 如果要绘制空白字符
      if ( ( tc != PFX_EC_CHR_SPACE ) || !pc.m_bDrawWhitespace ) {
        rnLogChar++;
        pCur++;
      } else {
        if ( pCur > pTmp ) {
          ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( pCur - pTmp ), NULL );
          ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
        }
        DrawVisualSpace( pDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, pc.m_nCharWidthSpace, pc.m_nFontHeight );
        rnLogChar++;
        pTmp = ++pCur;
        pc.m_ptCurPos.x += pc.m_nCharWidthSpace;
        ::MoveToEx( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, NULL );
      }
    } else {
      if ( pCur > pTmp ) {
        ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( pCur - pTmp ), NULL );
        ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
      }
      LONG nTabSize = pc.m_nTabSize - ( rnLogChar % pc.m_nTabSize );
      LONG nTabWidth = nTabSize * pc.m_nCharWidthSpace;
      if ( pc.m_bDrawWhitespace ) {
        DrawVisualTab( pDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, nTabWidth, pc.m_nFontHeight );
      }
      rnLogChar += nTabSize;
      pTmp = ++pCur;
      pc.m_ptCurPos.x += nTabWidth;
      ::MoveToEx( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, NULL );
    }
  }
  // 输出剩余部分
  if ( ( pCur > pTmp ) && ( x2 > pc.m_ptCurPos.x ) ) {
    ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( min( pEnd, pCur ) - pTmp ), NULL );
    ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
  }
}
// 根据给出的上一行的COOKIE来分析并显示与该COOKIE相关的字符部分
// LPCTSTR pData   - 一行数据的开始
// LONG nLen       - 该行数据的长度
// LONG& rnLogChar - 开始的逻辑字符数
// DWORD dwCookie  - COOKIE
// HDC hDC        - 用于输出的CDC指针
// CPfxEditViewPaintContext& pc - 与显示相关的参数
// 返回NULL表示不需要继续分析和显示，返回非NULL表示应该从该地址起继续分析下面的内容
LPCTSTR CPfxEditView::DrawHilightedStringBaseOnCookie( LPCTSTR pData, LONG nLen, LONG& rnLogChar, DWORD dwCookie, HDC hDC, CPfxEditViewPaintContext& pc ) {
  CDCH pDC = hDC;
  const LPCTSTR pStart = pData;
  const LPCTSTR pEnd = pStart + nLen;
  // 由上一行开始使用了行注释：
  // abcdefg
  // hijklmn
  // 这种情况只需要输出当前行的全部内容便可
  if ( dwCookie & PFX_EC_LF_COOKIE_LINE_COMMENT1 ) {
    DrawHilightedString2( pStart, ( pEnd - pStart ), rnLogChar, pc.COLOR( kcLineComment1 ), pDC, pc );
    return NULL;
  }
  if ( dwCookie & PFX_EC_LF_COOKIE_LINE_COMMENT2 ) {
    DrawHilightedString2( pStart, ( pEnd - pStart ), rnLogChar, pc.COLOR( kcLineComment2 ), pDC, pc );
    return NULL;
  }
  // 这一行的开头部分或全部是块注释的一部分：
  // abcdefg /* this is block comment
  // samples */ hijklmn
  if ( dwCookie & PFX_EC_LF_COOKIE_BLOCK_COMMENT1 ) {
    LPCTSTR pFound = ParserGetMarkerCloseEndPosition( pStart, pStart, pEnd, false, m_pti->GetMarkerPair( mpBlockComment1 ) );
    if ( pFound == NULL )           //      没有找到？一直输出到行末
    {
      pFound = pEnd;
    }
    DrawHilightedString2( pStart, ( pFound - pStart ), rnLogChar, pc.COLOR( kcBlockComment1 ), pDC, pc );
    return ( ( pFound == pEnd ) ? NULL : pFound );
  }
  if ( dwCookie & PFX_EC_LF_COOKIE_BLOCK_COMMENT2 ) {
    LPCTSTR pFound = ParserGetMarkerCloseEndPosition( pStart, pStart, pEnd, false, m_pti->GetMarkerPair( mpBlockComment2 ) );
    if ( pFound == NULL )           //      没有找到？一直输出到行末
    {
      pFound = pEnd;
    }
    DrawHilightedString2( pStart, ( pFound - pStart ), rnLogChar, pc.COLOR( kcBlockComment2 ), pDC, pc );
    return ( ( pFound == pEnd ) ? NULL : pFound );
  }
  if ( dwCookie & PFX_EC_LF_COOKIE_QUOTED_STRING1 ) {
    LPCTSTR pFound = ParserGetMarkerCloseEndPosition( pStart, pStart, pEnd, false, m_pti->GetMarkerPair( mpQuotedString1 ) );
    if ( pFound == NULL )           //      没有找到？一直输出到行末
    {
      pFound = pEnd;
    }
    DrawHilightedString2( pStart, ( pFound - pStart ), rnLogChar, pc.COLOR( kcQuotedString1 ), pDC, pc );
    return ( ( pFound == pEnd ) ? NULL : pFound );
  }
  if ( dwCookie & PFX_EC_LF_COOKIE_QUOTED_STRING2 ) {
    LPCTSTR pFound = ParserGetMarkerCloseEndPosition( pStart, pStart, pEnd, false, m_pti->GetMarkerPair( mpQuotedString2 ) );
    if ( pFound == NULL )           //      没有找到？一直输出到行末
    {
      pFound = pEnd;
    }
    DrawHilightedString2( pStart, ( pFound - pStart ), rnLogChar, pc.COLOR( kcQuotedString2 ), pDC, pc );
    return ( ( pFound == pEnd ) ? NULL : pFound );
  }
  return pStart;
}
// 绘制语法高亮时一行中的非选择部分
void CPfxEditView::DrawHilightedNormal( LONG nLine, const CPfxEditLineInfo* pli, HDC hDC, CPfxEditViewPaintContext& pc ) {
  UNUSED( nLine );
  ASSERT( !PFX_EC_IS_EMPTY_LINE( pli ) );
  CDCH pDC = hDC;
  LONG x2 = pc.m_rectText.right;
  const LPCTSTR pStart = pli->pData;
  const LPCTSTR pEnd = pStart + pli->nLen;
  LPCTSTR pCur = pStart;
  LONG nLogChar = pli->nFirstChar;
  DWORD dwCookie = pli->dwFlags & PFX_EC_LF_COOKIE_ALL;
  // 撤换当前用于绘制不可见字符的画笔
  if ( pc.m_bDrawWhitespace && ( pc.m_pPenWhitespace != pc.m_penNormal ) ) {
    ::SelectObject( hDC, pc.m_penNormal );
    pc.m_pPenWhitespace = pc.m_penNormal;
  }
  pc.m_clrCurText = pc.COLOR( kcText );
  ::SetTextColor( hDC, pc.m_clrCurText );
  pCur = DrawHilightedStringBaseOnCookie( pStart, pli->nLen, nLogChar, dwCookie, pDC, pc );
  // 如果这一行是上一行COOKIE的延续，就不需要继续
  if ( pCur == NULL ) {
    return ;
  }
  // 每次分析一个字符的时候将该字符和下面的标识符的第一个字符比较，如
  // 果不等就则不需要继续比较，这样可以加快分析速度
  CPfxEditViewMarkerChar mc;
  ParserGetFirstCharOfMarkerStart( mc );
  LPCTSTR pTmp = pCur;
  while ( ( pCur < pEnd ) && ( x2 > pc.m_ptCurPos.x ) ) {
    BOOL bLeadByte = PFX_EC_IS_LEAD_BYTE( *pCur );
    WCHAR wc = ( ( !bLeadByte || ( ( pCur + PFX_EC_MAX_MB_LEN ) > pEnd ) ) ? ( ( WCHAR ) * pCur ) : ( *( LPWSTR ) pCur ) );
    // 行注释1
    if ( ( ( wc == mc.m_cLowerLineComment1 ) || ( wc == mc.m_cUpperLineComment1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpLineComment1 ) ) ) {
      DrawHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      DrawHilightedString2( pCur, ( pEnd - pCur ), nLogChar, pc.COLOR( kcLineComment1 ), pDC, pc );
      pCur = pTmp = pEnd;
      break;
    }
    // 行注释2
    if ( ( ( wc == mc.m_cLowerLineComment2 ) || ( wc == mc.m_cUpperLineComment2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpLineComment2 ) ) ) {
      DrawHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      DrawHilightedString2( pCur, ( pEnd - pCur ), nLogChar, pc.COLOR( kcLineComment2 ), pDC, pc );
      pCur = pTmp = pEnd;
      break;
    }
    // 块注释1
    if ( ( ( wc == mc.m_cLowerBlockComment1 ) || ( wc == mc.m_cUpperBlockComment1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpBlockComment1 ) ) ) {
      DrawHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      pTmp = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpBlockComment1 ) );
      if ( pTmp == NULL )           //      没有找到？一直输出到行末
      {
        pTmp = pEnd;
      }
      DrawHilightedString2( pCur, ( pTmp - pCur ), nLogChar, pc.COLOR( kcBlockComment1 ), pDC, pc );
      pCur = pTmp;
      continue;
    }
    if ( ( ( wc == mc.m_cLowerBlockComment2 ) || ( wc == mc.m_cUpperBlockComment2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpBlockComment2 ) ) ) {
      DrawHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      pTmp = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpBlockComment2 ) );
      if ( pTmp == NULL )           //      没有找到？一直输出到行末
      {
        pTmp = pEnd;
      }
      DrawHilightedString2( pCur, ( pTmp - pCur ), nLogChar, pc.COLOR( kcBlockComment2 ), pDC, pc );
      pCur = pTmp;
      continue;
    }
    // 常量字符串1
    if ( ( ( wc == mc.m_cLowerQuotedString1 ) || ( wc == mc.m_cUpperQuotedString1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpQuotedString1 ) ) ) {
      DrawHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      pTmp = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpQuotedString1 ) );
      if ( pTmp == NULL )           //      没有找到？一直输出到行末
      {
        pTmp = pEnd;
      }
      DrawHilightedString2( pCur, ( pTmp - pCur ), nLogChar, pc.COLOR( kcQuotedString1 ), pDC, pc );
      pCur = pTmp;
      continue;
    }
    // 常量字符串2
    if ( ( ( wc == mc.m_cLowerQuotedString2 ) || ( wc == mc.m_cUpperQuotedString2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpQuotedString2 ) ) ) {
      DrawHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      pTmp = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpQuotedString2 ) );
      if ( pTmp == NULL )           //      没有找到？一直输出到行末
      {
        pTmp = pEnd;
      }
      DrawHilightedString2( pCur, ( pTmp - pCur ), nLogChar, pc.COLOR( kcQuotedString2 ), pDC, pc );
      pCur = pTmp;
      continue;
    }
    // 如果这个字符不是块的标识符，下面将会分析它是否是关键字的
    // 一部分，或者是空白字符，或者是单词定界符等
    if ( !IsWhitespace( wc ) && !IsDelimiter( wc ) ) {
      if ( bLeadByte ) {
        pCur += PFX_EC_MAX_MB_LEN;
        nLogChar += PFX_EC_MAX_MB_LEN;
      } else {
        pCur++;
        nLogChar++;
      }
    } else {
      if ( pCur > pTmp )           //      绘制之前的部分
      {
        DrawHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
        pTmp = pCur;
      }
      if ( wc == PFX_EC_CHR_SPACE ) {
        if ( pc.m_bDrawWhitespace ) {
          DrawVisualSpace( pDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, pc.m_nCharWidthSpace, pc.m_nFontHeight );
        }
        pCur++;
        nLogChar++;
        pc.m_ptCurPos.x += pc.m_nCharWidthSpace;
        ::MoveToEx( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, NULL );
        pTmp = pCur;
      } else if ( wc == PFX_EC_CHR_TAB ) {
        LONG nTabSize = pc.m_nTabSize - ( nLogChar % pc.m_nTabSize );
        LONG nTabWidth = nTabSize * pc.m_nCharWidthSpace;
        if ( pc.m_bDrawWhitespace ) {
          DrawVisualTab( pDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, nTabWidth, pc.m_nFontHeight );
        }
        pCur++;
        nLogChar += nTabSize;
        pc.m_ptCurPos.x += nTabWidth;
        ::MoveToEx( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, NULL );
        pTmp = pCur;
      } else // Delimiter!
      {
        if ( bLeadByte ) {
          pCur += PFX_EC_MAX_MB_LEN;
          nLogChar += PFX_EC_MAX_MB_LEN;
        } else {
          pCur++;
          nLogChar++;
        }
        //  有些编程语言的关键字中可能含有单词定界符例如汇编语言经常有“.XLIST，.XALL”， 等
        //  伪指令，C++语言有“==，!=，->，>>” 等运算符，因此当遇到单词定界符时应该再分析一
        //  下它后面的单词与它是否是一个关键字
        LPCTSTR pTmpData = pCur;
        LONG nTmpLogChar = nLogChar;
        while ( ( pTmpData < pEnd ) && !IsWhitespace( *pTmpData ) ) {
          LONG nStep = ( PFX_EC_IS_LEAD_BYTE( *pTmpData ) ? PFX_EC_MAX_MB_LEN : 1 );
          //   如果遇到的第一个字符就是单词定界
          //   符，就包括该定界符，否则不包括它
          if ( IsDelimiter( pTmpData, pEnd ) ) {
            if ( pTmpData == pCur ) {
              pTmpData += nStep;
              nTmpLogChar += nStep;
            }
            break;
          }
          pTmpData += nStep;
          nTmpLogChar += nStep;
        }
        pTmpData = min( pTmpData, pEnd );
        if ( ( pTmpData != pCur )
             && ParserIsKeyword( pTmp, ( pTmpData - pTmp ) ) ) {
          pCur = pTmpData;
          nLogChar = nTmpLogChar;
        }
        DrawHilightedString1( pTmp, ( min( pEnd, pCur ) - pTmp ), pDC, pc );
        pTmp = pCur;
      }
    }
  }
  // 绘制剩余的部分，由于没有考虑到自动换行时子行的问题，所以当一个关
  // 键字被强行折断的时候，它将不会被正确地分析和正确的显示
  if ( ( pCur > pTmp ) && ( x2 > pc.m_ptCurPos.x ) ) {
    DrawHilightedString1( pTmp, ( min( pEnd, pCur ) - pTmp ), pDC, pc );
  }
}
// void DrawHilightedSelection(LONG nLine,const CPfxEditLineInfo* pli,HDC hDC,CPfxEditViewPaintContext& pc)
// 绘制语法高亮时一行中的选择部分
void CPfxEditView::DrawHilightedSelection( LONG nLine, const CPfxEditLineInfo* pli, HDC hDC, CPfxEditViewPaintContext& pc ) {
  CDCH pDC = hDC;
  LONG x2 = pc.m_rectText.right;
  LONG nLogChar = pli->nFirstChar;
  const LPCTSTR pLineStart = pli->pData;
  const LPCTSTR pLineEnd = pLineStart + pli->nLen;
  const LPCTSTR pSelStart = pLineStart + pli->nSelFrom;
  const LPCTSTR pSelEnd = pLineStart + min( pli->nSelTo, pli->nLen );
  // 计算绘制坐标
  CPoint pt;
  ::GetCurrentPositionEx( hDC, &pt );
  // 如果起始绘制点已经在更新区域的右边就不需要进行绘制
  if ( pt.x >= x2 ) {
    return ;
  }
  // 如果选择部分的第一个字符不是本行的第一个字符，就要计算选择部分第一个字符所在的水平坐标，
  // 这个坐标用于绘制，另外还要计算与该字符对应的逻辑列数
  if ( pSelStart > pLineStart ) {
    pt.x += CalcStringWidth( pLineStart, ( pSelStart - pLineStart ), nLogChar, pDC );
  }
  // 如果起始绘制点已经在更新区域的右边就不需要进行绘制
  if ( x2 <= pt.x ) {
    return ;
  }
  CPoint ptEnd;
  ptEnd.x = pc.m_rectText.right;
  // 如果选择部分的最后字符不是本行的最后字符，就要计算选择部分的宽度
  if ( pSelEnd < pLineEnd ) {
    LONG nLogCharTemp = nLogChar;
    ptEnd.x = ( pt.x + CalcStringWidth( pSelStart, ( pSelEnd - pSelStart ), nLogCharTemp, pDC ) );
  }
  ::MoveToEx( hDC, pt.x, pt.y, NULL );
  // 填充背景
  ::SetBkColor( hDC, pc.COLOR( kcBkTextSelected ) );
  ::SetTextColor( hDC, pc.COLOR( kcTextSelected ) );
  CRect rect;
  ::SetRect( &rect, pt.x, pc.m_rectText.top, ptEnd.x, pc.m_rectText.bottom );
  ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
  // 撤换当前用于绘制不可见字符的画笔
  if ( pc.m_bDrawWhitespace && ( pc.m_pPenWhitespace != pc.m_penSelect ) ) {
    ::SelectObject( hDC, pc.m_penSelect );
    pc.m_pPenWhitespace = pc.m_penSelect;
  }
  // 开始绘制
  LPCTSTR pTmp = pSelStart;
  LPCTSTR pCur = pSelStart;
  LPCTSTR pEnd = pSelEnd;
  while ( ( pCur < pEnd ) && ( x2 > pt.x ) ) {
    TCHAR tc = *pCur;
    if ( PFX_EC_IS_LEAD_BYTE( tc ) ) {
      pCur += PFX_EC_MAX_MB_LEN;
      nLogChar += PFX_EC_MAX_MB_LEN;
      continue;
    }
    if ( tc != PFX_EC_CHR_TAB ) {
      // 如果要绘制空白字符
      if ( ( tc != PFX_EC_CHR_SPACE ) || !pc.m_bDrawWhitespace ) {
        pCur++;
        nLogChar++;
      } else {
        if ( pCur > pTmp ) {
          ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( pCur - pTmp ), NULL );
          ::GetCurrentPositionEx( hDC, &pt );
        }
        DrawVisualSpace( pDC, pt.x, pt.y, pc.m_nCharWidthSpace, pc.m_nFontHeight );
        nLogChar++;
        pTmp = ++pCur;
        pt.x += pc.m_nCharWidthSpace;
        ::MoveToEx( hDC, pt.x, pt.y, NULL );
      }
    } else {
      if ( pCur > pTmp ) {
        ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( pCur - pTmp ), NULL );
        ::GetCurrentPositionEx( hDC, &pt );
      }
      LONG nTabSize = pc.m_nTabSize - ( nLogChar % pc.m_nTabSize );
      nLogChar += nTabSize;
      nTabSize *= pc.m_nCharWidthSpace;
      if ( pc.m_bDrawWhitespace ) {
        DrawVisualTab( pDC, pt.x, pt.y, nTabSize, pc.m_nFontHeight );
      }
      pTmp = ++pCur;
      pt.x += nTabSize;
      ::MoveToEx( hDC, pt.x, pt.y, NULL );
    }
  }
  // 输出未绘制的剩余部分
  if ( ( pCur > pTmp ) && ( x2 > pt.x ) ) {
    ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( min( pEnd, pCur ) - pTmp ), NULL );
    ::GetCurrentPositionEx( hDC, &pt );
  }
  // 当pli->nSelTo大于pli->nLen时表明这一行的选择包括换行符
  if ( pli->nSelTo > pli->nLen ) {
    pt.x += pc.m_nCharWidthLineBreak;
  }
  // 填充后面的背景
  if ( ( x2 > pt.x ) && ( pSelEnd >= pLineEnd ) ) {
    DrawLineBkgnd( nLine, pli, pt.x, pDC, pc );
  }
  // 如果是列块选择并且选择内容已经是本行最后的字符，以及选择内容右边比列块范围的右边小，
  // 就填充选择内容最后一个字符与列块范围右边的间隙，以使看起来是一整块
  if ( IsColumnSelect()
       && ( x2 > pt.x )
       && ( pt.x < pc.m_rectColSelect.right )
       && ( pli->nSelTo >= pli->nLen ) ) {
    ::SetRect( &rect, max( pc.m_rectColSelect.left, pt.x ), pc.m_rectText.top, pc.m_rectColSelect.right, pc.m_rectText.bottom );
    ::SetBkColor( hDC, pc.COLOR( kcBkTextSelected ) );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
  }
}
void CPfxEditView::DrawHilighted( HDC hdcSrc, CPfxEditViewPaintContext& pc ) {
  CRect rect;
  CDC hdcMem;
  hdcMem.CreateCompatibleDC( hdcSrc );
  hdcMem.SetBkMode( TRANSPARENT );
  CBitmap bitmap;
  bitmap.CreateCompatibleBitmap( hdcSrc, pc.m_rectUpdate.Width(), pc.m_rectUpdate.Height() );
  hdcMem.SelectBitmap( bitmap );
  HDC hDC = hdcMem;
  BOOL bWrapping = IsWrapping();
  CPoint pt;
  pt.x = ( pc.m_rectText.left - pc.m_nTextHorzOffset );
  pt.y = ( pc.m_rectText.top + pc.m_nSpacingLines );
  for ( LONG i = pc.m_nFirstUpdateLine; i <= pc.m_nLastUpdateLine; i++ ) {
    const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( i );
    // 输出一行文本
    pc.m_ptCurPos.x = pt.x;
    pc.m_ptCurPos.y = pt.y;
    ::SetTextAlign( hDC, TA_LEFT | TA_TOP | TA_UPDATECP );
    ::MoveToEx( hDC, pt.x, pt.y, NULL );
    if ( ( i <= m_nSelLineTo ) && ( i >= m_nSelLineFrom ) ) {
      BOOL bNeedUpdateCurrentPosition = false;
      if ( ( pli->nSelFrom > 0 ) || ( pli->nSelTo < pli->nLen ) ) {
        DrawLineBkgnd( i, pli, pc.m_rectText.left, hdcMem, pc );
        DrawHilightedNormal( i, pli, hdcMem, pc );
        //  恢复原来的坐标，因为绘制选择部分的代码必
        //  须从头开始计算
        ::MoveToEx( hDC, pt.x, pt.y, NULL );
        bNeedUpdateCurrentPosition = true;
      }
      DrawHilightedSelection( i, pli, hdcMem, pc );
      // 因为非选择部分总是先输出一整行，然后再在上面覆盖
      // 选择内容，但无论如何如果要绘制换行符，该换行符总
      // 是在行末，所以要保存行末位置
      if ( !bNeedUpdateCurrentPosition ) {
        ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
      } else {
        ::MoveToEx( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, NULL );
      }
    } else {
      DrawLineBkgnd( i, pli, pc.m_rectText.left, hdcMem, pc );
      // 如果这一行没有选择文本，一次过绘制以加快速度
      if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
        DrawHilightedNormal( i, pli, hdcMem, pc );
      }
    }
    // 绘制回车符
    if ( pc.m_bDrawLineBreak ) {
      DrawLineBreak( i, pli, hdcMem, pc );
    }
    // 绘制行号
    DrawLineNumber( ( ( !PFX_EC_IS_SUBLINE( pli ) && !bWrapping ) ? i : pli->nIndex ), hdcMem, pc );
    // 为列块选择操作支持
    if ( ( i <= pc.m_rectColSelect.bottom ) && ( i >= pc.m_rectColSelect.top ) ) {
      ::SetRect( &rect, pc.m_rectColSelect.left, pc.m_rectText.top, pc.m_rectColSelect.right, pc.m_rectText.bottom );
      ::InvertRect( hDC, &rect );
    }
    // 输出到屏幕
    ::BitBlt( hdcSrc, pc.m_rectUpdate.left, pc.m_rectUpdate.top, pc.m_sizeBitmap.cx, pc.m_sizeBitmap.cy, hDC, 0, 0, SRCCOPY );
    // 下一行
    pc.m_rectUpdate.top += pc.m_sizeBitmap.cy;
  }
  // 填充未绘制的剩余屏幕部分
  if ( pc.m_rectUpdate.top < pc.m_rectUpdate.bottom ) {
    hDC = hdcSrc;
    // 填充行号区背景
    i = ( pc.m_rectLineNumber.right - pc.m_rectIndent.left );
    if ( i > 0 )           //      如果不显示行号就不需要填充
    {
      ::SetBkColor( hDC, pc.COLOR( kcBkLineNumber ) );
      ::SetRect( &rect, 0, pc.m_rectUpdate.top, i, pc.m_rectUpdate.bottom );
      ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
    }
    // 填充文本区背景
    ::SetBkColor( hDC, pc.COLOR( kcBkText ) );
    ::SetRect( &rect, i, pc.m_rectUpdate.top, pc.m_rectText.right, pc.m_rectUpdate.bottom );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
  }
}
// CPfxEditView 辅助函数
LONG CPfxEditView::TransViewPosToTextPosX( LONG xViewPos ) const {
  LONG x = ( ( m_ptFirstVisible.x * PFX_EC_DATA.m_nCharWidthAve ) + ( xViewPos - m_rectText.left ) );
  return max( 0, x );
}
LONG CPfxEditView::TransViewPosToTextPosY( LONG yViewPos ) const {
  LONG y = ( ( m_ptFirstVisible.y * PFX_EC_DATA.m_nLineHeight ) + ( yViewPos - m_rectText.top ) );
  return max( 0, y );
}
LONG CPfxEditView::TransTextPosToViewPosX( LONG xTextPos ) const {
  ASSERT( xTextPos >= 0 );
  return m_rectText.left + ( xTextPos - ( m_ptFirstVisible.x * PFX_EC_DATA.m_nCharWidthAve ) );
}
LONG CPfxEditView::TransTextPosToViewPosY( LONG yTextPos ) const {
  ASSERT( yTextPos >= 0 );
  return m_rectText.top + ( yTextPos - ( m_ptFirstVisible.y * PFX_EC_DATA.m_nLineHeight ) );
}
// 返回与指定行相应的文本坐标，该坐标从文本影像的垂直方向起点开始计算
LONG CPfxEditView::TransLinePosToTextPos( LONG nLine ) const {
  ASSERT( m_pdb->TextIsValidLine( nLine ) );
  return ( nLine * PFX_EC_DATA.m_nLineHeight );
}
// 返回与指定行中某列相对应的文本坐标，该坐标从文本影像的水平方向起点开始计算
// 如果rpt.x不是在正确的位置上，则函数试图修正该参数
LONG CPfxEditView::TransCharPosToTextPos( CPoint& rpt ) const {
  ASSERT( m_pdb->TextIsValidLine( rpt.y ) );
  ASSERT( m_pdb->TextIsValidChar( rpt.y, rpt.x ) );
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( rpt.y );
  if ( ( rpt.x == 0 ) || PFX_EC_IS_EMPTY_LINE( pli ) ) {
    return 0;
  }
  // 在这里不能使用GetTabbedTextExtent()，因为它好象不能正常工作（或
  // 者由于字体有KERNINGPAIR对的原因，使它返回的数值与视图的绘图输出
  // 模块所计算出来字符串长度不一致），另外它最大只能返回
  // USHRT_MAX(65535)的逻辑单元数，这个数值上限不能满足我们的要求，例
  // 如控件允许一行的最大长度是8192（Win9x文本输出函数的最大上限），
  // 而每个字符的宽度是10个逻辑单元，则GetTabbedTextExtent()就不能胜
  // 任了，所以在这里我们必须使用自己的与绘图输出模块一致的计算方法，
  // 使到绘图输出与插入符定位吻合
  CClientDC1 dc( const_cast<CPfxEditView*>( this ) );
  HGDIOBJ hFont = dc.SelectObject( PFX_EC_DATA.m_hFontText );
  LPCTSTR pLineStart = pli->pData;
  LPCTSTR pLineEnd = pLineStart + pli->nLen;
  LPCTSTR pCur = pLineStart;
  LPCTSTR pEnd = pLineStart + min( rpt.x, pli->nLen );
  LONG nLogChar = pli->nFirstChar;
  LONG nTestPos = 0;
  while ( pCur < pEnd ) {
    if ( PFX_EC_IS_LEAD_BYTE( *pCur ) ) {
      pCur += PFX_EC_MAX_MB_LEN;
      nLogChar += PFX_EC_MAX_MB_LEN;
      continue;
    }
    if ( *pCur != PFX_EC_CHR_TAB ) {
      pCur++;
      nLogChar++;
    } else {
      LONG nTabSize = GetTabSize() - ( nLogChar % GetTabSize() );
      nTestPos += ( ( nTabSize * PFX_EC_DATA.m_nCharWidthSpace ) - PFX_EC_DATA.m_nCharWidthTab );
      pCur++;
      nLogChar += nTabSize;
    }
  }
  // 重新用真正的行末来参照，这样可以避免原来的rpt.x落在一个MBCS字符的
  // 中间而导致的错误
  rpt.x = min( pCur, pLineEnd ) - pLineStart;
  CSize size = dc.GetTextExtent( pLineStart, rpt.x );
  nTestPos += size.cx;
  dc.SelectObject( hFont );
  return nTestPos;
}
// 计算给定的垂直Y坐标落在哪一行上，并返回该行的行序号
// LONG yTextPos - 是相对于文本影像垂直方向起始点的坐标返回相应的行序号
LONG CPfxEditView::TransTextPosToLinePos( LONG yTextPos ) const {
  ASSERT( yTextPos >= 0 );
  return min( ( yTextPos / PFX_EC_DATA.m_nLineHeight ), m_pdb->TextGetLineUpperBound() );
}
// 计算在一行中给定的水平坐标X落在哪个字符上，并返回该字符的X文本坐标和
// 该字符的偏移量
// LONG nLine     - 行序号
// LONG rxTextPos - 是相对于文本影像水平方向起始点的坐标，函数返回后该地方存放与返回值（列序号）
//               相对应的相对于文本影像水平方向起始点的坐标，该坐标通常用于定位插入符
// LONG& rxTextPos 接收该字符的水平坐标
// 返回与xTextPos相对应的字符的列序号（以TCHAR为单位）
// 函数根据如下规则进行换算，如果x落在某个字符的前半部分，则返回该字符
// 所在列号（以零为基值），否则返回下一个字符或行末的列号
LONG CPfxEditView::TransTextPosToCharPos( LONG nLine, LONG& rxTextPos ) const {
  ASSERT( rxTextPos >= 0 );
  ASSERT( m_pdb->TextIsValidLine( nLine ) );
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( nLine );
  LPCTSTR pStart = pli->pData;
  LPCTSTR pCur = pStart;
  LONG nTestPos = 0;
  if ( ( rxTextPos > 0 ) && !PFX_EC_IS_EMPTY_LINE( pli ) ) {
    nTestPos = rxTextPos;
    LONG nTestLen = nTestPos / PFX_EC_DATA.m_nCharWidthMax;
    nTestLen = min( nTestLen, ( PFX_EC_MAX_LINE_LENGTH / 50 ) );
    nTestLen = max( nTestLen, 1 );
    LPCTSTR pEnd = pStart + pli->nLen;
    CClientDC1 dc( const_cast<CPfxEditView*>( this ) );
    HGDIOBJ hFont = dc.SelectObject( PFX_EC_DATA.m_hFontText );
    CalcCharsToFit( pCur, nTestPos, dc, pStart, pEnd, 0, nTestLen, pli->nFirstChar );
    dc.SelectObject( hFont );
    pCur = min( pCur, pEnd );
  }
  rxTextPos = nTestPos;
  return ( pCur - pStart );
}
CPoint CPfxEditView::TransViewPosToEditPos( CPoint pt ) const {
  CPoint ptText;
  ptText.x = TransViewPosToTextPosX( pt.x );
  ptText.y = TransViewPosToTextPosY( pt.y );
  CPoint ptEdit;
  ptEdit.y = TransTextPosToLinePos( ptText.y );
  ptEdit.x = TransTextPosToCharPos( ptText.y, ptText.x );
  m_pdb->TextSubLineToAbsLine( ptEdit );
  return ptEdit;
}
CPoint CPfxEditView::TransEditPosToViewPos( CPoint pt ) const {
  CPoint ptEdit;
  ptEdit.y = m_pdb->GetValidLine( pt.y );
  ptEdit.x = m_pdb->GetValidChar( ptEdit.y, pt.x );
  m_pdb->TextAbsLineToSubLine( ptEdit );
  CPoint ptText;
  ptText.y = TransLinePosToTextPos( ptEdit.y );
  ptText.x = TransCharPosToTextPos( ptEdit );
  CPoint ptView;
  ptView.x = TransTextPosToViewPosX( ptText.x );
  ptView.y = TransTextPosToViewPosY( ptText.y );
  return ptView;
}
CPoint CPfxEditView::TransCharOfsToEditPos( LONG nOffset ) const {
  nOffset = max( nOffset, 0 );
  nOffset = min( nOffset, GetDocSize() );
  CPoint ptEdit( 0, 0 );
  m_pdb->TextOffsetToSubLine( nOffset, ptEdit );
  m_pdb->TextSubLineToAbsLine( ptEdit );
  return ptEdit;
}
LONG CPfxEditView::TransEditPosToCharOfs( CPoint pt ) const {
  CPoint ptEdit;
  ptEdit.y = m_pdb->GetValidLine( pt.y );
  ptEdit.x = m_pdb->GetValidChar( ptEdit.y, pt.x );
  m_pdb->TextAbsLineToSubLine( ptEdit );
  return m_pdb->TextSubLineToOffset( ptEdit );
}
// 在一行的编辑中如果要填充某些空白字符，本函数计算需要填充的字符数，这是因
// 为需要考虑TAB扩展的原因。例如如果要填充10个空白字符但不一定需要10个TCHAR
// 空间，因为如果当中包含TAB字符的话可能两个TCHAR空间就足够了
LONG CPfxEditView::FillBufferWithWhitespacesCalcBufferSize( LONG nStartLogChar, LONG nSpacesToFill ) {
  if ( ( nStartLogChar < 0 ) || ( nSpacesToFill <= 0 ) ) {
    return 0;
  }
  if ( IsExpandTabToSpaceChars() ) {
    return nSpacesToFill;
  }
  for ( LONG nBufSize = 0, nTabSize = GetTabSize(); nSpacesToFill > 0; ) {
    LONG nSpaces = nTabSize - ( nStartLogChar % nTabSize );
    nBufSize += ( ( nSpacesToFill >= nSpaces ) ? 1 : nSpacesToFill );
    nStartLogChar += nSpaces;
    nSpacesToFill -= nSpaces;
  }
  return nBufSize;
}
// 使用空格或TAB填充指定的缓冲区
// LPTSTR pBuffer - 该缓冲区必须足够大，不小于FillBufferWithWhitespacesCalcBufferSize()返回的数值大小
// LONG nBufLen   - 缓冲区长度，增加这个参数只是从安全角度出发
void CPfxEditView::FillBufferWithWhitespacesDoFill( LONG nStartLogChar, LONG nSpacesToFill, LPTSTR pBuffer, LONG nBufLen ) {
  if ( ( pBuffer == NULL ) || ( nBufLen <= 0 ) || ( nSpacesToFill <= 0 ) || ( nStartLogChar < 0 ) ) {
    return ;
  }
  if ( IsExpandTabToSpaceChars() ) {
    nSpacesToFill = min( nSpacesToFill, nBufLen );
    for ( LONG i = 0; i < nSpacesToFill; i++ ) {
      pBuffer[ i ] = PFX_EC_CHR_SPACE;
    }
    return ;
  }
  // 从nStartLogChar到下一个TAB位置中包含多少个空格？如果是nTabSize个
  // 就用一个TAB代替，否则填入该数目的空格
  LPTSTR pEnd = pBuffer + nBufLen;
  for ( LONG nTabSize = GetTabSize(); ( nSpacesToFill > 0 ) && ( pBuffer < pEnd ); /*empty*/ ) {
    LONG nSpaces = nTabSize - ( nStartLogChar % nTabSize );
    if ( nSpacesToFill >= nSpaces ) {
      *pBuffer++ = PFX_EC_CHR_TAB;
    } else {
      LPTSTR s = min( ( pBuffer + nSpacesToFill ), pEnd );
      while ( pBuffer < s ) {
        *pBuffer++ = PFX_EC_CHR_SPACE;
      }
    }
    nStartLogChar += nSpaces;
    nSpacesToFill -= nSpaces;
  }
}
// 计算在某行中给定的水平范围内有多少个字符
void CPfxEditView::CalcCharsToFit( LPCTSTR& rpCharRet, LONG& rxTextPos, HDC dc, LPCTSTR pStart, LPCTSTR pEnd, LONG nTestPos, LONG nTestLen, LONG nLogChar ) const {
  LONG nLogCharSaved;
  LONG nTestPosSaved;
  CSize size;
  LONG nTabSize = size.cx = 0;
  LONG nTestLenSaved = nTestLen;
  LONG xTextPos = rxTextPos;
  LPCTSTR pCur = pStart;
  while ( pCur < pEnd ) {
    nLogCharSaved = nLogChar;
    nTestPosSaved = nTestPos;
    nTestLen = nTestLenSaved;
    do {
      if ( PFX_EC_IS_LEAD_BYTE( *pCur ) ) {
        nTestLen--;
        pCur += PFX_EC_MAX_MB_LEN;
        nLogChar += PFX_EC_MAX_MB_LEN;
        continue;
      }
      if ( *pCur != PFX_EC_CHR_TAB ) {
        nTestLen--;
        pCur++;
        nLogChar++;
      } else {
        nTabSize = GetTabSize() - ( nLogChar % GetTabSize() );
        nTestPos += ( nTabSize * PFX_EC_DATA.m_nCharWidthSpace - PFX_EC_DATA.m_nCharWidthTab );
        nTestLen -= nTabSize;
        pCur++;
        nLogChar += nTabSize;
      }
    } while ( ( pCur < pEnd ) && ( nTestLen > 0 ) );
    pCur = min( pCur, pEnd );
    ::GetTextExtentPoint32( dc, pStart, ( pCur - pStart ), &size );
    nTestPos += size.cx;
    if ( nTestPos >= xTextPos ) {
      break;
    }
    pStart = pCur;
  }
  // 如果已测得的距离大于给定的距离并且两者之差中包含多于一个字符，这
  // 种情况要丢弃上一次所测得的一段字符，并以更小的步进值重新检测，这
  // 种情况在NUM_MAX_LINE_LENGTH为千级数值时最多发生两次
  if ( ( nTestPos > xTextPos ) && ( GetNextCharPos( pStart, pEnd ) < pCur ) ) {
    nTestLen = max( 1, ( nTestLenSaved / 10 ) );
    CalcCharsToFit( rpCharRet, rxTextPos, dc, pStart, pEnd, nTestPosSaved, nTestLen, nLogCharSaved );
    return ;
  }
  if ( nTestPos > xTextPos ) {
    // 现在应该是每步前进一个字符的时候，如果这个字符是制表符，
    // 就要获取它的实际宽度
    if ( *pStart == PFX_EC_CHR_TAB ) {
      size.cx = ( nTabSize * PFX_EC_DATA.m_nCharWidthSpace );
    }
    // 如果落在字符的后半部就取下一个字符，否则取当前字符
    if ( ( nTestPos - xTextPos ) > ( size.cx / 2 ) ) {
      pCur = pStart;
      nTestPos = nTestPosSaved;
    }
  }
  rpCharRet = min( pCur, pEnd );
  rxTextPos = nTestPos;
}
// LONG CalcCharsToCopy(LPCTSTR pData, LONG cch) const
// 在某些诸如GetSelData()等操作中，用户给出了缓冲区的长度，当该缓冲区长
// 度小于将要复制的字符时便要截断字符，截断位置可能在一个MBCS字符的中间
// 位置，这种情况不应该发生，本函数执行判断看截断位置是否在MBCS字符的中
// 间，它经过计算返回正确的可复制长度，保证不会复制MBCS字符的一半
// LPCTSTR pData - 要复制字符串的首地址，一般是一行的开始地址
// LONG cch - 要复制的长度，如果发生字符被截断则返回值一般是(cch - 1)
// 返回正确的复制长度，复制这个长度的字符保证不会断开MBCS字符
LONG CPfxEditView::CalcCharsToCopy( LPCTSTR pData, LONG cch ) const {
  if ( ( pData == NULL ) || ( cch <= 0 ) ) {
    return 0;
  }
#ifdef _UNICODE
  return cch;
#else
  for ( LPCTSTR pCur = pData, pEnd = pData + cch; pCur < pEnd; ) {
    if ( !PFX_EC_IS_LEAD_BYTE( *pCur ) ) {
      pCur++;
    } else {
      pCur += PFX_EC_MAX_MB_LEN;
      if ( pCur > pEnd ) {
        pCur -= PFX_EC_MAX_MB_LEN;
        break;
      }
    }
  }
  return ( pCur - pData );
#endif
}
// 计算从指定行的行首到该行某字符之间的空格数
// CPoint pt - 最多计算到这个位置，如果在这个位置之前有非空白字符则到第
//          一个非空白字符处停止。这是相对子行位置
// 从行首到指定位置之间的空格数
LONG CPfxEditView::CalcLeadingWhitespaces( CPoint pt ) const {
  if ( !m_pdb->TextIsValidLine( pt.y ) ) {
    return 0;
  }
  m_pdb->TextSubLineToMainLine( pt );
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
  LPCTSTR pCur = pli->pData;
  const LPCTSTR pEnd = pCur + min( pt.x, pli->nActLen );
  LONG nSpaces = 0;
  LONG nTabSize = GetTabSize();
  while ( pCur < pEnd ) {
    TCHAR tc = *pCur;
    if ( PFX_EC_CHR_SPACE == tc ) {
      pCur++;
      nSpaces++;
    } else if ( PFX_EC_CHR_TAB == tc ) {
      pCur++;
      nSpaces += ( nTabSize - ( nSpaces % nTabSize ) );
    } else {
      break; // 遇到非空白字符就停止！
    }
  }
  return nSpaces;
}
LONG CPfxEditView::CalcLogicalChar( LPCTSTR pStart, LPCTSTR pEnd, LONG nTabSize, LONG nLogChar ) const {
  while ( pStart < pEnd ) {
    if ( PFX_EC_IS_LEAD_BYTE( *pStart ) ) {
      pStart += PFX_EC_MAX_MB_LEN;
      nLogChar += PFX_EC_MAX_MB_LEN;
      continue;
    }
    if ( *pStart != PFX_EC_CHR_TAB ) {
      pStart++;
      nLogChar++;
    } else {
      pStart++;
      nLogChar += ( nTabSize - ( nLogChar % nTabSize ) );
    }
  }
  return nLogChar;
}
// 计算一行中指定数目的连续字符的宽度之和
// LPCTSTR pData   - 字符串的指针
// LONG cch        - 要计算的字符数量，单位：TCHAR
// LONG& rnLogChar - 与第一个字符对应的逻辑列数，这个数值主要是用来计算制表符的宽度
// HDC hDC         - HDC 指针
// 成功返回该字符串的宽度，否则返回零字符串中可以包含有制表符
LONG CPfxEditView::CalcStringWidth( LPCTSTR pData, LONG cch, LONG& rnLogChar, HDC hDC ) const {
  CDCH pDC = hDC;
  ASSERT_VALID( this );
  if ( ( pData == NULL ) || ( cch == 0 ) ) {
    return 0;
  }
  LPCTSTR pCur = pData;
  LPCTSTR pEnd = pData + cch;
  LONG nWidth = 0;
  while ( pCur < pEnd ) {
    if ( PFX_EC_IS_LEAD_BYTE( *pCur ) ) {
      pCur += PFX_EC_MAX_MB_LEN;
      rnLogChar += PFX_EC_MAX_MB_LEN;
      continue;
    }
    if ( *pCur != PFX_EC_CHR_TAB ) {
      pCur++;
      rnLogChar++;
    } else {
      LONG nTabSize = GetTabSize();
      nTabSize = nTabSize - ( rnLogChar % nTabSize );
      nWidth += ( nTabSize * PFX_EC_DATA.m_nCharWidthSpace - PFX_EC_DATA.m_nCharWidthTab );
      pCur++;
      rnLogChar += nTabSize;
    }
  }
  if ( pCur > pEnd ) {
    rnLogChar--;
  }
  CSize size( 0, 0 );
  if ( pDC != NULL ) {
    size = pDC.GetTextExtent( pData, cch );
  } else {
    CClientDC1 dc( const_cast<CPfxEditView*>( this ) );
    HGDIOBJ hFont = dc.SelectObject( PFX_EC_DATA.m_hFontText );
    size = dc.GetTextExtent( pData, cch );
    dc.SelectObject( hFont );
  }
  return ( nWidth + size.cx );
}
// 将给定的行中的的逻辑字符位置（TAB扩展）转换成绝对字符位置（指针读写位置）
// LONG nLine          - 物理行索引
// LONG nLogCharToTest - 要转换的逻辑字符位置
LONG CPfxEditView::CalcAbsCharPosFromLogCharPos( LONG nLine, LONG nLogCharToTest ) const {
  if ( !m_pdb->IsValidLine( nLine ) ) {
    return 0;
  }
  // 将物理行转换成相对子行位置
  CPoint pt( 0, nLine );
  m_pdb->TextAbsLineToMainLine( pt );
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
  LPCTSTR pStart = pli->pData;
  LPCTSTR pCur = pStart;
  LPCTSTR pEnd = pStart + pli->nActLen;
  LONG nTabSize = GetTabSize();
  LONG nLogChar = 0;
  while ( pCur < pEnd ) {
    if ( PFX_EC_CHR_TAB != *pCur ) {
      nLogChar++;
    } else {
      nLogChar += ( nTabSize - ( nLogChar % nTabSize ) );
    }
    if ( nLogChar > nLogCharToTest ) {
      break;
    }
    pCur++;
  }
  return ( pCur - pStart );
}
// 给定一行号和列数，检查该指定的列是否在一个MBCS字符的后半部分，如果是则返回下一个字符的位置
void CPfxEditView::VerifyPointerPosition( CPoint& rpt ) const {
  ASSERT( m_pdb->TextIsValidLine( rpt.y ) );
  ASSERT( m_pdb->TextIsValidChar( rpt.y, rpt.x ) );
  // 检查行是否合法，如果不合法就纠正
  rpt.y = m_pdb->TextVerifyLine( rpt.y );
  rpt.x = max( 0, rpt.x );
  // 检查列是否合法和是否落在一MBCS字符的中间，如果错误就纠正
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( rpt.y );
  LPCTSTR pLineStart = pli->pData;
  LPCTSTR pLineEnd = pLineStart + pli->nLen;
  LPCTSTR pCur = pLineStart;
  LPCTSTR pEnd = pLineStart + min( pli->nLen, rpt.x );
  while ( pCur < pEnd ) {
    pCur += ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
  }
  rpt.x = ( min( pCur, pLineEnd ) - pLineStart );
}
// 检查当前编辑位置是否合法
void CPfxEditView::VerifyCurrentPosition() {
  // 验证行
  LONG nLineUpperBound = m_pdb->TextGetLineUpperBound();
  if ( m_ptEditPos.y < 0 ) {
    m_ptEditPos.y = 0;
  } else if ( m_ptEditPos.y > nLineUpperBound ) {
    m_ptEditPos.y = nLineUpperBound;
  }
  // 验证列
  LONG nCharUpperBound = m_pdb->TextGetLineLength( m_ptEditPos.y );
  if ( m_ptEditPos.x < 0 ) {
    m_ptEditPos.x = 0;
  } else if ( m_ptEditPos.x > nCharUpperBound ) {
    m_ptEditPos.x = nCharUpperBound;
  }
  // 首个可见行
  if ( m_ptFirstVisible.y < 0 ) {
    m_ptFirstVisible.y = 0;
  } else if ( m_ptFirstVisible.y > nLineUpperBound ) {
    m_ptFirstVisible.y = nLineUpperBound;
  }
}
// 刷新由nFirstUpdateLine到nLastUpdateLine之间的行所在的区域
// LONG nFirstUpdateLine - 要刷新的第一行（以零为基值）
// LONG nLastUpdateLine  - 要刷新的最后行
void CPfxEditView::UpdateLines( LONG nFirstUpdateLine, LONG nLastUpdateLine ) {
  if ( nFirstUpdateLine > nLastUpdateLine ) {
    LONG n = nLastUpdateLine;
    nLastUpdateLine = nFirstUpdateLine;
    nFirstUpdateLine = n;
  }
  nFirstUpdateLine = m_pdb->TextVerifyLine( nFirstUpdateLine );
  nLastUpdateLine = m_pdb->TextVerifyLine( nLastUpdateLine );
  LONG nFirstVisibleLine = m_ptFirstVisible.y;
  LONG nLastVisibleLine = m_ptFirstVisible.y + m_sizeVisible.cy;
  // 如果要更新的行都不在可见范围内就不需要更新
  if ( ( nFirstUpdateLine > nLastVisibleLine )
       || ( nLastUpdateLine < nFirstVisibleLine ) ) {
    return ;
  }
  if ( nFirstUpdateLine < nFirstVisibleLine ) {
    nFirstUpdateLine = nFirstVisibleLine;
  }
  if ( nLastUpdateLine > nLastVisibleLine ) {
    nLastUpdateLine = nLastVisibleLine;
  }
  CRect rect;
  rect.left = m_rectClient.left;
  rect.right = m_rectClient.right;
  rect.top = m_rectText.top + ( ( nFirstUpdateLine - nFirstVisibleLine ) * PFX_EC_DATA.m_nLineHeight );
  rect.bottom = rect.top + ( ( nLastUpdateLine - nFirstUpdateLine + 1 ) * PFX_EC_DATA.m_nLineHeight );
  InvalidateRect( &rect, false );
}
// 执行选择操作以后（例如SetSelRange()里面调用本函数），调用本函数刷新改
// 变的地方，但又不需要全不刷新
// LONG nOldSelLineFrom - 在新的选择操作之前的选择范围的第一行
// LONG nOldSelLineTo   - 在新的选择操作之前的选择范围的最后行
// 如果nOldSelLineFrom等于PFX_EC_INVALID_SELECT表示操作之前没有选择文本
// 如果nNewSelLineFrom等于PFX_EC_INVALID_SELECT表示操作之后没有选择文本
void CPfxEditView::UpdateLinesSelected( LONG nOldSelLineFrom, LONG nOldSelLineTo ) {
  LONG nFirstUpdateLine;
  LONG nLastUpdateLine;
  LONG nNewSelLineFrom = m_nSelLineFrom;
  LONG nNewSelLineTo = m_nSelLineTo;
  if ( ( nOldSelLineFrom == PFX_EC_INVALID_SELECT )
       || ( nNewSelLineFrom == PFX_EC_INVALID_SELECT ) ) {
    // 如果操作之前没有选择，操作之后也没有选择
    if ( ( nOldSelLineFrom == PFX_EC_INVALID_SELECT )
         && ( nNewSelLineFrom == PFX_EC_INVALID_SELECT ) ) {
      return ;
    }
    // 操作之前没有选择文本，只需更新操作之后的范围
    else if ( nOldSelLineFrom == PFX_EC_INVALID_SELECT ) {
      nFirstUpdateLine = nNewSelLineFrom;
      nLastUpdateLine = nNewSelLineTo;
    }
    // 操作之后没有选择文本，只需更新操作之前的范围
    else // (nNewSelLineFrom == PFX_EC_INVALID_SELECT)
    {
      nFirstUpdateLine = nOldSelLineFrom;
      nLastUpdateLine = nOldSelLineTo;
    }
  } else {
    // 操作是向下增加或减少选择范围，只需更新改变了的行
    if ( ( nOldSelLineFrom == nNewSelLineFrom )
         && ( nNewSelLineTo == m_ptEditPos.y ) ) {
      nFirstUpdateLine = min( nOldSelLineTo, nNewSelLineTo );
      nLastUpdateLine = max( nOldSelLineTo, nNewSelLineTo );
    }
    // 操作是向上增加或减少选择范围，只需更新改变了的行
    else if ( ( nOldSelLineTo == nNewSelLineTo )
              && ( nNewSelLineFrom == m_ptEditPos.y ) ) {
      nFirstUpdateLine = min( nOldSelLineFrom, nNewSelLineFrom );
      nLastUpdateLine = max( nOldSelLineFrom, nNewSelLineFrom );
    } else {
      nFirstUpdateLine = min( nOldSelLineFrom, nNewSelLineFrom );
      nLastUpdateLine = max( nOldSelLineTo, nNewSelLineTo );
    }
    nFirstUpdateLine -= ( ( ( nFirstUpdateLine - 1 ) == m_nSelLineFrom ) ? 1 : 0 );
    nLastUpdateLine += ( ( ( nLastUpdateLine + 1 ) == m_nSelLineTo ) ? 1 : 0 );
  }
  UpdateLines( nFirstUpdateLine, nLastUpdateLine );
}
// 计算标尺、行号、正文等所在的区域
void CPfxEditView::UpdateLayout() {
  // Layout
  // |=========================================== |
  // |   ruler(or indent if not display ruler)    |
  // | -------|---|-------------------------------|
  // |        | i |                               |
  // |        | n |                               |
  // | line   | d |                               |
  // | number | e |            text               |
  // |        | n |                               |
  // |        | t |                               |
  // |========|===|===============================|
  LONG nTextIndentLeft;
  LONG nTextIndentTop;
  LONG nLineNumberWidth;
  CRect rect;
  rect.CopyRect( &m_rectClient );
  // 当不显示标尺时，rectRuler所占据的区域其实是一空白区，也即是第一
  // 行文字到视图上边之间的空白区，这是为了不至于使第一行过分紧靠视图
  // 上边而空出来的区域
  if ( m_pdb->m_bDrawRuler ) {
    nTextIndentTop = PFX_EC_DATA.m_nRulerHeight;
    nTextIndentLeft = max( PFX_EC_MIN_TEXT_INDENT_LEFT, ( PFX_EC_DATA.m_nRulerCharWidthAve / 2 ) );
  } else {
    nTextIndentTop = PFX_EC_MIN_TEXT_INDENT_TOP;
    nTextIndentLeft = PFX_EC_MIN_TEXT_INDENT_LEFT;
  }
  m_rectRuler.SetRect( rect.left, rect.top, rect.right, nTextIndentTop );
  // 当不显示行号时，rectLineNumber所占据的区域其实是一空白区，这是为
  // 了不至于使视图的左边过分紧靠文本区的第一个字符而设置
  // 计算行号数字有多少位，行号宽度等于数字位数乘以最大数字字符宽度
  if ( m_pdb->m_bDrawLineNumber ) {
    nTextIndentLeft = max( nTextIndentLeft, PFX_EC_DATA.m_nSpacingLineNumberAndText );
    for ( LONG i = GetLineCount(), nLineNumberChars = 1; i >= 10; nLineNumberChars++ ) {
      i /= 10;
    }
    // 加5是为了使第一个行号数字与窗口左边框有一定间隔
    i = ( nLineNumberChars * PFX_EC_DATA.m_nCharWidthMaxNum + ( PFX_EC_LINE_NUMBER_INDENT_LEFT + PFX_EC_LINE_NUMBER_INDENT_RIGHT ) );
    nLineNumberWidth = max( i, PFX_EC_LINE_NUMBER_MIN_WIDTH );
  } else {
    nLineNumberWidth = 0;
  }
  m_rectLineNumber.SetRect( rect.left, m_rectRuler.bottom, nLineNumberWidth + nTextIndentLeft, rect.bottom );
  // 文本区
  m_rectText.SetRect( m_rectLineNumber.right, m_rectLineNumber.top, rect.right, rect.bottom );
  // 计算最大可显示行数，最后一行如果不能完全可见则该行不算入
  m_sizeVisible.cy = m_rectText.Height() / PFX_EC_DATA.m_nLineHeight;
  m_pdb->m_rectIndent.top = nTextIndentTop;
  m_pdb->m_rectIndent.left = nTextIndentLeft;
}
void CPfxEditView::UpdateCursor( CPoint pt ) {
  HCURSOR hCursor;
  if ( IsTracking() ) {
    if ( !IsColumnSelectTracking() || !IsCanColumnSelect() ) {
      hCursor = PFX_EC_DATA.m_hCurText;
    } else {
      hCursor = PFX_EC_DATA.m_hCurColSelect;
    }
  } else {
    switch ( GetHitTestCode( pt ) ) {
    case PFX_EC_HT_RULER:
    case PFX_EC_HT_LINE_NUMBER:
      hCursor = PFX_EC_DATA.m_hCurStandard;
      break;
    case PFX_EC_HT_TEXT:
    default:
      // 如果ALT 键按下且当前不是自动换行模式，则将光标设
      // 置成列块选择跟踪模式
      if ( ( IsColumnSelectTracking() || PFX_EC_IS_KEY_DOWN( VK_MENU ) )
           && IsCanColumnSelect() ) {
        hCursor = PFX_EC_DATA.m_hCurColSelect;
      } else {
        hCursor = PFX_EC_DATA.m_hCurText;
      }
      break;
    }
  }
  SetCursor( hCursor );
}
// 更新插入符位置，重新计算当前逻辑字符位置
// x 是逻辑坐标，即是从文本区左边开始计算，而不是从窗口左边开始计算
void CPfxEditView::UpdateCurrentPosition( LONG xTextPos, BOOL bUpdateCurCharPosition, BOOL bUpdateCaretReferPoint ) {
  ASSERT( xTextPos >= 0 );
  m_ptTextPos.x = xTextPos;
  m_ptTextPos.y = TransLinePosToTextPos( m_ptEditPos.y );
  if ( bUpdateCaretReferPoint ) {
    m_ptCaretRefer.y = 0; // unused
    m_ptCaretRefer.x = xTextPos;
  }
  if ( bUpdateCurCharPosition ) {
    const CPfxEditLineInfo * pli = m_pdb->TextGetLineAt( m_ptEditPos.y );
    m_ptLogiPos.y = 0; // unused
    m_ptLogiPos.x = CalcLogicalChar( pli->pData, pli->pData + m_ptEditPos.x, GetTabSize(), pli->nFirstChar );
  }
  // 如果使用OLE 拖放操作从其他窗口拖动文本到本窗口，那时候本窗口应该
  // 不是活动窗口，所以不应该显示插入符
  if ( IsFocused() ) {
    CaretSetPos( m_ptTextPos );
  }
}
void CPfxEditView::ScrollBarUpdate( LONG nBar ) {
  ASSERT( ( nBar == SB_HORZ ) || ( nBar == SB_VERT ) );
  // note:
  // nPage must from 0 to nMax - nMin + 1
  // nPos must between min and nMax - max(nPage – 1, 0)
  SCROLLINFO si;
  si.cbSize = sizeof( si );
  si.fMask = SIF_PAGE | SIF_RANGE;
  si.nMin = 0;
  if ( nBar == SB_VERT ) {
    si.nMax = m_pdb->TextGetLineCount();
    si.nPage = m_sizeVisible.cy - 1;
  } else {
    si.nMax = PFX_EC_MAX_LINE_LENGTH;
    si.nPage = m_rectText.Width() / PFX_EC_DATA.m_nCharWidthAve;
#ifdef _SYNC_SCROLLING
    EnableScrollBar( SB_HORZ, ( !IsWrapAtWindowEdge() ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH ) );
#else
    ShowScrollBar( SB_HORZ, !IsWrapAtWindowEdge() );
#endif
  }
  if ( si.nPage < 0 ) {
    si.nPage = 0;
  } else if ( ( LONG ) si.nPage > ( si.nMax - si.nMin + 1 ) ) {
    si.nPage = ( UINT ) si.nMax - si.nMin + 1;
  }
  SetScrollInfo( nBar, &si, true );
}
void CPfxEditView::ScrollBarSetPos( LONG nBar, LONG nPos ) {
  SCROLLINFO si;
  si.cbSize = sizeof( si );
  si.fMask = SIF_POS;
  si.nPos = nPos;
  SetScrollInfo( nBar, &si, true );
}
void CPfxEditView::UpdateAllViews( DWORD dwFlags, LONG nFirstUpdateLine, LONG nLastUpdateLine ) {
  CPfxEditView * pSiblingView = GetSiblingView();
  switch ( dwFlags ) {
  case PFX_EC_UT_HORZ:
  case PFX_EC_UT_VERT:
    // 更新已经分析过的行的范围
    m_pdb->m_nLineLastParsedButCanceled = m_pdb->m_nLineLastParsed;
    if ( m_pdb->m_nLineLastParsed >= nFirstUpdateLine ) {
      m_pdb->m_nLineLastParsed = nFirstUpdateLine - 1;
    }
    // 更新视图自身
    UpdateThisView( this, dwFlags, nFirstUpdateLine, nLastUpdateLine );
    // 更新兄弟视图
    if ( pSiblingView != NULL ) {
      UpdateThisView( pSiblingView, dwFlags, nFirstUpdateLine, nLastUpdateLine );
    }
    break;
  case PFX_EC_UT_LINES:           //      没有改变数据，仅仅是需要刷新指定的行
    if ( nLastUpdateLine == -1 ) {
      nLastUpdateLine = m_pdb->TextGetLineUpperBound();
    }
    nFirstUpdateLine = min( nFirstUpdateLine, nLastUpdateLine );
    nLastUpdateLine = max( nFirstUpdateLine, nLastUpdateLine );
    // 更新视图自身
    UpdateLines( nFirstUpdateLine, nLastUpdateLine );
    // 更新兄弟视图
    if ( pSiblingView != NULL ) {
      pSiblingView->UpdateLines( nFirstUpdateLine, nLastUpdateLine );
    }
    break;
  case PFX_EC_UT_LAYOUT:           //      没有改变文本内容，不需要重新分析COOKIE
    // 更新视图自身
    {
      UpdateLayout();
      UpdateCurrentPosition( m_ptTextPos.x, false, false );
      ScrollBarUpdate( SB_VERT );
      ScrollBarSetPos( SB_VERT, m_ptFirstVisible.y );
      ScrollBarUpdate( SB_HORZ );
      ScrollBarSetPos( SB_HORZ, m_ptFirstVisible.x );
      Invalidate( false );
    }
    // 更新兄弟视图
    if ( pSiblingView != NULL ) {
      pSiblingView->VerifyCurrentPosition();
      pSiblingView->UpdateLayout();
      pSiblingView->UpdateCurrentPosition( pSiblingView->m_ptTextPos.x, false, false );
      pSiblingView->ScrollBarUpdate( SB_VERT );
      pSiblingView->ScrollBarSetPos( SB_VERT, pSiblingView->m_ptFirstVisible.y );
      pSiblingView->ScrollBarUpdate( SB_HORZ );
      pSiblingView->ScrollBarSetPos( SB_HORZ, pSiblingView->m_ptFirstVisible.x );
      pSiblingView->Invalidate( false );
    }
    break;
  case PFX_EC_UT_ALL:           //      更改了文本内容，需要全面刷新并重新分析COOKIE
  default:
    m_pdb->m_nLineLastParsed = -1;
    m_pdb->m_nLineLastParsedButCanceled = -1;
    // 更新自己
    {
      UpdateLayout();
      UpdateCurrentPosition( m_ptTextPos.x, false, false );
      ScrollBarUpdate( SB_VERT );
      ScrollBarSetPos( SB_VERT, m_ptFirstVisible.y );
      ScrollBarUpdate( SB_HORZ );
      ScrollBarSetPos( SB_HORZ, m_ptFirstVisible.x );
      Invalidate( false );
    }
    if ( pSiblingView != NULL ) {
      pSiblingView->UpdateLayout();
      pSiblingView->VerifyCurrentPosition();
      pSiblingView->UpdateCurrentPosition( pSiblingView->m_ptTextPos.x, false, false );
      pSiblingView->ScrollBarUpdate( SB_VERT );
      pSiblingView->ScrollBarSetPos( SB_VERT, pSiblingView->m_ptFirstVisible.y );
      pSiblingView->ScrollBarUpdate( SB_HORZ );
      pSiblingView->ScrollBarSetPos( SB_HORZ, pSiblingView->m_ptFirstVisible.x );
      pSiblingView->Invalidate( false );
    }
    break;
  }
}
void CPfxEditView::UpdateThisView( CPfxEditView* pView, DWORD dwFlags, LONG nFirstUpdateLine, LONG nLastUpdateLine ) {
  LONG nLineNumberWidth = pView->m_rectLineNumber.right;
  if ( this == pView ) {
    m_ptFirstVisible.y = min( m_ptFirstVisible.y, m_pdb->TextGetLineUpperBound() );
  } else {
    pView->VerifyCurrentPosition();
  }
  if ( dwFlags == PFX_EC_UT_VERT ) {
    pView->UpdateLayout();
    pView->ScrollBarUpdate( SB_VERT );
    pView->ScrollBarSetPos( SB_VERT, pView->m_ptFirstVisible.y );
  } else {
    // 目前不支持更新水平滚动条
  }
  // 如果nLastUpdateLine == PFX_EC_INVALID_LINE就表示从nFirstUpdateLine更
  // 新到文档的最后一行
  if ( nLastUpdateLine == -1 ) {
    // 行号区的宽度改变则一定要全部刷新，因为标尺区一定要刷新
    if ( nLineNumberWidth != pView->m_rectLineNumber.right ) {
      pView->Invalidate( false );
    } else {
      CRect rectUpdate( &pView->m_rectClient );
      rectUpdate.top = pView->m_rectText.top + ( ( nFirstUpdateLine - pView->m_ptFirstVisible.y ) * PFX_EC_DATA.m_nLineHeight );
      pView->InvalidateRect( &rectUpdate, false );
    }
  } else {
    // 行号区的宽度改变则一定要全部刷新，因为标尺区一定要刷新
    if ( nLineNumberWidth != pView->m_rectLineNumber.right ) {
      pView->Invalidate( false );
    } else {
      pView->UpdateLines( nFirstUpdateLine, nLastUpdateLine );
    }
  }
}
void CPfxEditView::CaretRebuild() {
  CreateSolidCaret( PFX_EC_DATA.m_sizeCaret.cx, PFX_EC_DATA.m_sizeCaret.cy );
  ShowCaret();
}
void CPfxEditView::CaretDestroy() {
  HideCaret();
  DestroyCaret();
}
// CPoint pt - 以文本坐标为参照的光标位置
void CPfxEditView::CaretSetPos( CPoint pt ) {
  pt.x = TransTextPosToViewPosX( pt.x );
  pt.y = TransTextPosToViewPosY( pt.y ) + GetLineTextVertOffset();
  if ( !m_rectText.PtInRect( pt ) ) {
    pt.x = -PFX_EC_DATA.m_sizeCaret.cx;
    pt.y = -PFX_EC_DATA.m_sizeCaret.cy;
  }
  SetCaretPos( pt );
}
LONG CPfxEditView::GetHitTestCode( CPoint pt ) const {
  if ( ::PtInRect( &m_rectText, pt ) ) {
    return PFX_EC_HT_TEXT;
  } else if ( ::PtInRect( &m_rectLineNumber, pt ) ) {
    return PFX_EC_HT_LINE_NUMBER;
  } else if ( ::PtInRect( &m_rectRuler, pt ) ) {
    return PFX_EC_HT_RULER;
  } else {
    return PFX_EC_HT_ERROR;
  }
}
CPfxEditView* CPfxEditView::GetSiblingView() const {
  CPfxEditView * pView = m_pdb->m_arrViews[ 0 ];
  if ( this != pView ) {
    return pView;
  } else {
    return m_pdb->m_arrViews[ 1 ];
  }
}
LONG CPfxEditView::ScrollVert( UINT nSBCode ) {
#if 0
  LONG nFirstVisibleLine;
  SCROLLINFO si;
  switch ( nSBCode ) {
  case SB_TOP:
    nFirstVisibleLine = 0;
    break;
  case SB_BOTTOM:
    nFirstVisibleLine = m_pdb->TextGetLineUpperBound();
    break;
  case SB_LINEUP:
    nFirstVisibleLine = m_ptFirstVisible.y - 1;
    break;
  case SB_LINEDOWN:
    nFirstVisibleLine = m_ptFirstVisible.y + 1;
    break;
  case SB_PAGEUP:
    GetScrollInfo( SB_VERT, &si, SIF_PAGE );
    nFirstVisibleLine = m_ptFirstVisible.y - si.nPage;
    break;
  case SB_PAGEDOWN:
    GetScrollInfo( SB_VERT, &si, SIF_PAGE );
    nFirstVisibleLine = m_ptFirstVisible.y + si.nPage;
    break;
  case SB_THUMBPOSITION:
  case SB_THUMBTRACK:
    GetScrollInfo( SB_VERT, &si, SIF_TRACKPOS );
    nFirstVisibleLine = si.nTrackPos;
    break;
  default:
    nFirstVisibleLine = m_ptFirstVisible.y;
    break;
  }
  nFirstVisibleLine = m_pdb->TextVerifyLine( nFirstVisibleLine );
  if ( m_ptFirstVisible.y != nFirstVisibleLine ) {
    CRect rectUpdate;
    ::SetRect( &rectUpdate, m_rectClient.left, m_rectRuler.bottom, m_rectClient.right, m_rectText.bottom );
    ::ScrollWindowEx( m_hWnd, 0, ( ( m_ptFirstVisible.y - nFirstVisibleLine ) * PFX_EC_DATA.m_nLineHeight ), &rectUpdate, &rectUpdate, NULL, NULL, SW_INVALIDATE );
    ScrollBarSetPos( SB_VERT, nFirstVisibleLine );
  }
  return nFirstVisibleLine;
#endif
  return 0;
}
LONG CPfxEditView::ScrollHorz( UINT nSBCode ) {
#if 0
  LONG nFirstVisibleHCol;
  SCROLLINFO si;
  switch ( nSBCode ) {
  case SB_LEFT:
    nFirstVisibleHCol = 0;
    break;
  case SB_LINELEFT:
    nFirstVisibleHCol = m_ptFirstVisible.x - 1;
    break;
  case SB_LINERIGHT:
    nFirstVisibleHCol = m_ptFirstVisible.x + 1;
    break;
  case SB_PAGELEFT:
    GetScrollInfo( SB_HORZ, &si, SIF_PAGE );
    nFirstVisibleHCol = m_ptFirstVisible.x - si.nPage;
    break;
  case SB_PAGERIGHT:
    GetScrollInfo( SB_HORZ, &si, SIF_PAGE );
    nFirstVisibleHCol = m_ptFirstVisible.x + si.nPage;
    break;
  case SB_THUMBPOSITION:
  case SB_THUMBTRACK:
    GetScrollInfo( SB_HORZ, &si, SIF_TRACKPOS );
    nFirstVisibleHCol = si.nTrackPos;
    break;
  case SB_RIGHT:
  default:
    nFirstVisibleHCol = m_ptFirstVisible.x;
    break;
  }
  nFirstVisibleHCol = max( 0, nFirstVisibleHCol );
  if ( m_ptFirstVisible.x != nFirstVisibleHCol ) {
    CRect rectUpdate;
    ::SetRect( &rectUpdate, m_rectText.left, m_rectRuler.top, m_rectText.right, m_rectText.bottom );
    ::ScrollWindowEx( m_hWnd, ( ( m_ptFirstVisible.x - nFirstVisibleHCol ) * PFX_EC_DATA.m_nCharWidthAve ), 0, &rectUpdate, &rectUpdate, NULL, NULL, SW_INVALIDATE );
    ScrollBarSetPos( SB_HORZ, nFirstVisibleHCol );
  }
  return nFirstVisibleHCol;
#endif
  return 0;
}
// 正常粘贴
// CPoint pt - 要粘贴的位置（相对位置）
void CPfxEditView::PasteNormal( CPoint pt, LPCTSTR pData, LONG cch ) {
  ASSERT( m_pdb->TextIsValidLine( pt.y ) );
  ASSERT( m_pdb->TextIsValidChar( pt.y, pt.x ) );
  pt.y = m_pdb->TextVerifyLine( pt.y );
  pt.x = m_pdb->TextVerifyChar( pt.y, pt.x );
  CharsInsert( pt, pData, cch, true );
}
// 对齐式列块粘贴
// CPoint pt - 要粘贴的位置（相对位置）
void CPfxEditView::PasteColumnAlign( CPoint pt, LPCTSTR pData, LONG cch ) {
  ASSERT( !IsWrapping() );
  ASSERT( m_pdb->TextIsValidLine( pt.y ) );
  ASSERT( m_pdb->TextIsValidChar( pt.y, pt.x ) );
  if ( ( pData == NULL ) || ( cch <= 0 ) ) {
    return ;
  }
  pt.y = m_pdb->TextVerifyLine( pt.y );
  pt.x = m_pdb->TextVerifyChar( pt.y, pt.x );
  LPCTSTR pCur = pData;
  LPCTSTR pEnd = pData + cch;
  LPCTSTR pLineBreakData = m_pdb->GetLineBreakDataFromType( PFX_EC_LINE_BREAK_TYPE_DOS );
  // 按列模式来粘贴，粘贴时在每行的插入点由文本坐标来决定而不是左边列
  // 数来决定，这样做在一些非等距字体下也可以做到整齐排列。现在需要计
  // 算初始插入点的水平文本坐标，并以此坐标来计算出其它行的插入点
  const LONG nAlignPos = TransCharPosToTextPos( pt );
  CPfxEditHeap Heap;
  for ( LONG nCurLine = pt.y; pCur < pEnd; nCurLine++ ) {
    LPCTSTR pLineData = pCur;
    LPCTSTR pLineBreak = pEnd;
    while ( pCur < pEnd ) {
      if ( ( PFX_EC_CHR_CR == *pCur++ )
           && ( pCur < pEnd )
           && ( PFX_EC_CHR_LF == *pCur ) ) {
        pLineBreak = pCur - 1;
        //  使指针指向下一行的字符
        pCur++;
        break;
      }
    }
    // 要插入数据的长度
    LONG cchLine = pLineBreak - pLineData;
    // 如果当前要操作的行的行末文本坐标小于要测试的文本坐标或已
    // 经超出原来文档的最后一行，则要人为地补上一定数量的空白字
    // 符以达到列对齐的效果
    LONG xTextPos;
    BOOL bAppendLine;
    if ( nCurLine >= m_pdb->TextGetLineCount() ) {
      bAppendLine = true;
      xTextPos = 0;
      pt.y = m_pdb->TextGetLineUpperBound();
      pt.x = m_pdb->TextGetLineLength( pt.y );
    } else {
      bAppendLine = false;
      xTextPos = nAlignPos;
      pt.y = nCurLine;
      pt.x = TransTextPosToCharPos( pt.y, xTextPos );
    }
    // 如果需要就尽可能地补上一些空白字符
    LONG nSpacesToFill = ( max( 0, ( nAlignPos - xTextPos ) ) / PFX_EC_DATA.m_nCharWidthSpace );
    LONG nBufSizeExtra = nSpacesToFill;
    LONG nStartLogChar = 0;
    if ( !IsExpandTabToSpaceChars() ) {
      if ( !bAppendLine ) {
        const CPfxEditLineInfo * pli = m_pdb->TextGetLineAt( pt.y );
        nStartLogChar = CalcLogicalChar( pli->pData, ( pli->pData + pt.x ), GetTabSize(), pli->nFirstChar );
      }
      nBufSizeExtra = FillBufferWithWhitespacesCalcBufferSize( nStartLogChar, nSpacesToFill );
    }
    // 如果需要插入新行或补上空白字符就构造新的缓冲区，把需要插
    // 入的字符设置好然后一次过插入，这样可以令到UNDO更高效和缓
    // 冲区的使用效率更好，因为缓冲区管理器不需要频繁地做出调整
    if ( ( nBufSizeExtra > 0 ) || bAppendLine ) {
      LONG nBufLen;
      LPTSTR pBuffer;
      LPTSTR pInsert;
      // 计算缓冲区的长度
      if ( !bAppendLine ) {
        nBufLen = ( cchLine + nBufSizeExtra );
        pBuffer = static_cast<LPTSTR>( Heap.ReAlloc( nBufLen * sizeof( TCHAR ) ) );
        pInsert = pBuffer;
      } else {
        nBufLen = ( cchLine + nBufSizeExtra + PFX_EC_LINE_BREAK_SIZE_DOS );
        pBuffer = static_cast<LPTSTR>( Heap.ReAlloc( nBufLen * sizeof( TCHAR ) ) );
        pInsert = pBuffer + PFX_EC_LINE_BREAK_SIZE_DOS;
        memcpy( pBuffer, pLineBreakData, PFX_EC_LINE_BREAK_SIZE_DOS * sizeof( TCHAR ) );
      }
      if ( nBufSizeExtra > 0 )           //      填充用于对齐的空白字符
      {
        FillBufferWithWhitespacesDoFill( nStartLogChar, nSpacesToFill, pInsert, nBufSizeExtra );
        pInsert += nBufSizeExtra;
      }
      // 复制真正的行数据
      memcpy( pInsert, pLineData, ( cchLine * sizeof( TCHAR ) ) );
      cchLine = nBufLen;
      pLineData = pBuffer;
    }
    CharsInsert( pt, pLineData, cchLine, false );
    nCurLine = pt.y;
    // 如果是OLE拖放操作，那么“放”后仍然要试图恢复之前的选择，
    // 但如果列块中有TAB字符则可能会使列块的宽度发生变化，所以
    // 现在要“放”下一行便重新计算插入部分的右边的坐标，如果新
    // 的坐标比原来的列块矩形右边的数值大，就取较大的数值
    if ( IsDragging() ) {
      LONG nColSelWidth = TransCharPosToTextPos( pt ) - nAlignPos;
      if ( nColSelWidth > m_rectColSelect.Width() ) {
        m_rectColSelect.right = m_rectColSelect.left + nColSelWidth;
      }
    }
  }
  if ( pt != m_ptEditPos ) {
    m_ptEditPos = pt;
    UpdateAllViews( PFX_EC_UT_LAYOUT, 0, 0 );
    EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  }
}
// void PasteColumnAppend(CPoint pt, LPCTSTR pData, LONG cch)
// 在行末附加式列块粘贴
void CPfxEditView::PasteColumnAppend( CPoint pt, LPCTSTR pData, LONG cch ) {
  ASSERT( IsCanColumnPaste() );
  ASSERT( m_pdb->TextIsValidLine( pt.y ) );
  ASSERT( m_pdb->TextIsValidChar( pt.y, pt.x ) );
  // 按列块模式来粘贴数据，在每行的最后附加数据
  if ( ( pData == NULL ) || ( cch <= 0 ) ) {
    return ;
  }
  pt.y = m_pdb->TextVerifyLine( pt.y );
  pt.x = m_pdb->TextVerifyChar( pt.y, pt.x );
  LPCTSTR pCur = pData;
  LPCTSTR pEnd = pData + cch;
  LPCTSTR pLineBreakData = m_pdb->GetLineBreakDataFromType( PFX_EC_LINE_BREAK_TYPE_DOS );
  CPfxEditHeap Heap;
  for ( LONG nCurLine = pt.y; pCur < pEnd; nCurLine++ ) {
    LPCTSTR pLineData = pCur;
    LPCTSTR pLineBreak = pEnd;
    while ( pCur < pEnd ) {
      if ( ( PFX_EC_CHR_CR == *pCur++ )
           && ( pCur < pEnd )
           && ( PFX_EC_CHR_LF == *pCur ) ) {
        pLineBreak = pCur - 1;
        //  使指针指向下一行的字符
        pCur++;
        break;
      }
    }
    // 要插入数据的长度
    LONG cchLine = pLineBreak - pLineData;
    LONG nBufLen;
    LPTSTR pBuffer;
    LPTSTR pInsert;
    // 需要加入新行
    if ( nCurLine >= m_pdb->TextGetLineCount() ) {
      nBufLen = cchLine + PFX_EC_LINE_BREAK_SIZE_DOS;
      pBuffer = static_cast<LPTSTR>( Heap.Alloc( nBufLen * sizeof( TCHAR ) ) );
      pInsert = pBuffer + PFX_EC_LINE_BREAK_SIZE_DOS;
      memcpy( pBuffer, pLineBreakData, PFX_EC_LINE_BREAK_SIZE_DOS * sizeof( TCHAR ) );
      pt.y = m_pdb->TextGetLineUpperBound();
      pt.x = 0;
    } else {
      // 如果要插入本行的数据长度不为零就用一空格将原行数
      // 据和新附加数据隔开，否则不添加这个空格
      if ( cchLine > 0 ) {
        nBufLen = cchLine + 1;
        pBuffer = static_cast<LPTSTR>( Heap.ReAlloc( nBufLen * sizeof( TCHAR ) ) );
        pInsert = pBuffer;
        *pInsert++ = PFX_EC_CHR_SPACE;
      } else {
        nBufLen = cchLine;
        pBuffer = static_cast<LPTSTR>( Heap.ReAlloc( nBufLen * sizeof( TCHAR ) ) );
        pInsert = pBuffer;
      }
      pt.y = nCurLine;
      pt.x = m_pdb->TextGetLineLength( pt.y );
    }
    memcpy( pInsert, pLineData, cchLine * sizeof( TCHAR ) );
    CharsInsert( pt, pBuffer, nBufLen, false );
    nCurLine = pt.y;
  }
  if ( pt != m_ptEditPos ) {
    m_ptEditPos = pt;
    UpdateAllViews( PFX_EC_UT_LAYOUT, 0, 0 );
    EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  }
}
HGLOBAL CPfxEditView::DragDropRender( BOOL bDragDrop, BOOL bAppend ) {
  if ( !IsTextSelected() ) {
    return NULL;
  }
  // + 1 = 结尾的NULL字符
  LONG nSelSize = GetSelSize( PFX_EC_LINE_BREAK_TYPE_DOS, true ) + 1;
  if ( nSelSize <= 1 ) {
    return NULL;
  }
  CPfxEditViewClipboard clipboard( this, CF_TEXT, false );
  LPTSTR pBuffer = clipboard.GetBuffer( nSelSize, bAppend );
  if ( pBuffer == NULL ) {
    return NULL;
  }
  GetSelData( PFX_EC_LINE_BREAK_TYPE_DOS, pBuffer, nSelSize );
  HGLOBAL hGlobal = clipboard.Detach();
  // 如果这次复制是在列块选择模式下进行的就标识本次操作，这样
  // 可以让粘贴的时候实现列块功能
  if ( bDragDrop ) {
    if ( IsColumnSelect() ) {
      PFX_EC_DATA.m_hDataDragDropCopied = hGlobal;
    } else {
      PFX_EC_DATA.m_hDataDragDropCopied = NULL;
    }
  } else {
    if ( IsColumnSelect() ) {
      ::GlobalFlags( hGlobal );
      PFX_EC_DATA.m_nDataClipboardCopied = ::GlobalSize( hGlobal );
      PFX_EC_DATA.m_hDataClipboardCopied = hGlobal;
    } else {
      PFX_EC_DATA.m_nDataClipboardCopied = 0;
      PFX_EC_DATA.m_hDataClipboardCopied = NULL;
    }
  }
  return hGlobal;
}
// BOOL DragDropPaste(HGLOBAL hGlobal, CPoint pt)
// CPoint pt - 操作的子行相对位置
BOOL CPfxEditView::DragDropPaste( HGLOBAL hGlobal, CPoint pt ) {
  ASSERT( !IsReadOnly() ); // 应该在外层检测
  if ( hGlobal == NULL ) {
    return false;
  }
  if ( IsTextSelected() ) {
    SelNone();
  }
  LPCTSTR pData = static_cast<LPCTSTR>( ::GlobalLock( hGlobal ) );
  if ( pData == NULL ) {
    return false;
  }
  CPoint ptOld;
  ptOld.y = pt.y = m_pdb->TextVerifyLine( pt.y );
  ptOld.x = pt.x = m_pdb->TextVerifyChar( pt.y, pt.x );
  if ( IsWrapping() || !IsDataColumnModeRendered( true, hGlobal ) ) {
    // 如果数据源提供者是自己，则DROP后还要将新插入的数
    // 据选择加亮
    if ( IsDragging() ) {
      m_pdb->TextSubLineToAbsLine( ptOld );
    }
    PasteNormal( pt, pData, _tcslen( pData ) );
    if ( IsDragging() ) {
      m_pdb->TextAbsLineToSubLine( ptOld );
      SelRange( ptOld );
    }
  } else {
    // 如果数据源提供者是自己，则DROP后还要将新插入的数
    // 据选择加亮
    if ( IsDragging() ) {
      m_pdb->TextSubLineToAbsLine( ptOld );
    }
    PasteColumnAlign( pt, pData, _tcslen( pData ) );
    if ( IsDragging() ) {
      m_pdb->TextAbsLineToSubLine( ptOld );
      CRect rect;
      rect.left = TransCharPosToTextPos( ptOld );
      rect.right = rect.left + m_rectColSelect.Width();
      rect.top = TransLinePosToTextPos( ptOld.y );
      rect.bottom = TransLinePosToTextPos( m_ptEditPos.y );
      SelRange( rect );
    }
  }
  ::GlobalUnlock( hGlobal );
  return true;
}
// void DragDropRemove(CPoint& rpt)
// 在OLE拖放操作中用户在某个地方选择了一部分文本进行拖放，并将选择 内容
// 放编辑窗口的某个位置，现在要先删除拖放起点的选择内容，并反回正确的插
// 入点
// CPoint& rpt - DROP操作将要在这点（子行相对位置）插入数据
// 返回删除选择部分后的DROP点位置
void CPfxEditView::DragDropRemove( CPoint& rpt ) {
  ASSERT( IsTextSelected() );
  ASSERT( m_pdb->TextIsValidChar( rpt.y, rpt.x ) );
  // 数据“drop”在了别的窗口
  if ( !IsDragging() ) {
    SelNoneAndRemoveSelected();
    rpt = m_ptEditPos;
    return ;
  }
  // 拖放操作在自己范围内进行
  // 如果插入点在选择范围之前则只要记住插入位置就行了
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( rpt.y );
  if ( ( ( rpt.y == m_nSelLineFrom )
         && ( rpt.x <= pli->nSelFrom ) )
       || ( rpt.y < m_nSelLineFrom ) ) {
    m_pdb->TextSubLineToAbsLine( rpt );
    SelNoneAndRemoveSelected();
    m_pdb->TextAbsLineToSubLine( rpt );
    return ;
  }
  // 列块删除操作不会删除行只会删除列，而且也不会在自动换行模式下进行，
  // 所以行坐标是不会改变的
  if ( IsColumnSelect() ) {
    ASSERT( !IsWrapping() );
    if ( ( rpt.y >= m_nSelLineFrom )
         && ( rpt.y <= m_nSelLineTo )
         && ( rpt.x > pli->nSelFrom ) ) {
      rpt.x = max( rpt.x, pli->nSelTo );
      rpt.x -= pli->nSelTo;
      rpt.x += pli->nSelFrom;
    }
    SelNoneAndRemoveSelected();
  }
  // 非列快选择不可能在选择范围插入，所以插入点必定在选择范围之后
  else {
    ASSERT( ( rpt.y > m_nSelLineTo )
            || ( ( rpt.y == m_nSelLineTo )
                 && ( rpt.x >= pli->nSelTo ) ) );
    CPoint ptSelFrom;
    ptSelFrom.y = m_nSelLineFrom;
    ptSelFrom.x = m_pdb->TextGetLineAt( ptSelFrom.y ) ->nSelFrom;
    CPoint ptSelTo;
    ptSelTo.y = m_nSelLineTo;
    ptSelTo.x = m_pdb->TextGetLineAt( ptSelTo.y ) ->nSelTo;
    // 计算目标插入点和选择内容最后点之间的偏移量，新的插入点等
    // 于原选择范围的起点加上面的偏移量
    LONG nOffset = ( m_pdb->TextSubLineToOffset( rpt )
                     - m_pdb->TextSubLineToOffset( ptSelTo )
                     + m_pdb->TextSubLineToOffset( ptSelFrom ) );
    SelNoneAndRemoveSelected();
    m_pdb->TextOffsetToSubLine( nOffset, rpt );
  }
}
// void WrapAtUnits(LONG nWidth)
// 以指定逻辑单元数实施换行
// LONG nWidth - 以逻辑单元数指出换行宽度，这样可以弹性处理换行功能，例
//            如在窗口边换行或按打印页宽换行等
void CPfxEditView::WrapAtUnits( LONG nWidth ) {
  ASSERT( nWidth > 0 );
  CClientDC1 dc( this );
  HGDIOBJ hFont = dc.SelectObject( PFX_EC_DATA.m_hFontText );
  CPfxEditHeap Heap;
  Heap.Alloc( Heap.GetBufferConstSize() );
  CPfxEditViewWrapLineHint wl;
  wl.m_hDC = dc;
  wl.m_nWidth = nWidth;
  wl.m_nliCur = 0;
  wl.m_nliMax = Heap.GetBufferConstSize() / sizeof( CPfxEditLineInfo );
  wl.m_nGrowBy = 10000;
  wl.m_nTabSize = GetTabSize();
  wl.m_nCharWidthMax = PFX_EC_DATA.m_nCharWidthMax;
  wl.m_nCharWidthSpace = PFX_EC_DATA.m_nCharWidthSpace;
  wl.m_nCharWidthTab = PFX_EC_DATA.m_nCharWidthTab;
  LONG nMainLines = 0;
  LONG nLineCount = m_pdb->TextGetLineCount();
  CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( 0 );
  CPfxEditLineInfo* pliEnd = m_pdb->TextGetLineAt( ( nLineCount - 1 ) );
  // 计算换行进度数据，这些数据将会转换成完成百分比通知送给m_pdb->EventSendMsg()，
  // 一般用来显示当前的进度给用户看
  LONG nPercentCompleted = 0;
  LONG nLinesOfOnePercent = max( 1, ( ( GetLineCount() + 99 ) / 100 ) );
  while ( pli <= pliEnd ) {
    if ( ( nMainLines % nLinesOfOnePercent ) == 0 ) {
      nPercentCompleted++;
      m_pdb->EventSendMsg( PFX_EN_WORD_WRAPPING, ( LPARAM ) nPercentCompleted );
    }
    // 保存真实的行号
    pli->nIndex = nMainLines++;
    WrapLineAtUnits( Heap, pli, wl );
    // 跳过子行
    while ( ( ++pli <= pliEnd ) && PFX_EC_IS_SUBLINE( pli ) ) {
      // do nothing
    }
  }
  dc.SelectObject( hFont );
  if ( ( wl.m_nliCur > 0 ) && ( Heap.GetBuffer() != NULL ) ) {
    CPoint ptFake( 0, 0 );
    m_pdb->TextUpdateLineInfo( reinterpret_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ), 0, nLineCount, wl.m_nliCur, ptFake );
    // 一定要在这里更新该标识，否则后面便可能出错
    m_pdb->m_nWrapWidth = nWidth;
    m_pdb->m_nWrapStyle = PFX_EC_WRAP_WINDOWEDGE;
    ASSERT( !PFX_EC_IS_DRAW_HARDRETURN( m_pdb->TextGetLineAt( m_pdb->TextGetLineUpperBound() ) ) );
  }
  // 格式化操作完成%100
  if ( nPercentCompleted < 100 ) {
    m_pdb->EventSendMsg( PFX_EN_WORD_WRAPPING, 100 );
  }
}
// 以指定字符数计算换行，并实施换行操作
// LONG nWidth - 以逻辑字符数指出换行宽度，逻辑字符数是指将制表符扩展后的字符数
void CPfxEditView::WrapColumns( LONG nWidth ) {
  ASSERT( nWidth > 0 );
  CPfxEditHeap Heap;
  Heap.Alloc( Heap.GetBufferConstSize() );
  CPfxEditViewWrapLineHint wl;
  wl.m_nWidth = nWidth;
  wl.m_nliCur = 0;
  wl.m_nliMax = Heap.GetBufferConstSize() / sizeof( CPfxEditLineInfo );
  wl.m_nGrowBy = 10000;
  wl.m_nTabSize = GetTabSize();
  LONG nMainLines = 0;
  LONG nLineCount = m_pdb->TextGetLineCount();
  CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( 0 );
  CPfxEditLineInfo* pliEnd = m_pdb->TextGetLineAt( ( nLineCount - 1 ) );
  // 计算换行进度数据，这些数据将会转换成完成百分比通知送给m_pdb->EventSendMsg()，
  // 一般用来显示当前的进度给用户看
  LONG nPercentCompleted = 0;
  LONG nLinesOfOnePercent = max( 1, ( ( GetLineCount() + 99 ) / 100 ) );
  while ( pli <= pliEnd ) {
    if ( ( nMainLines % nLinesOfOnePercent ) == 0 ) {
      nPercentCompleted++;
      m_pdb->EventSendMsg( PFX_EN_WORD_WRAPPING, ( LPARAM ) nPercentCompleted );
    }
    // 保存真实的行号
    pli->nIndex = nMainLines++;
    WrapLineColumns( Heap, pli, wl );
    // 跳过子行
    while ( ( ++pli <= pliEnd ) && PFX_EC_IS_SUBLINE( pli ) ) {
      // do nothing
    }
  }
  if ( ( wl.m_nliCur > 0 ) && ( Heap.GetBuffer() != NULL ) ) {
    CPoint ptFake( 0, 0 );
    m_pdb->TextUpdateLineInfo( reinterpret_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ), 0, nLineCount, wl.m_nliCur, ptFake );
    // 一定要在这里更新该标识，否则后面便可能出错
    m_pdb->m_nWrapWidth = nWidth;
    m_pdb->m_nWrapStyle = PFX_EC_WRAP_COLUMNS;
    ASSERT( !PFX_EC_IS_DRAW_HARDRETURN( m_pdb->TextGetLineAt( m_pdb->TextGetLineUpperBound() ) ) );
  }
  // 格式化操作完成%100
  if ( nPercentCompleted < 100 ) {
    m_pdb->EventSendMsg( PFX_EN_WORD_WRAPPING, 100 );
  }
}
// void WrapRestore()
// 恢复行缓冲区内的行信息为没有换行时的状态
// 如果当前行是一个子行，则还原后当前行要落在其主行上，并且插入点要重新
// 调整，如果当前行不是子行，则还原后要确保当前行不变（内容不变而非行号）
void CPfxEditView::WrapRestore() {
  LONG nMainLines = 0;
  LONG nLineCount = m_pdb->TextGetLineCount();
  CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( 0 );
  CPfxEditLineInfo* pliEnd = m_pdb->TextGetLineAt( ( nLineCount - 1 ) );
  // 计算换行进度数据，这些数据将会转换成完成百分比通知送给m_pdb->EventSendMsg()，
  // 一般用来显示当前的进度给用户看
  LONG nPercentCompleted = 0;
  LONG nLinesOfOnePercent = max( 1, ( ( GetLineCount() + 99 ) / 100 ) );
  for ( CPfxEditLineInfo * pliMain = pli; pli <= pliEnd; pliMain++, nMainLines++ ) {
    ASSERT( !PFX_EC_IS_SUBLINE( pli ) );
    // 只有当文档体积大于64KTCHAR时才发送进度信息
    if ( ( nMainLines % nLinesOfOnePercent ) == 0 ) {
      nPercentCompleted++;
      m_pdb->EventSendMsg( PFX_EN_WORD_WRAPPING, ( LPARAM ) nPercentCompleted );
    }
    // 如果是主行并且有子行就要删除它的附属子行
    pliMain->dwFlags = ( pli->dwFlags | PFX_EC_LF_HARDRETURN );
    pliMain->nActLen = pliMain->nLen = pli->nActLen;
    pliMain->pData = pli->pData;
    pliMain->pMemBlock = pli->pMemBlock;
    pliMain->nIndex = PFX_EC_INVALID_LINE;
    pliMain->nFirstChar = 0;
    // 跳过子行
    while ( ( ++pli <= pliEnd ) && PFX_EC_IS_SUBLINE( pli ) ) {
      // do nothing...
    }
  }
  if ( nMainLines < nLineCount ) {
    m_pdb->TextRemoveLine( nMainLines, ( nLineCount - nMainLines ) );
  }
  // 一定要在这里更新该标识，否则便可能出错
  m_pdb->m_nWrapWidth = 0;
  m_pdb->m_nWrapStyle = PFX_EC_WRAP_NONE;
  // 最后一行肯定没有回车换行，所以要消去该标志
  pli = m_pdb->TextGetLineAt( m_pdb->TextGetLineUpperBound() );
  pli->dwFlags &= ~PFX_EC_LF_HARDRETURN;
  // 格式化操作完成%100
  if ( nPercentCompleted < 100 ) {
    m_pdb->EventSendMsg( PFX_EN_WORD_WRAPPING, 100 );
  }
}
// void WrapLineAtUnits(CPfxEditHeap& Heap,const CPfxEditLineInfo* pli,CPfxEditViewWrapLineHint& wl)
// 以指定逻辑单元宽度计算一行的信息
// wl.m_nWidth - w.m_hDC的逻辑单元数
void CPfxEditView::WrapLineAtUnits( CPfxEditHeap& Heap, const CPfxEditLineInfo* pli, CPfxEditViewWrapLineHint& wl ) {
  ASSERT( pli != NULL );
  ASSERT( wl.m_hDC != NULL );
  ASSERT( wl.m_nWidth > 0 );
  ASSERT( wl.m_nGrowBy > 0 );
  if ( wl.m_nliCur >= wl.m_nliMax ) {
    wl.m_nliMax += wl.m_nGrowBy;
    Heap.ReAlloc( wl.m_nliMax * sizeof( CPfxEditLineInfo ) );
  }
  LONG nLogChar;
  CPfxEditLineInfo* pliCached = ( ( static_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ) ) + wl.m_nliCur );
  pliCached->dwFlags = ( pli->dwFlags & ( ~( PFX_EC_LF_SUBLINE | PFX_EC_LF_HARDRETURN ) ) );
  pliCached->nActLen = pliCached->nLen = pli->nActLen;
  pliCached->pData = pli->pData;
  pliCached->pMemBlock = pli->pMemBlock;
  pliCached->nIndex = pli->nIndex;
  pliCached->nFirstChar = nLogChar = 0;
  // 假定每个字符都具有该字体中最宽字符的宽度，则在给定的宽度内最小能
  // 容纳的字符数为(width / max_char_width)，用这个字符数作为每次推进
  // 的长度，每次推进后用GetTextExtentPoint32来计算该段字符的实际长度，
  // 如果小于给定宽度则继续；如果刚好等于就停止；如果大于就丢弃已计算
  // 的这段字符的长度，将步进值降低一个数量级重新计算
  LPCTSTR pStart = pli->pData;
  LPCTSTR pEnd = pStart + pli->nActLen;
  // 这里nWrapWidth减去1是为了右边有一定的空隙
  for ( LONG nSubLine = 1, nWrapWidth = ( wl.m_nWidth - 1 ); pStart < pEnd; nSubLine++ ) {
    LPCTSTR pCur = pStart;
    LPCTSTR pTmp = pStart;
    LONG nTestWidth = 0;
    LONG nTestChars = ( nWrapWidth / wl.m_nCharWidthMax );
    for ( ;; )           //      下一次推测
    {
      CSize size;
      LONG nLogCharSaved = nLogChar;
      LONG nTestWidthSaved = nTestWidth;
      nTestChars = max( 1, nTestChars );
      for ( LONG i = nTestChars; ( i > 0 ) && ( pTmp < pEnd ); i-- ) {
        if ( PFX_EC_IS_LEAD_BYTE( *pTmp ) ) {
          pTmp += PFX_EC_MAX_MB_LEN;
          nLogChar += PFX_EC_MAX_MB_LEN;
          continue;
        }
        if ( *pTmp != PFX_EC_CHR_TAB ) {
          pTmp++;
          nLogChar++;
        } else {
          LONG nTabSize = wl.m_nTabSize - ( nLogChar % wl.m_nTabSize );
          //   上面for的i已经减了一
          i -= ( nTabSize - 1 );
          nTestWidth += ( nTabSize * wl.m_nCharWidthSpace - wl.m_nCharWidthTab );
          pTmp++;
          nLogChar += nTabSize;
        }
      }
      // 如果在这次推测后已经到达行末
      if ( pTmp >= pEnd ) {
        ::GetTextExtentPoint32( wl.m_hDC, pCur, pEnd - pCur, &size );
        if ( ( nTestWidth + size.cx ) <= nWrapWidth ) {
          goto WrapLineCompleted;
        }
        //  如果一个字符便已大于给定的宽度就取一个字
        //  符一行了，否则，推倒，将步进值降低一个数
        //  量级从头再来
        if ( nTestChars == 1 ) {
          if ( pCur > pStart ) {
            nLogChar = nLogCharSaved;
            break;
          }
          goto WrapLineCompleted;
        } else {
          pTmp = pCur;
          nLogChar = nLogCharSaved;
          nTestWidth = nTestWidthSaved;
          nTestChars /= 10;
          continue; // for (;;) // 下一次推测
        }
      } else {
        ::GetTextExtentPoint32( wl.m_hDC, pCur, pTmp - pCur, &size );
        nTestWidth += size.cx;
        if ( nTestWidth < nWrapWidth ) {
          pCur = pTmp;
          nTestChars = ( nWrapWidth - nTestWidth ) / wl.m_nCharWidthMax;
          continue; // for (;;) // 下一次推测
        }
        if ( nTestWidth == nWrapWidth ) {
          pCur = pTmp;
          break;
        }
        if ( nTestChars <= 1 ) {
          //   如果一个字符的宽度便已经超出给定
          //   的宽度，并且这一行只有一个字符，
          //   那么也只有让它超出
          if ( pCur == pStart ) {
            pCur = pTmp;
          }
          //   如果这一行不止一个字符就不将该字
          //   符纳入该行
          else {
            nLogChar = nLogCharSaved;
          }
          break;
        }
        //  推倒，将步进值降低一个数量级从头再来
        else {
          pTmp = pCur;
          nLogChar = nLogCharSaved;
          nTestWidth = nTestWidthSaved;
          nTestChars /= 10;
          continue; // for (;;) // 下一次推测
        }
      }
    }
    ASSERT( pCur > pStart );
    // 现在读写指针所处的位置可能在一个单词的中间，因此要作出判
    // 断，如果是在一个单词的中间就要回溯至一个单词定界符，这样
    // 速度将会非常慢，所以这里只识别空格或制表符，如果回溯时找
    // 不到空格和制表符，就只能将该“单词”强行断开，很明显，这
    // 样做将会导致“质量”问题
    if ( !IsWhitespace( *pCur ) && !PFX_EC_IS_LEAD_BYTE( *pCur ) ) {
      for ( pTmp = ( pCur - 1 ); pTmp > pStart; pTmp-- ) {
        //  如果pTmp所指向的字符就是空白字符，就要跨
        //  过该字符，以免下一行的开头就是空白字符
        if ( IsWhitespace( *pTmp ) ) {
          pTmp++;
          nLogChar -= ( pCur - pTmp );
          pCur = pTmp;
          break;
        }
      }
    }
    // 绝对不允许出现空的子行
    ASSERT( pCur < pEnd );
    ASSERT( pCur > pStart );
    pliCached->nLen = pCur - pStart;
    pliCached++;
    wl.m_nliCur++;
    if ( wl.m_nliCur >= wl.m_nliMax ) {
      wl.m_nliMax += wl.m_nGrowBy;
      Heap.ReAlloc( wl.m_nliMax * sizeof( CPfxEditLineInfo ) );
      pliCached = ( ( static_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ) ) + wl.m_nliCur );
    }
    pliCached->dwFlags = PFX_EC_LF_SUBLINE;
    pliCached->nLen = pEnd - pCur;
    pliCached->nActLen = nSubLine;
    pliCached->pData = const_cast<LPTSTR>( pCur );
    pliCached->pMemBlock = NULL;
    pliCached->nIndex = PFX_EC_INVALID_LINE;
    pliCached->nFirstChar = nLogChar;
    pStart = pCur;
  }
WrapLineCompleted:
  wl.m_nliCur++;
  pliCached->dwFlags |= PFX_EC_LF_HARDRETURN;
  return ;
}
// void WrapLineColumns(CPfxEditHeap& Heap,const CPfxEditLineInfo* pli,CPfxEditViewWrapLineHint& wl)
// wl.m_nWidth - 按列数而非字符数来计算，一个中文字符等于两列
void CPfxEditView::WrapLineColumns( CPfxEditHeap& Heap, const CPfxEditLineInfo* pli, CPfxEditViewWrapLineHint& wl ) {
  ASSERT( pli != NULL );
  ASSERT( wl.m_nWidth > 0 );
  ASSERT( wl.m_nGrowBy > 0 );
  if ( wl.m_nliCur >= wl.m_nliMax ) {
    wl.m_nliMax += wl.m_nGrowBy;
    Heap.ReAlloc( wl.m_nliMax * sizeof( CPfxEditLineInfo ) );
  }
  LONG nLogChar;
  CPfxEditLineInfo* pliCached = ( ( static_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ) ) + wl.m_nliCur );
  pliCached->dwFlags = ( pli->dwFlags & ( ~( PFX_EC_LF_SUBLINE | PFX_EC_LF_HARDRETURN ) ) );
  pliCached->nActLen = pliCached->nLen = pli->nActLen;
  pliCached->pData = pli->pData;
  pliCached->pMemBlock = pli->pMemBlock;
  pliCached->nIndex = pli->nIndex;
  pliCached->nFirstChar = nLogChar = 0;
  // 假设所有的字符都是TAB，如果都达不到换行的要求就不需要继续操作
  if ( ( pli->nActLen * wl.m_nTabSize ) > wl.m_nWidth ) {
    LPCTSTR pStart = pli->pData;
    LPCTSTR pCur = pStart;
    LPCTSTR pEnd = pStart + pli->nActLen;
    for ( LONG nSubLine = 1; pStart < pEnd; nSubLine++ ) {
      LONG nTestChars = 0;
      LPCTSTR pTmp = pCur;
      while ( ( pCur < pEnd ) && ( nTestChars < wl.m_nWidth ) ) {
        pTmp = pCur;
        if ( PFX_EC_IS_LEAD_BYTE( *pCur ) ) {
          nTestChars += PFX_EC_MAX_MB_LEN;
          pCur += PFX_EC_MAX_MB_LEN;
          nLogChar += PFX_EC_MAX_MB_LEN;
          continue;
        }
        if ( *pCur != PFX_EC_CHR_TAB ) {
          nTestChars++;
          pCur++;
          nLogChar++;
        } else {
          LONG nTabSize = wl.m_nTabSize
                          - ( nLogChar % wl.m_nTabSize );
          nTestChars += nTabSize;
          pCur++;
          nLogChar += nTabSize;
        }
      }
      // 这行长度未达到换行要求，不需要换行
      if ( pCur >= pEnd ) {
        break;
      }
      ASSERT( pCur > pStart );
      if ( nTestChars > wl.m_nWidth ) {
        pCur = pTmp;
      }
      // 现在读写指针所处的位置可能在一个单词的中间，因此
      // 要作出判断，如果是在一个单词的中间就要回溯至一个
      // 单词定界符，这样速度将会非常慢，所以这里只识别空
      // 格或制表符，如果回溯时找不到空格和制表符，就只能
      // 将该“单词”强行断开，很明显，这样做将会导致“质
      // 量”问题
      if ( !IsWhitespace( *pCur ) ) {
        for ( pTmp = ( pCur - 1 ); pTmp > pStart; pTmp-- ) {
          //   如果pTmp所指向的字符就是空白字符，
          //   就要跨过该字符，以免下一行的开头
          //   就是空白字符
          if ( IsWhitespace( *pTmp ) ) {
            pTmp++;
            nLogChar -= ( pCur - pTmp );
            pCur = pTmp;
            break;
          }
        }
      }
      // 绝对不允许出现空的子行
      ASSERT( pCur < pEnd );
      pliCached->nLen = pCur - pStart;
      pliCached++;
      wl.m_nliCur++;
      if ( wl.m_nliCur >= wl.m_nliMax ) {
        wl.m_nliMax += wl.m_nGrowBy;
        Heap.ReAlloc( wl.m_nliMax * sizeof( CPfxEditLineInfo ) );
        pliCached = ( ( static_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ) ) + wl.m_nliCur );
      }
      pliCached->dwFlags = PFX_EC_LF_SUBLINE;
      pliCached->nLen = pEnd - pCur;
      pliCached->nActLen = nSubLine;
      pliCached->pData = const_cast<LPTSTR>( pCur );
      pliCached->pMemBlock = NULL;
      pliCached->nIndex = PFX_EC_INVALID_LINE;
      pliCached->nFirstChar = nLogChar;
      pStart = pCur;
    }
  }
  // 完成一主行的计算，最后一定要加上硬回车标志，因为它标识一行的结束
  wl.m_nliCur++;
  pliCached->dwFlags |= PFX_EC_LF_HARDRETURN;
}
// LONG WrapLineSingle(CPfxEditHeap& Heap, const CPfxEditLineInfo* pli)
// 连同主行和子行在内的总行数
LONG CPfxEditView::WrapLineSingle( CPfxEditHeap& Heap, const CPfxEditLineInfo* pli ) {
  LONG nLines;
  CPfxEditLineInfo* pliTmp;
  CPfxEditViewWrapLineHint wl;
  switch ( GetWrapStyle() ) {
  case PFX_EC_WRAP_WINDOWEDGE:
    if ( PFX_EC_IS_EMPTY_LINE( pli ) ) {
      pliTmp = static_cast<CPfxEditLineInfo*>( Heap.Alloc( sizeof( CPfxEditLineInfo ) ) );
      m_pdb->TextConstructEmptyLineInfo( pliTmp, 1 );
      pliTmp->dwFlags = pli->dwFlags;
      nLines = 1; // 一行
    } else {
      CClientDC1 dc( this );
      HGDIOBJ hFont = dc.SelectObject( PFX_EC_DATA.m_hFontText );
      wl.m_hDC = dc;
      wl.m_nWidth = m_rectText.Width();
      wl.m_nliCur = 0;
      wl.m_nliMax = 0;
      wl.m_nGrowBy = Heap.GetBufferConstSize() / sizeof( CPfxEditLineInfo );
      wl.m_nTabSize = GetTabSize();
      wl.m_nCharWidthMax = PFX_EC_DATA.m_nCharWidthMax;
      wl.m_nCharWidthSpace = PFX_EC_DATA.m_nCharWidthSpace;
      wl.m_nCharWidthTab = PFX_EC_DATA.m_nCharWidthTab;
      WrapLineAtUnits( Heap, pli, wl );
      nLines = wl.m_nliCur;
      dc.SelectObject( hFont );
    }
    break;
  case PFX_EC_WRAP_COLUMNS:
    if ( PFX_EC_IS_EMPTY_LINE( pli ) ) {
      pliTmp = static_cast<CPfxEditLineInfo*>( Heap.Alloc( sizeof( CPfxEditLineInfo ) ) );
      m_pdb->TextConstructEmptyLineInfo( pliTmp, 1 );
      pliTmp->dwFlags = pli->dwFlags;
      nLines = 1; // 一行
    } else {
      wl.m_nWidth = GetWrapWidth();
      wl.m_nliCur = 0;
      wl.m_nliMax = 0;
      wl.m_nGrowBy = Heap.GetBufferConstSize() / sizeof( CPfxEditLineInfo );
      wl.m_nTabSize = GetTabSize();
      WrapLineColumns( Heap, pli, wl );
      nLines = wl.m_nliCur;
    }
    break;
  case PFX_EC_WRAP_NONE:
  default:           // 贯通
    {
      pliTmp = static_cast<CPfxEditLineInfo*>( Heap.Alloc( sizeof( CPfxEditLineInfo ) ) );
      memcpy( pliTmp, pli, sizeof( CPfxEditLineInfo ) );
      pliTmp->nLen = pliTmp->nActLen;
      nLines = 1; // 一行
    }
    break;
  }
  pliTmp = ( static_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ) );
  pliTmp->nIndex = pli->nIndex;
  pliTmp = ( static_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ) ) + ( nLines - 1 );
  pliTmp->dwFlags |= PFX_EC_LF_HARDRETURN;
  return nLines;
}
// LONG WrapLineMultiple(CPfxEditHeap& Heap,const CPfxEditLineInfo* pli,LONG nCount,LONG& rnLineToRemember)
// CPfxEditHeap& Heap      - 用于存放调整后行的信息的缓冲区
// const CPfxEditLineInfo* pli - 将被调整的第一行的指针
// LONG nCount             - 总共有多少行将被调整
// LONG& rnLineToRemember  - 在这nCount个主行中，跟踪该行序号的改变，
//                        输入时它的正确范围是[0, (nCount-1)]，返
//                        回后它的范围将会是[0, (返回数值-1)]
// 连同主行和子行在内的总行数
// rnLineToRemember - 将会始终指向被调整前那个要记忆的行，但它的序号可
// 能已经改变了，用pli = (static_cast<CPfxEditLineInfo*>(Heap.GetBuffer())
// + rnLineToRemember)在函数返回后可以指向它
LONG CPfxEditView::WrapLineMultiple( CPfxEditHeap& Heap, const CPfxEditLineInfo* pli, LONG nCount, LONG& rnLineToRemember ) {
  ASSERT( ( rnLineToRemember >= 0 ) && ( rnLineToRemember < nCount ) );
  LONG i;
  LONG nLines;
  LONG nLineToRemember;
  CPfxEditViewWrapLineHint wl;
  CPfxEditLineInfo* pliTmp;
  switch ( GetWrapStyle() ) {
  case PFX_EC_WRAP_WINDOWEDGE: {
      CClientDC1 dc( this );
      HGDIOBJ hFont = dc.SelectObject( PFX_EC_DATA.m_hFontText );
      wl.m_hDC = dc;
      wl.m_nWidth = m_rectText.Width();
      wl.m_nliCur = 0;
      wl.m_nliMax = Heap.GetBufferConstSize() / sizeof( CPfxEditLineInfo );
      wl.m_nGrowBy = 10000;
      wl.m_nTabSize = GetTabSize();
      wl.m_nCharWidthMax = PFX_EC_DATA.m_nCharWidthMax;
      wl.m_nCharWidthSpace = PFX_EC_DATA.m_nCharWidthSpace;
      wl.m_nCharWidthTab = PFX_EC_DATA.m_nCharWidthTab;
      Heap.Alloc( ( wl.m_nliMax * sizeof( CPfxEditLineInfo ) ) );
      for ( i = 0; i < nCount; i++, pli++ ) {
        if ( i == rnLineToRemember ) {
          nLineToRemember = wl.m_nliCur;
        }
        WrapLineAtUnits( Heap, pli, wl );
      }
      dc.SelectObject( hFont );
      nLines = wl.m_nliCur;
    }
    break;
  case PFX_EC_WRAP_COLUMNS: {
      wl.m_nWidth = GetWrapWidth();
      wl.m_nliCur = 0;
      wl.m_nliMax = Heap.GetBufferConstSize() / sizeof( CPfxEditLineInfo );
      wl.m_nGrowBy = 10000;
      wl.m_nTabSize = GetTabSize();
      Heap.Alloc( ( wl.m_nliMax * sizeof( CPfxEditLineInfo ) ) );
      for ( i = 0; i < nCount; i++, pli++ ) {
        if ( i == rnLineToRemember ) {
          nLineToRemember = wl.m_nliCur;
        }
        WrapLineColumns( Heap, pli, wl );
      }
      nLines = wl.m_nliCur;
    }
    break;
  case PFX_EC_WRAP_NONE:
  default:           // 贯通
    {
      pliTmp = static_cast<CPfxEditLineInfo*>( Heap.Alloc( ( sizeof( CPfxEditLineInfo ) * nCount ) ) );
      memcpy( pliTmp, pli, ( sizeof( CPfxEditLineInfo ) * nCount ) );
      for ( i = 0; i < nCount; i++, pliTmp++ ) {
        pliTmp->nLen = pliTmp->nActLen;
      }
      nLines = nCount; // 一行
      nLineToRemember = rnLineToRemember;
    }
    break;
  }
  pliTmp = ( static_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ) ) + ( nLines - 1 );
  pliTmp->dwFlags |= PFX_EC_LF_HARDRETURN;
  rnLineToRemember = nLineToRemember;
  return nLines;
}
// 派生类重载本函数设置打印参数及相关资源
BOOL CPfxEditView::OnSetupPrintInfo( CPfxEditPrintInfo* pPrintInfo ) {
  UNUSED( pPrintInfo );
  return true;
}
// void SelGetParam()
// 如果有选择内容又使用键盘或鼠标改变了当前位置，本函数将返回新的选择位
// 置以决定新的选择范围
CPoint CPfxEditView::SelGetParam() const {
  if ( !IsTextSelected() ) {
    return m_ptEditPos;
  }
  const CPfxEditLineInfo* pliFrom = m_pdb->TextGetLineAt( m_nSelLineFrom );
  CPoint ptFrom( min( pliFrom->nLen, pliFrom->nSelFrom ), m_nSelLineFrom );
  const CPfxEditLineInfo* pliTo = m_pdb->TextGetLineAt( m_nSelLineTo );
  CPoint ptTo( min( pliTo->nLen, pliTo->nSelTo ), m_nSelLineTo );
  ASSERT( ( ptFrom == m_ptEditPos ) || ( ptTo == m_ptEditPos ) );
  return ( ( ptFrom != m_ptEditPos ) ? ptFrom : ptTo );
}
// void SelRange(CPoint pt)
// 设置选择范围：从指定位置选择到当前位置
void CPfxEditView::SelRange( CPoint pt ) {
  LONG i;
  LONG nSelSize;
  CPfxEditLineInfo* pli;
  // 先保存当前的设置，当计算更新范围的时候要用到
  LONG nOldSelLineFrom = m_nSelLineFrom;
  LONG nOldSelLineTo = m_nSelLineTo;
  CPoint ptFrom = pt;
  CPoint ptTo = m_ptEditPos;
  // 确保参数正确
  ptFrom.y = m_pdb->TextVerifyLine( ptFrom.y );
  ptFrom.x = m_pdb->TextVerifyChar( ptFrom.y, ptFrom.x );
  ptTo.y = m_pdb->TextVerifyLine( ptTo.y );
  ptTo.x = m_pdb->TextVerifyChar( ptTo.y, ptTo.x );
  // 首先要确保ptFrom.y在ptTo.y之前
  if ( ptFrom.y > ptTo.y ) {
    i = ptFrom.y;
    ptFrom.y = ptTo.y;
    ptTo.y = i;
    i = ptFrom.x;
    ptFrom.x = ptTo.x;
    ptTo.x = i;
  }
  // 如果选择范围是在一行内
  if ( ptFrom.y == ptTo.y ) {
    pli = m_pdb->TextGetLineAt( ptFrom.y );
    // 首先要确保ptFrom.x在ptTo.x之前
    if ( ptFrom.x > ptTo.x ) {
      i = ptFrom.x;
      ptFrom.x = ptTo.x;
      ptTo.x = i;
    }
    pli->nSelFrom = ptFrom.x;
    pli->nSelTo = ptTo.x;
    // 如果ptFrom.y等于ptTo.y和ptFrom.x等于ptTo.x就等于没有选择，
    // 否则就会产生有选择但nSelSize又等于零的情况
    nSelSize = ptTo.x - ptFrom.x;
    if ( nSelSize == 0 ) {
      ptFrom.y = ptTo.y = PFX_EC_INVALID_SELECT;
    }
  }
  // 如果选择范围在多行内
  else {
    LONG nLineBreakSize = GetLineBreakSize();
    // 第一行选择后部分
    pli = m_pdb->TextGetLineAt( ptFrom.y );
    pli->nSelFrom = ptFrom.x;
    if ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      pli->nSelTo = pli->nLen + PFX_EC_VIRTUAL_LINE_BREAK_SIZE;
      nSelSize = pli->nLen - ptFrom.x + nLineBreakSize;
    } else {
      pli->nSelTo = pli->nLen;
      nSelSize = pli->nLen - ptFrom.x;
    }
    // 中间行全部选择
    for ( i = ptFrom.y + 1; i < ptTo.y; i++ ) {
      pli = m_pdb->TextGetLineAt( i );
      pli->nSelFrom = 0;
      // 有硬换行符的行才可以加上换行符的长度
      if ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
        pli->nSelTo = pli->nLen + PFX_EC_VIRTUAL_LINE_BREAK_SIZE;
        nSelSize += ( pli->nLen + nLineBreakSize );
      } else {
        pli->nSelTo = pli->nLen;
        nSelSize += pli->nLen;
      }
    }
    // 最后行选择前部分
    pli = m_pdb->TextGetLineAt( ptTo.y );
    pli->nSelFrom = 0;
    pli->nSelTo = ptTo.x;
    nSelSize += ptTo.x;
  }
  m_nSelLineFrom = ptFrom.y;
  m_nSelLineTo = ptTo.y;
  m_nSelSize = nSelSize;
  // 如果现在已经是列块选择则一定要取消列块选择
  if ( PFX_EC_SM_COLUMN == m_nSelMode ) {
    m_nSelMode = PFX_EC_SM_NORMAL;
    Invalidate( false );
  } else {
    // 更新改变的范围
    UpdateLinesSelected( nOldSelLineFrom, nOldSelLineTo );
  }
}
// void SelRange(CRect& rect)
// 执行列块选择
// CRect& rect - 以文本坐标指出要选择的范围，如果该矩形为空则取消当前任
//            何形式的选择
// 文本坐标是不是指系统逻辑坐标也不是指显示坐标，该坐标是对文档数据影像
// 而言，坐标原点是第一行第一个字符的左上角，单位是系统逻辑单位
void CPfxEditView::SelRange( CRect& rect ) {
  if ( IsTextSelected() ) {
    SelNone();
  }
  rect.NormalizeRect();
  LONG nLineFrom = TransTextPosToLinePos( rect.top );
  LONG nLineTo = TransTextPosToLinePos( rect.bottom );
  LONG nSelSize = 0;
  LONG xTextPosMax = 0;
  for ( LONG i = nLineFrom; i <= nLineTo; i++ ) {
    LONG xTextPos1 = rect.left;
    LONG xEditPos1 = TransTextPosToCharPos( i, xTextPos1 );
    LONG xTextPos2 = rect.right;
    LONG xEditPos2 = TransTextPosToCharPos( i, xTextPos2 );
    xTextPosMax = max( xTextPosMax, xTextPos2 );
    CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( i );
    pli->nSelFrom = xEditPos1;
    pli->nSelTo = xEditPos2;
    nSelSize += ( xEditPos2 - xEditPos1 );
  }
  m_nSelLineFrom = nLineFrom;
  m_nSelLineTo = nLineTo;
  m_nSelSize = nSelSize;
  m_nSelMode = PFX_EC_SM_COLUMN;
  m_rectColSelect.SetRect( rect.left, rect.top, xTextPosMax, rect.bottom );
  // 更新改变的范围
  UpdateLinesSelected( PFX_EC_INVALID_SELECT, PFX_EC_INVALID_SELECT );
}
// void SelWord(CPoint pt)
// 选择一词
// CPoint pt - 相对位置
// 如果选择成功，新的当前位置将被设置在选择范围的最后面
void CPfxEditView::SelWord( CPoint pt ) {
  ASSERT( m_pdb->TextIsValidLine( pt.y ) );
  ASSERT( m_pdb->TextIsValidChar( pt.y, pt.x ) );
  // 在读写一行中的数据之前，要注意当前位置是否在行末，在行末读写是非
  // 法的。但由于很多行共用一大块缓冲区，很多时候即使非法读写行末数据
  // 也不会即时出问题，除非这行是位于缓冲区的最后部分，所以要格外小心
  m_pdb->TextSubLineToMainLine( pt );
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
  if ( PFX_EC_IS_EMPTY_LINE( pli ) ) {
    m_ptEditPos.y = pt.y;
    m_ptEditPos.x = pt.x = 0;
  } else {
    LPCTSTR pStart = pli->pData;
    LPCTSTR pCur = pStart + pt.x;
    LPCTSTR pEnd = pStart + pli->nActLen;
    // 注意指针出界问题
    LPCTSTR pWordLeft = pCur;
    if ( !IsDelimiter( pCur, pEnd ) ) {
      pWordLeft = GetPrevDelimiterPos( pCur, pStart );
    }
    LPCTSTR pWordRight = GetNextCharPos( pCur, pEnd );
    if ( !IsDelimiter( pCur, pEnd ) ) {
      pWordRight = GetNextDelimiterPos( pCur, pEnd );
    }
    pt.x = pWordLeft - pStart;
    m_ptEditPos.x = pWordRight - pStart;
    m_ptEditPos.y = pt.y;
    m_pdb->TextMainLineToSubLine( pt );
    m_pdb->TextMainLineToSubLine( m_ptEditPos );
  }
  // 从pt选择至m_ptEditPos
  SelRange( pt );
}
// void SelLine(LONG nLine)
// 选择一整行
void CPfxEditView::SelLine( LONG nLine ) {
  CPoint ptFrom;
  // 确保位置正确
  m_ptEditPos.y = m_pdb->TextVerifyLine( nLine );
  m_ptEditPos.x = 0;
  // 从下一行的开始选择到本行的开始处，如果有回车就会连回车一并选择
  if ( m_ptEditPos.y < m_pdb->TextGetLineUpperBound() ) {
    ptFrom.y = m_ptEditPos.y + 1;
    ptFrom.x = 0;
  } else {
    ptFrom.y = m_ptEditPos.y;
    ptFrom.x = m_pdb->TextGetLineAt( ptFrom.y ) ->nLen;
  }
  // 从ptFrom选择至m_ptEditPos
  SelRange( ptFrom );
}
// void SelNone()
// 取消选择
void CPfxEditView::SelNone() {
  if ( !IsTextSelected() ) {
    return ;
  }
  LONG nOldSelLineFrom = m_nSelLineFrom;
  LONG nOldSelLineTo = m_nSelLineTo;
  m_nSelLineFrom = m_nSelLineTo = PFX_EC_INVALID_SELECT;
  m_nSelSize = 0;
  m_nSelMode = PFX_EC_SM_NORMAL;
  // 更新改变的范围
  UpdateLinesSelected( nOldSelLineFrom, nOldSelLineTo );
}
void CPfxEditView::SelNoneAndRemoveSelected() {
  if ( !IsTextSelected() ) {
    return ;
  }
  // 获取选择范围
  CPoint ptFrom;
  CPoint ptTo;
  ptFrom.y = m_nSelLineFrom;
  ptTo.y = m_nSelLineTo;
  // 准备用来更新兄弟视图
  DWORD dwFlags = ( ( ptFrom.y != ptTo.y ) ? PFX_EC_UT_VERT : PFX_EC_UT_HORZ );
  LONG nFirstUpdateLine = ptFrom.y;
  LONG nLastUpdateLine = ptTo.y;
  BOOL bColSelect = IsColumnSelect();
  // 取消选择
  SelNone();
  // 正常选择
  if ( !bColSelect ) {
    ptFrom.x = m_pdb->TextGetLineAt( ptFrom.y ) ->nSelFrom;
    ptTo.x = m_pdb->TextGetLineAt( ptTo.y ) ->nSelTo;
    CharsRemove( ptFrom, ptTo, true );
    ASSERT( m_pdb->TextIsValidLine( ptFrom.y ) );
    ASSERT( m_pdb->TextIsValidChar( ptFrom.y, ptFrom.x ) );
  }
  // 列块选择
  else {
    ASSERT( !IsWrapping() );
    // 执行删除操作
    for ( LONG i = ptTo.y, j = ptFrom.y; i >= j; i-- ) {
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( i );
      ptFrom.y = ptTo.y = i;
      ptFrom.x = pli->nSelFrom;
      ptTo.x = pli->nSelTo;
      CharsRemove( ptFrom, ptTo, ( i == j ) );
      ASSERT( m_pdb->TextIsValidLine( ptFrom.y ) );
      ASSERT( m_pdb->TextIsValidChar( ptFrom.y, ptFrom.x ) );
    }
  }
  if ( GetSiblingView() != NULL ) {
    UpdateAllViews( dwFlags, nFirstUpdateLine, nLastUpdateLine );
  }
}
void CPfxEditView::SelNoneAndUpdateCurrentPosition( LONG nAction ) {
  ASSERT( IsTextSelected() );
  CPoint ptFrom;
  CPoint ptTo;
  ptFrom.y = m_nSelLineFrom;
  ptTo.y = m_nSelLineTo;
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( ptFrom.y );
  ptFrom.x = min( pli->nLen, pli->nSelFrom );
  pli = m_pdb->TextGetLineAt( ptTo.y );
  ptTo.x = min( pli->nLen, pli->nSelTo );
  switch ( nAction ) {
  case VK_LEFT:           //      VK_UP, VK_PRIOR, VK_HOME是一样的处理
  case VK_UP:
  case VK_PRIOR:
  case VK_HOME:
    m_ptEditPos = ptFrom;
    break;
  case VK_RIGHT:           //      VK_DOWN, VK_NEXT, VK_END是一样的处理
  case VK_DOWN:
  case VK_NEXT:
  case VK_END:
    m_ptEditPos = ptTo;
    break;
  default:
    TRACE( "CPfxEditView::SelSelNoneAndUpdateCurrentPosition(): Unknown nAction %d\n", nAction );
    break;
  }
  ASSERT( m_pdb->TextIsValidLine( m_ptEditPos.y ) );
  ASSERT( m_pdb->TextIsValidChar( m_ptEditPos.y, m_ptEditPos.x ) );
  LONG xTextPos = TransCharPosToTextPos( m_ptEditPos );
  UpdateCurrentPosition( xTextPos, true, true );
  SelNone();
}
// void SetTracking(BOOL bOn)
// 是否使用SetCapture()跟踪鼠标
void CPfxEditView::SetTracking( BOOL bOn ) {
  if ( bOn ) {
    SetCapture();
  } else {
    if ( ::GetCapture() == M_HWND ) {
      ::ReleaseCapture();
    }
  }
  m_bTrackingMouse = bOn;
}
// BOOL EditUndo()
// 执行UNDO操作
BOOL CPfxEditView::EditUndo() {
  if ( !IsCanUndo() ) {
    return false;
  }
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  if ( IsTextSelected() ) {
    SelNone();
  }
  CPfxEditViewWorkingState state( this, ( ( GetUndoName() == PFX_EC_UNDO_REPLACE_ALL ) ) );
  LONG nLineCount = m_pdb->TextGetLineCount();
  // 开始UNDO循环，一个UNDO组中任何一个描述都有PFX_EC_UF_GROUP标志，一组中的
  // 最后一个描述有PFX_EC_UF_BEGIN_GROUP标志
  CPoint pt( 0, 0 );
  WORD fAction;
  do {
    const CPfxEditUndoInfo* pui = m_pdb->UndoGetInfo();
    if ( pui == NULL ) {
      break;
    }
    fAction = pui->fAction;
    if ( fAction & PFX_EC_UF_INSERT ) {
      m_pdb->UndoInsert( const_cast<CPfxEditUndoInfo*>( pui ), pt, false );
    } else {
      m_pdb->UndoRemove( const_cast<CPfxEditUndoInfo*>( pui ), pt, false );
    }
  } while ( !( fAction & PFX_EC_UF_BEGIN_GROUP ) && ( fAction & PFX_EC_UF_GROUP ) );
  m_ptEditPos = pt;
  ASSERT( m_pdb->TextIsValidLine( m_ptEditPos.y ) );
  ASSERT( m_pdb->TextIsValidChar( m_ptEditPos.y, m_ptEditPos.x ) );
  // 由于上面改变了缓冲区的数据，很多参数未经UpdateAllViews()和
  // EnsureVisible()修正之前可能无效，所以要小心调用函数
  // 例如在这里调用nFirstUpdateLine=GetFirstVisibleLine()便不正确
  DWORD dwFlags = ( ( m_pdb->TextGetLineCount() != nLineCount ) ? PFX_EC_UT_VERT : PFX_EC_UT_HORZ );
  UpdateAllViews( dwFlags, 0, -1 );
  EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  return true;
}
// 执行REDO操作
BOOL CPfxEditView::EditRedo() {
  if ( !IsCanRedo() ) {
    return false;
  }
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  if ( IsTextSelected() ) {
    SelNone();
  }
  CPfxEditViewWorkingState state( this, ( ( GetUndoName() == PFX_EC_UNDO_REPLACE_ALL ) ) );
  LONG nLineCount = m_pdb->TextGetLineCount();
  // 开始REDO循环，一个REDO组中任何一个描述都有PFX_EC_UF_GROUP标志，一组中的
  // 第一个描述有PFX_EC_UF_BEGIN_GROUP标志
  CPoint pt( 0, 0 );
  const CPfxEditUndoInfo* pui = m_pdb->UndoGetRedoInfo();
  WORD fAction = pui->fAction;
  do {
    if ( fAction & PFX_EC_UF_INSERT ) {
      m_pdb->UndoRemove( const_cast<CPfxEditUndoInfo*>( pui ), pt, true );
    } else {
      m_pdb->UndoInsert( const_cast<CPfxEditUndoInfo*>( pui ), pt, true );
    }
    pui = m_pdb->UndoGetRedoInfo();
    if ( pui == NULL ) {
      break;
    }
    fAction = pui->fAction;
  } while ( !( fAction & PFX_EC_UF_BEGIN_GROUP ) && ( fAction & PFX_EC_UF_GROUP ) );
  m_ptEditPos = pt;
  ASSERT( m_pdb->TextIsValidLine( m_ptEditPos.y ) );
  ASSERT( m_pdb->TextIsValidChar( m_ptEditPos.y, m_ptEditPos.x ) );
  // 由于上面改变了缓冲区的数据，很多参数未经UpdateAllViews()和
  // EnsureVisible()修正之前可能无效，所以要小心调用函数
  // 例如在这里调用nFirstUpdateLine=GetFirstVisibleLine()便不正确
  DWORD dwFlags = ( ( m_pdb->TextGetLineCount() != nLineCount ) ? PFX_EC_UT_VERT : PFX_EC_UT_HORZ );
  UpdateAllViews( dwFlags, 0, -1 );
  EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  return true;
}
// 将当前选择内容复制到剪贴板
// BOOL bAppend - 是否将选择内容附加到剪贴板已有的数据后面
BOOL CPfxEditView::EditCopy( BOOL bAppend ) {
  HGLOBAL hGlobal = DragDropRender( false, bAppend );
  if ( hGlobal == NULL ) {
    return false;
  }
  CPfxEditViewClipboard clipboard( this, CF_TEXT, false );
  clipboard.Attach( hGlobal );
  return clipboard.SetClipboardData();
}
// 剪切当前已选择内容
// BOOL bAppend - 是否将当前选择内容附加到剪贴板已有的数据后面
BOOL CPfxEditView::EditCut( BOOL bAppend ) {
  if ( !IsTextSelected() ) {
    return false;
  }
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  if ( !EditCopy( bAppend ) ) {
    return false;
  }
  return EditRemove( -1, 0, -1, 0 );
}
// 在指定位置粘贴来自剪贴板内的可用数据
// LONG nLine, LONG nChar - 粘贴位置
// LONG nType             - 粘贴类型，可选有：
//                       PFX_EC_PASTE_NORMAL        - 按正常方式粘贴
//                       PFX_EC_PASTE_COLUMN_ALIGN  - 按列块对齐方式粘贴
//                       PFX_EC_PASTE_COLUMN_APPEND - 按列块附加方式粘贴
// 1) 如果nLine和nChar都等于-1，从当前位置粘贴
// 2) 如果当前处于自动换行模式则列块粘贴操作将无效
// 3) 如果nType等于PFX_EC_PASTE_NORMAL，但之前进行过列块复制，而当前设
// 置又允许列块粘贴，则仍然按列块粘贴
// 4) 如果nType等于PFX_EC_PASTE_COLUMN_ALIGN或PFX_EC_PASTE_COLUMN_APPEND但当前
// 设置不允许列块粘贴则操作失败
// 5) 当nType等于PFX_EC_PASTE_COLUMN_APPEND时总是在行末附加数据，nChar仅是
// 与nLine一起用来指示是否在当前行粘贴
// 成功返回TRUE，失败返回FALSE
BOOL CPfxEditView::EditPaste( LONG nLine, LONG nChar, LONG nType ) {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  BOOL bCanPaste;
  switch ( nType ) {
  default:
  case PFX_EC_PASTE_NORMAL:
    bCanPaste = IsCanPaste();
    break;
  case PFX_EC_PASTE_COLUMN_ALIGN:
  case PFX_EC_PASTE_COLUMN_APPEND:
    bCanPaste = IsCanColumnPaste();
    break;
  }
  if ( !bCanPaste ) {
    return false;
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_PASTE );
  if ( IsTextSelected() ) {
    SelNoneAndRemoveSelected();
  }
  CPoint pt( nChar, nLine );
  // 从当前位置开始粘贴？
  if ( ( pt.y == PFX_EC_INVALID_LINE ) && ( pt.x == PFX_EC_INVALID_CHAR ) ) {
    pt = m_ptEditPos;
  } else {
    m_pdb->TextAbsLineToSubLine( pt );
    // 检查列是否合法并防止在MBCS字符的中间
    VerifyPointerPosition( pt );
  }
  // 获取剪贴板数据
  CPfxEditViewClipboard clipboard( this, CF_TEXT, true );
  LPCTSTR pData = clipboard.GetClipboardData();
  if ( pData == NULL ) {
    return false;
  }
  LONG cch = _tcslen( pData );
  switch ( nType ) {
  case PFX_EC_PASTE_COLUMN_ALIGN:
    PasteColumnAlign( pt, pData, cch );
    break;
  case PFX_EC_PASTE_COLUMN_APPEND:
    PasteColumnAppend( pt, pData, cch );
    break;
  case PFX_EC_PASTE_NORMAL:
  default:
    // 虽然是正常粘贴，但如果之前使用了列块复制，现在仍然要用列
    // 块粘贴
    if ( !IsCanColumnPaste()
         || !IsDataColumnModeRendered( false, clipboard.GetHandle() ) ) {
      PasteNormal( pt, pData, cch );
    } else {
      PasteColumnAlign( pt, pData, cch );
    }
    break;
  }
  return true;
}
// 删除指定范围内的内容
// 1）如果nFromLine和nToLine都等于-1，删除当前字符或已选择内容
// 2）如果nFromLine等于0而nToLine等于-1，清空文档
// 3）如果想删除一行，应该从某行的第一列删除到下一行的第一列
// *** 所有参数都是对物理行和列而言的，子行不算是一行 ***
BOOL CPfxEditView::EditRemove( LONG nFromLine, LONG nFromChar, LONG nToLine, LONG nToChar ) {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  // 删除当前字符或已选择内容？
  if ( ( nFromLine == PFX_EC_INVALID_LINE ) && ( nToLine == PFX_EC_INVALID_LINE ) ) {
    OnKeyDelete( false );
    return true;
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_REMOVE );
  if ( IsTextSelected() ) {
    SelNone();
  }
  ASSERT( !IsTextSelected() );
  CPoint ptFrom( nFromChar, nFromLine );
  CPoint ptTo( nToChar, nToLine );
  // 清空文档？
  if ( ( ptFrom.y == 0 ) && ( ptTo.y == PFX_EC_INVALID_LINE ) ) {
    ptFrom.x = 0;
    ptFrom.y = 0;
    ptTo.y = m_pdb->TextGetLineUpperBound();
    ptTo.x = m_pdb->TextGetLineLength( ptTo.y );
  } else // 否则，删除文档部分字符
  {
    m_pdb->TextAbsLineToSubLine( ptFrom );
    VerifyPointerPosition( ptFrom ); // 防止指针在MBCS字符的中间
    m_pdb->TextAbsLineToSubLine( ptTo );
    VerifyPointerPosition( ptTo ); // 防止指针在MBCS字符的中间
  }
  if ( ( ptFrom.y == ptTo.y ) && ( ptFrom.x > ptTo.x ) ) {
    LONG i = ptFrom.x;
    ptFrom.x = ptTo.x;
    ptTo.x = i;
  }
  CharsRemove( ptFrom, ptTo, true );
  ASSERT( m_pdb->TextIsValidLine( m_ptEditPos.y ) );
  ASSERT( m_pdb->TextIsValidChar( m_ptEditPos.y, m_ptEditPos.x ) );
  return true;
}
// 在指定位置插入字符（串）
// LONG nLine, LONG nChar - 要插入字符的位置
// LPCTSTR pData          - 要插入的字符的缓冲区指针
// LONG cch               - 字符长度单位：TCHAR
// 如果nLine和nChar都等于-1，从当前位置插入
// 所有参数都是对物理行和列而言的，子行不算是一行
BOOL CPfxEditView::EditInsert( LONG nLine, LONG nChar, LPCTSTR pData, LONG cch ) {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_INSERT );
  if ( IsTextSelected() ) {
    SelNoneAndRemoveSelected();
  }
  CPoint pt( nChar, nLine );
  // 从当前位置插入？
  if ( ( pt.x == PFX_EC_INVALID_CHAR ) && ( pt.y == PFX_EC_INVALID_LINE ) ) {
    pt = m_ptEditPos;
  } else {
    m_pdb->TextAbsLineToSubLine( pt );
    VerifyPointerPosition( pt ); // 防止指针在MBCS字符的中间
  }
  CharsInsert( pt, pData, cch, true );
  ASSERT( m_pdb->TextIsValidLine( m_ptEditPos.y ) );
  ASSERT( m_pdb->TextIsValidChar( m_ptEditPos.y, m_ptEditPos.x ) );
  return true;
}
// 选择指定范围内的内容
// 1）如果nFromLine和nToLine都等于-1，取消任何已有选择
// 2）如果nFromLine等于0而nToLine等于-1，全选
// 3）如果想选择一整行包括换行符，应从某行的第一列选择到下一行的第一列
// 所有参数都是对物理行和列而言的，子行不算是一行
BOOL CPfxEditView::EditSelect( LONG nFromLine, LONG nFromChar, LONG nToLine, LONG nToChar ) {
  ASSERT( m_pdb->TextGetLineCount() > 0 );
  CPoint ptFrom( nFromChar, nFromLine );
  CPoint ptTo( nToChar, nToLine );
  // 取消选择？
  if ( ( ptFrom.y == PFX_EC_INVALID_LINE ) && ( ptTo.y == PFX_EC_INVALID_LINE ) ) {
    SelNone();
    return true;
  }
  // 全选？
  if ( ( ptFrom.y == 0 ) && ( ptTo.y == PFX_EC_INVALID_LINE ) ) {
    ptFrom.x = 0;
    ptTo.y = m_pdb->TextGetLineUpperBound();
    ptTo.x = m_pdb->TextGetLineLength( ptTo.y );
  } else {
    // 绝对位置转换成相对位置并校验参数的正确性
    m_pdb->TextAbsLineToSubLine( ptFrom );
    VerifyPointerPosition( ptFrom ); // 防止指针在MBCS字符的中间
    m_pdb->TextAbsLineToSubLine( ptTo );
    VerifyPointerPosition( ptTo ); // 防止指针在MBCS字符的中间
  }
  CPoint cpOld = m_ptEditPos;
  m_ptEditPos = ptTo;
  SelRange( ptFrom );
  // 旧的当前位置被改变之前没有更新，所以要更新这一行
  UpdateLines( cpOld.y, cpOld.y );
  EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  return true;
}
// 删除行末空白字符
BOOL CPfxEditView::EditTrimTrailingSpaces() {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  if ( IsTextSelected() ) {
    SelNone();
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_TRIM_TRAILING_SPACES );
  // 修改前先保存当前的编辑位置以便修改后能够恢复
  CPoint cpOld = m_ptEditPos;
  m_pdb->TextSubLineToAbsLine( cpOld );
  BOOL bNeedUpdate = false;
  for ( LONG nLine = 0; nLine != PFX_EC_INVALID_LINE; nLine = m_pdb->TextGetNextMainLine( nLine ) ) {
    CPfxEditLineInfo * pli = m_pdb->TextGetLineAt( nLine );
    if ( PFX_EC_IS_EMPTY_LINE( pli ) ) {
      continue;
    }
    const LPCTSTR pStart = pli->pData;
    const LPCTSTR pEnd = pStart + pli->nActLen;
    LPCTSTR pCur = pEnd - 1; // 指向最后一个字符
    // 目前所有的MBCS字符集中没有任何一个MBCS字符的尾字节落入
    // _istspace()所表示的空白字符范围内
    while ( ( pCur >= pStart ) && IsWhitespace( *pCur ) ) {
      pCur--;
    }
    if ( ++pCur >= pEnd )           //      行末没有有空白字符！
    {
      continue;
    }
    CPoint ptFrom( ( pCur - pStart ), nLine );
    CPoint ptTo( ( pEnd - pStart ), nLine );
    m_pdb->TextMainLineToSubLine( ptFrom );
    m_pdb->TextMainLineToSubLine( ptTo );
    // VIEW指针为NULL表示修改数据后不需要更新，以便加快速度
    CharsRemove( ptFrom, ptTo, false );
    bNeedUpdate = true;
  }
  // 由于上面改变了缓冲区的数据，很多参数未经UpdateAllViews()和
  // EnsureVisible()修正之前可能无效，所以要小心调用函数
  if ( bNeedUpdate ) {
    m_ptEditPos = cpOld;
    m_pdb->TextAbsLineToSubLine( m_ptEditPos );
    VerifyCurrentPosition();
    UpdateAllViews( PFX_EC_UT_VERT, 0, -1 );
    EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  }
  return true;
}
// 空格字符转换成制表符
// BOOL bLeading - 是否只将行首的空格转换而忽略其它的
// 成果返回TRUE，失败返回FALSE
// 找到一个空格，假设它是制表符，则将它扩展成空格时应该用若干个空格，如
// 果该从这个空格开始后面有连续的足够数目的空格，则条件满足，可以转换成
// 制表符，否则将忽略这个空格
BOOL CPfxEditView::EditSpacesToTabs( BOOL bLeading ) {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  if ( IsTextSelected() ) {
    SelNone();
  }
  CPfxEditViewUndoManager um( this, ( bLeading ? PFX_EC_UNDO_SPACES_TO_TABS_LEADING : PFX_EC_UNDO_SPACES_TO_TABS ) );
  // 获取当前编辑位置的绝对坐标，以便在修改后能够恢复当前编辑位置
  CPoint cpOld = m_ptEditPos;
  m_pdb->TextSubLineToAbsLine( cpOld );
  BOOL bNeedUpdate = false;
  for ( LONG nLine = 0; nLine != PFX_EC_INVALID_LINE; nLine = m_pdb->TextGetNextMainLine( nLine ) ) {
    const CPfxEditLineInfo * pli = m_pdb->TextGetLineAt( nLine );
    if ( PFX_EC_IS_EMPTY_LINE( pli ) ) {
      continue;
    }
    // 开始扫描一行
    LPCTSTR pStart = pli->pData;
    LPCTSTR pEnd = pStart + pli->nActLen;
    LPCTSTR pCur = pStart;
    LONG nTabSize = GetTabSize();
    LONG nLogChar = 0;
    // 目前所有MBCS字符集中没有任何一个MBCS字符的前导字节或尾字
    // 节包含空格，所以可以使读写指针简单递增
    while ( pCur < pEnd ) {
      // 如果它是空格，想象把它当成制表符，则需要多少个空
      // 格才能代替这个制表符？
      if ( PFX_EC_CHR_SPACE != *pCur ) {
        if ( PFX_EC_CHR_TAB != *pCur ) {
          //   遇到第一非空白字符！这一行的扫描
          //   将停止
          if ( bLeading ) {
            break;
          }
          nLogChar++;
        } else {
          nLogChar += nTabSize - ( nLogChar % nTabSize );
        }
        pCur++;
        continue;
      }
      // 下面计算如果它是制表符，将会扩展成多少个空格
      LONG nCurTabSize = nTabSize - ( nLogChar % nTabSize );
      // 接下来的字符中包含有足够多的空格吗？
      LONG nSpaceCount = nCurTabSize - 1;
      // 加一是为了跳过已经发现的空格
      LPCTSTR pTmp = pCur + 1;
      while ( ( pTmp < pEnd )
              && ( nSpaceCount > 0 )
              && ( PFX_EC_CHR_SPACE == *pTmp ) ) {
        pTmp++;
        nSpaceCount--;
      }
      // YES，条件满足，可以将这nCurTabSize个空格转换成一
      // 个制表符
      if ( nSpaceCount != 0 ) {
        nLogChar += ( pTmp - pCur );
        pCur = pTmp;
        continue;
      }
      // 替换后新的开始扫描位置
      LONG nNewScanOffs = pCur - pStart + 1;
      // 将这nCurTabSize个空格用一个制表符替换掉！
      CPoint ptFrom( ( pCur - pStart ), nLine );
      m_pdb->TextMainLineToSubLine( ptFrom );
      CPoint ptTo( ( pTmp - pStart ), nLine );
      m_pdb->TextMainLineToSubLine( ptTo );
      TCHAR szTAB[ 1 ];
      szTAB[ 0 ] = PFX_EC_CHR_TAB;
      CharsReplace( ptFrom, ptTo, szTAB, 1, false );
      // 完成数据修改以后需要更新指针
      pli = m_pdb->TextGetLineAt( nLine );
      pStart = pli->pData;
      pEnd = pStart + pli->nActLen;
      pCur = pStart + nNewScanOffs;
      nLogChar += nCurTabSize;
      bNeedUpdate = true;
    }
  }
  if ( bNeedUpdate ) {
    // 如果当前编辑位置位于被替换的nCurTabSize个连续的空格中，则
    // 要同步更新当前编辑位置
    m_ptEditPos.y = cpOld.y;
    m_ptEditPos.x = CalcAbsCharPosFromLogCharPos( cpOld.y, m_ptLogiPos.x );
    m_pdb->TextAbsLineToSubLine( m_ptEditPos );
    VerifyCurrentPosition();
    UpdateAllViews( PFX_EC_UT_VERT, 0, -1 );
    EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  }
  return true;
}
// 制表符转换成空格字符
BOOL CPfxEditView::EditTabsToSpaces() {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  if ( IsTextSelected() ) {
    SelNone();
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_TABS_TO_SPACES );
  // 获取当前编辑位置的绝对坐标，以便在修改后能够恢复当前编辑位置
  CPoint cpOld = m_ptEditPos;
  m_pdb->TextSubLineToAbsLine( cpOld );
  BOOL bNeedUpdate = false;
  for ( LONG nLine = 0; nLine != PFX_EC_INVALID_LINE; nLine = m_pdb->TextGetNextMainLine( nLine ) ) {
    const CPfxEditLineInfo * pli = m_pdb->TextGetLineAt( nLine );
    if ( PFX_EC_IS_EMPTY_LINE( pli ) ) {
      continue;
    }
    // 开始扫描一行
    LPCTSTR pStart = pli->pData;
    LPCTSTR pEnd = pStart + pli->nActLen;
    LPCTSTR pCur = pStart;
    LONG nTabSize = GetTabSize();
    LONG nLogChar = 0;
    // 目前所有MBCS字符集中没有任何一个MBCS字符的前导字节或尾字
    // 节包含空格，所以可以使读写指针简单递增
    while ( pCur < pEnd ) {
      if ( PFX_EC_CHR_TAB != *pCur ) {
        pCur++;
        nLogChar++;
        continue;
      }
      // 下面计算将会扩展成多少个空格
      LONG nCurTabSize = nTabSize - ( nLogChar % nTabSize );
      // 替换后新的开始扫描位置
      LONG nNewScanOffs = pCur - pStart + nCurTabSize;
      // 将这nCurTabSize个空格用一个制表符替换掉！
      CPoint ptFrom( ( pCur - pStart ), nLine );
      m_pdb->TextMainLineToSubLine( ptFrom );
      CPoint ptTo( ( ptFrom.x + 1 ), ptFrom.y );
      TCHAR sz[ PFX_EC_MAX_TAB_SIZE ];
      for ( LONG i = 0; i < nCurTabSize; i++ ) {
        sz[ i ] = PFX_EC_CHR_SPACE;
      }
      CharsReplace( ptFrom, ptTo, sz, nCurTabSize, false );
      // 完成数据修改以后需要更新指针
      pli = m_pdb->TextGetLineAt( nLine );
      pStart = pli->pData;
      pEnd = pStart + pli->nActLen;
      pCur = pStart + nNewScanOffs;
      nLogChar += nCurTabSize;
      bNeedUpdate = true;
    }
  }
  if ( bNeedUpdate ) {
    // 如果当前编辑位置位于被替换的nCurTabSize个连续的空格中，则
    // 要同步更新当前编辑位置
    m_ptEditPos.y = cpOld.y;
    m_ptEditPos.x = CalcAbsCharPosFromLogCharPos( cpOld.y, m_ptLogiPos.x );
    m_pdb->TextAbsLineToSubLine( m_ptEditPos );
    VerifyCurrentPosition();
    UpdateAllViews( PFX_EC_UT_VERT, 0, -1 );
    EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  }
  return true;
}
// 将指定缓冲区范围内的字符转换成小写或大写
// LONG nLine  - 相对性的主行
// LONG nStart - 这行中的开始转换位置
// LONG nEnd   - 终止转换的字符位置
// BOOL bLower - 小写还是大写
// 如果执行了转换返回TRUE，否则返回FALSE
BOOL CPfxEditView::CaseChange( LONG nLine, LONG nStart, LONG nEnd, BOOL bLower ) {
  if ( !m_pdb->TextIsValidLine( nLine ) ) {
    return false;
  }
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( nLine );
  nStart = min( nStart, nEnd );
  nEnd = max( nEnd, nStart );
  nStart = min( nStart, pli->nActLen );
  nEnd = min( nEnd, pli->nActLen );
  // 空行或零长度？
  if ( PFX_EC_IS_EMPTY_LINE( pli ) || ( nStart == nEnd ) ) {
    return false;
  }
  // 记住物理位置
  CPoint ptAbs( 0, nLine );
  m_pdb->TextMainLineToAbsLine( ptAbs );
  WORD wCaseType = ( WORD ) ( ( bLower ? C1_LOWER : C1_UPPER ) );
  BOOL bModified = false;
  LPCTSTR pStart = pli->pData;
  LPCTSTR pEnd = pStart + nEnd;
  LPCTSTR pCur = pStart + nStart;
  // 开始扫描，下面的方法能够识别MBCS字符的大写和小写，而不仅仅是ANSI
  // 的大小写
  while ( pCur < pEnd ) {
    LPCTSTR pTmp = pCur;
    pCur += ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
    WORD wCharType[ PFX_EC_MAX_MB_LEN ];
    ::GetStringTypeEx( LOCALE_USER_DEFAULT, CT_CTYPE1, pTmp, min( pCur, pEnd ) - pTmp, wCharType );
    if ( ( ( wCharType[ 0 ] & C1_ALPHA ) == 0 )
         || ( ( wCharType[ 0 ] & ( C1_UPPER | C1_LOWER ) ) == 0 )
         || ( ( wCharType[ 0 ] & wCaseType ) != 0 ) ) {
      continue;
    }
    // 临时缓冲区，每组最大处理PFX_EC_SIZEOF(sz)个字符
    TCHAR sz[ 256 ];
    LPCTSTR pMax = min( ( pTmp + PFX_EC_SIZEOF( sz ) ), pEnd );
    while ( pCur < pMax ) {
      LPCTSTR pT = pCur + ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
      ::GetStringTypeEx( LOCALE_USER_DEFAULT, CT_CTYPE1, pCur, min( pT, pEnd ) - pCur, wCharType );
      if ( ( ( wCharType[ 0 ] & C1_ALPHA ) == 0 )
           || ( ( wCharType[ 0 ] & ( C1_UPPER | C1_LOWER ) ) == 0 )
           || ( ( wCharType[ 0 ] & wCaseType ) != 0 ) ) {
        break;
      }
      pCur = pT;
    }
    // 执行转换操作，将一（组）大写字符转换成小写
    pCur = min( pCur, pEnd );
    LONG cch = pCur - pTmp;
    memcpy( sz, pTmp, ( cch * sizeof( TCHAR ) ) );
    if ( bLower ) {
      ::CharLowerBuff( sz, cch );
    } else {
      ::CharUpperBuff( sz, cch );
    }
    // 替换后新的开始扫描位置
    LONG nResumePosCur = pCur - pStart;
    // 将转换好的字符替换掉原来的字符
    CPoint ptFrom( ( pTmp - pStart ), nLine );
    m_pdb->TextMainLineToSubLine( ptFrom );
    CPoint ptTo( ( pCur - pStart ), nLine );
    m_pdb->TextMainLineToSubLine( ptTo );
    CharsReplace( ptFrom, ptTo, sz, cch, false );
    // 修改以后相对位置可能会改变，因此要使用绝对位置来映射相对
    // 位置
    CPoint pt( 0, ptAbs.y );
    m_pdb->TextAbsLineToMainLine( pt );
    nLine = pt.y;
    // 完成数据修改以后需要更新指针
    pli = m_pdb->TextGetLineAt( nLine );
    pStart = pli->pData;
    pEnd = pStart + nEnd;
    pCur = pStart + nResumePosCur;
    bModified = true;
  }
  return bModified;
}
BOOL CPfxEditView::CaseInvert( LONG nLine, LONG nStart, LONG nEnd ) {
  if ( !m_pdb->TextIsValidLine( nLine ) ) {
    return false;
  }
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( nLine );
  nStart = min( nStart, nEnd );
  nEnd = max( nEnd, nStart );
  nStart = min( nStart, pli->nActLen );
  nEnd = min( nEnd, pli->nActLen );
  // 空行或零长度？
  if ( PFX_EC_IS_EMPTY_LINE( pli ) || ( nStart == nEnd ) ) {
    return false;
  }
  // 记住物理位置
  CPoint ptAbs( 0, nLine );
  m_pdb->TextMainLineToAbsLine( ptAbs );
  BOOL bModified = false;
  LPCTSTR pStart = pli->pData;
  LPCTSTR pEnd = pStart + nEnd;
  LPCTSTR pCur = pStart + nStart;
  // 开始扫描
  while ( pCur < pEnd ) {
    LPCTSTR pTmp = pCur;
    pCur += ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
    WORD wCharType[ PFX_EC_MAX_MB_LEN ];
    ::GetStringTypeEx( LOCALE_USER_DEFAULT, CT_CTYPE1, pTmp, min( pCur, pEnd ) - pTmp, wCharType );
    if ( ( wCharType[ 0 ] & C1_ALPHA ) == 0 ) {
      continue;
    }
    WORD wCaseType = ( WORD ) ( ( wCharType[ 0 ] & ( C1_LOWER | C1_UPPER ) ) );
    if ( wCaseType == 0 ) {
      continue;
    }
    // 临时缓冲区，每组最大处理PFX_EC_SIZEOF(sz)个字符
    TCHAR sz[ 256 ];
    LPCTSTR pMax = min( ( pTmp + PFX_EC_SIZEOF( sz ) ), pEnd );
    while ( pCur < pMax ) {
      LPCTSTR pT = pCur + ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
      ::GetStringTypeEx( LOCALE_USER_DEFAULT, CT_CTYPE1, pCur, min( pT, pEnd ) - pCur, wCharType );
      if ( ( wCharType[ 0 ] & C1_ALPHA ) == 0 ) {
        break;
      }
      if ( ( wCharType[ 0 ] & ( C1_LOWER | C1_UPPER ) ) != wCaseType ) {
        break;
      }
      pCur = pT;
    }
    // 执行转换操作，将一（组）大写字符转换成小写
    pCur = min( pCur, pEnd );
    LONG cch = pCur - pTmp;
    memcpy( sz, pTmp, ( cch * sizeof( TCHAR ) ) );
    if ( wCaseType & C1_UPPER )           //      反转大小写
    {
      ::CharLowerBuff( sz, cch );
    } else {
      ::CharUpperBuff( sz, cch );
    }
    // 替换后新的开始扫描位置
    LONG nResumePosCur = pCur - pStart;
    // 将转换好的字符替换掉原来的字符
    CPoint ptFrom( ( pTmp - pStart ), nLine );
    m_pdb->TextMainLineToSubLine( ptFrom );
    CPoint ptTo( ( pCur - pStart ), nLine );
    m_pdb->TextMainLineToSubLine( ptTo );
    CharsReplace( ptFrom, ptTo, sz, cch, false );
    // 修改以后相对位置可能会改变，因此要使用绝对位置来映射相对
    // 位置
    CPoint pt( 0, ptAbs.y );
    m_pdb->TextAbsLineToMainLine( ptAbs );
    nLine = pt.y;
    // 完成数据修改以后需要更新指针
    pli = m_pdb->TextGetLineAt( nLine );
    pStart = pli->pData;
    pEnd = pStart + nEnd;
    pCur = pStart + nResumePosCur;
    bModified = true;
  }
  return bModified;
}
BOOL CPfxEditView::CaseCapitalize( LONG nLine, LONG nStart, LONG nEnd ) {
  if ( !m_pdb->TextIsValidLine( nLine ) ) {
    return false;
  }
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( nLine );
  nStart = min( nStart, nEnd );
  nEnd = max( nEnd, nStart );
  nStart = min( nStart, pli->nActLen );
  nEnd = min( nEnd, pli->nActLen );
  // 空行或零长度？
  if ( PFX_EC_IS_EMPTY_LINE( pli ) || ( nStart == nEnd ) ) {
    return false;
  }
  // 记住物理位置
  CPoint ptAbs( 0, nLine );
  m_pdb->TextMainLineToAbsLine( ptAbs );
  BOOL bModified = false;
  LPCTSTR pStart = pli->pData;
  LPCTSTR pEnd = pStart + nEnd;
  LPCTSTR pCur = pStart + nStart;
  // 开始扫描
  while ( pCur < pEnd ) {
    LPCTSTR pTmp = pCur;
    pCur += ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
    // 查找单词的第一个字符
    if ( IsWhitespace( *pTmp ) || IsDelimiter( pTmp, pEnd ) ) {
      continue;
    }
    WORD wCharType[ PFX_EC_MAX_MB_LEN ];
    // 即使不是空白字符或已知的单词定界符，但它也有可能是某些标
    // 点符号，例如MBCS字符中的全角标点等，第一个字符只有是字母
    // （不管全角或半角），才算符合要求
    pCur = min( pCur, pEnd );
    LONG cch = pCur - pTmp;
    ::GetStringTypeEx( LOCALE_USER_DEFAULT, CT_CTYPE1, pTmp, cch, wCharType );
    WORD wCharInfo = wCharType[ 0 ];
    if ( ( ( wCharInfo & C1_ALPHA ) == 0 )
         || ( wCharInfo & ( C1_UPPER | C1_LOWER ) ) == 0 ) {
      continue;
    }
    // 临时缓冲区，每组最大处理PFX_EC_SIZEOF(sz)个字符
    CPoint ptFrom;
    CPoint ptTo;
    TCHAR sz[ 256 ];
    // 处理第一个字符
    if ( ( wCharInfo & C1_LOWER ) != 0 ) {
      memcpy( sz, pTmp, ( cch * sizeof( TCHAR ) ) );
      ::CharUpperBuff( sz, cch );
      ptFrom.y = nLine;
      ptFrom.x = pTmp - pStart;
      m_pdb->TextMainLineToSubLine( ptFrom );
      ptTo.y = nLine;
      ptTo.x = pCur - pStart;
      m_pdb->TextMainLineToSubLine( ptTo );
      CharsReplace( ptFrom, ptTo, sz, cch, false );
      LONG nResumePosCur = pCur - pStart;
      LONG nResumePosTmp = pTmp - pStart;
      // 可能改变了缓冲区，所以一定要更新指针
      pli = m_pdb->TextGetLineAt( nLine );
      pStart = pli->pData;
      pEnd = pStart + nEnd;
      pCur = pStart + nResumePosCur;
      pTmp = pStart + nResumePosTmp;
      bModified = true;
    }
    // 处理后面的字符
    while ( ( pCur < pEnd )
            && !IsWhitespace( *pCur )
            && !IsDelimiter( pCur, pEnd ) ) {
      pTmp = pCur;
      pCur += ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
      ::GetStringTypeEx( LOCALE_USER_DEFAULT, CT_CTYPE1, pTmp, ( min( pCur, pEnd ) - pTmp ), wCharType );
      wCharInfo = wCharType[ 0 ];
      // 遇到不是字母的字符就停下来
      if ( ( ( wCharInfo & C1_ALPHA ) == 0 )
           || ( wCharInfo & ( C1_UPPER | C1_LOWER ) ) == 0 ) {
        break;
      }
      // 忽略小写字符
      if ( ( wCharInfo & C1_LOWER ) != 0 ) {
        continue;
      }
      bModified = true;
      // 试图最大程度地处理一片大写字符，以减少UNDO记录数
ScanNextBlock:
      LPCTSTR pMax = min( ( pTmp + PFX_EC_SIZEOF( sz ) ), pEnd );
      while ( ( pCur < pMax )
              && !IsWhitespace( *pCur )
              && !IsDelimiter( pCur, pMax ) ) {
        cch = ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
        ::GetStringTypeEx( LOCALE_USER_DEFAULT, CT_CTYPE1, pCur, min( ( pMax - pCur ), cch ), wCharType );
        wCharInfo = wCharType[ 0 ];
        //  遇到不是字母的字符就停下来
        if ( ( ( wCharInfo & C1_ALPHA ) == 0 )
             || ( wCharInfo & ( C1_UPPER | C1_LOWER ) ) == 0 ) {
          break;
        }
        //  如果遇到小写就停止
        if ( ( wCharInfo & C1_LOWER ) != 0 ) {
          break;
        }
        pCur += cch;
      }
      pCur = min( pCur, pEnd );
      if ( pCur == pTmp ) {
        continue;
      }
      cch = pCur - pTmp;
      memcpy( sz, pTmp, ( cch * sizeof( TCHAR ) ) );
      ::CharLowerBuff( sz, cch );
      ptFrom.y = nLine;
      ptFrom.x = pTmp - pStart;
      m_pdb->TextMainLineToSubLine( ptFrom );
      ptTo.y = nLine;
      ptTo.x = pCur - pStart;
      m_pdb->TextMainLineToSubLine( ptTo );
      CharsReplace( ptFrom, ptTo, sz, cch, false );
      LONG nResumePosCur = pCur - pStart;
      LONG nResumePosTmp = pTmp - pStart;
      // 修改以后相对位置可能会改变，因此要使用绝对位置来
      // 映射相对位置
      CPoint pt( 0, ptAbs.y );
      m_pdb->TextAbsLineToMainLine( pt );
      nLine = pt.y;
      // 可能改变了缓冲区，所以一定要更新指针
      pli = m_pdb->TextGetLineAt( nLine );
      pStart = pli->pData;
      pEnd = pStart + nEnd;
      pCur = pStart + nResumePosCur;
      pMax = pStart + nResumePosTmp + PFX_EC_SIZEOF( sz );
      if ( ( pCur >= pMax ) && ( pCur < pEnd ) ) {
        pTmp = pCur;
        goto ScanNextBlock;
      }
    }
  }
  return bModified;
}
// BOOL EditCaseChange(BOOL bLower)
// 将指定范围内的字符转换成小写或大写
// BOOL bLower - 转换成小写或大写
// 如果执行了转换返回TRUE，否则返回FALSE
// 如果当前有选择，则转换将在选择范围内，否则将执行全文转换
BOOL CPfxEditView::EditCaseChange( BOOL bLower ) {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  CPfxEditViewUndoManager um( this, ( bLower ? PFX_EC_UNDO_CASE_LOWER : PFX_EC_UNDO_CASE_UPPER ) );
  // 由于改变大小写并不会改变数据量，因此不需要保存当前编辑位置
  BOOL bModified = false;
  BOOL bSelected = IsTextSelected();
  BOOL bColSelected = IsColumnSelect();
  const LONG nSelLineFrom = m_nSelLineFrom;
  const LONG nSelLineTo = m_nSelLineTo;
  // 首先取消选择
  if ( bSelected ) {
    SelNone();
  }
  if ( !bSelected ) {
    for ( LONG nLine = 0; nLine != PFX_EC_INVALID_LINE; nLine = m_pdb->TextGetNextMainLine( nLine ) ) {
      const CPfxEditLineInfo * pli = m_pdb->TextGetLineAt( nLine );
      if ( CaseChange( nLine, 0, pli->nActLen, bLower ) ) {
        bModified = true;
      }
    }
  } else if ( bColSelected )           //      列块选择模式下不会处于自动换行状态
  {
    for ( LONG nLine = nSelLineFrom; nLine <= nSelLineTo; nLine++ ) {
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( nLine );
      if ( CaseChange( nLine, pli->nSelFrom, pli->nSelTo, bLower ) ) {
        bModified = true;
      }
    }
  }
  else // 正常选择模式
  {
    // 获取选择内容起始和终止坐标
    const CPfxEditLineInfo* pliFrom = m_pdb->TextGetLineAt( nSelLineFrom );
    CPoint ptFrom( min( pliFrom->nSelFrom, pliFrom->nLen ), nSelLineFrom );
    const CPfxEditLineInfo* pliTo = m_pdb->TextGetLineAt( nSelLineTo );
    CPoint ptTo( min( pliTo->nSelTo, pliTo->nLen ), nSelLineTo );
    // 转换成物理位置
    m_pdb->TextSubLineToAbsLine( ptFrom );
    m_pdb->TextSubLineToAbsLine( ptTo );
    for ( LONG nPosStart = ptFrom.x; ptFrom.y <= ptTo.y; ptFrom.y++ ) {
      // 物理行位置转换成相对位置
      CPoint pt( 0, ptFrom.y );
      m_pdb->TextAbsLineToMainLine( pt );
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
      if ( CaseChange( pt.y, nPosStart, ( ( ptFrom.y < ptTo.y ) ? pli->nActLen : ptTo.x ), bLower ) ) {
        bModified = true;
      }
      nPosStart = 0;
    }
  }
  if ( bModified ) {
    VerifyCurrentPosition();
    UpdateAllViews( PFX_EC_UT_VERT, 0, -1 );
    EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  }
  return bModified;
}
// 将选择范围内的字符反转大小写
BOOL CPfxEditView::EditCaseInvert() {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_CASE_INVERT );
  // 由于改变大小写并不会改变数据量，因此不需要保存当前编辑位置
  BOOL bModified = false;
  BOOL bSelected = IsTextSelected();
  BOOL bColSelected = IsColumnSelect();
  const LONG nSelLineFrom = m_nSelLineFrom;
  const LONG nSelLineTo = m_nSelLineTo;
  // 首先取消选择
  if ( bSelected ) {
    SelNone();
  }
  if ( !bSelected ) {
    for ( LONG nLine = 0; nLine != PFX_EC_INVALID_LINE; nLine = m_pdb->TextGetNextMainLine( nLine ) ) {
      const CPfxEditLineInfo * pli = m_pdb->TextGetLineAt( nLine );
      if ( CaseInvert( nLine, 0, pli->nActLen ) ) {
        bModified = true;
      }
    }
  } else if ( bColSelected )           //      列块选择模式下不会处于自动换行状态
  {
    for ( LONG nLine = nSelLineFrom; nLine <= nSelLineTo; nLine++ ) {
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( nLine );
      if ( CaseInvert( nLine, pli->nSelFrom, pli->nSelTo ) ) {
        bModified = true;
      }
    }
  }
  else // 正常选择模式
  {
    // 获取选择内容起始和终止坐标
    const CPfxEditLineInfo* pliFrom = m_pdb->TextGetLineAt( nSelLineFrom );
    CPoint ptFrom( ( min( pliFrom->nSelFrom, pliFrom->nLen ) ), nSelLineFrom );
    const CPfxEditLineInfo* pliTo = m_pdb->TextGetLineAt( nSelLineTo );
    CPoint ptTo( min( pliTo->nSelTo, pliTo->nLen ), nSelLineTo );
    // 转换成物理位置
    m_pdb->TextSubLineToAbsLine( ptFrom );
    m_pdb->TextSubLineToAbsLine( ptTo );
    for ( LONG nPosStart = ptFrom.x; ptFrom.y <= ptTo.y; ptFrom.y++ ) {
      // 物理行位置转换成相对行位置
      CPoint pt( 0, ptFrom.y );
      m_pdb->TextAbsLineToMainLine( pt );
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
      if ( CaseInvert( pt.y, nPosStart, ( ( ptFrom.y < ptTo.y ) ? pli->nActLen : ptTo.x ) ) ) {
        bModified = true;
      }
      nPosStart = 0;
    }
  }
  if ( bModified ) {
    VerifyCurrentPosition();
    UpdateAllViews( PFX_EC_UT_VERT, 0, -1 );
    EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  }
  return bModified;
}
// 将选择范围内的句子首字母大写
BOOL CPfxEditView::EditCaseCapitalize() {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_CASE_CAPITALIZE );
  // 由于改变大小写并不会改变数据量，因此不需要保存当前编辑位置
  BOOL bModified = false;
  BOOL bSelected = IsTextSelected();
  BOOL bColSelected = IsColumnSelect();
  const LONG nSelLineFrom = m_nSelLineFrom;
  const LONG nSelLineTo = m_nSelLineTo;
  // 首先取消选择
  if ( bSelected ) {
    SelNone();
  }
  if ( !bSelected ) {
    for ( LONG nLine = 0; nLine != PFX_EC_INVALID_LINE; nLine = m_pdb->TextGetNextMainLine( nLine ) ) {
      const CPfxEditLineInfo * pli = m_pdb->TextGetLineAt( nLine );
      if ( CaseCapitalize( nLine, 0, pli->nActLen ) ) {
        bModified = true;
      }
    }
  } else if ( bColSelected )           //      列块选择模式下不会处于自动换行状态
  {
    for ( LONG nLine = nSelLineFrom; nLine <= nSelLineTo; nLine++ ) {
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( nLine );
      if ( CaseCapitalize( nLine, pli->nSelFrom, pli->nSelTo ) ) {
        bModified = true;
      }
    }
  }
  else // 正常选择模式
  {
    // 获取选择内容起始和终止坐标
    const CPfxEditLineInfo* pliFrom = m_pdb->TextGetLineAt( nSelLineFrom );
    CPoint ptFrom( min( pliFrom->nSelFrom, pliFrom->nLen ), nSelLineFrom );
    const CPfxEditLineInfo* pliTo = m_pdb->TextGetLineAt( nSelLineTo );
    CPoint ptTo( min( pliTo->nSelTo, pliTo->nLen ), nSelLineTo );
    // 转换成物理位置
    m_pdb->TextSubLineToAbsLine( ptFrom );
    m_pdb->TextSubLineToAbsLine( ptTo );
    for ( LONG nPosStart = ptFrom.x; ptFrom.y <= ptTo.y; ptFrom.y++ ) {
      // 物理行位置转换成相对行位置
      CPoint pt( 0, ptFrom.y );
      m_pdb->TextAbsLineToMainLine( pt );
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
      if ( CaseCapitalize( pt.y, nPosStart, ( ( ptFrom.y < ptTo.y ) ? pli->nActLen : ptTo.x ) ) ) {
        bModified = true;
      }
      nPosStart = 0;
    }
  }
  if ( bModified ) {
    VerifyCurrentPosition();
    UpdateAllViews( PFX_EC_UT_VERT, 0, -1 );
    EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  }
  return bModified;
}
// BOOL EditFind(CPoint& rptFrom,CPoint& rptTo,LPCTSTR pszFindWhat,LONG cchFindWhat,BOOL bMatchCase,BOOL bMatchWholeWord,BOOL bRegExp,BOOL bDirectDown,BOOL bMarkAll,BOOL bWrapAtEndOfFile)
// 执行查找操作
// CPoint& rptFrom       - [IN/OUT]范围起点
// CPoint& rptTo         - [OUT]范围终点
// LPCTSTR pszFindWhat   - 查找对象，长度由cchFindWhat给出
// LONG cchFindWhat      - 查找对象的长度，如果等于-1表示由函数自动计算长度，这时应以NULL结尾
// BOOL bMatchCase       - 是否匹配大小写
// BOOL bMatchWholeWord  - 是否单词匹配
// BOOL bRegExp          - 是否使用正则表达式（这时忽略单词匹配）
// BOOL bDirectDown      - 是否向下查找
// BOOL bMarkAll         - 是否用书签标记找到的对象所在行
// BOOL bWrapAtEndOfFile - 是否根据查找方向而忽略起点或终点并在文件尽头折回
// 找到返回TRUE，否则返回FALSE
BOOL CPfxEditView::EditFind( CPoint& rptFrom, CPoint& rptTo, LPCTSTR pszFindWhat, LONG cchFindWhat, BOOL bMatchCase, BOOL bMatchWholeWord, BOOL bRegExp, BOOL bDirectDown, BOOL bMarkAll, BOOL bWrapAtEndOfFile ) {
  CPfxEditViewFindReplace FindReplace;
  FindReplace.bMatchCase = bMatchCase;
  FindReplace.bMatchWholeWord = bMatchWholeWord;
  FindReplace.bRegExp = bRegExp;
  FindReplace.bDirectDown = bDirectDown;
  FindReplace.bWrapAtEndOfFile = bWrapAtEndOfFile;
  FindReplace.bMarkAll = bMarkAll;
  FindReplace.pszFindWhat = pszFindWhat;
  FindReplace.cchFindWhat = cchFindWhat;
  FindReplace.ptFrom = rptFrom;
  FindReplace.ptTo = rptTo;
  if ( !SearchPreFind( FindReplace ) ) {
    return false;
  }
  if ( FindReplace.bMarkAll ) {
    return SearchFindMarkAll( FindReplace );
  }
  CPoint ptTo;
  CPoint ptFrom;
  if ( FindReplace.bWrapAtEndOfFile ) {
    ptTo = FindReplace.ptFrom;
    if ( FindReplace.bDirectDown ) {
      ptFrom.y = 0;
      ptFrom.x = 0;
    } else {
      ptFrom.y = m_pdb->TextGetLineUpperBound();
      ptFrom.x = m_pdb->TextGetLineLength( ptFrom.y );
    }
    m_pdb->TextSubLineToMainLine( ptFrom );
  }
  BOOL bFound = SearchDoFind( FindReplace );
  if ( !bFound && FindReplace.bWrapAtEndOfFile ) {
    m_pdb->EventSendMsg( PFX_EN_SEARCH_PASSED_EOF, ( FindReplace.bDirectDown ? EOF : ~EOF ) );
    FindReplace.ptFrom = ptFrom;
    FindReplace.ptTo = ptTo;
    bFound = SearchDoFind( FindReplace );
  }
  if ( bFound ) {
    rptFrom = FindReplace.ptFrom;
    m_pdb->TextMainLineToAbsLine( rptFrom );
    rptTo = FindReplace.ptTo;
    m_pdb->TextMainLineToAbsLine( rptTo );
  }
  return bFound;
}
// 执行替换操作
// CPoint& rptFrom,        - [IN/OUT]范围起点，只有当bIgnoreFindWhat为
//                        TRUE时才有效，否则总是全部替换或在选择范围
//                        内替换
// CPoint& rptTo,          - [IN/OUT]范围终点，只有当bIgnoreFindWhat为
//                        TRUE时才有效，否则总是全部替换或在选择范围
//                        内替换
// LPCTSTR pszFindWhat,    - 查找对象，长度由cchFindWhat给出
// LONG cchFindWhat,       - 查找对象的长度，如果等于-1表示由函数自动计
//                        算长度，这时应以NULL结尾
// LPCTSTR pszReplaceWith, - 替换对象，长度由cchReplaceWith给出
// LONG cchReplaceWith,    - 替换对象的长度，如果等于-1表示由函数自动计
//                        算长度，这时应以NULL结尾
// BOOL bMatchCase,        - 是否匹配大小写
// BOOL bMatchWholeWord,   - 是否单词匹配
// BOOL bRegExp,           - 是否使用正则表达式（这时忽略单词匹配）
// BOOL bIgnoreFindWhat,   - 是否忽略查找对象而将指定范围内的文本直接由
//                        替换对象取代，此时起点和终点参数有效，否则
//                        忽略起点或终点
// BOOL bInSelection       - 是否只在选择范围内进行替换并忽略起点和终点，
//                        如果没有选择内容则失败
// 成功替换返回TRUE，否则返回FALSE
BOOL CPfxEditView::EditReplace( CPoint& rptFrom, CPoint& rptTo, LPCTSTR pszFindWhat, LONG cchFindWhat, LPCTSTR pszReplaceWith, LONG cchReplaceWith, BOOL bMatchCase, BOOL bMatchWholeWord, BOOL bRegExp, BOOL bIgnoreFindWhat, BOOL bInSelection ) {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  CPfxEditViewFindReplace FindReplace;
  FindReplace.bMatchCase = bMatchCase;
  FindReplace.bMatchWholeWord = bMatchWholeWord;
  FindReplace.bRegExp = bRegExp;
  FindReplace.bInSelection = bInSelection;
  FindReplace.bIgnoreFindWhat = bIgnoreFindWhat;
  FindReplace.pszFindWhat = pszFindWhat;
  FindReplace.cchFindWhat = cchFindWhat;
  FindReplace.pszReplaceWith = pszReplaceWith;
  FindReplace.cchReplaceWith = cchReplaceWith;
  FindReplace.ptFrom = rptFrom;
  FindReplace.ptTo = rptTo;
  if ( !SearchPreReplace( FindReplace ) ) {
    return false;
  }
  CPfxEditViewUndoManager um( this, ( FindReplace.bIgnoreFindWhat ? PFX_EC_UNDO_REPLACE : PFX_EC_UNDO_REPLACE_ALL ) );
  if ( IsTextSelected() ) {
    SelNone();
  }
  BOOL bFound;
  if ( FindReplace.bIgnoreFindWhat )           //      坐标已经被转换成相对子行位置
  {
    rptFrom = FindReplace.ptFrom;
    CharsReplace( FindReplace.ptFrom, FindReplace.ptTo, FindReplace.pszReplaceWith, FindReplace.cchReplaceWith, true );
    rptTo = FindReplace.ptFrom;
    m_pdb->TextSubLineToAbsLine( rptFrom );
    m_pdb->TextSubLineToAbsLine( rptTo );
    bFound = true;
  } else {
    CWaitCursor waitcursor;
    bFound = SearchDoReplace( FindReplace );
    if ( bFound ) {
      UpdateAllViews( PFX_EC_UT_LAYOUT, 0, 0 );
    }
    EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  }
  return bFound;
}
BOOL CPfxEditView::EditMatchBrace() {
  BOOL bMatch = false;
  CPfxEditViewBraceHint hint;
  if ( MatchBraceGetHint( hint ) ) {
    CPoint pt;
    if ( !hint.m_bDirectDown ) {
      bMatch = MatchBraceSearchUp( hint, pt );
    } else {
      bMatch = MatchBraceSearchDown( hint, pt );
    }
    if ( bMatch ) {
      m_pdb->TextMainLineToAbsLine( pt );
      SetCurrentPosition( pt.y, pt.x );
    }
  }
  return bMatch;
}
// 获取当前位置所在字符的数值
// 如果位置当前指向换行符或文档为空，返回-1
LONG CPfxEditView::GetCurrentChar() const {
  ASSERT( m_pdb->TextGetLineCount() > 0 );
  ASSERT( m_pdb->TextIsValidChar( m_ptEditPos.y, m_ptEditPos.x ) );
  CPoint ptMain = m_ptEditPos;
  m_pdb->TextSubLineToMainLine( ptMain ); // 相对子行位置转换为相对主行位置
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( ptMain.y );
  const LPCTSTR pStart = pli->pData;
  const LPCTSTR pEnd = pStart + pli->nActLen;
  LPCTSTR pCur = pStart + ptMain.x;
  // 空行或指向行末
  if ( PFX_EC_IS_EMPTY_LINE( pli ) || ( pCur >= pEnd ) ) {
    return -1;
  }
#ifdef _UNICODE
  return static_cast<LONG>( *pCur );
#else
  BYTE cl;
  BYTE ch;
  if ( !PFX_EC_IS_LEAD_BYTE( *pCur ) || ( ( pCur + PFX_EC_MAX_MB_LEN ) > pEnd ) ) {
    cl = *pCur;
    ch = 0;
  } else {
    cl = *pCur++;
    ch = *pCur;
  }
  return static_cast<LONG>( MAKEWORD( cl, ch ) );
#endif
}
// BOOL bLogicalPos - TRUE返回与当前编辑位置对应的逻辑位置，这时主要用做
//                 向用户反映当前的编辑坐标
//                 等于FALSE时返回当前的读写位置，它是绝对位置
CPoint CPfxEditView::GetCurrentPosition( BOOL bLogicalPos ) const {
  ASSERT_VALID( this );
  ASSERT( m_pdb->TextGetLineCount() > 0 );
  CPoint pt( 0, 0 );
  if ( !bLogicalPos ) {
    pt = m_ptEditPos;
    m_pdb->TextSubLineToAbsLine( pt );
  } else {
    if ( !IsWrapping() ) {
      pt.y = m_ptEditPos.y;
    } else {
      pt.y = m_pdb->TextGetThisMainLine( m_ptEditPos.y );
      m_pdb->TextMainLineToAbsLine( pt );
    }
    pt.x = m_ptLogiPos.x;
  }
  return pt;
}
void CPfxEditView::BookmarkToggle( BOOL bOn, LONG nLine, BOOL bUpdate ) {
  ASSERT_VALID( this );
  ASSERT( m_pdb->TextGetLineCount() > 0 );
  CPoint pt( 0, nLine );
  if ( pt.y == -1 ) {
    pt.y = m_pdb->TextGetThisMainLine( m_ptEditPos.y );
  } else {
    if ( !m_pdb->IsValidLine( pt.y ) ) {
      return ;
    }
    m_pdb->TextAbsLineToMainLine( pt );
  }
  CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
  DWORD dwFlags = pli->dwFlags;
  if ( bOn ) {
    dwFlags |= PFX_EC_LF_BOOKMARK;
  } else {
    dwFlags &= ~PFX_EC_LF_BOOKMARK;
  }
  if ( dwFlags != pli->dwFlags ) {
    m_pdb->m_nBookmarks += ( bOn ? + 1 : -1 );
    ASSERT( m_pdb->m_nBookmarks >= 0 );
    pli->dwFlags = dwFlags;
    if ( bUpdate ) {
      UpdateAllViews( PFX_EC_UT_LINES, nLine, nLine );
    }
  }
}
// 查找下一书签所在行
// LONG nLine - 从这行的下一行开始搜索
// 要搜索第一行可以传入nLine等于-1
LONG CPfxEditView::BookmarkGetNext( LONG nLine, BOOL bWrap ) {
  ASSERT_VALID( this );
  ASSERT( m_pdb->TextGetLineCount() > 0 );
  if ( ( BookmarkGetCount() == 0 )
       || ( ( nLine != -1 ) && !m_pdb->IsValidLine( nLine ) ) ) {
    return PFX_EC_INVALID_LINE;
  }
  CPoint pt( 0, nLine );
  if ( pt.y != -1 ) {
    m_pdb->TextAbsLineToMainLine( pt );
  }
  LONG nLineFound = PFX_EC_INVALID_LINE;
  {
    LONG nLineCount = m_pdb->TextGetLineCount();
    for ( LONG i = pt.y + 1; i < nLineCount; i++ ) {
      if ( PFX_EC_IS_DRAW_BOOKMARK( m_pdb->TextGetLineAt( i ) ) ) {
        nLineFound = i;
        break;
      }
    }
  }
  if ( ( nLineFound == PFX_EC_INVALID_LINE ) && bWrap ) {
    m_pdb->EventSendMsg( PFX_EN_SEARCH_PASSED_EOF, EOF );
    for ( LONG i = 0; i <= pt.y; i++ ) {
      if ( PFX_EC_IS_DRAW_BOOKMARK( m_pdb->TextGetLineAt( i ) ) ) {
        nLineFound = i;
        break;
      }
    }
  }
  if ( nLineFound != PFX_EC_INVALID_LINE ) {
    CPoint ptRet( 0, nLineFound );
    m_pdb->TextMainLineToAbsLine( ptRet );
    nLineFound = ptRet.y;
  }
  return nLineFound;
}
// LONG nLine - 从这行的上一行开始搜索
// 要搜索最后行可以传入nLine等于GetLineCount()
LONG CPfxEditView::BookmarkGetPrev( LONG nLine, BOOL bWrap ) {
  ASSERT_VALID( this );
  ASSERT( m_pdb->TextGetLineCount() > 0 );
  if ( ( BookmarkGetCount() == 0 )
       || ( ( nLine != GetLineCount() ) && !m_pdb->IsValidLine( nLine ) ) ) {
    return PFX_EC_INVALID_LINE;
  }
  CPoint pt( 0, nLine );
  if ( pt.y != GetLineCount() ) {
    m_pdb->TextAbsLineToMainLine( pt );
  }
  LONG nLineFound = PFX_EC_INVALID_LINE;
  {
    for ( LONG i = pt.y - 1; i >= 0; i-- ) {
      if ( PFX_EC_IS_DRAW_BOOKMARK( m_pdb->TextGetLineAt( i ) ) ) {
        nLineFound = i;
        break;
      }
    }
  }
  if ( ( nLineFound == PFX_EC_INVALID_LINE ) && bWrap ) {
    m_pdb->EventSendMsg( PFX_EN_SEARCH_PASSED_EOF, ~EOF );
    for ( LONG i = m_pdb->TextGetLineUpperBound(); i >= pt.y; i-- ) {
      if ( PFX_EC_IS_DRAW_BOOKMARK( m_pdb->TextGetLineAt( i ) ) ) {
        nLineFound = i;
        break;
      }
    }
  }
  if ( nLineFound != PFX_EC_INVALID_LINE ) {
    CPoint ptRet( 0, nLineFound );
    m_pdb->TextMainLineToAbsLine( ptRet );
    nLineFound = ptRet.y;
  }
  return nLineFound;
}
void CPfxEditView::BookmarkClearAll() {
  ASSERT_VALID( this );
  if ( BookmarkGetCount() == 0 ) {
    return ;
  }
  for ( LONG i = 0, nLineCount = m_pdb->TextGetLineCount(); i < nLineCount; i++ ) {
    if ( PFX_EC_IS_DRAW_BOOKMARK( m_pdb->TextGetLineAt( i ) ) ) {
      m_pdb->TextGetLineAt( i ) ->dwFlags &= ~PFX_EC_LF_BOOKMARK;
    }
  }
  m_pdb->m_nBookmarks = 0;
  UpdateAllViews( PFX_EC_UT_LINES, 0, -1 );
}
BOOL CPfxEditView::IsDrawBookmark( LONG nLine ) const {
  ASSERT( m_pdb->TextGetLineCount() > 0 );
  if ( ( BookmarkGetCount() == 0 )
       || ( ( nLine != -1 ) && !m_pdb->IsValidLine( nLine ) ) ) {
    return false;
  }
  if ( nLine != -1 ) {
    CPoint pt( 0, nLine );
    m_pdb->TextAbsLineToMainLine( pt );
    nLine = pt.y;
  } else {
    nLine = m_pdb->TextGetThisMainLine( m_ptEditPos.y );
  }
  return PFX_EC_IS_DRAW_BOOKMARK( m_pdb->TextGetLineAt( nLine ) );
}
// BOOL SetViewRuler(BOOL bOn)
// 设置显示或隐藏标尺
// BOOL bOn - 是否显示标尺
void CPfxEditView::SetViewRuler( BOOL bOn ) {
  if ( ( bOn && !m_pdb->m_bDrawRuler ) || ( !bOn && m_pdb->m_bDrawRuler ) ) {
    m_pdb->m_bDrawRuler = bOn;
    UpdateAllViews( PFX_EC_UT_LAYOUT, 0, 0 );
  }
}
void CPfxEditView::SetViewLineNumber( BOOL bOn ) {
  if ( ( bOn && !m_pdb->m_bDrawLineNumber )
       || ( !bOn && m_pdb->m_bDrawLineNumber ) ) {
    m_pdb->m_bDrawLineNumber = bOn;
    UpdateAllViews( PFX_EC_UT_LAYOUT, 0, 0 );
    if ( !SetViewWrapping( GetWrapStyle(), GetWrapWidth() ) ) {
      UpdateCurrentPosition( m_ptTextPos.x, false, false );
    }
  }
}
void CPfxEditView::SetViewLineBreak( BOOL bOn ) {
  if ( ( bOn && !m_pdb->m_bDrawLineBreak )
       || ( !bOn && m_pdb->m_bDrawLineBreak ) ) {
    m_pdb->m_bDrawLineBreak = bOn;
    UpdateAllViews( PFX_EC_UT_LINES, 0, -1 );
  }
}
void CPfxEditView::SetViewWhitespace( BOOL bOn ) {
  if ( ( bOn && !m_pdb->m_bDrawWhitespace )
       || ( !bOn && m_pdb->m_bDrawWhitespace ) ) {
    m_pdb->m_bDrawWhitespace = bOn;
    UpdateAllViews( PFX_EC_UT_LINES, 0, -1 );
  }
}
// 如果执行了换行操作并刷新了所有兄弟视图，返回TRUE，否则返回FALSE
BOOL CPfxEditView::SetViewWrapping( UINT nStyle, LONG nWidth ) {
  SetDelayRecalcWrapping( false );
  // 检测是否需要执行所需操作，如果不需要就直接返回FALSE
  switch ( nStyle ) {
  default:
  case PFX_EC_WRAP_NONE:
    if ( !IsWrapping() ) {
      return false;
    }
    nWidth = 0;
    break;
  case PFX_EC_WRAP_COLUMNS:
    if ( ( nWidth <= 0 )
         || ( IsWrapAfterColumns()
              && ( GetWrapWidth() == nWidth ) ) ) {
      return false;
    }
    break;
  case PFX_EC_WRAP_WINDOWEDGE:
    if ( IsWrapAtWindowEdge()
         && ( GetWrapWidth() == m_rectText.Width() ) ) {
      return false;
    }
    nWidth = m_rectText.Width();
    break;
  }
  // 保存各个视图当前的状态以便在执行操作后恢复这些设置
  struct CState {
    BOOL bSelected;
    CPoint ptSel;
    CPoint ptCurrent;
    CPfxEditView* pView;
  };
  CState state[ PFX_EC_SIZEOF( m_pdb->m_arrViews ) ];
  for ( LONG i = 0; i < PFX_EC_SIZEOF( state ); i++ ) {
    CState& rState = state[ i ];
    CPfxEditView* pView = m_pdb->m_arrViews[ i ];
    rState.pView = pView;
    if ( pView == NULL ) {
      continue;
    }
    rState.ptSel = pView->SelGetParam();
    rState.bSelected = pView->IsTextSelected();
    rState.ptCurrent = pView->m_ptEditPos;
    m_pdb->TextSubLineToAbsLine( rState.ptSel );
    m_pdb->TextSubLineToAbsLine( rState.ptCurrent );
  }
  // 执行所要求的操作
  switch ( nStyle ) {
  case PFX_EC_WRAP_WINDOWEDGE:
    WrapAtUnits( nWidth );
    break;
  case PFX_EC_WRAP_COLUMNS:
    WrapColumns( nWidth );
    break;
  case PFX_EC_WRAP_NONE:
  default:
    WrapRestore();
    break;
  }
  // 现在恢复各种设置
  for ( i = 0; i < PFX_EC_SIZEOF( state ); i++ ) {
    CState& rState = state[ i ];
    CPfxEditView* pView = rState.pView;
    if ( pView == NULL ) {
      continue;
    }
    pView->SetCurrentPosition( rState.ptCurrent.y, rState.ptCurrent.x );
    if ( rState.bSelected ) {
      m_pdb->TextAbsLineToSubLine( rState.ptSel );
      pView->SelRange( rState.ptSel );
    }
  }
  UpdateAllViews( PFX_EC_UT_ALL, 0, -1 );
  return true;
}
// 设置当前位置
// LONG (nLine, nChar) - 以零为基值的行，列位置
void CPfxEditView::SetCurrentPosition( LONG nLine, LONG nChar ) {
  // 任务：
  // 1) 更新编辑当前位置
  // 2) 更新插入符位置
  // 3) 更新滚动栏滑块位置
  // 4) 判断当前插入点是否可见
  // 这个函数可以被任何人使用，因此传入的参数最不可靠，所以要严格检测
  // 参数的有效性
  // EnsureVisible()是内部使用的函数，它只是确保指定位置在可见范围内，
  // 而SetCurrentPosition则尽量将指定位置放在视图的中间
  CPoint cpOld = m_ptEditPos;
  CPoint cpNew( nChar, nLine );
  m_pdb->TextAbsLineToSubLine( cpNew );
  LONG xTextPos = TransCharPosToTextPos( cpNew );
  m_ptEditPos = cpNew;
  if ( IsTextSelected() && ( cpNew != cpOld ) ) {
    SelNone();
  }
  BOOL bNeedUpdate = false;
  // 指定行是否可见？如果不可见就要将其放到可见范围
  // m_rectText.top+(m_sizeVisible.cy*PFX_EC_DATA.m_nLineHeight)
  // 并不一定等于 m_rectText.bottom
  // 因为 m_sizeVisible.cy==m_rectText.Height()/PFX_EC_DATA.m_nLineHeight
  // 而不是/ (m_rectText.Height()+PFX_EC_DATA.m_nLineHeight-1)/PFX_EC_DATA.m_nLineHeight
  LONG i;
  if ( ( cpNew.y < m_ptFirstVisible.y )
       || ( cpNew.y >= ( m_ptFirstVisible.y + m_sizeVisible.cy ) ) ) {
    i = ( cpNew.y - m_sizeVisible.cy / 2 );
    m_ptFirstVisible.y = max( 0, i );
    ScrollBarSetPos( SB_VERT, m_ptFirstVisible.y );
    bNeedUpdate = true;
  }
  // 指定列是否可见？如果不可见就要将其滚动到可见范围
  LONG nHorzFirstVisibleUnit = m_ptFirstVisible.x * PFX_EC_DATA.m_nCharWidthAve;
  LONG nHorzVisibleUnits = m_rectText.Width();
  if ( ( xTextPos < nHorzFirstVisibleUnit )
       || ( xTextPos >= ( nHorzFirstVisibleUnit + nHorzVisibleUnits ) ) ) {
    i = ( ( xTextPos - nHorzVisibleUnits / 2 ) / PFX_EC_DATA.m_nCharWidthAve );
    m_ptFirstVisible.x = max( 0, i );
    ScrollBarSetPos( SB_HORZ, m_ptFirstVisible.x );
    bNeedUpdate = true;
  }
  // 刷新将会被延迟直到下一条WM_PAINT信息产生
  if ( bNeedUpdate ) {
    Invalidate( false );
  } else {
    UpdateLines( cpOld.y, cpNew.y );
  }
  UpdateCurrentPosition( xTextPos, true, true );
}
// LONG nLine    - 以零为基值的行序号
// LONG xTextPos - 以零为基值的文本影像水平坐标
// true  - 表示指定位置不在可见范围内，函数已经更新了整个视图，调用者不
//      需要再更新，但调用者应该更新插入符的位置
// false - 表示指定位置在可见范围内并且函数没有执行更新视图操作，调用者
//      若需要更新视图某部分，应该自己更新
BOOL CPfxEditView::EnsureVisible( LONG nLine, LONG xTextPos ) {
  ASSERT( m_pdb->TextIsValidLine( nLine ) );
  ASSERT( xTextPos >= 0 );
  // 由于用户使用滚动条滚动到其他地方去浏览文档的其它部分，使到
  // “当前插入点”不在可视范围内，当用户按下其他键（包括四个方向键）
  // 修改文档时需要将“当前插入点”放到可视范围内
  // 任务：
  // 如果指定的位置可见则不做任何事，否则将指定位置滚动到可见范围内，
  // 并且更新滚动条位置
  BOOL bNeedUpdate = false;
  LONG i, nLastVisibleLine = m_ptFirstVisible.y + m_sizeVisible.cy;
  // 下面是特殊情况，如果最后一行只有部分显示而当前行又正是这行的话，
  // 就要将该行向上滚一行
  if ( ( m_ptEditPos.y == nLastVisibleLine )
       && ( ( m_rectText.top + ( m_sizeVisible.cy * PFX_EC_DATA.m_nLineHeight ) ) <= m_rectText.bottom ) ) {
    m_ptFirstVisible.y = m_pdb->TextVerifyLine( m_ptFirstVisible.y + 1 );
    ScrollBarSetPos( SB_VERT, m_ptFirstVisible.y );
    bNeedUpdate = true;
  }
  // 指定行是否可见？如果不可见就要将其放到可见范围，这里不能使用“<=”
  if ( nLine < m_ptFirstVisible.y ) {
    // 如果指定行在首个可见行上面，则指定行将成为首个可见行
    m_ptFirstVisible.y = nLine;
    ScrollBarSetPos( SB_VERT, m_ptFirstVisible.y );
    bNeedUpdate = true;
  }
  // 这里一定要用“>=”
  else if ( nLine >= nLastVisibleLine ) {
    // 如果指定行在最后可见行下面，则指定行将成为最后可见行
    m_ptFirstVisible.y = m_pdb->TextVerifyLine( nLine - m_sizeVisible.cy + 1 );
    ScrollBarSetPos( SB_VERT, m_ptFirstVisible.y );
    bNeedUpdate = true;
  }
  // 指定逻辑单元是否可见？如果不可见就要将其滚动到可见范围
  LONG nHorzFirstVisibleUnit = m_ptFirstVisible.x * PFX_EC_DATA.m_nCharWidthAve;
  LONG nHorzVisibleUnits = m_rectText.Width();
  // 这里不能使用“<=”
  if ( xTextPos < nHorzFirstVisibleUnit ) {
    // 如果指定水平位置在首个可见逻辑列之前则首个可见逻辑列等于
    // （指定水平位置 - 当前文本区宽度 / 4）/ 逻辑列宽度 ，其操
    // 作结果是滑动窗口向左水平移动（文本区宽度 / 4）逻辑单元
    i = ( xTextPos - nHorzVisibleUnits / 4 ) / PFX_EC_DATA.m_nCharWidthAve;
    m_ptFirstVisible.x = max( 0, i );
#ifdef _SYNC_SCROLLING
    CPfxEditView* pSiblingView = GetSiblingView();
    if ( pSiblingView != NULL ) {
      pSiblingView->m_ptFirstVisible.x = m_ptFirstVisible.x;
      pSiblingView->Invalidate( false );
    }
#endif
    ScrollBarSetPos( SB_HORZ, m_ptFirstVisible.x );
    bNeedUpdate = true;
  }
  // 这里一定要用“>=”
  else if ( xTextPos >= ( nHorzFirstVisibleUnit + nHorzVisibleUnits ) ) {
    // 如果指定水平位置在最后可见逻辑列之后则首个可见逻辑列等于
    // （指定水平位置 - （2 * 当前文本区宽度 / 4））/ 逻辑列宽
    // 度，其操作结果是滑动窗口向右水平移动（文本区宽度 / 4）逻
    // 辑单元
    i = ( xTextPos + nHorzVisibleUnits / 4 - nHorzVisibleUnits ) / PFX_EC_DATA.m_nCharWidthAve;
    m_ptFirstVisible.x = max( 0, i );
#ifdef _SYNC_SCROLLING
    CPfxEditView* pSiblingView = GetSiblingView();
    if ( pSiblingView != NULL ) {
      pSiblingView->m_ptFirstVisible.x = m_ptFirstVisible.x;
      pSiblingView->Invalidate( false );
    }
#endif
    ScrollBarSetPos( SB_HORZ, m_ptFirstVisible.x );
    bNeedUpdate = true;
  }
  if ( bNeedUpdate )           //      刷新将会被延迟直到下一条WM_PAINT信息产生
  {
    this->Invalidate( false );
  }
  return bNeedUpdate;
}
void CPfxEditView::EnsureVisibleAndUpdateCurrentPosition() {
  EnsureVisible( m_ptEditPos.y, m_ptTextPos.x );
  UpdateCurrentPosition( m_ptTextPos.x, false, false );
}
void CPfxEditView::EnsureVisibleAndUpdateLines( LONG nLine, LONG xTextPos, LONG nFirstUpdateLine, LONG nLastUpdateLine ) {
  if ( !EnsureVisible( nLine, xTextPos ) ) {
    UpdateLines( nFirstUpdateLine, nLastUpdateLine );
  }
}
void CPfxEditView::EnsureVisibleAndSetAsCurrentPosition( CPoint pt ) {
  if ( m_ptEditPos.y != pt.y )           //      首先刷新原来的当前行
  {
    UpdateLines( m_ptEditPos.y, m_ptEditPos.y );
  }
  m_ptEditPos = pt;
  LONG xTextPos = TransCharPosToTextPos( m_ptEditPos );
  EnsureVisible( m_ptEditPos.y, xTextPos );
  UpdateLines( m_ptEditPos.y, m_ptEditPos.y );
  UpdateCurrentPosition( xTextPos, true, true );
}
// LPCTSTR GetPrevCharPos(LPCTSTR pCur, LPCTSTR pStart) const
// 将读写指针移到上一个字符，相当于CharPrev()
LPCTSTR CPfxEditView::GetPrevCharPos( LPCTSTR pCur, LPCTSTR pStart ) const {
  if ( pCur > pStart ) {
    LPCTSTR lp = pCur - 1;
    if ( PFX_EC_IS_LEAD_BYTE( *lp ) ) {
      --lp;
      return const_cast<LPTSTR>( max( lp, pStart ) );
    } else {
      while ( ( pStart <= ( --lp ) ) && ( PFX_EC_IS_LEAD_BYTE( *lp ) ) ) {
        ;
      }
      return const_cast<LPTSTR> ( ( pCur - 1 - ( ( pCur - lp ) & 0x01 ) ) );
    }
  }
  return const_cast<LPTSTR>( pStart );
}
// LPCTSTR GetNextCharPos(LPCTSTR pCur, LPCTSTR pEnd) const
// 将读写指正移到下一个字符，相当于CharNext()
LPCTSTR CPfxEditView::GetNextCharPos( LPCTSTR pCur, LPCTSTR pEnd ) const {
  if ( pCur < pEnd ) {
    pCur += ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
  }
  return const_cast<LPTSTR>( min( pCur, pEnd ) );
}
LPCTSTR CPfxEditView::GetPrevWordPos( LPCTSTR pCur, LPCTSTR pStart ) const {
  if ( ( pCur == NULL ) || ( pStart == NULL ) ) {
    return NULL;
  }
  // 首先滤去仅挨当前点的空白字符（PFX_EC_CHR_SPACE和PFX_EC_CHR_TAB）
  LPCTSTR pEnd = pCur;
  pCur = GetPrevNonWhitespacePos( pCur, pStart );
  if ( pCur > pStart ) {
    pCur = GetPrevCharPos( pCur, pStart );
    // 如果第一个非空白字符是定界符就继续向前找，直到找到空白字
    // 符或非定界符或行首为止
    if ( IsDelimiter( pCur, pEnd ) ) {
      while ( pCur > pStart ) {
        pCur = GetPrevCharPos( pCur, pStart );
        if ( IsWhitespace( *pCur )
             || !IsDelimiter( pCur, pEnd ) ) {
          pCur = GetNextCharPos( pCur, pEnd );
          break;
        }
      }
    }
    // 如果第一个非空白字符不是定界符就继续向前找，直到找到空白
    // 字符或定界符或行首为止
    else {
      while ( pCur > pStart ) {
        pCur = GetPrevCharPos( pCur, pStart );
        if ( IsWhitespace( *pCur )
             || IsDelimiter( pCur, pEnd ) ) {
          pCur = GetNextCharPos( pCur, pEnd );
          break;
        }
      }
    }
  }
  return pCur;
}
LPCTSTR CPfxEditView::GetNextWordPos( LPCTSTR pCur, LPCTSTR pEnd ) const {
  if ( ( pCur == NULL ) || ( pEnd == NULL ) ) {
    return NULL;
  }
  LPCTSTR pTmp = pCur;
  pCur = GetNextCharPos( pTmp, pEnd );
  // 如果当前位置是空白字符（PFX_EC_CHR_SPACE和PFX_EC_CHR_TAB）就一直向下查找直到第
  // 一个非空白字符，新的当前位置将指向该字符。如果当前位置不是空白字
  // 符就查找第一个单词定界符或第一个空白字符
  // 如果第一个字符是空白字符就找到非空白字符为止
  if ( IsWhitespace( *pTmp ) ) {
    pCur = GetNextNonWhitespacePos( pCur, pEnd );
  }
  // 如果第一个字符是定界符就找到第一个非定界符为止
  else if ( IsDelimiter( pTmp, pEnd ) ) {
    while ( pCur < pEnd ) {
      if ( IsWhitespace( *pCur ) ) {
        pCur = GetNextNonWhitespacePos( pCur, pEnd );
        break;
      }
      if ( !IsDelimiter( pCur, pEnd ) ) {
        break;
      }
      pCur = GetNextCharPos( pCur, pEnd );
    }
  }
  // 否则如果第一个字符是非定界符就找到一个定界符为止
  else {
    while ( pCur < pEnd ) {
      if ( IsWhitespace( *pCur ) ) {
        pCur = GetNextNonWhitespacePos( pCur, pEnd );
        break;
      }
      if ( IsDelimiter( pCur, pEnd ) ) {
        break;
      }
      pCur = GetNextCharPos( pCur, pEnd );
    }
  }
  return pCur;
}
LPCTSTR CPfxEditView::GetPrevNonWhitespacePos( LPCTSTR pCur, LPCTSTR pStart ) const {
  ASSERT( pCur >= pStart );
  // 当(pCur == pStart == NULL)时，(--pCur)会变成(0xFFFFFFFF)
  if ( pCur > pStart ) {
    while ( ( --pCur ) >= pStart ) {
      if ( !IsWhitespace( *pCur ) ) {
        pCur++;
        break;
      }
    }
  }
  return const_cast<LPTSTR>( max( pCur, pStart ) );
}
LPCTSTR CPfxEditView::GetNextNonWhitespacePos( LPCTSTR pCur, LPCTSTR pEnd ) const {
  ASSERT( pCur <= pEnd );
  while ( ( pCur < pEnd ) && IsWhitespace( *pCur ) ) {
    pCur++;
  }
  return const_cast<LPTSTR>( min( pCur, pEnd ) );
}
// 返回的指针不是指向单词定界符，而是它的下一个字符
LPCTSTR CPfxEditView::GetPrevDelimiterPos( LPCTSTR pCur, LPCTSTR pStart ) const {
  ASSERT( pCur >= pStart );
  while ( pCur > pStart ) {
    LPCTSTR pNext = pCur;
    pCur = GetPrevCharPos( pCur, pStart );
    if ( IsWhitespace( *pCur ) || IsDelimiter( pCur, pNext ) ) {
      pCur = pNext;
      break;
    }
  }
  return const_cast<LPTSTR>( max( pCur, pStart ) );
}
// 如果当前字符是单词定界符就返回当前地址
LPCTSTR CPfxEditView::GetNextDelimiterPos( LPCTSTR pCur, LPCTSTR pEnd ) const {
  ASSERT( pCur <= pEnd );
  while ( pCur < pEnd ) {
    if ( IsWhitespace( *pCur ) || IsDelimiter( pCur, pEnd ) ) {
      break;
    }
    pCur = GetNextCharPos( pCur, pEnd );
  }
  return const_cast<LPTSTR>( min( pCur, pEnd ) );
}
LONG CPfxEditView::MoveLeft( CPoint& rpt, BOOL bWord, BOOL bCalcTextPos ) const {
  // 如果rpt==m_ptEditPos就不太安全，所以要使用当前位置的副本
  CPoint cp = m_ptEditPos;
  m_pdb->TextSubLineToMainLine( cp );
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( cp.y );
  const LPCTSTR pStart = pli->pData;
  if ( bWord ) {
    cp.x = GetPrevWordPos( ( pStart + cp.x ), pStart ) - pStart;
  } else {
    cp.x = GetPrevCharPos( ( pStart + cp.x ), pStart ) - pStart;
  }
  m_pdb->TextMainLineToSubLine( cp );
  // 如果当前位置是物理行的行首并且当前行不是文档的第一行，就将新位置
  // 移动到上一行的行末
  if ( ( cp == m_ptEditPos ) && !m_pdb->TextIsFirstLine( cp.y ) ) {
    cp.y--;
    cp.x = m_pdb->TextGetLineLength( cp.y );
  }
  rpt = cp;
  return ( !bCalcTextPos ? 0 : TransCharPosToTextPos( rpt ) );
}
LONG CPfxEditView::MoveRight( CPoint& rpt, BOOL bWord, BOOL bCalcTextPos ) const {
  // 如果rpt==m_ptEditPos就不太安全，所以要使用当前位置的副本
  CPoint cp = m_ptEditPos;
  m_pdb->TextSubLineToMainLine( cp );
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( cp.y );
  const LPCTSTR pStart = pli->pData;
  const LPCTSTR pEnd = pStart + pli->nActLen;
  if ( bWord ) {
    cp.x = GetNextWordPos( ( pStart + cp.x ), pEnd ) - pStart;
  } else {
    cp.x = GetNextCharPos( ( pStart + cp.x ), pEnd ) - pStart;
  }
  m_pdb->TextMainLineToSubLine( cp );
  // 如果当前位置是物理行的行末并且当前行不是文档的最后行，就将新位置
  // 移动到下一行的行首
  if ( ( cp == m_ptEditPos ) && !m_pdb->TextIsLastLine( cp.y ) ) {
    cp.y++;
    cp.x = 0;
  }
  rpt = cp;
  return ( !bCalcTextPos ? 0 : TransCharPosToTextPos( rpt ) );
}
// 获取当前的选择范围
// 处于列块选择模式下返回的rnFromChar和rnToChar只对第一和最后行有效
BOOL CPfxEditView::GetSelRange( CPoint& rptFrom, CPoint& rptTo ) const {
  if ( IsTextSelected() ) {
    const CPfxEditLineInfo * pliFrom = m_pdb->TextGetLineAt( m_nSelLineFrom );
    rptFrom.y = m_nSelLineFrom;
    rptFrom.x = min( pliFrom->nLen, pliFrom->nSelFrom );
    const CPfxEditLineInfo* pliTo = m_pdb->TextGetLineAt( m_nSelLineTo );
    rptTo.y = m_nSelLineTo;
    rptTo.x = min( pliTo->nLen, pliTo->nSelTo );
    m_pdb->TextSubLineToAbsLine( rptFrom );
    m_pdb->TextSubLineToAbsLine( rptTo );
  } else {
    rptFrom.y = rptTo.y = PFX_EC_INVALID_SELECT;
    rptFrom.x = rptTo.x = PFX_EC_INVALID_SELECT;
  }
  return IsTextSelected();
}
// 获取当前选择内容的大小，如果没有选择内容则返回零
// LONG nLineBreakType - 指出换行符的类型，例如
//                    DOS是PFX_EC_LINE_BREAK_TYPE_DOS，
//                    UNIX是PFX_EC_LINE_BREAK_TYPE_UNIX，
//                    MAC是PFX_EC_LINE_BREAK_TYPE_MAC
// BOOL bCopyBufSize   - 指出在列块选择模式下是否返回复制缓冲区的大小，
//                    本参数不影响正常模式选择模式下的返回值，如果该
//                    参数为TRUE则返回包含换行符的缓冲区大小，否则返
//                    回实际的选择内容的大小
// 如果在列块选择下为了向用户反映当前的选择大小，应该将bCopyBufSize设置
// 为FALSE，这样将不会加上换行符的大小，因为列块选择下的选择内容都不包含
// 换行符，而在执行复制操作时再强行在每行的结尾加上换行符。也即是说如果
// 是为GetSelData()确定缓冲区大小就应该将该参数设置为TRUE
LONG CPfxEditView::GetSelSize( LONG nLineBreakType, BOOL bCopyBufSize ) const {
  if ( !IsTextSelected() ) {
    return 0;
  }
  // bCopyBufSize参数只会影响列块选择模式
  if ( IsColumnSelect() ) {
    if ( !bCopyBufSize ) {
      return m_nSelSize;
    }
  } else {
    if ( GetLineBreakType() == nLineBreakType ) {
      return m_nSelSize;
    }
  }
  LONG nSelSize = 0;
  LONG nLineBreakSize = m_pdb->GetLineBreakSizeFromType( nLineBreakType );
  if ( IsColumnSelect() )           //      列块选择下的选择大小
  {
    for ( LONG i = m_nSelLineFrom; i <= m_nSelLineTo; i++ ) {
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( i );
      // 列块选择没有包含换行符，即使如此每一行都要加上换
      // 行符
      nSelSize += ( min( pli->nSelTo, pli->nLen ) - pli->nSelFrom );
      nSelSize += nLineBreakSize;
    }
  }
  else // 正常选择下的选择大小
  {
    for ( LONG i = m_nSelLineFrom; i <= m_nSelLineTo; i++ ) {
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( i );
      // 当pli->nSelTo大于pli->nLen时表明这一行的选择包括
      // 换行符
      nSelSize += ( ( pli->nSelTo > pli->nLen ) ? ( pli->nLen - pli->nSelFrom + nLineBreakSize ) : ( pli->nSelTo - pli->nSelFrom ) );
    }
  }
  return nSelSize;
}
// 获取当前的选择内容
// LONG nLineBreakType - 换行符类型，例如
//                    PFX_EC_LINE_BREAK_TYPE_DOS,PFX_EC_LINE_BREAK_TYPE_UNIX,PFX_EC_LINE_BREAK_TYPE_MAC，
//                    遇到换行符，函数会以指定类型的换行符数据填入
// LPTSTR pBuffer      - 接收数据的缓冲区指针
// LONG nMaxBuf        - 该缓冲区的长度
// 返回实际复制的字符数，不包含结尾的NULL字符单位：TCHAR
// 遇到NULL字符，函数将会以空格字符代替
LONG CPfxEditView::GetSelData( LONG nLineBreakType, LPTSTR pBuffer, LONG nMaxBuf ) const {
  if ( !IsTextSelected() || ( pBuffer == NULL ) || ( nMaxBuf <= 0 ) ) {
    return 0;
  }
  // 用来容纳结尾的NULL字符
  nMaxBuf--;
  LONG nLineBreakSize = m_pdb->GetLineBreakSizeFromType( nLineBreakType );
  LPCTSTR pLineBreakData = m_pdb->GetLineBreakDataFromType( nLineBreakType );
  LONG nSelLineTo = m_nSelLineTo;
  LPTSTR pCur = pBuffer;
  // 获取列块选择时的数据
  if ( IsColumnSelect() ) {
    for ( LONG i = m_nSelLineFrom; ( i <= nSelLineTo ) && ( nMaxBuf > 0 ); i++ ) {
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( i );
      LPCTSTR pData = pli->pData + pli->nSelFrom;
      LONG nSelLen = min( pli->nLen, pli->nSelTo ) - pli->nSelFrom;
      if ( nMaxBuf < nSelLen ) {
        nSelLen = CalcCharsToCopy( pData, nMaxBuf );
      }
      // 如果缓冲区已经不足以容纳所有本行字符，就需要将输
      // 出内容截断，截断可能会发生在一个MBCS字符的中间，
      // 但这种情况是不允许的，所以要定位该字符的位置
      if ( nSelLen > 0 ) {
        memcpy( pCur, pData, ( nSelLen * sizeof( TCHAR ) ) );
        pCur += nSelLen;
        nMaxBuf -= nSelLen;
      }
      // 列块选择下无论如何都要复制换行符
      if ( nMaxBuf >= nLineBreakSize ) {
        memcpy( pCur, pLineBreakData, nLineBreakSize * sizeof( TCHAR ) );
        pCur += nLineBreakSize;
        nMaxBuf -= nLineBreakSize;
      } else {
        break;
      }
    }
  }
  // 获取正常选择时的数据
  else {
    for ( LONG i = m_nSelLineFrom; ( i <= nSelLineTo ) && ( nMaxBuf > 0 ); i++ ) {
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( i );
      LPCTSTR pData = pli->pData + pli->nSelFrom;
      LONG nSelLen = min( pli->nLen, pli->nSelTo ) - pli->nSelFrom;
      // 如果缓冲区已经不足以容纳所有本行字符，就需要将输
      // 出内容截断，截断可能会发生在一个MBCS字符的中间，
      // 但这种情况是不允许的，所以要定位该字符的位置
      if ( nMaxBuf < nSelLen ) {
        nSelLen = CalcCharsToCopy( pData, nMaxBuf );
      }
      if ( nSelLen > 0 ) {
        memcpy( pCur, pData, ( nSelLen * sizeof( TCHAR ) ) );
        pCur += nSelLen;
        nMaxBuf -= nSelLen;
      }
      // 当pli->nSelTo大于pli->nLen时表明这一行的选择包括
      // 换行符，需要复制换行符CRLF
      if ( pli->nSelTo > pli->nLen ) {
        if ( nMaxBuf >= nLineBreakSize ) {
          memcpy( pCur, pLineBreakData, nLineBreakSize * sizeof( TCHAR ) );
          pCur += nLineBreakSize;
          nMaxBuf -= nLineBreakSize;
        } else {
          break;
        }
      }
    }
  }
  // 因为控件打开文件的时候是原封不动的将数据读入，即使有NULL
  // 字符也不改动，所以选择的文本中可能有NULL字符，现在要进行
  // 检查，将选择文本中的NULL字符替换为空格
  LONG nCharsCopied = pCur - pBuffer;
  pCur = pBuffer;
  LPTSTR pEnd = pBuffer + nCharsCopied;
  while ( pCur < pEnd ) {
    if ( '\0' == *pCur ) {
      *pCur = PFX_EC_CHR_SPACE;
    }
    pCur++;
  }
  // 填入结尾字符NULL
  *pCur = '\0';
  return nCharsCopied;
}
void CPfxEditView::CharsInsert( CPoint& rptFrom, LPCTSTR pData, LONG cch, BOOL bUpdate ) {
  if ( ( pData == NULL ) || ( cch == 0 ) ) {
    return ;
  }
  CPoint ptOld = rptFrom;
  m_pdb->TextSubLineToMainLine( ptOld );
  m_pdb->TextInsertChars( rptFrom, pData, cch, ( bUpdate ? this : NULL ) );
  m_pdb->UndoAddInsert( ptOld, rptFrom );
  ASSERT( m_pdb->TextIsValidLine( rptFrom.y ) );
  ASSERT( m_pdb->TextIsValidChar( rptFrom.y, rptFrom.x ) );
}
void CPfxEditView::CharsRemove( CPoint& rptFrom, CPoint& rptTo, BOOL bUpdate ) {
  if ( rptFrom == rptTo ) {
    return ;
  }
  if ( rptFrom.y > rptTo.y ) {
    LONG i = rptFrom.y;
    rptFrom.y = rptTo.y;
    rptTo.y = i;
    i = rptFrom.x;
    rptFrom.x = rptTo.x;
    rptTo.x = i;
  }
  m_pdb->UndoAddRemove( rptFrom, rptTo );
  m_pdb->TextRemoveChars( rptFrom, rptTo, ( bUpdate ? this : NULL ) );
  ASSERT( m_pdb->TextIsValidLine( rptFrom.y ) );
  ASSERT( m_pdb->TextIsValidChar( rptFrom.y, rptFrom.x ) );
}
void CPfxEditView::CharsReplace( CPoint& rptFrom, CPoint& rptTo, LPCTSTR pData, LONG cch, BOOL bUpdate ) {
  // 相当于CharsRemove()
  if ( rptFrom != rptTo ) {
    if ( rptFrom.y > rptTo.y ) {
      CPoint pt = rptFrom;
      rptFrom = rptTo;
      rptTo = pt;
    }
    m_pdb->UndoAddRemove( rptFrom, rptTo );
    m_pdb->TextRemoveChars( rptFrom, rptTo, ( bUpdate ? this : NULL ) );
    ASSERT( m_pdb->TextIsValidLine( rptFrom.y ) );
    ASSERT( m_pdb->TextIsValidChar( rptFrom.y, rptFrom.x ) );
  }
  // 相当于CharsInsert()
  if ( ( pData != NULL ) && ( cch > 0 ) ) {
    CPoint ptOld = rptFrom;
    m_pdb->TextSubLineToMainLine( ptOld );
    m_pdb->TextInsertChars( rptFrom, pData, cch, ( bUpdate ? this : NULL ) );
    m_pdb->UndoAddInsert( ptOld, rptFrom );
    ASSERT( m_pdb->TextIsValidLine( rptFrom.y ) );
    ASSERT( m_pdb->TextIsValidChar( rptFrom.y, rptFrom.x ) );
  }
}
BOOL CPfxEditView::IsDelimiter( WCHAR wc ) const {
  ASSERT( m_pti != NULL );
  LPCWSTR lp = m_pti->m_arrDelimiters;
  while ( ( *lp != '\0' ) && ( *lp != wc ) ) {
    ++lp;
  }
  return ( *lp != '\0' );
}
BOOL CPfxEditView::IsDelimiter( LPCTSTR pCur, LPCTSTR pEnd ) const {
  if ( pCur >= pEnd ) {
    return false;
  }
  WCHAR wc = ( ( !PFX_EC_IS_LEAD_BYTE( *pCur )
                 || ( ( pCur + PFX_EC_MAX_MB_LEN ) > pEnd ) ) ? *pCur : *( ( LPCWSTR ) pCur ) );
  ASSERT( m_pti != NULL );
  LPCWSTR lp = m_pti->m_arrDelimiters;
  while ( ( *lp != '\0' ) && ( *lp != wc ) ) {
    ++lp;
  }
  return ( *lp != '\0' );
}
// 测试给定位置是否在选择范围内
// CPoint ptView - 测试的窗口客户区坐标
BOOL CPfxEditView::IsInsideSelect( CPoint ptView ) const {
  if ( !IsTextSelected() ) {
    return false;
  }
  CPoint ptText;
  ptText.x = TransViewPosToTextPosX( ptView.x );
  ptText.y = TransViewPosToTextPosY( ptView.y );
  // 非列块选择，只探测选择部分第一行和最后行，中间的部分无论水平坐标
  // 落在那里都算命中
  if ( !IsColumnSelect() ) {
    CPoint pt;
    pt.y = TransTextPosToLinePos( ptText.y );
    pt.x = TransTextPosToCharPos( pt.y, ptText.x );
    const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
    // 如果只有一行被选择，则必须要在选择范围内才算命中
    if ( m_nSelLineFrom == m_nSelLineTo ) {
      return ( ( pt.y == m_nSelLineFrom )
               && ( pt.x >= pli->nSelFrom )
               && ( pt.x <= pli->nSelTo ) );
    }
    // 夹在中间的行无论在哪里都算命中
    if ( ( pt.y > m_nSelLineFrom ) && ( pt.y < m_nSelLineTo ) ) {
      return true;
    }
    // 第一行要在选择的第一列之前才算不命中
    if ( ( pt.y == m_nSelLineFrom ) && ( pt.x >= pli->nSelFrom ) ) {
      return true;
    }
    // 最后行要在选择的最后列之后才算不命中
    if ( ( pt.y == m_nSelLineTo ) && ( pt.x < pli->nSelTo ) ) {
      return true;
    }
  } else // 列块选择，只有真正击中选择部分才算命中
  {
    CPoint pt;
    pt.y = TransTextPosToLinePos( ptText.y );
    pt.x = 0;
    if ( ( pt.y < m_nSelLineFrom ) || ( pt.y > m_nSelLineTo ) ) {
      return false;
    }
    LONG x1 = min( m_rectColSelect.left, m_rectColSelect.right );
    LONG x2 = max( m_rectColSelect.left, m_rectColSelect.right );
    const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
    pt.x = pli->nSelFrom;
    LONG p1 = TransCharPosToTextPos( pt );
    pt.x = pli->nSelTo;
    LONG p2 = TransCharPosToTextPos( pt );
    // 如果是列块选择并且选择内容已经是本行最后的字符，以及选择
    // 内容右边比列块范围的右边小，就填充选择内容最后一个字符与
    // 列块范围右边的间隙，以使看起来是一整块
    if ( ( pli->nSelFrom >= pli->nLen ) && ( p1 < x1 ) ) {
      p1 = x1;
    }
    if ( ( pli->nSelTo >= pli->nLen ) && ( p2 < x2 ) ) {
      p2 = x2;
    }
    if ( ( ptText.x >= p1 ) && ( ptText.x <= p2 ) ) {
      return true;
    }
  }
  return false;
}
// 判断给定的HGLOBAL内的数据是否是在列块模式下提交的
// BOOL bDragDrop  - 是否是OLE拖放操作
// HGLOBAL hGlobal - 用GlobalAlloc()分配的内存据柄
BOOL CPfxEditView::IsDataColumnModeRendered( BOOL bDragDrop, HGLOBAL hGlobal ) const {
  if ( hGlobal == NULL ) {
    return false;
  }
  if ( bDragDrop ) {
    return ( hGlobal == PFX_EC_DATA.m_hDataDragDropCopied );
  }
  // 这种办法并不100%可靠
  if ( ( hGlobal == PFX_EC_DATA.m_hDataClipboardCopied )
       && ( PFX_EC_DATA.m_nDataClipboardCopied > 0 ) ) {
    ::GlobalFlags( hGlobal );
    return ( PFX_EC_DATA.m_nDataClipboardCopied == ::GlobalSize( hGlobal ) );
  }
  return false;
}
// 打印...
// void PrintReleaseResource()
// 释放打印时所申请的资源
void CPfxEditView::PrintReleaseResource() {
  if ( m_ppc != NULL ) {
    delete m_ppc;
    m_ppc = NULL;
  }
}
// 检查pc.m_arrKeyClasses[i]的颜色是否是指定打印机（pc.m_pdcPrinter)所
// 支持
// CPfxEditViewPrintContext& pc - 已经初始化好的对象，m_arrKeyClasses
//       已经从m_pti->m_arrKeyClasses.m_arrClass处复制
// 某些屏幕颜色可能并不被打印机所支持，所以要根据打印机的支持能力进行
// 调整，如果某种颜色打印机不支持就用默认的文本颜色
void CPfxEditView::PrintCheckColors( CPfxEditViewPrintContext& pc ) {
  // 先获取默认的文本颜色，如果屏幕所用的文本颜色不被支持就用最保守的
  // 黑色来代替
  CDCH pDC = pc.m_pdcPrinter;
  COLORREF clrPaper = pDC.GetNearestColor( RGB( 255, 255, 255 ) );
  COLORREF clrText = pDC.GetNearestColor( m_pti->GetKeyClassColor( kcText ) );
  if ( ( clrText == CLR_INVALID ) || ( clrText == clrPaper ) ) {
    clrText = pDC.GetNearestColor( RGB( 0, 0, 0 ) );
  }
  for ( int i = 0; i < PFX_EC_SIZEOF( pc.m_arrKeyClasses ); i++ ) {
    COLORREF color = clrText;
    if ( !pc.m_arrKeyClasses[ i ].m_bEmpty ) {
      color = pDC.GetNearestColor(
                pc.m_arrKeyClasses[ i ].m_color );
      if ( ( color == CLR_INVALID ) || ( color == clrPaper ) ) {
        color = clrText;
      }
    }
    pc.m_arrKeyClasses[ i ].m_color = color;
  }
}
// 计算页面布局
// HDC hdcPrinter - 打印DC
// CPfxEditViewPrintContext& pc - 打印信息
void CPfxEditView::PrintCalcLayout( HDC hdcPrinter, CPfxEditViewPrintContext& pc ) {
  CDCH pdcPrinter = hdcPrinter;
  CSize size;
  for ( LONG nCharWidthMaxNum = 0, i = '0'; i <= '9'; i++ ) {
    size = pdcPrinter.GetTextExtent( ( LPCTSTR ) & i, 1 );
    nCharWidthMaxNum = max( nCharWidthMaxNum, size.cx );
  }
  i = PFX_EC_CHR_SPACE;
  size = pdcPrinter.GetTextExtent( ( LPCTSTR ) & i, 1 );
  LONG nCharWidthSpace = size.cx;
  i = PFX_EC_CHR_TAB;
  size = pdcPrinter.GetTextExtent( ( LPCTSTR ) & i, 1 );
  LONG nCharWidthTab = size.cx;
  // 行号数字位数
  LONG nLineNumberChars = 0;
  // 行号区域宽度，不打印行号时为零
  LONG nWidthLineNumber = 0;
  // 边框线宽度
  LONG nWidthMarginLine = 0;
  // 水平分隔线宽度
  LONG nWidthSeparateLine = 0;
  // 标题区高度
  LONG nHeightHeader = 0;
  // 脚注区高度
  LONG nHeightFooter = 0;
  // 标题脚注字体高度
  LONG nHeightHeaderAndFooterLine = 0;
  // 正文和标题之间的垂直距离，不打印标题时为零
  LONG nSpacingHeaderAndText = 0;
  // 正文和脚注之间的垂直距离，不打印脚注时为零
  LONG nSpacingFooterAndText = 0;
  // 正文和行号之间的水平距离，不打印行号时为零
  LONG nSpacingLineNumberAndText = 0;
  // 计算行号区宽度
  if ( pc.IsPrintLineNumber() ) {
    for ( i = GetLineCount(), nLineNumberChars = 1; i >= 10; nLineNumberChars++ ) {
      i /= 10;
    }
    nWidthLineNumber = nLineNumberChars * nCharWidthMaxNum;
    nSpacingLineNumberAndText = pc.PrinterLogicUnitFromDisplayX( m_pdb->m_rectIndent.left );
  }
  TEXTMETRIC tm;
  pdcPrinter.GetTextMetrics( &tm );
  pc.m_nCharWidthTab = nCharWidthTab;
  pc.m_nCharWidthSpace = nCharWidthSpace;
  pc.m_nCharWidthMax = max( tm.tmMaxCharWidth, ( tm.tmAveCharWidth * 2 ) );
  pc.m_nFontHeight = tm.tmHeight;
  pc.m_nLineHeight = tm.tmHeight + pc.PrinterLogicUnitFromDisplayY( GetSpacingLines() );
  pc.m_nSpacingLines = pc.PrinterLogicUnitFromDisplayY( GetLineTextVertOffset() );
  // 边框线宽度
  LOGPEN logpen;
  if ( pc.IsPrintMargin() ) {
    pc.m_penMargin.GetLogPen( &logpen );
    nWidthMarginLine = logpen.lopnWidth.x;
  }
  pc.m_nWidthMarginLine = nWidthMarginLine;
  // 计算除去页边距后的可打印范围，和修正页边距
  CSize sizePaper; // 纸张尺寸
  sizePaper.cx = pdcPrinter.GetDeviceCaps( PHYSICALWIDTH );
  sizePaper.cy = pdcPrinter.GetDeviceCaps( PHYSICALHEIGHT );
  pdcPrinter.DPtoLP( &sizePaper );
  CSize sizePrint; // 可打印范围
  sizePrint.cx = pdcPrinter.GetDeviceCaps( HORZRES );
  sizePrint.cy = pdcPrinter.GetDeviceCaps( VERTRES );
  pdcPrinter.DPtoLP( &sizePrint );
  CSize sizeOfsLT; // 左边和上边最小页边距
  sizeOfsLT.cx = pdcPrinter.GetDeviceCaps( PHYSICALOFFSETX );
  sizeOfsLT.cy = pdcPrinter.GetDeviceCaps( PHYSICALOFFSETY );
  pdcPrinter.DPtoLP( &sizeOfsLT );
  CSize sizeOfsRB; // 右边和下边最小页边距
  sizeOfsRB.cx = sizePaper.cx - sizePrint.cx - sizeOfsLT.cx;
  sizeOfsRB.cy = sizePaper.cy - sizePrint.cy - sizeOfsLT.cy;
  CRect rectMargin( &pc.m_rectMargin );
  rectMargin.left = max( 0, ( rectMargin.left - sizeOfsLT.cx ) );
  rectMargin.top = max( 0, ( rectMargin.top - sizeOfsLT.cy ) );
  rectMargin.right = max( 0, ( rectMargin.right - sizeOfsRB.cx ) );
  rectMargin.bottom = max( 0, ( rectMargin.bottom - sizeOfsRB.cy ) );
  CRect rectDraw( &pc.m_rectDraw );
  rectDraw.DeflateRect( &rectMargin );
  /*
  // 可打印范围
  CSize sizeOffset;
  sizeOffset.cx         = pdcPrinter.GetDeviceCaps(PHYSICALOFFSETX);
  sizeOffset.cy         = pdcPrinter.GetDeviceCaps(PHYSICALOFFSETY);
  pdcPrinter.DPtoLP(&sizeOffset);
  CRect rectMargin(&pc.m_rectMargin);
  rectMargin.left       = max(sizeOffset.cx, rectMargin.left);
  rectMargin.top        = max(sizeOffset.cy, rectMargin.top);
  rectMargin.right      = max(sizeOffset.cx, rectMargin.right);
  rectMargin.bottom     = max(sizeOffset.cy, rectMargin.bottom);
  CRect rectDraw(&pc.m_rectDraw);
  rectDraw.DeflateRect(&rectMargin);
  rectDraw.InflateRect(sizeOffset.cx, sizeOffset.cy);
  rectDraw.IntersectRect(&rectDraw, &pc.m_rectDraw);
  */ 
  // 计算出可打印范围
  pc.m_rectDraw.CopyRect( &rectDraw );
  // 标题和脚注和水平分隔线高度
  if ( pc.IsPrintHeader() || pc.IsPrintFooter() ) {
    if ( pc.PenSeparateLine() != NULL ) {
      pc.m_penSeparateLine.GetLogPen( &logpen );
      nWidthSeparateLine = logpen.lopnWidth.x + pc.PrinterLogicUnitFromDisplayY( PFX_EC_DEF_SPACING_HEADER_FOOTER_TEXT_AND_SEPARATOR );
    }
    if ( pc.FontHeaderAndFooter() == NULL )           //      使用正文字体
    {
      nHeightHeaderAndFooterLine = pc.m_nFontHeight;
    } else {
      CFontH pFontOld = pdcPrinter.SelectObject( pc.GetFontHeaderAndFooter() );
      pdcPrinter.GetTextMetrics( &tm );
      pdcPrinter.SelectObject( pFontOld );
      nHeightHeaderAndFooterLine = tm.tmHeight;
    }
  }
  // 计算标题区的最大高度
  if ( pc.IsPrintHeader() ) {
    // 标题区和正文之间的垂直高度
    nSpacingHeaderAndText = pc.m_nSpacingHeaderAndText;
    // 标题区高度，高度等于行数乘以每行高度加水平分隔线宽度
    const CString& strHeader = pc.m_strHeader;
    for ( LONG i = 0, nLines = 1; ; nLines++ ) {
      i = strHeader.Find( PFX_EC_PRINT_MACRO_CRLF, i );
      if ( i == -1 ) {
        break;
      }
      i += _tcslen( PFX_EC_PRINT_MACRO_CRLF );
    }
    nHeightHeader = nLines * nHeightHeaderAndFooterLine + nWidthSeparateLine;
  }
  // 计算脚注区的最大高度
  if ( pc.IsPrintFooter() ) {
    // 脚注区和正文之间的垂直高度
    nSpacingFooterAndText = pc.m_nSpacingFooterAndText;
    // 标题区高度，高度等于行数乘以每行高度加水平分隔线宽度
    const CString& strFooter = pc.m_strFooter;
    for ( LONG i = 0, nLines = 1; ; nLines++ ) {
      i = strFooter.Find( PFX_EC_PRINT_MACRO_CRLF, i );
      if ( i == -1 ) {
        break;
      }
      i += _tcslen( PFX_EC_PRINT_MACRO_CRLF );
    }
    nHeightFooter = nLines * nHeightHeaderAndFooterLine + nWidthSeparateLine;
  }
  pc.m_nWidthSeparateLine = nWidthSeparateLine;
  pc.m_nHeaderAndFooterLineHeight = nHeightHeaderAndFooterLine;
  // 标题区域
  CRect rectHeader( &rectDraw );
  rectHeader.bottom = rectHeader.top + nHeightHeader;
  rectHeader.NormalizeRect();
  pc.m_rectHeader.CopyRect( &rectHeader );
  // 脚注区域
  CRect rectFooter( &rectDraw );
  rectFooter.top = rectFooter.bottom - nHeightFooter;
  rectFooter.NormalizeRect();
  pc.m_rectFooter.CopyRect( &rectFooter );
  // 行号区域
  CRect rectLineNumber;
  rectLineNumber.SetRect( rectDraw.left, ( rectHeader.bottom + nSpacingHeaderAndText ), ( rectDraw.left + nWidthLineNumber ), ( rectFooter.top - nSpacingFooterAndText ) );
  rectLineNumber.NormalizeRect();
  pc.m_rectLineNumber.CopyRect( &rectLineNumber );
  // 文本区域
  CRect rectText;
  rectText.SetRect( ( rectLineNumber.right + nSpacingLineNumberAndText ), rectLineNumber.top, rectDraw.right, rectLineNumber.bottom );
  rectText.NormalizeRect();
  pc.m_rectText.CopyRect( &rectText );
  // 每页能够显示的行数
  pc.m_nLinesPerPage = rectText.Height() / pc.m_nLineHeight;
}
void CPfxEditView::PrintFormatText( HDC hdcPrinter, CPfxEditViewPrintContext& pc ) {
  CPfxEditViewWrapLineHint wl;
  wl.m_hDC = hdcPrinter;
  wl.m_nliCur = 0;
  wl.m_nGrowBy = 10000;
  wl.m_nTabSize = pc.m_nTabSize;
  wl.m_nCharWidthMax = pc.m_nCharWidthMax;
  wl.m_nCharWidthTab = pc.m_nCharWidthTab;
  wl.m_nCharWidthSpace = pc.m_nCharWidthSpace;
  CPfxEditHeap& Heap = pc.m_cHeap;
  CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( 0 );
  CPfxEditLineInfo* pliEnd = m_pdb->TextGetLineAt( m_pdb->TextGetLineUpperBound() );
  CPfxEditLineInfo* pliLine = NULL;
  // 计算换行进度数据，这些数据将会转换成完成百分比通知送给
  // m_pdb->EventSendMsg()，一般用来显示当前的进度给用户看
  // 每百分一所占的行数，这些行数是指物理行
  LONG nLine = 0;
  LONG nPercentCompleted = 0;
  LONG nLinesOfOnePercent = max( 1, ( ( GetLineCount() + 99 ) / 100 ) );
  switch ( pc.m_nWrapStyle ) {
  case PFX_EC_WRAP_WINDOWEDGE:
    wl.m_nWidth = pc.m_rectText.Width();
    wl.m_nliMax = Heap.GetBufferConstSize() / sizeof( CPfxEditLineInfo );
    Heap.Alloc( ( wl.m_nliMax * sizeof( CPfxEditLineInfo ) ) );
    for ( nLine = 0; pli <= pliEnd; nLine++ ) {
      // 记下当前的指针偏移量，因为如果当前不处于自动换行
      // 模式下pli->nIndex 都是等于PFX_EC_INVALID_LINE，但打
      // 印模块需要该数值为真实行号
      LONG nli = wl.m_nliCur;
      WrapLineAtUnits( Heap, pli, wl );
      pliLine = ( static_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ) + nli );
      pliLine->nIndex = nLine;
      // 跳过子行
      while ( ( ++pli <= pliEnd ) && PFX_EC_IS_SUBLINE( pli ) ) {
        //  do nothing
      }
      if ( ( nLine % nLinesOfOnePercent ) == 0 ) {
        nPercentCompleted++;
        m_pdb->EventSendMsg( PFX_EN_WORD_WRAPPING, ( LPARAM ) nPercentCompleted );
      }
    }
    break;
  case PFX_EC_WRAP_COLUMNS:
    wl.m_nWidth = GetWrapWidth();
    wl.m_nliMax = Heap.GetBufferConstSize() / sizeof( CPfxEditLineInfo );
    Heap.Alloc( ( wl.m_nliMax * sizeof( CPfxEditLineInfo ) ) );
    for ( nLine = 0; pli <= pliEnd; nLine++ ) {
      // 记下当前的指针偏移量，因为如果当前不处于自动换行
      // 模式下pli->nIndex 都是等于PFX_EC_INVALID_LINE，但打
      // 印模块需要该数值为真实行号
      LONG nli = wl.m_nliCur;
      WrapLineColumns( Heap, pli, wl );
      pliLine = ( static_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ) + nli );
      pliLine->nIndex = nLine;
      // 跳过子行
      while ( ( ++pli <= pliEnd ) && PFX_EC_IS_SUBLINE( pli ) ) {
        //  do nothing
      }
      if ( ( nLine % nLinesOfOnePercent ) == 0 ) {
        nPercentCompleted++;
        m_pdb->EventSendMsg( PFX_EN_WORD_WRAPPING, ( LPARAM ) nPercentCompleted );
      }
    }
    break;
  case PFX_EC_WRAP_NONE:
  default:           // 贯通
    wl.m_nWidth = 0;
    wl.m_nliMax = GetLineCount() + 1;
    Heap.Alloc( ( wl.m_nliMax * sizeof( CPfxEditLineInfo ) ) );
    pliLine = static_cast<CPfxEditLineInfo*>( Heap.GetBuffer() );
    for ( nLine = 0; pli <= pliEnd; pliLine++, nLine++, wl.m_nliCur++ ) {
      pliLine->dwFlags = ( pli->dwFlags | PFX_EC_LF_HARDRETURN );
      pliLine->nActLen = pliLine->nLen = pli->nActLen;
      pliLine->pData = pli->pData;
      pliLine->pMemBlock = pli->pMemBlock;
      pliLine->nIndex = nLine;
      pliLine->nFirstChar = 0;
      // 跳过子行
      while ( ( ++pli <= pliEnd ) && PFX_EC_IS_SUBLINE( pli ) ) {
        //  do nothing
      }
      if ( ( nLine % nLinesOfOnePercent ) == 0 ) {
        nPercentCompleted++;
        m_pdb->EventSendMsg( PFX_EN_WORD_WRAPPING, ( LPARAM ) nPercentCompleted );
      }
    }
    break;
  }
  // 总共有这个数的行被存储在这个缓冲区内
  pc.m_nMaxLines = wl.m_nliCur;
  // 格式化操作完成%100
  if ( nPercentCompleted < 100 ) {
    m_pdb->EventSendMsg( PFX_EN_WORD_WRAPPING, 100 );
  }
}
void CPfxEditView::PrintHeader( HDC hDC, CPfxEditViewPrintContext& pc ) {
  CDCH pDC = hDC;
  CRect rectHeader( &pc.m_rectHeader );
  if ( pc.IsPrintHeaderSeparator() && ( pc.PenSeparateLine() != NULL ) ) {
    pDC.SelectObject( pc.GetPenSeparateLine() );
    pDC.MoveTo( rectHeader.left, rectHeader.bottom );
    pDC.LineTo( rectHeader.right, rectHeader.bottom );
    rectHeader.bottom -= pc.m_nWidthSeparateLine;
  }
  CString& strText = pc.m_strHeader;
  for ( LONG nEnd, nStart = 0, nLength = strText.GetLength(); ; ) {
    nEnd = strText.Find( PFX_EC_PRINT_MACRO_CRLF, nStart );
    if ( nEnd == -1 ) {
      nEnd = nLength;
    }
    PrintHeaderFooterText( strText.Mid( nStart, ( nEnd - nStart ) ), rectHeader, pDC, pc );
    if ( nEnd >= nLength ) {
      break;
    }
    nStart = nEnd + _tcslen( PFX_EC_PRINT_MACRO_CRLF );
    rectHeader.OffsetRect( 0, pc.m_nHeaderAndFooterLineHeight );
  }
}
void CPfxEditView::PrintFooter( HDC hDC, CPfxEditViewPrintContext& pc ) {
  CDCH pDC = hDC;
  CRect rectFooter( &pc.m_rectFooter );
  if ( pc.IsPrintFooterSeparator() && ( pc.PenSeparateLine() != NULL ) ) {
    pDC.SelectObject( pc.GetPenSeparateLine() );
    pDC.MoveTo( rectFooter.left, rectFooter.top );
    pDC.LineTo( rectFooter.right, rectFooter.top );
    rectFooter.top += pc.m_nWidthSeparateLine;
  }
  CString& strText = pc.m_strFooter;
  for ( LONG nEnd, nStart = 0, nLength = strText.GetLength(); ; ) {
    nEnd = strText.Find( PFX_EC_PRINT_MACRO_CRLF, nStart );
    if ( nEnd == -1 ) {
      nEnd = nLength;
    }
    PrintHeaderFooterText( strText.Mid( nStart, ( nEnd - nStart ) ), rectFooter, pDC, pc );
    if ( nEnd >= nLength ) {
      break;
    }
    nStart = nEnd + _tcslen( PFX_EC_PRINT_MACRO_CRLF );
    rectFooter.OffsetRect( 0, pc.m_nHeaderAndFooterLineHeight );
  }
}
void CPfxEditView::PrintLineNumber( LONG nLine, HDC hDC, CPfxEditViewPrintContext& pc ) {
  CDCH pDC = hDC;
  if ( ( !pc.IsPrintLineNumber() ) || ( nLine == PFX_EC_INVALID_LINE ) ) {
    return ;
  }
  TCHAR sz[ 16 ];
  LONG cchText = _stprintf( sz, _T( "%ld" ), ( nLine + 1 ) );
  CRect rect( pc.m_rectLineNumber.left, pc.m_rectLine.top, pc.m_rectLineNumber.right, pc.m_rectLine.bottom );
  CPoint pt( rect.right, rect.top + ( rect.Height() - pc.m_nLineHeight ) / 2 );
  pDC.SetTextAlign( TA_RIGHT | TA_TOP | TA_NOUPDATECP );
  pDC.SetTextColor( pc.COLOR( kcLineNumber ) );
  ::ExtTextOut( hDC, pt.x, pt.y, ETO_CLIPPED, &rect, sz, cchText, NULL );
}
void CPfxEditView::PrintMargin( const CRect& rectDraw, HDC hDC, CPfxEditViewPrintContext& pc ) {
  CDCH pDC = hDC;
  // 如果是预览，绘制页边距
  if ( !pc.IsPreview() || !pc.IsPrintMargin() ) {
    return ;
  }
  pDC.SelectObject( pc.GetPenMargin() );
  // ------------->
  pDC.MoveTo( rectDraw.left, pc.m_rectDraw.top );
  pDC.LineTo( rectDraw.right, pc.m_rectDraw.top );
  // -------------|
  //        |
  //        V
  pDC.MoveTo( pc.m_rectDraw.right, rectDraw.top );
  pDC.LineTo( pc.m_rectDraw.right, rectDraw.bottom );
  // -------------|
  //        |
  //        |
  // <-------------
  pDC.MoveTo( rectDraw.right, pc.m_rectDraw.bottom );
  pDC.LineTo( rectDraw.left, pc.m_rectDraw.bottom );
  // -------------|
  // ^            |
  // |            |
  // |------------|
  pDC.MoveTo( pc.m_rectDraw.left, rectDraw.bottom );
  pDC.LineTo( pc.m_rectDraw.left, rectDraw.top );
}
void CPfxEditView::PrintHilighted( HDC hDC, CPfxEditViewPrintContext& pc ) {
  CDCH pDC = hDC;
  const CPfxEditLineInfo * pli = reinterpret_cast<const CPfxEditLineInfo*>( pc.m_cHeap.GetBuffer() );
  const CPfxEditLineInfo* pliEnd = pli;
  // 分析这页中要打印的行
  ParserPreParse( pc.m_nCurPageLastLine, ( pc.m_nMaxLines - 1 ), 0, pc.m_nLineLastParsed, const_cast<CPfxEditLineInfo*>( pli ) );
  pli += pc.m_nCurPageFirstLine;
  pliEnd += pc.m_nCurPageLastLine;
  LONG nTabSize = pc.m_nTabSize;
  LONG nLineHeight = pc.m_nLineHeight;
  pc.m_rectLine.CopyRect( &pc.m_rectText );
  pc.m_rectLine.bottom = pc.m_rectText.top + nLineHeight;
  for ( ; pli < pliEnd; pli++ ) {
    // 绘制行号
    PrintLineNumber( pli->nIndex, pDC, pc );
    // 绘制文本
    if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
      PrintHilightedLine( pli, pDC, pc );
    }
    pc.m_rectLine.OffsetRect( 0, nLineHeight );
  }
}
void CPfxEditView::PrintHilightedString1( LPCTSTR pData, LONG nLen, HDC hDC, CPfxEditViewPrintContext& pc ) {
  CDCH pDC = hDC;
  if ( ( pData == NULL ) || ( nLen <= 0 ) ) {
    return ;
  }
  LONG i = m_pti->m_mapKeywords.Lookup( pData, nLen );
  COLORREF clr = pc.COLOR( ( i != -1 ) ? i : kcText );
  if ( clr != pc.m_clrCurText ) {
    pc.m_clrCurText = clr;
    pDC.SetTextColor( clr );
  }
  ::ExtTextOut( hDC, 0, 0, ETO_CLIPPED, &pc.m_rectLine, pData, nLen, NULL );
  ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
  pc.m_ptCurPos.y = pc.m_rectLine.top;
  pDC.MoveTo( pc.m_ptCurPos );
}
void CPfxEditView::PrintHilightedString2( LPCTSTR pData, LONG nLen, LONG& rnLogChar, COLORREF color, HDC hDC, CPfxEditViewPrintContext& pc ) {
  if ( ( pData == NULL ) || ( nLen == 0 ) ) {
    return ;
  }
  CDCH pDC = hDC;
  LONG x2 = pc.m_rectText.right;
  pc.m_clrCurText = color;
  pDC.SetTextColor( pc.m_clrCurText );
  LPCTSTR pCur = pData;
  LPCTSTR pTmp = pCur;
  const LPCTSTR pEnd = pData + nLen;
  while ( ( pCur < pEnd ) && ( x2 > pc.m_ptCurPos.x ) ) {
#ifdef _MBCS
    if ( PFX_EC_IS_LEAD_BYTE( *pCur ) ) {
      rnLogChar += PFX_EC_MAX_MB_LEN;
      pCur += PFX_EC_MAX_MB_LEN;
      continue;
    }
#endif
    if ( *pCur != PFX_EC_CHR_TAB ) {
      rnLogChar++;
      pCur++;
    } else {
      if ( pCur > pTmp ) {
        ::ExtTextOut( hDC, 0, 0, ETO_CLIPPED, &pc.m_rectLine, pTmp, ( pCur - pTmp ), NULL );
      }
      LONG nTabSize = pc.m_nTabSize - rnLogChar % pc.m_nTabSize;
      pTmp = ++pCur;
      rnLogChar += nTabSize;
      // 如果使用按比例缩放的方法决定制表符的宽度，在打印
      // 预览的时候可能会出现不一致，所以要由GDI 来计算制
      // 表宽度
      TCHAR sz[ 1 ];
      sz[ 0 ] = PFX_EC_CHR_SPACE;
      CSize size;
      ::GetTextExtentPoint32( hDC, sz, 1, &size );
      ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
      pc.m_ptCurPos.x += nTabSize * size.cx;
      pc.m_ptCurPos.y = pc.m_rectLine.top;
      pDC.MoveTo( pc.m_ptCurPos );
    }
  }
  if ( pCur > pTmp ) {
    ::ExtTextOut( hDC, 0, 0, ETO_CLIPPED, &pc.m_rectLine, pTmp, ( min( pEnd, pCur ) - pTmp ), NULL );
    ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
    pc.m_ptCurPos.y = pc.m_rectLine.top;
    pDC.MoveTo( pc.m_ptCurPos );
  }
}
LPCTSTR CPfxEditView::PrintHilightedStringBaseOnCookie( LPCTSTR pData, LONG nLen, LONG& rnLogChar, DWORD dwCookie, HDC hDC, CPfxEditViewPrintContext& pc ) {
  const LPCTSTR pStart = pData;
  const LPCTSTR pEnd = pStart + nLen;
  CDCH pDC = hDC;
  // 由上一行开始使用了行注释：
  // abcdefg
  // hijklmn
  // 这种情况只需要输出当前行的全部内容便可
  if ( dwCookie & PFX_EC_LF_COOKIE_LINE_COMMENT1 ) {
    PrintHilightedString2( pStart, ( pEnd - pStart ), rnLogChar, pc.COLOR( kcLineComment1 ), pDC, pc );
    return NULL;
  }
  if ( dwCookie & PFX_EC_LF_COOKIE_LINE_COMMENT2 ) {
    PrintHilightedString2( pStart, ( pEnd - pStart ), rnLogChar, pc.COLOR( kcLineComment2 ), pDC, pc );
    return NULL;
  }
  // 这一行的开头部分或全部是块注释的一部分：
  // abcdefg /* this is block comment
  // samples */ hijklmn
  if ( dwCookie & PFX_EC_LF_COOKIE_BLOCK_COMMENT1 ) {
    LPCTSTR pFound = ParserGetMarkerCloseEndPosition( pStart, pStart, pEnd, false, m_pti->GetMarkerPair( mpBlockComment1 ) );
    if ( pFound == NULL )           //      没有找到？一直输出到行末
    {
      pFound = pEnd;
    }
    PrintHilightedString2( pStart, ( pFound - pStart ), rnLogChar, pc.COLOR( kcBlockComment1 ), pDC, pc );
    return ( ( pFound == pEnd ) ? NULL : pFound );
  }
  if ( dwCookie & PFX_EC_LF_COOKIE_BLOCK_COMMENT2 ) {
    LPCTSTR pFound = ParserGetMarkerCloseEndPosition( pStart, pStart, pEnd, false, m_pti->GetMarkerPair( mpBlockComment2 ) );
    if ( pFound == NULL )           //      没有找到？一直输出到行末
    {
      pFound = pEnd;
    }
    PrintHilightedString2( pStart, ( pFound - pStart ), rnLogChar, pc.COLOR( kcBlockComment2 ), pDC, pc );
    return ( ( pFound == pEnd ) ? NULL : pFound );
  }
  if ( dwCookie & PFX_EC_LF_COOKIE_QUOTED_STRING1 ) {
    LPCTSTR pFound = ParserGetMarkerCloseEndPosition( pStart, pStart, pEnd, false, m_pti->GetMarkerPair( mpQuotedString1 ) );
    if ( pFound == NULL )           //      没有找到？一直输出到行末
    {
      pFound = pEnd;
    }
    PrintHilightedString2( pStart, ( pFound - pStart ), rnLogChar, pc.COLOR( kcQuotedString1 ), pDC, pc );
    return ( ( pFound == pEnd ) ? NULL : pFound );
  }
  if ( dwCookie & PFX_EC_LF_COOKIE_QUOTED_STRING2 ) {
    LPCTSTR pFound = ParserGetMarkerCloseEndPosition( pStart, pStart, pEnd, false, m_pti->GetMarkerPair( mpQuotedString2 ) );
    if ( pFound == NULL )           //      没有找到？一直输出到行末
    {
      pFound = pEnd;
    }
    PrintHilightedString2( pStart, ( pFound - pStart ), rnLogChar, pc.COLOR( kcQuotedString2 ), pDC, pc );
    return ( ( pFound == pEnd ) ? NULL : pFound );
  }
  return pStart;
}
void CPfxEditView::PrintHilightedLine( const CPfxEditLineInfo* pli, HDC hDC, CPfxEditViewPrintContext& pc ) {
  ASSERT( !PFX_EC_IS_EMPTY_LINE( pli ) );
  CDCH pDC = hDC;
  LONG x2 = pc.m_rectLine.right;
  LONG nTabSize = 0;
  LONG nLogChar = pli->nFirstChar;
  DWORD dwCookie = pli->dwFlags & PFX_EC_LF_COOKIE_ALL;
  pc.m_ptCurPos.x = pc.m_rectLine.left;
  pc.m_ptCurPos.y = pc.m_rectLine.top;
  const LPCTSTR pStart = pli->pData;
  const LPCTSTR pEnd = pStart + pli->nLen;
  LPCTSTR pCur = pStart;
  pc.m_clrCurText = pc.COLOR( kcText );
  pDC.SetTextColor( pc.m_clrCurText );
  pDC.SetTextAlign( TA_LEFT | TA_TOP | TA_UPDATECP );
  pDC.MoveTo( pc.m_ptCurPos );
  pCur = PrintHilightedStringBaseOnCookie( pStart, pli->nLen, nLogChar, dwCookie, pDC, pc );
  // 如果这一行是上一行COOKIE的延续，就不需要继续
  if ( pCur == NULL ) {
    return ;
  }
  // 每次分析一个字符的时候将该字符和下面的标识符的第一个字符比较，如
  // 果不等就则不需要继续比较，这样可以加快分析速度
  CPfxEditViewMarkerChar mc;
  ParserGetFirstCharOfMarkerStart( mc );
  LPCTSTR pTmp = pCur;
  while ( ( pCur < pEnd ) && ( x2 > pc.m_ptCurPos.x ) ) {
    BOOL bLeadByte = PFX_EC_IS_LEAD_BYTE( *pCur );
    WCHAR wc = ( ( !bLeadByte || ( ( pCur + PFX_EC_MAX_MB_LEN ) > pEnd ) ) ? ( ( WCHAR ) * pCur ) : ( *( LPWSTR ) pCur ) );
    // 行注释1
    if ( ( ( wc == mc.m_cLowerLineComment1 ) || ( wc == mc.m_cUpperLineComment1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpLineComment1 ) ) ) {
      PrintHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      PrintHilightedString2( pCur, ( pEnd - pCur ), nLogChar, pc.COLOR( kcLineComment1 ), pDC, pc );
      pCur = pTmp = pEnd;
      break;
    }
    // 行注释2
    if ( ( ( wc == mc.m_cLowerLineComment2 ) || ( wc == mc.m_cUpperLineComment2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpLineComment2 ) ) ) {
      PrintHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      PrintHilightedString2( pCur, ( pEnd - pCur ), nLogChar, pc.COLOR( kcLineComment2 ), pDC, pc );
      pCur = pTmp = pEnd;
      break;
    }
    // 块注释1
    if ( ( ( wc == mc.m_cLowerBlockComment1 ) || ( wc == mc.m_cUpperBlockComment1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpBlockComment1 ) ) ) {
      PrintHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      pTmp = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpBlockComment1 ) );
      if ( pTmp == NULL )           //      没有找到？一直输出到行末
      {
        pTmp = pEnd;
      }
      PrintHilightedString2( pCur, ( pTmp - pCur ), nLogChar, pc.COLOR( kcBlockComment1 ), pDC, pc );
      pCur = pTmp;
      continue;
    }
    // 块注释2
    if ( ( ( wc == mc.m_cLowerBlockComment2 ) || ( wc == mc.m_cUpperBlockComment2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpBlockComment2 ) ) ) {
      PrintHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      pTmp = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpBlockComment2 ) );
      if ( pTmp == NULL )           //      没有找到？一直输出到行末
      {
        pTmp = pEnd;
      }
      PrintHilightedString2( pCur, ( pTmp - pCur ), nLogChar, pc.COLOR( kcBlockComment2 ), pDC, pc );
      pCur = pTmp;
      continue;
    }
    // 常量字符串1
    if ( ( ( wc == mc.m_cLowerQuotedString1 ) || ( wc == mc.m_cUpperQuotedString1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpQuotedString1 ) ) ) {
      PrintHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      pTmp = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpQuotedString1 ) );
      if ( pTmp == NULL )           //      没有找到？一直输出到行末
      {
        pTmp = pEnd;
      }
      PrintHilightedString2( pCur, ( pTmp - pCur ), nLogChar, pc.COLOR( kcQuotedString1 ), pDC, pc );
      pCur = pTmp;
      continue;
    }
    // 常量字符串2
    if ( ( ( wc == mc.m_cLowerQuotedString2 ) || ( wc == mc.m_cUpperQuotedString2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpQuotedString2 ) ) ) {
      PrintHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      pTmp = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpQuotedString2 ) );
      if ( pTmp == NULL )           //      没有找到？一直输出到行末
      {
        pTmp = pEnd;
      }
      PrintHilightedString2( pCur, ( pTmp - pCur ), nLogChar, pc.COLOR( kcQuotedString2 ), pDC, pc );
      pCur = pTmp;
      continue;
    }
    // 如果这个字符不是块的标识符，下面将会分析它是否是关键字的
    // 一部分，或者是空白字符，或者是单词定界符等
    if ( !IsWhitespace( wc ) && !IsDelimiter( wc ) ) {
      if ( bLeadByte ) {
        pCur += PFX_EC_MAX_MB_LEN;
        nLogChar += PFX_EC_MAX_MB_LEN;
      } else {
        pCur++;
        nLogChar++;
      }
    } else {
      if ( pCur > pTmp )           //      绘制之前的部分
      {
        PrintHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
        pTmp = pCur;
      }
      // 如果使用按比例缩放的方法决定制表符的宽度，在打印
      // 预览的时候可能会出现不一致，所以要由GDI 来计算制
      // 表宽度
      if ( ( wc == PFX_EC_CHR_SPACE ) || ( wc == PFX_EC_CHR_TAB ) ) {
        pCur++;
        PrintHilightedString2( pTmp, 1, nLogChar, pc.m_clrCurTextBkgnd, pDC, pc );
      } else // Delimiter!
      {
        if ( bLeadByte ) {
          pCur += PFX_EC_MAX_MB_LEN;
          nLogChar += PFX_EC_MAX_MB_LEN;
        } else {
          pCur++;
          nLogChar++;
        }
        //  有些编程语言的关键字中可能含有单词定界符
        //  例如汇编语言经常有“.XLIST，.XALL”， 等
        //  伪指令，C++语言有“==，!=，->，>>” 等运
        //  算符，因此当遇到单词定界符时应该再分析一
        //  下它后面的单词与它是否是一个关键字
        LPCTSTR pTmpData = pCur;
        LONG nTmpLogChar = nLogChar;
        while ( ( pTmpData < pEnd )
                && !IsWhitespace( *pTmpData ) ) {
          LONG nStep = ( PFX_EC_IS_LEAD_BYTE( *pTmpData ) ? PFX_EC_MAX_MB_LEN : 1 );
          //   如果遇到的第一个字符就是单词定界
          //   符，就包括该定界符，否则不包括它
          if ( IsDelimiter( pTmpData, pEnd ) ) {
            if ( pTmpData == pCur ) {
              pTmpData += nStep;
              nTmpLogChar += nStep;
            }
            break;
          }
          pTmpData += nStep;
          nTmpLogChar += nStep;
        }
        pTmpData = min( pTmpData, pEnd );
        if ( ( pTmpData != pCur )
             && ParserIsKeyword( pTmp, ( pTmpData - pTmp ) ) ) {
          pCur = pTmpData;
          nLogChar = nTmpLogChar;
        }
        PrintHilightedString1( pTmp, ( min( pEnd, pCur ) - pTmp ), pDC, pc );
      }
      pTmp = pCur;
    }
  }
  // 绘制剩余的部分，由于没有考虑到自动换行时子行的问题，所以当一个关
  // 键字被强行折断的时候，它将不会被正确地分析和正确的显示
  if ( ( pCur > pTmp ) && ( x2 > pc.m_ptCurPos.x ) ) {
    PrintHilightedString1( pTmp, ( min( pEnd, pCur ) - pTmp ), pDC, pc );
  }
}
void CPfxEditView::PrintPlainText( HDC hDC, CPfxEditViewPrintContext& pc ) {
  const CPfxEditLineInfo * pli =
    reinterpret_cast<const CPfxEditLineInfo*>( pc.m_cHeap.GetBuffer() );
  const CPfxEditLineInfo* pliEnd = pli;
  CDCH pDC = hDC;
  pli += pc.m_nCurPageFirstLine;
  pliEnd += pc.m_nCurPageLastLine;
  LONG nTabSize = pc.m_nTabSize;
  LONG nLineHeight = pc.m_nLineHeight;
  pc.m_rectLine.CopyRect( &pc.m_rectText );
  pc.m_rectLine.bottom = pc.m_rectText.top + nLineHeight;
  for ( ; pli < pliEnd; pli++ ) {
    // 绘制行号
    PrintLineNumber( pli->nIndex, pDC, pc );
    // 绘制文本
    if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
      PrintPlainTextLine( pli, pDC, pc );
    }
    pc.m_rectLine.OffsetRect( 0, nLineHeight );
  }
}
void CPfxEditView::PrintPlainTextLine( const CPfxEditLineInfo* pli, HDC hDC, CPfxEditViewPrintContext& pc ) {
  ASSERT( !PFX_EC_IS_EMPTY_LINE( pli ) );
  CDCH pDC = hDC;
  LONG x2 = pc.m_rectLine.right;
  LONG nTabSize = 0;
  LONG nLogChar = pli->nFirstChar;
  pc.m_ptCurPos.x = pc.m_rectLine.left;
  pc.m_ptCurPos.y = pc.m_rectLine.top;
  LPCTSTR pCur = pli->pData;
  LPCTSTR pTmp = pCur;
  const LPCTSTR pEnd = pCur + pli->nLen;
  pDC.SetTextAlign( TA_LEFT | TA_TOP | TA_UPDATECP );
  pDC.SetTextColor( pc.COLOR( kcText ) );
  pDC.MoveTo( pc.m_ptCurPos );
  while ( ( pCur < pEnd ) && ( x2 > pc.m_ptCurPos.x ) ) {
#ifdef _MBCS
    if ( PFX_EC_IS_LEAD_BYTE( *pCur ) ) {
      nLogChar += PFX_EC_MAX_MB_LEN;
      pCur += PFX_EC_MAX_MB_LEN;
      continue;
    }
#endif
    if ( *pCur != PFX_EC_CHR_TAB ) {
      nLogChar++;
      pCur++;
    } else {
      if ( pCur > pTmp ) {
        ::ExtTextOut( hDC, 0, 0, ETO_CLIPPED, &pc.m_rectLine, pTmp, pCur - pTmp, NULL );
      }
      LONG nTabSize = pc.m_nTabSize - nLogChar % pc.m_nTabSize;
      pTmp = ++pCur;
      nLogChar += nTabSize;
      // 如果使用按比例缩放的方法决定制表符的宽度，在打印
      // 预览的时候可能会出现不一致，所以要由GDI 来计算制
      // 表宽度
      TCHAR sz[ 1 ];
      sz[ 0 ] = PFX_EC_CHR_SPACE;
      CSize size;
      ::GetTextExtentPoint32( hDC, sz, 1, &size );
      ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
      pc.m_ptCurPos.x += nTabSize * size.cx;
      pc.m_ptCurPos.y = pc.m_rectLine.top;
      pDC.MoveTo( pc.m_ptCurPos );
    }
  }
  if ( pCur > pTmp ) {
    ::ExtTextOut( hDC, 0, 0, ETO_CLIPPED, &pc.m_rectLine, pTmp, min( pEnd, pCur ) - pTmp, NULL );
  }
}
void CPfxEditView::PrintHeaderFooterText( const CString& strText, const CRect& rectDraw, HDC hDC, CPfxEditViewPrintContext& pc ) {
  struct CAlignParam {
    UINT nAlign;
    LPCTSTR pszAlignParam;
  };
  static const CAlignParam s_align[] = { {
                                           TA_LEFT, PFX_EC_PRINT_MACRO_ALIGN_LEFT
                                         }
                                         , {TA_CENTER, PFX_EC_PRINT_MACRO_ALIGN_CENTER}, {TA_RIGHT, PFX_EC_PRINT_MACRO_ALIGN_RIGHT},
                                       };
  CString strLeft;
  CString strCenter;
  CString strRight;
  // 首先分类和归并所有向左、向右、居中对齐的部分
  LONG i;
  LONG nStart = 0;
  UINT nAlign = TA_LEFT;
  for ( ;; ) {
    LONG nEnd = INT_MAX;
    const CAlignParam* pAlign = NULL;
    for ( i = 0; i < PFX_EC_SIZEOF( s_align ); i++ ) {
      LONG nPos = strText.Find( s_align[ i ].pszAlignParam, nStart );
      if ( ( nPos != -1 ) && ( nPos < nEnd ) ) {
        nEnd = nPos;
        pAlign = &s_align[ i ];
      }
    }
    nEnd = min( strText.GetLength(), nEnd );
    switch ( nAlign ) {
    case TA_CENTER:
      strCenter += strText.Mid( nStart, ( nEnd - nStart ) );
      break;
    case TA_RIGHT:
      strRight += strText.Mid( nStart, ( nEnd - nStart ) );
      break;
    case TA_LEFT:
    default:
      strLeft += strText.Mid( nStart, ( nEnd - nStart ) );
      break;
    }
    if ( pAlign == NULL ) {
      break;
    }
    nStart = nEnd + _tcslen( pAlign->pszAlignParam );
    nAlign = pAlign->nAlign;
  }
  // 格式化各种参数
  CString* arrStr[] = {&strLeft, &strCenter, &strRight};
  for ( i = 0; i < PFX_EC_SIZEOF( arrStr ); i++ ) {
    CString& str = *arrStr[ i ];
    if ( str.IsEmpty() ) {
      continue;
    }
    TCHAR sz[ MAX_PATH ];
    // 实时格式化当前页数
    if ( str.Find( PFX_EC_PRINT_MACRO_CURRENT_PAGE ) != -1 ) {
      _stprintf( sz, _T( "%d" ), ( pc.m_nCurPage + 1 ) );
      str.Replace( PFX_EC_PRINT_MACRO_CURRENT_PAGE, sz );
    }
    // 总页数
    if ( str.Find( PFX_EC_PRINT_MACRO_TOTAL_PAGES ) != -1 ) {
      _stprintf( sz, _T( "%d" ), pc.m_nTotalPages );
      str.Replace( PFX_EC_PRINT_MACRO_TOTAL_PAGES, sz );
    }
    // 时间
    if ( str.Find( PFX_EC_PRINT_MACRO_TIME ) != -1 ) {
      ::GetTimeFormat( LOCALE_USER_DEFAULT, TIME_NOSECONDS, &pc.m_time, NULL, sz, PFX_EC_SIZEOF( sz ) );
      str.Replace( PFX_EC_PRINT_MACRO_TIME, sz );
    }
    // 日期
    if ( str.Find( PFX_EC_PRINT_MACRO_DATE ) != -1 ) {
      ::GetDateFormat( LOCALE_USER_DEFAULT, DATE_LONGDATE, &pc.m_time, NULL, sz, PFX_EC_SIZEOF( sz ) );
      str.Replace( PFX_EC_PRINT_MACRO_DATE, sz );
    }
    // 文件名
    if ( str.Find( PFX_EC_PRINT_MACRO_FILE_NAME ) != -1 ) {
      TCHAR szFileExt[ MAX_PATH ];
      _tsplitpath( pc.m_strPathName, NULL, NULL, sz, szFileExt );
      _tcscat( sz, szFileExt );
      str.Replace( PFX_EC_PRINT_MACRO_FILE_NAME, sz );
    }
    // 路径名
    if ( str.Find( PFX_EC_PRINT_MACRO_PATH_NAME ) != -1 ) {
      str.Replace( PFX_EC_PRINT_MACRO_PATH_NAME, pc.m_strPathName );
    }
    // 制表符
    if ( str.Find( PFX_EC_PRINT_MACRO_TAB ) != -1 ) {
      str.Replace( PFX_EC_PRINT_MACRO_TAB, _T( "\t" ) );
    }
  }
  CRect rect( &rectDraw );
  CSize size;
  CPoint pt;
  CDCH pDC = hDC;
  LONG nLogChar = 0;
  LONG nTabSize = 0;
  pDC.SetBkMode( TRANSPARENT );
  pDC.SetTextColor( pc.COLOR( kcHeaderAndFooter ) );
  // 绘制左边部分信息串
  if ( !strLeft.IsEmpty() ) {
    pt.x = rect.left;
    pt.y = rect.top;
    LPCTSTR pStart = strLeft;
    LPCTSTR pCur = pStart;
    LPCTSTR pEnd = pStart + strLeft.GetLength();
    pDC.SetTextAlign( TA_TOP | TA_LEFT | TA_UPDATECP );
    pDC.MoveTo( pt );
    while ( pCur < pEnd ) {
      if ( PFX_EC_IS_LEAD_BYTE( *pCur ) ) {
        nLogChar += PFX_EC_MAX_MB_LEN;
        pCur += PFX_EC_MAX_MB_LEN;
        continue;
      }
      if ( *pCur != PFX_EC_CHR_TAB ) {
        nLogChar++;
        pCur++;
      } else {
        ::ExtTextOut( hDC, 0, 0, ETO_CLIPPED, &rect, pStart, pCur - pStart, NULL );
        ::GetCurrentPositionEx( hDC, &pt );
        nTabSize = pc.m_nTabSize - nLogChar % pc.m_nTabSize;
        pt.x += ( nTabSize * pc.m_nCharWidthSpace );
        nLogChar += nTabSize;
        pStart = ++pCur;
        pDC.MoveTo( pt.x, rect.top );
      }
    }
    if ( pStart < pEnd ) {
      ::ExtTextOut( hDC, 0, 0, ETO_CLIPPED, &rect, pStart, pEnd - pStart, NULL );
    }
  }
  // 绘制中间部分信息串
  if ( !strCenter.IsEmpty() ) {
    nTabSize = pc.m_nTabSize * pc.m_nCharWidthSpace;
    DWORD dwTabbedExt = ::GetTabbedTextExtent( hDC, strCenter, strCenter.GetLength(), 1, reinterpret_cast<LPINT>( &nTabSize ) );
    size.cx = LOWORD( dwTabbedExt );
    size.cy = HIWORD( dwTabbedExt );
    pt.x = rect.left + ( rect.Width() - size.cx ) / 2;
    pt.y = rect.top;
    LPCTSTR pStart = strCenter;
    LPCTSTR pCur = pStart;
    LPCTSTR pEnd = pStart + strCenter.GetLength();
    pDC.SetTextAlign( TA_TOP | TA_LEFT | TA_UPDATECP );
    pDC.MoveTo( pt );
    while ( pCur < pEnd ) {
#ifdef _MBCS
      if ( PFX_EC_IS_LEAD_BYTE( *pCur ) ) {
        nLogChar += PFX_EC_MAX_MB_LEN;
        pCur += PFX_EC_MAX_MB_LEN;
        continue;
      }
#endif
      if ( *pCur != PFX_EC_CHR_TAB ) {
        nLogChar++;
        pCur++;
      } else {
        ::ExtTextOut( hDC, 0, 0, ETO_CLIPPED, &rect, pStart, pCur - pStart, NULL );
        ::GetCurrentPositionEx( hDC, &pt );
        nTabSize = pc.m_nTabSize - nLogChar % pc.m_nTabSize;
        pt.x += ( nTabSize * pc.m_nCharWidthSpace );
        nLogChar += nTabSize;
        pStart = ++pCur;
        pDC.MoveTo( pt.x, rect.top );
      }
    }
    if ( pStart < pEnd ) {
      ::ExtTextOut( hDC, 0, 0, ETO_CLIPPED, &rect, pStart, pEnd - pStart, NULL );
    }
  }
  // 绘制右边部分信息串
  if ( !strRight.IsEmpty() ) {
    pt.x = rect.right;
    pt.y = rect.top;
    LPCTSTR pStart = strRight;
    LPCTSTR pEnd = pStart + strRight.GetLength();
    LPCTSTR pCur = pEnd - 1;
    LPCTSTR pTmp = pCur;
    pDC.SetTextAlign( TA_TOP | TA_LEFT | TA_NOUPDATECP );
    for ( ;; ) {
      if ( *pTmp == PFX_EC_CHR_TAB ) {
        ::GetTextExtentPoint32( hDC, pCur, ( pEnd - pCur ), &size );
        pt.x -= size.cx;
        ::ExtTextOut( hDC, pt.x, pt.y, ETO_CLIPPED, &rect, pCur, ( pEnd - pCur ), NULL );
        nTabSize = pc.m_nTabSize - nLogChar % pc.m_nTabSize;
        nLogChar += nTabSize;
        pt.x -= ( nTabSize * pc.m_nCharWidthSpace );
        pEnd = pTmp;
      } else {
        if ( pTmp == pStart ) {
          ::GetTextExtentPoint32( hDC, pTmp, ( pEnd - pTmp ), &size );
          pt.x -= size.cx;
          ::ExtTextOut( hDC, pt.x, pt.y, ETO_CLIPPED, &rect, pTmp, ( pEnd - pTmp ), NULL );
          break;
        }
        nLogChar += ( pCur - pTmp );
      }
      pCur = pTmp;
      pTmp = GetPrevCharPos( pTmp, pStart );
    }
  }
}
BOOL CPfxEditView::SearchPreFind( CPfxEditViewFindReplace& rFindReplace ) {
  if ( rFindReplace.pszFindWhat == NULL ) {
    return false;
  }
  if ( rFindReplace.cchFindWhat == -1 ) {
    rFindReplace.cchFindWhat = _tcslen( rFindReplace.pszFindWhat );
  }
  if ( rFindReplace.cchFindWhat == 0 ) {
    return false;
  }
  // 如果不是匹配大小写，将要查找的字符串转换成小写
  if ( !rFindReplace.bMatchCase ) {
    LPTSTR pFindWhat =
      rFindReplace.strCaseBuffer.GetBufferSetLength( rFindReplace.cchFindWhat );
    _tcsncpy( pFindWhat, rFindReplace.pszFindWhat, rFindReplace.cchFindWhat );
    _tcslwr( pFindWhat );
    rFindReplace.pszFindWhat = pFindWhat;
  }
  // “全部标记”操作将忽略坐标参数
  if ( rFindReplace.bMarkAll ) {
    return true;
  }
  if ( rFindReplace.bDirectDown ) {
    rFindReplace.ptTo.y = m_pdb->TextGetLineUpperBound();
    rFindReplace.ptTo.x = m_pdb->TextGetLineLength( rFindReplace.ptTo.y );
  } else {
    rFindReplace.ptTo.y = 0;
    rFindReplace.ptTo.x = 0;
  }
  m_pdb->TextSubLineToMainLine( rFindReplace.ptTo );
  // 校验行、列是否合法
  rFindReplace.ptFrom.y = m_pdb->GetValidLine( rFindReplace.ptFrom.y );
  rFindReplace.ptFrom.x = m_pdb->GetValidChar( rFindReplace.ptFrom.y, rFindReplace.ptFrom.x );
  // 将绝对位置转换为相对位置
  m_pdb->TextAbsLineToSubLine( rFindReplace.ptFrom );
  // 检查是否在MBCS字符的中间
  VerifyPointerPosition( rFindReplace.ptFrom );
  // 子行偏移量转换成主行偏移量
  m_pdb->TextSubLineToMainLine( rFindReplace.ptFrom );
  return true;
}
// 找到返回TRUE，否则返回FALSE
// 参考CPfxEditViewFindReplace结构的描述
BOOL CPfxEditView::SearchPreReplace( CPfxEditViewFindReplace& rFindReplace ) {
  // pszReplaceWith可以为NULL也可以为零长度，这等于删除
  if ( ( rFindReplace.pszReplaceWith != NULL )
       && ( rFindReplace.cchReplaceWith == -1 ) ) {
    rFindReplace.cchReplaceWith = _tcslen( rFindReplace.pszReplaceWith );
  }
  if ( rFindReplace.bIgnoreFindWhat ) {
    ASSERT( !rFindReplace.bInSelection );
    // 校验行列是否合法
    rFindReplace.ptFrom.y = m_pdb->GetValidLine( rFindReplace.ptFrom.y );
    rFindReplace.ptFrom.x = m_pdb->GetValidChar( rFindReplace.ptFrom.y, rFindReplace.ptFrom.x );
    rFindReplace.ptTo.y = m_pdb->GetValidLine( rFindReplace.ptTo.y );
    rFindReplace.ptTo.x = m_pdb->GetValidChar( rFindReplace.ptTo.y, rFindReplace.ptTo.x );
    // 将绝对位置转换为相对位置
    m_pdb->TextAbsLineToSubLine( rFindReplace.ptFrom );
    m_pdb->TextAbsLineToSubLine( rFindReplace.ptTo );
    // 检查指定位置是否在MBCS字符的中间
    VerifyPointerPosition( rFindReplace.ptFrom );
    VerifyPointerPosition( rFindReplace.ptTo );
  } else {
    if ( rFindReplace.pszFindWhat == NULL ) {
      return false;
    }
    if ( rFindReplace.cchFindWhat == -1 ) {
      rFindReplace.cchFindWhat = _tcslen( rFindReplace.pszFindWhat );
    }
    if ( rFindReplace.cchFindWhat == 0 ) {
      return false;
    }
    // 如果不是匹配大小写，将要查找的字符串转换成小写
    if ( !rFindReplace.bMatchCase ) {
      LPTSTR pFindWhat =
        rFindReplace.strCaseBuffer.GetBufferSetLength( rFindReplace.cchFindWhat );
      _tcsncpy( pFindWhat, rFindReplace.pszFindWhat, rFindReplace.cchFindWhat );
      _tcslwr( pFindWhat );
      rFindReplace.pszFindWhat = pFindWhat;
    }
    if ( rFindReplace.bInSelection && IsTextSelected() && !IsColumnSelect() ) {
      GetSelRange( rFindReplace.ptFrom, rFindReplace.ptTo );
    } else {
      rFindReplace.ptFrom.y = 0;
      rFindReplace.ptFrom.x = 0;
      rFindReplace.ptTo.y = m_pdb->TextGetLineUpperBound();
      rFindReplace.ptTo.x = m_pdb->TextGetLineLength( rFindReplace.ptTo.y );
      // 子行偏移量转换成绝对位置
      m_pdb->TextSubLineToAbsLine( rFindReplace.ptFrom );
      m_pdb->TextSubLineToAbsLine( rFindReplace.ptTo );
    }
  }
  return true;
}
BOOL CPfxEditView::SearchReplaceNormal( CPfxEditViewFindReplace& rFindReplace ) {
  ASSERT( !IsWrapping() );
  CPoint ptFrom = rFindReplace.ptFrom;
  CPoint ptTo = rFindReplace.ptTo;
  // 当起始和结束行是同一行时，如果查找字符串的长度与替换字符串的长度
  // 不一致时结束点X的坐标就会改变，所以要随时更新结束点X坐标
  LONG cchDiffer = rFindReplace.cchReplaceWith - rFindReplace.cchFindWhat;
  BOOL bFound = false;
  while ( SearchFindNormalDown( rFindReplace ) ) {
    bFound = true;
    CharsReplace( rFindReplace.ptFrom, rFindReplace.ptTo, rFindReplace.pszReplaceWith, rFindReplace.cchReplaceWith, false );
    if ( ptTo.y == rFindReplace.ptFrom.y ) {
      ptTo.x += cchDiffer;
    }
    rFindReplace.ptTo = ptTo;
    m_ptEditPos = rFindReplace.ptFrom;
  }
  rFindReplace.ptFrom = ptFrom;
  rFindReplace.ptTo = ptTo;
  return bFound;
}
BOOL CPfxEditView::SearchReplaceWholeWord( CPfxEditViewFindReplace& rFindReplace ) {
  ASSERT( !IsWrapping() );
  CPoint ptFrom = rFindReplace.ptFrom;
  CPoint ptTo = rFindReplace.ptTo;
  // 当起始和结束行是同一行时，如果查找字符串的长度与替换字符串的长度
  // 不一致时结束点X的坐标就会改变，所以要随时更新结束点X坐标
  LONG cchDiffer = rFindReplace.cchReplaceWith - rFindReplace.cchFindWhat;
  BOOL bFound = false;
  while ( SearchFindWholeWordDown( rFindReplace ) ) {
    bFound = true;
    CharsReplace( rFindReplace.ptFrom, rFindReplace.ptTo, rFindReplace.pszReplaceWith, rFindReplace.cchReplaceWith, false );
    if ( ptTo.y == rFindReplace.ptFrom.y ) {
      ptTo.x += cchDiffer;
    }
    rFindReplace.ptTo = ptTo;
    m_ptEditPos = rFindReplace.ptFrom;
  }
  rFindReplace.ptFrom = ptFrom;
  rFindReplace.ptTo = ptTo;
  return bFound;
}
BOOL CPfxEditView::SearchReplaceRegExp( CPfxEditViewFindReplace& rFindReplace ) {
  ASSERT( !IsWrapping() );
  UNUSED( rFindReplace );
  ASSERT( false );
  return true;
}
BOOL CPfxEditView::SearchDoReplace( CPfxEditViewFindReplace& rFindReplace ) {
  CPfxEditViewWorkingState state( this, true );
  ASSERT( !IsWrapping() );
  BOOL bFound;
  if ( rFindReplace.bRegExp ) {
    bFound = SearchReplaceRegExp( rFindReplace );
  } else if ( rFindReplace.bMatchWholeWord ) {
    bFound = SearchReplaceWholeWord( rFindReplace );
  } else // 部分匹配
  {
    bFound = SearchReplaceNormal( rFindReplace );
  }
  if ( bFound && rFindReplace.bInSelection ) {
    m_ptEditPos = rFindReplace.ptTo;
    SelRange( rFindReplace.ptFrom );
  }
  return bFound;
}
BOOL CPfxEditView::SearchDoFind( CPfxEditViewFindReplace& rFindReplace ) {
  BOOL bFound;
  if ( rFindReplace.bRegExp ) {
    if ( rFindReplace.bDirectDown ) {
      // ????
      bFound = false;
    } else {
      // ????
      bFound = false;
    }
  } else if ( rFindReplace.bMatchWholeWord ) {
    if ( rFindReplace.bDirectDown ) {
      bFound = SearchFindWholeWordDown( rFindReplace );
    } else {
      bFound = SearchFindWholeWordUp( rFindReplace );
    }
  } else // 部分匹配
  {
    if ( rFindReplace.bDirectDown ) {
      bFound = SearchFindNormalDown( rFindReplace );
    } else {
      bFound = SearchFindNormalUp( rFindReplace );
    }
  }
  return bFound;
}
BOOL CPfxEditView::SearchFindMarkAll( CPfxEditViewFindReplace& rFindReplace ) {
  // 首先清除所有现有书签
  BookmarkClearAll();
  LPCTSTR pszFindWhat = rFindReplace.pszFindWhat;
  LONG cchFindWhat = rFindReplace.cchFindWhat;
  LONG nLineFrom = 0;
  LONG nLineTo = m_pdb->TextGetLineUpperBound();
  BOOL bFound = false;
  while ( ( nLineFrom <= nLineTo ) && ( nLineFrom != PFX_EC_INVALID_LINE ) ) {
    const CPfxEditLineInfo * pli = m_pdb->TextGetLineAt( nLineFrom );
    LPCTSTR pStart = pli->pData;
    LPCTSTR pEnd = pStart + pli->nActLen;
    LPCTSTR pFind = NULL;
    if ( rFindReplace.bMatchCase ) {
      if ( rFindReplace.bRegExp ) {
        //  ???????????????????
      } else if ( rFindReplace.bMatchWholeWord ) {
        pFind = SearchFindWholeWordStringCase( pStart, pEnd, pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
      } else {
        pFind = SearchFindNormalStringCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
      }
    } else {
      if ( rFindReplace.bRegExp ) {
        //  ??????????????
      } else if ( rFindReplace.bMatchWholeWord ) {
        pFind = SearchFindWholeWordStringNoCase( pStart, pEnd, pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
      } else {
        pFind = SearchFindNormalStringNoCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
      }
    }
    if ( pFind != NULL ) {
      CPoint pt( 0, nLineFrom );
      m_pdb->TextMainLineToAbsLine( pt );
      BookmarkToggle( true, pt.y, false );
      bFound = true;
    }
    nLineFrom = m_pdb->TextGetNextMainLine( nLineFrom );
  }
  rFindReplace.ptFrom.x = rFindReplace.ptTo.x = 0;
  rFindReplace.ptFrom.y = rFindReplace.ptTo.y = 0;
  Invalidate( false );
  return bFound;
}
// 匹配大小写但不匹配整字查找字符串
// 找到匹配对象返回第一个字符的地址，没有找到匹配的字符串返回NULL
LPCTSTR CPfxEditView::SearchFindNormalStringCase( LPCTSTR pData, LONG cch, LPCTSTR pszFindWhat, LONG cchFindWhat ) {
  ASSERT( cch >= 0 );
  ASSERT( cchFindWhat > 0 );
  ASSERT( pszFindWhat != NULL );
  if ( ( cch < cchFindWhat ) || ( pData == NULL ) ) {
    return NULL;
  }
  LPCTSTR pEndString = pData + cch;
  LPCTSTR pEndSearch = pData + ( cch - cchFindWhat );
  LPCTSTR pEndFindWhat = pszFindWhat + cchFindWhat;
  while ( pData <= pEndSearch ) {
    LPCTSTR s1 = pData;
    LPCTSTR s2 = pszFindWhat;
    // MBCS: ok to ++ since doing equality comparison.
    // [This depends on MBCS strings being "legal".]
    while ( ( s1 < pEndString )
            && ( s2 < pEndFindWhat )
            && ( *s1 == *s2 ) ) {
      s1++;
      s2++;
    }
    // OK，匹配！
    if ( s2 == pEndFindWhat ) {
      return pData;
    }
    pData += ( PFX_EC_IS_LEAD_BYTE( *pData ) ? PFX_EC_MAX_MB_LEN : 1 );
  }
  return NULL;
}
// 忽略大小写和非整字匹配查找字符串
// 找到匹配对象返回第一个字符的地址，没有找到匹配的字符串返回NULL
// 考虑到复杂度，本函数将区分“全角/半角”数字字符和字母字符
// 有关“全角/半角”数字字符和写字母字符的定义请参考c runtime "mbctype.c"
LPCTSTR CPfxEditView::SearchFindNormalStringNoCase( LPCTSTR pData, LONG cch, LPCTSTR pszFindWhat, LONG cchFindWhat ) {
  ASSERT( cch >= 0 );
  ASSERT( cchFindWhat > 0 );
  ASSERT( pszFindWhat != NULL );
  if ( ( cch < cchFindWhat ) || ( pData == NULL ) ) {
    return NULL;
  }
  LPCTSTR pEndString = pData + cch;
  LPCTSTR pEndSearch = pData + ( cch - cchFindWhat );
  LPCTSTR pEndFindWhat = pszFindWhat + cchFindWhat;
  while ( pData <= pEndSearch ) {
    LPCTSTR s1 = pData;
    LPCTSTR s2 = pszFindWhat;
    while ( ( s1 < pEndString ) && ( s2 < pEndFindWhat ) ) {
      WCHAR c1;
      if ( PFX_EC_IS_LEAD_BYTE( *s1 )
           && ( ( s1 + PFX_EC_MAX_MB_LEN ) <= pEndString ) ) {
        c1 = ( WCHAR ) * ( ( LPWSTR ) s1 );
        s1 += PFX_EC_MAX_MB_LEN;
      } else {
        c1 = ( WCHAR ) * s1;
        s1++;
        //  单纯地转换成拉丁小写字符，忽略全角数字字
        //  符和全角字母字符
        if ( ( c1 >= 'A' ) && ( c1 <= 'Z' ) ) {
          c1 += ( 'a' - 'A' );
        }
      }
      WCHAR c2;
      if ( PFX_EC_IS_LEAD_BYTE( *s2 )
           && ( ( s2 + PFX_EC_MAX_MB_LEN ) <= pEndFindWhat ) ) {
        c2 = ( WCHAR ) * ( ( LPWSTR ) s2 );
        s2 += PFX_EC_MAX_MB_LEN;
      } else {
        c2 = ( WCHAR ) * s2;
        s2++;
      }
      if ( c1 != c2 ) {
        goto ContinueCompareNextChar;
      }
    }
    // OK，匹配！
    if ( s2 >= pEndFindWhat ) {
      return pData;
    }
ContinueCompareNextChar:
    pData += ( PFX_EC_IS_LEAD_BYTE( *pData ) ? PFX_EC_MAX_MB_LEN : 1 );
  }
  return NULL;
}
// 按不是“规则表达式”也不是“整字匹配”的方式查找字符串
// 找到返回TRUE，rFindReplace将包含对象信息，失败返回FALSE
BOOL CPfxEditView::SearchFindNormalDown( CPfxEditViewFindReplace& rFindReplace ) {
  LPCTSTR pStart;
  LPCTSTR pEnd;
  LPCTSTR pFind;
  const CPfxEditLineInfo* pli;
  CPoint ptFrom = rFindReplace.ptFrom;
  CPoint ptTo = rFindReplace.ptTo;
  LONG cchFindWhat = rFindReplace.cchFindWhat;
  LPCTSTR pszFindWhat = rFindReplace.pszFindWhat;
  if ( rFindReplace.bMatchCase ) {
    // 如果查找范围在同一行内
    if ( ptFrom.y == ptTo.y ) {
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pStart = pli->pData + ptFrom.x;
      pEnd = pli->pData + ptTo.x;
      pFind = SearchFindNormalStringCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
    }
    // 查找范围不在同一行内
    else {
      // 搜索第一行
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pStart = pli->pData + ptFrom.x;
      pEnd = pli->pData + pli->nActLen;
      pFind = SearchFindNormalStringCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
      // 搜索中间行
      while ( pFind == NULL ) {
        ptFrom.y = m_pdb->TextGetNextMainLine( ptFrom.y );
        if ( ( ptFrom.y == PFX_EC_INVALID_LINE )
             || ( ptFrom.y >= ptTo.y ) ) {
          break;
        }
        pli = m_pdb->TextGetLineAt( ptFrom.y );
        pFind = SearchFindNormalStringCase( pli->pData, pli->nActLen, pszFindWhat, cchFindWhat );
      }
      // 搜索最后行
      if ( pFind == NULL ) {
        ptFrom.y = ptTo.y;
        pli = m_pdb->TextGetLineAt( ptTo.y );
        pFind = SearchFindNormalStringCase( pli->pData, ptTo.x, pszFindWhat, cchFindWhat );
      }
    }
  }
  // 匹配大小写向下查找
  else {
    // 如果查找范围在同一行内
    if ( ptFrom.y == ptTo.y ) {
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pStart = pli->pData + ptFrom.x;
      pEnd = pli->pData + ptTo.x;
      pFind = SearchFindNormalStringNoCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
    }
    // 查找范围不在同一行内
    else {
      // 搜索第一行
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pStart = pli->pData + ptFrom.x;
      pEnd = pli->pData + pli->nActLen;
      pFind = SearchFindNormalStringNoCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
      // 搜索中间行
      while ( pFind == NULL ) {
        ptFrom.y = m_pdb->TextGetNextMainLine( ptFrom.y );
        if ( ( ptFrom.y == PFX_EC_INVALID_LINE )
             || ( ptFrom.y >= ptTo.y ) ) {
          break;
        }
        pli = m_pdb->TextGetLineAt( ptFrom.y );
        pFind = SearchFindNormalStringNoCase( pli->pData, pli->nActLen, pszFindWhat, cchFindWhat );
      }
      // 搜索最后行
      if ( pFind == NULL ) {
        ptFrom.y = ptTo.y;
        pli = m_pdb->TextGetLineAt( ptTo.y );
        pFind = SearchFindNormalStringNoCase( pli->pData, ptTo.x, pszFindWhat, cchFindWhat );
      }
    }
  }
  if ( pFind != NULL ) {
    rFindReplace.ptFrom.y = ptFrom.y;
    rFindReplace.ptFrom.x = pFind - m_pdb->TextGetLineData( ptFrom.y );
    rFindReplace.ptTo.y = ptFrom.y;
    rFindReplace.ptTo.x = rFindReplace.ptFrom.x + cchFindWhat;
  }
  return ( pFind != NULL );
}
// 按不是“规则表达式”也不是“整字匹配”的方式查找字符串
// 找到返回TRUE，rFindReplace将包含对象信息，失败返回FALSE
BOOL CPfxEditView::SearchFindNormalUp( CPfxEditViewFindReplace& rFindReplace ) {
  LPCTSTR pStart;
  LPCTSTR pEnd;
  LPCTSTR pFind;
  const CPfxEditLineInfo* pli;
  CPoint ptFrom = rFindReplace.ptFrom;
  CPoint ptTo = rFindReplace.ptTo;
  LONG cchFindWhat = rFindReplace.cchFindWhat;
  LPCTSTR pszFindWhat = rFindReplace.pszFindWhat;
  // 由于向上搜索时从后面回溯指针非常慢，所以采取从一行的开头开始查找，
  // 然后比较位置，如果查找位置已经过了启动位置则查找结束
  LPCTSTR pLastMatch = NULL;
  if ( rFindReplace.bMatchCase ) {
    // 如果查找范围在同一行内
    if ( ptFrom.y == ptTo.y ) {
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pStart = pli->pData + ptTo.x;
      pEnd = pli->pData + ptFrom.x;
      for ( ;; ) {
        pFind = SearchFindNormalStringCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
        if ( ( pFind == NULL )
             || ( ( pFind + cchFindWhat ) > pEnd ) ) {
          pFind = pLastMatch;
          break;
        }
        pLastMatch = pFind;
        pStart = pFind + cchFindWhat;
      }
    }
    // 查找范围在连续的多行内
    else {
      // 搜索第一行
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pStart = pli->pData;
      pEnd = pli->pData + ptFrom.x;
      for ( ;; ) {
        pFind = SearchFindNormalStringCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
        if ( ( pFind == NULL )
             || ( ( pFind + cchFindWhat ) > pEnd ) ) {
          pFind = pLastMatch;
          break;
        }
        pLastMatch = pFind;
        pStart = pFind + cchFindWhat;
      }
      // 搜索中间行
      while ( pFind == NULL ) {
        ptFrom.y = m_pdb->TextGetPrevMainLine( ptFrom.y );
        if ( ( ptFrom.y == PFX_EC_INVALID_LINE )
             || ( ptFrom.y < ptTo.y ) ) {
          break;
        }
        pli = m_pdb->TextGetLineAt( ptFrom.y );
        pStart = pli->pData;
        pEnd = pli->pData + pli->nActLen;
        for ( ;; ) {
          pFind = SearchFindNormalStringCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
          if ( ( pFind == NULL )
               || ( ( pFind + cchFindWhat ) > pEnd ) ) {
            pFind = pLastMatch;
            break;
          }
          pLastMatch = pFind;
          pStart = pFind + cchFindWhat;
        }
      }
      // 搜索最后行
      if ( pFind == NULL ) {
        ptFrom.y = ptTo.y;
        pli = m_pdb->TextGetLineAt( ptTo.y );
        pStart = pli->pData + ptTo.x;
        pEnd = pli->pData + pli->nActLen;
        for ( ;; ) {
          pFind = SearchFindNormalStringCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
          if ( ( pFind == NULL )
               || ( ( pFind + cchFindWhat ) > pEnd ) ) {
            pFind = pLastMatch;
            break;
          }
          pLastMatch = pFind;
          pStart = pFind + cchFindWhat;
        }
      }
    }
  }
  // 忽略大小写向上查找
  else {
    // 如果查找范围在同一行内
    if ( ptFrom.y == ptTo.y ) {
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pStart = pli->pData + ptTo.x;
      pEnd = pli->pData + ptFrom.x;
      for ( ;; ) {
        pFind = SearchFindNormalStringNoCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
        if ( ( pFind == NULL )
             || ( ( pFind + cchFindWhat ) > pEnd ) ) {
          pFind = pLastMatch;
          break;
        }
        pLastMatch = pFind;
        pStart = pFind + cchFindWhat;
      }
    }
    // 查找范围在连续的多行内
    else {
      // 搜索第一行
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pStart = pli->pData;
      pEnd = pli->pData + ptFrom.x;
      for ( ;; ) {
        pFind = SearchFindNormalStringNoCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
        if ( ( pFind == NULL )
             || ( ( pFind + cchFindWhat ) > pEnd ) ) {
          pFind = pLastMatch;
          break;
        }
        pLastMatch = pFind;
        pStart = pFind + cchFindWhat;
      }
      // 搜索中间行
      while ( pFind == NULL ) {
        ptFrom.y = m_pdb->TextGetPrevMainLine( ptFrom.y );
        if ( ( ptFrom.y == PFX_EC_INVALID_LINE )
             || ( ptFrom.y < ptTo.y ) ) {
          break;
        }
        pli = m_pdb->TextGetLineAt( ptFrom.y );
        pStart = pli->pData;
        pEnd = pli->pData + pli->nActLen;
        for ( ;; ) {
          pFind = SearchFindNormalStringNoCase( pStart, pEnd - pStart, pszFindWhat, cchFindWhat );
          if ( ( pFind == NULL )
               || ( ( pFind + cchFindWhat ) > pEnd ) ) {
            pFind = pLastMatch;
            break;
          }
          pLastMatch = pFind;
          pStart = pFind + cchFindWhat;
        }
      }
      // 搜索最后行
      if ( pFind == NULL ) {
        ptFrom.y = ptTo.y;
        pli = m_pdb->TextGetLineAt( ptTo.y );
        pStart = pli->pData + ptTo.x;
        pEnd = pli->pData + pli->nActLen;
        for ( ;; ) {
          pFind = SearchFindNormalStringNoCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
          if ( ( pFind == NULL )
               || ( ( pFind + cchFindWhat ) > pEnd ) ) {
            pFind = pLastMatch;
            break;
          }
          pLastMatch = pFind;
          pStart = pFind + cchFindWhat;
        }
      }
    }
  }
  if ( pFind != NULL ) {
    rFindReplace.ptFrom.y = ptFrom.y;
    rFindReplace.ptFrom.x = pFind - m_pdb->TextGetLineData( ptFrom.y );
    rFindReplace.ptTo.y = ptFrom.y;
    rFindReplace.ptTo.x = rFindReplace.ptFrom.x + cchFindWhat;
  }
  return ( pFind != NULL );
}
// LPCTSTR SearchFindWholeWordStringCase(LPCTSTR pStart,LPCTSTR pEnd,LPCTSTR pData,LONG cch,LPCTSTR pszFindWhat,LONG cchFindWhat)
// LPCTSTR pStart      - 缓冲区开头的地址，它可以与pData一样，也可能比
//                    pData小，作用与成员函数
//                    GetPrevCharPos(LPCTSTR pCur, LPCTSTR pStart)中
//                    的pStart一样
// LPCTSTR pEnd        - 缓冲区结尾地址，作用与成员函数
//                    GetNextCharPos(LPCTSTR pCur, LPCTSTR pEnd)中的
//                    一样
// LPCTSTR pData       - 搜索的开始地址
// LONG cch            - 搜索的长度
// LPCTSTR pszFindWhat - 要查找的目标字符串
// LONG cchFindWhat    - 要查找的目标字符串长度，它不一定要NULL结尾
LPCTSTR CPfxEditView::SearchFindWholeWordStringCase( LPCTSTR pStart, LPCTSTR pEnd, LPCTSTR pData, LONG cch, LPCTSTR pszFindWhat, LONG cchFindWhat ) {
  ASSERT( cch >= 0 );
  ASSERT( cchFindWhat > 0 );
  ASSERT( pszFindWhat != NULL );
  if ( ( cch < cchFindWhat ) || ( pData == NULL ) ) {
    return NULL;
  }
  LPCTSTR pEndString = pData + cch;
  LPCTSTR pEndSearch = pData + ( cch - cchFindWhat );
  LPCTSTR pEndFindWhat = pszFindWhat + cchFindWhat;
  LPCTSTR pPrev = ( ( pStart == pData ) ? NULL : GetPrevCharPos( pData, pStart ) );
  // 如果缓冲区的第一个字符开始就是要查找的对象，则应该被认为是符合要
  // 求的，所以要人为的设置第一个字符之前的“字符”是空白字符
  while ( pData <= pEndSearch ) {
    LPCTSTR s1 = pData;
    LPCTSTR s2 = pszFindWhat;
    WCHAR c1;
    WCHAR c2;
    while ( ( s1 < pEndString ) && ( s2 < pEndFindWhat ) ) {
      if ( PFX_EC_IS_LEAD_BYTE( *s1 )
           && ( ( s1 + PFX_EC_MAX_MB_LEN ) <= pEndString ) ) {
        c1 = ( WCHAR ) * ( ( LPWSTR ) s1 );
        s1 += PFX_EC_MAX_MB_LEN;
      } else {
        c1 = ( WCHAR ) * s1;
        s1++;
      }
      if ( PFX_EC_IS_LEAD_BYTE( *s2 )
           && ( ( s2 + PFX_EC_MAX_MB_LEN ) <= pEndFindWhat ) ) {
        c2 = ( WCHAR ) * ( ( LPWSTR ) s2 );
        s2 += PFX_EC_MAX_MB_LEN;
      } else {
        c2 = ( WCHAR ) * s2;
        s2++;
      }
      if ( c1 != c2 ) {
        goto ContinueCompareNextChar;
      }
    }
    if ( ( s2 == pEndFindWhat )           //      OK，匹配？
         //  如果要查找的字符串第一个字符是单词定界符或当前找
         //  到的对象的前一个字符是单词定界符，就算命中前部分
         && ( ( pPrev == NULL )           //      从行首开始搜索？
              || IsWhitespace( *pPrev )
              || IsDelimiter( pPrev, pEnd )
              || IsWhitespace( *pszFindWhat )
              || IsDelimiter( pszFindWhat, pEndFindWhat ) )
         //  如果要查找的字符串最后的字符是单词定界符或当前找
         //  到的对象的后一个字符是单词定界符，就算命中后部分
         && ( ( s1 >= pEnd )           //      到达行末？
              || IsWhitespace( *s1 )
              || IsDelimiter( s1, pEnd )
              || IsWhitespace( c2 )
              || IsDelimiter( c2 ) ) ) {
      return pData;
    }
ContinueCompareNextChar:
    // 记下当前字符位置，当找到匹配字符串时需要判断当前字符是否
    // 是单词定界符
    pPrev = pData;
    pData += ( PFX_EC_IS_LEAD_BYTE( *pData ) ? PFX_EC_MAX_MB_LEN : 1 );
  }
  return NULL;
}
// LPCTSTR SearchFindWholeWordStringNoCase(LPCTSTR pStart,LPCTSTR pEnd,LPCTSTR pData,LONG cch,LPCTSTR pszFindWhat,LONG cchFindWhat)
// LPCTSTR pStart      - 缓冲区开头的地址，它可以与pData一样，也可能比
//                    pData小，作用与成员函数
//                    GetPrevCharPos(LPCTSTR pCur, LPCTSTR pStart)中
//                    的pStart一样
// LPCTSTR pEnd        - 缓冲区结尾地址，作用与成员函数
//                    GetNextCharPos(LPCTSTR pCur, LPCTSTR pEnd)中的
//                    一样
// LPCTSTR pData       - 搜索的开始地址
// LONG cch            - 搜索的长度
// LPCTSTR pszFindWhat - 要查找的目标字符串
// LONG cchFindWhat    - 要查找的目标字符串长度，它不一定要NULL结尾
LPCTSTR CPfxEditView::SearchFindWholeWordStringNoCase( LPCTSTR pStart, LPCTSTR pEnd, LPCTSTR pData, LONG cch, LPCTSTR pszFindWhat, LONG cchFindWhat ) {
  ASSERT( cch >= 0 );
  ASSERT( cchFindWhat > 0 );
  ASSERT( pszFindWhat != NULL );
  if ( ( cch < cchFindWhat ) || ( pData == NULL ) ) {
    return NULL;
  }
  LPCTSTR pEndString = pData + cch;
  LPCTSTR pEndSearch = pData + ( cch - cchFindWhat );
  LPCTSTR pEndFindWhat = pszFindWhat + cchFindWhat;
  LPCTSTR pPrev = ( ( pStart == pData ) ? NULL : GetPrevCharPos( pData, pStart ) );
  while ( pData <= pEndSearch ) {
    LPCTSTR s1 = pData;
    LPCTSTR s2 = pszFindWhat;
    WCHAR c1;
    WCHAR c2;
    while ( ( s1 < pEndString ) && ( s2 < pEndFindWhat ) ) {
      if ( PFX_EC_IS_LEAD_BYTE( *s1 )
           && ( ( s1 + PFX_EC_MAX_MB_LEN ) <= pEndString ) ) {
        c1 = ( WCHAR ) * ( ( LPWSTR ) s1 );
        s1 += PFX_EC_MAX_MB_LEN;
      } else {
        c1 = ( WCHAR ) * s1;
        s1++;
        //  单纯地转换成拉丁小写字符，忽略全角数字字
        //  符和全角字母字符
        if ( ( c1 <= 'Z' ) && ( c1 >= 'A' ) ) {
          c1 += ( 'a' - 'A' );
        }
      }
      if ( PFX_EC_IS_LEAD_BYTE( *s2 )
           && ( ( s2 + PFX_EC_MAX_MB_LEN ) <= pEndFindWhat ) ) {
        c2 = ( WCHAR ) * ( ( LPWSTR ) s2 );
        s2 += PFX_EC_MAX_MB_LEN;
      } else {
        c2 = ( WCHAR ) * s2;
        s2++;
      }
      if ( c1 != c2 ) {
        goto ContinueCompareNextChar;
      }
    }
    if ( ( s2 == pEndFindWhat )           //      OK，匹配？
         //  如果要查找的字符串第一个字符是单词定界符或当前找
         //  到的对象的前一个字符是单词定界符，就算命中前部分
         && ( ( pPrev == NULL )           //      从行首开始搜索？
              || IsWhitespace( *pPrev )
              || IsDelimiter( pPrev, pEnd )
              || IsWhitespace( *pszFindWhat )
              || IsDelimiter( pszFindWhat, pEndFindWhat ) )
         //  如果要查找的字符串最后的字符是单词定界符或当前找
         //  到的对象的后一个字符是单词定界符，就算命中后部分
         && ( ( s1 >= pEnd )           //      到达行末？
              || IsWhitespace( *s1 )
              || IsDelimiter( s1, pEnd )
              || IsWhitespace( c2 )
              || IsDelimiter( c2 ) ) ) {
      return pData;
    }
ContinueCompareNextChar:
    // 记下当前字符位置，当找到匹配字符串时需要判断当前字符是否
    // 是单词定界符
    pPrev = pData;
    pData += ( PFX_EC_IS_LEAD_BYTE( *pData ) ? PFX_EC_MAX_MB_LEN : 1 );
  }
  return NULL;
}
// rFindReplace.ptFrom - 相对主行位置
// rFindReplace.ptTo   - 相对主行位置
BOOL CPfxEditView::SearchFindWholeWordDown( CPfxEditViewFindReplace& rFindReplace ) {
  LPCTSTR pDataStart;
  LPCTSTR pDataEnd;
  LPCTSTR pSearchStart;
  LPCTSTR pSearchEnd;
  LPCTSTR pFind;
  const CPfxEditLineInfo* pli;
  CPoint ptFrom = rFindReplace.ptFrom;
  CPoint ptTo = rFindReplace.ptTo;
  LONG cchFindWhat = rFindReplace.cchFindWhat;
  LPCTSTR pszFindWhat = rFindReplace.pszFindWhat;
  if ( rFindReplace.bMatchCase ) {
    // 如果查找范围在同一行内
    if ( ptFrom.y == ptTo.y ) {
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pDataStart = pli->pData;
      pDataEnd = pDataStart + pli->nActLen;
      pSearchStart = pDataStart + ptFrom.x;
      pSearchEnd = pDataStart + ptTo.x;
      pFind = SearchFindWholeWordStringCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
    }
    // 查找范围不在同一行内
    else {
      // 搜索第一行
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pDataStart = pli->pData;
      pDataEnd = pDataStart + pli->nActLen;
      pSearchStart = pDataStart + ptFrom.x;
      pSearchEnd = pDataStart + pli->nActLen;
      pFind = SearchFindWholeWordStringCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
      // 搜索中间行
      while ( pFind == NULL ) {
        ptFrom.y = m_pdb->TextGetNextMainLine( ptFrom.y );
        if ( ( ptFrom.y == PFX_EC_INVALID_LINE )
             || ( ptFrom.y >= ptTo.y ) ) {
          break;
        }
        pli = m_pdb->TextGetLineAt( ptFrom.y );
        pDataStart = pli->pData;
        pFind = SearchFindWholeWordStringCase( pDataStart, ( pDataStart + pli->nActLen ), pDataStart, pli->nActLen, pszFindWhat, cchFindWhat );
      }
      // 搜索最后行
      if ( pFind == NULL ) {
        ptFrom.y = ptTo.y;
        pli = m_pdb->TextGetLineAt( ptTo.y );
        pDataStart = pli->pData;
        pFind = SearchFindWholeWordStringCase( pDataStart, ( pDataStart + pli->nActLen ), pDataStart, ptTo.x, pszFindWhat, cchFindWhat );
      }
    }
  }
  // 忽略大小写向下查找
  else {
    // 如果查找范围在同一行内
    if ( ptFrom.y == ptTo.y ) {
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pDataStart = pli->pData;
      pDataEnd = pDataStart + pli->nActLen;
      pSearchStart = pDataStart + ptFrom.x;
      pSearchEnd = pDataStart + ptTo.x;
      pFind = SearchFindWholeWordStringNoCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
    }
    // 查找范围不在同一行内
    else {
      // 搜索第一行
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pDataStart = pli->pData;
      pDataEnd = pDataStart + pli->nActLen;
      pSearchStart = pDataStart + ptFrom.x;
      pSearchEnd = pDataStart + pli->nActLen;
      pFind = SearchFindWholeWordStringNoCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
      // 搜索中间行
      while ( pFind == NULL ) {
        ptFrom.y = m_pdb->TextGetNextMainLine( ptFrom.y );
        if ( ( ptFrom.y == PFX_EC_INVALID_LINE )
             || ( ptFrom.y >= ptTo.y ) ) {
          break;
        }
        pli = m_pdb->TextGetLineAt( ptFrom.y );
        pDataStart = pli->pData;
        pFind = SearchFindWholeWordStringNoCase( pDataStart, ( pDataStart + pli->nActLen ), pDataStart, pli->nActLen, pszFindWhat, cchFindWhat );
      }
      // 搜索最后行
      if ( pFind == NULL ) {
        ptFrom.y = ptTo.y;
        pli = m_pdb->TextGetLineAt( ptTo.y );
        pDataStart = pli->pData;
        pFind = SearchFindWholeWordStringNoCase( pDataStart, ( pDataStart + pli->nActLen ), pDataStart, ptTo.x, pszFindWhat, cchFindWhat );
      }
    }
  }
  if ( pFind != NULL ) {
    rFindReplace.ptFrom.y = ptFrom.y;
    rFindReplace.ptFrom.x = pFind - m_pdb->TextGetLineData( ptFrom.y );
    rFindReplace.ptTo.y = ptFrom.y;
    rFindReplace.ptTo.x = rFindReplace.ptFrom.x + cchFindWhat;
  }
  return ( pFind != NULL );
}
// rFindReplace.ptFrom - 相对主行位置
// rFindReplace.ptTo   - 相对主行位置
BOOL CPfxEditView::SearchFindWholeWordUp( CPfxEditViewFindReplace& rFindReplace ) {
  LPCTSTR pDataStart;
  LPCTSTR pDataEnd;
  LPCTSTR pSearchStart;
  LPCTSTR pSearchEnd;
  LPCTSTR pFind;
  const CPfxEditLineInfo* pli;
  CPoint ptFrom = rFindReplace.ptFrom;
  CPoint ptTo = rFindReplace.ptTo;
  LONG cchFindWhat = rFindReplace.cchFindWhat;
  LPCTSTR pszFindWhat = rFindReplace.pszFindWhat;
  // 由于向上搜索时从后面回溯指针非常慢，所以采取从一行的开头开始查找，
  // 然后比较位置，如果查找位置已经过了启动位置则查找结束
  LPCTSTR pLastMatch = NULL;
  if ( rFindReplace.bMatchCase ) {
    // 如果查找范围在同一行内
    if ( ptFrom.y == ptTo.y ) {
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pDataStart = pli->pData;
      pDataEnd = pDataStart + pli->nActLen;
      pSearchStart = pDataStart + ptTo.x;
      pSearchEnd = pDataStart + ptFrom.x;
      for ( ;; ) {
        pFind = SearchFindWholeWordStringCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
        if ( ( pFind == NULL )
             || ( ( pFind + cchFindWhat ) > pSearchEnd ) ) {
          pFind = pLastMatch;
          break;
        }
        pLastMatch = pFind;
        pSearchStart = pFind + cchFindWhat;
      }
    }
    // 查找范围在连续的多行内
    else {
      // 搜索第一行
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pDataStart = pli->pData;
      pDataEnd = pDataStart + pli->nActLen;
      pSearchStart = pDataStart;
      pSearchEnd = pDataStart + ptFrom.x;
      for ( ;; ) {
        pFind = SearchFindWholeWordStringCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
        if ( ( pFind == NULL )
             || ( ( pFind + cchFindWhat ) > pSearchEnd ) ) {
          pFind = pLastMatch;
          break;
        }
        pLastMatch = pFind;
        pSearchStart = pFind + cchFindWhat;
      }
      // 搜索中间行
      while ( pFind == NULL ) {
        ptFrom.y = m_pdb->TextGetPrevMainLine( ptFrom.y );
        if ( ( ptFrom.y == PFX_EC_INVALID_LINE )
             || ( ptFrom.y < ptTo.y ) ) {
          break;
        }
        pli = m_pdb->TextGetLineAt( ptFrom.y );
        pDataStart = pli->pData;
        pDataEnd = pDataStart + pli->nActLen;
        pSearchStart = pDataStart;
        pSearchEnd = pDataStart + pli->nActLen;
        for ( ;; ) {
          pFind = SearchFindWholeWordStringCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
          if ( ( pFind == NULL )
               || ( ( pFind + cchFindWhat ) > pSearchEnd ) ) {
            pFind = pLastMatch;
            break;
          }
          pLastMatch = pFind;
          pSearchStart = pFind + cchFindWhat;
        }
      }
      // 搜索最后行
      if ( pFind == NULL ) {
        ptFrom.y = ptTo.y;
        pli = m_pdb->TextGetLineAt( ptTo.y );
        pDataStart = pli->pData;
        pDataEnd = pDataStart + pli->nActLen;
        pSearchStart = pDataStart + ptTo.x;
        pSearchEnd = pDataStart + pli->nActLen;
        for ( ;; ) {
          pFind = SearchFindWholeWordStringCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
          if ( ( pFind == NULL )
               || ( ( pFind + cchFindWhat ) > pSearchEnd ) ) {
            pFind = pLastMatch;
            break;
          }
          pLastMatch = pFind;
          pSearchStart = pFind + cchFindWhat;
        }
      }
    }
  }
  // 忽略大小写向上查找
  else {
    // 如果查找范围在同一行内
    if ( ptFrom.y == ptTo.y ) {
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pDataStart = pli->pData;
      pDataEnd = pDataStart + pli->nActLen;
      pSearchStart = pDataStart + ptTo.x;
      pSearchEnd = pDataStart + ptFrom.x;
      for ( ;; ) {
        pFind = SearchFindWholeWordStringNoCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
        if ( ( pFind == NULL )
             || ( ( pFind + cchFindWhat ) > pSearchEnd ) ) {
          pFind = pLastMatch;
          break;
        }
        pLastMatch = pFind;
        pSearchStart = pFind + cchFindWhat;
      }
    }
    // 查找范围在连续的多行内
    else {
      // 搜索第一行
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pDataStart = pli->pData;
      pDataEnd = pDataStart + pli->nActLen;
      pSearchStart = pDataStart;
      pSearchEnd = pDataStart + ptFrom.x;
      for ( ;; ) {
        pFind = SearchFindWholeWordStringNoCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
        if ( ( pFind == NULL )
             || ( ( pFind + cchFindWhat ) > pSearchEnd ) ) {
          pFind = pLastMatch;
          break;
        }
        pLastMatch = pFind;
        pSearchStart = pFind + cchFindWhat;
      }
      // 搜索中间行
      while ( pFind == NULL ) {
        ptFrom.y = m_pdb->TextGetPrevMainLine( ptFrom.y );
        if ( ( ptFrom.y == PFX_EC_INVALID_LINE )
             || ( ptFrom.y < ptTo.y ) ) {
          break;
        }
        pli = m_pdb->TextGetLineAt( ptFrom.y );
        pDataStart = pli->pData;
        pDataEnd = pDataStart + pli->nActLen;
        pSearchStart = pDataStart;
        pSearchEnd = pDataStart + pli->nActLen;
        for ( ;; ) {
          pFind = SearchFindWholeWordStringNoCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
          if ( ( pFind == NULL )
               || ( ( pFind + cchFindWhat ) > pSearchEnd ) ) {
            pFind = pLastMatch;
            break;
          }
          pLastMatch = pFind;
          pSearchStart = pFind + cchFindWhat;
        }
      }
      // 搜索最后行
      if ( pFind == NULL ) {
        ptFrom.y = ptTo.y;
        pli = m_pdb->TextGetLineAt( ptTo.y );
        pDataStart = pli->pData;
        pDataEnd = pDataStart + pli->nActLen;
        pSearchStart = pDataStart + ptTo.x;
        pSearchEnd = pDataStart + pli->nActLen;
        for ( ;; ) {
          pFind = SearchFindWholeWordStringNoCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
          if ( ( pFind == NULL )
               || ( ( pFind + cchFindWhat ) > pSearchEnd ) ) {
            pFind = pLastMatch;
            break;
          }
          pLastMatch = pFind;
          pSearchStart = pFind + cchFindWhat;
        }
      }
    }
  }
  if ( pFind != NULL ) {
    rFindReplace.ptFrom.y = ptFrom.y;
    rFindReplace.ptFrom.x = pFind - m_pdb->TextGetLineData( ptFrom.y );
    rFindReplace.ptTo.y = ptFrom.y;
    rFindReplace.ptTo.x = rFindReplace.ptFrom.x + cchFindWhat;
  }
  return ( pFind != NULL );
}
// 语法分析...
// BOOL ParserIsMarkerString(LPCTSTR pStart,LPCTSTR pCur,LPCTSTR pEnd,LPCTSTR pString,LONG nStrLen,BOOL bCase) const
// 检查在一行中从pCur开始到pEnd结束是否包含pString的字符串，匹配必须从
// pCur开始
// LPCTSTR pStart  - 一行数据的开始地址
// LPCTSTR pCur    - 从这个地址开始比较
// LPCTSTR pEnd    - 一行数据的结束地址
// LPCTSTR pString - 要查找的目标字符串
// LONG nStrLen    - 该目标字符串的长度
// BOOL bCase      - 是否匹配大小写
// 如果匹配则返回TRUE，否则返回FALSE
BOOL CPfxEditView::ParserIsMarkerString( LPCTSTR pStart, LPCTSTR pCur, LPCTSTR pEnd, LPCTSTR pString, LONG nStrLen, BOOL bCase ) const {
  ASSERT( pStart != NULL );
  ASSERT( pCur != NULL );
  ASSERT( pEnd != NULL );
  ASSERT( nStrLen > 0 );
  ASSERT( pString != NULL );
  if ( ( pEnd - pCur ) < nStrLen ) {
    return false;
  }
  // 检查它前面的字符是否是转义字符
  LONG nEscapes = 0;
  for ( LPCTSTR p1 = pCur; p1 > pStart; ) {
    LPCTSTR p2 = GetPrevCharPos( p1, pStart );
    if ( !ParserIsEscape( ( ( ( p1 - p2 ) >= sizeof( WCHAR ) ) ? *( LPCWSTR ) p2 : ( WCHAR ) * p2 ) ) ) {
      break;
    }
    p1 = p2;
    nEscapes++;
  }
  // 如果在要检查的字符前面有奇数个转义字符，则表明转义字符生效，要检
  // 查的字符串肯定不匹配
  if ( ( nEscapes % 2 ) != 0 ) {
    return false;
  }
  // 上面的判断已经决定(pEnd - pCur)一定不能比nStrLen小
  return ( ( bCase ? _tcsncmp : _tcsnicmp ) ( pCur, pString, nStrLen ) == 0 );
}
// BOOL ParserIsMarkerStart(LPCTSTR pStart,LPCTSTR pCur,LPCTSTR pEnd,CPfxEditMarkerPair* pMarker) const
BOOL CPfxEditView::ParserIsMarkerStart( LPCTSTR pStart, LPCTSTR pCur, LPCTSTR pEnd, CPfxEditMarkerPair* pMarker ) const {
  if ( ( pStart == NULL )
       || ( pMarker == NULL )
       || ( pMarker->pszOn == NULL )
       || ( m_pti == NULL ) ) {
    return false;
  }
  return ParserIsMarkerString( pStart, pCur, pEnd, pMarker->pszOn, pMarker->cchOn, m_pti->IsMatchCase() );
}
// BOOL ParserIsMarkerClose(LPCTSTR pStart,LPCTSTR pCur,LPCTSTR pEnd,CPfxEditMarkerPair* pMarker) const
BOOL CPfxEditView::ParserIsMarkerClose( LPCTSTR pStart, LPCTSTR pCur, LPCTSTR pEnd, CPfxEditMarkerPair* pMarker ) const {
  if ( ( pStart == NULL )
       || ( pMarker == NULL )
       || ( pMarker->pszOff == NULL )
       || ( m_pti == NULL ) ) {
    return false;
  }
  return ParserIsMarkerString( pStart, pCur, pEnd, pMarker->pszOff, pMarker->cchOff, m_pti->IsMatchCase() );
}
// BOOL ParserIsEscapedLine(LPCTSTR pStart, LPCTSTR pEnd) const
// 判断给定的字符串的最后一个字符是否是转义字符
BOOL CPfxEditView::ParserIsEscapedLine( LPCTSTR pStart, LPCTSTR pEnd ) const {
  if ( ( pStart == NULL ) || ( pStart >= pEnd ) ) {
    return false;
  }
  // 检查它前面的字符是否是转义字符
  LONG nEscapes = 0;
  for ( LPCTSTR p1 = pEnd; p1 > pStart; ) {
    LPCTSTR p2 = GetPrevCharPos( p1, pStart );
    if ( !ParserIsEscape( ( ( ( p1 - p2 ) >= sizeof( WCHAR ) ) ? *( LPCWSTR ) p2 : ( WCHAR ) * p2 ) ) ) {
      break;
    }
    p1 = p2;
    nEscapes++;
  }
  // 如果从行末开始倒数有奇数个转义字符，则表明转义字符生效，行末有转
  // 义字符
  return ( ( nEscapes % 2 ) != 0 );
}
// BOOL ParserIsInsideBlock(CPoint pt) const
// 判断一给定位置是否是块的一部分，块是指块注释、行注释、字符串等
// CPoint pt - 相对子行位置
// 如果是块的一部分返回TRUE，否则返回FALSE
BOOL CPfxEditView::ParserIsInsideBlock( CPoint pt ) const {
  ASSERT( IsDrawHilighted() );
  if ( !m_pdb->TextIsValidChar( pt.y, pt.x ) ) {
    return false;
  }
  if ( pt.y > m_pdb->m_nLineLastParsed ) {
    ParserPreParse( pt.y, m_pdb->TextGetLineUpperBound(), m_pdb->m_nLineLastParsedButCanceled, m_pdb->m_nLineLastParsed, m_pdb->TextGetLineAt( 0 ) );
  }
  // 如果指定行是行注释则不需要进一步判断
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
  const LPCTSTR pStart = pli->pData;
  const LPCTSTR pEnd = pStart + pli->nLen;
  const LPCTSTR pTargetPos = pStart + pt.x;
  LPCTSTR pCur = pStart;
  DWORD dwCookie = pli->dwFlags;
  CPfxEditMarkerPair* pMarker;
  if ( ( dwCookie & ( PFX_EC_LF_COOKIE_LINE_COMMENT1 | PFX_EC_LF_COOKIE_LINE_COMMENT2 ) ) != 0 ) {
    return true;
  } else if ( dwCookie & PFX_EC_LF_COOKIE_BLOCK_COMMENT1 ) {
    pMarker = m_pti->GetMarkerPair( mpBlockComment1 );
  } else if ( dwCookie & PFX_EC_LF_COOKIE_BLOCK_COMMENT2 ) {
    pMarker = m_pti->GetMarkerPair( mpBlockComment2 );
  } else if ( dwCookie & PFX_EC_LF_COOKIE_QUOTED_STRING1 ) {
    pMarker = m_pti->GetMarkerPair( mpQuotedString1 );
  } else if ( dwCookie & PFX_EC_LF_COOKIE_QUOTED_STRING2 ) {
    pMarker = m_pti->GetMarkerPair( mpQuotedString2 );
  } else {
    pMarker = NULL;
  }
  if ( pMarker != NULL ) {
    pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, false, pMarker );
    if ( ( pCur == NULL ) || ( pCur >= pTargetPos ) ) {
      return true;
    }
  }
  // 继续分析这一行中余下的字符
  // 每次分析一个字符的时候将该字符和下面的标识符的第一个字符比较，如
  // 果不等就则不需要继续比较，这样可以加快分析速度
  CPfxEditViewMarkerChar mc;
  ParserGetFirstCharOfMarkerStart( mc );
  while ( ( pCur < pEnd ) && ( pCur <= pTargetPos ) ) {
    BOOL bLeadByte = PFX_EC_IS_LEAD_BYTE( *pCur );
    WCHAR wc = ( ( !bLeadByte
                   || ( ( pCur + PFX_EC_MAX_MB_LEN ) > pEnd ) ) ? ( ( WCHAR ) * pCur ) : ( *( LPWSTR ) pCur ) );
    if ( ( ( wc == mc.m_cLowerLineComment1 ) || ( wc == mc.m_cUpperLineComment1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpLineComment1 ) ) ) {
      return true;
    }
    if ( ( ( wc == mc.m_cLowerLineComment2 ) || ( wc == mc.m_cUpperLineComment2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpLineComment2 ) ) ) {
      return true;
    }
    if ( ( ( wc == mc.m_cLowerBlockComment1 ) || ( wc == mc.m_cUpperBlockComment1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpBlockComment1 ) ) ) {
      pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpBlockComment1 ) );
      if ( ( pCur == NULL ) || ( pCur >= pTargetPos ) ) {
        return true;
      } else {
        continue;
      }
    }
    if ( ( ( wc == mc.m_cLowerBlockComment2 ) || ( wc == mc.m_cUpperBlockComment2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpBlockComment2 ) ) ) {
      pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpBlockComment2 ) );
      if ( ( pCur == NULL ) || ( pCur >= pTargetPos ) ) {
        return true;
      } else {
        continue;
      }
    }
    if ( ( ( wc == mc.m_cLowerQuotedString1 ) || ( wc == mc.m_cUpperQuotedString1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpQuotedString1 ) ) ) {
      pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpQuotedString1 ) );
      if ( ( pCur == NULL ) || ( pCur >= pTargetPos ) ) {
        return true;
      } else {
        continue;
      }
    }
    if ( ( ( wc == mc.m_cLowerQuotedString2 ) || ( wc == mc.m_cUpperQuotedString2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpQuotedString2 ) ) ) {
      pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpQuotedString2 ) );
      if ( ( pCur == NULL ) || ( pCur >= pTargetPos ) ) {
        return true;
      } else {
        continue;
      }
    }
    // 指向下一个字符
    pCur += ( bLeadByte ? PFX_EC_MAX_MB_LEN : 1 );
  }
  return false;
}
// void ParserGetFirstCharOfMarkerStart(CPfxEditViewMarkerChar& mc) const
void CPfxEditView::ParserGetFirstCharOfMarkerStart( CPfxEditViewMarkerChar& mc ) const {
  if ( m_pti == NULL ) {
    return ;
  }
  CPfxEditMarkerPair* pMarker;
  pMarker = m_pti->GetMarkerPair( mpQuotedString1 );
  mc.m_cLowerQuotedString1 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wLowerOn );
  mc.m_cUpperQuotedString1 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wUpperOn );
  pMarker = m_pti->GetMarkerPair( mpQuotedString2 );
  mc.m_cLowerQuotedString2 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wLowerOn );
  mc.m_cUpperQuotedString2 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wUpperOn );
  pMarker = m_pti->GetMarkerPair( mpLineComment1 );
  mc.m_cLowerLineComment1 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wLowerOn );
  mc.m_cUpperLineComment1 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wUpperOn );
  pMarker = m_pti->GetMarkerPair( mpLineComment2 );
  mc.m_cLowerLineComment2 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wLowerOn );
  mc.m_cUpperLineComment2 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wUpperOn );
  pMarker = m_pti->GetMarkerPair( mpBlockComment1 );
  mc.m_cLowerBlockComment1 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wLowerOn );
  mc.m_cUpperBlockComment1 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wUpperOn );
  pMarker = m_pti->GetMarkerPair( mpBlockComment2 );
  mc.m_cLowerBlockComment2 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wLowerOn );
  mc.m_cUpperBlockComment2 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wUpperOn );
}
// LPCTSTR ParserGetMarkerCloseEndPosition(LPCTSTR pStart,LPCTSTR pCur,LPCTSTR pEnd,BOOL bCurrentAddressPointToMarkerStart,CPfxEditMarkerPair* pMarker) const
// 查找块定义结束标志
// 如找到就返回该结束标志的下一个字符的地址，否则返回NULL
LPCTSTR CPfxEditView::ParserGetMarkerCloseEndPosition( LPCTSTR pStart, LPCTSTR pCur, LPCTSTR pEnd, BOOL bCurrentAddressPointToMarkerStart, CPfxEditMarkerPair* pMarker ) const {
  if ( ( pMarker == NULL ) || ( pMarker->pszOff == NULL ) ) {
    return pStart;
  }
  // 如果当前指针指向“on”就要跨过“on”
  if ( bCurrentAddressPointToMarkerStart ) {
    pCur += pMarker->cchOn;
  }
  WCHAR cLower = pMarker->wLowerOff;
  WCHAR cUpper = pMarker->wUpperOff;
  while ( pCur < pEnd ) {
    BOOL bLeadByte = PFX_EC_IS_LEAD_BYTE( *pCur );
    WCHAR wc = ( ( !bLeadByte
                   || ( ( pCur + PFX_EC_MAX_MB_LEN ) > pEnd ) ) ? ( ( WCHAR ) * pCur ) : ( *( LPWSTR ) pCur ) );
    if ( ( ( wc == cLower ) || ( wc == cUpper ) )
         && ParserIsMarkerClose( pStart, pCur, pEnd, pMarker ) ) {
      return ( pCur + pMarker->cchOff );
    }
    pCur += ( bLeadByte ? PFX_EC_MAX_MB_LEN : 1 );
  }
  return NULL;
}
// DWORD ParserParseLine(LPCTSTR pData, LONG nLen, DWORD dwCookie) const
// 分析一行的数据，返回分析结果
DWORD CPfxEditView::ParserParseLine( LPCTSTR pData, LONG nLen, DWORD dwCookie ) const {
  const LPCTSTR pStart = pData;
  const LPCTSTR pEnd = pStart + nLen;
  LPCTSTR pCur = pStart;
  // 这一行是上一行的PFX_EC_LF_COOKIE_LINE_COMMENT1的延续，如果这一行的最后一
  // 个字符是转义字符，那么下一行也是PFX_EC_LF_COOKIE_LINE_COMMENT1的延续
  if ( dwCookie & PFX_EC_LF_COOKIE_LINE_COMMENT1 ) {
    return ( ( ( ( dwCookie & PFX_EC_LF_HARDRETURN ) == 0 ) || ParserIsEscapedLine( pStart, pEnd ) ) ? PFX_EC_LF_COOKIE_LINE_COMMENT1 : PFX_EC_LF_COOKIE_NONE );
  }
  // 这一行是上一行的PFX_EC_LF_COOKIE_LINE_COMMENT2的延续，如果这一行的最后一
  // 个字符是转义字符，那么下一行也是PFX_EC_LF_COOKIE_LINE_COMMENT2的延续
  else if ( dwCookie & PFX_EC_LF_COOKIE_LINE_COMMENT2 ) {
    return ( ( ( ( dwCookie & PFX_EC_LF_HARDRETURN ) == 0 ) || ParserIsEscapedLine( pStart, pEnd ) ) ? PFX_EC_LF_COOKIE_LINE_COMMENT2 : PFX_EC_LF_COOKIE_NONE );
  }
  // 这一行是PFX_EC_LF_COOKIE_BLOCK_COMMENT1的延续，如果不能在这一行中找到块注
  // 释结束标识，则下一行也是PFX_EC_LF_COOKIE_BLOCK_COMMENT1的延续，否则
  // PFX_EC_LF_COOKIE_BLOCK_COMMENT1将会在这一行中终止
  else if ( dwCookie & PFX_EC_LF_COOKIE_BLOCK_COMMENT1 ) {
    pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, false, m_pti->GetMarkerPair( mpBlockComment1 ) );
    // 如果找到就取消原来的标识，否则仍然使用旧有标识
    if ( pCur == NULL ) {
      return PFX_EC_LF_COOKIE_BLOCK_COMMENT1;
    }
  }
  // 这一行是PFX_EC_LF_COOKIE_BLOCK_COMMENT2的延续，如果不能在这一行中找到块注
  // 释结束标识，则下一行也是PFX_EC_LF_COOKIE_BLOCK_COMMENT2的延续，否则
  // PFX_EC_LF_COOKIE_BLOCK_COMMENT2将会在这一行中终止
  else if ( dwCookie & PFX_EC_LF_COOKIE_BLOCK_COMMENT2 ) {
    pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, false, m_pti->GetMarkerPair( mpBlockComment2 ) );
    // 如果找到就取消原来的标识，否则仍然使用旧有标识
    if ( pCur == NULL ) {
      return PFX_EC_LF_COOKIE_BLOCK_COMMENT2;
    }
  }
  // 这一行是上一行的PFX_EC_LF_COOKIE_QUOTED_STRING1的延续，如果不能在这行中找
  // 到PFX_EC_LF_COOKIE_QUOTED_STRING1的结束标识，并且这行的最后一个字符是转义
  // 字符，那么下一行也是PFX_EC_LF_COOKIE_QUOTED_STRING1的延续
  else if ( dwCookie & PFX_EC_LF_COOKIE_QUOTED_STRING1 ) {
    pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, false, m_pti->GetMarkerPair( mpQuotedString1 ) );
    if ( pCur == NULL ) {
      return ( ( ( ( dwCookie & PFX_EC_LF_HARDRETURN ) == 0 ) || ParserIsEscapedLine( pStart, pEnd ) ) ? PFX_EC_LF_COOKIE_QUOTED_STRING1 : PFX_EC_LF_COOKIE_NONE );
    }
  } else if ( dwCookie & PFX_EC_LF_COOKIE_QUOTED_STRING2 ) {
    pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, false, m_pti->GetMarkerPair( mpQuotedString2 ) );
    // 如果找到就取消原来的标识，否则仍然使用旧有标识
    if ( pCur == NULL ) {
      return ( ( ( ( dwCookie & PFX_EC_LF_HARDRETURN ) == 0 ) || ParserIsEscapedLine( pStart, pEnd ) ) ? PFX_EC_LF_COOKIE_QUOTED_STRING2 : PFX_EC_LF_COOKIE_NONE );
    }
  }
  // 继续分析这一行中余下的字符
  // 每次分析一个字符的时候将该字符和下面的标识符的第一个字符比较，如
  // 果不等就则不需要继续比较，这样可以加快分析速度
  CPfxEditViewMarkerChar mc;
  ParserGetFirstCharOfMarkerStart( mc );
  while ( pCur < pEnd ) {
    BOOL bLeadByte = PFX_EC_IS_LEAD_BYTE( *pCur );
    WCHAR wc = ( ( !bLeadByte || ( ( pCur + PFX_EC_MAX_MB_LEN ) > pEnd ) ) ? ( ( WCHAR ) * pCur ) : ( *( LPWSTR ) pCur ) );
    if ( ( ( wc == mc.m_cLowerLineComment1 ) || ( wc == mc.m_cUpperLineComment1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpLineComment1 ) ) ) {
      return ( ( ( ( dwCookie & PFX_EC_LF_HARDRETURN ) == 0 ) || ParserIsEscapedLine( pStart, pEnd ) ) ? PFX_EC_LF_COOKIE_LINE_COMMENT1 : PFX_EC_LF_COOKIE_NONE );
    }
    if ( ( ( wc == mc.m_cLowerLineComment2 ) || ( wc == mc.m_cUpperLineComment2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpLineComment2 ) ) ) {
      return ( ( ( ( dwCookie & PFX_EC_LF_HARDRETURN ) == 0 ) || ParserIsEscapedLine( pStart, pEnd ) ) ? PFX_EC_LF_COOKIE_LINE_COMMENT2 : PFX_EC_LF_COOKIE_NONE );
    }
    if ( ( ( wc == mc.m_cLowerBlockComment1 ) || ( wc == mc.m_cUpperBlockComment1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpBlockComment1 ) ) ) {
      pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpBlockComment1 ) );
      // 如果没有找到则下面的字符都属于块注释
      if ( pCur != NULL ) {
        continue;
      } else {
        return PFX_EC_LF_COOKIE_BLOCK_COMMENT1;
      }
    }
    if ( ( ( wc == mc.m_cLowerBlockComment2 ) || ( wc == mc.m_cUpperBlockComment2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpBlockComment2 ) ) ) {
      pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpBlockComment2 ) );
      // 如果没有找到则下面的字符都属于块注释
      if ( pCur != NULL ) {
        continue;
      } else {
        return PFX_EC_LF_COOKIE_BLOCK_COMMENT2;
      }
    }
    if ( ( ( wc == mc.m_cLowerQuotedString1 ) || ( wc == mc.m_cUpperQuotedString1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpQuotedString1 ) ) ) {
      pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpQuotedString1 ) );
      // 如果没有找到则下面的字符都属于常量字符1
      if ( pCur != NULL ) {
        continue;
      } else {
        return ( ( ( ( dwCookie & PFX_EC_LF_HARDRETURN ) == 0 ) || ParserIsEscapedLine( pStart, pEnd ) ) ? PFX_EC_LF_COOKIE_QUOTED_STRING1 : PFX_EC_LF_COOKIE_NONE );
      }
    }
    if ( ( ( wc == mc.m_cLowerQuotedString2 ) || ( wc == mc.m_cUpperQuotedString2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpQuotedString2 ) ) ) {
      pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpQuotedString2 ) );
      // 如果没有找到则下面的字符都属于常量字符1
      if ( pCur != NULL ) {
        continue;
      } else {
        return ( ( ( ( dwCookie & PFX_EC_LF_HARDRETURN ) == 0 ) || ParserIsEscapedLine( pStart, pEnd ) ) ? PFX_EC_LF_COOKIE_QUOTED_STRING2 : PFX_EC_LF_COOKIE_NONE );
      }
    }
    // 指向下一个字符
    pCur += ( bLeadByte ? PFX_EC_MAX_MB_LEN : 1 );
  }
  return PFX_EC_LF_COOKIE_NONE;
}
// BOOL ParserPreParse(LONG nLineToEndParse,LONG nLineUpperBound,LONG& rnLineLastParsed,CPfxEditLineInfo* pliHead)
// 从rnLineLastParsed开始分析COOKIE到nLineToEndParse行
// LONG nLineToEndParse     - 分析到这一行终止
// LONG nLineUpperBound     - pliHead缓冲区内的总行数
// LONG& rnLineLastParsed   - 上一次最后分析过的行
// CPfxEditLineInfo* pliHead - 行信息缓冲区首地址
// 返回TRUE表示要刷新 rnLineLastParsed传入时的行及下面的部分，返回FALSE
// 表示不需要进一步操作
BOOL CPfxEditView::ParserPreParse( LONG nLineToEndParse, LONG nLineUpperBound, LONG nLineLastParsedButCanceled, LONG& rnLineLastParsed, CPfxEditLineInfo* pliHead ) const {
  if ( ( pliHead == NULL ) || ( nLineUpperBound < 0 ) ) {
    return false;
  }
  // 检查并指定行是否合法，如果不合法就修正到合法范围
  nLineToEndParse = max( nLineToEndParse, 0 );
  LONG nLineEndParsing = min( nLineToEndParse, nLineUpperBound );
  LONG nLineLastParsed = rnLineLastParsed;
  // 如果原来的信息无效就重新从头分析
  if ( ( nLineLastParsed <= 0 ) || ( nLineLastParsed > nLineUpperBound ) ) {
    // 第一行总是等于PFX_EC_LF_COOKIE_NONE
    pliHead->dwFlags &= ~PFX_EC_LF_COOKIE_ALL;
    nLineLastParsed = -1;
  }
  // 本次从上一次最后分析过的行的下一行开始分析
  LONG nStartParsingLine = nLineLastParsed + 1;
  // 如果指定行已经分析过，或开始分析的行是文档的最后行就不需要分析
  if ( ( nStartParsingLine >= nLineEndParsing )
       || ( nStartParsingLine >= nLineUpperBound ) ) {
    return false;
  }
  // 某行的分析结果保存在它的下一行中，最后一行不需要分析
  CPfxEditLineInfo* pli = pliHead + nStartParsingLine;
  CPfxEditLineInfo* pliEnd = pliHead + nLineEndParsing;
  CPfxEditLineInfo* pliCanceled = pliHead + nLineLastParsedButCanceled;
  DWORD dwCookie = pli->dwFlags;
  BOOL bRefreshBelow = false;
  for ( ;; ) {
    dwCookie = ParserParseLine( pli->pData, pli->nLen, dwCookie );
    if ( ++pli > pliEnd ) {
      break;
    }
    nLineLastParsed++;
    // 如果原来有块标记（块注释、行注释、字符串标识等）现在没有，
    // 或原来没有现在变成有，就要刷新这行下面的区域
    if ( ( pliCanceled >= pli )
         && ( ( pli->dwFlags & PFX_EC_LF_COOKIE_ALL ) != ( dwCookie & PFX_EC_LF_COOKIE_ALL ) ) ) {
      bRefreshBelow = true;
    }
    pli->dwFlags &= ~PFX_EC_LF_COOKIE_ALL;
    pli->dwFlags |= dwCookie;
    dwCookie |= pli->dwFlags;
  }
  rnLineLastParsed = nLineLastParsed;
  return bRefreshBelow;
}
// BOOL MatchBraceGetString(CPoint pt,BOOL bPrevStr,CPfxEditViewBraceHint& hint)
// 获取当前编辑位置所在的单词
// BOOL bPrevStr  - 是否获取上一单词
BOOL CPfxEditView::MatchBraceGetString( CPoint pt, BOOL bPrevStr, CPfxEditViewBraceHint& hint ) {
  if ( !m_pdb->TextIsValidChar( pt.y, pt.x ) ) {
    return false;
  }
  // 相对性的子行位置转换成相对性的主行位置，这是因为当处于自动换行模
  // 式时可能会将一单词强行断开，而使用主行方式可以避免出现这种情况
  m_pdb->TextSubLineToMainLine( pt );
  // 获取数据地址
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
  const LPCTSTR pStart = pli->pData;
  const LPCTSTR pEnd = pStart + pli->nActLen;
  LPCTSTR pCur = pStart + min( pli->nActLen, pt.x );
  if ( bPrevStr ) {
    pCur = GetPrevCharPos( pCur, pStart );
  }
  // 空的行或者当前位置指向一空白字符
  if ( ( pCur >= pEnd ) || IsWhitespace( *pCur ) ) {
    return false;
  }
  // A) !bPrevStr
  // 当前位置可能会出现如下情况之一：
  //                              |
  // 1 当前位置指向一单词的中间，例如： v
  //                             "abcdefghijkl"
  // 这种情况将向上和向下查找定界符，并提取两个定界符之间的部分
  //                              |
  // 2 当前位置指向一单词定界符，例如： v
  //                             "(abdefghijk)"
  // 这种情况只提取单词定界符
  // B) bPrevStr
  // 当前位置可能会出现如下情况之一：
  //                                         |
  // 1 当前位置指向一单词的中间，例如：            v
  //                             "abcdefghijklmn"
  // 这种情况将向上和向下查找定界符，并提取两个定界符之间的部分
  //                                          |
  // 2 当前位置的上一个字符是一单词定界符，例如：   v
  //                             "(abdefghijk)lmn"
  // 这种情况只提取单词定界符
  LONG nStrLen;
  LPCTSTR pString;
  if ( IsDelimiter( pCur, pEnd ) ) {
    pString = pCur;
    nStrLen = ( ( PFX_EC_IS_LEAD_BYTE( *pCur )
                  && ( ( pCur + PFX_EC_MAX_MB_LEN ) <= pEnd ) ) ? PFX_EC_MAX_MB_LEN : 1 );
  } else {
    pString = GetPrevDelimiterPos( pCur, pStart );
    nStrLen = GetNextDelimiterPos( pCur, pEnd ) - pString;
  }
  // 考虑到有些行可能会很长，而CString的缓冲区一但大于512便会使用new
  // 来获取缓冲区而不是使用局部缓冲池，所以在这里首先判断要获取的字符
  // 串的长度是否大于已知的最长的BRACE字符串的长度，如果是就肯定不会
  // 匹配，可以返回FALSE
  if ( nStrLen > hint.m_nMaxBraceString ) {
    return false;
  }
  // 将相对主行位置转换成相对子行位置
  hint.m_ptStrStart.y = pt.y;
  hint.m_ptStrStart.x = min( pli->nActLen, ( pString - pStart ) );
  hint.m_ptStrEnd.y = pt.y;
  hint.m_ptStrEnd.x = min( pli->nActLen, ( hint.m_ptStrStart.x + nStrLen ) );
  m_pdb->TextMainLineToSubLine( hint.m_ptStrStart );
  m_pdb->TextMainLineToSubLine( hint.m_ptStrEnd );
  // 成功获取一单词的位置后还要检查该单词所在的位置，如果该单词位于块
  // 注释、行注释、或引用字符串的范围内就算无效
  if ( !ParserIsInsideBlock( hint.m_ptStrStart ) ) {
    LPTSTR lp = hint.m_strBrace.GetBufferSetLength( nStrLen );
    if ( lp != NULL ) {
      memcpy( lp, pString, nStrLen * sizeof( TCHAR ) );
    }
  }
  return !hint.m_strBrace.IsEmpty();
}
// BOOL MatchBraceGetHint(CPfxEditViewBraceHint& hint)
BOOL CPfxEditView::MatchBraceGetHint( CPfxEditViewBraceHint& hint ) {
  if ( ( m_pti == NULL ) || ( m_pti->GetMarkerPair( mpBraceStrings ) == NULL ) ) {
    return false;
  }
  LONG nMaxBraceLen = 0;
  BOOL arrBOOL[] = { false, true};
  BOOL bResult = false;
  BOOL bMatchCase = m_pti->IsMatchCase();
  // 查找最大长度的BRACE字符串并获取它的长度
  for ( CPfxEditMarkerPair * pMarker = m_pti->GetMarkerPair( mpBraceStrings ); pMarker != NULL; pMarker = pMarker->pNext ) {
    nMaxBraceLen = max( nMaxBraceLen, pMarker->cchOn );
    nMaxBraceLen = max( nMaxBraceLen, pMarker->cchOff );
  }
  hint.m_nMaxBraceString = nMaxBraceLen;
  // 首先获取当前位置所在的单词进行比较，如果不匹配再获取当前位置的上
  // 一个单词进行比较
  for ( LONG i = 0; !bResult && ( i < PFX_EC_SIZEOF( arrBOOL ) ); i++ ) {
    if ( !MatchBraceGetString( m_ptEditPos, arrBOOL[ i ], hint ) ) {
      continue;
    }
    // 成功获取一单词后就遍历整个配对标识符链，依次与ON和OFF标
    // 识符比较，如果匹配就停止查找操作
    for ( pMarker = m_pti->GetMarkerPair( mpBraceStrings ); pMarker != NULL; pMarker = pMarker->pNext ) {
      if ( ( pMarker->pszOn != NULL )
           && ( ( bMatchCase ? _tcscmp : _tcsicmp ) ( hint.m_strBrace, pMarker->pszOn ) == 0 ) ) {
        hint.m_strOn = pMarker->pszOn;
        hint.m_strOff = pMarker->pszOff;
        hint.m_bDirectDown = true;
        //  向下查找首先越过当前配对字符串
        hint.m_ptFindStart = hint.m_ptStrEnd;
        bResult = true;
        break;
      }
      if ( ( pMarker->pszOff != NULL ) && ( ( bMatchCase ? _tcscmp : _tcsicmp ) ( hint.m_strBrace, pMarker->pszOff ) == 0 ) ) {
        hint.m_strOn = pMarker->pszOff;
        hint.m_strOff = pMarker->pszOn;
        hint.m_bDirectDown = false;
        //  向上查找首先越过当前配对字符串
        hint.m_ptFindStart = hint.m_ptStrStart;
        bResult = true;
        break;
      }
    }
  }
  if ( bResult ) {
    hint.m_bMatchCase = bMatchCase;
    if ( !hint.m_bMatchCase )           //      转换成小写
    {
      _tcslwr( hint.m_strOn.GetBufferSetLength( hint.m_strOn.GetLength() ) );
      _tcslwr( hint.m_strOff.GetBufferSetLength( hint.m_strOff.GetLength() ) );
    }
    // 相对子行位置转换成绝对主行位置
    m_pdb->TextSubLineToMainLine( hint.m_ptFindStart );
  }
  return bResult;
}
// BOOL MatchBraceSearchUp(CPfxEditViewBraceHint& hint, CPoint& rpt)
// CPoint& rpt - 找到的匹配对象的相对主行位置
BOOL CPfxEditView::MatchBraceSearchUp( CPfxEditViewBraceHint& hint, CPoint& rpt ) {
  // 查找算法如下：
  // 1. 从当前点POINT_START向上找到第一个OFF字符串并确定它的位置
  // 2. 从上面步骤1找到的OFF字符串的尾部向下查找并统计ON字符串的数量
  //  直到步骤1的当前点POINT_START
  // 3. 如果找到的OFF字符串与ON字符串数目相等就可以认为步骤1所找到的
  //  OFF字符串是我们的目标字符串，查找可以结束。否则将步骤1所找到
  //  的OFF字符串设置为当前点POINT_START并从步骤1开始重新查找，直到
  //  文档的开头
  // 注：结束查找的条件：
  // 1. 找到的ON字符串和OFF字符串数量一样多
  // 2. 已经查找到文档的开头
  CPoint ptResultFrom;
  CPoint ptResultTo;
  CPoint ptReverseScanFrom;
  CPoint ptReverseScanTo = hint.m_ptFindStart;
  CPoint ptFindEnd = CPoint( 0, 0 );
  m_pdb->TextSubLineToMainLine( ptFindEnd );
  CPfxEditViewFindReplace FindReplace;
  for ( LONG nCountOn = 1, nCountOff = 0; nCountOn != nCountOff; ) {
    FindReplace.bMatchCase = hint.m_bMatchCase;
    FindReplace.pszFindWhat = hint.m_strOff;
    FindReplace.cchFindWhat = hint.m_strOff.GetLength();
    FindReplace.ptFrom = hint.m_ptFindStart;
    FindReplace.ptTo = ptFindEnd;
    FindReplace.bMatchWholeWord = true;
    FindReplace.bRegExp = false;
    FindReplace.bDirectDown = false;
    if ( !SearchDoFind( FindReplace ) )           //      查找到文档开头都没有找到！
    {
      break;
    }
    ptResultFrom = FindReplace.ptFrom;
    ptResultTo = FindReplace.ptTo;
    hint.m_ptFindStart = FindReplace.ptFrom;
    // 检查找到的标识符是否是块的一部分，如果是则无效
    m_pdb->TextMainLineToSubLine( FindReplace.ptFrom );
    if ( ParserIsInsideBlock( FindReplace.ptFrom ) ) {
      continue;
    }
    nCountOff++;
    ptReverseScanFrom = ptResultTo;
    while ( true )           //      下面开始查找ON字符串并统计数量
    {
      FindReplace.bMatchCase = hint.m_bMatchCase;
      FindReplace.pszFindWhat = hint.m_strOn;
      FindReplace.cchFindWhat = hint.m_strOn.GetLength();
      FindReplace.ptFrom = ptReverseScanFrom;
      FindReplace.ptTo = ptReverseScanTo;
      FindReplace.bMatchWholeWord = true;
      FindReplace.bRegExp = false;
      FindReplace.bDirectDown = true;
      if ( !SearchDoFind( FindReplace )
           || ( FindReplace.ptFrom.y > ptReverseScanTo.y )
           || ( ( FindReplace.ptFrom.y == ptReverseScanTo.y )
                && ( FindReplace.ptFrom.x >= ptReverseScanTo.x ) ) ) {
        break;
      }
      ptReverseScanFrom = FindReplace.ptTo;
      // 检查找到的标识符是否是块的一部分，如果是则无效
      m_pdb->TextMainLineToSubLine( FindReplace.ptFrom );
      if ( !ParserIsInsideBlock( FindReplace.ptFrom ) ) {
        nCountOn++;
      }
    }
    ptReverseScanTo = ptResultFrom;
  }
  rpt = ptResultFrom;
  return ( nCountOn == nCountOff );
}
BOOL CPfxEditView::MatchBraceSearchDown( CPfxEditViewBraceHint& hint, CPoint& rpt ) {
  CPoint ptResultFrom;
  CPoint ptResultTo;
  CPoint ptReverseScanFrom = hint.m_ptFindStart;
  CPoint ptReverseScanTo;
  CPoint ptFindEnd;
  ptFindEnd.y = m_pdb->TextGetLineUpperBound();
  ptFindEnd.x = m_pdb->TextGetLineLength( ptFindEnd.y );
  m_pdb->TextSubLineToMainLine( ptFindEnd );
  CPfxEditViewFindReplace FindReplace;
  for ( LONG nCountOn = 1, nCountOff = 0; nCountOn != nCountOff; ) {
    FindReplace.bMatchCase = hint.m_bMatchCase;
    FindReplace.pszFindWhat = hint.m_strOff;
    FindReplace.cchFindWhat = hint.m_strOff.GetLength();
    FindReplace.ptFrom = hint.m_ptFindStart;
    FindReplace.ptTo = ptFindEnd;
    FindReplace.bMatchWholeWord = true;
    FindReplace.bRegExp = false;
    FindReplace.bDirectDown = true;
    if ( !SearchDoFind( FindReplace ) )           //      查找到文档结尾都没有找到！
    {
      break;
    }
    ptResultFrom = FindReplace.ptFrom;
    ptResultTo = FindReplace.ptTo;
    hint.m_ptFindStart = FindReplace.ptTo;
    // 检查找到的标识符是否是块的一部分
    m_pdb->TextMainLineToSubLine( FindReplace.ptFrom );
    if ( ParserIsInsideBlock( FindReplace.ptFrom ) ) {
      continue;
    }
    nCountOff++;
    ptReverseScanTo = ptResultFrom;
    while ( true ) {
      FindReplace.bMatchCase = hint.m_bMatchCase;
      FindReplace.pszFindWhat = hint.m_strOn;
      FindReplace.cchFindWhat = hint.m_strOn.GetLength();
      FindReplace.ptFrom = ptReverseScanFrom;
      FindReplace.ptTo = ptReverseScanTo;
      FindReplace.bMatchWholeWord = true;
      FindReplace.bRegExp = false;
      FindReplace.bDirectDown = true;
      if ( !SearchDoFind( FindReplace )
           || ( FindReplace.ptFrom.y > ptReverseScanTo.y )
           || ( ( FindReplace.ptFrom.y == ptReverseScanTo.y )
                && ( FindReplace.ptFrom.x >= ptReverseScanTo.x ) ) ) {
        break;
      }
      ptReverseScanFrom = FindReplace.ptTo;
      // 检查找到的标识符是否是块的一部分
      m_pdb->TextMainLineToSubLine( FindReplace.ptFrom );
      if ( !ParserIsInsideBlock( FindReplace.ptFrom ) ) {
        nCountOn++;
      }
    }
    ptReverseScanFrom = ptResultTo;
  }
  rpt = ptResultFrom;
  return ( nCountOn == nCountOff );
}
