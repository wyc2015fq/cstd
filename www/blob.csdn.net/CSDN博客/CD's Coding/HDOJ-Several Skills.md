# HDOJ-Several Skills - CD's Coding - CSDN博客





2013年10月27日 18:00:15[糖果天王](https://me.csdn.net/okcd00)阅读数：450













Q How to use 64-bit integer types ?


A Server supports signed and unsigned 64-bit integers.
- Signed 64-bit integer. Value range: -9223372036854775808 .. 9223372036854775807.
|Language|GCC/G++|Pascal|C/C++|
|----|----|----|----|
|Type name|__int64orlong long|int64|__int64|
|Input|scanf("%I64d", &x);orcin >> x;|read(x);|scanf("%I64d", &x);|
|Output|printf("%I64d", x);|cout << x; write(x);|printf("%I64d", x);|

- Unsigned 64-bit integer. Value range: 0 .. 18446744073709551615.
|Language|GCC/G++|Pascal|C/C++|
|----|----|----|----|
|Type name|unsigned __int64orunsigned long long|qword|unsigned __int64|
|Input|scanf("%I64u", &x);orcin >> x;|read(x);|scanf("%I64u", &x);|
|Output|printf("%I64u", x);or cout << x;|write(x);|printf("%I64u", x);|




Q How can I read input data until the end of file ?


A You can do it for example like this:
|Language|C|C++|Pascal|
|----|----|----|----|
|To read numbers|int n;while(scanf("%d", &n) != EOF){　　...}|int n;while (cin >> n){　　...}|var n: integer;...while not seekeof dobegin　　read(n);　　...end;|
|To read characters|int c;while ((c = getchar()) != EOF){　　...}|char c;while (cin.get(c)){　　...}|var c: char;...while not eof dobegin　　read(c); 　　...end;|
|To read lines|char line[1024];while(gets(line)){　　...}|string line;while (getline(cin, line)){　　...}|var line: string;...while not eof dobegin　　readln(line);　　...end;|









