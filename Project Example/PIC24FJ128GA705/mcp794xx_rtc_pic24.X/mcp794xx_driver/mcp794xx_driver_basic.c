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
 * File:   mcp794xx_driver_basic.c
 * Author: Cedric Akilimali
 *
 * Created on May 06, 2023, 2:34 PM
 */

 #include "mcp794xx_driver_basic.h"

 /**
 * @brief basic example initialize
 * @param[in] variant is the device type
 * @return status code
 *          - 0 success
 *          - 1 initialize failed
 * @note    none
 */

uint8_t mcp794xx_basic_initialize(mcp794xx_variant_t variant)
{
   /*link function*/
    DRIVER_MCP794XX_LINK_INIT(&mcp794xx_handler, mcp794xx_handle_t);                               /**< Link the  */
    DRIVER_MCP794XX_LINK_I2C_INIT(&mcp794xx_handler, mcp794xx_interface_i2c_init);                 /**< Link the i2c initialize function */
    DRIVER_MCP794XX_LINK_I2C_DEINIT(&mcp794xx_handler, mcp794xx_interface_i2c_deinit);             /**< Link the the i2c de-initialize function */
    DRIVER_MCP794XX_LINK_I2C_WRITE(&mcp794xx_handler, mcp794xx_interface_i2c_write);               /**< Link the i2c  write function */
    DRIVER_MCP794XX_LINK_I2C_READ(&mcp794xx_handler, mcp794xx_interface_i2c_read);                 /**< Link the i2c read function */
    DRIVER_MCP794XX_LINK_DELAY_MS(&mcp794xx_handler,mcp794xx_interface_delay_ms);                  /**< Link delay function */
    DRIVER_MCP794XX_LINK_DEBUG_PRINT(&mcp794xx_handler, mcp794xx_interface_debug_print);           /**< Link the debug print function */
    DRIVER_MCP794XX_LINK_RECEIVE_CALLBACK(&mcp794xx_handler, mcp794xx_interface_irq_callback);     /**< Link the IRQ callback function */

    /* initialize */
    err = mcp794xx_init(&mcp794xx_handler);
    if(err)
    {
       mcp794xx_interface_debug_print("initialize");
        return err; /**< return error code */       /**< failed */
    }

        /* set device variant */
    err = mcp794xx_set_variant(&mcp794xx_handler, variant);
    if(err)
    {
        return err; /**< return error code */   /**< failed */
    }

    /* set i2c address mask */
    err = mcp794xx_set_addr_pin(&mcp794xx_handler);
    if(err)
    {
        return err; /**< return error code */ /**< failed */
    }

    /* enable oscillator */
    err = mcp794xx_set_osc_status(&mcp794xx_handler, MCP794XX_OSC_ENABLED);
    if(err)
    {
        return err; /**< return error code */ /**< failed */
    }

    /* clear alarm 0 interrupt status bit */
    err = mcp794xx_clr_alarm_interrupt_flag(&mcp794xx_handler, MCP794XX_ALARM0);
    if(err)
    {
        return err; /**< return error code */ /**< failed */
    }

    /* clear alarm 1 interrupt status bit */
    err = mcp794xx_clr_alarm_interrupt_flag(&mcp794xx_handler, MCP794XX_ALARM1);
    if(err)
    {
        return err; /**< return error code */ /**< failed */
    }

    /* disable alarms */
    err= mcp794xx_set_alarm_enable_status(&mcp794xx_handler, MCP794XX_ALARM0, MCP794XX_BOOL_FALSE);
    if(err)
    {
      return err; /**< return error code */ /**< failed */
    }

        /* disable alarms */
    err= mcp794xx_set_alarm_enable_status(&mcp794xx_handler, MCP794XX_ALARM1, MCP794XX_BOOL_FALSE);
    if(err)
    {
      return err; /**< return error code */ /**< failed */
    }

    /* set the gpio logic level */
    err = mcp794xx_set_mfp_logic_level(&mcp794xx_handler, MCP794XX_MFP_LOGIC_HIGH);
    if(err)
    {
        return err; /**< return error code */ /**< failed */
    }

    /* disable the square wave frequency output */
    err = mcp794xx_set_sqr_wave_enable_status(&mcp794xx_handler, MCP794XX_BOOL_FALSE);
    if(err)
    {
        return err; /**< return error code */ /**< failed */
    }

    /* disable the trim coarse */
    err = mcp794xx_set_coarse_trim_mode_status(&mcp794xx_handler, MCP794XX_BOOL_FALSE);
    if(err)
    {
        return err; /**< return error code */ /**< failed */
    }

    /* enable the external oscillator pins to use by the 32.78Khz oscillator */
    err = mcp794xx_set_ext_osc_enable_status(&mcp794xx_handler, MCP794XX_BOOL_FALSE);
    if(err)
    {
        return err; /**< return error code */ /**< failed */
    }

    /* start the oscillator */
    err = mcp794xx_set_osc_start_bit(&mcp794xx_handler, MCP794XX_BOOL_TRUE);
    if(err)
    {
        return err; /**< return error code */ /**< failed */
    }

    /* disable eeprom protection */
//    err = mcp794xx_eeprom_set_bp_status(&mcp794xx_handler, MCP794XX_EEPROM_BP00);             //IMPLEMENT THE CORRECT VARIANT THIS FUNCTION CAN RUN ON */
//    if(err)
//    {
//        return err; /**< return error code */ /**< failed */
//    }

    /* define printout buffer size */
    err = mcp794xx_set_debug_print_buffer_size(&mcp794xx_handler, MCP794XX_DEBUG_PRINT_BUFFER_SIZE_64B); /**< set the debug print function buffer to 64 bytes */
    if(err)
    {
        return err; /**< return error code */ /**< failed */
    }

    mcp794xx_interface_delay_ms(10);																	/**< wait 10 ms after initialize complete */

    return 0;   /**< Initialize success */

}

 /**
 * @brief basic implementation of interrupt handler
 * @param[in] alarm is the alarm number (ALARM0/ALARM1)
 * @return status code
 *          - 0 success
 *          - 1 fail to run handler
 */
