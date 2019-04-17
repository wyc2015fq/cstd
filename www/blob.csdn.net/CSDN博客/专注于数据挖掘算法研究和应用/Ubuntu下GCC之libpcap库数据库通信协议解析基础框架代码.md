# Ubuntu下GCC之libpcap库数据库通信协议解析基础框架代码 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年10月13日 16:21:16[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2555








/*

 * dbaudit.c

 *

 * dbaudit example of TCP/IP packet capture using libpcap.

 * 

 * Version 0.1 (2011-09-20)

 * Copyright (c) 2011 The dbaudit Group

 *

 * This software is intended to be used as a practical example and 

 * demonstration of the libpcap library; available at:

 * [http://www.tcpdump.org/](http://www.tcpdump.org/)

 *

 ****************************************************************************

 * 

 * Example compiler command-line for GCC:

 *   gcc -Wall -o dbaudit dbaudit.c -lpcap

 * 

 ****************************************************************************

 *

 * Code Comments

 *

 * This section contains additional information and explanations regarding

 * comments in the source code. It serves as documentaion and rationale

 * for why the code is written as it is without hindering readability, as it

 * might if it were placed along with the actual code inline. References in

 * the code appear as footnote notation (e.g. [1]).

 *

 * 1. Ethernet headers are always exactly 14 bytes, so we define this

 * explicitly with "#define". Since some compilers might pad structures to a

 * multiple of 4 bytes - some versions of GCC for ARM may do this -

 * "sizeof (struct sniff_ethernet)" isn't used.

 * 

 * 2. Check the link-layer type of the device that's being opened to make

 * sure it's Ethernet, since that's all we handle in this example. Other

 * link-layer types may have different length headers (see [1]).

 *

 * 3. This is the filter expression that tells libpcap which packets we're

 * interested in (i.e. which packets to capture). Since this source example

 * focuses on IP and TCP, we use the expression "ip", so we know we'll only

 * encounter IP packets. The capture filter syntax, along with some

 * examples, is documented in the tcpdump man page under "expression."

 * Below are a few simple examples:

 *

 * Expression   Description

 * ----------   -----------

 * ip    Capture all IP packets.

 * tcp    Capture only TCP packets.

 * tcp port 80   Capture only TCP packets with a port equal to 80.

 * ip host 10.1.2.3  Capture all IP packets to or from host 10.1.2.3.

 *

 ****************************************************************************

 *

 */

/*include start*/

#define APP_NAME  "fdbaudit"

#define APP_DESC  "dbaudit example using libpcap"

#define APP_COPYRIGHT         "Copyright (c) 2011 dbaudit"

#define APP_DISCLAIMER         "THERE IS ABSOLUTELY NO WARRANTY FOR THIS PROGRAM."

#include <pcap.h>

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <ctype.h>

#include <errno.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

//#define OFFLINE //when read offline file

#define MAX_S 1024//copy to file

/*include end*/

/*protocol define start*/

/* default snap length (maximum bytes per packet to capture) */

#define SNAP_LEN 1518

/* ethernet headers are always exactly 14 bytes [1] */

#define SIZE_ETHERNET 14

/* Ethernet addresses are 6 bytes */

#define ETHER_ADDR_LEN 6

/* Ethernet header */

struct db_ethernet {

        u_char  ether_dhost[ETHER_ADDR_LEN];    /* destination host address */

        u_char  ether_shost[ETHER_ADDR_LEN];    /* source host address */

        u_short ether_type;                     /* IP? ARP? RARP? etc */

};

/* IP header */

struct db_ip {

        u_char  ip_vhl;                 /* version << 4 | header length >> 2 */

        u_char  ip_tos;                 /* type of service */

        u_short ip_len;                 /* total length */

        u_short ip_id;                  /* identification */

        u_short ip_off;                 /* fragment offset field */

        #define IP_RF 0x8000            /* reserved fragment flag */

        #define IP_DF 0x4000            /* dont fragment flag */

        #define IP_MF 0x2000            /* more fragments flag */

        #define IP_OFFMASK 0x1fff       /* mask for fragmenting bits */

        u_char  ip_ttl;                 /* time to live */

        u_char  ip_p;                   /* protocol */

