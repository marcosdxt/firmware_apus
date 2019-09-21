/*__________________________________________________________________________________
|
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
|       Este c�digo � propriedade da Chave Digital e n�o pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Chave Digital ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Arquivo            :  Sis_normal.c
|       Descri��o          :  Fun��es do estado normal da m�quina aplica��o
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  17/03/2011
|
|       Revis�es           :  1.0
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*		Includes
***********************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "sis_normal.h"
#include "mc9s08ac128.h"

#include "aplicacao.h"
#include "hd44780.h"
#include "ht1381.h"
#include "beep.h"
#include "exthw.h"
#include "teclado.h"
#include "eventosSistema.h"
#include "falhasPendentes.h"
/***********************************************************************************
*   Enumera��es
***********************************************************************************/
typedef enum{
  eWriteClock,
  eSplashClock,
  eWriteCalendar,
  eSplashCalendar
}eNormalStates;
/***********************************************************************************
*		Constantes
***********************************************************************************/


/***********************************************************************************
*		Variaveis locais
***********************************************************************************/
unsigned int SISNORMAL_counter=0;

/***********************************************************************************
*		Fun��es locais
***********************************************************************************/
void SISNORMAL_splashClock(void);
void SISNORMAL_splashCalendar(void);
 
/***********************************************************************************
*		Implementa��o
***********************************************************************************/

/***********************************************************************************
*   Descri��o     :   Fun��o principal do estado sistema normal
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void SISNORMAL_entry(void){
  static eNormalStates estadoAtual = eWriteClock;
  static unsigned char ciclos=0;
  
  switch(estadoAtual){
    case eWriteClock    :
                          if(!SISNORMAL_counter)
                            estadoAtual = eSplashClock;                             
                          break;
    case eSplashClock   : if(ciclos){
                            estadoAtual = eWriteClock;
                            SISNORMAL_counter = 200; 
                          }
                          else{
                            estadoAtual = eWriteCalendar;
                            ciclos = 40;  
                          } 
                          SISNORMAL_splashClock();   
                          ciclos--;
                          break;                         
    case eWriteCalendar : if(!SISNORMAL_counter)
                            estadoAtual = eSplashCalendar;         
                          break;
    case eSplashCalendar: if(ciclos){
                            estadoAtual = eWriteCalendar;
                            SISNORMAL_counter = 50; 
                          }
                          else{
                            estadoAtual = eWriteClock;
                            ciclos = 40;
                          }
                          SISNORMAL_splashCalendar();
                          ciclos--;    
                          break;
  }       
}
/***********************************************************************************
*   Descri��o     :   Escreve o rel�gio no display
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void SISNORMAL_splashClock(void){
  unsigned char hora,minuto,segundo;
  char bufferLinha1[17],bufferLinha2[17];
  
  HT1381_leRelogio(&hora,&minuto,&segundo);  
  (void)sprintf(bufferLinha1," SISTEMA NORMAL ");
  (void)sprintf(bufferLinha2,"    %02d:%02d:%02d    ",hora,minuto,segundo);          
  HD44780_writeTwoLines(bufferLinha1,bufferLinha2);
}
/***********************************************************************************
*   Descri��o     :   Escreve o calend�rio no display
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void SISNORMAL_splashCalendar(void){
  unsigned char dia,mes,ano;
  char bufferLinha1[17],bufferLinha2[17];
  
  HT1381_leCalendario(&dia,&mes,&ano);  
  (void)sprintf(bufferLinha1," SISTEMA NORMAL ");
  (void)sprintf(bufferLinha2,"    %02d/%02d/%02d    ",dia,mes,ano);
  HD44780_writeTwoLines(bufferLinha1,bufferLinha2);  
}
/***********************************************************************************
*   Descri��o     :   Fun��o que processa os sinais e retorna um estado
*                     para a m�quina principal
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
eAplicacaoStates SISNORMAL_process(void){
  eTeclas tecla;
  unsigned char borda;
  
  tecla = TECLADO_getTecla(&borda);
  switch(tecla){
    case TeclaSilenciarBipInterno:
                                   break;    
    case TeclaParametros         : 
                                   return eMenuParametros;
    case TeclaBloqueios          :
                                   return eConfigBloqueio;
    case TeclaAlarmeGeral        :
                                   break;    
  }
  
  /*
  //Existem novos alarmes na fila
  if(EVSY_getNumberOfAlarms())
    return ePreAlarme;
  
  //Existem novos defeitos na fila
  if(EVSY_getNumberOfDefeitos())
    return eFalhas;
  */
  
  //Existem falhas pendentes na fila  
  if(FP_getElementsInQueue())
    return eMostraInfoFalhas;        
  
  return eSistemaNormal;    
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/