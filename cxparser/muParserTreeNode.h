// ParserTreeNode.h: interface for the CParserTreeNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__PARSERTREENODE_H__)
#define __PARSERTREENODE_H__


#include "muParserBytecode.h"
typedef struct  POINTXY
{
	long x;
	long y;
}	POINTXY;
namespace mu
{
	class muParserTreeNode  
	{
		private:
			
			typedef std::vector<ParserByteCode::map_type> storage_type;

		public:

			storage_type m_Base;
			string_type m_string;
			std::vector<muParserTreeNode*> m_Vusenode;
			std::vector<muParserTreeNode*> m_VuseTHISnode;
		
			//assist attribution
			POINTXY m_point;
			muParserTreeNode();
			virtual ~muParserTreeNode();
			
			void SetBase(storage_type Base);		
			storage_type GetBase();
			void SetString(string_type str);
			void SetPoint( int nEditItem,int nEditSubItem);
			string_type GetString();
			
			void AddParserUseNode(muParserTreeNode* pNode);
			muParserTreeNode* FindUseNode(string_type str);
			bool DeleteParserUseNode(string_type str);
			void AddParserUseThisNode(muParserTreeNode* pNode);
			muParserTreeNode* FindUseThisNode(string_type str);
			bool DeleteParserUseThisNode(string_type str);
			void ClearUseNode();
			void ClearUseThisNode();
	};
}
#endif // !defined(AFX_PARSERTREENODE_H__1E493C04_D688_4B97_99F3_DAA141F41111__INCLUDED_)
