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
|       Arquivo            :  Aplicacao.h
|       Data criação       :  17/03/2010
|       Revisões		       :  1.0
|
| __________________________________________________________________________________
*/

#ifndef _APLICACAO_H_
#define	_APLICACAO_H_

typedef enum{
  eEstadoInicial,
  eSistemaNormal,
  ePreAlarme,
  eAlarmeGeral,
  eMostraInfoAlarmes,
  eResetGeral,
  eFalhas,
  eMostraInfoFalhas,
  eMenuParametros,
  eConfigBloqueio
}eAplicacaoStates;

void APLICACAO_entry(void);
eAplicacaoStates SISNORMAL_process(void);


/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/
#endif // _APLICACAO_H_