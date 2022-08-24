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
#include "muParserBytecode.h"


#include <cassert>
#include <string>
#include <stack>
#include <vector>
#include <iostream>

#include "muParserDef.h"
#include "muParserError.h"
#include "muParserToken.h"


namespace mu
{
  //---------------------------------------------------------------------------
  /** \brief Bytecode default constructor. 
     
      \pre [assert] sizeof(value_type)>=sizeof(map_type)
      \pre [assert] sizeof(value_type*)>=sizeof(map_type)
  */
  ParserByteCode::ParserByteCode()
    :m_iStackPos(0)
    ,m_vBase()
    ,mc_iSizeVal( sizeof(value_type) / sizeof(map_type) )
    ,mc_iSizePtr(  std::max( (int)sizeof(value_type*) / 
                            (int)sizeof(map_type), 1 ) )
    ,mc_iSizeValEntry( 2 + mc_iSizeVal)
	,m_pcreateclass(NULL)
  {
    assert( sizeof(value_type)>=sizeof(map_type) );
  }

  //---------------------------------------------------------------------------
  /** \brief Destructor (trivial).*/
  ParserByteCode::~ParserByteCode()
  {}

  //---------------------------------------------------------------------------
  /** \brief Copy constructor. 
    
      Implemented in Terms of Assign(const ParserByteCode &a_ByteCode)
  */
  ParserByteCode::ParserByteCode(const ParserByteCode &a_ByteCode)
    :mc_iSizeVal( sizeof(value_type)/sizeof(map_type) )
    ,mc_iSizePtr( sizeof(value_type*) / sizeof(map_type) )
    ,mc_iSizeValEntry( 2 + mc_iSizeVal)
	,m_pcreateclass(NULL)
  {
    Assign(a_ByteCode);
  }
  //---------------------------------------------------------------------------
  /** \brief Assignment operator.
    
      Implemented in Terms of Assign(const ParserByteCode &a_ByteCode)
  */
  ParserByteCode& ParserByteCode::operator=(const ParserByteCode &a_ByteCode)
  {
    Assign(a_ByteCode);
    return *this;
  }

  //---------------------------------------------------------------------------
  /** \brief Store an address in bytecode.
  
      \param a_pAddr Address to be stored.
      \throw nothrow
  */
  void ParserByteCode::StorePtr(void *a_pAddr)
  {
    #if defined(_MSC_VER)
      #pragma warning( disable : 4311 )
    #endif

    // demo code for packing / unpacking pointers into bytecode
//    void *ptr(NULL);
//    double **pVal;
//    double fVal;
//    map_type dbg[2]; 
//    dbg[0] = *( reinterpret_cast<map_type*>(&a_pAddr) ), 
//    dbg[1] = *( reinterpret_cast<map_type*>(&a_pAddr) + 1 );     
// Version 1:
//    *( (map_type*)&ptr+0) = dbg[0];
//    *( (map_type*)&ptr+1) = dbg[1];
// Version 2:
//    memcpy(&ptr, dbg, sizeof(dbg));
// Version 3:    
//    pVal = (double**)dbg;
//    fVal = **(double**)dbg;

    for (int i=0; i<mc_iSizePtr; ++i)
    {
        m_vBase.push_back( *( reinterpret_cast<map_type*>(&a_pAddr) + i ) );
    }

    #if defined(_MSC_VER)
      #pragma warning( default : 4311 )
    #endif
  }

  //---------------------------------------------------------------------------
  /** \brief Copy state of another object to this. 
    
      \throw nowthrow
  */
  void ParserByteCode::Assign(const ParserByteCode &a_ByteCode)
  {
    if (this==&a_ByteCode)    
      return;

    m_iStackPos = a_ByteCode.m_iStackPos;
    m_vBase = a_ByteCode.m_vBase;
  }

  //---------------------------------------------------------------------------
  /** \brief Add a Variable pointer to bytecode. 
      \param a_pVar Pointer to be added.
      \throw nothrow
  */
  void ParserByteCode::AddVar(value_type *a_pVar)
  {
    m_vBase.push_back( ++m_iStackPos );
    m_vBase.push_back( cmVAR );

    StorePtr(a_pVar);

    int iSize = GetValSize()-GetPtrSize();
    assert(iSize>=0);

    // Make sure variable entries have the same size like value entries.
    // (necessary for optimization; fill with zeros)
    for (int i=0; i<iSize; ++i)
      m_vBase.push_back(0);
  }

