#include <MC9S08AC128.H>
#include "i2c_lib.h"
/***********************************************************************************
*   Definições
***********************************************************************************/
#define BIT_0                             1
#define BIT_1                             2
#define BIT_2                             4
#define BIT_3                             8
#define BIT_4                             16
#define BIT_5                             32
#define BIT_6                             64
#define BIT_7                             128
// IICC register
#define bIICEN                            BIT_7
#define bIICIE                            BIT_6
#define bMST                              BIT_5
#define bTX                               BIT_4
#define bTXAK                             BIT_3
#define bRSTA                             BIT_2
// IICS register
#define bTCF                              BIT_7
#define bIAAS                             BIT_6
#define bBUSY                             BIT_5
#define bARBL                             BIT_4
#define bSRW                              BIT_2
#define bIICIF                            BIT_1
#define bRXAK                             BIT_0
// IICF register
#define I2C_M1                            0
#define I2C_M2                            BIT_6
#define I2C_M4                            BIT_7

#define RD                                1
#define WR                                2
/***********************************************************************************
*   Enumerações
***********************************************************************************/
typedef enum{
  i2c_Idle,
  i2c_Start,
  i2c_repeatedStart,
  i2c_writeHighAddress,
  i2c_writeLowerAddress,
  i2c_writeByte,
  i2c_dummyRead,
  i2c_read,
  i2c_stop,
  i2c_blockWrite,
  i2c_blockRead
}eI2cStates;
/***********************************************************************************
*   Variaveis - Globais
***********************************************************************************/
unsigned char I2CLIB_buffer[70],
              bytesToWR=0,
              stopFlag=0;



/***********************************************************************************
*   Implementação
***********************************************************************************/

