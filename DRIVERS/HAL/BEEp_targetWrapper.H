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
|       Arquivo            :  beep_targetWrapper.h
|       Data criação       :  07/05/2010
|       Revisões		       :  1.0
|
| __________________________________________________________________________________
*/

#ifndef _BEEP_TARGET_WRAPPER_H_
#define	_BEEP_TARGET_WRAPPER_H_


/***********************************************************************************
*       Definição para seleção do port que será utilizado
***********************************************************************************/
#define AC32_PORT                                                    
/***********************************************************************************
*       Implementação para utilização com o AC32
***********************************************************************************/
#ifdef AC32_PORT

#include <MC9S08AC128.H>

#define BEEP_PIN_INIT                               PTBDD_PTBDD1=1
#define SET_BEEP                                    PTBD_PTBD1=1
#define CLR_BEEP                                    PTBD_PTBD1=0

#define INIT_BEEP_HW                                {  TPM3SC   = 0b00001011;\
                                                       TPM3MOD  = 100;\
                                                       TPM3C1SC = 0b10010100;\
                                                       TPM3C1V  = 50;}

#define SET_CICLE(X)                                TPM3MOD = X; TPM3C1V = X;

#define KILL_SOUND()                                CLR_BEEP; TPM3SC=0b00000011;
#define ON_SOUND()                                  TPM3SC |= 0b00001000;

#endif
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/
#endif // _BEEP_TARGET_WRAPPER_H_