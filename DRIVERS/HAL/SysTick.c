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
|       Arquivo            :  SysTick.c
|       Descrição          :  Implementação do timer periódico do sistema
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  07/05/2010
|
|       Revisões           :  1.0
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*		Includes
***********************************************************************************/
#include "SysTick_targetWrapper.h"
#include "SysTick.h"
#include "Beep.h"
#include "exthw.h"
#include "monitoraRedeBat.h"
/***********************************************************************************
*		Definições com constantes utilizadas no programa
***********************************************************************************/ 
//#define MEDINDO_TEMPO
#define PINO_TESTE_DIR                              PTCDD_PTCDD2
#define PINO_TESTE                                  PTCD_PTCD2

/***********************************************************************************
*   Variavéis globais
***********************************************************************************/
unsigned char ligaBeep=0x00;
unsigned int delayCounter=0;
unsigned int ticksCounter=0;

/***********************************************************************************
*   Variaveis externas
***********************************************************************************/
extern unsigned int SISNORMAL_counter,MENU_GENERICO_counter,MENU_GENERICO_timeOut;
extern unsigned char LACOS_fitroTempo[4],MAIN_tickCounter,MONITORA_TENSAO_counter,contatorTerra;

/***********************************************************************************
*		Funções locais
***********************************************************************************/
void oneTickEvent(void);
void tick50ms(void);
void tick100ms(void);
void tick1000ms(void);
void beepTick(void);
/***********************************************************************************
*   Funções externas
***********************************************************************************/
extern void TECLADO_entraFilaTeclado(void); 
extern void EXTHW_amostra(void);
extern void TECLADO_tick(void);
extern void FILADEVICES_main(void);

/***********************************************************************************
*		Implementação
***********************************************************************************/

/**********************************************************************************
*   Descrição       :   Inicialização do módulo
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void SYSTICK_init(void){
        
  TPM1SC   = 0b00001111;  
  TPM1MOD  = 0x0000;    
  
  TPM1C0SC = 0b01010000;   
  TPM1C0V  = TPM1CNT + 100;
  
  #ifdef MEDINDO_TEMPO
    PINO_TESTE_DIR = 1;
  #endif
}
/***********************************************************************************
*   Descrição       :   Função periódica para sincronização do sistema
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/

void interrupt VectorNumber_Vtpm1ch0 rtiTimer(void){  
  static unsigned char tempoLeds=250;
  static unsigned int toogleLeds=0;
  
  #ifdef MEDINDO_TEMPO 
    PINO_TESTE ^= 1;    
  #endif  
  
  TPM1C0SC_CH0F = 0;
  TPM1C0V+= 158;
  
  oneTickEvent();
  tick50ms();
  tick100ms();
  tick1000ms();
  beepTick();
      
  
  CLEAR_INT_FLAG();                               
  
  if(!--tempoLeds){
    toogleLeds = ~toogleLeds;
    tempoLeds = 250;
  }
  /*
  if(toogleLeds)
    EXTHW_writeLeds(APLICACAO_espelhoLeds);
  else
    EXTHW_writeLeds(APLICACAO_espelhoLeds & 0b00000110);     
  */
}
/***********************************************************************************
*   Descrição       :   Evento de 1 tick do timer
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
#pragma INLINE
void oneTickEvent(void){
 
  //Tarefas periódicas de 1ms
 
  //Contador utilizado na máquina de estados do sistema normal
  if(SISNORMAL_counter)
    SISNORMAL_counter--;    
  
  //Contador utilizado para o systick delay counter
  if(delayCounter)
    delayCounter--;  
  
  if(LACOS_fitroTempo[0])
    LACOS_fitroTempo[0]--;
  if(LACOS_fitroTempo[1])
    LACOS_fitroTempo[1]--;
  if(LACOS_fitroTempo[2])
    LACOS_fitroTempo[2]--;
  if(LACOS_fitroTempo[3])
   LACOS_fitroTempo[3]--;
  
  if(MENU_GENERICO_counter)
    MENU_GENERICO_counter--;
  
  if(MENU_GENERICO_timeOut)
    MENU_GENERICO_timeOut--;
  
  if(MAIN_tickCounter)
    MAIN_tickCounter--;  
    
  TECLADO_tick(); //Faz a amostragem das teclas  
  EXTHW_amostra();//Faz a amostragem dos sinais de controle dos laços    
  MONITORA_REDE_BAT_tick(); //Controla o pino do carregador de bateria e amostra os sinais de FB e FR
}
/***********************************************************************************
*   Descrição       :   Tick de 50 milisegundos
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
#pragma INLINE
void tick50ms(void){
  static unsigned char ciclos=0;
  
  if(ciclos){
    ciclos--;
    return;  
  }  
  ciclos = 50;
  //Tarefas periódicas de 50ms
  
  FILADEVICES_main();
  TECLADO_entraFilaTeclado();
}
/***********************************************************************************
*   Descrição       :   Tick de 100 milisegundos
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
#pragma INLINE
void tick100ms(void){
  static unsigned char ciclos=100;
  
  if(ciclos){
    ciclos--;
    return;  
  }  
  ciclos = 100;
  //Tarefas periódicas de 100ms  
   
  
}
/***********************************************************************************
*   Descrição       :   Tick de 1 segundo
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
#pragma INLINE
void tick1000ms(void){
  static unsigned int ciclos=1000;
  
  if(ciclos){
    ciclos--;
    return;  
  }  
  ciclos=1000;
  
  //Tarefas periódicas de 1S  
  if(MONITORA_TENSAO_counter)
    MONITORA_TENSAO_counter--;
  
  if(contatorTerra)
    contatorTerra--;
  
}
/***********************************************************************************
*   Descrição       :   Beep machine
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
#pragma INLINE
void beepTick(void){
  static unsigned char tempoBeep,toogle=0; 
 
  if(ligaBeep){
    tempoBeep--;
    if(tempoBeep==1){
      toogle = ~toogle;  
      if(toogle)
        BEEP_setSound(2500);
      else
        BEEP_setSound(2000);  
    }    
  }  
}
/***********************************************************************************
*   Descrição       :   Loop de atraso para o sistema
*   Parametros      :   (unsigned int) ciclos de espera
*   Retorno         :   nenhum
***********************************************************************************/
void SYSTICK_delay(unsigned int delay){
  
  delayCounter=delay;
  while(delayCounter);  
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/