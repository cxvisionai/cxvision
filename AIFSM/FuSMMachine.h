#ifndef __FUSMMACHINE
#define __FUSMMACHINE

#include <vector>
#include <string>
#include "FuSMState.h"
#pragma warning(disable: 4786)


class FuSMMachine//: public FuSMState
{
public:
    //constructor/functions
    FuSMMachine(int type = FUSM_MACH_NONE,Control* parent = NULL);
	~FuSMMachine();
    virtual void UpdateMachine(float dt);

    void AddState(FuSMState* state);
	void AddApproachState(const std::string &statename,int idnum);
	FuSMState* GetState(int istateid);
	void AddTransition( FuSMState *InputState, FuSMState *OutputState );// add a state transition to the array
	
	void AddTransition( const std::string &instatename ,const std::string &outstatename);

	virtual bool IsActive(FuSMState *state);
    virtual void Reset();
    
	int GetStateSum(){return m_states.size();}

	void ReleaseState();
	void SetRunState(FuSMState *state){m_curstate = m_curstate;};
	void SetRunState(const std::string &statename);

	int GetCurStateOutPutSum();
	std::string GetCurStateConnectString(const int &iID);
	std::string GetRunStateConnectStateName(const int &iID);
    //data
	int m_type;
	Control*   m_parent;
protected:
	FuSMState * m_curstate;
    std::vector<FuSMState*> m_states;
    std::vector<FuSMState*> m_activatedStates;
};
#endif
