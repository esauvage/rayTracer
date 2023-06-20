#include "stl2json.h"

#include <map>
#include <iostream>

using namespace std;

//Extrait les arguments de la ligne de commande en système "clef", "valeur".
//Une clef n'a de valeur associée que si :
// - elle commence par '-'
// - la chaîne suivante ne commence pas par '-'.
//Ainsi, "./usr/bin/truc toto -w64 -o ./test.txt -h 320 titi" sera décomposée comme suit :
// "./usr/bin/truc", 
// "toto", 
// "-w64",
// "-o" : "./test.txt", 
// "-h" : "320", 
// "titi"
map <string, string> arguments(int argc, char *argv[])
{
	map <string, string> res;
	for (auto i = 0; i < argc; i++)
	{
		const string k {argv[i]};
		res[k] = string{};
		if ((k[0] == '-') && (i < argc - 1) && (argv[i + 1][0] != '-'))
		{
			res[k] = argv[++i];
		}
	}
	return res;
}

int main(int argc, char *argv[])
{
	auto args = arguments(argc, argv);//atoi ne prend pas de const
	for (auto&& arg : args)
	{
		cout << arg.first << " : " << arg.second << std::endl;
	}
	const pair <int, int> size {atoi(args["-w"].c_str()), atoi(args["-h"].c_str())};
    STL2JSON stl2json;
    stl2json.convert(args["-i"].c_str(), args["-o"].c_str());
	return 0;
}
