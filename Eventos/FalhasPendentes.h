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
|       Arquivo        :  FalhasPendentes.h
|       Data cria��o   :  16/07/2010
|       Revis�es		   :  1.0.0
|
| __________________________________________________________________________________
*/

#ifndef _FALHAS_PENDENTES_H_
#define _FALHAS_PENDENTES_H_

void FP_init(void);
void FP_insereFalha(eDefs descricao,unsigned char parametro);
void FP_removeFalha(eDefs descricao,unsigned char parametro);
unsigned char FP_getNodo(unsigned char indice,eDefs *descricao,unsigned char *parametro);
void FP_makeString(eDefs descritor,unsigned char parametro,char *linha);
unsigned char FP_getElementsInQueue(void);

#endif