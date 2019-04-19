# Colossus: Successor to the Google File System (GFS) - =朝晖= - 博客园
# [Colossus: Successor to the Google File System (GFS)](https://www.cnblogs.com/dhcn/p/7389645.html)
Colossus is the successor to the [Google](https://www.systutorials.com/tag/google/) File [System](https://www.systutorials.com/tag/system/) (GFS) as mentioned in the recent [paper on Spanner](http://research.google.com/archive/spanner.html) on OSDI 2012. Colossus is also used by spanner to store its tablets. The information about Colossus is slim compared with GFS which is published in [the paper on SOSP 2003](http://research.google.com/archive/gfs.html). There is still some information about Colossus on the Web. Here, I list some of them.
## Storage Architecture and Challenges
On Faculty Summit, July 29, 2010, by Andrew Fikes, Principal Engineer.
[The slides](https://www.systutorials.com/3306/storage-architecture-and-challenges/). Some interesting points:
> 
- Storage [Software](https://www.systutorials.com/category/tutorial/software/): Colossus
- Next-generation cluster-level file system
- Automatically sharded metadata layer
- Data typically written using Reed-Solomon (1.5x)
- Client-driven replication, encoding and replication
- Metadata space has enabled availability analyses
- Why Reed-Solomon?
- Cost. Especially w/ cross cluster replication.
- Field data and simulations show improved MTTF
- More flexible cost vs. availability choices
## GFS: Evolution on Fast-forward
An interview with Google’s Sean Quinlan by the Association for Computer Machinery (ACM).
[View the interview](http://queue.acm.org/detail.cfm?id=1594206).
Some important info:
> - “We also ended up doing what we call a “multi-cell” approach, which basically made it possible to put multiple GFS masters on top of a pool of chunkservers.”
- “We also have something we called Name Spaces, which are just a very static way of partitioning a namespace that people can use to hide all of this from the actual application.” … “a namespace file describes”
- “The distributed master certainly allows you to grow file counts, in line with the number of machines you’re willing to throw at it.” … “Our distributed master system that will provide for 1-MB files is essentially a whole new design. That way, we can aim for something on the order of 100 million files per master. You can also have hundreds of masters.”
- BitTable “as one of the major adaptations made along the way to help keep GFS viable in the face of rapid and widespread change.”
## Google File System II: Dawn of the Multiplying Master Nodes Comments on GFS2 (colossus)
by Cade Metz in San Francisco.
[The article](http://www.theregister.co.uk/2009/08/12/google_file_system_part_deux/) and [some excerpt](http://tab.d-thinker.org/showthread.php?tid=1&pid=332#pid332).

