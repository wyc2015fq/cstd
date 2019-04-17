# NSCLIENT++可以采集的指标 - DoubleLi - 博客园






 Documentation 
  Information 
  Commands/Modules 
  CheckDisk 
  CheckFileSize 
  CheckDriveSize 
  CheckFile 
  CheckEventLog 
  CheckSystem 
  CheckCPU 
  CheckUpTime 
  CheckServiceState 
  CheckProcState 
  CheckMem 
  CheckCounter 
  CheckHelpers 
  CheckAlwaysOK 
  CheckAlwaysCRITICAL 
  CheckAlwaysWARNING 
  CheckMultiple 
  FileLogger 
  NRPEListener 
  NSClientListener 
  SysTray 
  CheckWMI 
  CheckWMIValue 
  RemoteConfiguration 
  CheckExternalScripts 
  LUAScript 
  Guide 
  NSCAAgent 
  Donate 
CheckDisk.dll 
The CheckDisk module has various disk related checks currently focusing on size. But hopefully it will be extended with date existence and many other disk related checks in the future. Feel free to request checks that you need. 
•	CheckFileSize, Check the size of one or more files or directories. 
•	CheckDriveSize, Check the size of one or more Drives 
•	CheckFile, Check various aspects on one or more files or directories. 
Configuration 
This module has no configuration directives. 
CheckFileSize 
CheckFileSize is part of the wiki:CheckDisk module 
This check does a recursive size calculation of the directory (or file) specified. A request has one or more options described in the table below. The order only matter in that the size has to be specified before the File option this becaous you can change the size for each drive by specifying multiple Size options. 
Option	Values	Description 
MaxWarn 
size-value 
The maximum size the directory is allowed before a warning state is returned. 
MaxCrit 
size-value 
The maximum size the directory is allowed before a critical state is returned. 
MinWarn 
size-value 
The minimum size the directory is allowed before a warning state is returned. 
MinCrit 
size-value 
The minimum size the directory is allowed before a critical state is returned. 
ShowAll 
None	A Boolean flag to show size of directories that are not in an alarm state. If this is not specified only drives with an alarm state will be listed in the resulting string. 
File	File or directory name	The name of the file or directory that should have its size calculated. Notice that large directory structures will take a long time to check. 
File:<alias>	File or directory name	Same as the file option but using a short alias in the returned data. 
The size-value is a normal numeric-value with a unit postfix. The available postfixes are B for Byte, K for Kilobyte, M for Megabyte and finally G for Gigabyte. 
Examples 
Check the size of the windows directory and make sure it stays below 1 gigabyte: 
CheckFileSize ShowAll MaxWarn=1024M MaxCrit=4096M File:WIN=c:\WINDOWS\*.* 
WARNING: WIN: 2G (2325339822B)|WIN=2325339822;1073741824;4294967296 
define command { 
  command_name CheckFileSize 
  command_line check_nrpe -H $HOSTADDRESS$ -p 5666 -c CheckFileSize -a ShowAll MaxWarn=$ARG1$ MaxCrit=$ARG2$ File:$ARG4$=$ARG5$ 
} 
check_command CheckFileSize!1024M!4096M!WIN!c:\WINDOWS\*.* 
Check the size of the pagefile.sys and make sure it stays above 1 gigabyte: 
CheckFileSize ShowAll MinWarn=1G  MinCrit=512M File=c:\pagefile.sys 
OK: c:\pagefile.sys: 1G (1610612736B) 
define command { 
command_name CheckPageFile 
command_line check_nrpe -H $HOSTADDRESS$ -p 5666 -c CheckFileSize -a ShowAll MinWarn=$ARG1$ MinCrit=$ARG2$ File:PageFile=c:\pagefile.sys 
} 
check_command CheckPageFile!1G!512M 
Sample of using individual size for multiple files.: 
CheckFileSize MaxWarn=2G MaxCrit=4G File=c:\\pagefile.sys MaxWarn=1K MaxCrit=512 File=c:\\boot.ini 
OK all file sizes are within bounds. 
define command { 
command_name CheckMyFiles 
command_line check_nrpe -H $HOSTADDRESS$ -p 5666 -c CheckFileSize -a MaxWarn=2G MaxCrit=4G File=c:\\pagefile.sys MaxWarn=1K MaxCrit=512 File=c:\\boot.ini 
} 
check_command CheckMyFiles 
I have had to set this up like this for our Windows Servers 
# Check File Size === Greg Small	
define command { 
  command_name CheckFileSize 
  command_line $USER1$/check_nrpe -H $HOSTADDRESS$ -p 5666 -c CheckFileSize -a MaxWarn=$ARG1$ MaxCrit=$ARG2$ File=$ARG3$ 
} 

