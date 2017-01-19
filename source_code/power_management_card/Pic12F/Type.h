/* 
 * File:   Type.h
 * Author: francois
 *
 * Created on 1 novembre 2014, 19:22
 */

#ifndef TYPE_H
#define	TYPE_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef  unsigned long  u32;
typedef  unsigned int  u16;
typedef  int           s16;
typedef  unsigned char u8;
typedef  char          s8;
typedef  unsigned char boolean;

#define TRUE 1
#define FALSE 0

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

#ifdef	__cplusplus
}
#endif

#endif	/* TYPE_H */

