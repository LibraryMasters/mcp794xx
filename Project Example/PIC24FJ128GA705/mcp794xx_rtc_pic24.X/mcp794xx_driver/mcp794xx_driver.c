/**
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sub-license, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * File:   mcp794xx_driver.c
 * Author: Cedric Akilimali
 *
 * Created on May 06, 2023, 2:34 PM
 */

#include "mcp794xx_driver.h"

/**
* @brief chip information definition
*/
#define CHIP_NAME                "MCP794xx"              /**< chip name */
#define INTERFACE                 "I2C"                  /**< interface protocol */
#define MANUFACTURER_NAME         "Microchip"            /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.3f                   /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f                   /**< chip max supply voltage */
#define MAX_CURRENT               400                    /**< chip max current (μA)*/
#define MIN_CURRENT               300                    /**< chip min current (μA)*/
#define TEMPERATURE_MIN           -40.0f                 /**< chip min operating temperature (°C) */
#define TEMPERATURE_MAX           85.0f                  /**< chip max operating temperature (°C) */
#define MCU_FLASH_MIN             64                     /**< Micro-controller minimum recommended flash size (kB) */
#define MCU_RAM_MIN               8                      /**< Micro-controller minimum recommended RAM size (KB)*/
#define DRIVER_VERSION            1000                   /**< driver version */

/**
* @brief i2c rtc write byte
* @param[in] *pHandle points to mcp794xx handle structure
* @param[in] u8Reg is the slave device register address
* @param[in] *pBuf point to data to write
* @return status code
            - 0 success
            - 1 failed to write
* @note none
*/
uint8_t rtc_mcp794xx_i2c_write(mcp794xx_handle_t *const pHandle, uint8_t u8Reg, uint8_t *pBuf)
{
	uint8_t tempBuffer[2];    //len +1
	tempBuffer[0] = u8Reg;

	for(int index = 1; index < 2; index++){
		tempBuffer[index] = pBuf[index - 1];
	}

    if (pHandle->i2c_write(pHandle->rtc_address, (uint8_t *)tempBuffer, 2) != 0) /**< read data   (size is len +1) */
    {
        return 1; /**< read fail */
    }
    return 0;                                            /**< return success */

}

/**
* @brief i2c rtc read byte
* @param[in] *pHandle points to mcp794xx handle structure
* @param[in] u8Reg is the slave device register address
* @param[out] *pBuf point to data to read
* @param[in] u8Length is the data length to read (number of byte)
* @return status code
*          - 0 success
*          - 1 failed to read
* @note none
*/
uint8_t rtc_mcp794xx_i2c_read(mcp794xx_handle_t  *const pHandle, uint16_t u8Reg, uint8_t *pBuf, uint8_t u8Length)
{

	if (pHandle->i2c_write(pHandle->rtc_address, (uint8_t *)&u8Reg, 1) != 0) /**< read data */
	{
		return 1; /**< read fail */
	}
    if (pHandle->i2c_read(pHandle->rtc_address, (uint8_t *)pBuf, u8Length) != 0) /**< read data */
    {
        return 1; /**< read fail */
    }
    return 0;                                           /**< return success */
}

/**
* @brief i2c eeprom write byte
* @param[in] *pHandle points to mcp794xx handle structure
* @param[in] u8Reg is the slave device register address
* @param[in] *pBuf point to data to write
* @param[in] u8Length is the data length to write (number of byte)
* @return status code
            - 0 success
            - 1 failed to write
* @note none
*/
uint8_t eeprom_mcp794xx_i2c_write(mcp794xx_handle_t *const pHandle, uint8_t u8Reg, uint8_t *pBuf, uint8_t u8Length)
{
	int index;
	uint8_t tempBuffer[u8Length + 1];    //len +1
	tempBuffer[0] = u8Reg;

	for(index = 1; index < (u8Length + 1); index++){
		tempBuffer[index] = pBuf[index - 1];
	}

    if (pHandle->i2c_write(pHandle->eeprom_address, (uint8_t *)tempBuffer, (u8Length + 1)) != MCP794XX_DRV_OK) /**< read data   (size is len +1) */
    {
        return MCP794XX_DRV_FAILED; /**< read fail */
    }
    return MCP794XX_DRV_OK;                                            /**< return success */

}

/**
* @brief i2c eeprom read byte
* @param[in] *pHandle points to mcp794xx handle structure
* @param[in] u8Reg is the slave device register address
* @param[out] *pBuf point to data to read
* @param[in] u8Length is the data length to read (number of byte)
* @return status code
*          - 0 success
*          - 1 failed to read
* @note none
*/
uint8_t eeprom_mcp794xx_i2c_read(mcp794xx_handle_t  *const pHandle, uint16_t u8Reg, uint8_t *pBuf, uint8_t u8Length)
{
	if (pHandle->i2c_write(pHandle->eeprom_address, (uint8_t *)&u8Reg, 1) != MCP794XX_DRV_OK) /**< read data */
	{
		return MCP794XX_DRV_FAILED; /**< read fail */
	}
    if (pHandle->i2c_read(pHandle->eeprom_address, (uint8_t *)pBuf, u8Length) != MCP794XX_DRV_OK) /**< read data */
    {
        return MCP794XX_DRV_FAILED; /**< read fail */
    }
    return MCP794XX_DRV_OK;                                           /**< return success */
}

/**
* @brief i2c sram write byte
* @param[in] *pHandle points to mcp794xx handle structure
* @param[in] u8Reg is the slave device register address
* @param[in] *pBuf point to data to write
* @param[in] u8Length is the data length to write (number of byte)
* @return status code
            - 0 success
            - 1 failed to write
* @note none
*/
uint8_t sram_mcp794xx_i2c_write(mcp794xx_handle_t *const pHandle, uint8_t u8Reg, uint8_t *pBuf, uint8_t u8Length)
{
	int index;
	uint8_t tempBuffer[u8Length + 1];    //len +1
	tempBuffer[0] = u8Reg;

	for(index = 1; index < u8Length; index++){
		tempBuffer[index] = pBuf[index - 1];
	}

    if (pHandle->i2c_write(pHandle->rtc_address, (uint8_t *)tempBuffer, u8Length + 1) != MCP794XX_DRV_OK) /**< read data   (size is len +1) */
    {
        return MCP794XX_DRV_FAILED; /**< read fail */
    }
	return MCP794XX_DRV_OK;
}

/**
* @brief This function prints the error message
* @param[in] *pHandle points to mcp794xx handle structure
* @param[in] *pBuffer point to the string to be printed
* @return none
* @note   none
*/
void a_mcp794xx_print_error_msg(mcp794xx_handle_t *const pHandle, char *const pBuffer)
{
#ifdef MCP794XX_DEBUG_MODE
    pHandle->debug_print("mcp794xx: failed to %s.\r\n", pBuffer);
#endif // MCP794XX_DEBUG_MODE
}

/**
 * @brief decimal to BCD
 * @param[in] u8Decimal is the decimal value to be converted
 * @return BCD data
 * @note    none
 */
uint8_t a_pcf85xxx_dec2bcd(uint8_t u8Decimal)
{
    return (((u8Decimal / 10) << 4) | (u8Decimal % 10));
}

/**
 * @brief BCD to decimal
 * @param[in] u8Bcd is the BCD value to be converted
 * @return decimal data
 * @note     none
 */
uint8_t a_pcf85xxx_bcd2dec(uint8_t u8Bcd)
{
    return ( (u8Bcd/16*10) + (u8Bcd%16) );
}

/**
 * @brief     This function initialize the chip
 * @param[in] pHandle points to mcp794xx pHandle structure
 * @return  status code
 *            - 0 success
 *            - 1 i2c initialization failed
 *            - 2 pHandle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t mcp794xx_init(mcp794xx_handle_t *const pHandle)
{
    if (pHandle == NULL)
        return 2;
    if (pHandle->debug_print == NULL)
        return 3;
    if (pHandle->i2c_init == NULL)
    {
#ifdef MCP794XX_DEBUG_MODE
        pHandle->debug_print("mcp794xx: i2c initialize is null\n");
#endif
        return 3;
    }
    if (pHandle->i2c_deinit == NULL)
    {
#ifdef MCP794XX_DEBUG_MODE
        pHandle->debug_print("mcp794xx: i2c_deint is null\n");
#endif
        return 3;
    }
    if (pHandle->i2c_read == NULL)
    {
#ifdef MCP794XX_DEBUG_MODE
        pHandle->debug_print("mcp794xx: i2c_read is null\n");
#endif
        return 3;
    }
    if (pHandle->i2c_write == NULL)
    {
#ifdef MCP794XX_DEBUG_MODE
        pHandle->debug_print("mcp794xx: i2c_write is null\n");
#endif
        return 3;
    }
    if (pHandle->receive_callback == NULL)
    {
#ifdef MCP794XX_DEBUG_MODE
        pHandle->debug_print("mcp794xx: receive_callback\n");
#endif
        return 3;
    }
    if (pHandle->delay_ms == NULL)
    {
#ifdef MCP794XX_DEBUG_MODE
        pHandle->debug_print("mcp794xx: delay_ms\n");
#endif
    }
    if (pHandle->i2c_init())
    {
#ifdef MCP794XX_DEBUG_MODE
        pHandle->debug_print("mcp794xx: i2c initialize failed\n");
#endif
        return 1;
    }

    pHandle->inited = 1; /* flag finish initialization */

    return 0;
}

/**
 * @brief     This function close the chip
 * @param[in] pHandle points to mcp794xx pHandle structure
 * @return  status code
 *            - 0 success
 *            - 1 i2c de-init failed
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      none
 */
uint8_t mcp794xx_deinit(mcp794xx_handle_t *const pHandle)
{
    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(pHandle->i2c_deinit() != 0)
    {
        a_mcp794xx_print_error_msg(pHandle, "de-initialize i2c");
        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief     This function execute irq pHandler
 * @param[in] alarm is the alarm number (ALARM0/ALARM1)
 * @param[in] pHandle points to mcp794xx pHandle structure
 * @return  status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      none
 */
uint8_t mcp794xx_irq_pHandler(mcp794xx_handle_t *const pHandle, mcp794xx_alarm_t alarm)
{
    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

	switch(alarm)
	{
		case MCP794XX_ALARM0:
		{
			pHandle->receive_callback(MCP794XX_ALARM0);
			break;
		}

		case MCP794XX_ALARM1:
		{
			pHandle->receive_callback(MCP794XX_ALARM1);
			break;
		}

		default:
			return 1;
		break;
	}

    return 0;              /**< success */
}

/**
 * @brief     This function set the address pin
 * @param[in] pHandle points to a mcp794xx pHandle structure
 * @return  status code
 *            - 0 success
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      none
 */
uint8_t mcp794xx_set_addr_pin(mcp794xx_handle_t *const pHandle)
{

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    pHandle->rtc_address = MCP794XX_RTC_IIC_ADDRESS;
    pHandle->eeprom_address = MCP794XX_EEPROM_IIC_ADDRESS;
    if((pHandle->rtc_address != MCP794XX_RTC_IIC_ADDRESS) || (pHandle->eeprom_address != MCP794XX_EEPROM_IIC_ADDRESS))
    {
        a_mcp794xx_print_error_msg(pHandle, "set i2c slave address");
        return 1;           /**< failed error */
    }

    return 0;           /**< success */
}

/**
 * @brief     This function set the chip variant
 * @param[in] pHandle points to mcp794xx handle structure
 * @param[in] variant is the chip variant type
 * @return    status code
 *            - 0 success
 *            - 1 set variant failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp794xx_set_variant(mcp794xx_handle_t *const pHandle, mcp794xx_variant_t variant)
{
    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    pHandle->device_variant = variant;
    if(pHandle->device_variant != variant)
    {
        a_mcp794xx_print_error_msg(pHandle, "set variant");
        return 1;           /**< failed error */
    }

    return 0;           /**< success */
}

/**
 * @brief     This function get the chip variant
 * @param[in] pHandle points to mcp794xx handle structure
 * @param[out] pVariant point to the chip variant type
 * @return    status code
 *            - 0 success
 *            - 1 get variant failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp794xx_get_variant(mcp794xx_handle_t *const pHandle, mcp794xx_variant_t *pVariant)
{
    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    *pVariant = pHandle->device_variant;
    if( *pVariant != pHandle->device_variant)
    {
        a_mcp794xx_print_error_msg(pHandle, "get variant");
        return 1;           /**< failed error */
    }

    return 0;           /**< success */
}

/**
 * @brief     This function set the interface debug print buffer size
 * @param[in] pHandle points to mcp794xx handle structure
 * @param[in] buffer_size is the size in byte
 * @return    status code
 *            - 0 success
 *            - 1 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp794xx_set_debug_print_buffer_size(mcp794xx_handle_t *const pHandle, mcp794xx_degub_print_buffer_size_t bf_size)
{
    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    pHandle->buffer_size = bf_size;
    if(pHandle->buffer_size != bf_size)
    {
        a_mcp794xx_print_error_msg(pHandle,"set debug print function buffer size");
        return 1;
    }
    return 0;       /**< success */
}

