#include "../CompilerKit/Sema.hpp"
using namespace CompilerKit;

class MySema : public Sema {
public:
    MySema(Parser& parser);
    virtual ~MySema();

    void define(const Token& var, Type type);

    Type checkVariable(const Token& var);
    Type variableType(const Token& var);

    Type checkExpression(Type lhs, const Token& op, Type rhs);
    void checkBoolean(Type lhs, const Token& op, Type rhs);
    void checkAssign(const Token& var, Type rhs);
};