/*__________________________________________________________________________________
|
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
|       Arquivo            : MonitoraRedeBat.c
|       Descri��o          : Fun��es para monitora��o da rede el�trica e 
|                            bateria
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
#include "monitoraRedeBat.h"
#include "eventosSistema.h"
#include "falhasPendentes.h"

/***********************************************************************************
*		Defini��es com constantes utilizadas no programa
***********************************************************************************/ 
#define MODULA_BATERIA_PIN_DIR                PTCDD_PTCDD2
#define MODULA_BATERIA_PIN                    PTCD_PTCD2

#define SINAL_FALTA_BATERIA_DIR               PTGDD_PTGDD0
#define SINAL_FALTA_BATERIA                   PTGD_PTGD0

#define SINAL_FALTA_REDE_DIR                  PTGDD_PTGDD1
#define SINAL_FALTA_REDE                      PTGD_PTGD1

#define FLAG_REDE                             0x01
#define FLAG_BATERIA                          0x02
/***********************************************************************************
*		Enumera��es
***********************************************************************************/
typedef enum{
  FonteNormal,
  FiltroFaltaRede,
  FaltaRede,
  FiltroVoltaRede,
  FiltroFaltaBateria,
  FaltaBat,
  FiltroVoltaBateria  
}eFonteState;
/***********************************************************************************
*		Estruturas
***********************************************************************************/


/***********************************************************************************
*		Uni�es
***********************************************************************************/


/***********************************************************************************
*		Constantes
***********************************************************************************/
const unsigned char tabelaEstadoPino[]= 
{
   1,1,1,1,1,1,1,1, // 1  
   1,1,1,1,1,1,1,1, // 2
   1,1,1,1,1,1,1,1, // 3   
   0,0,0,0,0,0,0,0, // 4
};
/***********************************************************************************
*		Variaveis locais
***********************************************************************************/
unsigned char bufferAmostragem[8],
              ponteiroAmostragem=0,
              ponteiroPino=0;

/***********************************************************************************
*		Fun��es locais
***********************************************************************************/
eFonteState MONITORA_REDE_BAT_decodeState(eFonteState estado);
unsigned char MONITORA_REDE_BAT_getFlags(void);
unsigned int contadorCiclos=12000; 
/***********************************************************************************
*		Implementa��o
***********************************************************************************/

