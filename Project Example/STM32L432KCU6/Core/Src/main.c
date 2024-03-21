/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mcp794xx_driver_basic.h"
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
 I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


typedef enum{                   /**< driver test state chine */

	SET_TIME_DATE,
	GET_COMPILE_TIME_DATE,
	READ_TIME_AND_DATE,
	TEST_ALARM,
	TEST_POWER_FAIL,
	TEST_FREQ_OUT,
	TEST_TIME_TRIM,
	TEST_EPOCH_TIME,
	WRITE_SRAM,
	READ_SRAM,
	ERASE_SRAM_SECTOR,
	WIPE_SRAM,
	WRITE_EEPROM,
	READ_EEPROM,
	PUT_EEPROM,
	GET_EEPROM,
	WIPE_EEPROM,
	READ_UID,
	COUNTDOWN_TIMER
}driver_example_t;

typedef struct test_s{
	driver_example_t state;
}test_t;

test_t test;

void serial_print(const char *pString, uint8_t u8Length);
uint8_t i2c_write(uint8_t addr, uint8_t *buf, uint16_t len);
uint8_t i2c_read(uint8_t addr, uint8_t *buf, uint16_t len);

struct mcp794xx_time_s default_time_date = {   	        /**< default time and date Tuesday 24/12/2023 20:24:48*/
	.year    = 23,          							/**< year 2023*/
	.date    = 24,         	 							/**< date 24th */
	.hour    = 20,										/**< hour 8pm */
	.minute  = 24,										/**< minute 24 */
	.second  = 48,           							/**< seconds 48 */
	.month   = MCP794XX_MTH_DECEMBER,					/**< month of December */
	.time_Format = MCP794XX_24HR_FORMAT,                /**< 24hr time format */
	.weekDay = MCP794XX_WKDAY_SUNDAY,                  /**< week day Sunday */
	.am_pm_indicator = MCP794XX_AM_INDICATOR          	/**< am and pm indicator is ignored when using 24hr time format */
};

struct mcp794xx_time_s alarm_default_time = {           /**< alarm default time and date */
	.date   = 10,
	.hour   = 11,
	.minute = 59,
	.second = 15,
	.month = MCP794XX_MTH_NOVEMBER,
	.time_Format = MCP794XX_24HR_FORMAT,
	.weekDay = MCP794XX_WKDAY_TUESDAY,
	.am_pm_indicator = MCP794XX_AM_INDICATOR          	/**< am and pm indicator is ignored when using 24hr time format */
};

mcp794xx_time_t now, alarm, pwr_fail, epoch_t, compile_time_date;          /**< define a new time objects to read current time and date, alarm time and date and power fail time stamp */
uint8_t alarm_mask, previous_seconds;
uint8_t alarm_flag, set_time_status;
bool pwr_fail_read, set_alarm_status;
uint32_t epoch_time;
uint32_t epoch_time_read;
uint8_t *pEeprom_data_read;
uint8_t *pEeprom_data_write;
uint8_t *pUnique_id;
uint8_t *pSram_data_read;
uint8_t *pSram_data_write;

