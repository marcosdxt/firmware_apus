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
|       Arquivo            : eeprom.c
|       Descrição          : Funções para abstração da escrita às páginas da eeprom
| 
|       Autor              : Marcos Aquino
|       Data criação       : 18/03/2011
|
|       Revisões           : 1.1
|
|
| __________________________________________________________________________________
*/

/***********************************************************************************
*	Includes
***********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "i2c_lib.h"
#include "eeprom.h"

/***********************************************************************************
*	Defines - Local
***********************************************************************************/ 
#define PAGE_0_INIT                               0x00000000
#define PAGE_1_INIT                               0x00020000
#define PAGE_2_INIT                               0x00040000
#define PAGE_3_INIT                               0x00060000

#define EEPROM_ID                                 0xA0
#define EEPROM_READ                               0x01
#define EEPROM_WRITE                              0x00

#define WRITE_PAGE_SIZE                           64
#define MEMORY_PAGE_SIZE                          0x4000
/***********************************************************************************
*	 Enumerations - Local
***********************************************************************************/


/***********************************************************************************
*	 Structs - Local
***********************************************************************************/


/***********************************************************************************
*	 Types - Local
***********************************************************************************/


/***********************************************************************************
*	 Variables - Local
***********************************************************************************/


/***********************************************************************************
*  Functions - Local
***********************************************************************************/
extern void SYSTICK_delay(unsigned int delay);

unsigned char directWriteBlock(unsigned int enderecoInicial,unsigned char *pData,
                               unsigned int tamanho);

unsigned char diretecReadBlock(unsigned int enderecoInicial,unsigned char *pData,
                               unsigned int tamanho);

void resolveEndereco(unsigned int endereco,unsigned int *address,unsigned char *ctrl);
/***********************************************************************************
*  Implementations
***********************************************************************************/

/***********************************************************************************
*  Descrição     : Inicialização do módulo
*  Parametros    : (nenhum)
*  Retorno       : (nenhum)
***********************************************************************************/
void EEPROM_init(void){

  I2CLIB_init();
}
/***********************************************************************************
*       Descrição     : Escreve um bloco de dados na EEPROM
*       Parametros    : (unsigned long int) número de 32 bits
*                       (unsigned char*) Ponteiro para o buffer que será salvo
*                       na eeprom
*                       (unsigned int) tamanho do bloco que será salvo na eeprom
*       Retorno       : (unsigned char) Resultado da operação de escrita na eeprom
***********************************************************************************/
unsigned char EEPROM_writeBlock(unsigned int address,unsigned char *pData,unsigned int size){
  unsigned int pagina,offSet,toWriteBytes,espacoInicial;
        
  pagina = address/WRITE_PAGE_SIZE;
  offSet = address - (pagina*WRITE_PAGE_SIZE);    
      
  espacoInicial = WRITE_PAGE_SIZE - offSet;
  
  while(size){
    
    offSet = address - (pagina*WRITE_PAGE_SIZE);    
      
    if(espacoInicial<size){
      toWriteBytes = espacoInicial;         
       espacoInicial = 0xFFFFFFFF;                
    }
    else{
      if(size>WRITE_PAGE_SIZE)
        toWriteBytes = WRITE_PAGE_SIZE;
      else
        toWriteBytes = size;                   
    }
      
    (void)directWriteBlock(address,pData,toWriteBytes);
      
    pData+=toWriteBytes; // Ajusta o ponteiro com a quantidade de bytes que foi enviada ao controlador da memória
    
    pagina++;    
    size-=toWriteBytes;
    address+=toWriteBytes;    
     
    SYSTICK_delay(7);//15);//7); 
  } 
       
  return 0;
}
/***********************************************************************************
*       Descrição     :  Lê um bloco de dados da EEPROM
*       Parametros    : (unsigned long int) Número de 32 bit referente ao endereço
*                       (unsigned char*) POnteiro para o buffer que receberá os
*                       os dados
*       Retorno       : (unsigned char) Resultado da operação de leitura da eeprom
***********************************************************************************/
unsigned char EEPROM_readBlock(unsigned int address,unsigned char *pData,unsigned int size){
 unsigned int pagina,offSet,toWriteBytes,espacoInicial;
          
 pagina = address/WRITE_PAGE_SIZE;
 offSet = address - (pagina*WRITE_PAGE_SIZE);  
  
 espacoInicial = WRITE_PAGE_SIZE - offSet;  
  
 while(size){    
  offSet = address - (pagina*WRITE_PAGE_SIZE);    
    
  if(espacoInicial<size){
   toWriteBytes = espacoInicial;         
   espacoInicial = 0xFFFFFFFF;
  }
  else{      
   if(size>WRITE_PAGE_SIZE)
    toWriteBytes = WRITE_PAGE_SIZE;
   else
    toWriteBytes = size;      
  } 
  (void)diretecReadBlock(address,pData,toWriteBytes);
  pData+=toWriteBytes; // Ajusta o ponteiro com a quantidade de bytes que foi enviada ao controlador da memória    
  pagina++;    
  size-=toWriteBytes;    
  address+=toWriteBytes;    
 }         
    
 return 0;
}
/***********************************************************************************
*       Descrição     : Escreve um bloco de bytes fazendo acesso direto à memória
*       Parametrso    : (unsigned int) enderecoInicial
*                       (unsigned char*) Ponteiro para o buffer que receberá os dados
*                       (unsigned int) tamanho do bloco de dados
*       Retorno       : (unsigned char) Resultado da operação de leitura da eeprom
***********************************************************************************/
unsigned char directWriteBlock(unsigned int enderecoInicial,unsigned char *pData,
                               unsigned int tamanho){
   
  writeBlock(enderecoInicial,pData,tamanho); 
  return 0;
}
/***********************************************************************************
*       Descrição     : Lê um bloco de bytes fazendo acesso direto à memória
*       Parametros    : (unsigned int) endereço inicial
*                       (unsigned char*) Ponteiro para o buffer que receberá
*                       os dados
*                       (unsigned int) tamanho do bloco de dados
*       Retorno       : (unsigned char) Resultado da operação da leitura da eprom
***********************************************************************************/
unsigned char diretecReadBlock(unsigned int enderecoInicial,unsigned char *pData,
                               unsigned int tamanho){

  readBlock(enderecoInicial,pData,tamanho);                      
  return 0;                                 
}
/***********************************************************************************
*   Fim do arquivo
***********************************************************************************/