/**
 * @brief     This function get the interface debug print buffer size
 * @param[in] pHandle points to mcp794xx handle structure
 * @param[out] buffer_size point to the size in byte
 * @return    status code
 *            - 0 success
 *            - 1 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp794xx_get_debug_print_buffer_size(mcp794xx_handle_t *const pHandle, mcp794xx_degub_print_buffer_size_t *pBuffer_size)
{
    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    *pBuffer_size = pHandle->buffer_size;
    if(*pBuffer_size != pHandle->buffer_size)
    {
        a_mcp794xx_print_error_msg(pHandle,"get debug print function buffer size");
        return 1;
    }
    return 0;           /**< success */
}

/**
 * @brief     This function set the time and date
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] pTime point to the date and time structure object
 * @return    status code
 *            - 0 success
 *            - 1 set time failed
 *            - 2 handle or time is NULL
 *            - 3 handle is not initialized
 *            - 4 time is invalid
 * @note      none
 */
uint8_t mcp794xx_set_time_and_date(mcp794xx_handle_t *const pHandle, mcp794xx_time_t *pTime)
{

    uint8_t ptimeBuffer[MCP794XX_TIME_BUFFER_SIZE];
    uint8_t status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

        if((pTime->year < 0) || (pTime->year > 99))
        {
            a_mcp794xx_print_error_msg(pHandle, "set time, year can not be less than 0 or greater than 99");
            return 4;
        }

        if((pTime->month < 1) || (pTime->month > 12))
        {
            a_mcp794xx_print_error_msg(pHandle, "set time, month can not be less than 1 or greater than 12");
            return 4;
        }

        if((pTime->date < 1) || (pTime->date > 31))
        {
            a_mcp794xx_print_error_msg(pHandle, "set time, date can not be less than 1 or greater than 31");
            return 4;
        }

        if((pTime->weekDay < 1) || (pTime->weekDay > 7))
        {
            a_mcp794xx_print_error_msg(pHandle, "set time, week day can not be less than 1 or greater than 7");
            return 4;
        }

        if((pTime->minute < 0) || (pTime->minute > 59))
        {
            a_mcp794xx_print_error_msg(pHandle, "set time, minute can not be less than 0 or greater than 59");
            return 4;
        }

        if((pTime->second < 0) || (pTime->second > 59))
        {
            a_mcp794xx_print_error_msg(pHandle, "set time, seconds can not be less than 0 or greater than 59");
            return 4;
        }


    if(pHandle->time_param.time_Format == MCP794XX_24HR_FORMAT)
    {

        if((pTime->hour < 0) || (pTime->hour > 23))
        {
            a_mcp794xx_print_error_msg(pHandle, "set time, hour can not be less than 0 or greater than 23");
            return 4;
        }
    }

    else if(pHandle->time_param.time_Format == MCP794XX_12HR_FORMAT)
    {

        if((pTime->hour < 1) || (pTime->hour > 12))
        {
            a_mcp794xx_print_error_msg(pHandle, "set time, hour can not be less than 1 or greater than 12");
            return 4;
        }

    }

    else
    {
        a_mcp794xx_print_error_msg(pHandle, "set time, invalid time format");
        return 4;
    }

    memset(ptimeBuffer, 0, sizeof(ptimeBuffer));
    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_RTC_SECOND_REG, (uint8_t *)ptimeBuffer, MCP794XX_TIME_BUFFER_SIZE);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read time before write");
        return 1;
    }

    status = (ptimeBuffer[0] & MCP794XX_ST_MASK) | a_pcf85xxx_dec2bcd(pTime->second);
    err =  rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_SECOND_REG,  (uint8_t *)&status);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "to write seconds");
        return 1;
    }

    status = a_pcf85xxx_dec2bcd(pTime->minute);
    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_MINUTE_REG, (uint8_t *)&status);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "to write minutes");
        return 1;
    }

    if(pHandle->time_param.time_Format == MCP794XX_24HR_FORMAT)
    {

        status = a_pcf85xxx_dec2bcd(pTime->hour);
        err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_HOUR_REG,  (uint8_t *)&status);
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "to write hours");
            return 1;
        }
    }

    else if(pHandle->time_param.time_Format == MCP794XX_12HR_FORMAT)
    {
        status = a_pcf85xxx_dec2bcd(pTime->hour) |(ptimeBuffer[2] & (MCP794XX_12HR_24HR_FRMT_STAT_MASK | MCP794XX_TIME_AM_PM_MASK));
        err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_HOUR_REG, (uint8_t *)&status);
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "to write hours");
            return 1;
        }

        err = mcp794xx_set_am_pm(pHandle, MCP794XX_RTC_HOUR_REG, pTime->am_pm_indicator);
        if(err)
        {
            return err;
        }
    }

    status = a_pcf85xxx_dec2bcd(pTime->weekDay) | (ptimeBuffer[3] & (MCP794XX_OSC_RUN_STATUS_MASK | MCP794XX_PWR_FAIL_STATUS_MASK | MCP794XX_VBAT_EN_MASK));
    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_WKDAY_REG, (uint8_t *)&status);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "to write week day");
        return 1;
    }

    status = a_pcf85xxx_dec2bcd(pTime->date);
    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_DATE_REG, (uint8_t *)&status);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "to write date");
        return 1;
    }

    status = a_pcf85xxx_dec2bcd(pTime->month) | (a_pcf85xxx_bcd2dec(ptimeBuffer[5] & MCP794XX_LEAP_YEAR_STATUS_MASK));
    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_MONTH_REG,  (uint8_t *)&status);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "to write month");
        return 1;
    }

    status = a_pcf85xxx_dec2bcd(pTime->year);
    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_YEAR_REG, (uint8_t *)&status);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "to write year");
        return 1;
    }

    return 0;           /**< success */
}

/**
 * @brief      This function get the current time and date
 * @param[in]  pHandle points to a mcp794xx handle structure
 * @param[out] pTime points to a time structure object
 * @return     status code
 *             - 0 success
 *             - 1 set time failed
 *             - 2 handle or time is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp794xx_get_time_and_date(mcp794xx_handle_t *const pHandle, mcp794xx_time_t *pTime)
{
    uint8_t timeBuffer[MCP794XX_TIME_BUFFER_SIZE];

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_RTC_SECOND_REG, (uint8_t *)timeBuffer, MCP794XX_TIME_BUFFER_SIZE);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read time and date");
        return 1;           /**< failed error */
    }

    pTime->second = a_pcf85xxx_bcd2dec(timeBuffer[0] & MCP794XX_SEC_BCD_MASK);
    pTime->minute = a_pcf85xxx_bcd2dec(timeBuffer[1] & MCP794XX_MIN_BCD_MASK);
    if(pHandle->time_param.time_Format == MCP794XX_24HR_FORMAT)
    {
        pTime->hour = a_pcf85xxx_bcd2dec(timeBuffer[2] & MCP794XX_24HR_FRMT_BCD_MASK);
    }
    else if(pHandle->time_param.time_Format == MCP794XX_12HR_FORMAT)
    {
        pTime->hour = a_pcf85xxx_bcd2dec(timeBuffer[2] & MCP794XX_12HR_FRMT_BCD_MASK);
        pTime->am_pm_indicator = ((timeBuffer[2] & MCP794XX_TIME_AM_PM_MASK) >> 5) ;
    }
    pTime->weekDay = a_pcf85xxx_bcd2dec(timeBuffer[3] & MCP794XX_WKDAY_BCD_MASK);
    pTime->date = a_pcf85xxx_bcd2dec(timeBuffer[4] & MCP794XX_DATE_BCD_MASK);
    pTime->month = a_pcf85xxx_bcd2dec(timeBuffer[5] & MCP794XX_MONTH_BCD_MASK);
    pTime->year = a_pcf85xxx_bcd2dec(timeBuffer[6] & MCP794XX_YEAR_BCD_MASK) + 2000;

    return 0;           /**< success */
}

/**
 * @brief This function set the time format 12hr or 24hr
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] u8Register is the register to set hour format
 * @param[in] format hour format
 * @return status code
 *          - 0 success
 *          - 1 failed to set time format
 * @note        none
 */
uint8_t mcp794xx_set_hour_format(mcp794xx_handle_t *const pHandle, uint8_t u8Register, mcp794xx_time_format_t format)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;      /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, u8Register, (uint8_t*)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read hour format before write");
        return 1;            /**< failed error */
    }

    read_status &= ~(1 << 6);            /*< clear hour format status bit */
    read_status |= (format << 6);        /*< write hour format status bit */

    err = rtc_mcp794xx_i2c_write(pHandle, u8Register, (uint8_t*)&read_status);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "set hour format ");
        return 1;           /**< failed error */
    }

    pHandle->time_param.time_Format = format;

    return 0;              /**< success */
}

/**
 * @brief This function get the time format 12hr or 24hr
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] u8Register is the register to read hour format
 * @param[out] pFormat format point to hour format (12hr/24hr)
 * @return status code
 *          - 0 success
 *          - 1 failed to set time format
 * @note        none
 */
uint8_t mcp794xx_get_hour_format(mcp794xx_handle_t *const pHandle, uint8_t u8Register, mcp794xx_time_format_t *pFormat)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, u8Register, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "get hour format");
        return 1;           /**< failed error */
    }

    *pFormat = (mcp794xx_time_format_t)((read_status & MCP794XX_ALARMx_12HR_24HR_FRMT_STAT_MASK) >> 6);

    pHandle->time_param.time_Format = *pFormat;

    *pFormat =  pHandle->time_param.time_Format;

    return 0;              /**< success */
}

/**
 * @brief This function set AM or PM indicator for 12hr time format
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] u8Register is the register to set am/pm indicator
 * @param[in] am_pm is the indicator period for 12hr time format
 * @return status code
 *          - 0 success
 *          - 1 failed to set AM/PM period
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_set_am_pm(mcp794xx_handle_t *const pHandle, uint8_t u8Register, mcp794xx_am_pm_indicator_t am_pm)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, u8Register, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read time am/pm indicator before write");
        return 1;           /**< failed error */
    }

    read_status &= ~(1 << 5);
    read_status |= (am_pm << 5);

    err = rtc_mcp794xx_i2c_write(pHandle, u8Register, (uint8_t *)&read_status);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "set time am/pm indicator");
        return 1;           /**< failed error */
    }


    return 0;              /**< success */
}

/**
 * @brief This function get the AM or PM time indicator for 12hr time format
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] u8Register is the register to read am/pm status
 * @param[out] pAm_pm point to the indicator period for 12hr time format
 * @return status code
 *          - 0 success
 *          - 1 failed to get AM/PM period
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_get_am_pm(mcp794xx_handle_t *const pHandle, uint8_t u8Register, mcp794xx_am_pm_indicator_t *pAm_pm)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, u8Register, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read time am/pm indicator");
        return 1;           /**< failed error */
    }

    *pAm_pm = (mcp794xx_am_pm_indicator_t)((read_status & MCP794XX_ALARMx_AM_PM_MASK) >> 5);

    return 0;              /**< success */
}

/**
 * @brief This function set the oscillator status bit
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] status point to the status to be set
 * @return status code
 *          - 0 success
 *          - 1 failed to set oscillator status bit
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_set_osc_status(mcp794xx_handle_t *const pHandle, mcp794xx_osc_status_t status)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_RTC_SECOND_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read oscillator status before write");
        return 1;           /**< failed error */
    }
    read_status &= ~(1 << 7);
    read_status |= (status << 7);

    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_SECOND_REG, (uint8_t*)&read_status);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "set oscillator status");
        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief This function get the oscillator status bit
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[out] pStatus point to the status to be set
 * @return status code
 *          - 0 success
 *          - 1 failed to set oscillator status bit
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_get_osc_status(mcp794xx_handle_t *const pHandle, mcp794xx_osc_status_t *pStatus)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_RTC_SECOND_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "get oscillator status");
        return 1;           /**< failed error */
    }

    *pStatus = (mcp794xx_osc_status_t)((read_status & MCP794XX_ST_MASK) >> 7);

    return 0;              /**< success */
}

/**
 * @brief This function clears the power fail status bit
 * @param[in] pHandle points to a mcp794xx handle structure
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_clr_pwr_fail_status(mcp794xx_handle_t *const pHandle)
{

    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(pHandle->device_variant == MCP7940M_VARIANT){
         a_mcp794xx_print_error_msg(pHandle, "clear pwr fail status, not supported for the mcp7940M variant");
        return 4;
    }

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_RTC_WKDAY_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read status reg");
        return 1;           /**< failed error */
    }

    read_status &= ~(1 << 4);
    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_WKDAY_REG, (uint8_t*)&read_status);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "clear pwr fail status");
        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief This function get the power fail status bit
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[out] pStatus point to the status be set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 * @note  * @note    The PWRFAIL bit must be cleared to log new timestamp data.
 */
uint8_t mcp794xx_get_pwr_fail_status(mcp794xx_handle_t *const pHandle, mcp794xx_pwr_fail_status_t *pStatus)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(pHandle->device_variant == MCP7940M_VARIANT){
         a_mcp794xx_print_error_msg(pHandle, "get pwr fail status, not supported for the mcp7940M variant");
        return 4;
    }

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_RTC_WKDAY_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read status reg");
        return 1;           /**< failed error */
    }

    *pStatus = (mcp794xx_pwr_fail_status_t)(read_status & 0x10) >> 4;

    return 0;              /**< success */
}

