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
|       Arquivo            :  interface_bloqueio.c
|       Descrição          :  tela para bloqueio dos laços do sistema
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  19/03/2011 - Refatoração...
|
|       Revisões           :  1.0
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*		Includes
***********************************************************************************/
#include <MC9S08AC128.H>
#include "interface_bloqueio.h"
#include "systick.h"
#include "hd44780.h"
#include "teclado.h"
#include "beep.h"
#include "parametros.h"
#include "eventossistema.h"
#include "exthw.h"

/***********************************************************************************
*   Constantes
***********************************************************************************/
const char mensagens[5][17] = {"[ ] laco 01     ",
                               "[ ] laco 02     ",
                               "[ ] laco 03     ",                               
                               "[ ] laco 04     ",
                               "    Sair        "};
  
/***********************************************************************************
*   Implementação
***********************************************************************************/
/***********************************************************************************
*   Descrição       :   Menu para bloqueio dos laços
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void INTERFACEBLOQUEIO_entry(void){
  unsigned int timeOut=1000;
  unsigned char loop=0xFF,borda,ponteiro=0,atualiza=1,bits[5]={0,0,0,0,0},trava[4];
  eTeclas tecla;
  
  HD44780_writeTwoLines(" BLOQUEIO LACO  ","                ");
      
  bits[0] = PARAMETROS_leBloqueio(0);
  bits[1] = PARAMETROS_leBloqueio(1);
  bits[2] = PARAMETROS_leBloqueio(2);
  bits[3] = PARAMETROS_leBloqueio(3);
  
  if(bits[0])bits[0] = 0x01;
  if(bits[1])bits[1] = 0x01;
  if(bits[2])bits[2] = 0x01;
  if(bits[3])bits[3] = 0x01;      
  
  trava[0] = bits[0];
  trava[1] = bits[1];
  trava[2] = bits[2];
  trava[3] = bits[3];  
       
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
      HD44780_writeString((char*)mensagens[ponteiro]);//mensagemBloqueioLacos[idiomaCorrente][1+ponteiro]);      
      if(bits[ponteiro]){
        HD44780_posicionaTexto(1,1);         
        HD44780_writeChar('X');
      }                    
      EXTHW_signal(&semaforoBus);                    
    }                
  }
  while(loop && --timeOut);      
    
  // Salva o flag de bloqueio do laço 01
  if(trava[0] != bits[0]){    
    (void)PARAMETROS_gravaBloqueio(0,bits[0]); 
    (void)EVSY_insereBloqueio(bits[0],0);
  }
  
  // Salva o flag de bloqueio do laço 02
  if(trava[1] != bits[1]){    
    (void)PARAMETROS_gravaBloqueio(1,bits[1]);
    (void)EVSY_insereBloqueio(bits[1],1);    
  }
  
  // Salva o flag de bloqueio do laço 03
  if(trava[2] != bits[2]){    
    (void)PARAMETROS_gravaBloqueio(2,bits[2]);
    (void)EVSY_insereBloqueio(bits[2],2);        
  }
  
  // Salva o flag de bloqueio do laço 04
  if(trava[3] != bits[3]){    
    (void)PARAMETROS_gravaBloqueio(3,bits[3]);    
    (void)EVSY_insereBloqueio(bits[3],3);            
  }  
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/