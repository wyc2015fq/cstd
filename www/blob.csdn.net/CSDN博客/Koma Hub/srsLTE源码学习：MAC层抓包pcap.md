# srsLTE源码学习：MAC层抓包pcap - Koma Hub - CSDN博客
2019年04月08日 22:04:52[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：25
**Table of Contents**
[pcap.h    lib\include\srslte\common    13945    2/28/2019    182](#pcap.h%C2%A0%C2%A0%20%C2%A0lib%5Cinclude%5Csrslte%5Ccommon%C2%A0%C2%A0%20%C2%A013945%C2%A0%C2%A0%20%C2%A02%2F28%2F2019%C2%A0%C2%A0%20%C2%A0182)
[mac_pcap.h    lib\include\srslte\common    2564    4/8/2019    1](#mac_pcap.h%C2%A0%C2%A0%20%C2%A0lib%5Cinclude%5Csrslte%5Ccommon%C2%A0%C2%A0%20%C2%A02564%C2%A0%C2%A0%20%C2%A04%2F8%2F2019%C2%A0%C2%A0%20%C2%A01)
[mac_pcap.cc    lib\src\common    5971    4/8/2019    114](#mac_pcap.cc%C2%A0%C2%A0%20%C2%A0lib%5Csrc%5Ccommon%C2%A0%C2%A0%20%C2%A05971%C2%A0%C2%A0%20%C2%A04%2F8%2F2019%C2%A0%C2%A0%20%C2%A0114)
- pcap：这个抓包库给抓包系统提供了一个高层次的接口。所有网络上的数据包，甚至是那些发送给其他[主机](https://baike.baidu.com/item/%E4%B8%BB%E6%9C%BA/455151)的，通过这种机制，都是可以捕获的。它也支持把捕获的数据包保存为本地文件和从本地文件读取信息。
- PDU （协议数据单元）；
### pcap.h    lib\include\srslte\common    13945    2/28/2019    182
```cpp
/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2015 Software Radio Systems Limited
 *
 * \section LICENSE
 *
 * This file is part of the srsUE library.
 *
 * srsUE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsUE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */
#ifndef SRSLTE_PCAP_H
#define SRSLTE_PCAP_H
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#define MAC_LTE_DLT  147
#define NAS_LTE_DLT  148
#define RLC_LTE_DLT  149 // UDP needs to be selected as protocol
#define S1AP_LTE_DLT  150 
/* This structure gets written to the start of the file */
typedef struct pcap_hdr_s {
        unsigned int   magic_number;   /* magic number */
        unsigned short version_major;  /* major version number */
        unsigned short version_minor;  /* minor version number */
        unsigned int   thiszone;       /* GMT to local correction */
        unsigned int   sigfigs;        /* accuracy of timestamps */
        unsigned int   snaplen;        /* max length of captured packets, in octets */
        unsigned int   network;        /* data link type */
} pcap_hdr_t;
/* This structure precedes each packet */
typedef struct pcaprec_hdr_s {
        unsigned int   ts_sec;         /* timestamp seconds */
        unsigned int   ts_usec;        /* timestamp microseconds */
        unsigned int   incl_len;       /* number of octets of packet saved in file */
        unsigned int   orig_len;       /* actual length of packet */
} pcaprec_hdr_t;
/* radioType */
#define FDD_RADIO 1
#define TDD_RADIO 2
/* Direction */
#define DIRECTION_UPLINK   0
#define DIRECTION_DOWNLINK 1
/* rntiType */
#define NO_RNTI  0  /* Used for BCH-BCH */
#define P_RNTI   1
#define RA_RNTI  2
#define C_RNTI   3
#define SI_RNTI  4
#define SPS_RNTI 5
#define M_RNTI   6
#define MAC_LTE_START_STRING        "mac-lte"
#define MAC_LTE_PAYLOAD_TAG         0x01
#define MAC_LTE_RNTI_TAG            0x02
#define MAC_LTE_UEID_TAG            0x03
#define MAC_LTE_FRAME_SUBFRAME_TAG  0x04
#define MAC_LTE_PREDFINED_DATA_TAG  0x05
#define MAC_LTE_RETX_TAG            0x06
#define MAC_LTE_CRC_STATUS_TAG      0x07
/* Context information for every MAC PDU that will be logged */
typedef struct MAC_Context_Info_t {
    unsigned short radioType;
    unsigned char  direction;
    unsigned char  rntiType;
    unsigned short rnti;
    unsigned short ueid;
    unsigned char  isRetx;
    unsigned char  crcStatusOK;
    unsigned short sysFrameNumber;
    unsigned short subFrameNumber;
} MAC_Context_Info_t;
/* Context information for every NAS PDU that will be logged */
typedef struct NAS_Context_Info_s {
  // No Context yet
} NAS_Context_Info_t;
/* RLC-LTE disector */
/* rlcMode */
#define RLC_TM_MODE 1
#define RLC_UM_MODE 2
#define RLC_AM_MODE 4
#define RLC_PREDEF  8
/* priority ? */
/* channelType */
#define CHANNEL_TYPE_CCCH 1
#define CHANNEL_TYPE_BCCH_BCH 2
#define CHANNEL_TYPE_PCCH 3
#define CHANNEL_TYPE_SRB 4
#define CHANNEL_TYPE_DRB 5
#define CHANNEL_TYPE_BCCH_DL_SCH 6
#define CHANNEL_TYPE_MCCH 7
#define CHANNEL_TYPE_MTCH 8
/* sequenceNumberLength */
#define UM_SN_LENGTH_5_BITS 5
#define UM_SN_LENGTH_10_BITS 10
#define AM_SN_LENGTH_10_BITS 10
#define AM_SN_LENGTH_16_BITS 16
/* Narrow band mode */
typedef enum {
  rlc_no_nb_mode = 0,
  rlc_nb_mode = 1
} rlc_lte_nb_mode;
/* Context information for every RLC PDU that will be logged */
typedef struct {
  unsigned char   rlcMode;
  unsigned char   direction;
  unsigned char   priority;
  unsigned char   sequenceNumberLength;
  unsigned short  ueid;
  unsigned short  channelType;
  unsigned short  channelId; /* for SRB: 1=SRB1, 2=SRB2, 3=SRB1bis; for DRB: DRB ID */
  unsigned short  pduLength;
  bool            extendedLiField;
  rlc_lte_nb_mode nbMode;
} RLC_Context_Info_t;
// See Wireshark's packet-rlc-lte.h for details
#define RLC_LTE_START_STRING "rlc-lte"
#define RLC_LTE_SN_LENGTH_TAG    0x02
#define RLC_LTE_DIRECTION_TAG    0x03
#define RLC_LTE_PRIORITY_TAG     0x04
#define RLC_LTE_UEID_TAG         0x05
#define RLC_LTE_CHANNEL_TYPE_TAG 0x06
#define RLC_LTE_CHANNEL_ID_TAG   0x07
#define RLC_LTE_EXT_LI_FIELD_TAG 0x08
#define RLC_LTE_NB_MODE_TAG      0x09
#define RLC_LTE_PAYLOAD_TAG      0x01
/* Context information for every S1AP PDU that will be logged */
typedef struct S1AP_Context_Info_s {
  // No Context yet
} S1AP_Context_Info_t;
/**************************************************************************
 * API functions for opening/closing LTE PCAP files                       *
 **************************************************************************/
/* Open the file and write file header */
inline FILE *LTE_PCAP_Open(uint32_t DLT, const char *fileName)
{
    pcap_hdr_t file_header =
    {
        0xa1b2c3d4,   /* magic number */
        2, 4,         /* version number is 2.4 */
        0,            /* timezone */
        0,            /* sigfigs - apparently all tools do this */
        65535,        /* snaplen - this should be long enough */
        DLT           /* Data Link Type (DLT).  Set as unused value 147 for now */
    };
    FILE *fd = fopen(fileName, "w");
    if (fd == NULL) {
        printf("Failed to open file \"%s\" for writing\n", fileName);
        return NULL;
    }
    /* Write the file header */
    fwrite(&file_header, sizeof(pcap_hdr_t), 1, fd);
    return fd;
}
/* Close the PCAP file */
inline void LTE_PCAP_Close(FILE *fd)
{
  if(fd)
    fclose(fd);
}
/**************************************************************************
 * API functions for writing MAC-LTE PCAP files                           *
 **************************************************************************/
/* Write an individual PDU (PCAP packet header + mac-context + mac-pdu) */
inline int LTE_PCAP_MAC_WritePDU(FILE *fd, MAC_Context_Info_t *context,
                                 const unsigned char *PDU, unsigned int length)
{
    pcaprec_hdr_t packet_header;
    char context_header[256];
    int offset = 0;
    uint16_t tmp16;
    /* Can't write if file wasn't successfully opened */
    if (fd == NULL) {
        printf("Error: Can't write to empty file handle\n");
        return 0;
    }
    /*****************************************************************/
    /* Context information (same as written by UDP heuristic clients */
    context_header[offset++] = context->radioType;
    context_header[offset++] = context->direction;
    context_header[offset++] = context->rntiType;
    /* RNTI */
    context_header[offset++] = MAC_LTE_RNTI_TAG;
    tmp16 = htons(context->rnti);
    memcpy(context_header+offset, &tmp16, 2);
    offset += 2;
    /* UEId */
    context_header[offset++] = MAC_LTE_UEID_TAG;
    tmp16 = htons(context->ueid);
    memcpy(context_header+offset, &tmp16, 2);
    offset += 2;
    /* Subframe Number and System Frame Number */
    /* SFN is stored in 12 MSB and SF in 4 LSB */
    context_header[offset++] = MAC_LTE_FRAME_SUBFRAME_TAG;
    tmp16 = (context->sysFrameNumber << 4) | context->subFrameNumber;
    tmp16 = htons(tmp16);
    memcpy(context_header+offset, &tmp16, 2);
    offset += 2;
    /* CRC Status */
    context_header[offset++] = MAC_LTE_CRC_STATUS_TAG;
    context_header[offset++] = context->crcStatusOK;
    /* Data tag immediately preceding PDU */
    context_header[offset++] = MAC_LTE_PAYLOAD_TAG;
    /****************************************************************/
    /* PCAP Header                                                  */
    struct timeval t;
    gettimeofday(&t, NULL);
    packet_header.ts_sec = t.tv_sec;
    packet_header.ts_usec = t.tv_usec;
    packet_header.incl_len = offset + length;
    packet_header.orig_len = offset + length;
    /***************************************************************/
    /* Now write everything to the file                            */
    fwrite(&packet_header, sizeof(pcaprec_hdr_t), 1, fd);
    fwrite(context_header, 1, offset, fd);
    fwrite(PDU, 1, length, fd);
    return 1;
}
/**************************************************************************
 * API functions for writing NAS-EPS PCAP files                           *
 **************************************************************************/
/* Write an individual PDU (PCAP packet header + nas-context + nas-pdu) */
inline int LTE_PCAP_NAS_WritePDU(FILE *fd, NAS_Context_Info_t *context,
                                 const unsigned char *PDU, unsigned int length)
{
    pcaprec_hdr_t packet_header;
    /* Can't write if file wasn't successfully opened */
    if (fd == NULL) {
        printf("Error: Can't write to empty file handle\n");
        return 0;
    }
    
    /****************************************************************/
    /* PCAP Header                                                  */
    struct timeval t;
    gettimeofday(&t, NULL);
    packet_header.ts_sec = t.tv_sec;
    packet_header.ts_usec = t.tv_usec;
    packet_header.incl_len = length;
    packet_header.orig_len = length;
    /***************************************************************/
    /* Now write everything to the file                            */
    fwrite(&packet_header, sizeof(pcaprec_hdr_t), 1, fd);
    fwrite(PDU, 1, length, fd);
    return 1;
}
/**************************************************************************
 * API functions for writing RLC-LTE PCAP files                           *
 **************************************************************************/
/* Write an individual RLC PDU (PCAP packet header + UDP header + rlc-context + rlc-pdu) */
inline int LTE_PCAP_RLC_WritePDU(FILE *fd, RLC_Context_Info_t *context,
                                 const unsigned char *PDU, unsigned int length)
{
    pcaprec_hdr_t packet_header;
    char context_header[256];
    int offset = 0;
    uint16_t tmp16;
    /* Can't write if file wasn't successfully opened */
    if (fd == NULL) {
        printf("Error: Can't write to empty file handle\n");
        return 0;
    }
    /*****************************************************************/
    // Add dummy UDP header, start with src and dest port
    context_header[offset++] = 0xde;
    context_header[offset++] = 0xad;
    context_header[offset++] = 0xbe;
    context_header[offset++] = 0xef;
    // length
    tmp16 = length + 12;
    memcpy(context_header+offset, &tmp16, 2);
    offset += 2;
    // dummy CRC
    context_header[offset++] = 0xde;
    context_header[offset++] = 0xad;
    // Start magic string
    memcpy(&context_header[offset], RLC_LTE_START_STRING, strlen(RLC_LTE_START_STRING));
    offset += strlen(RLC_LTE_START_STRING);
    // Fixed field RLC mode
    context_header[offset++] = context->rlcMode;
    // Conditional fields
    if (context->rlcMode == RLC_UM_MODE) {
        context_header[offset++] = RLC_LTE_SN_LENGTH_TAG;
        context_header[offset++] = context->sequenceNumberLength;
    }
    // Optional fields
    context_header[offset++] = RLC_LTE_DIRECTION_TAG;
    context_header[offset++] = context->direction;
    context_header[offset++] = RLC_LTE_PRIORITY_TAG;
    context_header[offset++] = context->priority;
    context_header[offset++] = RLC_LTE_UEID_TAG;
    tmp16 = htons(context->ueid);
    memcpy(context_header+offset, &tmp16, 2);
    offset += 2;
    context_header[offset++] = RLC_LTE_CHANNEL_TYPE_TAG;
    tmp16 = htons(context->channelType);
    memcpy(context_header+offset, &tmp16, 2);
    offset += 2;
    context_header[offset++] = RLC_LTE_CHANNEL_ID_TAG;
    tmp16 = htons(context->channelId);
    memcpy(context_header+offset, &tmp16, 2);
    offset += 2;
    // Now the actual PDU
    context_header[offset++] = RLC_LTE_PAYLOAD_TAG;
    // PCAP header
    struct timeval t;
    gettimeofday(&t, NULL);
    packet_header.ts_sec = t.tv_sec;
    packet_header.ts_usec = t.tv_usec;
    packet_header.incl_len = offset + length;
    packet_header.orig_len = offset + length;
    // Write everything to file
    fwrite(&packet_header, sizeof(pcaprec_hdr_t), 1, fd);
    fwrite(context_header, 1, offset, fd);
    fwrite(PDU, 1, length, fd);
    return 1;
}
/**************************************************************************
 * API functions for writing S1AP PCAP files                           *
 **************************************************************************/
/* Write an individual PDU (PCAP packet header + s1ap-context + s1ap-pdu) */
inline int LTE_PCAP_S1AP_WritePDU(FILE *fd, S1AP_Context_Info_t *context,
                                 const unsigned char *PDU, unsigned int length)
{
    pcaprec_hdr_t packet_header;
    /* Can't write if file wasn't successfully opened */
    if (fd == NULL) {
        printf("Error: Can't write to empty file handle\n");
        return 0;
    }
    /****************************************************************/
    /* PCAP Header                                                  */
    struct timeval t;
    gettimeofday(&t, NULL);
    packet_header.ts_sec = t.tv_sec;
    packet_header.ts_usec = t.tv_usec;
    packet_header.incl_len = length;
    packet_header.orig_len = length;
    /***************************************************************/
    /* Now write everything to the file                            */
    fwrite(&packet_header, sizeof(pcaprec_hdr_t), 1, fd);
    fwrite(PDU, 1, length, fd);
    return 1;
}
#endif // SRSLTE_PCAP_H
```
### mac_pcap.h    lib\include\srslte\common    2564    4/8/2019    1
```cpp
/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2015 Software Radio Systems Limited
 *
 * \section LICENSE
 *
 * This file is part of the srsUE library.
 *
 * srsUE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsUE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */
#ifndef SRSLTE_MAC_PCAP_H
#define SRSLTE_MAC_PCAP_H
#include <stdint.h>
#include "srslte/common/pcap.h"
namespace srslte {
//PCAP 抓包
//这个抓包库给抓包系统提供了一个高层次的接口。
//所有网络上的数据包，甚至是那些发送给其他主机的，通过这种机制，都是可以捕获的。
//它也支持把捕获的数据包保存为本地文件和从本地文件读取信息。
class mac_pcap
{
public:
  mac_pcap() {enable_write=false; ue_id=0; pcap_file = NULL; };
  void enable(bool en);
  void open(const char *filename, uint32_t ue_id = 0);
  void close();
  void set_ue_id(uint16_t ue_id);
  void write_ul_crnti(uint8_t *pdu, uint32_t pdu_len_bytes, uint16_t crnti, uint32_t reTX, uint32_t tti);
  void write_dl_crnti(uint8_t *pdu, uint32_t pdu_len_bytes, uint16_t crnti, bool crc_ok, uint32_t tti);
  void write_dl_ranti(uint8_t *pdu, uint32_t pdu_len_bytes, uint16_t ranti, bool crc_ok, uint32_t tti);
  // SI and BCH only for DL
  void write_dl_sirnti(uint8_t *pdu, uint32_t pdu_len_bytes, bool crc_ok, uint32_t tti);
  void write_dl_bch(uint8_t *pdu, uint32_t pdu_len_bytes, bool crc_ok, uint32_t tti);
  void write_dl_pch(uint8_t *pdu, uint32_t pdu_len_bytes, bool crc_ok, uint32_t tti);
  void write_dl_mch(uint8_t *pdu, uint32_t pdu_len_bytes, bool crc_ok, uint32_t tti);
  void write_ul_rrc_pdu(const uint8_t* input, const int32_t input_len);
private:
  bool     enable_write;
  FILE*    pcap_file;
  uint32_t ue_id;
  void pack_and_write(uint8_t* pdu, uint32_t pdu_len_bytes, uint32_t reTX, bool crc_ok, uint32_t tti,
                              uint16_t crnti_, uint8_t direction, uint8_t rnti_type);
};
} // namespace srslte
#endif // SRSLTE_MAC_PCAP_H
```
### mac_pcap.cc    lib\src\common    5971    4/8/2019    114
```cpp
/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2015 Software Radio Systems Limited
 *
 * \section LICENSE
 *
 * This file is part of the srsUE library.
 *
 * srsUE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsUE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */
#include <stdint.h>
#include "srslte/srslte.h"
#include "srslte/common/pcap.h"
#include "srslte/common/mac_pcap.h"
namespace srslte {
 
void mac_pcap::enable(bool en)
{
  enable_write = true; 
}
void mac_pcap::open(const char* filename, uint32_t ue_id)
{
  pcap_file = LTE_PCAP_Open(MAC_LTE_DLT, filename); ///Data Link Terminal 数据链路终端;
  this->ue_id = ue_id;
  enable_write = true;
}
void mac_pcap::close()
{
  fprintf(stdout, "Saving MAC PCAP file\n");
  LTE_PCAP_Close(pcap_file);
}
void mac_pcap::set_ue_id(uint16_t ue_id) {
  this->ue_id = ue_id;
}
void mac_pcap::pack_and_write(uint8_t* pdu, uint32_t pdu_len_bytes, uint32_t reTX, bool crc_ok, uint32_t tti, 
                              uint16_t crnti, uint8_t direction, uint8_t rnti_type)
{
  if (enable_write) {
    MAC_Context_Info_t  context =
    {
        FDD_RADIO, direction, rnti_type,
        crnti,       /* RNTI */
        (uint16_t)ue_id,      /* UEId */
        (uint8_t)reTX,        /* Retx */
        crc_ok,        /* CRC Stsatus (i.e. OK) */
        (uint16_t)(tti/10),        /* Sysframe number */
        (uint16_t)(tti%10)        /* Subframe number */
    };
    if (pdu) {
      LTE_PCAP_MAC_WritePDU(pcap_file, &context, pdu, pdu_len_bytes);
    }
  }
}
void mac_pcap::write_dl_crnti(uint8_t* pdu, uint32_t pdu_len_bytes, uint16_t rnti, bool crc_ok, uint32_t tti)
{
  pack_and_write(pdu, pdu_len_bytes, 0, crc_ok, tti, rnti, DIRECTION_DOWNLINK, C_RNTI);
}
void mac_pcap::write_dl_ranti(uint8_t* pdu, uint32_t pdu_len_bytes, uint16_t rnti, bool crc_ok, uint32_t tti)
{
  pack_and_write(pdu, pdu_len_bytes, 0, crc_ok, tti, rnti, DIRECTION_DOWNLINK, RA_RNTI);
}
void mac_pcap::write_ul_crnti(uint8_t* pdu, uint32_t pdu_len_bytes, uint16_t rnti, uint32_t reTX, uint32_t tti)
{
  pack_and_write(pdu, pdu_len_bytes, reTX, true, tti, rnti, DIRECTION_UPLINK, C_RNTI);
}
void mac_pcap::write_dl_bch(uint8_t* pdu, uint32_t pdu_len_bytes, bool crc_ok, uint32_t tti)
{
  pack_and_write(pdu, pdu_len_bytes, 0, crc_ok, tti, 0, DIRECTION_DOWNLINK, NO_RNTI);
}
void mac_pcap::write_dl_pch(uint8_t* pdu, uint32_t pdu_len_bytes, bool crc_ok, uint32_t tti)
{
  pack_and_write(pdu, pdu_len_bytes, 0, crc_ok, tti, SRSLTE_PRNTI, DIRECTION_DOWNLINK, P_RNTI);
}
void mac_pcap::write_dl_mch(uint8_t* pdu, uint32_t pdu_len_bytes, bool crc_ok, uint32_t tti)
{
  pack_and_write(pdu, pdu_len_bytes, 0, crc_ok, tti, SRSLTE_MRNTI, DIRECTION_DOWNLINK, M_RNTI);
}
void mac_pcap::write_dl_sirnti(uint8_t* pdu, uint32_t pdu_len_bytes, bool crc_ok, uint32_t tti)
{
  pack_and_write(pdu, pdu_len_bytes, 0, crc_ok, tti, SRSLTE_SIRNTI, DIRECTION_DOWNLINK, SI_RNTI);
}
//RRC（Radio Resource Control）是指无线资源控制。
void mac_pcap::write_ul_rrc_pdu(const uint8_t* input, const int32_t input_len)
{
  uint8_t pdu[1024];
  bzero(pdu, sizeof(pdu));
  // Size is limited by PDU buffer and MAC subheader (format 1 < 128 B)
  if (input_len > 128 - 7) {
    printf("PDU too large.\n");
    return;
  }
  //  MAC PDU Header (Short BSR) (1:54) (Padding:remainder)  [3 subheaders]
  //  Sub-header (lcid=Short BSR)
  //      0... .... = SCH reserved bit: 0x0
  //      .0.. .... = Format2: Data length is < 32768 bytes
  //      ..1. .... = Extension: 0x1
  //      ...1 1101 = LCID: Short BSR (0x1d)
  //  Sub-header (lcid=1, length=54)
  //      0... .... = SCH reserved bit: 0x0
  //      .0.. .... = Format2: Data length is < 32768 bytes
  //      ..1. .... = Extension: 0x1
  //      ...0 0001 = LCID: 1 (0x01)
  //      0... .... = Format: Data length is < 128 bytes
  //      .011 0110 = Length: 54 (Will be dynamically updated)
  //  Sub-header (lcid=Padding, length is remainder)
  //      0... .... = SCH reserved bit: 0x0
  //      .0.. .... = Format2: Data length is < 32768 bytes
  //      ..0. .... = Extension: 0x0
  //      ...1 1111 = LCID: Padding (0x1f)
  uint8_t mac_hdr[] = {0x3D, 0x21, 0x36, 0x1F, 0x0C};
  // Update MAC length
  mac_hdr[2] = input_len + 7; // rlc_hdr (2) + pdcp_hdr (1) + MAC (4)
  //  AM Header  (P) sn=4
  //  1... .... = Frame type: Data PDU (0x1)
  //  .0.. .... = Re-segmentation Flag: AMD PDU (0x0)
  //  ..1. .... = Polling Bit: Status report is requested (0x1)
  //  ...0 0... = Framing Info: First byte begins a RLC SDU and last byte ends a RLC SDU (0x0)
  //  .... .0.. = Extension: Data field follows from the octet following the fixed part of the header (0x0)
  //  .... ..00 0000 0100 = Sequence Number: 4
  uint8_t rlc_hdr[] = {0xA0, 0x04};
  //  PDCP-LTE sn=3
  //  000. .... = Reserved: 0
  //  ...0 0011 = Seq Num: 3
  uint8_t pdcp_hdr[] = {0x03};
  uint8_t* pdu_ptr = pdu;
  memcpy(pdu_ptr, mac_hdr, sizeof(mac_hdr));
  pdu_ptr += sizeof(mac_hdr);
  memcpy(pdu_ptr, rlc_hdr, sizeof(rlc_hdr));
  pdu_ptr += sizeof(rlc_hdr);
  memcpy(pdu_ptr, pdcp_hdr, sizeof(pdcp_hdr));
  pdu_ptr += sizeof(pdcp_hdr);
  memcpy(pdu_ptr, input, input_len);
  pdu_ptr += input_len;
  // MAC
  uint8_t pad = 0x00;
  for (uint32_t i = 0; i < 4; i++) {
    memcpy(pdu_ptr, &pad, 1);
    pdu_ptr += 1;
  }
  // Pad
  memcpy(pdu_ptr, &pad, 1);
  pdu_ptr += 1;
  write_ul_crnti(pdu, pdu_ptr - pdu, 14931, true, 0);
}
}
```
