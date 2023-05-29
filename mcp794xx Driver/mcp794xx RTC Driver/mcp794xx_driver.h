
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
 * File:   mcp794xx_driver.h
 * Author: Cedric Akilimali
 *
 * Created on May 06, 2023, 2:34 PM
 */

#ifndef MCP794XX_DRIVER_H_INCLUDED
#define MCP794XX_DRIVER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#define MCP794XX_DEBUG_MODE              /**< enables/disabled all printout on the code*/
/**
* @defgroup driver_mcp794xx mcp794xx driver function
* @brief mcp794xx driver modules
* @{
*/

/**
 * @addtogrouup mcp794xx_base_driver
 * @{
 */

/** RTC REGISTER **/
#define MCP794XX_RTC_SECOND_REG                      0x00                           /**< Real time clock second register */
#define MCP794XX_RTC_MINUTE_REG                      0x01                           /**< Real time clock minute register */
#define MCP794XX_RTC_HOUR_REG                        0x02                           /**< Real time clock hour register */
#define MCP794XX_RTC_WKDAY_REG                       0x03                           /**< Real time clock week day register */
#define MCP794XX_RTC_DATE_REG                        0x04                           /**< Real time clock date register */
#define MCP794XX_RTC_MONTH_REG                       0x05                           /**< Real time clock month register */
#define MCP794XX_RTC_YEAR_REG                        0x06                           /**< Real time clock year register */
#define MCP794XX_CONTROL_REG                         0x07                           /**< Control register */
#define MCP794XX_OSC_TRIM_REG                        0x08                           /**< Oscillator trim register */
#define MCP794XX_ALM0_SEC_REG                        0x0A                           /**< Alarm 0 second register */
#define MCP794XX_ALM0_MIN_REG                        0x0B                           /**< Alarm 0 minute register */
#define MCP794XX_ALM0_HOUR_REG                       0x0C                           /**< Alarm 0 hour register */
#define MCP794XX_ALM0_WKDAY_REG                      0x0D                           /**< Alarm 0 week day register */
#define MCP794XX_ALM0_DATE_REG                       0x0E                           /**< Alarm 0 date register */
#define MCP794XX_ALM0_MONTH_REG                      0x0F                           /**< Alarm 0 month register */
#define MCP794XX_ALM1_SEC_REG                        0x11                           /**< Alarm 1 second register */
#define MCP794XX_ALM1_MIN_REG                        0x12                           /**< Alarm 1 minute register */
#define MCP794XX_ALM1_HOUR_REG                       0x13                           /**< Alarm 1 hour register */
#define MCP794XX_ALM1_WKDAY_REG                      0x14                           /**< Alarm 1 week day register */
#define MCP794XX_ALM1_DATE_REG                       0x15                           /**< Alarm 1 date register */
#define MCP794XX_ALM1_MONTH_REG                      0x16                           /**< Alarm 1 month register */
#define MCP794XX_PWR_DWN_MIN_REG                     0x18                           /**< Power down minute register */
#define MCP794XX_PWR_DWN_HOUR_REG                    0x19                           /**< Power down hour register */
#define MCP794XX_PWR_DWN_DATE_REG                    0x1A                           /**< Power down date register */
#define MCP794XX_PWR_DWN_MONTH_REG                   0x1B                           /**< Power down month register */
#define MCP794XX_PWR_UP_MIN_REG                      0x1C                           /**< Power up minute register */
#define MCP794XX_PWR_UP_HOUR_REG                     0x1D                           /**< Power up hour register */
#define MCP794XX_PWR_UP_DATE_REG                     0x1E                           /**< Power up date register */
#define MCP794XX_PWR_UP_MONTH_REG                    0x1F                           /**< Power up month register */