/***********************************************************************************
*   Descrição     : 
*   Parametros    : 
*   Retorno       : 
***********************************************************************************/
void I2CLIB_init(void){
    
  IIC1F = 0x00;
  IIC1C = 0x00;
  
  __asm NOP;
  __asm NOP;  
  __asm NOP;
  __asm NOP;
  __asm NOP;
  __asm NOP;
    
  IIC1F = I2C_M4 | 0x0B;
  IIC1C = bIICEN | bIICIE; // enable the I2C interface
}
/***********************************************************************************
*   Descrição     : 
*   Parametros    : 
*   Retorno       : 
***********************************************************************************/
eI2cStates I2CLIB_fsm(unsigned char newState){
  static eI2cStates estadoAtual = i2c_Idle;
  static unsigned char comand,count=0,timeout;
  
  //Escreve um bloco no dispositivo
  if(newState == i2c_blockWrite){
    estadoAtual = i2c_Start;
    comand = WR;       
  }
  
  //Lê um bloco a partir do dispositivo
  if(newState == i2c_blockRead){
    estadoAtual = i2c_Start;         
    comand = RD;
  }
  
  
  if(!IIC1S_TCF)
    return estadoAtual;
    
  switch(estadoAtual){    
    case i2c_Idle             : 
                                break;
    case i2c_Start            : // Gera o sinal de start no barramento
                                IIC1C = bIICEN + bIICIE + bMST + bTX;
                                IIC1D = I2CLIB_buffer[0];
                                estadoAtual = i2c_writeHighAddress;     
                                break;    
    case i2c_repeatedStart    : // Repete o sinal de start
                                IIC1C_RSTA = 1;               
                                IIC1D = I2CLIB_buffer[0] | 0x01;
                                estadoAtual = i2c_dummyRead;
                                break;    
    case i2c_writeHighAddress : // Envia a parte alta do endereço
                                IIC1D = I2CLIB_buffer[1];
                                estadoAtual = i2c_writeLowerAddress;
                                break;    
    case i2c_writeLowerAddress: // Envia a parte baixa do endereço
                                IIC1D = I2CLIB_buffer[2];
                                count = 0x00;
                                if(comand==WR)
                                  estadoAtual = i2c_writeByte;
                                else
                                  estadoAtual = i2c_repeatedStart;                                  
                                break;    
    case i2c_writeByte        : // Escreve um byte
                                IIC1D = I2CLIB_buffer[3+count];
                                count++;               
                                bytesToWR--;    
                                
                                if(bytesToWR)
                                  estadoAtual = i2c_writeByte;                    
                                else
                                  estadoAtual = i2c_stop;                                  
                                break;    
    case i2c_dummyRead        : // Dummy read 
                                IIC1C_TX = 0;                                              
                                I2CLIB_buffer[3+count] = IIC1D;
                                estadoAtual = i2c_read;    
                                break;    
    case i2c_read             : // Lê os dados da memória
                                I2CLIB_buffer[3+count] = IIC1D;                                                                
                                IIC1C_TXAK = 1;
                                
                                timeout=50;
                                while(IIC1S_RXAK && --timeout);
                                
                                if(!IIC1S_RXAK){
                                  count++;
                                  bytesToWR--;
                                  if(bytesToWR){                                    
                                    estadoAtual = i2c_read;
                                    IIC1C_TXAK = 0x00;
                                  }
                                  else
                                    estadoAtual = i2c_stop;
                                }
                                
                                if(!timeout)
                                  estadoAtual = i2c_stop;
                                break;    
    case i2c_stop             : // Gera o sinal de stop para o dispositivo
                                IIC1C_MST = 0;
                                estadoAtual = i2c_Idle;
                                stopFlag = 1;
                                break;    
  }   
  
  return estadoAtual; 
}
/***********************************************************************************
*   Descrição     : 
*   Parametros    : 
*   Retorno       : 
***********************************************************************************/
#pragma CODE_SEG __FAR_SEG P0
void interrupt VectorNumber_Viic1 rtiIIC(void){
      
  if (IIC1S_TCF)
    (void)I2CLIB_fsm(0); 


  if (IIC1S_ARBL)
  {
    IIC1S_ARBL = 1;
    (void)I2CLIB_fsm(i2c_repeatedStart);
  }
  
  IIC1S_IICIF = 1;  
}
/***********************************************************************************
*   Descrição      :    Escreve um bloco no dispositivo do barramento i2c
*   Parametros     :    (unsigned int) endereço do dispositivo
*                       (unsigned char*) Ponteiro para os dados
*                       (unsigned int) tamanho do buffer de dados
*   Retorno        :    nenhum
***********************************************************************************/
void writeBlock(unsigned int address,unsigned char *pData,unsigned int size){
  unsigned int i;
      
  stopFlag = 0x00;
  I2CLIB_buffer[0] = 0xA0;
  I2CLIB_buffer[1] = (unsigned char)(address>>8);
  I2CLIB_buffer[2] = (unsigned char)address;
  for(i=0;i<size;i++)
    I2CLIB_buffer[3+i] = pData[i];
    
  bytesToWR = (unsigned char)size;//Bytes de dados que são enviados no estado byteWrite
  (void)I2CLIB_fsm(i2c_blockWrite);    
  
  while(!stopFlag);  
}
/***********************************************************************************
*   Descrição     :     Faz a leitura de um bloco de dados de um dispositivo
*                       no barramento I2c
*   Parametros    :     (unsigned int) endereço do dispositivo
*                       (unsigned char*) Ponteiro para o buffer que armazenará os
*                       dados lidos do barramento
*                       (unsigned int) Número de bytes que serão lidos
*   Retorno       ;     (nenhum)
***********************************************************************************/
void readBlock(unsigned int address,unsigned char *pData,unsigned int size){  
  unsigned int i;//,timeOut;
 
  for(i=0;i<size;i++)
    I2CLIB_buffer[3+i] = 0x00; 
  
  stopFlag = 0x00;
  I2CLIB_buffer[0] = 0xA0;
  I2CLIB_buffer[1] = (unsigned char)(address>>8);
  I2CLIB_buffer[2] = (unsigned char) address;
  bytesToWR = (unsigned char)size;    
  (void)I2CLIB_fsm(i2c_blockRead);   
  
  while(!stopFlag);
  
  
  for(i=0;i<size;i++)
    pData[i] = I2CLIB_buffer[3+i]; 
}
/***********************************************************************************
*   Fim do arquivo
***********************************************************************************/