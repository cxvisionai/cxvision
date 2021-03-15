
#ifndef MUP_DEF_H
#define MUP_DEF_H

#include <string>
#include <map>
#include <vector>

#include "muParserFixes.h"

//////////////////////////////////////////////////////////////////////////
//  [10/22/2012 cxy]  muparserSSE 
/** \file
\brief This file contains standard definitions used by the parser.
*/

#define MEC_VERSION _T("1.0.0")

/** \brief Characters for use in unary and binary operators. */
#define MEC_OPRT_CHARS _T("+-*^/?<>=#!$%&|~'_")

#define MEC_CHARS _T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")


/** \brief Define the base datatype for values.

  This datatype must be a built in value type. You can not use custom classes.
  It has been tested with float, double and long double types, int should
  work as well.
*/
#define MUP_BASETYPE double

/** \brief Definition of the basic parser string type. */
#define MUP_STRING_TYPE std::string

/** \brief Definition of the basic bytecode datatype. */
#define MUP_BYTECODE_TYPE int

//-------------------------------------------------------
//cxyadd
#define MUP_VISION_TYPE double
#define MUP_VISIONLP_TYPE int*
#define MUP_VOID_TYPE void

/** \brief Maybe I use this for unicode support later. */
#ifndef _T
  #define _T(x) x
#endif

