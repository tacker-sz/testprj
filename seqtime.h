


enum
{
	STOP,				// ptr off / power off / pwr 0%
	START_DELAY,		// ptr on / power off / pwr 0%
	IDLE,				// power on / pwr 0%
	TRAN_IL,			// pwr 0% Å® pwr low%
	LOW_DRIVE,			// pwr low%
	TRAN_LH,			// pwr low% Å® pwr high%
	HIGH_DRIVE,			// pwr high%
	TRAN_HL,			// pwr high% Å® pwr low%
}

#define	START_DELAY_TIME	10
#define	IDLE_DRIVE_TIME		10
#define	TRAN_IL_TIME		1
#define	LOW_DRIVE_TIME		10
#define	TRAN_LH_TIME		1
#define	HIGH_DRIVE			1
#define	TRAN_HL_TIME		1

#define	LOW_POWER_LEVEL		40	//MIN= 0
#define	HIGH_POWER_LEVEL	200	//MAX= 256


