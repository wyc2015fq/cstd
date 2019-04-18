# stl vector 模拟实现修改版 - ljx0305的专栏 - CSDN博客
2009年03月26日 08:37:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：674标签：[vector																[iterator																[delete																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=iterator&t=blog)](https://so.csdn.net/so/search/s.do?q=vector&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
// Iterator.cpp : Defines the entry point for the console application.
//
#include "stdafx.h" 
#include <iostream> 
#include <new>
usingnamespace std;
template <class T>
class Iterator
{
public:
    Iterator()
        : m_pData(NULL)//, m_nPos(0)
        , m_nSize(0), m_nLen(0)
    {
    }
virtual ~Iterator()
    {
if (NULL != m_pData)
        {
            delete[] m_pData;
            m_pData = NULL;
        }
    }
public:
virtual T* begin() = 0;
virtual T* end() = 0;
protected:
// int  m_nPos;
int m_nSize;
int m_nLen;
    T *m_pData;
};
template <class T>
class CMyVector : public Iterator<T>
{
public:
    typedef T* iterator;
public:
    CMyVector(int nSize = 10)
    {
if (nSize <= 0)
        {
            nSize = 10;
        }
        m_nSize = nSize;
        m_pData = new T[m_nSize];
    }
    ~CMyVector()
    {
    }
public:
int Length() const
    {
return m_nLen;
    }
bool push_back(T obj)
    {
if (m_nLen >= m_nSize)
        {
//get more memory
int nSize = m_nSize * 2 + 1;
            T *pTemp = new(nothrow) T[nSize];
if (!pTemp)
            {
returnfalse;
            }
// memset(pTemp, 0, sizeof(T) * m_nSize);
            memcpy(pTemp, m_pData, sizeof(T) * m_nLen);         
            delete []m_pData;
            m_pData = pTemp;
            m_nSize = nSize;
        }
        memcpy(m_pData + m_nLen, &obj, sizeof(obj));
        m_nLen++;
returntrue;
    }
public:
virtual T* end()
    {
return m_pData + m_nLen;
    }
virtual T* begin()
    {
// m_nPos = 0;
return m_pData;
    }
};
int _tmain(int argc, _TCHAR* argv[])
{
    CMyVector<int> vtData;
    CMyVector<int>::iterator it;
for (int i = 0; i < 30; i++)
    {
        vtData.push_back(i);
//cout << "nLen = " << vtData.Length() << endl;
    }
    cout << "vector data: " << endl;
for (it = vtData.begin(); it != vtData.end(); it++)
    {
        cout << *it << "/t";
    }
    cout << endl;
return 0;
}
引用于:http://blog.csdn.net/andylin02/archive/2008/11/27/3392746.aspx