uint8_t mcp794xx_basic_irq_handler(mcp794xx_alarm_t alarm)
{
    err = mcp794xx_irq_pHandler(&mcp794xx_handler, alarm);
    return err;
}

/**
 * @brief callback function to run in the interrupt service routine
 * @param[in] alarm is the alarm number to service the callback for
 * @param[in] cb point to the interrupt routine function
 * @return status code
 *          - 0 succeed
 *          - 1 failed to run
 * @note none
 */
uint8_t mcp794xx_basic_irq_callBack(mcp794xx_alarm_t alarm, mcp794xx_irq_callback_t cb)
{
	err = cb(alarm);
	return err;
}

/**
 * @brief     This function get the device variant
 * @param[out] pVariant point to the device variant
 * @return  status code
 *            - 0 success
 *            - 1 failed to get device variant
 * @note      none
 */
uint8_t mcp794xx_basic_get_variant(mcp794xx_variant_t *pVariant)
{
  err = mcp794xx_get_variant(&mcp794xx_handler, pVariant);
  return err; /**< return error code */

}

uint8_t mcp794xx_basic_get_debug_print_buffer_size(mcp794xx_degub_print_buffer_size_t *pBuffer_size)
{
   err = mcp794xx_get_debug_print_buffer_size(&mcp794xx_handler,pBuffer_size);
   return err; /**< return error code */   /**< return error code */
}

/**
 * @brief basic example set time and date
 * @param[in] *time point to a time structure
 * @return status code
 *          - 0 success
 *          - 1 set time failed
 * @note    am_pm is don't care when 24hr time format is used
 */
uint8_t mcp794xx_basic_set_time_date(mcp794xx_time_t *pTime)
{
    err = mcp794xx_basic_set_time_format(pTime->time_Format);
    if(err != MCP794XX_DRV_OK)
    	return err; /**< return error code */

    err = mcp794xx_set_time_and_date(&mcp794xx_handler, pTime);  /**< set time and date */
    return err; /**< return error code */
}

/**
 * @brief basic example get current time and date
 * @param[out] *time point to the time structure
 * @return status code
 *          - 0 success
 *          - 1 failed to get time
 * @note    am_pm is don't care when 24hr time format is used
 */
uint8_t mcp794xx_basic_get_current_time_date(mcp794xx_time_t *pTime)
{
   err = mcp794xx_get_time_and_date(&mcp794xx_handler, pTime);
   return err; /**< return error code */
}

