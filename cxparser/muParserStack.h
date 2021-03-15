 /*
  Copyright (C) 2004, 2005 Ingo Berg

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
#ifndef MU_PARSER_STACK_H
#define MU_PARSER_STACK_H

#include <cassert>
#include <string>
#include <stack>
#include <queue>

#include "muParserError.h"
#include "muParserToken.h"

namespace mu
{

/** \brief Parser stack implementation. 

 Stack implementation based on a std::stack. The behaviour of pop() had been
 slightly changed in order to get an error code if the stack is empty.
 The stack is used within the Parser both as a value stack and as an operator stack.

 \author (C) 2004, 2005 Ingo Berg 
*/
template <typename TValueType>
class ParserStack
{
  private:
     /** \brief Type of the underlying stack implementation. */
    //typedef std::stack<TValueType, std::vector<TValueType> > impl_type;

    typedef std::deque<TValueType> impl_type;
    impl_type m_Stack;

  public:	
	 
     //---------------------------------------------------------------------------
     ParserStack()
       :m_Stack()
     {
     }

     //---------------------------------------------------------------------------
     virtual ~ParserStack()
     {
     }


     //---------------------------------------------------------------------------
     /** \brief Pop a value from the stack.
     
       Unlike the standard implementation this function will return the value that
       is going to be taken from the stack.

       \throw ParserException in case the stack is empty.
       \sa pop(int &a_iErrc)
     */
	 TValueType pop()
     {
       if (empty())
         throw ParserError("stack is empty.");

       TValueType el = top();
       m_Stack.pop_back();
       return el;
     }

     /** \brief Push an object into the stack. 

         \param a_Val object to push into the stack.
         \throw nothrow
     */
     void push(const TValueType& a_Val) 
     { 
       //m_Stack.push(a_Val);
       m_Stack.push_back(a_Val);
     }


     /** \brief Return the number of stored elements. */
     unsigned size() const
     { 
       return (unsigned)m_Stack.size(); 
     }

     /** \brief Returns true if stack is empty false otherwise. */
     bool empty() const
     {
       return m_Stack.size()==0; 
     }
     
     /** \brief Return reference to the top object in the stack. 
         The top object is the one pushed most recently.
     */
     TValueType& top() 
     { 
         return m_Stack[size()-1];
      // return m_Stack.top();
     }
	 //  [10/24/2012 cxy]
     TValueType& operator[](unsigned a_iTdx)
     {
        return	m_Stack[a_iTdx];
     }

	 //  [10/26/2012 cxy]
    TValueType* GetAt(unsigned a_iTdx)
    {
       return &m_Stack[a_iTdx];
    }
	 //  [10/31/2012 cxy] 
	 void Assign(const ParserStack &a_stack)
	 {

	 }
	/*
	*/
	void clear()
	{
        m_Stack.clear();
    //	int isum=m_Stack.size();
    //	for(int i=0;i<isum;i++)
    //	{
    //		m_Stack.pop();
    //	}
	}
};
} // namespace MathUtils

#endif
