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
|       Arquivo            :  TelaFalhasPendentes.c
|       Descri��o          :  Tela de rolagem para verifica��o das falhas
|                             que est�o pendentes no sistema
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  24/03/2011
|
|       Revis�es           :  1.0
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*		Includes
***********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <mc9s08ac128.h>
#include "eventosSistema.h"
#include "falhasPendentes.h"
#include "hd44780.h"
#include "teclado.h"
#include "telaFalhasPendentes.h"
/***********************************************************************************
*		Defini��es com constantes utilizadas no programa
***********************************************************************************/ 



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
extern unsigned int MENU_GENERICO_timeOut;

/***********************************************************************************
*		Fun��es locais
***********************************************************************************/
void showFault(unsigned char index,unsigned char total);
 
/***********************************************************************************
*		Implementa��o
***********************************************************************************/

/***********************************************************************************
*   Descri��o     :   Ponto de entrada da tela de verifica��o das falhas
*                     pendentes do sistema
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void TELA_FALHAS_PENDENTES_entry(void){
  eTeclas tecla;
  unsigned char loop=0xFF,borda,ponteiro=0,mostraTela=1;
      
  if(!FP_getElementsInQueue())
    return;  
 
  do{
    //Decodifica o teclado
    tecla = TECLADO_getTecla(&borda);
    switch(tecla){
      case TeclaUP      : (ponteiro)?(ponteiro--):(ponteiro =FP_getElementsInQueue()-1); 
                          mostraTela = 1;
                          break;      
      case TeclaDown    : ponteiro = (unsigned char)((ponteiro+1)% FP_getElementsInQueue());
                          mostraTela = 1;
                          break;      
      case TeclaConfirma:
      case TeclaCancelar: loop = 0x00;
                          break;
    }
    //Fim da decodifica��o do teclado
    
    if(mostraTela){
      mostraTela = 0x00;
      showFault(ponteiro,FP_getElementsInQueue());   
    }    
    
  }
  while(loop && MENU_GENERICO_timeOut);
}
/***********************************************************************************
*   Descri��o     :   Mostra o texto de uma falha pendente no LCD
*   Parametros    :   (unsigned char) n�mero da falha pendente
*   Retorno       :   nenhum
***********************************************************************************/
void showFault(unsigned char index,unsigned char total){
  char bufferLinha[17],bufferFalha[17],parametro;
  eDefs descritor;
  
  (void)FP_getNodo(index,&descritor,&parametro);  
  FP_makeString(descritor,parametro,bufferFalha);
 
  (void)sprintf(bufferLinha,"[Falha %02d de %02d]",index+1,total);
  HD44780_writeTwoLines(bufferLinha,bufferFalha);  
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/