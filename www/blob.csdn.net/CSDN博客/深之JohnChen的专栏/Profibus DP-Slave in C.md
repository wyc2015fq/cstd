# Profibus DP-Slave in C - 深之JohnChen的专栏 - CSDN博客

2017年10月08日 15:59:42[byxdaz](https://me.csdn.net/byxdaz)阅读数：1291


转载https://www.mikrocontroller.net/topic/106174

//profibus.h

```cpp
/*!
 * \file    profibus.h
 * \brief   Ablaufsteuerung Profibus DP-Slave Kommunikation, h-Datei
 * \author  © Joerg S.
 * \date    9.2007 (Erstellung) 7.2008 (Aktueller Stand)
 * \note    Verwendung nur fuer private Zwecke / Only for non-commercial use
 */

#ifndef PROFIBUS_H
#define PROFIBUS_H 1

///////////////////////////////////////////////////////////////////////////////////////////////////
// Ident Nummer DP Slave
///////////////////////////////////////////////////////////////////////////////////////////////////
#define IDENT_HIGH_BYTE       0x80
#define IDENT_LOW_BYTE        0x70
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// Adressen
///////////////////////////////////////////////////////////////////////////////////////////////////
//#define MASTER_ADD            0x02  // SPS Adresse
#define BROADCAST_ADD         0x7F
#define SAP_OFFSET            0x80  // Service Access Point Adress Offset
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// Kommandotypen
///////////////////////////////////////////////////////////////////////////////////////////////////
#define SD1                   0x10  // Telegramm ohne Datenfeld
#define SD2                   0x68  // Daten Telegramm variabel
#define SD3                   0xA2  // Daten Telegramm fest
#define SD4                   0xDC  // Token
#define SC                    0xE5  // Kurzquittung
#define ED                    0x16  // Ende
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function Codes
///////////////////////////////////////////////////////////////////////////////////////////////////
#define REQ_FDL_STATUS        0x49  // SPS: Status Abfrage
#define FDL_STATUS_OK         0x00  // SLA: OK
//#define SEND_DATA
#define SEND_REQ_DATA         0x5D  // SPS: Ausgaenge setzen, Eingaenge lesen
#define SEND_REQ_DATA_T       0x7D  // SPS: Ausgaenge setzen, Eingaenge lesen (Toggle)
#define SEND_REQ_DATA_FIRST   0x6D  // SPS: Erste Anfrage
#define SLAVE_DATA            0x08  // SLA: Daten Eingaenge senden
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// Service Access Points (DP Slave) MS0
///////////////////////////////////////////////////////////////////////////////////////////////////
#define SAP_SET_SLAVE_ADR     55  // Master setzt Slave Adresse, Slave Anwortet mit SC
#define SAP_RD_INP            56  // Master fordert Input Daten, Slave sendet Input Daten
#define SAP_RD_OUTP           57  // Master fordert Output Daten, Slave sendet Output Daten
#define SAP_GLOBAL_CONTROL    58  // Master Control, Slave Antwortet nicht
#define SAP_GET_CFG           59  // Master fordert Konfig., Slave sendet Konfiguration
#define SAP_SLAVE_DIAGNOSIS   60  // Master fordert Diagnose, Slave sendet Diagnose Daten
#define SAP_SET_PRM           61  // Master sendet Parameter, Slave sendet SC
#define SAP_CHK_CFG           62  // Master sendet Konfuguration, Slave sendet SC
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// SAP: Global Control (Daten Master)
///////////////////////////////////////////////////////////////////////////////////////////////////
#define CLEAR_DATA_           0x02
#define UNFREEZE_             0x04
#define FREEZE_               0x08
#define UNSYNC_               0x10
#define SYNC_                 0x20
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// SAP: Diagnose (Antwort Slave)
///////////////////////////////////////////////////////////////////////////////////////////////////
/* Status Byte 1 */
#define STATION_NOT_EXISTENT_ 0x01
#define STATION_NOT_READY_    0x02
#define CFG_FAULT_            0x04
#define EXT_DIAG_             0x08  // Erweiterte Diagnose vorhanden
#define NOT_SUPPORTED_        0x10
#define INV_SLAVE_RESPONSE_   0x20
#define PRM_FAULT_            0x40
#define MASTER_LOCK           0x80

/* Status Byte 2 */
#define STATUS_2_DEFAULT      0x04
#define PRM_REQ_              0x01
#define STAT_DIAG_            0x02
#define WD_ON_                0x08
#define FREEZE_MODE_          0x10
#define SYNC_MODE_            0x20
//#define free                  0x40
#define DEACTIVATED_          0x80

/* Status Byte 3 */
#define DIAG_SIZE_OK          0x00
#define DIAG_SIZE_ERROR       0x80

/* Adresse */
#define MASTER_ADD_DEFAULT    0xFF

/* Erweiterte Diagnose (EXT_DIAG_ = 1) */
#define EXT_DIAG_TYPE_        0xC0  // Bit 6-7 ist Diagnose Typ
#define EXT_DIAG_GERAET       0x00  // Wenn Bit 7 und 6 = 00, dann Geraetebezogen
#define EXT_DIAG_KENNUNG      0x40  // Wenn Bit 7 und 6 = 01, dann Kennungsbezogen
#define EXT_DIAG_KANAL        0x80  // Wenn Bit 7 und 6 = 10, dann Kanalbezogen

#define EXT_DIAG_BYTE_CNT_    0x3F  // Bit 0-5 sind Anzahl der Diagnose Bytes
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// SAP: Set Parameters Request (Daten Master)
///////////////////////////////////////////////////////////////////////////////////////////////////
//#define
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// SAP: Check Config Request (Daten Master)
///////////////////////////////////////////////////////////////////////////////////////////////////
#define CFG_DIRECTION_        0x30  // Bit 4-5 ist Richtung. 01 =  Eingang, 10 = Ausgang, 11 = Eingang/Ausgang
#define CFG_INPUT             0x10  // Eingang
#define CFG_OUTPUT            0x20  // Ausgang
#define CFG_INPUT_OUTPUT      0x30  // Eingang/Ausgang
#define CFG_SPECIAL           0x00  // Spezielles Format wenn mehr als 16/32Byte uebertragen werden sollen

#define CFG_KONSISTENZ_       0x80  // Bit 7 ist Konsistenz. 0 = Byte oder Wort, 1 = Ueber gesamtes Modul
#define CFG_KONS_BYTE_WORT    0x00  // Byte oder Wort
#define CFG_KONS_MODUL        0x80  // Modul

#define CFG_WIDTH_            0x40  // Bit 6 ist IO Breite. 0 = Byte (8bit), 1 = Wort (16bit)
#define CFG_BYTE              0x00  // Byte
#define CFG_WORD              0x40  // Wort

/* Kompaktes Format */
#define CFG_BYTE_CNT_         0x0F  // Bit 0-3 sind Anzahl der Bytes oder Worte. 0 = 1 Byte, 1 = 2 Byte usw.

/* Spezielles Format */
#define CFG_SP_DIRECTION_     0xC0  // Bit 6-7 ist Richtung. 01 =  Eingang, 10 = Ausgang, 11 = Eingang/Ausgang
#define CFG_SP_VOID           0x00  // Leerplatz
#define CFG_SP_INPUT          0x40  // Eingang
#define CFG_SP_OUTPUT         0x80  // Ausgang
#define CFG_SP_INPUT_OPTPUT   0xC0  // Eingang/Ausgang

#define CFG_SP_VENDOR_CNT_    0x0F  // Bit 0-3 sind Anzahl der herstellerspezifischen Bytes. 0 = keine

/* Spezielles Format / Laengenbyte */
#define CFG_SP_BYTE_CNT_      0x3F  // Bit 0-5 sind Anzahl der Bytes oder Worte. 0 = 1 Byte, 1 = 2 Byte usw.
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
#define TIMEOUT_MAX_SYN_TIME  33 * DELAY_TBIT // 33 TBit = TSYN
#define TIMEOUT_MAX_RX_TIME   15 * DELAY_TBIT
#define TIMEOUT_MAX_TX_TIME   15 * DELAY_TBIT
#define TIMEOUT_MAX_SDR_TIME  15 * DELAY_TBIT // 15 Tbit = TSDR

#define TA_SMCLK_500KHZ_INT   TACTL = MC1 + TASSEL1 + ID_3;
//#define DELAY_TIMER_1_7MS   0x035C  // 33 TB (UART @ 19200)
//#define DELAY_TIMER_782US   0x0187  // 15 TB (UART @ 19200)
//#define DELAY_TBIT          26.04   // UART @ 19200

//#define DELAY_TIMER_1_7MS   0x00B1  // 33 TB (UART @ 93750)
//#define DELAY_TIMER_782US   0x0051  // 15 TB (UART @ 93750)
#define DELAY_TBIT            5.33    // UART @ 93750
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_BUFFER_SIZE       45

#define INPUT_DATA_SIZE       35    // Anzahl Bytes die vom Master kommen
#define OUTPUT_DATA_SIZE      35    // Anzahl Bytes die an Master gehen
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// Profibus Ablaufsteuerung
///////////////////////////////////////////////////////////////////////////////////////////////////
#define PROFIBUS_WAIT_SYN     1
#define PROFIBUS_WAIT_DATA    2
#define PROFIBUS_GET_DATA     3
#define PROFIBUS_SEND_DATA    4
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
void          init_Profibus               (void);
void          profibus_RX                 (void);
void          profibus_send_CMD           (unsigned char type,
                                           unsigned char function_code,
                                           unsigned char sap_offset,
                                           char *pdu,
                                           unsigned char length_pdu);
void          profibus_TX                 (char *data, unsigned char length);

unsigned char checksum                    (char *data, unsigned char length);
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif
```

//profibus.c

```cpp
/*!
 * \file    profibus.c
 * \brief   Ablaufsteuerung Profibus DP-Slave Kommunikation
 * \author  � Joerg S.
 * \date    9.2007 (Erstellung) 7.2008 (Aktueller Stand)
 * \note    Verwendung nur fuer private Zwecke / Only for non-commercial use
 */

#include "treiber_import.h"

char uart_buffer[MAX_BUFFER_SIZE];
unsigned int uart_byte_cnt = 0;
unsigned int uart_transmit_cnt = 0;

// Profibus Flags und Variablen
unsigned char profibus_status;
unsigned char slave_addr;
unsigned char master_addr;
unsigned char group;

unsigned char data_out_register[OUTPUT_DATA_SIZE];
unsigned char data_in_register [INPUT_DATA_SIZE];
unsigned char Input_Data_size;
unsigned char Output_Data_size;

#define TX_IRQ

///////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * \brief Profibus Timer und Variablen initialisieren
 */
void init_Profibus (void)
{
  unsigned char cnt;

  // Variablen initialisieren
  master_addr = 0x02;
  Input_Data_size = 0;
  Output_Data_size = 0;
  group = 0;

  // Slave Adresse aus FLASH holen
  slave_addr = para_mem_read_byte(EE_BUS_ADDR);

  // Register loeschen
  for (cnt = 0; cnt < OUTPUT_DATA_SIZE; cnt++)
  {
    data_out_register[cnt] = 0xFF;
  }
  for (cnt = 0; cnt < INPUT_DATA_SIZE; cnt++)
  {
    data_in_register[cnt] = 0x00;
  }

  profibus_status = PROFIBUS_WAIT_SYN;

  // Timer init
  TAR = 0;
  TACCR0 = TIMEOUT_MAX_SYN_TIME;
  TA_SMCLK_500KHZ_INT;
  TACCTL0 = CCIE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * \brief ISR UART0 Receive
 */
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR (void)
{

  // Nur einlesen wenn TSYN abgelaufen ist, oder schon Daten reinkommen
  if (profibus_status == PROFIBUS_GET_DATA || profibus_status == PROFIBUS_WAIT_DATA)
  {
    // TSYN abgelaufen, Daten einlesen

    uart_buffer[uart_byte_cnt++] = UCA0RXBUF;

    profibus_status = PROFIBUS_GET_DATA;
  }
  else
  {
    // Daten ignorieren

    IFG2 &=~UCA0RXIFG;  // IFG zuruecksetzen
  }

  // Profibus Timer ruecksetzen
  TAR = 0;

}
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * \brief Profibus auswertung
 */
void profibus_RX (void)
{
  unsigned char cnt;
  unsigned char telegramm_type;
  unsigned char process_data;

  // Profibus Datentypen
  unsigned char destination_add;
  unsigned char source_add;
  unsigned char function_code;
  unsigned char FCS_data;   // Frame Check Sequence
  unsigned char PDU_size;   // PDU Groesse
  unsigned char DSAP_data;  // SAP Destination
  unsigned char SSAP_data;  // SAP Source

  process_data = FALSE;

  telegramm_type = uart_buffer[0];

  switch (telegramm_type)
  {
    case SD1: // Telegramm ohne Daten, max. 6 Byte

        if (uart_byte_cnt != 6) break;

        destination_add = uart_buffer[1];
        source_add      = uart_buffer[2];
        function_code   = uart_buffer[3];
        FCS_data        = uart_buffer[4];

        if (checksum(&uart_buffer[1], 3) != FCS_data) break;

        process_data = TRUE;

        break;

    case SD2: // Telegramm mit variabler Datenlaenge

        if (uart_byte_cnt != uart_buffer[1] + 6) break;

        PDU_size        = uart_buffer[1];
        destination_add = uart_buffer[4];
        source_add      = uart_buffer[5];
        function_code   = uart_buffer[6];
        FCS_data        = uart_buffer[PDU_size + 4];

        if (checksum(&uart_buffer[4], PDU_size) != FCS_data) break;

        process_data = TRUE;

        break;

    case SD3: // Telegramm mit 5 Byte Daten, max. 11 Byte

        if (uart_byte_cnt != 11) break;

        destination_add = uart_buffer[1];
        source_add      = uart_buffer[2];
        function_code   = uart_buffer[3];
        FCS_data        = uart_buffer[9];

        if (checksum(&uart_buffer[1], 8) != FCS_data) break;

        process_data = TRUE;

        break;

    case SD4: // Token 3 Byte

        if (uart_byte_cnt != 3) break;

        destination_add = uart_buffer[1];
        source_add      = uart_buffer[2];

        break;

    default:

        break;

  } // Switch Ende

  // Nur Daten f黵 eigene Adresse oder Broadcast auswerten
  if ((destination_add != slave_addr) &&                // Slave
      (destination_add != slave_addr + SAP_OFFSET) &&   // SAP Slave
      (destination_add != BROADCAST_ADD) &&             // Broadcast
      (destination_add != BROADCAST_ADD + SAP_OFFSET))  // SAP Broadcast
    process_data = FALSE;

  // Nur auswerten wenn Daten OK sind
  if (process_data == TRUE)
  {
    master_addr = source_add; // Master Adresse ist Source Adresse

    // Service Access Point erkannt?
    if ((destination_add & 0x80) && (source_add & 0x80))
    {
      DSAP_data = uart_buffer[7];
      SSAP_data = uart_buffer[8];

      // Ablauf Reboot:
      // 1) SSAP 62 -> DSAP 60 (Get Diagnostics Request)
      // 2) SSAP 62 -> DSAP 61 (Set Parameters Request)
      // 3) SSAP 62 -> DSAP 62 (Check Config Request)
      // 4) SSAP 62 -> DSAP 60 (Get Diagnostics Request)
      // 5) Data Exchange Request (normaler Zyklus)

      switch (DSAP_data)
      {
        case SAP_SET_SLAVE_ADR: // Set Slave Address (SSAP 62 -> DSAP 55)

            // Adressaenderung siehe Seite 93

            //new_addr = uart_buffer[9];
            //IDENT_HIGH_BYTE = uart_buffer[10];
            //IDENT_LOW_BYTE = uart_buffer[11];
            //if (uart_buffer[12] & 0x01) adress_aenderung_sperren = TRUE;

            profibus_send_CMD(SC, 0, SAP_OFFSET, &uart_buffer[0], 0);

            break;

        case SAP_GLOBAL_CONTROL: // Global Control Request (SSAP 62 -> DSAP 58)
/*
            // Wenn "Clear Data" high, dann SPS CPU auf "Stop"
            if (uart_buffer[9] & CLEAR_DATA_)
            {
              LED_ERROR_AN;  // Status "SPS nicht bereit"
            }
            else
            {
              LED_ERROR_AUS; // Status "SPS OK"
            }

            // Gruppe berechnen
            for (cnt = 0;  uart_buffer[10] != 0; cnt++) uart_buffer[10]>>=1;

            // Wenn Befehl fuer uns ist
            if (cnt == group)
            {
              if (uart_buffer[9] & UNFREEZE_)
              {
                // FREEZE Zustand loeschen
              }
              else if (uart_buffer[9] & UNSYNC_)
              {
                // SYNC Zustand loeschen
              }
              else if (uart_buffer[9] & FREEZE_)
              {
                // Eingaenge nicht mehr neu einlesen
              }
              else if (uart_buffer[9] & SYNC_)
              {
                // Ausgaenge nur bei SYNC Befehl setzen
              }
            }
*/
            break;

        case SAP_SLAVE_DIAGNOSIS: // Get Diagnostics Request (SSAP 62 -> DSAP 60)

            // Nach dem Erhalt der Diagnose wechselt der DP-Slave vom Zustand
            // "Power on Reset" (POR) in den Zustand "Wait Parameter" (WPRM)

            if (function_code == SEND_REQ_DATA_FIRST)
            {

              //uart_buffer[4]  = master_addr;                  // Ziel Master (mit SAP Offset)
              //uart_buffer[5]  = slave_addr + SAP_OFFSET;      // Quelle Slave (mit SAP Offset)
              //uart_buffer[6]  = SLAVE_DATA;
              uart_buffer[7]  = SSAP_data;                    // Ziel SAP Master
              uart_buffer[8]  = DSAP_data;                    // Quelle SAP Slave
              uart_buffer[9]  = STATION_NOT_READY_;           // Status 1
              uart_buffer[10] = STATUS_2_DEFAULT + PRM_REQ_;  // Status 2
              uart_buffer[11] = DIAG_SIZE_OK;                 // Status 3
              uart_buffer[12] = MASTER_ADD_DEFAULT;           // Adresse Master
              uart_buffer[13] = IDENT_HIGH_BYTE;              // Ident high
              uart_buffer[14] = IDENT_LOW_BYTE;               // Ident low
              //uart_buffer[15] = 0x05;     // Geraetebezogene Diagnose (Anzahl Bytes)
              //uart_buffer[16] = 0x00;     //
              //uart_buffer[17] = 0x20;
              //uart_buffer[18] = 0x20;
              //uart_buffer[19] = 0x00;

              profibus_send_CMD(SD2, SLAVE_DATA, SAP_OFFSET, &uart_buffer[7], 8);//13);
            }
            else if (function_code == SEND_REQ_DATA || function_code == SEND_REQ_DATA_T)
            {

              //uart_buffer[4]  = master_addr;                  // Ziel Master (mit SAP Offset)
              //uart_buffer[5]  = slave_addr + SAP_OFFSET;      // Quelle Slave (mit SAP Offset)
              //uart_buffer[6]  = SLAVE_DATA;
              uart_buffer[7]  = SSAP_data;                    // Ziel SAP Master
              uart_buffer[8]  = DSAP_data;                    // Quelle SAP Slave
              uart_buffer[9]  = 0x00;                         // Status 1
              uart_buffer[10] = STATUS_2_DEFAULT;             // Status 2
              uart_buffer[11] = DIAG_SIZE_OK;                 // Status 3
              uart_buffer[12] = master_addr - SAP_OFFSET;     // Adresse Master
              uart_buffer[13] = IDENT_HIGH_BYTE;              // Ident high
              uart_buffer[14] = IDENT_LOW_BYTE;               // Ident low
              //uart_buffer[15] = 0x05;     // Geraetebezogene Diagnose (Anzahl Bytes)
              //uart_buffer[16] = 0x00;     //
              //uart_buffer[17] = 0x20;
              //uart_buffer[18] = 0x20;
              //uart_buffer[19] = 0x00;

              profibus_send_CMD(SD2, SLAVE_DATA, SAP_OFFSET, &uart_buffer[7], 8);//13);
            }

            break;

        case SAP_SET_PRM: // Set Parameters Request (SSAP 62 -> DSAP 61)

            // Nach dem Erhalt der Parameter wechselt der DP-Slave vom Zustand
            // "Wait Parameter" (WPRM) in den Zustand "Wait Configuration" (WCFG)

            // Master Daten siehe Seite 99

            // Nur Daten fuer unser Geraet akzeptieren
            if ((uart_buffer[13] == IDENT_HIGH_BYTE) && (uart_buffer[14] == IDENT_LOW_BYTE))
            {
              //uart_buffer[9] = Befehl
              //uart_buffer[10] = Watchdog 1
              //uart_buffer[11] = Watchdog 2
              //uart_buffer[12] = Min TSDR
              //uart_buffer[13] = Ident H
              //uart_buffer[14] = Ident L
              //uart_buffer[15] = Gruppe
              //uart_buffer[16] = DPV1 Status 1
              //uart_buffer[17] = DPV1 Status 2
              //uart_buffer[18] = DPV1 Status 3

              // Gruppe einlesen
              for (group = 0;  uart_buffer[15] != 0; group++) uart_buffer[15]>>=1;

              profibus_send_CMD(SC, 0, SAP_OFFSET, &uart_buffer[0], 0);
            }

            break;

        case SAP_CHK_CFG: // Check Config Request (SSAP 62 -> DSAP 62)

            // Nach dem Erhalt der Konfiguration wechselt der DP-Slave vom Zustand
            // "Wait Configuration" (WCFG) in den Zustand "Data Exchange" (DXCHG)

            // Master Daten siehe Seite 94

            // IO Konfiguration:
            // Kompaktes Format fuer max. 16/32 Byte IO
            // Spezielles Format fuer max. 64/132 Byte IO

            // Je nach PDU Datengroesse mehrere Bytes auswerten
            // LE/LEr - (DA+SA+FC+DSAP+SSAP) = Anzahl Config Bytes
            for (cnt = 0; cnt < uart_buffer[1] - 5; cnt++)
            {
              switch (uart_buffer[9+cnt] & CFG_DIRECTION_)
              {
                case CFG_INPUT:

                    Input_Data_size = (uart_buffer[9+cnt] & CFG_BYTE_CNT_) + 1;
                    if (uart_buffer[9+cnt] & CFG_WIDTH_ & CFG_WORD)
                      Input_Data_size = Input_Data_size*2;

                    break;

                case CFG_OUTPUT:

                    Output_Data_size = (uart_buffer[9+cnt] & CFG_BYTE_CNT_) + 1;
                    if (uart_buffer[9+cnt] & CFG_WIDTH_ & CFG_WORD)
                      Output_Data_size = Output_Data_size*2;

                    break;

                case CFG_INPUT_OUTPUT:

                    Input_Data_size = (uart_buffer[9+cnt] & CFG_BYTE_CNT_) + 1;
                    Output_Data_size = (uart_buffer[9+cnt] & CFG_BYTE_CNT_) + 1;
                    if (uart_buffer[9+cnt] & CFG_WIDTH_ & CFG_WORD)
                    {
                      Input_Data_size = Input_Data_size*2;
                      Output_Data_size = Output_Data_size*2;
                    }

                    break;

                case CFG_SPECIAL:

                    // Spezielles Format
                    switch (uart_buffer[9+cnt] & CFG_SP_DIRECTION_)
                    {
                      case CFG_SP_VOID:
                          // Leeres Datenfeld
                          break;

                      case CFG_SP_INPUT:

                          Input_Data_size = (uart_buffer[10+cnt] & CFG_SP_BYTE_CNT_) + 1;
                          if (uart_buffer[10+cnt] & CFG_WIDTH_ & CFG_WORD)
                            Input_Data_size = Input_Data_size*2;

                          cnt++;  // Dieses Byte haben wir jetzt schon

                          break;

                      case CFG_SP_OUTPUT:

                          Output_Data_size = (uart_buffer[10+cnt] & CFG_SP_BYTE_CNT_) + 1;
                          if (uart_buffer[10+cnt] & CFG_WIDTH_ & CFG_WORD)
                            Output_Data_size = Output_Data_size*2;

                          cnt++;  // Dieses Byte haben wir jetzt schon

                          break;

                      case CFG_SP_INPUT_OPTPUT:

                          // Erst Ausgang...
                          Output_Data_size = (uart_buffer[10+cnt] & CFG_SP_BYTE_CNT_) + 1;
                          if (uart_buffer[10+cnt] & CFG_WIDTH_ & CFG_WORD)
                            Output_Data_size = Output_Data_size*2;

                          // Dann Eingang
                          Input_Data_size = (uart_buffer[11+cnt] & CFG_SP_BYTE_CNT_) + 1;
                          if (uart_buffer[11+cnt] & CFG_WIDTH_ & CFG_WORD)
                            Input_Data_size = Input_Data_size*2;

                          cnt += 2;  // Diese Bytes haben wir jetzt schon

                          break;

                    } // Switch Ende

                    break;

                default:

                    Input_Data_size = 0;
                    Output_Data_size = 0;

                    break;

              } // Switch Ende
            } // For Ende

            // Kurzquittung wenn alles OK ist
            if (Input_Data_size == INPUT_DATA_SIZE)
              if (Output_Data_size == OUTPUT_DATA_SIZE)
                profibus_send_CMD(SC, 0, SAP_OFFSET, &uart_buffer[0], 0);

            break;

        default:

            // Unbekannter SAP

            break;

      } // Switch DSAP_data Ende

    }
    // Ziel: Slave Adresse
    else if (destination_add == slave_addr)
    {

      // Status Abfrage
      if (function_code == REQ_FDL_STATUS)
      {
        profibus_send_CMD(SD1, FDL_STATUS_OK, 0, &uart_buffer[0], 0);
      }
      // Master sendet Ausgangsdaten und verlangt Eingangsdaten (Send and Request Data)
      else if (function_code == SEND_REQ_DATA || function_code == SEND_REQ_DATA_T)
      {

        // Daten von Master einlesen
        for (cnt = 0; cnt < INPUT_DATA_SIZE; cnt++)
        {
          data_in_register[cnt] = uart_buffer[cnt + 7];
        }

        // Antwort an Master erstellen
        uart_buffer[4] = master_addr;
        uart_buffer[5] = slave_addr;
        uart_buffer[6] = SLAVE_DATA;

        // Daten fuer Master in Buffer schreiben
        for (cnt = 0; cnt < OUTPUT_DATA_SIZE; cnt++)
        {
          uart_buffer[cnt + 7] = data_iout_register[cnt];
        }

        profibus_send_CMD(SD2, SLAVE_DATA, 0, &uart_buffer[7], OUTPUT_DATA_SIZE);
      }
    }

  } // Daten gueltig Ende

}
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * \brief Profibus Telegramm zusammenstellen und senden
 * \param type          Telegrammtyp (SD1, SD2 usw.)
 * \param function_code Function Code der uebermittelt werden soll
 * \param sap_offset    Wert des SAP-Offset
 * \param pdu           Pointer auf Datenfeld (PDU)
 * \param length_pdu    Laenge der reinen PDU ohne DA, SA oder FC
 */
void profibus_send_CMD (unsigned char type,
                        unsigned char function_code,
                        unsigned char sap_offset,
                        char *pdu,
                        unsigned char length_pdu)
{
  unsigned char length_data;

  switch(type)
  {
    case SD1:

      uart_buffer[0] = SD1;
      uart_buffer[1] = master_addr;
      uart_buffer[2] = slave_addr + sap_offset;
      uart_buffer[3] = function_code;
      uart_buffer[4] = checksum(&uart_buffer[1], 3);
      uart_buffer[5] = ED;

      length_data = 6;

      break;

    case SD2:

      uart_buffer[0] = SD2;
      uart_buffer[1] = length_pdu + 3;  // Laenge der PDU inkl. DA, SA und FC
      uart_buffer[2] = length_pdu + 3;
      uart_buffer[3] = SD2;
      uart_buffer[4] = master_addr;
      uart_buffer[5] = slave_addr + sap_offset;
      uart_buffer[6] = function_code;

      // Daten werden vor Aufruf der Funktion schon aufgefuellt

      uart_buffer[7+length_pdu] = checksum(&uart_buffer[4], length_pdu + 3);
      uart_buffer[8+length_pdu] = ED;

      length_data = length_pdu + 9;

      break;

    case SD3:

      uart_buffer[0] = SD3;
      uart_buffer[1] = master_addr;
      uart_buffer[2] = slave_addr + sap_offset;
      uart_buffer[3] = function_code;

      // Daten werden vor Aufruf der Funktion schon aufgefuellt

      uart_buffer[9] = checksum(&uart_buffer[4], 8);
      uart_buffer[10] = ED;

      length_data = 11;

      break;

    case SD4:

      uart_buffer[0] = SD4;
      uart_buffer[1] = master_addr;
      uart_buffer[2] = slave_addr + sap_offset;

      length_data = 3;

      break;

    case SC:

      uart_buffer[0] = SC;

      length_data = 1;

      break;

    default:

      break;

  }

  profibus_TX(&uart_buffer[0], length_data);

}
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * \brief Telegramm senden
 * \param data Pointer auf Datenfeld
 * \param length Laenge der Daten
 */
void profibus_TX (char *data, unsigned char length)
{
#ifndef TX_IRQ
// Ohne Interrupt senden
  RS485_TX_EN;
  uart0_send_data(&data[0], length);

  // Warten bis letztes Byte gesendet worden ist
  while (UCA0STAT & UCBUSY);
  RS485_RX_EN;

  TACCTL0 = CCIE;  // Interrupt ein
  TAR = 0;
  TACCR0 = TIMEOUT_MAX_SYN_TIME;
  profibus_status = PROFIBUS_WAIT_SYN;

#endif

#ifdef TX_IRQ
// Mit Interrupt

  RS485_TX_EN;

  TAR = 0;
  TACCR0 = TIMEOUT_MAX_TX_TIME;   // Timeout setzen
  profibus_status = PROFIBUS_SEND_DATA;

//DEBUG
TACCTL0 &=~CCIE;  // Interrupt aus

  uart_byte_cnt = length;         // Anzahl zu sendender Bytes
  uart_transmit_cnt = 0;          // Zahler fuer gesendete Bytes

  IFG2 |= UCA0TXIFG;              // TX Flag setzen
  IE2  |= UCA0TXIE;               // Enable USCI_A0 TX interrupt

#endif
}
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * \brief Checksumme berechnen. Einfaches addieren aller Datenbytes im Telegramm.
 * \param data Pointer auf Datenfeld
 * \param length Laenge der Daten
 * \return Checksumme
 */
unsigned char checksum(char *data, unsigned char length)
{
  unsigned char csum = 0;

  while(length--)
  {
    csum += data[length];
  }

  return csum;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * \brief ISR UART0 Transmit
 */
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR (void)
{

  if (uart_transmit_cnt < uart_byte_cnt) // Alles gesendet?
  {
    UCA0TXBUF = uart_buffer[uart_transmit_cnt++];

    TAR = 0;
  }
  else
  {
    TAR = 0;

    // Alles gesendet!
    while (UCA0STAT & UCBUSY);      // Warten bis letztes Byte gesendet worden ist

    RS485_RX_EN;     // Auf Receive umschalten

    TAR = 0;
    TACCR0 = TIMEOUT_MAX_SYN_TIME;
    profibus_status = PROFIBUS_WAIT_SYN;

    IFG2 &= ~UCA0TXIFG;             // TX Flag loeschen
    IE2  &= ~UCA0TXIE;              // Disable USCI_A0 TX interrupt

//DEBUG
TACCTL0 = CCIE;  // Interrupt ein

  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * \brief ISR TIMER A
 */
#pragma vector=TIMERA0_VECTOR
__interrupt void TIMERA0_ISR (void)
{
  LPM0_EXIT;

  TAR = 0;

  switch (profibus_status)
  {
    case PROFIBUS_WAIT_SYN: // TSYN abgelaufen

        profibus_status = PROFIBUS_WAIT_DATA;

        TACCR0 = TIMEOUT_MAX_SDR_TIME;
        uart_byte_cnt = 0;

        break;

    case PROFIBUS_WAIT_DATA:  // TSDR abgelaufen aber keine Daten da

        break;

    case PROFIBUS_GET_DATA:   // TSDR abgelaufen und Daten da

        profibus_status = PROFIBUS_WAIT_SYN;

        profibus_RX();

        break;

    case PROFIBUS_SEND_DATA:  // Fehler, Timeout abgelaufen, wieder auf empfang gehen

        profibus_status = PROFIBUS_WAIT_SYN;
        TACCR0 = TIMEOUT_MAX_SYN_TIME;
        RS485_RX_EN;                    // Auf Receive umschalten

        break;

    default:

        break;

  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
```

