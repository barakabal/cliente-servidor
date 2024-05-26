This's a simple bittorrent made in C language.

To Build and run:
gcc aplicacao.c -o <execname> -lpthread
./<execname>

When the first time the program was executed on prompt, the options will show like below

[1] RCA user List
[2] User list. 
[3] Download files from a server

The program waits for an entry, being a number corresponding to the menu.
Option 1: all servers running on different machines will each return
the real and local IP address for the machine that requested and save the logs to a file
information. The IP address of each server is printed on the screen of the person requesting the option.
A file called log.txt is created where the program stores the log of requests (when the
the program is in the server feature).

Option 2: all servers will return the list of files contained in their current directory (the
directory where the application executable is located)
Note: The server returns a buffer with a maximum size of 1000
The server starts storing the filenames in a vector, and if this vector exceeds the size
1000 an error will occur. For directories that host many files, failure will occur.
segmentation (because the vector ends up overflowing).
If it is necessary to list file names for a very large directory, please set the macro below
with higher value.
#define BUFFSIZE 1000

Option 3:
prompts the user to enter the server's IP address.
prompts the user to type the file name (we only consider a text file).
We only consider text files for download.
The file content also cannot be longer than 1000 characters.
If it breaks, segmentation fault will occur.

Notes:
We consider the files to be downloaded as text files.
The content of the downloadable file cannot be longer than 1000 characters
If you need to download larger files, please set the macro to a higher value
#define BUFFSIZE 1000
A log.txt file is created where the server records requests
