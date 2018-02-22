/*
 * cyg_debug.h
 *
 * Created: 2011/8/25 14:17:08
 *  Author: Steven
 */ 


#ifndef CYG_DEBUG_H_
#define CYG_DEBUG_H_

#if CYG_DEBUG

extern FILE gfile_dbg;

#define DbgPrint(format, args...)		fprintf(&gfile_dbg, (format) , ## args)

void cygOpenDebugPort(void);
void cygCloseDebugPort(void);
void cygTestDebugPort(void);

#else

#define DbgPrint(...)

#endif


#endif /* CYG_DEBUG_H_ */