        u_short ip_sum;                 /* checksum */

        struct  in_addr ip_src,ip_dst;  /* source and dest address */

};

#define IP_HL(ip)               (((ip)->ip_vhl) & 0x0f)

#define IP_V(ip)                (((ip)->ip_vhl) >> 4)

/* TCP header */

typedef u_int tcp_seq;

struct db_tcp {

        u_short th_sport;               /* source port */

        u_short th_dport;               /* destination port */

        tcp_seq th_seq;                 /* sequence number */

        tcp_seq th_ack;                 /* acknowledgement number */

        u_char  th_offx2;               /* data offset, rsvd */

#define TH_OFF(th)      (((th)->th_offx2 & 0xf0) >> 4)

        u_char  th_flags;

        #define TH_FIN  0x01

        #define TH_SYN  0x02

        #define TH_RST  0x04

        #define TH_PUSH 0x08

        #define TH_ACK  0x10

        #define TH_URG  0x20

        #define TH_ECE  0x40

        #define TH_CWR  0x80

        #define TH_FLAGS        (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)

        u_short th_win;                 /* window */

        u_short th_sum;                 /* checksum */

        u_short th_urp;                 /* urgent pointer */

};

/* UDP header */

struct db_udp//udp protocol

{

 u_int16_t udp_source_port;

 u_int16_t udp_destination_port;

 u_int16_t udp_length;

 u_int16_t udp_checksum;

};

/* mysql header */

struct db_mysql

{

 u_int8_t mysql_header_length;

 u_int16_t unknow1;

 u_int8_t packet_num;

 u_int8_t qry;

 u_int16_t q1;

 u_int16_t q2;

 u_int16_t q3; 

};



/*oracle header-tns protocol*/

struct db_tns

{

 u_short length;//package  length

 u_short packet_checksum;//

 u_char type;//tns type

 u_char flag;//status

 u_short header_checksum;

};

struct db_tns_connect//0x01

{

 u_short ns_vr;

 u_short ns_compatible_ver;

 u_short ser_opt;

// u_char ser_opt2;

 u_short SDU_size;

 u_short TDU_size;

 u_short NT_protocol_ch;

// u_char NT_protocol_ch2;

 u_short max_packets;

 u_short hardware;

 u_short data_length;

 u_short offset;

 u_int32_t max_data;

 u_char flag0;

 u_char flag1;

/*

if not jdbc,use offet to override these;

*/

 u_int32_t item;

 u_int32_t item2;

 u_char id[8];

 u_char unkonwn[8];

// u_char *buff; //no need

};

struct db_tns_accept//0x02

{

 u_short ns_vr;

 u_short ser_opt;

// u_char ser_opt2;//no need

 u_short SDU_size;

 u_short TDU_size;

 u_short hardware;

 u_short data_length;

 u_short offset;

 u_char flag0;

 u_char flag1;

};

struct db_tns_reject//0x04

{

 u_char usr_refuse_reason;

 u_char sys_refuse_reason;

 u_char data_length;

};

struct db_tns_redirect//0x05

{

 u_short data_length;

 u_char data;//

};

struct db_tns_marker//0x0c

{

 //u_char data_length[3];

 u_char type;

 u_char data_byte0;

 u_char data_byte1;

};

struct db_tns_data

{

 u_short data_flag;//Data Flag


};

/*sqlserver header-tds protocol*/

/*DB2 header-db2 protocol*/

/*sybase header-npgsql protocol*/

/*Postgresql header-tds protocol*/

/*protocol define end*/

/*function define start*/

void

got_ethernet_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

void 

got_ip_package(u_char *args,const struct pcap_pkthdr *header,const u_char *packet);

void 

got_tcp_package(u_char *args,const struct pcap_pkthdr *header,const u_char *packet,int size_ip);

void 

got_udp_package(u_char *args,const struct pcap_pkthdr *header,const u_char *packet,int size_ip);

void 

got_mysql_package(u_char *args,const struct pcap_pkthdr *header,const u_char *packet,int size_tcp);

void

got_tns_package(u_char *args,const struct pcap_pkthdr *header,const u_char *packet,int size_tcp);

void 

