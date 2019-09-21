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
|       Arquivo            :  EEPROM.H
|       Data criação       :
|       Revisões		       :
|
| __________________________________________________________________________________
*/

#ifndef _EEPROM_H_
#define _EEPROM_H_


void EEPROM_init(void);
unsigned char EEPROM_writeBlock(unsigned int address,unsigned char *pData,unsigned int size);
unsigned char EEPROM_readBlock(unsigned int address,unsigned char *pData,unsigned int size);


#endif // _EEPROM_H_