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
#include "muParserTokenReader.h"


#include "muParserBytecode.h"
#include "muParserError.h"

#include "muParserStrClassMap.h"


namespace mu
{

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	muParserStrClassMap::muParserStrClassMap():
	m_pclassgroup(NULL),
	m_iclassnum(0)
	{
	}

	muParserStrClassMap::~muParserStrClassMap()
	{
		if(NULL!=m_pclassgroup)
			delete m_pclassgroup;
		m_iclassnum=0;
	}

}
