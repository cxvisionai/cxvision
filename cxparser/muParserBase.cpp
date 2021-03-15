/*
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
//#include "StdAfx.h"
#include "muParser.h"

#include <cassert>
#include <cmath>
#include <memory>
#include <vector>
#include <stack>
#include <deque>
#include <list>
#include <sstream>

using namespace std;

//////////////////////////////////////////////////////////////////////////
//
/*
class DLLDIR CToolTime
{
public:

	CToolTime(void);

public:

	~CToolTime(void);
	void SetTimeRecord();
	double GetTimeRecord();

public:

	//时间记录和频率使用变量
	LARGE_INTEGER  m_litmp; 
	LONGLONG m_QPart1,m_QPart2;
	double m_dfMinus;
	double m_dfFreq;
	double m_dfTim;	
};

CToolTime::CToolTime(void)
{
	m_QPart1=0;
	m_QPart2=0;
	//获得主板频率
	QueryPerformanceFrequency(&m_litmp);
	// 获得计数器的时钟频率
	m_dfFreq = (double)m_litmp.QuadPart; 
}

CToolTime::~CToolTime()
{
}

//////////////////////////////////////////////////////////////////////////
// function:
//  开始计时间
//////////////////////////////////////////////////////////////////////////
void CToolTime::SetTimeRecord()
{
	//开始计时
	QueryPerformanceCounter(&m_litmp);
	m_QPart1=m_litmp.QuadPart;
}

double CToolTime::GetTimeRecord()
{
	QueryPerformanceCounter(&m_litmp);
	m_QPart2=m_litmp.QuadPart;
	m_dfMinus=(double)m_QPart2-m_QPart1;
	m_dfTim=m_dfMinus/m_dfFreq;
	return m_dfTim;
}
*/


