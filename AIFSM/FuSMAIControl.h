#ifndef __FUSMAICONTROL
#define __FUSMAICONTROL

#include "FuSM.h"
#include "AIControl.h"
#include<string>
//class Ship;
class FuSMMachine;
//class GameObj;

#define FU_POWERUP_SCAN_DIST 250.0f
#define FU_APPROACH_DIST     120.0f
#define FU_ATTACK_DIST       240.0f

class FuSMAIControl: public AIControl
{
public:
    //constructor/functions
    FuSMAIControl();
	~FuSMAIControl();
    void Update(float dt);
    void UpdatePerceptions(float dt);
    void Init();
	void AddState(const std::string &statename,int istatetype=FUSM_STATE_APPROACH);
	void AddTransition( const std::string &instatename ,const std::string &outstatename);

	void SetRunState(const std::string &statename);
	int GetRunStateConnectSum();
	std::string GetRunStateConnectName(const int &inum);
	std::string GetRunStateConnectStateName(const int &inum);
	//perception data 
    //(public so that states can share it)
    //GameObj*    m_nearestAsteroid;
    //GameObj*    m_nearestPowerup;
    //float       m_nearestAsteroidDist;
    //float       m_nearestPowerupDist;
    //bool        m_willCollide;
    //float       m_safetyRadius;
    
private:
    //data
    FuSMMachine* m_machine;
	std::string m_curstatename;
	int m_icurstateidnum;//
};
#endif