define service{ 
use	xx-app-service 
host_name	SERVER01 
service_description	        Information store database 
check_command         CheckFileSize!13G!15.5G!'d:\\exchsrvr\\mdbdata\\priv1.edb' 
}	
CheckDriveSize 
CheckDriveSize is part of the CheckDisk module 
This check verifies the size of various drives specified on the command line. A request has one or more options described in the table below. The order only matters in that the size has to be specified before the Drive option because you can change the size for each drive by specifying multiple Size options. 
Option	Values	Description 
MaxWarnFree	size-value or % 
The maximum allowed free space for the drive(s). 
MaxCritFree	size-value or % 
The maximum allowed free space for the drive(s). 
MinWarnFree	size-value or % 
The minimum allowed free space for the drive(s). 
MinCritFree	size-value or % 
The minimum allowed free space for the drive(s). 
MaxWarnUsed	size-value or % 
The maximum allowed used space for the drive(s). 
MaxCritUsed	size-value or % 
The maximum allowed used space for the drive(s). 
MinWarnUsed	size-value or % 
The minimum allowed used space for the drive(s). 
MinCritUsed	size-value or % 
The minimum allowed used space for the drive(s). 
ShowAll	Empty, long	If present will display information even if an item is not reporting a state. If set to long will display more information. 
Drive	A Drive letter or the path of a mounted Volume	The letter of the drive to check. 
FilterType	FIXED, CDROM, REMOVABLE, REMOTE	Filter for drive type to prevent checking drives of certain kinds (most useful when using CheckAll?). The default is FIXED 
CheckAll	None	Check all available drives 
CheckAllOthers	None	Check all drives (matching Filters) except those specified in the Drive= clause. 
The size-value or % is a normal numeric-value with an optional unit or percentage postfix to specify large sizes. The available postfixes are B for Byte, K for Kilobyte, M for Megabyte, G for Gigabyte and finally % for percent free space. 
Examples 
Check the size of C:\ and make sure it has 10% free space: 
CheckDriveSize ShowAll MinWarnFree=10% MinCritFree=5% Drive=c:\ 
CRITICAL: C:: Total: 74.5G - Used: 71.2G (95%) - Free: 3.28G (5%) < critical|C:;5%;10;5; 
define command { 
  command_name CheckDriveSize 
  command_line check_nrpe -H $HOSTADDRESS$ -p 5666 -c CheckDriveSize -a Drive=$ARG1$ ShowAll MaxWarn=$ARG2$ MaxCrit=$ARG3$ 
} 
check_command CheckDriveSize!C:\!50%!75% 
To check the size of mounted volume c:\volumne_test and make sure it has 1M free space 
CheckDriveSize ShowAll MaxWarn=1M MaxCrit=2M Drive="c:\\volumne_test\\" 
CRITICAL: c:\volumne_test\: 3M (4193280B)|c:\volumne_test\=4193280;0;0; 
check_command CheckDriveSize!c:\volumne_test\!1M!2M 
To check the size of all fixed and network drives and make sure they have atleast 1gig free space 
CheckDriveSize MinWarn=50% MinCrit=25% CheckAll FilterType=FIXED FilterType=REMOTE 
CRITICAL: D:\: Total: 186G - Used: 186G (99%) - Free: 709M (1%) < warning, Y:\: Total: 279G - Used: 279G (99%) - Free: 384M (1%) < critical|C:\;76514398208;1073741824;536870912; D:\;199303897088;1073741824;536870912; X:\;35467034624;1073741824;536870912; Y:\;299649466368;1073741824;536870912; 
define command { 
command_name CheckAllDrives 
command_line check_nrpe -H $HOSTADDRESS$ -p 5666 -c CheckDriveSize -a CheckAll FilterType=FIXED FilterType=REMOTE MinWarn=$ARG1$ MinCrit=$ARG2$ 
} 
check_command CheckAllDrives!25%!50% 
Check all fixed and network drives but ignore C and F 
define command { 
command_name CheckAllDrivesExceptCF 
command_line check_nrpe -H $HOSTADDRESS$ -p 5666 -c CheckDriveSize -a CheckAllOthers FilterType=FIXED FilterType=REMOTE MinWarn=$ARG1$ MinCrit=$ARG2$ "Drive=C:\\" Drive=F:\\ 
} 
CheckFile 
A new command to check a bunch of files. 
BETA 
Simple draft and subject to change. This works similarily to CheckEventLog and uses filters to filter out (or in) a certain amount of files and then checks the count against a given number reporting an error if the count is not within bounds. 
Option	Values	Description 
file	A filename, filter or path	The files to check for instance c:\*.* will check the entire drive (bad idea) c:\foo\bar\my\file.txt will check only that file. 
filter	in, out, any, all	Specify the way you want to filter things. (See section below) 
descriptions	None	Flag to specify if you want to include string representation of the file data (not implemented yet) 
truncate	length of the returned set	This will truncate the output after the specified length. As NRPE can only handle 1024 chars you need to truncate the output. 
MaxWarn 
number of records	The maximum hits to allow before reporting a warning state. 
MaxCrit 
number of records	The maximum hits to allow before reporting a critical state. 
MinWarn 
number of records	The minimum hits to allow before reporting a warning state. 
MinCrit 
number of records	The minimum hits to allow before reporting a critical state. 
filter-size	size-expression 
The size a file should have. 
filter-creation	time-expression 
Filter on a date (the time is specified from "now" not absolute. so filter-created=>2d will mean files created more then 2 days ago will match. 
filter-accessed	time-expression 
Filter on a date (the time is specified from "now" not absolute.) 
filter-written	time-expression 
Filter on a date (the time is specified from "now" not absolute.) 
syntax	String	A syntax string that describes how each matched file is shown the string can have the following keywords: %filename%, %creation%, %access%, %write%, %size% 
Filter 
filter = (in, out, any, all) Specify the way you want to filter things. 
in	means any thing matching this filter will be included in the result. 
out	means any thing matching this filter will be excluded from the result. 
any	means any of the filter rules has to match. 
all	means all of the filter rules have to match. 
Thus filter=all filter=in and filter=any filter=out is the combinations that makes the most sense depending on your filter concept. 
Examples 
This command will check the date of a file (boot.ini) and alert if it's modified (written) date is greater than 3 days (3d). 

checkcommand.cfg { 
command_line	$USER1$/check_nrpe -H $HOSTADDRESS$ -c CheckFile -a file="$ARG1$" filter-written=\>"$ARG2$" syntax="%filename% last updated %write%" MaxCrit=1 
} 
service.cfg { 
check_command	nrpe_check_file_last_written!"C:\\boot.ini"!3d 
} 
CheckEventLog.dll 
The CheckEventLog module checks for problems reported to the windows event log. 
•	CheckEventLog, Check to find errors in the event log. 
Configuration 
Option	Default value	Description 
syntax	Set this to use a specific syntax string for all commands (that don't specify one) 
Advanced options: 
Option	Default value	Description 
debug	0	Log all "hits" and "misses" on the eventlog filter chain, useful for debugging eventlog checks but very very very noisy so you don't want to accidentally set this on a real machine. 
CheckSystem.dll 
A module to check various system related things. A list of the modules and there potential use is listed below here. 
•	wiki:CheckCPU, Check CPU load 
•	wiki:CheckUpTime, Check system uptime 
•	wiki:CheckServiceState, Check state of a service 
•	wiki:CheckProcState, Check state of a process 
•	wiki:CheckMem, Check memory usage (page) 
Command Line 
To simplify debug and setup there is two commandline options that list and test all avalible PDH counters. 
•	NSClient++ -noboot CheckSystem debugpdh 
•	NSClient++ -noboot CheckSystem listpdh 
Configuration 
This module should auto detect most versions of windows (if you have a problem with this let me know so I can update it). Thus you no longer need to configure the grayed out options. There is also some other tweaks that can be configured such as check resolution and buffer size. For more details on how to configure this refer to the Configuration section. 
[Check System] 
Option	Default value	Description 
CPUBufferSize	1h	The time to store CPU load data. 
CheckResolution? 
10	Time between checks in 1/10 of seconds. 
Advanced options: 
Option	Default value	Description 
auto_detect_pdh	1	Set this to 0 to disable auto detect (counters.defs) PDH language and OS version. 
dont_use_pdh_index	0	Set this to 1 if you dont want to use indexes for finding PDH counters. 
force_language	Set this to a locale ID if you want to force auto-detection of counters from that locale. 
ProcessEnumerationMethod? 
auto	Set the method to use when enumerating processes PSAPI, TOOLHELP or auto 
check_all_services[SERVICE_BOOT_START]	ignored	Set how to handle services set to SERVICE_BOOT_START state when checking all services 
check_all_services[SERVICE_SYSTEM_START]	ignored	Set how to handle services set to SERVICE_SYSTEM_START state when checking all services 
check_all_services[SERVICE_AUTO_START]	started	Set how to handle services set to SERVICE_AUTO_START state when checking all services 
check_all_services[SERVICE_DEMAND_START]	ignored	Set how to handle services set to SERVICE_DEMAND_START state when checking all services 
check_all_services[SERVICE_DISABLED]	stopped	Set how to handle services set to SERVICE_DISABLED state when checking all services 
MemoryCommitLimit? 
\Memory\Commit Limit	Counter to use to check upper memory limit. 
MemoryCommitByte? 
\Memory\Committed Bytes	Counter to use to check current memory usage. 
SystemSystemUpTime? 
\System\System Up Time	Counter to use to check the uptime of the system. 
SystemTotalProcessorTime? 
\Processor(_total)\% Processor Time	Counter to use for CPU load. 
ProcessEnumerationMethod? 
auto	Set the PROCESS enumeration method (auto or TOOLHELP or PSAPI) 
CheckCPU 
CheckCPU is part of the wiki:CheckSystem module. 
This check calculates an average of CPU usage for a specified period of time. The data is always collected in the background and the size and interval is configured from the CPUBufferSize and CheckResolution? options. A request has one or more options described in the table below. 
Option	Values	Description 
warn	load in %	Load to go above to generate a warning. 
crit	load in %	Load to go above to generate a critical state. 
Time	time with optional prefix	The time to calculate average over. 
Multiple time= entries can be given - generating multiple CPU usage summaries and multiple warn/crits. 
nsclient	Flag to make the plug in run in NSClient compatibility mode 
ShowAll 
none, long	Add this option to show info even if no errors are detected. Set it to long to show detailed information. 
Time can use any of the following postfixes. w=week, d=day, h=hour, m=minute and s=second. 
Examples 
Check that the CPU load for various times is below 80%: 
checkCPU warn=80 crit=90 time=20m time=10s time=4 
CPU Load ok.|'20m average'=11%;80;90; '10s average'=7%;80;90; '4 average'=10%;80;90; 
Showing multiple time entry usage and returned data 
CheckCPU warn=2 crit=80 time=5m time=1m time=10s 
WARNING: 5m: average load 8% > warning, 1m: average load 2% > warning, 10s: average load 4% > warning|'5m'=8;2;80; '1m'=2;2;80; '10s'=4;2;80; 
Check CPU load with intervals like known from Linux/Unix (with example thresholds): 
checkCPU warn=100 crit=100 time=1 warn=95 crit=99 time=5 warn=90 crit=95 time=15 
FAQ 
•	Question: How does it handle multi CPU machines? 
•	Answer: The returned value is the average value of the CPU load of all the processors. 
CheckUpTime 
This check checks the uptime of a server and if the time is less then the times given as arguments a state is returned. 
Option	Values	Description 
MaxCrit 
time	Maximum time the system is allowed to be up 
MinCrit 
time	Minimum time the system is allowed to be up 
MaxWarn 
time	Maximum time the system is allowed to be up 
MinWarn 
time	Minimum time the system is allowed to be up 
nsclient	Flag to make the plug in run in NSClient compatibility mode 
ShowAll 
Add this option to show details even if an error is not encountered. 
Alias	string	A string to use as alias for the values (default is uptime) 
Examples 
Check that the system has been running for at least a day: 
checkUpTime MinWarn=1d MinCrit=12h 
Client has uptime (19h) < warning (24h) 

CheckServiceState 
This check checks the state of one or more service on the system and generates a critical state if any service is not in the required state. 
Option	Values	Description 
ShowAll 
A flag to toggle if all service states should be listed. 
ShowFail? 
(default)	A flag to indicate if only failed service states should be listed. 
service=state	A service name or service display name and a state the service should have. The state can be either started or stopped. If no state is given started is assumed. 
CheckAll? 
Check to see that all services set to auto-start are started and all set to disabled are not started. 
exclude	service name	Exclude this service from CheckAll? 

Examples 
Check that myService is running and that MyStoppedService? is not running: 
checkServiceState ShowAll myService MyStoppedService=stopped 
myService : Running - MyStoppedService : Stopped 
Check that all auto-start services are running but exclude some that are intentionaly not in the correct state: 
checkServiceState CheckAll exclude=wampmysqld exclude=MpfService 
OK: All services are running. 
CheckMem 
This check checks the memory (page) usage and generates a state if the memory is above or below give parameters. 
Option	Values	Description 
MaxWarn 
size-value or % 
The maximum size allowed before a warning state is returned. 
MaxCrit 
size-value or % 
The maximum size allowed before a critical state is returned. 
MinWarn 
size-value or % 
The minimum size allowed before a warning state is returned. 
MinCrit 
size-value or % 
The minimum size allowed before a critical state is returned. 
ShowAll 
None	A Boolean flag to show size even if no state is returned (?). 
type	page, paged, virtual, physical	What kind of memory to check (does not yet support stacking to check multiple kinds) 
The size-value or % is a normal numeric-value with an optional unit or percentage postfix to specify large sizes. The available postfixes are B for Byte, K for Kilobyte, M for Megabyte, G for Gigabyte and finally % for percent free space. 
What the different types really mean 
Type	Meaning 
page	Number of .. ?? This is the commitlimit (same as used in NSCLient, ie. via PDH, available on NT4) 
paged	Number of .. ?? (Usually identical to 'page', different APIs for I think the same thing) According to M$ this is: Size of available memory to commit, in bytes. 
virtual	Number of pages of swap currently in use (note - it does NOT = (physical + swap) as on *nix boxes) According to M$ this is: Size of unreserved and uncommitted memory in the user mode portion of the virtual address space of the calling process, in bytes. 
physical	Total amount of RAM in the server ?? (does it change?) According to M$ this is: Total size of physical memory, in bytes. 
Examples 
Check that the page is below 80%: 
checkMem MaxWarn=80% MaxCrit=90% ShowAll type=page 
OK: page: 758M (795205632B)|page=30% 80%;90%; 
Check that the physical is below 80%: 
checkMem MaxWarn=80% MaxCrit=90% ShowAll type=physical 
OK: physical: 758M (795205632B)|'physical memory'=30% 80%;90%; 
CheckCounter 
Used to check performance counters. This is probably how most things can be checked as there are a lot of performance counters. To find performance counters, use the program perfmon that is shipped with windows. An important note is that performance counters are language and version specific. You can list all available performance counters using the listpdh option as shown in the examples below. 
Option	Values	Description 
MaxWarn 
Number	Maximum allowed number 
MaxCrit 
Number	Maximum allowed number 
MinWarn 
Number	Minimum allowed number 
MinCrit 
Number	Minimum allowed number 
ShowAll 
None	A Boolean flag to show value even if no state is returned. 
Counter	Performance Counter	Add a performance counter to this check 
Counter:<name>	Performance Counter	Add a named performance counter. The <name> will be used as an alias. 
Averages	true, false	Set this to false to make performance checking faster of counters that doesn't represent average values. 
Examples 
Check that mutex count (on WinXP) is below 500: 
checkCounter "Counter:mutex=\\Objects\\Mutexes" ShowAll MaxWarn=500 MaxCrit=1000 
WARNING: mutex: 673 > warning|mutex=673;500;1000; 
Using instances(?) in counters 
checkCounter "Counter=\\Processor(_total)\\% Processor Time" ... 
... 
List all available performance counters, and debug them (means, open, try to read, close, etc) 
nsclient++ CheckSystem listpdh 
... 
"NSClient++.exe" CheckSystem debugpdh 
... 
I have had to set this up for Counter Checking =-= Greg Small 
define command { 
        command_name    check_counter 
#       command_line    $USER1$/check_nt $HOSTADDRESS$ -p 12489 -v COUNTER -l $ARG1$ -d SHOWALL -w $ARG2$ -c $ARG3$ 
        command_line    $USER1$/check_nrpe -H $HOSTADDRESS$ -c CheckCounter -a $ARG1$ ShowAll MaxWarn=$ARG2$ MaxCrit=$ARG3$ 
} 

define service{ 
use	itx-app-service 
host_name	HOST 
service_description	Microsoft Exchange 5.5 IS RPC Operations / Sec 
check_command check_counter!"\\MSExchangeIS\\RPC Operations/sec"!300!400 
}	

define service{ 
use	itx-app-service 
host_name	HOST 
service_description	Windows 2000/2003 Physical Disk Time 
check_command check_counter!"\\PhysicalDisk(_Total)\\% Disk Time"!60!90 
notes	More than 60 percent indicates a problem. 
} 
CheckProcState 
This check checks the state of one or more processes on the system and generates a critical state if any process is not in the required state 
Option	Values	Description 
ShowAll 
A flag to toggle if all process states should be listed. 
ShowFail? 
(default)	A flag to indicate if only failed process states should be listed. 
process=state	A process name and a state the process should have. The state can be either started or stopped. If no state is given started is assumed. The name is the name of the executable. 
Alias	alias	Give a process an alias 
ignore-perf-data	If present performance data will be stripped out 
Proc:<alias>=<state>	A process name and a state the process should have. 
(Max|Min)(Warn|Crit)Count	number	Process count bounds For instance: MaxCritCount?=4 means if a process has more then 4 instances it will be a critical condition. 
Examples 
Check that quake.exe is not running and NSClient++.exe is running: 
checkProcState ShowAll quake.exe=stopped NSClient++.exe=started 
quake.exe : Stopped - NSClient++.exe : Running 
Check that quake.exe is not running and my.exe and NSClient++.exe is running and only show problems: 
checkProcState my.exe quake.exe=stopped NSClient++.exe=started 
my.exe : Stopped 

checkProcState MinCritCount=50 svchost.exe=started 

Check that cmd.exe with substring printloop in commandline is running: 
NSClient++ /test 
checkProcState match=regexp cmdline ShowAll .*cmd.*printloop.*=started 

CheckWMI.dll 
The CheckWMI module has various WMI related functions used to query and check the WMI (Windows Managment Instrumenation). Feel free to request checks that you need. 
•	CheckWMI, Check large resultsets from (for instance are there more then 5-rows matching criteria X, ie. more then 5 internet explorer processes witch uses more then 123Mb memory). 
•	CheckWMIValue, Check the result of a query (ie. are the current memory utilization over X) 
Configuration 
This module has no configuration directives. 
CheckWMI 
CheckWMI is part of the wiki:CheckWMI module. 
New version that is *a lot* more usefull (i hope). It is still alpha need to do more testing but I would like to get some initialö feedback on the syntax and such. Also feel free to try it out and report bugs to me (might wanna keep track of memory and such as I have not done so myself yet) To debug and help you setup your queries there is also a command line interface to run queries and see the result. 
nsclient++ CheckWMI <query> 
The syntax for this is Similar to CheckEventLog but simpliefied in regards to syntax so I hope it shall be easier to use and udnerstand. 
This check enumerates all rows returned from your query filtering results and check the count against a set war and crit threshold. If you want to check a value there will soon be a separet check for that. This is designed to find "anomalies" in result-sets. 
Option	Values	Description 
filter	any, all	Specify the way you want to filter things. (See section below) Not yet implemented (default is all) 
truncate	length of the returned set	This will truncate the output after the specified length. As NRPE can only handle 1024 chars you need to truncate the output. 
MaxWarn 
number of records	The maximum records to allow before reporting a warning state. 
MaxCrit 
number of records	The maximum records to allow before reporting a critical state. 
<mode>filter-<type>:<Column>	<filter value>	A number of strings to use for filtering the event log 
Filters 
Capturing result entries (or discarding them) are done with filters. There are three kinds of filters. 
* positive requirments (+) 
All these filters must match or the row is discarded. 
* negative requriments (-) 
None of these filters can match (if any do the row is discarded). 
* normal matches (.) 
If this matches the line is included. 
The syntax of the filter is: <mode>filter-<type>:<Column>=<expression> 
Filter <Mode>s 
<mode>	title	description 
+	required filter	If you miss this filter the line is discarded 
.	normal filter	If a hit the line is included 
-	negative filter	If a line hits this it is discarded 
Filter <Type>s 
<type>	Value	Description 
string	[[string expression]]	Match the column againast a string expression 
numeric	[[numeric expression]]	Match the column againast a numeric expression 
Filter <Columns>s 
A Column (if specified) will make the filter woirk againast a specific column in the result set. 
string expression 
A string expression is a key followed by a string that specifies a string expression. Currently substr and regexp are supported. Thus you enter filter.message=regexp:(foo|bar) to enter a regular expression and filter-message=substr:foo to enter a substring patter match. 
Examples 
Check to see if there is 2 CPUs present (or cores) 
CheckWMI MaxCrit=3 MinWarn=1 "Query:load=Select * from win32_Processor" 
Check to see that no CPU has less then 2.66GHz frequency 
CheckWMI MinCrit=1 MinWarn=1 "Query:load=Select CurrentClockSpeed from win32_Processor" +filter-numeric:CurrentClockSpeed=<2659 
To try a query use the following syntax: 
nsclient++ CheckWMI Select * from win32_Processor 
(I know these are not very usefull, but hopefully they will give you an idea how to do things, there a *a lot* of things to check via the WMI so you ought to be able to do most things you want.) 
CheckWMIValue 
CheckWMIValue is part of the wiki:CheckWMI module. 
It is still alpha need to do more testing but I would like to get some initialö feedback on the syntax and such. Also feel free to try it out and report bugs to me (might wanna keep track of memory and such as I have not done so myself yet) To debug and help you setup your queries there is also a command line interface to run queries and see the result. 
nsclient++ CheckWMI <query> 
The syntax for this is Similar to other check commands so it should be pretty straight forward to set it up. The plugin will run a WMI query and check the returned columns against bounds provided by the checker (nagios) and report the result. 
Option	Values	Description 
MaxWarn	Numeric value	The maximum allowed value for the column(s). 
MaxCrit	Numeric value	The maximum allowed value for the column(s). 
MinWarn	Numeric value	The minimum allowed value for the column(s). 
MinCrit	Numeric value	The minimum allowed value for the column(s). 
ShowAll	Empty, long	If present will display information even if an item is not reporting a state. If set to long will display more information. 
Query	WMI Query	The WMI query to ask (not stackable, only one query at a time) 
Check	A column name	A column name to check (if * all columns will be checked) (this is stackable, so you can compare any number of columns) 
truncate	numeric value	The maximum length of the query-result. 
AliasCol? 
Column name	A column to be included (prefixed) in the alias for matching columns. 
Examples 
Check to see if there is 2 CPUs present (or cores) 
CheckWMI MaxCrit=3 MinWarn=1 "Query:load=Select * from win32_Processor" 
Check the CPU load on all CPUs and warn if above 50 and critical if above 80 
CheckWMIValue "Query=Select * from win32_Processor" MaxWarn=50 MaxCrit=80 Check:CPU=LoadPercentage ShowAll=long 
To try a query use the following syntax: 
nsclient++ CheckWMI Select * from win32_Processor 
Check threads in processes and make sure a process does not have more then 50 threads (critical at 100) 
CheckWMIValue "Query=select Caption, ThreadCount from Win32_Process" MaxWarn=50 MaxCrit=100 Check:threads=ThreadCount AliasCol=Caption 
Gives you: 
System threads: 98 > warning, svchost.exe threads: 87 > warning|'System Idle Process threads'=2;50;100; 'System threads'=98;50;100; 'smss.exe threads'=3;50;100; 'csrss.exe threads'=13;50;100; 'winlogon.exe threads'=21;50;100; 'services.exe threads'=18;50;100; 'lsass.exe threads'=21;50;100; 'svchost.exe threads'=19;50;100; 'svchost.exe threads'=11;50;100; 'svchost.exe threads'=87;50;100; 'svchost.exe threads'=6;50;100; 'svchost.exe threads'=18;50;100; 'aswUpdSv.exe threads'=3;50;100; 'ashServ.exe threads'=28;50;100; 'sdmcp.exe threads'=3;50;100; 'spoolsv.exe threads'=11;50;100; 'svchost.exe threads'=4;50;100; 'MSCamS32.exe threads'=5;50;100; 'nTuneService.exe threads'=4;50;100; 'nvsvc32.exe threads'=4;50;100; 'svchost.exe threads'=7;50;100; 'tlswrap.exe threads'=4;50;100; 'vmount2.exe threads'=4;50;100; 'vmnat.exe threads'=3;50;100; 'vmnetdhcp.exe threads'=2;50;100; 'vmware-authd.exe threads'=7;50;100; 'ashMaiSv.exe threads'=8;50;100; 'ashWebSv.exe threads'=22;50;100; 'alg.exe threads'=5;50;100; 'explorer.exe 
(I know these are not very usefull, but hopefully they will give you an idea how to do things, there a *a lot* of things to check via the WMI so you ought to be able to do most things you want.) 
CheckExternalScripts.dll 
This module allows you to check external scripts and/or programs. 
Configuration 
[External Script] 
Configure how the External Scripts module works (not to be confused with the "External Scripts" section below that holds scripts that can be run. 
Option	Default value	Description 
command_timeout	60	The maximum time in seconds that a command can execute. (if more then this execution will be aborted). NOTICE this only affects external commands not internal ones. 
allow_arguments	0	A Boolean flag to determine if arguments are accepted on the incoming socket. If arguments are not accepted you can still use external commands that need arguments but you have to define them in the NRPE handlers below. This is similar to the NRPE "dont_blame_nrpe" option. 
allow_nasty_meta_chars	0	Allow NRPE execution to have “nasty” meta characters that might affect execution of external commands (things like > “ etc). 
script_dir	When set all files in this directory will be available as scripts. This is pretty dangerous but can be a bit useful if you use many scripts and you are sure no one else can add files there. 
[External Scripts] 
A list of scripts available to run from the CheckExternalScripts module. Syntax is: <command>=<script> <arguments> for instance: 
check_es_long=scripts\long.bat 
check_es_ok=scripts\ok.bat 
check_es_nok=scripts\nok.bat 
check_vbs_sample=cscript.exe //T:30 //NoLogo scripts\check_vb.vbs 
[External Alias] 
Works like the "inject" concept of NRPE scripts module. But in short a list of aliases available. An alias is an internal command that has been "wrapped" (to add arguments). Be careful so you don't create loops (ie check_loop=check_a, check_a=check_loop) 
alias_cpu=checkCPU warn=80 crit=90 time=5m time=1m time=30s 
alias_disk=CheckDriveSize MinWarn=10% MinCrit=5% CheckAll FilterType=FIXED 
alias_service=checkServiceState CheckAll 
alias_mem=checkMem MaxWarn=80% MaxCrit=90% ShowAll type=physical 
LUAScript.dll 
This module allows you to write and change checks in the Lua scripting language. For a quite "guide" on how to write scripts for NSClient++ see the LUAScript/guide page. For information on the Lua scripting language and built-in modules and commands refer to the official Lua pages at: http://www.lua.org/ and http://lua-users.org/wiki/SampleCode 
Configuration 
[LUA Scripts] 
A list of LUA script to load at startup. In difference to "external checks" all LUA scripts are loaded at startup. Names have no meaning since the script (on boot) submit which commands are available and tie that to various functions. 
[LUA Scripts] 
scripts\test.lua 
This is just a quick intro, I will try to add more info here and also try to add more system related functions (like WMI and performance counter access) in the future. 
Debugging Lua 
Use the print statement to print to the console (can bee sen from nsclient++ /test). 
With a Lua script like this loaded: 
register_command('lua_debug', 'debug') 

function debug (command) 
    print ('Hello world: ' .. command) 
end 
Then you run nsclient++ /test: 
nsclient++ /test 
... 
lua_debug Greetings 
... 
d \nsclient++.cpp(540) Injecting: lua_debug: Greetings 
Hello world: lua_debug 
e \script_wrapper.hpp(280) No arguments returned from script. 
l \nsclient++.cpp(575) No handler for command: 'lua_debug' 
A simple script 
print('Loading test script...') -- Just print some debug info 

register_command('check_something', 'something') -- Register a check-command to a function 

function something (command) 
  -- Check command function (notice arguments are not supported yet) 

  -- Inject and run another check command 
  code, msg, perf = inject('CheckCPU','time=5','MaxCrit=5') 
  -- Print the resulting code 
  print(code .. ': ' .. msg .. ', ' .. perf) 
  -- Return the information (slightly modified) 
  return code, 'hello from LUA: ' .. msg, perf 
end 
Structure of a script 
First all script register all commands they will use (it is possible to register commands at a later time) So you could have a command that "turn on" other commands, but since there is no "turn off" (ie. remove) it does not make much sense as of yet. 
To register command you call the register_command function like so: 
register_command('command_alias', 'function_in_lua_to_use'); 
This will when the command command_alias is run execute the function_in_lua_to_use in your script. You can have as many commands as you like so the following is possible: 
register_command('lua_1', 'lua_function_1'); 
register_command('lua_2', 'lua_function_2'); 
register_command('lua_3', 'lua_function_3'); 
register_command('lua_4', 'lua_function_4'); 
register_command('lua_5', 'lua_function_5'); 
The functions have the following syntax: 
function lua_check_function (command) 
  print ('Hello world: ' .. command) 
  return 'ok', 'Everything is fine!', 'fine=10%;80;90;' 
As of now there are no support for arguments but in the future they will be added. Printing from a check_function is useless (apart from debug) so generally don't do that. The return is a variable list If; 
•	3 options are returned they are assumed to be in order: code, message and performance data 
•	2 options are returned they are assumed to be in order: code, message 
•	1 options are returned they are assumed to be in order: code 
The code can be: 
•	crit (critical) 
•	warn (warning) 
•	ok (ok) 
•	error (critical) 
A 'useful' script 
-- Register the command 
register_command('has', 'check_file_exists') 

-- Return true if file exists and is readable. 
function file_exists(path) 
  local file = io.open(path, "rb") 
  if file then file:close() end 
  return file ~= nil 
end 

function check_file_exists (command) 
  if file_exists('c:\\foo.bar') then 
    return 'ok', 'File exists' 
  else 
    return 'crit', 'File does not exist' 
  end 
end 


USAGE 
TODO 
for now: 
If you only wish to test it or debug the client you can use the following without installing it first. 
NSClient++ /test 
When you see this prompt: 
Enter command to inject or exit to terminate... 
you can paste in examples from the Commands page at CheckCommands. For example, I get this on my system: 
C:\Program Files\NSClient++>NSClient++.exe -test 
Launching test mode... 
d \NSClient++.cpp(375) Loading plugin: CheckDisk... 
d \NSClient++.cpp(375) Loading plugin: CheckSystem... 
d \CheckSystem.cpp(79) Autodetected W2K>, using TOOLHELP process enumeration. 
d \PDHCollector.cpp(83) Autodetected w2k or later, using w2k PDH counters. 
d \NSClient++.cpp(375) Loading plugin: NSClient server... 
d \PDHCollector.cpp(103) Detected language: English US (0x0409) 
l \NSClient++.cpp(123) Using settings from: INI-file 
d \Socket.h(538) Bound to: 0.0.0.0:12489 
l \NSClient++.cpp(124) Enter command to inject or exit to terminate... 
CheckDriveSize ShowAll MinWarnFree=10% MinCritFree=5% Drive=c:\ 
d \NSClient++.cpp(440) Injecting: CheckDriveSize: ShowAll, MinWarnFree=10%, MinC 
ritFree=5%, Drive=c:\ 
d \NSClient++.cpp(460) Injected Result: OK 'OK: c:\: 12.3G' 
d \NSClient++.cpp(461) Injected Performance Result: ''c:\'=38%;10;5; ' 
OK:OK: c:\: 12.3G|'c:\'=38%;10;5; 
exit 
d \NSClient++.cpp(349) Unloading plugin: NSClient server... 
d \Socket.h(558) Listener is preparing to shutdown... 
d \NSClient++.cpp(349) Unloading plugin: CheckSystem... 
d \NSClient++.cpp(349) Unloading plugin: CheckDisk... 

C:\Program Files\NSClient++> 

Configuration 
Configuration is fairly simple and straight forward. Open the configuration file in notepad (or you favorite editor) "notepad <installation path>\NSC.ini" and edit it accordingly. A longer description of the Configuration file is included in the following page. 
The file has sections (denoted with section name in brackets) and key/value pairs (denoted by key=value). Thus it has the same syntax as pretty much any other INI file in windows. 
The sections are described in short below. The default configuration file has a lot of examples and comments so make sure you change this before you use NSClient++ as some of the examples might be potential security issues. 
The configuration can also be stored in the system registry (HKLM\Software\NSClient++) there is currently no UI to configure this so the simplest way is to maintain the configuration in the INI file and "Migrate that" to the registry. This is can be done via the [RemoteConfiguration] module but in short: 
NSClient++ -noboot RemoteConfiguration ini2reg 
A sample configuration file is included in the download but can also be found here trunk/NSC.dist 
Modules 
This is a list of modules to load at startup. All the modules included in this list has to be NSClient++ modules and located in the modules subdirectory. This is in effect the list of plug-ins that will be available as the service is running. For information on the various plug-ins check the Modules section in the navigation box. 
A good idea here is to disable all modules you don’t actually use for two reasons. One less code equals less potential security holes and two less modules means less resource drain. 
Settings 
This section has generic options for how NSClient++will work, some of these settings (such as allowed_hosts) is inherited in sections below so it is probably a better idea to set them here in the "global" section. 
The options you have available here are 
Option	Default value	Description 
obfuscated_password	...	An obfuscated version of password. For more details refer to the password option below. To create the obfuscated Password use: "NSClient++.exe /encrypt" 
password	...	The password used by various (presently only NSClient) daemons. If no password is set everyone will be able to use this service remotely. 
allowed_hosts	127.0.0.1	A list (comma separated) with hosts that are allowed to connect and query data. If this is empty all hosts will be allowed to query data. BEWARE: NSClient++ will not resolve the IP address of DNS entries if the service is set to startup automatically. Use an IP address instead. 
use_file	0	Has to be set to 1 if you want the file to be read (if set to 0, and the use_reg is set to 1 the registry will be used instead) 
Advanced options: 
Option	Default value	Description 
master_key	...	The secret "key" used when (de)obfuscating passwords. 
cache_allowed_hosts	1	Used to cache looked up hosts if you check dynamic/changing hosts set this to 0. 
Log 
This section has options for how logging is performed. First off notice that for logging to make sense you need to enable the “FileLogger.dll” module that logs all log data to a text file in the same directory as the NSClient++ binary if you don’t enable any logging module nothing will be logged. 
The options you have available here are 
Option	Default value	Description 
debug	0	A Boolean value that toggles if debug information should be logged or not. This can be either 1 or 0. 
file	nsclient.log	The file to write log data to. If no directory is used this is relative to the NSClient++ binary. 
date_mask	%Y-%m-%d %H:%M:%S	The date format used when logging to a file 
NSClient 
This is the NSClient module configuration options. 
Option	Default value	Description 
port	12489	The port to listen to 
obfuscated_password	An obfuscated version of password. For more details refer to the password option below.
password	The password that incoming client needs to authorize themselves by. This option will replace the one found under Settings for NSClient. If this is blank the option found under Settings will be used. If both are blank everyone will be granted access. 
allowed_hosts	A list (coma separated) with hosts that are allowed to poll information from NSClient++. This will replace the one found under Setting for NSClient if present. If not present the same option found under Settings will be used. If both are blank all hosts will be allowed to access the system. BEWARE: NSClient++ will not resolve the IP address of DNS entries if the service is set to startup automatically. Use an IP address instead or set cache_allowed_hosts=0 see above. 
bind_to_address	The address to bind to when listening to sockets, useful if you have more then one NIC/IP address and want the agent to answer on a specific one. 
socket_timeout	30	The timeout when reading packets on incoming sockets. If the data has not arrived within this time we will bail out. and discard the connection. 
version	auto	The version number to return for the CLIENTVERSION check (useful to "simulate" an old/different version of the client, auto will be generated from the compiled version string inside NSClient++ 
Advanced options: 
Option	Default value	Description 
socket_back_log	Number of sockets to queue before starting to refuse new incoming connections. This can be used to tweak the amount of simultaneous sockets that the server accepts. This is an advanced option and should not be used. 
NRPE 
This is configuration for the NRPE module that controls how the NRPE listener operates. 
Option	Default value	Description 
port	5666	The port to listen to 
allowed_hosts	A list (coma separated) with hosts that are allowed to poll information from NRPE. This will replace the one found under Setting for NRPE if present. If not present the same option found under Settings will be used. If both are blank all hosts will be allowed to access the system 
use_ssl	1	Boolean value to toggle SSL encryption on the socket connection 
bind_to_address	The address to bind to when listening to sockets. 
command_timeout	60	The maximum time in seconds that a command can execute. (if more then this execution will be aborted). NOTICE this only affects external commands not internal ones. 
allow_arguments	0	A Boolean flag to determine if arguments are accepted on the incoming socket. If arguments are not accepted you can still use external commands that need arguments but you have to define them in the NRPE handlers below. This is similar to the NRPE "dont_blame_nrpe" option. 
allow_nasty_meta_chars	0	Allow NRPE execution to have “nasty” meta characters that might affect execution of external commands (things like > “ etc). 
socket_timeout	30	The timeout when reading packets on incoming sockets. If the data has not arrived within this time we will bail out. and discard the connection. 
script_dir	Load all scripts in a directory and use them as commands. Probably dangerous but usefull if you have loads of scripts![](http://hongliangpan.iteye.com/images/smiles/icon_smile.gif)
Advanced options: 
Option	Default value	Description 
performance_data	1	Send performance data back to nagios (set this to 0 to remove all performance data) 
socket_back_log	Number of sockets to queue before starting to refuse new incoming connections. This can be used to tweak the amount of simultaneous sockets that the server accepts. This is an advanced option and should not be used. 
string_length	1024	Length of payload to/from the NRPE agent. This is a hard specific value so you have to "configure" (read recompile) your NRPE agent to use the same value for it to work. 
Check System 
Here you can set various options to configure the System Check module. 
Option	Default value	Description 
CPUBufferSize	1h	The time to store CPU load data. 
CheckResolution? 
10	Time between checks in 1/10 of seconds. 
Advanced options: 
Option	Default value	Description 
auto_detect_pdh	1	Set this to 0 to disable auto detect (counters.defs) PDH language and OS version. 
dont_use_pdh_index	0	Set this to 1 if you dont want to use indexes for finding PDH counters. 
force_language	Set this to a locale ID if you want to force auto-detection of counters from that locale. 
ProcessEnumerationMethod? 
auto	Set the method to use when enumerating processes PSAPI, TOOLHELP or auto 
check_all_services[SERVICE_BOOT_START]	ignored	Set how to handle services set to SERVICE_BOOT_START state when checking all services 
check_all_services[SERVICE_SYSTEM_START]	ignored	Set how to handle services set to SERVICE_SYSTEM_START state when checking all services 
check_all_services[SERVICE_AUTO_START]	started	Set how to handle services set to SERVICE_AUTO_START state when checking all services 
check_all_services[SERVICE_DEMAND_START]	ignored	Set how to handle services set to SERVICE_DEMAND_START state when checking all services 
check_all_services[SERVICE_DISABLED]	stopped	Set how to handle services set to SERVICE_DISABLED state when checking all services 
MemoryCommitLimit? 
\Memory\Commit Limit	Counter to use to check upper memory limit. 
MemoryCommitByte? 
\Memory\Committed Bytes	Counter to use to check current memory usage. 
SystemSystemUpTime? 
\System\System Up Time	Counter to use to check the uptime of the system. 
SystemTotalProcessorTime? 
\Processor(_total)\% Processor Time	Counter to use for CPU load. 
ProcessEnumerationMethod? 
auto	Set the PROCESS enumeration method (auto or TOOLHELP or PSAPI) 
External Script 
Configure how the External Scripts module works (not to be confused with the "External Scripts" section below that holds scripts that can be run. 
Option	Default value	Description 
command_timeout	60	The maximum time in seconds that a command can execute. (if more then this execution will be aborted). NOTICE this only affects external commands not internal ones. 
allow_arguments	0	A Boolean flag to determine if arguments are accepted on the incoming socket. If arguments are not accepted you can still use external commands that need arguments but you have to define them in the NRPE handlers below. This is similar to the NRPE "dont_blame_nrpe" option. 
allow_nasty_meta_chars	0	Allow NRPE execution to have “nasty” meta characters that might affect execution of external commands (things like > “ etc). 
script_dir	When set all files in this directory will be available as scripts. This is pretty dangerous but can be a bit useful if you use many scripts and you are sure no one else can add files there. 
External Scripts 
A list of scripts available to run from the CheckExternalScripts module. Syntax is: <command>=<script> <arguments> for instance: 
check_es_long=scripts\long.bat 
check_es_ok=scripts\ok.bat 
check_es_nok=scripts\nok.bat 
check_vbs_sample=cscript.exe //T:30 //NoLogo scripts\check_vb.vbs 
External Alias 
Works like the "inject" concept of NRPE scripts module. But in short a list of aliases available. An alias is an internal command that has been "wrapped" (to add arguments). Be careful so you don't create loops (ie check_loop=check_a, check_a=check_loop) 
alias_cpu=checkCPU warn=80 crit=90 time=5m time=1m time=30s 
alias_disk=CheckDriveSize MinWarn=10% MinCrit=5% CheckAll FilterType=FIXED 
alias_service=checkServiceState CheckAll 
alias_mem=checkMem MaxWarn=80% MaxCrit=90% ShowAll type=physical 
Eventlog 
Some options for the EventLog? module. 
Option	Default value	Description 
syntax	Set this to use a specific syntax string for all commands (that don't specify one) 
Advanced options: 
Option	Default value	Description 
debug	0	Log all "hits" and "misses" on the eventlog filter chain, useful for debugging eventlog checks but very very very noisy so you don't want to accidentally set this on a real machine. 
includes 
A list of other configuration files to include when reading this file. Might be useful if you have a very complex setup or want to have setting split up in segments. 
NSCA Agent 
Options to configure the new NSCA module. 
Option	Default value	Description 
interval	60	Time in seconds between each report back to the server (cant as of yet be set individually so this is for all "checks") 
nsca_host	...	The NSCA/Nagios(?) server to report results to. 
nsca_port	5667	The NSCA server port 
encryption_method	1	Number corresponding to the various encryption algorithms (see below). Has to be the same as the server or it wont work at all. 
password	The password to use. Again has to be the same as the server or it wont work at all. 
Advanced options: 
Option	Default value	Description 
hostname	The host name of this host if set to blank (default) the windows name of the computer will be used. 
debug_threads	1	Number of threads to run, no reason to change this really (unless you want to stress test something) 
Supported encryption methods: 
#	Algorithm 
0	None (Do NOT use this option) 
1	Simple XOR (No security, just obfuscation, but very fast) 
2	DES 
3	3DES (Triple DES) 
4	CAST-128 
6	xTEA 
8	BLOWFISH 
9	TWOFISH 
11	RC2 
14	RIJNDAEL-128 (AES) 
20	SERPENT 
NSCA Commands 
A list of commands to run and submit each time we report back to the NSCA server. A command starting with host_ will be submitted as a host command. For an example see below: This will report back one service check (called my_cpu_check) and one host check (host checks has no service name). 
[NSCA Commands] 
my_cpu_check=checkCPU warn=80 crit=90 time=20m time=10s time=4 
host_check=check_ok 
LUA Scripts 
A list of LUA script to load at startup. In difference to "external checks" all LUA scripts are loaded at startup. Names have no meaning since the script (on boot) submit which commands are available and tie that to various functions. 
[LUA Scripts] 
scripts\test.lua 
NRPE Handlers 
This is a list of handlers for NRPE execution this can of course be used by any module (such as NSClient) but for historical reasons they are located in this section especially as NRPE plug-in is the one that does the actual execution. 
The handlers can have two different syntaxes: 
•	command[my_command]=/some/executable 
•	my_command=/some/executable 
The latter is the preferred way as it is shorter. 
About NSClient++ 
NSClient++ (or nscp as I tend to call it nowadays) aims to be a simple yet powerful and secure monitoring daemon for Windows operating systems. It is built for Nagios, but nothing in the daemon is actually Nagios specific and could probably, with little or no change, be integrated into any monitoring software that supports running user tools for polling. 
The structure of the daemon is a simple NT service that loads plug-ins to an internal stack. The plug-ins can then request data (poll performance data) from the other plug-ins through the internal stack. As of now there are a few plug-ins for basic performance data collection. For details of supplied modules, see CheckCommands. 
NSClient++ can be extended in two ways: you can either write your own plug-in or you can execute an external script (as of now batch/exe/*). Writing your own plug-in is, of course, the most powerful way but requires knowledge of C++ or other languages which can produce DLLs and interface with regular C programs (generally, every other language available, but there is some simple API helpers for C/C++ as well as descriptions). 
As for checking with NSClient++, I would recommend NRPE as it is a lot more flexible than check_nt. But NSClient has full support for check_nt, and if there is an interest, I could probably add support for check_nt from nc_net. 
Supported OS/Platform 
NSClient++ should run on the following operating systems: 
•	NT4 (SP5?) 
•	Windows 2000 W2K 
•	Windows XP 
•	Windows 2003 
•	Windows Vista 
•	... 
...as well as the following platforms: 
•	Win32 
•	x64 (AMT64/EMT64) 
•	IA64 (Itanium) 
Whats in a name? 
Since I have noticed some ppl. use other names for the client I decided to list them here to make it simpler (ie. Goggle might find it) for people to find it. 
•	NSClient++ (the "real" name) 
•	NSCP (what I sometimes use) 
•	NSClientpp (version of NSCLient++) 
•	NSClient (?) 
•	Saw a French (Spanish) site use: NSC++ 
Again I myself as stated before prefer NSClient++ or NSCP. 
Installation 
NSClient++ comes with simple command line option for registering (and deregistering) the service but it does not have a GUI installer. 
Thus to install the Client you only need to copy the files to a directory of your choice and then run “NSClient++ /install”. 
Before you start NSClient++ you need to configure it by editing the configuration file (NSC.ini). The configuration file is a simple text file and is explained in detail under Configuration. The files needed by NSClient++ varies but mainly the exe and DLL's in the NSClient++ root are required as well as all the modules you plan to use from the modules subdirectory (/modules/*). 
The configuration file (NSC.ini) NEEDS to be configured as for security reasons all plug-ins are disabled by default. The reason for this is so no one will accidentally install this and get potential security issues, I believe that things should be "off" by default. Also notice that by default allowed_hosts are 127.0.0.1 so you need to modify this as well. 
If you plan to use the SystemTray module (that shows a system tray icon on the desktop you need to install the SystemTray module as well as NSClient++. To install NSClient++ execute the following command: 
NSClient++ /install 
NSClient++ SysTray install 
To uninstall NSClient++ execute the following command: 
NSClient++ SysTray uninstall 
NSClient++ /uninstall 
To start NSClient++ execute the following command: 
NSClient++ /start 
To stop NSClient++ execute the following command: 
NSClient++ /stop 
If you only wish to test it or debug the client you can use the following without installing it first. 
NSClient++ /test 
Firewall 
Firewall configuration should be pretty straight forward: 
If you use NRPEListener (check_nrpe) you need the NRPE port open (usually 5666) from the nagios server towards the client. 
nagios:* -> client:5666 
If you use the NSClientListener (check_nt) you need the (modified) NSClient port open (usually 12489) from the nagios server towards the client. 
nagios:* -> client:12489 
If you use the NSCA Module (passive checks) you need the NSCA port open from the client towards the nagios server. 
client:* -> nagios:5667 
If you use the NRPEClient module to check any remote systems (use NSClient++ as a proxy) you need to have NRPE port (usually 5666) open from NSClient++ (the proxy) to the remote-client in addition to the method you use to submit the results to the server. 
nsclient-proxy:* -> remote-client:5666 
All these ports can be changed so check your nsc.ini. 
NT4 
NT4 does not come with the PDH library and you need to install that before using NSClient++. PDH can be downloaded from Microsoft: http://support.microsoft.com/default.aspx?scid=kb;en-us;Q284996 and the simplest way to install it is to uncompress it directly into the NSClient++ directory. 
NT4 also (sometimes) lack the PSAPI helper which is available in the "Platform SDK Redistributable: PSAPI for Windows NT" from Microsoft. http://www.microsoft.com/downloads/details.aspx?FamilyID=3d1fbaed-d122-45cf-9d46-1cae384097ac as with the PDH either install in system32 or local NSClient++ directory. 









