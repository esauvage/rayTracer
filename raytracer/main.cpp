#include "raytracer.h"

#include <QApplication>
#include <QLoggingCategory>
#include "modeler.h"
#include "vulkanwindow.h"

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
    QApplication app(argc, argv);

    const bool dbg = qEnvironmentVariableIntValue("QT_VK_DEBUG");

    QVulkanInstance inst;

    if (dbg) {
        QLoggingCategory::setFilterRules(QStringLiteral("qt.vulkan=true"));
        inst.setLayers({ "VK_LAYER_KHRONOS_validation" });
    }

    if (!inst.create())
        qFatal("Failed to create Vulkan instance: %d", inst.errorCode());

    VulkanWindow *vulkanWindow = new VulkanWindow(dbg);
    vulkanWindow->setVulkanInstance(&inst);

    auto args = arguments(argc, argv);//atoi ne prend pas de const
    for (auto&& arg : args)
    {
        cout << arg.first << " : " << arg.second << std::endl;
    }

    Modeler mainWindow(vulkanWindow);
    mainWindow.resize(1024, 768);
    mainWindow.show();

    return app.exec();
//	const pair <int, int> size {atoi(args["-w"].c_str()), atoi(args["-h"].c_str())};
//	RayTracer rayTracer;
//	rayTracer.generateFile(args["-o"], size, args["-i"]);
//	return 0;
}
