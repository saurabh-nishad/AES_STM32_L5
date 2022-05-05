/* USER CODE BEGIN Header */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "GCM.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 CRYP_HandleTypeDef hcryp;
__ALIGN_BEGIN static const uint32_t pKeyAES[4] __ALIGN_END = {
                            0xC939CC13,0x397C1D37,0xDE6AE0E1,0xCB7C423C};
__ALIGN_BEGIN static const uint32_t pInitVectAES[4] __ALIGN_END = {
                            0xB3D8CC01,0x7CBB89B3,0x9E0F67E2,0x00000002};
__ALIGN_BEGIN static const uint32_t HeaderAES[1] __ALIGN_END = {
                            0x00000000};

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
CRYP_ConfigTypeDef pconf;

uint8_t cm = 0,m=0,ed=0;
uint32_t TIMEOUT = 0xFFFFFF;
int a,b,i,j,k;

OperationCase State = STATE_RESET;
ChainingMode chainingMode = CM_NONE;
BitMode bitMode = BM_NONE;
OperationMode operationMode = OM_NONE;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ICACHE_Init(void);
static void MX_AES_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void selectChainingMode(ChainingMode* chainingMode);
void selectBitMode(BitMode* bitMode);
void selectOperation(OperationMode* operation);
void operation_state(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void vprint(const char *fmt,va_list argp){
	char string[200];
	if(0<vsprintf(string,fmt,argp)){

		HAL_UART_Transmit(&huart1,(uint8_t*)string,strlen(string),0xFFFFFF);

	}
}
void debug_printf(const char *fmt, ...){
	va_list argp;
	va_start(argp,fmt);
	vprint(fmt,argp);
	va_end(argp);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ICACHE_Init();
  MX_AES_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  State++;
  /* USER CODE END 2 */

  /* Infinite loop */
  while(1) {

	/* USER CODE BEGIN WHILE */
	  operation_state();
	/* USER CODE END WHILE */

	/* USER CODE BEGIN 3 */
	/* USER CODE END 3 */
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief AES Initialization Function
  * @param None
  * @retval None
  */
static void MX_AES_Init(void)
{

  /* USER CODE BEGIN AES_Init 0 */
  /* USER CODE END AES_Init 0 */

  /* USER CODE BEGIN AES_Init 1 */
  /* USER CODE END AES_Init 1 */
  hcryp.Instance = AES;
  hcryp.Init.DataType = CRYP_DATATYPE_32B;
  hcryp.Init.KeySize = CRYP_KEYSIZE_128B;
  hcryp.Init.pKey = (uint32_t *)pKeyAES;
  hcryp.Init.pInitVect = (uint32_t *)pInitVectAES;
  hcryp.Init.Algorithm = CRYP_AES_GCM_GMAC;
  hcryp.Init.Header = (uint32_t *)HeaderAES;
  hcryp.Init.HeaderSize = 1;
  hcryp.Init.DataWidthUnit = CRYP_DATAWIDTHUNIT_WORD;
  hcryp.Init.KeyIVConfigSkip = CRYP_KEYIVCONFIG_ALWAYS;
  if (HAL_CRYP_Init(&hcryp) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN AES_Init 2 */
  /* USER CODE END AES_Init 2 */

}

/**
  * @brief ICACHE Initialization Function
  * @param None
  * @retval None
  */
static void MX_ICACHE_Init(void)
{

  /* USER CODE BEGIN ICACHE_Init 0 */
  /* USER CODE END ICACHE_Init 0 */

  /* USER CODE BEGIN ICACHE_Init 1 */
  /* USER CODE END ICACHE_Init 1 */

  /** Enable instruction cache in 1-way (direct mapped cache)
  */
  if (HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ICACHE_Init 2 */
  /* USER CODE END ICACHE_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */
  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */
  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : GREEN_LED_Pin */
  GPIO_InitStruct.Pin = GREEN_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GREEN_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USR_BUTTON_Pin */
  GPIO_InitStruct.Pin = USR_BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USR_BUTTON_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI13_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI13_IRQn);

}

/* USER CODE BEGIN 4 */
/*
 * @brief
 */
void selectChainingMode(ChainingMode* chainingMode) {
	debug_printf("\nSelect one of the chainning modes:");
	debug_printf("\n\r1: GCM");
	debug_printf("\n\r2: CTR");
	debug_printf("\n\r3: ECB\n");
	HAL_UART_Receive (&huart1,&cm, 1, TIMEOUT);

	switch (cm) {
		case 0x31:
			*chainingMode = GCM;
			debug_printf("\nSelected chaining mode: ECB\n");
			break;

		case 0x32:
			*chainingMode = CTR;
			debug_printf("\nSelected chaining mode: CTR\n");
			break;

		case 0x33:
			*chainingMode = ECB;
			debug_printf("\nSelected chaining mode: GCM\n");
			break;

		default:
			debug_printf("\n\rInvalid input");
			break;

	}

}

/**
 * @brief
 */
void selectBitMode(BitMode* bitMode) {
	debug_printf("\n\rSelect Mode: ");
	debug_printf("\n\r1: 128-bit");
	debug_printf("\n\r2: 256-bit\n");
	HAL_UART_Receive(&huart1,&m,1,TIMEOUT);

	switch (m) {
		case 0x31:
			*bitMode = key_128_Bit;
			debug_printf("\nSelected  mode: key_128_Bit\n");
			break;

		case 0x32:
			*bitMode = key_256_Bit;
			debug_printf("\nSelected  mode: key_256_Bit\n");
			break;

		default:
			debug_printf("\n\rInvalid input");
			break;

	}
}

/**
 * @brief
 */
void selectOperation(OperationMode* operation) {
	debug_printf("\n\rSelect one of the following: ");
	debug_printf("\n\r1: Encryption");
	debug_printf("\n\r2: Decryption\n\r");
	HAL_UART_Receive(&huart1,&ed,1,TIMEOUT);

	switch (ed) {
		case 0x31:
			*operation = Encrypt;
			debug_printf("\nSelected  operation: Encrypt\n");
			break;

		case 0x32:
			*operation = Decrypt;
			debug_printf("\nSelected  operation: Decrypt\n");
			break;

		default:
			debug_printf("\n\rInvalid input");
			break;

	}
}

/**
 * @brief
 */
void operation_state(void) {
	switch(State) {
		case USER_INPUT:
			chainingMode = CM_NONE;
			bitMode = BM_NONE;
			operationMode = OM_NONE;
			selectChainingMode(&chainingMode);
			selectBitMode(&bitMode);
			selectOperation(&operationMode);
			State = chainingMode ^ bitMode ^ operationMode;
			break;
		case GCM_128_Encrypt:
			debug_printf("\n***********GCM 128 bit Encryption************\n");
			GCM_128_Encryption();
			break;
		case GCM_128_Decrypt:
			debug_printf("\n***********GCM 128 bit Decryption************\n");
			GCM_128_Decryption();
			break;
		case GCM_256_Encrypt:
			debug_printf("\n***********GCM 256 bit Encryption************\n");
			GCM_256_Encryption();
			break;
		case GCM_256_Decrypt:
			debug_printf("\n***********GCM 256 bit Decryption************\n");
			GCM_256_Decryption();
			break;
		default:
			break;
	}
}

/**
 *
 */
void Display_PlainData(uint8_t *plaintext128, uint32_t datalength)
{
	uint32_t BufferCounter = 0;
	uint8_t * ptr = (uint8_t *)plaintext128;

	debug_printf("\n\rPlain Data in register:");

	for (BufferCounter = 0; BufferCounter < datalength; BufferCounter++)
	{
		debug_printf("%02x",(*ptr++));
	}
}


void Display_key(uint8_t *key128,uint32_t datalength)
{
	uint32_t BufferCounter = 0;
	uint8_t * ptr = (uint8_t *)key128;

	debug_printf("\n\rKey in register:");

	for (BufferCounter = 0; BufferCounter < datalength; BufferCounter++)
	{
		debug_printf("%02x", (*ptr++));
	}
}


void Display_IV(uint8_t *IV128,uint32_t datalength)
{
	uint32_t BufferCounter = 0;
	uint8_t * ptr = (uint8_t *)IV128;

	debug_printf("\n\rIV value in register:");

	for (BufferCounter = 0; BufferCounter < datalength; BufferCounter++)
	{
		debug_printf("%02x", (*ptr++));
	}
}



void Display_EncryptedData(uint32_t *encryptedtext, uint8_t mode, uint16_t keysize, uint32_t datalength)
{
	int BufferCounter = 0;
	uint8_t * ptr = (uint8_t *)encryptedtext;

	debug_printf("\n\n\rOutput: Encrypted Data with AES %d  Mode  ", keysize);

	if (mode == 0x31)
	{
		debug_printf("ECB\n\r");
	}
	else if (mode == 0x32)
	{
		debug_printf("CTR\n\r");
	}
	else if(mode == 0x33)
	{
		debug_printf("GCM\n\r");
	}
	debug_printf("==================================================\n\r");

	for (BufferCounter = 0; BufferCounter < datalength;BufferCounter=BufferCounter+4)
	{
		for(a=3;a>=0;a--)
		{
			debug_printf("%02x", *(ptr+BufferCounter+a));
		}
	}

}

void Display_DecryptedData(uint32_t *decryptedtext, uint8_t mode, uint16_t keysize, uint32_t datalength)
{
  int BufferCounter = 0;
	uint8_t * ptr = (uint8_t *)decryptedtext;

	debug_printf("\n\n\rOutput: Decrypted Data with AES %d  Mode  ", keysize);

	if (mode == 0x31)
	{
		debug_printf("ECB\n\r");
	}
	else if (mode == 0x32)
	{
		debug_printf("CTR\n\r");
	}
	else if(mode == 0x33)
	{
		debug_printf("GCM\n\r");
	}
	debug_printf("==================================================\n\r");

	for (BufferCounter = 0; BufferCounter < datalength;BufferCounter=BufferCounter+4)
	{
		for(a=3;a>=0;a--)
		{
			debug_printf("%02x", *(ptr+BufferCounter+a));
		}
	}

}

void Display_Header(uint8_t *Header128,uint32_t datalength)
{

	uint32_t BufferCounter = 0;

	uint8_t * ptr = (uint8_t *)Header128;

	debug_printf("\n\rHeader value in register:");


	for (BufferCounter = 0; BufferCounter < datalength; BufferCounter++)
	{
		debug_printf("%02x", (*ptr++));
	}
}



void Display_AuthTAG(uint32_t *AuthTAG, uint8_t mode, uint16_t keysize, uint32_t datalength)
{
	int BufferCounter = 0;
	uint8_t * ptr = (uint8_t *)AuthTAG;

	debug_printf("\n\n\rOutput: Authentication Tag with AES %d  Mode  ", keysize);

	if (mode == 0x31)
	{
		debug_printf("ECB\n\r");
	}
	else if (mode == 0x32)
	{
		debug_printf("CTR\n\r");
	}
	else if(mode == 0x33)
	{
		debug_printf("GCM\n\r");
	}

	debug_printf("==================================================\n\r");
	for (BufferCounter = 0; BufferCounter < datalength;BufferCounter=BufferCounter+4)
	{
		for(a=3;a>=0;a--)
		{
			debug_printf("%02x", *(ptr+BufferCounter+a));
		}
	}

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
