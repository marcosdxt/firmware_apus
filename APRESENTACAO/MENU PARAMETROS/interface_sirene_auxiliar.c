/*__________________________________________________________________________________
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
|       Este código é propriedade da Chave Digital e não pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Chave Digital ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Arquivo            :  interface_sirene_auxiliar.c
|       Descrição          :  função para configurar a presença do cartão de
|                             sirene auxiliar
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  19/03/2011
|
|       Revisões           :  1.0 - refatoração
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*		Includes
***********************************************************************************/
#include <MC9S08AC128.H>
#include <stdio.h>
#include <stdlib.h>
#include "teclado.h"
#include "beep.h"
#include "systick.h"
#include "exthw.h"
#include "parametros.h"
#include "hd44780.h"
#include "interface_sirene_auxiliar.h"
/***********************************************************************************
*		Definições com constantes utilizadas no programa
***********************************************************************************/ 
//-------------------------------------------------------------------------------------
//     Mensagens para a tela de configuração do módulo de 
//-------------------------------------------------------------------------------------
const char mensagemReles[1][3][17] = { 
                                       {  
                                         {" Expans. sirene"}, 
                                         {" [ ] Instalado "}, 
                                         {" Erro: 02      "}                                          
                                       }                                  
                                     };  
/***********************************************************************************
*		Implementação
***********************************************************************************/

/***********************************************************************************
*     Descrição   :   Menu para configuração do módulo de relés auxiliar
*     Parametros  :   nenhum
*     Retorno     :   nenhum
***********************************************************************************/
void INTERFACE_SIRENE_AUXILIAR_entry(void){
  unsigned int timeOut=1000;
  unsigned char loop=0xFF,borda,atualiza=1,flagHabilitacao,salvar=0x00;  
  eTeclas tecla;

  HD44780_writeTwoLines((char*)mensagemReles[0][0],(char*)mensagemReles[0][1]);
  
  flagHabilitacao = PARAMETROS_leConfigExpansaoSirenes();          
  
  do{
  
    tecla = TECLADO_getTecla(&borda);
    if(tecla && !borda)
      switch(tecla){         
        case TeclaConfirma: (flagHabilitacao)?(flagHabilitacao=0):(flagHabilitacao=0xFF);
                            atualiza = 1;
                            timeOut = 1000;
                            break;        
        case TeclaCancelar: loop = 0x00;
                            salvar = 1;
                            atualiza = 1;                            
                            break;               
      }
                    
      //Dá um tempo...    
      SYSTICK_delay(50);      
    
      if(atualiza){
        atualiza = 0x00;
        
        while(!EXTHW_waitSecurity(&semaforoBus));
        PTADD = 0xff;                 
        HD44780_posicionaTexto(2,1);
        if(flagHabilitacao)
          HD44780_writeChar('X');
        else
          HD44780_writeChar(' ');
        EXTHW_signal(&semaforoBus);                      
      }
    
  }
  while(loop && timeOut--);
         
  //Flag de salvar o parametro       
  if(salvar){
    salvar = 0x00;       
    if(!PARAMETROS_gravarConfigExpansaoSirenes(flagHabilitacao)){
      //IU_writeDisplay(1,(char*)mensagemReles[0][2],NULL);
      HD44780_writeTwoLines((char*)mensagemReles[0][2],"                ");      
      SYSTICK_delay(60);            
      BEEP_setSound(1000);
      SYSTICK_delay(500);         
      BEEP_setSound(1200);
      SYSTICK_delay(500);                  
      BEEP_setSound(0);        
      
      HD44780_writeTwoLines("                ","                ");
    }          
  } 
 
}

/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/