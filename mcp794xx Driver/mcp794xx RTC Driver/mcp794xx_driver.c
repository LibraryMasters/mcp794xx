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
uint8_t rtc_mcp794xx_i2c_write(mcp794xx_handle_t *const pHandle, uint8_t u8Reg, void *pBuf)
{
    if(pHandle->i2c_write(pHandle->rtc_address, u8Reg, (uint8_t*)pBuf, 1) !=  0)
    {
        return 1;                                      /**< return an error if failed to execute */
    }
    return 0;                                          /**< return success */
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

uint8_t rtc_mcp794xx_i2c_read(mcp794xx_handle_t  *const pHandle, uint16_t u8Reg, void *pBuf, uint8_t u8Length)
{
    if(pHandle->i2c_read(pHandle->rtc_address, u8Reg, (uint8_t*)pBuf, u8Length) != 0)
    {
        return 1;                                       /**< return an error if failed to execute */
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
uint8_t eeprom_mcp794xx_i2c_write(mcp794xx_handle_t *const pHandle, uint8_t u8Reg, void *pBuf, uint8_t u8Length)
{
    if(pHandle->i2c_write(pHandle->eeprom_address, u8Reg, (uint8_t*)pBuf, u8Length) !=  0)
    {
        return 1;                                      /**< return an error if failed to execute */
    }
    return 0;                                          /**< return success */
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

uint8_t eeprom_mcp794xx_i2c_read(mcp794xx_handle_t  *const pHandle, uint16_t u8Reg, void *pBuf, uint8_t u8Length)
{
    if(pHandle->i2c_read(pHandle->eeprom_address, u8Reg, (uint8_t*)pBuf, u8Length) != 0)
    {
        return 1;                                       /**< return an error if failed to execute */
    }
    return 0;                                           /**< return success */
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
#ifdef mcp794xx_DEBUG_MODE
    pHandle->debug_print("mcp794xx: failed to %s.\r\n", pBuffer);
#endif // mcp794xx_DEBUG_MODE
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
    return (((u8Bcd >> 4)*10) + (u8Bcd & 0xF));
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
 *            - 1 i2c deinit failed
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
 * @param[in] pHandle points to mcp794xx pHandle structure
 * @return  status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      none
 */
uint8_t mcp794xx_irq_pHandler(mcp794xx_handle_t *const pHandle)
{
    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief     This function set the address pin
 * @param[in] pHandle points to a mcp794xx pHandle structure
 * @param[in] pI2c_address point to both i2c addresses pins
 * @return  status code
 *            - 0 success
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      none
 */
uint8_t mcp794xx_set_addr_pin(mcp794xx_handle_t *const pHandle, mcp794xx_i2c_addr_t *pI2c_address)
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
 * @brief     This function set the time
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
uint8_t mcp794xx_set_time(mcp794xx_handle_t *const pHandle, mcp794xx_time_t *pTime)
{

    uint8_t ptimeBuffer[MCP794XX_TIME_BUFFER_SIZE];

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(pTime->time_Format == MCP794XX_24HR_FORMAT)
    {
        if((pTime->year < 0) || (pTime->year > 99))
        {
            a_mcp794xx_print_error_msg(pHandle, "set time, year can not be less than 0 or greater than 99");
            return 4;
        }

        if((pTime->month < 0) || (pTime->month > 12))
        {
            a_mcp794xx_print_error_msg(pHandle, "set time, month can not be less than 0 or greater than 12");
            return 4;
        }

        if((pTime->date < 0) || (pTime->date > 31))
        {
            a_mcp794xx_print_error_msg(pHandle, "set time, date can not be less than 0 or greater than 31");
            return 4;
        }

        if((pTime->weekDay < 0) || (pTime->weekDay > 6))
        {
            a_mcp794xx_print_error_msg(pHandle, "set time, week day can not be less than 0 or greater than 6");
            return 4;
        }

        if((pTime->hour < 0) || (pTime->hour > 23))
        {
            a_mcp794xx_print_error_msg(pHandle, "set time, hour can not be less than 0 or greater than 23");
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


    }

    else if(pTime->time_Format == MCP794XX_12HR_FORMAT)
    {
        if((pTime->year < 0) || (pTime->year > 99))
        {
            a_mcp794xx_print_error_msg(pHandle, "set time, year can not be less than 0 or greater than 99");
            return 4;
        }

        if((pTime->month < 0) || (pTime->month > 12))
        {
            a_mcp794xx_print_error_msg(pHandle, "set time, month can not be less than 0 or greater than 12");
            return 4;
        }

        if((pTime->date < 0) || (pTime->date > 31))
        {
            a_mcp794xx_print_error_msg(pHandle, "set time, date can not be less than 0 or greater than 31");
            return 4;
        }

        if((pTime->weekDay < 0) || (pTime->weekDay > 6))
        {
            a_mcp794xx_print_error_msg(pHandle, "set time, week day can not be less than 0 or greater than 6");
            return 4;
        }

        if((pTime->hour < 0) || (pTime->hour > 12))
        {
            a_mcp794xx_print_error_msg(pHandle, "set time, hour can not be less than 0 or greater than 23");
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
    err =  rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_SECOND_REG, a_pcf85xxx_dec2bcd(pTime->second) | (a_pcf85xxx_bcd2dec(ptimeBuffer[0] & MCP794XX_ST_MASK)));
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "to write seconds");
        return 1;
    }

    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_MINUTE_REG, a_pcf85xxx_dec2bcd(pTime->minute));
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "to write minutes");
        return 1;
    }

    if(pTime->time_Format == MCP794XX_24HR_FORMAT)
    {
        err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_HOUR_REG, a_pcf85xxx_dec2bcd(pTime->hour) | (a_pcf85xxx_bcd2dec(ptimeBuffer[1] & MCP794XX_12HR_24HR_FRMT_STAT_MASK)));
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "to write hours");
            return 1;
        }
    }

    else if(pTime->time_Format == MCP794XX_12HR_FORMAT)
    {
        err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_HOUR_REG, a_pcf85xxx_dec2bcd(pTime->hour) | (a_pcf85xxx_bcd2dec(ptimeBuffer[2] & (MCP794XX_12HR_24HR_FRMT_STAT_MASK | MCP794XX_TIME_AM_PM_MASK))));
        if(err)
        {
            a_mcp794xx_print_error_msg(pHandle, "to write hours");
            return 1;
        }
    }

    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_WKDAY_REG, a_pcf85xxx_dec2bcd(pTime->weekDay) | (a_pcf85xxx_bcd2dec(ptimeBuffer[3] & (MCP794XX_OSC_RUN_STATUS_MASK | MCP794XX_PWR_FAIL_STATUS_MASK | MCP794XX_VBAT_EN_MASK))));
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "to write week day");
        return 1;
    }

    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_DATE_REG, a_pcf85xxx_dec2bcd(pTime->date));
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "to write date");
        return 1;
    }

    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_MONTH_REG, a_pcf85xxx_dec2bcd(pTime->month) | (a_pcf85xxx_bcd2dec(ptimeBuffer[5] & MCP794XX_LEAP_YEAR_STATUS_MASK)));
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "to write month");
        return 1;
    }

    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_MONTH_REG, a_pcf85xxx_dec2bcd(pTime->year));
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
uint8_t mcp794xx_get_time(mcp794xx_handle_t *const pHandle, mcp794xx_time_t *pTime)
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
    if(pTime->time_Format == MCP794XX_24HR_FORMAT)
    {
        pTime->hour = a_pcf85xxx_bcd2dec(timeBuffer[2] & MCP794XX_24HR_FRMT_BCD_MASK);
    }
    else if(pTime->time_Format == MCP794XX_12HR_FORMAT)
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
 * @param[in] format hour format
 * @return status code
 *          - 0 success
 *          - 1 failed to set time format
 * @note        none
 */
