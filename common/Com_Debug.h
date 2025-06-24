#ifndef __COM_DEBUG_H
#define __COM_DEBUG_H

#include "usart.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

//调试开关,上线时注释掉
#define DEBUG_ENABLE

#ifdef DEBUG_ENABLE


#define FILE__NAME__ (strrchr(__FILE__,'\\') ? strrchr(__FILE__,'\\') + 1 : __FILE__)
#define __FILE__NAME__ (strrchr(FILE__NAME__,'/') ? strrchr(FILE__NAME__,'/') + 1 : FILE__NAME__)

#define debug_printf(format,...) printf("[%s:%d]"format,__FILE__NAME__,__LINE__,##__VA_ARGS__)
#define debug_printfln(format,...) printf("[%s:%d]"format"\r\n",__FILE__NAME__,__LINE__,##__VA_ARGS__)

#else 

//空宏定义，确保编译通过
#define FILE__NAME__ 
#define __FILE__NAME__ 

#define debug_printf(format,...) 
#define debug_printfln(format,...) 

#endif


void Com_Debug_Init(void);




#endif