got_data_package(u_char *args,const struct pcap_pkthdr *header,const u_char *packet,int size_header);

void

print_payload(const u_char *payload, int len);

void

print_hex_ascii_line(const u_char *payload, int len, int offset);

void

print_app_banner(void);

void

print_app_usage(void);

void 

put_in_file(const u_char *buff,int len);

/*function define end*/

/*function implement start*/

/*

 * app name/banner

 */

void

print_app_banner(void)

{

 printf("%s - %s\n", APP_NAME, APP_DESC);

 printf("%s\n", APP_COPYRIGHT);

 printf("%s\n", APP_DISCLAIMER);

 printf("\n");

return;

}

/*

 * print help text

 */

void

print_app_usage(void)

{

 printf("Usage: %s [interface]\n", APP_NAME);

 printf("\n");

 printf("Options:\n");

 printf("    interface    Listen on <interface> for packets.\n");

 printf("\n");

return;

}

/*

 * print data in rows of 16 bytes: offset   hex   ascii

 *

 * 00000   47 45 54 20 2f 20 48 54  54 50 2f 31 2e 31 0d 0a   GET / HTTP/1.1..

 */

void

print_hex_ascii_line(const u_char *payload, int len, int offset)

{

 int i;

 int gap;

 const u_char *ch;

 /* offset */

 printf("%05d   ", offset);


 /* hex */

 ch = payload;

 for(i = 0; i < len; i++) {

  printf("%02x ", *ch);

  ch++;

  /* print extra space after 8th byte for visual aid */

  if (i == 7)

   printf(" ");

 }

 /* print space to handle line less than 8 bytes */

 if (len < 8)

  printf(" ");


 /* fill hex gap with spaces if not full line */

 if (len < 16) {

  gap = 16 - len;

  for (i = 0; i < gap; i++) {

   printf("   ");

  }

 }

 printf("   ");


 /* ascii (if printable) */

 ch = payload;

 for(i = 0; i < len; i++) {

  if (isprint(*ch))

   printf("%c", *ch);

  else

   printf(".");

  ch++;

 }

 printf("\n");

return;

}

/*

 * print packet payload data (avoid printing binary data)

 */

void

print_payload(const u_char *payload, int len)

{

 int len_rem = len;

 int line_width = 16;   /* number of bytes per line */

 int line_len;

 int offset = 0;     /* zero-based offset counter */

 const u_char *ch = payload;

 if (len <= 0)

  return;

 /* data fits on one line */

 if (len <= line_width) {

  print_hex_ascii_line(ch, len, offset);

  return;

 }

 /* data spans multiple lines */

 for ( ;; ) {

  /* compute current line length */

  line_len = line_width % len_rem;

  /* print line */

  print_hex_ascii_line(ch, line_len, offset);

  /* compute total remaining */

  len_rem = len_rem - line_len;

  /* shift pointer to remaining bytes to print */

  ch = ch + line_len;

  /* add offset */

  offset = offset + line_width;

  /* check if we have line width chars or less */

  if (len_rem <= line_width) {

   /* print last line and get out */

   print_hex_ascii_line(ch, len_rem, offset);

   break;

  }

 }

return;

}

/*

putting buffer into buffer.txt

*/

void put_in_file(const u_char *buff,int len)

{

 u_char tmp[MAX_S];

 FILE *buffer;

 memcpy(tmp,buff,len);

 buffer=fopen("buffer.txt","ab");//append binary

 if(buffer==NULL)

  printf("FILE OPEN/CREATE FAIL\n");

 else{

 fwrite(tmp,sizeof(u_char),len,buffer);

 fwrite("\n",sizeof(char),1,buffer);

 fclose(buffer);

 }


}

/*

 * dissect/print data packet

 */

void 

got_data_package(u_char *args,const struct pcap_pkthdr *header,const u_char *packet,int size_header)

{

 const u_char *payload;                    /* Packet payload */ 

 int size_payload;


 /* define/compute  payload (segment) offset */

 payload = (u_char *)(packet + size_header);


 /* compute tcp payload (segment) size */

 const struct db_ip *ip;

 ip = (struct db_ip*)(packet + SIZE_ETHERNET);

 size_payload = ntohs(ip->ip_len) - (size_header-SIZE_ETHERNET);//data len=total len-size_ip-size_tcp


 /*

  * Print payload data; it might be binary, so don't just

  * treat it as a string.

  */

 if (size_payload > 0) {

  printf("   Payload (%d bytes):\n", size_payload);

  print_payload(payload, size_payload);

 }

return;

}



