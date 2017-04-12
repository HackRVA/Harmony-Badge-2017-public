#include "buttons.h"

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
