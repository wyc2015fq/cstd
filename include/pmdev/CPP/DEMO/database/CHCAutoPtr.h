#ifndef CHC_AUOTPTR_H
#define CHC_AUOTPTR_H

#include <windows.h>
template<typename T>
class CHCAutoPtr
{
	class CCounter
	{
	public:
		CCounter() : m_counter(0){}
		~CCounter(){}
		long m_counter;
	};
public:
	CHCAutoPtr() : m_pElement(NULL), m_pCounter(new CCounter){}
	~CHCAutoPtr(){
		DecreaseCounter();
	}
	explicit CHCAutoPtr(T* pElement) : m_pElement(pElement), m_pCounter(new CCounter){
		IncreaseCounter();
	}
	CHCAutoPtr(const CHCAutoPtr& element) : m_pElement(element.m_pElement), m_pCounter(element.m_pCounter){
		IncreaseCounter();
	}
	CHCAutoPtr& operator =(const CHCAutoPtr& element){
		if (this != &element)
		{
			DecreaseCounter();
			m_pElement = element.m_pElement;
			m_pCounter = element.m_pCounter;
			IncreaseCounter();
		}
		return *this;
	}
	CHCAutoPtr& operator =(T* pElement){
		if (m_pElement != pElement)
		{
			DecreaseCounter();
			m_pElement = pElement;
			m_pCounter = new CCounter;
			IncreaseCounter();
		}
		return *this;
	}
	BOOL operator ==(T* pElement){return m_pElement == pElement;}
	BOOL operator ==(const CHCAutoPtr& element){return m_pElement == element.m_pElement;}

	BOOL operator !=(T* pElement){return m_pElement != pElement;}
	BOOL operator !=(const CHCAutoPtr& element){return m_pElement != element.m_pElement;}
public:
	T* operator->(){return m_pElement;}
	T& operator*(){return *m_pElement;}
	operator T*(){return m_pElement;}
	operator const T*(){return m_pElement;}
	T* Get(){return m_pElement;}
	BOOL IsNull(){ return m_pElement ? FALSE:TRUE;}
private:
	void IncreaseCounter()
	{
		if (m_pCounter) ::InterlockedIncrement(&(m_pCounter->m_counter));
	}
	void DecreaseCounter()
	{
		if (m_pCounter == NULL) return;
		if (::InterlockedDecrement(&(m_pCounter->m_counter)) == 0)
		{
			delete m_pCounter;
			m_pCounter = NULL;
			delete m_pElement;
			m_pElement = NULL;
		}
	}
private:
	T* m_pElement;
	CCounter* m_pCounter;
};
#endif