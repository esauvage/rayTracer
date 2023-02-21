#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>

using namespace std;

class Parameter
{
public:
	struct Bad_entry{}; //pour les exceptions.
	~Parameter();
	Parameter & operator =(const Parameter& p); // Nécessaire à cause de string
	Parameter(const Parameter&);
	Parameter(float v);
	Parameter(const string & ss);
	Parameter();
	
	float number() const;
	string text() const;
	
	void setNumber(float f);
	void setText(const string& ss);
private:
	enum class Tag {number, text};
	Tag type; //discriminant
	
	union {	//representation
		float f;
		string s; //string a un constructeur par défaut, les opérateurs de copie et un destructeur
	};
};

#endif// PARAMETER_H