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
|       Arquivo        :
|       Data criação   :
|       Revisões		   :
|
| __________________________________________________________________________________
*/

#ifndef _FILA_DEVICES_H_
#define _FILA_DEVICES_H_


void FILADEVICES_init(void);
unsigned char FILADEVICES_get(unsigned char loop,unsigned char *device);
void FILADEVICES_main(void);



#endif