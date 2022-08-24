/** 
  Copyright (C) 2004-2006 Ingo Berg

  Permission is hereby granted, free of charge, to any person obtaining a copy of this 
  software and associated documentation files (the "Software"), to deal in the Software
  without restriction, including without limitation the rights to use, copy, modify, 
  merge, publish, distribute, sublicense, and/or sell copies of the Software, and to 
  permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or 
  substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
  NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
*/
#ifndef MU_PARSER_BASE_H
#define MU_PARSER_BASE_H

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
#include "muParserClass.h"
#include "muParserTreeNode.h"
#include "muparserClassReader.h"
#include "muParserClassFunctionReader.h"


namespace mu
{

/** \brief Mathematical expressions parser (base parser engine).
  
  Version 1.26 (20060904)

  This is the implementation of a bytecode based mathematical expressions parser. 
  The formula will be parsed from string and converted into a bytecode. 
  Future calculations will be done with the bytecode instead the formula string
  resulting in a significant performance increase. 
  Complementary to a set of internally implemented functions the parser is able to handle 
  user defined functions and variables. 

  \author (C) 2004-2006 Ingo Berg
*/
	static const char_type *c_DefaultOprt[28]=
	{ 
		"<=",  ///< Operator item:  cmLE
		">=",  ///< Operator item:  cmGE
		"!=",  ///< Operator item:  cmNEQ
		"==",  ///< Operator item:  cmEQ
		"<",   ///< Operator item:  cmLT 
		">",   ///< Operator item:  cmGT
		"+",  ///< Operator item:  cmADD
		"-",   ///< Operator item:  cmSUB
		"*",  ///< Operator item:  cmMUL
		"/",   ///< Operator item:  cmDIV
		"^",  ///< Operator item:  cmPOW
		"and",  ///< Operator item:  cmAND
		"or",   ///< Operator item:  cmOR
		"xor",  ///< Operator item:  cmXOR
		"=",  ///< Operator item:  cmASSIGN
		"(",   ///< Operator item:  cmBO
		")",  ///< Operator item:  cmBC
		",",  ///< Operator item:  cmCOMMA
		";",  ///< Operator item:  cmSEMICOLON
		"{",  ///< Operator item:  cmLB
		"}",  ///< Operator item:  cmRB
		".",  ///< Operator item:  cmMember
		"->",  ///< Operator item:  cmPointer
		"if",  ///< Operator item:  cmIf
		"else", ///< Operator item:  cmElse
		"while", ///< Operator item:  cmWhile
		"for",
		0		///cxyadd ";","{","}", ".","->","if","else","while"
	};
class   ParserBase
{
friend class ParserTokenReader;
friend class ParserClassReader;
friend class ParserClassFunctionReader;
private:
    typedef value_type (ParserBase::*ParseFunction)() const;  

	typedef ParserToken<value_type, string_type> token_type;

    typedef std::vector<string_type> stringbuf_type;

    typedef ParserTokenReader token_reader_type;



	//cxyaddbegin
    typedef ParserToken<vision_type, string_type> token_viosiontype;

	//cxyaddend
 public: 

    /** \brief Type of the error class. 
    
      Included for backwards compatibility.
    */
    typedef ParserError exception_type;

    ParserBase(); 
    ParserBase( const ParserBase &a_Parser );
    ParserBase& operator=(const ParserBase &a_Parser);

    //---------------------------------------------------------------------------
    /** \brief Destructor. (trivial) 

        \throw nothrow
    */
    virtual ~ParserBase()
    {
		//cxyadd 

			classbasemap_type:: iterator pIter,pIter2;
			for ( pIter = m_ClassDefMap.begin( ) ; pIter != m_ClassDefMap.end( ) ;)
			{
				pIter2=pIter;
				pIter++ ;
				//cxy noted !!! need to delete m_ClassDefMap here
				//m_ClassDefMap.delete()

				classbase *pbase=pIter2->second;
				switch(pbase->m_iclasstype)
				{
					case CLASS_ORG:
						//OrgClass *pclass = (OrgClass *)pbase;
						//delete pclass;
					break;
					case CLASS_PARSER:
						//ParserClass *pclass = (ParserClass*)pbase;
						//delete pclass;
					break;
					case CLASS_CREATE:
						//CreateClass *pclass = (CreateClass*)pbase;
						//delete pclass;
					break;
					default:
						//delete pbase;
					break;
				}
				delete pbase;
				
			}
		//exit(1);
	}
    
