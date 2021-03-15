#ifndef __FSTATEAPPROACH
#define __FSTATEAPPROACH

#include "FuSMState.h"

class FStateApproach : public FuSMState
{
public:
    //constructor/functions
	FStateApproach(const std::string &statename,int idnum,Control* parent):FuSMState(statename,idnum,FUSM_STATE_APPROACH,parent){}
    void Update(float dt);
    int CalculateActivation();
    void Exit();
};
#endif