/** Time Keeping Register Mask **/
#define MCP794XX_ST_MASK                             0x80                           /**< Start Oscillator mask */
#define MCP794XX_SEC_BCD_MASK                        0x7F                           /**< BCD value of seconds mask */
#define MCP794XX_MIN_BCD_MASK                        0x7F                           /**< BCD value of minutes mask */
#define MCP794XX_12HR_24HR_FRMT_STAT_MASK            0x40                           /**< Time hour format mask */
#define MCP794XX_TIME_AM_PM_MASK                     0x20                           /**< Am/PM indicator mask */
#define MCP794XX_12HR_FRMT_BCD_MASK                  0x1F                           /**< BCD value of hour in 12 format mask */
#define MCP794XX_24HR_FRMT_BCD_MASK                  0x3F                           /**< BCD value of hour in 24 format mask */
#define MCP794XX_OSC_RUN_STATUS_MASK                 0x20                           /**< Oscillator run status mask */
#define MCP794XX_PWR_FAIL_STATUS_MASK                0x10                           /**< Power fail status mask */
#define MCP794XX_VBAT_EN_MASK                        0x08                           /**< Battery backup enable mask */
#define MCP794XX_WKDAY_BCD_MASK                      0x07                           /**< BCD value of Week day mask */
#define MCP794XX_DATE_BCD_MASK                       0x3F                           /**< BCD value of date mask */
#define MCP794XX_LEAP_YEAR_STATUS_MASK               0x20                           /**< BCD value of leap Year mask */
#define MCP794XX_MONTH_BCD_MASK                      0x1F                           /**< BCD value of month mask */
#define MCP794XX_YEAR_BCD_MASK                       0xFF                           /**< BCD value of year mask */

/** Alarm Configuration Mask **/
#define MCP794XX_ALARMx_SEC_BCD_MASK                 0x7F                           /**< Alarm BCD value seconds mask */
#define MCP794XX_ALARMx_MIN_BCD_MASK                 0x7F                           /**< Alarm BCD value minute mask */
#define MCP794XX_ALARMx_12HR_24HR_FRMT_STAT_MASK     0x40                           /**< Alarm hour format mask */
#define MCP794XX_ALARMx_AM_PM_MASK                   0x20                           /**< Alarm am/pm indicator mask */
#define MCP794XX_ALARMx_12HR_FRMT_BCD_MASK           0x1F                           /**< Alarm BCD value of hour in 12 format mask */
#define MCP794XX_ALARMx_24HR_FRMT_BCD_MASK           0x3F                           /**< Alarm BCD value of hour in 24 format mask */
#define MCP794XX_ALARMx_INT_POL_MASK                 0x8F                           /**< Alarm interrupt polarity mask */
#define MCP794XX_ALARMx_TYPE_MASK                    0x7F                           /**< Alarm type mask */
#define MCP794XX_ALARMx_IF_MASK                      0x08                           /**< Alarm interrupt flag mask */
#define MCP794XX_ALARMx_WDAY_MASK                    0x07                           /**< Alarm BCD value week day mask */
#define MCP794XX_ALARMx_DATE_MASK                    0x3F                           /**< Alarm BCD value date mask */
#define MCP794XX_ALARMx_MONTH_MASK                   0x1F                           /**< Alarm BCD value month mask */

/** Control Register Mask **/
#define MCP794XX_CTRL_OUT_MASK                       0x80                           /**< General purpose output bit enable mask */
#define MCP794XX_CTRL_SQR_WAVE_EN_MASK               0x40                           /**< Square wave enable bit mask*/
#define MCP794XX_CTRL_ALARM1_EN_MASK                 0x20                           /**< Alarm 1 enable bit mask */
#define MCP794XX_CTRL_ALARM0_EN_MASK                 0x10                           /**< Alarm 0 enable bit mask */
#define MCP794XX_CTRL_EXTR_OSC_EN_MASK               0x08                           /**< external oscillator enable bit mask */
#define MCP794XX_CTRL_CRS_TRIM_EN_MASK               0x04                           /**< coarse trim enable bit mask */
#define MCP794XX_CTRL_SQR_WAVE_FREQ_SELEC_MASK       0x03                           /**< Square wave freq out select mask */
#define MCP794XX_TRIM_SIGN_MASK                      0x80                           /**< Trim clock cycle sign mask*/
#define MCP794XX_TRIM_VAL_MASK                       0x7F                           /**< Coarse trim value mask */

