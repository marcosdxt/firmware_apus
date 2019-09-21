/*__________________________________________________________________________________
|	Chave Digital Tecnologia Eletronica Ltda. 
|       
|       Blumenau - SC
|       www.chavedigital.com.br
| __________________________________________________________________________________
|
|       This source code was developed by Chave Digital and cannot be copied, in part 
|       or in whole, or used, except when legally licensed by Chave Digital
|       or its distributors.
|
|       Este código é propriedade da Chave Digital e não pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Chave Digital ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Arquivo            :  Teclado_targetWrapper.h
|       Data criação       :  07/05/2010
|       Revisões		       :  1.0
|
| __________________________________________________________________________________
*/

#ifndef _TECLADO_TARGET_WRAPPER_H_
#define	_TECLADO_TARGET_WRAPPER_H_

/***********************************************************************************
*     Definição dos portes
***********************************************************************************/
#define AC32
/***********************************************************************************
*     Implementação das macros para o porte
***********************************************************************************/
#ifdef AC32
  
#include <MC9S08AC128.H>  
#include "BEEP.H"

#define HABILITA_DRIVER_DIR                     PTBDD_PTBDD2
#define HABILITA_LINHA_0_DIR                    PTEDD_PTEDD3
#define HABILITA_LINHA_1_DIR                    PTEDD_PTEDD4

#define HABILITA_DRIVER                         PTBD_PTBD2
#define HABILITA_LINHA_0                        PTED_PTED3
#define HABILITA_LINHA_1                        PTED_PTED4


#define INIT_PINS             PTADD = 0x00;\
                              HABILITA_DRIVER_DIR = 1;\
                              HABILITA_LINHA_0_DIR = 1;\
                              HABILITA_LINHA_1_DIR = 1;\
                              HABILITA_LINHA_0=1;\
                              HABILITA_LINHA_1=1;\
                              HABILITA_DRIVER=1;
                              
#define SET_BUS_INPUT         PTADD = 0x00; 

#define NOP                   asm (nop;)


#define SET_SOUND(X)          BEEP_setSound(X);
#define KILL_SOUND()          BEEP_killSound();

#endif
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/
#endif // _TECLADO_TARGET_WRAPPER_H_