    //---------------------------------------------------------------------------
    /** \brief Calculate the result.

      A note on const correctness: 
      I consider it important that Calc is a const function.
      Due to caching operations Calc changes only the state of internal variables with one exception
      m_UsedVar this is reset during string parsing and accessible from the outside. Instead of making
      Calc non const GetUsedVar is non const because it explicitely calls Eval() forcing this update. 

      \pre A formula must be set.
      \pre Variables must have been set (if needed)
  
      \sa #m_pParseFormula
      \return The evaluation result
      \throw ParseException if no Formula is set or in case of any other error related to the formula.
    */
  	inline value_type Eval() const
    {
      return (this->*m_pParseFormula)(); 
    }

    void SetExpr(const string_type &a_sExpr);
	void SetVarFactory(facfun_type a_pFactory,void *a_pvoid=0);
    void EnableOptimizer(bool a_bIsOn=true);
    void EnableByteCode(bool a_bIsOn=true);
    void EnableBuiltInOprt(bool a_bIsOn=true);
    bool HasBuiltInOprt() const;
    void AddValIdent(identfun_type a_pCallback);

#define MUP_DEFINE_FUNC(TYPE)                                                           \
    inline void DefineFun(const string_type &a_strName, TYPE a_pFun, bool a_bAllowOpt = true)  \
	{                                                                                   \
		AddCallback( a_strName, ParserCallback(a_pFun, a_bAllowOpt),                      \
		m_FunDef, ValidNameChars() );                                        \
	}

    MUP_DEFINE_FUNC(fun_type1)
    MUP_DEFINE_FUNC(fun_type2)
    MUP_DEFINE_FUNC(fun_type3)
    MUP_DEFINE_FUNC(fun_type4)
    MUP_DEFINE_FUNC(fun_type5)
    MUP_DEFINE_FUNC(multfun_type)
    MUP_DEFINE_FUNC(strfun_type1)
#undef MUP_DEFINE_FUNC


    void DefineOprt(const string_type &a_strName, fun_type2 a_pFun, unsigned a_iPri=0, bool a_bAllowOpt = false);
    void DefineConst(const string_type &a_sName, value_type a_fVal);
    void DefineStrConst(const string_type &a_sName, const string_type &a_strVal);
    void DefineVar(const string_type &a_sName, value_type *a_fVar);
    void DefinePostfixOprt(const string_type &a_strFun, fun_type1 a_pOprt, bool a_bAllowOpt=true);
    void DefineInfixOprt(const string_type &a_strName, fun_type1 a_pOprt, int a_iPrec=prINFIX, bool a_bAllowOpt=true);
	void DefineGetAdress(const string_type &a_strName,fun_lptype a_pFun, int a_iPrec=prINFIX, bool a_bAllowOpt=true);

	//////////////////////////////////////////////////////////////////////////
	//cxyadd <ver1>///////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

#define MUP_DEFINE_VOID_FUNC(TYPE)                                                               \
	inline void DefineVoidFun(const string_type &a_strName, TYPE a_pFun, bool a_bAllowOpt = true)  \
	{                                                                                              \
		AddCallback( a_strName, ParserCallback(a_pFun, a_bAllowOpt),						       \
		m_FunDef, ValidNameChars() );														       \
	}

	MUP_DEFINE_VOID_FUNC(fun_void_type1)
	MUP_DEFINE_VOID_FUNC(fun_void_type2)
	MUP_DEFINE_VOID_FUNC(fun_void_type3)
	MUP_DEFINE_VOID_FUNC(fun_void_type4)
	MUP_DEFINE_VOID_FUNC(fun_void_type5)
	MUP_DEFINE_VOID_FUNC(multfun_void_type)
	MUP_DEFINE_VOID_FUNC(strfun_void_type1)
#undef MUP_DEFINE_VOID_FUNC


	//
	void UsingClass(bool usingclass)
	{
		m_pTokenReader->UsingClassDef(true);
    }

	template<class ACLASS>
	void DefineOrgClass(const char *a_szName,//const string_type &a_sName,
						ACLASS *apclass)
	{
		string_type a_sName(a_szName);
		//// Test if a constant with that names already exists
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sName);
		if (itor!=m_ClassDefMap.end())
		//if (m_ClassDefMap.find(a_sName)!=m_ClassDefMap.end())
			Error(ecNAME_CONFLICT);

		CheckName(a_sName, ValidNameChars());

		OrgClass<ACLASS> *paclass=new OrgClass<ACLASS>;