/***********************************************************************************
*   Descri��o     :   Inicializa��o do m�dulo
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void MONITORA_REDE_BAT_init(void){
 
  MODULA_BATERIA_PIN_DIR = 1;
  SINAL_FALTA_BATERIA_DIR = 0;
  SINAL_FALTA_REDE_DIR = 0;
}
/***********************************************************************************
*   Descri��o     :   Tick do m�dulo
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
#pragma INLINE
void MONITORA_REDE_BAT_tick(void){
  static unsigned char counter=50;
   
  MODULA_BATERIA_PIN = tabelaEstadoPino[(ponteiroPino++)&0x1F];   
  
  if(contadorCiclos)
    contadorCiclos--;
  
  if(counter){    
    counter--;
    return;  
  }
    
  if(MODULA_BATERIA_PIN && !counter){
    counter=50;    
    bufferAmostragem[ponteiroAmostragem] = (SINAL_FALTA_BATERIA&0x01);
    bufferAmostragem[ponteiroAmostragem]<<=1;
    bufferAmostragem[ponteiroAmostragem]|= (~(SINAL_FALTA_REDE&0x01))&0x01;
    
    ponteiroAmostragem = (unsigned char)(ponteiroAmostragem + 1) % 8;       
  }
}
/***********************************************************************************
*   Descri��o     :   M�quina de estados da supervis�o da rede e bateria
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void MONITORA_REDE_BAT_entry(void){
  static eFonteState estadoAtual = FonteNormal; 
 
  estadoAtual = MONITORA_REDE_BAT_decodeState(estadoAtual);   
}
/***********************************************************************************
*   Descri��o     :   Switch da maquina de estados da supervis�o da bateria
*   Parametros    :   (eFonteState) estado atual
*   Retorno       :   (eFonteState) novo estado
***********************************************************************************/
eFonteState MONITORA_REDE_BAT_decodeState(eFonteState estado){
  unsigned char acc;
 
  switch(estado){    
    // Faz a monitora��o da fonte e comuta para:
    // 1) Filtro falta rede: Caso o flag FR e FB estejam zerados
    // 2) Filtro falta bat : CAso o flag FB esteja zerado
    case FonteNormal       : if(!contadorCiclos){
                               acc = MONITORA_REDE_BAT_getFlags();
                               
                               if(!acc){
                                 contadorCiclos=2000;
                                 estado = FiltroFaltaRede;
                                 break;
                               }
                               
                               if(!(acc & FLAG_BATERIA)){
                                 contadorCiclos=5000;                                
                                 estado = FiltroFaltaBateria;
                                 break;
                               }
                             }
                             else
                               estado = FonteNormal; 
                             break;
    // Faz a confirma��o da falta de rede el�trica e comuta para:
    // 1) Fonte normal  : Caso ap�s o tempo de filtro seja constatado
    //                    que os flags FB e FR est�o setados
    // 2) Falta rede    : Caso ap�s o fim da contagem os flags FB e FR estejam apagados
    case FiltroFaltaRede   : if(!contadorCiclos){                               
                               acc = MONITORA_REDE_BAT_getFlags();                               
                               if(!acc){
                                 estado = FaltaRede;
                                 // Ponto para logar a falta de rede el�trica                                 
                                 (void)EVSY_insereDefeito(FaltaRedeEletrica,0,0);  
                                 FP_insereFalha(FaltaRedeEletrica,0);                                  
                                 // Falta inserir bloco do Status e controle do led
                                                                 
                               }
                               else
                                 estado = FonteNormal;
                             } 
                             else    
                               estado = FiltroFaltaRede;
                             break;                                     
    // Estado de sinaliza��o da falta de rede el�trica no sistema e comuta para:
    // 1) Filtro volta rede  : Caso sejam detectados os sinais de FB e FR
    // 2) Fonte normal       : Caso seja detecta a presen�a de rede el�trica
    case FaltaRede         : acc = MONITORA_REDE_BAT_getFlags();
                             if(!acc){                              
                               estado = FaltaRede;
                               break;
                             }
                             else{
                               contadorCiclos = 500;
                               estado = FiltroVoltaRede;
                               break;
                             }                             
                             break;                                 
    // Estado onde � confirmado a volta da rede el�trica, ou n�o. Comuta para:
    // 1) Fonte normal : Caso o sinal de rede el�trica esteja setado
    // 2) Falta rede   : Caso o sinal de rede esteja zerado
    case FiltroVoltaRede   : if(!contadorCiclos){
                               acc = MONITORA_REDE_BAT_getFlags();
                               if(acc){                                
                                 estado = FonteNormal;
                                 // Ponto para logar a normaliza��o da fonte
                                 FP_removeFalha(FaltaRedeEletrica,0);
                                 (void)EVSY_insereDefeito(NormalizadaRedeEletrica,0,0);                                   
                                 // Falta inserir chamada para o status e controle do led
                                 
                                 break;
                               }
                               else
                                 estado = FaltaRede;      
                             } 
                             else
                               estado = FiltroVoltaRede;
                             break;    
    // Estado onde � confirmada a falta da bateria e comuta para :
    // 1)  Fonte normal caso o sinal de bateria seja detectado
    // 2) Falta bateria caso o sinal de bateria esteja zerado    
    case FiltroFaltaBateria: if(!contadorCiclos){
                               acc = MONITORA_REDE_BAT_getFlags();
                               if(!(acc&FLAG_BATERIA)){                                
                                 estado = FaltaBat;
                                 // Ponto para logar a falta de bateria
                                 (void)EVSY_insereDefeito(FaltaBateria,0,0); 
                                 FP_insereFalha(FaltaBateria,0);                                 
                                 // Falta inserir o controle do status e do led
                                 
                                 break;
                               }
                               else
                                 estado = FonteNormal;      
                             }
                             else
                               estado = FiltroFaltaBateria;                               
                             break;    
    // Estado onde � sinalizada a falta de bateria e comuta para:
    // 1) Filtro volta bateria : Caso seja detectado o sinal de bateria    
    case FaltaBat          : acc = MONITORA_REDE_BAT_getFlags();
                             if((acc&FLAG_BATERIA) || !acc){
                              contadorCiclos = 500;
                              estado = FiltroVoltaBateria;
                              break;
                             }
                             else
                               estado = FaltaBat;
                             break;    
    // Estado onde � confirmada a volta da bateria e comuta para:
    // 1) Falta bat: Caso n�o seja detectado o sinal de falta de bateria
    // 2) Fonte normal: Caso seja detectado o sinal de falta de bateria
    case FiltroVoltaBateria: acc = MONITORA_REDE_BAT_getFlags();
                             if( (acc & FLAG_BATERIA) || !acc){                              
                               estado = FonteNormal;
                               // Ponto para logar a normaliza��o da bateria
                               (void)EVSY_insereDefeito(NormalizadaBateria,0,0);      
                               FP_removeFalha(FaltaBateria,0);                               
                               
                               // Falta inserir a chamada para controle do status e dos leds                                                              
                               break;
                             }
                             else
                               estado = FiltroVoltaBateria;
                             break;    
  }
  
  return estado;
}
/***********************************************************************************
*   Descri��o     :   Veririca se os sinais de rede e bateria est�o presentes
*   Parametros    :   nenhum
*   Retorno       :   (unsigned char) flag de rede e flag de bateria
***********************************************************************************/
unsigned char MONITORA_REDE_BAT_getFlags(void){
  unsigned char i,retorno=0,b=0,c=0;  
  
  
  for(i=0;i<8;i++){
     if(bufferAmostragem[i]&0x01)
       b++;
     if(bufferAmostragem[i]&0x02)
       c++;    
  }  
  
  if(b>4)
    retorno |= FLAG_REDE;
  if(c>4)
    retorno |= FLAG_BATERIA;
  
  return retorno;
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/