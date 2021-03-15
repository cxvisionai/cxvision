 /*
  Copyright (C) 2005 Ingo Berg

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
#include <cassert>
#include <cstdio>
#include <cstring>
#include <map>
#include <stack>
#include <string>

#include "muParserTokenReader.h"
//////////////////////////////////////////////////////////////////////////

#include "muParserBase.h"


namespace mu
{
  // Forward declaration
  class ParserBase;
  //---------------------------------------------------------------------------
  /** \brief Copy constructor.

      \sa Assign
      \throw nothrow
  */
  ParserTokenReader::ParserTokenReader(const ParserTokenReader &a_Reader) 
  {
    Assign(a_Reader);
  }
  //---------------------------------------------------------------------------
  /** \brief Assignement operator.

      Self assignement will be suppressed otherwise #Assign is called.

      \param a_Reader Object to copy to this token reader.
      \throw nothrow
  */
  ParserTokenReader& ParserTokenReader::operator=(const ParserTokenReader &a_Reader) 
  {
    if (&a_Reader!=this)
      Assign(a_Reader);

    return *this;
  }

  //---------------------------------------------------------------------------
  /** \brief Assign state of a token reader to this token reader. 
      赋值
      \param a_Reader Object from which the state should be copied.
      \throw nothrow
  */
  void ParserTokenReader::Assign(const ParserTokenReader &a_Reader)
  {
    m_pParser = a_Reader.m_pParser;
    m_strFormula = a_Reader.m_strFormula;
    m_iPos = a_Reader.m_iPos;
    m_iSynFlags = a_Reader.m_iSynFlags;
    
    m_UsedVar = a_Reader.m_UsedVar;
    m_pFunDef = a_Reader.m_pFunDef;
    m_pConstDef = a_Reader.m_pConstDef;
    m_pVarDef = a_Reader.m_pVarDef;
    m_pStrVarDef = a_Reader.m_pStrVarDef;
	m_pStringVarDef = a_Reader.m_pStringVarDef;
    m_pPostOprtDef = a_Reader.m_pPostOprtDef;
    m_pInfixOprtDef = a_Reader.m_pInfixOprtDef;
    m_pOprtDef = a_Reader.m_pOprtDef;
    m_bIgnoreUndefVar = a_Reader.m_bIgnoreUndefVar;
    m_vIdentFun = a_Reader.m_vIdentFun;
    m_pFactory = a_Reader.m_pFactory;
    m_iBrackets = a_Reader.m_iBrackets;

	m_iBigBrackets =a_Reader.m_iBigBrackets;
	m_pClassDef=a_Reader.m_pClassDef;
	m_bClassDef=a_Reader.m_bClassDef;
	m_iPreFlags=a_Reader.m_iPreFlags;

	m_icurID = a_Reader.m_icurID;
  }

  //---------------------------------------------------------------------------
  /** \brief Constructor. 
      
      Create a Token reader and bind it to a parser object. 

      \pre [assert] a_pParser may not be NULL
      \post #m_pParser==a_pParser
      \param a_pParent Parent parser object of the token reader.
  */
  ParserTokenReader::ParserTokenReader(ParserBase *a_pParent)
    :m_pParser(a_pParent)
    ,m_strFormula()
    ,m_iPos(0)
    ,m_iSynFlags(0)
    ,m_bIgnoreUndefVar(false)
    ,m_pFunDef(0)
    ,m_pPostOprtDef(0)
    ,m_pInfixOprtDef(0)
    ,m_pOprtDef(0)
    ,m_pConstDef(0)
    ,m_pStrVarDef(0)
    ,m_pVarDef(0)
    ,m_pFactory(0)
    ,m_vIdentFun()
    ,m_UsedVar()
    ,m_fZero(0)
    ,m_iBrackets(0)
	,m_iBigBrackets(0)
	,m_pClass(0)
	,m_pClassDef(0)
	,m_pClassBase(0)
	,m_pClassObj(0)
	,m_bClassDef(false)
	,m_iPreFlags(0)
	,m_pStringVarDef(0)

  {
    assert(m_pParser);
    SetParent(m_pParser);
  }    
  //---------------------------------------------------------------------------
  /** \brief Destructor (trivial).
  
      \throw nothrow
  */
  ParserTokenReader::~ParserTokenReader()
  {}

  //---------------------------------------------------------------------------
  /** \brief Create instance of a ParserTokenReader identical with this 
              and return its pointer. 

      This is a factory method the calling function must take care of the object destruction.

      \return A new ParserTokenReader object.
      \throw nothrow
  */
  ParserTokenReader* ParserTokenReader::Clone(ParserBase *a_pParent) const
  {
    std::auto_ptr<ParserTokenReader> ptr(new ParserTokenReader(*this));
    ptr->SetParent(a_pParent);
    return ptr.release();
  }

  //---------------------------------------------------------------------------
  void ParserTokenReader::AddValIdent(identfun_type a_pCallback)
  {
    m_vIdentFun.push_back(a_pCallback);
  }

  //---------------------------------------------------------------------------
  void ParserTokenReader::SetVarCreator(facfun_type a_pFactory)
  {
    m_pFactory = a_pFactory;
  }
  //cxyadd---------------------------------------------------------------------
  void ParserTokenReader::SetClassUing(void * a_pVoid)
  {
	  m_pClass=a_pVoid;
  }
  //cxyaddend------------------------------------------------------------------
  //---------------------------------------------------------------------------
  /** \brief Return the current position of the token reader in the formula string. 

      \return #m_iPos
      \throw nothrow
  */
  int ParserTokenReader::GetPos() const
  {
    return m_iPos;
  }

  //---------------------------------------------------------------------------
  /** \brief Return a reference to the formula. 

      \return #m_strFormula
      \throw nothrow
  */
  const string_type& ParserTokenReader::GetFormula() const
  {
    return m_strFormula;
  }

  //---------------------------------------------------------------------------
  /** \brief Return a map containing the used variables only. */
  const varmap_type& ParserTokenReader::GetUsedVar() const
  {
    return m_UsedVar;
  }

  //---------------------------------------------------------------------------
  /** \brief Initialize the token Reader. 
  
      Sets the formula position index to zero and set Syntax flags to default for initial formula parsing.
      \pre [assert] triggered if a_szFormula==0
  */
  void ParserTokenReader::SetFormula(const string_type &a_strFormula)
  {
    m_strFormula = a_strFormula;
    ReInit();
  }

  //---------------------------------------------------------------------------
  void ParserTokenReader::SetDefs( const funmap_type *a_pFunDef, 
	  const funmap_type *a_pOprtDef,
                                   const funmap_type *a_pInfixOprtDef,
                                   const funmap_type *a_pPostOprtDef,
                                   varmap_type *a_pVarDef,
                                   const strmap_type *a_pStrVarDef,
                                   const valmap_type *a_pConstDef)
  {
    m_pFunDef = a_pFunDef;
    m_pOprtDef = a_pOprtDef;
    m_pInfixOprtDef = a_pInfixOprtDef;
    m_pPostOprtDef = a_pPostOprtDef;
    m_pVarDef = a_pVarDef;
    m_pStrVarDef = a_pStrVarDef;
    m_pConstDef = a_pConstDef;
  }

  //---------------------------------------------------------------------------
  /** \brief Set Flag that contronls behaviour in case of undefined variables beeing found. 
  
    If true, the parser does not throw an exception if an undefined variable is found. 
    otherwise it does. This variable is used internally only!
    It supresses a "undefined variable" exception in GetUsedVar().  
    Those function should return a complete list of variables including 
    those the are not defined by the time of it's call.
  */
  void ParserTokenReader::IgnoreUndefVar(bool bIgnore)
  {
    m_bIgnoreUndefVar = bIgnore;
  }

  //----------------------------------------------------------------------------------
  //使用类型判断状态
  void ParserTokenReader::UsingClassDef(bool busing)
  {
	  m_bClassDef=busing;
  }
  //---------------------------------------------------------------------------
  /** \brief Reset the token reader to the start of the formula. 

      The syntax flags will be reset to a value appropriate for the 
      start of a formula.
      \post #m_iPos==0, #m_iSynFlags = noOPT | noBC | noPOSTOP | noSTR
      \throw nothrow
      \sa ESynCodes
  */
  void ParserTokenReader::ReInit()
  {
    m_iPos = 0;
    m_iSynFlags = noOPT | noBC | noPOSTOP | noASSIGN | noRB  | noClassObjDef;
    m_iBrackets = 0;
	m_iBigBrackets =0;
	m_iPreFlags=0;
    m_UsedVar.clear();
  }



  void ParserTokenReader::EndExpress()
  {
    m_iSynFlags = noOPT | noBC | noPOSTOP | noASSIGN | noRB | noClassObjDef;
    m_iBrackets = 0;
	m_iPreFlags = isANY;
    m_UsedVar.clear();
  }
  //---------------------------------------------------------------------------
  /** \brief Read the next token from the string. */ 
 