/*

 * dissect/print mysql packet

 */

void 

got_mysql_package(u_char *args,const struct pcap_pkthdr *header,const u_char *packet,int size_tcp)

{

 struct  db_mysql *mysql;

 int  size_mysql; 

 u_short pa_num;

 u_short qry;

 u_short  hlen;

 mysql=(struct db_mysql*)(packet+size_tcp);

 hlen=ntohs(mysql->mysql_header_length);

 pa_num=ntohs(mysql->packet_num);

 qry=ntohs(mysql->qry);

 printf("********************MYSQL Protocol*************************\n");

 printf("Unknown:%02x\n",mysql->unknow1);

 printf("Header length:%d----------%02x\n",hlen,mysql->mysql_header_length); 

 printf("Packet Number:%d---------%02x\n",pa_num,mysql->packet_num); 

 printf("Query :%d----------------%02x\n*****************************\n",qry,mysql->qry);

 printf("Unknown:%02x\n",mysql->q1); 

 printf("Unknown:%02x\n",mysql->q2); 

 printf("Unknown:%02x\n",mysql->q3);


 /*handle  the  mysql  data */

 size_mysql=size_tcp+sizeof(mysql);

 got_data_package(args,header,packet,size_mysql);


return;

}

/*

 * dissect/print tns packet

 */

void

got_tns_package(u_char *args,const struct pcap_pkthdr *header,const u_char *packet,int size_tcp)