uint8_t mcp794xx_set_hour_format(mcp794xx_handle_t *const pHandle, mcp794xx_hour_format_t format)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_RTC_HOUR_REG, (uint8_t*)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read hour format before write");
        return 1;           /**< failed error */
    }

    read_status &= ~(1 << 6);            /*< clear hour format status bit */
    read_status |= (format << 6);        /*< write hour format status bit */

    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_HOUR_REG, (uint8_t*)&read_status);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "set hour format ");
        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief This function get the time format 12hr or 24hr
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[out] pFormat format point to hour format (12hr/24hr)
 * @return status code
 *          - 0 success
 *          - 1 failed to set time format
 * @note        none
 */
uint8_t mcp794xx_get_hour_format(mcp794xx_handle_t *const pHandle, mcp794xx_hour_format_t *pFormat)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_RTC_HOUR_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "get hour format");
        return 1;           /**< failed error */
    }

    *pFormat = (mcp794xx_hour_format_t)((read_status & MCP794XX_ALARMx_12HR_24HR_FRMT_STAT_MASK) >> 6);

    return 0;              /**< success */
}

/**
 * @brief This function set AM or PM indicator for 12hr time format
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] am_pm is the indicator period for 12hr time format
 * @return status code
 *          - 0 success
 *          - 1 failed to set AM/PM period
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_set_am_pm(mcp794xx_handle_t *const pHandle, mcp794xx_am_pm_indicator_t am_pm)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_RTC_HOUR_REG, (uint8_t *)&read_status, 1);
    if(err)
    {
        a_mcp794xx_print_error_msg(pHandle, "read time am/pm indicator before write");
        return 1;           /**< failed error */
    }

    read_status &= ~(1 << 5);
    read_status |= (am_pm << 5);

    err = rtc_mcp794xx_i2c_write(pHandle, MCP794XX_RTC_HOUR_REG, (uint8_t *)&read_status);
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
 * @param[out] pAm_pm point to the indicator period for 12hr time format
 * @return status code
 *          - 0 success
 *          - 1 failed to get AM/PM period
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_get_am_pm(mcp794xx_handle_t *const pHandle, mcp794xx_am_pm_indicator_t *pAm_pm)
{
    uint8_t read_status;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = rtc_mcp794xx_i2c_read(pHandle, MCP794XX_RTC_HOUR_REG, (uint8_t *)&read_status, 1);
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
 * @param[in] status is the status to be set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_clr_pwr_fail_status(mcp794xx_handle_t *const pHandle, mcp794xx_pwr_fail_status_t status)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

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
 */
uint8_t mcp794xx_get_pwr_fail_status(mcp794xx_handle_t *const pHandle, mcp794xx_pwr_fail_status_t *pStatus)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief This function get the power fail time stamp
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] powerMode is the power fail mode (power-up/power-down)
 * @param[out] pTime point to the time struct object
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_get_pwr_fail_time_stamp(mcp794xx_handle_t *const pHandle, mcp794xx_pwr_fail_type_t powerMode, mcp794xx_time_t *pTime)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief This function get the leap year status
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[out] pLeapYearStatus point to leap year status
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_get_leap_year_status(mcp794xx_handle_t *const pHandle, mcp794xx_leap_year_status_t *pLeapYearStatus)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
* @brief This function set the alarm enable status bit
* @param[in] pHandle points to a mcp794xx handle structure
* @param[in] status is the enable boolean status to be set
* @return status code
*          - 0 success
*          - 1 failed
           - 2 handle null
