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
|       Este código é propriedade da Chave Digital e não pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Chave Digital ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Arquivo            :  menu_generico.c
|       Descrição          :  Menu e telas de configurações genéricas
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "teclado.h"
#include "menu_generico.h"
#include "appConfig.h"
#include "HD44780.h"
#include "SYSTICK.H"
#include "exthw.h"
/***********************************************************************************
*		Definições com constantes utilizadas no programa
***********************************************************************************/ 
const unsigned char tabelaCaracteres[]=
{
"0123456789ABCDEFGHIJKLMNOPQRSTUVXYWZabcdefghijklmnopqrstuvxywz,.;:?/!+-[]* "    
};
/***********************************************************************************
*		Enumerações
***********************************************************************************/


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
unsigned int MENU_GENERICO_counter=0,MENU_GENERICO_timeOut=0;
unsigned char mirrorBuffer[32],incInfo[32];
/***********************************************************************************
*		Funções locais
***********************************************************************************/
void writeLCD(char *linha1,char *linha2);

/***********************************************************************************
*		Implementação
***********************************************************************************/

/***********************************************************************************
*   Descrição     :   Função interna do módulo para escrever no LCD
*   Parametros    :   (char*) conteúdo da primeira linha
*                     (char*) conteúdo da segunda linha
*   Retorno       :   (nenhum)
***********************************************************************************/
void writeLCD(char *linha1,char *linha2){
 
  while(!EXTHW_waitSecurity(&semaforoBus));
  PTADD = 0xff;             
  HD44780_posicionaTexto(0,0);      
  HD44780_writeString(linha1);
  HD44780_posicionaTexto(0,1);
  HD44780_writeString(linha2);  
  EXTHW_signal(&semaforoBus);    
}
/***********************************************************************************
*   Descrição     :   Faz o processamento de um menu
*   Parametros    :   (sMenuInfoTable) estrutura com os dados do menu
*   Retorno       :   nenhum
***********************************************************************************/
void MENUGENERICO_show(sMenuInfoTable tabelaMenu){ 
  unsigned char loop=0xff,ciclos=100,borda,ponteiro=0,redrawMenu=1;
  eTeclas tecla;

  HD44780_cls();
  HD44780_writeString((char*)tabelaMenu.menu_title);
  
  do{
    
    tecla = TECLADO_getTecla(&borda);
    
    switch(tecla){    
      case TeclaDown    : ponteiro = (unsigned char)((ponteiro+1) % (tabelaMenu.number_of_options));
                          redrawMenu = 1;
                          break;
      case TeclaUP      : (ponteiro)?(ponteiro--):(ponteiro = tabelaMenu.number_of_options-1);
                          redrawMenu = 1;
                          break;      
      case TeclaConfirma: if(tabelaMenu.options_table[ponteiro].function != NULL)
                            tabelaMenu.options_table[ponteiro].function();
                          else
                            loop = 0x00; 
                          redrawMenu = 1;    
                          HD44780_cls();
                          HD44780_writeString((char*)tabelaMenu.menu_title);                                                
                          break;      
      case TeclaCancelar: loop = 0x00;
                          break;      
    }
    
    if(redrawMenu){
      redrawMenu = 0x00;
      
      while(!EXTHW_waitSecurity(&semaforoBus));
      PTADD = 0xff;             
      HD44780_posicionaTexto(0,1);      
      HD44780_writeChar(0x7E);
      HD44780_writeString((char*)tabelaMenu.options_table[ponteiro].optionName);  
      EXTHW_signal(&semaforoBus);            
    }    
  }
  while(loop && MENU_GENERICO_timeOut);   
}  
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/