  //---------------------------------------------------------------------------
  /** \brief Add a Variable pointer to bytecode. 

      Value entries in byte code consist of:
      <ul>
        <li>value array position of the value</li>
        <li>the operator code according to ParserToken::cmVAL</li>
        <li>the value stored in #mc_iSizeVal number of bytecode entries.</li>
      </ul>

      \param a_pVal Value to be added.
      \throw nothrow
  */
  void ParserByteCode::AddVal(value_type a_fVal)
  {
    m_vBase.push_back( ++m_iStackPos );
    m_vBase.push_back( cmVAL );

    for (int i=0; i<mc_iSizeVal; ++i)
      m_vBase.push_back( *(reinterpret_cast<map_type*>(&a_fVal) + i) );
  }

  //---------------------------------------------------------------------------
  /** \brief Add an operator identifier to bytecode. 
    
      Operator entries in byte code consist of:
      <ul>
        <li>value array position of the result</li>
        <li>the operator code according to ParserToken::ECmdCode</li>
      </ul>

      \sa  ParserToken::ECmdCode
  */
  void ParserByteCode::AddOp(ECmdCode a_Oprt)
  {
    m_vBase.push_back(--m_iStackPos);
    m_vBase.push_back(a_Oprt);
  }

  //---------------------------------------------------------------------------
  /** \brief Add an assignement operator
    
      Operator entries in byte code consist of:
      <ul>
        <li>cmASSIGN code</li>
        <li>the pointer of the destination variable</li>
      </ul>

      \sa  ParserToken::ECmdCode
  */
  void ParserByteCode::AddAssignOp(value_type *a_pVar)
  {
    m_vBase.push_back(--m_iStackPos);
    m_vBase.push_back(cmASSIGN);
    StorePtr(a_pVar);
  }

  //---------------------------------------------------------------------------
  /** \brief Add function to bytecode. 

      \param a_iArgc Number of arguments, negative numbers indicate multiarg functions.
      \param a_pFun Pointer to function callback.
  */
  void ParserByteCode::AddFun(void *a_pFun, int a_iArgc)
  {
    if (a_iArgc>=0)
    {
      m_iStackPos = m_iStackPos - a_iArgc + 1; 
    }
    else
    {
      m_iStackPos = m_iStackPos + a_iArgc + 1; 
    }

    m_vBase.push_back(m_iStackPos);
    m_vBase.push_back(cmFUNC);
	m_vBase.push_back(a_iArgc);

    StorePtr(a_pFun);
  }
  //---------------------------------------------------------------------------
  /** \brief Add class member function to bytecode. 

  \param a_iArgc Number of arguments, negative numbers indicate multiarg functions.
  \param a_pFun Pointer to function callback.
  */
  void ParserByteCode::AddClassMemberFunNum(void *a_pclass,void *a_pobj,void *a_pFun, int a_iArgc)
  {
	  if (a_iArgc>=0)
	  {
		  m_iStackPos = m_iStackPos - a_iArgc + 1; 
	  }
	  else
	  {
		  m_iStackPos = m_iStackPos + a_iArgc + 1; 
	  }

	  m_vBase.push_back(m_iStackPos);
	  m_vBase.push_back(cmClassFucNum);
	  m_vBase.push_back(a_iArgc);
		
	  StorePtr(a_pclass);
	  StorePtr(a_pobj);
	  StorePtr(a_pFun);
	 //如果是在编译新类成员函数将对象在类成员变量列表推入m_vBase
	if(NULL !=m_pcreateclass)
	  m_vBase.push_back(m_pcreateclass->findstacknum(a_pobj));
	else
	  m_vBase.push_back(-1);
 }

  void ParserByteCode::AddClassMemberFunVoidp(void *a_pclass,void *a_pobj,void *a_pFun, int istocknum2,void *the_pobj)
  {
	  //if (a_iArgc>=0)
	  //{
		 // m_iStackPos = m_iStackPos - a_iArgc + 1; 
	  //}
	  //else
	  //{
		 // m_iStackPos = m_iStackPos + a_iArgc + 1; 
	  //}

	  m_vBase.push_back(++m_iStackPos);
	  m_vBase.push_back(cmClassFucVp);
	  m_vBase.push_back(istocknum2);

	  StorePtr(a_pclass);
	  StorePtr(a_pobj);
	  StorePtr(a_pFun);
	  StorePtr(the_pobj);
	  //如果是在编译新类成员函数将对象在类成员变量列表推入m_vBase
	  if(NULL !=m_pcreateclass)
		  m_vBase.push_back(m_pcreateclass->findstacknum(a_pobj));
	  else
		  m_vBase.push_back(-1);
  }

