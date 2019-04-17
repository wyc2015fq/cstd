# Linux下用gSOAP开发Web Service服务端和客户端程序(一) - DoubleLi - 博客园






1、功能说明：

       要开发的Web Service功能非常简单，就是一个add函数，将两个参数相加，返回其和。



2、C版本的程序:

（1）头文件：SmsWBS.h，注释部分不可少，url部分的IP必须填写当前Linux电脑的IP

//gsoap ns service name: SmsWBS
//gsoap ns service style: rpc
//gsoap ns service namespace: [http://192.168.2.161:8000/SmsWBS.wsdl](http://192.168.2.161:9000/SmsWBS.wsd)
//gsoap ns service location: [http://192.168.2.161:8000](http://192.168.2.161:9000/)
//gsoap ns service encoding: encoded
//gsoap ns schema namespace: urn:SmsWBS

int ns__add(int num1, int num2, int *sum);
这个头文件需要注意的是，前面的 // 部分是有意义的，可以在上面修改，如果完全去掉，将会导致生成的中间文件不同，由此会引起要修改Makefile文件                                           

（2）Makefile文件：

比较关键，如果头文件中的 // 部分有修改，要检查是否要修改Makefile文件

GSOAP_ROOT=/usr/local/gSOAP
WSNAME0=soap
WSNAME=SmsWBS
CC=g++ -g -DWITH_NONAMESPACES
INCLUDE=-I $(GSOAP_ROOT)/include
SERVER_OBJS=$(WSNAME0)C.o $(WSNAME0)Server.o stdsoap2.o
CLIENT_OBJS=$(GSOAP_ROOT)/env/envC.o $(WSNAME0)ClientLib.o stdsoap2.o
ALL_OBJS=${WSNAME}server.o $(WSNAME0)C.o $(WSNAME0)Server.o ${WSNAME}test.o $(WSNAME0)ClientLib.o
#GSOAP_SRC=/usr/local/gsoap-2.7/gsoap

all:server

${WSNAME}.wsdl:${WSNAME}.h
        $(GSOAP_ROOT)/bin/soapcpp2 -c $(GSOAP_ROOT)/import ${WSNAME}.h

stdsoap2.o:$(GSOAP_ROOT)/src/stdsoap2.c
        $(CC) -c $? $(INCLUDE)

$(ALL_OBJS):%.o:%.c
        $(CC) -c $? $(INCLUDE)

server:Makefile ${WSNAME}.wsdl ${WSNAME}server.o $(SERVER_OBJS)
        $(CC) ${WSNAME}server.o $(SERVER_OBJS) -o ${WSNAME}server

client:Makefile ${WSNAME}.wsdl ${WSNAME}test.c $(ALL_OBJS) stdsoap2.o
        $(CC) ${WSNAME}test.o $(CLIENT_OBJS) -o ${WSNAME}test

clean:
        rm -f *.o *.xml *.a *.wsdl *.nsmap $(WSNAME0)H.h $(WSNAME0)C.c $(WSNAME0)Server.c $(WSNAME0)Client.c $(WSNAME0)Stub.* $(WSNAME)$(WSNAME)Proxy.* $(WSNAME)$(WSNAME)Object.* $(WSNAME0)ServerLib.c $(WSNAME0)ClientLib.c $(WSNAME)server ns.xsd $(WSNAME)test



（3）服务端程序SmsWBSserver.c：

#include "soapH.h"
#include "SmsWBS.nsmap"

int main(int argc, char **argv)
{
        int m, s;               /* master and slave sockets */
        struct soap SmsWBS_soap;

        soap_init(&SmsWBS_soap);

        soap_set_namespaces(&SmsWBS_soap, namespaces);

        if (argc < 2)
        {
                printf("usage: %s <server_port> \n", argv[0]);
                exit(1);
        }
        else
        {
                m = soap_bind(&SmsWBS_soap, NULL, atoi(argv[1]), 100);
                if (m < 0)
                {
                        soap_print_fault(&SmsWBS_soap, stderr);
                        exit(-1);
                }

                fprintf(stderr, "Socket connection successful: master socket = %d\n", m);

                for (;;)
                {
                        s = soap_accept(&SmsWBS_soap);

                        if (s < 0)
                        {
                                soap_print_fault(&SmsWBS_soap, stderr);
                                exit(-1);
                        }

                        fprintf(stderr, "Socket connection successful: slave socket = %d\n", s);
                        soap_serve(&SmsWBS_soap);
                        soap_end(&SmsWBS_soap);
                }

       }

        return 0;
}

int ns__add(struct soap *add_soap, int num1, int num2, int *sum)
{
        *sum = num1 + num2;
        return 0;
}



（4）客户端程序SmsWBStest.c：

#include <stdio.h>
#include <stdlib.h>
#include "soapStub.h"
#include "SmsWBS.nsmap"

int add(const char *server, int num1, int num2, int *sum);

int add(const char *server, int num1, int num2, int *sum)
{
        struct soap SmsWBS_soap;
        int result = 0;

        soap_init(&SmsWBS_soap);
        soap_set_namespaces(&SmsWBS_soap, namespaces);

        soap_call_ns__add(&SmsWBS_soap, server, "", num1, num2, sum);

        if(SmsWBS_soap.error)
        {
                printf("soap error:%d, %s, %s ", SmsWBS_soap.error, *soap_faultcode(&SmsWBS_soap), *soap_faultstring(&SmsWBS_soap));
                result = SmsWBS_soap.error;
         }

        soap_end(&SmsWBS_soap);
        soap_done(&SmsWBS_soap);

        return result;
}

int main(int argc, char **argv)
{
        int result = -1;
        char* server="[http://localhost:8000](http://localhost:9000/)";

        int num1 = 0;
        int num2 = 0;
        int sum = 0;

        if( argc < 3 )
        {
                printf("usage: %s num1 num2 \n", argv[0]);
                exit(0);

        }

        num1 = atoi(argv[1]);
        num2 = atoi(argv[2]);

        result = add(server, num1, num2, &sum);
        if (result != 0)
        {
                printf("soap err, errcode = %d \n", result);
        }
        else
        {
                printf("%d + %d = %d \n", num1, num2, sum);
        }

        return 0;
}

（5）编译和运行：

前面都已经准备好了，现在只需要：

make                                     ---得到服务端程序SmsWBSserver

make client                            ---得到客户端程序SmsWBStest

SmsWBSserver 9000             ----运行服务端程序

出来类似下面的显示就表示运行正常

Socket connection successful: master socket = 3

再运行客户端程序：

SmsWBStest 67 78

显示：

67 + 78 = 145

调用Web Service成功









