#include "FuSMState.h" 

//---------------------------------------------------------
 
FuSMState::FuSMState(const std::string &statename,
					 int idnum,
					 int type,
					 Control* parent)
{
	m_type = type;
	m_parent = parent;
	m_activationLevel = 0;
	m_statename = statename;
	m_iStateID = idnum;


}

void FuSMState::Update(float dt)
{
	//put AI code here
}

void FuSMState::AddOutputState( FuSMState * OutputState )
{
	FuSMState *pState = NULL;
	int ioutputstatnum = m_outputvector.size();
	if( !m_outputvector.empty() )
	{
		for( int i=0;i< ioutputstatnum;i++ )
		{
			if(m_outputvector[i]==OutputState)
				return ;
		}
	}
	m_outputvector.push_back(OutputState);
}
// remove a state transation from the array
void FuSMState::DeleteOutputState( FuSMState *OutputState )
{
//no do
}
// get the output state and effect a transistion
void FuSMState::AddInputState( FuSMState * InputState )
{
	FuSMState *pState = NULL;
	int iinputstatnum = m_inputvector.size();
	if( !m_inputvector.empty() )
	{
		for( int i=0;i< iinputstatnum;i++ )
		{
			if(m_inputvector[i]==InputState)
				return ;
		}
	}
	m_inputvector.push_back(InputState);
}
// remove a state transation from the array
void FuSMState::DeleteInputState( FuSMState *OutputState )
{
	//no do
}
FuSMState*FuSMState::GetOutputState(const std::string outputstrname)
{
	FuSMState *pState = NULL;
	int ioutputstatnum = m_outputvector.size();
	// try to find this FSMstate in the map
	if( !m_outputvector.empty() )
	{
		for( int i=0;i< ioutputstatnum;i++ )
		{
			pState = m_outputvector[i];
			if(pState->m_statename==outputstrname)
				return pState;
		}
	}
	return( pState );
}
FuSMState*FuSMState::GetInputState(const std::string inputstrname)
{
	FuSMState *pState = NULL;
	int iinputstatnum = m_inputvector.size();
	// try to find this FSMstate in the map
	if( !m_inputvector.empty() )
	{
		for( int i=0;i< iinputstatnum;i++ )
		{
			pState = m_inputvector[i];
			if(pState->m_statename==inputstrname)
				return pState;
		}
	}
	return( pState );
}
std::string FuSMState::GetOutPutConnectString(const int &iID)
{ 
		int ioutputstatnum = m_outputvector.size();
	if(ioutputstatnum<=iID)
		return std::string("");

	FuSMState *pState = m_outputvector[iID];

	std::string strconnect = m_statename +std::string("_")+pState->m_statename;
	return strconnect;
}

std::string  FuSMState::GetOutPutStateString(const int &iID)
{
	int ioutputstatnum = m_outputvector.size();
	if(ioutputstatnum<=iID)
		return std::string("");

	FuSMState *pState = m_outputvector[iID];

	return   pState->m_statename;

}
