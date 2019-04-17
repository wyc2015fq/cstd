# [odb-users] query results not being cached? - DoubleLi - 博客园






**Burton, Craig**[crburton at tnsi.com](mailto:odb-users%40codesynthesis.com?Subject=%5Bodb-users%5D%20query%20results%20not%20being%20cached%3F&In-Reply-To=boris.20120606085947%40codesynthesis.com)
*Wed Jun 6 13:58:03 EDT 2012*


- Previous message: [[odb-users] query results not being cached?](http://www.codesynthesis.com/pipermail/odb-users/2012-June/000588.html)
- Next message: [[odb-users] query results not being cached?](http://www.codesynthesis.com/pipermail/odb-users/2012-June/000594.html)
- **Messages sorted by:**[[ date ]](http://www.codesynthesis.com/pipermail/odb-users/2012-June/date.html#591)[[ thread ]](http://www.codesynthesis.com/pipermail/odb-users/2012-June/thread.html#591)[[ subject ]](http://www.codesynthesis.com/pipermail/odb-users/2012-June/subject.html#591)[[ author ]](http://www.codesynthesis.com/pipermail/odb-users/2012-June/author.html#591)
Hi Boris,

My apologies; I see now (with some embarrassment) that this specific Oracle limitation is well documented.

Under many circumstances, our application expects to find exactly zero or one instance for many specific queries, but finding two or more would lead to an error scenario.  Application logic is the following in many such cases:

        if zero instances, create a new one
        if one instance, update the one found in the db
        if two or more, do nothing and return an error

Your alternative approach using the view works well for my needs, since I can determine the count and then perform the query if there is, indeed, only one instance.  I don't see a clean way to do the same by only iterating over the result, unless the iteration determines the count and temporarily holds a reference to the single instance (if there is only one).  I'll continue to try a few different approaches to see which one is most compatible with our current implementation.

Thanks again for your help!

Craig

-----Original Message-----
From: Boris Kolpackov [mailto:[boris at codesynthesis.com](http://codesynthesis.com/mailman/listinfo/odb-users)]
Sent: Wednesday, June 06, 2012 12:15 AM
To: Burton, Craig
Cc: [odb-users at codesynthesis.com](http://codesynthesis.com/mailman/listinfo/odb-users)
Subject: Re: [odb-users] query results not being cached?

Hi Craig,

Burton, Craig <[crburton at tnsi.com](http://codesynthesis.com/mailman/listinfo/odb-users)> writes:

>* I am not able to call "size()" on query results even if I try to use*>* the "cache()" method on the results template.*
For some databases (right now Oracle, SQLite, and MS SQL Server),
caching (and therefore size()) is not supported. This is documented
in the database-specific chapters. For Oracle that would be Section
16.5.2, "Query Result Caching":

[http://www.codesynthesis.com/products/odb/doc/manual.xhtml#16.5.2](http://www.codesynthesis.com/products/odb/doc/manual.xhtml#16.5.2)

If you are interested, in the case or Oracle, supporting size() would
require switching the query result cursor to the scrollable mode. This
results in a much worse performance compared to the forward-only mode.
There is also a long and complicated list of limitations of scrollable
cursors that stipulate the kind of situations that are not supported
(specifically, support for BLOB/LONG types is very limited).

So it is better to try not to rely on knowing in advance the number
of entries in the result. Specifically, any optimizations that you
may want to make (e.g., reserve the space in the vector, etc), will
be inconsequential compared to the cost of supporting size(). If
you really need to know the size prior to the iteration, then the
best you can do is probably run a separate query that returns the
count, for example:

#pragma db view object(MyPersistentClass)
struct MyPersistentClassCount
{
  #pragma db column("count(*)")
  std::size_t count;
};

size_t count = ora_db->query<MyPersistentClassCount> (pred)->begin ()->count;

Boris

This e-mail message is for the sole use of the intended recipient(s)and may
contain confidential and privileged information of Transaction Network Services.
Any unauthorised review, use, disclosure or distribution is prohibited. If you
are not the intended recipient, please contact the sender by reply e-mail and destroy all copies of the original message.




- Previous message: [[odb-users] query results not being cached?](http://www.codesynthesis.com/pipermail/odb-users/2012-June/000588.html)
- Next message: [[odb-users] query results not being cached?](http://www.codesynthesis.com/pipermail/odb-users/2012-June/000594.html)
- **Messages sorted by:**[[ date ]](http://www.codesynthesis.com/pipermail/odb-users/2012-June/date.html#591)[[ thread ]](http://www.codesynthesis.com/pipermail/odb-users/2012-June/thread.html#591)[[ subject ]](http://www.codesynthesis.com/pipermail/odb-users/2012-June/subject.html#591)[[ author ]](http://www.codesynthesis.com/pipermail/odb-users/2012-June/author.html#591)

[More information about the odb-users mailing list](http://codesynthesis.com/mailman/listinfo/odb-users)









