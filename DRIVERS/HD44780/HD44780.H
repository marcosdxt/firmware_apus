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
|       Arquivo            :  HD44780.H
|       Data criação       :  06/05/2010
|       Revisões		       :  1.0 - INICIAL
|
| __________________________________________________________________________________
*/

#ifndef _HD44780_H_
#define	_HD44780_H_


#define LCD_CURSOR_LIGADO                 0x02
#define LCD_CURSOR_DESLIGADO              0x00
#define LCD_CURSOR_PISCANTE               0x01
#define LCD_CURSOR_FIXO                   0x00
#define LCD_DISPLAY_LIGADO                0x04
#define LCD_DISPLAY_DESLIGADO             0x00
#define LCD_DISPLAY_8X5                   0x00
#define LCD_DISPLAY_10X5                  0x04
#define LCD_2_LINHAS                      0x08
#define LCD_1_LINHA                       0x00


void HD44780_init(unsigned char modo1,unsigned char modo2);
void HD44780_posicionaTexto(unsigned char x,unsigned char y);
void HD44780_writeChar(char c);
void HD44780_writeString(unsigned char *string);

void HD44780_ligaDisplay(void);
void HD44780_desligaDisplay(void);
void HD44780_ligaCursor(void);
void HD44780_desligaCursor(void);
void HD44780_ligaCursorPiscante(void);
void HD44780_desligaCursorPiscante(void);
void HD44780_writeTwoLines(char * line1,char *line2);

#define HD44780_cls()         HD44780_writeChar('\f');
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/
#endif // _HD44780_H_