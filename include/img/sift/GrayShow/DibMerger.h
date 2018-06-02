// DibMerger.h: interface for the CDibMerger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIBMERGER_H_INCLUDED_)
#define AFX_DIBMERGER_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDibMerger  
{
public:
	CDibMerger();
	virtual ~CDibMerger();
private:
	int m_iMAXarea;
	//左图重叠开始的横坐标位置
	int left_starX; 
	//左图重叠开始的纵坐标位置
	int left_starY;
    //左图重叠结束的横坐标位置
	int left_endX;
	//左图重叠结束的纵坐标位置
	int left_endY;
	//右图重叠开始的横坐标位置
	int right_starX;
	//右图重叠开始的纵坐标位置
	int right_starY;
	//右图重叠结束的横坐标位置
	int right_endX;
	//右图重叠结束的纵坐标位置
	int right_endY;
};

#endif // !defined(AFX_DIBMERGER_H_INCLUDED_)
