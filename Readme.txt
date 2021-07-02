Information

1. In order to create the program we have to use the command "make"
2. We have to call the command with the file we want to read e.i ./simpleshell file.txt
3. If no errors on the command it will display the output of the commands otherwise it will exit

Resources:

Main resources for this HW was the man command page and other online source like the following:

0.https://www.gnu.org/software/libc/manual/html_node/Process-Creation-Example.html
1.https://stackoverflow.com/questions/27541910/how-to-use-execvp
2.https://linux.die.net/man/3/execvp
3.https://www.programiz.com/c-programming/c-file-input-output
4.http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/create.html
5.https://linux.die.net/man/3/waitpid


Limitations;

When using the command "make" a warning comes up saying waitID which is pid_t is not being used in line 109. I am not sure why i get this warning even though I am using this processor in line 139. Despite the warning, the program runs with no issues.
