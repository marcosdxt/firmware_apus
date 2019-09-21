/*__________________________________________________________________________________
|     	Chave Digital Tecnologia Eletronica Ltda. 
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
|       Arquivo            :  lacos.c
|       Descri��o          :  m�quina de estados que faz a monitora��o dos la�os
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  18/03/2011
|
|       Revis�es           :  1.0
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*		Includes
***********************************************************************************/
#include "appconfig.h"
#include "lacos.h"
#include "exthw.h"
#include "api_lacos.h"
#include "parametros.h"
#include "eventosSistema.h"
#include "falhasPendentes.h"
#include "setupDecoder.h"
/***********************************************************************************
*		Defini��es com constantes utilizadas no programa
***********************************************************************************/ 



/***********************************************************************************
*		Enumera��es
***********************************************************************************/
typedef enum{
  eLacoNormal,
  eFiltroFaltaTerminador,
  eFaltaTerminador,
  eFiltroVoltaTerminador,
  eFiltroAlarmeAnalogico,
  eAlarmeAnalogico,
  eResetGlobal,
  eCurcoCircuito,
  eFiltroCurtoCircuito,
  eFiltroSaidaCurto 
}eEstadosLaco;
/***********************************************************************************
*		Estruturas
***********************************************************************************/
typedef struct{
  unsigned char laco_01:1;
  unsigned char laco_02:1;
  unsigned char laco_03:1;
  unsigned char laco_04:1;  
}sBitsLacos;
/***********************************************************************************
*		Uni�es
***********************************************************************************/


/***********************************************************************************
*		Constantes
***********************************************************************************/


/***********************************************************************************
*		Variaveis locais
***********************************************************************************/
eEstadosLaco estadosAtuais[4]= {eLacoNormal,eLacoNormal,eLacoNormal,eLacoNormal};
unsigned char LACOS_fitroTempo[4] = {250,250,250,250},LACOS_almEnd[4]={0,0,0,0};
sBitsLacos flagAlarme;

/***********************************************************************************
*		Fun��es locais
***********************************************************************************/  
eEstadosLaco LACOS_machineMemoryLess(unsigned char laco,eEstadosLaco estado);


/***********************************************************************************
*		Implementa��o
***********************************************************************************/

