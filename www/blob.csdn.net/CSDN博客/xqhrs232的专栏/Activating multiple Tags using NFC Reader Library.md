# Activating multiple Tags using NFC Reader Library - xqhrs232的专栏 - CSDN博客
2017年09月27日 11:47:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：253
原文地址::[https://community.nxp.com/docs/DOC-335265](https://community.nxp.com/docs/DOC-335265)
Hello NFC enthusiasts,
In the NFC communication protocol, when a device acts as a NFC reader (it provides its own field), it is waiting for a tag to approach. When this occurs, the reader energizes the tag and depending on the application, it can read from or write to a tag.
When multiple tags are in the field, the power decreases according to the number of tags being energized, for which the tag operations will not work properly. For this, there is a process called anti-collision, in which the reader decides, from the detected
 tags, one to work with.
The purpose of this document is to demonstrate the activation of each tag at a given index.
This demonstration is going to be made with two NTAG 216.
![](https://community.nxp.com/servlet/JiveServlet/downloadImage/102-335265-8-193835/WhatsApp+Image+2017-09-13+at+4.36.50+PM.jpeg)
This demonstration is based on NXP NFC Reader Library v05.02.00, NfcrdlibEx3_NFCForum project for PNEV7462B, in which some modifications are going to be made in order to carry this out. These tags are compliant with NFC Forum Type 2 Tag and ISO/IEC14443 Type
 A specifications.
- 
 In phacDiscLoop.h modify the max number of cards supported (two cards for this demonstration):
`#define PHAC_DISCLOOP_CFG_MAX_CARDS_SUPPORTED 0x02U`
- 
 In NfcrdlibEx3_NFCForum.c add the following code in LoadDiscoveryConfiguration():
```
static phStatus_t LoadDiscoveryConfiguration()
{
...
/*Passive max typea devices*/
 status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_DEVICE_LIMIT, 2);
 CHECK_STATUS(status);
}
```
- 
A fix to the SW stack has to be made (Fix will be implemented in the next release):
open "phacDiscLoop_Sw_Int_A.c", line 511, change if statement as below.
`if((pDataParams->sTypeATargetInfo.bTotalTagsFound > 1) && ((bTypeATagIdx) < pDataParams->sTypeATargetInfo.bTotalTagsFound))`
Until now, the reader is able to detect a maximum of two tags and work with up to two type A devices.
The activation of a tag at a given index is possible to the phacDiscLoop_ActivateCard() function.
Once this function is called, it will receive the discovery loop data parameters, the type of tag and the index of a tag to be activated.
- 
The code will be added after knowing that multiple tags are detected and resolved in the NfcrdlibEx3_NFCForum.c file.
```
else if((status & PH_ERR_MASK) == PHAC_DISCLOOP_MULTI_DEVICES_RESOLVED)
 {
 /*
 * Multiple cards resolved. It enters here if DEVICE LIMIT > 1 and more than one devices are
 * detected and resolved.
 */
 DEBUG_PRINTF (" \n Multiple cards resolved: \n");
/* Get detected technology type */
 status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
 CHECK_STATUS(status);
/* Get number of tags detected */
 status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_NR_TAGS_FOUND, &wNumberOfTags);
 CHECK_STATUS(status);
DEBUG_PRINTF ("\tNumber of tags: %d \n",wNumberOfTags);
/* Code */
...
}
```
Note: The
 code to be inserted in the comment /* Code */ is below in the Code
 section of this document.
The demonstration will be as simple as activating one tag, read its NDEF message, activate the second tag and read its NDEF message as well so that we make sure the activation process is performed correctly.
Each tag was previously written with a text NDEF message respectively.
Tag 1:
Text: Hallo!
Language: de
Tag 2:
Text: ¡Hola!
Language: es
Writing to a tag can be done by making use of our TagWriter app available in the play store:
[NFC
 TagWriter by NXP - Aplicaciones de Android en Google Play](https://community.nxp.com/external-link.jspa?url=https%3A%2F%2Fplay.google.com%2Fstore%2Fapps%2Fdetails%3Fid%3Dcom.nxp.nfc.tagwriter%26hl%3Des)
Code section:
```
uint8_t bTagState1;
/* Tag 1 */
/* Activate tag at index 0 */
status = phacDiscLoop_ActivateCard(pDataParams, PHAC_DISCLOOP_TECH_TYPE_A, 0x00);
/* Check for NDEF presence */
 status = phalTop_CheckNdef(palTop, &bTagState1);
/* Read NDEF message */
status = ReadNdefMessage(PHAL_TOP_TAG_TYPE_T2T_TAG);
 DEBUG_ERROR_PRINT(status);
/* Tag 2 */
/* Activate tag at index 1 */
status = phacDiscLoop_ActivateCard(pDataParams, PHAC_DISCLOOP_TECH_TYPE_A, 0x01);
/* Check for NDEF presence */
 status = phalTop_CheckNdef(palTop, &bTagState1);
/* Read NDEF message */
status = ReadNdefMessage(PHAL_TOP_TAG_TYPE_T2T_TAG);
 DEBUG_ERROR_PRINT(status);
```
Behavior shown in the console monitor:
NFC Forum Example:
![](https://community.nxp.com/servlet/JiveServlet/downloadImage/102-335265-8-193925/pastedImage_3.png)
![](https://community.nxp.com/servlet/JiveServlet/downloadImage/102-335265-8-193926/pastedImage_4.png)
This implementation demonstrated the activation of two type A tags at a given index.
I hope this is of great help!
Best regards,
Ivan.
1
 位用户发现它有用
## 附件
- [](https://community.nxp.com/servlet/JiveServlet/download/335265-8-409576/Project%20files.zip)
[Project
 files.zip](https://community.nxp.com/servlet/JiveServlet/download/335265-8-409576/Project%20files.zip)[](https://community.nxp.com/docs/DOC-335265#)28.2
 KB