/** Power Down/Power-up Time stamp Mask **/
#define MCP794XX_PWRXX_MIN_BCD_MASK                  0x7F                           /**< Power down/up BCD value minute mask */
#define MCP794XX_PWRXX_12HR_24HR_FRMT_STAT_MASK      0x40                           /**< Power down/up BCD value hour format mask*/
#define MCP794XX_PWRXX_TIME_AM_PM_MASK               0x20                           /**< Power down/up am/pm indicator mask */
#define MCP794XX_PWRXX_12HR_FRMT_BCD_MASK            0x1F                           /**< Power down/up BCD value hour in 12 format mask */
#define MCP794XX_PWRXX_24HR_FRMT_BCD_MASK            0x3F                           /**< Power down/up hour in 24 format mask */
#define MCP794XX_PWRXX_DATE_BCD_MASK                 0x3F                           /**< Power down/up BCD value Date mask */
#define MCP794XX_PWRXX_WKDAY_BCD_MASK                0xE0                           /**< Power down/up BCD value week day mask */
#define MCP794XX_PWRXX_MONTH_BCD_MASK                0x1F                           /**< Power down/up BCD value month mask */

#define PCF8523_TIME_BUFFER_SIZE    7                                               /**< Time buffer size */


/** EEPROM Definitions **/
#define EEPROM_PAGE_SIZE           8                                                /**< eeprom page size 8 bytes */
#define EEPROM_HIGHEST_ADDRESS     127                                              /**< eeprom highest address allowed */
#define EEPROM_UID_ADDRESS         0xF0                                             /**< eeprom unique ID address */

static uint8_t err;

/**
* @brief execution status enumeration
*/
typedef enum
{
    MCP794XX_DRV_OK          = 0x00,                                     /**< status execute success */
    MCP794XX_DRV_FAILED      = 0x01,                                     /**< status execute failed */
    MCP794XX_DRV_ERR_HANDLER = 0x02,                                     /**< status execute failed, handle is null */
    MCP794XX_DRV_ERR_INIT    = 0x03                                      /**< status execute failed, handle not initialize */
} mcp794xx_driver_execute_stat_t;

/**
* @brief mcp794xx variant type enumeration
*/
typedef enum
{
    MCP79400_VARIANT  = 0x00,                                             /**< mcp79400 variant type */
    MCP79401_VARIANT  = 0x01,                                             /**< mcp79401 variant type */
    MCP79402_VARIANT  = 0x02,                                             /**< mcp79402 variant type */
    MCP7940M_VARIANT  = 0x03,                                             /**< mcp7940M variant type */
    MCP7940N_VARIANT  = 0x04,                                             /**< mcp7940N variant type */
    MCP79410_VARIANT  = 0x05,                                             /**< mcp79410 variant type */
    MCP79411_VARIANT  = 0x06,                                             /**< mcp79411 variant type */
    MCP79412_VARIANT  = 0x07                                              /**< mcp79412 variant type */

}mcp794xx_variant_t;

/**
* @brief mcp794xx i2c address enumeration
*/
typedef enum
{
   MCP794XX_RTC_IIC_ADDRESS    = 0x58,                                   /**< Real time clock i2c address */
   MCP794XX_EEPROM_IIC_ADDRESS = 0x57                                    /**< EEPROM i2c address */
}mcp794xx_i2c_addr_t;

 /**
 * @brief mcp794xx boolean enumeration
 */
typedef enum
{
    MCP794XX_BOOL_FALSE  = 0x00,                                        /**< boolean state false */
    MCP794XX_BOOL_TRUE   = 0x01                                         /**< boolean state true  */
}mcp794xx_bool_t;


 /**
 * @brief mcp794xx hour format enumeration
 */
 typedef enum{
    MCP794XX_24HR_FORMAT = 0x00,                                        /**< 24 hour time format */
    MCP794XX_12HR_FORMAT = 0x01                                         /**< 12 hour time format */
 }mcp794xx_hour_format_t;

 /**
 * @brief mcp794xx am/pm indicator enumeration
 */
 typedef enum {
    MCP794XX_AM_INDICATOR = 0x00,                                       /**< am Indicator */
    MCP794XX_PM_INDICATOR = 0x01                                        /**< pm Indicator */
} mcp794xx_am_pm_indicator_t;

 /**
 * @brief mcp794xx oscillator status enumeration
 */
typedef enum{
    MCP794XX_OSC_DISABLED = 0x00,                                       /**< Oscillator status enabled */
    MCP794XX_OSC_ENABLED  = 0x01                                        /**< Oscillator status disabled */
}mcp794xx_osc_status_t;

 /**
 * @brief mcp794xx power failure status enumeration
 */
typedef enum{
    MCP794XX_NO_PWR_FAILED = 0x00,                                      /**< Primary power was lost */
    MCP794XX_PWR_FAILED    = 0x01                                       /**< Primary power has not been lost */
}mcp794xx_pwr_fail_status_t;

 /**
 * @brief mcp794xx leap year status enumeration
 */
