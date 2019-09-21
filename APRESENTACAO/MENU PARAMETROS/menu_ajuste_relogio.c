#include <stdlib.h>
#include <stdio.h>
#include "appConfig.h"
#include "menu_ajuste_relogio.h"
#include "ht1381.h"
#include "exthw.h"
#include "beep.h"
#include "teclado.h"
#include "hd44780.h"
#include "systick.h"

void MENURELOGIO_telaAjuste(void){
  unsigned int timeOut=1000;
  const tamanhoDigitos[12] = {3,10,
                              7,10,
                              7,10,
                              4,10,
                              2,10,
                              10,10};    
                              
  unsigned char loop=0xff,ponteiro=0,digitos[12]={0,0,0,0,0,0,0,0,0,0,0,0},borda,
                hour,minute,second,day,month,year,atualizaLCD=1,toogle=0x00,
                digitosTmp[12],counter=5,salvar;
  eTeclas tecla;
  char linha[2][9];
  
  HD44780_cls();  
  HD44780_writeTwoLines(NULL,NULL);
  
  HT1381_leRelogio(&hour,&minute,&second);
  HT1381_leCalendario(&day,&month,&year);
 
  if(hour>23) hour = 0;
  if(minute>59) minute = 0;
  if(second>59) second = 0;
  if(day>31) day = 0;
  if(month>12) month=0;
  if(year>99) year=0;
 
  digitos[0]  = (hour/10)%10;
  digitos[1]  = hour%10;
  digitos[2]  = (minute/10)%10;
  digitos[3]  = minute%10;
  digitos[4]  = (second/10)%10;
  digitos[5]  = second%10;
  digitos[6]  = (day/10)%10;
  digitos[7]  = day%10;
  digitos[8]  = (month/10)%10;
  digitos[9]  = month%10;
  digitos[10] = (year/10)%10;
  digitos[11] = year%10;
 
    
  do{
  
    if(counter)
      counter--;
    else{
      counter = 5;
      atualizaLCD = 1;
      toogle = ~toogle; 
    }
             
    tecla = TECLADO_getTecla(&borda);
    if(tecla && !borda)
      switch(tecla){  
        case TeclaDown    : (digitos[ponteiro])?(digitos[ponteiro]--):(digitos[ponteiro]=tamanhoDigitos[ponteiro]-1);
                            atualizaLCD = 1;
                            toogle = 0x00;                            
                            break;
        case TeclaUP      : digitos[ponteiro] = (unsigned char)((digitos[ponteiro]+1)%tamanhoDigitos[ponteiro]);
                            atualizaLCD = 1;
                            toogle = 0x00;                            
                            break;        
        case TeclaLeft    : (ponteiro)?(ponteiro--):(ponteiro=11);
                            atualizaLCD = 1;
                            toogle = 0x00;                            
                            break;
        case TeclaRigth   : ponteiro = (unsigned char)(ponteiro+1)%12;
                            atualizaLCD = 1;
                            toogle = 0x00;
                            break;        
        case TeclaConfirma: if(ponteiro<11){
                              ponteiro++;
                            } 
                            else{                              
                              salvar = 0xFF;
                              loop = 0x00;
                            }
                            break;        
        case TeclaCancelar: loop = 0x00;
                            break;        
      }
      
    
    // Espera para não ficar loco!!!!                            
    SYSTICK_delay(50);
                
    if(atualizaLCD){      
      (void)memcpy(digitosTmp,digitos,12);
       
      if(toogle)
        digitosTmp[ponteiro] = 0xEF;
      
      linha[0][0] = '0' + digitosTmp[0];
      linha[0][1] = '0' + digitosTmp[1];
      linha[0][2] = ':';
      linha[0][3] = '0' + digitosTmp[2];
      linha[0][4] = '0' + digitosTmp[3];
      linha[0][5] = ':';
      linha[0][6] = '0' + digitosTmp[4];
      linha[0][7] = '0' + digitosTmp[5];
      linha[0][8] = '\0';
      
      linha[1][0] = '0' + digitosTmp[6];
      linha[1][1] = '0' + digitosTmp[7];
      linha[1][2] = '/';
      linha[1][3] = '0' + digitosTmp[8];
      linha[1][4] = '0' + digitosTmp[9];
      linha[1][5] = '/';
      linha[1][6] = '0' + digitosTmp[10];
      linha[1][7] = '0' + digitosTmp[11];
      linha[1][8] = '\0';
      
      while(!EXTHW_waitSecurity(&semaforoBus));
      PTADD = 0xff;                 
      HD44780_posicionaTexto(4,0);
      HD44780_writeString(linha[0]);     
      HD44780_posicionaTexto(4,1);
      HD44780_writeString(linha[1]);                  
      EXTHW_signal(&semaforoBus);                     
    }            
  }
  while(loop && timeOut--);
  
  
  if(salvar){
       
    hour   = digitos[0]*10 + digitos[1];
    minute = digitos[2]*10 + digitos[3];
    second = digitos[4]*10 + digitos[5];
    
    day   = digitos[6 ]*10 + digitos[7];
    month = digitos[8 ]*10 + digitos[9];   
    year  = digitos[10]*10 + digitos[11];
    
    //Deu erro
    if(hour>23 || minute>59 || second>59 || day>31 || month>12 || year>99){      
      HD44780_writeTwoLines("   Parametros   ",
                            "   invalidos    ");
      SYSTICK_delay(60);             
      BEEP_setSound(1000);      
      SYSTICK_delay(500);         
      BEEP_setSound(1200);
      SYSTICK_delay(500);                  
      BEEP_setSound(0);        
    }
    
    HT1381_ajustaRelogio(hour,minute,second);
    HT1381_ajustaCalendario(day,month,year);                                        
  }          
}