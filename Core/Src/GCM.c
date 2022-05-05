#include "GCM.h"


extern UART_HandleTypeDef huart1;
extern CRYP_HandleTypeDef hcryp;
extern CRYP_ConfigTypeDef pconf;

extern uint32_t TIMEOUT;

extern int a,b,i,j,k;

uint16_t Key, IVlen, PTlen, AADlen, Taglen;

uint8_t key128[16] = {0},key256[32]= {0};
uint8_t plaintext128[16]={0}, plaintext256[64]={0},cyphertext128[16]={0}, cyphertext256[64]={0};
//uint8_t plaintext128[51]={0}, plaintext256[51]={0},cyphertext128[51]={0}, cyphertext256[51]={0};
uint8_t expectedencryptedtext128[51]={0},expectedencryptedtext256[51]={0};
//uint8_t encryptedtext128[16]={0},encryptedtext256[32]={0};
uint32_t encryptedtext128[16]={0},encryptedtext256[16]={0};
uint32_t decryptedtext128[16]={0},decryptedtext256[16]={0};
uint8_t IV128[16]={0};
uint8_t TAG128[16]={0};

uint32_t plaintext128_32[4]={0},key128_32[4]={0},encryptedtext128_32[4]={0},IV128_32[4] ={0},cyphertext128_32[4]={0}, TAG128_32[4]={0};//shifting reversing bytes variables
uint32_t plaintext256_32[13]={0},key256_32[8]={0},encryptedtext256_32[13]={0},IV256_32[32] ={0},cyphertext256_32[13]={0};

uint8_t B0[16],B1[16];
uint8_t Header128[16]={0};
uint32_t B0_32[4], B1_32[4];
uint32_t Header128GCM128_32_[51] ={0};
//uint32_t Header128GCM128_32_[4] = {0x24825602,0xBD12A984,0xE0092D3E,0x448EDA5F};  //24825602BD12A984E0092D3E448EDA5F
uint32_t BlockB0[4] = {0x7A05C8CC, 0x7732B3B4, 0x7F08AF1D, 0xAF000011};
uint32_t BlockB1[4] = {0x00073421, 0x5F032567, 0x0B000000, 0x00000000};
uint32_t TAG[4]={0};

#define AES_PAYLOAD_SIZE_GCM            4  /* in 32-bit long words */
#define AES_PAYLOAD_SIZE_CCM            8  /* in 32-bit long words */
#define AES_ACTUAL_PAYLOAD_SIZE_CCM    17  /* Plaintext_CCM[] message is actually 17-byte long */ //17*8=136bit

#define AES_HEADER_SIZE_GMAC           17  /* in 32-bit long words */
#define AES_HEADER_SIZE_CCM             4  /* in 32-bit long words */

#define TAG_SIZE                        4  /* in 32-bit long words */

#define KEY_256_SIZE                    8  /* in 32-bit long words */
#define IV_SIZE                         4  /* in 32-bit long words */

#define TIMEOUT_VALUE                0xFF
/**
 * @brief
 */