/**
 * @brief This function get the power fail time stamp
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] powerMode is the power fail mode (power-up/power-down)
 * @param[out] pTime point to the time structure object
 * @return status code
 *          - 0 success
 *          - 1 failed
 *          - 2 handle null
 *          - 3 handle is not initialized
 *          - 4 invalid power failure mode
 * @note    - The PWRFAIL bit must be cleared to log new timestamp data.
            - Reading or writing the external battery back up bit will clear the pwr fail time stamp.
            - Time stamp should be read at start up before reading or setting current time and date.
 */
uint8_t mcp794xx_get_pwr_fail_time_stamp(mcp794xx_handle_t *const pHandle, mcp794xx_pwr_fail_type_t powerMode, mcp794xx_time_t *pTime)
{

    uint8_t timeBuffer[MCP794XX_PWRFAIL_BUFFER_LENGTH];

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(pHandle->device_variant == MCP7940M_VARIANT){
         a_mcp794xx_print_error_msg(pHandle, "read power-up time stamp, not supported for the mcp7940M variant");
        return 4;
    }

   switch((int)powerMode)
   {
    case MCP794XX_PWR_DOWN_TIME_STAMP:
        {
            err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_PWR_DWN_MIN_REG, (uint8_t *)timeBuffer, MCP794XX_PWRFAIL_BUFFER_LENGTH);
            if(err)
            {
                a_mcp794xx_print_error_msg(pHandle, "read power-down time stamp");
                return 1;           /**< failed error */
            }

            break;

        }

    case MCP794XX_PWR_UP_TIME_STAMP:
        {

            err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_PWR_UP_MIN_REG, (uint8_t *)timeBuffer, MCP794XX_PWRFAIL_BUFFER_LENGTH);
            if(err)
            {
                a_mcp794xx_print_error_msg(pHandle, "read power-up time stamp");
                return 1;           /**< failed error */
            }

            break;
        }

    default:
         a_mcp794xx_print_error_msg(pHandle, "invalid pwr fail mode selected");
        return 4;   /**< invalid power failure mode */
        break;

   }

    pTime->minute = a_pcf85xxx_bcd2dec(timeBuffer[0] & MCP794XX_PWRXX_MIN_BCD_MASK);         /**< read the power failure minute timestamp */
    if(pHandle->time_param.time_Format == MCP794XX_24HR_FORMAT)                              /**< read the power failure hour timestamp in 24hr format*/
    {
        pTime->hour = a_pcf85xxx_bcd2dec(timeBuffer[1] & MCP794XX_PWRXX_24HR_FRMT_BCD_MASK);

    }
    else if(pHandle->time_param.time_Format == MCP794XX_12HR_FORMAT)                          /**< read the power failure hour timestamp in 12hr format*/
    {
        pTime->hour = a_pcf85xxx_bcd2dec(timeBuffer[1] & MCP794XX_PWRXX_12HR_FRMT_BCD_MASK);
        pTime->am_pm_indicator = ((timeBuffer[1] & MCP794XX_PWRXX_TIME_AM_PM_MASK) >> 5) ;
    }
    pTime->date = a_pcf85xxx_bcd2dec(timeBuffer[2] & MCP794XX_PWRXX_DATE_BCD_MASK);          /**< read the power failure date */
    pTime->month = a_pcf85xxx_bcd2dec(timeBuffer[3] & MCP794XX_MONTH_BCD_MASK);              /**< read the power failure month timestamp */

    return 0;  /**< success */
}

/**
 * @brief This function set the am/pm and time format for pwr up/down time stamp
 * @param[in] pHandle points to a mcp794xx handle structure
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_set_pwr_fail_time_param(mcp794xx_handle_t *const pHandle)
{
	uint8_t format, am_pm_indicator;

    if(pHandle->device_variant == MCP7940M_VARIANT){
         a_mcp794xx_print_error_msg(pHandle, "pwr fail param, not supported for the mcp7940M variant");
        return 4;
    }

	err = mcp794xx_get_hour_format(pHandle, MCP794XX_RTC_HOUR_REG, &format);
	if(err){
	  return err;
	}

	err = mcp794xx_get_am_pm(pHandle, MCP794XX_RTC_HOUR_REG, &am_pm_indicator);
	if(err){
	  return err;
	}

	err = mcp794xx_set_hour_format(pHandle, MCP794XX_PWR_DWN_HOUR_REG, format);          /**< set time format for power down time stamp */
	if(err){
	  return err;
	}

	err = mcp794xx_set_hour_format(pHandle, MCP794XX_PWR_UP_HOUR_REG, format);          /**< set time format for power up time stamp */
	if(err){
	  return err;
	}

	err = mcp794xx_set_am_pm(pHandle, MCP794XX_PWR_DWN_HOUR_REG,  am_pm_indicator);
	if(err){
	  return err;
	}
	err = mcp794xx_set_am_pm(pHandle, MCP794XX_PWR_UP_HOUR_REG,  am_pm_indicator);
	if(err){
	  return err;
	}
	return 0; 			/**< success */
}

/**
 * @brief This function get the leap year status
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[out] pLeap_year_status point to the leap year status bit
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_get_leap_year_status(mcp794xx_handle_t *const pHandle, mcp794xx_leap_year_status_t *pLeap_year_status)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_RTC_MONTH_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read leap year register");
        return 1;           /**< failed error */
    }

    *pLeap_year_status = (mcp794xx_leap_year_status_t) ((read_status & MCP794XX_LEAP_YEAR_STATUS_MASK) >> 5);

    return 0;              /**< success */
}

/**
* @brief This function set the alarm enable status bit
* @param[in] pHandle points to a mcp794xx handle structure
* @param[in] status is the enable boolean status to be set
* @param[in] alarm is the number 1/0 to enable
* @return status code
*          - 0 success
*          - 1 failed
           - 2 handle null
*          - 3 handle is not initialized
*/
uint8_t mcp794xx_set_alarm_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_alarm_t alarm, mcp794xx_bool_t status)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_CONTROL_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read control register");
        return 1;           /**< failed error */
    }

    if(alarm == MCP794XX_ALARM0)
    {
        read_status &= ~(1 << 4);           /**< clear alarm enable status */
        read_status |= (status << 4);
        err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_CONTROL_REG, (uint8_t *)&read_status);
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "write alarm enable status");
            return 1;           /**< failed error */
        }

    }
    if(MCP794XX_ALARM1)
    {
        read_status &= ~(1 << 5);           /**< clear alarm enable status */
        read_status |= (status << 5);
        err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_CONTROL_REG, (uint8_t *)&read_status);
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "write alarm enable status");
            return 1;           /**< failed error */
        }
    }
    else
    {
        a_mcp794xx_print_error_msg(pHandle, "invalid alarm selected");
        return 4;           /**< invalid alarm */
    }

    return 0;              /**< success */
}

 /**
 * @brief This function set the alarm enable status bit
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] alarm is the alarm to read status of 1/0
 * @param[out] pStatus point to the enable boolean status set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_get_alarm_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_alarm_t alarm, mcp794xx_bool_t *pStatus)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_CONTROL_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read alarm enable register");
        return 1;           /**< failed error */
    }

    if(alarm == MCP794XX_ALARM0)
    {
        *pStatus = (mcp794xx_bool_t) ((read_status & MCP794XX_ALRAM0_ENABLE_MASK) >> 4);

    }
    else if(MCP794XX_ALARM1)
    {
        *pStatus = (mcp794xx_bool_t) ((read_status & MCP794XX_ALRAM1_ENABLE_MASK) >> 5);
    }
    else
    {
        a_mcp794xx_print_error_msg(pHandle, "invalid alarm selected");
        return 4;           /**< invalid alarm */
    }

    return 0;               /**< success */
}

/**
 * @brief This function set the alarm interrupt output pin polarity
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] polarity is the polarity to be set
 * @param[in] alarm is the alarm number 1/0 to set interrupt polarity
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 *          - 4 invalid alarm
 * @note none
 */
uint8_t mcp794xx_set_alarm_interrupt_output_polarity(mcp794xx_handle_t *const pHandle, mcp794xx_alarm_t alarm, mcp794xx_int_output_polarity_t polarity)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    switch((int)alarm)
    {

        case MCP794XX_ALARM0:
        {
            err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_ALM0_WKDAY_REG, (uint8_t *)&read_status, 1);
            if(err)
            {
                a_mcp794xx_print_error_msg(pHandle, "read alarm 0 polarity register");
                return 1;           /**< failed error */
            }

            read_status &= ~(1 << 7);                                   /**< clear polarity bit */
            read_status |= (polarity << 7);

            err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_ALM0_WKDAY_REG, (uint8_t *)&read_status);
            if(err)
            {
                a_mcp794xx_print_error_msg(pHandle, "read alarm 0 polarity register");
                return 1;           /**< failed error */
            }
            break;
        }

        case MCP794XX_ALARM1:
        {
            err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_ALM1_WKDAY_REG, (uint8_t *)&read_status, 1);
            if(err)
            {
                a_mcp794xx_print_error_msg(pHandle, "read alarm 1 polarity register");
                return 1;           /**< failed error */
            }

            read_status &= ~(1 << 7);                                   /**< clear polarity bit */
            read_status |= (polarity << 7);

            err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_ALM1_WKDAY_REG, (uint8_t *)&read_status);
            if(err)
            {
                a_mcp794xx_print_error_msg(pHandle, "read alarm 0 polarity register");
                return 1;           /**< failed error */
            }

            break;
        }

        default :
        {
            a_mcp794xx_print_error_msg(pHandle, "invalid alarm selected");
            return 4;
            break;
        }

    }

    return 0;              /**< success */
}

/**
 * @brief This function get the alarm interrupt output pin polarity
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[out] pPolarity point to the the polarity read
 * @param[in] alarm is the alarm number 1/0 to set interrupt polarity
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 *          - 4 invalid alarm
 * @note none
 */
uint8_t mcp794xx_get_alarm_interrupt_output_polarity(mcp794xx_handle_t *const pHandle, mcp794xx_alarm_t alarm, mcp794xx_int_output_polarity_t *pPolarity)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

     switch((int)alarm)
    {

        case MCP794XX_ALARM0:
        {
            err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_ALM0_WKDAY_REG, (uint8_t *)&read_status, 1);
            if(err)
            {
                a_mcp794xx_print_error_msg(pHandle, "read alarm 0 polarity register");
                return 1;           /**< failed error */
            }

            *pPolarity = (mcp794xx_int_output_polarity_t) ((read_status & MCP794XX_ALARMx_INT_POL_MASK)>> 7);
            break;
        }

        case MCP794XX_ALARM1:
        {
            err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_ALM1_WKDAY_REG, (uint8_t *)&read_status, 1);
            if(err)
            {
                a_mcp794xx_print_error_msg(pHandle, "read alarm 1 polarity register");
                return 1;           /**< failed error */
            }
            *pPolarity = (mcp794xx_int_output_polarity_t) ((read_status & MCP794XX_ALARMx_INT_POL_MASK)>> 7);

            break;
        }

        default :
        {
            a_mcp794xx_print_error_msg(pHandle, "invalid alarm selected");
            return 4;
            break;
        }

    }

    return 0;              /**< success */
}

/**
 * @brief This function set the alarm time and date
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] alarm is the alarm type (ALARM0/ALARM1)
 * @param[in] pTime point to the alarm time to set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 *          - 5 invalid mask
 */
