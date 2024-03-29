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
|       Produto            :  Central Apus CD432
|       Arquivo            :  Lacos.c
|       Descri��o          :  Fun��es para acionamento e monitora��o dos
|                             la�os do sistema
| 
|       Autor              :  Marcos Aqino
|       Data cria��o       :  13/07/2010
|
|       Revis�es           :  1.0.0
|
|
| __________________________________________________________________________________
*/

/***********************************************************************************
*		Includes
***********************************************************************************/
#include <MC9S08AC128.H>
#include "api_lacos.h"
#include "Systick.h"
/***********************************************************************************
*		Defini��es com constantes utilizadas no programa
***********************************************************************************/ 
#define SINAL_BLOQUEIO_01_DIR               PTBDD_PTBDD4
#define SINAL_BLOQUEIO_02_DIR               PTBDD_PTBDD5
#define SINAL_BLOQUEIO_03_DIR               PTCDD_PTCDD4
#define SINAL_BLOQUEIO_04_DIR               PTCDD_PTCDD6

#define SINAL_BLOQUEIO_01                   PTBD_PTBD4
#define SINAL_BLOQUEIO_02                   PTBD_PTBD5
#define SINAL_BLOQUEIO_03                   PTCD_PTCD4
#define SINAL_BLOQUEIO_04                   PTCD_PTCD6

#define TERMINADOR_SIRENE_DIR               PTGDD_PTGDD2
#define CURTO_SIRENE_DIR                    PTGDD_PTGDD3
#define ATIVA_SIRENE_DIR                    PTGDD_PTGDD4

#define TERMINADOR_SIRENE                   PTGD_PTGD2
#define CURTO_SIRENE                        PTGD_PTGD3
#define ATIVA_SIRENE                        PTGD_PTGD4

#define TERMINADOR_SIRENE_AUX_DIR           PTDDD_PTDDD4
#define CURTO_SIRENE_AUX_DIR                PTDDD_PTDDD5
#define ATIVA_SIRENE_AUX_DIR                PTDDD_PTDDD3

#define TERMINADOR_SIRENE_AUX               PTDD_PTDD4
#define CURTO_SIRENE_AUX                    PTDD_PTDD5
#define ATIVA_SIRENE_AUX                    PTDD_PTDD3

#define RELE_01_PIN_DIR                     PTEDD_PTEDD5
#define RELE_02_PIN_DIR                     PTEDD_PTEDD6
#define RELE_03_PIN_DIR                     PTEDD_PTEDD7
#define RELE_04_PIN_DIR                     PTFDD_PTFDD2

#define RELE_01_PIN                         PTED_PTED5
#define RELE_02_PIN                         PTED_PTED6
#define RELE_03_PIN                         PTED_PTED7
#define RELE_04_PIN                         PTFD_PTFD2

/***********************************************************************************
*		Enumera��es
***********************************************************************************/


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
extern unsigned char bufferSinaisLacos[2][10];

/***********************************************************************************
*		Fun��es locais
***********************************************************************************/
 
/***********************************************************************************
*		Implementa��o
***********************************************************************************/

