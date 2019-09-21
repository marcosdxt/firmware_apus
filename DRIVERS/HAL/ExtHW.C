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
|       Produto            :  Central Apus CD432
|       Arquivo            :  ExtHW.C
|       Descrição          :  Funções para acesso ao hardware externo
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  13/07/2010
|
|       Revisões           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*		Includes
***********************************************************************************/
#include <MC9S08AC128.H>
#include "ExtHW.H"
/***********************************************************************************
*   Constantes do programa
***********************************************************************************/
#define TAMANHO_BUFFER_LACOS                      10
#define TAMANHO_BUFFER_TECLADO                    10
/***********************************************************************************
*		Definições com constantes utilizadas no programa
***********************************************************************************/ 
#define ENABLE_BUFFER_01_DIR                      PTDDD_PTDDD0
#define ENABLE_BUFFER_02_DIR                      PTDDD_PTDDD1
#define ENABLE_BUFFER_03_DIR                      PTBDD_PTBDD2
#define LATCH_LEDS_DIR                            PTDDD_PTDDD2
#define ENABLE_LINHA_01_DIR                       PTEDD_PTEDD3
#define ENABLE_LINHA_02_DIR                       PTEDD_PTEDD4

#define ENABLE_BUFFER_01                          PTDD_PTDD0
#define ENABLE_BUFFER_02                          PTDD_PTDD1
#define ENABLE_BUFFER_03                          PTBD_PTBD2
#define LATCH_LEDS                                PTDD_PTDD2
#define ENABLE_LINHA_01                           PTED_PTED3
#define ENABLE_LINHA_02                           PTED_PTED4
/***********************************************************************************
*		Variaveis locais
***********************************************************************************/
Semaphore semaforoBus=1;

unsigned char bufferSinaisLacos[2][TAMANHO_BUFFER_LACOS],
              ponteiroAmostraSinais=0,
              bufferTeclado[2][TAMANHO_BUFFER_TECLADO],
              ponteiroAmostraTeclado=0;

/***********************************************************************************
*		Funções locais
***********************************************************************************/

 /***********************************************************************************
*		Implementação
***********************************************************************************/