*          - 3 handle is not initialized
*/
uint8_t mcp794xx_set_alarm_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t status)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
* @brief This function set the alarm enable status bit
* @param[in] pHandle points to a mcp794xx handle structure
* @param[out] pStatus point to the enable boolean status set
* @return status code
*          - 0 success
*          - 1 failed
           - 2 handle null
*          - 3 handle is not initialized
*/
uint8_t mcp794xx_get_alarm_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t *pStatus)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief This function set the alarm interrupt output pin polarity
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] polarity is the polarity to be set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_set_alarm_int_output_polarity(mcp794xx_handle_t *const pHandle, mcp794xx_int_output_polarity_t polarity)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief This function get the alarm interrupt output pin polarity
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[out] pPolarity point to the polarity set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_get_alarm_int_output_polarity(mcp794xx_handle_t *const pHandle, mcp794xx_int_output_polarity_t *pPolarity)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief This function set the alarm
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] alarm is the alarm type (ALARM0/ALARM1)
 * @param[in] mask is the alarm mask to be set (second/minute/hour/.. etc)
 * @param[in] pTime point to the alarm time to set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_set_alarm(mcp794xx_handle_t *const pHandle, mcp794xx_alarm_t alarm, mcp794xx_alarm_mask_t mask, mcp794xx_time_t *pTime)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */

}