		m_ClassDefMap[a_sName] = (classbase*) paclass;

		ReInit();
	}

	template<class ACLASS>
	void DefineClass(const char *a_szName,/*const string_type &a_sName,*/
						ACLASS *apclass)
	{
		//// Test if a constant with that names already exists
		string_type a_sName(a_szName);
		if (m_ClassDefMap.find(a_sName)!=m_ClassDefMap.end())
			Error(ecNAME_CONFLICT);

		CheckName(a_sName, ValidNameChars());

		ParserClass<ACLASS> *paclass=new ParserClass<ACLASS>;

		m_ClassDefMap[a_sName] = (classbase*) paclass;

		ReInit();
	}


	void DefineCreateClass(const char *a_szName,/*const string_type &a_sName,*/
							const char *a_szstr)/*class def string*/
	{
		//// Test if a constant with that names already exists
		string_type a_sName(a_szName);

		if (m_ClassDefMap.find(a_sName)!=m_ClassDefMap.end())
			Error(ecNAME_CONFLICT);

		CheckName(a_sName, ValidNameChars());

		CreateClass *paclass=new CreateClass(a_sName,this);

		
		m_ClassDefMap[a_sName] = (classbase*) paclass;
		
		//paclass->AddClassDef()
		
		CompileClassDeclara(a_szstr,paclass);
		ReInit();
    }
	void DefineCreateClasFun(const char *a_szClassName,//const string_type&a_sClassName,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		const char *a_szFuncStr)
	{
		string_type a_sClassName(a_szClassName);
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		string_type a_sClassFunStr(a_szFuncStr);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		CreateClass *pCreateclass = (CreateClass *)pclass;

		pCreateclass->AddClassFun(a_szClassmemberFuncName,a_sClassFunStr);
	}
	//template<class ACLASS,class PARAM>
	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
						ACLASS *apclass,
						const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
						void (ACLASS::*aparclass)(void ) )
	{
		string_type a_sClassName(a_szClassName);
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);
		
		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}

	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
						ACLASS *apclass,
						const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
						void (ACLASS::*aparclass)(double ) )
	{
		//// Test if a constant with that names already exists
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}

	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		void (ACLASS::*aparclass)(double,double ) )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}

	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		void (ACLASS::*aparclass)(double,double,double ) )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}


	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		void (ACLASS::*aparclass)(double,double,double,double ) )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}



	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		void (ACLASS::*aparclass)(int ) )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}
	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		void (ACLASS::*aparclass)(int,int ) )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}
	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		void (ACLASS::*aparclass)(int,int,int ) )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}
	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		void (ACLASS::*aparclass)(int,int,int,int ) )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}

	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		void (ACLASS::*aparclass)(int,int,int ,int,int) )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}

	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		void (ACLASS::*aparclass)(int,int,int,int ,int,int) )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}

	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		void (ACLASS::*aparclass)(int,int,int,int,int ,int,int) )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}


	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		void (ACLASS::*aparclass)(const char *) )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}

	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		void (ACLASS::*aparclass)(void *) )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}
	
	//

	//template<class ACLASS>
	//void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
	//	ACLASS *apclass,
	//	const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
	//	void (ACLASS::*aparclass)(void *,void *) )
	//{
	//	string_type a_sClassName( a_szClassName); 
	//	string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
	//	//// Test if a constant with that names already exists
	//	classbase *pclass;
	//	classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
	//	if (itor!=m_ClassDefMap.end())
	//		pclass=itor->second;
	//	else
	//		Error(ecNAME_CONFLICT);

	//	ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
	//	ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

	//	ReInit();
	//}
	//template<class ACLASS>
	//void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
	//	ACLASS *apclass,
	//	const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
	//	void (ACLASS::*aparclass)(void *,void *,void *) )
	//{
	//	string_type a_sClassName( a_szClassName); 
	//	string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
	//	//// Test if a constant with that names already exists
	//	classbase *pclass;
	//	classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
	//	if (itor!=m_ClassDefMap.end())
	//		pclass=itor->second;
	//	else
	//		Error(ecNAME_CONFLICT);

	//	ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
	//	ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

	//	ReInit();
	//}

	//template<class ACLASS>
	//void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
	//	ACLASS *apclass,
	//	const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
	//	void (ACLASS::*aparclass)(void *,void *,void *,void *) )
	//{
	//	string_type a_sClassName( a_szClassName); 
	//	string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
	//	//// Test if a constant with that names already exists
	//	classbase *pclass;
	//	classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
	//	if (itor!=m_ClassDefMap.end())
	//		pclass=itor->second;
	//	else
	//		Error(ecNAME_CONFLICT);

	//	ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
	//	ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

	//	ReInit();
	//}
	//template<class ACLASS>
	//void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
	//	ACLASS *apclass,
	//	const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
	//	void (ACLASS::*aparclass)(void *,void *,void *,void *,void *) )
	//{
	//	string_type a_sClassName( a_szClassName); 
	//	string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
	//	//// Test if a constant with that names already exists
	//	classbase *pclass;
	//	classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
	//	if (itor!=m_ClassDefMap.end())
	//		pclass=itor->second;
	//	else
	//		Error(ecNAME_CONFLICT);

	//	ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
	//	ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

	//	ReInit();
	//}
	
	//

	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		double (ACLASS::*aparclass)(void *) )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}

	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		double (ACLASS::*aparclass)(int ) )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}
	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		double (ACLASS::*aparclass)(int,int ) )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}

	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		double (ACLASS::*aparclass)(int,int,int ) )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}

	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		int (ACLASS::*aparclass)(int,int ) )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}
	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		int (ACLASS::*aparclass)(int ) )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}

	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		int (ACLASS::*aparclass)() )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}
	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		double (ACLASS::*aparclass)() )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}
	template<class ACLASS>
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		ACLASS *apclass,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		char* (ACLASS::*aparclass)() )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		ParserClass<ACLASS> *ptureclass=(ParserClass<ACLASS> *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, aparclass);

		ReInit();
	}
