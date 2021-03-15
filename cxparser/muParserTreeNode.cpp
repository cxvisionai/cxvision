// ParserTreeNode.cpp: implementation of the CParserTreeNode class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include <cmath>
#include <string>
#include <iostream>
#include <map>
#include <memory>
#include "muParserDef.h"
#include "muParserStack.h"

//////////////////////////////////////////////////////////////////////////

#include "muParserBytecode.h"
#include "muParserError.h"
#include "muParserTreeNode.h"


namespace mu
{

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	muParserTreeNode::muParserTreeNode()
	{

	}

	muParserTreeNode::~muParserTreeNode()
	{

	}
	void muParserTreeNode::SetBase(storage_type Base)
		{
			m_Base=Base;
		}
		
	muParserTreeNode::storage_type  muParserTreeNode::GetBase()
		{
			return m_Base;
		}
		
		void  muParserTreeNode::SetString(string_type str)
		{
			m_string=str;
		}
		void muParserTreeNode::SetPoint(int x,int y)
		{
			m_point.x=x;
			m_point.y=y;
		}
		
		string_type  muParserTreeNode::GetString()
		{
			return m_string;
		}
		
		void  muParserTreeNode::AddParserUseNode(muParserTreeNode* pNode)
		{
			m_Vusenode.push_back(pNode);
		}

		void  muParserTreeNode::ClearUseNode()
		{
			m_Vusenode.clear();
		}
		
		void muParserTreeNode::ClearUseThisNode()
		{
			m_VuseTHISnode.clear();
		}
		muParserTreeNode*  muParserTreeNode::FindUseNode(string_type str)
		{
			for(int i=0;i<m_Vusenode.size();i++)
			{
				if(str==m_Vusenode[i]->m_string)
					return m_Vusenode[i];
			}
			return NULL;
		}
		
		bool  muParserTreeNode::DeleteParserUseNode(string_type str)
		{
			int iGetit=-1;
			std::vector < muParserTreeNode* > ::iterator iter;
			for(int i=0;i<m_Vusenode.size();i++)
			{
				if(str==m_Vusenode[i]->m_string)
				{
					iGetit=i;
					break;
				}
			}
			if(-1!=iGetit)
			{
				m_Vusenode.erase(m_Vusenode.begin()+iGetit);
			}
			else
			{
				return false;
			}
		}

		void  muParserTreeNode::AddParserUseThisNode(muParserTreeNode* pNode)
		{
			m_VuseTHISnode.push_back(pNode);
		}

		muParserTreeNode*  muParserTreeNode::FindUseThisNode(string_type str)
		{
			for(int i=0;i<m_VuseTHISnode.size();i++)
			{
				if(str==m_VuseTHISnode[i]->m_string)
					return m_VuseTHISnode[i];
			}
			return NULL;
		}
		
		bool  muParserTreeNode::DeleteParserUseThisNode(string_type str)
		{
			int iGetit=-1;
			std::vector < muParserTreeNode* > ::iterator iter;
			for(int i=0;i<m_VuseTHISnode.size();i++)
			{
				if(str==m_VuseTHISnode[i]->m_string)
				{
					iGetit=i;
					break;
				}
			}
			if(-1!=iGetit)
			{
				m_VuseTHISnode.erase(m_VuseTHISnode.begin()+iGetit);
			}
			else
			{
				return false;
			}
		}

}
