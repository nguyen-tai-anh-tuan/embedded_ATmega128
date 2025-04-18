
/* 
 * File:   User
 * Author: NGUYEN TAI ANH TUAN
 * Comments:    HA_HA_HA
 * Revision history: 27-03-2025
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

////////////////////////////////////////////////////////////////////////////////
// DINH NGHIA DIA CHI BÊN NGOÀI 
#define EXMEM_ADD 0x1100

// DINH NGHIA CÁC CHÂN CS
#define CS0 0
#define CS1 1
#define CS2 2
#define CS3 3
#define CS4 4
#define CS5 5
#define CS6 6
#define CS7 7
#define CS8 8
#define CS9 9
#define CS10 10
#define CS11 11
#define CS12 12
#define CS13 13
#define CS14 14
#define CS15 15
#define CS16 16
#define CS17 17
#define CS18 18
#define CS19 19

// DINH NGHIA EXTERNAL DEVICE (DÙNG 4 NHÓM LED0/1/2/3)
#define LED0 *(unsigned char *) (EXMEM_ADD + CS0)
#define LED1 *(unsigned char *) (EXMEM_ADD + CS1)
#define LED2 *(unsigned char *) (EXMEM_ADD + CS2)
#define LED3 *(unsigned char *) (EXMEM_ADD + CS3)