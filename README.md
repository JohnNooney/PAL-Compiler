# PAL-Compiler
 A recursive descent LL(1) Compiler for the PAL language. 
 
 **Note**: The Syntatic and Semantic analyser are fully implemented but the compiler is not able to execute PAL programs
 
# How to Use
In order to test the syntatic and semantic correctness of your program follow these steps:

1. Write in a text file your PAL program
2. Navigate to the root directory of this repo in command line
3. Enter `Compiler.exe path/to/pal-program.txt`

The compiler will have no output if the program successfully pases the semantic and syntatic analyser.
However if the program contains an error, information relating to the error will be output to the console.
