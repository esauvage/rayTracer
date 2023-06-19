#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H

#include <string>
#include <istream>

enum class Kind : char {
	name, number, end, sep=',', type, group
};

struct Token {
    Kind kind;
    std::string string_value;
	float number_value;
};

class Token_stream
{
public:
    Token_stream(std::istream& s):ip{&s}, owns{false} {}
    Token_stream(std::istream* p):ip{p}, owns{true} {}
	~Token_stream() {close();}
	Token get();
    const Token & current() {return ct;}
    void set_input(std::istream& s) {close(); ip = &s; owns = false;}
    void set_input(std::istream* p) {close(); ip = p; owns = true;}
private:
    void close() {if (owns) delete ip;}

    std::istream *ip;
	bool owns;
	Token ct{Kind::end, "", 0.};
};

#endif //TOKEN_STREAM_H
