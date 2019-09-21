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
|       Arquivo        :
|       Data criação   :
|       Revisões		   :
|
| __________________________________________________________________________________
*/

#ifndef _API_LACOS_H_
#define _API_LACOS_H_

typedef struct{
  unsigned int flagAlarme:1;
  unsigned int flagTerminador:1;
  unsigned int flagCurto:1;    
}sBitsLaco;

typedef struct{
  unsigned int flagCurto:1;
  unsigned int flagTerminador:1;    
}sBitsSirene;
              
typedef enum{
  Laco_01=0,
  Laco_02,
  Laco_03,
  Laco_04  
}eLacos;

typedef enum{
  Rele_01,
  Rele_02,
  Rele_03,
  Rele_04  
}eRele;

typedef enum{
  SaidaSireneOnboard,
  SaidaSireneAuxiliar    
}eSaidasSirene;

typedef enum{
  ErroAoAcionarSaida,
  SaidaAcionadaComSucesso,
  Desligado    
}eResAcionamento;

//Essas funções devem ser removidas da interface do módulo...
sBitsLaco APILACOS_getFlagsLacoOnBoard(void);
sBitsSirene APILACOS_getFlagsSireneOnBoard(void);
//---

void APILACOS_init(void);

sBitsLaco APILACOS_getFlags(eLacos laco);
void APILACOS_setterCoils(eLacos laco,unsigned char valor);

sBitsSirene APILACOS_getFlagSirenes(eSaidasSirene saida);
eResAcionamento APILACOS_acionaSaida(eSaidasSirene saida,unsigned char valor);

void APILACOS_acionaRele(eRele rele,unsigned char valor);


#endif//_API_LACOS_H_