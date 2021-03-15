#ifndef __FUSM
#define __FUSM

//FuSM state types
enum
{
	FUSM_STATE_NONE=0,
	FUSM_STATE_APPROACH,//一般运行状态机
	FUSM_STATE_REONE,//运行后回转到前一状态
	FUSM_STATE_PALRUN,//运行并发多个后续状态
	FUSM_STATE_END//结束状态机
};

//FuSM machine types
enum
{
	FUSM_MACH_NONE,
	FUSM_MACH_SAUCER,
    FUSM_MACH_COUNT
};


#endif
