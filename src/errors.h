/******************************************************************************************
* Copyright (C) 2019 by Victor Kronberg
*
* Redistribution, modification or use of this software in source or binary forms is 
* permitted as long as files maintain this copyright. Victor Kronberg and the 
* University of Colorado are not liable for any misuse of this material.
*
*******************************************************************************************
**
* @file errors.h
* @brief Error enum definitions
*
* @author: Victor Kronberg
* @date September 08 2019
* @version 2.0
* 
* @compiler: GCC
*
*/
#ifndef __ERRORS_H__
#define __ERRORS_H__

typedef enum errors  {
	eErrorPeriodTooLong = 2,
	eErrorLEDOnTooLong = 1,
	eErrorNoError = 0
} eError;


#endif
