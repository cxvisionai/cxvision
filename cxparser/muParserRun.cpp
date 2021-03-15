
#include "muParserRun.h"
#include "muParserDef.h"
#include "muParserTest.h"

namespace mu
{
	namespace runmu
	{

		ParserRun::ParserRun():
		m_iVal(0)
		{
			
		}

		 ParserRun::~ParserRun()
		{
			 
		}
		void ParserRun::SetExpr(string str)
		{
			m_parser.SetExpr(str);
		}
		value_type ParserRun::Eval()
		{
			return m_parser.Eval();
		}
		double ParserRun::GetResult()
		{
			return m_parser.Eval();
		}
		void ParserRun::DefineVar(string str,double *dvalue)
		{
			m_parser.DefineVar(str,dvalue);
		} 
		//need to do
		//void ParserRun::DefineFunc(string str,)
		//{
		//	m_parser.DefineFun(str,)
		//}
		void ParserRun::SetVarFactory()
		{
			//CXListCtrl *pGrid=	reinterpret_cast<CXListCtrl *>(pView);
			m_parser.SetVarFactory((mu::facfun_type)&mu::runmu::ParserRun::AddVariable,this);
		}
		void ParserRun::FunTest(void *func)
		{
			m_Func=func;
			//*func;
		}

		double* ParserRun::AddVariable(const char *a_szName,void *pClass)
		{
			// I don't want dynamic allocation here, so i used this static buffer
			// If you want dynamic allocation you must allocate all variables dynamically
			// in order to delete them later on. Or you find other ways to keep track of 
			// variables that have been created implicitely.
				ParserRun *me= reinterpret_cast<ParserRun *>(pClass);
			// static double afValBuf[100];  
			// static int iVal = 0;          

			*(me->m_stream) << "Generating new variable \"" 
				<< a_szName << "\" (slots left: " 
				<< 99-me->m_iVal << ")" << "\r\n";

			me->m_afValBuf[me->m_iVal] = 0;
			if (me->m_iVal>=99)
				throw mu::ParserError("Variable buffer overflow.");

			return &(me->m_afValBuf[me->m_iVal++]);
		}

		ParserByteCode::storage_type ParserRun::GetByteCode()
		{
			return m_parser.GetStorageBase();
		}
		double ParserRun::RunByteCode(ParserByteCode::storage_type Base)
		{
			m_parser.m_vByteCodeCollection.SetStorageBase(Base);
			double dresult= m_parser.RunCollectionCmdCode();
			m_parser.ClearCollection();
			return dresult;
		}

