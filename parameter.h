#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>

class Parameter
{
public:
	struct Bad_entry{}; //pour les exceptions.
	~Parameter();
	Parameter & operator =(const Parameter& p); // Nécessaire à cause de string
	Parameter(const Parameter&);
    Parameter(float v);
    Parameter(const std::string & ss);
	Parameter();
	
    float number() const;
    std::string text() const;
	
    void setNumber(float f);
    void setText(const std::string& ss);
private:
	enum class Tag {number, text};
	Tag type; //discriminant
	
	union {	//representation
        float f;
        std::string s; //string a un constructeur par défaut, les opérateurs de copie et un destructeur
	};
};

#endif// PARAMETER_H