/***********************************************************************************
*   Descrição     : Inicialização do módulo
*   Parametros    : nenhum
*   Retorno       : nenhum
***********************************************************************************/
void EXTHW_init(void){
  
 ENABLE_BUFFER_01_DIR = 1;
 ENABLE_BUFFER_02_DIR = 1;
 ENABLE_BUFFER_03_DIR = 1;
 LATCH_LEDS_DIR = 1; 
   
   
 ENABLE_BUFFER_01 = 1;
 ENABLE_BUFFER_02 = 1;
 ENABLE_BUFFER_03 = 1;
 
 ENABLE_LINHA_01_DIR = 1;
 ENABLE_LINHA_02_DIR = 1;
 
 ENABLE_LINHA_01 = 1;
 ENABLE_LINHA_02 = 1;
 
 PTADD = 0xFF;
 PTAD = 0x00;
 LATCH_LEDS = 0;
 __asm NOP;
 __asm NOP;
 LATCH_LEDS = 1; 
}
/***********************************************************************************
*   Descrição     :   Implementa a função que verifica se o semáforo
*   Parametros    :   (unsigned char*)referência para o semáforo
*   Retonro       :   (unsigned char) Maior do que zero se houverem recursos
*                     disponiveis
***********************************************************************************/
unsigned char EXTHW_wait(Semaphore *semaphore){
 unsigned char flag=0;

  if(!(*semaphore))
    return 0;
 
  __asm CLI;

  if(*semaphore){    
    flag = 1;  
    *semaphore = 0;
  }
  
  __asm SEI;                                                 
  
  return flag;
}
/***********************************************************************************
*   Descrição     :   Wait que roda dentro da interrupção
*   Parametros    :   (unsigned char*) Ponteiro para o semáforo
*   Retorno       :   (unsigned char) 
***********************************************************************************/
unsigned char EXTHW_waitSecurity(Semaphore *semaphore){
 unsigned char flag=0;

  if(!(*semaphore))
    return 0;
 

  if(*semaphore){    
    flag = 1;  
    *semaphore = 0;
  }
    
  return flag;    
}
/***********************************************************************************
*   Descrição     :   Libera o recurso do semaphoro
*   Parametros    :   (unsigned char*) Referência para o semaforo
*   Retorno       :   Deixa o semaforo normal
***********************************************************************************/
void EXTHW_signal(Semaphore *semaphore){
  
  *semaphore = 1;  
}
/***********************************************************************************
*   Descrição     :   Faz a amostragem dos sinais dos flags de laço e 
*                     bits do teclado
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
#pragma INLINE
void EXTHW_amostra(void){
  static unsigned char tempoAmostraLacos=10,tempoAmostraTeclas=5;
  unsigned char estadoAnteriorBarramento;
  
  //Se o barramento não estiver disponível deixa de amostrar nesse ciclo  
  if(!EXTHW_waitSecurity(&semaforoBus))
    return;
            
  if(!--tempoAmostraLacos){
  
    ENABLE_BUFFER_01_DIR = 1;
    ENABLE_BUFFER_02_DIR = 1;
    
    tempoAmostraLacos = 100;
    estadoAnteriorBarramento = PTADD;
    PTADD = 0x00;
    
    //Lê os bits de incêndio do primeiro driver                             
    ENABLE_BUFFER_01 = 0x00;        
    
    __asm NOP;  _asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP;    
            
    bufferSinaisLacos[0][ponteiroAmostraSinais] = ~PTAD;
    
    __asm NOP;  _asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP;
    
    ENABLE_BUFFER_01 = 0x01;            
    
    //Lê os bits de incêndio do segundo driver
    ENABLE_BUFFER_02 = 0x00; 
    
    __asm NOP;  _asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP;
    
    bufferSinaisLacos[1][ponteiroAmostraSinais] = ~PTAD;
    ENABLE_BUFFER_02 = 0x01;        
    ponteiroAmostraSinais = (unsigned char)(ponteiroAmostraSinais+1) % TAMANHO_BUFFER_LACOS;    
    PTADD = estadoAnteriorBarramento;    
  }     
  
  if(!--tempoAmostraTeclas){
    estadoAnteriorBarramento = PTADD;  
    ENABLE_BUFFER_03_DIR = 1;    
    PTADD = 0x00;
    
    tempoAmostraTeclas = 3;   
    ENABLE_BUFFER_03 = 0x00;
    ENABLE_LINHA_01 = 0; 
    
    __asm NOP;  _asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP;
       
    bufferTeclado[0][ponteiroAmostraTeclado] = ~PTAD;    
    ENABLE_LINHA_01 = 1;
    
    __asm NOP;  _asm NOP; __asm NOP; __asm NOP; __asm NOP;  __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP;
    
    ENABLE_LINHA_02 = 0;

    __asm NOP;  _asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP; __asm NOP;
    
    bufferTeclado[1][ponteiroAmostraTeclado] = ~PTAD;        
    ENABLE_LINHA_02 = 1;        
    ENABLE_BUFFER_03 = 0x01;        
    ponteiroAmostraTeclado = (unsigned char)((ponteiroAmostraTeclado+1)% TAMANHO_BUFFER_TECLADO);
    PTADD = estadoAnteriorBarramento;    
  }
             
  EXTHW_signal(&semaforoBus);//Libera o recurso          
}
/***********************************************************************************
*   Descrição     :   Escreve nos leds do sistema
*   Parametros    :   (unsigned char)Valor para escrever lá
*   Retorno       :   nenhum
***********************************************************************************/
unsigned char EXTHW_writeLeds(unsigned char valor){
  unsigned char flag=0,
                estadoAnterior=0;
  
  if(!EXTHW_waitSecurity(&semaforoBus))
    return 0; 
  
  estadoAnterior = PTADD;
  
  PTADD = 0xFF;
  PTAD = valor;
  
  LATCH_LEDS = 0;
  __asm NOP;
  __asm NOP;
  LATCH_LEDS = 1;    
  
  PTADD = estadoAnterior;  
  
  
  EXTHW_signal(&semaforoBus);  
  
  return 1;
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/








