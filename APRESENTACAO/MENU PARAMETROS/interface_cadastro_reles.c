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
|       Arquivo            :  interface_cadastro_reles.c
|       Descrição          :  Tela para configuração das regras de relé
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  19/03/2011
| 
|       Revisões           :  1.0 - refatoração
|  
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*		Includes
***********************************************************************************/
#include <mc9s08ac128.h>
#include <stdlib.h>
#include <stdio.h>
#include "teclado.h"
#include "beep.h"
#include "hd44780.h"
#include "systick.h"
#include "exthw.h"
#include "interface_cadastro_reles.h"
#include "parametros.h"
/***********************************************************************************
*		Definições com constantes utilizadas no programa
***********************************************************************************/ 
const char mensagemTabelaReles[1][22][17] ={ 
                                           { 
                                             {" Config. reles "}, 
                                             {"  Rele 01      "},
                                             {"  Rele 02      "},
                                             {"  Rele 03      "},
                                             {"  Rele 04      "},
                                             {"  Voltar       "}, 
                                             {"Config Rele XX "},                                                                                                                                        
                                             {"[ ]Alarme geral"}, 
                                             {"[ ]Pre-alarme  "}, 
                                             {"[ ]Laco alarme "}, 
                                             {"[ ]Ponto alarme"}, 
                                             {"[ ]Laco bloq.  "}, 
                                             {"[ ]Falha sist. "}, 
                                             {"[ ]Rede ou Bat "}, 
                                             {"[ ]Laco Cruzado"},
                                             {"LacoA: XX      "}, 
                                             {"LacoB: XX      "}, 
                                             {"Ponto: XX      "},                                              
                                             {"Voltar         "},
                                             {" Laco trigger  "}, 
                                             {" Ponto trigger "}, 
                                             {" Erro 03       "}                                              
                                           }
                                         };
                                         
#define LINHA_EM_BRANCO                 "                "                                         
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


/***********************************************************************************
*		Funções locais
***********************************************************************************/
void IU_configuraRele(unsigned char rele);
unsigned char IU_editaValor(char *tituloTela,unsigned char valor,unsigned char limite); 
/***********************************************************************************
*		Implementação
***********************************************************************************/