{

 int i;

 struct db_tns *tns;

 u_char *buff;

 tns=(struct db_tns*)(packet+size_tcp);

 printf("********************ORACLE TNS Protocol*************************\n"); printf("size_tcp:%d\n",size_tcp);

 if(size_tcp==header->len)

  printf("The TNS Packet is empty\n");

 else{

 printf("Length:%d\n",ntohs(tns->length)); 

 printf("Packet checksum:0x%02x\n",tns->packet_checksum);

 printf("Type:%02x\n",tns->type);

 printf("Flag:%02x\n",tns->flag);

 printf("Header Checksum:0x%02x\n",tns->header_checksum);


 int  size_data=size_tcp+sizeof(struct db_tns);

 switch (tns->type)

 {

  case 0x01: //analyzing connect packet

  {

   printf("connect  package\n");

   struct db_tns_connect* con;

   con=(struct db_tns_connect*)(packet+size_data);


   printf("version::%d\n",ntohs(con->ns_vr));

   printf("version(compatible):%d\n",ntohs(con->ns_compatible_ver));

   printf("Service Options:%02x\n",ntohs(con->ser_opt));

//   printf("Service Options2:%04x\n",con->ser_opt2);

   printf("Session Data Unit Size:%d\n",ntohs(con->SDU_size));

   printf("Max Trans Data Unit:%d\n",ntohs(con->TDU_size));

   printf("NT Protocol characteristics1:%04x\n",ntohs(con->NT_protocol_ch));

//   printf("NT Protocol characteristics2:%02x\n",con->NT_protocol_ch2);

   printf("Max packets:%d\n",ntohs(con->max_packets));

   printf("Hardware:%02x\n",con->hardware);

   printf("lentgh of Connect Data:%d\n",ntohs(con->data_length));

   printf("Offset of Connect Data:%d\n",ntohs(con->offset));

   printf("Max Receivable Connect Data:%d\n",ntohl(con->max_data));

   printf("Flags 0:%02x\n",con->flag0);

   printf("Flags 1:%02x\n",con->flag1);

   printf("Trace Cross Facility Item 1:%08x\n",con->item);

   printf("Trace Cross Facility Item 2:%08x\n",con->item2);

   printf("Trace Unique Connect ID:%16x\n",ntohs(con->id));

//   print_payload(con->buff,strlen(con->buff));

   buff=(u_char *)(packet+size_data+sizeof(struct db_tns_connect)-2);//-2 =offset?

   printf("BUFFER LEN:%d\n",strlen(buff));

   put_in_file(buff,strlen(buff));

   break;

  }

  case 0x02://analyzing accept package

  { 

   printf("accept package\n");

   struct db_tns_accept* con;

   con=(struct db_tns_accept*)(packet+size_data);   

   printf("Version:%d\n",ntohs(con->ns_vr));

   printf("Service Options:%02x\n",ntohs(con->ser_opt));

   printf("Session Data Unit Size:%d\n",ntohs(con->SDU_size));

   printf("Max Trans Data Unit:%d\n",ntohs(con->TDU_size));

   printf("Hardware:%02x\n",con->hardware);

   printf("Accept Data Length:%d\n",ntohs(con->data_length));

   printf("Offset to Accept Data:%d\n",ntohs(con->offset));

   printf("Flags 0:%02x\n",con->flag0);

   printf("Flags 1:%02x\n",con->flag1);

   break;

  }

  case 0x03: 

  { 

   printf("ack package\n");




   break;

  }

  case 0x04: 

  {

   printf("reject package\n");

   struct db_tns_reject* con;

   con=(struct db_tns_reject*)(packet+size_data);

   printf("User Refuse Reason 0:%02x\n",con->usr_refuse_reason);

   printf("System Refuse Reason 0:%02x\n",con->sys_refuse_reason);

   printf("Data Length:%d\n",ntohs(con->data_length));

   break;

  }

  case 0x05: 

  {

   printf("redirect package\n");

   struct db_tns_redirect* con;

   con=(struct db_tns_redirect*)(packet+size_data);

   printf("Data Length:%d\n",ntohs(con->data_length));

   printf("Data:%02x\n",con->data);//?

   break;

  }

  case 0x06: 

  { printf("data package\n");

   //need to analyse data packages?

   break;

  }

  case 0x09: printf("abort package\n");break;

  case 0x0b:printf("resend package,no content in this type of packet\n");break;

  case 0x0c:

  {

   printf("marker package\n");

   struct db_tns_marker* con;

   con=(struct db_tns_marker*)(packet+size_data);

   printf("Marker Type:%02x\n",con->type); 

   printf("Marker Data Type:%02x\n",con->data_byte0); 

   printf("Marker Data Type:%02x\n",con->data_byte1);   

   break;

  }

  case 0x0d:printf("attention package\n");break;

                case 0x0e:printf("control package\n");break;

  default:break;

 }}

return;

}

/*

 * dissect/print udp packet

 */

void 

got_udp_package(u_char *args,const struct pcap_pkthdr *header,const u_char *packet,int size_ip)

{

 struct db_udp *udp;

 int  size_udp;

 u_short sport;

 u_short dport;

 u_short length;

 udp=(struct db_udp*)(packet+size_ip);

 sport=ntohs(udp->udp_source_port);

 dport=ntohs(udp->udp_destination_port);

 length=ntohs(udp->udp_length);

 printf("-------UDP Protocol (Transport Layer)---------\n");

 printf("Source Port:%d\n",sport);

 printf("Destination Port:%d\n",dport);

 printf("Length:%d\n",length);

 printf("Checksum:%d\n",ntohs(udp->udp_checksum));

 size_udp=length+size_ip;


 if(sport==3306  ||  dport==3306)

 {

  printf("mysql  protocol\n");

  got_mysql_package(args,header,packet,size_udp);

 }

 else if(sport==1521  ||  dport==1521)

 {

  printf("Oracle tns  protocol\n");

  got_tns_package(args,header,packet,size_udp);

 }

 else if(sport==1433  ||  dport==1433)

  printf("SQLSERVER tds  protocol\n");

 else got_data_package(args,header,packet,size_udp);

return;

}

/*

 * dissect/print tcp packet

 */

void 

got_tcp_package(u_char *args,const struct pcap_pkthdr *header,const u_char *packet,int size_ip)

