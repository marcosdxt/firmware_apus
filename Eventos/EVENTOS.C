/*__________________________________________________________________________________
|	      Chave Digital 
|       Blumenau - SC
|       www.chavedigital.com.br
| __________________________________________________________________________________
|
|       This source code was developed by Chave Digital and cannot be copied, in part or
|       in whole, or used, except when legally licensed by Chave Digital or its distributors.
|
|       Este código é propriedade da Chave Digital e não pode ser copiado, em parte ou
|       em todo, ou utilizado, exceto quando for legalmente licenciado pela Chave Digital 
|       ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       File	      :  FilaEvento.c
|       Description :  Implementação do gerenciador de filas em memória
|                      externa
|	      Author      :  Marcos Aquino
|
|       Created on  :  25/01/2010
|
|       History     :
|                      03/08/2010 - Alterado indexador da leitura dos blocos
|                      na fila.
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*	Includes
***********************************************************************************/
#include "Eventos.h"
#include "FilaeventoType.h"

/***********************************************************************************
*	Defines - Local
***********************************************************************************/ 


/***********************************************************************************
*	Enumerations - Local
***********************************************************************************/


/***********************************************************************************
*	Structs - Local
***********************************************************************************/
typedef struct{
  unsigned short int ultimoEventoInserido,
                     ultimoEventoLido,
                     inicioFila,                                          
                     crc16;  
}eControlBlock;
/***********************************************************************************
*	Types - Local
***********************************************************************************/


/***********************************************************************************
*	Variables - Local
***********************************************************************************/


/***********************************************************************************
*	Functions - Local
***********************************************************************************/
unsigned char FILAEVENTO_checkControlBlock(eFila *fila,eControlBlock *c1,eControlBlock *c2);

/***********************************************************************************
*	Implementations
***********************************************************************************/
                  
