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
|       Arquivo            :  ExtHW.H
|       Data criação       :  02/07/2010
|       Revisões		       :  0.0.0.1
|
| __________________________________________________________________________________
*/

#ifndef _EXTHW_H_
#define _EXTHW_H_

typedef unsigned char Semaphore;

typedef struct{
  unsigned int curtoCircuito:1;
  unsigned int flagAlarme:1;
  unsigned int terminador:1;  
  unsigned int dummy:5;  
}sLacosFlag;

extern Semaphore semaforoBus;


unsigned char EXTHW_wait(Semaphore *semaphore);
unsigned char EXTHW_waitSecurity(Semaphore *semaphore);
void EXTHW_signal(Semaphore *semaphore);

void EXTHW_init(void);
void EXTHW_amostra(void);
unsigned char EXTHW_writeLeds(unsigned char valor);

#endif                                                               