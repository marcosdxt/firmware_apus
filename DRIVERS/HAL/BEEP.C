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
|       Este código é propriedade da Chave Digital e não pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Chave Digital ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Módulo de software :  Manipulador do gerador de som
|       Arquivo            :  Beep.C
|       Descrição          :  Funções para gerar frequências utilizado
|                             hardware de algum mcu
|       Autor              :  Marcos Aquino
|       Data criação       :  07/05/2010
|
|       Revisões           :  1.0
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
*		Funções locais
***********************************************************************************/
 
/***********************************************************************************
*		Implementação
***********************************************************************************/

/***********************************************************************************
*   Descrição       :   Inicialização do módulo
*   Parametros      :   (nenhum)
*   Retorno         :   (nenhum)
***********************************************************************************/
void BEEP_init(void){
 
  INIT_BEEP_HW;
  KILL_SOUND();    
}
/***********************************************************************************
*   Descrição       :   Progrma o hardware para gerar uma certa frequência
*   Parametros      :   (unsigned int) frequência
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
*   Descrição       :   Desliga o gerador de frequências do sistema
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void BEEP_killSound(void){

  KILL_SOUND();  
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/