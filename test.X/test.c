/*
 * File:   test.c
 * Author: Lenovo
 *
 * Created on March 27, 2025, 8:55 PM
 */




#include <xc.h>
#include "user.h"

unsigned long led = 0x55555555;

void main(void) 
{
    MCUCR |= 0x80;
    
    LED0 = led & 0x000000ff;
    LED1 = (led >> 8) & 0x000000ff;
    LED2 = (led >> 16) & 0x000000ff;
    LED3 = (led >> 24) & 0x000000ff;
    
    while(1);

    return;
}
