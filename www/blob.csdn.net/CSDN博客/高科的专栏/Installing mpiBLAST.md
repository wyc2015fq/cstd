
# Installing mpiBLAST - 高科的专栏 - CSDN博客

2011年09月06日 10:16:36[高科](https://me.csdn.net/pbymw8iwm)阅读数：755标签：[structure																](https://so.csdn.net/so/search/s.do?q=structure&t=blog)[statistics																](https://so.csdn.net/so/search/s.do?q=statistics&t=blog)[function																](https://so.csdn.net/so/search/s.do?q=function&t=blog)[patch																](https://so.csdn.net/so/search/s.do?q=patch&t=blog)[command																](https://so.csdn.net/so/search/s.do?q=command&t=blog)[file																](https://so.csdn.net/so/search/s.do?q=file&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=command&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=patch&t=blog)个人分类：[linux																](https://blog.csdn.net/pbymw8iwm/article/category/863753)
[
																								](https://so.csdn.net/so/search/s.do?q=patch&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=function&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=function&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=statistics&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=structure&t=blog)
# Installing mpiBLAST
This is part one of a two-part tutorial on[
mpiBLAST](http://debianclusters.org/index.php/MpiBLAST:_Nucleotide/Protein_Searching). The full tutorial includes
Installing mpiBLAST
[Using mpiBLAST](http://debianclusters.org/index.php/Using_mpiBLAST)
## Getting mpiblast
mpiblast is freely available from the site at[
http://www.mpiblast.org/Downloads/Stable](http://www.mpiblast.org/Downloads/Stable). At the time of this writing, the most recent version is 1.5.0, but you may want to check yourself as versions are subject to change. From your source code directory, run
`wget http://www.mpiblast.org/downloads/files/mpiBLAST-1.5.0-pio.tgz`or the location for the most recent version if there's one newer. When it's finished, untar the file with
`tar xvzf mpiBLAST*.tgz`This will create a new directory, mpiblast-something. Cd into that directory. Inside this directory, mpiblast needs another file to be downloaded: a specific version of the NCBI toolkit. Do this with
`wget ftp://ftp.ncbi.nih.gov/toolbox/ncbi_tools/old/20061015/ncbi.tar.gz`and then untar it with
`tar xvzf ncbi.tar.gz`
## Building mpiblast
### NCBI Toolkit
First, you'll need to patch and build the NCBI toolkit. I'm not completely sure that the patch needs to be applied (or does anything to this older version of the NCBI toolkit), but it certainly didn't hurt in my installation. To patch it, cd into the`ncbi`directory and run
`patch -p1 < ../patches/ncbi_Oct2006_evalue_pio.patch`Then, cd back up one level to the mpiblast folder (`cd ..`) and run
`./ncbi/make/makedis.csh`(**Note:**If you receive an error about CSH, you don't have the C shell installed, but that's easy to fix with`apt-get install csh`.)
The above command (makedis.csh) needs to be run three times. The first time you run, it will finish with an error:
make: *** No rule to make target `ncbimain.o', needed by `libncbi.a'.  Stop.
Fatal error building NCBI core libraries.
Please be sure that you have X11 and Motif libraries installed.
The NCBI toolkit FAQ at ftp://ftp.ncbi.nih.gov/toolbox/FAQ.html may be helpful.Run the same command again,
`./ncbi/make/makedis.csh`even though it finished with an error. This time, it will take quite a bit longer, and it will finish with a different error:
make: *** No rule to make target `ni_debug.o', needed by `libnetcli.a'.  Stop.
FAILURE primary make status = 0, demo = 0, threaded_demo = 0, net = 2
\#\#\#\#\#\#\#
\#        \#\#\#\#\#   \#\#\#\#\#    \#\#\#\#   \#\#\#\#\#
\#        \#    \#  \#    \#  \#    \#  \#    \#
\#\#\#\#\#    \#    \#  \#    \#  \#    \#  \#    \#
\#        \#\#\#\#\#   \#\#\#\#\#   \#    \#  \#\#\#\#\#
\#        \#   \#   \#   \#   \#    \#  \#   \#
\#\#\#\#\#\#\#  \#    \#  \#    \#   \#\#\#\#   \#    \#Run
`./ncbi/make/makedis.csh`one last time, and it should finish successfully:
Put the date stamp to the file ../VERSION
*********************************************************
*The new binaries are located in ./ncbi/build/ directory*
*********************************************************Then it's safe to continue on to building mpiblast itself.
### mpiblast
From here, mpiblast follows the standard[
source installation paradigm](http://debianclusters.org/index.php/Source_Installation_Paradigm)and there shouldn't be many surprises. (See the[
Source Installation Paradigm](http://debianclusters.org/index.php/Source_Installation_Paradigm)page for a more gentle introduction on installation of software from source.) Run`./configure --help`to see all the possible options.
Important options include
--prefix=- used to specify where the files should be installed. These files will need to be accessible by all of the worker nodes. A good place would be an[
NFS mount](http://debianclusters.org/index.php/Mounted_File_System:_NFS)
--with-ncbi=- specifies where the NCBI toolkit is found. An easy way to tell it to use "the ncbi directory within the current directory" is to use--with-ncbi=`pwd`/ncbi
--enable-MPI_Alloc_mem- this is a new feature in MPI 2. (If you followed my[
MPICH tutorial](http://debianclusters.org/index.php/MPICH:_Parallel_Programming), you installed MPICH2.) If you are using a current implementation, you'll want to enable this for performance.
My NFS mount is located at`/shared`. The ./configure line I ran was
`./configure --prefix=/shared --with-ncbi=`pwd`/ncbi --enable-MPI_Alloc_mem`Once it finishes, it's time to run`make`. Make should finish successfully, unless you grabbed the wrong version of the NCBI toolkit. In my experience, the newer version didn't work - you specifically need the older version of the NCBI toolkit.
 With the newer version, you'll see errors like this:
blast_hooks.c: In function `initBLAST':
blast_hooks.c:757: error: structure has no member named `query_adjustments'
blast_hooks.c:758: error: structure has no member named `effective_db_lengths'
blast_hooks.c: In function `getQuery':
blast_hooks.c:836: error: structure has no member named `current_queryI'
blast_hooks.c: In function `writeSearchStatistics':
blast_hooks.c:1332: error: structure has no member named `stats'
blast_hooks.c: In function `runBLAST':
blast_hooks.c:1455: error: structure has no member named `calculate_statistics_and_exit'
blast_hooks.c:1457: error: structure has no member named `calculate_statistics_and_exit'
blast_hooks.c:1484: error: structure has no member named `current_queryI'
blast_hooks.c: In function `runBLASTPIO':
blast_hooks.c:1552: error: structure has no member named `calculate_statistics_and_exit'Go back and wget the older NCBI toolkit and build it again, then run ./configure and make again. This time, it should work.
Once make finishes, it's time to run`make install`.
## Sanity Check
It's always a good idea to make sure that the mpiblast binaries have been installed and are in the users' PATHs. As a user, run
`which mpiblast`and it should return the location of the mpiblast binary, if that file is in the user's path. If it isn't, first install locate if you haven't already (`apt-get install locate`) and then you can run
`updatedb``locate mpiblast | grep -v src`Thegrep -v srcpart excludes any directories or files with the word "src" in them. If you keep your source files in a directory named differently, update that section. Otherwise, you'll find all of the source code as part of the search.
Once you've found the binary, you can[
update user's PATHs](http://debianclusters.org/index.php/Bash_Profile_Modifications#PATH_Additions)accordingly.


