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
|       Arquivo            :  Teclado.h
|       Data criação       :  07/05/2010
|       Revisões		       :  1.0
|
| __________________________________________________________________________________
*/

#ifndef _TECLADO_H_
#define	_TECLADO_H_

#define NIVEIS_BUFFER_TECLAS                          10

typedef enum{
  SemTecla=0,
  TeclaDesligaSirenes,
  TeclaSilenciarBipInterno,
  TeclaParametros,
  TeclaBloqueios,
  TeclaAlarmeGeral,
  TeclaReinicarSistema,
  TeclaUP,
  TeclaDown,
  TeclaLeft,
  TeclaRigth,
  TeclaConfirma,
  TeclaCancelar
}eTeclas;

void TECLADO_init(void);
eTeclas TECLADO_getTecla(unsigned char *borda);
const unsigned char *TECLADO_getKeyName(eTeclas tecla);
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/
#endif // _TECLADO_H_