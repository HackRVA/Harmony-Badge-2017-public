#ifdef SDL_BADGE
#include "sdl_buttons.h"
#else
#include "buttons.h"
#endif


#include "utils.h"
#include <math.h> // for sin/cos, may want to approx instead

#define IB1 1
#define IB2 2
#define IB5 16
#define IB18 131072

#define MASK (IB1+IB2+IB5)

unsigned int irbit2(unsigned int iseed)
{
  if (iseed & IB18){
    iseed = ((iseed ^ MASK) << 1) | IB1;
  }
  else{
    iseed <<= 1;
  }
  return iseed;
}

unsigned int quick_rand(unsigned int seed){
    return irbit2(seed ^ G_entropy_pool);
}



// given points and offset, see if two ranges are overlapping
unsigned char check_range_overlap(unsigned char p1, unsigned char o1,
                                 unsigned char p2, unsigned char o2)
{
    return ((p1+o1) > p2) && (p1 < (p2 + o2));
}


unsigned char check_box_collision(unsigned char x1, unsigned char y1,
                                  unsigned char w1, unsigned char h1,
                                  unsigned char x2, unsigned char y2,
                                  unsigned char w2, unsigned char h2)
{
    unsigned char x_check = check_range_overlap(x1, w1, x2, w2);
    unsigned char y_check = check_range_overlap(y1, h1, y2, h2);
    return (x_check && y_check);
}


void rotate_points_to(short point_arr[][2],
                   unsigned int n_points, 
                   float to_rads)
{
    unsigned int n;
    float delta_rad = 0.0;
    float radius = 0.0;
    
    for(n=0; n < (n_points); n++)
    {
        // Nth vertex
        //rads = (delta_rad * (float)(n)) + rotate_rads;
        //rads = atan(point_arr[n][1]/point_arr[n][0]) + to_rads;
        radius = sqrt(pow(point_arr[n][1], 2) + pow(point_arr[n][0], 2));
        point_arr[n][1] = (radius * SIN(to_rads));
        point_arr[n][0] = (radius * COS(to_rads));
    }
}

void scale_points(short point_arr[][2],
                  unsigned int n_points,
                  float scale){
    unsigned int n;
    for(n=0; n < n_points; n++){
        point_arr[n][0] = (short)((float)point_arr[n][0] * scale);
        point_arr[n][1] = (short)((float)point_arr[n][1] * scale);
    }

}

void rotate_points(short point_arr[][2],
                   unsigned int n_points, 
                   float rotate_rads)
{
    unsigned int n;
    double s = sin(rotate_rads), c = cos(rotate_rads);
    short new_x = 0, new_y = 0;


#define _X  point_arr[n][0]
#define _Y  point_arr[n][1]
    for(n=0; n < (n_points); n++)
    {
        //tmp_x = point_arr[n][0];
        //tmp_y = point_arr[n][1];

        new_x = (_X * c - _Y * s);// + point_arr[n][0];
        new_y = (_X * s + _Y * c);// + point_arr[n][1];

        point_arr[n][0] = new_x;
        point_arr[n][1] = new_y;
    }
}

void equilateral_polygon_points(short point_arr[][2], 
                                float radius, 
                                unsigned int n_sides,
                                float rotate_rads)
{
    unsigned int n;
    float delta_rad = 0.0, rads = 0.0;
   
    if(n_sides < 3){
#ifndef SDL_BADGE
        led(1, 0, 0);
#endif
        return;
    }
    
    delta_rad = TWO_PI/(float)n_sides;
    
    for(n=0; n < (n_sides); n++)
    {
        // Nth vertex
        rads = (delta_rad * (float)(n)) + rotate_rads;

        point_arr[n][1] = (radius * SIN(rads));
        point_arr[n][0] = (radius * COS(rads));
    }  
}