# Create schema error (unknown database schema '') - DoubleLi - 博客园








Andrey Devyatka

4 years ago



[Permalink](http://narkive.com/3Ms11hIV.1)
Raw Message


Hi,
Please tell me, can I use the static library in the following case:

library.hpp:
#ifndef _CPP_ODB_STATIC_LIBRARY_CPP_
#define _CPP_ODB_STATIC_LIBRARY_CPP_
#include <odb/database.hxx>
odb::database* createDb(void);
#pragma db object
struct Test
{
#pragma db id auto
int m_id;
};
#endif // _CPP_ODB_STATIC_LIBRARY_CPP_

library.cpp:
#include <odb/sqlite/database.hxx>
#include <odb/transaction.hxx>
#include <odb/schema-catalog.hxx>
#include "library.hpp"
odb::database* createDb(void)
{
odb::database* db = new odb::sqlite::database("test.db",
SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
odb::transaction t(db->begin());
odb::schema_catalog::create_schema(*db);
t.commit();
return db;
}

main.cpp:
include <iostream>
#include <odb/exception.hxx>
#include "library.hpp"
int main()
{
try {
odb::database* db = createDb();
std::cout << "SUCCESS" << std::endl;
}catch(const odb::exception& ex){
std::cout << "ERROR: " << ex.what() << std::endl;
}
}

build:
$ odb --database sqlite --generate-schema library.hpp
$ g++ -c library-odb.cxx library.cpp main.cpp
$ ar cr library.a library.o library-odb.o
$ g++ -o test1 library.o library-odb.o main.o -lodb-sqlite -lodb
$ g++ -o test2 main.o library.a -lodb-sqlite -lodb

test:
$ ./test1
SUCCESS
$ ./test2
ERROR: unknown database schema ''

misc:
$ uname -srm
Linux 3.8.0-23-generic x86_64
$ g++ --version | head -n1
g++ (Ubuntu/Linaro 4.7.3-1ubuntu1) 4.7.3
$ odb --version | head -n1
ODB object-relational mapping (ORM) compiler for C++ 2.2.0

Thanks!

--
Regards,
Andrey






Boris Kolpackov

4 years ago



[Permalink](http://narkive.com/3Ms11hIV.2)
Raw Message



Hi Andrey,

[*Post by Andrey Devyatka*](http://odb-users.codesynthesis.narkive.com/3Ms11hIV/create-schema-error-unknown-database-schema#post1)
[...]


This is exactly the same issue that we've discussed just a few days
ago:

http://www.codesynthesis.com/pipermail/odb-users/2013-May/001286.html

Because your application doesn't directly reference any symbols from
library-odb.cxx, the linker ignores library-odb.o from library.a. As
a result the schema creation code does not end up in the executable.

Normally this is not a problem since most application executables
will also include code that persists objects, etc., which will
force the linker to include all the object files from the library.

In your test, however, all you do is create the schema. As mentioned
in the above email, with GNU ld you can use the --whole-archive
option to force the linker to include every object file from your

[*Post by Andrey Devyatka*](http://odb-users.codesynthesis.narkive.com/3Ms11hIV/create-schema-error-unknown-database-schema#post1)
$ g++ -o test2 main.o library.a -lodb-sqlite -lodb


g++ -o test2 main.o -Wl,-whole-archive library.a -Wl,-no-whole-archive -lodb-sqlite -lodb

Another thing that you may find useful is the 'separate' value for
the --schema-format option. It will trigger the generation of the
schema creation code as a separate C++ source file (library-schema.cxx).
You can then perhaps link it directly to your executable instead of
packaging it into a static library. See the ODB compiler command line
documentation (man pages) for more information on this option.

Boris







Andrey Devyatka

4 years ago



[Permalink](http://narkive.com/3Ms11hIV.3)
Raw Message



Thank you!

...

--
С уважением,
А. П. Девятка.







from:http://odb-users.codesynthesis.narkive.com/3Ms11hIV/create-schema-error-unknown-database-schema









