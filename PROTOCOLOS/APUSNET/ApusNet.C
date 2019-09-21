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
|       Produto            :  Central Apus CD 432
|       Arquivo            :  ApusNet.C
|       Descrição          :  Implementação do protocolo ApusNET
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  13/07/2010
|
|       Revisões           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*		Includes
***********************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "ApusNet.h"
#include "ApusNet_table.h"
#include "ApusNet_targetWrapper.h"

#include "EventosSistema.h"
#include "UART.H"
#include "Util.h"
/***********************************************************************************
*   Definições
***********************************************************************************/
#define SIZE_INPUT_BUFFER                           32
#define SIZE_OUTPUT_BUFFER                          32
#define SILENT_TIME_RELOAD                          5
/***********************************************************************************
*		Enumerações
***********************************************************************************/


/***********************************************************************************
*		Estruturas
***********************************************************************************/


/***********************************************************************************
*		Uniões
***********************************************************************************/


/***********************************************************************************
*		Constantes
***********************************************************************************/


/***********************************************************************************
*		Variaveis locais
***********************************************************************************/
unsigned char inputBuffer[SIZE_INPUT_BUFFER],
              outputBuffer[SIZE_OUTPUT_BUFFER],
              APUSNET_bytesRecebidos=0,
              APUSNET_silentTime=0,
              bytesToSend,
              counterOut;
              
unsigned char APUSNET_progConexaoCounter=0,
              APUSNET_status=0,
              APUS_command=0;              
              
unsigned char APLICACAO_tempoAlarmeGeral=0;              
/***********************************************************************************
*		Funções locais
***********************************************************************************/
void APUSNET_rxdFunction(unsigned char data);

unsigned char *APUSNET_txdFunction(void); 

void APUSNET_decodeRTCwrite(unsigned char hour,unsigned char minute,unsigned char second,
                            unsigned char day,unsigned char month,unsigned char year);
                            
void APUSNET_decodeRTCread(void);

void APUSNET_setName(unsigned char laco,unsigned char ponto,char* nome);

void APUSNET_getName(unsigned char laco,unsigned char ponto);

void APUSNET_clrName(unsigned char laco,unsigned char ponto);

void APUSNET_numeroDeAlarmes(void);

void APUSNET_numeroDeDefeitos(void);

void APUSNET_numeroDeBloqueios(void);

void APUSNET_leEventoAlarme(unsigned int indice);

void APUSNET_leEventoDefeito(unsigned int indice);

void APUSNET_leEventoBloqueio(unsigned int indice);

void APUSNET_ackAlarmes(unsigned int numero);

void APUSNET_ackDefeitos(unsigned int numero);

void APUSNET_ackBloqueios(unsigned int numero);

void APUSNET_configuraRele(unsigned char nRegra,unsigned char flagAlarmeGeral,unsigned char flagPreAlarme,
                           unsigned char flagLacoEmAlarme,unsigned char flagPontoEmAlarme,unsigned char flagLacoBloqueado,
                           unsigned char flagLacoDefeito,unsigned char flagFaltaRede,unsigned char flagFaltaBateria,
                           unsigned char lacoParametro,unsigned char pontoParametro,unsigned char pontoAuxiliar);
                           
void APUSNET_carregaConfigRele(unsigned char numeroRegra); 

void APUSNET_abreConexao(void);           

void APUSNET_fechadConexao(void);    

void APUSNET_cfgTempoAlarmeGeral(unsigned char tempo);       

void APUSNET_leCfgTempoAlarmeGeral(void); 

void APUSNET_sendPackage(unsigned char *buffer,unsigned char tamanho);   

//Macro pra compatibilizar o código com o antigo driver da serial
#define UART1_writePackage(X,Y)           APUSNET_sendPackage(X,Y)      
/***********************************************************************************
*		Implementação
***********************************************************************************/