typedef enum{
    MCP794XX_NO_LEAP_YEAR  = 0x00,                                      /**< Year is not a leap year */
    MCP794XX_LEAP_YEAR     = 0x01                                       /**< Year is a leap year */
}mcp794xx_leap_year_status_t;

 /**
 * @brief mcp794xx alarm interrupt output polarity enumeration
 */
 typedef enum{
    MCP794XX_INT_POLARITY_LOW  = 0x00,                                  /**< Asserted output state of MFP is a logic low level */
    MCP794XX_INT_POLARITY_HIGH = 0x01                                   /**< Asserted output state of MFP is a logic high level */
 }mcp794xx_int_output_polarity_t;

 /**
 * @brief mcp794xx alarm mask enumeration
 */
 typedef enum{
    MCP794XX_MASK_SECONDS      = 0x00,                                  /**< Alarm Seconds match */
    MCP794XX_MASK_MINUTES      = 0x01,                                  /**< Alarm Minutes match */
    MCP794XX_MASK_HOURS        = 0x02,                                  /**< Alarm Hours match (logic takes into account 12-/24-hour operation) */
    MCP794XX_MASK_WDAYS        = 0x03,                                  /**< Alarm Day of week match */
    MCP794XX_MASK_DATE         = 0x04,                                  /**< Alarm Date match */
    MCP794XX_MASK_S_M_H_WD_D_M = 0x07                                   /**< Alarm Seconds, Minutes, Hour, Day of Week, Date and Month */
 }mcp794xx_alarm_mask_t;

 /**
 * @brief mcp794xx interrupt flag enumeration
 */
 typedef enum{
     MCP794XX_NO_ALARM_INT_OCCURED = 0x00,                              /**< Alarm match did not occur */
     MCP794XX_ALARM_INT_OCCURED    = 0x01                               /**< Alarm match occurred (must be cleared in software) */
 }mcp794xx_alarm_int_flag_t;

/**
 * @brief mcp794xx alarm enumeration
 */
 typedef enum{
     MCP794XX_ALARM0    = 0x00,                                         /**< Alarm 0 select */
     MCP794XX_ALARM1    = 0x01                                          /**< Alarm 1 select */
 }mcp794xx_alarm_t;

/**
 * @brief mcp794xx MFP logic level enumeration
 */
 typedef enum{
     MCP794XX_MFP_LOGIC_LOW  = 0x00,                                    /**< MFP signal level is logic low */
     MCP794XX_MFP_LOGIC_HIGH = 0x01                                     /**< MFP signal level is logic high */
 }mcp794xx_mfp_logic_level_t;

 /**
 * @brief mcp794xx Square Wave Clock Output Frequency enumeration
 */
 typedef enum{
    MCP794XX_SQR_FREQ_SELCET_1HZ       = 0x00,                          /**< Square Wave Clock Output Frequency 1 Hz */
    MCP794XX_SQR_FREQ_SELCET_4_096KHZ  = 0x01,                          /**< Square Wave Clock Output Frequency 4.096 kHz */
    MCP794XX_SQR_FREQ_SELCET_8_192KHZ  = 0x02,                          /**< Square Wave Clock Output Frequency 8.192 kHz */
    MCP794XX_SQR_FREQ_SELCET_32_768KHZ = 0x03                           /**< Square Wave Clock Output Frequency 32.768 kHz */
 }mcp94xx_sqr_wave_freq_t;

 /**
 * @brief mcp794xx Oscillator Trim Value enumeration
 */
typedef enum{
    MCP794XX_TRIM_DISABLE        = 0x00,                                /**< Disable digital trimming */
    MCP794XX_TIM_MIN_CLK_CYCLES  = 0x01,                                /**< Add or subtract 2 clock cycles 128 */
    MCP794XX_TRIM_MAX_CLK_CYCLES = 0x3F                                 /**< Add or subtract 254 clock cycles */
}mcp794xx_trim_val_t;

 /**
 * @brief mcp794xx Trim sign enumeration
 */
typedef enum{
    MCP794XX_SUBTRACT_CLK_CYLCES = 0x00,                                /**< Add clocks to correct for slow time */
    MCP794XX_ADD_CLK_CYLCES      = 0x01                                 /**< Subtract clocks to correct for fast time */
}mcp794xx_trim_sign_t;

 /**
 * @brief mcp794xx power fail time stamp enumeration
 */
