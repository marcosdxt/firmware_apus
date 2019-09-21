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
|       Arquivo            :  tela_tempo_alarme.c
|       Descrição          :  Tela para configuração do parametro de tempo
|                             para a central entrar em alarme geral
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  19/03/2011
|
|       Revisões           :  1.0
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*		Includes
***********************************************************************************/
#include <MC9S08AC128.H>
#include <stdlib.h>
#include <stdio.h>
#include "tela_tempo_alarme.h"
#include "hd44780.h"
#include "parametros.h"
#include "teclado.h"
#include "beep.h"
#include "exthw.h"
#include "systick.h"
#include "tela_tempo_alarme.h"
/*******************************************************************************
*   Descrição       :   Interface para cadastro do tempo para alarme geral
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void TELA_TEMPO_ALARME_entry(void){
  unsigned int timeOut=1000;
  unsigned char loop=0xFF,borda,tempoAlarme,salvar=0,atualiza=1,toogle=0,toogleTime=5;
  eTeclas tecla;
 
  HD44780_writeTwoLines("  TEMPO P/ ALM  ","       segundos ");

  tempoAlarme = PARAMETROS_tempoAlarmeGeral(); 

  do{
  
    //--------------------------------//
    //                                //
    //--------------------------------//    
    tecla = TECLADO_getTecla(&borda);
    if(tecla && !borda){
      switch(tecla){
        case TeclaUP      : tempoAlarme = (unsigned char)((tempoAlarme+1)%241); toogle = 0x00; atualiza = 1; toogleTime = 0;  break;
        case TeclaDown    : (tempoAlarme)?(tempoAlarme--):(tempoAlarme=240) ; toogle = 0x00; atualiza = 1; toogleTime = 0;    break;
        case TeclaConfirma: salvar = 0x01; loop=0x00; break; 
        case TeclaCancelar: loop=0x00; break;
      }
    }     
    
    
    
    if(toogleTime)
      toogleTime--;
    else{
      toogleTime = 5;  
      atualiza = 1;
    }
    
    if(atualiza){
      char buffer[10];
      atualiza = 0x00;      
      
      toogle=~toogle;
      if(toogle){        
        buffer[0] = '0' + ((tempoAlarme/100)%10);
        buffer[1] = '0' + ((tempoAlarme/10)%10);
        buffer[2] = '0' + (tempoAlarme%10);
        buffer[3] = '\0';
      }
      else{
        buffer[0] = ' ';
        buffer[1] = ' ';
        buffer[2] = ' ';
        buffer[3] = '\0';        
      }
      
      while(!EXTHW_waitSecurity(&semaforoBus));
      PTADD = 0xff;                 
      HD44780_posicionaTexto(2,1);     
      HD44780_writeString(buffer);
      EXTHW_signal(&semaforoBus);             
    }
    
    
    SYSTICK_delay(50);            
  }
  while(loop && timeOut--);
    
  if(salvar)
    PARAMETROS_gravaTempoAlarmeGeral(tempoAlarme);       
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/