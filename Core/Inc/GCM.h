/**
  ******************************************************************************
  * @file           : GCM.h
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GCM_H
#define __GCM_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

void GCM_128_Encryption(void);
void GCM_128_Decryption(void);
void GCM_256_Encryption(void);
void GCM_256_Decryption(void);

#endif /* __MAIN_H */