//////////////////////////////////////////////////////////////////////////
//createclass
	void DefineClassFun(const char *a_szClassName,//const string_type&a_sClassName,
		const char *a_szClassmemberFuncName,//const string_type&a_sClassmemberFuncName,
		const char *a_szClassFuncStr )
	{
		string_type a_sClassName( a_szClassName); 
		string_type a_sClassmemberFuncName(a_szClassmemberFuncName);
		string_type a_sFuncstr(a_szClassFuncStr);
		//// Test if a constant with that names already exists
		classbase *pclass;
		classbasemap_type::iterator itor=m_ClassDefMap.find(a_sClassName);
		if (itor!=m_ClassDefMap.end())
			pclass=itor->second;
		else
			Error(ecNAME_CONFLICT);

		CreateClass *ptureclass=(CreateClass *)pclass;
		ptureclass->AddClassFun(a_sClassmemberFuncName, a_sFuncstr);

		ReInit();
    }
	void SetClassObjectPoint()
	{

    }
	void SetClassMemberVarPoint()
	{

	}



	//////////////////////////////////////////////////////////////////////////
	//cxyadd end//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////



    // Clear user defined variables, constants or functions
  	void ClearVar();
    void ClearFun();
    void ClearConst();
    void ClearInfixOprt();
    void ClearPostfixOprt();
    void ClearOprt();
    void ClearClassObj();
    void RemoveVar(const string_type &a_strVarName);
	void RemoveClassObject(const string_type &a_strClassnName);
	void RemoveClassObject(const string_type &a_strClassnName,const string_type &a_strOjectName);
    const varmap_type& GetUsedVar() const;
    const varmap_type& GetVar() const;
    const valmap_type& GetConst() const;
    const string_type& GetExpr() const;
    const funmap_type& GetFunDef() const;

	const classbasemap_type& GetClassMap() const;
	const string_type& GetFormula() const;
	
    //---------------------------------------------------------------------------
    /** \brief Return the strings of all Operator identifiers. 
    
        GetOprt is a const function returning a pinter to an array of const char pointers.
        
        \return Returns a pointer to the c_DefaultOprt array of const char *.
        \throw nothrow
		"<=", ">=", "!=", "==", "<", ">", "+", "-", "*", "/", 
		"^", "and", "or", "xor", "=", "(", ")", ",", 0 
    */
    const char_type ** GetOprtDef() const
    {
      return (const char **)(&c_DefaultOprt[0]);
    }

    //---------------------------------------------------------------------------
    /** \brief Define the set of valid characters to be used in names of
               functions, variables, constants.
    */
    void DefineNameChars(const char_type *a_szCharset)
    {
      m_sNameChars = a_szCharset;
    }

    //---------------------------------------------------------------------------
    /** \brief Define the set of valid characters to be used in names of
               binary operators and postfix operators.
    */
    void DefineOprtChars(const char_type *a_szCharset)
    {
		m_sOprtChars = a_szCharset;
    }

    //---------------------------------------------------------------------------
    /** \brief Define the set of valid characters to be used in names of
               infix operators.
    */
    void DefineInfixOprtChars(const char_type *a_szCharset)
    {
		m_sInfixOprtChars = a_szCharset;
    }

    //---------------------------------------------------------------------------
    /** \brief Virtual function that defines the characters allowed in name identifiers. 
        \sa #ValidOprtChars, #ValidPrefixOprtChars
    */ 
    const char_type* ValidNameChars() const
    {/*DefineNameChars("0123456789_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");*/
		assert(m_sNameChars.size());
		return m_sNameChars.c_str();
    }

    //---------------------------------------------------------------------------
    /** \brief Virtual function that defines the characters allowed in operator definitions. 
        \sa #ValidNameChars, #ValidPrefixOprtChars
    */
    const char_type* ValidOprtChars() const
    {
		assert(m_sOprtChars.size());
		return m_sOprtChars.c_str();
    }

 
    /** \brief Virtual function that defines the characters allowed in infix operator definitions.
        \sa #ValidNameChars, #ValidOprtChars
    */
    const char_type* ValidInfixOprtChars() const
    {
		assert(m_sInfixOprtChars.size());
		return m_sInfixOprtChars.c_str();
    }

    void  Error( EErrorCodes a_iErrc, 
                 int a_iPos = (int)mu::string_type::npos, 
                 const string_type &a_strTok = string_type() ) const;
	void RunCode()
	{
		RunCollectionCmdCode();
	}

 protected:
	  
    //---------------------------------------------------------------------------
    /** \brief Initialize user defined functions. 
     
      Calls the virtual functions InitFun(), InitConst() and InitOprt().
    */
    void Init()
    {
      InitCharSets();
      InitFun();
      InitConst();
      InitOprt();
    }

    //---------------------------------------------------------------------------
    virtual void InitCharSets() = 0;
    virtual void InitFun() = 0;
    virtual void InitConst() = 0;
    virtual void InitOprt() = 0; 

 private:
    void Assign(const ParserBase &a_Parser);
    void InitTokenReader();
	void InitClassReader();
	void InitClassFunReader();
    void ReInit() const;
	void EndExpress() const;
    void AddCallback( const string_type &a_strName, 
                      const ParserCallback &a_Callback, 
                      funmap_type &a_Storage,
                      const char_type *a_szCharSet );

    void ApplyBinOprt(ParserStack<token_type> &a_stOpt,
                      ParserStack<token_type> &a_stVal,
					  ParserStack<token_type> &a_classobj) const;

    void ApplyFunc(ParserStack<token_type> &a_stOpt,					
                   ParserStack<token_type> &a_stVal, 
				   ParserStack<token_type> &a_classobj,
                   int iArgCount) const; 


    token_type ApplyNumFunc(const token_type &a_FunTok,
                            const std::vector<token_type> &a_vArg) const;

    token_type ApplyStrFunc(const token_type &a_FunTok,
                            token_type &a_Arg) const;

    int GetOprtPri(const token_type &a_Tok) const;

    value_type ParseString() const; 
    value_type ParseCmdCode() const;
    value_type ParseValue() const;

    void  ClearFormula();
    void  CheckName(const string_type &a_strName, const string_type &a_CharSet) const;

