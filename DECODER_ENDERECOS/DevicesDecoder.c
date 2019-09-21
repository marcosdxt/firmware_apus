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
|       Este c�digo � propriedade da Chave Digital e n�o pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Chave Digital ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Produto            :  Central Apus CD432
|       Arquivo            :  DevicesDecoder.c
|       Descri��o          :  Fun��es para decodifica��o dos endere�os recebidos
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  13/07/2010
|
|       Revis�es           :  1.0.0.
|
|
| __________________________________________________________________________________
*/

/***********************************************************************************
*		Includes
***********************************************************************************/
#include "appConfig.h"
#include "DevicesDecoder.h"

//#define MEASURE 
/***********************************************************************************
*		Defini��o dos pinos utilizados como IO
***********************************************************************************/ 
#define PINO_MODULADOR_LACO_01                    PTFDD_PTFDD4
#define PINO_MODULADOR_LACO_02                    PTFDD_PTFDD5
#define PINO_MODULADOR_LACO_03                    PTFDD_PTFDD0  
#define PINO_MODULADOR_LACO_04                    PTFDD_PTFDD1

/***********************************************************************************
*   Defini��o de constantes
***********************************************************************************/
#define MIN_START_SIZE                      15000
#define MAX_START_SIZE                      22000

#define MIN_HIGH_SIZE                       3100 
#define MAX_HIGH_SIZE                       8000 

#define MIN_LOW_SIZE                        550  
#define MAX_LOW_SIZE                        3000 

#define MIN_STOP_SIZE                       8400 
#define MAX_STOP_SIZE                       14000

/***********************************************************************************
*		Enumera��es
***********************************************************************************/


/***********************************************************************************
*		Estruturas
***********************************************************************************/


/***********************************************************************************
*		Uni�es
***********************************************************************************/


/***********************************************************************************
*		Constantes
***********************************************************************************/


/***********************************************************************************
*		Variaveis locais
***********************************************************************************/           
unsigned int  bitStream[4];
              
unsigned char novaCaptura[4] = {0,0,0,0};              
                            
/***********************************************************************************
*		Fun��es locais
***********************************************************************************/ 


/***********************************************************************************
*		Implementa��o
***********************************************************************************/

