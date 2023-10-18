#include "modeler.h"
#include "ui_modeler.h"
#include "vulkanwindow.h"
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QLCDNumber>
#include <QCheckBox>
#include <QGridLayout>
#include <fstream>

#include "renderer.h"

using namespace std;
using namespace nlohmann;

template <class T>
void toBigEndian(QByteArray & a, T val)
{
    char *asChar = ( char* ) & val;
    for (size_t i = 0; i < sizeof(T); ++i)
    {
        a += asChar[i];
    }
    return;
}

QMap <QString, QString> arguments()
{
    QMap <QString, QString> res;
    for (auto i = 0; i < QApplication::arguments().size(); i++)
    {
        const QString k {QApplication::arguments()[i]};
        res[k] = QString{};
        if ((k[0] == '-') && (i <  QApplication::arguments().size() - 1) && (QApplication::arguments()[i + 1][0] != '-'))
        {
            res[k] = QApplication::arguments()[++i];
        }
    }
    return res;
}

Modeler::Modeler(VulkanWindow *vulkanWindow)
{
    //Lecture fichier
//    ifstream i(arguments()["-i"].toStdString());
//    json data;
//    i >> data;
//    _scene = data.get<Scene>();
//    i.close();
//    _scene.updateMaterials();
    Mesh mesh;
    mesh.data()->vertexCount = 4;
    toBigEndian(mesh.data()->geom, 0.f);
    toBigEndian(mesh.data()->geom, 0.f);
    toBigEndian(mesh.data()->geom, 0.f);
    toBigEndian(mesh.data()->geom, 0.f);
    toBigEndian(mesh.data()->geom, 0.f);
    toBigEndian(mesh.data()->geom, 0.f);
    toBigEndian(mesh.data()->geom, 0.f);
    toBigEndian(mesh.data()->geom, 0.f);

    toBigEndian(mesh.data()->geom, 1.f);
    toBigEndian(mesh.data()->geom, 0.f);
    toBigEndian(mesh.data()->geom, 0.f);
    toBigEndian(mesh.data()->geom, 1.f);
    toBigEndian(mesh.data()->geom, 0.f);
    toBigEndian(mesh.data()->geom, 0.f);
    toBigEndian(mesh.data()->geom, 0.f);
    toBigEndian(mesh.data()->geom, 1.f);

    toBigEndian(mesh.data()->geom, 0.f);
    toBigEndian(mesh.data()->geom, 1.f);
    toBigEndian(mesh.data()->geom, 0.f);
    toBigEndian(mesh.data()->geom, 0.f);
    toBigEndian(mesh.data()->geom, 1.f);
    toBigEndian(mesh.data()->geom, 0.f);
    toBigEndian(mesh.data()->geom, 0.f);
    toBigEndian(mesh.data()->geom, 1.f);

    toBigEndian(mesh.data()->geom, 1.f);
    toBigEndian(mesh.data()->geom, 1.f);
    toBigEndian(mesh.data()->geom, 0.f);
    toBigEndian(mesh.data()->geom, 1.f);
    toBigEndian(mesh.data()->geom, 1.f);
    toBigEndian(mesh.data()->geom, 0.f);
    toBigEndian(mesh.data()->geom, 0.f);
    toBigEndian(mesh.data()->geom, 1.f);
    vulkanWindow->renderer()->setBlockMesh(mesh);
    QWidget *wrapper = QWidget::createWindowContainer(vulkanWindow);
    wrapper->setFocusPolicy(Qt::StrongFocus);
    wrapper->setFocus();

    infoLabel = new QLabel;
    infoLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setText(tr("This example demonstrates instanced drawing\nof a mesh loaded from a file.\n"
                          "Uses a Phong material with a single light.\n"
                          "Also demonstrates dynamic uniform buffers\nand a bit of threading with QtConcurrent.\n"
                          "Uses 4x MSAA when available.\n"
                          "Comes with an FPS camera.\n"
                          "Hit [Shift+]WASD to walk and strafe.\nPress and move mouse to look around.\n"
                          "Click Add New to increase the number of instances."));

    meshSwitch = new QCheckBox(tr("&Use Qt logo"));
    meshSwitch->setFocusPolicy(Qt::NoFocus); // do not interfere with vulkanWindow's keyboard input

    counterLcd = new QLCDNumber(5);
    counterLcd->setSegmentStyle(QLCDNumber::Filled);
    counterLcd->display(m_count);

    newButton = new QPushButton(tr("&Add new"));
    newButton->setFocusPolicy(Qt::NoFocus);
    quitButton = new QPushButton(tr("&Quit"));
    quitButton->setFocusPolicy(Qt::NoFocus);
    pauseButton = new QPushButton(tr("&Pause"));
    pauseButton->setFocusPolicy(Qt::NoFocus);

    connect(quitButton, &QPushButton::clicked, qApp, &QCoreApplication::quit);
    connect(newButton, &QPushButton::clicked, vulkanWindow, [=] {
        vulkanWindow->addNew();
        m_count = vulkanWindow->instanceCount();
        counterLcd->display(m_count);
    });
    connect(pauseButton, &QPushButton::clicked, vulkanWindow, &VulkanWindow::togglePaused);
    connect(meshSwitch, &QCheckBox::clicked, vulkanWindow, &VulkanWindow::meshSwitched);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(infoLabel, 0, 2);
    layout->addWidget(meshSwitch, 1, 2);
    layout->addWidget(createLabel(tr("INSTANCES")), 2, 2);
    layout->addWidget(counterLcd, 3, 2);
    layout->addWidget(newButton, 4, 2);
    layout->addWidget(pauseButton, 5, 2);
    layout->addWidget(quitButton, 6, 2);
    layout->addWidget(wrapper, 0, 0, 7, 2);
    setLayout(layout);
}

QLabel *Modeler::createLabel(const QString &text)
{
    QLabel *lbl = new QLabel(text);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    return lbl;
}