/***********************************************************************************
*   Descrição         :   Interface para cadastro das regras de relés
*   Parametros        :   nenhum
*   Retorno           :   nenhum
***********************************************************************************/
void INTERFACE_REGRAS_RELE_entry(void){
  unsigned int timeOut=1000;
  unsigned char loop=0xFF,borda,ponteiro=0,atualiza=1;
  eTeclas tecla;
  
  HD44780_writeTwoLines((char*)mensagemTabelaReles[0][0],LINHA_EM_BRANCO);  
  
  do{
      //-- Decodificação do teclado    
      tecla = TECLADO_getTecla(&borda);
      if(tecla && !borda)
        switch(tecla){     
          case TeclaDown    : ponteiro = (unsigned char)(ponteiro+1)%5; atualiza = 1; timeOut = 1000; break;
          case TeclaUP      : (ponteiro)?(ponteiro--):(ponteiro=4);     atualiza = 1; timeOut = 1000; break;        
          case TeclaLeft    : break;        
          case TeclaRigth   : break;        
          case TeclaConfirma: if(ponteiro==4) loop = 0x00;
                              else{                              
                                IU_configuraRele(ponteiro);
                                HD44780_writeTwoLines((char*)mensagemTabelaReles[0][0],LINHA_EM_BRANCO);
                                atualiza = 1;    
                                timeOut = 1000;                            
                              }
                              break;        
          case TeclaCancelar: loop = 0x00;break;        
      }
   
      //-- Dá um tempo...    
      SYSTICK_delay(50);     
      
      //-- Atualização dos dados no display
      if(atualiza){
        atualiza = 0x00;          
        while(!EXTHW_waitSecurity(&semaforoBus));
        PTADD = 0xff;                 
        HD44780_posicionaTexto(0,1);                    
        HD44780_writeString((char*)mensagemTabelaReles[0][1+ponteiro]);          
        HD44780_posicionaTexto(0,1);         
        HD44780_writeChar(0x7E);
        EXTHW_signal(&semaforoBus);                           
      }
                        
  }
  while(loop && timeOut--);    
}
/***********************************************************************************
*     Descrição     :   Configura os bits de sinalização de um dos relés
*     Parametros    :   (unsigned char) Número do relé
*     Retorno       :   nenhum
***********************************************************************************/
void IU_configuraRele(unsigned char rele){
  unsigned int timeOut=1000;  
  unsigned char loop=0xFF,borda,ponteiro=0,atualiza=1,salvar=0x00,laco=1,ponto=1,lacoAuxiliar=0,desenhaFundo=0xFF;
  eTeclas tecla;
  char buffer[10];
  eReleRow cfg;
  
  while(!EXTHW_waitSecurity(&semaforoBus));
  PTADD = 0xff;                 
  HD44780_writeChar('\f');               
  HD44780_writeString((char*)mensagemTabelaReles[0][6]);            
  buffer[0] = (((rele+1)/10)%10)+'0';
  buffer[1] = ((rele+1)%10)+'0';
  buffer[2] = '\0';
  HD44780_posicionaTexto(12,0);                    
  HD44780_writeString(buffer);    
  EXTHW_signal(&semaforoBus);   
  
  //--------------------------------------//
  //  Inicializa os flags com as configurações da memória
  //--------------------------------------// 
  if(!PARAMETROS_leConfigReles(rele,&cfg)){
    *(unsigned char*)&cfg.bits = 0x00;
    cfg.laco = 1;
    cfg.ponto = 1;  
    cfg.pontoAuxiliar = 1;    
  }  
  
  laco = cfg.laco;
  ponto = cfg.ponto;
  lacoAuxiliar = cfg.pontoAuxiliar; 
   
  
  do{
      //------------------------------------------------------------------------------    
      tecla = TECLADO_getTecla(&borda);
      if(tecla && !borda)
        switch(tecla){
          case TeclaUP      : (ponteiro)?(ponteiro--):(ponteiro=11);     atualiza=1; timeOut = 1000; break; 
          case TeclaDown    : ponteiro = (unsigned char)(ponteiro+1)%12; atualiza=1; timeOut = 1000; break;
          case TeclaLeft    : break;
          case TeclaRigth   : break;
          case TeclaConfirma: if(ponteiro<11){                                 
                                switch(ponteiro){
                                  case 0 : cfg.bits.AlarmeGeral   ^= 1; break;
                                  case 1 : cfg.bits.PreAlarme     ^= 1; break;
                                  case 2 : cfg.bits.LacoEmAlarme  ^= 1; break;
                                  case 3 : cfg.bits.PontoEmAlarme ^= 1; break;
                                  case 4 : cfg.bits.LacoBloqueado ^= 1; break;
                                  case 5 : cfg.bits.LacoDefeito   ^= 1; break;
                                  case 6 : cfg.bits.FaltaFB       ^= 1; break;
                                  case 7 : cfg.bits.LacoCruzado   ^= 1; break;
                                  case 8 : laco  = IU_editaValor((char*)mensagemTabelaReles[0][19],laco,4);
                                           desenhaFundo = 0xFF;           
                                           break;
                                  case 9 : lacoAuxiliar  = IU_editaValor((char*)mensagemTabelaReles[0][19],lacoAuxiliar,4);
                                           desenhaFundo = 0xFF;                            
                                           break;
                                  case 10: ponto = IU_editaValor((char*)mensagemTabelaReles[0][20],ponto,32); 
                                           desenhaFundo = 0xFF;                                
                                           break;
                                                                
                                }
                                atualiza = 1;  
                                timeOut = 1000;          
                              }
                              else{
                                salvar = 0x01;
                                loop = 0x00;
                              }
                              break;                                                                                                          
          case TeclaCancelar: loop = 0x00; break;
        }   
        
      if(desenhaFundo){
        desenhaFundo = 0x00;  
        
        while(!EXTHW_waitSecurity(&semaforoBus));
        PTADD = 0xff;                 
        HD44780_writeChar('\f');               
        HD44780_writeString((char*)mensagemTabelaReles[0][6]);            
        buffer[0] = (((rele+1)/10)%10)+'0';
        buffer[1] = ((rele+1)%10)+'0';
        buffer[2] = '\0';
        HD44780_posicionaTexto(12,0);                    
        HD44780_writeString(buffer);    
        EXTHW_signal(&semaforoBus);           
      }
           
      //------------------------------------------------------------------------------
      if(atualiza){
        atualiza = 0x00;        
        while(!EXTHW_waitSecurity(&semaforoBus));
        PTADD = 0xff;                 
        HD44780_posicionaTexto(0,1);                        
        HD44780_writeString((char*)mensagemTabelaReles[0][7+ponteiro]);          
        
        if(ponteiro<8){          
          HD44780_posicionaTexto(1,1);                        
          HD44780_writeChar(' ');        
          HD44780_posicionaTexto(1,1);                                
          switch(ponteiro){
            case 0 : if(cfg.bits.AlarmeGeral  ) HD44780_writeChar('X');break;
            case 1 : if(cfg.bits.PreAlarme    ) HD44780_writeChar('X');break;
            case 2 : if(cfg.bits.LacoEmAlarme ) HD44780_writeChar('X');break;
            case 3 : if(cfg.bits.PontoEmAlarme) HD44780_writeChar('X');break;
            case 4 : if(cfg.bits.LacoBloqueado) HD44780_writeChar('X');break;
            case 5 : if(cfg.bits.LacoDefeito  ) HD44780_writeChar('X');break;
            case 6 : if(cfg.bits.FaltaFB      ) HD44780_writeChar('X');break;
            case 7 : if(cfg.bits.LacoCruzado  ) HD44780_writeChar('X');break;
          } 
        }        
        if(ponteiro==8){
          buffer[0] = '0' + ((laco/10)%10);
          buffer[1] = '0' + (laco%10);
          buffer[2] = '\0';         
          HD44780_posicionaTexto(7,1);                        
          HD44780_writeString(buffer);
        }        
        if(ponteiro==9){
          buffer[0] = '0' + ((lacoAuxiliar/10)%10);   
          buffer[1] = '0' + (lacoAuxiliar%10);
          buffer[2] = '\0';         
          HD44780_posicionaTexto(7,1);                        
          HD44780_writeString(buffer);                         
        }      
        if(ponteiro==10){
          buffer[0] = '0' + ((ponto/10)%10);
          buffer[1] = '0' + (ponto%10);
          buffer[2] = '\0';         
          HD44780_posicionaTexto(7,1);                        
          HD44780_writeString(buffer);                                   
        }                    
        EXTHW_signal(&semaforoBus);                              
      }           
      //------------------------------------------------------------------------------
      SYSTICK_delay(50);          
  }  
  while(loop && timeOut--);    
  
  
  if(salvar){
  
    if(!PARAMETROS_gravaConfigRele(rele,cfg.bits.AlarmeGeral  ,cfg.bits.PreAlarme    ,cfg.bits.LacoEmAlarme,                                           cfg.bits.PontoEmAlarme,cfg.bits.LacoBloqueado,cfg.bits.LacoDefeito
    ,                                        cfg.bits.FaltaFB    ,cfg.bits.LacoCruzado,
                                        laco,ponto,lacoAuxiliar)){  

      HD44780_writeTwoLines((char*)mensagemTabelaReles[0][20],LINHA_EM_BRANCO);
      
      SYSTICK_delay(60);            
      BEEP_setSound(1000);
      SYSTICK_delay(500);         
      BEEP_setSound(1200);
      SYSTICK_delay(500);                  
      BEEP_setSound(0);      
      
      HD44780_writeTwoLines(LINHA_EM_BRANCO,LINHA_EM_BRANCO);
     }              
  }    
}
/***********************************************************************************
*   Descrição     :   Tela para edição de um número de 2 digitos
*   Parametros    :   (String*) Titulo da janela
*                     (unsigned char) valor inicial para edição
*   Retorno       :   (unsigned char) valor editado pelo usuário
***********************************************************************************/
unsigned char IU_editaValor(char *tituloTela,unsigned char valor,unsigned char limite){
  unsigned int timeOut=1000;
  unsigned char loop=0xFF,ponteiroDigito=0,borda,atualiza=1,toogle=0x00,timeToogle=5,emp;
  char bufferDigitos[3];    
  eTeclas tecla;  
  
  bufferDigitos[0] = '0' + (valor/10)%10;
  bufferDigitos[1] = '0' + valor % 10;
  bufferDigitos[2] = 0x00;
  

  while(!EXTHW_waitSecurity(&semaforoBus));
  PTADD = 0xff;                 
  HD44780_writeChar('\f');               
  HD44780_writeString(tituloTela);  
  HD44780_posicionaTexto(6,1);    
  HD44780_writeString(bufferDigitos);  
  EXTHW_signal(&semaforoBus);    

 
  do{
      //--------------------------
      //  Decodificação do teclado
      //--------------------------
      tecla = TECLADO_getTecla(&borda);
      if(tecla && !borda)
        switch(tecla){ 
          case TeclaDown    : //----------------------------------------------
                              if(bufferDigitos[ponteiroDigito]=='0')
                                bufferDigitos[ponteiroDigito]='9';
                              else
                                bufferDigitos[ponteiroDigito]--;
                              
                              atualiza = 1;
                              timeToogle = 0x00;
                              toogle = 0xFF;  
                              timeOut = 1000;                            
                              break;
          case TeclaUP      : //----------------------------------------------
                              bufferDigitos[ponteiroDigito]++;
                              if(bufferDigitos[ponteiroDigito]>'9')
                                bufferDigitos[ponteiroDigito]='0';
                              
                              atualiza = 1;
                              timeToogle = 0x00;
                              toogle = 0xFF; 
                              timeOut = 1000;                                                    
                              break;          
          case TeclaLeft    : //----------------------------------------------
                              (ponteiroDigito)?(ponteiroDigito--):(ponteiroDigito=1);
                              atualiza = 1;
                              timeToogle = 0x00;
                              toogle = 0xFF; 
                              timeOut = 1000;                             
                              break;          
          case TeclaRigth   : //----------------------------------------------
                              ponteiroDigito = (unsigned char)(ponteiroDigito+1)%2;
                              atualiza = 1;
                              timeToogle = 0x00;
                              toogle = 0xFF; 
                              timeOut = 1000;                             
                              break;          
          case TeclaConfirma:
                              loop = 0x00;
                              break;          
          case TeclaCancelar: 
                              loop = 0x00;                   
                              break;          
        }
     
      if(timeToogle)
        timeToogle--;
      else{
        toogle = ~toogle;
        timeToogle = 5;
        atualiza = 1;  
      }          
    
      if(atualiza){
        char tmp;        
        atualiza = 0x00;        
        if(toogle){          
          tmp = bufferDigitos[ponteiroDigito];
          bufferDigitos[ponteiroDigito] = '_';
        }    
        
        while(!EXTHW_waitSecurity(&semaforoBus));
        PTADD = 0xff;                    
        HD44780_posicionaTexto(6,1);    
        HD44780_writeString(bufferDigitos);  
        EXTHW_signal(&semaforoBus);                                                      
        
        if(toogle)
          bufferDigitos[ponteiroDigito] = tmp;
      }        
      //--------------------------
      //
      //--------------------------      
      SYSTICK_delay(50);               
  }
  while(loop && timeOut--);  
  
  emp = ((bufferDigitos[0]-'0')*10) + (bufferDigitos[1]-'0');
  
  if(emp>limite)
    emp=limite;
  
  return emp;
} 


/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/