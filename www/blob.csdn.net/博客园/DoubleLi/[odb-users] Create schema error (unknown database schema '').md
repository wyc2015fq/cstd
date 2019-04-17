# [odb-users] Create schema error (unknown database schema '') - DoubleLi - 博客园






**Boris Kolpackov**[boris at codesynthesis.com](mailto:odb-users%40codesynthesis.com?Subject=%5Bodb-users%5D%20Create%20schema%20error%20%28unknown%20database%20schema%20%27%27%29&In-Reply-To=CAFtousN%3Dt2KRPWofcrh7DmsdcbbZtGx4WStx4qUBEDxCPzOZmQ%40mail.gmail.com)
*Fri May 31 11:13:02 EDT 2013*


- Previous message:[[odb-users] Create schema error (unknown database schema '')](http://www.codesynthesis.com/pipermail/odb-users/2013-May/001298.html)
- Next message:[[odb-users] Create schema error (unknown database schema '')](http://www.codesynthesis.com/pipermail/odb-users/2013-May/001300.html)
- **Messages sorted by:**[[ date ]](http://www.codesynthesis.com/pipermail/odb-users/2013-May/date.html#1299)[[ thread ]](http://www.codesynthesis.com/pipermail/odb-users/2013-May/thread.html#1299)[[ subject ]](http://www.codesynthesis.com/pipermail/odb-users/2013-May/subject.html#1299)[[ author ]](http://www.codesynthesis.com/pipermail/odb-users/2013-May/author.html#1299)
Hi Andrey,

Andrey Devyatka <[an9bit at gmail.com](http://codesynthesis.com/mailman/listinfo/odb-users)> writes:

>* Please tell me, can I use the static library in the following case:*>>* [...]*
This is exactly the same issue that we've discussed just a few days
ago:

[http://www.codesynthesis.com/pipermail/odb-users/2013-May/001286.html](http://www.codesynthesis.com/pipermail/odb-users/2013-May/001286.html)

Because your application doesn't directly reference any symbols from
library-odb.cxx, the linker ignores library-odb.o from library.a. As
a result the schema creation code does not end up in the executable.

Normally this is not a problem since most application executables
will also include code that persists objects, etc., which will
force the linker to include all the object files from the library.

In your test, however, all you do is create the schema. As mentioned
in the above email, with GNU ld you can use the --whole-archive
option to force the linker to include every object file from your
static library:


>* $ g++ -o test2 main.o library.a -lodb-sqlite -lodb*
g++ -o test2 main.o -Wl,-whole-archive library.a -Wl,-no-whole-archive -lodb-sqlite -lodb

Another thing that you may find useful is the 'separate' value for
the --schema-format option. It will trigger the generation of the
schema creation code as a separate C++ source file (library-schema.cxx).
You can then perhaps link it directly to your executable instead of
packaging it into a static library. See the ODB compiler command line
documentation (man pages) for more information on this option.

Boris



- Previous message:[[odb-users] Create schema error (unknown database schema '')](http://www.codesynthesis.com/pipermail/odb-users/2013-May/001298.html)
- Next message:[[odb-users] Create schema error (unknown database schema '')](http://www.codesynthesis.com/pipermail/odb-users/2013-May/001300.html)
- **Messages sorted by:**[[ date ]](http://www.codesynthesis.com/pipermail/odb-users/2013-May/date.html#1299)[[ thread ]](http://www.codesynthesis.com/pipermail/odb-users/2013-May/thread.html#1299)[[ subject ]](http://www.codesynthesis.com/pipermail/odb-users/2013-May/subject.html#1299)[[ author ]](http://www.codesynthesis.com/pipermail/odb-users/2013-May/author.html#1299)

from:http://www.codesynthesis.com/pipermail/odb-users/2013-May/001299.html