/***********************************************************************************
*   Descri��o       :   Inicializa��o do m�dulo
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void DEVICES_init(void){
  
  TPM2SC   = 0b00001111;  
  TPM2MOD  = 0x0000;    
}
/***********************************************************************************
*   Descri��o     :   Decodifica os dados recebidos atrav�s do modulador de 2 fios
*   Parametros    :   (nenhum)
*   Retorno       :   (nenhum)
***********************************************************************************/
unsigned char decodificaBitStream(unsigned char canal){
  unsigned char mascara,tmp,paridade,i,p;
  
  mascara = bitStream[canal]>>10;
  
  if(mascara == 0x0A){
    
    paridade = (bitStream[canal]>>1)&0x01;
    tmp = (bitStream[canal]>>2)&0xFF;
    
    p=0;
    for(i=0;i<8;i++)
      if(tmp&(0x01<<i))
        p++;
                            
    if( ((p&0x01)^0x01) == (paridade&0x01) )
      return tmp;                              
  }
  
  return 0;
}
/***********************************************************************************
*   Descri��o       :   Vetor de interrup��o do TPM2
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void interrupt VectorNumber_Vtpm2ch0 rtiTPM0(void){
  static unsigned int larguraAnterior=0,
                      larguraAtual=0,
                      pulsoMedido;
               
  if(!TPM2C0SC_CH0F)
    return;
  
  TPM2C0SC_CH0F=0;                          
    
    
  larguraAtual  = TPM2C0V;         
  //Calcula a largura efetiva do pulso recebido                      
  if(larguraAtual>larguraAnterior)
    pulsoMedido = larguraAtual -  larguraAnterior;
  else
    pulsoMedido = (unsigned int)((unsigned int)(65535 - larguraAnterior) + larguraAtual); 
  
  larguraAnterior = larguraAtual;  
  
  
  if(pulsoMedido>MAX_START_SIZE || pulsoMedido<MIN_LOW_SIZE)
   return;
  
  if(pulsoMedido>MIN_START_SIZE && pulsoMedido<MAX_START_SIZE){    
    bitStream[0] = 0x00;
    return;
  }
    
  //---------------------------------------------------------  
  if(pulsoMedido>MIN_HIGH_SIZE && pulsoMedido<MAX_HIGH_SIZE){
      bitStream[0]|= 0x01;
      bitStream[0]<<=1;            
  } 
  else             
    if(pulsoMedido>MIN_LOW_SIZE && pulsoMedido<MAX_LOW_SIZE)
      bitStream[0]<<=1;
    else
      if(pulsoMedido>MIN_STOP_SIZE && pulsoMedido<MAX_STOP_SIZE)        
        novaCaptura[0] = 0x01;     
  //----------------------------------------------------------
}
/***********************************************************************************
*   Descri��o     :   Tratamento da interrup��o do canal 2 do TPM
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void interrupt VectorNumber_Vtpm2ch1 rtiTMP1(void){
  static unsigned int larguraAnterior=0,
                      larguraAtual=0,
                      pulsoMedido;
               
  if(!TPM2C1SC_CH1F)
    return;
  
  TPM2C1SC_CH1F=0;                          
        
  larguraAtual  = TPM2C1V;         
  //Calcula a largura efetiva do pulso recebido                      
  if(larguraAtual>larguraAnterior)
    pulsoMedido = larguraAtual -  larguraAnterior;
  else
    pulsoMedido = (unsigned int)((unsigned int)(65535 - larguraAnterior) + larguraAtual); 
  
  larguraAnterior = larguraAtual;  
  
  
  if(pulsoMedido>MAX_START_SIZE || pulsoMedido<MIN_LOW_SIZE)
   return;
  
  if(pulsoMedido>MIN_START_SIZE && pulsoMedido<MAX_START_SIZE){    
    bitStream[1] = 0x00;
    return;
  }
    
  //---------------------------------------------------------  
  if(pulsoMedido>MIN_HIGH_SIZE && pulsoMedido<MAX_HIGH_SIZE){
      bitStream[1]|= 0x01;
      bitStream[1]<<=1;            
  } 
  else             
    if(pulsoMedido>MIN_LOW_SIZE && pulsoMedido<MAX_LOW_SIZE)
      bitStream[1]<<=1;
    else
      if(pulsoMedido>MIN_STOP_SIZE && pulsoMedido<MAX_STOP_SIZE)        
        novaCaptura[1] = 0x01;     
  //----------------------------------------------------------         
}
/***********************************************************************************
*   Descri��o         :   Tratamento da interrup��o do canal 2 do TPM1
*   Parametros        :   nenhum
*   Retorno           :   nenhum
***********************************************************************************/
void interrupt VectorNumber_Vtpm1ch2 rtiTPM3(void){
  static unsigned int larguraAnterior=0,
                      larguraAtual=0,
                      pulsoMedido;
               
  if(!TPM1C2SC_CH2F)
    return;
  
  TPM1C2SC_CH2F = 0;                          
        
  larguraAtual  = TPM1C2V;         
  //Calcula a largura efetiva do pulso recebido                      
  if(larguraAtual>larguraAnterior)
    pulsoMedido = larguraAtual -  larguraAnterior;
  else
    pulsoMedido = (unsigned int)((unsigned int)(65535 - larguraAnterior) + larguraAtual); 
  
  larguraAnterior = larguraAtual;  
  
  
  if(pulsoMedido>MAX_START_SIZE || pulsoMedido<MIN_LOW_SIZE)
   return;
  
  if(pulsoMedido>MIN_START_SIZE && pulsoMedido<MAX_START_SIZE){    
    bitStream[2] = 0x00;
    return;
  }
    
  //---------------------------------------------------------  
  if(pulsoMedido>MIN_HIGH_SIZE && pulsoMedido<MAX_HIGH_SIZE){
      bitStream[2]|= 0x01;
      bitStream[2]<<=1;            
  } 
  else             
    if(pulsoMedido>MIN_LOW_SIZE && pulsoMedido<MAX_LOW_SIZE)
      bitStream[2]<<=1;
    else
      if(pulsoMedido>MIN_STOP_SIZE && pulsoMedido<MAX_STOP_SIZE)        
        novaCaptura[2] = 0x01;     
  //----------------------------------------------------------         
}
/***********************************************************************************
*   Descri��o         :   Tratamento da interrup��o do canal 3 do TPM1
*   Parametros        :   nenhum
*   Retorno           :   nenhum
***********************************************************************************/
void interrupt VectorNumber_Vtpm1ch3 rtiTPM4(void){
  static unsigned int larguraAnterior=0,
                      larguraAtual=0,
                      pulsoMedido;
               
  if(!TPM1C3SC_CH3F)
    return;
  
  TPM1C3SC_CH3F = 0;                          
        
  larguraAtual  = TPM1C3V;         
  //Calcula a largura efetiva do pulso recebido                      
  if(larguraAtual>larguraAnterior)
    pulsoMedido = larguraAtual -  larguraAnterior;
  else
    pulsoMedido = (unsigned int)((unsigned int)(65535 - larguraAnterior) + larguraAtual); 
  
  larguraAnterior = larguraAtual;  
  
  
  if(pulsoMedido>MAX_START_SIZE || pulsoMedido<MIN_LOW_SIZE)
   return;
  
  if(pulsoMedido>MIN_START_SIZE && pulsoMedido<MAX_START_SIZE){    
    bitStream[3] = 0x00;
    return;
  }
    
  //---------------------------------------------------------  
  if(pulsoMedido>MIN_HIGH_SIZE && pulsoMedido<MAX_HIGH_SIZE){
      bitStream[3]|= 0x01;
      bitStream[3]<<=1;            
  } 
  else             
    if(pulsoMedido>MIN_LOW_SIZE && pulsoMedido<MAX_LOW_SIZE)
      bitStream[3]<<=1;
    else
      if(pulsoMedido>MIN_STOP_SIZE && pulsoMedido<MAX_STOP_SIZE)        
        novaCaptura[3] = 0x01;     
  //----------------------------------------------------------        
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/