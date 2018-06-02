// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
#include "stdafx.h"
#ifdef AFX_DB_SEG
//#pragma code_seg(AFX_DB_SEG)
#endif
//////////////////////////////////////////////////////////////////////////////
// Implementation of CDBVariant
CDBVariant::CDBVariant()
{
  // Initialize type and value
  m_dwType = DBVT_NULL;
}
void CDBVariant::Clear()
{
  if (m_dwType == DBVT_STRING) {
    delete(CString*)m_pstring;
  }
  else if (m_dwType == DBVT_BINARY) {
    delete(CLongBinary*)m_pbinary;
  }
  else if (m_dwType == DBVT_DATE) {
    delete(TIMESTAMP_STRUCT*)m_pdate;
  }
  m_dwType = DBVT_NULL;
}
CDBVariant::~CDBVariant()
{
  Clear();
}
//////////////////////////////////////////////////////////////////////////////
#ifdef AFX_INIT_SEG
//#pragma code_seg(AFX_INIT_SEG)
#endif
/////////////////////////////////////////////////////////////////////////////