{ 

 const struct db_tcp *tcp;            /* The TCP header */

 int size_tcp; 

 u_char flags;

 u_short windows;

 u_short urgent_pointer;

 u_int sequence;

 u_int acknowledgement;

 u_int16_t checksum;

 u_short sport;

 u_short dport;


 /* define/compute tcp header offset */

 tcp = (struct db_tcp*)(packet + size_ip);

 size_tcp = TH_OFF(tcp)*4;

 if (size_tcp < 20) {

  printf(" * Invalid TCP header length: %u bytes\n", size_tcp);

  return;

 }

 sequence=ntohl(tcp->th_seq);

 acknowledgement=ntohl(tcp->th_ack);

 windows=ntohs(tcp->th_win);

 urgent_pointer=ntohs(tcp->th_urp);

 flags=tcp->th_flags;

 checksum=ntohs(tcp->th_sum);

 sport=ntohs(tcp->th_sport);

 dport=ntohs(tcp->th_dport);

 printf("-------TCP Protocol (Transport Layer)---------\n");

 printf("   Src port: %d\n", sport);

 printf("   Dst port: %d\n", dport);

 printf("Sequence Number:%u\n Acknowledgement Number:%u\n Header Length:%d\n Reserved:%d\n",sequence,acknowledgement,size_tcp,tcp->th_offx2);

 printf("Flags:");

 if(flags & 0x08) printf("PSH");

 if(flags & 0x10) printf("ACK");

 if(flags & 0x02) printf("SYN");

 if(flags & 0x20) printf("URG");

 if(flags & 0x01) printf("FIN");

 if(flags & 0x04) printf("RST");

 printf("\n");

 printf("Window Size:%d\n",windows);

 printf("Checksum:%d\n",checksum);

 printf("Urgent Pointer:%d\n",urgent_pointer);


 size_tcp=size_tcp+size_ip;

 if(sport==3306  ||  dport==3306)

 {

  printf("mysql  protocol\n");

  got_mysql_package(args,header,packet,size_tcp);

 }

 else if(sport==1521  ||  dport==1521)

 {

  printf("Oracle tns  protocol\n");

  got_tns_package(args,header,packet,size_tcp);

 }

 else if(sport==1433  ||  dport==1433)

  printf("SQLSERVER tds  protocol\n");

 else got_data_package(args,header,packet,size_tcp);


return;

}

/*

 * dissect/print ip packet

 */

void 

got_ip_package(u_char *args,const struct pcap_pkthdr *header,const u_char *packet)

{

        const struct db_ip *ip;              /* The IP header */

        int size_ip;

        u_int offset;

        u_char tos;

 u_int16_t checksum;


        /* define/compute ip header offset */

 ip = (struct db_ip*)(packet + SIZE_ETHERNET);

 size_ip = IP_HL(ip)*4;

 if (size_ip < 20) {

  printf("*Invalid IP header length: %u bytes\n", size_ip);

  return;

 }

        checksum=ntohs(ip->ip_sum);

 tos=ip->ip_tos;

 offset=ntohs(ip->ip_off);

 /* print source and destination IP addresses */

        printf("-------IP Protocol (Network Layer)---------\n");

        printf("IP Version:%d\n",IP_V(ip));

 printf("Header Length:%d\n",size_ip);

 printf("TOS:%d\n",tos);

 printf("Total length:%d\n",ntohs(ip->ip_len));

 printf("Identification:%d\n",ntohs(ip->ip_id));

 printf("Offset:%d\n",(offset & 0x1fff)*8);

 printf("TTL:%d\n",ip->ip_ttl);

 printf("Header checksum:%d\n",checksum);

 printf(" From: %s", inet_ntoa(ip->ip_src));

 printf("  To: %s\n", inet_ntoa(ip->ip_dst));


 /* determine protocol */ 

 size_ip=size_ip+SIZE_ETHERNET;

 switch(ip->ip_p) {

  case IPPROTO_TCP:

   got_tcp_package(args,header,packet,size_ip);

   break;

  case IPPROTO_UDP: 

   got_udp_package(args,header,packet,size_ip);

   break;

  default:

   printf("   Protocol: unknown\n");

   break;

 }

return;

}

/*

 * dissect/print ethernet packet

 */

void

got_ethernet_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)

