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
|       Este código é propriedade da Chave Digital e não pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Chave Digital ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Arquivo            :  menu_mod_lacos.c
|       Descrição          :  Funções para configuração dos módulos de laço
|                             instalados no sistema
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  19/03/2011
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
#include <mc9s08ac128.h>
#include "hd44780.h"
#include "beep.h"
#include "systick.h"
#include "parametros.h"
#include "exthw.h"
#include "teclado.h"
#include "menu_mod_lacos.h"
/***********************************************************************************
*		Constantes do programa
***********************************************************************************/ 
const char tabelaNomes[5][17]=
{
 "[ ] Laco 01     ",
 "[ ] Laco 02     ",
 "[ ] Laco 03     ",
 "[ ] Laco 04     ",
 "    Sair        "
};

/***********************************************************************************
*   Implementação
***********************************************************************************/

/***********************************************************************************
*   Descrição         :   Menu para habilitação/desabilitação dos laços
*   Parametros        :   nenhum
*   Retorno           :   nenhum
***********************************************************************************/
void MENU_MOD_LACOS_entry(void){
  unsigned int timeOut=1000;
  unsigned char loop=0xFF,borda,ponteiro=0,atualiza=1,bits[5]={0,0,0,0,0},i;
  eTeclas tecla;
    
  HD44780_writeTwoLines("   MOD. LACOS   ","                ");
  
  for(i=0;i<4;i++){
    bits[i] = PARAMETROS_carregaConfigLaco(i);
  }
         
  do{
        
    ///-------------------------------///
    /// Decodificação do teclado      ///
    ///-------------------------------///
    tecla = TECLADO_getTecla(&borda);
    if(tecla && !borda)
      switch(tecla){
        case TeclaUP      : (ponteiro)?(ponteiro--):(ponteiro=4);
                            atualiza = 1;
                            timeOut = 1000;
                            break;
        case TeclaDown    : ponteiro = (unsigned char)(ponteiro+1)%5;
                            atualiza = 1;
                            timeOut = 1000;
                            break;         
        case TeclaConfirma: 
                            switch(ponteiro){
                              case 0: (bits[0])?(bits[0]=0):(bits[0]=1); atualiza = 1; break;
                              case 1: (bits[1])?(bits[1]=0):(bits[1]=1); atualiza = 1; break;
                              case 2: (bits[2])?(bits[2]=0):(bits[2]=1); atualiza = 1; break;
                              case 3: (bits[3])?(bits[3]=0):(bits[3]=1); atualiza = 1; break;
                              case 4: loop = 0x00; break;
                            }
                            timeOut = 1000;
                            break;                            
        case TeclaCancelar: loop = 0x00;               
                            break;        
      }
                
    // Espera para não ficar loco!!!!                            
    SYSTICK_delay(50);
    
    if(atualiza){
      atualiza = 0x00;
      
      while(!EXTHW_waitSecurity(&semaforoBus));
      
      PTADD = 0xff;                 
      HD44780_posicionaTexto(0,1);              
      HD44780_writeString((char*)tabelaNomes[ponteiro]);
      if(bits[ponteiro]){
        HD44780_posicionaTexto(1,1);         
        HD44780_writeChar('X');
      }                    
      EXTHW_signal(&semaforoBus);                    
    }                
  }
  while(loop && --timeOut);      
  
  for(i=0;i<4;i++)
    PARAMETROS_configLaco(i,bits[i]);
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/