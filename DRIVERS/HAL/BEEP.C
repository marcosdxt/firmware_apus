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
|       M�dulo de software :  Manipulador do gerador de som
|       Arquivo            :  Beep.C
|       Descri��o          :  Fun��es para gerar frequ�ncias utilizado
|                             hardware de algum mcu
|       Autor              :  Marcos Aquino
|       Data cria��o       :  07/05/2010
|
|       Revis�es           :  1.0
|
|
| __________________________________________________________________________________
*/
//#define _DEBUG_
/***********************************************************************************
*		Includes
***********************************************************************************/
#include "beep_targetWrapper.h"
#include "beep.h"
/***********************************************************************************
*		Constantes
***********************************************************************************/


/***********************************************************************************
*		Variaveis locais
***********************************************************************************/


/***********************************************************************************
*		Fun��es locais
***********************************************************************************/
 
/***********************************************************************************
*		Implementa��o
***********************************************************************************/

/***********************************************************************************
*   Descri��o       :   Inicializa��o do m�dulo
*   Parametros      :   (nenhum)
*   Retorno         :   (nenhum)
***********************************************************************************/
void BEEP_init(void){
 
  INIT_BEEP_HW;
  KILL_SOUND();    
}
/***********************************************************************************
*   Descri��o       :   Progrma o hardware para gerar uma certa frequ�ncia
*   Parametros      :   (unsigned int) frequ�ncia
*   Retorno         :   nenhum
***********************************************************************************/
void BEEP_setSound(unsigned int frequencia){
    
  if(frequencia==0){    
    KILL_SOUND();    
    return;
  }
  
  #ifndef _DEBUG_          
  ON_SOUND();
  SET_CICLE(frequencia<<1);           
  #endif
}
/***********************************************************************************
*   Descri��o       :   Desliga o gerador de frequ�ncias do sistema
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void BEEP_killSound(void){

  KILL_SOUND();  
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/