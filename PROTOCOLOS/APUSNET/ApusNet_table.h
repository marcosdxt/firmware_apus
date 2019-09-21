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
|       Arquivo            :  ApusNet_table.h
|       Data criação       :  06/07/2010
|       Revisões		       :  1.1
|
| __________________________________________________________________________________
*/

#ifndef _APUS_NET_TABLE_H_
#define _APUS_NET_TABLE_H_


typedef enum{
  ReadOnly,
  WriteOnly,
  ReadAndWrite    
}eRegisterPermission;



typedef struct{
  unsigned char *pData;
  eRegisterPermission permission;  
}sTableRow;


#define ROW_COUNT                             1
#define PROTOCOL_VERSION                      1

const unsigned char versao= ROW_COUNT;

const sTableRow sharedTable[] = {{(unsigned char*)versao,ReadAndWrite}};

#endif //