		//////////////////////////////////////////////////////////////////////////
		bool ParserRun::CommandLine(string astr)
		{
			std::string sLine=astr;
			if(sLine=="quit")
			{
				exit(0);
			}
			else if(sLine=="help"||sLine=="Help")
			{
				ShowHelp();
				return true;
			}
			else if (sLine=="list var")
			{
				ListVar(&m_parser);
				return true;
			}
			else if (sLine=="list const")
			{
				ListConst(&m_parser);
				return true;
			}
			else if (sLine=="list exprvar")
			{
				ListExprVar(&m_parser);
				return true;
			}
			else if (sLine=="list const")
			{
				ListConst(&m_parser);
				return true;
			}
			else if(sLine=="list func")
			{
				ListFunction(&m_parser);
				return true;
			}
			else if(sLine=="list class")
			{
				ListClass( m_parser);
				return true;
			}
			else if(sLine =="run")
			{
				m_parser.RunCode();

				////std::ostringstream os;
				*m_stream <<"create code :"<<"\r\n";
				*m_stream <<m_parser.m_StrCollection;
				//CString cstrget;
				//GetWindowText(cstrget);
				//SetWindowText(LPCTSTR(cstrget+CString(os.str().c_str())));
				//GetEditCtrl().SetSel(EM_AUTOURLDETECT, -1);
				return true;
			}
			else if(sLine=="open collec")
			{
				m_parser.SetColllection(true);
				return true;
			}
			else if(sLine=="close collec")
			{
				m_parser.SetColllection(false);
				return true;
			}
			else if(sLine=="clear collec")
			{
				m_parser.ClearCollection();
				return true;
			}
			else if(sLine=="clear")
			{
				//SetWindowText("");
				return true;
			}
			else if(sLine=="clear var")
			{
				m_parser.ClearVar();
				return true;
			}
			else if(sLine=="test")
			{
				SelfTest();
				return true;
			}
			else if(sLine=="Set VarFact")
			{//设置变量工厂 可以自生成未知变量为 默认类型（double）变量，前提是类定义使用关闭（close classdef）
				SetVarFactory();
				return true;
			}
			else if(sLine=="open classdef")
			{
				m_parser.UsingClass(true);
				return true;
			}
			else if(sLine=="close classdef")
			{
				m_parser.UsingClass(false);
				return true;
			}
			else if(sLine=="ini test")//初始化
			{
				double *apdouble=0;
				m_parser.DefineOrgClass("double",apdouble);
				 
				m_parser.UsingClass(true);
				return true;
			}

			return false;
		}
		void ParserRun::ShowHelp()
		{
			*m_stream<<"!-----------------help------------------- "<<"\r\n";
			*m_stream << "Commands:\r\n";
			*m_stream << "  list var     - list parser variables\r\n";
			*m_stream << "  list exprvar - list expression variables\r\n";
			*m_stream << "  list const   - list all numeric parser constants\r\n";
			*m_stream << "  exit         - exits the parser\r\n";
 
			*m_stream << "  list func    - list parser express function"<<"\r\n";
			*m_stream << "  list class   - list parser class define and class var"<<"\r\n";
			

			*m_stream << "  open collec  - open a vector to collection expression"<<"\r\n";
			*m_stream << "  close collec - close vector collection"<<"\r\n";
			*m_stream << "  clear collec - clear expression vector"<<"\r\n";
			*m_stream << "  run          - run collection expression"<<"\r\n";

			*m_stream << "  clear       -clear output screen\r\n"<<"\r\n";
			*m_stream << "  clear var   -clear variables"<<"\r\n";

			*m_stream << "  open classdef  - make parser to recognize class define"<<"\r\n";
			*m_stream << "  close classdef - close recognize class define"<<"\r\n";


			*m_stream << "  test        -self test "<<"\r\n";
			*m_stream << "  ini test	-operate the parser"<<"\r\n";

			*m_stream << "Constants:\r\n";
			*m_stream << "  \"_e\"   2.718281828459045235360287\r\n";
			*m_stream << "  \"_pi\"  3.141592653589793238462643\r\n";
			*m_stream << "---------------------------------------\r\n";
			*m_stream << "Enter a formula or a command:\r\n";
		}
		//---------------------------------------------------------------------------
		// Factory function for creating new parser variables
		// This could as well be a function performing database queries.
		//double*  ParserRun::AddVariable(const char *a_szName)
		//{
		//	//std::ostringstream os;
		//	// I don't want dynamic allocation here, so i used this static buffer
		//	// If you want dynamic allocation you must allocate all variables dynamically
		//	// in order to delete them later on. Or you find other ways to keep track of 
		//	// variables that have been created implicitely.
		//	static double afValBuf[100];  
		//	static int iVal = 0;          
		//	*m_stream << "Generating new variable \"" 
		//		<< a_szName << "\" (slots left: " 
		//		<< 99-iVal << ")" << "\r\n";
		//	afValBuf[iVal] = 0;
		//	if (iVal>=99)
		//		throw mu::ParserError("Variable buffer overflow.");
		//	return &afValBuf[iVal++];
		//}

		//---------------------------------------------------------------------------
		void  ParserRun::SelfTest()
		{
			mu::Test::ParserTester pt;
			pt.Run();
		}

		//---------------------------------------------------------------------------
		void  ParserRun::ListVar(const mu::Parser  *Pparser)
		{
			//std::ostringstream os;
			// Query the used variables (must be done after calc)
			mu::varmap_type variables = Pparser->GetVar();
			if (!variables.size())
				return;
			*m_stream << "\nParser variables:\r\n";
			*m_stream <<   "-----------------\r\n";
			*m_stream << "Number: " << (int)variables.size() << "\r\n";
			varmap_type::const_iterator item = variables.begin();
			for (; item!=variables.end(); ++item)
			{
				*m_stream << "Name: " << item->first << "   Address: [0x" << item->second << "]  ";
				m_parser.SetExpr(item->first);
				*m_stream << "Result: "<<Pparser->Eval()<<"\r\n";
			} 
		}

		//---------------------------------------------------------------------------
		void  ParserRun::ListConst(const mu::Parser * Pparser)
		{
			//std::ostringstream os;
			*m_stream << "\nParser constants:\r\n";
			*m_stream <<   "-----------------\r\n";
			mu::valmap_type cmap = Pparser->GetConst();
			if (!cmap.size())
			{
				*m_stream << "Expression does not contain constants\r\n";
			}
			else
			{
				valmap_type::const_iterator item = cmap.begin();
				for (; item!=cmap.end(); ++item)
					*m_stream << "  " << item->first << " =  " << item->second << "\r\n";
			} 
		}

