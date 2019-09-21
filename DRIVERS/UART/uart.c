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
|       Descrição          :  Funções para abstração do hardware da scis
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  15/02/2010
|
|       Revisões           :  1.0 (15/02/2010) - Primeira versão com controle
|                             básico: Inicialização, funções nos vetores e
|                             função de habilitação do sinal de txd
|
|
| __________________________________________________________________________________
*/
#include "derivative.h"
#include <stdio.h>
#include "uart.h"
/******************************************************************************
*     Definições
******************************************************************************/
#define BIT_0                                     1
#define BIT_1                                     2
#define BIT_2                                     4
#define BIT_3                                     8
#define BIT_4                                     16
#define BIT_5                                     32
#define BIT_6                                     64
#define BIT_7                                     128

#define bLOOPS                                    BIT_7
#define bSCISWAI                                  BIT_6
#define bRSRC                                     BIT_5
#define bM                                        BIT_4
#define bWAKE                                     BIT_3
#define bILT                                      BIT_2
#define bPE                                       BIT_1
#define bPT                                       BIT_0

#define bTIE                                      BIT_7
#define bTCIE                                     BIT_6
#define bRIE                                      BIT_5
#define bILIE                                     BIT_4
#define bTE                                       BIT_3
#define bRE                                       BIT_2
#define bRWU                                      BIT_1
#define bSBK                                      BIT_0
/*******************************************************************************
*     Estruturas
*******************************************************************************/
typedef struct{
  void(*rxdFunc)(unsigned char a);
  unsigned char*(*txdFunc)(void);
  void(*enableFunc)(unsigned char a);    
}sDriverFunctions;
/*******************************************************************************
*     Variaveis locais
*******************************************************************************/
sDriverFunctions sciFunctions[2];


/*******************************************************************************
*     Implementação
*******************************************************************************/

/*******************************************************************************
*     Descrição     :  Incializa a SCI 1 em um determinado baudrate
*     Parametros    :  (unsigned int) baudrate
*     Retorno       :  nenhum
*******************************************************************************/
void init_sci1(unsigned int baudrate,void(*rxd)(unsigned char a),
                                     unsigned char*(*txd)(void),
                                     void(*enable)(unsigned char state)){  
  unsigned long int contagemBaud;

  contagemBaud = (unsigned int)((unsigned long int)(BUS_CLK/baudrate));
  contagemBaud>>= 4;
  
  if(contagemBaud &~0x1FFF) 
    return;
      
  SCI1BDL = (unsigned char)contagemBaud;
  SCI1BDH = (unsigned char)(contagemBaud>>8);                                 
  SCI1C2  = bRIE | bTE | bRE;
  
  sciFunctions[0].enableFunc = enable;
  sciFunctions[0].rxdFunc = rxd;
  sciFunctions[0].txdFunc = txd;
}
/*******************************************************************************
*     Descrição     :   Inicializa a SCI 2 em um determinado baudrate
*     Parametros    :   (unsigned int) baudrate
*     Retorno       :   nenhum
*******************************************************************************/
void init_sci2(unsigned int baudrate,void(*rxd)(unsigned char a),
                                     unsigned char*(*txd)(void),
                                     void(*enable)(unsigned char state)){
  unsigned long int contagemBaud;

  contagemBaud = (unsigned int)((unsigned long int)(BUS_CLK/baudrate));
  contagemBaud>>= 4;
  
  if(contagemBaud &~0x1FFF) 
    return;
      
  SCI2BDL = (unsigned char)contagemBaud;
  SCI2BDH = (unsigned char)(contagemBaud>>8);                                 
  SCI2C2  = bRIE | bTE | bRE;
  
  sciFunctions[0].enableFunc = enable;
  sciFunctions[0].rxdFunc = rxd;
  sciFunctions[0].txdFunc = txd;   
}
/*******************************************************************************
*     Descrição     :   Função tratadora da interrupção de TXD da sci 1
*     Parametros    :   nenhum
*     Retorno       :   nenhum
*******************************************************************************/
void interrupt VectorNumber_Vsci1tx isrSCI1_txd(void){
  unsigned char *ptr;
  
  if(SCI1S1_TC){
   
     ptr = sciFunctions[0].txdFunc();
     if(ptr!=NULL){
        
        if(sciFunctions[0].enableFunc != NULL)
          sciFunctions[0].enableFunc(1);
        
        SCI1D = *ptr;      
     }
     else{      
       SCI1C2 &= ~bTIE;               
       sciFunctions[0].enableFunc(0);       
     }
     
     SCI1S1_TDRE = 0;
  }
}
/*******************************************************************************
*     Descrição     :   Função tratadora da interrupção de RXD da sci 1 
*     Parametros    :   nenhum
*     Retorno       :   nenhum
*******************************************************************************/
void interrupt VectorNumber_Vsci1rx isrSCI1_rxd(void){
  unsigned char temp;
  
  temp = SCI1S1;
  temp = SCI1D; 
    
  sciFunctions[0].rxdFunc(temp);
}
/*******************************************************************************
*     Descrição     :   Trigger para o envio de dados pela serial
*     Parametros    :   (unsigned char*) Ponteiro para o buffer que será
*                       enviad
*                       (unsigned int) tamanho do pacote que será enviado
*     Retorno       :   nenhum
*******************************************************************************/
void isc1_sendFirstByte(unsigned char *pData){
     
  SCI1D = *pData;
  SCI1C2 |= bTCIE;    
}
/*******************************************************************************
*     Descrição     :   Função tratadora da interrupção de TXD da sci  2
*     Parametros    :   nenhum
*     Retorno       :   nenhum
*******************************************************************************/
void interrupt VectorNumber_Vsci2tx isrSCI2_txd(void){
  unsigned char *ptr;
  
  if(SCI2S1_TC){
   
     ptr = sciFunctions[1].txdFunc();
     if(ptr!=NULL){
        
        if(sciFunctions[1].enableFunc != NULL)
          sciFunctions[1].enableFunc(1);
        
        SCI2D = *ptr;      
     }
     else{      
       SCI2C2 &= ~bTIE;               
       sciFunctions[1].enableFunc(0);     
     }
     
     SCI2S1_TDRE = 0;
  }    
}
/*******************************************************************************
*     Descrição     :   Função tratadora da interrupção de RXD da sci 2
*     Parametros    :   nenhum
*     Retorno       :   nenhum
*******************************************************************************/
void interrupt VectorNumber_Vsci2rx isrSCI2_rxd(void){
  unsigned char temp;
  
  temp = SCI2S1;
  temp = SCI2D; 
    
  sciFunctions[1].rxdFunc(temp);      
}
/*******************************************************************************
*     Descrição     :   Função para enviar o primeiro byte manualmente
*     Parametros    :   (unsigned char*) ponteiro para o byte
*     Retorno       :   nenhum
*******************************************************************************/
void isc2_sendFirstByte(unsigned char *pData){
 
  SCI2D = *pData;
  SCI2C2 |= bTCIE;      
}
/*******************************************************************************
*     Fim do arquivo
*******************************************************************************/