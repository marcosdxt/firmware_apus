/*__________________________________________________________________________________
|    	  Chave Digital Tecnologia Eletronica Ltda. 
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
|       Arquivo            :  menu_parametros.c
|       Descrição          :  Menu para configurações do sistema
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  18/03/2011
|
|       Revisões           :  1.0
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*		Includes
***********************************************************************************/
#include <stdlib.h>
#include "menu_parametros.h"
#include "menu_generico.h"
#include "menu_ajuste_relogio.h"
#include "tela_tempo_alarme.h"
#include "menu_mod_lacos.h"
#include "interface_nome_pontos.h"
#include "interface_sirene_auxiliar.h"
#include "interface_cadastro_reles.h"


const sTermo tabelaOpcoes[]=
{ 
  {{" Ajuste relogio "},{MENURELOGIO_telaAjuste}}, 
  {{" Modulos laco   "},{MENU_MOD_LACOS_entry}}, 
  {{" Nomes de Pontos"},{INTERFACENOME_entry}}, 
  {{" Mod. sir. aux. "},{INTERFACE_SIRENE_AUXILIAR_entry}}, 
  {{" Reles          "},{INTERFACE_REGRAS_RELE_entry}}, 
  {{" Rede ApusNET   "},{NULL}}, 
  {{" Tempo p/ alm   "},{TELA_TEMPO_ALARME_entry}}, 
  {{" Sair           "},{NULL}}  
};
          
const sMenuInfoTable menuParametros[]=
{ 
  "   PARAMETROS",
  8,    
  (sTermo*)tabelaOpcoes  
};          

/***********************************************************************************
*   Descrição     :   Ponto de entrada do menu parametros
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void MENUPARAMETROS_entry(void){
 
  MENUGENERICO_show(*(sMenuInfoTable*)&menuParametros);  
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/