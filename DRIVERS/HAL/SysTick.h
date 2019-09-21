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
|       Este c�digo � propriedade da Chave Digital e n�o pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Chave Digital ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Arquivo            :   SysTick.h
|       Data cria��o       :   07/05/2010
|       Revis�es		       :   1.0
|
| __________________________________________________________________________________
*/

#ifndef _SYSTICK_H_
#define	_SYSTICK_H_

extern unsigned int ticksCounter;

#define GET_TICK(X)               (ticksCounter%X)==0

void SYSTICK_init(void);
void SYSTICK_delay(unsigned int delay);



/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/
#endif // _SYSTICK_H_