#if defined(MU_PARSER_DUMP_STACK) | defined(MU_PARSER_DUMP_CMDCODE)
    void StackDump(const ParserStack<token_type > &a_stVal, 
                   const ParserStack<token_type > &a_stOprt) const;
#endif

    /** \brief Pointer to the parser function.     
      Eval() calls the function whose address is stored there.
    */
	mutable ParseFunction  m_pParseFormula; ///< Function callback adress such as muParserBase::ParseString
    mutable const ParserByteCode::map_type *m_pCmdCode; ///< Formula converted to bytecode, points to the data of the bytecode class.
    mutable ParserByteCode m_vByteCode;   ///< The Bytecode class.
    mutable stringbuf_type m_vStringBuf; ///< String buffer, used for storing string function arguments

    stringbuf_type m_vStringVarBuf;

    /** \brief Managed pointer to the token reader object. */
    std::auto_ptr<token_reader_type> m_pTokenReader; 
    std::auto_ptr<ParserClassReader> m_pClassReader; 
	std::auto_ptr<ParserClassFunctionReader> m_pClassFunReader;

    funmap_type  m_FunDef;        ///< Map of function names and pointers.
    funmap_type  m_PostOprtDef;   ///< Postfix operator callbacks
    funmap_type  m_InfixOprtDef;  ///< unary infix operator.
    funmap_type  m_OprtDef;       ///< Binary operator callbacks
 
	valmap_type  m_ConstDef;      ///< user constants.
    strmap_type  m_StrVarDef;     ///< user defined string constants
    varmap_type  m_VarDef;        ///< user defind variables.
	stringmap_type m_StringVarDef;///< user defined string variables constants

    bool m_bOptimize;             ///< Flag that indicates if the optimizer is on or off.
    bool m_bUseByteCode;          ///< Flag that indicates if bytecode parsing is on or off.
    bool m_bBuiltInOp;            ///< Flag that can be used for switching built in operators on and off
	bool m_bcolllection;		  ///

    string_type m_sNameChars;      ///< Charset for names
    string_type m_sOprtChars;      ///< Charset for postfix/ binary operator tokens
    string_type m_sInfixOprtChars; ///< Charset for infix operator tokens
  public:
	mutable string_type m_StrCollection;
	mutable ParserByteCode m_vByteCodeCollection;
	mutable const ParserByteCode::map_type *m_pCmdCodeCollection; 
	mutable classbasemap_type m_ClassDefMap;
	mutable string_type m_StringFormula;
