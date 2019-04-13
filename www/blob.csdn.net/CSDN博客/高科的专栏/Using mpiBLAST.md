
# Using mpiBLAST - 高科的专栏 - CSDN博客

2011年09月06日 10:16:00[高科](https://me.csdn.net/pbymw8iwm)阅读数：691标签：[database																](https://so.csdn.net/so/search/s.do?q=database&t=blog)[file																](https://so.csdn.net/so/search/s.do?q=file&t=blog)[permissions																](https://so.csdn.net/so/search/s.do?q=permissions&t=blog)[input																](https://so.csdn.net/so/search/s.do?q=input&t=blog)[download																](https://so.csdn.net/so/search/s.do?q=download&t=blog)[parallel																](https://so.csdn.net/so/search/s.do?q=parallel&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=download&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=input&t=blog)个人分类：[linux																](https://blog.csdn.net/pbymw8iwm/article/category/863753)
[
																								](https://so.csdn.net/so/search/s.do?q=input&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=permissions&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=permissions&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=file&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=database&t=blog)
# Using mpiBLAST
This is part one of a two-part tutorial on[
mpiBLAST](http://debianclusters.org/index.php/MpiBLAST:_Nucleotide/Protein_Searching). The full tutorial includes
[Installing mpiBLAST](http://debianclusters.org/index.php/Installing_mpiBLAST)
Using mpiBLAST
## Organization and Setup
Before downloading a bunch of databases to use with mpiblast, it'd be a good idea to make a decision about where the databases are going to be kept. Each user will need to have a profile for it in their home directory telling mpiblast where to find the databases
 and also where to find work space. These need to be available to all of the worker nodes, so putting the files on an[
NFS mount](http://debianclusters.org/index.php/Mounted_File_System:_NFS)would be a good idea.
Optional components are often kept under`/opt`. My NFS mount is at`/shared`, so I'll be putting all of my databases into`/shared/opt/mpiblast`.
This directory needs to be writable by users so that they can format the databases to be run over different numbers of processes. There are a few different ways to do this. The directory could be set to be world-writeable, so that anyone can write to the
 directory. This is done with`chmod a+w`. Another way would be to change the permissions to be group-writeable (`chmod g+w`), create a new group (`addgroup mpigroup`), add all the mpiblast users to that group, and change the
 owner of the directory to that group (`chown mpigroup:mpigroup`). The second method is more secure, but may take more work to setup the new group and then add the users to the group using[
LDAP](http://debianclusters.org/index.php/User_Authentication:_LDAP).
As a third option, you could let users download their own database files and set mpiblast to point within the users' home directories using the .ncbirc file (explained below).
### .ncbirc File
The file each mpiblast user needs in his/her home directory is called`.ncbirc`. It uses the same format as BLAST. The Shared variable refers to where the databases are kept. The Local variable refers to any local disk space used as a workspace;
 this can be part of the user's home directory. If you're using an NFS-mount, and/or your users' home directories are NFS-mounted, this will probably be in the same directory or in similar directories.
Here's an example .ncbirc file for the user kwanous:
[mpiBLAST]
Shared=/shared/opt/mpiblast
Local=/shared/home/kwanous/mpiblast
### Getting Databases
Nucleotide and protein databases are available from the National Institute of Health (NIH). A list of what the different databases contained is available as part of the[
Blast tutorial](http://www.ncbi.nlm.nih.gov/Education/blasttutorial.html)and the files themselves are at[
ftp://ftp.ncbi.nlm.nih.gov/blast/db/](ftp://ftp.ncbi.nlm.nih.gov/blast/db/). The files under the FASTA directory are pre-formatted for FASTA, and these can be used as well.
To download a file, find the location for the one you want on the FTP site. Right-click it and copy the location of the file. Then, from wherever you're keeping your databases, use`wget`. For instance, to download the*Drosophila melanogaster'*(fruit fly) nucleotide database, run
`wget ftp://ftp.ncbi.nlm.nih.gov/blast/db/FASTA/drosoph.nt.gz`Then you'll need to untar or just ungzip them. If they end in`.tg.gz`, use to tar to untar the file:
`tar xvzf yourfile.tg.gz`If they just end in`.gz`, they're only gzipped, so unzip them with
`gunzip yourfile.gz`
## Formatting the Database
In order to allow multiple processes to use the large database at the same time, the database needs first be broken down into smaller chunks. The program that does this is called`mpiformatdb`. It takes several arguments: one for the number of fragments to split the database into (this should be the number of processes that will be running mpiblast), one for the input file, and one to indicate whether it's a protein file
 or not.
*Note:*One of the great things about the mpiblast tools is that you can issue most of the commands followed by a space and a hyphen (like`mpiformatdb -`) and it will show you all the options for the command.
To segment the fruit fly database I just downloaded into four separate fragments, I ran
`mpiformatdb -i drosoph.nt --nfrags=4 -pF`-ispecifies my input file, drosoph.nt
--nfrags=4tells it to make four fragments
-pspecifies whether the file is a protein file or not. I put F for false, since this is a nucleotide file, not an amino acid file.
The process should look like this:
gyrfalcon:/shared/opt/mpiblast\# mpiformatdb -i drosoph.nt --nfrags=4 -pF
Reading input file
Done, read 1534943 lines
Reordering 1170 sequence entries
Breaking drosoph.nt into 4 fragments
Executing: formatdb -p F -i /tmp/reordermc8uxl -N 4 -n /shared/opt/mpiblast/drosoph.nt -o T
Removed /tmp/reordermc8uxl
Created 4 fragments.The output will be dropped into the same folder as the original file, which is why users need to have write permissions on the directory any shared databases will be kept in.
## Running mpiblast
Once the database has been formatted to support multiple processes, it's time to run mpiblast.
Mpiblast takes an input file with a sequence to query the databases against. This file is usually just a testfile with a nucleotide or amino acid sequence, depending on what kind of databases are being searched against. You can create a test input sequence
 by opening a file with your favorite text editor (like vi or nano) and pasting one in. A greater than symbol on the first line can give the name of the input sequence. For example, I'll create`test.in`with these contents:
>Test
AGCTTTTCATTCTGACTGCAACGGGCAATATGTCTCTGTGTGGATTAAAAAAAGAGTGTCTGATAGCAGC
TTCTGAACTGGTTACCTGCCGTGAGTAAATTAAAATTTTATTGACTTAGGTCACTAAATACTTTAACCAA
TATAGGCATAGCGCACAGACAGATAAAAATTACAGAGTACACAACATCCATGAAACGCATTAGCACCACC
ATTACCACCACCATCACCATTACCACAGGTAACGGTGCGGGCTGACGCGTACAGGAAACACAGAAAAAAG
CCCGCACCTGACAGTGCGGGCTTTTTTTTTCGACCAAAGGTAACGAGGTAACAACCATGCGAGTGTTGAA
GTTCGGCGGTACATCAGTGGCAAATGCAGAACGTTTTCTGCGTGTTGCCGATATTCTGGAAAGCAATGCC
AGGCAGGGGCAGGTGGCCACCGTCCTCTCTGCCCCCGCCAAAATCACCAACCACCTGGTGGCGATGATTG
AAAAAACCATTAGCGGCCAGGATGCTTTACCCAATATCAGCGATGCCGAACGTATTTTTGCCGAACTTTTMpiblast will be run just using`mpirun`or`mpiexec`, but there are a few important command line arguments. These include
-d- the database to be queried against
-i- the input file
-p- the type of blast query to run, includingblastnfor nucleotides
blastpfor proteins
-o- the name of the file to save the output in
The command run in its complete state should look something like this:
`mpiexec -np 4 /shared/bin/mpiblast -d drosoph.nt -i test.in -p blastn -o results.txt`Of course, if you're using the[
version of mpiexec with Torque functionality](http://debianclusters.org/index.php/MPICH_with_Torque_Functionality), you'll need to wrap this in a[
qsub script](http://debianclusters.org/index.php/Torque_Qsub_Scripts).
When it's finished running, you should have a new`results.txt`file that looks something like this:
BLASTN 2.2.15 [Oct-15-2006]
Reference: 
Aaron E. Darling, Lucas Carey, and Wu-chun Feng,
"The design, implementation, and evaluation of mpiBLAST".
In Proceedings of 4th International Conference on Linux Clusters: The HPC Revolution 2003, 
June 24-26 2003, San Jose, CA
Heshan Lin, Xiaosong Ma, Praveen Chandramohan, Al Geist, and Nagiza Samatova,
"Efficient Data Access for Parallel BLAST".
In Proceedings of 19th International Parallel & Distributed Processing Symposium 2005, 
April 3-8 2005, Denver, CO
Query= Test
         (560 letters)
Database: /shared/opt/mpiblast/drosoph.nt 
           1170 sequences; 122,655,632 total letters

                                                                 Score    E
Sequences producing significant alignments:                      (bits) Value
gb|AE003681.2|AE003681 Drosophila melanogaster genomic scaffold ...    36   0.86 
gb|AE002936.2|AE002936 Drosophila melanogaster genomic scaffold ...    36   0.86 
gb|AE003698.2|AE003698 Drosophila melanogaster genomic scaffold ...    36   0.86 
gb|AE003493.2|AE003493 Drosophila melanogaster genomic scaffold ...    36   0.86 
gb|AE002615.2|AE002615 Drosophila melanogaster genomic scaffold ...    34   3.4  
gb|AE003441.1|AE003441 Drosophila melanogaster genomic scaffold ...    34   3.4  
gb|AE003525.2|AE003525 Drosophila melanogaster genomic scaffold ...    34   3.4  

...snipped...

Query: 96     taaattaaaattttatt 112
              |||||||||||||||||
Sbjct: 226581 taaattaaaattttatt 226565

>gb|AE003447.2|AE003447 Drosophila melanogaster genomic scaffold 142000013386054 section 31 of
              35, complete sequence
          Length = 304085
 Score = 34.2 bits (17), Expect = 3.4
 Identities = 17/17 (100%)
 Strand = Plus / Minus
                               
Query: 377    cagaacgttttctgcgt 393
              |||||||||||||||||
Sbjct: 177093 cagaacgttttctgcgt 177077

  Database: /shared/opt/mpiblast/drosoph.nt
    Posted date:  Apr 18, 2008  2:48 PM
  Number of letters in database: 30,663,804
  Number of sequences in database:  292
  
  Database: /shared/opt/mpiblast/drosoph.nt.001
    Posted date:  Apr 18, 2008  2:48 PM
  Number of letters in database: 30,664,011
  Number of sequences in database:  293
  
  Database: /shared/opt/mpiblast/drosoph.nt.002
    Posted date:  Apr 18, 2008  2:48 PM
  Number of letters in database: 30,664,004
  Number of sequences in database:  293
  
  Database: /shared/opt/mpiblast/drosoph.nt.003
    Posted date:  Apr 18, 2008  2:48 PM
  Number of letters in database: 30,663,813
  Number of sequences in database:  292

