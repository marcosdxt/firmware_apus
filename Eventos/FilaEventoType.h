/*__________________________________________________________________________________
| Chave Digital
|       Blumenau - SC
| __________________________________________________________________________________
|
|       This source code was developed by Chave Digital and cannot be copied, in part or
|       in whole, or used, except when legally licensed by Chave Digital or its distributors.
|
|       Este código é propriedade da Chave Digital e não pode ser copiado, em parte ou
|       em todo, ou utilizado, exceto quando for legalmente licenciado pela Chave Digital 
|       ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Product	    :        
|       Module	    : 
|       File	      :  FilaEventoTypes.h
|       Description :  Estruturas de uso coletivo pelos módulos que utilizam
|                      a fila
|       Created on  :  25/01/2010
|       Author      :  Marcos Aquino
|
|       History     :
|
|
| __________________________________________________________________________________
*/
#ifndef _FILA_EVENTO_TYPES_H
#define	_FILA_EVENTO_TYPES_H

// Wrapper section
#include "EEPROM.H"

#define BLOCK_READ(ENDERECO_INICIAL,PDATA,TAMANHO)           EEPROM_readBlock(ENDERECO_INICIAL,PDATA,TAMANHO)
#define BLOCK_WRITE(ENDERECO_INICIAL,PDATA,TAMANHO)          EEPROM_writeBlock(ENDERECO_INICIAL,PDATA,TAMANHO)


#include "util.h"

#define CRC_16(PDATA,TAMANHO)                                UTIL_calculaCRC16(PDATA,TAMANHO)


#endif //_FILA_EVENTO_TYPES_H