/**
 * @brief This function is a basic implementation to set the time format 12hr or 24hr
 * @param[in] format hour format
 * @return status code
 *          - 0 success
 *          - 1 failed to set time format
 * @note        none
 */
uint8_t mcp794xx_basic_set_time_format(mcp794xx_time_format_t format)
{
    err = mcp794xx_set_hour_format(&mcp794xx_handler, MCP794XX_RTC_HOUR_REG, format);  /**< set the time format for real time clock register */
    if(err != MCP794XX_DRV_OK){
      return err; /**< return error code */
    }

    err = mcp794xx_set_hour_format(&mcp794xx_handler, MCP794XX_PWR_DWN_HOUR_REG, format); /**< set time format for power down time stamp */
    if(err != MCP794XX_DRV_OK){
      return err; /**< return error code */
    }

    err = mcp794xx_set_hour_format(&mcp794xx_handler, MCP794XX_PWR_UP_HOUR_REG, format); /**< set time format for power up time stamp */
    if(err != MCP794XX_DRV_OK){
      return err; /**< return error code */
    }

	return err; /**< return error code */
}

/**
 * @brief get time format 12hr or 24hr
 * @param[in] *format point to the hour format to set
 * @return status code
 *          - 0 success
 *          - 1 failed to set format
 * @note    none
 */
uint8_t mcp794xx_basic_get_time_format(mcp794xx_time_format_t *pFormat)
{
   err = mcp794xx_get_hour_format(&mcp794xx_handler, MCP794XX_RTC_HOUR_REG, pFormat);
   return err; /**< return error code */
}

/**
 * @brief This function clears the power fail status bit
 * @param[in] pHandle points to a mcp794xx handle structure
 * @return status code
 *          - 0 success
 *          - 1 failed
 *          - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_basic_clr_pwr_fail_status(void)
{
    err = mcp794xx_clr_pwr_fail_status(&mcp794xx_handler);
    return err; /**< return error code */
}

/**
 * @brief This function is a basic implementation to get the power fail status bit
 * @param[out] pStatus point to the status be set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_basic_get_pwr_fail_status(mcp794xx_pwr_fail_status_t *pStatus)
{
    err = mcp794xx_get_pwr_fail_status(&mcp794xx_handler, pStatus);
    return err; /**< return error code */
}

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
uint8_t mcp794xx_basic_get_pwr_fail_time_stamp(mcp794xx_pwr_fail_type_t powerMode, mcp794xx_time_t *pTime)
{
    mcp794xx_set_pwr_fail_time_param(&mcp794xx_handler);      						/**< set indicator parameters for power fail time stamp */
    err = mcp794xx_get_pwr_fail_time_stamp(&mcp794xx_handler, powerMode, pTime);
    return err; /**< return error code */
}

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
uint8_t  mcp794xx_basic_enable_ext_batt_bckup_pwr(void)
{
    err = mcp794xx_set_ext_batt_enable_status(&mcp794xx_handler, MCP794XX_BOOL_TRUE);
    return err; /**< return error code */
}

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
uint8_t  mcp794xx_basic_disable_ext_batt_bckup_pwr(void)
{
    err = mcp794xx_set_ext_batt_enable_status(&mcp794xx_handler, MCP794XX_BOOL_FALSE);
    return err; /**< return error code */
}

/**
* @brief This function is a basic implementation to enable alarm status bit
* @param[in] polarity is the interrupt output polarity to be set
* @param[in] alarm is the  alarm number 1/0 to enable
* @return status code
*          - 0 success
*          - 1 failed
           - 2 handle null
*          - 3 handle is not initialized
*/
uint8_t mcp794xx_basic_enable_alarm(mcp794xx_alarm_t alarm, mcp794xx_int_output_polarity_t polarity)
{
    err = mcp794xx_set_alarm_enable_status(&mcp794xx_handler, alarm, MCP794XX_BOOL_TRUE);
    if(err != MCP794XX_DRV_OK)
    {
    	return err; /**< return error code */
    }

    err = mcp794xx_set_alarm_interrupt_output_polarity(&mcp794xx_handler, alarm, polarity);
    return err; /**< return error code */
}

