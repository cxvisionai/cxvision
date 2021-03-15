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
#ifndef MU_PARSER_CALLBACK_H
#define MU_PARSER_CALLBACK_H

#include "muParserDef.h"



namespace mu
{

/** \brief Encapsulation of prototypes for a numerical parser function.

    Encapsulates the prototyp for numerical parser functions. The class
    stores the number of arguments for parser functions as well
    as additional flags indication the function is non optimizeable.
    The pointer to the callback function pointer is stored as void* 
    and needs to be casted according to the argument count.
    Negative argument counts indicate a parser function with a variable number
    of arguments. 
    This class is not used for string function prototyping.

    \author (C) 2004-2006 Ingo Berg
*/
class  ParserCallback
{
public:
    ParserCallback(fun_type1 a_pFun, bool a_bAllowOpti, int a_iPrec = -1, ECmdCode a_iCode=cmFUNC);
    ParserCallback(fun_type2 a_pFun, bool a_bAllowOpti, int a_iPrec = -1, ECmdCode a_iCode=cmFUNC);
    ParserCallback(fun_type3 a_pFun, bool a_bAllowOpti);
    ParserCallback(fun_type4 a_pFun, bool a_bAllowOpti);
    ParserCallback(fun_type5 a_pFun, bool a_bAllowOpti);
    ParserCallback(multfun_type a_pFun, bool a_bAllowOpti);
    ParserCallback(strfun_type1 a_pFun, bool a_bAllowOpti);
	//cxyaddBegin
	ParserCallback(fun_lptype a_pFun, bool a_bAllowOpti, int a_iPrec = -1, ECmdCode a_iCode=cmFUNC);
	ParserCallback(visionfun_type a_pFun,bool a_bAllowOpti);
	ParserCallback(visionfun_lptype a_pFun,bool a_bAllowOpti);

	//new add
	ParserCallback(fun_void_type1 a_pFun, bool a_bAllowOpti, int a_iPrec = -1, ECmdCode a_iCode=cmFUNC);
	ParserCallback(fun_void_type2 a_pFun, bool a_bAllowOpti, int a_iPrec = -1, ECmdCode a_iCode=cmFUNC);
	ParserCallback(fun_void_type3 a_pFun, bool a_bAllowOpti);
	ParserCallback(fun_void_type4 a_pFun, bool a_bAllowOpti);
	ParserCallback(fun_void_type5 a_pFun, bool a_bAllowOpti);
	ParserCallback(multfun_void_type a_pFun, bool a_bAllowOpti);
	ParserCallback(strfun_void_type1 a_pFun, bool a_bAllowOpti);
	//cxyaddEnd
    ParserCallback();
    ParserCallback(const ParserCallback &a_Fun);
    
    ParserCallback* Clone() const;

    bool IsOptimizable() const;
    void* GetAddr() const;
    ECmdCode  GetCode() const;
    ETypeCode GetType() const;
    int GetPri()  const;
    int GetArgc() const;
	ETypeCode GetReturn() const;
private:
    void *m_pFun;       ///< Pointer to the callback function, casted to void
    
    /** \brief Number of function arguments
    
        This number is negative for functions with variable number of arguments. in this cases
        they represent the actual number of arguments found.
    */
    int   m_iArgc;    
    int   m_iPri;       ///< Valid only for binary and infix operators; Operator precedence.
    ECmdCode  m_iCode;
    ETypeCode m_iType;
    bool  m_bAllowOpti; ///< Flag indication optimizeability 
	ETypeCode m_iReturnType;

};

//------------------------------------------------------------------------------
/** \brief Container for Callback objects. */
typedef std::map<string_type, ParserCallback> funmap_type; 

} // namespace mu

#endif
