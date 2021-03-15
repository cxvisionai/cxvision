#include "FuSMAIControl.h"
#include "FuSMMachine.h"
//#include "GameSession.h"
//#include "TestSession.h"
//#include "utility.h"

//#include "FStateAttack.h"
//#include "FStateEvade.h"
//#include "FStateGetPowerup.h"

//#include "Target.h"


//---------------------------------------------------------
FuSMAIControl::FuSMAIControl():
AIControl()
{
    //construct the state machine and add the necessary states
    m_machine = new FuSMMachine(FUSM_MACH_SAUCER,this);

    m_machine->Reset();
	m_icurstateidnum =0;
}
FuSMAIControl::~FuSMAIControl()
{
	delete m_machine;
}
void FuSMAIControl::AddState(const std::string &statename,int istatetype)
{
	switch(istatetype)
	{
	case FUSM_STATE_NONE:
		break;
	case FUSM_STATE_APPROACH:

		m_machine->AddApproachState(statename,m_icurstateidnum);

		m_icurstateidnum ++;
		break;
	case FUSM_STATE_REONE:
		break;
	case FUSM_STATE_PALRUN:
		break;
	case FUSM_STATE_END:
		break;
	}
}

//----------------------------------------------------
void FuSMAIControl::AddTransition( const std::string &instatename ,
								  const std::string &outstatename)
{
	m_machine->AddTransition(instatename,outstatename);
}
//---------------------------------------------------------
//设置运行状态机
void FuSMAIControl::SetRunState(const std::string &statename)
{
	                        
	m_machine->SetRunState(statename);
}
//---------------------------------------------------------
int FuSMAIControl::GetRunStateConnectSum()
{
	return m_machine->GetCurStateOutPutSum();
}
//----------------------------------------------------------
std::string FuSMAIControl::GetRunStateConnectName(const int&inum)
{
	return m_machine->GetCurStateConnectString(inum);
}
//----------------------------------------------------------
std::string FuSMAIControl::GetRunStateConnectStateName(const int&inum)
{
	return m_machine->GetRunStateConnectStateName(inum);
}
//---------------------------------------------------------
//---------------------------------------------------------
void FuSMAIControl::Init()
{
    //m_willCollide  = false;
    //m_nearestAsteroid = NULL;
    //m_nearestPowerup  = NULL;
    //m_safetyRadius    = 15.0f;

    //if(!m_target)
    //{
    //    m_target = new Target;
    //    m_target->m_size = 1;
    //    Game.PostGameObj(m_target);
    //}
	m_machine->Reset();
}

//---------------------------------------------------------
void FuSMAIControl::Update(float dt)
{
    //if(!m_ship)
    //{
    //    m_machine->Reset();
    //    return;
    //}
    
    UpdatePerceptions(dt);
    m_machine->UpdateMachine(dt);
}

//---------------------------------------------------------
void FuSMAIControl::UpdatePerceptions(float dt)
{
}