//////////////////////////////////////////////////////////////////////////
//
    virtualclass *m_PVirClassGroup;

	ParserByteCode::storage_type GetCollectionStorage()
	{
		return m_vByteCodeCollection.GetStorageBaseData();
	}

	value_type RunCollectionCmdCode()const;

	void SetColllection(bool Collect)
	{
		m_bcolllection=Collect;
	}

	void ClearCollection()
	{
		m_vByteCodeCollection.clear();
		m_StrCollection=string_type("");
	}

	 ParserByteCode::storage_type GetStorageBase();

	 bool m_bstopcompile;
	 bool m_bstoprun;
	 void RunStop()
	 {
		m_bstoprun=true;
	 }
	 void CompileStop()
	 {
		 m_bstopcompile=true;
	 }
	 void RunOk()
	 {
		m_bstoprun=false;
	 }
	 void CompileOk()
	 {
		 m_bstopcompile=false;
	 }


	typedef ParserStack<token_type> TokeStack;

	mutable TokeStack m_OptStack;

	//mutable TokeStack m_optstack1;
	//mutable TokeStack m_optstack2;
	//mutable TokeStack m_optstack3;

	mutable map<string_type,TokeStack> m_mapoptstack;

	void RunOptString(const char *optstackname);
	void SetOptStack(const string_type & optstackname);
	void ClearOptStack();
	//mutable TokeVector m_optvector;
    //mutable TokeVector m_optvector1;
    //mutable TokeVector m_optvector2;
    //mutable TokeVector m_optvector3;

	
	bool m_boptcollect;
	void SetOptCollect(bool bcollec)
	{
		m_boptcollect=bcollec;
	}
//////////////////////////////////////////////////////////////////////////
	

	value_type RunCollectionOpt() const;//运行收集字串剖析后生成的操作符
	value_type RunOptStack(TokeStack & optstack) const;
	//double m_dtimelimit;
	//mutable double m_druntime;
	//void SetOptLimitTime(double dlimittime){m_dtimelimit = dlimittime;}
	//double GetOptRunTime(){return m_druntime;}
	value_type RunOptVect_SetTime(TokeStack & optstack) const;
	value_type RunClassFuncCode(int * vFuncByteCode,int iFuncBytesize,stringbuf_type& strtab) const;

//////////////////////////////////////////////////////////////////////////
	void CopyRUNOpt(int ioptnum); 
	void RunOpt(int ioptnum);
	//void RunOptTimeLimit(int ioptnum);
//////////////////////////////////////////////////////////////////////////
	void CompileClassDeclara(const string_type &a_strdeclarastr,CreateClass *paclass);
	void CompileFuncAndRunString(const string_type &a_strfucstr,const string_type &a_strclass,const string_type &a_strobj);
	
	CreateClass *m_pcreateclass;

    bool m_bprecompile;
	
	void* GetClassObj(const string_type & strclass,const string_type & strobj);
    void* GetClassObj(const string_type &  strclass,int iobjnum);
	int GetClassObjSum(const string_type &  strclass);

    bool m_bcmd;

};

} // namespace mu

#endif

