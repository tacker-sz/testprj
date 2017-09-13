
#include seqtime.h

enum
{
	STOP,				// ptr off / power off / pwr 0%
	START_DELAY,		// ptr on / power off / pwr 0%
	IDLE,				// power on / pwr 0%
	TRAN_IL,			// pwr 0% → pwr low%
	LOW_DRIVE,			// pwr low%
	TRAN_LH,			// pwr low% → pwr high%
	HIGH_DRIVE,			// pwr high%
	TRAN_HL,			// pwr high% → pwr low%
}


#define	IDLE_POWER_LEVEL	0	//MIN= 0
#define	LOW_POWER_LEVEL		40	//MIN= 0
#define	HIGH_POWER_LEVEL	200	//MAX= 256

long steptime[]{
	0,	// STOP,				// ptr off / power off / pwr 0%
	10	// START_DELAY,		// ptr on / power off / pwr 0%
	10	// IDLE,				// power on / pwr 0%
	1	// TRAN_IL,			// pwr 0% → pwr low%
	10	// LOW_DRIVE,			// pwr low%
	1	// TRAN_LH,			// pwr low% → pwr high%
	2	// HIGH_DRIVE,			// pwr high%
	1	// TRAN_HL,			// pwr high% → pwr low%
}


long get_machinetime(void)			// カウンタ値⇒
{
	return TCX;
}

void reset_machinetime(void)
{
	
}

long get_levelsteptimecount(start, end, seqtime)
{
	if (end == start) {
		return 0;
	}
	else {
		return seqtime / (end - start);
	}
}


time = get_machinetime();

//endless
if(time >= THTIME){
	reset_machinetime();
	continue;
}


// photo DI sens
level = analogRead(0)
if(level > DOOR_OPEN){
	seq = STOP;					// ptr off / power off / pwr 0%
	power_off();
	reset_machinetime();
	nextseq_time = steptime[seq];
}
else{							// Door Close
	if(nextseq_time > time){	// seq
		switch(seq){
			case STOP:			// ptr off / power off / pwr 0%
				break;
			case START_DELAY:	// ptr on / power off / pwr 0%
				break;
			case IDLE:			// power on / pwr 0%
				break;
			case TRAN_IL:		// pwr 0% → pwr low%
				powerlevel += powerstep;
				power_(powerlevel);
				break;
			case LOW_DRIVE:		// pwr low%
				break;
			case TRAN_LH:		// pwr low% → pwr high%
				powerlevel += powerstep;
				power_(powerlevel);
				break;
			case HIGH_DRIVE:	// pwr high%
				break;
			case TRAN_HL:		// pwr high% → pwr low%
				powerlevel += powerstep;
				power_(powerlevel);
				break;
		}
	}
	else{						// transition seq time up
		switch(seq){
			case STOP:			// ptr off / power off / pwr 0%
				seq++;
				power_off();
				break;
			case START_DELAY:	// ptr on / power off / pwr 0%
				seq++;
				powerlevel = IDLE_POWER_LEVEL;
				power_(powerlevel);
				power_on();
				break;
			case IDLE:			// power on / pwr 0%
				seq++;
				powerstep = get_levelsteptimecount(IDLE_POWER_LEVEL, LOW_POWER_LEVEL, steptime[seq]);
				break;
			case TRAN_IL:		// pwr 0% → pwr low%
				seq++;
				powerstep = LOW_POWER_LEVEL;
				power_(LOW_POWER_LEVEL);
				break;
			case LOW_DRIVE:		// pwr low%
				seq++;
				powerstep = get_levelsteptimecount(LOW_POWER_LEVEL, HIGH_POWER_LEVEL, steptime[seq]);
				break;
			case TRAN_LH:		// pwr low% → pwr high%
				seq++;
				powerstep = HIGH_POWER_LEVEL;
				power_(HIGH_POWER_LEVEL);
				break;
			case HIGH_DRIVE:	// pwr high%
				seq++;
				powerstep = get_levelsteptimecount(HIGH_POWER_LEVEL, LOW_POWER_LEVEL, steptime[seq]);
				break;
			case TRAN_HL:		// pwr high% → pwr low%
				seq = LOW_DRIVE;
				powerstep = LOW_POWER_LEVEL;
				power_(LOW_POWER_LEVEL);
				break;
		}
		reset_machinetime();
		nextseq_time = steptime[seq];
	}
}


