# WinCE Security --- 文件及证书签名与验证 - xqhrs232的专栏 - CSDN博客
2014年10月18日 16:31:49[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：695
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/nanjianhui/article/details/5830977](http://blog.csdn.net/nanjianhui/article/details/5830977)
相关网帖
1、[WinCE Security --- X509证书及私钥的导入](http://blog.csdn.net/nanjianhui/article/details/5797772)----[http://blog.csdn.net/nanjianhui/article/details/5797772](http://blog.csdn.net/nanjianhui/article/details/5797772)
2、[WinCE Security --- 系统X509证书库定制](http://blog.csdn.net/nanjianhui/article/details/5777937)----[http://blog.csdn.net/nanjianhui/article/details/5777937](http://blog.csdn.net/nanjianhui/article/details/5777937)
3、[WinCE 与Linux之间的签名验证及加解密](http://blog.csdn.net/nanjianhui/article/details/5880084)----[http://blog.csdn.net/nanjianhui/article/details/5880084](http://blog.csdn.net/nanjianhui/article/details/5880084)
4、[WinCE Security---简介](http://blog.csdn.net/nanjianhui/article/details/5760771)----[http://blog.csdn.net/nanjianhui/article/details/5760771](http://blog.csdn.net/nanjianhui/article/details/5760771)
作者：ARM-WinCE
前面的blog中介绍了如何导入X509证书及私钥，私钥本身也包含公钥，私钥自己保留，公钥可以给用户，这样用私钥对文件或者证书签名之后发给用户，如果用户通过公钥能够验证签名就说明用户知道文件是从签名方发来的并且是可信任的。
最近一些blog主要在谈WinCE的Security模块，涉及到PKI方面的知识，我在这里不对PKI做过多的介绍，网上有很多相关文章，这里主要还是再说WinCE上CryptoAPI的使用，下面介绍一些签名和验证签名的具体流程：
**1. 签名(Signature):**
**(1) 读取文件并进行哈希**
比如要对一个文件签名，那么首先读取文件的数据，然后对数据进行哈希(Hash)，哈希的算法有很多，比如MD5，SHA1，SHA256等。什么叫哈希，哈希就是对任意长度的数据通过散列算法变成一个固定长度的数据，该固定长度的数据常被称为摘要(Digest)，签名方和验证方需要确认使用同样的哈希算法，否则验证方无法验证签名。
**(2) 对摘要进行签名**
在生成摘要以后，签名方拥有私钥，通过私钥对摘要进行签名(Signature)，最终签名的长度一般取决于私钥的长度，私钥可以1024bits，也可以是2048bits，而签名数据的长度可能是128Bytes或者256Bytes。
**(3) 发送文件及签名**
签名完成后，要将签名附在文件的最后，然后将文件和签名一起发给接收方，接收方会对签名进行验证。
**2. 验证签名(Verify signature):**
**(1) 接收签名并进行哈希**
接收方接到发来的文件后，首先将文件本身和签名进行分离，然后采用和签名方一样的哈希算法，对文件本身进行哈希，从而形成摘要(Digest)。
**(2) 验证签名**接收方拥有公钥，通过公钥对签名进行解密得到摘要，然后对比该摘要和自己生成的摘要，如果一样则表示签名得到了验证，发送方是可信任的。
**3. X509证书签名及验证**
X509证书的签名和验证的本质和文件的签名与验证是一样的，因为X509证书也是文件，只不过在对X509证书进行签名的时候，先要对它进行解码，因为X509证书是经过ASN.1编码的，在进行解码以后，就可以对它签名了。同样，验证X509证书的签名也要先对X509证书进行解码，然后哈希它，最后验证签名。
下面先介绍一下签名及验证签名的相关函数：
**1. BOOL CRYPTFUNC CryptCreateHash( HCRYPTPROV hProv, ALG_ID Algid, HCRYPTKEY hKey, DWORD dwFlags, HCRYPTHASH* phHash)**
该函数用于创建一个哈希对象，然后通过该哈希对象可以对数据进行哈希。
**hProv:** 秘钥容器提供者的句柄
**Algid: **哈希算法，比如MD2,MD5,SHA1,SHA256等
**hKey: **如果哈希算法是一种基于加密的哈希算法，比如HMAC，那么这里需要提供秘钥，否则该参数为0
**dwFlags:**设置为0
**phHash:** 返回一个创建好的哈希对象句柄
函数调用成功会返回TRUE，否则返回FALSE。
**2. BOOL WINAPI CryptHashData(HCRYPTHASH hHash, BYTE* pbData, DWORD dwDataLen, DWORD dwFlags)**
该函数用于对数据进行哈希，最终形成摘要。
**hHash:** 哈希对象的句柄
**pbData:** 要被哈希的数据
**dwDataLen:** 要被哈希的数据长度
**dwFlags:** 设置为0
函数调用成功会返回TRUE，否则返回FALSE。
**3. BOOL WINAPI CryptSignHash(HCRYPTHASH hHash, DWORD dwKeySpec, LPCTSTR sDescription, DWORD dwFlags, BYTE* pbSignature, DWORD* pdwSigLen)**
该函数用于对哈希后的数据进行签名。
**hHash:** 哈希对象的句柄
**dwKeySpec:** 签名秘钥还是交换秘钥
**sDescription:** 设置为NULL
**dwFlags:** 设置为0
**pbSignature:** 输出签名的数据，如果该参数为NULL，则该函数只返回签名数据的长度
**pdwSigLen:** 输出签名数据的长度
函数调用成功会返回TRUE，否则返回FALSE。
**4. BOOL CRYPTFUNC CryptDestroyHash(HCRYPTHASH hHash)**
该函数用于释放哈希对象
**hHash:** 哈希对象的句柄
函数调用成功会返回TRUE，否则返回FALSE。
**5. BOOL WINAPI CryptVerifySignature(HCRYPTHASH hHash, BYTE* pbSignature, DWORD dwSigLen, HCRYPTKEY hPubKey, LPCTSTR sDescription, DWORD dwFlags)**
该函数用于验证签名
**hHash:** 哈希对象的句柄
**pbSignature:** 签名数据
**dwSigLen:** 签名数据的长度
**hPubKey:** 用于验证签名的公钥的句柄
**sDescription:** 设置为NULL
**dwFlags:** 设置为0
该函数返回TRUE表示签名得到验证，否则验证失败。
**6. BOOL WINAPI CryptDecodeObjectEx(DWORD dwCertEncodingType, LPCSTR lpszStructType, const BYTE* pbEncoded, DWORD cbEncoded, DWORD dwFlags, PCRYPT_DECODE_PARA pDecodePara, void* pvStructInfo, DWORD* pcbStructInfo)**
该函数目前在WinCE中只支持对X509证书数据进行解码。
**dwCertEncodingType:** 编码数据类型，目前只能是X509_ASN_ENCODING
**lpszStructType:** 设置为X509_CERT
**pbEncoded:** X509证书数据
**cbEncoded:** X509证书数据的长度
**dwFlags:** 解码标记位
**pDecodePara:** 解码信息参数，可以设置为NULL
**pvStructInfo:** 输出解码后的证书数据
**pcbStructInfo:** 作为输入，表示pvStructInfo的Buffer的长度，作为输出，表示解码后的证书数据的长度
函数调用成功会返回TRUE，否则返回FALSE。
**7. BOOL WINAPI CryptVerifyCertificateSignature(HCRYPTPROV hCryptProv, DWORD dwCertEncodingType, BYTE* pbEncoded, DWORD cbEncoded, PCERT_PUBLIC_KEY_INFO pPublicKey)**
该函数用于验证X509证书的签名。
**hCryptProv:** 秘钥提供者的句柄
**dwCertEncodingType:** X509_ASN_ENCODING
**pbEncoded:** 一个被签名的X509证书数据
**cbEncoded:** 一个被签名的X509证书数据的长度
**pPublicKey: **一个包含公钥的结构，用于验证签名
函数调用成功会返回TRUE，否则返回FALSE。
**8. BOOL WINAPI CertVerifySubjectCertificateContext(PCCERT_CONTEXT pSubject, PCCERT_CONTEXT pIssuer, DWORD* pdwFlags)**
该函数可以通过一个证书来验证另一个证书，实际上是通过一个证书中的公钥来验证另一个证书中的签名。
**pSubject:** 要被验证的带签名的X509证书
**pIssuer:** 包含公钥的X509证书
**pdwFlag:** 验证方式包括签名验证和有效期验证
如果验证成功，该函数返回TRUE同时pdwFlag返回0，否则失败。
**注意：**最后两个函数**CertVerifySubjectCertificateContext**和**CryptVerifyCertificateSignature**在PC上面使用没有问题，但是在WinCE上面对于某些哈希算法是不支持的。所以比较稳妥的做法还是将X509证书数据进行解码，然后通过函数**CryptVerifySignature**来验证其签名的有效性。
下面给出用于签名和验证签名的两个函数：
**1. 签名函数**
/********************************************************************************
* FUNCTION NAME: CAPISign  
*
* DESCRIPTION:    Use the private key to sign the data
*
* INPUT PARAMETERS:
*                   pbData:         The data to be signed
*                   dwDataLen:  The data length
*
* OUTPUT PARAMETERS:
*                   pbSign:           The signature
*                   pdwSigLen:  The signature length
*
* RETURN VALUE:
*                   TRUE:           Success
*                   Others:           Failed
*
* SPECIAL CONSIDERATIONS:
*                   <none>
*********************************************************************************/
BOOL CAPISign(BYTE* pbData, DWORD dwDataLen, BYTE* pbSign, DWORD* pdwSigLen)
{
    BOOL fRet = FALSE;
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    PBYTE pbSignature = NULL;
    DWORD i, dwLen, cbSize, dwTmp;
static UCHAR buf[4096];
      BYTE* ptptr;
// Get handle of the provider which should include private key
if(CryptAcquireContext(&hProv, szKeyContainer, NULL, PROV_RSA_AES, 0) == TRUE)
      {
// Create a hash object.
if(CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash) == TRUE)
            {
// Get the length of the data to be signed
                  cbSize = dwDataLen;
                  ptptr = pbData;
while (cbSize)
                  {
                        dwTmp = cbSize > sizeof(buf) ? sizeof(buf) : cbSize;
                        memcpy(buf, ptptr, dwTmp);
// Add data to hash object.
if(CryptHashData(hHash, buf, dwTmp, 0) != TRUE)
                        {
                              printf("Add data to hash Error./r/n");
                              cbSize = 0xFFFFFFFF;
break;
                        }
                        cbSize -= dwTmp;
                        ptptr += dwTmp;
                  }
// All data has been hashed successfully.
if (cbSize == 0)
                  {
// Determine size of signature.
if(CryptSignHash(hHash, Hdr.dwKeySpec, NULL, 0, NULL, pdwSigLen) == TRUE)
                        {
// Allocate memory for 'pbSignature'.
                              pbSignature = (PBYTE)LocalAlloc(LPTR, *pdwSigLen);
                        }
                  }
            }
    }
if (pbSignature != NULL)
      {
// Sign hash object (with signature key).
if(CryptSignHash(hHash, Hdr.dwKeySpec, NULL, 0, pbSignature, pdwSigLen) == TRUE)
            {
                  dwLen = *pdwSigLen;
for (i = 0; i < dwLen; i ++)
                  {
                        pbSign[dwLen - 1 - i] = pbSignature[i];
                  }
                  printf("Signed successfully./r/n");
                  fRet = TRUE;
            }
else
            {
                  printf("Signature Error./r/n");
            }
      }
if (pbSignature)
      {
            LocalFree(pbSignature);
      }
// Destroy hash object.
      CryptDestroyHash(hHash);
// Release provider handle.
      CryptReleaseContext(hProv, 0);
return fRet;
}
**2. 验证签名的函数**
/********************************************************************************
* FUNCTION NAME: Cert_VerifySig  
*
* DESCRIPTION:    It's used to verify the signature
*
* INPUT PARAMETERS:
*                   gszFileName:   The Orginal file
*                   szSignName:    The Signature file
*                   pbPubKey:         The pbulic key
*
* OUTPUT PARAMETERS:
*                   <None>
*
* RETURN VALUE:
*                   TRUE:           Success
*                   Others:           Failed
*
* SPECIAL CONSIDERATIONS:
*                   <none>
*********************************************************************************/
DWORD Cert_VerifySig(TCHAR* gszFileName, TCHAR* szSignName, BYTE* pbPubKey)
{
      CERT_PUBLIC_KEY_INFO keyinfo;
      HCRYPTKEY hKey;
      HCRYPTPROV hProv;
      HCRYPTHASH hHash;
      HANDLE hFile;
      HANDLE hSigFile;
static UCHAR data[2048];
static UCHAR dataRev[2048];
static UCHAR SigData[1024];
      DWORD i, cbSize, cbSizeRead, dwSize, dwRealSize, fRet;
      LPTSTR pszFileName = gszFileName;
      LPTSTR pszSigName = szSignName;
// Get handle to the default provider.
if (!CryptAcquireContext(&hProv, szKeyContainer, NULL, PROV_RSA_AES, 0))
      {
            fRet = GetLastError();
            printf("CryptAcquireContext Error./r/n");
return fRet;
    }
if (!(pCryptImportPublicKeyInfoEx(hProv, X509_ASN_ENCODING, &keyinfo, 0, 0, NULL, &hKey)))
      {
            fRet = GetLastError();
            MessageBox(NULL, _T("CryptImportPublicKeyInfoEx Error."), _T("Message"), MB_OK);
return FALSE;
      }
      hFile = CreateFile(pszFileName, (GENERIC_READ | GENERIC_WRITE), 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
if(hFile == INVALID_HANDLE_VALUE)
    {
        MessageBox(NULL, _T("Open File Error."), _T("Message"), MB_OK);
return FALSE;
    }
if(!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash))
      {
          MessageBox(NULL, _T("Create hash object Error."), _T("Message"), MB_OK);
return FALSE;
    }
    cbSize = GetFileSize(hFile, NULL);
while (cbSize)
    {
if (!ReadFile(hFile, data, sizeof(data), &cbSizeRead, NULL) || cbSizeRead == 0)
        {
                  MessageBox(NULL, _T("Read File Error."), _T("Message"), MB_OK);
return FALSE;
        }
// Add data to hash object.
if(!CryptHashData(hHash, data, cbSizeRead, 0))
            {
            MessageBox(NULL, _T("Add data to hash Error."), _T("Message"), MB_OK);
return FALSE;
        }
        cbSize -= cbSizeRead;
    }
      hSigFile = CreateFile(pszSigName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
if (hSigFile)
      {     // Get the file size
            dwSize = GetFileSize(hSigFile, NULL);
if (dwSize != 0xFFFFFFFF)
            {    
if (ReadFile(hSigFile, SigData, dwSize, &dwRealSize, NULL) == TRUE)
                  {
                       MessageBox(NULL, _T("Read Signature File OK."), _T("Message"), MB_OK);
                  }
            }
      }
for (i = 0; i < dwRealSize; i ++)
      {
            dataRev[dwRealSize - 1 - i] = SigData[i];
      }
if(CryptVerifySignature(hHash, (BYTE*)dataRev, dwRealSize, hKey, NULL, 0) == FALSE)
      {
            fRet = GetLastError();
            MessageBox(NULL, _T("Verify Signature Error."), _T("Message"), MB_OK);
      }
else
    {
            MessageBox(NULL, _T("Verify Signature OK."), _T("Message"), MB_OK);
    }
return TRUE;
}
最后要说的是，无论是第一个函数中签名后得到的签名数据，还是第二个函数中从文件中读取的签名数据，都对数据进行了反转，这样做的原因主要是为了支持Linux下的Openssl，在Openssl中的签名数据和WinCE下的字序是相反的，所以如果要在WinCE和Linux之间彼此进行验证，就需要对数据进行反转。
