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
|       Este código é propriedade da Chave Digital e não pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Chave Digital ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Arquivo            :  mostra_falhas.c
|       Descrição          :  funções do estado mostra falhas
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  17/03/2011
|
|       Revisões           :  1.0
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*		Includes
***********************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "mostra_falhas.h"
#include "aplicacao.h"
#include "teclado.h"
#include "sis_normal.h"
#include "hd44780.h"
#include "eventosSistema.h"
#include "falhasPendentes.h"
#include "telaFalhasPendentes.h"
#include "ht1381.h"
/***********************************************************************************
*		Enumerações
***********************************************************************************/


/***********************************************************************************
*		Estruturas
***********************************************************************************/


/***********************************************************************************
*		Uniões
***********************************************************************************/


/***********************************************************************************
*		Constantes
***********************************************************************************/


/***********************************************************************************
*		Variaveis locais
***********************************************************************************/


/***********************************************************************************
*		Funções locais
***********************************************************************************/
void escreveFalhaPendente(void);
void escreveRelogio(void); 
/***********************************************************************************
*		Implementação
***********************************************************************************/

/***********************************************************************************
*   Descrição     :   Função principal do estado sistema normal
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void MOSTRA_FALHAS_entry(void){
  static unsigned char counter=6;

  if(!SISNORMAL_counter){
    escreveRelogio();
    if(!--counter){
      counter=6;
      escreveFalhaPendente();
    }
    SISNORMAL_counter = 500;       
  }      
}
/***********************************************************************************
*   Descrição     :   Escreve na primeira linha do display uma das falhas
*                     da lista
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void escreveFalhaPendente(void){
   eDefs descritor;
   static unsigned char ptr;
   unsigned char parametro;
   char bufferLinha[17];
   
   if(!FP_getElementsInQueue())
     return;
   
   if(FP_getNodo((unsigned char)ptr,&descritor,&parametro)){    
     FP_makeString(descritor,parametro,bufferLinha);
     HD44780_writeTwoLines(bufferLinha,NULL); 
   }
  
   ptr = (unsigned char)((ptr+1) % FP_getElementsInQueue());     
}
/***********************************************************************************
*   Descrição     :   Mostra o relógio no LCD durante o estado de falha
*                     pendente
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void escreveRelogio(void){
  unsigned char hora,minuto,segundo,dia,mes,ano;
  static unsigned char cicles=0;
  char bufferLinha[17];
  
  if(cicles<10){     
    HT1381_leRelogio(&hora,&minuto,&segundo);        
    (void)sprintf(bufferLinha,"    %02d:%02d:%02d    ",hora,minuto,segundo);
  }
  else
    if(cicles<13){
      HT1381_leCalendario(&dia,&mes,&ano);
      (void)sprintf(bufferLinha,"   %02d/%02d/%02d     ",dia,mes,ano);    
    }
    else{                      //1234567890123456
      (void)sprintf(bufferLinha,"FALHA NO SISTEMA");
    }
    
 cicles++;
 if(cicles>16)
   cicles=0;  
    
  HD44780_writeTwoLines(NULL,bufferLinha);   
}
/***********************************************************************************
*   Descrição     :   Função que processa os sinais e retorna um estado
*                     para a máquina principal
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
eAplicacaoStates MOSTRA_FALHAS_process(void){
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
    case TeclaLeft               :
    case TeclaRigth              :
    case TeclaUP                 :
    case TeclaDown               : TELA_FALHAS_PENDENTES_entry();
                                   HD44780_writeTwoLines("  Pesquisando   ","   na FLASH     ");
                                   break;
  }  
  
  /*  
  if(EVSY_getNumberOfAlarms())
    return ePreAlarme;
  
  if(EVSY_getNumberOfDefeitos())
    return eFalhas;
  */  
  
  if(!FP_getElementsInQueue())
    return eSistemaNormal;   
  
  return eMostraInfoFalhas;
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/