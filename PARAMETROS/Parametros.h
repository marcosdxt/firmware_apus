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

#ifndef _PARAMETROS_H_
#define _PARAMETROS_H_


typedef struct{
  unsigned int AlarmeGeral  :1;
  unsigned int PreAlarme    :1;  
  unsigned int LacoEmAlarme :1;
  unsigned int PontoEmAlarme:1;  
  unsigned int LacoBloqueado:1;
  unsigned int LacoDefeito  :1;  
  unsigned int FaltaFB      :1;
  unsigned int LacoCruzado  :1;
}eRelesSignal;

typedef struct{
  eRelesSignal bits;
  unsigned char laco;
  unsigned char ponto;
  unsigned char pontoAuxiliar;
  unsigned int CRC;    
}eReleRow;


typedef enum{
  Impressora=0xBA,
  Modbus  
}eModCommType;

void PARAMETROS_configLaco(unsigned char laco,unsigned char bit);
unsigned char PARAMETROS_carregaConfigLaco(unsigned char laco);

unsigned char PARAMETROS_gravaConfiguracaoNome(unsigned char laco,unsigned char ponto,char* nome);
unsigned char PARAMETROS_leConfiguracaoNome(unsigned char laco,unsigned char ponto,char *nome);
unsigned char PARAMETROS_apagaConfiguracaoNome(unsigned char laco,unsigned char ponto);

unsigned char PARAMETROS_gravarConfigExpansaoSirenes(unsigned char habilita);
unsigned char PARAMETROS_leConfigExpansaoSirenes(void);

unsigned char PARAMETROS_gravaConfigRele(unsigned char moduloReles,
                                         unsigned char alarmeGeral,unsigned char preAlarme,
                                         unsigned char lacoEmAlarme,unsigned char pontoEmAlarme,
                                         unsigned char lacoBloqueado,unsigned char lacoDefeito,
                                         unsigned char faltaRede,unsigned char faltaBateria,
                                         unsigned char lacoP,unsigned char pontoP,unsigned char pontoAuxiliar);
unsigned char PARAMETROS_leConfigReles(unsigned char moduloReles,eReleRow* rele);     

unsigned char PARAMETROS_gravaEnderecoApusNet(unsigned char endereco);
unsigned char PARAMETROS_leEnderecoApusNet(void);        

unsigned char PARAMETROS_gravaConfigModuloComm(eModCommType modulo);
eModCommType PARAMETROS_leConfigModuloComm(void);   

void PARAMETROS_gravaTempoAlarmeGeral(unsigned char tempo);
unsigned char PARAMETROS_tempoAlarmeGeral(void);           

unsigned char PARAMETROS_gravaBloqueio(unsigned char laco,unsigned char valor);
unsigned char PARAMETROS_leBloqueio(unsigned char laco);                                                                                                

#endif



  