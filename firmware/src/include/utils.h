/* 
 * File:   utils.h
 * Author: morgan
 *
 * Created on April 8, 2017, 4:14 PM
 */

#ifndef UTILS_H
#define	UTILS_H

#define SIN(x) sin(x)
#define COS(x) cos(x)
#define PI 3.141
#define TWO_PI 6.282


unsigned int irbit2(unsigned int iseed);
unsigned int quick_rand(unsigned int seed);


// given points and offset, see if two ranges are overlapping
unsigned char check_range_overlap(unsigned char p1, unsigned char o1,
                                 unsigned char p2, unsigned char o2);

unsigned char check_box_collision(unsigned char x1, unsigned char y1,
                                  unsigned char w1, unsigned char h1,
                                  unsigned char x2, unsigned char y2,
                                  unsigned char w2, unsigned char h2);

void equilateral_polygon_points(short point_arr[][2], 
                                float radius, 
                                unsigned int n_sides,
                                float rotate_rads);
void scale_points(short point_arr[][2],
                  unsigned int n_points,
                  float scale);

unsigned char distance_between_coords(unsigned char x1, unsigned char y1,
                                      unsigned char x2, unsigned y2);

void rotate_points(short point_arr[][2],
                   unsigned int n_points,
                   float rotate_rads);


void path_between_points(unsigned char *x0, unsigned char *y0,
                         unsigned char x1, unsigned char y1);

void badge_itoa(int value, unsigned char buffer[]);
#endif	/* UTILS_H */

