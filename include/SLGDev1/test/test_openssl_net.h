#define MAXBUF 1024
#define MYPORT 7838
#undef free
int sock_init();
int test_server()
{
    int sockfd, new_fd;
    socklen_t len;
    struct sockaddr_in my_addr, their_addr;
    unsigned int myport, lisnum;
    char buf[MAXBUF + 1];
    SSL_CTX* ctx;
    const char* s_cert_file = NULL;
    const char* s_key_file = NULL;
    myport = MYPORT;
    lisnum = 2;
    const char* path = NULL;
    path = "E:/pub/bin/ssl/openssl_exe";
    _chdir(path);
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_server_method());
    if (ctx == NULL)
    {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    path = "C:/rootca/111";
    _chdir(path);
    if (0)
    {
        if (1)
        {
            s_cert_file = "jim_crt.pem";
            s_key_file = "jim_key.pem";
        }
        else
        {
            s_cert_file = "yi_crt1.pem";
            s_key_file = "yi_key1.pem";
        }
        s_cert_file = "usercrt1.pem";
        s_key_file = "userkey1.pem";
        if (SSL_CTX_use_certificate_file(ctx, s_cert_file, SSL_FILETYPE_PEM) <= 0)
        {
            ERR_print_errors_fp(stdout);
            exit(1);
        }
        if (SSL_CTX_use_PrivateKey_file(ctx, s_key_file, SSL_FILETYPE_PEM) <= 0)
        {
            ERR_print_errors_fp(stdout);
            exit(1);
        }
        if (!SSL_CTX_check_private_key(ctx))
        {
            ERR_print_errors_fp(stdout);
            exit(1);
        }
    }
    else
    {
        pk12_t pk12[1] = {0};
        const char* pass = "123456";
        DEV_APPUPDATE c = {0};
        char* path = "C:/rootca/111/usercrt(T)(1).pfx";
        char* pk12_buf = NULL;
        int pk12_buflen;
        if (1)
        {
            pass = "secret";
            path = "jim_cert.pfx";
            pass = "secret";
            path = "keycert.pfx";
        }
        else
        {
            pass = "password";
            path = "keypair.p12";
            pass = "123456";
            path = "yi_cert.pfx";
        }
        pk12_buflen = loadfile(path, &pk12_buf);
        int ret = pk12_VerifyCert(pk12_buf, pk12_buflen, pass);
        pk12_unpack(pk12, pk12_buf, pk12_buflen, pass);
        SSL_CTX_use_certificate(ctx, pk12->pCert);
        SSL_CTX_use_PrivateKey(ctx, pk12->pPkey);
        pk12_free(pk12);
        if (!SSL_CTX_check_private_key(ctx))
        {
            ERR_print_errors_fp(stdout);
            SSL_CTX_free(ctx);
            return 0;
        }
        SSL_CTX_set_options(ctx, SSL_OP_ALL);
        if (!SSL_CTX_set_cipher_list(ctx , TLS1_TXT_RSA_WITH_AES_256_GCM_SHA384))
        {
            DEBUG_INFO("%s" , "error setting cipher AES256-GCM-SHA384");
        }
    }
    sock_init();
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    else
    {
        printf("socket created\n");
    }
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = PF_INET;
    my_addr.sin_port = htons(myport);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr*) &my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }
    else
    {
        printf("binded\n");
    }
    if (listen(sockfd, lisnum) == -1)
    {
        perror("listen");
        exit(1);
    }
    else
    {
        printf("begin listen\n");
    }
    while (1)
    {
        SSL* ssl;
        len = sizeof(struct sockaddr);
        if ((new_fd = accept(sockfd, (struct sockaddr*) &their_addr, &len)) == -1)
        {
            perror("accept");
            exit(errno);
        }
        else
            printf("server: got connection from %s, port %d, socket %d\n",
                   inet_ntoa(their_addr.sin_addr),
                   ntohs(their_addr.sin_port), new_fd);
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, new_fd);
        //server code
        if (SSL_accept(ssl) == -1)
        {
            perror("accept");
            close(new_fd);
            break;
        }
        bzero(buf, MAXBUF + 1);
        strcpy(buf, "server->client");
        len = SSL_write(ssl, buf, strlen(buf));
        if (len <= 0)
        {
            printf("消息'%s'发送失败！错误代码是%d，错误信息是'%s'\n",
                   buf, errno, strerror(errno));
            goto finish;
        }
        else
        {
            printf("消息'%s'发送成功，共发送了%d个字节！\n",
                   buf, len);
        }
        bzero(buf, MAXBUF + 1);
        len = SSL_read(ssl, buf, MAXBUF);
        if (len > 0)
        {
            printf("接收消息成功:'%s'，共%d个字节的数据\n",
                   buf, len);
        }
        else
        {
            printf("消息接收失败！错误代码是%d，错误信息是'%s'\n",
                   errno, strerror(errno));
        }
finish:
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(new_fd);
    }
    close(sockfd);
    SSL_CTX_free(ctx);
    return 0;
}
void ShowCerts1(SSL* ssl)
{
    X509* cert;
    char* line;
    cert = SSL_get_peer_certificate(ssl);
    if (cert != NULL)
    {
        printf("数字证书信息:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("证书: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("颁发者: %s\n", line);
        free(line);
        X509_free(cert);
    }
    else
    {
        printf("无证书信息！\n");
    }
}
int test_client()
{
    int sockfd, len;
    struct sockaddr_in dest;
    char buffer[MAXBUF + 1];
    SSL_CTX* ctx;
    SSL* ssl;
    const char* str_ip = NULL;
    str_ip = "192.168.1.3";
    const char* path = NULL;
    path = "E:/pub/bin/ssl/openssl_exe";
    const char* aa = SSLeay_version(0);
    _chdir(path);
    sock_init();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_client_method());
    if (ctx == NULL)
    {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket");
        exit(errno);
    }
    printf("socket created\n");
    bzero(&dest, sizeof(dest));
    if (net_addr(str_ip, MYPORT, &dest) == 0)
    {
        perror(str_ip);
        exit(errno);
    }
    printf("address created\n");
    if (connect(sockfd, (struct sockaddr*) &dest, sizeof(dest)) != 0)
    {
        perror("Connect ");
        exit(errno);
    }
    printf("server connected\n");
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);
    if (SSL_connect(ssl) == -1)
    {
        ERR_print_errors_fp(stderr);
    }
    else
    {
        printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
        //ShowCerts(ssl, 3);
    }
    bzero(buffer, MAXBUF + 1);
    len = SSL_read(ssl, buffer, MAXBUF);
    if (len > 0)
    {
        printf("接收消息成功:'%s'，共%d个字节的数据\n", buffer, len);
    }
    else
    {
        printf("消息接收失败！错误代码是%d，错误信息是'%s'\n", errno, strerror(errno));
        goto finish;
    }
    bzero(buffer, MAXBUF + 1);
    strcpy(buffer, "from client->server");
    len = SSL_write(ssl, buffer, strlen(buffer));
    if (len < 0)
    {
        printf("消息'%s'发送失败！错误代码是%d，错误信息是'%s'\n", buffer, errno, strerror(errno));
    }
    else
    {
        printf("消息'%s'发送成功，共发送了%d个字节！\n", buffer, len);
    }
finish:
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sockfd);
    SSL_CTX_free(ctx);
    return 0;
}

