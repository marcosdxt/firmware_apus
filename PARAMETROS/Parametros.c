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
|       Arquivo            :  Parametros.c
|       Descrição          :  Funções para salvar e ler os parametros na eeprom
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  13/07/2010
|
|       Revisões           :  1.0.0
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*		Includes
***********************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "eeprom.h"
#include "Eeprom_Map.h"
#include "Parametros.h"
#include "util.h"
/***********************************************************************************
*   Definições
***********************************************************************************/
#define LACO_HABILITADO                                       0xA5

/***********************************************************************************
*   Estruturas
***********************************************************************************/
typedef struct{
  unsigned char habilitado;  
  unsigned char CRC[2];  
}sLacoSetup;

typedef struct{
  unsigned char habilitado;
  char nome[16];  
  unsigned char CRC[2];
}sPontoSetup;
/***********************************************************************************
*		Funções locais
***********************************************************************************/
   
/***********************************************************************************
*		Implementação
***********************************************************************************/

/***********************************************************************************
*   Descrição     :   Habilita/Desabilita os laços da central Apus
*   Parametros    :   (unsigned char) Número do laço
*                     (unsigned char) Maior do que zero habilita e menor do que
*                                     zero desabilita
*   Retorno       :   nenhum
***********************************************************************************/
void PARAMETROS_configLaco(unsigned char laco,unsigned char bit){  
  sLacoSetup tmp;
  unsigned int crc;
    
  if(bit)
    tmp.habilitado = LACO_HABILITADO;
  else
    tmp.habilitado = 0x00;
  
  crc = UTIL_calculaCRC16((unsigned char*)&tmp,1);
  tmp.CRC[0] = crc>>8;
  tmp.CRC[1] = (unsigned char)crc;
  
  switch(laco){
    case 0: EEPROM_writeBlock(ADR_CONFIG_LACO_01,(unsigned char*)&tmp,3); 
            break;
    case 1: EEPROM_writeBlock(ADR_CONFIG_LACO_02,(unsigned char*)&tmp,3);
            break;    
    case 2: EEPROM_writeBlock(ADR_CONFIG_LACO_03,(unsigned char*)&tmp,3);
            break;    
    case 3: EEPROM_writeBlock(ADR_CONFIG_LACO_04,(unsigned char*)&tmp,3);   
            break;    
  }    
}
/***********************************************************************************
*   Descrição   :   Carrega as configurações dos laços do sistema
*   Parametros  :   (unsigned char) número do laço
*   Retorno     :   (unsigned char) Maior do que zero se o laço estiver habilitado
***********************************************************************************/
unsigned char PARAMETROS_carregaConfigLaco(unsigned char laco){
  sLacoSetup tmp;
  unsigned int crc,calc;
  
  switch(laco){
    case 0: EEPROM_readBlock(ADR_CONFIG_LACO_01,(unsigned char*)&tmp,3);
            break;
    case 1: EEPROM_readBlock(ADR_CONFIG_LACO_02,(unsigned char*)&tmp,3);
            break;    
    case 2: EEPROM_readBlock(ADR_CONFIG_LACO_03,(unsigned char*)&tmp,3);
            break;    
    case 3: EEPROM_readBlock(ADR_CONFIG_LACO_04,(unsigned char*)&tmp,3);   
            break;    
  }
  
  crc = UTIL_calculaCRC16((unsigned char*)&tmp,1);
  calc = (unsigned int)(tmp.CRC[0]<<8 | tmp.CRC[1]);
  
  if(crc == calc && tmp.habilitado == LACO_HABILITADO)
    return 0xFF; 
 
  return 0x00;  
}
/***********************************************************************************
*   Descrição     :   Nomeia um dos pontos
*   Parametros    :   (unsigned char) número do laço
*                     (unsigned char) número do ponto
*                     (char*) Ponteiro para o nome
*   Retorno       :
***********************************************************************************/
unsigned char PARAMETROS_gravaConfiguracaoNome(unsigned char laco,unsigned char ponto,char* nome){
  sPontoSetup point,lido;
  unsigned char i;
  unsigned int crc;
   
  point.habilitado = LACO_HABILITADO;
  for(i=0;i<16;i++)
    point.nome[i] = ' ';
    
  i=0;
  while(*nome && i<16){
    point.nome[i] = *nome;
    nome++; 
    i++;
  }
  
  crc = UTIL_calculaCRC16((unsigned char*)&point,17); 
  point.CRC[0] = (unsigned char)(crc>>8); 
  point.CRC[1] = (unsigned char)crc;
  
  if(laco==3 && (ponto>=12 && ponto<=14))
    __asm NOP;
  
  (void)EEPROM_writeBlock(ADR_INIT_CONFIG_PONTOS_LACO_01 +(laco*32*19) + (ponto*19),(unsigned char*)&point,19);
  (void)EEPROM_readBlock (ADR_INIT_CONFIG_PONTOS_LACO_01 +(laco*32*19) + (ponto*19),(unsigned char*)&lido,19);  
    
  if(point.habilitado == lido.habilitado && point.CRC[0] == lido.CRC[0] && point.CRC[1] == lido.CRC[1])     
     return 0xFF;  
    
  return 0x00;
}
/***********************************************************************************
*   Descrição     :   Apaga a configuração de um ponto
*   Parametros    :   (unsigned char) número do laço 
*                     (unsigned char) número do ponto
*   Retorno       :   (unsigned char) maior do que zero se conseguir alterar
***********************************************************************************/
unsigned char PARAMETROS_apagaConfiguracaoNome(unsigned char laco,unsigned char ponto){
  sPontoSetup point,lido;
  unsigned char i;
  unsigned int crc;
   
  point.habilitado = 0x00;
  for(i=0;i<16;i++)
    point.nome[i] = ' ';
      
  crc = UTIL_calculaCRC16((unsigned char*)&point,17); 
  point.CRC[0] = (unsigned char)(crc>>8); 
  point.CRC[1] = (unsigned char)crc;
  
  (void)EEPROM_writeBlock(ADR_INIT_CONFIG_PONTOS_LACO_01 +(laco*32*19) + (ponto*19),(unsigned char*)&point,19);
  (void)EEPROM_readBlock (ADR_INIT_CONFIG_PONTOS_LACO_01 +(laco*32*19) + (ponto*19),(unsigned char*)&lido,19);  
    
  if(point.habilitado == lido.habilitado && point.CRC[0] == lido.CRC[0] && point.CRC[1] == lido.CRC[1])     
     return 0xFF;  
    
  return 0x00;    
}
/***********************************************************************************
*   Descrição     :   Lê a configuração de nome de ponto
*   Parametros    :   (unsigned char) Número do laço
*                     (unsigned char) Número do ponto
*                     (char*) Ponteiro para o nome
*   Retorno       :   (unsigned char) Maior do que zero se for um nome válido
***********************************************************************************/
unsigned char PARAMETROS_leConfiguracaoNome(unsigned char laco,unsigned char ponto,char *nome){
  sPontoSetup point;
  unsigned int crc; 
      
  (void)EEPROM_readBlock ( ADR_INIT_CONFIG_PONTOS_LACO_01+(laco*32*19) + (ponto*19),(unsigned char*)&point,19);    
  crc = UTIL_calculaCRC16((unsigned char*)&point,17); 
  (void)memcpy(nome,point.nome,16);  
  
  return crc == (point.CRC[0]<<8 | point.CRC[1]) && point.habilitado == LACO_HABILITADO;    
}
/***********************************************************************************
*   Descrição     :   Grava a configuração de habilitação do módulo de 
*                     expansão de sirenes
*   Parametros    :   (unsigned char) maior do que zero habilita o módulo
*   Retorno       :   (unsigned char) maior do que zero se conseguiu alterar os
*                     parametros
***********************************************************************************/
unsigned char PARAMETROS_gravarConfigExpansaoSirenes(unsigned char habilita){
  unsigned char buffer[2]={0,0},lido[2];  
  
  if(habilita){    
    buffer[0] = 0xB1;
    buffer[1] = 0x1B;
  }
  
  (void)EEPROM_writeBlock(ADR_PARAMETRO_EXPANSAO_SIRENE,buffer,2);
  (void)EEPROM_readBlock (ADR_PARAMETRO_EXPANSAO_SIRENE,lido,2);
    
  return buffer[0] == lido[0] && buffer[1] == lido[1];    
}
/***********************************************************************************
*   Descrição     :   Lê as configurações de habilitação do módulo de 
*                     expansão de sirenes
*   Parametros    :   nenhum
*   Retorno       :   (unsigned char) Maior do que zero se o módulo de sirenes
*                     auxiliar estiver instalado
***********************************************************************************/
unsigned char PARAMETROS_leConfigExpansaoSirenes(void){
  unsigned char buffer[2];
  
  (void)EEPROM_readBlock(ADR_PARAMETRO_EXPANSAO_SIRENE,buffer,2);  
  
  return buffer[0] == 0xB1 && buffer[1] == 0x1B;  
}
/***********************************************************************************
*   Descrição     :   Salva uma configuração de reles na tabela 
*   Parametros    :   (unsigned char) alarme Geral;
*                     (unsigned char) Laço em alarme
*                     (unsigned char) Ponto em alarme
*                     (unsigned char) laco bloqueado
*                     (unsigned char) laco com defeito
*                     (unsigned char) falta rede
*                     (unsigned char) falta bateria
*                     (unsigned char) número do laço de trigger
*                     (unsigned char) número do ponto de trigger
*   Retorno       :   (unsigned char) Maior do que zero conseguir gravar os parametros
***********************************************************************************/
unsigned char PARAMETROS_gravaConfigRele(unsigned char moduloReles,
                                         unsigned char alarmeGeral,unsigned char preAlarme,
                                         unsigned char lacoEmAlarme,unsigned char pontoEmAlarme,
                                         unsigned char lacoBloqueado,unsigned char lacoDefeito,
                                         unsigned char faltaRede,unsigned char faltaBateria,
                                         unsigned char lacoP,unsigned char pontoP,unsigned char pontoAuxiliar){
  eReleRow row,lido;
  
  row.bits.AlarmeGeral = alarmeGeral;
  row.bits.PreAlarme = preAlarme;
  row.bits.LacoEmAlarme = lacoEmAlarme;
  row.bits.PontoEmAlarme = pontoEmAlarme;
  row.bits.LacoBloqueado = lacoBloqueado;
  row.bits.LacoDefeito = lacoDefeito;
  row.bits.FaltaFB = faltaRede;
  row.bits.LacoCruzado = faltaBateria;
  row.laco = lacoP;
  row.ponto = pontoP;
  row.pontoAuxiliar = pontoAuxiliar;
  row.CRC =  UTIL_calculaCRC16((unsigned char*)&row,4);
  
  switch(moduloReles){
    case 0: (void)EEPROM_writeBlock(ADR_ROW_01_CONFIG_RELES_INIT,(unsigned char*)&row ,6);
            (void)EEPROM_readBlock (ADR_ROW_01_CONFIG_RELES_INIT, (unsigned char*)&lido,6);    
            break;
    case 1: (void)EEPROM_writeBlock(ADR_ROW_02_CONFIG_RELES_INIT,(unsigned char*)&row ,6);
            (void)EEPROM_readBlock (ADR_ROW_02_CONFIG_RELES_INIT, (unsigned char*)&lido,6);    
            break;
    case 2: (void)EEPROM_writeBlock(ADR_ROW_03_CONFIG_RELES_INIT,(unsigned char*)&row ,6);
            (void)EEPROM_readBlock (ADR_ROW_03_CONFIG_RELES_INIT, (unsigned char*)&lido,6);    
            break;
    case 3: (void)EEPROM_writeBlock(ADR_ROW_04_CONFIG_RELES_INIT,(unsigned char*)&row ,6);
            (void)EEPROM_readBlock (ADR_ROW_04_CONFIG_RELES_INIT, (unsigned char*)&lido,6);    
            break;                        
  }
     
  return row.laco  == lido.laco && 
         row.ponto == lido.ponto &&
         *(unsigned char*)&row.bits == *(unsigned char*)&lido.bits;   
}
/***********************************************************************************
*   Descrição         :   Lê uma configuração de relés na tabela
*   Parametros        :   (unsigned char) Número do módulo de relés
*                         (eReleRow*) estrutura com os dados do relé
*   Retorno           :   (unsigned char) maior do que zero se CRC dos 
*                         parametros estiver correto
***********************************************************************************/
unsigned char PARAMETROS_leConfigReles(unsigned char moduloReles,eReleRow* rele){
  unsigned int crc;  
    
  switch(moduloReles){
    case 0: (void)EEPROM_readBlock(ADR_ROW_01_CONFIG_RELES_INIT, (unsigned char*)rele,6);    
            break;
    case 1: (void)EEPROM_readBlock(ADR_ROW_02_CONFIG_RELES_INIT, (unsigned char*)rele,6);    
            break;
    case 2: (void)EEPROM_readBlock(ADR_ROW_03_CONFIG_RELES_INIT, (unsigned char*)rele,6);    
            break;
    case 3: (void)EEPROM_readBlock(ADR_ROW_04_CONFIG_RELES_INIT, (unsigned char*)rele,6);
            break;                        
  }    
  
  crc = UTIL_calculaCRC16((unsigned char*)rele,4);
  
  return crc == rele->CRC;  
}
/***********************************************************************************
*   Descrição       :   Escreve a configuração do módulo de comunicação
*                       instalado no sistema
*   Parametros      :   (eModCommType) módulo
*   Retorno         :   nenhum
***********************************************************************************/
unsigned char PARAMETROS_gravaConfigModuloComm(eModCommType modulo){
  unsigned char escritos[2],lidos[2];
  
  escritos[0] = (unsigned char) modulo;
  escritos[1] = escritos[0] ^ 0xFF;
  
  (void)EEPROM_writeBlock(ADR_CONFIG_MODULO_COMM,escritos,2);
  (void)EEPROM_readBlock (ADR_CONFIG_MODULO_COMM,lidos   ,2);
  
  return escritos[0] == lidos[0] &&
         escritos[1] == lidos[1];    
}
/***********************************************************************************
*   Descrição     :   Lê a confguração do módulo de comunicação instalado
*                     no sistema
*   Parametros    :   nenhum
*   Retorno       :   (eModCommType) Módulo instaldo no sistema, default é printer
***********************************************************************************/
eModCommType PARAMETROS_leConfigModuloComm(void){
  unsigned char lidos[2];
  
  (void)EEPROM_readBlock(ADR_CONFIG_MODULO_COMM,lidos,2);
  
  lidos[1] ^= 0xFF;
  if(lidos[0] == lidos[1])
    return lidos[0];   
  
  return Impressora;  
}
/***********************************************************************************
*   Descrição     :   Grava o parametro tempo para alarme geral
*   Parametros    :   (unsigned int) tempo em segundos para entrar em 
*                     alarme geral
*   Retorno       :   nenhum
***********************************************************************************/
void PARAMETROS_gravaTempoAlarmeGeral(unsigned char tempo){
  unsigned char buffer[2];
  
  buffer[0] = tempo;
  buffer[1] = ~tempo;
  
  (void)EEPROM_writeBlock(ADR_CONFIG_TEMPO_ALARME_GERAL,buffer,2);            
}
/***********************************************************************************
*   Descrição     :   Lê o valor do parametro do tempo para entrar em alarme
*                     geral
*   Parametros    :   (unsigned int) tempo para entrar em alarme geral
*   Retorno       :   nenhum
***********************************************************************************/
unsigned char PARAMETROS_tempoAlarmeGeral(void){
  unsigned char buffer[2];
  
  (void)EEPROM_readBlock(ADR_CONFIG_TEMPO_ALARME_GERAL,buffer,2);            
  
  buffer[1]=~buffer[1];
  
  if(buffer[0]==buffer[1]){
   
    if(buffer[0]>240)
      buffer[0] = 0x00;
    
      return buffer[0];    
  }     

  return 15;  
}
/***********************************************************************************
*   Descrição     :   Bloqueia um dos laços do sistema
*   Parametros    :   (unsigned char) Número do laço
*                     (unsigned char) valor, 1 bloqueado,0 desbloqueado
*   Retorno       :   nenhum
***********************************************************************************/
unsigned char PARAMETROS_gravaBloqueio(unsigned char laco,unsigned char valor){
  unsigned char buffer[2],lido[2];
  
  buffer[0] = valor;
  buffer[1] = ~buffer[0];
  
  switch(laco){
    case 0: (void)EEPROM_writeBlock(ADR_FLAG_BLOQUEIO_LACO_01,buffer,2); 
            (void)EEPROM_readBlock (ADR_FLAG_BLOQUEIO_LACO_01,lido  ,2);
            break;
    case 1: (void)EEPROM_writeBlock(ADR_FLAG_BLOQUEIO_LACO_02,buffer,2); 
            (void)EEPROM_readBlock (ADR_FLAG_BLOQUEIO_LACO_02,lido  ,2);    
            break;
    case 2: (void)EEPROM_writeBlock(ADR_FLAG_BLOQUEIO_LACO_03,buffer,2); 
            (void)EEPROM_readBlock (ADR_FLAG_BLOQUEIO_LACO_03,lido  ,2);    
            break;
    case 3: (void)EEPROM_writeBlock(ADR_FLAG_BLOQUEIO_LACO_04,buffer,2); 
            (void)EEPROM_readBlock (ADR_FLAG_BLOQUEIO_LACO_04,lido  ,2);    
            break;
  }      
 
  return lido[0] == buffer[0] &&
         lido[1] == buffer[1];
}
/***********************************************************************************
*   Descrição     :   Lê as configurações de bloqueio de um dos laços
*   Parametros    :   (unsigned char) Número do laço
*   Retorno       :   (unsigned char) Maior do que zero é desbloqueado
***********************************************************************************/
unsigned char PARAMETROS_leBloqueio(unsigned char laco){
  unsigned char buffer[2];
  
  switch(laco){
    case 0: (void)EEPROM_readBlock(ADR_FLAG_BLOQUEIO_LACO_01,buffer,2); break;
    case 1: (void)EEPROM_readBlock(ADR_FLAG_BLOQUEIO_LACO_02,buffer,2); break;
    case 2: (void)EEPROM_readBlock(ADR_FLAG_BLOQUEIO_LACO_03,buffer,2); break;
    case 3: (void)EEPROM_readBlock(ADR_FLAG_BLOQUEIO_LACO_04,buffer,2); break;
  }
    
  buffer[1] = ~buffer[1];
  
  if(buffer[0] == buffer[1])
    return buffer[0];  

  return 0x00;  
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/