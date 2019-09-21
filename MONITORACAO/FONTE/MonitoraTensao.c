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
|       Arquivo            : MonitoraTensao.c
|       Descrição          : Faz a monitoração do nivel de tensão sob o capacitor
| 
|       Autor              : Marcos Aquino
|       Data criação       : 23/03/2011
|
|       Revisões           : 1.0
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*		Includes
***********************************************************************************/
#include <MC9S08AC128.H>
#include "monitoraTensao.h"
#include "eventosSistema.h"
#include "falhasPendentes.h"
/***********************************************************************************
*		Definições com constantes utilizadas no programa
***********************************************************************************/ 
#define NIVEL_SUB_TENSAO                      340
#define NIVEL_SOBRE_TENSAO                    650

/***********************************************************************************
*		Enumerações
***********************************************************************************/
typedef enum{
 NivelNormal,
 FiltroSobreTensao,
 SobreTensao,
 FiltroSairSobreTensao,
 FiltroSubTensao,
 SubTensao,
 FiltroSairSubTensao   
}eTensaoStates;

/***********************************************************************************
*		Estruturas
***********************************************************************************/


/***********************************************************************************
*		Uniões
***********************************************************************************/


/***********************************************************************************
*		Constantes
***********************************************************************************/


/***********************************************************************************
*		Variaveis locais
***********************************************************************************/
unsigned int bufferAmostragemTensao[8]={0,0,0,0,0,0,0,0};
unsigned char ponteiroTensao=0,MONITORA_TENSAO_counter=10;
  

/***********************************************************************************
*		Funções locais
***********************************************************************************/
unsigned int calculaMediaTensao(void);
eTensaoStates decodeStates(eTensaoStates estado);
 
/***********************************************************************************
*		Implementação
***********************************************************************************/

/***********************************************************************************
*   Descrição     :   inicialização do módulo
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void MONITORA_TENSAO_init(void){
  
   
  
}
/***********************************************************************************
*   Descrição     :   Ponto de entrada da monitoração do nível de tensão
*                     da fonte
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void MONITORA_TENSAO_entry(void){
  static eTensaoStates estadoAtual = NivelNormal;  
  
  estadoAtual =  decodeStates(estadoAtual);  
}
/***********************************************************************************
*   Descrição     :   Switch da máquina de estados
*   Parametros    :   (eTensaoStates) estado
*   Retorno       :   (eTensaoStates) retorno
***********************************************************************************/
eTensaoStates decodeStates(eTensaoStates estado){
  unsigned int acc;
 
  switch(estado){
  
    case NivelNormal          : if(!MONITORA_TENSAO_counter){
                                  acc = calculaMediaTensao();
                                  if(acc > NIVEL_SOBRE_TENSAO) {
                                    MONITORA_TENSAO_counter = 3;
                                    estado = FiltroSobreTensao;
                                  }
                                  else
                                    
                                    if(estado<NIVEL_SUB_TENSAO){
                                      MONITORA_TENSAO_counter = 3;
                                      estado = FiltroSubTensao;
                                    }
                                    else
                                      estado = NivelNormal;                                                                    
                                }
                                break;
                                
    case FiltroSobreTensao    : if(!MONITORA_TENSAO_counter){
                                  acc = calculaMediaTensao();
                                  if(acc > NIVEL_SOBRE_TENSAO){
                                    estado = SobreTensao;
                                    //Log das falhas  
                                    
                                    
                                    //Fim do bloco de log
                                  }
                                  else
                                    estado = NivelNormal;                                  
                                } 
                                else
                                  estado = FiltroSobreTensao;
                                break;    
                                
    case SobreTensao          : acc = calculaMediaTensao();
                                if(acc<NIVEL_SOBRE_TENSAO){
                                  MONITORA_TENSAO_counter = 3;
                                  estado = FiltroSairSobreTensao;
                                }
                                break;    
                                
    case FiltroSairSobreTensao: if(!MONITORA_TENSAO_counter){
                                  acc = calculaMediaTensao();
                                  if(acc<NIVEL_SOBRE_TENSAO){
                                    estado = NivelNormal;
                                    //Loga aqui a volta do nível normal de tensão
                                  }
                                }
                                break;    
                                
    case FiltroSubTensao      : if(!MONITORA_TENSAO_counter){
                                  acc = calculaMediaTensao();
                                  if(acc<NIVEL_SUB_TENSAO){
                                    estado = SubTensao;
                                    //Loga aqui a entrada em sub-tensão
                                    
                                    
                                    //Fim do bloco de log  
                                  }
                                  else
                                    if(acc>NIVEL_SUB_TENSAO)
                                      estado = NivelNormal;                                  
                                } 
                                else
                                  estado = NivelNormal;
                                break; 
                                
    case SubTensao            : acc = calculaMediaTensao();
                                if(acc>NIVEL_SUB_TENSAO){
                                  estado = FiltroSairSubTensao;
                                  MONITORA_TENSAO_counter = 3; 
                                }
                                break;    

    case FiltroSairSubTensao  : if(!MONITORA_TENSAO_counter){
                                  acc = calculaMediaTensao();
                                  if(acc> NIVEL_SUB_TENSAO){                                                                       
                                    estado = NivelNormal;
                                    //Loga a normalização do nível do link DC
                                    
                                    
                                    //Fim do bloco de log
                                  } 
                                  else
                                    estado = SubTensao;                                    
                                }
                                break;    
  } 
 
  return estado;  
}
/***********************************************************************************
*   Descrição     :   Tick do módulo de monitoração do nível de tensão
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
#pragma INLINE
void MONITORA_TENSAO_tick(void){
 
  ADC1CFG = 0b01111001;
  APCTL1  = 0x08;
  ADC1SC1 = 0x03;
   
  ADC1SC1 = 0x03;    
  while(!ADC1SC1_COCO);   
  bufferAmostragemTensao[ponteiroTensao] = ADC1R;   
  ponteiroTensao = (unsigned char)((ponteiroTensao +1 ) % 8);
}
/***********************************************************************************
*   Descrição     :   Faz o calculo da média das amostragens do nível de tensão
*                     do barramento DC
*   Parametros    :   nenhum
*   Retorno       :   (unsigned int) Valor médio das amostragens
***********************************************************************************/
unsigned int calculaMediaTensao(void){
  unsigned long int media=0;
  unsigned char i;
  
  for(i=0;i<8;i++)
    media+= bufferAmostragemTensao[i];
  media>>=3;
  
  return (unsigned int) media;     
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/