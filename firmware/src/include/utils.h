/* 
 * File:   utils.h
 * Author: morgan
 *
 * Created on April 8, 2017, 4:14 PM
 */

#ifndef UTILS_H
#define	UTILS_H

unsigned int irbit2(unsigned int iseed);
unsigned int quick_rand(unsigned int seed);


// given points and offset, see if two ranges are overlapping
unsigned char check_range_overlap(unsigned char p1, unsigned char o1,
                                 unsigned char p2, unsigned char o2);

unsigned char check_box_collision(unsigned char x1, unsigned char y1,
                                  unsigned char w1, unsigned char h1,
                                  unsigned char x2, unsigned char y2,
                                  unsigned char w2, unsigned char h2);
#endif	/* UTILS_H */

