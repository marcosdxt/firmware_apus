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
|       Produto            :  Central Apus CD432
|       Arquivo            :  EventosSistema.c
|       Descrição          :  Filas de eventos do sistema
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  ??/??/??
|
|       Revisões           :  1.0.0.
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*		Includes
***********************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "eeprom_Map.h"
#include "EventosSistema.h"
#include "eventos.h"
#include "HT1381.H"
#include "UTIL.H"
#include "parametros.h"
/***********************************************************************************
*		Variaveis locais
***********************************************************************************/
const eFila  filaAlarmes   = {ADR_INICIO_BANCO_LOG_ALARMES,ADR_INICIO_BLOCOS_ALARME,CAPACIDADE_BLOCOS_ALARME,TAMANHO_BLOCO_ALARME},
             filaDefeitos  = {ADR_INICIO_BANCO_LOG_DEFEITOS,ADR_INICIO_BLOCO_DEFEITOS,CAPACIDADE_BLOCOS_DEFEITO,TAMANHO_BLOCO_DEFEITO},              
             filaBloqueios = {ADR_INICIO_BANCO_BLOQUEIOS,ADR_INICIO_BLOCO_BLOQUEIOS,CAPACIDADE_BLOCOS_BLOQUEIOS, TAMANHO_BLOCO_BLOQUEIOS};
           
unsigned char novoRelatorio=0;             
/***********************************************************************************
*		Funções locais
***********************************************************************************/
 
/***********************************************************************************
*		Implementação
***********************************************************************************/

