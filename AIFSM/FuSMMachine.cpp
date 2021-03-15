#include "FuSMMachine.h"


#include "FStateApproach.h"
//---------------------------------------------------------
FuSMMachine::FuSMMachine(int type,Control* parent)
{
	m_type = type;
	m_parent = parent;
	m_curstate = NULL;
}
//----------------------------------------------------
FuSMMachine::~FuSMMachine()
{
	ReleaseState();
}

//----------------------------------------------------
void FuSMMachine::ReleaseState()
{
	int istatesum = m_states.size();
	for(int i=0;i<istatesum;i++)
	{
		FuSMState *pstate = m_states[i];
		delete pstate;
		pstate =NULL;
	}
}
//---------------------------------------------------------
void FuSMMachine::UpdateMachine(float dt)
{
    //don't do anything if you have no states
    if(m_states.size() == 0)
        return;
    //check for activations, and then update
    m_activatedStates.clear();
    std::vector<FuSMState*> nonActiveStates;
    for(int i =0;i<m_states.size();i++)
    {
        if(m_states[i]->CalculateActivation() > 0)
            m_activatedStates.push_back(m_states[i]);
        else
            nonActiveStates.push_back(m_states[i]);
    }

    //Exit all non active states for cleanup
    if(nonActiveStates.size() != 0)
    {
        for(int i =0;i<nonActiveStates.size();i++)
            nonActiveStates[i]->Exit(); 
    }
    
    //Update all activated states
    if(m_activatedStates.size() != 0)
    {
        for(int i =0;i<m_activatedStates.size();i++)
            m_activatedStates[i]->Update(dt);   
    }
}

//---------------------------------------------------------
void FuSMMachine::AddApproachState(const std::string &statename,int idnum)
{
	FStateApproach *pstate = new FStateApproach(statename,idnum,m_parent);
	AddState(pstate);
}
//---------------------------------------------------------
void FuSMMachine::AddState(FuSMState* state)
{
	if(state==NULL)
		return;
	for(int i=0;i<m_states.size();i++)
	{
		if(state==m_states[i])
			return;
	}

	m_states.push_back(state);
}
//---------------------------------------------------------
FuSMState*FuSMMachine::GetState(int istateid)
{
	for(int i=0;i<m_states.size();i++)
	{
		if(istateid==m_states[i]->m_iStateID)
			return m_states[i]  ;
	}
return NULL;
}
//---------------------------------------------------------
void FuSMMachine::AddTransition(FuSMState *InputState, FuSMState *OutputState )
{
	if(InputState==NULL
		||OutputState==NULL)
		return;
	InputState->AddOutputState(OutputState);
	OutputState->AddInputState(InputState);
}
void FuSMMachine::AddTransition( const std::string &instatename ,const std::string &outstatename)
{
	FuSMState*pinputstate =	NULL;
	FuSMState*poutputstate = NULL;

	if(instatename==outstatename)
		return;
	for(int i =0;i<m_states.size();i++)
	{
		if(m_states[i]->m_statename==instatename)
			pinputstate = m_states[i];
		 if(m_states[i]->m_statename==outstatename)
			 poutputstate = m_states[i];
	}
	if(pinputstate!=NULL&&poutputstate!=NULL)
		AddTransition(pinputstate, poutputstate );
}
//---------------------------------------------------------

//---------------------------------------------------------
bool FuSMMachine::IsActive(FuSMState *state)
{
    if(m_activatedStates.size() != 0)
    {
        for(int i =0;i<m_activatedStates.size();i++)
            if(m_activatedStates[i] == state)
				return true;   
    }
	return false;
}

//---------------------------------------------------------
void FuSMMachine::Reset()
{
	//exit and init all the states
	for(int i =0;i<m_states.size();i++)
    {
        m_states[i]->Exit();
        m_states[i]->Init();
    }
	ReleaseState();
	m_states.clear();
}
//----------------------------------------------------------
void FuSMMachine::SetRunState(const std::string &statename)
{
	for(int i=0;i<m_states.size();i++)
	{
		if(statename==m_states[i]->m_statename)
		{
			m_curstate =  m_states[i];
		}
	}
}
int FuSMMachine::GetCurStateOutPutSum()
{
	if(m_curstate!=NULL)
	{
		return m_curstate->GetOutputSum();
	}
	return 0;
}
std::string FuSMMachine::GetCurStateConnectString(const int &iID)
{
	if(m_curstate!=NULL)
	{
		return m_curstate->GetOutPutConnectString(iID);
	}
	return std::string("");
}
std::string FuSMMachine::GetRunStateConnectStateName(const int &iID)
{
	if(m_curstate!=NULL)
	{
		return m_curstate->GetOutPutStateString(iID);
	}
	return std::string("");
}
