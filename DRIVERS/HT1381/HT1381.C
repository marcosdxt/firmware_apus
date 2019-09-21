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
|       Módulo de software :  Driver para acesso ao HT1381
|       Arquivo            :  HT1381.C
|       Descrição          :  Funções para ajustar e ler o calendário e relógio
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
#include "HT1381.H"
#include "HT1381_targetWrapper.h"

/***********************************************************************************
*   Definições
***********************************************************************************/
#define COMAND_BYTE                                   0x80

#define READ                                          0x01
#define WRITE                                         0x00

#define SECONDS_REGISTER                              (0x00)<<1
#define MINUTES_REGISTER                              (0x01)<<1
#define HOURS_REGISTER                                (0x02)<<1

#define DATE_REGISTER                                 (0x03)<<1
#define MONTH_REGISTER                                (0x04)<<1
#define DAY_REGISTER                                  (0x05)<<1
#define YEAR_REGISTER                                 (0x06)<<1

#define PROTECTION_REGISTER                           (0x07)<<1
/***********************************************************************************
*		Variaveis locais
***********************************************************************************/


/***********************************************************************************
*		Funções locais
***********************************************************************************/
unsigned char dec_to_bcd(unsigned char decimal);
unsigned char bcd_to_dec(unsigned char bcd); 
void HT1381_writeByte(unsigned char data);
void HT1381_readByte(unsigned char *pData);
void HT1381_initChip(void);
void HT1381_writeRegister(unsigned char registrador,unsigned char valor);
unsigned char HT1381_readRegister(unsigned char registrador);
/***********************************************************************************
*		Implementação
***********************************************************************************/