/***********************************************************************************
*   Descrição       :   Inicializa o módulo de gerenciamento dos eventos
*                       do sistema
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void EVSY_init(void){  
 //(void)FILAEVENTO_clear((eFila*)&filaAlarmes);//PARA TESTE SOMENTE... 
 //(void)FILAEVENTO_clear((eFila*)&filaDefeitos);//PARA TESTE SOMENTE...
 //(void)FILAEVENTO_clear((eFila*)&filaBloqueios);//PARA TESTE SOMENTE...
}
/***********************************************************************************
*   Descrição       :   Insere um novo alarme na fila de alarmes
*   Parametros      :   (unsigned char) laco
*                       (unsigned char) ponto
*   Retorno         :   (unsigned char) Maior do que zero se conseguir inserir
*                       o evento na fila
***********************************************************************************/
unsigned char EVSY_insereAlarme(unsigned char laco,unsigned char ponto){
  unsigned char hour,minute,second,day,month,year;
  sEventoAlarme alarme;
      
  HT1381_leRelogio(&hour,&minute,&second);       
  HT1381_leCalendario(&day,&month,&year);     
  
  alarme.laco = laco;
  alarme.ponto = ponto;  
  alarme.hora = hour;
  alarme.minuto = minute;
  alarme.segundo = second;
  alarme.dia = day;
  alarme.mes = month;
  alarme.ano = year;
  alarme.crc = UTIL_calculaCRC16((unsigned char*)&alarme,8);
  
  (void)FILAEVENTO_insert((eFila*)&filaAlarmes,(void*)&alarme);      
  return 0xFF;  
}
/***********************************************************************************
*   Descrição       :   Lê um evento da fila a partir dos eventos do sistema
*   Parametros      :   (unsigned int) indice do evento
*                       (unsigned char*) laco
*                       (unsigned char*) ponto
*                       (unsigned char*) hora
*                       (unsigned char*) minuto
*                       (unsigned char*) segundo
*                       (unsigned char*) dia
*                       (unsigned char*) mes
*                       (unsigned char*) ano
*   Retorno          :  (unsigned char) maior do que zero se o evento existir na
*                       fila
***********************************************************************************/
unsigned char EVSY_getAlarme(unsigned int indice,unsigned char *laco,unsigned char *ponto,
                             unsigned char *hour,unsigned char *minute,unsigned char *second,
                             unsigned char *day,unsigned char *month,unsigned char *year){
  sEventoAlarme alarme;
  unsigned char flag;
  
  flag = FILAEVENTO_getAt((eFila*)&filaAlarmes,indice,(void*)&alarme);  
  
  *laco = alarme.laco;
  *ponto = alarme.ponto;
  *hour = alarme.hora;
  *minute = alarme.minuto;
  *second = alarme.segundo;
  *day = alarme.dia;
  *month = alarme.mes;
  *year = alarme.ano;      
  
  return flag;
}
/***********************************************************************************
*   Descrição        :   Insere um evento de defeito na fila de defeitos do
*                        sistema
*   Parametros       :   (eDefs) defeito
*                        (unsigned char) parametro 1
*                        (unsigned char) parametro 2
*   Retorno          :   (unsigned char) maior do que zero se conseguir inserir
*                        o evento na fila
***********************************************************************************/
unsigned char EVSY_insereDefeito(eDefs defeito,unsigned char p1,unsigned char p2){
  sEventoDefeito fault;
  unsigned char hour,minute,second,day,month,year;
  
  HT1381_leRelogio(&hour,&minute,&second); 
  HT1381_leCalendario(&day,&month,&year);  
  
  
  fault.descricao = defeito;
  fault.parametro1 = p1;
  fault.parametro2 = p2;
  fault.hora = hour;
  fault.minuto = minute;
  fault.segundo = second;
  fault.dia = day;
  fault.mes = month;
  fault.ano = year;
  fault.crc = UTIL_calculaCRC16((unsigned char*)&fault,10);
  (void)FILAEVENTO_insert((eFila*)&filaDefeitos,(void*)&fault);           
  return 1; 
}
/***********************************************************************************
*   Descrição         :   Lê um evento da fila a partir dos eventos do sistema
*   Parametros        :   (unsigned int) indice da fila de eventos
*                         (eDefs*) ponteiro para a descrição do evento
*                         (unsigned char*) parametro 1
*                         (unsigned char*) parametro 2
*                         (unsigned char*) hora
*                         (unsigned char*) minuto
*                         (unsigned char*) segundo
*                         (unsigned char*) dia
*                         (unsigned char*) mes
*                         (unsigned char*) ano
*   Retorno            :  (unsigned char) Maior do que zero se o evento existir na
*                         fila
***********************************************************************************/
unsigned char EVSY_getDefeito(unsigned int indice,eDefs *descricao,unsigned char *pm1,unsigned char *pm2,
                              unsigned char *hour,unsigned char *minute,unsigned char *second,
                              unsigned char *day,unsigned char *month,unsigned char *year){
  sEventoDefeito fault;
  unsigned char flag;
 
  flag = FILAEVENTO_getAt((eFila*)&filaDefeitos,indice,(void*)&fault);       
  *descricao = fault.descricao;  
  *pm1 = fault.parametro1;
  *pm2 = fault.parametro2;
  *hour = fault.hora;
  *minute = fault.minuto;
  *second = fault.segundo;
  *day = fault.dia;
  *month = fault.mes;
  *year = fault.ano;      
  
  return flag;  
}
/***********************************************************************************
*   Descrição           :   Insere um evento de bloqueio na fila de eventos
*                           de bloqueio do sistema
*   Parametros          :   (unsigned char) bloqueio/desbloqueio
*                           (unsigned char) número do laço
*   Retorno             :   (unsigned char) maior do que zero se conseguir 
*                           inserir o evento na memória
***********************************************************************************/
unsigned char EVSY_insereBloqueio(unsigned char bloqueio,unsigned char laco){
  sEventoBloqueio lock;
  unsigned char hour,minute,second,day,month,year;
  //char linha[33];
  
  HT1381_leRelogio(&hour,&minute,&second); 
  HT1381_leCalendario(&day,&month,&year);  
  
  lock.laco = laco;
  lock.flagBloqueio = bloqueio;
  lock.hora = hour;
  lock.minuto = minute;
  lock.segundo = second;
  lock.dia = day;
  lock.mes = month;
  lock.ano = year;  
  lock.crc = UTIL_calculaCRC16((unsigned char*)&lock,10);
  (void)FILAEVENTO_insert((eFila*)&filaBloqueios,(void*)&lock); 
  return 0xFF; 
}
/***********************************************************************************
*   Descrição           :   Lê um evento de bloqueio da fila
*   Parametros          :   (unsigned int) indice do evento
*                           (unsigned  char*)bloqueio/desbloqueio
*                           (unsigned char) número do laço
*                           (unsigned char*) hora
*                           (unsigned char*) minuto
*                           (unsigned char*) segundo
*                           (unsigned char*) dia
*                           (unsigned char*) mes
*                           (unsigned char*) ano
*   Retorno              :  nenhum
***********************************************************************************/
unsigned char EVSY_getEventoBloqueio(unsigned int indice,unsigned char *bloqueio,unsigned char *laco,
                                     unsigned char *hour,unsigned char *minute,unsigned char *second,
                                     unsigned char *day,unsigned char *month,unsigned char *year){
  sEventoBloqueio lock;
  unsigned char flag;
  
  flag = FILAEVENTO_getAt((eFila*)&filaBloqueios,indice,(void*)&lock); 
  
  *bloqueio = lock.flagBloqueio;
  *laco = lock.laco;
  *hour = lock.hora;
  *minute = lock.minuto;
  *second = lock.segundo;
  *day = lock.dia;
  *month = lock.mes;
  *year = lock.ano; 
  
  return 0;  
}                                       
/***********************************************************************************
*   Descrição       :   Cria um toString para a enumeração dos defeitos
*                       do sistema
*   Parametros      :   (eDefs) tipo do defeit
*   Retorno         :   (char*) Ponteiro para a string com o nome do defeito
***********************************************************************************/
char *EVSY_defToString(eDefs defeito){
  
  switch(defeito){
    case FaltaTerminador        : return " Falta RTL      ";
    case CurtoCircuitoLaco      : return " Curto laco     ";
    case FaltaTerminadorSirene  : return " Falta RTS      ";
    case CurtoSaidaSirene       : return " Curto Saida sir";
    case LacoNormalizado        : return "Laco normalizado";
    case SireneNormalizada      : return "Sir normalizada ";
    case FaltaRedeEletrica      : return "Falta rede      ";
    case FaltaBateria           : return "Falta bateria   ";
    case SubTensaoFonte         : return "Sub tensao      ";
    case SobreTensaoFonte       : return "Sobre tensao    ";
    case FugaAoTerraPositivo    : return "Fuga ao terra + ";
    case FugaAoTerraNegativo    : return "Fuga ao terra - ";
    case NormalizadaRedeEletrica: return "Norm. rede      ";
    case NormalizadaBateria     : return "Norm. bateria   ";
    case NormalizadaNivelTensao : return "Norm. nivel     ";
    case NormalizadaFugaAoTerra : return "Norm. fuga      ";
    case FalhaCommImpressora    : return "Falha comm imp. ";
    case FalhaCommModbus        : return "Falha comm mod  ";
    case FalhaCommApusNet       : return "Falha comm Apus ";
    default                     : return "                ";
  }    
}
/***********************************************************************************
*   Descrição       :   Retorna a quantidade de novo eventos de alarme
*                       disponíveis na fila
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
unsigned int EVSY_getNumberOfAlarms(void){
  
  return FILAEVENTOS_size((eFila*)&filaAlarmes);     
}
/***********************************************************************************
*   Descrição       :   Faz o ack dos eventos que foram mostrados
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void EVSY_ackAlarms(unsigned int ackCounter){
  
  FILAEVENTOS_ackEventos((eFila*)&filaAlarmes,ackCounter);
} 
/***********************************************************************************
*   Descrição       :   Retorna a quantidade de novo eventos de alarme
*                       disponíveis na fila
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
unsigned int EVSY_getNumberOfDefeitos(void){
  
  return FILAEVENTOS_size((eFila*)&filaDefeitos);     
}
/***********************************************************************************
*   Descrição       :   Faz o ack dos eventos que foram mostrados
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void EVSY_ackDefeitos(unsigned int ackCounter){
  
  FILAEVENTOS_ackEventos((eFila*)&filaDefeitos,ackCounter);
} 
/***********************************************************************************
*   Descrição       :   Retorna a quantidade de novo eventos de alarme
*                       disponíveis na fila
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
unsigned int EVSY_getNumberOfBloqueios(void){
  
  return FILAEVENTOS_size((eFila*)&filaBloqueios);     
}
/***********************************************************************************
*   Descrição       :   Faz o ack dos eventos que foram mostrados
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void EVSY_ackBloqueios(unsigned int ackCounter){
  
  FILAEVENTOS_ackEventos((eFila*)&filaBloqueios,ackCounter);
} 
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/