/***********************************************************************************
*   Descri��o     :   Faz a inicializa��o dos la�os
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void LACOS_init(void){
  unsigned char i;
  
  APILACOS_init();
  *(unsigned char*)&flagAlarme = 0x00;
  
  //Habilita os decodificadores de endere�o
  for(i=0;i<3;i++)
    if(!PARAMETROS_leBloqueio(i) && PARAMETROS_carregaConfigLaco(i))
      SETUPDECODER_enable(i,1);  
}
/***********************************************************************************
*   Descri��o     :   Fun��o principal 
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void LACOS_entry(void){
  unsigned char i; 
 
  for(i=0;i<3;i++)
    if(!(*(unsigned char*)&flagAlarme & (0x01<<i)) && !PARAMETROS_leBloqueio(i) && PARAMETROS_carregaConfigLaco(i)) 
      estadosAtuais[i] = (eEstadosLaco)LACOS_machineMemoryLess(i,estadosAtuais[i]);         
}
/***********************************************************************************
*   Descri��o     :   M�quina de est�vel com mem�ria externa
*   Parametros    :   (eEstadosLaco) Estado atual
*   Retorno       :   (eEstadosLaco) novo estado
***********************************************************************************/
eEstadosLaco LACOS_machineMemoryLess(unsigned char laco,eEstadosLaco estado){
  sBitsLaco flags;
  
  flags = APILACOS_getFlags(laco);
  
  switch(estado){
    // Comuta do estado laco normal para:
    //
    // a) filtro falta terminador: Caso o flag terminador esteja em zero
    // b) filtro alarme anal�gico: Caso o flag de alarme esteja em Um e o de curto em zero
    // c) filtro curto-circuito  : Caso o flag de curto-circuito esteja em um
    case eLacoNormal           : if(LACOS_fitroTempo[laco])
                                   break;
    
                                 if(!flags.flagTerminador){
                                   LACOS_fitroTempo[laco] = 200;
                                   estado = eFiltroFaltaTerminador;  
                                 }
                                 else
                                   if(flags.flagAlarme && !flags.flagCurto){
                                     LACOS_fitroTempo[laco] = 200;
                                     estado = eFiltroAlarmeAnalogico;
                                   } 
                                   else
                                     if(flags.flagCurto){
                                       LACOS_fitroTempo[laco] = 200;
                                       estado = eFiltroCurtoCircuito; 
                                     }                                 
                                 break;
    // Comuta do estado Filtro falta terminador para:
    //
    // a) LacoNormal      : Caso quando a contagem do filtro chegue a zero o flag do terminador esteja em um
    // b) Falta Terminador: Caso o bit do terminador esteja em zero quando o filtro zerar.
    case eFiltroFaltaTerminador: if(!LACOS_fitroTempo[laco]){
                                   if(flags.flagTerminador)
                                     estado = eLacoNormal;
                                   else
                                     estado = eFaltaTerminador;       
                                     //Ponto onde foi detectada a falta do termindor no la�o
                                     FP_insereFalha(FaltaTerminador,laco);
                                     (void)EVSY_insereDefeito(FaltaTerminador,laco,0);
                                 }
                                 break;                                     
    // Comutado do estado falta do terminador para:
    //
    // a) Filtro volta terminador : Caso seja detectado a presen�a do terminador atrav�s do bit
    // b) Filtro alarme anal�gico : Caso seja detectado o bit de alarme setado e o de curto zerado
    // c) Filtro curto-circuito   : Caso seja detectado o bit de curto-circuito setado                                 
    case eFaltaTerminador      : if(flags.flagTerminador && !flags.flagAlarme && !flags.flagCurto){
                                   LACOS_fitroTempo[laco] = 200;
                                   estado = eFiltroVoltaTerminador;
                                 }
                                 else{
                                   if(flags.flagAlarme && !flags.flagCurto){
                                     LACOS_fitroTempo[laco] = 200;
                                     estado =  eFiltroAlarmeAnalogico;
                                   }
                                   else {
                                     if(flags.flagCurto){                                      
                                       LACOS_fitroTempo[laco] = 200;
                                       estado = eFiltroCurtoCircuito; 
                                     }
                                   }                                   
                                 }
                                 break;    
    // Comuta do estado Filtro volta terminador para :
    //
    // a) La�o normal     : Quando acabada a contagem for detectado que o flag de falta do terminador est� setado.
    // b) Filtro alarme   : Quando ap�s zerada a contagem, detectado o flag de alarme setado.
    // c) Curto-circuito  : Quando finalizada a contagem, detectado o flag de curto-circuito setado.                                 
    case eFiltroVoltaTerminador: if(!LACOS_fitroTempo[laco]){
                                   if(flags.flagTerminador && !flags.flagCurto && !flags.flagAlarme){                                    
                                     estado = eLacoNormal;
                                     //Ponto onde foi detectado a conex�o do terminador no la�o
                                     FP_removeFalha(FaltaTerminador,laco);
                                     (void)EVSY_insereDefeito(LacoNormalizado,laco,0);                                     
                                   }
                                   else{
                                     if(flags.flagAlarme && !flags.flagCurto){
                                       estado = eFiltroAlarmeAnalogico;
                                       LACOS_fitroTempo[laco] = 200; 
                                     }
                                     else
                                       if(flags.flagCurto){
                                         estado =  eFiltroCurtoCircuito;
                                         LACOS_fitroTempo[laco] = 200;
                                       }
                                       else
                                         if(!flags.flagTerminador)
                                           estado = eFaltaTerminador;                                                                           
                                   }
                                 }
                                 break;    
    // Comuta do estado alarme no anal�gico para :
    //
    // a) Alarme anal�gico  : Caso ao final da contagem seja detectado que o flag alarme est� setado
    // b) Laco normal       : Caso ao final da contagem seja detectado que o flag alarme est� em zero                                 
    case eFiltroAlarmeAnalogico: if(!LACOS_fitroTempo[laco]){
                                   if(flags.flagAlarme && !flags.flagCurto){
                                     estado = eAlarmeAnalogico; 
                                     if(!(*(unsigned char*)&flagAlarme&(0x01<<laco))){
                                       *(unsigned char*)&laco |= 0x01<<laco; 
                                       (void)EVSY_insereAlarme(laco,0);                                       
                                     }
                                   }
                                   else
                                     if(!flags.flagAlarme)
                                       estado = eLacoNormal;                                   
                                 }
                                 break;   
    // No estado alarme n�o faz nada, pois j� est� em procedimento de alarme no display
    // 
    case eAlarmeAnalogico      :
                                 break;       
    // Comuta do estado de curto-circuito para:
    //
    // a)  Quando o flag curto estiver zero passa para o estado filtro saida curto
    case eCurcoCircuito        :
                                 if(!flags.flagCurto){
                                   LACOS_fitroTempo[laco] = 200;
                                   estado = eFiltroSaidaCurto; 
                                 }                                   
                                 break;    
    // Comuta para do esado filtro curto-circuito para:
    //
    // a) Curto-circuito    : Quando o flag curto estiver setado ap�s a contagem chegar a zero
    // b) Laco-normal       : Se ap�s a finaliza��o da contagem o flag de curto estiver zerado                                 
    case eFiltroCurtoCircuito  : if(!LACOS_fitroTempo[laco]){
                                   if(flags.flagCurto){                                    
                                     estado = eCurcoCircuito;
                                     //Ponto onde foi detectado o curto-circuito no la�o
                                     FP_insereFalha(CurtoCircuitoLaco,laco);
                                     (void)EVSY_insereDefeito(CurtoCircuitoLaco,laco,0);  
                                   }
                                   else
                                     estado = eLacoNormal;
                                   
                                 }    
                                 break;    
    // Comuta para do estado filtro saida para :
    //
    // a) La�o normal       : quando chegar ao sinal da contagem e o curto n�o estiver mais ali
    // b) Curto-circuito    : 
    case eFiltroSaidaCurto     :
                                 if(!LACOS_fitroTempo[laco]){
                                   if(!flags.flagCurto){
                                     estado = eLacoNormal;                                      
                                     //Ponto onde foi detectado a normaliza��o do la�o
                                     FP_removeFalha(CurtoCircuitoLaco,laco);
                                     (void)EVSY_insereDefeito(LacoNormalizado,laco,0);  
                                   }
                                   else
                                     estado = eCurcoCircuito;                                                                      
                                 }
                                 break;    
  }       
  
  return estado;
}
/***********************************************************************************
*   Descri��o     :   Seta o flag de alarme do la�o
*   Parametros    :   (unsigned char) n�mero do la�o
*   Parametros    :   nenhum
***********************************************************************************/
void LACOS_setAlarme(unsigned char laco){
    
   *(unsigned char*)&flagAlarme |= 0x01<<laco;
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/