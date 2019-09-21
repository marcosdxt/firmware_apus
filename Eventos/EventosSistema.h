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

#ifndef _EVENTOS_SISTEMA_H_
#define	_EVENTOS_SISTEMA_H_

typedef struct{
  unsigned char laco;
  unsigned char ponto;
  unsigned char hora;
  unsigned char minuto;
  unsigned char segundo;
  unsigned char dia;
  unsigned char mes;
  unsigned char ano;  
  unsigned int  crc;  
}sEventoAlarme;

#define TAMANHO_ENUM_FALHAS                 19
typedef enum{
  FaltaTerminador,
  CurtoCircuitoLaco,  
  FaltaTerminadorSirene,
  CurtoSaidaSirene,  
  LacoNormalizado,
  SireneNormalizada,  
  FaltaRedeEletrica,
  FaltaBateria,
  SubTensaoFonte,
  SobreTensaoFonte,
  FugaAoTerraPositivo,
  FugaAoTerraNegativo,  
  NormalizadaRedeEletrica,
  NormalizadaBateria,
  NormalizadaNivelTensao,
  NormalizadaFugaAoTerra,  
  FalhaCommImpressora,
  FalhaCommModbus,
  FalhaCommApusNet,  
  Laco_01_bloqueado,
  Laco_02_bloqueado,
  Laco_03_bloqueado,
  Laco_04_bloqueado    
}eDefs;

typedef struct{
  eDefs descricao;
  unsigned char parametro1;
  unsigned char parametro2;
  unsigned char hora;
  unsigned char minuto;
  unsigned char segundo;
  unsigned char dia;
  unsigned char mes;
  unsigned char ano;
  unsigned int crc;      
}sEventoDefeito;

typedef struct{
  unsigned char laco;
  unsigned char flagBloqueio;
  unsigned char hora;
  unsigned char minuto;
  unsigned char segundo;
  unsigned char dia;
  unsigned char mes;
  unsigned char ano; 
  unsigned int crc;   
}sEventoBloqueio;     


void EVSY_init(void);

unsigned char EVSY_insereAlarme(unsigned char laco,unsigned char ponto);

unsigned char EVSY_getAlarme(unsigned int indice,unsigned char *laco,unsigned char *ponto,
                             unsigned char *hour,unsigned char *minute,unsigned char *second,
                             unsigned char *day,unsigned char *month,unsigned char *year);
                             
unsigned char EVSY_insereDefeito(eDefs defeito,unsigned char p1,unsigned char p2);                             

unsigned char EVSY_getDefeito(unsigned int indice,eDefs *descricao,unsigned char *pm1,unsigned char *pm2,
                              unsigned char *hour,unsigned char *minute,unsigned char *second,
                              unsigned char *day,unsigned char *month,unsigned char *year);
                              
unsigned char EVSY_insereBloqueio(unsigned char bloqueio,unsigned char laco);

unsigned char EVSY_getEventoBloqueio(unsigned int indice,unsigned char *bloqueio,unsigned char *laco,
                                     unsigned char *hour,unsigned char *minute,unsigned char *second,
                                     unsigned char *day,unsigned char *month,unsigned char *year);  
                                     
char *EVSY_defToString(eDefs defeito);                                                                 

unsigned int EVSY_getNumberOfAlarms(void);
void EVSY_ackAlarms(unsigned int ackCounter);
unsigned int EVSY_getNumberOfDefeitos(void);
void EVSY_ackDefeitos(unsigned int ackCounter);
unsigned int EVSY_getNumberOfBloqueios(void);
void EVSY_ackBloqueios(unsigned int ackCounter);


#endif // _FILE_NAME_H