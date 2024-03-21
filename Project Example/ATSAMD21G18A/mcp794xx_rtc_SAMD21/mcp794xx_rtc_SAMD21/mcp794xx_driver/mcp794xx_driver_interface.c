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
 * File:   mcp794xx_driver_interface.c
 * Author: Cedric Akilimali
 *
 * Created on May 06, 2023, 2:34 PM
 */

 #include "mcp794xx_driver_interface.h"

/**
* @brief  interface i2c bus init
* @return status code
*         - 0 success
*         - 1 i2c init failed
* @note   none
*/
uint8_t mcp794xx_interface_i2c_init(void)
{
    /*call your i2c initialize function here*/
    /*user code begin */

    /*user code end*/
    return 0; /**< success */
}

/**
 * @brief interface i2c bus deinit
 * @return status code
 *          - 0 success
 *          - 1 i2c deinit fail
 */
uint8_t mcp794xx_interface_i2c_deinit(void)
{
    /*call your i2c de-initialize function here*/
    /*user code begin */

    /*user code end*/
    return 0; /**< success */
}

/**
 * @brief      interface i2c bus read
 * @param[in]  u8Addr is the i2c device address 7 bit
 * @param[in]  u8Reg is the i2c register address
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
    if(i2c_read(u8Addr, pBuf, u8Length) !=  0)
    {
	    mcp794xx_interface_debug_print("i2c write failed, check the i2c address or wiring\n");
	    return 1;
    }
    /*user code end*/
    return 0; /**< success */
}

/**
 * @brief     interface i2c bus write
 * @param[in] u8Addr is the i2c device address 7 bit
 * @param[in] u8Reg is the i2c register address
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
    if(i2c_write(u8Addr, pBuf, u8Length) !=  0)
    {
	    mcp794xx_interface_debug_print("i2c write failed, check the i2c address or wiring\n");
	    return 1;
    }
    /*user code end*/
    return 0; /**< success */
}

/**
 * @brief     interface delay ms
 * @param[in] u32Ms is the time in milliseconds
 * @note      none
 */
void mcp794xx_interface_delay_ms(uint32_t u32Ms)
{
    /*call your delay function here*/
    /*user code begin */
    delay_ms(u32Ms);
    /*user code end*/
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void mcp794xx_interface_debug_print(const char *const fmt, ...)
{
    //int buffer_size;
    /*call your call print function here*/
    /*user code begin */
    //mcp794xx_basic_get_debug_print_buffer_size(&buffer_size);           /**< retrieve the buffer size set */
#ifdef MCP794XX_DEBUG_MODE
    volatile char str[256];
    volatile uint8_t len;
    va_list args;

    memset((char *) str, 0, sizeof (char) * 256);
    va_start(args, fmt);
    vsnprintf((char *) str, 256, (char const *) fmt, args);
    va_end(args);

    len = strlen((char *) str);
    //   EUSART1_Write_Text((const char *) str, len);        /**< example of a usart function */
      (void)serial_print((uint8_t *)str, len);                     /**< example of printf function, comment out if used */

    /*user code end*/
#endif
}

/**
 * @brief     interface interrupt request callback
 * @param[in] u8Type is the interrupt type
 * @note      Do not use delay function or blocking code in here, such code could crush your code.
 */
void mcp794xx_interface_irq_callback(mcp794xx_alarm_t u8Type)
{
    /*call your delay function here*/
    /*user code begin */

    switch(u8Type)
    {
        case MCP794XX_ALARM0:
        {
          /* It's highly recommended to keep your code as short as possible in an interrupt callback function */
          /*user code begin */

         /*user code end*/
         break;
        }


        case MCP794XX_ALARM1:
        {
          /* It's highly recommended to keep your code as short as possible in an interrupt callback function */
          /*user code begin */
		  
			led_toggle();
         /*user code end*/
         break;
        }

        default:

        break;

    }
	
    /*user code end*/

}