typedef enum{
    MCP794XX_PWR_DOWN_TIME_STAMP = 0x00,                                /**< Power-down time stamp */
    MCP794XX_PWR_UP_TIME_STAMP   = 0x01                                 /**< Power-up time stamp */
}mcp794xx_pwr_fail_type_t;


/**
* @brief mcp794xx EEPROM Block Protection enumeration
*/
typedef enum
{
    MCP794XX_EEPROM_BP00 = 0x00,                                       /**< Array Address Write-Protected(None)/ Array Addresses Unprotected (All)*/
    MCP794XX_EEPROM_BP01 = 0x01,                                       /**< Array Address Write-Protected(Upper 1/4)/ Array Addresses Unprotected (Upper 3/4) */
    MCP794XX_EEPROM_BP10 = 0x02,                                       /**< Array Address Write-Protected(Upper 1/2)/ Array Addresses Unprotected (Upper 1/2) */
    MCP794XX_EEPROM_BP11 = 0x03                                        /**< Array Address Write-Protected(All)/ Array Addresses Unprotected (None) */
} mcp794xx_eeprom_block_protect_stat_t;

/**
* @brief mcp794xx eeprom data read enumeration
*/
union
{
    uint8_t buffer[8];                                                 /**< buffer to hold data when eeprom_get is called */
    uint32_t data;                                                     /**< Combines buffer data in Big indian format */
}static eeprom;

/**
 * @brief mcp794xx time structure definition
 */
typedef struct mcp794xx_time_s {
    uint16_t year;                                                     /**< Year variable */
    uint8_t month;                                                     /**< Month variable */
    uint8_t weekDay;                                                   /**< Day of week variable */
    uint8_t date;                                                      /**< Date variable */
    uint8_t hour;                                                      /**< Hour variable */
    uint8_t minute;                                                    /**< Minute variable */
    uint8_t second;                                                    /**< Second variable */
    mcp794xx_hour_format_t time_Format;                                /**< Time format 12hr/24hr variable */
    mcp794xx_am_pm_indicator_t am_pm_indicator;                        /**< AM/PM indicator variable */
} mcp794xx_time_t;

/**
* @brief mcp794xx handle enumeration
*/
typedef struct mcp794xx_handle_s
{
    uint8_t (*i2c_init)(void);                                                                  /**< point to a i2c init function address */
    uint8_t (*i2c_deinit)(void);                                                                /**< point to a i2c deinit function address */
    uint8_t (*i2c_read)(uint8_t u8Addr, uint8_t u8Reg, uint8_t *pBuf,  uint8_t u8Length);       /**< point to a i2c read function address */
    uint8_t (*i2c_write)(uint8_t u8Addr, uint8_t u8Reg, uint8_t *pBuf,  uint8_t u8Length);      /**< point to a i2c write function address */
    void (*delay_ms)(uint32_t u32Ms);                                                           /**< point to a delay_ms function address */
    void(*debug_print)(char *fmt, ...);                                                         /**< point to a debug_print function address */
    void (*receive_callback)(uint8_t u8Flag, uint8_t u8Status);                                 /**< point to a receive callback function address */
    uint8_t rtc_address;                                                                        /**< rtc device address */
    uint8_t eeprom_address;                                                                        /**< eeprom device address */
    uint8_t device_variant;                                                                     /**< Device variant */
    uint8_t inited;                                                                             /**< Driver initialize status bit */
} mcp794xx_handle_t;

 /**
 * @brief mcp794xx information structure definition
 */
typedef struct mcp794xx_info_s
{
    char chip_name[10];                                                 /**< chip name */
    char manufacturer_name[25];                                         /**< manufacturer name */
    char interface[5];                                                  /**< chip interface name */
    float supply_voltage_min_v;                                         /**< chip min supply voltage */
    float supply_voltage_max_v;                                         /**< chip max supply voltage */
    float max_current_ma;                                               /**< chip max current */
    float temperature_min;                                              /**< chip min operating temperature */
    float temperature_max;                                              /**< chip max operating temperature */
    float driver_version;                                               /**< driver version */
} mcp794xx_info_t;

 /**
 * @}
 */

 /**
* @defgroup mcp794xx mcp794xx link driver function
* @brief    mcp794xx link driver modules
* @ingroup  mcp794xx driver
* @{
*/

