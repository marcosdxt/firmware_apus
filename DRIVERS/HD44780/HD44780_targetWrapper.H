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
|       Arquivo            :  HD44780_targetWrapper.H
|       Data criação       :  06/05/2010
|       Revisões		       :  1.0 - Inicial
|
| __________________________________________________________________________________
*/
#ifndef _HD44780_TARGET_WRAPPER_H_
#define	_HD44780_TARGET_WRAPPER_H_

#include "MC9S08AC32.H"

#define LCD_D4_DIR            PTADD_PTADD4
#define LCD_D5_DIR            PTADD_PTADD5
#define LCD_D6_DIR            PTADD_PTADD6
#define LCD_D7_DIR            PTADD_PTADD7

#define LCD_RS_DIR            PTBDD_PTBDD7
#define LCD_ENABLE_DIR        PTBDD_PTBDD6

#define LCD_D4                PTAD_PTAD4
#define LCD_D5                PTAD_PTAD5
#define LCD_D6                PTAD_PTAD6
#define LCD_D7                PTAD_PTAD7
#define LCD_RS                PTBD_PTBD7
#define LCD_ENABLE            PTBD_PTBD6

#define INIT_DIR_PINS         { LCD_D4_DIR     = 0x01; \
                                LCD_D5_DIR     = 0x01; \
                                LCD_D6_DIR     = 0x01; \
                                LCD_D7_DIR     = 0x01; \
                                LCD_RS_DIR     = 0x01; \
                                LCD_ENABLE_DIR = 0x01;}
                                
#define LCD_WRITE_NIBBLE(X)   { LCD_D4 = (X & 0x01)?1:0;\
                                LCD_D5 = (X & 0x02)?1:0;\
                                LCD_D6 = (X & 0x04)?1:0;\
                                LCD_D7 = (X & 0x08)?1:0;}
                                
#define SET_LCD_RS            LCD_RS = 1
#define CLR_LCD_RS            LCD_RS = 0

#define SET_LCD_ENABLE        LCD_ENABLE = 1
#define CLR_LCD_ENABLE        LCD_ENABLE = 0
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/
#endif // _HD44780_TARGET_WRAPPER_H_