/**
* @brief This function is a basic implementation to disable alarm status bit
* @param[in] alarm is the  alarm number 1/0 to disable
* @return status code
*          - 0 success
*          - 1 failed
           - 2 handle null
*          - 3 handle is not initialized
*/
uint8_t mcp794xx_basic_disable_alarm(mcp794xx_alarm_t alarm)
{
    err = mcp794xx_set_alarm_enable_status(&mcp794xx_handler, alarm, MCP794XX_BOOL_FALSE);
    return err; /**< return error code */
}

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
uint8_t mcp794xx_basic_set_alarm_time_date(mcp794xx_alarm_t alarm, mcp794xx_alarm_mask_t mask, mcp794xx_time_t *pTime)
{
	if(alarm == MCP794XX_ALARM0){
		err = mcp794xx_set_hour_format(&mcp794xx_handler, MCP794XX_ALM0_HOUR_REG, pTime->time_Format);
		if(err){return err; /**< return error code */}
	}else if(alarm == MCP794XX_ALARM1)
	{
		err = mcp794xx_set_hour_format(&mcp794xx_handler, MCP794XX_ALM1_HOUR_REG, pTime->time_Format);
		if(err != MCP794XX_DRV_OK)
		{
			return err; /**< return error code */
		}
	}
    err = mcp794xx_set_alarm(&mcp794xx_handler, alarm, mask, pTime);
    return err; /**< return error code */
}

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
uint8_t mcp794xx_basic_get_alarm_time_date(mcp794xx_alarm_t alarm, mcp794xx_alarm_mask_t *pMask, mcp794xx_time_t *pTime)
{
    err = mcp794xx_get_alarm(&mcp794xx_handler, alarm, pMask, pTime);
    return err; /**< return error code */
}

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
uint8_t mcp794xx_basic_get_alarm_interrupt_flag(mcp794xx_alarm_t alarm, mcp794xx_alarm_int_flag_t *pFlag)
{
    err = mcp794xx_get_alarm_interrupt_flag(&mcp794xx_handler, alarm, pFlag);
    return err; /**< return error code */
}

/**
 * @brief This function is a basic implementation to clear alarm interrupt flag
 * @param[in] alarm is the alarm type (ALARM0/ALARM1)
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_basic_clr_alarm_interrupt_flag(mcp794xx_alarm_t alarm)
{
    err = mcp794xx_clr_alarm_interrupt_flag(&mcp794xx_handler,alarm);
    return err; /**< return error code */
}

/**
 * @brief     This function is a basic implementation to enable countdown interrupt
 * @return  status code
 *            - 0 success
 *            - 1 failed
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      This function use ALARM1 and overwrite existing data within register
 */
uint8_t mcp794xx_basic_enable_countdown_interrupt(void)
{
    err = mcp794xx_set_countdown_interrupt_enable_status(&mcp794xx_handler, MCP794XX_BOOL_TRUE);
    return err;
}

/**
 * @brief     This function is a basic implementation to disable countdown interrupt
 * @return  status code
 *            - 0 success
 *            - 1 failed
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note
 */
uint8_t mcp794xx_basic_disable_countdown_interrupt(void)
{
    err = mcp794xx_set_countdown_interrupt_enable_status(&mcp794xx_handler, MCP794XX_BOOL_FALSE);
    return err;
}

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
uint8_t mcp794xx_basic_set_countdown_time(uint16_t u16Time_value, mcp794xx_countdwon_time_unit_t time_unit)
{
    err = mcp794xx_set_countdown_interrupt_time(&mcp794xx_handler, u16Time_value, time_unit);
    return err;
}

/**
 * @brief This function is a basic implementation to enable the square wave clock freq. output
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] enable is the enable boolean status to set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_basic_enable_sqr_wave_output(void)
{
    err = mcp794xx_set_sqr_wave_enable_status(&mcp794xx_handler, MCP794XX_BOOL_TRUE);
    return err; /**< return error code */
}

/**
 * @brief This function is a basic implementation to disables the square wave clock freq. output
 * @param[in] pHandle points to a mcp794xx handle structure
 * @param[in] enable is the enable boolean status to set
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 */
uint8_t mcp794xx_basic_disable_sqr_wave_output(void)
{
    err = mcp794xx_set_sqr_wave_enable_status(&mcp794xx_handler, MCP794XX_BOOL_FALSE);
    return err; /**< return error code */
}

/**
 * @brief This function is a basic implementation to set the coarse trim mode enable status
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 * @note Coarse Trim mode results in the MCP7941X applying digital trimming every 64 Hz clock cycle.
 */
