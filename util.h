/*********************************************************************************
*       Arquivo     : Util.h
*       Descri��o   : Fun��es uteis
*       Autor       : Marcos Aquino
*       Revis�o     : 1.0.0.0
*       Data        : 27/11/2008
*********************************************************************************/

/*********************************************************************************
*       Defini��es 
*********************************************************************************/


/*********************************************************************************
*       Tipos
*********************************************************************************/


/*********************************************************************************
*       Variaveis globais
*********************************************************************************/


/*********************************************************************************
*       Fun��es 
*********************************************************************************/
void UTIL_calcula_crc(unsigned char *inicio_frame,unsigned char tamanho);
unsigned char UTIL_checa_crc(unsigned char *inicio_frame, unsigned char tamanho);
unsigned int UTIL_calculaCRC16(unsigned char *buffer,unsigned char tamanho);
/*********************************************************************************
*       Fim do arquivo
*********************************************************************************/
