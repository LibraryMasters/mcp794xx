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
 * File:   mcp794xx_driver_basic.h
 * Author: Cedric Akilimali
 *
 * Created on May 06, 2023, 2:34 PM
 */


#ifndef MCP794XX_DRIVER_BASIC_H_INCLUDED
#define MCP794XX_DRIVER_BASIC_H_INCLUDED

#include "mcp794xx_driver_interface.h"

/**
 * @brief basic example initialize
 * @param[in] variant is the device type
 * @return status code
 *          - 0 success
 *          - 1 initialize failed
 * @note    none
 */

uint8_t mcp794xx_basic_initialize(mcp794xx_variant_t variant);

 /**
 * @brief basic implementation of interrupt handler
 * @param[in] alarm is the alarm number (ALARM0/ALARM1)
 * @return status code
 *          - 0 success
 *          - 1 fail to run handler
 */
uint8_t mcp794xx_basic_irq_handler(mcp794xx_alarm_t alarm);

/**
 * @brief interrupt request callback function
 * @param[in] alarm is the alarm number to service the callback for
 * @param[in] cb point to the interrupt routine function
 * @return status code
 *          - 0 succeed
 *          - 1 failed to run
 * @note none
 */
uint8_t mcp794xx_basic_irq_callBack(mcp794xx_alarm_t alarm, mcp794xx_irq_callback_t cb);

/**
 * @brief     This function get the device variant
 * @param[out] pVariant point to the device variant
 * @return  status code
 *            - 0 success
 *            - 1 failed to get device variant
 * @note      none
 */
uint8_t mcp794xx_basic_get_variant(mcp794xx_variant_t *pVariant);

/**
 * @brief basic example set time and date
 * @param[in] *time point to a time structure
 * @return status code
 *          - 0 success
 *          - 1 set time failed
 * @note    am_pm is don't care when 24hr time format is used
 */
uint8_t mcp794xx_basic_set_time_date(mcp794xx_time_t *pTime);

/**
 * @brief basic example get current time and date
 * @param[out] *time point to the time structure
 * @return status code
 *          - 0 success
 *          - 1 failed to get time
 * @note    am_pm is don't care when 24hr time format is used
 */
uint8_t mcp794xx_basic_get_current_time_date(mcp794xx_time_t *pTime);

/**
 * @brief get time format 12hr or 24hr
 * @param[in] *format point to the hour format to set
 * @return status code
 *          - 0 success
 *          - 1 failed to set format
 * @note    none
 */
uint8_t mcp794xx_basic_set_time_format(mcp794xx_time_format_t format);

/**
 * @brief get time format 12hr or 24hr
 * @param[in] *format point to the hour format to set
 * @return status code
 *          - 0 success
 *          - 1 failed to set format
 * @note    none
 */
uint8_t mcp794xx_basic_get_time_format(mcp794xx_time_format_t *pFormat);

/**
 * @brief This function clears the power fail status bit
 * @param[in] pHandle points to a mcp794xx handle structure
 * @return status code
 *          - 0 success
 *          - 1 failed
 *          - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_basic_clr_pwr_fail_status(void);

/**
 * @brief This function is a basic implementation to get the power fail status bit
 * @param[out] pStatus point to the status be set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_basic_get_pwr_fail_status(mcp794xx_pwr_fail_status_t *pStatus);

/**
 * @brief This function is a basic implementation on how to get the power fail time stamp
 * @param[in] powerMode is the power fail mode (power-up/power-down)
 * @param[out] pTime point to the time structure object
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 * @note reading or writing the the external battery back up bit will clear the pwr fail time stamp.
         Time stamp should be read at start up before reading or setting current time and date
 */
uint8_t mcp794xx_basic_get_pwr_fail_time_stamp(mcp794xx_pwr_fail_type_t powerMode, mcp794xx_time_t *pTime);

/**
 * @brief This function is a basic implementation on how to  enable the external battery back up power
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 * @note reading or writing the the external battery back up bit will clear the pwr fail time stamp.
         Time stamp should be read at start up before reading or setting current time and date
 */
uint8_t  mcp794xx_basic_enable_ext_batt_bckup_pwr(void);


