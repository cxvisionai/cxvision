#ifndef MU_PARSERRUN_H
#define MU_PARSERRUN_H
#include "muParser.h"
#include <string>
using namespace std;

#if defined(_WIN32x)
#ifdef _EX
#define   __declspec(dllexport)
#else
#define   __declspec(dllimport)
#endif
#endif

namespace mu
{

	namespace runmu
	{

		class  ParserRun
		{
		public:
			ParserRun(void);
		public:
			~ParserRun(void);

		public:
			mu::Parser  m_parser;
 
			mu::Parser *GetParser()
			{
				return &m_parser;
			}

			value_type Eval();
			void SetExpr(string str);
			double GetResult();
			void DefineVar(string str,double *dvalue);
			static double* AddVariable(const char *a_szName,void *pClass);
			void SetVarFactory();
			ParserByteCode::storage_type GetByteCode();
			double RunByteCode(ParserByteCode::storage_type Base);
   
 
		private:
			double m_afValBuf[100];  
			int m_iVal; 	
			char m_line[100];
			int m_iget;
			void ListFunction(const mu::Parser  * pParser);
			void ListExprVar(const mu::Parser *parser);
			void ListConst(const mu::Parser *parser);
			void ListVar(const mu::Parser *parser);
			void ListClass(mu::Parser  &Pparser);
			void ListFormula(mu::Parser &parser);
		public:
			void FunTest(void *func);
			void * m_Func;
			void SelfTest();

			bool CommandLine(string szLine);
			void ShowHelp();
			void Calc(const char *pszFormula);
			void Compile(const char *a_szLine);
			//
			void SetStream(std::ostream *a_stream);

		private:
			std::ostream *m_stream;
		};
	}

}

#endif