uint8_t mcp794xx_set_alarm_time_date(mcp794xx_handle_t *const pHandle, mcp794xx_alarm_t alarm, mcp794xx_time_t *pTime)
{

    uint8_t ptimeBuffer[MCP794XX_TIME_BUFFER_SIZE];
    uint8_t status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(pHandle->time_param.time_Format == MCP794XX_24HR_FORMAT)
    {

        if((pTime->month < 1) || (pTime->month > 12))
        {
            a_mcp794xx_print_error_msg(pHandle, "set alarm time, month can not be less than 0 or greater than 12");
            return 4;
        }

        if((pTime->date < 1) || (pTime->date > 31))
        {
            a_mcp794xx_print_error_msg(pHandle, "set alarm time, date can not be less than 0 or greater than 31");
            return 4;
        }

        if((pTime->weekDay < 1) || (pTime->weekDay > 7))
        {
            a_mcp794xx_print_error_msg(pHandle, "set alarm time, week day can not be less than 1 or greater than 7");
            return 4;
        }

        if((pTime->hour < 0) || (pTime->hour > 23))
        {
            a_mcp794xx_print_error_msg(pHandle, "set alarm time, hour can not be less than 0 or greater than 23");
            return 4;
        }

        if((pTime->minute < 0) || (pTime->minute > 59))
        {
            a_mcp794xx_print_error_msg(pHandle, "set alarm time, minute can not be less than 0 or greater than 59");
            return 4;
        }

        if((pTime->second < 0) || (pTime->second > 59))
        {
            a_mcp794xx_print_error_msg(pHandle, "set alarm time, seconds can not be less than 0 or greater than 59");
            return 4;
        }

    }

    else if(pHandle->time_param.time_Format == MCP794XX_12HR_FORMAT)
    {

        if((pTime->month < 0) || (pTime->month > 12))
        {
            a_mcp794xx_print_error_msg(pHandle, "set alarm time, month can not be less than 0 or greater than 12");
            return 4;
        }

        if((pTime->date < 1) || (pTime->date > 31))
        {
            a_mcp794xx_print_error_msg(pHandle, "set alarm time, date can not be less than 0 or greater than 31");
            return 4;
        }

        if((pTime->weekDay < 1) || (pTime->weekDay > 7))
        {
            a_mcp794xx_print_error_msg(pHandle, "set alarm time, week day can not be less than 1 or greater than 7");
            return 4;
        }

        if((pTime->hour < 1) || (pTime->hour > 12))
        {
            a_mcp794xx_print_error_msg(pHandle, "set alarm time, hour can not be less than 0 or greater than 12");
            return 4;
        }

        if((pTime->minute < 0) || (pTime->minute > 59))
        {
            a_mcp794xx_print_error_msg(pHandle, "set alarm time, minute can not be less than 0 or greater than 59");
            return 4;
        }

        if((pTime->second < 0) || (pTime->second > 59))
        {
            a_mcp794xx_print_error_msg(pHandle, "set alarm time, seconds can not be less than 0 or greater than 59");
            return 4;
        }

    }

    else
    {
        a_mcp794xx_print_error_msg(pHandle, "set alarm time, invalid time format");
        return 4;
    }

    memset(ptimeBuffer, 0, sizeof(ptimeBuffer));              /**< clear buffer */

    if(alarm == MCP794XX_ALARM0)
    {

        err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_ALM0_SEC_REG, (uint8_t *)ptimeBuffer, (MCP794XX_TIME_BUFFER_SIZE - 1) );

        status =  a_pcf85xxx_dec2bcd(pTime->second);
        err =  rtc_mcp794xx_i2c_write(pHandle, MCP794XX_ALM0_SEC_REG,(uint8_t *)&status);
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "to write alarm 0 seconds");
            return 1;
        }

        status = a_pcf85xxx_dec2bcd(pTime->minute);
        err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_ALM0_MIN_REG, (uint8_t *)&status);
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "to write alarm 0 minutes");
            return 1;
        }

        if(pHandle->time_param.time_Format == MCP794XX_24HR_FORMAT)
        {
            status = a_pcf85xxx_dec2bcd(pTime->hour) | (ptimeBuffer[2] & MCP794XX_ALARMx_12HR_24HR_FRMT_STAT_MASK);
            err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_ALM0_HOUR_REG,(uint8_t *)&status);
            if(err)
            {
                a_mcp794xx_print_error_msg(pHandle, "to write alarm 0 hours");
                return 1;
            }
        }

        else if(pHandle->time_param.time_Format == MCP794XX_12HR_FORMAT)
        {
        	status = a_pcf85xxx_dec2bcd(pTime->hour) | (ptimeBuffer[2] & (MCP794XX_ALARMx_12HR_24HR_FRMT_STAT_MASK | MCP794XX_ALARMx_AM_PM_MASK));
            err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_ALM0_HOUR_REG, (uint8_t *)&status);
            if(err)
            {
                a_mcp794xx_print_error_msg(pHandle, "to write alarm 0 hours");
                return 1;
            }
            err = mcp794xx_set_am_pm(pHandle, MCP794XX_ALM0_HOUR_REG, pTime->am_pm_indicator);
            if(err)
            {
                return err;
            }
        }

        status = a_pcf85xxx_dec2bcd(pTime->weekDay) | (ptimeBuffer[3] & (MCP794XX_ALARMx_INT_POL_MASK | MCP794XX_ALARMx_TYPE_MASK | MCP794XX_ALARMx_IF_MASK));
        err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_ALM0_WKDAY_REG, (uint8_t *)&status);
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "to write alarm 0 week day");
            return 1;
        }

        status = a_pcf85xxx_dec2bcd(pTime->date);
        err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_ALM0_DATE_REG, (uint8_t *)&status);
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "to write alarm 0 date");
            return 1;
        }

        status =  a_pcf85xxx_dec2bcd(pTime->month);
        err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_ALM0_MONTH_REG, (uint8_t *)&status);
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "to write alarm 0 month");
            return 1;
        }
    }
    else if(alarm == MCP794XX_ALARM1)
    {

        err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_ALM1_SEC_REG, (uint8_t *)ptimeBuffer, (MCP794XX_TIME_BUFFER_SIZE - 1) );

        status =  a_pcf85xxx_dec2bcd(pTime->second);
		err =  rtc_mcp794xx_i2c_write(pHandle, MCP794XX_ALM1_SEC_REG,(uint8_t *)&status);
		if(err)
		{
			a_mcp794xx_print_error_msg(pHandle, "to write alarm 1 seconds");
			return 1;
		}

		status = a_pcf85xxx_dec2bcd(pTime->minute);
		err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_ALM1_MIN_REG, (uint8_t *)&status);
		if(err)
		{
			a_mcp794xx_print_error_msg(pHandle, "to write alarm 1 minutes");
			return 1;
		}

		if(pHandle->time_param.time_Format == MCP794XX_24HR_FORMAT)
		{
			status = a_pcf85xxx_dec2bcd(pTime->hour) | (ptimeBuffer[2] & MCP794XX_ALARMx_12HR_24HR_FRMT_STAT_MASK);
			err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_ALM1_HOUR_REG,(uint8_t *)&status);
			if(err)
			{
				a_mcp794xx_print_error_msg(pHandle, "to write alarm 1 hours");
				return 1;
			}
		}

		else if(pHandle->time_param.time_Format == MCP794XX_12HR_FORMAT)
		{
			status = a_pcf85xxx_dec2bcd(pTime->hour) | (ptimeBuffer[2] & (MCP794XX_ALARMx_12HR_24HR_FRMT_STAT_MASK | MCP794XX_ALARMx_AM_PM_MASK));
			err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_ALM1_HOUR_REG, (uint8_t *)&status);
			if(err)
			{
				a_mcp794xx_print_error_msg(pHandle, "to write alarm 1 hours");
				return 1;
			}
			err = mcp794xx_set_am_pm(pHandle, MCP794XX_ALM1_HOUR_REG, pTime->am_pm_indicator);
			if(err)
			{
				return err;
			}
		}

		status = a_pcf85xxx_dec2bcd(pTime->weekDay) | (ptimeBuffer[3] & (MCP794XX_ALARMx_INT_POL_MASK | MCP794XX_ALARMx_TYPE_MASK | MCP794XX_ALARMx_IF_MASK));
		err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_ALM1_WKDAY_REG, (uint8_t *)&status);
		if(err)
		{
			a_mcp794xx_print_error_msg(pHandle, "to write alarm 1 week day");
			return 1;
		}

		status = a_pcf85xxx_dec2bcd(pTime->date);
		err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_ALM1_DATE_REG, (uint8_t *)&status);
		if(err)
		{
			a_mcp794xx_print_error_msg(pHandle, "to write alarm 1 date");
			return 1;
		}

		status =  a_pcf85xxx_dec2bcd(pTime->month);
		err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_ALM1_MONTH_REG, (uint8_t *)&status);
		if(err)
		{
			a_mcp794xx_print_error_msg(pHandle, "to write alarm 1 month");
			return 1;
		}
    }

    else{
        a_mcp794xx_print_error_msg(pHandle, "invalid alarm selected");
        return 4;
    }


    return 0;     /**< success */

}

/**
 * @brief This function set the alarm
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] alarm is the alarm number (ALARM0/ALARM1)
 * @param[in] mask is the alarm mask to be set (second/minute/hour/.. etc.)
 * @param[in] pTime point to the alarm time to set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 *          - 4 invalid alarm
 *          - 5 invalid mask
 */
uint8_t mcp794xx_set_alarm(mcp794xx_handle_t *const pHandle, mcp794xx_alarm_t alarm, mcp794xx_alarm_mask_t mask, mcp794xx_time_t *pTime)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    switch((int)alarm)
    {

        case MCP794XX_ALARM0:
        {
            err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_ALM0_WKDAY_REG, (uint8_t *)&read_status, 1);
            if(err)
            {
                a_mcp794xx_print_error_msg(pHandle, "read alarm 0 mask register");
                return 1;           /**< failed error */
            }

            read_status &= ~(0b111 << 4);                           /**< clear alarm mask bit */
            read_status |= (mask << 4);

            err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_ALM0_WKDAY_REG, (uint8_t *)&read_status);
            if(err)
            {
                a_mcp794xx_print_error_msg(pHandle, "read alarm 0 mask register");
                return 1;           /**< failed error */
            }

            err = mcp794xx_set_alarm_time_date(pHandle, alarm, pTime);
            if(err)
            {
                a_mcp794xx_print_error_msg(pHandle, "failed to set alarm 0");
                return err;           /**< failed error */
            }

            break;
        }

        case MCP794XX_ALARM1:
        {
            err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_ALM1_WKDAY_REG, (uint8_t *)&read_status, 1);
            if(err)
            {
                a_mcp794xx_print_error_msg(pHandle, "read alarm 1 mask register");
                return 1;           /**< failed error */
            }

            read_status &= ~(0b111 << 4);                           /**< clear alarm mask bit */
            read_status |= (mask << 4);

            err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_ALM1_WKDAY_REG, (uint8_t *)&read_status);
            if(err)
            {
                a_mcp794xx_print_error_msg(pHandle, "read alarm 1 mask register");
                return 1;           /**< failed error */
            }

            err = mcp794xx_set_alarm_time_date(pHandle, alarm, pTime);
            if(err)
            {
                a_mcp794xx_print_error_msg(pHandle, "failed to set alarm 1");
                return err;           /**< failed error */
            }

            break;
        }

        default :
        {
            a_mcp794xx_print_error_msg(pHandle, "invalid alarm selected");
            return 4;
            break;
        }

    }

    return 0;              /**< success */

}

/**
 * @brief This function get the alarm time and date
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] alarm is the alarm number (ALARM0/ALARM1)
 * @param[out] pTime point to the alarm time and date to set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 *          - 4 invalid alarm
 */
uint8_t mcp794xx_get_alarm_time_date(mcp794xx_handle_t *const pHandle, mcp794xx_alarm_t alarm, mcp794xx_time_t *pTime)
{
    uint8_t timeBuffer[MCP794XX_TIME_BUFFER_SIZE];

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    memset(timeBuffer, 0, sizeof(timeBuffer));              /**< clear buffer */

    if(alarm == MCP794XX_ALARM0)
    {

        err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_ALM0_SEC_REG, (uint8_t *)timeBuffer, (MCP794XX_TIME_BUFFER_SIZE - 1));    /**< read alarm time and date registers */
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "read alarm 0 time and date");
            return 1;                                        /**< failed error */
        }
    }

    else if(alarm == MCP794XX_ALARM1)
    {
        err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_ALM1_SEC_REG, (uint8_t *)timeBuffer, (MCP794XX_TIME_BUFFER_SIZE - 1));    /**< read alarm time and date registers */
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "read alarm 1 time and date");
            return 1;                                        /**< failed error */
        }
    }
    else
    {
        a_mcp794xx_print_error_msg(pHandle, "invalid alarm selected");
        return 4;
    }

    pTime->second = a_pcf85xxx_bcd2dec(timeBuffer[0] & MCP794XX_ALARMx_SEC_BCD_MASK);
    pTime->minute = a_pcf85xxx_bcd2dec(timeBuffer[1] & MCP794XX_ALARMx_MIN_BCD_MASK);
    if(pHandle->time_param.time_Format == MCP794XX_24HR_FORMAT)
    {
        pTime->hour = a_pcf85xxx_bcd2dec(timeBuffer[2] & MCP794XX_ALARMx_24HR_FRMT_BCD_MASK);
    }
    else if(pHandle->time_param.time_Format == MCP794XX_12HR_FORMAT)
    {
        pTime->hour = a_pcf85xxx_bcd2dec(timeBuffer[2] & MCP794XX_ALARMx_12HR_FRMT_BCD_MASK);
        pTime->am_pm_indicator = ((timeBuffer[2] & MCP794XX_ALARMx_AM_PM_MASK) >> 5) ;
    }
    pTime->weekDay = a_pcf85xxx_bcd2dec(timeBuffer[3] & MCP794XX_ALARMx_WDAY_MASK);
    pTime->date = a_pcf85xxx_bcd2dec(timeBuffer[4] & MCP794XX_ALARMx_DATE_MASK);
    pTime->month = a_pcf85xxx_bcd2dec(timeBuffer[5] & MCP794XX_ALARMx_MONTH_MASK);

    return 0;              /**< success */
}

/**
 * @brief This function get the alarm time and date
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] alarm is the alarm type (ALARM0/ALARM1)
 * @param[out] pMask point to the alarm mask set (second/minute/hour/.. etc)
 * @param[out] pTime point to the alarm time set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 *          - 4 invalid alarm
 */
uint8_t mcp794xx_get_alarm(mcp794xx_handle_t *const pHandle, mcp794xx_alarm_t alarm, mcp794xx_alarm_mask_t *pMask, mcp794xx_time_t *pTime)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = mcp794xx_get_alarm_time_date(pHandle, alarm, pTime);
    if(err){
         a_mcp794xx_print_error_msg(pHandle, "failed to get alarm time and date");
        return err;
    }

    if(alarm == MCP794XX_ALARM0)
    {
        err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_ALM0_WKDAY_REG, (uint8_t *)&read_status, 1);
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "failed read alarm mask");
            return err;
        }
    }
    else if(alarm == MCP794XX_ALARM1)
    {
        err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_ALM1_WKDAY_REG, (uint8_t *)&read_status, 1);
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "failed read alarm mask");
            return err;
        }

    }

    else{
        a_mcp794xx_print_error_msg(pHandle, "invalid alarm selected");
        return 4;
    }

    *pMask = (mcp794xx_alarm_mask_t)((read_status & MCP794XX_ALARMx_TYPE_MASK) >> 4);

    return 0;
}

