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
|       Revisões		       :
|
| __________________________________________________________________________________
*/

#ifndef _DEVICES_DECODER_H_
#define	_DEVICES_DECODER_H_


extern unsigned char novaCaptura[4];
//extern unsigned int timeOutCaptura[4];

#define LACO_01                   0
#define LACO_02                   1
#define LACO_03                   2
#define LACO_04                   3


#define NEW_CAPTURE(X)                  (novaCaptura[X])// && !timeOutCaptura[X])
#define ACK_CAPTURE(X)                  (novaCaptura[X] = 0x00)


void DEVICES_init(void);
unsigned char decodificaBitStream(unsigned char canal);


/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/
#endif // _DEVICES_DECODER_H_