#ifndef _APUSNET_TARGET_WRAPPER_H_
#define _APUSNET_TARGET_WRAPPER_H_

#include <MC9S08AC32.H>
#include "HT1381.H"
#include "PARAMETROS.H"


#define SET_CLOCK(X,Y,Z)                HT1381_ajustaRelogio(X,Y,Z);
#define GET_CLOCK(X,Y,Z)                HT1381_leRelogio(X,Y,Z);

#define SET_CALENDAR(X,Y,Z)             HT1381_ajustaCalendario(X,Y,Z);
#define GET_CALENDAR(X,Y,Z)             HT1381_leCalendario(X,Y,Z);             

#define SET_NAME(LACO,PONTO,NOME)       PARAMETROS_gravaConfiguracaoNome(LACO,PONTO,NOME)
#define GET_NAME(LACO,PONTO,NOME)       PARAMETROS_leConfiguracaoNome(LACO,PONTO,NOME)
#define RESET_NAME(LACO,PONTO)          PARAMETROS_apagaConfiguracaoNome(LACO,PONTO)


#endif