 

<div align=center>
<img src="Document/Image/avento.png" width="400" height="200"/>
</div>

## Library Maters MCP794XX 

The MCP794XX general purpose I2C™Compatible real-time clock/calendar (RTCC) are highly integrated with nonvolatile memory and advanced features normally found in higher priced devices.
These features include a battery switchover circuit for backup power, a timestamp to log power failures and digital trimming for accuracy. 
Using a low-cost 32.768 kHz crystal or other clock source, time is tracked in either a 12-hour or 24-hour format with an AM/PM indicator and timing to the second, minute, hour, day of the week, day, month and year.
As an interrupt or wakeup signal, a multifunction open drain output can be programmed as an Alarm Out or as a Clock Out that supports 4 selectable frequencies.
In addition, non-volatile memory is included along with a Unique ID in a locked section of EEPROM that is factory programmed with an MAC Address.

The Library masters SHT4x is the full-function driver of the SHT4x sensors series (MCP7940N, MCP7940M, MCP79401, MCP79402, MCP79410, MCP79411, MCP79412). The driver is written in C language with Code Blocks.

### Table of Contents

  - [Install](#Install)
  - [Examples](#Examples)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example interface](#example-interface)
  - [Document](#Document)
  - [How to contribute](#Contribute)
  - [License](#License)
  - [Contact Us](#Contact-Us)
  - [Acknowledgements](#Acknowledgements)

  ### Install
  - The interface .C file expects bellow functions to correctly link the driver 
  ```
    - i2c_initialize function  ( optional )
    - i2c_deinitialize function ( optional )
    - i2c_read function ( Mandatory )
    - i2c_write function ( Mandatory )
    - delay function ( Mandatory )
    - print function (optional )
  ```
  - refer to the video: "Coming soon..."
  - Use example project 



  ### Examples
  - [STM32L432 (STM32CubeIDE)](https://github.com/LibraryMasters/mcp794xx/tree/master/Project%20Example/STM32L432KCU6)
  - [PIC24FJ128GA705 (MPLAB X IDE)](https://github.com/LibraryMasters/mcp794xx/tree/master/Project%20Example/PIC24FJ128GA705/mcp794xx_rtc_pic24.X)
  - [SAMD21G18 (Atmel studio 7)](https://github.com/LibraryMasters/mcp794xx/tree/master/Project%20Example/ATSAMD21G18A/mcp794xx_rtc_SAMD21)
  - [ATMEGA4808 (Atmel studio 7)](https://github.com/LibraryMasters/mcp794xx/tree/master/Project%20Example/mcp794xx_rtc_atmega4808)
  - [RP2040 Pi Pico (VScode)](https://github.com/LibraryMasters/mcp794xx/tree/master/Project%20Example/RP2040%20Pi%20Pico/mcp794xx_rtc_RP2040)
  ### Usage
  #### example basic

  ```C
#include "mcp794xx_driver_basic.h"
#include <unistd.h>


typedef enum{                   /**< driver test state chine */

	SET_TIME_DATE,
	READ_TIME_AND_DATE,
	GET_COMPILE_TIME_DATE,
	TEST_ALARM,
	COUNTDOWN_TIMER,
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
	READ_UID

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
	.weekDay = MCP794XX_WKDAY_SUNDAY,                   /**< week day Sunday */
	.am_pm_indicator = MCP794XX_AM_INDICATOR,          	/**< am and pm indicator is ignored when using 24hr time format */
	.local_time_zone = -10                              /**< local time zone Relative to UTC/GMT */
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

mcp794xx_time_t now, alarm, pwr_fail, local, gmt, compile_time_date;          /**< define a new time objects to read current time and date, alarm time and date and power fail time stamp */
uint8_t alarm_mask, previous_seconds;
uint8_t alarm_flag, set_time_status;
bool pwr_fail_read, set_alarm_status;
uint32_t epoch_time = 1707903900;
uint32_t epoch_time_read;
uint8_t *pEeprom_data_read;
uint8_t *pEeprom_data_write;
uint8_t *pUnique_id;
uint8_t *pSram_data_read;
uint8_t *pSram_data_write;

mcp794xx_irq_callback_t mcp794x_irq_cb = mcp794xx_basic_irq_handler;	/**< define a callback function for external interrupt */

int index;

int main()
{

    err = mcp794xx_basic_initialize(MCP79412_VARIANT);                  /**< initialize device driver  passing in the correct variant*/
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
				   mcp794xx_basic_clr_alarm_interrupt_flag(MCP794XX_ALARM0);												/**< clear alarm time flag (note that if time still match alarm flag will be held */

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
					mcp794xx_basic_clr_alarm_interrupt_flag(MCP794XX_ALARM1);				/**< clear interrupt flag */
					mcp794xx_basic_set_countdown_time(5, MCP794XX_CNTDWN_SECONDS);			/**< reload time in register every when count down expires  (not advisable to call this function within an interrupt callback function)*/

//					mcp794xx_basic_irq_callBack(MCP794XX_ALARM1, mcp794x_irq_cb);			/**< interrupt callback function (to be called in the external interrupt callback function) */
				}
				test.state = READ_TIME_AND_DATE;
				break;
			}

			case TEST_POWER_FAIL:
			{
				if(pwr_fail_read == false)   /**< !! make sure power fail time stamp is read before any other instruction clears existing data (must be called before setting time at start-up !! */
				{
					pwr_fail_read = true;
					mcp794xx_basic_get_pwr_fail_time_stamp(MCP794XX_PWR_UP_TIME_STAMP, &pwr_fail);           /**< read power fail time stamp */
					mcp794xx_basic_enable_ext_batt_bckup_pwr();											     /**< enable backup power for future power fail event (calling this function will clear power fail time stamp event) */
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
				mcp794xx_basic_convert_time_to_epoch_unix_time(&now, (uint32_t *)&epoch_time);                                             /**< convert current time and date to epoch unix time (local time) */
				mcp794xx_interface_debug_print("time date: %lu\r\n",epoch_time);

				mcp794xx_basic_convert_epoch_to_human_time_format_local(epoch_time, default_time_date.local_time_zone, &local);             /**< convert epoch time format to local human readable format*/
				mcp794xx_interface_debug_print("epoch current local tm: %d-%d-%d  %02d:%02d:%02d, %s\n\r", local.year, local.month, local.date, local.hour, local.minute, local.second, week_days_arr[local.weekDay]);

				mcp794xx_basic_convert_epoch_to_human_time_format_gmt(epoch_time, &gmt);          /**< convert epoch time format to gmt human readable format*/
				mcp794xx_interface_debug_print("epoch current gmt tm: %d-%d-%d  %02d:%02d:%02d, %s\n\r", gmt.year, gmt.month, gmt.date, gmt.hour, gmt.minute, gmt.second, week_days_arr[gmt.weekDay]);

				test.state = READ_TIME_AND_DATE;
				break;
			}

			case READ_UID:
			{
				pUnique_id = (uint8_t *)calloc(UID_MAX_LENGTH, sizeof(uint8_t));     	  /**< allocate memory for data to read */
				if(pUnique_id == NULL)
					mcp794xx_interface_debug_print("failed to allocate memory\n\r");

                mcp794xx_basic_uid_read((uint8_t*)pUnique_id);                            /**< read device unique identifier */
                for(index = 0; index < UID_MAX_LENGTH; index++){					  /*< print id */
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
				for(index = 0; index < SRAM_MEMORY_SIZE; index++){
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
				for(index = 0; index < EEPROM_PAGE_SIZE ; index++){
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

				for(index = 0; index < EEPROM_PAGE_SIZE; index++){                          					  /**< fill the first few byte of the memory 0 - 10 */
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
    return 0;
}

  ```
  #### example interface
  
  ```C
  ...

/**
 * @brief      interface i2c bus read
 * @param[in]  u8Addr is the i2c device address 7 bit
 * @param[out] *pBuf points to a data buffer
 * @param[in]  u8length is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t mcp794xx_interface_i2c_read(uint8_t u8Addr, uint8_t *pBuf, uint8_t u8Length)
{
    /*call your i2c read function here*/
    /*user code begin */

    /*user code end*/
    return 0; /**< success */
}

/**
 * @brief     interface i2c bus write
 * @param[in] u8Addr is the i2c device address 7 bit
 * @param[in] *pBuf points to a data buffer
 * @param[in] u8length is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t mcp794xx_interface_i2c_write(uint8_t u8Addr, uint8_t *pBuf, uint8_t u8Length)
{
    /*call your i2c write function here*/
    /*user code begin */

    /*user code end*/
    return 0; /**< success */
}
  ...
  
  ```

  ### Document
  [datasheet](https://github.com/LibraryMasters/mcp794xx/tree/master/Document)
  
  ### Contribute
   1. Clone repo and create a new branch: ```https://github.com/LibraryMasters/mcp794xx_PR.git```
   2. Make changes and test
   3. Submit a Pull Request with a comprehensive description of changes
  ### License
  [MIT](https://choosealicense.com/licenses/mit/)
### Contact Us

Email address: cedricmalyam@gmail.com

### Acknowledgements 
- @PeterHenderson https://dribbble.com/peterhenderson for the logo
