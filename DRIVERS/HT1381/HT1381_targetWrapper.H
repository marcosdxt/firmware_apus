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
|       Arquivo            :
|       Data criação       :
|       Revisões		       :
|
| __________________________________________________________________________________
*/
#ifndef _HT1381_TARGET_WRAPPER_H_
#define _HT1381_TARGET_WRAPPER_H_


#include <MC9S08AC32.H>

#define SCLK_PIN_DIR        PTFDD_PTFDD6
#define RESET_PIN_DIR       PTDDD_PTDDD7
#define IO_PIN_DIR          PTFDD_PTFDD7

#define SCLK_PIN            PTFD_PTFD6
#define RESET_PIN           PTDD_PTDD7
#define IO_PIN              PTFD_PTFD7

#define SET_SCLK()          SCLK_PIN = 1
#define CLR_SCLK()          SCLK_PIN = 0

#define SET_RESET_PIN()     RESET_PIN = 1
#define CLR_RESET_PIN()     RESET_PIN = 0

#define SET_IO_PIN()        IO_PIN = 1
#define CLR_IO_PIN()        IO_PIN = 0
#define GET_IO_PIN()        IO_PIN
#define IO_PIN_INPUT()      IO_PIN_DIR = 0
#define IO_PIN_OUTPUT()     IO_PIN_DIR = 1

#define INIT_PINS_RTC()     {SCLK_PIN_DIR = 1;\
                             IO_PIN_DIR = 1;\
                             RESET_PIN_DIR =1;}
                             
extern void SYSTICK_delay(unsigned int delay);                             
#define DELAY(X)           SYSTICK_delay(X);
    

#endif