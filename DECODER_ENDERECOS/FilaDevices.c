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
|       Produto            :  Central Apus CD432
|       Arquivo            :  FilaDevices.c
|       Descri��o          :  Fila que guarda os ultimos 10 endere�os
|                             recebidos em cada la�o do sistema
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  13/07/2010
|
|       Revis�es           :
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*		Includes
***********************************************************************************/
#include "devicesDecoder.h"
#include "filaDevices.h"

/***********************************************************************************
*     DEFINI��ES
***********************************************************************************/
#define TAMANHO_FILA                                      10
/***********************************************************************************
*		Variaveis locais
***********************************************************************************/
static unsigned char filaDevices[4][TAMANHO_FILA],
                     ultimoDeviceInserido[4],
                     ultimoDeviceLido[4],
                     devicesNaFila[4],
                     semaforoFila=1;
              
              
/***********************************************************************************
*		Fun��es locais
***********************************************************************************/
unsigned char FILADEVICES_insere(unsigned char loop,unsigned char device);  
/***********************************************************************************
*		Implementa��o
***********************************************************************************/

/***********************************************************************************
*   Descri��o       :   Inicializa a fila de devices recebidos
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void FILADEVICES_init(void){
   unsigned char i;
  
   for(i=0;i<4;i++){
     ultimoDeviceInserido[i] = 0x00;      
     ultimoDeviceLido[i] = 0x00;  
     devicesNaFila[i] = 0x00;  
   }    
}
/***********************************************************************************
*   Descri��o       :   Insere um device em uma das filas dos la�os
*   Parametros      :   (unsigned char) fila
*                       (unsigned char) device
*   Retoro          :   (unsigned char) maior do que zero se conseguir inserir
***********************************************************************************/
unsigned char FILADEVICES_insere(unsigned char loop,unsigned char device){
    
  while(!semaforoFila);
  semaforoFila=0;
  
  
  if(devicesNaFila[loop]>TAMANHO_FILA || device == 0)  {
    semaforoFila =1; 
    return 0;
  }
    
  filaDevices[loop][ultimoDeviceInserido[loop]] = device;
  ultimoDeviceInserido[loop] = (unsigned char)((ultimoDeviceInserido[loop]+1) % TAMANHO_FILA);
  devicesNaFila[loop]++; 
  
  semaforoFila=1;  
  
  return 0xFF;   
}
/***********************************************************************************
*   Descri��o       :   Remove um device de uma das filas
*   Parametros      :   (unsigned char) central
*                       (unsigned char*) n�mero do device
*   Retorno         :   (unsigned char) maior do que zero se houver alguem na fila
***********************************************************************************/
unsigned char FILADEVICES_get(unsigned char loop,unsigned char *device){

  while(!semaforoFila);
  semaforoFila=0;

  
  if(!devicesNaFila[loop]){
    semaforoFila=1; 
    return 0;
  }
    
  *device = filaDevices[loop][ultimoDeviceLido[loop]];
  ultimoDeviceLido[loop] = (unsigned char)((ultimoDeviceLido[loop]+1)%TAMANHO_FILA);
  devicesNaFila[loop]--;  
  
  semaforoFila=1;
  
  return 0xFF;
}
/***********************************************************************************
*   Descri��o       :   Faz a monitora��o dos endere�os que s�o recebidos
*                       pelos capturas dos la�os
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void FILADEVICES_main(void){
  static unsigned char ponto;
  //N�o � feito itera��o pq assim � mais r�pido

  //endere�o dos dispositivos do la�o 01
  if(NEW_CAPTURE(0)){
    ponto = decodificaBitStream(0);        
    (void)FILADEVICES_insere(0,ponto);    
    ACK_CAPTURE(0);
  }
  
  //endere�o dos dispositivos do la�o 02
  if(NEW_CAPTURE(1)){
    ponto = decodificaBitStream(1);        
    (void)FILADEVICES_insere(1,ponto);    
    ACK_CAPTURE(1);
  }
  
  //endere�o dos dispositivos do la�o 03
  if(NEW_CAPTURE(2)){
    ponto = decodificaBitStream(2);        
    (void)FILADEVICES_insere(2,ponto);    
    ACK_CAPTURE(2);
  }
  
  //endere�o dos dispositivos do la�o 04
  if(NEW_CAPTURE(3)){
    ponto = decodificaBitStream(3);        
    (void)FILADEVICES_insere(3,ponto);    
    ACK_CAPTURE(3);
  }           
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/