//#define nodefCmdCode
namespace mu
{

//------------------------------------------------------------------------------
/** \brief Identifiers for built in binary operators. 

    When defining custom binary operators with #AddOprt(...) make sure not to choose 
    names conflicting with these definitions. 
*/
//const char_type* ParserBase::c_DefaultOprt[] = 
//{ 
//  "<=", ">=", "!=", "==", "<", ">", "+", "-", "*", "/", 
//  "^", "and", "or", "xor", "=", "(", ")", ",",";","{","}", ".","->",0 //cxyadd ";","{","}", ".","->"
//};

//------------------------------------------------------------------------------
/** \brief Constructor.
  \param a_szFormula the formula to interpret.
  \throw ParserException if a_szFormula is null.
*/
ParserBase::ParserBase()
  :m_pParseFormula(&ParserBase::ParseString)
  ,m_pCmdCode(0)
  ,m_vByteCode()
  ,m_vStringBuf()
  ,m_pTokenReader()
  ,m_FunDef()
  ,m_PostOprtDef()
  ,m_InfixOprtDef()
  ,m_OprtDef()
  ,m_ConstDef()
  ,m_StrVarDef()
  ,m_VarDef()
  ,m_bOptimize(true)
  ,m_bUseByteCode(true)
  ,m_bBuiltInOp(true)
  ,m_sNameChars()
  ,m_sOprtChars()
  ,m_sInfixOprtChars()
  ,m_bcolllection(false)
  ,m_pClassReader()
  ,m_pClassFunReader()
{

  InitTokenReader();
  InitClassReader();
  InitClassFunReader();
  //编译及其运行状态控制符
  m_bstopcompile=false;
  m_bstoprun=false;
  m_boptcollect=false;
  m_bprecompile = false;
  m_pcreateclass = NULL;
  m_bcmd = true;
  //m_dtimelimit = -1;
}

//---------------------------------------------------------------------------
/** \brief Copy constructor. 

 Implemented by calling Assign(a_Parser)
*/
ParserBase::ParserBase(const ParserBase &a_Parser)
  :m_pParseFormula(&ParserBase::ParseString)
  ,m_pCmdCode(0)
  ,m_vByteCode()
  ,m_vStringBuf()
  ,m_pTokenReader()
  ,m_FunDef()
  ,m_PostOprtDef()
  ,m_InfixOprtDef()
  ,m_OprtDef()
  ,m_ConstDef()
  ,m_StrVarDef()
  ,m_VarDef()
  ,m_bOptimize(true)
  ,m_bUseByteCode(true)
  ,m_bBuiltInOp(true)
  ,m_bcolllection(false)
  ,m_pClassReader()
  ,m_pClassFunReader()
{
  m_pTokenReader.reset(new token_reader_type(this));
  m_pClassReader.reset(new ParserClassReader(this));
  m_pClassFunReader.reset(new ParserClassFunctionReader(this));
  Assign(a_Parser);
}

//---------------------------------------------------------------------------
/** \brief Assignment operator. 

   Implemented by calling Assign(a_Parser). Self assignment is suppressed.
   \param a_Parser Object to copy to this.
   \return *this
   \throw nothrow
*/
ParserBase& ParserBase::operator=(const ParserBase &a_Parser)
{
  Assign(a_Parser);
  return *this;
}

//---------------------------------------------------------------------------
/** \brief Copy state of a parser object to this. 

  Clears Variables and Functions of this parser.
  Copies the states of all internal variables.
  Resets parse function to string parse mode.

  \param a_Parser the source object.
*/
void ParserBase::Assign(const ParserBase &a_Parser)
{
  if (&a_Parser==this)
    return;

  // Don't copy bytecode instead cause the parser to create new bytecode
  // by resetting the parse function.
  ReInit();

  m_ConstDef = a_Parser.m_ConstDef;         // Copy user define constants
  m_VarDef = a_Parser.m_VarDef;             // Copy user defined variables
  m_bOptimize  = a_Parser.m_bOptimize;
  m_bUseByteCode = a_Parser.m_bUseByteCode;
  m_bBuiltInOp = a_Parser.m_bBuiltInOp;
  m_vStringBuf = a_Parser.m_vStringBuf;
  m_pTokenReader.reset(a_Parser.m_pTokenReader->Clone(this));
  m_pClassReader.reset(a_Parser.m_pClassReader->Clone(this));
  m_pClassFunReader.reset(a_Parser.m_pClassFunReader->Clone(this));
  m_StrVarDef = a_Parser.m_StrVarDef;
  m_vStringVarBuf = a_Parser.m_vStringVarBuf;

  // Copy function and operator callbacks
  m_FunDef = a_Parser.m_FunDef;             // Copy function definitions
  m_PostOprtDef = a_Parser.m_PostOprtDef;   // post value unary operators
  m_InfixOprtDef = a_Parser.m_InfixOprtDef; // unary operators for infix notation

  m_sNameChars = a_Parser.m_sNameChars;
  m_sOprtChars = a_Parser.m_sOprtChars;
  m_sInfixOprtChars = a_Parser.m_sInfixOprtChars;
  
}

//---------------------------------------------------------------------------
/** \brief Initialize the token reader. 

   Create new token reader object and submit pointers to function, operator,
   constant and variable definitions.

   \post m_pTokenReader.get()!=0
   \throw nothrow
*/
void ParserBase::InitTokenReader()
{
   m_pTokenReader.reset(new token_reader_type(this));
}
void ParserBase::InitClassReader()
{
	m_pClassReader.reset(new ParserClassReader(this));
}
void ParserBase::InitClassFunReader()
{
    m_pClassFunReader.reset(new ParserClassFunctionReader(this));
}

//---------------------------------------------------------------------------
/** \brief Reset parser to string parsing mode and clear internal buffers.

    Clear bytecode, reset the token reader.
    \throw nothrow
*/
void ParserBase::ReInit() const
{
  //cxyexplain: get function point
  m_pParseFormula = &ParserBase::ParseString;
  m_vStringBuf.clear();
  m_vByteCode.clear();
  m_pTokenReader->ReInit();
}
//cxyaddBegin
void ParserBase::EndExpress()const
{
  m_vStringBuf.clear();
  m_vByteCode.clear();
  m_pTokenReader->EndExpress();
}
//---------------------------------------------------------------------------
void ParserBase::AddValIdent(identfun_type a_pCallback)
{
  m_pTokenReader->AddValIdent(a_pCallback);
  //cxyadd 
  m_pClassFunReader->AddValIdent(a_pCallback);
}

//---------------------------------------------------------------------------
void ParserBase::SetVarFactory(facfun_type a_pFactory,void * a_pvoid)
{
	m_pTokenReader->SetVarCreator(a_pFactory); 
	m_pTokenReader->SetClassUing(a_pvoid);//set token create var factory from the class member function 
}
//---------------------------------------------------------------------------
/** \brief Add a function or operator callback to the parser.
*/
void ParserBase::AddCallback( const string_type &a_strName,
                              const ParserCallback &a_Callback, 
                              funmap_type &a_Storage,
                              const char_type *a_szCharSet )
{
  if (a_Callback.GetAddr()==0)
      Error(ecINVALID_FUN_PTR);

  const funmap_type *pFunMap = &a_Storage;

  // Check for conflicting operator or function names
  if ( pFunMap!=&m_FunDef && m_FunDef.find(a_strName)!=m_FunDef.end() )
    Error(ecNAME_CONFLICT);

  if ( pFunMap!=&m_PostOprtDef && m_PostOprtDef.find(a_strName)!=m_PostOprtDef.end() )
    Error(ecNAME_CONFLICT);

  if ( pFunMap!=&m_InfixOprtDef && pFunMap!=&m_OprtDef && m_InfixOprtDef.find(a_strName)!=m_InfixOprtDef.end() )
    Error(ecNAME_CONFLICT);

  if ( pFunMap!=&m_InfixOprtDef && pFunMap!=&m_OprtDef && m_OprtDef.find(a_strName)!=m_OprtDef.end() )
    Error(ecNAME_CONFLICT);

  CheckName(a_strName, a_szCharSet);
  a_Storage[a_strName] = a_Callback;
  ReInit();
}

//---------------------------------------------------------------------------
/** \brief Check if a name contains invalid characters. 

    \throw ParserException if the name contains invalid charakters.
*/
void ParserBase::CheckName(const string_type &a_sName,
                           const string_type &a_szCharSet) const
{
  if ( !a_sName.length() ||
       (a_sName.find_first_not_of(a_szCharSet)!=string_type::npos) ||
       (a_sName[0]>='0' && a_sName[0]<='9'))
  {
    Error(ecINVALID_NAME);
  }
}

//---------------------------------------------------------------------------
/** \brief Set the formula. 
    Triggers first time calculation thus the creation of the bytecode and
    scanning of used variables.

    \param a_strFormula Formula as string_type
    \throw ParserException in case of syntax errors.
*/
void ParserBase::SetExpr(const string_type &a_sExpr)
{
  // <ibg> 20060222: Bugfix for Borland-Kylix:
  // adding a space to the expression will keep Borlands KYLIX from going wild
  // when calling tellg on a stringstream created from the expression after 
  // reading a value at the end of an expression. (mu::Parser::IsVal function)
  // (tellg returns -1 otherwise causing the parser to ignore the value)
  string_type sBuf(a_sExpr + _T(" ") );
  m_pTokenReader->SetFormula(sBuf);
//m_sFormalu=sBuf;
  ReInit();
}

//---------------------------------------------------------------------------
/** \brief Add a user defined operator. 
    \post Will reset the Parser to string parsing mode.
    \param a_sName [in] operator Identifier 
    \param a_pFun [in] Operator callback function
    \param a_iPrec [in] Operator Precedence (default=prSIGN)
    \param a_bAllowOpt [in] True if operator is volatile (default=false)

    \sa EPrec
*/
void ParserBase::DefineInfixOprt(const string_type &a_sName, 
                                 fun_type1 a_pFun, 
                                 int a_iPrec, 
                                 bool a_bAllowOpt)
{
  AddCallback( a_sName, 
               ParserCallback(a_pFun, a_bAllowOpt, a_iPrec, cmOPRT_INFIX), 
               m_InfixOprtDef, 
               ValidOprtChars() );
}

//---------------------------------------------------------------------------
/** \brief Add a user defined operator. 
    \post Will reset the Parser to string parsing mode.
    \param a_sName [in] operator Identifier 
    \param a_pFun [in] Operator callback function
    \param a_iPrec [in] Operator Precedence (default=prSIGN)
    \param a_bAllowOpt [in] True if operator is volatile (default=false)

    \sa 
*/
void ParserBase::DefineGetAdress(const string_type &a_sName, 
								 fun_lptype a_pFun, 
								 int a_iPrec, 
								 bool a_bAllowOpt)
{
	AddCallback( a_sName, 
		ParserCallback(a_pFun, a_bAllowOpt, a_iPrec, cmOPRT_INFIX), 
		m_InfixOprtDef, 
		ValidOprtChars() );
}

//---------------------------------------------------------------------------
/** \brief Add a user defined operator. 
    \post Will reset the Parser to string parsing mode.
*/
void ParserBase::DefinePostfixOprt(const string_type &a_sName, 
                                   fun_type1 a_pFun,
                                   bool a_bAllowOpt)
{
  AddCallback( a_sName, 
               ParserCallback(a_pFun, a_bAllowOpt, prPOSTFIX, cmOPRT_POSTFIX),
               m_PostOprtDef, 
               ValidOprtChars() );
}

//---------------------------------------------------------------------------
/** \brief Add a user defined operator. 
    \post Will reset the Parser to string parsing mode.
    \param a_sName [in] operator Identifier 
    \param a_pFun [in] Operator callback function
    \param a_iPrec [in] Operator Precedence (default=prSIGN)
    \param a_bAllowOpt [in] True if operator is volatile (default=false)

    \sa EPrec
*/

//---------------------------------------------------------------------------
void ParserBase::DefineOprt( const string_type &a_sName, 
                             fun_type2 a_pFun, 
                             unsigned a_iPrec, 
                             bool a_bAllowOpt )
{
  // Check for conflicts with built in operator names
  for (int i=0; m_bBuiltInOp && i<cmCOMMA; ++i)
    if (a_sName == string_type(c_DefaultOprt[i]))
      Error(ecBUILTIN_OVERLOAD);

  AddCallback( a_sName, 
               ParserCallback(a_pFun, a_bAllowOpt, a_iPrec, cmOPRT_BIN), 
               m_OprtDef, 
               ValidOprtChars() );
}

//---------------------------------------------------------------------------no using
void ParserBase::DefineStrConst(const string_type &a_strName, const string_type &a_strVal)
{
  // Test if a constant with that names already exists
  if (m_StrVarDef.find(a_strName)!=m_StrVarDef.end())
    Error(ecNAME_CONFLICT);

  CheckName(a_strName, ValidNameChars());
  
  // Store variable string in internal buffer
  m_vStringVarBuf.push_back(a_strVal);

  // bind buffer index to variable name
  m_StrVarDef[a_strName] = m_vStringBuf.size();

  ReInit();
}

//---------------------------------------------------------------------------
/** \brief Add a user defined variable. 
    \post Will reset the Parser to string parsing mode.
    \pre [assert] a_fVar!=0
    \throw ParserException in case the name contains invalid signs.
*/
void ParserBase::DefineVar(const string_type &a_sName, value_type *a_pVar)
{
  if (a_pVar==0)
    Error(ecINVALID_VAR_PTR);

  // Test if a constant with that names already exists
  if (m_ConstDef.find(a_sName)!=m_ConstDef.end())
    Error(ecNAME_CONFLICT);

  if (m_FunDef.find(a_sName)!=m_FunDef.end())
    Error(ecNAME_CONFLICT);

  CheckName(a_sName, ValidNameChars());
  m_VarDef[a_sName] = a_pVar;
  ReInit();
}
//////////////////////////////////////////////////////////////////////////
//cxy add
//template<class ACLASS>
//void ParserBase::DefineClass(const string_type &a_sName,ACLASS pclass)
//{
//	// Test if a constant with that names already exists
//	if (m_ClassDefMap.find(a_sName)!=m_ClassDefMap.end())
//		Error(ecNAME_CONFLICT);
//
//	CheckName(a_sName, ValidNameChars());
//
//	ParserClass<ACLASS,value_type> aclass;
//
//	//m_ClassDefMap[a_sName] = aclass;
//
//	ReInit();
//}
//cxyaddend
//////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
/** \brief Add a user defined constant. 
    \post Will reset the Parser to string parsing mode.
    \throw ParserException in case the name contains invalid signs.
*/
void ParserBase::DefineConst(const string_type &a_sName, value_type a_fVal)
{
  CheckName(a_sName, ValidNameChars());
  m_ConstDef[a_sName] = a_fVal;
  ReInit();
}

//---------------------------------------------------------------------------
/** \brief Get operator priority.

 \throw ParserException if a_Oprt is no operator code
*/
int ParserBase::GetOprtPri(const token_type &a_Tok) const
{
  switch (a_Tok.GetCode())
  {
    // built in operators
    case cmEND:        return -5;
	case cmCOMMA:      return -4;
    case cmBO :	
    case cmBC :        return -2;
    case cmASSIGN:     return -1;               
    case cmAND:
    case cmXOR:
    case cmOR:         return  prLOGIC;  
    case cmLT :
    case cmGT :
    case cmLE :
    case cmGE :
    case cmNEQ:
    case cmEQ :        return  prCMP; 
    case cmADD:
    case cmSUB:        return  prADD_SUB;
    case cmMUL:
    case cmDIV:        return  prMUL_DIV;
    case cmPOW:        return  prPOW;

    // user defined binary operators
    case cmOPRT_INFIX: 
    case cmOPRT_BIN:   return a_Tok.GetPri();
    default:  Error(ecINTERNAL_ERROR, 5);
              return 999;
  }  
}

//---------------------------------------------------------------------------
/** \brief Return a map containing the used variables only. */
const varmap_type& ParserBase::GetUsedVar() const
{
  try
  {
    m_pTokenReader->IgnoreUndefVar(true);
	//cxy explain
    ParseString(); // impolitely create or update the map with the
                   // used variables stored in the token reader if not already done
    m_pTokenReader->IgnoreUndefVar(false);
  }
  catch(exception_type &e)
  {
    m_pTokenReader->IgnoreUndefVar(false);
    throw e;
  }
  
  // Make sure to stay in string parse mode, dont call ReInit()
  // because it deletes the array with the used variables
  m_pParseFormula = &ParserBase::ParseString;
  
  return m_pTokenReader->GetUsedVar();
}

//---------------------------------------------------------------------------
/** \brief Return a map containing the used variables only. */
const varmap_type& ParserBase::GetVar() const
{
  return m_VarDef;
}
//cxyadd----------------------------------------------------------------------

/** \brief Return a map containing the used class only. */
const classbasemap_type& ParserBase::GetClassMap() const
{
	return m_ClassDefMap;
}
const string_type& ParserBase::GetFormula() const
{
	return m_StringFormula;
}
//cxyaddend//////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
/** \brief Return a map containing all parser constants. */
const valmap_type& ParserBase::GetConst() const
{
   return m_ConstDef;
}

//---------------------------------------------------------------------------
/** \brief Return prototypes of all parser functions.
    
    The return type is a map of the public type #funmap_type containing the prototype
    definitions for all numerical parser functions. String functions are not part of 
    this map. The Prototype definition is encapsulated in objects of the class FunProt
    one per parser function each associated with function names via a map construct.
    \return #m_FunDef
    \sa FunProt
    \throw nothrow
*/
const funmap_type& ParserBase::GetFunDef() const
{
  return m_FunDef;
}

//---------------------------------------------------------------------------
/** \brief Retrieve the formula. */
const string_type& ParserBase::GetExpr() const
{
  return m_pTokenReader->GetFormula();
}

//---------------------------------------------------------------------------
ParserBase::token_type ParserBase::ApplyNumFunc( const token_type &a_FunTok,
                                                 const std::vector<token_type> &a_vArg) const
{
  token_type  valTok;
  int  iArgCount = (unsigned)a_vArg.size();
  void  *pFunc = a_FunTok.GetFuncAddr();
  assert(pFunc);
 
  // Collect the function arguments from the value stack
  switch(a_FunTok.GetArgCount())
  {
	case -1:
			  // Function with variable argument count
			  // copy arguments into a vector<value_type>
			{          
		    if (iArgCount==0)
				Error(ecTOO_FEW_PARAMS, m_pTokenReader->GetPos(), a_FunTok.GetAsString());

				/** \todo remove the unnecessary argument vector by changing order in stArg. */
				std::vector<value_type> vArg;
				for (int i=0; i<iArgCount; ++i)
				  vArg.push_back(a_vArg[i].GetVal());
				valTok.SetVal( ((multfun_type)a_FunTok.GetFuncAddr())(&vArg[0], (int)vArg.size()) );  
			}
			break;
	case 1: 
			valTok.SetVal( ((fun_type1)a_FunTok.GetFuncAddr())(a_vArg[0].GetVal()));
			break;
	case 2: 
			valTok.SetVal( ((fun_type2)a_FunTok.GetFuncAddr())(a_vArg[1].GetVal(),
																 a_vArg[0].GetVal()) );
			break;
	case 3: 
			valTok.SetVal( ((fun_type3)a_FunTok.GetFuncAddr())(a_vArg[2].GetVal(), 
																   a_vArg[1].GetVal(), 
																	a_vArg[0].GetVal()) );
			break;
	case 4: 
			valTok.SetVal( ((fun_type4)a_FunTok.GetFuncAddr())(a_vArg[3].GetVal(),
															   a_vArg[2].GetVal(),
		   	  												   a_vArg[1].GetVal(),
															   a_vArg[0].GetVal()));
			break;
	case 5: 
			valTok.SetVal( ((fun_type5)a_FunTok.GetFuncAddr())(a_vArg[4].GetVal(), 
															   a_vArg[3].GetVal(), 
															   a_vArg[2].GetVal(), 
		   	  												   a_vArg[1].GetVal(),
															   a_vArg[0].GetVal()));
			break;
	default:
			Error(ecINTERNAL_ERROR, 6);
  }
 
  // Find out if the result will depend on a variable
  /** \todo remove this loop, put content in the loop that takes the argument values.
    
      (Attention: SetVal will reset Flags.)
  */
  bool bVolatile = a_FunTok.IsFlagSet(token_type::flVOLATILE);
  for (int i=0; (bVolatile==false) && (i<iArgCount); ++i)
	  bVolatile |= a_vArg[i].IsFlagSet(token_type::flVOLATILE);

  if (bVolatile)
    valTok.AddFlags(token_type::flVOLATILE);

#if defined(_MSC_VER)
  #pragma warning( disable : 4311 ) 
#endif

#ifndef nodefCmdCode
if(true == m_bcmd)
{
  // Formula optimization
  if ( m_bOptimize && 
	  !valTok.IsFlagSet(token_type::flVOLATILE) &&
	  !a_FunTok.IsFlagSet(token_type::flVOLATILE) ) 
  {//参数是常量 优化运行表操作设置
	  m_vByteCode.RemoveValEntries(iArgCount);
	  m_vByteCode.AddVal( valTok.GetVal() );
  }
  else 
  {//参数是变量 优化运行表操作
	  // operation dosnt depends on a variable or the function is flagged unoptimizeable
	  // we cant optimize here...
	  m_vByteCode.AddFun(pFunc, (a_FunTok.GetArgCount()==-1) ? -iArgCount : iArgCount);
  }
}
#endif
  return valTok;

#if defined(_MSC_VER)
  #pragma warning( default : 4311 )
#endif
}

//---------------------------------------------------------------------------
/** \brief Execute a function that takes a single string argument.
      
    \param a_FunTok Function token.
    \throw exception_type If the function token is not a string function
*/
ParserBase::token_type ParserBase::ApplyStrFunc(const token_type &a_FunTok,
                                                token_type &a_Arg) const
{
  if (a_Arg.GetCode()!=cmSTRING)
    Error(ecSTRING_EXPECTED, m_pTokenReader->GetPos(), a_FunTok.GetAsString());

  strfun_type1 pFunc = (strfun_type1)a_FunTok.GetFuncAddr();
  assert(pFunc);
  
  value_type fResult = pFunc( a_Arg.GetAsString().c_str() );
#ifndef nodefCmdCode
if(true == m_bcmd)
{
  // Formula optimization
  if ( m_bOptimize && 
       !a_FunTok.IsFlagSet(token_type::flVOLATILE) )
	{
	    m_vByteCode.AddVal( fResult );
	}
	else 
	{ 
		// operation dosnt depends on a variable or the function is flagged unoptimizeable
		// we cant optimize here...
		m_vByteCode.AddStrFun((void*)pFunc, a_FunTok.GetArgCount(), a_Arg.GetIdx());
	}
}
#endif
	a_Arg.SetVal(fResult);
	return a_Arg;
}

//---------------------------------------------------------------------------
/** \brief Apply a function token. 

    \param iArgCount Number of Arguments actually gathered used only for multiarg functions.
    \post Function have been taken from the stack, the result has been pushed 
    \post The function token is removed from the stack
    \throw exception_type if Argument count does not mach function requirements.
*/
void ParserBase::ApplyFunc( ParserStack<token_type> &a_stOpt,
                            ParserStack<token_type> &a_stVal, 
							ParserStack<token_type> &a_classobj,
                            int a_iArgCount) const
{
  assert(m_pTokenReader.get());
 
  // Operator stack empty or does not contain tokens with callback functions
 //cxyadd if (a_stOpt.empty())// || a_stOpt.top().GetFuncAddr()==0 )
 bool bVolatile;
  if (a_stOpt.empty() )  
	  return;
  if(a_stOpt.top().GetFuncAddr()==0
	  &&a_stOpt.top().GetCode()!=cmClassFuc)
	  return;
  token_type funTok = a_stOpt.pop();

  if(cmClassFuc!=funTok.GetCode())
	{	

		// Binary operators must rely on their internal operator number
	  // since counting of operators relies on commas for function arguments
	  // binary operators do not have commas in their expression
		int iArgCount = ( funTok.GetCode()==cmOPRT_BIN ) ? funTok.GetArgCount() : a_iArgCount;

		if (funTok.GetArgCount()>0 && iArgCount>funTok.GetArgCount()) 
			Error(ecTOO_MANY_PARAMS, m_pTokenReader->GetPos()-1, funTok.GetAsString());

		if ( funTok.GetCode()!=cmOPRT_BIN && iArgCount<funTok.GetArgCount() )
			Error(ecTOO_FEW_PARAMS, m_pTokenReader->GetPos()-1, funTok.GetAsString());

		// Collect the function arguments from the value stack and store them
		// in a vector
		std::vector<token_type> stArg;  
		for (int i=0; i<iArgCount; ++i)
		{
			stArg.push_back( a_stVal.pop() );
			if ( stArg.back().GetType()==tpSTR && funTok.GetType()!=tpSTR )
				Error(ecVAL_EXPECTED, m_pTokenReader->GetPos(), funTok.GetAsString());
		}
		// String functions accept only one parameter
		switch(funTok.GetType())
		{
		case tpSTR:
			a_stVal.push(ApplyStrFunc(funTok,stArg.back()));
			break;
		case tpDBL:
			a_stVal.push(ApplyNumFunc(funTok, stArg));
			break;
		case tpVOID:
			break;
		}
	}
	else
	{
		std::vector<token_type> stArg;  

		 token_type  valTok,classobjTok; 
		 value_type avalue;
		 int  iArgCount =  funTok.GetClassFucArgCount(); 
		 int iClassObjCount = (unsigned)a_classobj.size();
		 classbase  *pclass =funTok.m_pClass;
		 void *pobj = funTok.m_pClassObj;
		 assert(pclass);
		 //std::vector<value_type> vArg;
		 paramvect vArg;
		 voidparamvect voidpArg;
		 charpvect charpArg;
		 // Collect the function arguments from the value stack
		 switch(funTok.GetClassFucArgType())
		{
		 case Param_none:
		 case Param_double_1:
		 case Param_double_2:
		 case Param_double_3:
		 case Param_double_4:
		 case Param_int_1:
		 case Param_int_2:
		 case Param_int_3:
		 case Param_int_4:
		 case Param_int_5:
		 case Param_int_6:
		 case Param_int_7:
			 for (int i=0; i<iArgCount; ++i)
			 {
				 valTok=a_stVal.pop();
				 //stArg.push_back( valTok );
				 //if ( stArg.back().GetType()==tpSTR && funTok.GetType()!=tpSTR )
					// Error(ecVAL_EXPECTED, m_pTokenReader->GetPos(), funTok.GetAsString());
				 avalue=valTok.GetVal();
				 vArg.push_back( avalue);
			 }
			 if(false==m_bprecompile)//not to pre compile so need to run code
			 pclass->ApplyClassFunc(pobj,funTok.GetAsString(),vArg);

			//////////////////////////////////////////////////////////////////////////
			//byte run oper
			 {//参数是变量 优化运行表操作
				 // operation dosnt depends on a variable or the function is flagged unoptimizeable
				 // we cant optimize here...
#ifndef nodefCmdCode
			if(true == m_bcmd)
				 m_vByteCode.AddClassMemberFunNum(pclass,
												pobj,
												pclass->GetClassFuncLP(funTok.GetAsString()),
											iArgCount);
#endif
			 }

			///////////////////////////////////////////////////////////////ere///////////
			 break;
		 case Param_charp_1:
			for(int i=0;i<iArgCount;++i)
			{
				valTok=a_stVal.pop();
				stArg.push_back( valTok );
				if ( stArg.back().GetType()!=tpSTR )
					Error(ecVAL_EXPECTED, m_pTokenReader->GetPos(), funTok.GetAsString());
				charpArg.push_back(valTok.GetAsString());
			}
			 if(false==m_bprecompile)//not to pre compile so need to run code
				 pclass->ApplyClassFunc(pobj,funTok.GetAsString(),charpArg);
			
			 //////////////////////////////////////////////////////////////////////////
			 //byte run oper
			 {//参数是变量 优化运行表操作
				 // operation dosnt depends on a variable or the function is flagged unoptimizeable
				 // we cant optimize here...
#ifndef nodefCmdCode
if(true == m_bcmd)				
				 m_vByteCode.AddClassMemberFunStr(pclass,
					 pobj,
					 pclass->GetClassFuncLP(funTok.GetAsString()),
					 iArgCount,stArg.back().GetIdx());
#endif
			 }
			 break;
		

		 case Param_voidp_1:
				{			
					classobjTok = a_classobj.pop();
					void *ptheobj = classobjTok.GetClassObj();
						voidpArg.push_back(ptheobj);
					if(false==m_bprecompile)//not to pre compile so need to run code
						pclass->ApplyClassFunc(pobj,funTok.GetAsString(),voidpArg);

					//参数是变量 优化运行表操作
					// operation dosnt depends on a variable or the function is flagged unoptimizeable
					// we cant optimize here...
#ifndef nodefCmdCode
if(true == m_bcmd)
{					classbase *pvoidpclass = classobjTok.GetClass();
					int istacknum = pvoidpclass->findstacknum(ptheobj);
					m_vByteCode.AddClassMemberFunVoidp(pclass,
						pobj,
						pclass->GetClassFuncLP(funTok.GetAsString()),
						istacknum,
						ptheobj);
}
#endif
				}
				break;
		 case Param_voidp_2:
		 case Param_voidp_3:
		 case Param_voidp_4:
		 case Param_voidp_5:
		 

			 for (int i=0; i<iArgCount; ++i)
			 {
				 classobjTok = a_classobj.pop();
				 void *ptheobj = classobjTok.GetClassObj();
				 voidpArg.push_back(ptheobj);
			 }
			 if(false==m_bprecompile)//not to pre compile so need to run code
				 pclass->ApplyClassFunc(pobj,funTok.GetAsString(),voidpArg);
				 
 
			 break;
		 case Param_voidp_1_Return_double:
				 {
					 voidpArg.push_back((a_classobj.pop()).GetClassObj());
				 }
				 if(false==m_bprecompile)//not to pre compile so need to run code
					 valTok.SetVal(pclass->ApplyClassFunc(pobj,funTok.GetAsString(),voidpArg));
				 a_stVal.push(valTok);
			 break;
		 case Param_0_Return_int:
		 case Param_0_Return_double:
		 case Param_int_1_Return_int:
		 case Param_int_2_Return_int:
		 case Param_int_1_Return_double:
		 case Param_int_2_Return_double:
		 case Param_int_3_Return_double:

			 for (int i=0; i<iArgCount; ++i)
				{
					valTok=a_stVal.pop();
					avalue=valTok.GetVal();
					vArg.push_back( avalue);
				}
			 if(false==m_bprecompile)//not to pre compile so need to run code
			 valTok.SetVal(pclass->ApplyClassFunc(pobj,funTok.GetAsString(),vArg));
			 a_stVal.push(valTok);
			//cxyadd need add
			 break;
		 case Param_0_Return_charp:
			 {
			// classobjTok = a_classobj.pop();
			 //void *ptheobj = classobjTok.GetClassObj();
			 //voidpArg.push_back(ptheobj);
			// if(false==m_bprecompile)//not to pre compile so need to run code
			//	valTok.SetVal(pclass->ApplyClassFuncString(pobj,funTok.GetAsString()) );
			 string_type astring(pclass->ApplyClassFuncString(pobj,funTok.GetAsString()));
			valTok.SetString(astring,astring.size());
			a_stVal.push(valTok);
			 }

			break;
		 default:
			 Error(ecINTERNAL_ERROR, 6);
			}

		 //////////////////////////////////////////////////////////////////////////
		 //no to confirm return type
		 //////////////////////////////////////////////////////////////////////////

		 bool bVolatile = funTok.IsFlagSet(token_type::flVOLATILE);
		 //for (int i=0; (bVolatile==false) && (i<iArgCount); ++i)
		 //	bVolatile |= a_stVal[i].IsFlagSet(token_type::flVOLATILE);

		 if (bVolatile)
			 valTok.AddFlags(token_type::flVOLATILE);
	}

}

////---------------------------------------------------------------------------
///** \brief Stack a function token. 
//
//    \param iArgCount Number of Arguments actually gathered used only for multiarg functions.
//    \post Function have been taken from the stack, the result has been pushed 
//    \post The function token is removed from the stack
//    \throw exception_type if Argument count does not mach function requirements.
//*/
//void ParserBase::StackFunc(ParserRunToken &a_runtaken, 
//						   ParserStack<token_type> &a_stOpt,
//                            ParserStack<token_type> &a_stVal, 
//                            int a_iArgCount) const
//{ 
//  assert(m_pTokenReader.get());
//
//  // Operator stack empty or does not contain tokens with callback functions
//  if (a_stOpt.empty() || a_stOpt.top().GetFuncAddr()==0 )
//    return;
//
//  token_type funTok = a_stOpt.pop();
//  assert(funTok.GetFuncAddr());
//
//  // Binary operators must rely on their internal operator number
//  // since counting of operators relies on commas for function arguments
//  // binary operators do not have commas in their expression
//  int iArgCount = ( funTok.GetCode()==cmOPRT_BIN ) ? funTok.GetArgCount() : a_iArgCount;
//
//  if (funTok.GetArgCount()>0 && iArgCount>funTok.GetArgCount()) 
//	    Error(ecTOO_MANY_PARAMS, m_pTokenReader->GetPos()-1, funTok.GetAsString());
//
//  if ( funTok.GetCode()!=cmOPRT_BIN && iArgCount<funTok.GetArgCount() )
//	   Error(ecTOO_FEW_PARAMS, m_pTokenReader->GetPos()-1, funTok.GetAsString());
//
//  // Collect the function arguments from the value stack and store them
//  // in a vector
//  std::vector<token_type> stArg;  
//  for (int i=0; i<iArgCount; ++i)
//  {
//    stArg.push_back( a_stVal.pop() );
//    if ( stArg.back().GetType()==tpSTR && funTok.GetType()!=tpSTR )
//      Error(ecVAL_EXPECTED, m_pTokenReader->GetPos(), funTok.GetAsString());
//  }
// // String functions accept only one parameter
//  a_stVal.push((funTok.GetType()==tpSTR) ?
//               ApplyStrFunc(funTok, stArg.back()) :
//               ApplyNumFunc(funTok, stArg));
//}

//---------------------------------------------------------------------------
void ParserBase::ApplyBinOprt( ParserStack<token_type> &a_stOpt,
                               ParserStack<token_type> &a_stVal,
							   ParserStack<token_type> &a_classobj) const
{
  assert(a_stOpt.size());

  // user defined binary operator
  if (a_stOpt.top().GetCode()==cmOPRT_BIN)
  {
     ApplyFunc(a_stOpt, a_stVal, a_classobj,2);
  }
  else
  {
    // internal binary operator
    MUP_ASSERT(a_stVal.size()>=2||a_classobj.size()>=2); //类对象或 变量最少两个元素

	if(a_stVal.size()>=2)
	{
		token_type valTok1 = a_stVal.pop(),
				   valTok2 = a_stVal.pop(),
				   optTok = a_stOpt.pop(),
				   resTok; 

		if ( valTok1.GetType()!=valTok2.GetType() || 
			 (valTok1.GetType()==tpSTR && valTok2.GetType()==tpSTR) )
		  Error(ecOPRT_TYPE_CONFLICT, m_pTokenReader->GetPos(), optTok.GetAsString());

		value_type x = valTok2.GetVal(),
					 y = valTok1.GetVal();

		switch (optTok.GetCode())
		{
		  // built in binary operators
		  case cmAND: resTok.SetVal( (int)x & (int)y ); break;
		  case cmOR:  resTok.SetVal( (int)x | (int)y ); break;
		  case cmXOR: resTok.SetVal( (int)x ^ (int)y ); break;
		  case cmLT:  resTok.SetVal( x < y ); break;
		  case cmGT:  resTok.SetVal( x > y ); break;
		  case cmLE:  resTok.SetVal( x <= y ); break;
		  case cmGE:  resTok.SetVal( x >= y ); break;
		  case cmNEQ: resTok.SetVal( x != y ); break;
		  case cmEQ:  resTok.SetVal( x == y ); break;
		  case cmADD: resTok.SetVal( x + y ); break;
		  case cmSUB: resTok.SetVal( x - y ); break;
		  case cmMUL: resTok.SetVal( x * y ); break;
		  case cmDIV: resTok.SetVal( x / y ); break;
  		  case cmPOW: resTok.SetVal(pow(x, y)); break;

		  case cmASSIGN: 
					// The assignement operator needs special treatment
					// it uses a different format when stored in the bytecode!
					{
						if (valTok2.GetCode()!=cmVAR
							||valTok2.GetCode()!=cmVAR)
						Error(ecINTERNAL_ERROR, 7);
	                    
					  value_type *pVar = valTok2.GetVar();
					  resTok.SetVal( *pVar = y );
					  a_stVal.push( resTok );
#ifndef nodefCmdCode
				if(true == m_bcmd)
					  m_vByteCode.AddAssignOp(pVar);
#endif
					  return;  // we must return since the following 
							   // stuff does not apply
					}

		  default:  Error(ecINTERNAL_ERROR, 8);
		}
#ifndef nodefCmdCode
if(true == m_bcmd)
{
		// Create the bytecode entries
		if (!m_bOptimize)
		{
		  // Optimization flag is not set

		  m_vByteCode.AddOp(optTok.GetCode());

		}
		else if ( valTok1.IsFlagSet(token_type::flVOLATILE) || 
				  valTok2.IsFlagSet(token_type::flVOLATILE) )
		{
		  // Optimization flag is not set, but one of the value
		  // depends on a variable
		  m_vByteCode.AddOp(optTok.GetCode());
		  resTok.AddFlags(token_type::flVOLATILE);
		}
		else
		{
		  // operator call can be optimized; If optimization is possible 
		  // the two previous tokens must be value tokens / they will be removed
		  // and replaced with the result of the pending operation.
		  m_vByteCode.RemoveValEntries(2);
		  m_vByteCode.AddVal(resTok.GetVal());
		}
}
#endif
		a_stVal.push( resTok );
	}
	else if(a_classobj.size()>=2)// object assign
	{
		token_type objTok1 = a_classobj.pop(),
			objTok2 = a_classobj.pop(),
			optTok = a_stOpt.pop(),
			resTok; 

		if ( objTok1.GetType()!=objTok2.GetType() || 
			(objTok1.GetType()==tpSTR && objTok2.GetType()==tpSTR) )
			Error(ecOPRT_TYPE_CONFLICT, m_pTokenReader->GetPos(), optTok.GetAsString());

		void* x = objTok2.GetClassObj();
		void* y = objTok1.GetClassObj();

		switch (optTok.GetCode())
		{
			// built in binary operators
		case cmAND: //resTok.SetVal( (int)x & (int)y ); 
			break;
		case cmOR:  //resTok.SetVal( (int)x | (int)y ); 
			break;
		case cmXOR: //resTok.SetVal( (int)x ^ (int)y ); 
			break;
		case cmLT:  //resTok.SetVal( x < y ); 
			break;
		case cmGT:  //resTok.SetVal( x > y ); 
			break;
		case cmLE:  //resTok.SetVal( x <= y ); 
			break;
		case cmGE:  //resTok.SetVal( x >= y ); 
			break;
		case cmNEQ: //resTok.SetVal( x != y ); 
			break;
		case cmEQ:  //resTok.SetVal( x == y ); 
			break;
		case cmADD: //resTok.SetVal( x + y ); 
			break;
		case cmSUB: //resTok.SetVal( x - y ); 
			break;
		case cmMUL: //resTok.SetVal( x * y ); 
			break;
		case cmDIV: //resTok.SetVal( x / y ); 
			break;
		case cmPOW: //resTok.SetVal(pow(x, y)); 
			break;
		case cmASSIGN: 
			// The assignement operator needs special treatment
			// it uses a different format when stored in the bytecode!
			{
				if (objTok2.GetCode()!=cmClassObj
					||objTok2.GetCode()!=cmClassObj)
					Error(ecINTERNAL_ERROR, 7);
				if(objTok2.m_pClass != objTok2.m_pClass)
					Error(ecINTERNAL_ERROR, 7);
				void * pObj = objTok2.GetClassObj();
				pObj = y;
				resTok.SetClassVar(objTok2.m_pClass, pObj );
				a_classobj.push( resTok );
				//m_vByteCode.AddAssignOp();
				return;  // we must return since the following 
				// stuff does not apply
			}

			default:  Error(ecINTERNAL_ERROR, 8);
		}

		a_classobj.push( resTok );
	}

  }
}

//---------------------------------------------------------------------------
/** \brief Parse the command code.

  Command code contains precalculated stack positions of the values and the
  associated operators.  
  The Stack is filled beginning from index one the value at index zero is
  not used at all.

  \sa ParseString(), ParseValue()
*/
value_type ParserBase::ParseCmdCode() const
{
#if defined(_MSC_VER)
  #pragma warning( disable : 4312 )
#endif
	//
	paramvect vArg;
	voidparamvect voidpArg;
	charpvect charpArg;

  value_type Stack[99];
  ECmdCode iCode;
  bytecode_type idx(0);
  int i(0);

  __start:

  //m_pCmdCode[i]存放序号,m_pCmdCode[i+1]存放操作运算码 
  idx = m_pCmdCode[i];
  iCode = (ECmdCode)m_pCmdCode[i+1];
  i += 2;

#ifdef _DEBUG
  if (idx>=99)
    throw exception_type(ecGENERIC, "", m_pTokenReader->GetFormula(), -1);
#endif

  switch (iCode)
  {
    // built in binary operators
    case cmAND: Stack[idx]  = (int)Stack[idx] & (int)Stack[idx+1]; goto __start;
    case cmOR:  Stack[idx]  = (int)Stack[idx] | (int)Stack[idx+1]; goto __start;
    case cmXOR: Stack[idx]  = (int)Stack[idx] ^ (int)Stack[idx+1]; goto __start;
    case cmLE:  Stack[idx]  = Stack[idx] <= Stack[idx+1]; goto __start;
    case cmGE:  Stack[idx]  = Stack[idx] >= Stack[idx+1]; goto __start;
    case cmNEQ: Stack[idx]  = Stack[idx] != Stack[idx+1]; goto __start;
    case cmEQ:  Stack[idx]  = Stack[idx] == Stack[idx+1]; goto __start;
	case cmLT:  Stack[idx]  = Stack[idx] < Stack[idx+1];  goto __start;
	case cmGT:  Stack[idx]  = Stack[idx] > Stack[idx+1];  goto __start;
    case cmADD: Stack[idx] += Stack[1+idx]; goto __start;
	case cmSUB: Stack[idx] -= Stack[1+idx]; goto __start;
	case cmMUL: Stack[idx] *= Stack[1+idx]; goto __start;
	case cmDIV: Stack[idx] /= Stack[1+idx]; goto __start;
    case cmPOW: Stack[idx]  = pow(Stack[idx], Stack[1+idx]); goto __start;

    // Assignement needs special treatment
    case cmASSIGN:
	{
		// next is a pointer to the target
		value_type **pDest = (value_type**)(&m_pCmdCode[i]);
		// advance index according to pointer size
		i += m_vByteCode.GetPtrSize();
		// assign the value
		Stack[idx] = **pDest = Stack[idx+1];
	}
	goto __start;

    // user defined binary operators
    case cmOPRT_BIN:
		Stack[idx] = (**(fun_type2**)(&m_pCmdCode[i]))(Stack[idx], Stack[idx+1]);
		++i;
	goto __start;

    // variable tokens
	case cmVAR:
		Stack[idx] = **(value_type**)(&m_pCmdCode[i]);
		i += m_vByteCode.GetValSize();
	goto __start;

    // value tokens
	case cmVAL:
	Stack[idx] = *(value_type*)(&m_pCmdCode[i]);
	i += m_vByteCode.GetValSize();
	goto __start;

    // Next is treatment of string functions
    case cmFUNC_STR:
	{
		i++; // skip the unused argument count
		strfun_type1 pFun = *(strfun_type1*)(&m_pCmdCode[i]);
		i += m_vByteCode.GetPtrSize();

		int iIdxStack = (int)m_pCmdCode[i++];

		MUP_ASSERT( iIdxStack>=0 && iIdxStack<(int)m_vStringBuf.size() );
		Stack[idx] = pFun(m_vStringBuf[iIdxStack].c_str());
	}
	goto __start;

    // Next is treatment of numeric functions
    case cmFUNC:
    {
		int iArgCount = (int)m_pCmdCode[i++];
		switch(iArgCount)  // switch according to argument count
		{
			case 1: Stack[idx] = (*(fun_type1*)(&m_pCmdCode[i]))(Stack[idx]); break;
			case 2: Stack[idx] = (*(fun_type2*)(&m_pCmdCode[i]))(Stack[idx], Stack[idx+1]); break;
			case 3: Stack[idx] = (*(fun_type3*)(&m_pCmdCode[i]))(Stack[idx], Stack[idx+1], Stack[idx+2]); break;
			case 4: Stack[idx] = (*(fun_type4*)(&m_pCmdCode[i]))(Stack[idx], Stack[idx+1], Stack[idx+2], Stack[idx+3]); break;
			case 5: Stack[idx] = (*(fun_type5*)(&m_pCmdCode[i]))(Stack[idx], Stack[idx+1], Stack[idx+2], Stack[idx+3], Stack[idx+4]); break;
			default:
			if (iArgCount>0) // function with variable arguments store the number as a negative value
			Error(ecINTERNAL_ERROR, 1);

			Stack[idx] =(*(multfun_type*)(&m_pCmdCode[i]))(&Stack[idx], -iArgCount);
			break;
		}
		i += m_vByteCode.GetPtrSize();
	}
    goto __start;
	case cmClassFucNum:
		{
			int iArgCount = (int)m_pCmdCode[i++];
			vArg.clear();
			for (int inum=0; inum<iArgCount; ++inum)
			{
				vArg.push_back( Stack[idx+iArgCount-inum-1]);
			}
			classbase *pclass = (classbase  *)m_pCmdCode[i];
			void *pobj = (void *)m_pCmdCode[i+1];
			void *pfunc = ( void *)m_pCmdCode[i+2];
			Stack[idx]= pclass->ApplyClassFunc(pobj,pfunc,vArg);
			i = i + 4;

		}
		goto __start;
	case cmClassFucStr:
		{
			//
			int iArgCount = (int)m_pCmdCode[i++];
			int iIdxStack;
			charpArg.clear();
			for (int inum=0; inum<iArgCount; ++inum)
			{
				iIdxStack = (int)m_pCmdCode[i++];
				charpArg.push_back(m_vStringBuf[iIdxStack].c_str());
			}
			classbase *pclass = (classbase  *)m_pCmdCode[i];
			void *pobj = (void *)m_pCmdCode[i+1];
			void *pfunc = ( void *)m_pCmdCode[i+2];
			Stack[idx]= pclass->ApplyClassFunc(pobj,pfunc,charpArg);
			i = i + 3;

		}
		goto __start;
	case cmClassFucVp:
		{
			//
			int iArgCount = (int)m_pCmdCode[i++];
			void * ptheobj;
			voidpArg.clear();
			for (int inum=0; inum<iArgCount; ++inum)
			{
				ptheobj = (void *)m_pCmdCode[i+3];
				voidpArg.push_back(ptheobj);
			}
			classbase *pclass = (classbase  *)m_pCmdCode[i];
			void *pobj = (void *)m_pCmdCode[i+1];
			void *pfunc = ( void *)m_pCmdCode[i+2];
			Stack[idx]= pclass->ApplyClassFunc(pobj,pfunc,voidpArg);
			i = i + 5;

		}
		goto __start;
	case cmEND:
		return Stack[1];
	default:
		Error(ecINTERNAL_ERROR, 2);
    return 0;
  }

#if defined(_MSC_VER)
  #pragma warning( default : 4312 )
#endif
}

//---------------------------------------------------------------------------
/** \brief Return result for constant functions.
  Seems pointless, but for parser functions that are made up of only a value, which occur
  in real world applications, this speeds up things by removing the parser overhead almost
  completely.
*///cxy explain :main function
value_type ParserBase::ParseValue() const
{
  return *(value_type*)(&m_pCmdCode[2]);
}

//---------------------------------------------------------------------------
/** \brief One of the two main parse functions.
 Parse expression from input string. Perform syntax checking and create bytecode.
 After parsing the string and creating the bytecode the function pointer 
 #m_pParseFormula will be changed to the second parse routine the uses bytecode instead of string parsing.
 \sa ParseCmdCode(), ParseValue()
*/
value_type ParserBase::ParseString() const
{
	#if defined(_MSC_VER)
	#pragma warning( disable : 4311 )
	#endif
	if(!m_pTokenReader->GetFormula().length())
		Error(ecUNEXPECTED_EOF, 0);

	//运算堆栈
	ParserStack<token_type> stOpt, stVal,stClassObj;

	ParserStack<int> stArgCount;
	token_type opta, opt ,opt1,opt2;  // for storing operators
	token_type * popt;
	token_type val, tval;  // for storing value
	string_type strBuf;    // buffer for string function arguments

	ParserStack<token_type> stOptStack;

	token_type ObjectTok;
//	token_type *ptoke,*ptoketemp;
	classbase  *pclass;
	void *pobj ;
	value_type avalue;

	//  [10/26/2012 cxy]
    std::deque<int> stOptStack_Conditionstorage;
    std::deque<int> stOptStack_prebracketstorage;

	ReInit();
	//////////////////////////////////////////////////////////////////////////
	//分析字串,解析
    //int igetstep = 0;
	int icuroptid = 0;

	int iprelbid ;
	int iconditionid ;

	for(;;)
	{
        opt = m_pTokenReader->ReadNextToken();//igetstep);

		opt.SetRunIndex(icuroptid);
 	
		if(opt.GetCode() == cmIf 
		|| opt.GetCode() == cmElse 
		|| opt.GetCode() == cmWhile )
		{
             stOptStack_Conditionstorage.push_front(icuroptid);
			
		}
		if(opt.GetCode() == cmLB  )//{
		{
             stOptStack_prebracketstorage.push_front(icuroptid);
		}
		if(opt.GetCode() == cmRB  )//}
		{

			if(stOptStack_prebracketstorage.size()==stOptStack_Conditionstorage.size())
			{
                iconditionid =  stOptStack_Conditionstorage[0];
                stOptStack_Conditionstorage.pop_front();
				if(cmWhile==stOptStack[iconditionid].GetCode())
				{				
					opt.SetGotoIndex(iconditionid);//回跳到判断
				}
			}
            iprelbid =  stOptStack_prebracketstorage[0];
            stOptStack_prebracketstorage.pop_front();

			popt = stOptStack.GetAt(iprelbid);
			popt->SetGotoIndex(icuroptid+1);//跳转到下一个指令

		}

		//push read opt to opt stack
		stOptStack.push(opt);

		icuroptid ++;

		if( opt.GetCode() == cmEND )
		{
		  break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//反转搜集 公式分析都是从后到前
	//需要改进地方: 当遇到 ";" 分号 为一小段

	if(true==m_boptcollect)
	{	
		m_OptStack.clear();
		m_OptStack = stOptStack;
	}
 
	//for(;;)
	//{
	//	stOptStack2.push(stOptStack1.top());
	//	
	//	if(true==m_optcollect)
	//	{
	//		m_optvector.push_back(stOptStack1.top());
	//	}
	//	stOptStack1.pop();
	//	if( stOptStack1.empty() )
	//	{
	//		break;
	//	}
	//}

	//////////////////////////////////////////////////////////////////////////
	//分类堆放 堆栈运算
	//stVal 字串堆栈
	//stOpt 操作堆栈

	//  [10/30/2012 cxy]记录堆栈号
	std::stack<int>  stopt_storage;
	std::stack<int>  stval_storage;
	std::stack<int>  stClassObj_storage;
	int ioptid =0;

	ECmdCode iGetCode;
	int igotoindex =0;
	int icurstep = 0;
	for(;;)
	{
		if(true==m_bstoprun)
		{
			#ifndef nodefCmdCode
			if(true == m_bcmd)
				m_vByteCode.Finalize();
			#endif
				break;//停止运行
		}

		//  [10/23/2012 cxy]
		opt = stOptStack[icurstep];
		iGetCode=opt.GetCode();
		icurstep++;

		switch (iGetCode)
		{
		  //
		  // Next three are different kind of value entries
		  //
		  case cmSTRING:
				  opt.SetIdx((int)m_vStringBuf.size()); // Assign buffer index to token 
				  stVal.push(opt);
				  m_vStringBuf.push_back(opt.GetAsString()); // Store string in internal buffer
				  break;
		 
		  case cmVAR:
				  stVal.push(opt);
				#ifndef nodefCmdCode
				if(true == m_bcmd)
				{
				//					  	if(b_while_run!=true)
							   m_vByteCode.AddVar( static_cast<value_type*>(opt.GetVar()) );
				}
				#endif
				  break;
		  case cmVARLP:  // var point
				  stVal.push(opt);
				#ifndef nodefCmdCode
				if(true == m_bcmd)
				{
				//					  if(b_while_run!=true)
							  m_vByteCode.AddVar( static_cast<value_type*>(opt.GetVar()) );
				}
				#endif
				  break;
		  case cmVAL:
				  stVal.push(opt);
				#ifndef nodefCmdCode
				if(true == m_bcmd)
				{
				//if(b_while_run!=true)
					m_vByteCode.AddVal( opt.GetVal());
				}
				#endif
				  break;
		  case cmCOMMA:
				  if (stArgCount.empty())
					Error(ecUNEXPECTED_COMMA, m_pTokenReader->GetPos());
				  ++stArgCount.top(); 
				  // Record number of arguments
				  // fall through...
				  // not to break!
		  case cmSEMICOLON:
		  case cmEND: 
		  case cmBC:
			  {
				  // format end or met )
				  // The argument count for parameterless functions is zero
				  // by default an opening bracket sets parameter count to 1
				  // in preparation of arguments to come. If the last token
				  // was an opening bracket we know better...
				  if (opta.GetCode()==cmBO)
					  --stArgCount.top();
				  {
					while ( stOpt.size() && stOpt.top().GetCode() != cmBO )//在函数fun()括号内部判断有否参数数运算a*-+b直到调用函数本身结束循环
					{//非函数操作
						  if (stOpt.top().GetCode()==cmOPRT_INFIX)
						  {
							  ApplyFunc(stOpt, stVal,stClassObj, 1);  // infix operator"!" "-"
						  }
						  else
						  {
							  ApplyBinOprt(stOpt, stVal,stClassObj);  //操作符操作"+","-","*","/"
						  }
					}
					// <ibg> 20060218 infix operator treatment moved here
					if (stOpt.size() && stOpt.top().GetCode()==cmOPRT_INFIX) 
					  ApplyFunc(stOpt, stVal,stClassObj, 1);  // infix operator
					if ( opt.GetCode()!=cmBC 
						|| stOpt.size()==0 
						|| stOpt.top().GetCode()!=cmBO )
						break;
					// if opt is ")" and opta is "(" the bracket has been evaluated, now its time to check
					// if there is either a function or a sign pending
					// neither the opening nor the closing bracket will be pushed back to
					// the operator stack
					// Check if a function is standing in front of the opening bracket, 
					// if yes evaluate it afterwards check for infix operators
					assert(stArgCount.size());
					int iArgCount = stArgCount.pop();

					stOpt.pop(); // Take opening bracket from stack

					if(stOpt.size() 
						&&(stOpt.top().GetCode()==cmWhile))//进入while循环
					{
						avalue= stVal.top().GetVal();
						//  [10/23/2012 cxy] next to change
						// set goto next position in after "{" "}",then the value not true jump to next
						if(avalue!=0)
						{
							igotoindex = 0;
							stVal.pop();
							stOpt.pop();
						}
						else
						{ 
							igotoindex = 1;
							stVal.pop();
							stOpt.pop();
							//跳过{}堆栈语句 or rerun
						}
						break;
					}
					if(stOpt.size() 
						&&(stOpt.top().GetCode()==cmIf))//进入while循环或if判断
					{
						avalue= stVal.top().GetVal();
						//  [10/23/2012 cxy] next to change
						// set goto next position in after "{" "}",then the value not true jump to next
						if(avalue>0)
						{

							igotoindex = 0;
							stVal.pop();
							stOpt.pop();
						}
						else
						{ 
							//跳过{}堆栈语句
							igotoindex = 1;
							stVal.pop();
							stOpt.pop();
						}
						break;
					}

					if (iArgCount>1 
						&&(stOpt.size()==0 || stOpt.top().GetCode()!=cmFUNC)) //一般函数
						if(stOpt.size()!=0 && stOpt.top().GetCode()!=cmClassFuc)//类成员函数
					  Error(ecUNEXPECTED_ARG, m_pTokenReader->GetPos());
	            
					if (stOpt.size() && stOpt.top().GetCode()!=cmOPRT_INFIX)
					  ApplyFunc(stOpt, stVal,stClassObj, iArgCount);
				  } // if bracket content is evaluated
				  break;	
			  }
		  //
		  // Next are the binary operator entries
		  //
		  case cmAND:   // built in binary operators
		  case cmOR:
		  case cmXOR:
		  case cmLT:
		  case cmGT:
		  case cmLE:
		  case cmGE:
		  case cmNEQ:
		  case cmEQ:
		  case cmADD:
		  case cmSUB:
		  case cmMUL:
		  case cmDIV:
		  case cmPOW:
		  case cmASSIGN:
		  case cmOPRT_BIN:// + - * /
			  // A binary operator (user defined or built in) has been found. 
				  while ( stOpt.size() && stOpt.top().GetCode() != cmBO)
				  {
					  if (GetOprtPri(stOpt.top()) < GetOprtPri(opt))
						break;
					if (stOpt.top().GetCode()==cmOPRT_INFIX)
					  ApplyFunc(stOpt, stVal, stClassObj,1);  // infix operator
					else
					  ApplyBinOprt(stOpt, stVal,stClassObj);
				  } // while ( ... )

				// The operator can't be evaluated right now, push back to the operator stack
				  stOpt.push(opt);
				  break;
		  //
		  // Last section contains functions and operators implicitely mapped to functions
		  //
		  case cmBO:
				  stArgCount.push(1); // we use this for counting the bracket level
				  stOpt.push(opt);
    					break;
		  case cmFUNC:
		  case cmFUNC_STR:
		  case cmOPRT_INFIX:
				  stOpt.push(opt);
    					break;
		  case cmOPRT_POSTFIX:
				  stOpt.push(opt);
				  ApplyFunc(stOpt, stVal, stClassObj,1);  // this is the postfix operator
				  break;

		  case cmLB:  ///< Operator item:  lift bracket  "{"
				if(1==igotoindex)
					icurstep=opt.GetGotoIndex();

				ioptid = stOpt.size();
				ioptid = ioptid >0? ioptid-1:0;
				stopt_storage.push(ioptid);
				ioptid = stVal.size();
				ioptid = ioptid >0? ioptid-1:0;
				stval_storage.push(ioptid);
				ioptid = stClassObj.size();
				ioptid = ioptid >0? ioptid-1:0;
				stClassObj_storage.push(ioptid);
 
			 break;
		  case cmRB: ///< Operator item:  right bracket  "}"
			  icurstep=opt.GetGotoIndex()>-1?opt.GetGotoIndex():icurstep;

			  ioptid = stopt_storage.top();
			  for(int i=stOpt.size();i>ioptid;i--)
				  stOpt.pop();

			  ioptid = stopt_storage.top();
			  for(int i=stVal.size();i>ioptid;i--)
				  stVal.pop();

			  ioptid = stClassObj_storage.top();
			  for(int i=stClassObj.size();i>ioptid;i--)
				  stClassObj.pop();

			  stopt_storage.pop();
			  stval_storage.pop();
			  stClassObj_storage.pop();

			break;
		  case cmIf:
		  case cmWhile:
			   stOpt.push(opt);
			  break;
		  case cmElse:
			  //  [11/13/2012 cxy]
			if(1==igotoindex)//if 判断结论设置状态位 相反
			{
				igotoindex = 0;
			}
			else
			{
				igotoindex = 1;
			}
			break;
		  case cmCallin:
			  break;
		  case cmClassObjDef:
			{
				pclass= opt.m_pClass;
				pobj=pclass->addvar(opt.GetAsString());//
				if(0!=pobj)
					opt.SetClassVarDef(pclass,pobj, opt.GetAsString() );
			}
			  break;
		  case cmClass:

			  break;

		 case cmClassObj:

			  stClassObj.push(opt);
			  //处理类对象成员变量操作 ACLASS aobjec.m_a+aobjec.m_b
			  //处理类对象操作 ACLASS aobject bobject;fun(ACLASS ,ACLASS);>>ok
			  //处理类成员函数调用  ACLASS aobject.fun(); >>ok
			  break;


		  case cmMember:
			  stClassObj.pop();
			  break;
		  case cmPointer:

			  break;
		  case cmClassFuc:
			  stOpt.push(opt);
			  break;
		//cxyaddend
		  default:  
			Error(ecINTERNAL_ERROR, 3);
		} // end of switch operator-token

		if( opt.GetCode() == cmEND )
		{
	#ifndef nodefCmdCode
	if(true == m_bcmd)	
	{
	//				if(b_while_run!=true)
			m_vByteCode.Finalize();
	}
	#endif
			break;
		}
	  } 

#ifndef nodefCmdCode	
if(true == m_bcmd)
{
		  // Store pointer to start of bytecode
		  m_pCmdCode = m_vByteCode.GetRawData();
}
#endif

		  //Set Formula string
		  m_StringFormula=m_pTokenReader->GetFormula();
#ifndef nodefCmdCode
if(true == m_bcmd)
{
		  // Collection the bytecode
		  if(true==m_bcolllection)
		  {
			  m_vByteCodeCollection.SetStorageBase(
				  m_vByteCodeCollection.CollectStorageData(m_vByteCodeCollection.GetStorageBase(),
															m_vByteCode.GetStorageBase())
													);
			  //  m_vByteCodeCollection.CollectRawData(m_vByteCode.GetStorageBase());
			  string_type sBul=m_pTokenReader->GetFormula()+"\r\n";
			  m_StrCollection=m_StrCollection+sBul;
		  }
}
#endif

		  if(0!=stVal.size())
		  {
			  // get the last value (= final result) from the stack
			  if (stVal.size()!=1)
			  {
				  //存在多个运算等式
				  //Error(ecINTERNAL_ERROR, 4);
			  }
			  if (stVal.top().GetType()!=tpDBL)
				  Error(ecSTR_RESULT);

			  // no error, so change the function pointer for the main parse routine
			  value_type fVal = stVal.top().GetVal();   // Result from String parsing

#ifdef nodefCmdCode
			  if (m_bUseByteCode)
			  {
				  m_pParseFormula = 
					  (m_pCmdCode[1]==cmVAL && m_pCmdCode[6]==cmEND) ? 
					  &ParserBase::ParseValue :
				  &ParserBase::ParseCmdCode;
			  }
#endif

			  return fVal;
		  }
		  else
		  {
			  return 0;\
		  }
}

ParserByteCode::storage_type ParserBase::GetStorageBase()
{
	return m_vByteCode.GetStorageBaseData();
}
//---------------------------------------------------------------------------
/** \brief Create an error containing the parse error position.

  This function will create an Parser Exception object containing the error text and
  its position.

  \param a_iErrc [in] The error code of type #EErrorCodes.
  \param a_iPos [in] The position where the error was detected.
  \param a_strTok [in] The token string representation associated with the error.
  \throw ParserException always throws thats the only purpose of this function.
*/
void  ParserBase::Error(EErrorCodes a_iErrc, int a_iPos, const string_type &a_sTok) const
{
  throw exception_type(a_iErrc, a_sTok, m_pTokenReader->GetFormula(), a_iPos);
}

//------------------------------------------------------------------------------
/** \brief Clear all user defined variables.

    Resets the parser to string parsing mode by calling #ReInit.
    \throw nothrow
*/
void ParserBase::ClearVar()
{
  m_VarDef.clear();
  ReInit();
}

//------------------------------------------------------------------------------
/** \brief Remove a variable from internal storage.

Removes a variable if it exists. If the Variable does not exist nothing will be done.

\throw nothrow
*/
void ParserBase::RemoveVar(const string_type &a_strVarName)
{
	varmap_type::iterator item = m_VarDef.find(a_strVarName);
	if (item!=m_VarDef.end())
	{
		m_VarDef.erase(item);
		ReInit();
	}
}

//------------------------------------------------------------------------------
//cxvision function
/** \brief Clear the formula. 
Clear the formula and existing Bytecode.
\post Resets the parser to string parsing mode.
\throw nothrow
*/
void ParserBase::ClearFormula()
{
  m_vByteCode.clear();
  m_pCmdCode = 0;
  m_pTokenReader->SetFormula("");
  ReInit();
}

//------------------------------------------------------------------------------
//cxvision function
/** \brief Clear all functions.
    \post Resets the parser to string parsing mode.
    \throw nothrow
*/
void ParserBase::ClearFun()
{
  m_FunDef.clear();
  ReInit();
}

//------------------------------------------------------------------------------
//cxvision function
/** \brief Clear all user defined constants.
    Both numeric and string constants will be removed from the internal storage.
    \post Resets the parser to string parsing mode.
    \throw nothrow
*/
void ParserBase::ClearConst()
{
  m_ConstDef.clear();
  m_StrVarDef.clear();
  ReInit();
}

//------------------------------------------------------------------------------
//cxvision function
/** \brief Clear all user defined postfix operators.
    \post Resets the parser to string parsing mode.
    \throw nothrow
*/
void ParserBase::ClearPostfixOprt()
{
  m_PostOprtDef.clear();
  ReInit();
}

//------------------------------------------------------------------------------
//cxvision function
/** \brief Clear all user defined binary operators.
    \post Resets the parser to string parsing mode.
    \throw nothrow
*/
void ParserBase::ClearOprt()
{
  m_OprtDef.clear();
  ReInit();
}
//------------------------------------------------------------------------------
//cxvision function
// clear class object
void ParserBase::ClearClassObj()
{
	classbasemap_type:: iterator pIter;

	for ( pIter = m_ClassDefMap.begin( ) ; pIter != m_ClassDefMap.end(); pIter++ )
	{
		classbase *pbase=pIter->second;
		pbase->clearvar();
	}
	
}
//------------------------------------------------------------------------------
//cxvision function
//clear some class all var
void ParserBase::RemoveClassObject(const string_type &a_strClassnName)
{
	classbasemap_type:: iterator pIter= m_ClassDefMap.find(a_strClassnName);

	classbase *pbase=pIter->second;

	pbase->clearvar();
}
//------------------------------------------------------------------------------
//cxvision function
//clear some class some var
void ParserBase::RemoveClassObject(const string_type &a_strClassnName,const string_type &a_strOjectName)
{
	classbasemap_type:: iterator pIter= m_ClassDefMap.find(a_strClassnName);

	classbase *pbase=pIter->second;
	
	pbase->delvar(a_strOjectName);
}
//------------------------------------------------------------------------------
//cxvision function
/** \brief Clear the user defined Prefix operators. 
    \post Resets the parser to string parser mode.
    \throw nothrow
*/
void ParserBase::ClearInfixOprt()
{
  m_InfixOprtDef.clear();
  ReInit();
}
//------------------------------------------------------------------------------
//cxvision function
/** \brief Enable or disable the formula optimization feature. 
    \post Resets the parser to string parser mode.
    \throw nothrow
*/
void ParserBase::EnableOptimizer(bool a_bIsOn)
{
  m_bOptimize = a_bIsOn;
  ReInit();
}
//------------------------------------------------------------------------------
//cxvision function
/** \brief Enable or disable parsing from Bytecode. 

    \attention There is no reason to disable bytecode. It will 
               drastically decrease parsing speed.
*/
void ParserBase::EnableByteCode(bool a_bIsOn)
{
  m_bUseByteCode = a_bIsOn;
  if (!a_bIsOn)
    ReInit();
}
//------------------------------------------------------------------------------
//cxvision function
/** \brief Enable or disable the built in binary operators.

   If you disable the built in binary operators there will be no binary operators
   defined. Thus you must add them manually one by one. It is not possible to
   disable built in operators selectively. This function will Reinitialize the
   parser by calling ReInit().

   \throw nothrow
   \sa m_bBuiltInOp, ReInit()
*/
void ParserBase::EnableBuiltInOprt(bool a_bIsOn)
{
  m_bBuiltInOp = a_bIsOn;
  ReInit();
}
//------------------------------------------------------------------------------
//cxvision function
/** \brief Query status of built in variables.
    \return #m_bBuiltInOp; true if built in operators are enabled.
    \throw nothrow
*/
bool ParserBase::HasBuiltInOprt() const
{
  return m_bBuiltInOp;
}

//---------------------------------------------------------------------------
//cxvision function
/** \brief Parse the command code.

  Command code contains precalculated stack positions of the values and the
  associated operators.  
  The Stack is filled beginning from index one the value at index zero is
  not used at all.

  \sa ParseString(), ParseValue(),ParseCmdCode()
	堆栈优化后的收集

*/
value_type  ParserBase::RunCollectionCmdCode() const
{
   int isize= m_vByteCodeCollection.GetRawDataSize();
   if(isize<3)
	   return 0;

   //
   paramvect vArg;
   voidparamvect voidpArg;
   charpvect charpArg;

  m_pCmdCodeCollection=m_vByteCodeCollection.GetRawData();
 
  value_type Stack[99];
  ECmdCode iCode;
  bytecode_type idx(0);
  int i(0);

  __start:

  idx = m_pCmdCodeCollection[i];
  iCode = (ECmdCode)m_pCmdCodeCollection[i+1];
  i += 2;

#ifdef _DEBUG
  if (idx>=99)
    throw exception_type(ecGENERIC, "", m_pTokenReader->GetFormula(), -1);
#endif

  switch (iCode)
  {
    // built in binary operators
    case cmAND: Stack[idx]  = (int)Stack[idx] & (int)Stack[idx+1]; goto __start;
    case cmOR:  Stack[idx]  = (int)Stack[idx] | (int)Stack[idx+1]; goto __start;
    case cmXOR: Stack[idx]  = (int)Stack[idx] ^ (int)Stack[idx+1]; goto __start;
    case cmLE:  Stack[idx]  = Stack[idx] <= Stack[idx+1]; goto __start;
    case cmGE:  Stack[idx]  = Stack[idx] >= Stack[idx+1]; goto __start;
    case cmNEQ: Stack[idx]  = Stack[idx] != Stack[idx+1]; goto __start;
    case cmEQ:  Stack[idx]  = Stack[idx] == Stack[idx+1]; goto __start;
	case cmLT:  Stack[idx]  = Stack[idx] < Stack[idx+1];  goto __start;
	case cmGT:  Stack[idx]  = Stack[idx] > Stack[idx+1];  goto __start;
    case cmADD: Stack[idx] += Stack[1+idx]; goto __start;
 	case cmSUB: Stack[idx] -= Stack[1+idx]; goto __start;
	case cmMUL: Stack[idx] *= Stack[1+idx]; goto __start;
	case cmDIV: Stack[idx] /= Stack[1+idx]; goto __start;
    case cmPOW: Stack[idx]  = pow(Stack[idx], Stack[1+idx]); goto __start;

    // Assignement needs special treatment
    case cmASSIGN:
           {
             // next is a pointer to the target
             value_type **pDest = (value_type**)(&m_pCmdCodeCollection[i]);

             // advance index according to pointer size
             i += m_vByteCodeCollection.GetPtrSize();
             // assign the value
             Stack[idx] = **pDest = Stack[idx+1];
           }
           goto __start;

    // user defined binary operators
    case cmOPRT_BIN:
			Stack[idx] = (**(fun_type2**)(&m_pCmdCodeCollection[i]))(Stack[idx], Stack[idx+1]);
			++i;
			goto __start;

    // variable tokens
	  case cmVAR:
	        Stack[idx] = **(value_type**)(&m_pCmdCodeCollection[i]);
	        i += m_vByteCodeCollection.GetValSize();
	        goto __start;

    // value tokens
	  case cmVAL:
            Stack[idx] = *(value_type*)(&m_pCmdCodeCollection[i]);
 	          i += m_vByteCodeCollection.GetValSize();
            goto __start;

    // Next is treatment of string functions
    case cmFUNC_STR:
            {
              i++; // skip the unused argument count
              strfun_type1 pFun = *(strfun_type1*)(&m_pCmdCodeCollection[i]);
              i += m_vByteCodeCollection.GetPtrSize();

              int iIdxStack = (int)m_pCmdCodeCollection[i++];

              MUP_ASSERT( iIdxStack>=0 && iIdxStack<(int)m_vStringBuf.size() );
              Stack[idx] = pFun(m_vStringBuf[iIdxStack].c_str());
            }
            goto __start;

    // Next is treatment of numeric functions
    case cmFUNC:
		{
		      int iArgCount = (int)m_pCmdCodeCollection[i++];
              switch(iArgCount)  // switch according to argument count
		      {
                case 1: Stack[idx] = (*(fun_type1*)(&m_pCmdCodeCollection[i]))(Stack[idx]); break;
			    case 2: Stack[idx] = (*(fun_type2*)(&m_pCmdCodeCollection[i]))(Stack[idx], Stack[idx+1]); break;
			    case 3: Stack[idx] = (*(fun_type3*)(&m_pCmdCodeCollection[i]))(Stack[idx], Stack[idx+1], Stack[idx+2]); break;
			    case 4: Stack[idx] = (*(fun_type4*)(&m_pCmdCodeCollection[i]))(Stack[idx], Stack[idx+1], Stack[idx+2], Stack[idx+3]); break;
  		        case 5: Stack[idx] = (*(fun_type5*)(&m_pCmdCodeCollection[i]))(Stack[idx], Stack[idx+1], Stack[idx+2], Stack[idx+3], Stack[idx+4]); break;
                default:
				 if (iArgCount>0) // function with variable arguments store the number as a negative value
                  Error(ecINTERNAL_ERROR, 1);
                 Stack[idx] =(*(multfun_type*)(&m_pCmdCodeCollection[i]))(&Stack[idx], -iArgCount);
                  break;
		       }
		          i += m_vByteCodeCollection.GetPtrSize();
		  }
		  goto __start;
	case cmClassFucNum:
		{
			int iArgCount = (int)m_pCmdCodeCollection[i++];
			vArg.clear();
			for (int inum=0; inum<iArgCount; ++inum)
			{
				vArg.push_back( Stack[idx+iArgCount-inum-1]);
			}
			classbase *pclass = (classbase  *)m_pCmdCodeCollection[i];
			void *pobj = (void *)m_pCmdCodeCollection[i+1];
			void *pfunc = ( void *)m_pCmdCodeCollection[i+2];
			Stack[idx]= pclass->ApplyClassFunc(pobj,pfunc,vArg);
			i = i + 4;

		}
		goto __start;
	case cmClassFucStr:
		{
			//
			int iArgCount = (int)m_pCmdCodeCollection[i++];
			int iIdxStack;
			charpArg.clear();
			for (int inum=0; inum<iArgCount; ++inum)
			{
				iIdxStack = (int)m_pCmdCodeCollection[i+3];
				charpArg.push_back(m_vStringBuf[iIdxStack].c_str());
			}
			classbase *pclass = (classbase  *)m_pCmdCodeCollection[i];
			void *pobj = (void *)m_pCmdCodeCollection[i+1];
			void *pfunc = ( void *)m_pCmdCodeCollection[i+2];
			Stack[idx]= pclass->ApplyClassFunc(pobj,pfunc,charpArg);
			i = i + 5;

		}
		goto __start;
	case cmClassFucVp:
		{
			//
			int iArgCount = (int)m_pCmdCodeCollection[i++];
			void * ptheobj;
			voidpArg.clear();
			for (int inum=0; inum<iArgCount; ++inum)
			{
				ptheobj = (void *)m_pCmdCodeCollection[i+3];
				voidpArg.push_back(ptheobj);
			}
			classbase *pclass = (classbase  *)m_pCmdCodeCollection[i];
			void *pobj = (void *)m_pCmdCodeCollection[i+1];
			void *pfunc = ( void *)m_pCmdCodeCollection[i+2];
			Stack[idx]= pclass->ApplyClassFunc(pobj,pfunc,voidpArg);
			i = i + 5;

		}
		goto __start;
	case cmEND:
		        return Stack[1];

	  default:
            Error(ecINTERNAL_ERROR, 2);
            return 0;
  }
}






//---------------------------------------------------------------------------
//cxvision function
//运行收集字串剖析后生成的操作符(非堆栈优化后的收集)
value_type ParserBase::RunCollectionOpt() const
{
#if defined(_MSC_VER)
#pragma warning( disable : 4311 )
#endif
	//运算堆栈
	ParserStack<token_type> stOpt, stVal,stClassObj;

	ParserStack<int> stArgCount;
	token_type opta, opt ,opt1,opt2;  // for storing operators
	token_type * popt;
	token_type val, tval;  // for storing value
	string_type strBuf;    // buffer for string function arguments

	token_type ObjectTok;
	token_type *ptoke,*ptoketemp;
	classbase  *pclass;
	void *pobj ;
	value_type avalue;

	ReInit();
	//////////////////////////////////////////////////////////////////////////
	//分析字串,解析
	int igetstep = 0;
	int icuroptid = 0;

	int iprelbid ;
	int iconditionid ;

	//  [10/30/2012 cxy]记录堆栈号
	std::stack<int>  stopt_storage;
	std::stack<int>  stval_storage;
	std::stack<int>  stClassObj_storage;
	int ioptid =0;

	ECmdCode iGetCode;
	int igotoindex =0;
	int icurstep = 0;
	for(;;)
	{
		if(true==m_bstoprun)
			break;//停止运行

		//  [10/23/2012 cxy]
		opt = m_OptStack[icurstep];
		iGetCode=opt.GetCode();
		icurstep++;

		switch (iGetCode)
		{
			//
			// Next three are different kind of value entries
			//
		case cmSTRING:
			opt.SetIdx((int)m_vStringBuf.size()); // Assign buffer index to token 
			stVal.push(opt);
			m_vStringBuf.push_back(opt.GetAsString()); // Store string in internal buffer
			break;

		case cmVAR:
			stVal.push(opt);
			break;
		case cmVARLP:  // var point
			stVal.push(opt);
			break;
		case cmVAL:
			stVal.push(opt);
			break;
		case cmCOMMA:
			if (stArgCount.empty())
				Error(ecUNEXPECTED_COMMA, m_pTokenReader->GetPos());
			++stArgCount.top(); 
			// Record number of arguments
			// fall through...
			// not to break!
		case cmSEMICOLON:
		case cmEND: 
		case cmBC:
			{
				// format end or met )
				// The argument count for parameterless functions is zero
				// by default an opening bracket sets parameter count to 1
				// in preparation of arguments to come. If the last token
				// was an opening bracket we know better...
				if (opta.GetCode()==cmBO)
					--stArgCount.top();
				{
					while ( stOpt.size() && stOpt.top().GetCode() != cmBO )//在函数fun()括号内部判断有否参数数运算a*-+b直到调用函数本身结束循环
					{//非函数操作
						if (stOpt.top().GetCode()==cmOPRT_INFIX)
						{
							ApplyFunc(stOpt, stVal,stClassObj, 1);  // infix operator"!" "-"
						}
						else
						{
							ApplyBinOprt(stOpt, stVal,stClassObj);  //操作符操作"+","-","*","/"
						}
					}
					// <ibg> 20060218 infix operator treatment moved here
					if (stOpt.size() && stOpt.top().GetCode()==cmOPRT_INFIX) 
						ApplyFunc(stOpt, stVal,stClassObj, 1);  // infix operator
					if ( opt.GetCode()!=cmBC 
						|| stOpt.size()==0 
						|| stOpt.top().GetCode()!=cmBO )
						break;
					// if opt is ")" and opta is "(" the bracket has been evaluated, now its time to check
					// if there is either a function or a sign pending
					// neither the opening nor the closing bracket will be pushed back to
					// the operator stack
					// Check if a function is standing in front of the opening bracket, 
					// if yes evaluate it afterwards check for infix operators
					assert(stArgCount.size());
					int iArgCount = stArgCount.pop();

					stOpt.pop(); // Take opening bracket from stack

					if(stOpt.size() 
						&&(stOpt.top().GetCode()==cmWhile))//进入while循环
					{
						avalue= stVal.top().GetVal();
						//  [10/23/2012 cxy] next to change
						// set goto next position in after "{" "}",then the value not true jump to next
						if(avalue!=0)
						{
							igotoindex = 0;
							stVal.pop();
							stOpt.pop();
						}
						else
						{ 
							igotoindex = 1;
							stVal.pop();
							stOpt.pop();
							//跳过{}堆栈语句 or rerun
						}
						break;
					}
					if(stOpt.size() 
						&&(stOpt.top().GetCode()==cmIf))//进入while循环或if判断
					{
						avalue= stVal.top().GetVal();
						//  [10/23/2012 cxy] next to change
						// set goto next position in after "{" "}",then the value not true jump to next
						if(avalue>0)
						{

							igotoindex = 0;
							stVal.pop();
							stOpt.pop();
						}
						else
						{ 
							//跳过{}堆栈语句
							igotoindex = 1;
							stVal.pop();
							stOpt.pop();
						}
						break;
					}


					if (iArgCount>1 
						&&(stOpt.size()==0 || stOpt.top().GetCode()!=cmFUNC)) //一般函数
						if(stOpt.size()!=0 && stOpt.top().GetCode()!=cmClassFuc)//类成员函数
							Error(ecUNEXPECTED_ARG, m_pTokenReader->GetPos());

					if (stOpt.size() && stOpt.top().GetCode()!=cmOPRT_INFIX)
						ApplyFunc(stOpt, stVal,stClassObj, iArgCount);
				} // if bracket content is evaluated
				break;	
			}
			//
			// Next are the binary operator entries
			//
		case cmAND:   // built in binary operators
		case cmOR:
		case cmXOR:
		case cmLT:
		case cmGT:
		case cmLE:
		case cmGE:
		case cmNEQ:
		case cmEQ:
		case cmADD:
		case cmSUB:
		case cmMUL:
		case cmDIV:
		case cmPOW:
		case cmASSIGN:
		case cmOPRT_BIN:// + - * /
			// A binary operator (user defined or built in) has been found. 
			while ( stOpt.size() && stOpt.top().GetCode() != cmBO)
			{
				if (GetOprtPri(stOpt.top()) < GetOprtPri(opt))
					break;
				if (stOpt.top().GetCode()==cmOPRT_INFIX)
					ApplyFunc(stOpt, stVal, stClassObj,1);  // infix operator
				else
					ApplyBinOprt(stOpt, stVal,stClassObj);
			} // while ( ... )

			// The operator can't be evaluated right now, push back to the operator stack
			stOpt.push(opt);
			break;
			//
			// Last section contains functions and operators implicitely mapped to functions
			//
		case cmBO:
			stArgCount.push(1); // we use this for counting the bracket level
			stOpt.push(opt);
			break;
		case cmFUNC:
		case cmFUNC_STR:
		case cmOPRT_INFIX:
			stOpt.push(opt);
			break;
		case cmOPRT_POSTFIX:
			stOpt.push(opt);
			ApplyFunc(stOpt, stVal, stClassObj,1);  // this is the postfix operator
			break;

		case cmLB:  ///< Operator item:  lift bracket  "{"
			if(1==igotoindex)
				icurstep=opt.GetGotoIndex();

			ioptid = stOpt.size();
			ioptid = ioptid >0? ioptid-1:0;
			stopt_storage.push(ioptid);
			ioptid = stVal.size();
			ioptid = ioptid >0? ioptid-1:0;
			stval_storage.push(ioptid);
			ioptid = stClassObj.size();
			ioptid = ioptid >0? ioptid-1:0;
			stClassObj_storage.push(ioptid);

			break;
		case cmRB: ///< Operator item:  right bracket  "}"
			icurstep=opt.GetGotoIndex()>-1?opt.GetGotoIndex():icurstep;

			ioptid = stopt_storage.top();
			for(int i=stOpt.size();i>ioptid;i--)
				stOpt.pop();

			ioptid = stopt_storage.top();
			for(int i=stVal.size();i>ioptid;i--)
				stVal.pop();

			ioptid = stClassObj_storage.top();
			for(int i=stClassObj.size();i>ioptid;i--)
				stClassObj.pop();

			stopt_storage.pop();
			stval_storage.pop();
			stClassObj_storage.pop();

			break;
		case cmIf:
		case cmWhile:
			stOpt.push(opt);
			break;
		case cmElse:
			//  [11/13/2012 cxy]
			if(1==igotoindex)//if 判断结论设置状态位 相反
			{
				igotoindex = 0;
			}
			else
			{
				igotoindex = 1;
			}
			break;
		case cmCallin:
			break;
		case cmClassObjDef:
			{
				pclass= opt.m_pClass;
				pobj=pclass->addvar(opt.GetAsString());//
				if(0!=pobj)
					opt.SetClassVarDef(pclass,pobj, opt.GetAsString() );
			}
			break;
		case cmClass:

			break;

		case cmClassObj:

			stClassObj.push(opt);
			//处理类对象成员变量操作 ACLASS aobjec.m_a+aobjec.m_b
			//处理类对象操作 ACLASS aobject bobject;fun(ACLASS ,ACLASS);>>ok
			//处理类成员函数调用  ACLASS aobject.fun(); >>ok
			break;


		case cmMember:
			stClassObj.pop();
			break;
		case cmPointer:

			break;
		case cmClassFuc:
			stOpt.push(opt);
			break;
			//cxyaddend
		default:  
			Error(ecINTERNAL_ERROR, 3);
		} // end of switch operator-token

		if( opt.GetCode() == cmEND )
			break;
	} 



	//Set Formula string
	m_StringFormula=m_pTokenReader->GetFormula();


	if(0!=stVal.size())
	{

		if (stVal.top().GetType()!=tpDBL)
			Error(ecSTR_RESULT);

		// no error, so change the function pointer for the main parse routine
		value_type fVal = stVal.top().GetVal();   // Result from String parsing

		return fVal;
	}
	else
	{
		return 0;
	}
}

//---------------------------------------------------------------------------
//cxvision function
//拷贝收集的运行操作码
void ParserBase::CopyRUNOpt(int inum)
{	
	char chnum[25];
	sprintf(chnum, "%d" ,inum);
	//if (m_mapoptstack.find(string(chnum))==m_mapoptstack.end())
	m_mapoptstack[string(chnum)] = m_OptStack;

	//switch(inum)
	//{
	//	case 1:
	//		m_optstack1 = m_OptStack;
	//	break;
	//	case 2:
	//		m_optstack2 = m_OptStack;
	//	break;
	//	case 3:
	//		m_optstack3 = m_OptStack;
	//	break;
	//}
}

//////////////////////////////////////////////////////////////////////////
//cxvision function
//run option code
void ParserBase::RunOpt(int ioptnum)
{
	char chnum[25];
	sprintf(chnum, "%d" ,ioptnum);
	if (m_mapoptstack.find(string(chnum))!=m_mapoptstack.end())
	RunOptStack(m_mapoptstack[string(chnum)]);
	//switch(ioptnum)
	//{
	//	case 1:
	//		RunOptStack(m_optstack1);
	//	break;
	//	case 2:
	//		RunOptStack(m_optstack2);
	//	break;
	//	case 3:
	//		RunOptStack(m_optstack3);
	//	break;
	//}
}
//-----------------------------------------------------------------------
//  [12/18/2012 cxy]
//运行map堆栈
void ParserBase::RunOptString(const char *poptstackname)
{
	//AfxMessageBox(poptstackname);
	std::string optstackname(poptstackname);
	if(optstackname.size()<=0)
		return;
	if (m_mapoptstack.find(optstackname)!=m_mapoptstack.end())
	RunOptStack(m_mapoptstack[optstackname]);
}
//---------------------------------------------------------------------------
//  [12/18/2012 cxy]
//设置m_OptStack到 map opt stack 
void ParserBase::SetOptStack(const string_type & optstackname)
{	 
	if(optstackname.size()<=0)
	return;
	//if (m_mapoptstack.find(optstackname)==m_mapoptstack.end())
	m_mapoptstack[optstackname] = m_OptStack;
}
void ParserBase::ClearOptStack()
{
	m_mapoptstack.clear();
}
//////////////////////////////////////////////////////////////////////////
//run function in time else escape to outside
//void ParserBase::RunOptTimeLimit(int ioptnum)
//{
//	switch(ioptnum)
//	{
//	case 1:
//		RunOptVect_SetTime(m_optvector1);
//		break;
//	case 2:
//		RunOptVect_SetTime(m_optvector2);
//		break;
//	case 3:
//		RunOptVect_SetTime(m_optvector3);
//		break;
//	}
//}
//////////////////////////////////////////////////////////////////////////
//opt vect run
//运行搜集的OPT码
inline value_type ParserBase::RunOptStack(TokeStack & optstack) const
{
#if defined(_MSC_VER)
#pragma warning( disable : 4311 )
#endif
	//if(!m_pTokenReader->GetFormula().length())
	//	Error(ecUNEXPECTED_EOF, 0);

	if(optstack.size()==0)
		return 0;
	//运算堆栈
	ParserStack<token_type> stOpt, stVal,stClassObj;

	ParserStack<int> stArgCount;
	token_type opta, opt ,opt1,opt2;  // for storing operators
	token_type * popt;
	token_type val, tval;  // for storing value
	string_type strBuf;    // buffer for string function arguments


	token_type ObjectTok;
	token_type *ptoke,*ptoketemp;
	classbase  *pclass;
	void *pobj ;
	value_type avalue;

	ReInit();
	//////////////////////////////////////////////////////////////////////////
	//分析字串,解析
//	int igetstep = 0;
//	int icuroptid = 0;

//	int iprelbid ;
//	int iconditionid ;



	//////////////////////////////////////////////////////////////////////////
	//分类堆放 堆栈运算
	//stVal 字串堆栈
	//stOpt 操作堆栈

	//  [10/30/2012 cxy]记录堆栈号
	std::stack<int>  stopt_storage;
	std::stack<int>  stval_storage;
	std::stack<int>  stClassObj_storage;
	int ioptid =0;

	ECmdCode iGetCode;
	int igotoindex =0;
	int icurstep = 0;
	for(;;)
	{
		if(true==m_bstoprun)
			break;//停止运行

		//  [10/23/2012 cxy]
		opt = optstack[icurstep];
		iGetCode=opt.GetCode();
		icurstep++;

		switch (iGetCode)
		{
			//
			// Next three are different kind of value entries
			//
		case cmSTRING:
			opt.SetIdx((int)m_vStringBuf.size()); // Assign buffer index to token 
			stVal.push(opt);
			m_vStringBuf.push_back(opt.GetAsString()); // Store string in internal buffer
			break;

		case cmVAR:
			stVal.push(opt);

			break;
		case cmVARLP:  // var point
			stVal.push(opt);

			break;
		case cmVAL:
			stVal.push(opt);

			break;
		case cmCOMMA:
			if (stArgCount.empty())
				Error(ecUNEXPECTED_COMMA, icurstep);
			++stArgCount.top(); 
			// Record number of arguments
			// fall through...
			// not to break!
		case cmSEMICOLON:
		case cmEND: 
		case cmBC:
			{
				// format end or met )
				// The argument count for parameterless functions is zero
				// by default an opening bracket sets parameter count to 1
				// in preparation of arguments to come. If the last token
				// was an opening bracket we know better...
				if (opta.GetCode()==cmBO)
					--stArgCount.top();
				{
					while ( stOpt.size() && stOpt.top().GetCode() != cmBO )//在函数fun()括号内部判断有否参数数运算a*-+b直到调用函数本身结束循环
					{//非函数操作
						if (stOpt.top().GetCode()==cmOPRT_INFIX)
						{
							ApplyFunc(stOpt, stVal,stClassObj, 1);  // infix operator"!" "-"
						}
						else
						{
							ApplyBinOprt(stOpt, stVal,stClassObj);  //操作符操作"+","-","*","/"
						}
					}
					// <ibg> 20060218 infix operator treatment moved here
					if (stOpt.size() && stOpt.top().GetCode()==cmOPRT_INFIX) 
						ApplyFunc(stOpt, stVal,stClassObj, 1);  // infix operator
					if ( opt.GetCode()!=cmBC 
						|| stOpt.size()==0 
						|| stOpt.top().GetCode()!=cmBO )
						break;
					// if opt is ")" and opta is "(" the bracket has been evaluated, now its time to check
					// if there is either a function or a sign pending
					// neither the opening nor the closing bracket will be pushed back to
					// the operator stack
					// Check if a function is standing in front of the opening bracket, 
					// if yes evaluate it afterwards check for infix operators
					assert(stArgCount.size());
					int iArgCount = stArgCount.pop();

					stOpt.pop(); // Take opening bracket from stack

					if(stOpt.size() 
						&&(stOpt.top().GetCode()==cmWhile))//进入while循环
					{
						avalue= stVal.top().GetVal();
						//  [10/23/2012 cxy] next to change
						// set goto next position in after "{" "}",then the value not true jump to next
						if(avalue!=0)
						{
							igotoindex = 0;
							stVal.pop();
							stOpt.pop();
						}
						else
						{ 
							igotoindex = 1;
							stVal.pop();
							stOpt.pop();
							//跳过{}堆栈语句 or rerun
						}
						break;
					}
					if(stOpt.size() 
						&&(stOpt.top().GetCode()==cmIf))//进入while循环或if判断
					{
						avalue= stVal.top().GetVal();
						//  [10/23/2012 cxy] next to change
						// set goto next position in after "{" "}",then the value not true jump to next
						if(avalue>0)
						{

							igotoindex = 0;
							stVal.pop();
							stOpt.pop();
						}
						else
						{ 
							//跳过{}堆栈语句
							igotoindex = 1;
							stVal.pop();
							stOpt.pop();
						}
						break;
					}


					if (iArgCount>1 
						&&(stOpt.size()==0 || stOpt.top().GetCode()!=cmFUNC)) //一般函数
						if(stOpt.size()!=0 && stOpt.top().GetCode()!=cmClassFuc)//类成员函数
							Error(ecUNEXPECTED_ARG, m_pTokenReader->GetPos());

					if (stOpt.size() && stOpt.top().GetCode()!=cmOPRT_INFIX)
						ApplyFunc(stOpt, stVal,stClassObj, iArgCount);
				} // if bracket content is evaluated
				break;	
			}
			//
			// Next are the binary operator entries
			//
		case cmAND:   // built in binary operators
		case cmOR:
		case cmXOR:
		case cmLT:
		case cmGT:
		case cmLE:
		case cmGE:
		case cmNEQ:
		case cmEQ:
		case cmADD:
		case cmSUB:
		case cmMUL:
		case cmDIV:
		case cmPOW:
		case cmASSIGN:
		case cmOPRT_BIN:// + - * /
			// A binary operator (user defined or built in) has been found. 
			while ( stOpt.size() && stOpt.top().GetCode() != cmBO)
			{
				if (GetOprtPri(stOpt.top()) < GetOprtPri(opt))
					break;
				if (stOpt.top().GetCode()==cmOPRT_INFIX)
					ApplyFunc(stOpt, stVal, stClassObj,1);  // infix operator
				else
					ApplyBinOprt(stOpt, stVal,stClassObj);
			} // while ( ... )

			// The operator can't be evaluated right now, push back to the operator stack
			stOpt.push(opt);
			break;
			//
			// Last section contains functions and operators implicitely mapped to functions
			//
		case cmBO:
			stArgCount.push(1); // we use this for counting the bracket level
			stOpt.push(opt);
			break;
		case cmFUNC:
		case cmFUNC_STR:
		case cmOPRT_INFIX:
			stOpt.push(opt);
			break;
		case cmOPRT_POSTFIX:
			stOpt.push(opt);
			ApplyFunc(stOpt, stVal, stClassObj,1);  // this is the postfix operator
			break;

		case cmLB:  ///< Operator item:  lift bracket  "{"
			if(1==igotoindex)
				icurstep=opt.GetGotoIndex();

			ioptid = stOpt.size();
			ioptid = ioptid >0? ioptid-1:0;
			stopt_storage.push(ioptid);
			ioptid = stVal.size();
			ioptid = ioptid >0? ioptid-1:0;
			stval_storage.push(ioptid);
			ioptid = stClassObj.size();
			ioptid = ioptid >0? ioptid-1:0;
			stClassObj_storage.push(ioptid);

			break;
		case cmRB: ///< Operator item:  right bracket  "}"
			icurstep=opt.GetGotoIndex()>-1?opt.GetGotoIndex():icurstep;

			ioptid = stopt_storage.top();
			for(int i=stOpt.size();i>ioptid;i--)
				stOpt.pop();

			ioptid = stopt_storage.top();
			for(int i=stVal.size();i>ioptid;i--)
				stVal.pop();

			ioptid = stClassObj_storage.top();
			for(int i=stClassObj.size();i>ioptid;i--)
				stClassObj.pop();

			stopt_storage.pop();
			stval_storage.pop();
			stClassObj_storage.pop();

			break;
		case cmIf:
		case cmWhile:
			stOpt.push(opt);
			break;		 
		case cmElse:
			//  [11/13/2012 cxy]
			if(1==igotoindex)//if 判断结论设置状态位 相反
			{
				igotoindex = 0;
			}
			else
			{
				igotoindex = 1;
			}
			break;
		case cmCallin:
			break;
		case cmClassObjDef:
			{
				pclass= opt.m_pClass;
				pobj=pclass->addvar(opt.GetAsString());//
				if(0!=pobj)
					opt.SetClassVarDef(pclass,pobj, opt.GetAsString() );
			}
			break;
		case cmClass:

			break;

		case cmClassObj:

			stClassObj.push(opt);
			//处理类对象成员变量操作 ACLASS aobjec.m_a+aobjec.m_b
			//处理类对象操作 ACLASS aobject bobject;fun(ACLASS ,ACLASS);>>ok
			//处理类成员函数调用  ACLASS aobject.fun(); >>ok
			break;


		case cmMember:
			stClassObj.pop();
			break;
		case cmPointer:

			break;
		case cmClassFuc:
			stOpt.push(opt);
			break;
			//cxyaddend
		default:  
			Error(ecINTERNAL_ERROR, 3);
		} // end of switch operator-token

		if( opt.GetCode() == cmEND )
			break;

	} 


	//Set Formula string
	//m_StringFormula=m_pTokenReader->GetFormula();


	if(0!=stVal.size())
	{
		if (stVal.top().GetType()!=tpDBL)
			Error(ecSTR_RESULT);
		// no error, so change the function pointer for the main parse routine
		value_type fVal = stVal.top().GetVal();   // Result from String parsing
		return fVal;
	}
	else
	{
		return 0;
	}
}
//////////////////////////////////////////////////////////////////////////
//Run class function
//cxvision function
//value_type  ParserBase::RunClassFuncCode(int * vFuncByteCode,int iFuncBytesize,stringbuf_type& strtab) const
//{
//	int isize= iFuncBytesize;
//	int iPtrSize = std::max( (int)sizeof(value_type*) / (int)sizeof(ParserByteCode::map_type), 1 );
//	int iValSize = sizeof(value_type)/sizeof(ParserByteCode::map_type);
//
//	if(isize<3)
//		return 0;
//	//
//	paramvect vArg;
//	voidparamvect voidpArg;
//	charpvect charpArg;
//
//
//	value_type Stack[99];
//	ECmdCode iCode;
//	bytecode_type idx(0);
//	int i(0);
//
//__start:
//
//	idx = vFuncByteCode[i];
//	iCode = (ECmdCode)vFuncByteCode[i+1];
//	i += 2;
//
//#ifdef _DEBUG
//	if (idx>=99)
//		throw exception_type(ecGENERIC, "", m_pTokenReader->GetFormula(), -1);
//#endif
//
//	switch (iCode)
//	{
//	// built in binary operators
//	case cmAND: Stack[idx]  = (int)Stack[idx] & (int)Stack[idx+1]; goto __start;
//	case cmOR:  Stack[idx]  = (int)Stack[idx] | (int)Stack[idx+1]; goto __start;
//	case cmXOR: Stack[idx]  = (int)Stack[idx] ^ (int)Stack[idx+1]; goto __start;
//	case cmLE:  Stack[idx]  = Stack[idx] <= Stack[idx+1]; goto __start;
//	case cmGE:  Stack[idx]  = Stack[idx] >= Stack[idx+1]; goto __start;
//	case cmNEQ: Stack[idx]  = Stack[idx] != Stack[idx+1]; goto __start;
//	case cmEQ:  Stack[idx]  = Stack[idx] == Stack[idx+1]; goto __start;
//	case cmLT:  Stack[idx]  = Stack[idx] < Stack[idx+1];  goto __start;
//	case cmGT:  Stack[idx]  = Stack[idx] > Stack[idx+1];  goto __start;
//	case cmADD: Stack[idx] += Stack[1+idx]; goto __start;
//	case cmSUB: Stack[idx] -= Stack[1+idx]; goto __start;
//	case cmMUL: Stack[idx] *= Stack[1+idx]; goto __start;
//	case cmDIV: Stack[idx] /= Stack[1+idx]; goto __start;
//	case cmPOW: Stack[idx]  = pow(Stack[idx], Stack[1+idx]); goto __start;
//
//		// Assignement needs special treatment
//	case cmASSIGN:
//		{
//			// next is a pointer to the target
//			value_type **pDest = (value_type**)(&vFuncByteCode[i]);
//
//			// advance index according to pointer size
//			i += iPtrSize;
//			// assign the value
//			Stack[idx] = **pDest = Stack[idx+1];
//		}
//		goto __start;
//
//		// user defined binary operators
//	case cmOPRT_BIN:
//		Stack[idx] = (**(fun_type2**)(&vFuncByteCode[i]))(Stack[idx], Stack[idx+1]);
//		++i;
//		goto __start;
//
//		// variable tokens
//	case cmVAR:
//		Stack[idx] = **(value_type**)(&vFuncByteCode[i]);
//		i += iValSize;
//		goto __start;
//
//		// value tokens
//	case cmVAL:
//		Stack[idx] = *(value_type*)(&vFuncByteCode[i]);
//		i += iValSize;
//		goto __start;
//
//		// Next is treatment of string functions
//	case cmFUNC_STR:
//		{
//			i++; // skip the unused argument count
//			strfun_type1 pFun = *(strfun_type1*)(&vFuncByteCode[i]);
//			i += iPtrSize;
//
//			int iIdxStack = (int)vFuncByteCode[i++];
//
//			MUP_ASSERT( iIdxStack>=0 && iIdxStack<(int)strtab.size() );
//			Stack[idx] = pFun(strtab[iIdxStack].c_str());
//		}
//		goto __start;
//
//		// Next is treatment of numeric functions
//	case cmFUNC:
//		{
//			int iArgCount = (int)vFuncByteCode[i++];
//			switch(iArgCount)  // switch according to argument count
//			{
//			case 1: Stack[idx] = (*(fun_type1*)(&vFuncByteCode[i]))(Stack[idx]); break;
//			case 2: Stack[idx] = (*(fun_type2*)(&vFuncByteCode[i]))(Stack[idx], Stack[idx+1]); break;
//			case 3: Stack[idx] = (*(fun_type3*)(&vFuncByteCode[i]))(Stack[idx], Stack[idx+1], Stack[idx+2]); break;
//			case 4: Stack[idx] = (*(fun_type4*)(&vFuncByteCode[i]))(Stack[idx], Stack[idx+1], Stack[idx+2], Stack[idx+3]); break;
//			case 5: Stack[idx] = (*(fun_type5*)(&vFuncByteCode[i]))(Stack[idx], Stack[idx+1], Stack[idx+2], Stack[idx+3], Stack[idx+4]); break;
//			default:
//				if (iArgCount>0) // function with variable arguments store the number as a negative value
//					Error(ecINTERNAL_ERROR, 1);
//				Stack[idx] =(*(multfun_type*)(&vFuncByteCode[i]))(&Stack[idx], -iArgCount);
//				break;
//			}
//			i += iPtrSize;
//		}
//		goto __start;
//	case cmClassFucNum:
//		{
//			int iArgCount = (int)vFuncByteCode[i++];
//			vArg.clear();
//			for (int inum=0; inum<iArgCount; ++inum)
//			{
//				vArg.push_back( Stack[idx+iArgCount-inum-1]);
//			}
//			classbase *pclass = (classbase  *)vFuncByteCode[i];
//			void *pobj = (void *)vFuncByteCode[i+1];
//			void *pfunc = ( void *)vFuncByteCode[i+2];
//			Stack[idx]= pclass->ApplyClassFunc(pobj,pfunc,vArg);
//			i = i + 4;
//
//		}
//		goto __start;
//	case cmClassFucStr:
//		{
//			//
//			int iArgCount = (int)vFuncByteCode[i++];
//			int iIdxStack;
//			charpArg.clear();
//			for (int inum=0; inum<iArgCount; ++inum)
//			{
//				iIdxStack = (int)vFuncByteCode[i+3];
//				charpArg.push_back(strtab[iIdxStack].c_str());
//			}
//			classbase *pclass = (classbase  *)vFuncByteCode[i];
//			void *pobj = (void *)vFuncByteCode[i+1];
//			void *pfunc = ( void *)vFuncByteCode[i+2];
//			Stack[idx]= pclass->ApplyClassFunc(pobj,pfunc,charpArg);
//			i = i + 3;
//
//		}
//		goto __start;
//	case cmClassFucVp:
//		{
//			//
//			int iArgCount = (int)vFuncByteCode[i++];
//			void * ptheobj;
//			voidpArg.clear();
//			for (int inum=0; inum<iArgCount; ++inum)
//			{
//				ptheobj = (void *)vFuncByteCode[i+3];
//				voidpArg.push_back(ptheobj);
//			}
//			classbase *pclass = (classbase  *)vFuncByteCode[i];
//			void *pobj = (void *)vFuncByteCode[i+1];
//			void *pfunc = ( void *)vFuncByteCode[i+2];
//			Stack[idx]= pclass->ApplyClassFunc(pobj,pfunc,voidpArg);
//			i = i + 5;
//
//		}
//		goto __start;
//	case cmEND:
//		return Stack[1];
//
//	default:
//		Error(ecINTERNAL_ERROR, 2);
//		return 0;
//	}
//}
//
//
//

//////////////////////////////////////////////////////////////////////////
//compile class declaration 
void ParserBase::CompileClassDeclara(const string_type &a_strdeclarastr,CreateClass *paclass)
{
	//m_pClassReader->SetFormula(a_strdeclarastr);
	//m_pcreateclass = paclass;
	////
	//m_pClassReader->ReInit();
	////ReInit();
	////ParseString();
	//m_pClassReader->SetCreateClass(m_pcreateclass);
	////////////////////////////////////////////////////////////////////////////
	////分析字串
	//token_type opt;
	//for(;;)
	//{
	//	opt = m_pClassReader->ReadNextToken();

	//	if( opt.GetCode() == cmEND )
	//	{
	//		break;
	//	}
	//}
	//m_pcreateclass = NULL;
}
//////////////////////////////////////////////////////////////////////////
//compile class function
//cxvision function
void ParserBase::CompileFuncAndRunString(const string_type &a_strfucstr,const string_type &a_strclass,const string_type &a_strobj)
{
//	m_pClassFunReader->ReInit();
//	m_pClassFunReader->SetFormula(a_strfucstr);
//	m_pClassFunReader->SetCreateClassObj(a_strclass,a_strobj);
//
//	//m_bprecompile = true;//预编译
//	//ParseString();
//	//////////////////////////////////////////////////////////////////////////
//	//分析字串且收集
//	//cxyaddBegin
// 
//	TokeStack optstack;
// 
//	//////////////////////////////////////////////////////////////////////////
//	//运行
//m_bcmd = false;
//	RunOptVect(optvector);
//m_bcmd = true;
}


#if defined(MUP_DUMP_STACK) | defined(MUP_DUMP_CMDCODE)

//------------------------------------------------------------------------------
/** \brief Dump stack content. 

    This function is used for debugging only.
*/
void ParserBase::StackDump( const ParserStack<token_type > &a_stVal, 
				             			  const ParserStack<token_type > &a_stOprt ) const
{
  using std::cout;
  ParserStack<token_type> stOprt(a_stOprt), 
                          stVal(a_stVal);

  cout << "\nValue stack:\r\n";
  while ( !stVal.empty() ) 
  {
    token_type val = stVal.pop();
    cout << " " << val.GetVal() << " ";
  }
  cout << "\nOperator stack:\r\n";

  while ( !stOprt.empty() )
  {
     if (stOprt.top().GetCode()<=cmASSIGN) 
  	 {
	  	 cout << "OPRT_INTRNL \"" 
            << ParserBase::c_DefaultOprt[stOprt.top().GetCode()] 
            << "\" \r\n";
	   }
     else
     {
		    switch(stOprt.top().GetCode())
		    {
		      case cmVAR:        cout << "VAR\r\n";  break;
		      case cmVAL:        cout << "VAL\r\n";  break;
		      case cmFUNC:       cout << "FUNC_NUM \"" 
                                  << stOprt.top().GetAsString() 
                                  << "\"\r\n";   break;
		      case cmOPRT_INFIX: cout << "OPRT_INFIX \"" 
                                  << stOprt.top().GetAsString() 
                                  << "\"\r\n";   break;
			  case cmOPRT_BIN:   cout << "OPRT_BIN \"" 
                                  << stOprt.top().GetAsString() 
                                  << "\"\r\n";        break;
              case cmFUNC_STR:   cout << "FUNC_STR\r\n";  break;
		      case cmEND:        cout << "END\r\n";       break;
		      case cmUNKNOWN:    cout << "UNKNOWN\r\n";   break;
		      case cmBO:         cout << "BRACKET \"(\"\r\n";  break;
		      case cmBC:         cout << "BRACKET \")\"\r\n";  break;
		      default:           cout << stOprt.top().GetType() << " ";  break;
		    }
     }
     stOprt.pop();
  }

  cout << dec << "\r\n";
}

#endif
void* ParserBase::GetClassObj(const string_type & strclass,const string_type & strobj)
{
	mu::classbasemap_type classmap = GetClassMap();
	classbase *pclass;
	if(!classmap.size())
		return NULL;

	classbasemap_type::const_iterator item = classmap.begin();
	for (; item!=classmap.end(); ++item)
	{
		if(strclass==item->first)
		{
			pclass=(item->second);

			for(int i=0;i<pclass->size();i++)
			{
				if(strobj == pclass->getvar(i))
					return pclass->getvarpoint(i);
			}
		}
	}
	return NULL;
}
void* ParserBase::GetClassObj(const string_type &  strclass,int iobjnum)
{
	mu::classbasemap_type classmap = GetClassMap();
	classbase *pclass;
	if(!classmap.size())
		return 0;

	classbasemap_type::const_iterator item = classmap.begin();
	for (; item!=classmap.end(); ++item)
	{
		if(strclass==item->first)
		{
			pclass=(item->second);

			if(iobjnum>=pclass->size())
				return 0;
			for(int i=0;i<pclass->size();i++)
			{
				return pclass->getvarpoint(iobjnum);
			}
		}
	}
	return NULL;
}
int ParserBase::GetClassObjSum(const string_type &  strclass)
{
	mu::classbasemap_type classmap = GetClassMap();
	classbase *pclass;
	if(!classmap.size())
		return 0;

	classbasemap_type::const_iterator item = classmap.begin();
	for (; item!=classmap.end(); ++item)
	{
		if(strclass==item->first)
		{
			pclass=(item->second);

			return pclass->size();
		}
	}
	return 0;
}
} // namespace mu
