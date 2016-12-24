#include <list>
#include <VECTOR>

template<typename T>
void dispt(T t)
{
	for (T::iterator it = t.begin(); it != t.end(); it++)
		cout << *it << " ";
	cout << endl;
}

////////////////////////////////////////////////////////////
class CMyObject
{
public:
	CMyObject() {}
	// 基类析构函数若需要delete的，务必定义为virtual，否则可能导致无能delete
	virtual ~CMyObject() {}
	
};
////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// CTList用于实现 list<T> m_tList; 的串行化

template <typename T = int>
class CTList : public CMyObject
{
public:
	list<T> m_tList;
	CTList() {}
	
	void Serialize(CXArchive& ar)
	{
		if (ar.IsStoring())
		{
			ar << m_tList.size();
			for (list<T>::iterator it = m_tList.begin(); it != m_tList.end(); it++)
				ar << *it;
		}
		else
		{
			int size;	ar >> size;
			for (int i = 0; i < size; ++i)
			{
				T t;
				ar >> t;
				m_tList.push_back(t);
			}
		}
		
	}

	////////////////////////////////////////////////////////////
	// 获取STL list中的指定元素只能用遍历了
	//CMyObject* getAt(list<CMyObject*>* pList, unsigned n)
	template <typename T>
	T* getAt(list<T>& xList, unsigned n)
	{
		list<T>::iterator it = xList.begin();
		for (unsigned i = 0; i < n; i++)
			it++;
		
		return &(*it);
	}
};

// CTList用于实现 list<T> m_tList; 的串行化
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// CTvec用于实现 vector<T> m_tvec; 的串行化

template <typename T = int>
class CTvec : public CMyObject
{
public:
	vector<T> m_tVec;
	CTvec() {}
	
	void Serialize(CXArchive& ar)
	{
		if (ar.IsStoring())
		{
			ar << m_tVec.size();
			for (vector<T>::iterator it = m_tVec.begin(); it != m_tVec.end(); it++)
				ar << *it;
		}
		else
		{
			int size;	ar >> size;
			for (int i = 0; i < size; ++i)
			{
				T t;
				ar >> t;
				m_tVec.push_back(t);
			}
		}
		
	}
};

// CTvec用于实现 vector<T> m_tvec; 的串行化
//////////////////////////////////////////////////////////////