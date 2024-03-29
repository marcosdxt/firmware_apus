/*__________________________________________________________________________________
|	Chave Digital 
|       Blumenau - SC
| __________________________________________________________________________________
|
|       This source code was developed by Chave Digital and cannot be copied, in part or
|       in whole, or used, except when legally licensed by Chave Digital or its distributors.
|
|       Este c�digo � propriedade da Chave Digital e n�o pode ser copiado, em parte ou
|       em todo, ou utilizado, exceto quando for legalmente licenciado pela Chave Digital 
|       ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Product	    :        
|       Module	    : 
|       File	      :   FilaEvento.h
|       Description :   Gerenciador de fila de eventos em mem�ria externa
|
|       Created on  :   25/01/2010
|       Author      :   Marcos Aquino
|
|       History     :
|
|
| __________________________________________________________________________________
*/
#ifndef _FILA_EVENTO_H
#define	_FILA_EVENTO_H
/***********************************************************************************
*	Defines - Global
***********************************************************************************/ 


/***********************************************************************************
*	Enumerations - Global
***********************************************************************************/


/***********************************************************************************
*	Structs - Global
***********************************************************************************/
typedef struct{      
  unsigned int inicioFila,
               inicioBlocos;
  unsigned short int blocosNaFila,
                     tamanhoBloco;
}eFila;
/***********************************************************************************
*	Types - Global
***********************************************************************************/


/***********************************************************************************
*	Variables - Global
***********************************************************************************/


/***********************************************************************************
*	Functions - Global
***********************************************************************************/

//
//   Cria um novo descritor para gerenciador de fila
//     Todas as fun��es do gerenciador de fila necessitam que 
//     um descritor seja previamente criado.
//     O inicio do bloco sempre ser� ocupado pela estrutura de controle e 
//     esse ajuste � realizado automaticamente.
//
eFila *FILAEVENTO_create(eFila *filaRecord,unsigned int enderecoInicial,unsigned short int posicoesNaFila,
                         unsigned short int tamanhoDoBloco);

//  
//    Insere um novo bloco de dados na fila
//       o tamanho do bloco de dados � definido na cria��o da fila
//       se por acaso for passado um bloco maior ele ser� truncado
//       
unsigned char FILAEVENTO_insert(eFila *fila,void* block);

//
//    L� um evento da fila a partir do ultimo evento inserido
//       
unsigned char FILAEVENTO_getAt(eFila* fila,unsigned short int ponteiro,void *block);


//
//    Zera uma fila de eventos
//      --
//      --
//      --
unsigned char FILAEVENTO_clear(eFila* fila);


//
//    Retorna o n�mero de novos eventos inseridos na fila
//      --
unsigned short int FILAEVENTOS_size(eFila *fila);

//
//    Faz o ack de uma determinada quantidade de eventos da mem�ria
//
void FILAEVENTOS_ackEventos(eFila *fila,unsigned short int ackNumber);
/***********************************************************************************
*	End of file
***********************************************************************************/
#endif //_FILA_EVENTO_H