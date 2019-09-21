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
|       Arquivo            :  FilaDevices.c
|       Descrição          :  Fila que guarda os ultimos 10 endereços
|                             recebidos em cada laço do sistema
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  13/07/2010
|
|       Revisões           :
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
*     DEFINIÇÕES
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
*		Funções locais
***********************************************************************************/
unsigned char FILADEVICES_insere(unsigned char loop,unsigned char device);  
/***********************************************************************************
*		Implementação
***********************************************************************************/

/***********************************************************************************
*   Descrição       :   Inicializa a fila de devices recebidos
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
*   Descrição       :   Insere um device em uma das filas dos laços
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
*   Descrição       :   Remove um device de uma das filas
*   Parametros      :   (unsigned char) central
*                       (unsigned char*) número do device
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
*   Descrição       :   Faz a monitoração dos endereços que são recebidos
*                       pelos capturas dos laços
*   Parametros      :   nenhum
*   Retorno         :   nenhum
***********************************************************************************/
void FILADEVICES_main(void){
  static unsigned char ponto;
  //Não é feito iteração pq assim é mais rápido

  //endereço dos dispositivos do laço 01
  if(NEW_CAPTURE(0)){
    ponto = decodificaBitStream(0);        
    (void)FILADEVICES_insere(0,ponto);    
    ACK_CAPTURE(0);
  }
  
  //endereço dos dispositivos do laço 02
  if(NEW_CAPTURE(1)){
    ponto = decodificaBitStream(1);        
    (void)FILADEVICES_insere(1,ponto);    
    ACK_CAPTURE(1);
  }
  
  //endereço dos dispositivos do laço 03
  if(NEW_CAPTURE(2)){
    ponto = decodificaBitStream(2);        
    (void)FILADEVICES_insere(2,ponto);    
    ACK_CAPTURE(2);
  }
  
  //endereço dos dispositivos do laço 04
  if(NEW_CAPTURE(3)){
    ponto = decodificaBitStream(3);        
    (void)FILADEVICES_insere(3,ponto);    
    ACK_CAPTURE(3);
  }           
}
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/