/**
 * @brief This function get the alarm interrupt flag
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] alarm is the alarm type (ALARM0/ALARM1)
 * @param[out] pFlag point to the interrupt flag read
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 *          - 4 invalid alarm
 */
uint8_t mcp794xx_get_alarm_interrupt_flag(mcp794xx_handle_t *const pHandle, mcp794xx_alarm_t alarm, mcp794xx_alarm_int_flag_t *pFlag)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(alarm == MCP794XX_ALARM0)
    {
         err = rtc_mcp794xx_i2c_read(pHandle,MCP794XX_ALM0_WKDAY_REG, (uint8_t *)&read_status, 1);
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "failed to read alarm 0 interrupt flag");
            return 1;           /**< failed error */
        }
       *pFlag = (mcp794xx_alarm_int_flag_t)((read_status & MCP794XX_ALARMx_IF_MASK ) >> 3);

    }

    else if(alarm == MCP794XX_ALARM1)
    {
        err = rtc_mcp794xx_i2c_read(pHandle,MCP794XX_ALM1_WKDAY_REG, (uint8_t *)&read_status, 1);
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "failed to read alarm 1 interrupt flag");
            return 1;           /**< failed error */
        }
       *pFlag = (mcp794xx_alarm_int_flag_t)((read_status & MCP794XX_ALARMx_IF_MASK ) >> 3);

    }

    else{

        a_mcp794xx_print_error_msg(pHandle, "invalid alarm selected");
        return 4;
    }

    return 0;              /**< success */
}

/**
 * @brief This function clears the alarm interrupt flag
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] alarm is the alarm type (ALARM0/ALARM1)
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 *          - 4 invalid alarm
 */
uint8_t mcp794xx_clr_alarm_interrupt_flag(mcp794xx_handle_t *const pHandle, mcp794xx_alarm_t alarm)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(alarm == MCP794XX_ALARM0)
    {
        err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_ALM0_WKDAY_REG, (uint8_t *)&read_status, 1);
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "read alarm 0 interrupt flag register");
            return 1;           /**< failed error */
        }

        read_status &= ~(1 << 3);
        err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_ALM0_WKDAY_REG, (uint8_t *)&read_status);
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "clear alarm 0 interrupt flag bit");
            return 1;           /**< failed error */
        }

    }
    else if(alarm == MCP794XX_ALARM1)
    {

        err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_ALM1_WKDAY_REG, (uint8_t *)&read_status, 1);
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "read alarm 1 interrupt flag register");
            return 1;           /**< failed error */
        }

        read_status &= ~(1 << 3);
        err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_ALM1_WKDAY_REG, (uint8_t *)&read_status);
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "clear alarm 1 interrupt flag bit");
            return 1;           /**< failed error */
        }
    }

    else
    {

        a_mcp794xx_print_error_msg(pHandle, "invalid alarm selected");
        return 4;
    }

    return 0;              /**< success */
}

/**
 * @brief This function set the general purpose output pin level
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] logicLevel is the logic level to be set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_set_mfp_logic_level(mcp794xx_handle_t *const pHandle, mcp794xx_mfp_logic_level_t logicLevel)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_CONTROL_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read ctrl register");
        return 1;           /**< failed error */
    }

    read_status &= ~(1 << 7);
    read_status |= (logicLevel << 7);

    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_CONTROL_REG, (uint8_t *)&read_status);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "write ctrl register");
        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief This function get the general purpose output pin level
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[out] pLogicLevel point to the logic level set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_get_mfp_logic_level(mcp794xx_handle_t *const pHandle, mcp794xx_mfp_logic_level_t *pLogicLevel)
{

    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_CONTROL_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read ctrl register");
        return 1;           /**< failed error */
    }

    *pLogicLevel = (mcp794xx_mfp_logic_level_t) ((read_status & MCP794XX_CTRL_OUT_MASK) >> 7);

    return 0;              /**< success */
}

/**
 * @brief This function set the square wave frequency clock output on MFP
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] freq is the frequency clock output to set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 * @note prior to setting frequency output, the "mcp794xx_set_sqr_wave_enable_status" must called first to enable this feature.
 */
uint8_t mcp94xx_set_sqr_wave_output_freq(mcp794xx_handle_t *const pHandle, mcp94xx_sqr_wave_freq_t freq)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_CONTROL_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read ctrl register");
        return 1;           /**< failed error */
    }

    read_status &= ~ (0b11 << 0);
    read_status |= (freq << 0);

    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_CONTROL_REG, (uint8_t *)&read_status);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "write ctrl register");
        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief This function get the square wave frequency clock output on MFP
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[out] pFreq point to the frequency clock output set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp94xx_get_sqr_wave_freq(mcp794xx_handle_t *const pHandle, mcp94xx_sqr_wave_freq_t *pFreq)
{

    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_CONTROL_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read ctrl register");
        return 1;           /**< failed error */
    }

    *pFreq = (mcp94xx_sqr_wave_freq_t)(read_status & MCP794XX_CTRL_SQR_WAVE_FREQ_SELEC_MASK);

    return 0;              /**< success */
}

/**
 * @brief This function enables/disables the square wave clock freq. output
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] enable is the enable boolean status to set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_set_sqr_wave_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t enable)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_CONTROL_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read ctrl register");
        return 1;           /**< failed error */
    }

    read_status &= ~ (1 << 6);
    read_status |= (enable << 6);

    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_CONTROL_REG, (uint8_t *)&read_status);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "write ctrl register");
        return 1;           /**< failed error */
    }

    return 0;           /**< success */
}

/**
 * @brief This function get enables/disables the square wave clock freq. status bit
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[out] pStatus point to the enable boolean status set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_get_sqr_wave_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t *pStatus)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_CONTROL_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read ctrl register");
        return 1;           /**< failed error */
    }

    *pStatus = (mcp794xx_bool_t)((read_status & MCP794XX_CTRL_SQR_WAVE_EN_MASK) >> 6);

    return 0;              /**< success */
}

/**
 * @brief This function set the Oscillator Trim Value
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] sign is the arithmetic operation (+/-)
 * @param[in] value is the clock cycle value to the added or subtracted
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_set_trim_val(mcp794xx_handle_t *const pHandle, mcp794xx_trim_sign_t sign, mcp794xx_trim_val_t value)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_OSC_TRIM_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read oscillator digital trim register");
        return 1;           /**< failed error */
    }

    read_status &=  0x00;            /**< clear trim clock value */
    read_status |= (value << 0);     /**< set trim value */
    read_status |= (sign << 7);      /**< set trim sign */

    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_OSC_TRIM_REG, (uint8_t *)&read_status);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "write oscillator digital trim register");
        return 1;           /**< failed error */
    }

    return 0;           /**< success */
}

/**
 * @brief This function get the Oscillator Trim Value
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[out] pSign point to the arithmetic operation set (+/-)
 * @param[out] pValue point to the clock cycle value to the added or subtracted
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_get_trim_val(mcp794xx_handle_t *const pHandle, mcp794xx_trim_sign_t *pSign, mcp794xx_trim_val_t *pValue)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_OSC_TRIM_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read oscillator digital trim register");
        return 1;           /**< failed error */
    }

    *pSign = (mcp794xx_trim_sign_t)((read_status & 0x80) >> 7);             /**< read trim value sign */
    *pValue = (mcp794xx_trim_val_t)((read_status & 0x7f) >> 0);             /**< read trim value */

    return 0;              /**< success */
}

/**
 * @brief This function set the coarse trim mode enable status
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] status is the enable boolean status to set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 * @note Coarse Trim mode results in the MCP7941X applying digital trimming every 64 Hz clock cycle.
 */
uint8_t mcp794xx_set_coarse_trim_mode_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t status)
{

    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_CONTROL_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read ctrl register");
        return 1;           /**< failed error */
    }

    read_status &= ~ (1 << 2);
    read_status |= (status << 2);

    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_CONTROL_REG, (uint8_t *)&read_status);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "write ctrl register");
        return 1;           /**< failed error */
    }

    return 0;           /**< success */
}

/**
 * @brief This function get the coarse trim mode enable status
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[out] pStatus point to the enable boolean status
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_get_coarse_trim_mode_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t *pStatus)
{

    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_CONTROL_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read ctrl register");
        return 1;           /**< failed error */
    }

    *pStatus = (mcp794xx_bool_t)((read_status & MCP794XX_CTRL_CRS_TRIM_EN_MASK) >> 2);

    return 0;              /**< success */
}

/**
 * @brief This function get the oscillator running status
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[out] pStatus point to the enable boolean status
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_get_os_running_status(mcp794xx_handle_t *const pHandle, mcp794xx_osc_status_t *pStatus)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_RTC_WKDAY_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read oscillator running status");
        return 1;           /**< failed error */
    }

    *pStatus = (mcp794xx_osc_status_t)((read_status & MCP794XX_OSC_RUN_STATUS_MASK) >> 5);

    return 0;              /**< success */
}

/**
 * @brief This function set the external battery enable status bit
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] status is the enable boolean status to set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 *          - 4 variant not supported
 */
uint8_t  mcp794xx_set_ext_batt_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t status)
{

    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(pHandle->device_variant == MCP7940M_VARIANT){
         a_mcp794xx_print_error_msg(pHandle, "set external battery bckup, not supported for the mcp7940M variant");
        return 4;
    }

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_RTC_WKDAY_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "set external battery bckup");
        return 1;           /**< failed error */
    }

    read_status &= ~ (1 << 3);
    read_status |= (status << 3);

    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_WKDAY_REG, (uint8_t *)&read_status);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "set external battery bckup status");
        return 1;           /**< failed error */
    }

    return 0;           /**< success */
}

/**
 * @brief This function get the external battery enable bit
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[out] pStatus point to the enable boolean status
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t  mcp794xx_get_ext_batt_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t *pStatus)
{

    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_RTC_WKDAY_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read external battery bckup status");
        return 1;           /**< failed error */
    }

    *pStatus = (mcp794xx_bool_t)((read_status & MCP794XX_VBAT_EN_MASK) >> 3);

    return 0;              /**< success */
}

/**
 * @brief This function enables/disables the 32.78Khz external oscillator pins
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] status is the enable boolean status to set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_set_ext_osc_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_osc_status_t status)
{

    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_CONTROL_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "set external oscillator");
        return 1;           /**< failed error */
    }

    read_status &= ~ (1 << 3);
    read_status |= (status << 3);

    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_CONTROL_REG, (uint8_t *)&read_status);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "set external oscillator");
        return 1;           /**< failed error */
    }

    return 0;           /**< success */
}

/**
 * @brief This function get the status enables/disables of the 32.78Khz external oscillator pins
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[out] pStatus point to the enable boolean status to set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_get_ext_osc_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_osc_status_t *pStatus)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_CONTROL_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read oscillator status bit");
        return 1;           /**< failed error */
    }

    *pStatus = (mcp794xx_osc_status_t)((read_status & MCP794XX_VBAT_EN_MASK) >> 3);

    return 0;              /**< success */
}

/**
 * @brief     This function enables/disables the oscillator internally
 * @param[in] *pHandle points to mcp794xx pHandle structure
 * @param[in] status is the boolean status
 * @return  status code
 *            - 0 success
 *            - 1 failed
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      none
 */
uint8_t mcp794xx_set_osc_start_bit(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t status)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_RTC_SECOND_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "set osc start bit");
        return 1;           /**< failed error */
    }

    read_status &= ~ (1 << 7);
    read_status |= (status << 7);

    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_SECOND_REG, (uint8_t *)&read_status);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "set osc start bit");
        return 1;           /**< failed error */
    }
     return 0;  /**< success */
}

/**
 * @brief     This function get the enables/disable oscillator status
 * @param[in] *pHandle points to mcp794xx pHandle structure
 * @param[out] pStatus point the boolean status set
 * @return  status code
 *            - 0 success
 *            - 1 failed
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      none
 */
uint8_t mcp794xx_get_osc_start_bit(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t *pStatus)
{

    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_RTC_SECOND_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read oscillator start bit");
        return 1;           /**< failed error */
    }

    *pStatus = (mcp794xx_osc_status_t)((read_status & MCP794XX_ST_MASK) >> 7);

    return 0;              /**< success */
}

/**
 * @brief     This function read the current time in epoch/unix format
 * @param[in] *pHandle points to mcp794xx pHandle structure
 * @param[in] pTime point to the current time in human readable format
 * @param[out] pEpoch_time point to the epoch time converted
 * @return  status code
 *            - 0 success
 *            - 1 failed
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      This function will fail to execute from January 19, 2038
 */
uint8_t mcp794xx_read_epoch_unix_time_stamp(mcp794xx_handle_t *const pHandle, mcp794xx_time_t *pTime, uint32_t *pEpoch_time)
{
	 uint8_t num_years,num_months,num_days = 0;
     uint32_t total_days_from_years = 0;
     uint32_t total_days_from_months = 0;
     uint32_t total_num_days = 0;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    num_years = pTime->year - 1970;   /**< number years since 1970*/
    num_months = pTime->month - 1;    /**< number of months from January */
    num_days = pTime->date - 1;		  /**< number of days since the 1st of the current month*/

    for(int index = 0; index < num_years; index++)
    {
    	if(index % 4)
    	{
    		total_days_from_years += 365;
    	}else
    	{
    		total_days_from_years += 366;
    	}
    }

    for(int index = 0; index < num_months; index++)
    {
       total_days_from_months += days_in_month[index];
    }

    total_num_days = (total_days_from_years + total_days_from_months + num_days) - 1;                     /**< calculate total number of days since 1 January 1970 */

    *pEpoch_time = total_num_days * 86400 + pTime->hour * 3600 + pTime->minute * 60 + pTime->second;      /**< calculate number of seconds from current time and date */

	return 0;   /**< success */
}


