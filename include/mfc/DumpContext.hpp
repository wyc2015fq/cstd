
#ifndef __CDUMPCONTEXT_HPP__
#define __CDUMPCONTEXT_HPP__


#include "typedef.h"
#include "StringBuilder.hpp"

class CDumpContext : public StringBuilder
{
public:
	CString m_out;
	int m_nDepth;
public:
	// CDumpContext
	int GetDepth() const
	{
		return m_nDepth;
	}
	void SetDepth(int nNewDepth)
	{
		m_nDepth = nNewDepth;
	}

	CDumpContext() : StringBuilder(m_out)
	{
		m_nDepth = 0;
	}

	void Flush()
	{
	}



};

#endif // __CDUMPCONTEXT_HPP__