void GCM_128_Encryption(void) {
	/*************************************************/
	/*************GCM 128 bit Encryption**************/
	/*************************************************/

	debug_printf("\n\n\rInput: Plain text, Key, Initialization Vector and Header(Additional Authentication Data) are required");
	debug_printf("\n\r==================================\n\r");

	debug_printf("\n\r1)Enter %d-bit Plain text:\t",PTlen);
	HAL_UART_Receive(&huart1,(uint8_t *)plaintext128,16,15000);
	HAL_UART_Transmit(&huart1,(uint8_t *)plaintext128,16,100);
	Display_PlainData(plaintext128,PTlen/8);
	for(a=0;a<PTlen/8;a=a+4)
	{
		plaintext128_32[a/4] = plaintext128[3+a] | (plaintext128[2+a] << 8) | (plaintext128[1+a] << 16) | (plaintext128[0+a] << 24);
	}

	debug_printf("\n\r2)Enter 128-bit Key:\t\t");
	HAL_UART_Receive(&huart1,(uint8_t *)key128,16,15000);
	HAL_UART_Transmit(&huart1,(uint8_t *)key128,16,100);
	Display_key((uint8_t *)key128,16);
	for(a=0;a<16;a=a+4)
	{
		key128_32[a/4] = key128[3+a] | (key128[2+a] << 8) | (key128[1+a] << 16) | (key128[0+a] << 24);
	}

	debug_printf("\n\r3)Enter %d-bit Initialization Vector:\t\t",IVlen);
	HAL_UART_Receive(&huart1,(uint8_t *)IV128,IVlen/8,15000);
	HAL_UART_Transmit(&huart1,(uint8_t *)IV128,IVlen/8,100);
	Display_IV((uint8_t *)IV128,IVlen/8);
	for(a=0;a<IVlen/8;a=a+4)
	{
		IV128_32[a/4] = IV128[3+a] | (IV128[2+a] << 8) | (IV128[1+a] << 16) | (IV128[0+a] << 24);
	}


	debug_printf("\n\r4)Enter %d-bit Header AAD:\t\t",AADlen);
	HAL_UART_Receive(&huart1,(uint8_t *)Header128,AADlen/8,15000);
	HAL_UART_Transmit(&huart1,(uint8_t *)Header128,AADlen/8,100);
	Display_Header((uint8_t *)Header128,AADlen/8);
	for(a=0;a<AADlen/8;a=a+4)
	{
		Header128GCM128_32_[a/4] = Header128[3+a] | (Header128[2+a] << 8) | (Header128[1+a] << 16) | (Header128[0+a] << 24);
	}
	debug_printf("\n\r");



	/* Set the CRYP parameters */
	pconf.DataType = CRYP_DATATYPE_1B;
	pconf.KeySize = CRYP_KEYSIZE_128B;
	pconf.pKey = (uint32_t *)plaintext128;
	pconf.Algorithm = CRYP_AES_GCM_GMAC;
	pconf.HeaderSize = AADlen/32;
	pconf.Header = (uint32_t *)Header128GCM128_32_;
	pconf.pInitVect = (uint32_t *)IV128_32;
	pconf.KeyIVConfigSkip = CRYP_KEYIVCONFIG_ALWAYS;
	pconf.DataWidthUnit = CRYP_DATAWIDTHUNIT_BYTE;

	if (HAL_CRYP_SetConfig(&hcryp, &pconf) != HAL_OK)
	{
		Error_Handler();
	}
	debug_printf("PTlen/32 = %d", PTlen/32);
	/* Encrypted text generation*/
	if (HAL_CRYP_Encrypt(&hcryp,plaintext128_32, PTlen/8,encryptedtext128, TIMEOUT) == HAL_OK)
	{
		encryptedtext128[((PTlen/8)/4)] = encryptedtext128[((PTlen/8)/4)]&0xFF000000;
		Display_EncryptedData(encryptedtext128,0x33,128,PTlen/8);
	}
	else
	{
		Error_Handler();
	}
	while(HAL_CRYP_GetState(&hcryp) != HAL_CRYP_STATE_READY);

	/*Authentication Tag generation*/
	if (HAL_CRYPEx_AESGCM_GenerateAuthTAG(&hcryp, TAG, 0xFF) != HAL_OK)
	{
		Error_Handler();

	}
	Display_AuthTAG(TAG,0x33,128,Taglen/8);

}



/**
 * @brief
 */
