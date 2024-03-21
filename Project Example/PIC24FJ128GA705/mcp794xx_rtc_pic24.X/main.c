/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.4
        Device            :  PIC24FJ128GA705
    The generated drivers are tested against the following:
        Compiler          :  XC16 v2.10
        MPLAB 	          :  MPLAB X v6.05
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#include "mcp794xx_driver/mcp794xx_driver_basic.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/i2c1.h"

uint8_t i2c_write(uint8_t addr, uint8_t *pBuf, uint8_t len);
uint8_t i2c_read(uint8_t addr, uint8_t *pBuf, uint8_t len);
void led_toggle(void);
void delay_ms(uint32_t time_us);

I2C1_MESSAGE_STATUS i2c_err_status;

typedef enum{                   /**< driver test state machine */

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
	.minute = 11,
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

uint32_t i2c_time_out =  1000;

/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    
    err = mcp794xx_basic_initialize(MCP79412_VARIANT);												/**< initialize device driver  passing in the correct variant*/
	if(err != MCP794XX_DRV_OK){
		mcp794xx_interface_debug_print("initialize failed, error code: %d\r", err);
	}
//	err = mcp794xx_info(&mcp794xx_handler);
//
//	mcp794xx_interface_debug_print("Chip name :\t%s\n\r", mcp794xx_handler.info.chip_name);
//	mcp794xx_interface_debug_print("Manufacturer: \t%s\n\r",  mcp794xx_handler.info.manufacturer_name);
//
//	mcp794xx_interface_debug_print("Interface: \t%s\n\r",  mcp794xx_handler.info.interface);
//	mcp794xx_interface_debug_print("Supply voltage max : \t%0.2fV\n\r",  mcp794xx_handler.info.supply_voltage_max_v);
//	mcp794xx_interface_debug_print("Supply voltage min: \t%0.2fV\n\r",  mcp794xx_handler.info.supply_voltage_min_v);
//	mcp794xx_interface_debug_print("Maximum current: \t%0.1fmA\n\r",  mcp794xx_handler.info.max_current_ma);
//	mcp794xx_interface_debug_print("Temperature Max: \t%.1fC\n\r",  mcp794xx_handler.info.temperature_max);
//	mcp794xx_interface_debug_print("Temperature Min: \t%.1fC\n\r",  mcp794xx_handler.info.temperature_min);
//	mcp794xx_interface_debug_print("Driver version: \tV%.1f.%.2d\n\r", ( mcp794xx_handler.info.driver_version / 1000), (uint8_t)( mcp794xx_handler.info.driver_version - (uint8_t)( mcp794xx_handler.info.driver_version / 100)*100));

    test.state = SET_TIME_DATE;

    while (1)
    {
        switch((int)test.state)
		{
			case SET_TIME_DATE :
			{
				mcp794xx_basic_set_time_date(&default_time_date);   							/**< set rtc time and date manually */

				#ifdef USE_COMPILE_TIME_DATE         											/**< set time and date using compile time and date (if this routine fails use default time and date above )*/
					mcp794xx_basic_get_compile_time_date(__TIMESTAMP__, &compile_time_date);	/**< read and convert compile time and date to rtc time object */
					mcp794xx_basic_set_time_date(&compile_time_date);						    /**< set time and date */
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
					
//					test.state = READ_UID; 
				}
				break;
			}

			case TEST_ALARM:
			{
				if(set_alarm_status == false)
				{
					set_alarm_status = true;

					mcp794xx_basic_enable_alarm(MCP794XX_ALARM0, MCP794XX_INT_POLARITY_LOW);								/**< enable alarm 0 and set interrupt output polarity logic low*/
					mcp794xx_basic_set_alarm_time_date(MCP794XX_ALARM0, MCP794XX_MASK_MINUTES, &alarm_default_time);			/**< set hour alarm (alarm will fire an interrupt when hour match) */

					mcp794xx_basic_get_alarm_time_date(MCP794XX_ALARM0, &alarm_mask, &alarm);     							/**< read alarm time, date and alarm mask set(this operation is not necessary when setting alarm) */
					mcp794xx_interface_debug_print("alarm time: %.2d:",alarm.hour);											/**< print time and date set*/
				}

				mcp794xx_basic_get_alarm_interrupt_flag(MCP794XX_ALARM0, &alarm_flag);       								/**< read alarm interrupt flag */
                mcp794xx_interface_debug_print("int flag: %d\n", alarm_flag);

				if(alarm_flag == 1)    																						/**< this routine should be executed inside the external interrupt callback function */
				{
					user_led_Toggle();                                                                                      /**< toggle user led for debug purposes */;											
					mcp794xx_basic_clr_alarm_interrupt_flag(MCP794XX_ALARM0);												/**< clear alarm time flag (note that if time still match alarm flag will be held */

					//mcp794xx_basic_irq_callBack(MCP794XX_ALARM0, mcp794x_irq_cb);											/**< interrupt callback function (to be called in the external interrupt callback function )*/
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

				if(alarm_flag == 1)    														/**< this routine should be executed inside the external interrupt callback function */
				{
//					user_led_Toggle();;		/**< toggle user led for debug purposes */;
					mcp794xx_basic_clr_alarm_interrupt_flag(MCP794XX_ALARM1);				/**< clear interrupt flag */
					mcp794xx_basic_set_countdown_time(5, MCP794XX_CNTDWN_SECONDS);			/**< reload time in register every when count down expires*/

					//mcp794xx_basic_irq_callBack(MCP794XX_ALARM1, mcp794x_irq_cb);			/**< interrupt callback function (to be called in the external interrupt callback function) */
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
				mcp794xx_basic_convert_time_to_epoch_unix_time(&now, (uint32_t *)&epoch_time);              /**< convert current time and date to epoch Unix time (local time) */
				mcp794xx_interface_debug_print("time date: %lu\r\n",epoch_time);

//                 struct tm *time = localtime((const time_t *)&epoch_time);          						/**< built-in function from time.h library */
//				 mcp794xx_interface_debug_print("epoch m: %d-%d-%d  %02d:%02d:%02d\n\r", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec);

				mcp794xx_basic_convert_epoch_to_human_time_format(epoch_time, &epoch_t);                    /**< convert epoch time format to human readable format*/
				mcp794xx_interface_debug_print("current tm: %d-%d-%d  %02d:%02d:%02d, %s\n\r", epoch_t.year, epoch_t.month, epoch_t.date, epoch_t.hour, epoch_t.minute, epoch_t.second, week_days_arr[epoch_t.weekDay]);
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
					mcp794xx_interface_debug_print("%.2x :",pUnique_id[index]);
				}
				mcp794xx_interface_debug_print("\n\r");
				//free(pUnique_id);			/**< free memory allocated*/

				test.state = WRITE_EEPROM;
				break;
			}

			case READ_SRAM:
			{
				pSram_data_read = (uint8_t *)calloc(SRAM_MEMORY_SIZE, sizeof(uint8_t));     						  /**< allocate memory for data to read */
				if(pSram_data_read == NULL){
				    mcp794xx_interface_debug_print("failed to allocate memory\n\r");
                }

				mcp794xx_basic_sram_read_byte(SRAM_FIRST_ADDRESS, (uint8_t *)pSram_data_read, SRAM_MEMORY_SIZE);      /**< read the whole sram memory, starting from first address (0x20) */
				for(int index = 0; index < SRAM_MEMORY_SIZE; index++){
					mcp794xx_interface_debug_print("%.2p ",pSram_data_read[index]);
				}

				mcp794xx_interface_debug_print("\n\r");
				free(pSram_data_read);		/**< free memory allocated*/
				
				//test.state = WIPE_SRAM;
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
				test.state = READ_TIME_AND_DATE;
				break;
			} 

			case ERASE_SRAM_SECTOR:
			{
				mcp794xx_basic_sram_erase_selector(0x20, 0x40);             				 /**< erase sram data from address 0x20 - 0x40 */
				test.state = READ_UID;
				break;
			}

			case WIPE_SRAM:
			{
				mcp794xx_basic_sram_wipe();													/**< wipe the whole sram memory */
				test.state = READ_SRAM;
				break;
			}

			case READ_EEPROM:
			{
				mcp794xx_interface_delay_ms(5);
				
				pEeprom_data_read = (uint8_t *)calloc(EEPROM_PAGE_SIZE, sizeof(uint8_t));     						  /**< allocate memory for data to read */
				if(pEeprom_data_read == NULL)
					mcp794xx_interface_debug_print("failed to allocate memory\n\r");

				mcp794xx_basic_eeprom_read_byte(EEPROM_LOWEST_ADDRESS, (uint8_t *)pEeprom_data_read, EEPROM_PAGE_SIZE);   /**< read the 8 byte starting from address 0x00 */
				for(int index = 0; index < EEPROM_PAGE_SIZE ; index++){
					mcp794xx_interface_debug_print("%.2x ", pEeprom_data_read[index]);
				}
				mcp794xx_interface_debug_print("\n\r");
				free(pEeprom_data_read);
				
				test.state = GET_EEPROM;
				break;
			}

			case WRITE_EEPROM:
			{
				pEeprom_data_write = (uint8_t *)calloc(EEPROM_PAGE_SIZE, sizeof(uint8_t));							  /**< allocate a block memory for the eeprom write buffer */
				if(pEeprom_data_write == NULL)
					mcp794xx_interface_debug_print("failed to allocate dynamic memory\n\r");
					
				memcpy((uint8_t *)pEeprom_data_write, (uint8_t *)pUnique_id, UID_MAX_LENGTH);		 				  /**< copy UID and write it to eeprom */
				
				mcp794xx_basic_eeprom_write_byte(0x00, (uint8_t *)pEeprom_data_write, EEPROM_PAGE_SIZE);      		  /**< write the first 8 bytes address (keep in mind the 5ms Write Cycle Time before attempt to read back written data)*/

				free(pEeprom_data_write);   /**< allocated memory */
				free(pUnique_id);			/**< free memory allocated*/
				test.state = READ_EEPROM; 
				break;
			}

			case PUT_EEPROM:
			{
				mcp794xx_basic_convert_time_to_epoch_unix_time(&now, (uint32_t *)&epoch_time);			/**< convert current time stamp to epoch Unix time */

				err = mcp794xx_basic_eeprom_put_byte(0x00, (uint32_t*)&epoch_time, 4);  				  /**< store converted epoch time into eeprom from address 0x00, knowing that it is always a 4 bytes long number */
				mcp794xx_interface_debug_print("epoch written: %.2p\n\r",epoch_time);

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
			
			default:
			{
				
				break;
			}
		}
    }

    return 1;
}

/**
 * @brief i2c data transmit
 * @param[in] addr is the slave address
 * @param[in] reg the register to write
 * @param[in] buf the data to be written
 * @param[in] len size of data
 * @return  status code
 * 			- 0 success
 * 			- 1 failed to write
 * @note	none
 * */
uint8_t i2c_write(uint8_t addr, uint8_t *pBuf, uint8_t len)
{
    I2C1_MasterWrite((uint8_t *)pBuf, len, addr, &i2c_err_status);
    while ((i2c_err_status == I2C1_MESSAGE_PENDING) && --i2c_time_out);
    if (i2c_err_status == I2C1_MESSAGE_FAIL) {
        return 1;
    }
    return 0;
}

/**
 * @brief i2c data Read
 * @param[in] addr is the slave address
 * @param[in] reg the register to read
 * @param[out] buf point to data to read
 * @param[in] len size of data
 * @return  status code
 * 			- 0 success
 * 			- 1 failed to read
 * @note	none
 * */

uint8_t i2c_read(uint8_t addr, uint8_t *pBuf, uint8_t len)
{
    I2C1_MasterRead((uint8_t *)pBuf, len, addr, &i2c_err_status);
    while ((i2c_err_status == I2C1_MESSAGE_PENDING) && --i2c_time_out);
    if (i2c_err_status == I2C1_MESSAGE_FAIL) {
        return 1;
    }
    return 0;
}

void led_toggle(void)
{
   user_led_Toggle(); 
}

/**
 * @brief milli seconds delay function
 * @param[in] time_us time in milli seconds
 * @note for main Frequency clock at 32Mhz
 */
void delay_ms(uint32_t time_us)    
{
    for(uint32_t delay_index = 0; delay_index < (593 * time_us) ; delay_index++);
}


/**
 End of File
*/

