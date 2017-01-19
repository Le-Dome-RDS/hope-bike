/* 
 * File:   DC24_GlobalVar.h
 * Author: francois
 *
 * Created on 28 novembre 2014, 08:29
 */

#ifndef DC24_GLOBALVAR_H
#define	DC24_GLOBALVAR_H

#ifdef	__cplusplus
extern "C" {
#endif

// Network variable
extern  u8 Tempo500ms;

extern  u8 u8Pedalage;
extern  u8 u8PedalageCpt;
extern  boolean bPedalageLevelPrev;

extern  u8 u8RPM;
extern  u8 u8RPMCpt;
extern  boolean bRPMLevelPrev;


extern  u16 u16TensionBatterie;
extern  u16 u16CourantBatt;

#ifdef	__cplusplus
}
#endif

#endif	/* DC24_GLOBALVAR_H */

