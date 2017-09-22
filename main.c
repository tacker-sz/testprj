
#include seqtime.h

#define FAN_POWPIN	13
#define FAN_PLSPIN	12
#define FAN_CTLPIN	3

void init_fan(void)
{
	// port out
	// PWM mode
	// counter interrupt
}

#define FAN_STOP		-1
#define FAN_FULLSPEED	79
voit set_fan(int val)	// -1:off, 0-79:on(variable), 80-127:on(full)
{
	// OFF
	if (val<0) {
		degitalout(powpin, 0);
	}
	else {
	// ON
		OCRB = val;
		degitalout(powpin, 1);
	}
}



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


// 16e06 /(4*160)= 25e3
// 16e06 /(8*(1+79))= 25e3
// 16e06 /(2*8*40)= 25e3
fOCnxPWM = fclk_I/O / (N×(1+TOP))
fOCnxPCPWM = fclk_I/O / (2×N×TOP)
/*
TCCR2A:	COM2A1	COM2A0	COM2B1	COM2B0	   -   	  -  	WGM2_1	WGM2_0
TCCR2B:	FOC2A	FOC2B	  -  	  -  	WGM2_2	CS_2	CS_1	CS_0
*/


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



volatile unsigned gFanCount;
100 - 10 clt/sec

void interruptfan(void)
{
	cil();  //割り込み禁止
	gFanCount++;
	sei();    //割り込み許可
}

void reset_fanclk(void)
{
	cil();  //割り込み禁止
	gFanCount=0;
	sei();    //割り込み許可
}




// 1～10秒掛かる。100回転分の所要時間から算出。
inline unsinged short chk_fanrotation(unsigned long machinecount)
{
	static prev_machnecount;
	static unsinged short rotate=0;
	
	if(gFanCount>200){
		count = gFanCount;
		reset_fanclk();
		if(machinecount > prev_machnecount){
			rotate = (3000 * (usigned long)count) / (machinecount - prev_machnecount);
		}
		prev_machnecount = machinecount;
	}
	else{
		if(machinecount > prev_machnecount){
			if((machinecount - prev_machnecount) > 10000){	//10秒以上
				if(gFanCount < 20){							//10回転しない
					prev_machnecount = machinecount;
					reset_fanclk();
					rotate = 0;
				}
				else{
					count = gFanCount;
					reset_fanclk();
					rotate = (3000 * (usigned long)count) / (machinecount - prev_machnecount);
					prev_machnecount = machinecount;
				}
			}
		}
	}
	return (unsinged short)rotate;
}

10bps

1110_10101010_1100	start
1110_01010101_1100	end
1110_01100110_1100	get

1110_0000_1010 - 01_1111_10
1110_0000_1010 - 01_1111_10
1110_0000_1010 - 01_1111_10
1110_0000_1010 - 01_1111_10



hiで開始
25msで128bitサンプル4本に記録
スタート位置は初回ダウンビットの１後のサイクル
 1111 1111 1111 0000
                 ^  

スタート条件：
・1が11以上続く事。
・0が16*4以上続く事。

データ条件：
・同じ値は最長9（同バッファ内2）
・↑エッジ(01)を１、↓エッジ(10)を０とする。

ストア条件：
・前サイクルと同じ値である事。
・2つ後、３つ後は飛ばす。

エンド条件：
・0が12サイクル続いた場合