  //---------------------------------------------------------------------------
  /** \brief Add class member Strung function entry to the parser bytecode. 

  \throw nothrow

  A string function entry consists of the stack position of the return value,
  followed by a cmSTRFUNC code, the function pointer and an index into the 
  string buffer maintained by the parser.
  */
  void ParserByteCode::AddClassMemberFunStr(void *a_pclass,void *a_pobj,void *a_pFun, int a_iArgc, int a_iIdx)
  {

	  m_vBase.push_back(++m_iStackPos);
	  m_vBase.push_back(cmClassFucStr);
	  m_vBase.push_back(a_iArgc);



	  StorePtr(a_pclass);
	  StorePtr(a_pobj);
	  StorePtr(a_pFun);

	  m_vBase.push_back(a_iIdx);
	  //如果是在编译新类成员函数将对象在类成员变量列表推入m_vBase
	  if(NULL !=m_pcreateclass)
		  m_vBase.push_back(m_pcreateclass->findstacknum(a_pobj));
	  else
		  m_vBase.push_back(-1);
  }

  //---------------------------------------------------------------------------------
  //Create class setting
  void ParserByteCode::SetCreateClassLP(CreateClass *pcreateclass)
  {
		m_pcreateclass = pcreateclass;
  }

//---------------------------------------------------------------------------
  /** \brief Add Strung function entry to the parser bytecode. 

      \throw nothrow

      A string function entry consists of the stack position of the return value,
      followed by a cmSTRFUNC code, the function pointer and an index into the 
      string buffer maintained by the parser.
  */
  void ParserByteCode::AddStrFun(void *a_pFun, int a_iArgc, int a_iIdx)
  {
    // String functions do not reduce the value stack position they increase it
    // because they are not applied to values on the stack, they create them.
    m_vBase.push_back(++m_iStackPos);
    m_vBase.push_back(cmFUNC_STR);
    m_vBase.push_back(a_iArgc);

    StorePtr(a_pFun);

    m_vBase.push_back(a_iIdx);
  }

  //---------------------------------------------------------------------------
  /** \brief Add end marker to bytecode.
      
      \throw nothrow 
  */
  void ParserByteCode::Finalize()
  {
    // yes we need the end code three times!!
    m_vBase.push_back(cmEND);	
    m_vBase.push_back(cmEND);	
    m_vBase.push_back(cmEND);	

    // shrink bytecode vector to fit
    storage_type(m_vBase).swap(m_vBase);
  }

  //---------------------------------------------------------------------------
  /** \brief Get Pointer to bytecode data storage. */
  const ParserByteCode::map_type* ParserByteCode::GetRawData() const
  {
    assert(m_vBase.size());
    return &m_vBase[0];
  }
  void ParserByteCode::SetStorageBase(storage_type Base)
  {
		m_vBase=Base;
  }
  void ParserByteCode::CollectRawData(storage_type* pBase)
  {
	  if(m_vBase.size()>=3)
	  {
		  //we pop the end code three times!!
		  m_vBase.pop_back();
		  m_vBase.pop_back();
 		  m_vBase.pop_back();
	  }
	  m_vBase.insert(m_vBase.end() ,pBase->begin(),pBase->end());
  }
	
  ParserByteCode::storage_type ParserByteCode::CollectStorageData(storage_type* pBase1,storage_type *pBase2)
  {
	  storage_type collectdata;
	  if(pBase1->size()>=3)
	  {
		  pBase1->pop_back();
		  pBase1->pop_back();
		  pBase1->pop_back();
	  }
	  collectdata=*pBase1;
	  collectdata.insert(collectdata.end(),pBase2->begin(),pBase2->end());
	  return collectdata;
  }

  ParserByteCode::storage_type ParserByteCode::GetStorageBaseData()
  {
	  return m_vBase;
  }
  
  ParserByteCode::storage_type * ParserByteCode::GetStorageBase() 
  {
	  return &m_vBase;
  }

  //---------------------------------------------------------------------------
  /** \brief Delete the bytecode. 
  
      \throw nothrow

      The name of this function is a violation of my own coding guidelines
      but this way it's more in line with the STL functions thus more 
      intuitive.
  */
  void ParserByteCode::clear()
  {
    m_vBase.clear();
    m_iStackPos = 0;
  }

  //---------------------------------------------------------------------------
  /** \brief Remove a value number of entries from the bytecode. 
    
      \attention Currently I don't test if the entries are really value entries.
  */
  void ParserByteCode::RemoveValEntries(unsigned a_iNumber)
  {
    unsigned iSize = a_iNumber * mc_iSizeValEntry;   
    //cxy add assert( m_vBase.size() >= iSize );
    m_vBase.resize(m_vBase.size()-iSize);

    //cxy add assert(m_iStackPos >= a_iNumber);
    m_iStackPos -= (a_iNumber);
  }

} // namespace mu