uint8_t mcp794xx_basic_enable_coarse_trim(void)
{
    err = mcp794xx_set_coarse_trim_mode_status(&mcp794xx_handler, MCP794XX_BOOL_TRUE);
    return err; /**< return error code */
}

/**
 * @brief This function is a basic implementation to set the coarse trim mode disable status
 * @return status code
 *          - 0 success
 *          - 1 failed
            - 2 handle null
 *          - 3 handle is not initialized
 * @note Coarse Trim mode results in the MCP7941X applying digital trimming every 64 Hz clock cycle.
 */
uint8_t mcp794xx_basic_disable_coarse_trim(void)
{
    err = mcp794xx_set_coarse_trim_mode_status(&mcp794xx_handler, MCP794XX_BOOL_FALSE);
    return err; /**< return error code */
}

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
uint8_t mcp794xx_basic_set_coarse_trim_value(mcp794xx_trim_sign_t sign, mcp794xx_trim_val_t value)
{
    err = mcp794xx_set_trim_val(&mcp794xx_handler,sign, value);
    return err;
}

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
uint8_t mcp94xx_basic_set_sqr_wave_output_freq(mcp94xx_sqr_wave_freq_t freq)
{
    err = mcp94xx_set_sqr_wave_output_freq(&mcp794xx_handler,freq);
    return err; /**< return error code */
}

/**
 * @brief     This function is a basic implementation to read the current time in epoch/unix format
 * @param[in] pTime point to the current time in human readable format
 * @param[out] pEpoch_time point to the epoch time converted
 * @return  status code
 *            - 0 success
 *            - 1 failed
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      This function will fail to execute from January 19, 2038
 */
uint8_t mcp794xx_basic_convert_time_to_epoch_unix_time(mcp794xx_time_t *pTime, uint32_t *pEpoch_time)
{
    err = mcp794xx_read_epoch_unix_time_stamp(&mcp794xx_handler, pTime, (uint32_t *)pEpoch_time);
    return err; /**< return error code */
}


/**
 * @brief  This function is a basic implementation to convert epoch time to human readable time
 * @param[in] epoch_unix_time point to the epoch time converted
 * @param[out] pTime point to time output in human readable time
 * @return  status code
 *            - 0 success
 *            - 1 failed to get
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      This function will fail to execute from January 19, 2038
 */
uint8_t mcp794xx_basic_convert_epoch_to_human_time_format(uint32_t epoch_unix_time, mcp794xx_time_t *pTime)
{
	err = mcp794xx_convert_epoch_unix_time_stamp(&mcp794xx_handler, epoch_unix_time, pTime);
	return err; /**< return error code */
}

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
uint8_t mcp794xx_basic_get_compile_time_date(char *pCompile_time_date, mcp794xx_time_t *pTime)
{
	err = mcp794xx_get_compile_time_date(&mcp794xx_handler, pCompile_time_date, pTime);
	return err;
}

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
uint8_t mcp794xx_basic_sram_write_byte(uint8_t addr, uint8_t *pBuffer, size_t length)
{
	err = mcp794xx_sram_write_byte(&mcp794xx_handler, addr, (uint8_t *)pBuffer, length);
	return err; /**< return error code */
}

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
uint8_t mcp794xx_basic_sram_read_byte(uint8_t addr, uint8_t *pBuffer, size_t length)
{
	err = mcp794xx_sram_read_byte(&mcp794xx_handler, addr, (uint8_t *)pBuffer, length);
	return err; /**< return error code */
}

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
uint8_t mcp794xx_basic_sram_erase_selector(uint8_t start_addr, uint8_t end_addr)
{
	err = mcp794xx_sram_erase_selector(&mcp794xx_handler, start_addr, end_addr);
	return err;
}

/**
* @brief This function  is a basic implementation to wipe the entire sram memory
* @return status code
*          - 0 success
*          - 1 failed to write
*          - 2 pHandle is NULL
*          - 3 pHandle is not initialized
* @note none
*/
uint8_t mcp794xx_basic_sram_wipe(void)
{
	err = mcp794xx_wipe_sram(&mcp794xx_handler);
	return err; /**< return error code */
}

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
uint8_t mcp794xx_basic_uid_read(uint8_t *pUID)
{
	err = mcp794xx_uid_read(&mcp794xx_handler,(uint8_t*)pUID);
	return err; /**< return error code */
}

