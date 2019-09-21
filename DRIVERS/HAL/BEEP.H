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
|       Arquivo            :
|       Data criação       :
|       Revisões		   :
|
| __________________________________________________________________________________
*/

#ifndef _BEEP_H_
#define	_BEEP_H_


void BEEP_init(void);
void BEEP_setSound(unsigned int frequencia);
void BEEP_killSound(void);

/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/
#endif // _BEEP_H_