/***********************************************************************************
*   Descrição     : 
*   Parametros    : 
*   Retorno       : 
***********************************************************************************/
void APUSNET_init(void){
    
  init_sci2(19200, APUSNET_rxdFunction,APUSNET_txdFunction,NULL);           
}
/***********************************************************************************
*   Descrição     :   Função tratadora do RXD do protocolo
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void APUSNET_rxdFunction(unsigned char data){
  
  if(APUSNET_bytesRecebidos<SIZE_INPUT_BUFFER-1){    
    inputBuffer[APUSNET_bytesRecebidos] = data;
    APUSNET_bytesRecebidos++;
    
    APUSNET_silentTime = SILENT_TIME_RELOAD;     
  }
}
/***********************************************************************************
*   Descrição     :   Função tratadora do TXD do protocolo
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
unsigned char *APUSNET_txdFunction(void){
  
  if(bytesToSend){
    bytesToSend--;
    return &outputBuffer[counterOut++];
  }
  
  return NULL;  
}
/***********************************************************************************
*   Descrição     :   Decodificação da leitura do bloco de memória compartilhado
*   Parametros    :   (unsigned int) endereço no bloco
*   Retorno       :   (unsigned char) valor da posição de memória
***********************************************************************************/
unsigned char APUSNET_readByte(unsigned int address){
  
  if(address>ROW_COUNT)
    return 0;
  
  if(sharedTable[address].permission ==  ReadOnly || sharedTable[address].permission ==  ReadAndWrite)
    return *sharedTable[address].pData;
  
  return 0;
}
/***********************************************************************************
*   Descrição     :   Decodificação da escrita no bloco de memória compartilhado
*   Parametros    :   (unsigned int ) endereço do bloco
*                     (unsigned char) valor para ser escrito no bloco
*   Retorno       :   nenhum
***********************************************************************************/
void APUSNET_writeByte(unsigned int address,unsigned char data){
  
  if(address>ROW_COUNT)
    return;
  
  if(sharedTable[address].permission == ReadAndWrite || sharedTable[address].permission == WriteOnly)
    *sharedTable[address].pData = data;     
}
/***********************************************************************************
*   Descrição     :   Faz o pooling dos buffers para tratar os novos
*                     pacotes recebidos
*   Parametros    :   (nenhum)
*   Retorno       :   (nenhum)
***********************************************************************************/
void APUSNET_decodePackages(void){
  unsigned int crc;   
     
  //----------------------------------------------------------------------------------//    
  //       Aqui verifica os pacotes                                                   //
  //       recebidos pela serial ApusNET                                              //
  //----------------------------------------------------------------------------------//
  if(!APUSNET_silentTime && APUSNET_bytesRecebidos>2){
    
     if(inputBuffer[0] == 0x01 &&  UTIL_checa_crc(inputBuffer,APUSNET_bytesRecebidos-2)){
                              
       switch(inputBuffer[1]){
        case ReadStatus       :
                                outputBuffer[0] = 0x01;
                                outputBuffer[1] = ReadStatus;
                                outputBuffer[2] = 0x04;
                                outputBuffer[3] = APUSNET_status;
                                crc = UTIL_calculaCRC16( outputBuffer,4);
                                outputBuffer[4] = (unsigned char)(crc>>8);
                                outputBuffer[5] = (unsigned char)(crc);
                                UART1_writePackage(outputBuffer,6);                    
                                break;
        case SetClock         : APUSNET_decodeRTCwrite(inputBuffer[3],inputBuffer[4],inputBuffer[5],inputBuffer[6],inputBuffer[7],inputBuffer[8]);
                                break;      
        case GetClock         : APUSNET_decodeRTCread();
                                break;                                                                      
        case SetName          : APUSNET_setName(inputBuffer[4],inputBuffer[5],&inputBuffer[6]);
                                break;      
        case GetName          : APUSNET_getName(inputBuffer[4],inputBuffer[5]);
                                break;      
        case ClrName          : APUSNET_clrName(inputBuffer[4],inputBuffer[5]);
                                break;                                                                
        case NumberOfNewAlarms: APUSNET_numeroDeAlarmes();
                                break;        
        case NumberOfNewFaults: APUSNET_numeroDeDefeitos();
                                break;        
        case NumberOfNewLocks : APUSNET_numeroDeBloqueios();
                                break;                                                                                                
        case GetAlarmEvent    : APUSNET_leEventoAlarme  (inputBuffer[4]<<8 | inputBuffer[5]);
                                break;
        case GetFaultEvent    : APUSNET_leEventoDefeito (inputBuffer[4]<<8 | inputBuffer[5]);
                                break;        
        case GetLockEvent     : APUSNET_leEventoBloqueio(inputBuffer[4]<<8 | inputBuffer[5]);
                                break;                                                                                                  
        case AckAlarms        : APUSNET_ackAlarmes(inputBuffer[4]<<8 | inputBuffer[5]);
                                break;
        case AckFaults        : APUSNET_ackDefeitos(inputBuffer[4]<<8 | inputBuffer[5]);
                                break;
        case AckLocks         : APUSNET_ackBloqueios(inputBuffer[4]<<8 | inputBuffer[5]);
                                break;                                                               
        case SetRegraRele     : APUSNET_configuraRele(inputBuffer[3],inputBuffer[4],
                                                      inputBuffer[5],inputBuffer[6],
                                                      inputBuffer[7],inputBuffer[8],
                                                      inputBuffer[9],inputBuffer[10],
                                                      inputBuffer[11],inputBuffer[12],inputBuffer[13],inputBuffer[14]);                        
                                break;
        case GetRegraRele     : 
                                APUSNET_carregaConfigRele(inputBuffer[3]);        
                                break;
        case AbreConexaoProgramacao:
                                APUSNET_abreConexao();        
                                break;
        case FechaConexaoProgramacao:
                                APUSNET_fechadConexao();        
                                break;                                
        case SetTempoAlarmeGeral:
                                APUSNET_cfgTempoAlarmeGeral(inputBuffer[3]);
                                break;
        case GetTempoAlarmeGeral:     
                                APUSNET_leCfgTempoAlarmeGeral();
                                break;                                                                    
        case SetAlarmeGeral     : 
                                APUS_command = APUS_CMD_SETA_ALARME_GERAL;       
                                outputBuffer[0] = 0x01;
                                outputBuffer[1] = SetAlarmeGeral;
                                outputBuffer[2] = 0x04;
                                outputBuffer[3] = APUSNET_status;
                                crc = UTIL_calculaCRC16( outputBuffer,4);
                                outputBuffer[4] = (unsigned char)(crc>>8);
                                outputBuffer[5] = (unsigned char)(crc);
                                UART1_writePackage(outputBuffer,6);         
                                break;
        case ResetAlarmeGeral   :   
                                APUS_command = APUS_CMD_RESET_SISTEMA;      
                                outputBuffer[0] = 0x01;
                                outputBuffer[1] = ResetAlarmeGeral;
                                outputBuffer[2] = 0x04;
                                outputBuffer[3] = APUSNET_status;
                                crc = UTIL_calculaCRC16( outputBuffer,4);
                                outputBuffer[4] = (unsigned char)(crc>>8);
                                outputBuffer[5] = (unsigned char)(crc);
                                UART1_writePackage(outputBuffer,6);         
                                break;
        case DesligaSirenes     :        
                                APUS_command = APUS_CMD_DESLIGA_SIRENES; 
                                outputBuffer[0] = 0x01;
                                outputBuffer[1] = DesligaSirenes;
                                outputBuffer[2] = 0x04;
                                outputBuffer[3] = APUSNET_status;
                                crc = UTIL_calculaCRC16( outputBuffer,4);
                                outputBuffer[4] = (unsigned char)(crc>>8);
                                outputBuffer[5] = (unsigned char)(crc);
                                UART1_writePackage(outputBuffer,6);         
                                break;
        case DesligaBipInterno  : 
                                APUS_command = APUS_CMD_DESLIGA_BIP;       
                                outputBuffer[0] = 0x01;
                                outputBuffer[1] = DesligaBipInterno;
                                outputBuffer[2] = 0x04;
                                outputBuffer[3] = APUSNET_status;
                                crc = UTIL_calculaCRC16( outputBuffer,4);
                                outputBuffer[4] = (unsigned char)(crc>>8);
                                outputBuffer[5] = (unsigned char)(crc);
                                UART1_writePackage(outputBuffer,6);         
                                break;
        case TempoParaAlarme    :      
                                outputBuffer[0] = 0x01;
                                outputBuffer[1] = TempoParaAlarme;
                                outputBuffer[2] = 0x04;
                                outputBuffer[3] = APLICACAO_tempoAlarmeGeral;
                                crc = UTIL_calculaCRC16( outputBuffer,4);
                                outputBuffer[4] = (unsigned char)(crc>>8);
                                outputBuffer[5] = (unsigned char)(crc);
                                UART1_writePackage(outputBuffer,6);           
                                break;
     }                                                            
    }//Fim do if endereço e checagem do CRC    
    APUSNET_bytesRecebidos = 0x00;
  }//Fim das verificações
}
/***********************************************************************************
*   Descrição         :   Trata a função de escrita no RTC do sistema
*   Parametros        :   (unsigned char) hora
*                         (unsigned char) minuto
*                         (unsigned char) segundo
*                         (unsigned char) dia
*                         (unsigned char) mes
*                         (unsigned char) ano
*   Retorno           :   nenhum
***********************************************************************************/
void APUSNET_decodeRTCwrite(unsigned char hour,unsigned char minute,unsigned char second,
                            unsigned char day,unsigned char month,unsigned char year){
  unsigned int crc;
    
  SET_CLOCK(hour,minute,second);
  SET_CALENDAR(day,month,year);      
  
  outputBuffer[0] = 0x01;
  outputBuffer[1] = SetClock;
  outputBuffer[2] = 0x05;
  
  crc = UTIL_calculaCRC16(outputBuffer,3);
  outputBuffer[3] = (unsigned char)(crc>>8);
  outputBuffer[4] = (unsigned char)crc;
  
  bytesToSend = 5; 
  counterOut = 0x00;  
  UART1_writePackage(outputBuffer,5);
}
/***********************************************************************************
*   Descrição       :   Trata a função de leitura do RTC do sistema
*   Parametros      :   (nenhum)
*   Retorno         :   (nenhum)
***********************************************************************************/
void APUSNET_decodeRTCread(void){  
  unsigned int crc;  
  unsigned char hour,minute,second,day,month,year;
  
  GET_CLOCK(&hour,&minute,&second);
  GET_CALENDAR(&day,&month,&year);
 
  outputBuffer[0] = 0x01;
  outputBuffer[1] = GetClock;
  outputBuffer[2] = 11;
  outputBuffer[3] = hour;
  outputBuffer[4] = minute;
  outputBuffer[5] = second;
  outputBuffer[6] = day;
  outputBuffer[7] = month;
  outputBuffer[8] = year;
  crc = UTIL_calculaCRC16(outputBuffer,9);
  
  outputBuffer[9]  = (unsigned char)(crc>>8);
  outputBuffer[10] = (unsigned char)crc;
  
  bytesToSend = 11; 
  counterOut = 0x00;  
  UART1_writePackage(outputBuffer,11);
}
/***********************************************************************************
*   Descrição     :   Escreve na configuração dos nomes de ponto
*   Parametros    :   (unsigned char) número do laço
*                     (unsigned char) número do ponto
*                     (unsigned char*) nome do laço
*   Retorno       :   nenhum
***********************************************************************************/
void APUSNET_setName(unsigned char laco,unsigned char ponto,char* nome){
  char name[16];
  unsigned char i;
  unsigned int crc;
  
  for(i=0;i<16;i++)
    name[i] = nome[i];
    
  if(laco<=3 && ponto<=31){        
    (void)SET_NAME(laco,ponto,name);  
    
    outputBuffer[0] = 0x01;
    outputBuffer[1] = SetName;
    outputBuffer[2] = laco;
    outputBuffer[3] = ponto; 
    crc = UTIL_calculaCRC16(outputBuffer,4);
  
    outputBuffer[4] = (unsigned char)(crc>>8);
    outputBuffer[5] = (unsigned char)crc;   
    
    bytesToSend = 6; 
    counterOut = 0x00;  
    UART1_writePackage(outputBuffer,6);       
  }
  else{
    
    outputBuffer[0] = 0x01;
    outputBuffer[1] = 0x80 | SetName;
    outputBuffer[2] = laco;
    outputBuffer[3] = ponto; 
    crc = UTIL_calculaCRC16(outputBuffer,4);
  
    outputBuffer[4] = (unsigned char)(crc>>8);
    outputBuffer[5] = (unsigned char)crc;   
    
    bytesToSend = 6; 
    counterOut = 0x00;  
    UART1_writePackage(outputBuffer,6);       
  }
}
/***********************************************************************************
*   Descrição     :   Lê o nome de um dos pontos do sistema
*   Parametros    :   (unsigned char) número do laço
*                     (unsigned char) número do ponto
*   Retorno       :   nenhum
***********************************************************************************/
void APUSNET_getName(unsigned char laco,unsigned char ponto){
  char name[17];    
  unsigned int crc;
  
  if(laco<=3 && ponto<=31){
    if(!GET_NAME(laco,ponto,name)){
      name[0]  = ' ';
      name[1]  = 'N';
      name[2]  = 'a';
      name[3]  = 'o';
      name[4]  = ' ';
      name[5]  = 'c';
      name[6]  = 'o';
      name[7]  = 'n';
      name[8]  = 'f';
      name[9]  = 'i';
      name[10] = 'g';                                    
      name[11] = 'u';                                          
      name[12] = 'r';                                                
      name[13] = 'a';                                                      
      name[14] = 'd';                                                            
      name[15] = 'o';                                                            
    }
    outputBuffer[0] = 0x01;
    outputBuffer[1] = GetName;
    outputBuffer[2] = laco;
    outputBuffer[3] = ponto;
    outputBuffer[4] = name[0];
    outputBuffer[5] = name[1];
    outputBuffer[6] = name[2];
    outputBuffer[7] = name[3];
    outputBuffer[8] = name[4];
    outputBuffer[9] = name[5];
    outputBuffer[10]= name[6];
    outputBuffer[11]= name[7];
    outputBuffer[12]= name[8];
    outputBuffer[13]= name[9];
    outputBuffer[14]= name[10];
    outputBuffer[15]= name[11];
    outputBuffer[16]= name[12];
    outputBuffer[17]= name[13];
    outputBuffer[18]= name[14];
    outputBuffer[19]= name[15];    
    crc = UTIL_calculaCRC16(outputBuffer,20);        
    outputBuffer[20]= (unsigned char)(crc>>8);
    outputBuffer[21]= (unsigned char)(crc&0xFF);        
    bytesToSend = 22; 
    counterOut = 0x00;  
    UART1_writePackage(outputBuffer,22);         
  }
  else{
    outputBuffer[0] = 0x01;
    outputBuffer[1] = GetName;
    outputBuffer[2] = laco;
    outputBuffer[3] = ponto;

    crc = UTIL_calculaCRC16(outputBuffer,4);        
    
    outputBuffer[4]= (unsigned char)(crc>>8);
    outputBuffer[5]= (unsigned char)(crc&0xFF);        
    
    bytesToSend = 22; 
    counterOut = 0x00;  
    UART1_writePackage(outputBuffer,6);               
  }  
}
/***********************************************************************************
*   Descrição       :   Apaga uma configuração de nome de ponto 
*   Parametros      :   (unsigned char) Número do laço
*                       (unsigned char) Número do ponto
*   Retorno         :   nenhum
***********************************************************************************/
void APUSNET_clrName(unsigned char laco,unsigned char ponto){
  unsigned int crc;  
  
  if(laco<=3 && ponto<=31){      
   (void)PARAMETROS_apagaConfiguracaoNome(laco,ponto); 
   
   outputBuffer[0] = 0x01;
   outputBuffer[1] = ClrName;
   outputBuffer[2] = laco;
   outputBuffer[3] = ponto;
   
   crc = UTIL_calculaCRC16(outputBuffer,4);
   outputBuffer[4] = (unsigned char)(crc>>8);
   outputBuffer[5] = (unsigned char)crc;
   
   bytesToSend = 6; 
   counterOut = 0x00;  
   UART1_writePackage(outputBuffer,6);         
  }
  else{
    outputBuffer[0] = 0x01;
    outputBuffer[1] = 0x80 | ClrName;
    outputBuffer[2] = laco;
    outputBuffer[3] = ponto;  
    
   crc = UTIL_calculaCRC16(outputBuffer,4);
   outputBuffer[4] = (unsigned char)(crc>>8);
   outputBuffer[5] = (unsigned char)crc;  
      
   bytesToSend = 6; 
   counterOut = 0x00;  
   UART1_writePackage(outputBuffer,6);              
  }    
}
/***********************************************************************************
*   Descrição       :   Lê o número de eventos de novos alarmes disponíveis 
*                       na fila de alarmes do sistema
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void APUSNET_numeroDeAlarmes(void){
  unsigned int n,crc;
  
  n = EVSY_getNumberOfAlarms();  
  
  outputBuffer[0] = 0x01;
  outputBuffer[1] = NumberOfNewAlarms;
  outputBuffer[2] = 7;
  outputBuffer[3] = (unsigned char)(n>>8);
  outputBuffer[4] = (unsigned char) n;
  
  crc = UTIL_calculaCRC16(outputBuffer,5);
  outputBuffer[5] = crc>>8;
  outputBuffer[6] = (unsigned char)crc;
    
  bytesToSend = 7; 
  counterOut = 0x00;  
  UART1_writePackage(outputBuffer,7);           
}
/***********************************************************************************
*   Descrição       :   Lê o número de eventos de novos defeitos disponiveis 
*                       na fila de defeitos do sistema
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void APUSNET_numeroDeDefeitos(void){
  unsigned int n,crc;
  
  n = EVSY_getNumberOfDefeitos();  
  
  outputBuffer[0] = 0x01;
  outputBuffer[1] = NumberOfNewFaults;
  outputBuffer[2] = 7;
  outputBuffer[3] = (unsigned char)(n>>8);
  outputBuffer[4] = (unsigned char) n;
  
  crc = UTIL_calculaCRC16(outputBuffer,5);
  outputBuffer[5] = crc>>8;
  outputBuffer[6] = (unsigned char)crc;  
  
  bytesToSend = 7; 
  counterOut = 0x00;  
  UART1_writePackage(outputBuffer,7);          
}
/***********************************************************************************
*   Descrição       :   Lê o número de novos eventos de bloqueio na fila de 
*                       bloqueios do sistema
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void APUSNET_numeroDeBloqueios(void){
  unsigned int n,crc;
  
  n = EVSY_getNumberOfBloqueios();  
  
  outputBuffer[0] = 0x01;
  outputBuffer[1] = NumberOfNewLocks;
  outputBuffer[2] = 7;
  outputBuffer[3] = (unsigned char)(n>>8);
  outputBuffer[4] = (unsigned char) n;
  
  crc = UTIL_calculaCRC16(outputBuffer,5);
  outputBuffer[5] = crc>>8;
  outputBuffer[6] = (unsigned char)crc;  
  
  bytesToSend = 7; 
  counterOut = 0x00;  
  UART1_writePackage(outputBuffer,7);         
}
/***********************************************************************************
*   Descrição       :   Lê um evento de alame da fila do sistema
*   Parametros      :   (unsigned int) indice
*   Retorno         :   nenhum
***********************************************************************************/
void APUSNET_leEventoAlarme(unsigned int indice){
 unsigned char flag,laco,ponto,hour,minute,second,day,month,year; 
 unsigned int crc;
  
 flag = EVSY_getAlarme(indice,&laco,&ponto,&hour,&minute,&second,&day,&month,&year);
 
 outputBuffer[0]  = 0x01;
 outputBuffer[1]  = GetAlarmEvent;
 outputBuffer[2]  = 16;

 outputBuffer[3]  = indice>>8;
 outputBuffer[4]  = (unsigned char)indice; 
 
 outputBuffer[5]  = flag;
 outputBuffer[6]  = laco;
 outputBuffer[7]  = ponto;
 outputBuffer[8]  = hour;
 outputBuffer[9]  = minute;
 outputBuffer[10] = second;
 outputBuffer[11] = day;
 outputBuffer[12] = month;
 outputBuffer[13] = year;
 
 crc = UTIL_calculaCRC16(outputBuffer,14);
 
 outputBuffer[14] = crc>>8;
 outputBuffer[15] = (unsigned char) crc;

 bytesToSend = 16; 
 counterOut = 0x00;  
 UART1_writePackage(outputBuffer,16);                                                  
}
/***********************************************************************************
*   Descrição       :   Lê um evento de defeito da fila do sistema
*   Parametros      :   (unsigned int) indice
*   Retorno         :   nenhum
***********************************************************************************/
void APUSNET_leEventoDefeito(unsigned int indice){
  unsigned int crc;
  eDefs descricao;
  unsigned char flag,pm1,pm2,hour,minute,second,day,month,year;  

  flag = EVSY_getDefeito(indice,&descricao,&pm1,&pm2,&hour,&minute,&second,&day,&month,&year);
  
  outputBuffer[0]  = 0x01;
  outputBuffer[1]  = GetFaultEvent;  
  outputBuffer[2]  = 13;
  
  outputBuffer[3]  = indice>>8;
  outputBuffer[4]  = (unsigned char)indice;
  
  outputBuffer[5]  = flag;
  outputBuffer[6]  = (unsigned char) descricao;
  outputBuffer[7]  = pm1;
  outputBuffer[8]  = pm2;
  outputBuffer[9]  = hour;
  outputBuffer[10] = minute;
  outputBuffer[11] = second;
  outputBuffer[12] = day;
  outputBuffer[13] = month;
  outputBuffer[14] = year;
  
  crc = UTIL_calculaCRC16(outputBuffer,15);
  outputBuffer[15] = (unsigned char)(crc>>8);
  outputBuffer[16] = (unsigned char) crc;
  
 bytesToSend = 17; 
 counterOut = 0x00;  
 UART1_writePackage(outputBuffer,17);      
}
/***********************************************************************************
*   Descrição       :   Lê um evento de bloqueio da fila do sistema
*   Parametros      :   (unsigned int) indice
*   Retorno         :   nenhum
***********************************************************************************/
void APUSNET_leEventoBloqueio(unsigned int indice){
  unsigned int crc;
  unsigned char flag,bloqueio,laco,hour,minute,second,day,month,year;

  
  flag = EVSY_getEventoBloqueio(indice,&bloqueio,&laco,&hour,&minute,&second,&day,&month,&year);
  
  outputBuffer[0] = 0x01;
  outputBuffer[1] = GetLockEvent;
  outputBuffer[2] = 14;
  
  outputBuffer[3] = indice>>8;
  outputBuffer[4] = (unsigned char) indice;
  
  outputBuffer[5] = flag;
  outputBuffer[6] = bloqueio;
  outputBuffer[7] = laco;
  outputBuffer[8] = hour;
  outputBuffer[9] = minute;
  outputBuffer[10]= second;
  outputBuffer[11]= day;
  outputBuffer[12]= month;
  outputBuffer[13]= year;
  
  crc = UTIL_calculaCRC16(outputBuffer,14);
  
  outputBuffer[14] = crc>>8;
  outputBuffer[15] = (unsigned char)crc;
  
  bytesToSend = 16; 
  counterOut = 0x00;  
  UART1_writePackage(outputBuffer,16);        
}
/***********************************************************************************
*   Descrição       :   Faz o ACK de um número X de eventos de alarme
*   Parametros      :   (unsigned int) número de eventos
*   Retorno         :   nenhum
***********************************************************************************/
void APUSNET_ackAlarmes(unsigned int numero){
  unsigned int eventos;
  
  eventos = EVSY_getNumberOfAlarms();
  if(numero>eventos)
    numero=eventos;
  
  EVSY_ackAlarms(numero);
  
  outputBuffer[0] = 0x01;
  outputBuffer[1] = AckAlarms;
  outputBuffer[2] = 7;
  
  outputBuffer[3] = (unsigned char)(numero>>8);
  outputBuffer[4] = (unsigned char)numero;
  eventos = UTIL_calculaCRC16(outputBuffer,5);
  
  outputBuffer[5] = (unsigned char) (eventos>>8);
  outputBuffer[6] = (unsigned char) eventos;

  bytesToSend = 7; 
  counterOut = 0x00;  
  UART1_writePackage(outputBuffer,7);        
}
/***********************************************************************************
*   Descrição       :   Faz o ack de um número X de eventos de defeito
*   Parametros      :   (unsigned int) número de eventos
*   Retorno         :   nenhum
***********************************************************************************/
void APUSNET_ackDefeitos(unsigned int numero){
  unsigned int eventos;
  
  eventos = EVSY_getNumberOfDefeitos();
  if(numero>eventos)
    numero=eventos;
  
  EVSY_ackDefeitos(numero);
  
  outputBuffer[0] = 0x01;
  outputBuffer[1] = AckFaults;
  outputBuffer[2] = 7; 
  
  outputBuffer[3] = (unsigned char)(numero>>8);
  outputBuffer[4] = (unsigned char)numero;
  eventos = UTIL_calculaCRC16(outputBuffer,5);
  
  outputBuffer[5] = (unsigned char) (eventos>>8);
  outputBuffer[6] = (unsigned char) eventos;  

  bytesToSend = 7; 
  counterOut = 0x00;  
  UART1_writePackage(outputBuffer,7);        
}
/***********************************************************************************
*   Descrição       :   Faz o ack de um número X de eventos de bloqueio
*   Parametros      :   (unsigned int) número de bloqueios
*   Retorno         :   nenhum
***********************************************************************************/
void APUSNET_ackBloqueios(unsigned int numero){
  unsigned int eventos;
  
  eventos = EVSY_getNumberOfBloqueios();
  if(numero>eventos)
    numero=eventos;
  
  EVSY_ackBloqueios(numero);
  
  outputBuffer[0] = 0x01;
  outputBuffer[1] = AckLocks;
  outputBuffer[2] = 7;
  
  outputBuffer[3] = (unsigned char)(numero>>8);
  outputBuffer[4] = (unsigned char)numero;
  eventos = UTIL_calculaCRC16(outputBuffer,5);
  
  outputBuffer[5] = (unsigned char)(eventos>>8);
  outputBuffer[6] = (unsigned char) eventos;  

  bytesToSend = 7; 
  counterOut = 0x00;  
  UART1_writePackage(outputBuffer,7);    
}
/***********************************************************************************
*   Descrição       :   Altera as configurações de uma das regra de relés
*   Parametros      :   (unsigned char) número da regra
*                       (unsigned char) alarmeGeral
*                       (unsigned char) pre-alarme
*                       (unsigned char) laco em alarme
*                       (unsigned char) ponto em alarme
*                       (unsigned char) laco bloqueado
*                       (unsigned char) laco com defeito
*                       (unsigned char) falta rede
*                       (unsigned char) falta bateria
*                       (unsigned char) laco parametro
*                       (unsigned char) ponto parametro
*   Retorno         :   nenhum
***********************************************************************************/
void APUSNET_configuraRele(unsigned char nRegra,unsigned char flagAlarmeGeral,unsigned char flagPreAlarme,
                           unsigned char flagLacoEmAlarme,unsigned char flagPontoEmAlarme,unsigned char flagLacoBloqueado,
                           unsigned char flagLacoDefeito,unsigned char flagFaltaRede,unsigned char flagFaltaBateria,
                           unsigned char lacoParametro,unsigned char pontoParametro,unsigned char pontoAuxiliar){
  unsigned int crc;  
  
  // Exceção
  if(nRegra>3 || lacoParametro>4 || pontoParametro>32){
    
    outputBuffer[0] = 0x01;
    outputBuffer[1] = 0x80 | SetRegraRele;
    outputBuffer[2] = nRegra;
    outputBuffer[3] = lacoParametro;
    outputBuffer[4] = pontoParametro;
    
    crc = UTIL_calculaCRC16(outputBuffer,5);
    outputBuffer[5] = (unsigned char) (crc>>8);
    outputBuffer[6] = (unsigned char) crc;
        
    bytesToSend = 7; 
    counterOut = 0x00;  
    UART1_writePackage(outputBuffer,7);         
  }
  else{
   
    (void)PARAMETROS_gravaConfigRele(nRegra,flagAlarmeGeral,flagPreAlarme,flagLacoEmAlarme,flagPontoEmAlarme,flagLacoBloqueado,flagLacoDefeito,flagFaltaRede,flagFaltaBateria,lacoParametro,pontoParametro,pontoAuxiliar);
  
    outputBuffer[0] = 0x01;
    outputBuffer[1] = SetRegraRele;
    outputBuffer[2] = 0x05;
    
    crc = UTIL_calculaCRC16(outputBuffer,3);
    outputBuffer[3] = (unsigned char)(crc>>8);
    outputBuffer[4] = (unsigned char) crc;
        
    bytesToSend = 5;
    counterOut = 0x00;   
    UART1_writePackage(outputBuffer,5);    
  }  
}
/***********************************************************************************
*   Descrição       :   Lê uma das configurações de regra de relés
*   Parametros      :   (unsigned char) número da regra
*   Retorno         :   nenhum
***********************************************************************************/
void APUSNET_carregaConfigRele(unsigned char numeroRegra){
  eReleRow  rele;
  unsigned int crc;
    
  if(numeroRegra>3){
  
    outputBuffer[0] = 0x01;
    outputBuffer[1] = 0x80 | GetRegraRele;
    outputBuffer[2] = numeroRegra;
    
    crc = UTIL_calculaCRC16(outputBuffer,3);
    outputBuffer[3] = (unsigned char)(crc>>8);
    outputBuffer[4] = (unsigned char)(crc);
    
    bytesToSend = 5; 
    counterOut = 0x00;  
    UART1_writePackage(outputBuffer,5);                  
  }
  else{
    
    outputBuffer[0] = 0x01;
    outputBuffer[1] = GetRegraRele;  
    outputBuffer[2] = 15;
    outputBuffer[3] = PARAMETROS_leConfigReles(numeroRegra,&rele);
    
    outputBuffer[3] = rele.bits.AlarmeGeral;
    outputBuffer[4] = rele.bits.PreAlarme;
    outputBuffer[5] = rele.bits.LacoEmAlarme;
    outputBuffer[6] = rele.bits.PontoEmAlarme;
    outputBuffer[7] = rele.bits.LacoBloqueado;
    outputBuffer[8] = rele.bits.LacoDefeito;
    outputBuffer[9] = rele.bits.FaltaFB;
    outputBuffer[10]= rele.bits.LacoCruzado;
    outputBuffer[11]= rele.laco;
    outputBuffer[12]= rele.ponto;
    outputBuffer[13]= rele.pontoAuxiliar;
    
    crc = UTIL_calculaCRC16(outputBuffer,14);
    outputBuffer[14]= (unsigned char)(crc>>8);
    outputBuffer[15]= (unsigned char)(crc);    
    
    bytesToSend = 16;
    counterOut = 0x00;    
    UART1_writePackage(outputBuffer,16);
  }  
}
/***********************************************************************************
*   Descrição       :   Abre a conexão da rede apus net
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void APUSNET_abreConexao(void){
  unsigned int crc;
  
  APUSNET_progConexaoCounter = 240;// Tempo máximo de uma conexão de programação sem 
                                   // renovacao;   
  outputBuffer[0] = 0x01;                                  
  outputBuffer[1] = AbreConexaoProgramacao;
  outputBuffer[2] = 3;
  
  crc = UTIL_calculaCRC16(outputBuffer,3);
  outputBuffer[3] = (unsigned char)(crc>>8);
  outputBuffer[4] = (unsigned char) crc;
  
  bytesToSend = 5;
  counterOut = 0x00;
  UART1_writePackage(outputBuffer,5);                                    
}
/***********************************************************************************
*   Descriçaão      :   Fecha a conexão da rede apus net
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void APUSNET_fechadConexao(void){
  unsigned int crc;
  
  APUSNET_progConexaoCounter = 0; // Tempo máximo de uma conexão de programação sem 
                                  // renovacao;   
  outputBuffer[0] = 0x01;                                  
  outputBuffer[1] = FechaConexaoProgramacao;
  outputBuffer[2] = 3;
  
  crc = UTIL_calculaCRC16(outputBuffer,3);
  outputBuffer[3] = (unsigned char)(crc>>8);
  outputBuffer[4] = (unsigned char) crc;
  
  bytesToSend = 5;
  counterOut = 0x00;
  UART1_writePackage(outputBuffer,5);      
}
/***********************************************************************************
*   Descrição      :    Configura o tempo para alarme geral
*   Parametros     :    (unsigned char) tempo em segundos para entrar em alarme
*   Retorno        :    nenhum
***********************************************************************************/
void APUSNET_cfgTempoAlarmeGeral(unsigned char tempo){
  unsigned int crc; 
        
  PARAMETROS_gravaTempoAlarmeGeral(tempo);
   
  outputBuffer[0] = 0x01;
  outputBuffer[1] = SetTempoAlarmeGeral;
  outputBuffer[2] = 0x03;
   
  crc = UTIL_calculaCRC16(outputBuffer,3);
  outputBuffer[3] = (unsigned char)(crc>>8);
  outputBuffer[4] = (unsigned char)crc;
   
  bytesToSend = 5;
  counterOut = 0x00;
  UART1_writePackage(outputBuffer,5);         
}
/***********************************************************************************
*   Descrição     :   Lê o tempo para entrar em alarme geral
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void APUSNET_leCfgTempoAlarmeGeral(void){
  unsigned int crc;
  
  outputBuffer[0] =  0x01;
  outputBuffer[1] =  GetTempoAlarmeGeral;
  outputBuffer[2] =  0x04;
  outputBuffer[3] =  PARAMETROS_tempoAlarmeGeral();
  
  crc = UTIL_calculaCRC16(outputBuffer,4);
  outputBuffer[4] = (unsigned char)(crc>>8);
  outputBuffer[5] = (unsigned char)crc;
  
  bytesToSend = 6;
  counterOut = 6;
  UART1_writePackage(outputBuffer,6);
}
/***********************************************************************************
*   Descrição     :   Envia um pacote pelo canal serial da rede apusnet
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void APUSNET_sendPackage(unsigned char *buffer,unsigned char tamanho){
 
  bytesToSend = tamanho-1;
  counterOut = 1;
  isc2_sendFirstByte(&buffer[0]);
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/