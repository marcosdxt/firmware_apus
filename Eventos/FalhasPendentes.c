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
|       Produto            :  Central Apus CD432
|       Arquivo            :  FalhasPendentes.c
|       Descrição          :  Implementação da lista de falhas pendentes no sistema
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  16/07/2010
|
|       Revisões           :  1.0.0
|
|
| __________________________________________________________________________________
*/

/***********************************************************************************
*		Includes
***********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "eventosSistema.h"
#include "falhasPendentes.h"

/***********************************************************************************
*		Estruturas
***********************************************************************************/ 
struct nodoS{
  unsigned char descricao;
  unsigned char parametro;
  struct nodoS *next;  
};

typedef struct nodoS sNodo;
/***********************************************************************************
*		Variaveis locais
***********************************************************************************/
sNodo lista[TAMANHO_ENUM_FALHAS*2],
      *inicioLista=NULL,
      *fimLista=NULL;
      
unsigned char elementosNaLista=0;
/***********************************************************************************
*		Funções locais
***********************************************************************************/
unsigned char contains(eDefs descricao,unsigned char parametro);
sNodo *FP_alloc(void);
 
/***********************************************************************************
*		Implementação
***********************************************************************************/

/***********************************************************************************
*   Descrição       :   Inicialização da fila de falhas pendentes
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void FP_init(void){
  unsigned char j;
  
  inicioLista = 0x00;
  fimLista = 0x00;
  fimLista = 0x00;  
  
  for(j=0;j<TAMANHO_ENUM_FALHAS*2;j++){      
      lista[j].descricao = 0xFF;
      lista[j].parametro = 0xFF;
      lista[j].next = NULL;
  }
  
  inicioLista = NULL;
  fimLista = NULL;
  elementosNaLista = 0x00;            
}
/***********************************************************************************
*   Descrição       :   Insere uma nova falha na fila de falhas pendentes
*   Parametros      :   (eDefs) descrição da falha
*                       (unsigned char) parametro
*   Retorno         :   nenhum
***********************************************************************************/
void FP_insereFalha(eDefs descricao,unsigned char parametro){
  sNodo *p;
  
  if(contains(descricao,parametro) || (elementosNaLista>(TAMANHO_ENUM_FALHAS*2)-1) )
    return;        
  
  p = FP_alloc();
  if(p==NULL)
    return; 
  
  p->descricao = (unsigned char)descricao;
  p->parametro = parametro;
  p->next = NULL;
  
  if(elementosNaLista==0)
    inicioLista = p;   
  else
    fimLista->next = p;
      
  fimLista = p;
  
  elementosNaLista++;                    
}
/***********************************************************************************
*   Descrição       :   Remove uma falha da fila de falhas pendentes
*   Parametros      :   (eDefs) descrição da falha
*                       (unsigned char) parametro
*   Retorno         :   nenhum
***********************************************************************************/
void FP_removeFalha(eDefs descricao,unsigned char parametro){
  sNodo *p,*anterior;
  
  if(!contains(descricao,parametro) || !elementosNaLista)
    return;
    
  p = inicioLista;
      
  while(p != NULL){
    
    if( (p->descricao==(unsigned char) descricao) && (p->parametro == parametro)){      
    
      if( p == inicioLista )
        inicioLista = p->next;      
      else
        anterior->next = p->next;
      
      if(p==fimLista)
        fimLista = anterior;      
            
      p->descricao = 0xFF;
      p->parametro = 0xFF;
      p->next =NULL;      
              
      elementosNaLista--;    
      return;
    }    
    anterior = p;
    p = p->next;
  }//Fim do while  
}
/***********************************************************************************
*   Descrição       :   Verifica se a lista contém a falha
*   Parametros      :   (eDefs) descrição da falha
*                       (unsigned char) parametro
*   Retorno         :   nenhum
***********************************************************************************/
unsigned char contains(eDefs descricao,unsigned char parametro){
  unsigned char i;
  
  for(i=0;i<TAMANHO_ENUM_FALHAS*2;i++)  
    if( (lista[i].descricao == (unsigned char)descricao) && (lista[i].parametro == parametro))
      return 0xFF;        
  
  return 0x00;
}
/***********************************************************************************
*   Descrição       :   Aloca um bloco de memória para o evento
*   Parametros      :   nenhum
*   Retorno         :   (unsigned char*) ponteiro para o bloco 
***********************************************************************************/
sNodo *FP_alloc(void){
  unsigned char i;
  
  for(i=0;i<TAMANHO_ENUM_FALHAS*2;i++)  
    if( (lista[i].descricao == 0xFF) && (lista[i].parametro == 0xFF) )
      return &lista[i];
   
  return NULL;
}
/***********************************************************************************
*   Descrição       :   Pega uma falha pendente da lista
*   Parametros      :   (unsigned char) indice
*                       (eDefs*) Ponteiro para o descritor do defeito
*                       (unsigned char*) Ponteiro para o parametro da falha
*   Retorno         :   nenhum
***********************************************************************************/
unsigned char FP_getNodo(unsigned char indice,eDefs *descricao,unsigned char *parametro){
  unsigned char i;
  sNodo *nodo;
  
  if(indice>(elementosNaLista-1))
    return 0x00;
 
  i=0;
  nodo = inicioLista;
  while(nodo != NULL){   
   if(i==indice){
      *descricao = (eDefs)nodo->descricao;
      *parametro = nodo->parametro;
      return 0xFF;    
   }
   i++;         
   nodo = nodo->next; 
  }
 
  return 0x00;
}
/***********************************************************************************
*   Descrição       :   Monta as strings das falhas pendentes
*   Parametros      :   (eDefs) descritor do defeito
*                       (unsigned char) parametro
*   Retorno         :   nenhum
***********************************************************************************/
void FP_makeString(eDefs descritor,unsigned char parametro,char *linha){
  
  
  switch(descritor){                               //    0123456789012345
    case FaltaTerminador        : (void)sprintf(linha,  "Falta term l:%02d ",parametro+1);
                                  break;
    case CurtoCircuitoLaco      : (void)sprintf(linha,  "Curto laco :%02d  ",parametro+1);
                                  break;    
    case FaltaTerminadorSirene  : if(!parametro)
                                    (void)sprintf(linha,"Sem term sir pri");
                                  else
                                    (void)sprintf(linha,"Sem term sir aux");
                                  break;    
    case CurtoSaidaSirene       : if(!parametro)
                                    (void)sprintf(linha,"Curto sir prin  ");
                                  else
                                    (void)sprintf(linha,"Curto sir aux   ");
                                  break;    
    case FaltaRedeEletrica      : (void)sprintf(linha  ,"Falta rede elet ");
                                  break;    
    case FaltaBateria           : (void)sprintf(linha  ,"Falta bateria   ");
                                  break;    
    case SubTensaoFonte         : (void)sprintf(linha,  "Sub tensao      ");
                                  break;    
    case SobreTensaoFonte       : (void)sprintf(linha,  "Sobre tensao    ");
                                  break;    
    case FugaAoTerraPositivo    : (void)sprintf(linha,  "Fuga terra +    ");
                                  break;    
    case FugaAoTerraNegativo    : (void)sprintf(linha,  "Fuga terra -    ");
                                  break;    
    case FalhaCommImpressora    : 
                                  break;    
    case FalhaCommModbus        :
                                  break;    
    case FalhaCommApusNet       :
                                  break;  
    case Laco_01_bloqueado      : (void)sprintf(linha,  "Laco 01 bloq.   ");
                                  break;      
    case Laco_02_bloqueado      : (void)sprintf(linha,  "Laco 02 bloq.   ");
                                  break;      
    case Laco_03_bloqueado      : (void)sprintf(linha,  "Laco 03 bloq.   ");
                                  break;       
    case Laco_04_bloqueado      : (void)sprintf(linha,  "Laco 04 bloq.   ");
                                  break;      
  }
  
}
/***********************************************************************************
*   Descrição       :   Verifica o número de falhas pendentes na lista 
*   Parametros      :   
*   Retorno         :   nenhum
***********************************************************************************/
unsigned char FP_getElementsInQueue(void){
  
  return elementosNaLista; 
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/