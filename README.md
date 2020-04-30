Research :<br>
[Shell command language](https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_10)
<br>
[Parsing](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf)
<br><br>
Seq : 
- 0 : command
- 2 : arguments
- 3 : >
- 4 : >>
- 5 : <
- 6 : |
- 7 : ;
- 8 : file
- 9 : newline

free termcap : 
- https://www.gnu.org/software/termutils/manual/termcap-1.3/html_mono/termcap.html#SEC4
> If you are using the GNU version of termcap, you can alternatively ask tgetent to allocate enough space. Pass a null pointer for buffer, and tgetent itself allocates the storage using malloc. There is no way to get the address that was allocated, and you shouldn't try to free the storage. With the Unix version of termcap, you must allocate space for the description yourself and pass the address of the space as the argument buffer. There is no way you can tell how much space is needed, so the convention is to allocate a buffer 2048 characters long and assume that is enough. (Formerly the convention was to allocate 1024 characters and assume that was enough. But one day, for one kind of terminal, that was not enough.)
