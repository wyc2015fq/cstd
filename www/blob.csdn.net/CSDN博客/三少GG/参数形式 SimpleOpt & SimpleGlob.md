# 参数形式 SimpleOpt & SimpleGlob - 三少GG - CSDN博客
2013年03月05日 15:59:45[三少GG](https://me.csdn.net/scut1135)阅读数：1158
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
[http://www.codeproject.com/Articles/13882/Cross-platform-Command-Line-Argument-and-Option-Pa](http://www.codeproject.com/Articles/13882/Cross-platform-Command-Line-Argument-and-Option-Pa)
# Cross-platform Command Line Argument and Option Parsing (Plus File Glob)
- [Download source files](http://code.jellycan.com/simpleopt/)
- [Download HTML documentation](http://code.jellycan.com/files/simpleopt-doc.zip) (or view it[online](http://code.jellycan.com/simpleopt-doc/html/index.html))
- Get the latest information and the latest version from the homepage at [http://code.jellycan.com/simpleopt/](http://code.jellycan.com/simpleopt/)
## Introduction
This is a cross-platform command line library which can parse almost any of the standard command line formats in use to extract all arguments and options. It supports character types of ASCII, MBCS and Unicode. It is designed explicitly to be portable to
 any platform and has been tested on Windows and Linux. It also includes a cross-platform implementation of
```
glob()
```
so that wildcards in command line arguments are simply expanded for use by the program. It has been released as open-source and free using the MIT licence.
## Features
- MIT Licence allows free use in all software (including GPL and commercial) 
- Multi-platform (Windows 95/98/ME/NT/2K/XP, Linux, Unix) 
- Supports all types of options: 
|`-`|switch character only (e.g. use `stdin `for input)|
|----|----|
|`-o`|short (single character)|
|`-long`|long (multiple character, single switch character)|
|`--longer`|long (multiple character, multiple switch characters)|
|`word`|special word construed as an option, no switch characters|
- Supports all types of arguments for options: 
|`--option`|short/long option flag (no argument)|
|----|----|
|`--option ARG`|short/long option with separate required argument|
|`--option=ARG`|short/long option with combined required argument|
|`--option[=ARG]`|short/long option with combined optional argument|
|`-oARG`|short option with combined required argument|
|`-o[ARG]`|short option with combined optional argument|
- Supports options with multiple or variable numbers of arguments: 
|`--multi ARG1 ARG2`|Multiple arguments|
|----|----|
|`--multi N ARG-1 ARG-2 ... ARG-N`|Variable number of arguments|
- Supports clumping of multiple short options in a `string`|`foo.exe -defgcFILE`|==|`foo.exe -d -e -f -g -cFILE`|
- Automatic recognition of a single slash as equivalent to a single hyphen on Windows 
|`foo.exe /f FILE`|==|`foo.exe -f FILE`|
- File arguments can appear anywhere in the argument list 
`foo.exe FILE1 -a arg FILE2 --flag FILE3 FILE4`
- File arguments will be returned to the application in the same order they were supplied on the command line
- Short-circuit option matching: "`--man`" will match "`--mandate`"
- Invalid options can be handled while continuing to parse the command line 
- Support for case insensitive matching (independently optional for each of short, long and word option types)
- Valid options list can be changed dynamically during command line processing, i.e., accept different options depending on an option supplied earlier in the command line
- Implemented with only a single C++ header file 
- Uses no C runtime or OS functions 
- `Char`, `wchar_t` and Windows `TCHAR `in the same program
- Complete working examples included 
- Compiles cleanly at warning level 4 (Windows/VC.NET 2003), warning level 3 (Windows/VC6) and -Wall (Linux/gcc)
## SimpleOpt Usage
The `SimpleOpt `class is used by following these steps:
- Include the *SimpleOpt.h* header file: 
![](http://www.codeproject.com/images/minus.gif) Collapse |[Copy
 Code](http://www.codeproject.com/Articles/13882/Cross-platform-Command-Line-Argument-and-Option-Pa#)
#include"SimpleOpt.h"
- Define an array of valid options for your program: 
![](http://www.codeproject.com/images/minus.gif) Collapse |[Copy
 Code](http://www.codeproject.com/Articles/13882/Cross-platform-Command-Line-Argument-and-Option-Pa#)
enum { OPT_HELP, OPT_FLAG, OPT_ARG };
CSimpleOpt::SOption g_rgOptions[] = {
    { OPT_FLAG, _T("-a"),     SO_NONE    }, // "-a"
    { OPT_FLAG, _T("-b"),     SO_NONE    }, // "-b"
    { OPT_ARG,  _T("-f"),     SO_REQ_SEP }, // "-f ARG"
    { OPT_HELP, _T("-?"),     SO_NONE    }, // "-?"
    { OPT_HELP, _T("--help"), SO_NONE    }, // "--help"
    SO_END_OF_OPTIONS                       // END
};
Note that all options must start with a hyphen even if the slash will be accepted. This is because the slash character is automatically converted into a hyphen to test against the list of options. For example, the following line matches both "`-?`"
 and "`/?`" (on Windows). 
![](http://www.codeproject.com/images/minus.gif) Collapse |[Copy
 Code](http://www.codeproject.com/Articles/13882/Cross-platform-Command-Line-Argument-and-Option-Pa#)
{ OPT_HELP, _T("-?"),     SO_NONE    }, // "-?"
- Instantiate a `CSimpleOpt `object supplying `argc`, 
```
argv
```
and the option table: 
![](http://www.codeproject.com/images/minus.gif) Collapse |[Copy
 Code](http://www.codeproject.com/Articles/13882/Cross-platform-Command-Line-Argument-and-Option-Pa#)
CSimpleOpt args(argc, argv, g_rgOptions);
- Process the arguments by calling `Next() `until it returns `false`. On each call, first check for an error by calling`LastError()`, then either handle the error or process the argument. 
![](http://www.codeproject.com/images/minus.gif) Collapse |[Copy
 Code](http://www.codeproject.com/Articles/13882/Cross-platform-Command-Line-Argument-and-Option-Pa#)
while (args.Next()) {
    if (args.LastError() == SO_SUCCESS) {
        // handle option, using OptionId(), OptionText() and OptionArg()
    }
    else {
        // handle error, one of: SO_OPT_INVALID, SO_OPT_MULTIPLE, 
// SO_ARG_INVALID, SO_ARG_INVALID_TYPE, SO_ARG_MISSING
    }
}
- Process all non-option arguments with `File()`, `Files()` and`FileCount()`: 
![](http://www.codeproject.com/images/minus.gif) Collapse |[Copy
 Code](http://www.codeproject.com/Articles/13882/Cross-platform-Command-Line-Argument-and-Option-Pa#)
ShowFiles(args.FileCount(), args.Files());
## SimpleGlob Usage
The `SimpleGlob `class is used by following these steps:
- Include the *SimpleGlob.h* header file: 
![](http://www.codeproject.com/images/minus.gif) Collapse |[Copy
 Code](http://www.codeproject.com/Articles/13882/Cross-platform-Command-Line-Argument-and-Option-Pa#)
#include"SimpleGlob.h"
- Instantiate a `CSimpleGlob `object supplying the appropriate flags (see the header file for details): 
![](http://www.codeproject.com/images/minus.gif) Collapse |[Copy
 Code](http://www.codeproject.com/Articles/13882/Cross-platform-Command-Line-Argument-and-Option-Pa#)
CSimpleGlob glob(SG_GLOB_NODOT|SG_GLOB_NOCHECK);
- Add file specs with `Add()`: 
![](http://www.codeproject.com/images/minus.gif) Collapse |[Copy
 Code](http://www.codeproject.com/Articles/13882/Cross-platform-Command-Line-Argument-and-Option-Pa#)
if (SG_SUCCESS != glob.Add(argc, argv)) {
    // handle error
}
- Extract the file names using `FileCount() `and `Files()`: 
![](http://www.codeproject.com/images/minus.gif) Collapse |[Copy
 Code](http://www.codeproject.com/Articles/13882/Cross-platform-Command-Line-Argument-and-Option-Pa#)
for (int n = 0; n < glob.FileCount(); ++n) {
    printf("file %d: '%s'\n", n, glob.File(n));
}
## Examples
Basic usage of the `SimpleOpt `library is illustrated by the following code sample (included as*basicSample.cpp*). This shows simple command line processing including the use of
```
SimpleGlob
```
to expand wildcard filenames passed on the command line.
![](http://www.codeproject.com/images/minus.gif) Collapse |
[Copy Code](http://www.codeproject.com/Articles/13882/Cross-platform-Command-Line-Argument-and-Option-Pa#)
#include"SimpleOpt.h"#include"SimpleGlob.h"// define the ID values to indentify the option
enum { OPT_HELP, OPT_FLAG, OPT_ARG };
// declare a table of CSimpleOpt::SOption structures. See the SimpleOpt.h header
// for details of each entry in this structure. In summary they are:
//  1. ID for this option. This will be returned from OptionId() during processing.
//     It may be anything >= 0 and may contain duplicates.
//  2. Option as it should be written on the command line
//  3. Type of the option. See the header file for details of all possible types.
//     The SO_REQ_SEP type means an argument is required and must be supplied
//     separately or combined, e.g. "-f FILE" or "-f=ARG"
//  4. The last entry must be SO_END_OF_OPTIONS.
//CSimpleOpt::SOption g_rgOptions[] = {
    { OPT_FLAG, _T("-a"),     SO_NONE    }, // "-a"
    { OPT_FLAG, _T("-b"),     SO_NONE    }, // "-b"
    { OPT_ARG,  _T("-f"),     SO_REQ_SEP }, // "-f ARG" or "-f=ARG"
    { OPT_ARG,  _T("-g"),     SO_REQ_CMB }, // "-g=ARG"
    { OPT_HELP, _T("-?"),     SO_NONE    }, // "-?"
    { OPT_HELP, _T("--help"), SO_NONE    }, // "--help"
    SO_END_OF_OPTIONS                       // END
};
// show the usage of this program
void ShowUsage() {
    _tprintf(_T("Usage: basicSample [-a] [-b] [-f FILE] [-?] [--help] FILES\n"));
}
int _tmain(int argc, TCHAR * argv[]) {
    // declare our options parser, pass in the arguments from main
// as well as our array of valid options.
    CSimpleOpt args(argc, argv, g_rgOptions);
    // while there are arguments left to process
while (args.Next()) {
        if (args.LastError() == SO_SUCCESS) {
            if (args.OptionId() == OPT_HELP) {
                ShowUsage();
                return0;
            }
            _tprintf(_T("Option, ID: %d, Text: '%s', Argument: '%s'\n"),
                args.OptionId(), args.OptionText(),
                args.OptionArg() ? args.OptionArg() : "");
        }
        else {
            _tprintf(_T("Invalid argument: %s\n"), args.OptionText());
            return1;
        }
    }
    // process any files that were passed to us on the command line.
// send them to the globber so that all wildcards are expanded
// into valid filenames (e.g. *.cpp -> a.cpp, b.cpp, c.cpp, etc)
// See the SimpleGlob.h header file for details of the flags.
    CSimpleGlob glob(SG_GLOB_NODOT|SG_GLOB_NOCHECK);
    if (SG_SUCCESS != glob.Add(args.FileCount(), args.Files())) {
        _tprintf(_T("Error while globbing files\n"));
        return1;
    }
    // dump all of the details, the script that was passed on the
// command line and the expanded file names
for (int n = 0; n < glob.FileCount(); ++n) {
        _tprintf(_T("file %d: '%s'\n"), n, glob.File(n));
    }
    return0;
} 
Another included example '*fullSample.cpp*' implements the following command line. This demonstrates all of the different types of arguments which can be parsed by`SimpleOpt`.
![](http://www.codeproject.com/images/minus.gif) Collapse |
[Copy Code](http://www.codeproject.com/Articles/13882/Cross-platform-Command-Line-Argument-and-Option-Pa#)
Usage: fullSample [OPTIONS] [FILES]
--exact     Disallow partial matching of option names
--noslash   Disallow use of slash as an option marker on Windows
--shortarg  Permit arguments on single letter options with no equals sign
--clump     Permit single char options to be clumped as long string
--noerr     Do not generate any errors for invalid options
-a  -b  -c  -d  -flag  --flag               Flag (no arg)
-s ARG   -sep ARG  --sep ARG                Separate required arg
-cARG    -c=ARG    -com=ARG    --com=ARG    Combined required arg
-o[ARG]  -o[=ARG]  -opt[=ARG]  --opt[=ARG]  Combined optional arg
-man     -mandy    -mandate                 Shortcut matching tests
--man    --mandy   --mandate                Shortcut matching tests
open read write close zip unzip             Special words
-?  -h  -help  --help                       Output this help.
If a FILE is `-', read standard input.
A third included example '*globSample.cpp*' implements a test program to test all of the flags supported by`SimpleGlob`.
## Short-Circuit Option Matching (SimpleOpt)
The short-circuit matching of the long option names is enabled by default. It can be disabled by passing
```
SO_O_EXACT
```
as one of the flags to `Init()`. When disabled, only exact matches are accepted for options. When enabled, matches are calculated in the following manner:
- An exact match takes precedence over a partial match 
- Equal partial matches returns an "`SO_OPT_MULTIPLE`" error 
- A better partial match will return the matched option 
For example, if the command line has the following valid options:
- `--main`
- `--mainline`
- `--mainlamp`
The following matches would occur:
|Commandline|Result|Why|
|----|----|----|
|`--mai`|`SO_OPT_MULTIPLE`|Multiple partial matches|
|`--main`|`--main`|Exact match|
|`--mainl`|`SO_OPT_MULTIPLE`|Multiple partial matches|
|`--mainli`|`--mainline`|Single partial match|
|`--mainline`|`--mainline`|Exact match|
## Alternatives
This library is just one method of processing command line options. If it doesn't suit your requirements, have a look at the following alternative libraries:
- [SimpleOptions](http://www.codeproject.com/cpp/simpleoptions.asp)
- [boost program_options](http://www.boost.org/doc/html/program_options.html)
- [ACE Get_Opt](http://www.cs.wustl.edu/~schmidt/ACE.html) ([documentation](http://www.dre.vanderbilt.edu/Doxygen/Stable/libace-doc/a00175.html))
## MIT Licence
The licence text below is the boilerplate "MIT Licence" used from: [http://www.opensource.org/licenses/mit-license.php](http://www.opensource.org/licenses/mit-license.php)
Copyright (c) 2006, Brodie Thiesfield
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify,
 merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
## License
This article, along with any associated source code and files, is licensed under[The MIT License](http://www.opensource.org/licenses/mit-license.php)
