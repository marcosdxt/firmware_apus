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
|       Este código é propriedade da Chave Digital e não pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Chave Digital ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Arquivo            :  aplicacao.c
|       Descrição          :  máquina de estados da supervisão para o usuário
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  17/03/2011
|
|       Revisões           :  1.0
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
#include "aplicacao.h"
#include "sis_normal.h"
#include "pre_alarme.h"
#include "alarme_geral.h"
#include "falhas.h"
#include "mostra_falhas.h"
#include "mostra_info_alarmes.h"
#include "reset_geral.h"
#include "menu_parametros.h"
#include "interface_senha.h"
#include "interface_bloqueio.h"
#include "HD44780.H"
#include "mostra_falhas.H"
/***********************************************************************************
*		Definições com constantes utilizadas no programa
***********************************************************************************/ 
#define CLEAR_LINE          "                "

/***********************************************************************************
*		Enumerações
***********************************************************************************/
typedef enum{
  eNovoAlarme,
  eNovaFalha,  
  eBotaoAlarmeGeralPress,
  eTempoAlarmeMaiorZero,
  eTempoAlarmeIgualZero,
  eBotaoResetGeralPress,
  eBotaoParametrosPress,
  eBotaoBloqueioPress    
}eSignals;
/***********************************************************************************
*		Estruturas
***********************************************************************************/
typedef struct{
  eAplicacaoStates(*processeSignal)(void);   
  void(*main_function)(void);
}sAplicacaoStateBox;

/***********************************************************************************
*		Uniões
***********************************************************************************/
extern eAplicacaoStates MOSTRA_FALHAS_process(void);

/***********************************************************************************
*		Constantes
***********************************************************************************/
const sAplicacaoStateBox tabelaEstados[10]=  
{ 
  { SISNORMAL_process    , SISNORMAL_entry    }, // 0 : eEstadoInicial
  { SISNORMAL_process    , SISNORMAL_entry    }, // 1 : eSistemaNormal
  { SISNORMAL_process    , SISNORMAL_entry    }, // 2 : ePreAlarme
  { SISNORMAL_process    , SISNORMAL_entry    }, // 3 : eAlarmeGeral
  { SISNORMAL_process    , SISNORMAL_entry    }, // 4 : eMostraInfoAlarmes
  { SISNORMAL_process    , SISNORMAL_entry    }, // 5 : eResetGeral
  { SISNORMAL_process    , SISNORMAL_entry    }, // 6 : eFalhas
  { MOSTRA_FALHAS_process, MOSTRA_FALHAS_entry}, // 7 : eMostraInfoFalhas
  { SISNORMAL_process    , SISNORMAL_entry    }, // 8 : eMenuParametros
  { SISNORMAL_process    , SISNORMAL_entry    }, // 9 : eConfigBloqueio
};

/***********************************************************************************
*		Variaveis locais
***********************************************************************************/


/***********************************************************************************
*		Funções locais
***********************************************************************************/


/***********************************************************************************
*   Tabela de estados
***********************************************************************************/


/***********************************************************************************
*		Implementação
***********************************************************************************/

/***********************************************************************************
*   Descrição     :   Ponto de entrada da máquina de estados da aplicação
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void APLICACAO_entry(void){
  static eAplicacaoStates estadoAtual = eEstadoInicial;  
  
  tabelaEstados[estadoAtual].main_function();
  estadoAtual = tabelaEstados[estadoAtual].processeSignal();
  
  // Hook para entrar no menu parametros sem precisar
  // implementar uma sequencia de estado para o eMenuparametros....
  if(estadoAtual == eMenuParametros){    
    if(INTERFACESENHA_entry(7784))  
      MENUPARAMETROS_entry(); 
    estadoAtual = eSistemaNormal;  
    HD44780_writeTwoLines(CLEAR_LINE,CLEAR_LINE);         
  }
    
  // Hook para entrar no menu de bloqueios sem precisar 
  // implementar uma sequencia de estado para o eConfigBloqueio
  if(estadoAtual == eConfigBloqueio){
     INTERFACEBLOQUEIO_entry();
     estadoAtual = eSistemaNormal;      
     HD44780_writeTwoLines(CLEAR_LINE,CLEAR_LINE);     
      
  }
  
  if(EVSY_getNumberOfAlarms()){
    estadoAtual = ePreAlarme;
    HD44780_writeTwoLines("NOVO","ALARME");   
  }
  
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/
