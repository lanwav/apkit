/*
 * cyg_ant.h
 *
 * Created: 2013/3/22 16:26:52
 *  Author: win7
 */ 


#ifndef CYG_ANT_H_
#define CYG_ANT_H_

#if CYG_FEATURE_ANT

#include <stdio.h>

extern FILE gfile_ant;

void cygOpenANT(void);
void cygCloseANT(void);
int cygRcvANTDataIsReady(void);

#define ANT_IMP_KIT     1
#define ANT_IMP_SENSOR_ENVIRONMENT  0

#endif

#endif /* CYG_ANT_H_ */