/*__________________________________________________________________________________
|
|     	Chave Digital Tecnologia Eletronica Ltda. 
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
|       Data cria��o       :  15/02/2010
|       Revis�es		       :  1.0 - Cria��o da biblioteca HAL
|
| __________________________________________________________________________________
*/

#ifndef _UART_H_
#define	_UART_H_

#ifndef BUS_CLK
#define BUS_CLK                               3558139
#endif


void init_sci1(unsigned int baudrate,void(*rxd)(unsigned char a),
                                     unsigned char*(*txd)(void),
                                     void(*enable)(unsigned char state));
                                     
void isc1_sendFirstByte(unsigned char *pData);

void init_sci2(unsigned int baudrate,void(*rxd)(unsigned char a),
                                     unsigned char*(*txd)(void),
                                     void(*enable)(unsigned char state));
                                     
void isc2_sendFirstByte(unsigned char *pData);                                                                          
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/
#endif // _UART_H_