{

 static int count = 1;                   /* packet counter */


 /* declare pointers to packet headers */

 const struct db_ethernet *ethernet;  /* The ethernet header [1] */

        u_short ethernet_type;

        u_char *mac_string;

 printf("================The %d package is captured.======================\n",count);

 count++;

        printf("-------Ethernet Protocol (Link Layer)---------\n");

 /* get ethernet header */    

 ethernet = (struct db_ethernet*)(packet);

 printf("Mac Source Address is:\n"); 

 mac_string=(u_char*)ethernet->ether_shost;

 printf("%02x:%02x:%02x:%02x:%02x:%02x\n",*mac_string,*(mac_string+1),*(mac_string+2),*(mac_string+3),*(mac_string+4),*(mac_string+5));

 printf("Mac Destination Address is:\n");

 mac_string=(u_char*)ethernet->ether_dhost;

 printf("%02x:%02x:%02x:%02x:%02x:%02x\n",*mac_string,*(mac_string+1),*(mac_string+2),*(mac_string+3),*(mac_string+4),*(mac_string+5));

 ethernet_type=ntohs(ethernet ->ether_type);

        switch(ethernet_type)

 {

 case 0x0800: got_ip_package(args,header,packet);break;

 case 0x0806: printf("The network layer is ARP protocol\n");break;

 case 0x0835: printf("The network layer is RARP protocol\n");break;

 default:break;

 }

return;

}

/*function implement end*/

/*main function,entrance for system*/

int main(int argc, char **argv)

{

 char *dev = NULL;   /* capture device name */

 char errbuf[PCAP_ERRBUF_SIZE];  /* error buffer */

 pcap_t *handle;    /* packet capture handle */

 char filter_exp[] = "ip";

 //char filter_exp[] = "ip host 127.0.0.1";/*port 3306*//* filter expression [3] */

 struct bpf_program fp;   /* compiled filter program (expression) */

 bpf_u_int32 mask;   /* subnet mask */

 bpf_u_int32 net;   /* ip */

 int num_packets = -1;   /* number of packets to capture ,-1 is loop*/ 

 print_app_banner();

 /* check for capture device name on command-line */

 if (argc == 2) {

  dev = argv[1];

 }

 else if (argc > 2) {

  fprintf(stderr, "error: unrecognized command-line options\n\n");

  print_app_usage();

  exit(EXIT_FAILURE);

 }

 else {

  /* find a capture device if not specified on command-line */

  dev = pcap_lookupdev(errbuf);

  if (dev == NULL) {

   fprintf(stderr, "Couldn't find default device: %s\n",

       errbuf);

   exit(EXIT_FAILURE);

  }

 }


 /* get network number and mask associated with capture device */

 if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {

  fprintf(stderr, "Couldn't get netmask for device %s: %s\n",

      dev, errbuf);

  net = 0;

  mask = 0;

 }

 /* print capture info */

 printf("Device: %s\n", dev);

 printf("Number of packets: %d\n", num_packets);

 printf("Filter expression: %s\n", filter_exp);

#ifdef OFFLINE

 handle=pcap_open_offline("tns.pcap",errbuf);//read  offline  file

#else

 /* open capture device */

 handle = pcap_open_live(dev, SNAP_LEN, 1, 1000, errbuf);

#endif

 if (handle == NULL) {

  fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);

  exit(EXIT_FAILURE);

 }

 /* make sure we're capturing on an Ethernet device [2] */

 if (pcap_datalink(handle) != DLT_EN10MB) {

  fprintf(stderr, "%s is not an Ethernet\n", dev);

  exit(EXIT_FAILURE);

 }

 /* compile the filter expression */

 if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {

  fprintf(stderr, "Couldn't parse filter %s: %s\n",

      filter_exp, pcap_geterr(handle));

  exit(EXIT_FAILURE);

 }

 /* apply the compiled filter */

 if (pcap_setfilter(handle, &fp) == -1) {

  fprintf(stderr, "Couldn't install filter %s: %s\n",

      filter_exp, pcap_geterr(handle));

  exit(EXIT_FAILURE);

 }

 /* now we can set our callback function */

 pcap_loop(handle, num_packets, got_ethernet_packet, NULL);

 /* cleanup */

 pcap_freecode(&fp);

 pcap_close(handle);

 printf("\nCapture complete.\n");

return 0;

}