/**
 * @brief     initialize mcp794xx_handle_t structure
 * @param[in] pHandle points to mcp794xx pHandle structure
 * @param[in] STRUCTURE is mcp794xx_handle_t
 * @note      none
 */
#define DRIVER_MCP794XX_LINK_INIT(pHandle, STRUCTURE)           memset(pHandle, 0, sizeof(STRUCTURE))

/**
 * @brief     link i2c_init function
 * @param[in] pHandle points to mcp794xx pHandle structure
 * @param[in] FUC points to a i2c_init function address
 * @note      none
 */
#define DRIVER_MCP794XX_LINK_I2C_INIT(pHandle, FUC)              (pHandle)->i2c_init = FUC


/**
 * @brief     link i2c_deinit function
 * @param[in] pHandle points to mcp794xx pHandle structure
 * @param[in] FUC points to a i2c_deinit function address
 * @note      none
 */
#define DRIVER_MCP794XX_LINK_I2C_DEINIT(pHandle, FUC)            (pHandle)->i2c_deinit = FUC


/**
 * @brief     link i2c_write function
 * @param[in] pHandle points to mcp794xx pHandle structure
 * @param[in] FUC points to a i2c_write function address
 * @note      none
 */
#define DRIVER_MCP794XX_LINK_I2C_WRITE(pHandle, FUC)             (pHandle)->i2c_write = FUC


/**
 * @brief     link i2c_read function
 * @param[in] pHandle points to mcp794xx pHandle structure
 * @param[in] FUC points to a i2c_read function address
 * @note      none
 */
#define DRIVER_MCP794XX_LINK_I2C_READ(pHandle, FUC)              (pHandle)->i2c_read = FUC

/**
 * @brief     link delay_ms function
 * @param[in] pHandle points to mcp794xx pHandle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_MCP794XX_LINK_DELAY_MS(pHandle, FUC)             (pHandle)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] pHandle points to mcp794xx pHandle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_MCP794XX_LINK_DEBUG_PRINT(pHandle, FUC)          (pHandle)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] pHandle points to mcp794xx pHandle structure
 * @param[in] FUC points to a receive_callback function address
 * @note      none
 */
#define DRIVER_MCP794XX_LINK_RECEIVE_CALLBACK(pHandle, FUC)     (pHandle)->receive_callback = FUC

/**
 * @}
 */

 /**
* @defgroup mcp794xx_base_driver mcp794xx base driver function
* @brief    mcp794xx base driver modules
* @ingroup  mcp794xx_driver
* @{
*/

/**
 * @brief      get chip's information
 * @param[out] pInfo points to mcp794xx info structure
 * @return  status code
 *             - 0 success
 *             - 2 pHandle is NULL
 * @note       none
 */
uint8_t mcp794xx_info(mcp794xx_info_t *pInfo);

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
uint8_t mcp794xx_init(mcp794xx_handle_t *const pHandle);

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
uint8_t mcp794xx_deinit(mcp794xx_handle_t *const pHandle);

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
uint8_t mcp794xx_irq_pHandler(mcp794xx_handle_t *const pHandle);

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
uint8_t mcp794xx_set_addr_pin(mcp794xx_handle_t *const pHandle, mcp794xx_i2c_addr_t *pI2c_address);

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
uint8_t mcp794xx_set_variant(mcp794xx_handle_t *const pHandle, mcp794xx_variant_t variant);

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
uint8_t mcp794xx_get_variant(mcp794xx_handle_t *const pHandle, mcp794xx_variant_t *pVariant);

/**
 * @brief     This function set the time
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] pTime point to the date and time struct object
 * @return    status code
 *            - 0 success
 *            - 1 set time failed
 *            - 2 handle or time is NULL
 *            - 3 handle is not initialized
 *            - 4 time is invalid
 * @note      none
 */
uint8_t mcp794xx_set_time(mcp794xx_handle_t *const pHandle, mcp794xx_time_t *pTime);

/**
 * @brief      This function get the current time and date
 * @param[in]  pHandle points to a mcp794xx handle structure
 * @param[out] pTime points to a time struct object
 * @return     status code
 *             - 0 success
 *             - 1 set time failed
 *             - 2 handle or time is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp794xx_get_time(mcp794xx_handle_t *const pHandle, mcp794xx_time_t *pTime);

/**
 * @brief This function set the time format 12hr or 24hr
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] format hour format
 * @return status code
 *          - 0 success
 *          - 1 failed to set time format
 * @note        none
 */
