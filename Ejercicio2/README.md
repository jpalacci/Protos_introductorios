## Ej2 ##

To compile:

>make

To clean:

>make clean

-----------------------------------------

## Usage ##

>./ej2 buffer_lenght command

Receives as first parameter the length of the buffer that will be used in read and write operations. 
Receives as second parameter the command to execute.

The command will use the stdin of the program.

Example of usage:

>echo -n hola | ./ej2 256 "sed s/o/0/g | sed s/a/4/g"

Should output:

>h0l4
>
>in parity: 0x0A
>out parity: 0x00

-------------------------------------------

## Notes ##
* We introduced the buffer_lenght parameter in order to test the program performance with different buffer lenghts
* The program removes the trailing new line character in the output of the command and then calculates the output xor.
