#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

//#define FrancoisPCB 
#define  YuriyPCB

#define  IN_Y    29
typedef struct
{
  unsigned  char new_state;
  unsigned  char action;
  uint8_t   eventSeconds;
  uint8_t   eventMinutes;
  uint8_t   eventHours;
  uint8_t   eventDays;
  uint8_t   eventWeekDays;
  uint8_t   eventMonths;
  uint16_t  eventYears;
} input_data_t;
extern input_data_t input_data_Y;