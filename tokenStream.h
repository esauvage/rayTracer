#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H

#include <string>
#include <istream>

using namespace std;

enum class Kind : char {
	name, number, end, sep=',', type, group
};

struct Token {
	Kind kind;
	string string_value;
	float number_value;
};

class Token_stream
{
public:
	Token_stream(istream& s):ip{&s}, owns{false} {}
	Token_stream(istream* p):ip{p}, owns{true} {}
	~Token_stream() {close();}
	Token get();
	const Token & current() {return ct;}
	void set_input(istream& s) {close(); ip = &s; owns = false;}
	void set_input(istream* p) {close(); ip = p; owns = true;}
private:
	void close() {if (owns) delete ip;}
	
	istream *ip;
	bool owns;
	Token ct{Kind::end, "", 0.};
};

#endif //TOKEN_STREAM_H
