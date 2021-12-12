/*************************************************************************//**
 *****************************************************************************
 * @file   alloc.c
 * @brief  
 * @author Shuwen Chen
 * @date   Tue Dec  6 09:40:13 2021
 *****************************************************************************
 *****************************************************************************/

#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"

PUBLIC void* alloc(unsigned int size)
{
    MESSAGE msg;
    msg.type = ALLOC;
    msg.MM_SIZE = size;

	send_recv(BOTH, TASK_MM, &msg);
	assert(msg.type == SYSCALL_RET);
	assert(msg.RETVAL == 0);

	return msg.MM_ADDR;
}

PUBLIC int free(void* addr)
{
    MESSAGE msg;
    msg.type = FREE;
    msg.MM_ADDR = addr;

    send_recv(BOTH, TASK_MM, &msg);
    assert(msg.type == SYSCALL_RET);
	
    return msg.RETVAL;
}