/**
 * @brief This function get the alarm status
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] alarm is the alarm type (ALARM0/ALARM1)
 * @param[out] pMask point to the alarm mask set (second/minute/hour/.. etc)
 * @param[out] pTime point to the alarm time set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_get_alarm(mcp794xx_handle_t *const pHandle, mcp794xx_alarm_t alarm, mcp794xx_alarm_mask_t *pMask, mcp794xx_time_t *pTime)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief This function get the alarm interrupt flag
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] alarm is the alarm type (ALARM0/ALARM1)
 * @param[out] flag is the interrupt flag read
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_get_alarm_interrupt_flag(mcp794xx_handle_t *const pHandle, mcp794xx_alarm_t alarm, mcp794xx_alarm_int_flag_t flag)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
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
 */
uint8_t mcp794xx_clr_alarm_interrupt_flag(mcp794xx_handle_t *const pHandle, mcp794xx_alarm_t alarm)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
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


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

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


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

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
 */
uint8_t mcp94xx_set_sqr_wave_freq(mcp794xx_handle_t *const pHandle, mcp94xx_sqr_wave_freq_t freq)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

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


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief This function enables/disables the square wave clock freq. output
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] status is the enable boolean status to set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_set_sqr_wave_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t status)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
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


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

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


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
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


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

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
 */
uint8_t mcp794xx_set_coarse_trim_mode_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t status)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
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


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief This function set the oscillator run enable status
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] status is the enable boolean status to set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_set_start_osc_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t status)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief This function get the oscillator run enable status
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[out] pStatus point to the enable boolean status
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_get_start_os_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t *pStatus)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

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
 */
uint8_t  mcp794xx_set_ext_batt_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t status)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
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


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief This function set the external oscillator enable bit
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] status is the enable boolean status to set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_set_ext_osc_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t status)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief This function get the external oscillator enable bit
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[out] pStatus point to the enable boolean status to set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_get_ext_osc_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t *pStatus)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief     This function get the device serial number
 * @param[in] *pHandle points to mcp794xx pHandle structure
 * @param[out] pUID point to the device Unique ID
 * @return  status code
 *            - 0 success
 *            - 1 failed to get S/N
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      none
 */
uint8_t mcp794xx_get_serial_number(mcp794xx_handle_t *const pHandle, uint32_t *pUID)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief     This function write n byte to eeprom
 * @param[in] pHandle points to mcp794xx handle structure
 * @param[in] addr is the eeprom address to write to
 * @param[in] *buf points to data buffer to be written
 * @param[in] u16Length is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid address
 *            - 5 attempt to write a protected block array
 *            - 6 data length is larger than page size
 * @note      none
 */
uint8_t mcp794xx_eeprom_write_byte(mcp794xx_handle_t *const pHandle, uint16_t u16Addr, uint8_t *pBuf, uint16_t u16Length)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

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
 * @note      none
 */