ParserTokenReader::token_type ParserTokenReader::ReadNextToken()//int &irunstep)
{
 
m_icurID = 0;
    assert(m_pParser);
    std::stack<int> FunArgs;
    const char_type *szFormula = m_strFormula.c_str();
	int icharsize = m_strFormula.size();
    token_type tok; 

	if( IsComment(tok)) ; //remove null char and comment

	if ( IsEOF(tok) ) 
		return tok;    // Check for end of formula //cxyexplain: meet ""
	if ( IsOprt(tok) ) 
		return tok;		// Check for user defined binary operator//cxyexplain: meet sin cos...
	if( IsBuiltInCoditionTok(tok))
		return tok;
	if ( IsBuiltIn(tok) ) 
		return tok;		// Check built in operators / tokens{ , } = ,
    if ( IsFunTok(tok) )
		return tok;     // Check for function token
    if ( IsValTok(tok) )
		return tok;     // Check for values / constant tokens '1','2'.... 
    if ( IsVarTok(tok) )
		return tok;     // Check for variable tokens 'a','b','c'.....
	if ( IsStrVarTok(tok) )
		return tok;		// Check for string variables
    if ( IsString(tok) )
		return tok;     // Check for String tokens
    if ( IsInfixOpTok(tok) )
		return tok;		// Check for unary operators
    if ( IsPostOpTok(tok) )
		return tok;		// Check for unary operators
 
    // Check String for undefined variable token. Done only if a 
    // flag is set indicating to ignore undefined variables.
    // This is a way to conditionally avoid an error if 
    // undefined variables occur. 
    // The GetUsedVar function must supress the error for
    // undefined variables in order to collect all variable 
    // names including the undefined ones.	
	//cxyadd ver1.0
	//cxyadd
	//
	//if (m_bClassDef&&IsIfWhileok(tok))
	//	return tok;
	if ( m_bClassDef&&IsClassDefTok(tok))//类定义
		return tok;		//Check for class type is / int, double, myclassdef
	if ( m_bClassDef&&IsClassObjDefTok(tok))//类对象声明
		return tok;     // check for class value /  a (int a, double a)
	if ( m_bClassDef&&IsClassObjTok(tok))//类对象声明
		return tok;     // check for class value /  a (int a, double a)
	if ( m_bClassDef&&IsClassFucTok(tok))//类成员函数声明
		return tok;
	//cxyaddend
	//cxyadd end
    if ( (m_bIgnoreUndefVar || m_pFactory) && IsUndefVarTok(tok) )  
		return tok;

    // Check for unknown token
    // 
    // !!! From this point on there is no exit without an exception possible...
    // 
    string_type strTok;
    int iEnd = ExtractToken(m_pParser->ValidNameChars(), strTok, m_iPos);
    if (iEnd!=m_iPos)
      Error(ecUNASSIGNABLE_TOKEN, m_iPos, strTok);
    Error(ecUNASSIGNABLE_TOKEN, m_iPos, m_strFormula.substr(m_iPos));
    return token_type(); // never reached
}

  //---------------------------------------------------------------------------
  void ParserTokenReader::SetParent(ParserBase *a_pParent)
  {
    m_pParser  = a_pParent; 
    m_pFunDef  = &a_pParent->m_FunDef;
    m_pOprtDef = &a_pParent->m_OprtDef;
    m_pInfixOprtDef = &a_pParent->m_InfixOprtDef;
    m_pPostOprtDef  = &a_pParent->m_PostOprtDef;
    m_pVarDef       = &a_pParent->m_VarDef;
    m_pStrVarDef    = &a_pParent->m_StrVarDef;
    m_pConstDef     = &a_pParent->m_ConstDef;
	//cxyadd ver1.0
	m_pClassDef     = &a_pParent->m_ClassDefMap;
	m_pStringVarDef = &a_pParent->m_StringVarDef;
	//cxyadd end
  }

  //---------------------------------------------------------------------------
  /** \brief Extract all characters that belong to a certain charset.

    \param a_szCharSet [in] Const char array of the characters allowed in the token. 
    \param a_strTok [out]  The string that consists entirely of characters listed in a_szCharSet.
    \param a_iPos [in] Position in the string from where to start reading.
    \return The Position of the first character not listed in a_szCharSet.
    \throw nothrow
  */
  int ParserTokenReader::ExtractToken( const char_type *a_szCharSet, 
                                       string_type &a_sTok, int a_iPos ) const
  {
	//return end position and first
	int iEnd = (int)m_strFormula.find_first_not_of(a_szCharSet, a_iPos);

    if (iEnd==(int)string_type::npos)
        iEnd = (int)m_strFormula.length();

    a_sTok = string_type( m_strFormula.begin()+a_iPos, m_strFormula.begin()+iEnd);
    a_iPos = iEnd;
    return iEnd;
  }

  //---------------------------------------------------------------------------
  //  [10/17/2012 cxy]
  bool ParserTokenReader::IsComment(token_type &a_Tok)
  {
	 //command//代码注释去除“/* adadf*/”
	const char_type* szFormula = m_strFormula.c_str();
	int icharsize = m_strFormula.size();
	if(szFormula[m_iPos]=='/')
	{
	  if(szFormula[m_iPos+1]=='*')
	  {
		  ++m_iPos;
		  ++m_iPos;
		  while(szFormula[m_iPos]!='*')
		  {
			  ++m_iPos;
			  if(m_iPos>=icharsize-1)
				  break;
		  }
		  if(szFormula[m_iPos]=='*')
			  if(szFormula[m_iPos+1]=='/')
			  {
				  ++m_iPos;
				  ++m_iPos;
			  }
	  }
	  if(szFormula[m_iPos+1]=='/')
	  {
		  ++m_iPos;
		  ++m_iPos;
		  while(szFormula[m_iPos]!='\r'||szFormula[m_iPos]!='\n')
		  {
			  ++m_iPos;
			  if(m_iPos>=icharsize-1)
				  break;
		  }
	  }


	}
	//////////////////////////////////////////////////////////////////////////
	//remove unvalid char
	  while (szFormula[m_iPos]==' '
		  ||szFormula[m_iPos]==0x09//TAB
		  ||szFormula[m_iPos]=='\r'
		  ||szFormula[m_iPos]=='\n')// remove null char
		  ++m_iPos;

	  return false;
  }

  //---------------------------------------------------------------------------
  /** \brief Check whether the token at a given position is a function token.
  \param a_Tok [out] If a value token is found it will be placed here.
  \throw ParserException if Syntaxflags do not allow a function at a_iPos
  \return true if a function token has been found false otherwise.
  \pre [assert] m_pParser!=0
  */
  //  [10/17/2012 cxy]
  bool ParserTokenReader::IsBuiltInCoditionTok(token_type &a_Tok)
  {
	  string_type strTok;
	  int iEnd = ExtractToken(m_pParser->ValidNameChars(), strTok, m_iPos);
	  if (iEnd==m_iPos)
		  return false;

	  const char_type **const pOprt = m_pParser->GetOprtDef();
	  assert(pOprt);

	  // Check if the next sign is an opening bracket
	  const char_type *szExpr = m_strFormula.c_str();

	  // Check for intrinsic functions
	  ECmdCode eFunTok = cmUNKNOWN;
	  if(strTok==string_type(pOprt[cmIf]))
	  {
		  eFunTok = (ECmdCode)cmIf;
		  if (eFunTok==cmUNKNOWN)
			  return false;
		  if (szExpr[iEnd]!='(')
			  return false;

		  a_Tok.Set(eFunTok, strTok);

		  
		  m_iPos = (int)iEnd;
		  if (m_iSynFlags & noFUN)
			  Error(ecUNEXPECTED_FUN, m_iPos-(int)a_Tok.GetAsString().length(), a_Tok.GetAsString());

		  m_iSynFlags = noANY ^ noBO;
 
		  return true;
	  }
	  if(strTok==string_type(pOprt[cmWhile]))
	  {
		  eFunTok = (ECmdCode)cmWhile;
		  if (eFunTok==cmUNKNOWN)
			  return false;
		  if (szExpr[iEnd]!='(')
			  return false;

		  a_Tok.Set(eFunTok, strTok);

		  m_iPos = (int)iEnd;
		  if (m_iSynFlags & noFUN)
			  Error(ecUNEXPECTED_FUN, m_iPos-(int)a_Tok.GetAsString().length(), a_Tok.GetAsString());

		  m_iSynFlags = noANY ^ noBO;

	 
		  return true;
	  }

	  if(strTok==string_type(pOprt[cmElse]))
		{
		  eFunTok = (ECmdCode)cmElse;
		  if (eFunTok==cmUNKNOWN)
			  return false;
		  if (szExpr[iEnd]!='{')
			  return false;

		  a_Tok.Set(eFunTok, strTok);

		  m_iPos = (int)iEnd;
		  if (m_iSynFlags & noFUN)
			  Error(ecUNEXPECTED_FUN, m_iPos-(int)a_Tok.GetAsString().length(), a_Tok.GetAsString());

		  m_iSynFlags = noANY ^ noLB;

		  return true;
	  }
	  return false;
  }



  //---------------------------------------------------------------------------
  /** \brief Check if a built in operator or other token can be found
      \param a_Tok  [out] Operator token if one is found. This can either be a binary operator or an infix operator token.
      \return true if an operator token has been found.
  */
  bool ParserTokenReader::IsBuiltIn(token_type &a_Tok)
  {

	  const char_type **pOprtDef = m_pParser->GetOprtDef();
	  /*
	  "<=", ">=", "!=", "==", "<", ">", "+", "-", "*", "/", 
	  "^", "and", "or", "xor", "=", "(", ")", ",", 0 
	  */

	  const char_type* szFormula = m_strFormula.c_str();

    // Compare token with function and operator strings
    // check string for operator/function
    for (int i=0; pOprtDef[i]; i++)
    {
      std::size_t len = std::strlen( pOprtDef[i] );

      if (!std::strncmp(&szFormula[m_iPos], pOprtDef[i], len))
	    {
			switch(i)
			{
				/*
				"<=", ">=", "!=", "==", "<", ">", "+", "-", "*", "/", 
				"^", "and", "or", "xor", "=", "(", ")", ",",";","{","}",".","->"
				*/
				case cmAND:
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
				case cmMUL:// "*" and "cmPoint"
					//cxy add:
					if(isClassDef==m_iPreFlags)
					{
						m_iPreFlags=isClassObjDef;
						m_iSynFlags=noANY ^ ( noClassObjDef );
						break;
					}

				case cmDIV:
				case cmPOW:
				case cmASSIGN: //"="

				  // The assignement operator need special treatment]
				  if (i==cmASSIGN && m_iSynFlags & noASSIGN)
					Error(ecUNEXPECTED_OPERATOR, m_iPos, pOprtDef[i]);
				  if (!m_pParser->HasBuiltInOprt()) continue;
				  if (m_iSynFlags & noOPT) 
				  {
						// Maybe its an infix operator not an operator
						// Both operator types can share characters in 
						// their identifiers
						if ( IsInfixOpTok(a_Tok) ) 
						  return true;

						Error(ecUNEXPECTED_OPERATOR, m_iPos, pOprtDef[i]);
				  }
				  m_iPreFlags  = isASSIGN;
				  m_iSynFlags  = noBC | noOPT | noCOMMA | noPOSTOP | noASSIGN | noLB | noRB;
				  m_iSynFlags |= ( (i != cmEND) && ( i != cmBC) ) ? noEND : 0;
				  break;

				case cmCOMMA://","
						  if (m_iSynFlags & noCOMMA)
							  Error(ecUNEXPECTED_COMMA, m_iPos, pOprtDef[i]);

						  m_iPreFlags  = isCOMMA;
						  m_iSynFlags  = noBC | noOPT | noEND | noCOMMA | noPOSTOP | noASSIGN | noLB | noRB;
						break;

				case cmBO://"("
						  if (m_iSynFlags & noBO)
							  Error(ecUNEXPECTED_PARENS, m_iPos, pOprtDef[i]);

						  m_iPreFlags =  isBO;
						  m_iSynFlags =  noOPT | noEND | noCOMMA | noPOSTOP | noASSIGN | noLB | noRB;//noBC |
						++m_iBrackets;
						  break;

				case cmBC://")"
					if (m_iSynFlags & noBC)
						Error(ecUNEXPECTED_PARENS, m_iPos, pOprtDef[i]);
					
					m_iPreFlags  = isBC;
					m_iSynFlags  = noBO | noVAR | noVAL | noFUN | noINFIXOP | noSTR | noASSIGN | noRB;

					if (--m_iBrackets<0)
						Error(ecUNEXPECTED_PARENS, m_iPos, pOprtDef[i]);
					break;

				case cmLB://"{"
					if (m_iSynFlags & noLB)
						Error(ecMISSING_BIGBRACKET, m_iPos, pOprtDef[i]);

					m_iPreFlags =  isLB;
					m_iSynFlags =  noBC | noOPT | noEND | noCOMMA | noPOSTOP | noASSIGN | noRB | noSEMIC;
						
					++m_iBigBrackets;

					//m_prebracketstorage.push(m_icurID+1);

					break;

				case cmRB://"}"
					if (m_iSynFlags & noRB)
						Error(ecMISSING_BIGBRACKET, m_iPos, pOprtDef[i]);

					m_iPreFlags  = isRB;
					m_iSynFlags  = noINFIXOP | noASSIGN ;

					 
					//m_bacbracketstorage.push(m_icurID+1);//  [10/28/2012 cxy]
					

					if (--m_iBigBrackets<0)
						Error(ecMISSING_BIGBRACKET, m_iPos, pOprtDef[i]);

					break;
				case cmSEMICOLON:
					if (m_iSynFlags & noSEMIC)
						Error(ecMISSING_BIGBRACKET, m_iPos, pOprtDef[i]);
					
					m_iPreFlags = isSEMIC;
					m_iSynFlags  = noBO | noINFIXOP | noSTR | noASSIGN ;

					break;
				case cmMember:
					if (m_iSynFlags & noClassMemOp)
						Error(ecCLASSMEMOPT_CONFICT, m_iPos, pOprtDef[i]);

					m_iPreFlags = isClassMemOp;
					m_iSynFlags  =  noANY ^ ( noClassMemVar | noClassMemFuc );//只能是成员函数或成员变量 ;

					break;
				case cmPointer:
					if (m_iSynFlags & noClassPointOp)
						Error(ecCLASSPOINT_CONFICT, m_iPos, pOprtDef[i]);
					
					m_iPreFlags = isClassPointOp;
					m_iSynFlags  =  noANY ^ ( noClassMemVar | noClassMemFuc );//只能是成员函数或成员变量 ;

					break;
				default:      // The operator is listed in c_DefaultOprt, but not here. This is a bad thing...
				  Error(ecINTERNAL_ERROR);
			} // switch operator id
		
			
			m_iPos += (int)len;
			a_Tok.Set( (ECmdCode)i, pOprtDef[i] );
			return true;
	    } // if operator string found
    } // end of for all operator strings
  
    return false;
  }

  //---------------------------------------------------------------------------
  /** \brief Check for End of Formula.

      \return true if an end of formula is found false otherwise.
      \param a_Tok [out] If an eof is found the corresponding token will be stored there.
      \throw nothrow
      \sa IsOprt, IsFunTok, IsStrFunTok, IsValTok, IsVarTok, IsString, IsInfixOpTok, IsPostOpTok
  */
  bool ParserTokenReader::IsEOF(token_type &a_Tok)
  {
    const char_type* szFormula = m_strFormula.c_str();

    // check for EOF
    if ( !szFormula[m_iPos] || szFormula[m_iPos] == '\n')
    {
      if ( m_iSynFlags & noEND )
        Error(ecUNEXPECTED_EOF, m_iPos);

      if (m_iBrackets>0)
        Error(ecMISSING_PARENS, m_iPos, _T(")"));

	  if(m_iBigBrackets>0)
		  Error(ecMISSING_BIGBRACKET,m_iPos,_T("}"));

      m_iSynFlags = 0;
      a_Tok.Set(cmEND);//cxyexplain:set express end


      return true;
    }

//cx2017 02 05 chage 
// find return;
	string_type strTok;
	//Get function name and first position and end position
	int iEnd = ExtractToken(m_pParser->ValidNameChars(), strTok, m_iPos);
	if (iEnd==m_iPos)
		return false;

    if(strTok==string_type("return"))
	{
		m_iSynFlags = 0;
		a_Tok.Set(cmEND);//cxyexplain:set express end
	} 
	else
		return false;

	m_iPos = (int)iEnd;
	if (m_iSynFlags & noFUN)
		Error(ecUNEXPECTED_FUN, m_iPos-(int)a_Tok.GetAsString().length(), a_Tok.GetAsString());
 
	m_iSynFlags = noBO | noBC | noVAL | noVAR | noCOMMA | noFUN | noOPT 
	| noPOSTOP | noINFIXOP | noEND | noSTR
	| noASSIGN | noLB | noRB | noClassDef
	| noClassObjDef | noClassMemOp | noClassPointOp | noClassMemVar
	| noClassMemFuc | noClassObj  ;//only ";"
 
	m_iPreFlags = isEND;
  
    return true;



    return false;
  }
  ///////////////////////////////////////cxyadd end/////////////////////////////////

  //---------------------------------------------------------------------------
  /** \brief Check if a string position contains a unary infix operator. 
      \return true if a function token has been found false otherwise.
  */
  bool ParserTokenReader::IsInfixOpTok(token_type &a_Tok)
  {
    string_type sTok;
    int iEnd = ExtractToken(m_pParser->ValidInfixOprtChars(), sTok, m_iPos);
    if (iEnd==m_iPos)
      return false;

    funmap_type::const_iterator item = m_pInfixOprtDef->find(sTok);
    if (item==m_pInfixOprtDef->end())
      return false;

    a_Tok.Set(item->second, sTok);
    m_iPos = (int)iEnd;

    if (m_iSynFlags & noINFIXOP) 
      Error(ecUNEXPECTED_OPERATOR, m_iPos, a_Tok.GetAsString());

    m_iSynFlags = noPOSTOP | noINFIXOP | noOPT | noBC | noSTR | noASSIGN | noClassDef; 
    return true;
  }

  //---------------------------------------------------------------------------
  /** \brief Check whether the token at a given position is a function token.
      \param a_Tok [out] If a value token is found it will be placed here.
      \throw ParserException if Syntaxflags do not allow a function at a_iPos
      \return true if a function token has been found false otherwise.
      \pre [assert] m_pParser!=0
  */
  bool ParserTokenReader::IsFunTok(token_type &a_Tok)
  {
    string_type strTok;
	//Get function name and first position and end position
    int iEnd = ExtractToken(m_pParser->ValidNameChars(), strTok, m_iPos);
    if (iEnd==m_iPos)
      return false;

    funmap_type::const_iterator item = m_pFunDef->find(strTok);
    if (item==m_pFunDef->end())
        return false;

    a_Tok.Set(item->second, strTok);

    m_iPos = (int)iEnd;
    if (m_iSynFlags & noFUN)
      Error(ecUNEXPECTED_FUN, m_iPos-(int)a_Tok.GetAsString().length(), a_Tok.GetAsString());

    m_iSynFlags = noANY ^ noBO;//只能是BO "("
	m_iPreFlags = isFUN;
    return true;
  }

  //---------------------------------------------------------------------------
  /** \brief Check if a string position contains a unary post value operator. */
  bool ParserTokenReader::IsPostOpTok(token_type &a_Tok)
  {
    // Tricky problem with equations like "3m+5":
    //     m is a postfix operator, + is a valid sign for postfix operators and 
    //     for binary operators parser detects "m+" as operator string and 
    //     finds no matching postfix operator.
    // 
    // This is a special case so this routine slightly differs from the other
    // token readers.
    
    // Test if there could be a postfix operator
    string_type sTok;
    int iEnd = ExtractToken(m_pParser->ValidOprtChars(), sTok, m_iPos);
    if (iEnd==m_iPos)
      return false;

    // iteraterate over all postfix operator strings
    funmap_type::const_iterator item = m_pPostOprtDef->begin();
    for (item=m_pPostOprtDef->begin(); item!=m_pPostOprtDef->end(); ++item)
    {
      if (sTok.find(item->first)!=0)
        continue;

      a_Tok.Set(item->second, sTok);
  	  m_iPos += (int)item->first.length();

      if (m_iSynFlags & noPOSTOP)
        Error(ecUNEXPECTED_OPERATOR, m_iPos-(int)a_Tok.GetAsString().length(), a_Tok.GetAsString());

      m_iSynFlags = noVAL | noVAR | noFUN | noBO | noPOSTOP | noSTR | noASSIGN | noClassDef;
      return true;
    }

    return false;
  }

  //---------------------------------------------------------------------------
  /** \brief Check if a string position contains a binary operator.
      \param a_Tok  [out] Operator token if one is found. This can either be a binary operator or an infix operator token.
      \return true if an operator token has been found.
  */
  bool ParserTokenReader::IsOprt(token_type &a_Tok)
  {
    //get first point of string
	/*  Define Oprt Chars "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                  "+-*^/?<>=#!$%&|~'_" */
	const char_type *szFormula = m_strFormula.c_str();
    int iVarEnd = (int)strspn(&szFormula[m_iPos], m_pParser->ValidOprtChars());
    if (!iVarEnd) return false;
    //get iVarEnd sum Formulat string
    string_type strOprt(&szFormula[m_iPos], &szFormula[m_iPos+iVarEnd]);

    funmap_type::const_iterator item = m_pOprtDef->find(strOprt);
    if (item==m_pOprtDef->end())
      return false;

    a_Tok.Set(item->second, strOprt);

    if (m_iSynFlags & noOPT) 
    {
      // An operator was found but is not expected to occur at
      // this position of the formula, maybe it is an infix 
      // operator, not a binary operator. Both operator types
      // can share characters in their identifiers.
      if ( IsInfixOpTok(a_Tok) ) return true;
      // nope, no infix operator
      Error(ecUNEXPECTED_OPERATOR, m_iPos, a_Tok.GetAsString()); 
    }

	m_iPos += iVarEnd;
    m_iSynFlags  = noBC | noOPT | noCOMMA | noPOSTOP | noEND | noBC | noASSIGN | noClassDef;
    return true;
  }

  //---------------------------------------------------------------------------
  /** \brief Check whether the token at a given position is a value token.

    Value tokens are either values or constants.

    \param a_Tok [out] If a value token is found it will be placed here.
    \return true if a value token has been found.
  */
  bool ParserTokenReader::IsValTok(token_type &a_Tok)
  {
    assert(m_pConstDef);
    assert(m_pParser);

    #if defined(_MSC_VER)
      #pragma warning( disable : 4244 )
    #endif

    string_type strTok;
    value_type fVal(0);
    int iEnd(0);
    
    // 2.) Check for user defined constant
    // Read everything that could be a constant name
    iEnd = ExtractToken(m_pParser->ValidNameChars(), strTok, m_iPos);
    if (iEnd!=m_iPos)
    {
		  valmap_type::const_iterator item = m_pConstDef->find(strTok);
		  if (item!=m_pConstDef->end())
		  {
			m_iPos = iEnd;
			a_Tok.SetVal(item->second, strTok);

			if (m_iSynFlags & noVAL)
			  Error(ecUNEXPECTED_VAL, m_iPos - (int)strTok.length(), strTok);

			m_iSynFlags = noVAL | noVAR | noFUN | noBO | noINFIXOP | noSTR | noASSIGN | noClassDef |noClassObjDef |noClassObj; 
			return true;
		  }
    }

    // 3.call the value recognition functions provided by the user
    // Call user defined value recognition functions
    std::vector<identfun_type>::const_iterator item = m_vIdentFun.begin();
    for (item = m_vIdentFun.begin(); item!=m_vIdentFun.end(); ++item)
    {
      int iStart = m_iPos;
      if ( (*item)(m_strFormula.c_str() + m_iPos, m_iPos, fVal) )
      {
        strTok.assign(m_strFormula.c_str(), iStart, m_iPos);
        if (m_iSynFlags & noVAL)
          Error(ecUNEXPECTED_VAL, m_iPos - (int)strTok.length(), strTok);

        a_Tok.SetVal(fVal, strTok);
        m_iSynFlags = noVAL | noVAR | noFUN | noBO | noINFIXOP | noSTR | noASSIGN;
        return true;
      }
    }

    return false;

    #if defined(_MSC_VER)
      #pragma warning( default : 4244 )
    #endif
  }

  //---------------------------------------------------------------------------
  /** \brief Check wheter a token at a given position is a variable token. 
      \param a_Tok [out] If a variable token has been found it will be placed here.
	    \return true if a variable token has been found.
  */
  bool ParserTokenReader::IsVarTok(token_type &a_Tok)
  {
    if (!m_pVarDef->size())
      return false;

    string_type strTok;
    int iEnd = ExtractToken(m_pParser->ValidNameChars(), strTok, m_iPos);
    if (iEnd==m_iPos)
      return false;

    varmap_type::const_iterator item =  m_pVarDef->find(strTok);
    if (item==m_pVarDef->end())
      return false;

    if (m_iSynFlags & noVAR)
      Error(ecUNEXPECTED_VAR, m_iPos, strTok);

    m_iPos = iEnd;
    a_Tok.SetVar(item->second, strTok);
    m_UsedVar[item->first] = item->second;  // Add variable to used-var-list

    m_iSynFlags = noVAL | noVAR | noFUN | noBO | noPOSTOP | noINFIXOP | noSTR 
		| noClassDef | noClassObjDef | noClassObj;
    return true;
  }


  //cxyadd--------------------------------------------------------------------------
  //---------------------------------------------------------------------------
  /** \brief Check wheter a token at a given position is the front class undefined variable. 

  \param a_Tok [out] If a variable tom_pParser->m_vStringBufken has been found it will be placed here.
  \return true if a variable token has been found.
  \throw nothrow
  //申明类变量
  */
  bool ParserTokenReader::IsClassObjDefTok(token_type &a_Tok)
  {
	  if(isClassDef!=m_iPreFlags
		  &&isClassPointObjDef!=m_iPreFlags)
		  return false;

	  string_type strTok;

	  int iEnd = ExtractToken(m_pParser->ValidNameChars(), strTok, m_iPos);

	  if (iEnd==m_iPos)
		  return false;

	  if (m_iSynFlags & noClassObjDef)
			Error(ecUNEXPECTED_VAR, m_iPos - (int)a_Tok.GetAsString().length(), strTok);//2011 04 11 cxy change
		 // Error(ecUNEXPECTED_VAR, m_iPos - (int)a_Tok.GetAsString().length(), a_Tok.GetAsString());

	  //is org class need to impletement
	  string_type str=m_pClassBase->getclass();


	  if(isClassDef==m_iPreFlags)
	  {
		  if(str==typeid(value_type).name())//是底层运算类 double
		  {
				  if ( m_bIgnoreUndefVar || m_pFactory )  
				  {	
					  //如果变量工厂函数定义，从变量工厂产生变量
					  return false;
				  }
				  else
				  {
						  //如果变量工厂没定义从 ，类定义类自定义添加变量提供指针
						  value_type *fVar =(value_type *) ( m_pClassBase->addvar(strTok));//var factory org form a class
						  if(0==fVar)
							  Error(ecUNEXPECTED_VAR, m_iPos - (int)a_Tok.GetAsString().length(), a_Tok.GetAsString());
						  a_Tok.SetVar(fVar, strTok );
						  (*m_pVarDef)[strTok] = fVar;
						  m_UsedVar[strTok] = fVar;  // Add variable to used-var-list
				  }	
		  }
		  else
		  {
				//if(str=="string")
				//{//字串类型
				//	  string_type *fstr =(string_type*)( m_pClassBase->addvar(strTok));
				//	  if(0==fstr)
				//		  Error(ecUNEXPECTED_VAR, m_iPos - (int)a_Tok.GetAsString().length(), a_Tok.GetAsString());
				//	  a_Tok.SetStringVar(fstr,strTok);
				//	  (*m_pStringVarDef)[strTok] = fstr;
				//}
				//else
				{
					 //属于自定义类或其它定义类型 
					  void *pclassvar=m_pClassBase->addvar(strTok);
					  if(0==pclassvar)//类实现对象重复
						  Error(ecUNEXPECTED_VAR, m_iPos - (int)a_Tok.GetAsString().length(), strTok);
					  //need to set !need to do
					  m_pClassObj=pclassvar;
					  a_Tok.SetClassVarDef(m_pClassBase,pclassvar, strTok );
				}
		  }

	  }
	  else 
	  if(isClassPointObjDef==m_iPreFlags)
	  {
		  if(str==typeid(value_type).name())//是底层运算类
		  {
			  //底层运算类暂时不考虑指针
			  assert(0);//底层指针暂时不定义
			  //if ( m_bIgnoreUndefVar || m_pFactory )  
			  //{	
				 // //如果变量工厂函数定义，从变量工厂产生变量
				 // return false;
			  //}
			  //else
			  //{
				 // //如果变量工厂没定义从 ，类定义类自定义添加变量提供指针
				 // value_type *fVar =(value_type *) ( m_pClassBase->addvar(strTok));//var factory org form a class
				 // if(0==fVar)
					//  Error(ecUNEXPECTED_VAR, m_iPos - (int)a_Tok.GetAsString().length(), a_Tok.GetAsString());
				 // a_Tok.SetVar(fVar, strTok );

				 // (*m_pVarDef)[strTok] = fVar;
				 // m_UsedVar[strTok] = fVar;  // Add variable to used-var-list
			  //}
		  }
		  else
		  {
			  //属于自定义类或其它
			  void *pclassvar=m_pClassBase->addpointvar(strTok,0);

			  if(0==pclassvar)//类实现对象重复
				  Error(ecUNEXPECTED_VAR, m_iPos - (int)a_Tok.GetAsString().length(), strTok);
			  //need to set !need to do
			  m_pClassObj=pclassvar;
			  a_Tok.SetClassVar(m_pClassBase,pclassvar, strTok );
		  }

	  }
	  m_pClassBase=NULL;
	  m_iPos = iEnd;

	  // Call the variable factory in order to let it define a new parser variable
	  m_iSynFlags =noVAL | noVAR | noFUN | noBO | noPOSTOP | noINFIXOP | noSTR
		  | noClassDef | noClassObjDef | noClassObj;
	  m_iPreFlags = isClassObjDef;
	  return true;
  }


  //---------------------------------------------------------------------------
  /** \brief Check wheter a token at a given position is the front class undefined variable. 

  \param a_Tok [out] If a variable tom_pParser->m_vStringBufken has been found it will be placed here.
  \return true if a variable token has been found.
  \throw nothrow
  //类变量
  */
  bool ParserTokenReader::IsClassObjTok(token_type &a_Tok)
  {	
	  if( 0!=m_iPreFlags
		  &&isSEMIC!=m_iPreFlags
		  &&isBO!=m_iPreFlags
		  &&isCOMMA!=m_iPreFlags
		  &&isLB!=m_iPreFlags
		  &&isRB!=m_iPreFlags
		  &&isASSIGN!=m_iPreFlags)
		  return false;

	  string_type strTok;
	  int iEnd = ExtractToken(m_pParser->ValidNameChars(), strTok, m_iPos);
	  if (iEnd==m_iPos)
		  return false;

	  if (m_iSynFlags & noClassObj)
			Error(ecUNEXPECTED_VAR, m_iPos - (int)a_Tok.GetAsString().length(),strTok);//2011 04 11 cxy change
			//Error(ecUNEXPECTED_VAR, m_iPos - (int)a_Tok.GetAsString().length(), a_Tok.GetAsString());
	  //遍历类列表中的变量列表查找存在当前变量否
	  {
		  mu::classbasemap_type classmap = m_pParser->GetClassMap();
		  if(!classmap.size())
			  return false;

		  classbasemap_type::const_iterator item = classmap.begin();

		  for (; item!=classmap.end(); ++item)
		  {
			  classbase *pclass=(item->second);
			  if(pclass->findvar( strTok ))
			  {
				   void *pclassvar=pclass->getvar(strTok);
				  a_Tok.SetClassVar(pclass,pclassvar, strTok );
				  m_pClassBase=pclass;
				  m_pClassObj=pclassvar;
				  break;
			  }
		  }

		  if(item==classmap.end())
		  {//未在类变量中找到报错
			 
			  Error(ecUNEXPECTED_VAR, m_iPos - (int)a_Tok.GetAsString().length(), strTok);//2011 04 11 cxy change
			//Error(ecUNEXPECTED_VAR, m_iPos - (int)a_Tok.GetAsString().length(), a_Tok.GetAsString());
		  }

	  }

	  m_iPos = iEnd;

	  // Call the variable factory in order to let it define a new parser variable
	  m_iSynFlags =noVAL | noVAR | noFUN | noBO | noPOSTOP | noINFIXOP | noSTR 
		  | noClassDef | noClassObjDef | noClassObj;
	  m_iPreFlags = isClassObj;
	  return true;
  }


  /** \brief Check wheter a token at a given position is a variable token. 
  \param a_Tok [out] If a class name token has been found it will be placed here.
  \return true if a variable token has been found.
 
  类申明定义                                                           
 
  */
  bool ParserTokenReader::IsClassDefTok(token_type &a_Tok)
  {
	  if(0!=m_iPreFlags
		  &&isSEMIC!=m_iPreFlags
		  &&isLB!=m_iPreFlags
		  &&isRB!=m_iPreFlags)
		  return false;

	  if (!m_pClassDef->size())//类定义表中是否有定义
		  return false;

	  string_type strTok;
	  int iEnd = ExtractToken(m_pParser->ValidNameChars(), strTok, m_iPos);
	  if (iEnd==m_iPos)
		  return false;

	  //在类型定义map find the same class name
	  classbasemap_type::const_iterator item =  m_pClassDef->find(strTok);
	  if (item==m_pClassDef->end())
		  return false;

	  if (m_iSynFlags & noClassDef)
		  Error(ecCLASS_CONFICT, m_iPos, strTok);

	  m_iPos = iEnd;

	  //m_bClassDefVar=true;
	  m_pClassBase=item->second;
	  //set token 
	  a_Tok.SetClass(m_pClassBase,strTok);
	  //1.判断是否是 整数型类int 或 浮点double 是 判断当前变量是否存在 生成当前变量
	  //2.不是整数或浮点类型 添加到类指针的列表中等待调用其函数或是 使用其 整数或浮点类型进行计算
	  m_iSynFlags =  noANY ^ ( noClassObjDef );//后只能接 class obj def and "*"opt
	  m_iPreFlags = isClassDef;
	  return true;
  }


  //---------------------------------------------------------------------------
  /** \brief Check whether the token at a given position is a Class function token.
  // class function
  */
  bool ParserTokenReader::IsClassFucTok(token_type &a_Tok)
  {
 

	  if(isClassMemOp!=m_iPreFlags
		  &&isClassPointOp!=m_iPreFlags)
		  return false;
	  string_type strTok;
	  //Get function name and first position and end position
	  int iEnd = ExtractToken(m_pParser->ValidNameChars(), strTok, m_iPos);
	  if (iEnd==m_iPos)
		  return false;

		if(!m_pClassBase->findclassfun(strTok))
			Error(ecCLASSFUC_CONFICT, m_iPos - (int)a_Tok.GetAsString().length(), strTok);

		a_Tok.SetClassFuc(m_pClassBase,m_pClassObj,strTok);

	  m_iPos = (int)iEnd;

	  m_iSynFlags = noANY ^ noBO;//只能是BO "("
	  m_iPreFlags = isClassMemFuc;
	  return true;
  }

  //bool ParserTokenReader::IsIfWhileok(token_type &a_Tok)
  //{


	 // if(0!=m_iPreFlags
		//  &&isSEMIC!=m_iPreFlags
		//  &&isLB!=m_iPreFlags
		//  &&isRB!=m_iPreFlags)
		//  return false;
	 // string_type strTok;
	 // int iEnd = ExtractToken(m_pParser->ValidNameChars(), strTok, m_iPos);
	 // if (iEnd==m_iPos)
		//  return false;

	 // if (m_iSynFlags & noFUN)
		//  Error(ecUNEXPECTED_FUN, m_iPos, strTok);

	 // if(strTok==string_type("while")||strTok==string_type("if"))
	 // {

		//m_iPos = (int)iEnd;
		//m_iSynFlags = noANY ^ noBO;//只能是BO "("
		//m_iPreFlags = isIfWhile;
		//if(strTok==string_type("while"))
		//{
		//	
		//	a_Tok.SetIfWhile(strTok);
		//}
		//if(strTok==string_type("if"))
		//{
		//	a_Tok.SetIf(strTok);
		//}
		//return true;
	 // }
	 // else
	 // {
		//return false;
	 // }
  //}
  //---------------------------------------------------------------------------
  bool ParserTokenReader::IsStrVarTok(token_type &a_Tok)
  {
    if (!m_pStrVarDef || !m_pStrVarDef->size())
      return false;

    string_type strTok;
    int iEnd = ExtractToken(m_pParser->ValidNameChars(), strTok, m_iPos);
    if (iEnd==m_iPos)
      return false;

    strmap_type::const_iterator item =  m_pStrVarDef->find(strTok);
    if (item==m_pStrVarDef->end())
      return false;

    if (m_iSynFlags & noSTR)
      Error(ecUNEXPECTED_VAR, m_iPos, strTok);

    m_iPos = iEnd;
    if (!m_pParser->m_vStringVarBuf.size())
      Error(ecINTERNAL_ERROR);

    a_Tok.SetString(m_pParser->m_vStringVarBuf[item->second], m_pParser->m_vStringVarBuf.size() );

    m_iSynFlags = m_iSynFlags = noANY ^ ( noBC | noOPT | noEND | noCOMMA);
    return true;
  }

  //---------------------------------------------------------------------------
  /** \brief Check wheter a token at a given position is an undefined variable. 

      \param a_Tok [out] If a variable tom_pParser->m_vStringBufken has been found it will be placed here.
	    \return true if a variable token has been found.
      \throw nothrow
  */
  bool ParserTokenReader::IsUndefVarTok(token_type &a_Tok)
  {
    string_type strTok;
    int iEnd = ExtractToken(m_pParser->ValidNameChars(), strTok, m_iPos);
    if (iEnd==m_iPos)
      return false;

    if (m_iSynFlags & noVAR)
      Error(ecUNEXPECTED_VAR, m_iPos - (int)a_Tok.GetAsString().length(), strTok);

    // If a factory is available implicitely create new variables
    if (m_pFactory)
    {
		value_type *fVar = m_pFactory(strTok.c_str(),m_pClass);//var factory org form a class
		a_Tok.SetVar(fVar, strTok );
		//!!!!!!!!!!!!!!!!!!CXYADD TO DO: find insect parameter and create input parameter table
		// Do not use m_pParser->DefineVar( strTok, fVar );
		// in order to define the new variable, it will clear the
		// m_UsedVar array which will kill previously defined variables
		// from the list
		// This is safe because the new variable can never override an existing one
		// because they are checked first!
		(*m_pVarDef)[strTok] = fVar;
		m_UsedVar[strTok] = fVar;  // Add variable to used-var-list
    }
    else
    {
		a_Tok.SetVar((value_type*)&m_fZero, strTok);
		m_UsedVar[strTok] = 0;   // Add variable to used-var-list
    }

    m_iPos = iEnd;

    // Call the variable factory in order to let it define a new parser variable
    m_iSynFlags = noVAL | noVAR | noFUN | noBO | noPOSTOP | noINFIXOP | noSTR;
    return true;
  }
  //---------------------------------------------------------------------------
  /** \brief Check wheter a token at a given position is a string.

      \param a_Tok [out] If a variable token has been found it will be placed here.
  	  \return true if a string token has been found.
      \sa IsOprt, IsFunTok, IsStrFunTok, IsValTok, IsVarTok, IsEOF, IsInfixOpTok, IsPostOpTok
      \throw nothrow
  */
  bool ParserTokenReader::IsString(token_type &a_Tok)
  {
    if (m_strFormula[m_iPos]!='"') 
      return false;

    string_type strBuf(&m_strFormula[m_iPos+1]);
    std::size_t iEnd(0), iSkip(0);

    // parser over escaped '\"' end replace them with '"'
    for(iEnd=(int)strBuf.find("\""); iEnd!=string_type::npos; iEnd=(int)strBuf.find("\"", iEnd))
    {
      if (strBuf[iEnd-1]!='\\') break;
      strBuf.replace(iEnd-1, 2, "\"");
      iSkip++;
    }

    if (iEnd==string_type::npos)
      Error(ecUNTERMINATED_STRING, m_iPos, "\"");

    string_type strTok(strBuf.begin(), strBuf.begin()+iEnd);

    if (m_iSynFlags & noSTR)
      Error(ecUNEXPECTED_STR, m_iPos, strTok);

		m_pParser->m_vStringBuf.push_back(strTok); // Store string in internal buffer
    a_Tok.SetString(strTok, m_pParser->m_vStringBuf.size());

    m_iPos += (int)strTok.length() + 2 + (int)iSkip;  // +2 wg Anf黨rungszeichen; +iSkip f黵 entfernte escape zeichen
    m_iSynFlags = m_iSynFlags = noANY ^ ( noBC | noOPT | noEND );

    return true;
  }

  //---------------------------------------------------------------------------
  /** \brief Create an error containing the parse error position.

    This function will create an Parser Exception object containing the error text and its position.

    \param a_iErrc [in] The error code of type #EErrorCodes.
    \param a_iPos [in] The position where the error was detected.
    \param a_strTok [in] The token string representation associated with the error.
    \throw ParserException always throws thats the only purpose of this function.
  */
  void  ParserTokenReader::Error( EErrorCodes a_iErrc, 
                                  int a_iPos, 
                                  const string_type &a_sTok) const
  {
    m_pParser->Error(a_iErrc, a_iPos, a_sTok);
  }
} // namespace mu
