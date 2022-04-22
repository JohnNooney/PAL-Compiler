// 
// Compiler.cpp
// 
// This is your compiler's entry point. The `main()` function is called when your program starts,
// and the command-line arguments are given to you in the `args` string array. Use this to invoke
// your parser, scanner, or anything else you're using.
//
//

// Standard library includes, we need those to print to the console and read files
#include <iostream>
#include <cstdlib>
#include <fstream>

// Our actual classes!
//#include "myblock1compiler/MyScanner.hpp"
//#include "myblock1compiler/MyParser.hpp"
//#include "myblock1compiler/apps/countLet.hpp"
//#include "myblock1compiler/MyParserEBNF.hpp"
#include "mypalcompiler/MyPALScanner.hpp"
#include "mypalcompiler/MyPALParser.hpp"

// Attempts to load a file from a path into a read-only file stream, or exits the program
// entirely if it fails. This is a barebones implementation -- feel free to add to it to
// indicate *what* the error was, etc.
std::ifstream load_file(const char* path) {
	std::ifstream file{ path };

	if(!file.is_open()) {
		std::cerr << "fatal error: could not open source file `" << path << "`.\n";
		exit(EXIT_FAILURE);
	}

	return file;
}

// Program entry point. You can set the path to your PAL/Block-1 source text file as a
// command-line argument in VS's project settings.
// (Project > [name] Properties > Debugging > Command Arguments)
//
// Params:
//	- arg_count		the number of arguments given on the command line; always at least 1.
//	- args			the arguments; args[0] is always the name of your running program.
//
// Returns:
//	0 if the the compiler ran correctly, anything else if there was an error.
int main(int arg_count, const char* args[]) {

	// Check that we have the right number of arguments. We should have 2:
	//	- the first one is always the name of the program that was called, so we ignore it
	//	- the second one should be the path to the source file we want to parse
	if(arg_count != 2) {
		std::cerr << "fatal error: wrong number of arguments\n";
		std::cerr << "usage: " << args[0] << " SOURCE_FILE_PATH\n";
		exit(EXIT_FAILURE);
	}

	// Load the source file into a stream.
	std::ifstream source = load_file(args[1]);

	// create scanner object
	//MyScanner scanner(source);
	MyPALScanner scanner(source);

	// use BNF Parser
	/*MyParser parser = MyParser(scanner);
	parser.recStarter();*/

	// use EBNF Parser
	/*MyParserEBNF parserEBNF(scanner);
	bool success = parserEBNF.compile();

	for (const auto& error : parserEBNF.errors()) {
		std::cout << error << "\n";
	}*/

	// use PAL Parser
	MyPALParser parser(scanner);
	parser.recStarter();

	for (const auto& error : parser.errors()) {
		std::cout << error << "\n";
	}

	std::cout << "done.\n";

	// run test program
	/*CountLet countLet = CountLet();
	std::string cmd{ args[0] };
	if (cmd == "count-let") {
		return countLet.count_lets(source);
	}*/
	
	//return success ? EXIT_SUCCESS : EXIT_FAILURE;
	return 0;
}
