#include "MySema.hpp"

MySema::MySema(Parser& parser) : Sema(parser) {
}

MySema::~MySema() {
}

// check if variable has been declared in current scope.
//  If not then add to symbol table's scope
void MySema::define(const Token& var, Type type)
{
	if (Scope::current()->isDefined(var.text())) {
		redeclaredError(var);
		return;
	}

	Scope::current()->define(var, type);
}

// Check the type of the variable and return it.
// if type is invalid then throw undecalred error
Type MySema::checkVariable(const Token& var)
{
	Type type = variableType(var);
	
	if (type == Type::Invalid) {
		undeclaredError(var);
	}

	return type;
}

// return the type stored in the symbol table
Type MySema::variableType(const Token& var)
{
	const Symbol* sym = Scope::current()->get(var.text());
	return sym ? sym->type : Type::Invalid;
}


Type MySema::checkExpression(Type left, const Token& op, Type right)
{
	// If both sides of the epxression are Invalid then return 
	if (left == Type::Invalid && right == Type::Invalid) {
		return Type::Invalid;
	}

	// If only one side of the expression is invalid, use the type of the other side
	if (left == Type::Invalid) {
		left = right;
	}
	if (right == Type::Invalid) {
		right = left;
	}

	//throw error if both sides are different types
	if (left != right) {
		binaryExprError(op, left, right);
		return Type::Invalid;
	}

	return left;
}

void MySema::checkBoolean(Type left, const Token& op, Type right)
{
	// If both sides of the epxression are Invalid then return 
	if (left == Type::Invalid && right == Type::Invalid) {
		return ;
	}

	// If only one side of the expression is invalid, use the type of the other side
	if (left == Type::Invalid) {
		left = right;
	}
	if (right == Type::Invalid) {
		right = left;
	}

	//throw error if both sides are different types
	if (left != right) {
		booleanExprError(op, left, right);
		return;
	}

	return;
}

void MySema::checkAssign(const Token& var, Type right)
{
	// check if var is declared first
	if (!Scope::current()->isDefined(var.text())) {
		undeclaredError(var);
		return;
	}

	Type left = variableType(var);

	// If only one side of the expression is invalid, use the type of the other side
	if (right == Type::Invalid) right = left;

	// Throw error if both side are different types.
	if (left != right) {
		assignmentError(var, left, right);
	}
}

