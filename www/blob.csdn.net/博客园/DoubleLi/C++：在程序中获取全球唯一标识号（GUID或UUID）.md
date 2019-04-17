# C++：在程序中获取全球唯一标识号（GUID或UUID） - DoubleLi - 博客园






Windows：使用CoCreateGuid函数（GUID）

#include <objbase.h>
#include <stdio.h>

#define GUID_LEN 64

int main(int argc, char* argv[])
{
 char buffer[GUID_LEN] = { 0 };
 GUID guid;

 if ( CoCreateGuid(&guid) )
 {
  fprintf(stderr, "create guid error\n");
  return -1;
 }
 _snprintf(buffer, sizeof(buffer), 
  "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X", 
  guid.Data1, guid.Data2, guid.Data3, 
  guid.Data4[0], guid.Data4[1], guid.Data4[2], 
  guid.Data4[3], guid.Data4[4], guid.Data4[5], 
  guid.Data4[6], guid.Data4[7]);
 printf("guid: %s\n", buffer);

 return 0;
}

**Linux：使用uuid_generate函数（UUID）**

//编译命令：gcc atemp.c -o atemp -luuid
#include <stdio.h>
#include <uuid/uuid.h>

int main() 
{ 
 uuid_t uu;
 int i;
 uuid_generate( uu );

 for(i=0;i<16;i++) 
 { 
  printf("%02X-",uu[i]); 
 } 
 printf("\n");

 return 0; 
}