/**
 * @brief     This function erase eeprom page
 * @param[in] u8Page_number is the eeprom page number to erase
 * @return    status code
 *            - 0 success
 *            - 1 read failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid page (out of boundary)
 * @note      Page erase instruction will erase all bits (FFh) within a the given page.
 */
uint8_t mcp794xx_basic_eeprom_erase_page(uint8_t u8Page_number)
{
	err = mcp794xx_eeprom_erase_page(&mcp794xx_handler, u8Page_number);
	return err;
}

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
uint8_t mcp794xx_basic_eeprom_erase_sector(uint8_t start_addr, uint8_t end_addr)
{
	uint8_t addr_buf[2];

	addr_buf[0] = start_addr;
	addr_buf[1] = end_addr;

	err = mcp794xx_eeprom_erase_sector(&mcp794xx_handler, (uint8_t*)addr_buf);

	return err;
}

/**
 * @brief     This function erase chip stored data
 * @return    status code
 *            - 0 success
 *            - 1 erase failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      chip erase instruction will erase all bits (FFh) in the array.
 */
uint8_t mcp794xx_basic_eeprom_erase_chip(void)
{
	err = mcp794xx_eeprom_erase_chip(&mcp794xx_handler);
	return err;
}

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
uint8_t mcp794xx_basic_eeprom_set_bp_status(mcp794xx_eeprom_block_protect_stat_t status)
{
	err = mcp794xx_eeprom_set_bp_status(&mcp794xx_handler, status);
	return err;
}

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
uint8_t mcp794xx_basic_eeprom_get_bp_status(mcp794xx_eeprom_block_protect_stat_t *pStatus)
{
	err = mcp794xx_eeprom_get_bp_status(&mcp794xx_handler, pStatus);
	return err;
}

/**
 * @brief     This function is a basic implementation to write n number of bytes to eeprom
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
uint8_t mcp794xx_basic_eeprom_write_byte(uint8_t u8Addr, uint8_t *pBuf, size_t length)
{
	err = mcp794xx_eeprom_write_byte(&mcp794xx_handler, u8Addr, (uint8_t *)pBuf, length);
	return err; /**< return error code */
}

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
 * @note      none
 */
uint8_t mcp794xx_basic_eeprom_read_byte(uint8_t u8Addr, uint8_t *pBuf, size_t length)
{
	err = mcp794xx_eeprom_read_byte(&mcp794xx_handler, u8Addr, (uint8_t *)pBuf, length);
	return err; /**< return error code */
}

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

uint8_t mcp794xx_basic_eeprom_put_byte(uint8_t u8Addr, uint32_t *pBuf, uint8_t u8Length)
{
	err = mcp794xx_eeprom_put_byte(&mcp794xx_handler, u8Addr, (uint32_t *)pBuf, u8Length);
	return err; /**< return error code */
}

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

uint8_t mcp794xx_basic_eeprom_get_byte(uint8_t u8Addr, uint32_t *pBuf, uint8_t u8Length)
{
	err = mcp794xx_eeprom_get_byte(&mcp794xx_handler, u8Addr, (uint32_t *)pBuf, u8Length);
	return err; /**< return error code */
}

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
uint8_t mcp794xx_basic_eeprom_update(uint8_t u8Address, uint8_t *pBuffer, uint8_t u8Length)
{
	err = mcp794xx_eeprom_update(&mcp794xx_handler, u8Address, (uint8_t *)pBuffer, u8Length);
	return err;
}

/**
 * @brief basic example write register
 * @param[in] reg is the i2c register address
 * @param[in] *pBuf points to a data buffer to write
 * @param[in] u8Length is the data buffer length
 * @return status code
 *          - 0 success
 *          - 1 failed to write register
 */
uint8_t mcp794xx_basic_write_register(uint8_t u8Reg, uint8_t *pBuf, uint8_t u8Length)
{
    err = mcp794xx_set_reg(&mcp794xx_handler, u8Reg, (uint8_t *)pBuf, u8Length);
    return err; /**< return error code */
}

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
uint8_t mcp794xx_basic_read_reg(uint8_t u8Reg, uint8_t *pBuf, uint8_t u8Length)
{
    err = mcp794xx_get_reg(&mcp794xx_handler, u8Reg, pBuf, u8Length);
    return err; /**< return error code */
}