		//---------------------------------------------------------------------------
		void  ParserRun::ListExprVar(const mu::Parser * Pparser)
		{
			//std::ostringstream os;
			std::string sExpr = Pparser->GetExpr();
			if (sExpr.length()==0)
			{
				*m_stream << "Expression string is empty\r\n";
				return;
			}
			// Query the used variables (must be done after calc)
			*m_stream << "\nExpression variables:\r\n";
			*m_stream <<   "---------------------\r\n";
			*m_stream << "Expression: " <<Pparser->GetExpr() << "\r\n";
			varmap_type variables =Pparser->GetUsedVar();
			if (!variables.size())
			{
				*m_stream << "Expression does not contain variables\r\n";
			}
			else
			{
				*m_stream << "Number: " << (int)variables.size() << "\r\n";
				mu::varmap_type::const_iterator item = variables.begin();
				for (; item!=variables.end(); ++item)
					*m_stream << "Name: " << item->first << "   Address: [0x" << item->second << "]\r\n";
			}
		}
		//---------------------------------------------------------------------------
		void  ParserRun::ListFunction(const mu::Parser  *pParser)
		{
			// Make the type of the map containing function prototypes visible
			using mu::funmap_type;       
			funmap_type funmap = pParser->GetFunDef();
			funmap_type::const_iterator item;
			*m_stream << "\nFunctions available:\r\n";
			// iterate over all function definitions
			for (item=funmap.begin(); item!=funmap.end(); ++item)
			{
				*m_stream << "  " << item->first;   // This is the Function Name
				*m_stream << "(";
				// Deal with different argument numbers
				int iArgc = item->second.GetArgc();
				if (iArgc>=0)
				{
					// Indicate number of input variables
					for (int i=0; i<iArgc; ++i)
					{
						char cVar[] = "val ";
						cVar[3] = '1' + (char)i;
						*m_stream << cVar;
						if (i!=iArgc-1)
							*m_stream << ",";
					}
				}
				else
				{
					*m_stream << "...";  // Multiargument function
				} 
				*m_stream << ")\r\n";
			}

		}

		//----------------------------------------------------------------------------
		void ParserRun::ListClass(mu::Parser  &Pparser)
		{
			mu::classbasemap_type classmap = Pparser.GetClassMap();
			if(!classmap.size())
				return;
			*m_stream << "\nParser Class:\r\n";
			*m_stream <<   "-----------------\r\n";
			*m_stream << "Number: " << (int)classmap.size() << "\r\n";
			classbasemap_type::const_iterator item = classmap.begin();
			for (; item!=classmap.end(); ++item)
			{
				*m_stream << "Class Name: " << item->first << "   Address: [0x" << item->second << "]  "<< "\r\n";
				classbase *pclass=(item->second);

				*m_stream << "      variables Number: " <<  (int)pclass->size() << "\r\n";			
				for(int i=0;i<pclass->size();i++)
				{
					*m_stream << "            variables Name: " << pclass->getvar(i) <<"   Address: [0x" 
						<< pclass->getvarpoint(i)<< "]  "<< "\r\n";
				}
			}
			*m_stream <<"\r\n";
		}
		//----------------------------------------------------------------------------
		void ParserRun::ListFormula(mu::Parser &Pparser)
		{
			mu::string_type astring=Pparser.GetFormula();
			*m_stream << " Formula: " <<  astring << "\r\n";
		}
		//---------------------------------------------------------------------------
		void ParserRun::SetStream(std::ostream *a_stream)
		{
			assert(a_stream);
			m_stream = a_stream;
		}
		//---------------------------------------------------------------------------
		void ParserRun::Compile(const char *a_szLine)	
		{
			//string astr=str;
			try
			{
				if (CommandLine(a_szLine)) 
				{
					m_iget=0;
					return;
				}
				//else
				{
					SetExpr(a_szLine);
					*m_stream<<"Result:"<<Eval()<<"\r\n";
				}
			}
			catch(mu::Parser::exception_type &e)
			{
				*m_stream  << "\nError:\r\n";
				*m_stream  << "------\r\n";
				*m_stream  << "Message:  " << e.GetMsg() << "\r\n";
				//*m_stream  << "Formula:  " << e.GetExpr() << "\r\n";//20110411cxy change:
				*m_stream  << "Token:    " << e.GetToken() << "\r\n";
				*m_stream  << "Position: " << (int)e.GetPos() << "\r\n";
				*m_stream  << "Errc:     " << e.GetCode() << "\r\n";
			}
		}
	}

}
