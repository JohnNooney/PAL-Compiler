#include "../scanner.hpp"

const char *program = NULL;

int count_lets(std::istream& source){
    std::cout << "counting let statements... \n";

    MyScanner scanner(source);

    int lets = 0;

    while(!scanner.end()){
        Token token = scanner.lex();
        if(token.type() == "let") lets += 1;
    }

    std::cout << lets << "assignment (let) statements found. \n";
    std::cout << "done.\n";
    return EXIT_SUCCESS;
}

[[noreturn]]
void throw_error(const std::string& error){
    std::cerr << "fatal error: " << error << "\n";
    std::exit(EXIT_FAILURE);
}

int main(int args_count, const char** args) {
    program = args[0];

    std::string path{args[2]};
    std::ifstream source{path};
    if(!source.is_open()){
        throw_error("cannot open '" + path + "' for reading");
    }
    
    std::string cmd{args[1]};
    if(cmd == "count-let"){
        return count_lets(source);
    }
    
    return 0;
}