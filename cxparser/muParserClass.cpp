#include "muParserDef.h"
#include "muParserClass.h"

#include "muParserBase.h"
#include "muParser.h"
#include "muParserInt.h"

namespace mu
{
	//void* operator new(size_t sz)
	//{
	//	return ::HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sz);
	//}
	//void operator delete (void* pz,size_t sz)
	//{
	//	::HeapFree(GetProcessHeap(),HEAP_ZERO_MEMORY,pz);
	//}
	//void* CWinFunc::muHeapAlloc(size_t sz)
	//{
	//	return ::HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sz);

	//}
	//void CWinFunc::muHeapFree (void* pz,size_t sz)
	//{
	//	::HeapFree(GetProcessHeap(),HEAP_ZERO_MEMORY,pz);
	//}
	//////////////////////////////////////////////////////////////////////////
	//createclass 

	void * CreateClass::addvar(const string_type & strobjname)
	{
		objmap_type::iterator item = m_objmap.find(strobjname);
		if (item!=m_objmap.end())
			return 0;
		objstruct * pobj =  new objstruct;
		void *pclassvar = NULL;
		//建立类对象成员变量表
		string_type astrname,astrtype;
		string_type astr = m_classname+"@"+strobjname + "@";//建立类对象成员类名
		for(int i=0;i<m_classdefbuf.size();i++)
		{
			astrname = astr + m_classStr[i];
			pclassvar=m_classdefbuf[i]->addvar(astrname);
			astrtype=m_classdefbuf[i]->getclass();
			if(astrtype==typeid(value_type).name())
			{
				//
				m_pCurParser->DefineVar(astrname,(value_type *)pclassvar);
			}

			pobj->m_objname.push_back(astrname);
			pobj->m_objbuf.push_back(pclassvar);
		}
		m_objmap[strobjname] = pobj;
		return pobj;
	};


	double CreateClass::ApplyClassFunc(void *pobj,const string_type &a_strFuncName,paramvect& parm)
	{
		//todo 			
		string_type strobjname;
		objmap_type::iterator item = m_objmap.begin();
		int inum=0;
		for (; item!=m_objmap.end(); ++item)
		{
			if(item->second == pobj)
			{
				strobjname = item->first;
				break;
			}
		}
		if(item == m_objmap.end())
			return 0;
		//get function run string and run function
		m_pCurParser->CompileFuncAndRunString(m_codemap[a_strFuncName]->m_strbuf,m_classname,strobjname);

	};
	//////////////////////////////////////////////////////////////////////////
	//通过给定对象和浮点型参数成员函数指针 调用类成员函数
	double CreateClass::ApplyClassFunc(void *pobj,void  *apclassfunc,paramvect& parm)
	{
		//
		
		return 0;
	}
	//////////////////////////////////////////////////////////////////////////
	//通过给定对象和指针型参数成员函数指针 调用类成员函数
	double CreateClass::ApplyClassFunc(void *pobj,void  *apclassfunc,voidparamvect& parm)
	{
		return 0;
	} 
	//////////////////////////////////////////////////////////////////////////
	//通过给定对象和字符串型参数成员函数指针 调用类成员函数
	double CreateClass::ApplyClassFunc(void *pobj,void  *apclassfunc,charpvect& parm)
	{
		return 0;
	};
}