#if defined(_DEBUG)
  /** \brief Debug macro to force an abortion of the programm with a certain message.
  */
  #define MUP_FAIL(MSG)    \
          bool MSG=false;  \
          assert(MSG);

  /** \brief An assertion that does not kill the program.
  */
  #define MUP_ASSERT(COND)                              \
          if (!(COND))                                  \
          {                                             \
            stringstream_type ss;                       \
            ss << _T("Assertion \""#COND"\" failed: "); \
            ss << __FILE__ << _T(" line ") << __LINE__; \
            ss << ".";                                  \
            throw ParserError( ss.str() );              \
          }
#else
  #define MUP_FAIL(MSG)
  #define MUP_ASSERT(COND)
#endif

//------------------------------------------------------------------------------
//
// do not change anything beyond this point...
//
// !!! This section is devoted to macros that are used for debugging
// !!! or for features that are not fully implemented yet.
//
//#define MUP_DUMP_STACK
//#define MUP_DUMP_CMDCODE


namespace mu
{

//------------------------------------------------------------------------------
/** \brief Bytecode values.

    \attention The order of the operator entries must match the order in ParserBase::c_DefaultOprt!
*/
enum ECmdCode
{
/*  "<=", ">=", "!=", "==", "<", ">", "+", "-", "*", "/", 
  "^", "and", "or", "xor", "=", "(", ")", ",", 0 
*/
  // The following are codes for built in binary operators
  // apart from built in operators the user has the opportunity to
  // add user defined operators.
  cmLE            = 0,   ///< Operator item:  less or equal "<="
  cmGE            = 1,   ///< Operator item:  greater or equal ">="
  cmNEQ           = 2,   ///< Operator item:  not equal "!="
  cmEQ            = 3,   ///< Operator item:  equals "=="
  cmLT            = 4,   ///< Operator item:  less than "<"
  cmGT            = 5,   ///< Operator item:  greater than ">"
  cmADD           = 6,   ///< Operator item:  add "+"
  cmSUB           = 7,   ///< Operator item:  subtract "-"
  cmMUL           = 8,   ///< Operator item:  multiply "*"
  cmDIV           = 9,   ///< Operator item:  division "/"
  cmPOW           = 10,  ///< Operator item:  y to the power of "^"
  cmAND           = 11,  ///< Operator item:  logical and "and"
  cmOR            = 12,  ///< Operator item:  logical or "or"
  cmXOR           = 13,  ///< Operator item:  logical xor "xor"
  cmASSIGN        = 14,  ///< Operator item:  Assignment operator "="
  cmBO            = 15,  ///< Operator item:  opening bracket "("
  cmBC            = 16,  ///< Operator item:  closing bracket  ")"
  cmCOMMA         = 17,  ///< Operator item:  comma ","
  cmSEMICOLON     = 18,  ///< Operator item:  Semicolon ";"
  cmLB			  = 19,  ///< Operator item:  lift bracket  "{"
  cmRB			  = 20,  ///< Operator item:  right bracket  "}"
  cmMember		  = 21,  ///< Operator item:  member point "."
  cmPointer		  = 22,  ///< Operator item:  member pointer "->"
  cmIf			  = 23,	 ///< Operator item:  "if"
  cmElse		  = 24,  ///< Operator item:  "else"
  cmWhile         = 25,  ///< Operator item:  "while"

  cmVAR           = 26,  ///< variable item
  cmSTRVAR        = 27,
  cmVAL           = 28,  ///< value item

  cmFUNC          = 29,  ///< Code for a function item
  cmFUNC_STR      = 30,  ///< Code for a function with a string parameter

  cmSTRING        = 31,  ///< Code for a string token
  cmOPRT_BIN      = 32,  ///< user defined binary operator
  cmOPRT_POSTFIX  = 33,  ///< code for postfix operators
  cmOPRT_INFIX    = 34,  ///< code for infix operators "!" "-"
  cmEND           = 35,  ///< end of formula

//cxyadd
  cmClassObjDef   = 36,   ///<one express end ; 
  cmVARLP		  = 37,   ///<one variable item point
  cmONEAREAEND    = 38,
  cmClass		  = 39,   ///<class item
  cmClassObj	  = 40,   ///<class var item
  cmClassFuc	  = 41,   ///<class function param num
  cmClassFucNum	  = 42,   ///<class function param num
  cmClassFucStr	  = 43,   ///<class function param str
  cmClassFucVp	  = 44,	  ///<class function param void lp
 
  cmStorageEnd	  = 45,  ///while storage one circle run end
  cmCallin		  = 46,  ///circle block in point
  cmStringVar	  = 47,  ///string var item
  cmStorageBegin  = 48,	
  cmUNKNOWN       = 49   ///< uninitialized item
  //cxyaddend

};

//------------------------------------------------------------------------------
/** \brief Types internally used by the parser.
*/
enum ETypeCode
{
  tpSTR  = 0,     ///> String type (Function arguments and constants only, no string variables)

  tpDBL   = 1,     ///> Floating point variables
  tpVOID  = 2,      ///> Undefined type.
  tpSTRLP = 3,
  tpDBLLP = 4,
  tpVOIDLP= 5,
  tpCLASS = 6

};

enum EParamCodes
{
	Param_none=1,
	Param_voidp_1,
	Param_voidp_2,
	Param_voidp_3,
	Param_voidp_4,
	Param_voidp_5,
	Param_voidp_1_Return_double,
	Param_double_1,
	Param_double_2,
	Param_double_3,
	Param_double_4,
	Param_int_1,
	Param_int_2,
	Param_int_3,
	Param_int_4,
	Param_int_5,
	Param_int_6,
	Param_int_7,
	Param_charp_1,
	Param_0_Return_charp,
	Param_0_Return_int,
	Param_0_Return_double,
	Param_int_1_Return_int,
	Param_int_2_Return_int,
	Param_int_1_Return_double,
	Param_int_2_Return_double,
	Param_int_3_Return_double,
	Param_any
};

enum EClassTypeCodes
{
	CLASS_ORG=1,
	CLASS_PARSER,
	CLASS_CREATE,
	CLASS_ANY
};

//cxyaddEnd

//------------------------------------------------------------------------------
/** \brief Parser operator precedence values. */
enum EPrec
{
  // binary operators
  prLOGIC   = 1,  ///> logic operators
  prCMP     = 2,  ///> comparsion operators
  prADD_SUB = 3,  ///> addition
  prMUL_DIV = 4,  ///> multiplication/division
  prPOW     = 5,  ///> power operator priority (highest)

  // infix operators
  prINFIX    = 4, ///> Signs have a higher priority than ADD_SUB, but lower than power operator
  prPOSTFIX  = 4  ///> Postfix operator priority (currently unused)
};

//------------------------------------------------------------------------------
// basic types
typedef MUP_BASETYPE value_type;
typedef MUP_STRING_TYPE string_type;
typedef MUP_BYTECODE_TYPE bytecode_type;

//------------------------------------------------------------------------------
//cxyadd
typedef MUP_VISION_TYPE vision_type;
typedef MUP_VISIONLP_TYPE vision_lptype;
typedef MUP_VOID_TYPE void_type;
 

typedef string_type::value_type char_type;
typedef std::basic_stringstream<char_type,
                                std::char_traits<char_type>,
                                std::allocator<char_type> > stringstream_type;

// Data container types
typedef std::map<string_type, value_type*> varmap_type;
typedef std::map<string_type, value_type> valmap_type;
typedef std::map<string_type, std::size_t> strmap_type;

//cxyadd String container types
typedef std::map<string_type,string_type*> stringmap_type;

// Parser callbacks
typedef value_type (*fun_type1)(value_type);
typedef value_type (*fun_type2)(value_type, value_type);
typedef value_type (*fun_type3)(value_type, value_type, value_type);
typedef value_type (*fun_type4)(value_type, value_type, value_type, value_type);
typedef value_type (*fun_type5)(value_type, value_type, value_type, value_type, value_type);
typedef value_type (*multfun_type)(const value_type*, int);
typedef value_type (*strfun_type1)(const char *);

//cxyadd
typedef value_type* (*fun_lptype)(value_type);
//
typedef void_type  (*fun_void_type1)(value_type);
typedef void_type  (*fun_void_type2)(value_type, value_type);
typedef void_type  (*fun_void_type3)(value_type, value_type, value_type);
typedef void_type  (*fun_void_type4)(value_type, value_type, value_type, value_type);
typedef void_type  (*fun_void_type5)(value_type, value_type, value_type, value_type, value_type);
typedef void_type  (*multfun_void_type)(const value_type*, int);
typedef void_type  (*strfun_void_type1)(const char *);
//cxyadd 

typedef vision_type (*visionfun_type)(const vision_type*,vision_type);
typedef vision_type (*visionfun_lptype)(const vision_type*,vision_lptype);

// Parser utility callback functions (unrelated to the math callbacks)
typedef bool (*identfun_type)(const char_type*, int&, value_type&);

typedef value_type* (*facfun_type)(const char_type*,void *);

typedef struct virtualclass
{  
	MUP_BYTECODE_TYPE * pclass_inicall;
	int ilen_icall;
	MUP_BYTECODE_TYPE * pclass_funcall;
	int ilen_fcall;
}VIRCLASS;
} // end fo namespace

#endif
