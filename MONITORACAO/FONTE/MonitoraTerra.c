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
|       Arquivo            :  MonitoraTerra.c
|       Descri��o          :  Faz a monitora��o do n�vel de tens�o do aterramento
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  23/03/2011
|
|       Revis�es           :  1.0
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*		Includes
***********************************************************************************/
#include <MC9S08AC128.H>
#include "Monitoraterra.h"
#include "systick.h"
#include "eventosSistema.h"
#include "falhasPendentes.h"

/***********************************************************************************
*		Defini��es com constantes utilizadas no programa
***********************************************************************************/ 
#define NIVEL_FUGA_TERRA_NEGATIVO             180
#define NIVEL_FUGA_TERRA_POSITIVO             510

/***********************************************************************************
*		Enumera��es
***********************************************************************************/
typedef enum{
  TerraNormal,
  FiltroFugaPositivo,
  FugaPositivo,
  FiltroSaidaFugaPositivo,
  FiltroFugaNegativo,
  FugaNegativo,
  FiltroSaidaFugaNegativo  
}eEarthStates;
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
unsigned char contatorTerra=10,ponteiroTerra=0;
unsigned int bufferTerra[8]={0,0,0,0,0,0,0,0};

/***********************************************************************************
*		Fun��es locais
***********************************************************************************/
eEarthStates decodificaEstados(eEarthStates estado);
unsigned int calculaMediaAterramento(void);
 
/***********************************************************************************
*		Implementa��o
***********************************************************************************/

/***********************************************************************************
*   Descri��o     :   Inicializa��o do m�dulo
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void MONITORA_TERRA_init(void){
   
  
  
}
/***********************************************************************************
*   Descri��o     :   Ponto de entrada do m�dulo de monitora��o do n�vel do 
*                     aterramento
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void MONITORA_TERRA_entry(void){
  static eEarthStates estadoAtual = TerraNormal;
  
  estadoAtual = decodificaEstados(estadoAtual);      
}
/***********************************************************************************
*   Descri��o     :   Switch da m�quina de estados de monitora��o do n�vel
*                     do aterramento
*   Parametros    :   (eEarthStates) estado atual
*   Retorno       :   (eEarthStates) novo estado
***********************************************************************************/
eEarthStates decodificaEstados(eEarthStates estado){
  unsigned int acc;
 
  switch(estado){
    // Sai do estado terra normal quando
    // 1) Houver nivel acima do n�vel m�ximo
    // 2) Houver n�vel menor do que o n�vel m�nimo
    case TerraNormal            : if(!contatorTerra){
                                    acc = calculaMediaAterramento();
                                    if(acc>NIVEL_FUGA_TERRA_POSITIVO){                                      
                                      estado = FiltroFugaPositivo;
                                      contatorTerra = 3;
                                    }
                                    else
                                      if(acc<NIVEL_FUGA_TERRA_NEGATIVO){                                        
                                        estado = FiltroFugaNegativo;
                                        contatorTerra = 3;                                        
                                      }
                                      else
                                        estado = TerraNormal;       
                                  }
                                  break;    
    // Sai do estado filtro positivo para
    // 1) Terra normal se ao termino da contagem o 
    // 2) fuga ao terra positivo se ao final da contagem 
    //    o nivel estiver acima do parametro fuga ao positivo                                  
    case FiltroFugaPositivo     : if(!contatorTerra){
                                    acc = calculaMediaAterramento();
                                    if(acc> NIVEL_FUGA_TERRA_POSITIVO){
                                      estado = FugaPositivo;
                                      //Loga a fuga ao positivo
                                      
                                      //Fim do log  
                                    }
                                    else
                                      estado = TerraNormal;                                                                                
                                  }    
                                  break;    
    // Said do estado fuga posigito para:
    // 1) Filtro saida fuga ao positivo se detectar nivel abaixo                                   
    case FugaPositivo           : acc = calculaMediaAterramento();
                                  if(acc <NIVEL_FUGA_TERRA_POSITIVO){
                                    contatorTerra = 3;
                                    estado = FiltroSaidaFugaPositivo; 
                                  }
                                  break;    
    // Saida do estrado de filtro saida fuga ao positivo                                  
    case FiltroSaidaFugaPositivo: if(!contatorTerra){
                                    acc = calculaMediaAterramento();
                                    if(acc< NIVEL_FUGA_TERRA_POSITIVO){
                                      estado = TerraNormal;
                                      //Loga a normaliza��o do terra
                                      
                                      //Fim do log   
                                    }
                                  }
                                  break;    
    // Saida do filtro fuga negativo                                  
    case FiltroFugaNegativo     : if(!contatorTerra){
                                    acc = calculaMediaAterramento();
                                    if(acc>NIVEL_FUGA_TERRA_NEGATIVO){
                                      estado =  FugaNegativo;
                                      //Loga a fuga ao terra no negativo
                                      
                                      
                                      //Fim do log 
                                    } 
                                    else
                                      estado = TerraNormal; 
                                  }
                                  break;    
    // Fuga ao negativo do terra                                  
    case FugaNegativo           : acc = calculaMediaAterramento();
                                  if(acc>NIVEL_FUGA_TERRA_NEGATIVO){
                                    estado =  FiltroSaidaFugaNegativo;
                                    contatorTerra = 3; 
                                  }
                                  break;    
    // Filtro de sa�da da fuga ao negativo                                  
    case FiltroSaidaFugaNegativo: if(!contatorTerra){
                                    acc = calculaMediaAterramento();
                                    if(acc>NIVEL_FUGA_TERRA_NEGATIVO){
                                      estado = TerraNormal;
                                      //Loga o normaliza��o do terra
                                      
                                      
                                      //FIm do log  
                                    }                                    
                                  }
                                  break;
  }
 
  return estado;  
}        
/***********************************************************************************
*   Descri��o     :   Tick da monitora��o do n�vel do terra
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
#pragma INLINE
void MONITORA_TERRA_tick(void){

  ADC1CFG = 0b01111001;
  APCTL1  = 0x01;
  ADC1SC1 = 0x00;
  ADC1SC1 = 0x00;    
  while(!ADC1SC1_COCO); 
  bufferTerra[ponteiroTerra] = ADC1R; 
  ponteiroTerra = (unsigned char)( (ponteiroTerra + 1) % 8);    
}
/***********************************************************************************
*   Descri��o     :   Faz o c�lculo da m�dia da amostragem do n�vel do 
*                     aterramento
*   Parametros    :   nenhum
*   Retorno       :   (unsigned int) m�dia da amostragem
***********************************************************************************/
unsigned int calculaMediaAterramento(void){
  unsigned long int media=0;
  unsigned char i;
  
  for(i=0;i<8;i++)
    media |= bufferTerra[i];
  
  media>>=3;     
  return (unsigned int) media;  
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/