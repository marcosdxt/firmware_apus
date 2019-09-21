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
|       Módulo de software :  Gerenciador de teclado
|       Arquivo            :  Teclado.c
|       Descrição          :  Implementação de biblioteca que implementa um
|                             teclado com repetição de teclas, filtro e 
|                             auto-incremento 
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  07/05/2010
|
|       Revisões           :  1.0
|
|
| __________________________________________________________________________________
*/

#define _MEMBRANA_
/***********************************************************************************
*		Includes
***********************************************************************************/
#include "Teclado_targetWrapper.h"
#include "Teclado.h"

/***********************************************************************************
*   Definições para configuração dos buffers
***********************************************************************************/
#define TAMANHO_BUFFER_AMOSTRA                          10
#define RELOAD_TEMPO_BEEP                               100
#define FREQUENCIA_BEEP                                 800
/***********************************************************************************
*		Variaveis locais
***********************************************************************************/
unsigned char mediaAmostragem[2]   = {0,0},
              ultimaAmostra  [2]   = {0,0},
              ultimoInserido=0,
              ultimoLido=0,
              teclasNaFila=0,
              setBeep=0,
              semaforoTeclado=0;
              
#ifdef _MEMBRANA_              
                    
const              
eTeclas retornoTeclado[2][8] = { 
                                 {
                                   TeclaDesligaSirenes,
                                   TeclaSilenciarBipInterno,
                                   TeclaBloqueios,
                                   TeclaCancelar,
                                   TeclaLeft,
                                   TeclaUP,
                                   TeclaRigth,
                                   TeclaConfirma,
                                 },
                                 {
                                   TeclaAlarmeGeral,
                                   TeclaReinicarSistema,
                                   TeclaParametros,
                                   TeclaDown,
                                   SemTecla,
                                   SemTecla,
                                   SemTecla,
                                   SemTecla
                                 }
                               };    
#else
const              
eTeclas retornoTeclado[2][8] = { 
                                 {
                                   TeclaDesligaSirenes,
                                   TeclaSilenciarBipInterno,
                                   TeclaParametros,
                                   TeclaBloqueios,
                                   TeclaAlarmeGeral,
                                   TeclaReinicarSistema,
                                   TeclaUP,
                                   TeclaDown
                                 },
                                 {
                                   TeclaLeft,
                                   TeclaRigth,
                                   TeclaConfirma,
                                   TeclaCancelar,
                                   SemTecla,
                                   SemTecla,
                                   SemTecla,
                                   SemTecla
                                 }
                               };  
#endif                               
                               
extern unsigned char bufferTeclado[2][TAMANHO_BUFFER_AMOSTRA];

unsigned char filaTeclas[10],
              ultimaInserida=0,
              ultimaLida=0,
              tamanhoFila=0;     
extern unsigned int MENU_GENERICO_timeOut;                                                     
/***********************************************************************************
*		Implementação
***********************************************************************************/

/***********************************************************************************
*     Descrição     :   Remove as teclas do buffer circular de teclas pressionadas
*     Parametros    :   nenhum
*     Retorno       :   (eTeclas)
***********************************************************************************/
eTeclas TECLADO_getTecla2(unsigned char *borda){
  unsigned char i,j,counter;
  static unsigned char teclaLivre=1;
  static eTeclas last=(eTeclas)0x00;
  
  for(i=0;i<8;i++){
    counter = 0x00;   
    for(j=0;j<TAMANHO_BUFFER_AMOSTRA;j++)
      if(bufferTeclado[0][j] & (0x01<<i) )
        counter++;
      
    if(counter>4){
      if(!teclaLivre)
        return (eTeclas)0x00;
      teclaLivre = 0x00;  
      *borda = 0x00;
      last = retornoTeclado[0][i];                  
      setBeep = RELOAD_TEMPO_BEEP;
      SET_SOUND(FREQUENCIA_BEEP);
      return last;
    }
  }
    
  for(i=0;i<4;i++){
    counter = 0x00;   
    for(j=0;j<TAMANHO_BUFFER_AMOSTRA;j++)
      if(bufferTeclado[1][j]&(0x01<<i))
        counter++;
      
    if(counter>4){
      if(!teclaLivre)
        return (eTeclas)0x00;
      teclaLivre = 0x00; 
      last = retornoTeclado[1][i]; 
      *borda = 0x00;                
      setBeep = RELOAD_TEMPO_BEEP;
      SET_SOUND(FREQUENCIA_BEEP);        
      return last;
    } 
  }  
  
  if(!teclaLivre) {
    teclaLivre = 1;
    *borda = 0x01;  
    return last;
  }
  
  last = (eTeclas)0x00;
  return last;      
}
/***********************************************************************************
*   Descrição     :   Faz a leitura do teclado e vai jogando as teclas pressiondas
*                     pelo usuário em uma fila de teclas
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void TECLADO_entraFilaTeclado(void){
  eTeclas tecla;
  unsigned char borda;
  
  if(semaforoTeclado)
    return;
  
  semaforoTeclado = 1;
  
  tecla = TECLADO_getTecla2(&borda);
  if(tecla && !borda){
    if(tamanhoFila>10){
      semaforoTeclado = 0x00; 
      return;
    }
    
    filaTeclas[ultimaInserida] = (unsigned char)tecla;
    ultimaInserida = (unsigned char)((ultimaInserida+1)%10);
    tamanhoFila++;      
  }
 
 semaforoTeclado = 0x00;   
}
/***********************************************************************************
*   Descrição     :   Lê as teclas da fila
*   Parametros    :   (unsigned char*) borda
*   Retorno       :   (eTeclas) tecla
***********************************************************************************/
eTeclas TECLADO_getTecla(unsigned char *borda){
  eTeclas retorno; 
  *borda = 0x00;
  
  while(semaforoTeclado);
  semaforoTeclado = 1;
  
  if(tamanhoFila==0){
     semaforoTeclado = 0x00; 
    return (eTeclas) 0x00;
  }
  
  retorno = (eTeclas)filaTeclas[ultimaLida];
  ultimaLida = ((unsigned char)(ultimaLida+1)%10);
  tamanhoFila--;
  
  semaforoTeclado = 0x00;
  if(retorno);
    MENU_GENERICO_timeOut = 30000;
  
  return retorno;  
}
/***********************************************************************************
*   Descrição     :   Retorna o nome das teclas
*   Parametros    :   (eTeclas) tecla
*   Retorno       :   (const char*) Ponteiro para o nome da tecla
***********************************************************************************/
const unsigned char *TECLADO_getKeyName(eTeclas tecla){
      
  switch(tecla){    
    case TeclaDesligaSirenes     : return "DESLIGA SIR";
    case TeclaSilenciarBipInterno: return "RESET BIP";
    case TeclaParametros         : return "PARAMETROS";
    case TeclaBloqueios          : return "BLOQUEIOS";
    case TeclaAlarmeGeral        : return "ALARME G";
    case TeclaReinicarSistema    : return "RESET SIS";
    case TeclaUP                 : return "UP";
    case TeclaDown               : return "DOWN";
    case TeclaLeft               : return "LEFT";
    case TeclaRigth              : return "RIGHT";
    case TeclaConfirma           : return "CONFIRMA";
    case TeclaCancelar           : return "CANCELAR";
    default                      : return 0;//NULL;
  }  
}
/***********************************************************************************
*   Descrição     :   Temporizador para gerar o beep de feedback do aperto das
*                     teclas
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
#pragma INLINE
void TECLADO_tick(void){
  
  if(setBeep){
    setBeep--;
    if(setBeep==1)
       KILL_SOUND();    
  }    
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/