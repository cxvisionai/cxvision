#ifndef __FUSMSTATE
#define __FUSMSTATE

#include "FuSM.h"
#include <stdlib.h>
#include <string>
#include <vector>
class Control;
class FuSMState;

typedef std::vector<FuSMState*>  State_Vector;
typedef State_Vector::iterator State_Itor; // iterator for use in accessing member states


class FuSMState
{
public:
    //constructor/functions
    FuSMState(const std::string &statename,
		int idnum,
		int type = FUSM_STATE_NONE,
		Control* parent = NULL);
    virtual void Update(float dt);
	virtual void Enter(){} 
    virtual void Exit(){}
    virtual void Init(){m_activationLevel = 0 ;}
    virtual int CalculateActivation(){return m_activationLevel;}

	// access the state ID
	int GetID() { return m_iStateID; }
	// get the output state and effect a transistion
	void AddOutputState( FuSMState * OutputState );
	// remove a state transation from the array
	void DeleteOutputState( FuSMState *OutputState );
   
	// get the output state and effect a transistion
	void AddInputState( FuSMState * OutputState );
	// remove a state transation from the array
	void DeleteInputState( FuSMState *OutputState );

	FuSMState*GetOutputState(const std::string outputstrname);
	FuSMState*GetInputState(const std::string inputstrname);

	int  GetOutputSum()const{return m_outputvector.size();}
	std::string GetOutPutConnectString(const int &iID);
	std::string GetOutPutStateString(const int &iID);

	//data
    Control*   m_parent;
    int        m_type;
    int		  m_activationLevel;
	std::string m_statename;

	int m_iStateID;						// the unique ID of this state

	State_Vector m_inputvector;
	State_Vector m_outputvector;	
};
#endif
