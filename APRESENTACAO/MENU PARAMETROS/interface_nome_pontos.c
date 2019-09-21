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
|       Arquivo            :  interface_nome_pontos.c
|       Descrição          :  Telas para configuração dos nomes dos pontos
|                             da central
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
#include <MC9S08AC128.H>
#include "teclado.h"
#include "exthw.h"
#include "hd44780.h"
#include "beep.h"
#include "parametros.h"
#include "systick.h"
#include "interface_nome_pontos.h"
/***********************************************************************************
*		Definições com constantes utilizadas no programa
***********************************************************************************/ 
const char mensagemPontos[1][7][17] = { 
                                     {                                       
                                       {" Config. Ponto "}, 
                                       {"  Laco 01      "}, 
                                       {"  Laco 02      "}, 
                                       {"  Laco 03      "}, 
                                       {"  Laco 04      "}, 
                                       {"  Voltar       "},                                        
                                       {"Laco:X Ponto:XX"}
                                     }                                                                                                                                                                                                                                
                                   };
                                   
const char telaEdicaoNome[1][17] = { 
                                     {"[ ] HABILITADO "}
                                   };                                   
                                   
const char mensagemNomeNaoCadastrado[1][17] = { 
                                                {"Nao configurado "}
                                              };                                   

/***********************************************************************************
*		Funções locais
***********************************************************************************/
void listagemPontos(unsigned char laco);
void alteraNomePonto(unsigned char laco,unsigned char ponto); 
char IU_incrementaChar(char c);
char IU_decrementaChar(char c);
/***********************************************************************************
*		Implementação
***********************************************************************************/