/***********************************************************************************
*       Descrição        :    Carrega o bloco de controle do bloco
*                             de eventos
*       Parametros       :    (eFila*) Fila 
*                             (eControlBlock*) Bloco de controle
*       Retorno          :    (unsigned char) Maior do que zero 
***********************************************************************************/
unsigned char FILAEVENTO_carregaBlocoControle(eFila *fila,eControlBlock *bloco){
  eControlBlock blocoControle[2];
  unsigned int enderecoBloco;
  unsigned char cont=0;
  unsigned char i;  
    
  enderecoBloco = fila->inicioFila;
    
  (void)BLOCK_READ(enderecoBloco,(unsigned char*)&blocoControle,sizeof(eControlBlock)*2);
  
  for(i=0;i<2;i++)
    if(!UTIL_checa_crc((unsigned char*)&blocoControle[i],sizeof(eControlBlock)-2))
      cont++;
    else{
      bloco->ultimoEventoInserido = blocoControle[i].ultimoEventoInserido;
      bloco->ultimoEventoLido = blocoControle[i].ultimoEventoLido;
      bloco->inicioFila = blocoControle[i].inicioFila;
      bloco->crc16 = blocoControle[i].crc16;
      return 0x01;
    }
  
  if(cont==2){
    bloco->ultimoEventoInserido = 0x00;
    bloco->ultimoEventoLido = 0x00;
    bloco->inicioFila = 0x00;
    bloco->crc16 = 0x00;
    return 0;
  }
              
  return 1;
}
/***********************************************************************************
*       Descrição         :   Atualiza os dados do bloco de controle de uma
*                             fila de eventos
*       Parametros        :   (eFila*) Ponteiro para a fila
*                             (unsigned short int) ultimo evento inserido
*                             (unsigned short int) ultimo evento lido
*                             (unsigned short int) inicio da fila
*       Retorno           :   (unsigned char) Maior do que zero se conseguir
*                             atualizar os dados da estrutura com sucesso
***********************************************************************************/
unsigned char FILAEVENTO_salvaBlocoControle(eFila *fila,unsigned short int ultimoInserido,
                                            unsigned short int ultimoLido,
                                            unsigned short int inicioFila){                                              
  eControlBlock blocoControle[2],lido[2];
  
  blocoControle[0].ultimoEventoInserido = blocoControle[1].ultimoEventoInserido = ultimoInserido;
  blocoControle[0].ultimoEventoLido = blocoControle[1].ultimoEventoLido = ultimoLido;
  blocoControle[0].inicioFila = blocoControle[1].inicioFila = inicioFila;
  blocoControle[0].crc16 = blocoControle[1].crc16 = CRC_16((unsigned char*)&blocoControle[0],sizeof(eControlBlock)-2);    
                                              
  (void)BLOCK_WRITE(fila->inicioFila,(unsigned char*)blocoControle,sizeof(eControlBlock)*2);
  (void)BLOCK_READ(fila->inicioFila ,(unsigned char*)lido,sizeof(eControlBlock)*2);                                              
  
  
  return  blocoControle[0].ultimoEventoInserido == lido[0].ultimoEventoInserido &&
          blocoControle[0].ultimoEventoLido     == lido[0].ultimoEventoLido     &&
          blocoControle[0].inicioFila           == lido[0].inicioFila           &&
          blocoControle[0].crc16                == lido[0].crc16                &&            
          blocoControle[1].ultimoEventoInserido == lido[1].ultimoEventoInserido &&
          blocoControle[1].ultimoEventoLido     == lido[1].ultimoEventoLido     &&
          blocoControle[1].inicioFila           == lido[1].inicioFila           &&
          blocoControle[1].crc16                == lido[1].crc16;
}
/***********************************************************************************
*       Descrição         :   Adiciona um bloco de dados à uma determinada
*                             fila
*       Parametros        :   (eFila*) fila
*                             (void*) bloco de dados
*       Retorno           :   (unsigned char) Maior do que zero se conseguir
*                             inserir os dados na fila
***********************************************************************************/
unsigned char FILAEVENTO_insert(eFila* fila,void* block){
  eControlBlock controle;
  
  (void)FILAEVENTO_carregaBlocoControle(fila,&controle);
          
  (void)BLOCK_WRITE(fila->inicioBlocos +  (fila->tamanhoBloco * controle.ultimoEventoInserido),(unsigned char*)block, fila->tamanhoBloco);
  
  controle.ultimoEventoInserido = (controle.ultimoEventoInserido+1) % fila->blocosNaFila;
  (void)FILAEVENTO_salvaBlocoControle(fila,controle.ultimoEventoInserido,controle.ultimoEventoLido,controle.inicioFila);    
                     
  return 1;  
}
/***********************************************************************************
*       Descrição         :   Lê um bloco de uma determinada fila
*       Parametros        :   (eFila*) fila
*                             (unsigned short int) ponteiro para o evento
*                             (void*) ponteiro para a estrutura que receberá
*                             os dados da fila
*       Retorno           :   (unsigned char)
***********************************************************************************/
unsigned char FILAEVENTO_getAt(eFila* fila,unsigned short int ponteiro,void *block){
  eControlBlock controle;
  unsigned int offSet;
  
  (void)FILAEVENTO_carregaBlocoControle(fila,&controle);  
    
  offSet = (controle.ultimoEventoLido + ponteiro) % fila->blocosNaFila;
      
  (void)BLOCK_READ( (fila->inicioBlocos + (offSet* fila->tamanhoBloco)),
                    (unsigned char*)block,fila->tamanhoBloco);  
    
  return 1;  
}
/***********************************************************************************
*       Descrição         :   Zera a estrutura de controle de uma determinada
*                             fila
*       Parametros        :   (eFila*) fila
*       Retorno           :   (unsigned char) 
***********************************************************************************/
unsigned char FILAEVENTO_clear(eFila* fila){
  
  (void)FILAEVENTO_salvaBlocoControle(fila,0,0,0);  
  
  return 1;
}
/***********************************************************************************
*       Descrição         :   Retorna o número de eventos existentes em uma
*                             determinada fila
*       Parametros        :   (eFila*) fila
*       Retorno           :   (unsigned short int) número de blocos na fila
***********************************************************************************/
unsigned short int FILAEVENTOS_size(eFila *fila){
  eControlBlock controle;
  
  (void)FILAEVENTO_carregaBlocoControle(fila,&controle);    
  
  
  if(controle.ultimoEventoInserido>=controle.ultimoEventoLido)
    return controle.ultimoEventoInserido - controle.ultimoEventoLido;
  
  return (fila->blocosNaFila -  controle.ultimoEventoLido) + controle.ultimoEventoInserido;
}
/***********************************************************************************
*       Descrição         :   Realiza o ack em uma determinada fila
*       Parametros        :   (eFila *) fila de eventos 
*                             (unsigned short int) número de eventos que foram
*                             "lidos" da fila
*       Retorno           :   (nenhum)
***********************************************************************************/
void FILAEVENTOS_ackEventos(eFila *fila,unsigned short int ackNumber){
  eControlBlock controle;
  unsigned short int trunk;
  
  (void)FILAEVENTO_carregaBlocoControle(fila,&controle);      
  
  trunk = FILAEVENTOS_size(fila);
  if(ackNumber > trunk)
    ackNumber = trunk;
  
  controle.ultimoEventoLido = (controle.ultimoEventoLido + trunk) % fila->blocosNaFila;
  
  // Salva as configurações da fila na eeprom
  (void)FILAEVENTO_salvaBlocoControle(fila,
                                      controle.ultimoEventoInserido,
                                      controle.ultimoEventoLido,
                                      controle.inicioFila);     
}
/***********************************************************************************
*	End of file
***********************************************************************************/