/**
 * @brief  This function convert epoch time to human readable time
 * @param[in]  pHandle points to a mcp794xx handle structure
 * @param[in] epoch_unix_time point to the epoch time converted
 * @param[out] pTime point to time output in human readable time
 * @return  status code
 *            - 0 success
 *            - 1 failed to get
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      This function will fail to execute from January 19, 2038
 */
uint8_t mcp794xx_convert_epoch_unix_time_stamp(mcp794xx_handle_t *const pHandle, uint32_t u32Epoch_time, mcp794xx_time_t *pTime_out)
{
    uint32_t sec_of_day;
    uint16_t num_days;

	if(pHandle == NULL)
	return 2;     /**< return failed error */
	if(pHandle->inited != 1)
	return 3;      /**< return failed error */

	/**< get hour, minute and seconds */
    sec_of_day = u32Epoch_time % (24 * 60 * 60);

    pTime_out->hour       = sec_of_day / (60 * 60);
    pTime_out->minute     = sec_of_day % (60 * 60) / 60;
    pTime_out->second     = u32Epoch_time % 60;

    /**< get years */
    num_days = (u32Epoch_time - SECONDS_FROM_Y1970_TO_Y2000) / 86400;

    num_days += 1401;           			/**< move starting point to 1996-03-01 */
    pTime_out->year = (num_days/1461)*4;    /**< take care of 4-year intervals */
    num_days%=1461; 						/**< max 3 years 365 days (not 4 years!) remaining */

    if (num_days>=730) {num_days-=730; pTime_out->year+=2;} /**< 2 years */
    if (num_days>=365) {num_days-=365; pTime_out->year++;}  /**< 1 year */

    /**< get months */
    pTime_out->month = 3; 									/**< get started on the month */

    if (num_days>=306) {num_days-=306; pTime_out->year++; pTime_out->month=1;}  /**< for Jan. and Feb. */
    else if (num_days>=153) {num_days-=153; pTime_out->month=8;} 				/**< for Aug. thru Dec. */
    if (num_days>=122) {num_days-=122; pTime_out->month+=4;} 					/**< here, 122 days mean 4 months */
    else if (num_days>=61) {num_days-=61; pTime_out->month+=2;} 				/**< 61 days mean 2 months */
    if (num_days>=31) {num_days-=31; pTime_out->month++;} 						/**< 31 days means exactly 1 month */

    pTime_out->date = num_days + 1; 											/**< get the day of the month */

    pTime_out->year+=(2000 - 4); 												/**< make y be 0 for AD 2000 */

    /**< get weekdays */
    pTime_out->weekDay = ((pTime_out->date + ((153 * (pTime_out->month + 12 * ((14 - pTime_out->month) / 12) - 3) + 2) / 5)
               + (365 * (pTime_out->year + 4800 - ((14 - pTime_out->month) / 12)))
               + ((pTime_out->year + 4800 - ((14 - pTime_out->month) / 12)) / 4)
               - ((pTime_out->year + 4800 - ((14 - pTime_out->month) / 12)) / 100)
               + ((pTime_out->year + 4800 - ((14 - pTime_out->month) / 12)) / 400)
               - 32045)
        % 7) + 2;

//	struct tm *time = localtime((const time_t *)&u32Epoch_time);          /**< built-in function from time.h library */
//
//	{ /** copy time structure members */
//		pTime_out->year    = time->tm_year + 1900;
//		pTime_out->month   = time->tm_mon + 1;
//		pTime_out->date    = time->tm_mday;
//		pTime_out->weekDay = time->tm_wday + 1;
//		pTime_out->hour    = time->tm_hour;
//		pTime_out->minute  = time->tm_min;
//		pTime_out->second  = time->tm_sec;
//	}
//
//	if (time == NULL)
//	{
//		a_mcp794xx_print_error_msg(pHandle, "convert epoch time to human readable time");
//		return 1;						/**< failed */
//	}

    return 0;							/**< success */
}

/**
 * @brief      This function reads the compile time and date of the application
 * @param[in]  pHandle points to a mcp794xx handle structure
 * @param[in]  Compile_time_date compile time and date string variable (__TIMESTAMP__)
 * @param[out] pTime points to a time structure object
 * @return     status code
 *             - 0 success
 *             - 1 failed
 *             - 2 handle or time is NULL
 *             - 3 handle is not initialized
 * @note       this function returns time in 24Hr format
 */
uint8_t mcp794xx_get_compile_time_date(mcp794xx_handle_t *const pHandle, char *pCompile_time_date, mcp794xx_time_t *pTime)
{
    char year_string[3], month_string[3], date_string[3], week_day_string[1], hour_string[3], minute_string[3], second_string[3];
    uint32_t epoch_time;
    uint8_t sec_offset = 15;      /**< this is an offset time to compensate lost seconds during compiling and programming the device,
    									purely depends on how fast your computer does the job */

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    year_string[0] = BUILD_YEAR_CH2(pCompile_time_date);
    year_string[1] = BUILD_YEAR_CH3(pCompile_time_date);

    month_string[0] = BUILD_MONTH_CH0(pCompile_time_date);
    month_string[1] = BUILD_MONTH_CH1(pCompile_time_date);

    date_string[0] = BUILD_DATE_CH0(pCompile_time_date);
    date_string[1] = BUILD_DATE_CH1(pCompile_time_date);

    hour_string[0] = BUILD_HOUR_CH0(pCompile_time_date);
    hour_string[1] = BUILD_HOUR_CH1(pCompile_time_date);

    minute_string[0] = BUILD_MIN_CH0(pCompile_time_date);
    minute_string[1] = BUILD_MIN_CH1(pCompile_time_date);

    second_string[0] = BUILD_SEC_CH0(pCompile_time_date);
    second_string[1] = BUILD_SEC_CH1(pCompile_time_date);

    week_day_string[0] = BUILD_WEEK_DAY_CH(pCompile_time_date);

    pTime->year = atoi((const char *)year_string);
    pTime->month = atoi((const char *) month_string);
    pTime->date = atoi((const char *) date_string);
    pTime->weekDay = atoi((const char *) week_day_string);
    pTime->hour = atoi((const char *) hour_string);
    pTime->minute = atoi((const char *) minute_string);
    pTime->second = atoi((const char *) second_string);

	if(pTime->second < 45){
		pTime->second += sec_offset;
		}else{
		pTime->second = (pTime->second + sec_offset) - 60;
		if(pTime->minute < 58){
			pTime->minute += 1;
			}else{
			pTime->hour += 1;
			pTime->minute = 0;
		}
	}

    return 0;
}

/**
 * @brief     This function enables/disables countdown interrupt
 * @param[in] *pHandle points to mcp794xx pHandle structure
 * @param[in] status is the enable boolean status to be set
 * @return  status code
 *            - 0 success
 *            - 1 failed
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      This function use ALARM1 and disables ALARM 0
 */
uint8_t mcp794xx_set_countdown_interrupt_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t status)
{
    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

	err = mcp794xx_set_alarm_enable_status(pHandle, MCP794XX_ALARM0, MCP794XX_BOOL_FALSE);
	if(err != MCP794XX_DRV_OK)
	{
		a_mcp794xx_print_error_msg(pHandle, "set countdown interrupt, ALARM 0 not disabled");
		return 1;
	}

    err = mcp794xx_set_alarm_enable_status(pHandle, MCP794XX_ALARM1, status);
    if(err != MCP794XX_DRV_OK)
    {
        a_mcp794xx_print_error_msg(pHandle, "set countdown interrupt");
        return 1;
    }

    return 0;
}

/**
 * @brief     This function set countdown time interrupt to fire every xx time set
 * @param[in] *pHandle points to mcp794xx pHandle structure
 * @param[in] pTime point to current time and date structure object
 * @param[in] u16Time_value is the time value to start counting down from
 * @param[in] time_unit is the time unit to countdown at (seconds/minutes/hours)
 * @return  status code
 *            - 0 success
 *            - 1 failed
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 *            - 4 invalid time unit
 *            - 5 invalid time value
 *
 * @note      - This function use ALARM1 and overwrite existing data within register
 *            - time must be reloaded after interrupt has occurred
 *			  - ALARM0 must not not used if countdown time is enabled
 *            - "mcp794xx_set_countdown_interrupt_enable_status"  must be called before using this function
 *            - Time and date must be set prior to calling this function
 *            - day of the week must be accurate when setting time
 */
uint8_t mcp794xx_set_countdown_interrupt_time(mcp794xx_handle_t *const pHandle, uint16_t u16Time_value, mcp794xx_countdwon_time_unit_t time_unit)
{
	mcp794xx_time_t pTime;

    uint32_t countdown_time_in_seconds;
    uint32_t epoch_time;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if((time_unit != MCP794XX_CNTDWN_SECONDS) && (time_unit != MCP794XX_CNTDWN_MINUTES) && (time_unit != MCP794XX_CNTDWN_HOURS))
    {
        a_mcp794xx_print_error_msg(pHandle, "countdown interrupt, invalid time unit");
        return 4;
    }

	mcp794xx_clr_alarm_interrupt_flag(pHandle, MCP794XX_ALARM1);						/**< clear alarm flag */

	err = mcp794xx_get_time_and_date(pHandle, &pTime);										/**< get current time and date */
	if(err)
	{
		a_mcp794xx_print_error_msg(pHandle, "read current time and date");
		return err;
	}

    if( u16Time_value < 0 || u16Time_value > 0xffff)
    {
        a_mcp794xx_print_error_msg(pHandle, "countdown interrupt, value can not be less than 0 or greater than 65535");
       return 5;
    }

    err = mcp794xx_read_epoch_unix_time_stamp(pHandle, &pTime, (uint32_t *)&epoch_time);
    if(err)
    {
        return err;
    }

    switch(time_unit)
    {

        case MCP794XX_CNTDWN_SECONDS:
            {
                countdown_time_in_seconds = u16Time_value;
                epoch_time += countdown_time_in_seconds;
                break;
            }

        case MCP794XX_CNTDWN_MINUTES:
            {
                countdown_time_in_seconds = u16Time_value * 60;      /**< convert to second */
                 epoch_time += countdown_time_in_seconds;
                break;
            }

        case MCP794XX_CNTDWN_HOURS:
            {
                countdown_time_in_seconds = u16Time_value * 3600;    /**< convert to seconds */
                 epoch_time += countdown_time_in_seconds;
                break;
            }
    }

    err = mcp794xx_convert_epoch_unix_time_stamp(pHandle, epoch_time, &pTime);
    if(err != MCP794XX_DRV_OK)
    {
        return err;
    }

    err = mcp794xx_set_alarm(pHandle, MCP794XX_ALARM1, MCP794XX_MASK_S_M_H_WD_D_M, &pTime);
    if(err != MCP794XX_DRV_OK)
    {
        return err;
    }

    return 0;
}

/**
 * @brief     This function get the device unique identifier
 * @param[in] *pHandle points to mcp794xx pHandle structure
 * @param[out] pUID point to the device Unique ID
 * @return  status code
 *            - 0 success
 *            - 1 failed to get S/N
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      none
 */
uint8_t mcp794xx_uid_read(mcp794xx_handle_t *const pHandle, uint8_t *pUID)
{
    uint8_t temp_buffer[UID_MAX_LENGTH];

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    memset(temp_buffer, 0x00, sizeof(temp_buffer));                 /**< clear buffer before read */

    switch(pHandle->device_variant)
    {
        case MCP79400_VARIANT:
        {
            a_mcp794xx_print_error_msg(pHandle, "failed to read UID, unsupported cmd for mcp79400 variant");
            return 1;           /**< failed error */
            break;
        }

        case MCP79401_VARIANT:
        {
            err = eeprom_mcp794xx_i2c_read(pHandle, EEPROM_UID_ADDRESS + 2, (uint8_t *)temp_buffer, UID_MAX_LENGTH - 2);
            if(err)
            {
                a_mcp794xx_print_error_msg(pHandle, "failed to read UID");
                return 1;           /**< failed error */
            }
            memcpy((uint8_t *)pUID, (uint8_t *)temp_buffer, sizeof(temp_buffer) - 2);
            break;
        }

        case MCP79402_VARIANT:
        {
            err = eeprom_mcp794xx_i2c_read(pHandle, EEPROM_UID_ADDRESS, (uint8_t *)temp_buffer, UID_MAX_LENGTH);
            if(err)
            {
                a_mcp794xx_print_error_msg(pHandle, "failed to read UID");
                return 1;           /**< failed error */
            }
            memcpy((uint8_t *)pUID, (uint8_t *)temp_buffer, sizeof(temp_buffer));
            break;
        }

        case MCP7940M_VARIANT:
        {
            a_mcp794xx_print_error_msg(pHandle, "failed to read UID, unsupported cmd for mcp7940m variant");
            return 1;           /**< failed error */
            break;
        }

        case MCP7940N_VARIANT:
        {
            a_mcp794xx_print_error_msg(pHandle, "failed to read UID, unsupported cmd for mcp7940n variant");
            return 1;           /**< failed error */
            break;
        }

        case MCP79410_VARIANT:
        {
            a_mcp794xx_print_error_msg(pHandle, "failed to read UID, unsupported cmd for mcp79410 variant");
            return 1;           /**< failed error */
            break;
        }

        case MCP79411_VARIANT:
        {
            err = eeprom_mcp794xx_i2c_read(pHandle, EEPROM_UID_ADDRESS + 2, (uint8_t *)temp_buffer, UID_MAX_LENGTH - 2);
            if(err)
            {
                a_mcp794xx_print_error_msg(pHandle, "failed to read UID");
                return 1;           /**< failed error */
            }
            memcpy((uint8_t *)pUID, (uint8_t *)temp_buffer, sizeof(temp_buffer) - 2);
            break;
        }

        case MCP79412_VARIANT:
        {
            err = eeprom_mcp794xx_i2c_read(pHandle, EEPROM_UID_ADDRESS, (uint8_t *)temp_buffer, UID_MAX_LENGTH);
            if(err)
            {
                a_mcp794xx_print_error_msg(pHandle, "failed to read UID");
                return 1;           /**< failed error */
            }
            memcpy((uint8_t *)pUID, (uint8_t *)temp_buffer, sizeof(temp_buffer));
            break;
        }

    }

    return 0;              /**< success */
}