/**
 * @brief This function is a basic implementation on how to disable the external battery back up power
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 * @note reading or writing the the external battery back up bit will clear the pwr fail time stamp.
         Time stamp should be read at start up before reading or setting current time and date
 */
uint8_t  mcp794xx_basic_disable_ext_batt_bckup_pwr(void);

/**
* @brief This function is a basic implementation to disable alarm status bit
* @param[in] alarm is the  alarm number 1/0 to disable
* @return status code
*          - 0 success
*          - 1 failed
           - 2 handle null
*          - 3 handle is not initialized
*/
uint8_t mcp794xx_basic_enable_alarm(mcp794xx_alarm_t alarm, mcp794xx_int_output_polarity_t polarity);

/**
* @brief This function is a basic implementation to disable alarm status bit
* @param[in] alarm is the  alarm number 1/0 to disable
* @return status code
*          - 0 success
*          - 1 failed
           - 2 handle null
*          - 3 handle is not initialized
*/
uint8_t mcp794xx_basic_disable_alarm(mcp794xx_alarm_t alarm);

/**
 * @brief This function is a basic implementation to set alarm time and date
 * @param[in] alarm is the alarm type (ALARM0/ALARM1)
 * @param[in] mask is the alarm mask to be set (second/minute/hour/.. etc)
 * @param[in] pTime point to the alarm time to set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 *          - 4 invalid alarm
 *          - 5 invalid mask
 * @note prior to this function to be called, the "mcp794xx_basic_enable_alarm" to enable interrupt and set polarity
 */
uint8_t mcp794xx_basic_set_alarm_time_date(mcp794xx_alarm_t alarm, mcp794xx_alarm_mask_t mask, mcp794xx_time_t *pTime);

/**
 * @brief This function is a basic implementation to get the alarm status
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
uint8_t mcp794xx_basic_get_alarm_time_date(mcp794xx_alarm_t alarm, mcp794xx_alarm_mask_t *pMask, mcp794xx_time_t *pTime);

/**
 * @brief This function is a basic implementation to get the alarm interrupt flag
 * @param[in] alarm is the alarm type (ALARM0/ALARM1)
 * @param[out] pFlag point to the interrupt flag read
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 *          - 4 invalid alarm
 */
uint8_t mcp794xx_basic_get_alarm_interrupt_flag(mcp794xx_alarm_t alarm, mcp794xx_alarm_int_flag_t *pFlag);

/**
 * @brief This function is a basic implementation to clear alarm interrupt flag
 * @param[in] alarm is the alarm type (ALARM0/ALARM1)
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_basic_clr_alarm_interrupt_flag(mcp794xx_alarm_t alarm);

/**
 * @brief     This function is a basic implementation to enable countdown interrupt
 * @return  status code
 *            - 0 success
 *            - 1 failed
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      This function use ALARM1 and overwrite existing data within register
 */
uint8_t mcp794xx_basic_enable_countdown_interrupt(void);

/**
 * @brief     This function is a basic implementation to disable countdown interrupt
 * @return  status code
 *            - 0 success
 *            - 1 failed
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note
 */
uint8_t mcp794xx_basic_disable_countdown_interrupt(void);

/**
 * @brief     This function is a basic implementation to set countdown time interrupt to fire every xx time set
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
uint8_t mcp794xx_basic_set_countdown_time(uint16_t u16Time_value, mcp794xx_countdwon_time_unit_t time_unit);

/**
 * @brief This function is a basic implementation to get the countdown interrupt flag
 * @param[out] pFlag point to the interrupt flag read
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 *          - 4 invalid alarm
 */
uint8_t mcp794xx_basic_get_countdown_int_flag(mcp794xx_alarm_int_flag_t *pFlag);

/**
 * @brief This function is a basic implementation to clear countdown interrupt flag
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_basic_clr_countdown_int_flag(void);

/**
 * @brief This function is a basic implementation to enable the square wave clock freq. output
 * @param[in] enable is the enable boolean status to set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_basic_enable_sqr_wave_output(void);

/**
 * @brief This function is a basic implementation to disable the square wave clock freq. output
 * @param[in] enable is the enable boolean status to set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_basic_disable_sqr_wave_output(void);

/**
 * @brief This function is a basic implementation to set the square wave frequency clock output on MFP
 * @param[in] freq is the frequency clock output to set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 * @note prior to setting frequency output, the "mcp794xx_basic_enable_sqr_wave_output" must called first to enable this feature.
 */
