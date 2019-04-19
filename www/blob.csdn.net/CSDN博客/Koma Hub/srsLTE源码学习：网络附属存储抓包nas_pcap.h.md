# srsLTE源码学习：网络附属存储抓包nas_pcap.h - Koma Hub - CSDN博客
2019年04月10日 22:14:44[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：11
**Table of Contents**
[nas_pcap.h    lib\include\srslte\common    667    4/10/2019    1](#nas_pcap.h%C2%A0%C2%A0%20%C2%A0lib%5Cinclude%5Csrslte%5Ccommon%C2%A0%C2%A0%20%C2%A0667%C2%A0%C2%A0%20%C2%A04%2F10%2F2019%C2%A0%C2%A0%20%C2%A01)
[nas_pcap.cc    lib\src\common    676    2/28/2019    20](#nas_pcap.cc%C2%A0%C2%A0%20%C2%A0lib%5Csrc%5Ccommon%C2%A0%C2%A0%20%C2%A0676%C2%A0%C2%A0%20%C2%A02%2F28%2F2019%C2%A0%C2%A0%20%C2%A020)
- NAS（Network Attached Storage：网络附属存储）
- pcap这个抓包库给抓包系统提供了一个高层次的接口。
### nas_pcap.h    lib\include\srslte\common    667    4/10/2019    1
```cpp
#ifndef SRSLTE_NAS_PCAP_H
#define SRSLTE_NAS_PCAP_H
#include "srslte/common/pcap.h"
namespace srslte {
//NAS（Network Attached Storage：网络附属存储）
//pcap这个抓包库给抓包系统提供了一个高层次的接口。
class nas_pcap 
{
public:
    nas_pcap() {enable_write=false; ue_id=0; pcap_file = NULL; }
    void enable();
    void open(const char *filename, uint32_t ue_id=0);
    void close();
    void write_nas(uint8_t *pdu, uint32_t pdu_len_bytes);
private:
    bool enable_write;
    FILE *pcap_file;
    uint32_t ue_id;
    void pack_and_write(uint8_t* pdu, uint32_t pdu_len_bytes);
};
} //namespace srslte
#endif // SRSLTE_NAS_PCAP_H
```
### nas_pcap.cc    lib\src\common    676    2/28/2019    20
```cpp
#include <stdint.h>
#include "srslte/srslte.h"
#include "srslte/common/pcap.h"
#include "srslte/common/nas_pcap.h"
namespace srslte {
void nas_pcap::enable()
{
  enable_write = true;
}
void nas_pcap::open(const char* filename, uint32_t ue_id)
{
  pcap_file = LTE_PCAP_Open(NAS_LTE_DLT, filename);
  ue_id = ue_id;
  enable_write = true;
}
void nas_pcap::close()
{
  fprintf(stdout, "Saving NAS PCAP file\n");
  LTE_PCAP_Close(pcap_file);
}
void nas_pcap::write_nas(uint8_t *pdu, uint32_t pdu_len_bytes)
{
    if (enable_write) {
      NAS_Context_Info_t  context;
      if (pdu) {
        LTE_PCAP_NAS_WritePDU(pcap_file, &context, pdu, pdu_len_bytes);
      }
    }
}
}
```
