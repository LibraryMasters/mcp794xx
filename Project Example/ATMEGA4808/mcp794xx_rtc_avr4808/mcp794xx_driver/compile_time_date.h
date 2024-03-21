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
 * File:   compile_time_date.h
 * Author: Cedric Akilimali
 *
 * Created on January 08, 2024, 4:21 PM
 */

#ifndef COMPILE_TIME_TIME_DATE_H_INCLUDED
#define COMPILE_TIME_TIME_DATE_H_INCLUDED

 /**< Parse year */
#define BUILD_YEAR_CH0(time_date) (time_date[20])
#define BUILD_YEAR_CH1(time_date) (time_date[21])
#define BUILD_YEAR_CH2(time_date) (time_date[22])
#define BUILD_YEAR_CH3(time_date) (time_date[23])

/**< parse Month */
#define BUILD_MONTH_IS_JAN(time_date) (time_date[4] == 'J' && time_date[5] == 'a' && time_date[6] == 'n')
#define BUILD_MONTH_IS_FEB(time_date) (time_date[4] == 'F')
#define BUILD_MONTH_IS_MAR(time_date) (time_date[4] == 'M' && time_date[5] == 'a' && time_date[6] == 'r')
#define BUILD_MONTH_IS_APR(time_date) (time_date[4] == 'A' && time_date[5] == 'p')
#define BUILD_MONTH_IS_MAY(time_date) (time_date[4] == 'M' && time_date[5] == 'a' && time_date[6] == 'y')
#define BUILD_MONTH_IS_JUN(time_date) (time_date[4] == 'J' && time_date[5] == 'u' && time_date[6] == 'n')
#define BUILD_MONTH_IS_JUL(time_date) (time_date[4] == 'J' && time_date[5] == 'u' && time_date[6] == 'l')
#define BUILD_MONTH_IS_AUG(time_date) (time_date[4] == 'A' && time_date[5] == 'u')
#define BUILD_MONTH_IS_SEP(time_date) (time_date[4] == 'S')
#define BUILD_MONTH_IS_OCT(time_date) (time_date[4] == 'O')
#define BUILD_MONTH_IS_NOV(time_date) (time_date[4] == 'N')
#define BUILD_MONTH_IS_DEC(time_date) (time_date[4] == 'D')

#define BUILD_MONTH_CH0(time_date) \
    ((BUILD_MONTH_IS_OCT(time_date) || BUILD_MONTH_IS_NOV(time_date) || BUILD_MONTH_IS_DEC(time_date)) ? '1' : '0')

#define BUILD_MONTH_CH1(time_date) \
    ( \
        (BUILD_MONTH_IS_JAN(time_date)) ? '1' : \
        (BUILD_MONTH_IS_FEB(time_date)) ? '2' : \
        (BUILD_MONTH_IS_MAR(time_date)) ? '3' : \
        (BUILD_MONTH_IS_APR(time_date)) ? '4' : \
        (BUILD_MONTH_IS_MAY(time_date)) ? '5' : \
        (BUILD_MONTH_IS_JUN(time_date)) ? '6' : \
        (BUILD_MONTH_IS_JUL(time_date)) ? '7' : \
        (BUILD_MONTH_IS_AUG(time_date)) ? '8' : \
        (BUILD_MONTH_IS_SEP(time_date)) ? '9' : \
        (BUILD_MONTH_IS_OCT(time_date)) ? '0' : \
        (BUILD_MONTH_IS_NOV(time_date)) ? '1' : \
        (BUILD_MONTH_IS_DEC(time_date)) ? '2' : \
        /* error default */    '?' \
    )

/**< Parse date */
#define BUILD_DATE_CH0(time_date) ((time_date[8] >= '0') ? (time_date[8]) : '0')
#define BUILD_DATE_CH1(time_date) (time_date[ 9])

/**< Parse day of the week */
#define BUILD_WEEK_DAY_IS_MON(time_date)     (time_date[0] == 'M')
#define BUILD_WEEK_DAY_IS_TUE(time_date)     (time_date[0] == 'T' && time_date[1] == 'u' && time_date[2] == 'e')
#define BUILD_WEEK_DAY_IS_WED(time_date)     (time_date[0] == 'W')
#define BUILD_WEEK_DAY_IS_THU(time_date)     (time_date[0] == 'T' && time_date[1] == 'h' && time_date[2] == 'u')
#define BUILD_WEEK_DAY_IS_FRI(time_date)     (time_date[0] == 'F')
#define BUILD_WEEK_DAY_IS_SAT(time_date)     (time_date[0] == 'S' && time_date[1] == 'a' && time_date[2] == 't')
#define BUILD_WEEK_DAY_IS_SUN(time_date)     (time_date[0] == 'S' && time_date[1] == 'u' && time_date[2] == 'n')

#define BUILD_WEEK_DAY_CH(time_date) \
    ( \
       (BUILD_WEEK_DAY_IS_SUN(time_date)) ? '1' : \
       (BUILD_WEEK_DAY_IS_MON(time_date)) ? '2' : \
       (BUILD_WEEK_DAY_IS_TUE(time_date)) ? '3' : \
       (BUILD_WEEK_DAY_IS_WED(time_date)) ? '4' : \
       (BUILD_WEEK_DAY_IS_THU(time_date)) ? '5' : \
       (BUILD_WEEK_DAY_IS_FRI(time_date)) ? '6' : \
       (BUILD_WEEK_DAY_IS_SAT(time_date)) ? '7' : \
        /* error default */    '?' \
    )

/**< Parse time */
#define BUILD_HOUR_CH0(time_date) (time_date[11])
#define BUILD_HOUR_CH1(time_date) (time_date[12])

#define BUILD_MIN_CH0(time_date) (time_date[14])
#define BUILD_MIN_CH1(time_date) (time_date[15])

#define BUILD_SEC_CH0(time_date) (time_date[17])
#define BUILD_SEC_CH1(time_date) (time_date[18])

#endif // COMPILE_TIME_TIME_DATE_H_INCLUDED
