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
|       Arquivo            :  SysTick_targetWrapper.h
|       Data criação       :  07/05/2010
|       Revisões		       :  1.0
|
| __________________________________________________________________________________
*/

#ifndef _SYSTICK_TARGET_WRAPPER_H_
#define	_SYSTICK_TARGET_WRAPPER_H_
/***********************************************************************************
*       Constantes do programa
***********************************************************************************/
#define MODULO_CONTAGEM                                 190//91

/***********************************************************************************
*       Definição para os portes
***********************************************************************************/
#define AC32_PORT

/***********************************************************************************
*       Porte para uso em conjunto com o AC32
***********************************************************************************/
#ifdef AC32_PORT

#include <MC9S08AC32.H>
#define INIT_TIMER()                          { TPM1MODL = (unsigned char)(MODULO_CONTAGEM&0xff);\
                                                TPM1MODH = (unsigned char)(MODULO_CONTAGEM>>8);\
                                                TPM1SC  = 0b01001111;}
                                                
#define CLEAR_INT_FLAG()                        TPM1SC_TOF = 0;//  = (TPM2SC & 0x7F)

#endif
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/
#endif // _SYSTICK_TARGET_WRAPPER_H_