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
|       Este código é propriedade da Chave Digital e não pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Chave Digital ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Produto            : Central Apus Industrial
|       Descrição          : Módulo principal da central industrial Apus
| 
|       Autor              : Marcos Aquino
|       Data criação       : 17/03/2010
|
|       Revisões           : 1.0
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*		Includes
***********************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "MC9S08AC128.H"

#include "HD44780.H"
#include "beep.h"
#include "EXTHW.H"
#include "systick.h"
#include "HT1381.H"

#include "APLICACAO.H"
#include "lacos.h"
#include "eventos.h"
#include "eeprom.h"
#include "eventosSistema.h"
#include "falhasPendentes.h"
#include "apusnet.h"
#include "monitoraRedeBat.h"
#include "monitoraTensao.h"
#include "monitoraTerra.h"
#include "poolingDecoder.h"
/***********************************************************************************
*		Definições com constantes utilizadas no programa
***********************************************************************************/ 
#define STRING_VERSAO         "APUS REMOTA 1.0"


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
unsigned char MAIN_tickCounter=0;

/***********************************************************************************
*		Funções locais
***********************************************************************************/
void appInit(void);
void splashApus(void);
 
/***********************************************************************************
*		Implementação
***********************************************************************************/

/***********************************************************************************
*   Descrição     :   Função principal
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void main(void){
    
  appInit();     
   
  EnableInterrupts;    
  
  splashApus();  
  EEPROM_init();
  EVSY_init();
  FP_init();
  LACOS_init();  
  APUSNET_init();
  MONITORA_REDE_BAT_init(); 
  MONITORA_TENSAO_init();
  MONITORA_TERRA_init();
 
  for(;;){
  
    __RESET_WATCHDOG();      
      
    if(!MAIN_tickCounter){
      MAIN_tickCounter = 10;
      
      APLICACAO_entry();         // Interface do usuário
      LACOS_entry();             // Monitoração analógica dos laços
      MONITORA_REDE_BAT_entry(); // Faz a monitoração da fonte
      MONITORA_TENSAO_entry();   // Monitora o nível de tensão no capacitor de entrada da fonte
      MONITORA_TERRA_entry();    // Monitora o nível do equilíbrio da entrada do aterramento
      POOLINGDECODER_entry();    // Monitoras as filas de novos endereços decodificados
    }    
    
    //Função de entrada do protocolo do ApusTools
    APUSNET_decodePackages();    
    MONITORA_TENSAO_tick();  // Faz a amostagem do nível de tensão no barramento DC           
    MONITORA_TERRA_tick();
  }  
}
/***********************************************************************************
*   Descrição     :   Configura o hardware para rodar a aplicação
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void appInit(void){
  
  EXTHW_init();
  
  SOPT  = 0b00000000;
  
  ICGC1 = 0b11111110;
  ICGC2 = 0b10001000;
  while(!ICGS2_DCOS);
          
  PTADD = 0xFF; 
  HD44780_init(LCD_DISPLAY_8X5 | LCD_2_LINHAS,
               LCD_DISPLAY_LIGADO | LCD_CURSOR_DESLIGADO | LCD_CURSOR_FIXO);

  HD44780_writeChar('\f');    
  
  SYSTICK_init();   
  BEEP_init();      
  HT1381_init();  
}
/***********************************************************************************
*   Descrição       :   Splash da versão da central
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void splashApus(void){
  unsigned char j,i;
    
  HD44780_writeChar('\f');  
  HD44780_writeString("  CHAVE DIGITAL ");
  HD44780_posicionaTexto(0,1);
  HD44780_writeString(STRING_VERSAO);
  
  
  for(j=0;j<3;j++)
    for(i=0;i<20;i++){    
      BEEP_setSound(800+(i*50)); 
      SYSTICK_delay(10);
    }
  
  BEEP_setSound(0);
  SYSTICK_delay(3000);          
  HD44780_writeChar('\f');    
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/