uint8_t mcp794xx_set_hour_format(mcp794xx_handle_t *const pHandle, mcp794xx_hour_format_t format);

/**
 * @brief This function get the time format 12hr or 24hr
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[out] pFormat format point to hour format (12hr/24hr)
 * @return status code
 *          - 0 success
 *          - 1 failed to set time format
 * @note        none
 */
uint8_t mcp794xx_get_hour_format(mcp794xx_handle_t *const pHandle, mcp794xx_hour_format_t *pFormat);

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
uint8_t mcp794xx_set_am_pm(mcp794xx_handle_t *const pHandle, mcp794xx_am_pm_indicator_t am_pm);

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
uint8_t mcp794xx_get_am_pm(mcp794xx_handle_t *const pHandle, mcp794xx_am_pm_indicator_t *pAm_pm);

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
uint8_t mcp794xx_set_osc_status(mcp794xx_handle_t *const pHandle, mcp794xx_osc_status_t status);

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
uint8_t mcp794xx_get_osc_status(mcp794xx_handle_t *const pHandle, mcp794xx_osc_status_t *pStatus);

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
uint8_t mcp794xx_clr_pwr_fail_status(mcp794xx_handle_t *const pHandle, mcp794xx_pwr_fail_status_t status);

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
uint8_t mcp794xx_get_pwr_fail_status(mcp794xx_handle_t *const pHandle, mcp794xx_pwr_fail_status_t *pStatus);

