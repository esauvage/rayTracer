#include "parameter.h"

using namespace std;

Parameter::Parameter(const Parameter& p)
	:type(p.type)
{
	switch (type)
	{
		case Tag::number:
			f = p.f;
			return;
		case Tag::text:
			new(&s)string(p.s); //constructeur explicite
	}
}

Parameter::Parameter(float v)
	:type(Tag::number), f(v)
{
}

Parameter::Parameter(const string & ss)
	:type(Tag::text)
{
	new(&s)string(ss);
}

Parameter::Parameter()
{
}

Parameter& Parameter::operator=(const Parameter& p)
{
	if (type==Tag::text)
	{
		if (p.type==Tag::text)
		{
			s = p.s;
			return *this;
		}
		s.~string();//destruction explicite.
	}
	switch (p.type)
	{
		case Tag::number:
			f = p.f;
			break;
		case Tag::text:
			new(&s)string(p.s); //constructeur explicite
	}
	type = p.type;
	return *this;
}

Parameter::~Parameter()
{
	if (type == Tag::text) s.~string(); //destructeur explicite;
}

float Parameter::number() const
{
	if (type != Tag::number) throw Bad_entry{};
	return f;
}

string Parameter::text() const
{
	if (type != Tag::text) throw Bad_entry{};
	return s;
}

void Parameter::setNumber(float v)
{
	if (type == Tag::text)
	{
		s.~string();
		type = Tag::number;
	}
	f = v;
}

void Parameter::setText(const string& ss)
{
	if (type == Tag::text)
	{
		s = ss;
	}
	else
	{
		new(&s)string(ss); //constructeur explicite
		type = Tag::text;
	}
}