/***********************************************************************************
*   Descrição     :   Inicialização do módulo
*   Parametros    :   nenhum
*   Retorno       :   nenhum
***********************************************************************************/
void HT1381_init(void){
  
   INIT_PINS_RTC();    
   
   IO_PIN_OUTPUT();
   
   CLR_SCLK();
   CLR_RESET_PIN();
   CLR_IO_PIN();
   
   HT1381_initChip();
}
/***********************************************************************************
*  Descrição   : CONVERTE UM VALOR DECIMAL PARA BCD
*  Parametros  : DECIMAL(UNSIGNED CHAR)
*  Retorno     : UNSIGNED CHAR
***********************************************************************************/
unsigned char dec_to_bcd(unsigned char decimal){
  unsigned char valor_temp;

  valor_temp = (16*((unsigned char) decimal/10)) + ((unsigned char) decimal%10); 
   return valor_temp; 
}
/***********************************************************************************
*  Descrição   : CONVERTE UM VALOR BCD PARA DECIMAL
*  Parametros  : BCD(UNSIGNED CHAR)
*  Retorno     : UNSIGNED CHAR
***********************************************************************************/
unsigned char bcd_to_dec(unsigned char bcd){
  unsigned char  dado, aux; 

  aux = bcd; 
  dado = bcd & 0x0F; 
  aux >>= 4; 
  aux &= 0x0F; 
  dado += (aux*10); 
  return dado; 
}  
/***********************************************************************************
*   Descrição     :   Escreve um byte no relógio
*   Parametros    :   (unsigned char) valor
*   Retorno       :   nenhum
***********************************************************************************/
void HT1381_writeByte(unsigned char data){
  unsigned char i; 
        
  for(i=0;i<8;i++){          
     if(data&0x01) SET_IO_PIN();
     else CLR_IO_PIN();         
     SET_SCLK();     
     __asm NOP;
     __asm NOP;
     CLR_SCLK();     
     data>>=1;
  }      
}
/***********************************************************************************
*   Descrição     :   Lê um byte do relógio
*   Parametros    :   (unsigned*)pData)
*   Retorno       :   nenhum
***********************************************************************************/
void HT1381_readByte(unsigned char *pData){
  unsigned char i,mask=1;
  
  *pData = 0x00;  
  for(i=0;i<8;i++){      
    CLR_SCLK();        
    __asm NOP;
    __asm NOP; 
    SET_SCLK();    
    __asm NOP;
    __asm NOP;    
    if(GET_IO_PIN())
      *pData |= mask;      
    mask<<=1;
  }     
  CLR_SCLK();
}
/***********************************************************************************
*   Descrição     :   Escreve um byte em um dos registradores 
*   Parametros    :   (unsigned char) Registrador
*                     (unsigned char) valor
*   Retorno       :   nenhum
***********************************************************************************/
void HT1381_writeRegister(unsigned char registrador,unsigned char valor){
  
  IO_PIN_OUTPUT(); // Configura o pino de dados como saída  
  SET_RESET_PIN(); // da máquina de estados do chip
    
  HT1381_writeByte(COMAND_BYTE | registrador | WRITE);//Envia o byte de controle
  HT1381_writeByte(dec_to_bcd(valor));//envia o dado
  
  CLR_RESET_PIN();   // Apaga o pino de reset
}
/***********************************************************************************
*   Descrição     :   Lê um byte de um dos registradores 
*   Parametros    :   (unsigned char) Registrador
*   Retorno       :   (unsigned char) lido
***********************************************************************************/
unsigned char HT1381_readRegister(unsigned char registrador){
  unsigned char temp;
  
  IO_PIN_OUTPUT();// Configura o pino de dados como saída  
  SET_RESET_PIN();
    
  HT1381_writeByte(COMAND_BYTE | registrador | READ);//Envia o byte de controle
  
  IO_PIN_INPUT();//Configura o pino de dados como entrada
  __asm NOP;
  __asm NOP;
  
  HT1381_readByte(&temp);// Lê o byte do registrador solicitado
    
  CLR_RESET_PIN();// Apaga o pino de reset
  
  return bcd_to_dec(temp);//Antes de retornar converte o registrador para decimal.
}
/***********************************************************************************
*   Descrição     :   Atualiza o relógio
*   Parametros    :   (unsigned char) hora,minuto,segundo
*   Retorno       :   nenhum
***********************************************************************************/
void HT1381_ajustaRelogio(unsigned char hora,unsigned char minuto,unsigned char segundos){

  HT1381_writeRegister(HOURS_REGISTER,hora);      
  HT1381_writeRegister(MINUTES_REGISTER,minuto);
  HT1381_writeRegister(SECONDS_REGISTER,segundos);  
}
/***********************************************************************************  
*  Descrição     :  Lê os valores do relógio
*  Parametros    :  (unsigned char*) Hora,
*                   (unsigned char*) minuto
*                   (unsigned char*) segundo
*  Retorno       :  nenhum
***********************************************************************************/
void HT1381_leRelogio(unsigned char *hora,unsigned char *minuto,unsigned char *segundo){

  *hora = HT1381_readRegister(HOURS_REGISTER); 
  *minuto = HT1381_readRegister(MINUTES_REGISTER);       
  *segundo = HT1381_readRegister(SECONDS_REGISTER);  
}
/***********************************************************************************  
*  Descrição    : Ajusta o calendário
*  Parametros   : (unsigned char) dia
*                 (unsigned char) mes
*                 (unsigned char) ano
*  Retorno      : (nenhum)
***********************************************************************************/
void HT1381_ajustaCalendario(unsigned char dia,unsigned char mes,unsigned char ano){
 
  HT1381_writeRegister(DATE_REGISTER,dia);  
  HT1381_writeRegister(MONTH_REGISTER,mes);  
  HT1381_writeRegister(YEAR_REGISTER,ano);   
}
/***********************************************************************************  
*   Descrição   :   Lê os registradores do calendário
*   Parametros  :   (unsigned char*)dia,
*                   (unsigned char*)mes,
*                   (unsigned char*)ano
*   Retorno     :   nenhum
***********************************************************************************/
void HT1381_leCalendario(unsigned char *dia,unsigned char *mes,unsigned char *ano){

  *dia = HT1381_readRegister(DATE_REGISTER);
  *mes = HT1381_readRegister(MONTH_REGISTER);
  *ano = HT1381_readRegister(YEAR_REGISTER);
}
/***********************************************************************************  
*   Descrição   :   Inicializa o Chip
*   Parametros  :   nenhum
*   Retorno     :   nenhum
***********************************************************************************/
void HT1381_initChip(void){
   
  HT1381_writeRegister(PROTECTION_REGISTER,0x00);
}
/***********************************************************************************  
*	Fim do arquivo
***********************************************************************************/

