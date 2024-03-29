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
|       Arquivo            : eeprom.c
|       Descri��o          : Fun��es para abstra��o da escrita �s p�ginas da eeprom
| 
|       Autor              : Marcos Aquino
|       Data cria��o       : 18/03/2011
|
|       Revis�es           : 1.1
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
*  Descri��o     : Inicializa��o do m�dulo
*  Parametros    : (nenhum)
*  Retorno       : (nenhum)
***********************************************************************************/
void EEPROM_init(void){

  I2CLIB_init();
}
/***********************************************************************************
*       Descri��o     : Escreve um bloco de dados na EEPROM
*       Parametros    : (unsigned long int) n�mero de 32 bits
*                       (unsigned char*) Ponteiro para o buffer que ser� salvo
*                       na eeprom
*                       (unsigned int) tamanho do bloco que ser� salvo na eeprom
*       Retorno       : (unsigned char) Resultado da opera��o de escrita na eeprom
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
      
    pData+=toWriteBytes; // Ajusta o ponteiro com a quantidade de bytes que foi enviada ao controlador da mem�ria
    
    pagina++;    
    size-=toWriteBytes;
    address+=toWriteBytes;    
     
    SYSTICK_delay(7);//15);//7); 
  } 
       
  return 0;
}
/***********************************************************************************
*       Descri��o     :  L� um bloco de dados da EEPROM
*       Parametros    : (unsigned long int) N�mero de 32 bit referente ao endere�o
*                       (unsigned char*) POnteiro para o buffer que receber� os
*                       os dados
*       Retorno       : (unsigned char) Resultado da opera��o de leitura da eeprom
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
  pData+=toWriteBytes; // Ajusta o ponteiro com a quantidade de bytes que foi enviada ao controlador da mem�ria    
  pagina++;    
  size-=toWriteBytes;    
  address+=toWriteBytes;    
 }         
    
 return 0;
}
/***********************************************************************************
*       Descri��o     : Escreve um bloco de bytes fazendo acesso direto � mem�ria
*       Parametrso    : (unsigned int) enderecoInicial
*                       (unsigned char*) Ponteiro para o buffer que receber� os dados
*                       (unsigned int) tamanho do bloco de dados
*       Retorno       : (unsigned char) Resultado da opera��o de leitura da eeprom
***********************************************************************************/
unsigned char directWriteBlock(unsigned int enderecoInicial,unsigned char *pData,
                               unsigned int tamanho){
   
  writeBlock(enderecoInicial,pData,tamanho); 
  return 0;
}
/***********************************************************************************
*       Descri��o     : L� um bloco de bytes fazendo acesso direto � mem�ria
*       Parametros    : (unsigned int) endere�o inicial
*                       (unsigned char*) Ponteiro para o buffer que receber�
*                       os dados
*                       (unsigned int) tamanho do bloco de dados
*       Retorno       : (unsigned char) Resultado da opera��o da leitura da eprom
***********************************************************************************/
unsigned char diretecReadBlock(unsigned int enderecoInicial,unsigned char *pData,
                               unsigned int tamanho){

  readBlock(enderecoInicial,pData,tamanho);                      
  return 0;                                 
}
/***********************************************************************************
*   Fim do arquivo
***********************************************************************************/