/*__________________________________________________________________________________
|	     Chave Digital Tecnologia Eletronica Ltda. 
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
|       Arquivo            :  MonitoraRedeBat.h
|       Data criação       :  Funções para monitoração da bateria e da rede elétrica
|       Revisões		       :  1.0
| 
| __________________________________________________________________________________
*/

#ifndef _MONITORA_REDE_BAT_H_
#define	_MONITORA_REDE_BAT_H_


void MONITORA_REDE_BAT_init(void);
void MONITORA_REDE_BAT_tick(void);
void MONITORA_REDE_BAT_entry(void);

/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/
#endif // _FILE_NAME_H