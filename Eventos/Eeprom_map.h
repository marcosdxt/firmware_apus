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
|       Este c�digo � propriedade da Chave Digital e n�o pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Chave Digital ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Arquivo        :
|       Data cria��o   :
|       Revis�es		   :
|
| __________________________________________________________________________________
*/

#ifndef _EEPROM_MAP_H_
#define _EEPROM_MAP_H_

//Bloco utilizado no teste da eeprom
#define ADR_INIT_BLOCO_TESTE                                            0x00
#define TAMANHO_BLOCO_TESTE                                             16
#define FIM_BLOCO_TESTE                                                 16//  ADR_INIT_BLOCO_TESTE+TAMANHO_BLOCO_TESTE

//Bloco com as configura��es do la�o 01 (17,18,19)..
#define ADR_CONFIG_LACO_01                                              17//  FIM_BLOCO_TESTE+1                                                 
#define ADR_CONFIG_LACO_01_CRC_HIGH                                     18//  ADR_CONFIG_LACO_01+1
#define ADR_CONFIG_LACO_01_CRC_LOW                                      19//  ADR_CONFIG_LACO_01_CRC_HIGH+1
//Bloco com as configura��es do la�o 02 (20,21,22)...
#define ADR_CONFIG_LACO_02                                              20//  ADR_CONFIG_LACO_01_CRC_LOW+1
#define ADR_CONFIG_LACO_02_CRC_HIGH                                     21//  ADR_CONFIG_LACO_02+1
#define ADR_CONFIG_LACO_02_CRC_LOW                                      22//  ADR_CONFIG_LACO_02_CRC_HIGH+1
//Bloco com as configura��es do la�o 03 (23,24,25)
#define ADR_CONFIG_LACO_03                                              23//  ADR_CONFIG_LACO_02_CRC_LOW+1
#define ADR_CONFIG_LACO_03_CRC_HIGH                                     24//  ADR_CONFIG_LACO_03+1
#define ADR_CONFIG_LACO_03_CRC_LOW                                      25//  ADR_CONFIG_LACO_03_CRC_HIGH+1
//Bloco com as configura��es do la�o 04 (26,27,28)
#define ADR_CONFIG_LACO_04                                              26//  ADR_CONFIG_LACO_03_CRC_LOW+1
#define ADR_CONFIG_LACO_04_CRC_HIGH                                     27//  ADR_CONFIG_LACO_04+1                                                                                                                                                                                                                                 
#define ADR_CONFIG_LACO_04_CRC_LOW                                      28//  ADR_CONFIG_LACO_04_CRC_HIGH+1                                                                        
//Configura��o dos pontos do la�o 01  (540)
#define ADR_INIT_CONFIG_PONTOS_LACO_01                                  29//  ADR_CONFIG_LACO_04_CRC_LOW+1
#define TAMANHO_BLOCO_CONFIG_PONTOS_LACO_01                             19
#define FIM_BLOCO_CONFIG_PONTOS_LACO_01                                 638// ADR_INIT_CONFIG_PONTOS_LACO_01*16*32
//Configura��o dos pontos do la�o 02  (1052)
#define ADR_INIT_CONFIG_PONTOS_LACO_02                                  640// FIM_BLOCO_CONFIG_PONTOS_LACO_01+1
#define TAMANHO_BLOCO_CONFIG_PONTOS_LACO_02                             19
#define FIM_BLOCO_CONFIG_PONTOS_LACO_02                                 1248// ADR_INIT_CONFIG_PONTOS_LACO_02*16*32
//Configura��o dos pontos do la�o 03 (1592)
#define ADR_INIT_CONFIG_PONTOS_LACO_03                                  1250// FIM_BLOCO_CONFIG_PONTOS_LACO_02+1
#define TAMANHO_BLOCO_CONFIG_PONTOS_LACO_03                             19
#define FIM_BLOCO_CONFIG_PONTOS_LACO_03                                 1858// ADR_INIT_CONFIG_PONTOS_LACO_03*16*32
//Configura��o dos pontos do la�o 04 (2132)
#define ADR_INIT_CONFIG_PONTOS_LACO_04                                  1860// FIM_BLOCO_CONFIG_PONTOS_LACO_03+1
#define TAMANHO_BLOCO_CONFIG_PONTOS_LACO_04                             19
#define FIM_BLOCO_CONFIG_PONTOS_LACO_04                                 2468// ADR_INIT_CONFIG_PONTOS_LACO_04*16*32
//Configura��o do m�dulo de expans�o das sirenes (2134
#define ADR_PARAMETRO_EXPANSAO_SIRENE                                   2470// FIM_BLOCO_CONFIG_PONTOS_LACO_04
#define ADR_CONTRA_PARAMETRO_EXPANSAO_SIRENE                            2471// ADR_PARAMETRO_EXPANSAO_SIRENE+1                                                               
//Configura��o do m�dulo de rel�s 01  (2136)
#define ADR_ROW_01_CONFIG_RELES_INIT                                    2472// ADR_CONTRA_PARAMETRO_EXPANSAO_SIRENE+1 
#define ADR_ROW_01_SIZE                                                 6
#define ADR_ROW_01_CONFIG_RELES_END                                     2478// ADR_ROW_01_CONFIG_RELES_INIT+ADR_ROW_01_SIZE
//Configura��o do m�dulo de rel�s 02   (2141)
#define ADR_ROW_02_CONFIG_RELES_INIT                                    2480// ADR_ROW_01_CONFIG_RELES_END+1
#define ADR_ROW_02_SIZE                                                 6
#define ADR_ROW_02_CONFIG_RELES_END                                     2486// ADR_ROW_02_CONFIG_RELES_INIT+ADR_ROW_02_SIZE
//Configura��o do m�dulo de rel� 03 (2146)
#define ADR_ROW_03_CONFIG_RELES_INIT                                    2494// ADR_ROW_02_CONFIG_RELES_END+1
#define ADR_ROW_03_SIZE                                                 6
#define ADR_ROW_03_CONFIG_RELES_END                                     2500// ADR_ROW_03_CONFIG_RELES_INIT+ADR_ROW_03_SIZE
//Configura��o do m�dulo de rel�s 04 (2151)
#define ADR_ROW_04_CONFIG_RELES_INIT                                    2502// ADR_ROW_03_CONFIG_RELES_END+1
#define ADR_ROW_04_SIZE                                                 6
#define ADR_ROW_04_CONFIG_RELES_END                                     2508// ADR_ROW_04_CONFIG_RELES_INIT+ADR_ROW_04_SIZE                                     
//Endere�o na rede ApusNET (2170)
#define ADR_ENDERECO_APUS_NET                                           2520// ADR_ROW_04_CONFIG_RELES_END+1
#define ADR_ENDERECO_CONTRA_PARENTE_APUS_NET                            2521// ADR_ENDERECO_APUS_NET+1
//Sele��o do protocolo para o canal serial de expans�o  (2180)
#define ADR_CONFIG_MODULO_COMM                                          2530// ADR_ENDERECO_CONTRA_PARENTE_APUS_NET+1
#define ADR_CONFIG_CONTRAPARENTE_MODULO_COMM                            2531// ADR_CONFIG_MODULO_COMM+1                                                    
//Configura��o do idioma da placa (2190)
#define ADR_CONFIG_IDIOMA                                               2532// ADR_CONFIG_CONTRAPARENTE_MODULO_COMM+1
#define ADR_CONFIG_IDIOMA_CONTRAPARENTE                                 2533// ADR_CONFIG_IDIOMA+1                                 

