#ifndef MODELER_H
#define MODELER_H

#include <QMainWindow>
#include <QWidget>
#include "../common/boule.h"
#include "meshv.h"

namespace Ui {
class Modeler;
}

QT_BEGIN_NAMESPACE
class QLCDNumber;
class QLabel;
class QPushButton;
class QCheckBox;
QT_END_NAMESPACE

class VulkanWindow;
class Renderer;

class Modeler : public QMainWindow
{
public:
    Modeler(VulkanWindow *vulkanWindow);

private slots:
    void onRendererCreated(Renderer * r);
    void onMousePressed(QMouseEvent *e);
private:
    QLabel *createLabel(const QString &text);

    QLabel *infoLabel;
    QCheckBox *meshSwitch;
    QLCDNumber *counterLcd;
    QPushButton *newButton;
    QPushButton *quitButton;
    QPushButton *pauseButton;

    int m_count = 1;

    Ui::Modeler *ui;
//    Scene _scene;
    MeshV mesh;
    Renderer * _renderer{nullptr};
    Boule _boule;
};

//class Modeler : public QMainWindow
//{
//    Q_OBJECT

//public:
//    explicit Modeler(QWidget *parent = nullptr);
//    ~Modeler();


#endif // MODELER_H