uint8_t mcp94xx_basic_set_sqr_wave_output_freq(mcp94xx_sqr_wave_freq_t freq);

/**
 * @brief This function is a basic implementation to set the coarse trim mode enable status
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 * @note Coarse Trim mode results in the MCP7941X applying digital trimming every 64 Hz clock cycle.
 */
uint8_t mcp794xx_basic_enable_coarse_trim(void);

/**
 * @brief This function is a basic implementation to set the coarse trim mode disable status
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 * @note Coarse Trim mode results in the MCP7941X applying digital trimming every 64 Hz clock cycle.
 */
uint8_t mcp794xx_basic_disable_coarse_trim(void);

/**
 * @brief This function  is a basic implementation to set the Oscillator Trim Value
 * @param[in] sign is the arithmetic operation (+/-)
 * @param[in] value is the clock cycle value to the added or subtracted
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_basic_set_coarse_trim_value(mcp794xx_trim_sign_t sign, mcp794xx_trim_val_t value);

/**
 * @brief     This function is a basic implementation to read the current time in epoch/Unix format
 * @param[in] pTime point to the current time in human readable format
 * @param[out] pEpoch_time point to the epoch time converted
 * @return  status code
 *            - 0 success
 *            - 1 failed
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      This function will fail to execute from January 19, 2038
 */
uint8_t mcp794xx_basic_convert_time_to_epoch_unix_time(mcp794xx_time_t *pTime, uint32_t *pEpoch_time);

/**
 * @brief  This function is a basic implementation to convert epoch time to GMT human readable time
 * @param[in] epoch_unix_time point to the epoch time converted
 * @param[out] pTime point to time output in human readable format
 * @return  status code
 *            - 0 success
 *            - 1 failed to get
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      This function will fail to execute from January 19, 2038
 */
uint8_t mcp794xx_basic_convert_epoch_to_human_time_format_gmt(uint32_t epoch_unix_time, mcp794xx_time_t *pTime);

/**
 * @brief  This function is a basic implementation to convert epoch time to the local human readable time
 * @param[in] epoch_unix_time point to the epoch time converted
 * @param[in] time_zone is the local time zone in relation to UCT/GMT
 * @param[out] pTime point to time output in human readable format
 * @return  status code
 *            - 0 success
 *            - 1 failed to get
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      This function will fail to execute from January 19, 2038
 */
uint8_t mcp794xx_basic_convert_epoch_to_human_time_format_local(uint32_t epoch_unix_time, int time_zone, mcp794xx_time_t *pTime);

/**
 * @brief This function is a basic implementation to read the compile time and date of the application
 * @param[out] pTime points to a time structure object
 * @param[in]  pCompile_time_date compile time and date string variable (__TIMESTAMP__)
 * @return     status code
 *             - 0 success
 *             - 1 failed
 *             - 2 handle or time is NULL
 *             - 3 handle is not initialized
 * @note       this function returns time in 24Hr format
 */
uint8_t mcp794xx_basic_get_compile_time_date(char *pCompile_time, char *pCompile_date, mcp794xx_time_t *pTime);

/**
* @brief This function is a basic implementation to write data bytes to sram
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
uint8_t mcp794xx_basic_sram_write_byte(uint8_t addr, uint8_t *pBuffer, size_t length);

/**
* @brief This function is a basic implementation to read data bytes from sram
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
uint8_t mcp794xx_basic_sram_read_byte(uint8_t addr, uint8_t *pBuffer, size_t length);

/**
* @brief This function is a basic implementation to erase a sector of the sram
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
uint8_t mcp794xx_basic_sram_erase_selector(uint8_t start_addr, uint8_t end_addr);

/**
* @brief This function is a basic implementation to wipe the entire sram memory
* @return status code
*          - 0 success
*          - 1 failed to write
*          - 2 pHandle is NULL
*          - 3 pHandle is not initialized
* @note none
*/
uint8_t mcp794xx_basic_sram_wipe(void);

