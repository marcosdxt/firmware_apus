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
|       Produto            :
|       Módulo de software :
|       Local no servidor  :
|       Arquivo            :
|       Descrição          :
| 
|       Autor              :
|       Data criação       :
|
|       Revisões           :
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*		Includes
***********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "HD44780.H"
#include "HD44780_targetWrapper.h"
#include "EXTHW.H"
/***********************************************************************************
*		Constantes
***********************************************************************************/
#define ENDERECO_SEGUNDA_LINHA              0xC0
#define DELAY_CICLES                        1100
/***********************************************************************************
*		Variaveis locais
***********************************************************************************/
unsigned char controleLcd=0x00;

/***********************************************************************************
*		Funções locais
***********************************************************************************/
void HD44780_delay_ms(unsigned int delayCyCles); 
void HD44780_enviaNibble(unsigned char value);
void HD44780_enviaByte(unsigned char endereco,unsigned char value);

/***********************************************************************************
*		Implementação
***********************************************************************************/

/***********************************************************************************
*   Descrição   :   Inicialização do módulo
*   Parametros  :   nenhum
*   Retorno     :   nenhum
***********************************************************************************/
void HD44780_init(unsigned char modo1,unsigned char modo2){
  unsigned char i;
  
  INIT_DIR_PINS;
  
  
  HD44780_delay_ms(100);

  for(i=0;i<3;i++){
    HD44780_enviaNibble(0x03);
    HD44780_delay_ms(100);  
  }
  
  HD44780_enviaNibble(0x02);
  HD44780_delay_ms(20);      
  
  
  HD44780_enviaByte(0,0x20 | modo1);
  HD44780_delay_ms(20);        
  HD44780_enviaByte(0,0x08 | modo2);
  HD44780_delay_ms(20);        
  
  controleLcd = 0x08 | modo2;
  
  HD44780_enviaByte(0,0x10);
  HD44780_delay_ms(10);          
  HD44780_enviaByte(0,0x06);   
  HD44780_delay_ms(10);          
}
/***********************************************************************************
*    Descrição   :   Envia um nibble para o controlador do LCD
*    Parametros  :   (unsigned char) valor que será escrito no display
*    Retorno     :   nenhum
***********************************************************************************/
void HD44780_enviaNibble(unsigned char value){
 
  SET_LCD_ENABLE; 
  __asm NOP;  
  LCD_WRITE_NIBBLE(value);      
  __asm NOP;
                         
  CLR_LCD_ENABLE;   
}
/***********************************************************************************
*    Descrição    :   Envia um byte para o controlador do LCD
*    Parametros   :   (unsigned char) endereço
*                     (unsigned char) valor que será escrito no controlador
*    Retorno      :   nenhum
***********************************************************************************/
void HD44780_enviaByte(unsigned char endereco,unsigned char value){

  (endereco)?(SET_LCD_RS):(CLR_LCD_RS);                              
  HD44780_enviaNibble(value>>4);
  HD44780_enviaNibble(value);  
  
  HD44780_delay_ms(1);  
}
/***********************************************************************************
*    Descrição    :   Delay utilizado dentro do módulo
*    Parametros   :   (unsigned int) número de iterações
*    Retorno      :   nenh8um
***********************************************************************************/
void HD44780_delay_ms(unsigned int delayCyCles){
  unsigned int i,count;
  
  for(i=0;i<delayCyCles;i++)
    for(count=DELAY_CICLES;count;count--);           
}
/***********************************************************************************
*     Descrição   :   Posiciona o cursor em uma determinada linha do display   
*     Parametros  :   (unsigned char) X
*                     (unsigned char) Y
*     Retorno     :   (nenhum)
***********************************************************************************/
void HD44780_posicionaTexto(unsigned char x,unsigned char y){
    
  if(x<16 && y==0)
     HD44780_enviaByte(0,0x80 + x);
  else
    if(y==1)
      HD44780_enviaByte(0,0xC0 + x);
    
  HD44780_delay_ms(2);      
}
/***********************************************************************************
*     Descrição    :  Escreve um caracter no display
*     Parametrs    :  (char)
*     Retorno      :  nenhum
***********************************************************************************/
void HD44780_writeChar(char c){
  
  switch(c){
    case '\f':  HD44780_enviaByte(0,1);
                HD44780_delay_ms(12);
                break;    
    case '\n':    
    case '\r':  HD44780_enviaByte(1,2);
                break;    
    default  :  HD44780_enviaByte(1,c);
                break;
  }
}
/***********************************************************************************
*     Descrição   :   Escreve uma string no display
*     Parametros  :   (char*) Ponteiro para a string
*     Retorno     :   nenhum
***********************************************************************************/
void HD44780_writeString(unsigned char *string){
  
  while(*string)
    HD44780_writeChar(*string++);      
}
/***********************************************************************************
*     Descrição   :   Liga o display
*     Parametros  :   (nenhum)
*     Retorno     :   (nenhum)
***********************************************************************************/
void HD44780_ligaDisplay(void){
 
  controleLcd |= 0x04;
  HD44780_enviaByte(0,controleLcd);  
}
/***********************************************************************************
*     Descrição    :    Desliga o display
*     Parametros   :    (nenhum)
*     Retorno      :    (nenhum)
***********************************************************************************/
void HD44780_desligaDisplay(void){
 
  controleLcd &= 0xFB;
  HD44780_enviaByte(0,controleLcd);
}
/************************************************************************************
*     Descrição     :   Liga o cursor
*     Parametros    :   nenhum
*     Retorno       :   nenhum
***********************************************************************************/
void HD44780_ligaCursor(void){
  
  controleLcd |= 0x02;
  HD44780_enviaByte(0,controleLcd);  
}
/***********************************************************************************
*     Descrição     :   Desliga o cursor
*     Parametros    :   nenhum
*     Retorno       :   nenhum
***********************************************************************************/
void HD44780_desligaCursor(void){
  
  controleLcd &= 0xFD;
  HD44780_enviaByte(0,controleLcd);  
}
/***********************************************************************************
*     Descrição     :   Liga o cursor piscante
*     Parametros    :   nenhum
*     Retorno       :   nenhum
***********************************************************************************/
void HD44780_ligaCursorPiscante(void){
 
  controleLcd |= 0x01;
  HD44780_enviaByte(0,controleLcd); 
}
/***********************************************************************************
*     Descrição     :   Desliga o cursor piscante
*     Parametros    :   nenhum
*     Retorno       :   nenhum
***********************************************************************************/
void HD44780_desligaCursorPiscante(void){
 
   controleLcd &= 0xFE;
   HD44780_enviaByte(0,controleLcd);
}                                   
/***********************************************************************************
*     Descrição     :   Escreve nas duas linhas do LCD
*     Parametros    :   (char*) conteúdo da primeira linha
*                       (char*) conteúdo da segunda linha
*     Retorno       :   nenhum
***********************************************************************************/
void HD44780_writeTwoLines(char * line1,char *line2){
 
  while(!EXTHW_waitSecurity(&semaforoBus));
  PTADD = 0xff;  
  HD44780_posicionaTexto(0,0);  
  if(line1!=NULL)
    HD44780_writeString(line1);
  HD44780_posicionaTexto(0,1);
  if(line2!=NULL)
    HD44780_writeString(line2); 
  EXTHW_signal(&semaforoBus);         
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/