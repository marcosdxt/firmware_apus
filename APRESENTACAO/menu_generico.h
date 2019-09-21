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
|       Arquivo            :  menu_generico.h
|       Data criação       :  18/03/2011
|       Revisões		       :  1.0
|
| __________________________________________________________________________________
*/

#ifndef _MENU_GENERICO_H_
#define	_MENU_GENERICO_H_


#define TEMPO_MAXIMO_INATIVIDADE_TECLADO                                15

typedef enum{
  StaticLabel,
  TextBox,
  NumberBox,
  CheckBox   
}eComTypes;

typedef struct{
  char *optionName;
  void(*function)(void);  
}sTermo;

typedef struct{
  char *menu_title;
  unsigned char number_of_options;
  sTermo *options_table;    
}sMenuInfoTable;

typedef struct{
  unsigned char x,y,offSet,size;
  char *text;
}sComponente;

typedef struct{
  eComTypes tipo;
  sComponente component;    
}sGenericComponent;

typedef struct{
  char *title;
  unsigned char *tabelaDados,sizeTabelaDados;
  unsigned char components_counter;
  sGenericComponent *listaComponentes;
  unsigned char(*funcCarrega)(void);
  unsigned char(*funcValidacao)(void);
  void(*funcSalvamente)(void);
}sOneLineMenu;

void MENUGENERICO_show(sMenuInfoTable tabelaMenu);
void MENUGENERICO_showEdit(sOneLineMenu tabela);
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/
#endif // _MENU_GENERICO_H_