uint8_t mcp794xx_eeprom_read_byte(mcp794xx_handle_t *const pHandle, uint16_t u16Addr, uint8_t *pBuf, uint16_t u16Length)
{


    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

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
    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

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
    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief     This function erase eeprom page
 * @param[in] pHandle points to mcp794xx handle structure
 * @param[in] u8PageNumber is the eeprom page number to erase
 * @return    status code
 *            - 0 success
 *            - 1 read failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid page (out of boundary)
 * @note      Page erase instruction will erase all bits (FFh) within a the given page.
 */
uint8_t mcp794xx_eeprom_erase_page(mcp794xx_handle_t *const pHandle, uint8_t u8PageNumber)
{

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
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
    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
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
    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
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

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

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

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief This function runs basic checks before writing
 * @param[in] pHandle points to mcp794xx handle structure
 * @param[in] u8Address is the address to write
 * @return status code
 *          - 0 success
 *          - 1 failed
 * @note    none
 */
uint8_t mcp794xx_eeprom_check_bp_before_write(mcp794xx_handle_t *const pHandle, uint8_t u8Address)
{
    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}


/**
 * @brief This function validate the address to write or read
 * @param[in] *pHandle point to the handle structure
 * @param[in] u16Address is the address to validate
 * @return status code
 *          - 0 success (validated)
 *          - 1 failed validate
 * @note    none
 */
uint8_t mcp794xxx_eeprom_validate_address(mcp794xx_handle_t *const pHandle)
{

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
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

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief This function validate the page number to write or read
 * @param[in] *pHandle point to the handle structure
 * @param[in] u16Page is the page to validate
 * @param[out] *pStartAddr is the start address of the page to write
 * @return status code
 *          - 0 success (page)
 *          - 1 failed page (out of range)
*           - 2 handle is NULL
 *          - 3 handle is not initialized
 * @note    none
 */
uint8_t mcp794xxx_eeprom_validate_page(mcp794xx_handle_t *const pHandle, uint8_t u8Page, uint8_t *pStartAddr)
{

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

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
 * @note      none
 */
uint8_t mcp794xx_eeprom_get_legth(mcp794xx_handle_t *const pHandle)
{

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
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

    if(err)
    {

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

    if(err)
    {

        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief      get chip's information
 * @param[out] pInfo points to mcp794xx info structure
 * @return  status code
 *             - 0 success
 *             - 2 pHandle is NULL
 * @note       none
 */
uint8_t mcp794xx_info(mcp794xx_info_t *const pInfo)
{
    if (pInfo == NULL)     /**< check if handle is null */
    {
        return 2;              /**<failed, return error */
    }
    memset(pInfo, 0, sizeof (mcp794xx_info_t));                      /**< initialize mcp794xx info structure */
    strncpy(pInfo->chip_name, CHIP_NAME, 10);                        /**< copy chip name */
    strncpy(pInfo->interface, INTERFACE, 5);                         /**< copy interface name */
    strncpy(pInfo->manufacturer_name, MANUFACTURER_NAME, 25);        /**< copy manufacturer name */
    pInfo->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /**< set minimal supply voltage */
    pInfo->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /**< set maximum supply voltage */
    pInfo->max_current_ma = MAX_CURRENT;                             /**< set maximum current */
    pInfo->temperature_max = TEMPERATURE_MAX;                        /**< set minimal temperature */
    pInfo->temperature_min = TEMPERATURE_MIN;                        /**< set maximum temperature */
    pInfo->flash_size_min = MCU_FLASH_MIN;                           /**< set the Micro-controller minimum recommended flash size */
    pInfo->ram_size_min = MCU_RAM_MIN;                               /**< set the Micro-controller minimum recommended ram size */
    pInfo->driver_version = DRIVER_VERSION;                          /**< set driver version */

    return 0;
}


