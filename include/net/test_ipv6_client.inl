// for linux http://blog.csdn.net/caspiansea/article/details/29779655
// code source from windows: https://msdn.microsoft.com/ja-jp/library/windows/desktop/ms738630(v=vs.85).aspx
#undef UNICODE
#define DEFAULT_SERVER NULL // Will use the loopback interface
#define DEFAULT_FAMILY PF_UNSPEC // Accept either IPv4 or IPv6
#define DEFAULT_SOCKTYPE SOCK_STREAM // TCP
#define DEFAULT_PORT "5001" // Arbitrary, albiet a historical test port
#define DEFAULT_EXTRA 0 // Number of "extra" bytes to send
#undef BUFFER_SIZE
#define BUFFER_SIZE 65536
#define UNKNOWN_NAME "<unknown>"
void Usage_c(char* ProgName)
{
  printf("usage exit:%s\n", ProgName);
  WSACleanup();
  exit(1);
}
int ReceiveAndPrint(SOCKET ConnSocket, char* Buffer, int BufLen)
{
  int AmountRead;
  printf("***Ready to recv()***\n");
  AmountRead = recv(ConnSocket, Buffer, BufLen, 0);
  if (AmountRead == SOCKET_ERROR) {
    fprintf(stderr, "recv() failed with error %d: %s\n",
        WSAGetLastError(), PrintError(WSAGetLastError()));
    closesocket(ConnSocket);
    WSACleanup();
    exit(1);
  }
  // We are not likely to see this with UDP, since there is no
  // 'connection' established.
  if (AmountRead == 0) {
    printf("Server closed connection\n");
    closesocket(ConnSocket);
    WSACleanup();
    exit(0);
  }
  printf("Received %d bytes from server: [%.*s]\n",
      AmountRead, AmountRead, Buffer);
  return AmountRead;
}
int test_ipv6_client(int argc, char** argv)
{
  char Buffer[BUFFER_SIZE], AddrName[NI_MAXHOST];
  char* Server = DEFAULT_SERVER;
  int Family = DEFAULT_FAMILY;
  int SocketType = DEFAULT_SOCKTYPE;
  char* Port = DEFAULT_PORT;
  int i, RetVal, AddrLen, AmountToSend = 0;
  int ExtraBytes = DEFAULT_EXTRA;
  unsigned int Iteration, MaxIterations = 10;
  BOOL RunForever = FALSE;
  //addrinfo
  struct addrinfo Hints, *AddrInfo, *AI;
  SOCKET ConnSocket = INVALID_SOCKET;
  struct sockaddr_storage Addr;
  if (argc > 1) {
    for (i = 1; i < argc; i++) {
      if (((argv[i][0] == '-') || (argv[i][0] == '/')) && (argv[i][1] != 0) && (argv[i][2] == 0)) {
        switch (tolower(argv[i][1])) {
        case 'f':
          printf("case f\n");
          if (!argv[i + 1]) {
            Usage_c(argv[0]);
            printf("!argv[%s + 1]==0\n", i);
          }
          //====================
          if (!STRICMP(argv[i + 1], "PF_INET")) {
            Family = PF_INET;
          }
          else if (!STRICMP(argv[i + 1], "AF_INET")) {
            Family = PF_INET;
          }
          else if (!STRICMP(argv[i + 1], "PF_INET6")) {
            Family = PF_INET6;
          }
          else if (!STRICMP(argv[i + 1], "AF_INET6")) {
            Family = PF_INET6;
          }
          else if (!STRICMP(argv[i + 1], "PF_UNSPEC")) {
            Family = PF_UNSPEC;
          }
          else if (!STRICMP(argv[i + 1], "AF_UNSPEC")) {
            Family = PF_UNSPEC;
          }
          else {
            printf("argv[%s], else exit\n", i + 1);;
            Usage_c(argv[0]);
          }
          //========================
          i++;
          break;
        case 't':
          printf("case t\n");
          if (!argv[i + 1]) {
            Usage_c(argv[0]);
          }
          if (!STRICMP(argv[i + 1], "TCP")) {
            SocketType = SOCK_STREAM;
          }
          else if (!STRICMP(argv[i + 1], "UDP")) {
            SocketType = SOCK_DGRAM;
          }
          else {
            Usage_c(argv[0]);
          }
          i++;
          break;
        case 's':
          printf("case s\n");
          if (argv[i + 1]) {
            if (argv[i + 1][0] != '-') {
              Server = argv[++i];
              break;
            }
          }
          Usage_c(argv[0]);
          break;
        case 'p':
          printf("case p\n");
          if (argv[i + 1]) {
            if (argv[i + 1][0] != '-') {
              Port = argv[++i];
              break;
            }
          }
          Usage_c(argv[0]);
          break;
        case 'b':
          printf("case b\n");
          if (argv[i + 1]) {
            if (argv[i + 1][0] != '-') {
              ExtraBytes = atoi(argv[++i]);
              if (ExtraBytes > sizeof(Buffer) - sizeof("Message #4294967295")) {
                Usage_c(argv[0]);
              }
              break;
            }
          }
          Usage_c(argv[0]);
          break;
        case 'n':
          printf("case n\n");
          if (argv[i + 1]) {
            if (argv[i + 1][0] != '-') {
              MaxIterations = atoi(argv[++i]);
              break;
            }
          }
          RunForever = TRUE;
          break;
        default:
          printf("case default\n");
          Usage_c(argv[0]);
          break;
        }
      }
      else {
        printf("case else\n");
        Usage_c(argv[0]);
      }
    }
  }
  // Ask for Winsock version 2.2.
  printf("Ask for Winsock version 2.2.\n");
  // By not setting the AI_PASSIVE flag in the hints to getaddrinfo, we're
  // indicating that we intend to use the resulting address(es) to connect
  // to a service. This means that when the Server parameter is NULL,
  // getaddrinfo will return one entry per allowed protocol family
  // containing the loopback address for that family.
  memset(&Hints, 0, sizeof(Hints));
  Hints.ai_family = Family;
  Hints.ai_socktype = SocketType;
  RetVal = getaddrinfo(Server, Port, &Hints, &AddrInfo);
  if (RetVal != 0) {
    //fprintf(stderr,
    // "Cannot resolve address [%s] and port [%s], error %d: %s\n",
    // Server, Port, RetVal, gai_strerror(RetVal));
    //
    //!!!!!!!!!!The fucking stupid gai_strerror() function!!
    WSACleanup();
    return -1;
  }
  //
  // Try each address getaddrinfo returned, until we find one to which
  // we can successfully connect.
  //
  for (AI = AddrInfo; AI != NULL; AI = AI->ai_next) {
    // Open a socket with the correct address family for this address.
    ConnSocket = socket(AI->ai_family, AI->ai_socktype, AI->ai_protocol);
    //**** DEBUG
    printf("socket call with family: %d socktype: %d, protocol: %d\n",
        AI->ai_family, AI->ai_socktype, AI->ai_protocol);
    if (ConnSocket == INVALID_SOCKET) {
      printf("socket call failed with %d\n", WSAGetLastError());
    }
    //**** DEBUG END
    if (ConnSocket == INVALID_SOCKET) {
      fprintf(stderr, "Error Opening socket, error %d: %s\n",
          WSAGetLastError(), PrintError(WSAGetLastError()));
      continue;
    }
    //
    // Notice that nothing in this code is specific to whether we
    // are using UDP or TCP.
    //
    // When connect() is called on a datagram socket, it does not
    // actually establish the connection as a stream (TCP) socket
    // would. Instead, TCP/IP establishes the remote half of the
    // (LocalIPAddress, LocalPort, RemoteIP, RemotePort) mapping.
    // This enables us to use send() and recv() on datagram sockets,
    // instead of recvfrom() and sendto().
    //
    printf("Attempting to connect to: %s\n", Server ? Server : "localhost");
    if (connect(ConnSocket, AI->ai_addr, (int) AI->ai_addrlen) != SOCKET_ERROR) {
      break;
    }
    i = WSAGetLastError();
    if (getnameinfo(AI->ai_addr, (int) AI->ai_addrlen, AddrName, sizeof(AddrName), NULL, 0, NI_NUMERICHOST) != 0) {}
    //strcpy_s(AddrName, sizeof (AddrName), UNKNOWN_NAME);
    //fprintf(stderr, "connect() to %s failed with error %d: %s\n",AddrName, i, PrintError(i));
    closesocket(ConnSocket);
  }
  if (AI == NULL) {
    fprintf(stderr, "Fatal error: unable to connect to the server.\n");
    WSACleanup();
    return -1;
  }
  //
  // This demonstrates how to determine to where a socket is connected.
  //
  AddrLen = sizeof(Addr);
  if (getpeername(ConnSocket, (LPSOCKADDR) & Addr, (int*) &AddrLen) == SOCKET_ERROR) {
    fprintf(stderr, "getpeername() failed with error %d: %s\n",
        WSAGetLastError(), PrintError(WSAGetLastError()));
  }
  else {
    if (getnameinfo((LPSOCKADDR) & Addr, AddrLen, AddrName, sizeof(AddrName), NULL, 0, NI_NUMERICHOST) != 0) {}
    // strcpy_s(AddrName, sizeof (AddrName), UNKNOWN_NAME);
    //printf("Connected to %s, port %d, protocol %s, protocol family %s\n",
    // AddrName, ntohs(SS_PORT(&Addr)),
    // (AI->ai_socktype == SOCK_STREAM) ? "TCP" : "UDP",
    // (AI->ai_family == PF_INET) ? "PF_INET" : "PF_INET6");
  }
  // We are done with the address info chain, so we can free it.
  freeaddrinfo(AddrInfo);
  //
  // Find out what local address and port the system picked for us.
  //
  AddrLen = sizeof(Addr);
  if (getsockname(ConnSocket, (LPSOCKADDR) & Addr, &AddrLen) == SOCKET_ERROR) {
    fprintf(stderr, "getsockname() failed with error %d: %s\n",
        WSAGetLastError(), PrintError(WSAGetLastError()));
  }
  else {
    if (getnameinfo((LPSOCKADDR) & Addr, AddrLen, AddrName,
        sizeof(AddrName), NULL, 0, NI_NUMERICHOST) != 0) {}
    // strcpy_s(AddrName, sizeof (AddrName), UNKNOWN_NAME);
    //printf("Using local address %s, port %d\n",
    // AddrName, ntohs(SS_PORT(&Addr)));
  }
  Buffer[0] = 0;
  //
  // Send and receive in a loop for the requested number of iterations.
  //
  for (Iteration = 0; RunForever || Iteration < MaxIterations; Iteration++) {
    // Compose a message to send.
    //AmountToSend = sprintf(Buffer, sizeof (Buffer), "Message #%u", Iteration + 1);
    AmountToSend = 0;
    for (i = 0; i < ExtraBytes; i++) {
      Buffer[AmountToSend++] = (char)((i & 0x3f) + 0x20);
    }
    // Send the message. Since we are using a blocking socket, this
    // call shouldn't return until it's able to send the entire amount.
    RetVal = send(ConnSocket, "Fucking japan", 20, 0);
    //RetVal = send(ConnSocket, "gogogo!!!!!!!!!!!!!!!!!", AmountToSend, 0);
    //RetVal = send(ConnSocket, Buffer, AmountToSend, 0);
    if (RetVal == SOCKET_ERROR) {
      fprintf(stderr, "send() failed with error %d: %s\n",
          WSAGetLastError(), PrintError(WSAGetLastError()));
      WSACleanup();
      return -1;
    }
    printf("Sent %d bytes (out of %d bytes) of data: [%.*s]\n",
        RetVal, AmountToSend, AmountToSend, Buffer);
    // Clear buffer just to prove we're really receiving something.
    memset(Buffer, 0, sizeof(Buffer));
    // Receive and print server's reply.
    ReceiveAndPrint(ConnSocket, Buffer, sizeof(Buffer));
  }
  // Tell system we're done sending.
  printf("***Done sending***\n");
  shutdown(ConnSocket, SD_SEND);
  printf("***after shutdown***\n");
  // Since TCP does not preserve message boundaries, there may still
  // be more data arriving from the server. So we continue to receive
  // data until the server closes the connection.
  ReceiveAndPrint(ConnSocket, Buffer, sizeof(Buffer));
  printf("***after ReceiveAndPrint***\n");
  // if (SocketType == SOCK_STREAM)
  // while (ReceiveAndPrint(ConnSocket, Buffer, sizeof (Buffer)) != 0) ;
  closesocket(ConnSocket);
  WSACleanup();
  return 0;
}

