#include <stdio.h>

struct A
{
  int value;
};

struct B
{
  int value; 
};

struct data
{
  struct A a;
  struct B b;
};

enum which_struct_take 
{
  STRUCT_A,
  STRUCT_B
};

int get_value(struct data data, int type)
{
  int value;

  /* here we should get something like this */
  /* if type == STRUCT_A */
  /* return data.a.value */
  /* if type == STRUCT_B */
  /* return data.b.value */

  return data.GET_TYPE(type).value;
}


int main(void)
{
  struct data d;

  d.a.value = 100;
  d.b.value = 200;

  printf("%d\n", get_value(d, STRUCT_A));
  printf("%d\n", get_value(d, STRUCT_B));
  return 0;
}
