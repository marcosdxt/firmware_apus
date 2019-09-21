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
|       Este c�digo � propriedade da Chave Digital e n�o pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Chave Digital ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Arquivo            : MonitoraTensao.c
|       Descri��o          : Faz a monitora��o do nivel de tens�o sob o capacitor
| 
|       Autor              : Marcos Aquino
|       Data cria��o       : 23/03/2011
|
|       Revis�es           : 1.0
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
*		Defini��es com constantes utilizadas no programa
***********************************************************************************/ 
#define NIVEL_SUB_TENSAO                      340
#define NIVEL_SOBRE_TENSAO                    650

/***********************************************************************************
*		Enumera��es
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
*		Uni�es
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
*		Fun��es locais
***********************************************************************************/
unsigned int calculaMediaTensao(void);
eTensaoStates decodeStates(eTensaoStates estado);
 
/***********************************************************************************
*		Implementa��o
***********************************************************************************/

/***********************************************************************************
*   Descri��o     :   inicializa��o do m�dulo
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void MONITORA_TENSAO_init(void){
  
   
  
}
/***********************************************************************************
*   Descri��o     :   Ponto de entrada da monitora��o do n�vel de tens�o
*                     da fonte
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void MONITORA_TENSAO_entry(void){
  static eTensaoStates estadoAtual = NivelNormal;  
  
  estadoAtual =  decodeStates(estadoAtual);  
}
/***********************************************************************************
*   Descri��o     :   Switch da m�quina de estados
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
                                    //Loga aqui a volta do n�vel normal de tens�o
                                  }
                                }
                                break;    
                                
    case FiltroSubTensao      : if(!MONITORA_TENSAO_counter){
                                  acc = calculaMediaTensao();
                                  if(acc<NIVEL_SUB_TENSAO){
                                    estado = SubTensao;
                                    //Loga aqui a entrada em sub-tens�o
                                    
                                    
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
                                    //Loga a normaliza��o do n�vel do link DC
                                    
                                    
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
*   Descri��o     :   Tick do m�dulo de monitora��o do n�vel de tens�o
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
*   Descri��o     :   Faz o calculo da m�dia das amostragens do n�vel de tens�o
*                     do barramento DC
*   Parametros    :   nenhum
*   Retorno       :   (unsigned int) Valor m�dio das amostragens
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