#include <istream>

class CountLet {
public:
	int count_lets(std::istream&);

private:
	void throw_error(const std::string&);
};