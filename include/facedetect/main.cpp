
#if 1
#define test_rongyinet main
#include "test_rongyinet.inl"
#endif

#if 0
#define test_cam main
#include "test_cam.inl"
#endif

#if 0
#define test_rongyinet main
#include "test_rongyinet.inl"
#endif

#if 0
#define test_zip main
#include "test_zip.inl"
#endif

#if 0
#define test_http main
#include "test_http.inl"
#endif

#if 0
#define test_facetrack main
#include "face/test_facetrack.inl"
#endif

#if 0
#define test_socket main
#include "test_socket.inl"
#endif

#if 0
#define test_faceplusplus main
#include "test_faceplusplus.inl"
#endif

#if 0
#define test_openssl main
#include <openssl/ssl.h>
int test_openssl() {
  SSL *ssl = NULL;
  SSL_CTX *ssl_ctx = NULL;
  SSL_load_error_strings();
  SSLeay_add_ssl_algorithms();
  ssl_ctx = SSL_CTX_new(SSLv23_client_method());
  ssl = SSL_new(ssl_ctx);
  SSL_shutdown(ssl);
  printf("test_openssl exit\n");
  return 0;
}
#endif

#if 0
#define test_curl main
#include "test_curl.inl"
#endif

#if 0
#define test_face_lena main
#include "face/test_face_lena.inl"
#endif

#ifdef _WIN32
#include "sys/sys_win32.inl"
#else
#include "sys/sys_linux.inl"
#endif
