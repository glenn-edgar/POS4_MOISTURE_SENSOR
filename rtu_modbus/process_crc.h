/*
**
** File: process_crc.h
**
**
*/

#ifndef _PROCESS_CRC_H_
#define _PROCESS_CRC_H_
#include "project.h"
    
int compute_crc( int in_message_length, uint8 *in_message);
void append_crc( int out_length, uint8 *out_buffer);

#endif