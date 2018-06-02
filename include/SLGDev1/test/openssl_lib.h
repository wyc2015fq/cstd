
#ifndef _OPENSSL_LIB_INL_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <windows.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/aes.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rand.h>

#undef X509_NAME
#if 1
#pragma comment(lib,"libeay32.lib")
#pragma comment(lib,"ssleay32.lib")
#else
#pragma comment(lib,"libcrypto.lib")
#pragma comment(lib,"libssl.lib")
#endif

#endif // _OPENSSL_LIB_INL_