/**
* @brief This function write data bytes to sram
* @param[in] *pHandle points to mcp794xx handle structure
* @param[in] addr is the register address to write to
* @param[in] *pBuffer point to data to write
* @param[in] length is the number of bytes the write
* @return status code
*          - 0 success
*          - 1 failed to write
*          - 2 pHandle is NULL
*          - 3 pHandle is not initialized
*          - 4 invalid address
* @note none
*/
uint8_t mcp794xx_sram_write_byte(mcp794xx_handle_t *const pHandle, uint8_t addr, uint8_t *pBuffer, size_t length)
{

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(addr > SRAM_LAST_ADDRESS || addr < SRAM_FIRST_ADDRESS)
    {
    	a_mcp794xx_print_error_msg(pHandle, "write sram, invalid address");
    	return 4;
    }if(length > SRAM_MEMORY_SIZE){
    	a_mcp794xx_print_error_msg(pHandle, "write sram, address can't be greater than 0x5F");
    	return 5;
    }if((addr - SRAM_FIRST_ADDRESS)+ length > SRAM_MEMORY_SIZE){
    	a_mcp794xx_print_error_msg(pHandle, "write sram, address out of range");
    	return 5;
    }

    err = sram_mcp794xx_i2c_write(pHandle, addr, (uint8_t *)pBuffer, length);
    if(err)
    {
       a_mcp794xx_print_error_msg(pHandle, "write to sram");
       return 1;
    }
	return 0;          /**< success */
}

/**
* @brief This function reads data bytes from sram
* @param[in] *pHandle points to mcp794xx handle structure
* @param[in] addr is the register address to write to
* @param[out] *pBuffer point to data to read
* @param[in] length is the number of bytes the read
* @return status code
*          - 0 success
*          - 1 failed to read
*          - 2 pHandle is NULL
*          - 3 pHandle is not initialized
*          - 4 invalid address
* @note none
*/
uint8_t mcp794xx_sram_read_byte(mcp794xx_handle_t *const pHandle, uint8_t addr, uint8_t *pBuffer, size_t length)
{

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(addr > SRAM_LAST_ADDRESS || addr < SRAM_FIRST_ADDRESS)
    {
    	a_mcp794xx_print_error_msg(pHandle, "read sram, invalid address");
    	return 4;
    }if(length > SRAM_MEMORY_SIZE){
    	a_mcp794xx_print_error_msg(pHandle, "read sram, address can't be greater than 0x5F");
    	return 5;
    }if((addr - SRAM_FIRST_ADDRESS)+ length > SRAM_MEMORY_SIZE){
    	a_mcp794xx_print_error_msg(pHandle, "read sram, address out of range");
    	return 5;
    }

    err = rtc_mcp794xx_i2c_read(pHandle, addr, (uint8_t *)pBuffer, length);
    if(err)
    {
       a_mcp794xx_print_error_msg(pHandle, "read sram");
       return 1;
    }

 return 0;          /**< success */
}

/**
* @brief This function erases a sector of the sram
* @param[in] *pHandle points to mcp794xx handle structure
* @param[in] start_addr is the first register address erase
* @param[in] end_addr is the last register address erase
* @return status code
*          - 0 success
*          - 1 failed to write
*          - 2 pHandle is NULL
*          - 3 pHandle is not initialized
*          - 4 invalid address
* @note data from the start to the end address will be erased
*/
uint8_t mcp794xx_sram_erase_selector(mcp794xx_handle_t *const pHandle, uint8_t start_addr, uint8_t end_addr)
{
	int index;
	uint8_t dummy_dada = 0x00;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(start_addr > SRAM_LAST_ADDRESS || start_addr < SRAM_FIRST_ADDRESS)
    {
    	a_mcp794xx_print_error_msg(pHandle, "erase sram sector, invalid address (0x20 - 0x57)");
    	return 4;
    }if(end_addr > SRAM_LAST_ADDRESS || end_addr < SRAM_FIRST_ADDRESS){
    	a_mcp794xx_print_error_msg(pHandle, "erase sram sector, invalid address (0x20 - 0x57)");
    	return 4;
    }

    for(index = start_addr; index <= end_addr; index++)
    {
		err = sram_mcp794xx_i2c_write(pHandle, index, (uint8_t *)&dummy_dada, 1);
		if(err)
		{
		   a_mcp794xx_print_error_msg(pHandle, "erase sram sector");
		   return 1;
		}
    }

	return 0;   /**< success */
}

/**
* @brief This function wipes the entire sram memory
* @param[in] *pHandle points to mcp794xx handle structure
* @return status code
*          - 0 success
*          - 1 failed to write
*          - 2 pHandle is NULL
*          - 3 pHandle is not initialized
* @note none
*/
uint8_t mcp794xx_wipe_sram(mcp794xx_handle_t *const pHandle)
{
    uint8_t temp_buffer[SRAM_MEMORY_SIZE];

    memset(temp_buffer, 0x00, sizeof(temp_buffer));

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = sram_mcp794xx_i2c_write(pHandle, SRAM_FIRST_ADDRESS, (uint8_t *)temp_buffer, SRAM_MEMORY_SIZE);
	if(err)
	{
	   a_mcp794xx_print_error_msg(pHandle, "wipe sram");
	   return 1;
	}

	return 0;    /**< success */
}

/**
 * @brief     This function write n byte to eeprom
 * @param[in] pHandle points to mcp794xx handle structure
 * @param[in] addr is the eeprom address to write to
 * @param[in] *buf points to data buffer to be written
 * @param[in] length is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid address
 *            - 5 attempt to write a protected block array
 *            - 6 data length is larger than page size
 * @note      the eeprom has a 5ms write cycle time.
 */
