
# python之文件管理 - .NET博文收藏 - CSDN博客


2008年06月10日 12:44:00[hejishan](https://me.csdn.net/hejishan)阅读数：676


**File Management in Python**
(Page 1 of 5 )
File management is a basic function, and an important part of many[applications](http://www.devshed.com/#). Python makes file management surprisingly easy, especially when compared to other languages. Peyton McCullough explains the basics.
**Introduction**
The game you played yesterday uses files to store game saves. The order you placed yesterday was saved in a file. That report you typed up this morning was, obviously, stored in a file as well.
File management is an important part of many applications written in nearly every language. Python is no exception to this. In this article, we will explore the task of manipulating files using several modules. We'll read, write to, append and do other strange things to files. Let's get started.
**Reading and Writing**
The most basic tasks involved in file manipulation are reading[data](http://www.devshed.com/#)from files and writing data to files. This is a very simple task to learn. Let's open a file for writing:
> fileHandle = open ( 'test.txt', 'w' )
The "w" indicates that we will be writing to the file, and the rest is pretty simple to understand. The next step is to write data to the file:
> fileHandle.write ( 'This is a test./nReally, it is.' )
This will write the string "This is a test." to the file's first line and "Really, it is." to the file's second line. Finally, we need to clean up after ourselves and close the file:
> fileHandle.close()
As you can see, it's very easy, especially with Python's object orientation. Note that when you use the "w" mode to write to the file again, all its contents will be deleted. To get past this, use the "a" mode to append data to a file, adding data to the bottom:
> fileHandle = open ( 'test.txt', 'a' )

> fileHandle.write ( '/n/n/nBottom line.' )

> fileHandle.close()
Now let's read our file and display the contents:
> fileHandle = open ( 'test.txt' )

> [print](http://www.devshed.com/#)
> fileHandle.read()

> fileHandle.close()
This will read the entire file and print the data within it. We can also read a single line in the file:
> fileHandle = open ( 'test.txt' )

> print fileHandle.readline() \# "This is a test."

> fileHandle.close()
It is also possible to store the lines of a file into a list:
> fileHandle = open ( 'test.txt' )

> fileList = fileHandle.readlines()

> for fileLine in fileList:

> print '>>', fileLine

> fileHandle.close()
When reading a file, Python's place in the file will be remembered, illustrated in this example:
> fileHandle = open ( 'test.txt' )

> garbage = fileHandle.readline()

> fileHandle.readline() \# "Really, it is."

> fileHandle.close()
Only the second line is displayed. We can, however, get past this by telling Python to resume reading from a different position:
> fileHandle = open ( 'test.txt' )

> garbage = fileHandle.readline()

> fileHandle.seek ( 0 )

> print fileHandle.readline() \# "This is a test."

> fileHandle.close()
In the above example, we tell Python to continue reading from the first byte in the file. Thus, the first line is printed. We can also request Python's place within the file:
> fileHandle = open ( 'test.txt' )

> print fileHandle.readline() \# "This is a test."

> print fileHandle.tell() \# "17"

> print fileHandle.readline() \# "Really, it is."
It is also possible to read the file a few bytes at a time:
> fileHandle = open ( 'test.txt' )

> print fileHandle.read ( 1 ) \# "T"

> fileHandle.seek ( 4 )

> print FileHandle.read ( 1 ) \# "T"
When working with Windows and Macintosh, sometimes you are required to read and write files in binary mode, such as images or executional files. To do this, simply append "b" to the file mode:
> fileHandle = open ( 'testBinary.txt', 'wb' )

> fileHandle.write ( 'There is no spoon.' )

> fileHandle.close()

> fileHandle = open ( 'testBinary.txt', 'rb' )

> print fileHandle.read()

> fileHandle.close()
Using several of Python's modules, it is possible to obtain information on existig files. To get basic information, the "[os](http://www.devshed.com/#)" module can be used in conjunction with the "stat" module:
> import os

> import stat

> import time

> fileStats = os.stat ( 'test.txt' )

> fileInfo = {

> 'Size' : fileStats [ stat.ST_SIZE ],

> 'LastModified' : time.ctime ( fileStats [ stat.ST_MTIME ] ),

> 'LastAccessed' : time.ctime ( fileStats [ stat.ST_ATIME ] ),

> 'CreationTime' : time.ctime ( fileStats [ stat.ST_CTIME ] ),

> 'Mode' : fileStats [ stat.ST_MODE ]

> }

> for infoField, infoValue in fileInfo:

> [print](http://www.devshed.com/#)
> infoField, ':' + infoValue

> if stat.S_ISDIR ( fileStats [ stat.ST_MODE ] ):

> print 'Directory. '

> else:

> print 'Non-directory.'
The above example creates a dictionary containing some basic information about the file. It then displays the data and tells us if it's a directory or not. We can also check to see if the file is one of several other types:
> import os

> import stat

> fileStats = os.stat ( 'test.txt' )

> fileMode = fileStats [ stat.ST_MODE ]

> if stat.S_ISREG ( fileStats [ stat.ST_MODE ] ):

> print 'Regular file.'

> elif stat.S_ISDIR ( fileSTats [ stat.ST_MODe ] ):

> print 'Directory.'

> elif stat.S_ISLNK ( fileSTats [ stat.ST_MODe ] ):

> print 'Shortcut.'

> elif stat.S_ISSOCK ( fileSTats [ stat.ST_MODe ] ):

> print 'Socket.'

> elif stat.S_ISFIFO ( fileSTats [ stat.ST_MODe ] ):

> print 'Named pipe.'

> elif stat.S_ISBLK ( fileSTats [ stat.ST_MODe ] ):

> print 'Block special device.'

> elif stat.S_ISCHR ( fileSTats [ stat.ST_MODe ] ):

> print 'Character special device.'
Additionally, we can use "os.path" to gather basic information:
> import os.path

> fileStats = 'test.txt'

> if os.path.isdir ( fileStats ):

> print 'Directory.'

> elif os.path.isfile ( fileStats ):

> print 'File.'

> elif os.path.islink ( fileStats ):

> print 'Shortcut.'

> elif os.path.ismount ( fileStats ):

> print 'Mount point.'
**File Management in Python - Directories**
(Page 3 of 5 )

Directories, like regular files, are easy to work with. Let's start by listing the contents of a directory:
> import os

> for fileName in os.listdir ( '/' ):

> print fileName
As you can see, this is extremely simple, and it can be done in three lines.
Creating a directory is also simple:
> import os

> os.mkdir ( 'testDirectory' )
It is equally as easy to delete the directory we just created:
> import os

> os.rmdir ( 'testDirectory )
We can also create multiple directories at a time:
> import os

> os.makedirs ( 'I/will/show/you/how/deep/the/rabbit/hole/goes' )
Assuming we add nothing to the directories we just created, we can also delete them all at once:
> import os

> os.removedirs ( 'I/will/show/you/how/deep/the/rabbit/hole/goes' )
Suppose we want to perform a specific action when a specific file type is reached. This can easily be done with the "fnmatch" module. Let's print the contents of all the ".txt" files we encounter and print the filename of any ".exe" files we encounter:
> import fnmatch

> import os

> for fileName in os.listdir ( '/' ):

> if fnmatch.fnmath ( fileName, '*.txt' ):

> print open ( fileName ).read()

> elif fnmatch.fnmatch ( fileName, '*.exe' ):

> print fileName
The asterisk character can represent any amount of characters. If we want to match just one character, we can use the question mark:
> import fnmatch

> import os

> for fileName in os.listdir ( '/' ):

> if fnmatch.fnmatch ( fileName, '?.txt' ):

> print 'Text file.'
It is also possible to create a regular expression using the "fnmatch" module, matching filenames with the "re" module:

> import fnmatch

> import os

> import re

> filePattern = fnmatch.translate ( '*.txt' )

> for fileName in os.listdir ( '/' ):

> if re.match ( filePattern, fileName ):

> print 'Text file.'

If we're just looking for one type of filename, it is a lot easier to use the "glob" module. Its patterns are similar to those used in "fnmatch":
> import glob

> for fileName in glob.glob ( '*.txt' ):

> print 'Text file.'
It is also possible to use ranges of characters in the patterns, just as you would in regular expressions. Suppose you want to print the names of text files with one digit before the extension:
> import glob

> for fileName in glob.glob ( '[0-9].txt' ):

> print fileName
The "glob" module makes use of the "fnmatch" module.

(Page 4 of 5 )

Using the methods covered in the previous section, it is possible to read strings from[files](http://www.devshed.com/#)and write strings to files. However, in some situations, you may need to pass other types of data, such as lists, tuples, dictionaries and other objects. In Python, this is possible through a method known as pickling. To pickle data, you would use the "pickle" module included in the standard library.
Let's start by pickling a short list of strings and integers:
> import pickle

> fileHandle = open ( 'pickleFile.txt', 'w' )

> testList = [ 'This', 2, 'is', 1, 'a', 0, 'test.' ]

> pickle.dump ( testList, fileHandle )

> fileHandle.close()
Unpickling the data is just as easy:
> import pickle

> fileHandle = open ( 'pickleFile.txt' )

> testList = pickle.load ( fileHandle )

> fileHandle.cloes()
We can also store more complex data:
> import pickle

> fileHandle = open ( 'pickleFile.txt', 'w' )

> testList = [ 123, { 'Calories' : 190 }, 'Mr. Anderson', [ 1, 2, 7 ] ]

> pickle.dump ( testList, fileHandle )

> fileHandle.close()

> import pickle

> fileHandle = open ( 'pickleFile.txt' )

> testList = pickle.load ( fileHandle )

> fileHandle.close()
As you can see, pickling is extremely easy to do with Python's "pickle" module. Numeous objects may be stored in files with it. You can also use the "cPickle" module if it is availible to you. It's exactly the same as the "pickle" modue, but it's faster:
> import cPickle

> fileHandle = open ( 'pickleFile.txt', 'w' )

> cPickle.dump ( 1776, fileHandle )

> fileHandle.close()
**[File](http://www.devshed.com/#)****[Management](http://www.devshed.com/#)****in Python - Creating In-memory Files**
(Page 5 of 5 )

A number of modules you will encounter contain methods that require a file object as an argument. Sometimes, it is inconvenient to create and use a real file, however. Thankfully, you can create files that store themselves in a computer's memory using the "StringIO" module:
> import StringIO

> fileHandle = StringIO.StringIO ( "Let freedom ring." )

> [print](http://www.devshed.com/#)
> fileHandle.read() \# "Let freedom ring."

> fileHandle.close()
A "cStringIO" module is also availible. It is identical to the "StringIO" module in use, but, just like the "cPickle" module is to the "pickle" module, it is faster:
> import cStringIO

> fileHandle = cStringIO.cStringIO ( "To Kill a Mockingbird" )

> print fileHandle.read() \# "To Kill a Mockingbid"

> fileHandle.close()
**Conclusion**
File management is a task that programmers of many languages will often encounter in their applications. Thankfully, Python makes the task incredibly easy compared to other languages. It offers many modules in its standard library that aid programmers, and its object orientation further simplifies things.
You now have a basic understanding of file management in Python, and you will use it in many future applications.

|DISCLAIMER:|The content provided in this article is not warrantied or guaranteed by Developer Shed, Inc. The content provided is intended for entertainment and/or educational purposes in order to introduce to the reader key ideas, concepts, and/or product reviews. As such it is incumbent upon the reader to employ real-world tactics for security and implementation best practices. We are not liable for any negative consequences that may result by implementing any information covered in our articles or tutorials. If this is a hardware review, it is not recommended to open and/or modify your hardware.|