/**
 * @brief     This function is a basic implementation to read the device unique identifier
 * @param[out] pUID point to the device Unique ID
 * @return  status code
 *            - 0 success
 *            - 1 failed to get S/N
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      none
 */
uint8_t mcp794xx_basic_uid_read(uint8_t *pUID);

/**
 * @brief     This function erase eeprom page
 * @param[in] u8PageNumber is the eeprom page number to erase
 * @return    status code
 *            - 0 success
 *            - 1 read failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid page (out of boundary)
 * @note      Page erase instruction will erase all bits (FFh) within a the given page.
 */
uint8_t mcp794xx_basic_eeprom_erase_page(uint8_t u8PageNumber);

/**
 * @brief     This function erase a sector stored data
 * @param[in] start_addr is the eeprom sector address start address to erase
 * @param[in] end_addr is the eeprom sector address end address to erase
 * @return    status code
 *            - 0 success
 *            - 1 erase failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid address
 * @note      sector erase instruction will erase all bits (FFh) within a given sector
 */
uint8_t mcp794xx_basic_eeprom_erase_sector(uint8_t start_addr, uint8_t end_addr);

/**
 * @brief     This function erase chip stored data
 * @return    status code
 *            - 0 success
 *            - 1 erase failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      chip erase instruction will erase all bits (FFh) in the array.
 */
uint8_t mcp794xx_basic_eeprom_erase_chip(void);

/**
 * @brief  function is a basic implementation to write the block protect status
 * @param[in] status is the value to write
 * @return    status code
 *            - 0 success
 *            - 1 set bp failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp794xx_basic_eeprom_set_bp_status(mcp794xx_eeprom_block_protect_stat_t status);

/**
 * @brief This function is a basic implementation to read the block protect status
 * @param[out] *pStatus is the value to write
 * @return    status code
 *            - 0 success
 *            - 1 get bp failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp794xx_basic_eeprom_get_bp_status(mcp794xx_eeprom_block_protect_stat_t *pStatus);

/**
 * @brief     This function is a basic implementation to write n number bytes to eeprom
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
uint8_t mcp794xx_basic_eeprom_write_byte(uint8_t u8Addr, uint8_t *pBuf, size_t length);

/**
 * @brief     This function is a basic implementation to read n number of bytes to eeprom
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
uint8_t mcp794xx_basic_eeprom_read_byte(uint8_t u8Addr, uint8_t *pBuf, size_t length);

/**
 * @brief     This function is a basic implementation to writes data larger than a byte > 255 (equivalent to eeprom.put on Arduino)
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
uint8_t mcp794xx_basic_eeprom_put_byte(uint8_t u8Addr, uint32_t *pBuf, uint8_t u8Length);

/**
 * @brief     This function is a basic implementation to reads data larger than a byte > 255 (equivalent to eeprom.get on Arduino)
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

uint8_t mcp794xx_basic_eeprom_get_byte(uint8_t u8Addr, uint32_t *pBuf, uint8_t u8Length);

/**
 * @brief This function is a basic implementation to update data in the EEPROM memory.
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
uint8_t mcp794xx_basic_eeprom_update(uint8_t u8Address, uint8_t *pBuffer, uint8_t u8Length);

/**
 * @brief basic example write register
 * @param[in] reg is the i2c register address
 * @param[in] *pBuf points to a data buffer to write
 * @param[in] u8Length is the data buffer length
 * @return status code
 *          - 0 success
 *          - 1 failed to write register
 */
uint8_t mcp794xx_basic_write_register(uint8_t u8Reg, uint8_t *pBuf, uint8_t u8Length);


/**
* @brief This function reads the device registers
* @param[in] reg is the i2c register address
* @param[out] *pBuf points to a data buffer
* @param[in] u8Length is the data buffer length
* @return status code
*           - 0 success
*           - 1 failed
* @note none
*/
uint8_t mcp794xx_basic_read_reg(uint8_t u8Reg, uint8_t *pBuf, uint8_t u8Length);

#endif // MCP794XX_DRIVER_BASIC_H_INCLUDED