#define ADR_CONFIG_TEMPO_ALARME_GERAL                                   2534// ADR_CONFIG_IDIOMA_CONTRAPARENTE+1
#define ADR_CONFIG_TEMPO_ALARME_GERAL_CONTRAPARENTE                     2535// ADR_CONFIG_TEMPO_ALARME_GERAL+1
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
#define ADR_FLAG_BLOQUEIO_LACO_01                                       2536 // ADR_CONFIG_TEMPO_ALARME_GERAL_CONTRAPARENTE+10
#define ADR_FLAG_BLOQUEIO_LACO_01_CONTRAPARENTE                         2537 // ADR_FLAG_BLOQUEIO_LACO_01+1
                                                                                
#define ADR_FLAG_BLOQUEIO_LACO_02                                       2538 // ADR_FLAG_BLOQUEIO_LACO_01+10
#define ADR_FLAG_BLOQUEIO_LACO_02_CONTRAPARENTE                         2539 // ADR_FLAG_BLOQUEIO_LACO_02+1
                                                                                
#define ADR_FLAG_BLOQUEIO_LACO_03                                       2540 // ADR_FLAG_BLOQUEIO_LACO_02+10
#define ADR_FLAG_BLOQUEIO_LACO_03_CONTRAPARENTE                         2541 // ADR_FLAG_BLOQUEIO_LACO_03+1

#define ADR_FLAG_BLOQUEIO_LACO_04                                       2542 // ADR_FLAG_BLOQUEIO_LACO_03+10
#define ADR_FLAG_BLOQUEIO_LACO_04_CONTRAPARENTE                         2543 // ADR_FLAG_BLOQUEIO_LACO_04+1                                                                                                    
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
#define ADR_INICIO_BANCO_LOG_ALARMES                                    2550 // ADR_FLAG_BLOQUEIO_LACO_04+10
#define ADR_INICIO_BLOCOS_ALARME                                        2575 // ADR_INICIO_BANCO_LOG_ALARMES+25
#define CAPACIDADE_BLOCOS_ALARME                                        2500 // 2500
#define TAMANHO_BLOCO_ALARME                                            10   // 10
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
#define ADR_INICIO_BANCO_LOG_DEFEITOS                                   28000// ADR_INICIO_BLOCOS_ALARME+(CAPACIDADE_BLOCOS_ALARME*TAMANHO_BLOCO_ALARME)
#define ADR_INICIO_BLOCO_DEFEITOS                                       28025// ADR_INICIO_BANCO_LOG_DEFEITOS+25
#define CAPACIDADE_BLOCOS_DEFEITO                                       300  // 300
#define TAMANHO_BLOCO_DEFEITO                                           12   // 12
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
#define ADR_INICIO_BANCO_BLOQUEIOS                                      31625// ADR_INICIO_BLOCO_DEFEITOS+(CAPACIDADE_BLOCOS_DEFEITO*TAMANHO_BLOCO_DEFEITO)
#define ADR_INICIO_BLOCO_BLOQUEIOS                                      31650// ADR_INICIO_BANCO_BLOQUEIOS+25
#define CAPACIDADE_BLOCOS_BLOQUEIOS                                     200
#define TAMANHO_BLOCO_BLOQUEIOS                                         10                                      
                                  
                                                                                                                                                                                                                                                                                              
#endif