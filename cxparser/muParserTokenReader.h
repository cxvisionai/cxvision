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
#ifndef MU_PARSER_TOKEN_READER_H
#define MU_PARSER_TOKEN_READER_H

#include <cassert>
#include <cstdio>
#include <cstring>
#include <map>
#include <memory>
#include <stack>
#include <string>

#include "muParserDef.h"
#include "muParserToken.h"
#include "muParserClass.h"






namespace mu
{

  // Forward declaration
  class ParserBase;

  /** \brief Token reader for the ParserBase class.

  */
  class  ParserTokenReader
  {
  private:
      typedef ParserToken<value_type, string_type> token_type;

  private:
      ParserBase *m_pParser;
      string_type m_strFormula;
      int  m_iPos;
      int  m_iSynFlags;
      bool m_bIgnoreUndefVar;


      const funmap_type *m_pFunDef;
      const funmap_type *m_pPostOprtDef;
      const funmap_type *m_pInfixOprtDef;
      const funmap_type *m_pOprtDef;
      const valmap_type *m_pConstDef;
      const strmap_type *m_pStrVarDef;

      varmap_type *m_pVarDef;  ///< The only non const pointer to parser internals
      facfun_type m_pFactory;

      std::vector<identfun_type> m_vIdentFun; ///< Value token identification function
      varmap_type m_UsedVar;
      value_type m_fZero;      ///< Dummy value of zero, referenced by undefined variables
      int m_iBrackets;
	  int m_iBigBrackets;

	  //////////////////////////////////////////////////////////////////////////
	  //cxyadd
	  void *m_pClass;  ///< 生成变量工厂的基类
	  classbasemap_type *m_pClassDef;   ///< Class map point ,def class store in map
      bool m_bClassDef;		///< reader find a class def in front set m_bClassDefVar true,else false
	  classbase *m_pClassBase;  ///< Get the Class Object var map
	  void *m_pClassObj;		///< Get the spec Class Object
	  int  m_iPreFlags; ///< 前一个符号

	 stringmap_type *m_pStringVarDef;
	//////////////////////////////////////////////////////////////////////////

	 int m_icurID;

  private:
	  enum EPreCodes
	  {
		  isBO      = 1 << 0,  ///< " (" 
		  isBC      = 1 << 1,  ///< " )"
		  isVAL     = 1 << 2,  ///< "2"
		  isVAR     = 1 << 3,  ///< "var"
		  isCOMMA   = 1 << 4,  ///< "," 
		  isFUN     = 1 << 5,  ///< "sin(var)"	
		  isOPT     = 1 << 6,  ///< "(var1+var2)"
		  isPOSTOP  = 1 << 7,  ///< "sin!"
		  isINFIXOP = 1 << 8,  ///< "+var" "!var"
		  isEND     = 1 << 9,  ///< end of formula
		  isSTR     = 1 << 10, ///< ""string to demo""
		  isASSIGN  = 1 << 11, ///<  "="
		  isLB      = 1 << 12, ///<  "{"
		  isRB	    = 1 << 13, ///<  "}"
		  isSEMIC   = 1			<< 14, ///<  ";"
		  isClassDef= 1			<< 15, ///<  "int ","TCLASS" 
		  isClassObjDef= 1		<< 16,///< "int aobject;"  then ".aobject;" 
		  isClassPointObjDef=1	<<17,///<"*" then "int * apoint;"
		  isClassMemOp=1	<< 18,///<  "." 
		  isClassPointOp=1	<<19,///<  " ->"
		  isClassMemVar=1	<<20, ///<  ".m_var"
		  isClassMemFuc=1	<<21, ///<  ".m_fucn()"
		  isClassObj=1		<<22,	   ///
		  isIf	=1			<<23,    ///
		  isElse = 1		<<24,
		  isWhile = 1		<<25,
		  isANY     = ~0       ///< All of he above flags set
	  };
	  //cxyaddend
	  //////////////////////////////////////////////////////////////////////////