uint8_t mcp794xx_eeprom_write_byte(mcp794xx_handle_t *const pHandle, uint8_t u8Addr, uint8_t *pBuf, uint8_t length)
{
    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(u8Addr < EEPROM_LOWEST_ADDRESS || u8Addr > EEPROM_HIGHEST_ADDRESS){
    	a_mcp794xx_print_error_msg(pHandle, "to write eeprom, invalid address");
    	return 4;
    }if((u8Addr + length) > EEPROM_MEMORY_SIZE)
    {
    	a_mcp794xx_print_error_msg(pHandle, "to write eeprom, address out of range");
    	return 4;
    }

    err = mcp794xx_eeprom_check_bp_before_write(pHandle, u8Addr);   /**< check to make sure that the address to write is not protected */
    if(err != MCP794XX_DRV_OK)
    {
    	a_mcp794xx_print_error_msg(pHandle, "write eeprom, attempt to write a block protected region");
    	return 5;
    }

    err = eeprom_mcp794xx_i2c_write(pHandle, u8Addr, (uint8_t *)pBuf, length);
    if(err)
    {
    	a_mcp794xx_print_error_msg(pHandle, "write eeprom");
        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief     This function read n byte to eeprom
 * @param[in] pHandle points to mcp794xx handle structure
 * @param[in] addr is the eeprom address to read
 * @param[out] *buf points to data buffer read
 * @param[in] length is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 read failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid address
 *            - 5 invalid device variant
 * @note      none
 */
uint8_t mcp794xx_eeprom_read_byte(mcp794xx_handle_t *const pHandle, uint8_t u8Addr, uint8_t *pBuf, uint8_t length)
{

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(u8Addr < EEPROM_LOWEST_ADDRESS || u8Addr > EEPROM_HIGHEST_ADDRESS){
    	a_mcp794xx_print_error_msg(pHandle, "to read eeprom, invalid address");
    	return 4;
    }if((u8Addr + length) > EEPROM_MEMORY_SIZE)
    {
    	a_mcp794xx_print_error_msg(pHandle, "to read eeprom, address out of range");
    	return 4;
    }

    if(pHandle->device_variant == MCP79400_VARIANT || pHandle->device_variant == MCP7940M_VARIANT || pHandle->device_variant == MCP7940N_VARIANT || pHandle->device_variant == MCP79410_VARIANT){
 	   a_mcp794xx_print_error_msg(pHandle, "invalid device variant, the device does have a eeprom built-in");
 	   return 5;
    }

    err = eeprom_mcp794xx_i2c_read(pHandle, u8Addr, (uint8_t *)pBuf, length);
    if(err)
    {
    	a_mcp794xx_print_error_msg(pHandle, "read eeprom");
        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief     This function writes data larger than a byte > 255 (equivalent to eeprom.put on Arduino)
 * @param[in] pHandle points to mcp794xx handle structure
 * @param[in] addr is the eeprom address to start writing
 * @param[in] *buf points to data buffer write
 * @param[in] u8Length is the buffer size
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid address (failed to validate address)
 *            - 5 attempt to write a protected block array
 *            - 6 data length is larger than page can contain
 * @note      none
 */

uint8_t mcp794xx_eeprom_put_byte(mcp794xx_handle_t *const pHandle, uint8_t u8Addr, uint32_t *pBuf, uint8_t u8Length)
{
	uint8_t *pTemp_buffer; /**< Temporarily buffer */
    uint32_t dataOut;
    uint8_t index;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    pTemp_buffer = (uint8_t*)calloc(u8Length, sizeof(uint8_t));
	if(pTemp_buffer == NULL)
		pHandle->debug_print("failed to allocate dynamic memory\n\r");

    memcpy(pTemp_buffer, pBuf, u8Length);

    err = mcp794xx_eeprom_validate_page_boundary(pHandle, u8Addr, u8Length);
    if(err != MCP794XX_DRV_OK){
    	return err;
    }

    /**< Reverse array data to big Indian*/
	for (index = 0; index < (uint8_t)u8Length / 2; index++)
	{
		dataOut = pTemp_buffer[index];
		pTemp_buffer[index] = pTemp_buffer[u8Length - index - 1];
		pTemp_buffer[u8Length - index - 1] = dataOut;
	}

	err = mcp794xx_eeprom_write_byte(pHandle, u8Addr, (uint8_t *)pTemp_buffer, u8Length);
	if (err)
	{
		a_mcp794xx_print_error_msg(pHandle, "put bytes");
		return 1; /**< failed to write */
	}
	free(pTemp_buffer);

    return 0;              /**< success */
}

/**
 * @brief     This function reads data larger than a byte > 255 (equivalent to eeprom.get on Arduino)
 * @param[in] pHandle points to mcp794xx handle structure
 * @param[in] addr is the eeprom address to start writing
 * @param[out] *buf points to data buffer read
 * @param[in] u8Length is the buffer size
 * @return    status code
 *            - 0 success
 *            - 1 read failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid address (failed to validate address)
 * @note      none
 */

uint8_t mcp794xx_eeprom_get_byte(mcp794xx_handle_t *const pHandle, uint8_t u8Addr, uint32_t *pBuf, uint8_t u8Length)
{
    uint8_t index, dataOut;
    uint8_t *temp_buffer;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

	temp_buffer = (uint8_t*)calloc(u8Length, sizeof(uint8_t));

    err = mcp794xx_eeprom_validate_page_boundary(pHandle, u8Addr, u8Length);
    if(err != MCP794XX_DRV_OK){
    	return err;
    }

    err = (mcp794xx_eeprom_read_byte(pHandle, u8Addr, (uint8_t*) temp_buffer, u8Length));
    if (err != MCP794XX_DRV_OK)
    {
    	a_mcp794xx_print_error_msg(pHandle, "get bytes");
        return 1; /**< return error code */
    }

    /**< Reverse array data to Big Indian*/
    for (index = 0; index < (uint8_t)u8Length / 2; index++)
    {
        dataOut = temp_buffer[index];
        temp_buffer[index] = temp_buffer[u8Length - index - 1];
        temp_buffer[u8Length - index - 1] = dataOut;
    }

    memcpy(eeprom.buffer, temp_buffer, u8Length);

    *pBuf = (uint32_t)eeprom.data;

    return 0;              /**< success */
}

/**
 * @brief This function write a byte to the EEPROM.
 * @param[in] pHandle points to mcp794xx handle structure
 * @param[in] u8Address is the address to write
 * @param[in] *pBuffer point to data write
 * @param[in] u8Length is the data size
 * @return status code
 *            - 0 success
 *            - 1 read failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid address (failed to validate address)
 * @note    The value is written only if differs from the one already saved at the same address.
 */
uint8_t mcp794xx_eeprom_update(mcp794xx_handle_t *const pHandle, uint8_t u8Address, uint8_t *pBuffer, uint8_t u8Length)
{
    uint8_t tempBuf[u8Length];
    uint8_t index;

    if(pHandle == NULL)
        return 2;     					/**< return failed error */
    if(pHandle->inited != 1)
        return 3;     					/**< return failed error */

    memset(tempBuf, 0x00, u8Length);   	/* clear memory */

    err = mcp794xx_eeprom_read_byte(pHandle, u8Address, (uint8_t *)tempBuf, u8Length);
    if(err != MCP794XX_DRV_OK){
    	a_mcp794xx_print_error_msg(pHandle, "update eeprom, read failed");
    	return err;
    }

    for(index = 0; index < u8Length; index++)
    {
        if(tempBuf[index] != pBuffer[index])
        {
            err = mcp794xx_eeprom_write_byte(pHandle, (u8Address + index), (uint8_t*)&pBuffer[index], 1);
            if(err != MCP794XX_DRV_OK){
            	a_mcp794xx_print_error_msg(pHandle, "update eeprom, write failed");
            	return err;   /**< failed error code*/
            }
        }

    }

	return 0;    /**< success */
}

/**
 * @brief     This function erase eeprom page
 * @param[in] pHandle points to mcp794xx handle structure
 * @param[in] u8Page_number is the eeprom page number to erase
 * @return    status code
 *            - 0 success
 *            - 1 read failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid page (out of boundary)
 * @note      Page erase instruction will erase all bits (FFh) within a the given page.
 */
uint8_t mcp794xx_eeprom_erase_page(mcp794xx_handle_t *const pHandle, uint8_t u8Page_number)
{
	uint8_t page_start_addr;
	uint8_t dummy_byte_arr[EEPROM_PAGE_SIZE];

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    memset((uint8_t *)dummy_byte_arr, 0xff, sizeof(dummy_byte_arr));

    err = mcp794xxx_eeprom_validate_page(pHandle, u8Page_number, (uint8_t *)&page_start_addr);
    if(err)
    {
    	a_mcp794xx_print_error_msg(pHandle, "to erase page, invalid page number");
    	return err;
    }

    err = mcp794xx_eeprom_write_byte(pHandle, page_start_addr, (uint8_t *)dummy_byte_arr, EEPROM_PAGE_SIZE);
    if(err)
    {
    	a_mcp794xx_print_error_msg(pHandle, "to erase page");
    	return err;
    }

    return 0;              /**< success */
}

/**
 * @brief     This function erase a sector stored data
 * @param[in] pHandle points to mcp794xx handle structure
 * @param[in] pSector_addr is the eeprom sector address (start address and end address) to erase
 * @return    status code
 *            - 0 success
 *            - 1 erase failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid address
 * @note      sector erase instruction will erase all bits (FFh) within a given sector
 */
uint8_t mcp794xx_eeprom_erase_sector(mcp794xx_handle_t *const pHandle, uint8_t *pSector_addr)
{
	uint8_t index;
	uint8_t dummy_data = 0xff;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    for(index = pSector_addr[0]; index < pSector_addr[1]; index++){
        err = mcp794xx_eeprom_write_byte(pHandle, index, (uint8_t *)&dummy_data, 1);
        if(err)
        {
        	a_mcp794xx_print_error_msg(pHandle, "erase eeprom sector");
            return 1;           /**< failed error */
        }
    }

    return 0;              /**< success */
}

/**
 * @brief     This function erase chip stored data
 * @param[in] pHandle points to mcp794xx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 erase failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      chip erase instruction will erase all bits (FFh) in the array.
 */
uint8_t mcp794xx_eeprom_erase_chip(mcp794xx_handle_t *const pHandle)
{
	uint8_t index;
	uint8_t dummy_data_arr[EEPROM_PAGE_SIZE];

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    memset((uint8_t *)dummy_data_arr, 0xff, sizeof(dummy_data_arr));          /**< fill the entire array with a value of 0xff */

    for(index = EEPROM_LOWEST_ADDRESS; index < EEPROM_MEMORY_SIZE; (index += EEPROM_PAGE_SIZE))
    {
        err = mcp794xx_eeprom_write_byte(pHandle, index, (uint8_t *)dummy_data_arr, EEPROM_PAGE_SIZE);
        if(err)
        {
        	a_mcp794xx_print_error_msg(pHandle, "wipe eeprom");
            return 1;           /**< failed error */
        }
    }

    return 0;              /**< success */
}


/**
 * @brief     This function set write enable latch status
 * @param[in] pHandle points to mcp794xx handle structure
 * @param[in] status is the value to write
 * @return    status code
 *            - 0 success
 *            - 1 set bp failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp794xx_eeprom_set_bp_status(mcp794xx_handle_t *const pHandle, mcp794xx_eeprom_block_protect_stat_t status)
{
    uint8_t block_protect_status = 0;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    block_protect_status |=  (status << 2);
    err = eeprom_mcp794xx_i2c_write(pHandle, EEPROM_BLOCK_PROTECT_REG, (uint8_t *)&block_protect_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "write eeprom block protect  reg");
        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief     This function get write enable latch status
 * @param[in] pHandle points to mcp794xx handle structure
 * @param[out] *pStatus is the value to write
 * @return    status code
 *            - 0 success
 *            - 1 get bp failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp794xx_eeprom_get_bp_status(mcp794xx_handle_t *const pHandle, mcp794xx_eeprom_block_protect_stat_t *pStatus)
{
	uint8_t block_protect_status = 0;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = eeprom_mcp794xx_i2c_read(pHandle, EEPROM_BLOCK_PROTECT_REG, (uint8_t *)&block_protect_status, 1);
    if(err)
    {
    	a_mcp794xx_print_error_msg(pHandle, "read eeprom block protect reg");
        return 1;           /**< failed error */
    }

    *pStatus = (mcp794xx_eeprom_block_protect_stat_t)(block_protect_status & EEPROM_BLOCK_PROTECT_MASK) >> 2;

    return 0;               /**< success */
}

/**
 * @brief This function runs basic checks before writing
 * @param[in] pHandle points to mcp794xx handle structure
 * @param[in] u8Address is the address to write
 * @return status code
 *          - 0 success
 *          - 1 failed
 *          - 2 handle is NULL
 *          - 3 handle is not initialized
 *          - 4 invalid device variant
 * @note    none
 */
uint8_t mcp794xx_eeprom_check_bp_before_write(mcp794xx_handle_t *const pHandle, uint8_t u8Address)
{
	uint8_t status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

   err = mcp794xx_eeprom_get_bp_status(pHandle, (uint8_t *)&status);
   if(err != MCP794XX_DRV_OK){
	   a_mcp794xx_print_error_msg(pHandle, "to verify memory block protect status");
	   return err;
   }

   if(pHandle->device_variant == MCP79400_VARIANT || pHandle->device_variant == MCP7940M_VARIANT || pHandle->device_variant == MCP7940N_VARIANT || pHandle->device_variant == MCP79410_VARIANT){
	   a_mcp794xx_print_error_msg(pHandle, "invalid device variant, the device does have a eeprom built-in");
	   return 4;
   }

   switch(status)
   {
		case MCP794XX_EEPROM_BP00:
		{
		   return 0;
		   break;
		}

		case MCP794XX_EEPROM_BP01:
		{
			if (u8Address >= (uint8_t )(EEPROM_MEMORY_SIZE * 0.75))
			{
				return 1;
			}else{
				return 0;
			}

			break;
		}

		case MCP794XX_EEPROM_BP10:
		{
			if (u8Address >= (uint8_t )(EEPROM_MEMORY_SIZE * 0.5))
			{
				return 1;
			}else{
				return 0;
			}
			break;
		}

		case MCP794XX_EEPROM_BP11:
		{
			return 1;
			break;
		}
   }

    return 0;              /**< success */
}

/**
 * @brief This function checks to make that we're not trying  to write beyond page boundary
 * @param[in] *pHandle point to the handle structure
 * @param[in] u8Address is the address to validate
 * @param[in] u8Length is data length
 * @return status code
 *          - 0 success (validated)
 *          - 1 failed to validate
 *          - 2 handle is NULL
 *          - 3 handle is not initialized
 * @note    none
 */
uint8_t mcp794xx_eeprom_validate_page_boundary(mcp794xx_handle_t *const pHandle, uint8_t u8Address, uint8_t u8Length)
{
	int index, res;
	float status;
	uint16_t startAdress, endAddress;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    for (index = 0; index < (mcp794xx_eeprom_get_legth(pHandle) / EEPROM_PAGE_SIZE); index++)
    {

        if ((index * EEPROM_PAGE_SIZE) > u8Address) /**< get start address page number(index) */
        {
        	status = (u8Address + u8Length) / (float) EEPROM_PAGE_SIZE;
            res = ceil(status);
            if (res > index)
            {
                startAdress = (uint16_t) (EEPROM_PAGE_SIZE * (index - 1));
                endAddress = (startAdress + EEPROM_PAGE_SIZE) - 1;

                a_mcp794xx_print_error_msg(pHandle, "to execute, attempt to write beyond page boundary");
                pHandle->debug_print("mcp794xx: page %d start address:%d end address:%d\r\n", index, startAdress, endAddress);
                return 6; /**< return failed error **/
            }
            break;
        }
    }

    return 0;               /**< success */
}

/**
 * @brief This function validate the page number to write or read
 * @param[in] *pHandle point to the handle structure
 * @param[in] u8Page is the page to validate
 * @param[out] *pStart_addr is the start address of the page to write
 * @return status code
 *          - 0 success (page)
 *          - 1 failed page (out of range)
*           - 2 handle is NULL
 *          - 3 handle is not initialized
 * @note    none
 */
uint8_t mcp794xxx_eeprom_validate_page(mcp794xx_handle_t *const pHandle, uint8_t u8Page, uint8_t *pStart_addr)
{

	uint8_t max_num_pages; /**< maximum number of pages starting from page 1 */

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    max_num_pages = EEPROM_MEMORY_SIZE / EEPROM_PAGE_SIZE;
    if (u8Page > max_num_pages)
    {
       pHandle->debug_print("mcp794xx: page %d is greater than max page number %d\r\n", u8Page, max_num_pages);
        return 4; /**< return failed error **/
    }

    *pStart_addr = (uint8_t) (EEPROM_PAGE_SIZE * (u8Page - 1));
    return 0;              /**< success */

}

/**
 * @brief     This function get the size of eeprom in byte
 * @param[in] pHandle points to mcp794xx handle structure
 * @param[in] length is the eeprom size in byte
 * @return    status code
 *            - 0 success
 *            - 1 failed get length
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      Tgis function returns the eeprom size
 */
uint8_t mcp794xx_eeprom_get_legth(mcp794xx_handle_t *const pHandle)
{

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    return EEPROM_MEMORY_SIZE;              /**< success */
}

/**
 * @brief     set the chip register
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] reg is the i2c register address
 * @param[in] *buf points to a data buffer
 * @param[in] u8Len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp794xx_set_reg(mcp794xx_handle_t *const pHandle, uint8_t u8Reg, uint8_t *buf, size_t u8Len)
{

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */


    err = rtc_mcp794xx_i2c_write(pHandle, u8Reg, (uint8_t *)buf);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "write register");
        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief      get the chip register
 * @param[in]  pHandle points to a mcp794xx handle structure
 * @param[in]  reg is the i2c register address
 * @param[out] *buf points to a data buffer
 * @param[in]  u8Len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp794xx_get_reg(mcp794xx_handle_t *const pHandle, uint8_t u8Reg, uint8_t *buf, size_t u8Len)
{

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, u8Reg, (uint8_t *)buf, u8Len);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read register");
        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief      get chip's information
 * @param[in] mcp794xx_handle_t *const pHandle
 * @return  status code
 *             - 0 success
 *             - 2 pHandle is NULL
 * @note       none
 */
uint8_t mcp794xx_info(mcp794xx_handle_t *const pHandle)
{

    strncpy(pHandle->info.chip_name, CHIP_NAME, 10);                        /**< copy chip name */
    strncpy(pHandle->info.interface, INTERFACE, 5);                         /**< copy interface name */
    strncpy(pHandle->info.manufacturer_name, MANUFACTURER_NAME, 25);        /**< copy manufacturer name */
    pHandle->info.supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /**< set minimal supply voltage */
    pHandle->info.supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /**< set maximum supply voltage */
    pHandle->info.max_current_ma = MAX_CURRENT;                             /**< set maximum current */
    pHandle->info.temperature_max = TEMPERATURE_MAX;                        /**< set minimal temperature */
    pHandle->info.temperature_min = TEMPERATURE_MIN;                        /**< set maximum temperature */
    pHandle->info.flash_size_min = MCU_FLASH_MIN;                           /**< set the Micro-controller minimum recommended flash size */
    pHandle->info.ram_size_min = MCU_RAM_MIN;                               /**< set the Micro-controller minimum recommended ram size */
    pHandle->info.driver_version = DRIVER_VERSION;                          /**< set driver version */

    return 0;
}