/**
 * @brief This function get the power fail time stamp
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] powerMode is the power fail mode (power-up/power-down)
 * @param[out] pTime point to the time structure object
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_get_pwr_fail_time_stamp(mcp794xx_handle_t *const pHandle, mcp794xx_pwr_fail_type_t powerMode, mcp794xx_time_t *pTime);

/**
 * @brief This function get the power fail time stamp
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] powerMode is the power fail mode (power-up/power-down)
 * @param[out] pTime point to the time structure object
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_get_leap_year_status(mcp794xx_handle_t *const pHandle, mcp794xx_leap_year_status_t *pLeapYearStatus);

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
uint8_t mcp794xx_set_alarm_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t status);

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
uint8_t mcp794xx_get_alarm_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t *pStatus);

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
uint8_t mcp794xx_set_alarm_int_output_polarity(mcp794xx_handle_t *const pHandle, mcp794xx_int_output_polarity_t polarity);

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
uint8_t mcp794xx_get_alarm_int_output_polarity(mcp794xx_handle_t *const pHandle, mcp794xx_int_output_polarity_t *pPolarity);

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
uint8_t mcp794xx_set_alarm(mcp794xx_handle_t *const pHandle, mcp794xx_alarm_t alarm, mcp794xx_alarm_mask_t mask, mcp794xx_time_t *pTime);

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
uint8_t mcp794xx_get_alarm(mcp794xx_handle_t *const pHandle, mcp794xx_alarm_t alarm, mcp794xx_alarm_mask_t *pMask, mcp794xx_time_t *pTime);

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
uint8_t mcp794xx_get_alarm_interrupt_flag(mcp794xx_handle_t *const pHandle, mcp794xx_alarm_t alarm, mcp794xx_alarm_int_flag_t flag);

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
uint8_t mcp794xx_clr_alarm_interrupt_flag(mcp794xx_handle_t *const pHandle, mcp794xx_alarm_t alarm);

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
uint8_t mcp794xx_set_mfp_logic_level(mcp794xx_handle_t *const pHandle, mcp794xx_mfp_logic_level_t logicLevel);

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
uint8_t mcp794xx_get_mfp_logic_level(mcp794xx_handle_t *const pHandle, mcp794xx_mfp_logic_level_t *pLogicLevel);

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
uint8_t mcp94xx_set_sqr_wave_freq(mcp794xx_handle_t *const pHandle, mcp94xx_sqr_wave_freq_t freq);

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
uint8_t mcp94xx_get_sqr_wave_freq(mcp794xx_handle_t *const pHandle, mcp94xx_sqr_wave_freq_t *pFreq);

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
uint8_t mcp794xx_set_sqr_wave_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t status);

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
uint8_t mcp794xx_get_sqr_wave_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t *pStatus);

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
uint8_t mcp794xx_set_trim_val(mcp794xx_handle_t *const pHandle, mcp794xx_trim_sign_t sign, mcp794xx_trim_val_t value);

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
uint8_t mcp794xx_get_trim_val(mcp794xx_handle_t *const pHandle, mcp794xx_trim_sign_t *pSign, mcp794xx_trim_val_t *pValue);

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
uint8_t mcp794xx_set_coarse_trim_mode_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t status);

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
uint8_t mcp794xx_get_coarse_trim_mode_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t *pStatus);

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
uint8_t mcp794xx_set_start_osc_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t status);

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
uint8_t mcp794xx_get_start_os_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t *pStatus);

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
uint8_t  mcp794xx_set_ext_batt_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t status);

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
uint8_t  mcp794xx_get_ext_batt_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t *pStatus);

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
uint8_t mcp794xx_set_ext_osc_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t status);

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
uint8_t mcp794xx_get_ext_osc_enable_status(mcp794xx_handle_t *const pHandle, mcp794xx_bool_t *pStatus);

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
uint8_t mcp794xx_get_serial_number(mcp794xx_handle_t *const pHandle, uint32_t *pUID);

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
uint8_t mcp794xx_eeprom_write_byte(mcp794xx_handle_t *const pHandle, uint16_t u16Addr, uint8_t *pBuf, uint16_t u16Length);

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
uint8_t mcp794xx_eeprom_read_byte(mcp794xx_handle_t *const pHandle, uint16_t u16Addr, uint8_t *pBuf, uint16_t u16Length);

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

uint8_t mcp794xx_eeprom_put_byte(mcp794xx_handle_t *const pHandle, uint8_t u8Addr, uint32_t *pBuf, uint8_t u8Length);

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

uint8_t mcp794xx_eeprom_get_byte(mcp794xx_handle_t *const pHandle, uint8_t u8Addr, uint32_t *pBuf, uint8_t u8Length);

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
uint8_t mcp794xx_eeprom_erase_page(mcp794xx_handle_t *const pHandle, uint8_t u8PageNumber);

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
uint8_t mcp794xx_eeprom_erase_sector(mcp794xx_handle_t *const pHandle, uint8_t *pSector_addr);

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
uint8_t mcp794xx_eeprom_erase_chip(mcp794xx_handle_t *const pHandle);


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
uint8_t mcp794xx_eeprom_set_bp_status(mcp794xx_handle_t *const pHandle, mcp794xx_eeprom_block_protect_stat_t status);

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
uint8_t mcp794xx_eeprom_get_bp_status(mcp794xx_handle_t *const pHandle, mcp794xx_eeprom_block_protect_stat_t *pStatus);

/**
 * @brief This function runs basic checks before writing
 * @param[in] pHandle points to mcp794xx handle structure
 * @param[in] u8Address is the address to write
 * @return status code
 *          - 0 success
 *          - 1 failed
 * @note    none
 */
uint8_t mcp794xx_eeprom_check_bp_before_write(mcp794xx_handle_t *const pHandle, uint8_t u8Address);


/**
 * @brief This function validate the address to write or read
 * @param[in] *pHandle point to the handle structure
 * @param[in] u16Address is the address to validate
 * @return status code
 *          - 0 success (validated)
 *          - 1 failed validate
 * @note    none
 */
uint8_t mcp794xxx_eeprom_validate_address(mcp794xx_handle_t *const pHandle);

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
uint8_t mcp794xx_eeprom_validate_page_boundary(mcp794xx_handle_t *const pHandle, uint8_t u8Address, uint8_t u8Length);

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
uint8_t mcp794xxx_eeprom_validate_page(mcp794xx_handle_t *const pHandle, uint8_t u8Page, uint8_t *pStartAddr);

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
uint8_t mcp794xx_eeprom_get_legth(mcp794xx_handle_t *const pHandle);


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
uint8_t mcp794xx_set_reg(mcp794xx_handle_t *const pHandle, uint8_t u8Reg, uint8_t *buf, size_t u8Len);

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
uint8_t mcp794xx_get_reg(mcp794xx_handle_t *const pHandle, uint8_t u8Reg, uint8_t *buf, size_t u8Len);

/**
 * @}
 */

/**
 * @}
 */

#endif // MCP794XX_DRIVER_H_INCLUDED
