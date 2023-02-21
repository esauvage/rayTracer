#include "tokenStream.h"

Token Token_stream::get()
{
	char ch = 0;
	*ip >> ch;

	switch (ch)
	{
		case 0 : return ct = {Kind::end};
		case ',':
			return ct = {Kind::sep};// = {static_cast<Kind>(ch)};
		case '[':
			*ip >> ct.string_value;
			ct.kind = Kind::group;
			ct.string_value.pop_back(); //remove last ']'
			return ct;
		default:
			if (isalpha(ch))
			{
				ip->putback(ch);
				*ip >> ct.string_value;
				if (ct.string_value.back() == ',')
				{
					ip->putback(',');
					ct.string_value.pop_back();
				}
				ct.kind = Kind::type;
				return ct;
			}
			if (isdigit(ch) || ch == '-')
			{
				ip->putback(ch);
				*ip >> ct.number_value;
				ct.kind = Kind::number;
				return ct;
			}
	}
	return ct;
}