  //
  // private Functions
  //
  private:
      /*
		  brief Syntax codes. 
	        The syntax codes control the syntax check done during the first time parsing of 
          the expression string. They are flags that indicate which tokens are allowed next
          if certain tokens are identified.
  	  */
      enum ESynCodes
      {
        noBO      = 1 << 0,  ///< to avoid i.e. "cos(7)(" 
        noBC      = 1 << 1,  ///< to avoid i.e. "sin)" or "()"
        noVAL     = 1 << 2,  ///< to avoid i.e. "tan 2" or "sin(8)3.14"
        noVAR     = 1 << 3,  ///< to avoid i.e. "sin a" or "sin(8)a"
        noCOMMA   = 1 << 4,  ///< to avoid i.e. ",," or "+," ...
        noFUN     = 1 << 5,  ///< to avoid i.e. "sqrt cos" or "(1)sin"	
        noOPT     = 1 << 6,  ///< to avoid i.e. "(+)"
        noPOSTOP  = 1 << 7,  ///< to avoid i.e. "(5!!)" "sin!"
	    noINFIXOP = 1 << 8,  ///< to avoid i.e. "++4" "!!4"
        noEND     = 1 << 9,  ///< to avoid unexpected end of formula
        noSTR     = 1 << 10, ///< to block numeric arguments on string functions
        noASSIGN  = 1 << 11, ///< to block assignement to constant i.e. "4=7"
		noLB      = 1 << 12, ///< to avoid i.e. "({","cos{",",{","+{"
		noRB	  = 1 << 13, ///< to avoid i.e. "(}","cos}",",{}","+}"
		noSEMIC   = 1 << 14, ///< to avoid i.e. "(;","cos;","{;","+;",""
		noClassDef= 1 << 15, ///< to avoid i.e. "int int","int cos","int {"
		noClassObjDef= 1 << 16, ///< to avoid i.e. pre define("int aobject;") then ".aobject;" 
		noClassMemOp=1 << 17,///< to avoid i.e. "cos." "+." "(." 
		noClassPointOp=1<<18,///< to avoid i.e. "=->" "+->"
		noClassMemVar=1<<19, ///
		noClassMemFuc=1<<20, ///
		noClassObj=1<<21,
        noANY     = ~0       ///< All of he above flags set
      };

      ParserTokenReader(const ParserTokenReader &a_Reader);
      ParserTokenReader& operator=(const ParserTokenReader &a_Reader);
      void Assign(const ParserTokenReader &a_Reader);

  public:
      ParserTokenReader(ParserBase *a_pParent);
     ~ParserTokenReader();
      ParserTokenReader* Clone(ParserBase *a_pParent) const;
      void AddValIdent(identfun_type a_pCallback);
      void SetVarCreator(facfun_type a_pFactory);		
	  void SetClassUing(void * a_pVoid);
      int GetPos() const;
      const string_type& GetFormula() const;
      const varmap_type& GetUsedVar() const;
      void SetFormula(const string_type &a_strFormula);
      void SetDefs( const funmap_type *a_pFunDef, 
                    const funmap_type *a_pOprtDef,
                    const funmap_type *a_pInfixOprtDef,
                    const funmap_type *a_pPostOprtDef,
                    varmap_type *a_pVarDef,
                    const strmap_type *a_pStrVarDef,
                    const valmap_type *a_pConstDef );
      void IgnoreUndefVar(bool bIgnore);
	  void UsingClassDef(bool busing);

      void ReInit();
	  void EndExpress();
      token_type ReadNextToken();//int &irunstep);

  //
  // private functions
  //
  private:

      void SetParent(ParserBase *a_pParent);
      int ExtractToken( const char_type *a_szCharSet, 
                        string_type &a_strTok, int a_iPos ) const;
	  bool IsComment(token_type &a_Tok);
	  bool IsBuiltInCoditionTok(token_type &a_Tok);
      bool IsBuiltIn(token_type &a_Tok);
      bool IsEOF(token_type &a_Tok);

      bool IsInfixOpTok(token_type &a_Tok);
      bool IsFunTok(token_type &a_Tok);
      bool IsPostOpTok(token_type &a_Tok);
      bool IsOprt(token_type &a_Tok);
      bool IsValTok(token_type &a_Tok);
      bool IsVarTok(token_type &a_Tok);
      bool IsStrVarTok(token_type &a_Tok);
      bool IsUndefVarTok(token_type &a_Tok);
      bool IsString(token_type &a_Tok);
	  //cxyaddBegin
	  bool IsClassDefTok(token_type &a_Tok);
	  bool IsClassObjDefTok(token_type &a_Tok);
	  bool IsClassObjTok(token_type &a_Tok);
	  bool IsClassFucTok(token_type &a_Tok);
	  //bool IsIfWhileok(token_type &a_Tok);
	  //cxyaddEnd
	  void Error( EErrorCodes a_iErrc, int a_iPos = -1, 
                  const string_type &a_sTok = string_type() ) const;
  };
} // namespace mu

#endif

