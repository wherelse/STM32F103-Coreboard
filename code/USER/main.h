#ifndef __MAIN_H
#define __MAIN_H 
 

extern int CAN_CLIENT ;

extern float EncoderPulse;
extern int Pluse;
extern float BusVoltage , BusCurrent , BusPower ;
extern unsigned char FlashSaveFlag;
extern int Tempture;
void FlashSaveData(void);
void FlashInitData(void);
void Beeptimes(unsigned char times);
void SendviaCan(void);

#endif


