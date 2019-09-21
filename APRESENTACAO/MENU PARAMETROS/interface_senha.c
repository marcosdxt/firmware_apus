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
|       Arquivo            : interface_senha.c
|       Descrição          : implementação da tela para o usuário informar uma
|                            senha de 4 digitos
| 
|       Autor              : Marcos Aquino
|       Data criação       : 19/03/2011 - Refatoração...
|
|       Revisões           : 1.0
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*		Includes
***********************************************************************************/
#include <MC9S08AC128.H>
#include <stdio.h>
#include <stdlib.h>
#include "systick.h"
#include "hd44780.h"
#include "beep.h"
#include "teclado.h"
#include "interface_senha.h"
#include "exthw.h"
/***********************************************************************************
*		Definições com constantes utilizadas no programa
***********************************************************************************/ 
unsigned char INTERFACESENHA_entry(unsigned int password){
  unsigned int timeOut=1000;
  unsigned char loop=0xFF,borda,ponteiroSenha=0,digitos[4]={0,0,0,0},
                valida=0,tempoAtualizaDisplay=10,toogle=0x00;
  eTeclas teclaPress;
  char linha[5];
  
  BEEP_setSound(800);
  SYSTICK_delay(100);
  BEEP_setSound(0); 
  HD44780_writeTwoLines("SENHA DE ACESSO ","                ");
    
  do{
    
    // Verifica a tecla pressionada
    teclaPress = TECLADO_getTecla(&borda);
    if(!borda)
      switch(teclaPress){
        case TeclaUP      : digitos[ponteiroSenha] = (unsigned char)(digitos[ponteiroSenha]+1)%10;
                            timeOut=1000;
                            toogle = 0xFF;
                            tempoAtualizaDisplay = 0;
                            break;                            
        case TeclaDown    : (digitos[ponteiroSenha])?(digitos[ponteiroSenha]--):(digitos[ponteiroSenha]=9);
                            timeOut=1000; 
                            toogle = 0xFF;           
                            tempoAtualizaDisplay = 0;                                                    
                            break;        
        case TeclaLeft    : (ponteiroSenha)?(ponteiroSenha--):(ponteiroSenha=3);
                            timeOut=1000; 
                            toogle = 0xFF;           
                            tempoAtualizaDisplay = 0;                                                    
                            break;        
        case TeclaRigth   : ponteiroSenha = (unsigned char)(ponteiroSenha+1)%4;
                            timeOut=1000; 
                            toogle = 0xFF;           
                            tempoAtualizaDisplay = 0;                                                    
                            break;        
        case TeclaConfirma: if(ponteiroSenha<3)
                              ponteiroSenha++;
                            else{                              
                              valida = 0xFF;
                              loop = 0x00;
                            }
                            break;        
        case TeclaCancelar: loop = 0x00;
                            break;        
      }
    
    // Espera para não ficar loco!!!!                            
    SYSTICK_delay(50);
    
    if(tempoAtualizaDisplay)
      tempoAtualizaDisplay--;
    else{
      tempoAtualizaDisplay = 5;
      
      linha[0] = digitos[0] + '0';
      linha[1] = digitos[1] + '0';
      linha[2] = digitos[2] + '0';
      linha[3] = digitos[3] + '0';
      linha[4] = '\0';
      
      toogle = ~toogle;
      if(toogle)
        linha[ponteiroSenha] = ' ';
      
      while(!EXTHW_waitSecurity(&semaforoBus));//Pega o barramento      
      PTADD = 0xff;                 
      HD44780_posicionaTexto(5,1);
      HD44780_writeString(linha);
      EXTHW_signal(&semaforoBus);                     
    }
            
  }
  while(loop && timeOut--);  
      
  if(valida){  
    if( (digitos[0] == (password/1000)%10) && (digitos[1] == (password/100)%10) && 
        (digitos[2] == (password/10)%10)   && (digitos[3] == (password%10))){
       return 0xFF;                    
    }        
    HD44780_writeTwoLines("     SENHA      ",
                          "   INCORRETA    ");              
    SYSTICK_delay(60);            
    BEEP_setSound(1000);
    SYSTICK_delay(500);         
    BEEP_setSound(1200);
    SYSTICK_delay(500);                  
    BEEP_setSound(0);        
    HD44780_writeTwoLines("                ","                ");    
  }      
  return 0x00;  
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/