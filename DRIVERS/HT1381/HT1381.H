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
#ifndef _HT1381_H_
#define _HT1381_H_

void HT1381_init(void);

void HT1381_ajustaRelogio(unsigned char hora,unsigned char minuto,unsigned char segundos);
void HT1381_leRelogio(unsigned char *hora,unsigned char *minuto,unsigned char *segundo);

void HT1381_ajustaCalendario(unsigned char dia,unsigned char mes,unsigned char ano);
void HT1381_leCalendario(unsigned char *dia,unsigned char *mes,unsigned char *ano);

#endif