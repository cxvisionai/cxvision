// ParserTreeNode.h: interface for the CParserTreeNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__PARSERTREENODE_H__)
#define __PARSERTREENODE_H__

#include "muParserBytecode.h"


namespace mu
{
        class   muParserStrClassMap
	{
		private:
			
			typedef std::map<string_type,string_type> strmapfunc_type;//

			typedef struct stringvirclass//字串虚拟类
			{
				string_type m_classdef;//类字串定义
				strmapfunc_type m_classfuncdef;//类功能定义

			}strclass;

			typedef std::map<string_type,strclass*> classmap_type;//string class map

		public:
			classmap_type m_strclassmap;
			strclass *m_pclassgroup;//类存储列表
			int m_iclassnum;		//类个数
			
			muParserStrClassMap();
			virtual ~muParserStrClassMap();
			void AddClass(const string_type &a_strClassName)
			{
					classmap_type::iterator item = m_strclassmap.find(a_strClassName);
					if(item!=m_strclassmap.end())
						return ;
					strclass *apclass =new strclass;
					m_strclassmap[a_strClassName]=apclass;
			}
			void AddClassDef(const string_type &a_strClassName,
							const string_type &a_strclassdef)
			{
				classmap_type::iterator item = m_strclassmap.find(a_strClassName);
				strclass *apclass=NULL;
				if(item==m_strclassmap.end())
				{
					apclass =new strclass;
					m_strclassmap[a_strClassName]=apclass;
				}
				apclass->m_classdef	= a_strclassdef;
			}
			void AddClassFun(const string_type &a_strClassName,
							const string_type &a_strclassfuncname,
							const string_type &a_strclassfuncdef)
			{
				classmap_type::iterator item = m_strclassmap.find(a_strClassName);
				
				if(item!=m_strclassmap.end())
					return;
				
				strclass *apclass=item->second;
				apclass->m_classfuncdef[a_strclassfuncname]=a_strclassfuncdef;
			}
	};
}
#endif // !defined(AFX_PARSERTREENODE_H__1E493C04_D688_4B97_99F3_DAA141F41111__INCLUDED_)