mcp794xx_irq_callback_t mcp794x_irq_cb = mcp794xx_basic_irq_handler;	/**< define a callback function for external interrupt */

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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

	err = mcp794xx_basic_initialize(MCP79412_VARIANT);                               /**< initialize device driver  passing in the correct variant*/
	if(err != MCP794XX_DRV_OK){
		mcp794xx_interface_debug_print("initialize failed, error code: %d\r", err);
	}
	err = mcp794xx_info(&mcp794xx_handler);

	mcp794xx_interface_debug_print("Chip name :\t%s\n\r", mcp794xx_handler.info.chip_name);
	mcp794xx_interface_debug_print("Manufacturer: \t%s\n\r",  mcp794xx_handler.info.manufacturer_name);

	mcp794xx_interface_debug_print("Interface: \t%s\n\r",  mcp794xx_handler.info.interface);
	mcp794xx_interface_debug_print("Supply voltage max : \t%0.2fV\n\r",  mcp794xx_handler.info.supply_voltage_max_v);
	mcp794xx_interface_debug_print("Supply voltage min: \t%0.2fV\n\r",  mcp794xx_handler.info.supply_voltage_min_v);
	mcp794xx_interface_debug_print("Maximum current: \t%0.1fmA\n\r",  mcp794xx_handler.info.max_current_ma);
	mcp794xx_interface_debug_print("Temperature Max: \t%.1fC\n\r",  mcp794xx_handler.info.temperature_max);
	mcp794xx_interface_debug_print("Temperature Min: \t%.1fC\n\r",  mcp794xx_handler.info.temperature_min);
	mcp794xx_interface_debug_print("Driver version: \tV%.1f.%.2d\n\r", ( mcp794xx_handler.info.driver_version / 1000), (uint8_t)( mcp794xx_handler.info.driver_version - (uint8_t)( mcp794xx_handler.info.driver_version / 100)*100));

	test.state = SET_TIME_DATE;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

		switch((int)test.state)
		{
			case SET_TIME_DATE :
			{
				mcp794xx_basic_set_time_date(&default_time_date);   							/**< set rtc time and date manually */

				#ifdef USE_COMPILE_TIME_DATE         											/**< set time and date using compile time and date (if this routine fails use default time and date above )*/

				mcp794xx_basic_get_compile_time_date(__TIME__, __DATE__, &compile_time_date);   /**< read and convert compile time and date to rtc time object */
				mcp794xx_basic_set_time_date(&compile_time_date);								/**< set time and date */
				#endif

				test.state = READ_TIME_AND_DATE;
				break;
			}

			case READ_TIME_AND_DATE:
			{
				mcp794xx_basic_get_current_time_date(&now);				/**< read current time and date */

				if(now.second != previous_seconds){                     /**< print time every second */
					previous_seconds = now.second;

					/**< print current time and date*/
					mcp794xx_interface_debug_print("Time: %.2d:",now.hour);
					mcp794xx_interface_debug_print("%.2d:",now.minute);
					mcp794xx_interface_debug_print("%.2d",now.second);
					//	  mcp794xx_interface_debug_print(" %s \n\r",am_pm_array[now.am_pm_indicator]);
					mcp794xx_interface_debug_print("\r\ndate: %s",week_days_arr[now.weekDay]);
					mcp794xx_interface_debug_print(" %.2d",now.date);
					mcp794xx_interface_debug_print(" %s",months_array[now.month]);
					mcp794xx_interface_debug_print(" %d \n\r",now.year);

					test.state = COUNTDOWN_TIMER;

				}

				break;
			}

			case TEST_ALARM:
			{
				if(set_alarm_status == false)
				{
					set_alarm_status = true;

					mcp794xx_basic_enable_alarm(MCP794XX_ALARM0, MCP794XX_INT_POLARITY_LOW);								/**< enable alarm 0 and set interrupt output polarity logic low*/
					mcp794xx_basic_set_alarm_time_date(MCP794XX_ALARM0, MCP794XX_MASK_HOURS, &alarm_default_time);			/**< set hour alarm (alarm will fire an interrupt when hour match) */

					mcp794xx_basic_get_alarm_time_date(MCP794XX_ALARM0, &alarm_mask, &alarm);     							/**< read alarm time, date and alarm mask set(this operation is not necessary when setting alarm) */
					mcp794xx_interface_debug_print("alarm time: %.2d:",alarm.hour);											/**< print time and date set*/
				}

               mcp794xx_basic_get_alarm_interrupt_flag(MCP794XX_ALARM0, &alarm_flag);       								/**< read alarm interrupt flag */

			   if(alarm_flag == 1)    																						/**< this routine should be executed inside the external interrupt callback function */
			   {
				   HAL_GPIO_TogglePin(user_led_GPIO_Port, user_led_Pin);													/**< toggle user led for debug purposes */
				   mcp794xx_basic_clr_alarm_interrupt_flag(MCP794XX_ALARM0);												/**< clear alarm time flag (note that if time still match alarm flag will be held) */

//				   mcp794xx_basic_irq_callBack(MCP794XX_ALARM0, mcp794x_irq_cb);											/**< interrupt callback function (to be called in the external interrupt callback function )*/
			   }
			   test.state = READ_TIME_AND_DATE;
				break;
			}

			case COUNTDOWN_TIMER:
			{   /**< !! note that week day must be set accurately according to calendar for this routine to work !! */

				if(set_alarm_status == false)
				{
					set_alarm_status = true;
					mcp794xx_basic_enable_countdown_interrupt();							/**< enable count down timer interrupt (this routine uses ALARM1) and generate an interrupt when time expires*/
					mcp794xx_basic_set_countdown_time(5, MCP794XX_CNTDWN_SECONDS);			/**< set count down time, fires an interrupt every 5 sec(this routine will disable ALARM0)*/
				}

				mcp794xx_basic_get_alarm_interrupt_flag(MCP794XX_ALARM1, &alarm_flag);      /**< read alarm interrupt flag */
				mcp794xx_interface_debug_print("flag %d\n", alarm_flag);

				if(alarm_flag == 1)
				{
					HAL_GPIO_TogglePin(user_led_GPIO_Port, user_led_Pin);					/**< toggle user led for debug purposes  (not advisable to call this function within a interrupt callback function) */
					mcp794xx_basic_clr_alarm_interrupt_flag(MCP794XX_ALARM1);				/**< clear interrupt flag */
					mcp794xx_basic_set_countdown_time(5, MCP794XX_CNTDWN_SECONDS);			/**< reload time in register every when count down expires  (not advisable to call this function within a interrupt callback function)*/

//					mcp794xx_basic_irq_callBack(MCP794XX_ALARM1, mcp794x_irq_cb);			/**< interrupt callback function (to be called in the external interrupt callback function) */
				}
				test.state = READ_TIME_AND_DATE;
				break;
			}

			case TEST_POWER_FAIL:
			{
				if(pwr_fail_read == false)   /**< !! make sure power fail time stamp is read before any other instruction clears existing data (must be called before setting time at start up !! */
				{
					pwr_fail_read = true;
					mcp794xx_basic_get_pwr_fail_time_stamp(MCP794XX_PWR_UP_TIME_STAMP, &pwr_fail);           /**< read power fail time stamp */
					mcp794xx_basic_enable_ext_batt_bckup_pwr();											     /**< enable back up power for future power fail event (calling this function will clear power fail time stamp event) */
				}
				/*< print time stamp */
				mcp794xx_interface_debug_print("pwr fail:%.2d:",pwr_fail.hour);
				mcp794xx_interface_debug_print("%.2d - ",pwr_fail.minute);
				mcp794xx_interface_debug_print("%.2d\r\n",pwr_fail.time_Format);

				test.state = SET_TIME_DATE;

				break;
			}

			case TEST_FREQ_OUT:
			{
				mcp794xx_basic_enable_sqr_wave_output();									/**< enable frequency output */
				mcp94xx_basic_set_sqr_wave_output_freq(MCP794XX_SQR_FREQ_SELCET_4_096KHZ);	/**< set frequency output value */
				break;
			}

			case TEST_EPOCH_TIME:
			{
				mcp794xx_basic_convert_time_to_epoch_unix_time(&now, (uint32_t *)&epoch_time);              /**< convert current time and date to epoch unix time (local time) */
				mcp794xx_interface_debug_print("time date: %lu\r\n",epoch_time);

				mcp794xx_basic_convert_epoch_to_human_time_format(epoch_time, &epoch_t);                    /**< convert epoch time format to human readable format*/
				mcp794xx_interface_debug_print("epoch current tm: %d-%d-%d  %02d:%02d:%02d, %s\n\r", epoch_t.year, epoch_t.month, epoch_t.date, epoch_t.hour, epoch_t.minute, epoch_t.second, week_days_arr[epoch_t.weekDay]);

				test.state = READ_TIME_AND_DATE;
				break;
			}

			case READ_UID:
			{
				pUnique_id = (uint8_t *)calloc(UID_MAX_LENGTH, sizeof(uint8_t));     	  /**< allocate memory for data to read */
				if(pUnique_id == NULL)
					mcp794xx_interface_debug_print("failed to allocate memory\n\r");

                mcp794xx_basic_uid_read((uint8_t*)pUnique_id);                            /**< read device unique identifier */
                for(int index = 0; index < UID_MAX_LENGTH; index++){					  /*< print id */
                    mcp794xx_interface_debug_print("%.2p :",pUnique_id[index]);
                }
                free(pUnique_id);														  /**< free memory allocated*/

				break;
			}

			case READ_SRAM:
			{
				pSram_data_read = (uint8_t *)calloc(SRAM_MEMORY_SIZE, sizeof(uint8_t));     						  /**< allocate memory for data to read */
				if(pSram_data_read == NULL)
					mcp794xx_interface_debug_print("failed to allocate memory\n\r");

				mcp794xx_basic_sram_read_byte(SRAM_FIRST_ADDRESS, (uint8_t *)pSram_data_read, SRAM_MEMORY_SIZE);      /**< read the whole sram memory, starting from first address (0x20) */
				for(int index = 0; index < SRAM_MEMORY_SIZE; index++){
					mcp794xx_interface_debug_print("%.2p ",pSram_data_read[index]);
				}

				free(pSram_data_read);		/**< free memory allocated*/
				break;
			}


			case WRITE_SRAM:
			{
				pSram_data_write = (uint8_t *)calloc(SRAM_MEMORY_SIZE, sizeof(uint8_t));     /**< allocate memory for data to written */
				if(pSram_data_write == NULL)
					mcp794xx_interface_debug_print("failed to allocate memory\n\r");

				memcpy((uint8_t *)pSram_data_write, (uint8_t *)pUnique_id, UID_MAX_LENGTH);   /**< copy UID and write it to sram */

				mcp794xx_basic_sram_write_byte(SRAM_FIRST_ADDRESS, (uint8_t *)pSram_data_write, UID_MAX_LENGTH);   /**< write data starting from sram first address */

				free(pSram_data_write);
				break;
			}

			case ERASE_SRAM_SECTOR:
			{
				mcp794xx_basic_sram_erase_selector(0x20, 0x23);             				 /**< erase sram data from address 0x20 - 0x23 */
				break;
			}

			case WIPE_SRAM:
			{
				mcp794xx_basic_sram_wipe();													/**< wipe the whole sram memory */
				break;
			}

			case READ_EEPROM:
			{
				pEeprom_data_read = (uint8_t *)calloc(EEPROM_MEMORY_SIZE, sizeof(uint8_t));     						  /**< allocate memory for data to read */
				if(pEeprom_data_read == NULL)
					mcp794xx_interface_debug_print("failed to allocate memory\n\r");

				mcp794xx_basic_eeprom_read_byte(EEPROM_LOWEST_ADDRESS, (uint8_t *)pEeprom_data_read, EEPROM_PAGE_SIZE);   /**< read the 8 byte starting from address 0x00 */
				for(int index = 0; index < EEPROM_PAGE_SIZE ; index++){
					mcp794xx_interface_debug_print("%.2p ", pEeprom_data_read[index]);
				}

				free(pEeprom_data_read);
				break;
			}

			case WRITE_EEPROM:
			{
				pEeprom_data_write = (uint8_t *)calloc(EEPROM_PAGE_SIZE, sizeof(uint8_t));         /**< allocate a block memory for the eeprom write buffer */
				if(pEeprom_data_write == NULL)
					mcp794xx_interface_debug_print("failed to allocate dynamic memory\n\r");

				for(int index = 0; index < EEPROM_PAGE_SIZE; index++){                          					  /**< fill the first few byte of the memory 0 - 10 */
					pEeprom_data_write[index] = index;
					mcp794xx_interface_debug_print("to write :%.2p\n\r", pEeprom_data_write[index]);
				}

				mcp794xx_basic_eeprom_write_byte(0x00, (uint8_t *)pEeprom_data_write, EEPROM_PAGE_SIZE);      		  /**< write the first 8 bytes address (keep in mind the 5ms Write Cycle Time before attempt to read back written data)*/

				free(pEeprom_data_write);          													  /**< allocated memory */

				break;
			}

			case PUT_EEPROM:
			{

				mcp794xx_basic_convert_time_to_epoch_unix_time(&now, (uint32_t *)&epoch_time);   	  /**< convert current time stamp to epoch Unix time */

				err = mcp794xx_basic_eeprom_put_byte(0x00, (uint32_t*)&epoch_time, 4);  		      /**< store converted epoch time into eeprom from address 0x00, knowing that it is always a 4 bytes long number */
				mcp794xx_interface_debug_print("err code: %d\n\r",err);

				test.state = GET_EEPROM;
				break;
			}

			case GET_EEPROM:
			{
				mcp794xx_interface_delay_ms(5);														/**< wait 5ms before read */

				mcp794xx_basic_eeprom_get_byte(0x00, (uint32_t*)&epoch_time_read, 4);				/**<  read epoch time written in previous state */
				mcp794xx_interface_debug_print("eeprom get: %lu\n\r",epoch_time_read);

				test.state = READ_TIME_AND_DATE;

				break;
			}

		}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 8;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00707CBB;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(user_led_GPIO_Port, user_led_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : user_led_Pin */
  GPIO_InitStruct.Pin = user_led_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(user_led_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/**
 * @brief transmit serial data via usb com port
 * @param[in] pTxBuffer point to data to sent
 * @param[in] length is the data size
 * @return
 * @note		none
 * */
void serial_print(const char *pString, uint8_t u8Length)
{
  HAL_UART_Transmit(&huart2, (const uint8_t *)pString, u8Length, HAL_MAX_DELAY);
}

/**
 * @brief i2c data transmit
 * @param[in] addr is the slave address
 * @param[in] buf the data to be written
 * @param[in] len size of data
 * @return  status code
 * 			- 0 success
 * 			- 1 failed to write
 * @note	none
 * */
uint8_t i2c_write(uint8_t addr, uint8_t *buf, uint16_t len)
{
	err = HAL_I2C_Master_Transmit(&hi2c1, (addr << 1), (uint8_t *)buf, len, 1000);
	return err;
}

/**
 * @brief i2c data Read
 * @param[in] addr is the slave address
 * @param[out] buf point to data to read
 * @param[in] len size of data
 * @return  status code
 * 			- 0 success
 * 			- 1 failed to read
 * @note	none
 * */

uint8_t i2c_read(uint8_t addr, uint8_t *buf, uint16_t len)
{
	err = HAL_I2C_Master_Receive(&hi2c1, (addr << 1), (uint8_t *)buf, len, 1000);
	return err;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