void GCM_128_Decryption(void) {
	debug_printf("\n\n\rInput: Cyphertext, Key, Initialization Vector, Header(Additional Authentication Data) and Authentication TAG are required");
	debug_printf("\n\r==================================\n\r");

	debug_printf("\n\r1)Enter %d-bit cypher text:\t", PTlen);
	HAL_UART_Receive(&huart1,(uint8_t *)cyphertext128,PTlen/8,15000);
	HAL_UART_Transmit(&huart1,(uint8_t *)cyphertext128,PTlen/8,100);
	Display_PlainData(cyphertext128,PTlen/8);
	for(a=0;a<16;a=a+4)
	{
		cyphertext128_32[a/4] = cyphertext128[3+a] | (cyphertext128[2+a] << 8) | (cyphertext128[1+a] << 16) | (cyphertext128[0+a] << 24);
	}

//
//				debug_printf("\n\r2)Enter 128-bit Key:\t\t");
//				HAL_UART_Receive(&huart1,(uint8_t *)key128,16,15000);
//				HAL_UART_Transmit(&huart1,(uint8_t *)key128,16,100);
//				Display_key((uint8_t *)key128,16);
//				for(a=0;a<16;a=a+4)
//				{
//					key128_32[a/4] = key128[3+a] | (key128[2+a] << 8) | (key128[1+a] << 16) | (key128[0+a] << 24);
//				}
	key128_32[0] = 0x00010203;
	key128_32[1] = 0x04050607;
	key128_32[2] = 0x08090A0B;
	key128_32[3] = 0x0C0D0E0F;


//				debug_printf("\n\r3)Enter %d-bit Initialization Vector:\t\t",IVlen);
//				HAL_UART_Receive(&huart1,(uint8_t *)IV128,IVlen/8,15000);
//				HAL_UART_Transmit(&huart1,(uint8_t *)IV128,IVlen/8,100);
//				Display_IV((uint8_t *)IV128,IVlen/8);
//				for(a=0;a<IVlen/8;a=a+4)
//				{
//					IV128_32[a/4] = IV128[3+a] | (IV128[2+a] << 8) | (IV128[1+a] << 16) | (IV128[0+a] << 24);
//				}
	IV128_32[0] = 0x4D4D4D00;
	IV128_32[1] = 0x00BC614E;
	IV128_32[2] = 0x01234567;
	IV128_32[3] = 0x00000002;
	#if 0 													//If Header is available else predefined header in start will be considered
	debug_printf("\n\r4)Enter 128-bit Header (Additional Authentication Data:\t\t");
	HAL_UART_Receive(&huart1,(uint8_t *)Header128,16,15000);
	HAL_UART_Transmit(&huart1,(uint8_t *)Header128,16,100);
	Display_Header((uint8_t *)Header128,16);
	for(a=0;a<16;a=a+4)
	{
		Header128GCM128_32_[a/4] = Header128[3+a] | (Header128[2+a] << 8) | (Header128[1+a] << 16) | (Header128[0+a] << 24);
	}
	#endif

//				debug_printf("\n\r3)Enter 128-bit Authentication TAG:\t\t"); //why TAG is needed in decryption
//				HAL_UART_Receive(&huart1,(uint8_t *)TAG128,16,15000);
//				HAL_UART_Transmit(&huart1,(uint8_t *)TAG128,16,100);
//				//Display_AuthTAG((uint32_t *)TAG128,0x33,128,16);
//				for(a=0;a<16;a=a+4)
//				{
//					TAG128_32[a/4] = TAG128[3+a] | (TAG128[2+a] << 8) | (TAG128[1+a] << 16) | (TAG128[0+a] << 24);
//				}
//


	if (HAL_CRYP_DeInit(&hcryp) != HAL_OK)
	{
		Error_Handler();
	}
	/* Set the CRYP parameters */
	hcryp.Init.pKey = (uint32_t *)key128_32;
	hcryp.Init.pInitVect = (uint32_t *)IV128_32;
	hcryp.Init.Algorithm = CRYP_AES_GCM_GMAC;
	hcryp.Init.Header = (uint32_t *)Header128GCM128_32_;
	hcryp.Init.HeaderSize = AADlen/32;
	//hcryp.Init.HeaderWidthUnit = CRYP_HEADERWIDTHUNIT_BYTE;
	//hcryp.Init.KeyIVConfigSkip = CRYP_KEYIVCONFIG_ALWAYS;
	hcryp.Init.DataWidthUnit = CRYP_DATAWIDTHUNIT_WORD;


	if (HAL_CRYP_Init(&hcryp) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_CRYP_Decrypt(&hcryp,cyphertext128_32, PTlen/32,decryptedtext128, 0xFF) == HAL_OK)
	{
		Display_DecryptedData(decryptedtext128,0x33,128,PTlen/8);
	}
	else
	{
		Error_Handler();
	}
}


/**
 * @brief
 */
