#if 0
校验文件数字签名的合法性(VerifyPE)
Authenticode ? 是一种数字签名格式，它是用来验证二进制软件的来源和完整性。Authenticode是基于公开密匙加密标准(PKCS) #7 来签名数据，并使用X.509证书来绑定经过数字签发的二进制程序与其软件发布者的身份的联系。这份文档包含Authenticode签名的结构和技术细节。
这份文档不讨论如何签发 / 处理X.509证书，如何使用Windows SDK工具来签署二进制程序，如何部署代码签名基础设施，或者相关的Windows ? API函数。这方面的相关信息可以查看在本文档最后的“资源”章节。
本资料应用于下列操作系统：
Windows Server ? 2008
Windows Vista ?
Windows Server 2003
Windows ? XP
Windows 2000
这里引用和讨论的相关资源列于这份文档的结尾。
概览Overview
在PE文件中的可信代码数字签名是PKCS #7 签名块结构。该签名可以保证：
· 这份软件源于某个确定的软件发布者。
· 这份软件自从签署以来没有经过修改。
一份PKCS #7 签名块结构包含该PE文件的哈希值、通过软件发布者私匙创建的签名以及将软件发布者的签名密匙绑定到一个合法实体的X.509 v3证书。一份PKCS #7签名块可以包含以下可选信息：
· 关于软件发布者的描述
· 软件发布者的链接
· 可信代码签名的时间戳
签名时间戳由时间戳权威机构(TSA)生成，并且保证软件发布者所做的签名在这个时间戳之前已经存在。这个时间戳可以延长了这个签名的生命期，即便相关的签名证书已经过期或者后来被废除。
可信代码签名可以被嵌入到Windows的PE文件中，位于PE文件的Optional Header Data Directories结构中Certificate Table所指向的位置。当可信代码签名被用于签署一个Windows PE文件时，计算文件数字签名哈希值的算法略过PE文件结构中的特定字段。当把数字签名嵌入文件时，签名过程可以修改这些字段，而不致于影响文件的哈希值。
图1显示了一幅简单的PE文件全局图，它描述了数字签名是如何包含在PE文件中的。它包含了嵌入的可信代码数字签名和指出那些被略过计算PE文件哈希值的PE结构字段。
关于PE文件结构的细节，请参阅“Microsoft Portable Executable and Common Object File Format Specification”(PE / COFF specification)。
关于可信代码数字签名中PKCS #7部分的细节，请参阅本文档后面的Abstract Syntax Notation version 1 (ASN.1) 结构定义。
关于可信代码数字签名如何计算PE文件哈希值的细节，参阅本文档后面的“Calculating the PE Image Hash”。
PE结构中的两个位置
一个是 数字签名的 物理偏移地址 一个是 签名文件的长度
具体讲就是 PE结构的 IMAGE_NT_HEADERS结构中有一个 IMAGE_OPTIONAL_HEADER32
在IMAGE_OPTIONAL_HEADER32中有一个IMAGE_DATA_DIRECTORY项
而IMAGE_DATA_DIRECTORY项是一系列的数据 其中的第5项是指向 安全信息的
直接修改里面的 dwSingSize和VirtualAddress就可以了
验证经过数字签名的PE文件代码
#endif
#define _UNICODE 1
#define UNICODE 1
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <Softpub.h>
#include <wincrypt.h>
#include <wintrust.h>
// Link with the Wintrust.lib file.
#pragma comment (lib, "wintrust")
BOOL VerifyEmbeddedSignature(LPCWSTR pwszSourceFile)
{
    LONG lStatus;
    DWORD dwLastError;
    // Initialize the WINTRUST_FILE_INFO structure.
    WINTRUST_FILE_INFO FileData = {0};
    memset(&FileData, 0, sizeof(FileData));
    FileData.cbStruct = sizeof(WINTRUST_FILE_INFO);
    FileData.pcwszFilePath = pwszSourceFile;
    FileData.hFile = NULL;
    FileData.pgKnownSubject = NULL;
    /*
    WVTPolicyGUID specifies the policy to apply on the file
    WINTRUST_ACTION_GENERIC_VERIFY_V2 policy checks:
    1) The certificate used to sign the file chains up to a root
    certificate located in the trusted root certificate store. This
    implies that the identity of the publisher has been verified by
    a certification authority.
    2) In cases where user interface is displayed (which this example
    does not do), WinVerifyTrust will check for whether the
    end entity certificate is stored in the trusted publisher store,
    implying that the user trusts content from this publisher.
    3) The end entity certificate has sufficient permission to sign
    code, as indicated by the presence of a code signing EKU or no
    EKU.
    */
    GUID WVTPolicyGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;
    WINTRUST_DATA WinTrustData = {0};
    // Initialize the WinVerifyTrust input data structure.
    // Default all fields to 0.
    memset(&WinTrustData, 0, sizeof(WinTrustData));
    WinTrustData.cbStruct = sizeof(WinTrustData);
    // Use default code signing EKU.
    WinTrustData.pPolicyCallbackData = NULL;
    // No data to pass to SIP.
    WinTrustData.pSIPClientData = NULL;
    // Disable WVT UI.
    WinTrustData.dwUIChoice = WTD_UI_NONE;
    // No revocation checking.
    WinTrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
    // Verify an embedded signature on a file.
    WinTrustData.dwUnionChoice = WTD_CHOICE_FILE;
    // Default verification.
    WinTrustData.dwStateAction = 0;
    // Not applicable for default verification of embedded signature.
    WinTrustData.hWVTStateData = NULL;
    // Not used.
    WinTrustData.pwszURLReference = NULL;
    // This is not applicable if there is no UI because it changes
    // the UI to accommodate running applications instead of
    // installing applications.
    //WinTrustData.dwUIContext = 0;
    // Set pFile.
    WinTrustData.pFile = &FileData;
    // WinVerifyTrust verifies signatures as specified by the GUID
    // and Wintrust_Data.
    lStatus = WinVerifyTrust(NULL, &WVTPolicyGUID, &WinTrustData);
#define wprintf_s wprintf
    switch (lStatus)
    {
    case ERROR_SUCCESS:
        /*
        Signed file:
        - Hash that represents the subject is trusted.
        - Trusted publisher without any verification errors.
        - UI was disabled in dwUIChoice. No publisher or
        time stamp chain errors.
        - UI was enabled in dwUIChoice and the user clicked
        "Yes" when asked to install and run the signed
        subject.
        */
        wprintf_s(L"The file \"%s\" is signed and the signature was verified.\n",
                  pwszSourceFile);
        break;
    case TRUST_E_NOSIGNATURE:
        // The file was not signed or had a signature
        // that was not valid.
        // Get the reason for no signature.
        dwLastError = GetLastError();
        if (TRUST_E_NOSIGNATURE == dwLastError ||
                TRUST_E_SUBJECT_FORM_UNKNOWN == dwLastError ||
                TRUST_E_PROVIDER_UNKNOWN == dwLastError)
        {
            // The file was not signed.
            wprintf_s(L"The file \"%s\" is not signed.\n",
                      pwszSourceFile);
        }
        else
        {
            // The signature was not valid or there was an error
            // opening the file.
            wprintf_s(L"An unknown error occurred trying to "
                      L"verify the signature of the \"%s\" file.\n",
                      pwszSourceFile);
        }
        break;
    case TRUST_E_EXPLICIT_DISTRUST:
        // The hash that represents the subject or the publisher
        // is not allowed by the admin or user.
        wprintf_s(L"The signature is present, but specifically "
                  L"disallowed.\n");
        break;
    case TRUST_E_SUBJECT_NOT_TRUSTED:
        // The user clicked "No" when asked to install and run.
        wprintf_s(L"The signature is present, but not trusted.\n");
        break;
    case CRYPT_E_SECURITY_SETTINGS:
        /*
        The hash that represents the subject or the publisher
        was not explicitly trusted by the admin and the
        admin policy has disabled user trust. No signature,
        publisher or time stamp errors.
        */
        wprintf_s(L"CRYPT_E_SECURITY_SETTINGS - The hash "
                  L"representing the subject or the publisher wasn't "
                  L"explicitly trusted by the admin and admin policy "
                  L"has disabled user trust. No signature, publisher "
                  L"or timestamp errors.\n");
        break;
    default:
        // The UI was disabled in dwUIChoice or the admin policy
        // has disabled user trust. lStatus contains the
        // publisher or time stamp chain error.
        wprintf_s(L"Error is: 0x%x.\n", lStatus);
        break;
    }
    return true;
}
int test_VerifyPE()
{
    const wchar_t* fn;
    fn = L"D:\\keypair.p12";
    fn = L"D:\\Security-SPP-Component-SKU-Ultimate-OEM-NONSLP-pl.xrm-ms";
    int x = VerifyEmbeddedSignature(fn);
    return 0;
}

