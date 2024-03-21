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
 * File:   mcp794xx_driver_interface.h
 * Author: Cedric Akilimali
 *
 * Created on May 06, 2023, 2:34 PM
 */

#ifndef MCP794XX_DRIVER_INTERFACE_H_INCLUDED
#define MCP794XX_DRIVER_INTERFACE_H_INCLUDED

#include "mcp794xx_driver.h"

static mcp794xx_handle_t mcp794xx_handler;

extern void serial_print(const char *pString, uint8_t u8Length);
extern uint8_t i2c_write(uint8_t addr, uint8_t *buf, uint16_t len);
extern uint8_t i2c_read(uint8_t addr, uint8_t *buf, uint16_t len);

/**
 * @brief  interface i2c bus init
 * @return status code
 *         - 0 success
 *         - 1 i2c init failed
 * @note   none
 */
uint8_t mcp794xx_interface_i2c_init(void);

/**
 * @brief interface i2c bus deinit
 * @return status code
 *          - 0 success
 *          - 1 i2c deinit fail
 */
uint8_t mcp794xx_interface_i2c_deinit(void);

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
uint8_t mcp794xx_interface_i2c_read(uint8_t u8Addr, uint8_t *pBuf, uint8_t u8Length);

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
uint8_t mcp794xx_interface_i2c_write(uint8_t u8Addr, uint8_t *pBuf, uint8_t u8Length);

/**
 * @brief     interface delay ms
 * @param[in] u32Ms is the time in milliseconds
 * @note      none
 */
void mcp794xx_interface_delay_ms(uint32_t u32Ms);

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void mcp794xx_interface_debug_print(const char *const fmt, ...);

/**
 * @brief     interface interrupt request callback
 * @param[in] u8Type is the interrupt type
 * @note      none
 */
void mcp794xx_interface_irq_callback(uint8_t u8Type);


#endif // MCP794XX_DRIVER_INTERFACE_H_INCLUDED