/***********************************************************************************
*   Descri��o     :   Inicializa��o do m�dulo
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void APILACOS_init(void){  
  
  SINAL_BLOQUEIO_01_DIR = 1;
  SINAL_BLOQUEIO_02_DIR = 1;
  SINAL_BLOQUEIO_03_DIR = 1;
  SINAL_BLOQUEIO_04_DIR = 1;

  TERMINADOR_SIRENE_DIR = 0;
  CURTO_SIRENE_DIR      = 0;
  ATIVA_SIRENE_DIR      = 1;


  TERMINADOR_SIRENE_AUX_DIR = 0;
  CURTO_SIRENE_AUX_DIR      = 0;
  ATIVA_SIRENE_AUX_DIR      = 1;
  
  SINAL_BLOQUEIO_01 = 0x01;
  
  RELE_01_PIN = 0;
  RELE_02_PIN = 0;
  RELE_03_PIN = 0;
  RELE_04_PIN = 0;
  
  RELE_01_PIN_DIR = 1;
  RELE_02_PIN_DIR = 1;
  RELE_03_PIN_DIR = 1;
  RELE_04_PIN_DIR = 1; 
}
/***********************************************************************************
*   Descri��o     :   Faz a leitura dos bits do la�o on board
*   Parametros    :   nenhum
*   Retorno       :   (sBitsLaco) Bits de sinaliza��o do la�o
***********************************************************************************/
sBitsLaco APILACOS_getFlagsLacoOnBoard(void){
  unsigned char a,b,c,i;
  sBitsLaco tmp;
  
  a=0;
  b=0;
  c=0;
  
  for(i=0;i<10;i++){    
     if(bufferSinaisLacos[0][i]&0x01)
       a++;
     if(bufferSinaisLacos[0][i]&0x02)
       b++;
     if(bufferSinaisLacos[0][i]&0x04)
       c++;
  }  
  *(unsigned char*)&tmp = 0x00;  
    
  tmp.flagAlarme = 0;
  tmp.flagCurto = 0;
  tmp.flagTerminador = 0;
  
  if(a>5)
    tmp.flagCurto = 1;
  if(b>5)
    tmp.flagAlarme = 1;
  if(c>5)
    tmp.flagTerminador = 1;   
        
  return tmp;  
}
/***********************************************************************************
*   Descri��o     :   Faz a leitura dos bits de monitora��od da sa�da de
*                     sirene onboard
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
sBitsSirene APILACOS_getFlagsSireneOnBoard(void){
  sBitsSirene tmp;
  unsigned char contador[2]={0,0},i;
  
  *(unsigned char*)&tmp = 0x00;
  
  for(i=0;i<5;i++){
    if(!TERMINADOR_SIRENE)
      contador[0]++; 
    if(!CURTO_SIRENE)
      contador[1]++;
    
    SYSTICK_delay(5);
  }
  
  if(contador[0]>2)
    tmp.flagTerminador = 1;
  
  if(contador[1]>2)
    tmp.flagCurto = 1;     
  
  return tmp;
}
/***********************************************************************************
*     Descri��o    :    Setter para os bits de controle dos la�os
*     Parametros   :    (eLacos) laco
*                       (unsigned char) valor 
*     Retorno      :    nenhum
***********************************************************************************/
void APILACOS_setterCoils(eLacos laco,unsigned char valor){
  
  switch(laco){
   case Laco_01: SINAL_BLOQUEIO_01 = valor;
                 SINAL_BLOQUEIO_01_DIR = 1;
                 break;
   case Laco_02: SINAL_BLOQUEIO_02 = valor;
                 SINAL_BLOQUEIO_02_DIR = 1;   
                 break;
   case Laco_03: SINAL_BLOQUEIO_03 = valor;
                 SINAL_BLOQUEIO_03_DIR = 1;   
                 break;
   case Laco_04: SINAL_BLOQUEIO_04 = valor;
                 SINAL_BLOQUEIO_04_DIR = 1;           
                 break;
  }    
}
/***********************************************************************************
*     Descri��o   :   Monitora os bits do la�o 02
*     Parametros  :   (nenhum)
*     Retorno     :   (sBitsLaco) flag do la�o
***********************************************************************************/
sBitsLaco APILACOS_getFlagsLaco_02(void){
  unsigned char a,b,c,i;
  sBitsLaco tmp;
  
  a=0;
  b=0;
  c=0;
  
  for(i=0;i<10;i++){    
     if(bufferSinaisLacos[0][i]&0x08)
       a++;
     if(bufferSinaisLacos[0][i]&0x10)
       b++;
     if(bufferSinaisLacos[0][i]&0x20)
       c++;
  }  
  *(unsigned char*)&tmp = 0x00;  
    
  tmp.flagAlarme = 0;
  tmp.flagCurto = 0;
  tmp.flagTerminador = 0;
  
  if(a>5)
    tmp.flagCurto = 1;
  if(b>5)
    tmp.flagAlarme = 1;
  if(c>5)
    tmp.flagTerminador = 1;   
        
  return tmp;  
} 
/***********************************************************************************
*     Descri��o   :   Monitora os bits do la�o 03
*     Parametros  :   (nenhum)
*     Retorno     :   (sBitsLaco) flag do la�o
***********************************************************************************/
sBitsLaco APILACOS_getFlagsLaco_03(void){
  unsigned char a,b,c,i;
  sBitsLaco tmp;
  
  a=0;
  b=0;
  c=0;
  
  for(i=0;i<10;i++){    
     if(bufferSinaisLacos[1][i]&0x01)
       a++;
     if(bufferSinaisLacos[1][i]&0x02)
       b++;
     if(bufferSinaisLacos[1][i]&0x04)
       c++;
  }  
  *(unsigned char*)&tmp = 0x00;  
    
  tmp.flagAlarme = 0;
  tmp.flagCurto = 0;
  tmp.flagTerminador = 0;
  
  if(a>5)
    tmp.flagCurto = 1;
  if(b>5)
    tmp.flagAlarme = 1;
  if(c>5)
    tmp.flagTerminador = 1;   
        
  return tmp;  
}
/***********************************************************************************
*     Descri��o   :   Monitora os bits do la�o 04
*     Parametros  :   (nenhum)
*     Retorno     :   (sBitsLaco) flag do la�o
***********************************************************************************/
sBitsLaco APILACOS_getFlagsLaco_04(void){
  unsigned char a,b,c,i;
  sBitsLaco tmp;
  
  a=0;
  b=0;
  c=0;
  
  for(i=0;i<10;i++){    
     if(bufferSinaisLacos[1][i]&0x08)
       a++;
     if(bufferSinaisLacos[1][i]&0x10)
       b++;
     if(bufferSinaisLacos[1][i]&0x20)
       c++;
  }  
  *(unsigned char*)&tmp = 0x00;  
    
  tmp.flagAlarme = 0;
  tmp.flagCurto = 0;
  tmp.flagTerminador = 0;
  
  if(a>5)
    tmp.flagCurto = 1;
  if(b>5)
    tmp.flagAlarme = 1;
  if(c>5)
    tmp.flagTerminador = 1;   
        
  return tmp;  
}
/***********************************************************************************
*   Descri��o     :   Fun��o p�blica para acesso aos flags dos la�os
*   Parametros    :   ( eLacos) N�mero do la�o
*   Retorno       :   ( sBitsLaco) Flags do la�o
***********************************************************************************/
sBitsLaco APILACOS_getFlags(eLacos laco){
  
  switch(laco){
    case Laco_01: return APILACOS_getFlagsLacoOnBoard(); 
    case Laco_02: return APILACOS_getFlagsLaco_02();
    case Laco_03: return APILACOS_getFlagsLaco_03();
    case Laco_04: return APILACOS_getFlagsLaco_04();
    default     : return APILACOS_getFlagsLacoOnBoard();
  }      
}
/***********************************************************************************
*   Descri��o     :   Monitora a sa�da de sirene auxiliar
*   Parametros    :   nenhum
*   Retorno       :   
***********************************************************************************/
sBitsSirene APILACOS_getFlagSaidaSireneAuxiliar(void){
  sBitsSirene tmp;
  unsigned char contador[2]={0,0},i;
  
  *(unsigned char*)&tmp = 0x00;
  
  for(i=0;i<5;i++){
    if(!TERMINADOR_SIRENE_AUX)
      contador[0]++; 
    if(!CURTO_SIRENE_AUX)
      contador[1]++;
    
    SYSTICK_delay(5);
  }
  
  if(contador[0]>2)
    tmp.flagTerminador = 1;
  
  if(contador[1]>2)
    tmp.flagCurto = 1;     
  
  return tmp;       
}
/***********************************************************************************
*   Descri��o     :   Monitora os bits das sa�das de sirene do sistema
*   Parametros    :   (eSaidasSirene) sa�da
*   Retorno       :   (sBitsSirene) flags da sa�da de sirene
***********************************************************************************/
sBitsSirene APILACOS_getFlagSirenes(eSaidasSirene saida){
  
  switch(saida){
    case SaidaSireneOnboard : return APILACOS_getFlagsSireneOnBoard();
    
    case SaidaSireneAuxiliar: return APILACOS_getFlagSaidaSireneAuxiliar();
    
    default                 : return APILACOS_getFlagsSireneOnBoard();
  }      
}
/***********************************************************************************
*   Descri��o     :   Faz o acionamento das sa�das de sirene do sistema
*   Parametros    :   (eSaidasSirene) saida
*                     (unsigned char) valor
*   Retorno       :   (eResAcionamento) resultado do acionamento
***********************************************************************************/
eResAcionamento APILACOS_acionaSaida(eSaidasSirene saida,unsigned char valor){
  sBitsSirene flags;
  
  switch(saida){
    case SaidaSireneOnboard :
                              flags = APILACOS_getFlagSirenes(SaidaSireneOnboard);
                              if(valor){
                                
                                if(!flags.flagCurto){ 
                                  ATIVA_SIRENE_DIR = 1;                               
                                  ATIVA_SIRENE = 1;           
                                  return SaidaAcionadaComSucesso;
                                }
                                else
                                  return ErroAoAcionarSaida;                                                                
                              }
                              else{
                                ATIVA_SIRENE = 0; 
                                return Desligado;
                              }    
    case SaidaSireneAuxiliar:        
    default                 : 
                              flags = APILACOS_getFlagSirenes(SaidaSireneAuxiliar);
                              if(valor){
                                
                                if(!flags.flagCurto){ 
                                  ATIVA_SIRENE_AUX_DIR = 1;                               
                                  ATIVA_SIRENE_AUX = 1;           
                                  return SaidaAcionadaComSucesso;
                                }
                                else
                                  return ErroAoAcionarSaida;                                                                
                              }
                              else{
                                ATIVA_SIRENE_AUX = 0;
                                return Desligado;       
                              }
    
  }    
}
/***********************************************************************************
*   Descri��o        :    Faz o acionamento de um dos rel�s do sistema
*   Parametros       :    (eRele) rel�s
*   Retorno          :    nenhum
***********************************************************************************/
void APILACOS_acionaRele(eRele rele,unsigned char valor){
 
  switch(rele){
    case Rele_01: RELE_01_PIN = (valor)?1:0;
                  RELE_01_PIN_DIR = 1;
                  break; 
    case Rele_02: RELE_02_PIN = (valor)?1:0;
                  RELE_02_PIN_DIR = 1;    
                  break;    
    case Rele_03: RELE_03_PIN = (valor)?1:0;
                  RELE_03_PIN_DIR = 1;    
                  break;                      
    case Rele_04: RELE_04_PIN = (valor)?1:0;
                  RELE_04_PIN_DIR = 1;    
                  break;
  }    
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/