/*__________________________________________________________________________________
|	      Chave Digital Tecnologia Eletronica Ltda. 
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
|       Arquivo            :  MonitoraTerra.h
|       Data criação       :  23/03/2011  
|       Revisões		       :  1.0
|
| __________________________________________________________________________________
*/

#ifndef _MONITORA_TERRA_H_
#define	_MONITORA_TERRA_H_


void MONITORA_TERRA_init(void);
void MONITORA_TERRA_entry(void);
void MONITORA_TERRA_tick(void);
unsigned int calculaMediaAterramento(void);


/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/
#endif // _MONITORA_TERRA_H_