/* 
 * File:   rgb_led.h
 * Author: morgan
 *
 * Created on March 4, 2017, 6:31 PM
 */

#ifndef RGB_LED_H
#define	RGB_LED_H

#ifdef	__cplusplus
extern "C" {
#endif
    
void led(unsigned char r, unsigned char g, unsigned char b);

void red(unsigned char pwm);

void green(unsigned char pwm);

void blue(unsigned char pwm);

#ifdef	__cplusplus
}
#endif

#endif	/* RGB_LED_H */

