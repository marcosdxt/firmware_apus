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

#ifndef _APUS_NET_H_
#define	_APUS_NET_H_

/*
  Formatado das mensagens
  
  -- Request/Reply --
  
  [Endereço] [Código função] [Tamanho pacote] [Parametros] [Dados] [CRC]
  
  [Endereço]         : Número de 8 bits que representa o endereço lógico da central na rede
  [Código da função] : Código de 8 bits das funções da rede apus
  [Tamanho Pacote]   : Número de bytes dentro do pacote, contendo todo o bloco
  [Parametros]       : Parametros para execução da função pode variar de 0 à 64 bytes
  [Dados]            : Dados resultantes do processamento de função, pode variar de 0 à 64 bytes
  [CRC]              : Ciclical redundance check
  
  
    
*/

#define APUS_CMD_SETA_ALARME_GERAL                0xA1
#define APUS_CMD_RESET_SISTEMA                    0xA2
#define APUS_CMD_DESLIGA_SIRENES                  0xA3  
#define APUS_CMD_DESLIGA_BIP                      0xA4

// Comandos da rede ApusNet
typedef enum{ 
 ReadStatus=0xa,
 WriteStatus, 
 
 SetClock,
 GetClock, 
 
 SetName,
 GetName,  
 ClrName,
 
 NumberOfNewAlarms,
 NumberOfNewFaults,
 NumberOfNewLocks,
 
 GetAlarmEvent,
 GetFaultEvent,
 GetLockEvent,
 
 AckAlarms,
 AckFaults,
 AckLocks,
 
 SetRegraRele,
 GetRegraRele,
 ClrRegraRele,
 
 BlockWrite,
 BlockRead, 
 
 EepromWrite,
 EepromRead,
 
 AbreConexaoProgramacao,
 FechaConexaoProgramacao, 

 SetTempoAlarmeGeral,
 GetTempoAlarmeGeral,
 
 SetAlarmeGeral,
 ResetAlarmeGeral,
 DesligaSirenes,
 DesligaBipInterno,
 TempoParaAlarme
 
}eCommands;


extern unsigned char APUSNET_status;

#define SETA_FLAG_ALARME()                        APUSNET_status |= 0x01
#define CLR_FLAG_ALARME()                         APUSNET_status &= ~0x01

#define SETA_FLAG_DEFEITO()                       APUSNET_status |= 0x02
#define CLR_FLAG_DEFEITO()                        APUSNET_status &= ~0x02

#define SETA_FLAG_BLOQUEIOS()                     APUSNET_status |= 0x04
#define CLR_FLAG_BLOQUEIOS()                      APUSNET_status &= ~0x04                

#define SETA_FLAG_REDE_ELETRICA()                 APUSNET_status |= 0x08
#define CLR_FLAG_REDE_ELETRICA()                  APUSNET_status &= ~0x08

#define SETA_FLAG_BATERIA()                       APUSNET_status |= 0x10
#define CLR_FLAG_BATERIA()                        APUSNET_status &= ~0x10

#define SETA_FLAG_FALHA_PENDENTE()                APUSNET_status |= 0x20
#define CLR_FLAG_FALHA_PENDENTE()                 APUSNET_status &= ~0x20

#define SETA_FLAG_BLOQUEIO_PENDENTE()             APUSNET_status |= 0x40
#define CLR_FLAG_BLOQUEIO_PENDENTE()              APUSNET_status &= ~0x40

#define RESET_ALL_STATUS_FLAGS()                  APUSNET_status = 0x00    

void APUSNET_init(void);
void APUSNET_decodePackages(void);


#endif