/***********************************************************************************
*   Descrição       :   Menu para seleção do laço que terá os pontos editados
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void INTERFACENOME_entry(void){
  unsigned int timeOut=1000;
  unsigned char loop=0xFF,borda,ponteiro=0,atualiza=1;  
  eTeclas tecla;
    
  HD44780_writeTwoLines((char*)mensagemPontos[0][0],"                ");
    
  do{
    
    tecla = TECLADO_getTecla(&borda);
    if(tecla&&!borda){
      switch(tecla){   
        case TeclaDown    : ponteiro = (unsigned char)(ponteiro+1)%5; timeOut = 1000; atualiza=1; break;        
        case TeclaUP      : (ponteiro)?(ponteiro--):(ponteiro=4); timeOut = 1000; atualiza=1; break;
        case TeclaConfirma: 
                            if(ponteiro<4){
                              listagemPontos(ponteiro);                              
                              //Depois redesenha o fundo do menu..
                              HD44780_writeTwoLines((char*)mensagemPontos[0][0],NULL);
                             }                                
                             else{                               
                               loop = 0x00;
                             }
                             atualiza = 0x01;
                            break;
        case TeclaCancelar: loop = 0x00; break;
      }      
    }
                                                  
    SYSTICK_delay(50);  
    
    if(atualiza){
      atualiza = 0x00;
            
     while(!EXTHW_waitSecurity(&semaforoBus));
     PTADD = 0xff;                 
     HD44780_posicionaTexto(0,1);             
     HD44780_writeChar(0x7E);
     HD44780_writeString((char*)mensagemPontos[0][1+ponteiro]);                
     EXTHW_signal(&semaforoBus);                           
    }                              
  }
  while(loop && --timeOut);  
} 
/***********************************************************************************
*   Descrição         :    Menu de rolagem com as configurações de nome
*                          de pontos de um determinado laço
*   Parametros        :    (unsigned char) número do laço
*   Retorno           :    nenhum
***********************************************************************************/
void listagemPontos(unsigned char laco){
  unsigned int timeOut=1000;
  unsigned char loop=0xFF,borda,ponteiroLaco=0,ponteiroPonto=0,atualiza=1,keyCount=10;
  char buffer[17];
  eTeclas tecla,lastKey;
  
  while(!EXTHW_waitSecurity(&semaforoBus));
  PTADD = 0xff;                 
  HD44780_writeChar('\f');                 
  HD44780_writeString((char*)mensagemPontos[0][6]);        
  HD44780_posicionaTexto(5,0);     
  HD44780_writeChar(((laco+1)%10) +'0');    
  EXTHW_signal(&semaforoBus);        
  
  do{
    // Decodificação da ação do teclado    
    tecla = TECLADO_getTecla(&borda);
    if(tecla && !borda)
      switch(tecla){  
        case TeclaUP       : (ponteiroPonto)?(ponteiroPonto--):(ponteiroPonto=31);
                             atualiza = 1;
                             lastKey = tecla;
                             keyCount = 50;
                             break;
        case TeclaDown     : ponteiroPonto = (unsigned char)(ponteiroPonto+1)%32;
                             atualiza = 1;   
                             lastKey = tecla;                                  
                             keyCount = 50;                             
                             break;        
        case TeclaLeft     : 
                             break;        
        case TeclaRigth    :
                             break;        
        case TeclaConfirma : alteraNomePonto(laco,ponteiroPonto);
        
                              while(!EXTHW_waitSecurity(&semaforoBus));
                              PTADD = 0xff;                 
                              HD44780_writeChar('\f');                 
                              HD44780_writeString((char*)mensagemPontos[0][6]);        
                              HD44780_posicionaTexto(5,0);     
                              HD44780_writeChar(((laco+1)%10) +'0');    
                              EXTHW_signal(&semaforoBus);  
                              atualiza = 1;           
                             break;        
        case TeclaCancelar : loop = 0x00;
                             break;        
      }//Fim da decodificação do teclado
    
      // Atualiza os dados no display
      if(atualiza){
        atualiza = 0x00;        
        buffer[0] = (((ponteiroPonto+1)/10)%10 ) + '0';
        buffer[1] = ((ponteiroPonto+1)%10) + '0';
        buffer[2] = 0x00;     
                   
        while(!EXTHW_waitSecurity(&semaforoBus));
        PTADD = 0xff;                 
        HD44780_posicionaTexto(13,0);     
        HD44780_writeString(buffer);
        
        if(!PARAMETROS_leConfiguracaoNome(laco, ponteiroPonto,buffer)){          
          (void)memcpy(buffer,mensagemNomeNaoCadastrado[0],16);
          buffer[16] = 0x00;
        }        
        
        HD44780_posicionaTexto(0,1);
        HD44780_writeString(buffer);        
        
        EXTHW_signal(&semaforoBus);                                      
      }    
    
      //Dá um tempo...
      SYSTICK_delay(50);                     
  }
  while(loop && timeOut--);            
}
/***********************************************************************************
*   Descrição       :   Tela de edição do nome de ponto e habilitação
*                       do dispositivo
*   Parametros      :   (unsigned char) laco
*                       (unsigned char) ponto
*   Retorno         :   nenhum
***********************************************************************************/
void alteraNomePonto(unsigned char laco,unsigned char ponto){
  unsigned int timeOut=1000;
  unsigned char loop=0xFF,borda,configurado=0xFF,atualiza=1,ponteiro=0,toogle=0x00,tempoToogle=10,salvar=0x00;
  eTeclas tecla;
  char bufferNome[17]="                ";
  
  if(!PARAMETROS_leConfiguracaoNome(laco,ponto,bufferNome)){
    configurado = 0x00; 
    (void)sprintf(bufferNome,"                ",16);
  }
    
  //IU_writeDisplay(1,(char*)telaEdicaoNome[idiomaCorrente],NULL);
  HD44780_writeTwoLines((char*)telaEdicaoNome[0],"                ");
  
  do{
    
    tecla = TECLADO_getTecla(&borda);
    if(tecla && !borda)
      switch(tecla){
        case TeclaUP      : if(!ponteiro)
                              configurado = ~configurado;
                            else{
                               bufferNome[ponteiro-1] = IU_incrementaChar(bufferNome[ponteiro-1]);                              
                            }
                            atualiza = 1;
                            tempoToogle = 0;
                            toogle = 0xFF;
                            timeOut = 2000;
                            break;
        case TeclaDown    : if(!ponteiro)
                              configurado = ~configurado;
                            else{
                              bufferNome[ponteiro-1] = IU_decrementaChar(bufferNome[ponteiro-1]);                               
                            }                            
                            atualiza = 1;
                            tempoToogle = 0;
                            toogle = 0xFF;
                            timeOut = 2000;                            
                            break;
        case TeclaLeft    : (ponteiro)?(ponteiro--):(ponteiro=16);
                            atualiza = 1;
                            tempoToogle = 0;
                            toogle = 0xFF;
                            timeOut = 2000;                            
                            break;
        case TeclaRigth   : ponteiro = (unsigned char)(ponteiro+1)%17;
                            atualiza = 1;
                            tempoToogle = 0;
                            toogle = 0xFF;
                            timeOut = 2000;                            
                            break;
        case TeclaConfirma: salvar=0xFF;
                            loop = 0x00;
                            break;
        case TeclaCancelar: loop = 0x00;      
                            break;
      }        
            
    //Dá um tempo...    
    SYSTICK_delay(50);  
    
    if(tempoToogle)
      tempoToogle--;
    else{
      tempoToogle = 5;
      toogle = ~toogle;
      atualiza = 1; 
    }    
    //-----------------------------------------------------------------------------//
    //   Atualiza o nome em edição no display                                      //
    //-----------------------------------------------------------------------------//
    if(atualiza){
      atualiza = 0x00;      
      while(!EXTHW_waitSecurity(&semaforoBus));
      PTADD = 0xff;                              
      HD44780_posicionaTexto(0,1);            
      if(ponteiro && toogle){              
        char tmp;                
        tmp = bufferNome[ponteiro-1];
        bufferNome[ponteiro-1] = '_';      
        HD44780_writeString(bufferNome);        
        bufferNome[ponteiro-1] = tmp;        
      } 
      else
        HD44780_writeString(bufferNome);            
      
      HD44780_posicionaTexto(1,0);   
      
      if(!ponteiro && toogle)
        HD44780_writeChar('_');           
      else{
        if(configurado)
          HD44780_writeChar('X');
        else
          HD44780_writeChar(' ');                
      }

      EXTHW_signal(&semaforoBus);                  
    }//----------------------------------------------------------------------------                   
  }
  while(loop && timeOut--);        
  
  
  if(salvar){      
    if(configurado){      
      if(!PARAMETROS_gravaConfiguracaoNome(laco,ponto,bufferNome)){
        HD44780_writeTwoLines("  ERRO 01       ","                ");
        
        SYSTICK_delay(60);            
        BEEP_setSound(1000);
        SYSTICK_delay(500);         
        BEEP_setSound(1200);
        SYSTICK_delay(500);                  
        BEEP_setSound(0);              
        while(!EXTHW_waitSecurity(&semaforoBus));
        PTADD = 0xff;                 
        HD44780_writeChar('\f');
        EXTHW_signal(&semaforoBus);              
      }
    }
    else{
      if(!PARAMETROS_apagaConfiguracaoNome(laco,ponto)){
        HD44780_writeTwoLines("  ERRO 02       ","                ");
        
        SYSTICK_delay(60);            
        BEEP_setSound(1000);
        SYSTICK_delay(500);         
        BEEP_setSound(1200);
        SYSTICK_delay(500);                  
        BEEP_setSound(0);  
        HD44780_writeTwoLines("                ","                ");
        
      }//Fim do if apaga        
    }//Fim do else               
  }//Fim do if(configurado)
}
/***********************************************************************************
*   Descrição     : Incrementa char
*   Parametros    : (char) caracter
*   Retorno       : (char) char incrementado
***********************************************************************************/
char IU_incrementaChar(char c){
  
  switch(c){
    case ' ': return '0';  case '0': return '1';  case '1': return '2';  case '2': return '3';  case '3': return '4';
    case '4': return '5';  case '5': return '6';  case '6': return '7';  case '7': return '8';  case '8': return '9';
    case '9': return 'A';  case 'A': return 'B';  case 'B': return 'C';  case 'C': return 'D';  case 'D': return 'E';
    case 'E': return 'F';  case 'F': return 'G';  case 'G': return 'H';  case 'H': return 'I';  case 'I': return 'J';
    case 'J': return 'K';  case 'K': return 'L';  case 'L': return 'M';  case 'M': return 'N';  case 'N': return 'O';
    case 'O': return 'P';  case 'P': return 'Q';  case 'Q': return 'R';  case 'R': return 'S';  case 'S': return 'T';
    case 'T': return 'U';  case 'U': return 'V';  case 'V': return 'W';  case 'W': return 'X';  case 'X': return 'Y';
    case 'Y': return 'Z';  case 'Z': return 'a';  case 'a': return 'b';  case 'b': return 'c';  case 'c': return 'd';
    case 'd': return 'e';  case 'e': return 'f';  case 'f': return 'g';  case 'g': return 'h';  case 'h': return 'i';
    case 'i': return 'j';  case 'j': return 'k';  case 'k': return 'l';  case 'l': return 'm';  case 'm': return 'n';
    case 'n': return 'o';  case 'o': return 'p';  case 'p': return 'q';  case 'q': return 'r';  case 'r': return 's';
    case 's': return 't';  case 't': return 'u';  case 'u': return 'v';  case 'v': return 'w';  case 'w': return 'x';
    case 'x': return 'y';  case 'y': return 'z';  case 'z': return '+';  case '+': return '-';  case '-': return '*';
    case '*': return '/';  case '/': return '(';  case '(': return ')';  case ')': default : return ' ';
  }
}
/***********************************************************************************
*   Descrição     :   Decrementa char
*   Parametros    :   (char) caracter
*   Retorno       :   (char) char decrementado
***********************************************************************************/
char IU_decrementaChar(char c){
  
  switch(c){
    case ' ': return ')';   case '0': return ' ';   case '1': return '0';   case '2': return '1';   case '3': return '2';
    case '4': return '3';   case '5': return '4';   case '6': return '5';   case '7': return '6';   case '8': return '7';
    case '9': return '8';   case 'A': return '9';   case 'B': return 'A';   case 'C': return 'B';   case 'D': return 'C';
    case 'E': return 'D';   case 'F': return 'E';   case 'G': return 'F';   case 'H': return 'G';   case 'I': return 'H';    
    case 'J': return 'I';   case 'K': return 'J';   case 'L': return 'K';   case 'M': return 'L';   case 'N': return 'M';
    case 'O': return 'N';   case 'P': return 'O';   case 'Q': return 'P';   case 'R': return 'Q';   case 'S': return 'R';
    case 'T': return 'S';   case 'U': return 'T';   case 'V': return 'U';   case 'W': return 'V';   case 'X': return 'W';
    case 'Y': return 'X';   case 'Z': return 'Y';   case 'a': return 'Z';   case 'b': return 'a';   case 'c': return 'b';
    case 'd': return 'c';   case 'e': return 'd';   case 'f': return 'e';   case 'g': return 'f';   case 'h': return 'g';
    case 'i': return 'h';   case 'j': return 'i';   case 'k': return 'j';   case 'l': return 'k';   case 'm': return 'l';
    case 'n': return 'm';   case 'o': return 'n';   case 'p': return 'o';   case 'q': return 'p';   case 'r': return 'q';
    case 's': return 'r';   case 't': return 's';   case 'u': return 't';   case 'v': return 'u';   case 'w': return 'v';
    case 'x': return 'w';   case 'y': return 'x';   case 'z': return 'y';   case '+': return 'z';   case '-': return '+';
    case '*': return '-';   case '/': return '*';   case '(': return '/';   case ')': default : return '(';        
  }  
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/