// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifndef __AFXPLEX_H__
#define __AFXPLEX_H__

struct CPlex {   // warning variable length structure
  CPlex* pNext;
  // BYTE data[maxNum*elementSize];

  void* data() {
    return this + 1;
  }

  static CPlex* CPlex::Create(CPlex*& pHead, UINT nMax, UINT cbElement)
  {
	  ASSERT(nMax > 0 && cbElement > 0);
	  CPlex* p = (CPlex*) malloc(sizeof(CPlex) + nMax * cbElement);
	  if (p) {// may throw exception
	  p->pNext = pHead;
	  pHead = p;  // change head (adds in reverse order for simplicity)
	  }
	  return p;
  }
  
  void CPlex::FreeDataChain()     // free this one and links
  {
	  CPlex* p = this;
	  while (p != NULL)
	  {
		  BYTE* bytes = (BYTE*) p;
		  CPlex* pNext = p->pNext;
		  free(bytes);
		  p = pNext;
	  }
  }

};

#endif //__AFXPLEX_H__

/////////////////////////////////////////////////////////////////////////////