void GCM_256_Encryption(void) {
	debug_printf("\n\n\rInput: Plain text, Key, B0 and B1 (Header / Additional Authentication Data) are required");
	debug_printf("\n\r==================================\n\r");

	debug_printf("\n\r1)Enter 128-bit Plain text:\t");
	HAL_UART_Receive(&huart1,(uint8_t *)plaintext128,AES_ACTUAL_PAYLOAD_SIZE_CCM,15000);
	HAL_UART_Transmit(&huart1,(uint8_t *)plaintext128,AES_ACTUAL_PAYLOAD_SIZE_CCM,100);
	Display_PlainData(plaintext128,AES_ACTUAL_PAYLOAD_SIZE_CCM);
	for(a=0;a<AES_ACTUAL_PAYLOAD_SIZE_CCM;a=a+4)
	{
		plaintext128_32[a/4] = plaintext128[3+a] | (plaintext128[2+a] << 8) | (plaintext128[1+a] << 16) | (plaintext128[0+a] << 24);
	}

//			debug_printf("\n\r2)Enter 256-bit Key:\t\t");
//			HAL_UART_Receive(&huart1,(uint8_t *)key256,32,15000);
//			HAL_UART_Transmit(&huart1,(uint8_t *)key256,32,100);
//			Display_key((uint8_t *)key256,32);
//			for(a=0;a<32;a=a+4)
//			{
//				key256_32[a/4] = key256[3+a] | (key256[2+a] << 8) | (key256[1+a] << 16) | (key256[0+a] << 24);
//			}
	uint32_t key256_32[8] = {0xD346D11A, 0x7117CE04, 0x08089570, 0x7778287C, 0x40F5F473, 0xA9A8F2B1, 0x570F6137, 0x4669751A};


	#if 0													//If B0 is available else predefined B0 in start will be considered
	debug_printf("\n\r3)Enter 128-bit B0 (first authentication block):\t\t");
	HAL_UART_Receive(&huart1,(uint8_t *)B0,16,15000);
	HAL_UART_Transmit(&huart1,(uint8_t *)B0,16,100);
	Display_key((uint8_t *)B0,16);
	for(a=0;a<16;a=a+4)
	{
		B0_32[a/4] = B0[3+a] | (B0[2+a] << 8) | (B0[1+a] << 16) | (B0[0+a] << 24);
	}
	debug_printf("\n\r");

													//If Header is available else predefined header in start will be considered
	debug_printf("\n\r4)Enter 128-bit B1 (Header / Additional Authentication Data:\t\t");
	HAL_UART_Receive(&huart1,(uint8_t *)B1,16,15000);
	HAL_UART_Transmit(&huart1,(uint8_t *)B1,16,100);
	Display_Header((uint8_t *)B1,16);
	for(a=0;a<16;a=a+4)
	{
		B1_32[a/4] = B1[3+a] | (B1[2+a] << 8) | (B1[1+a] << 16) | (B1[0+a] << 24);
	}
	debug_printf("\n\r");
	#endif



	if (HAL_CRYP_DeInit(&hcryp) != HAL_OK)
	{
		Error_Handler();
	}
	/* Set the CRYP parameters */
	hcryp.Init.KeySize       = CRYP_KEYSIZE_256B;
	hcryp.Init.pKey          = (uint32_t *)key256_32;
	hcryp.Init.Algorithm     = CRYP_AES_CCM;
	hcryp.Init.B0            = (uint32_t *)BlockB0;
	hcryp.Init.Header        = (uint32_t *)BlockB1;
	hcryp.Init.HeaderSize    = 4;
	hcryp.Init.DataWidthUnit = CRYP_DATAWIDTHUNIT_BYTE;


	if (HAL_CRYP_Init(&hcryp) != HAL_OK)
	{
		Error_Handler();
	}

	/* Encrypted text generation*/
	if (HAL_CRYP_Encrypt(&hcryp,plaintext128_32,AES_ACTUAL_PAYLOAD_SIZE_CCM,encryptedtext256, TIMEOUT_VALUE) != HAL_OK)
	{
		Error_Handler();

	}
	encryptedtext256[(AES_ACTUAL_PAYLOAD_SIZE_CCM/4)] = encryptedtext256[(AES_ACTUAL_PAYLOAD_SIZE_CCM/4)]&0xFF000000; /* to compare only first byte of last word */

	Display_EncryptedData(encryptedtext256,0x33,256,AES_ACTUAL_PAYLOAD_SIZE_CCM);

	/*Authentication Tag generation*/
	if (HAL_CRYPEx_AESGCM_GenerateAuthTAG(&hcryp, TAG, 0xFF) != HAL_OK)
	{
		Error_Handler();

	}
	Display_AuthTAG(TAG,0x33,256,16);
}


/**
 * @brief
 */
void GCM_256_Decryption(void) {

}
