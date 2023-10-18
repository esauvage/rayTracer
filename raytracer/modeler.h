#ifndef MODELER_H
#define MODELER_H

#include <QMainWindow>
#include <QWidget>
#include "scene.h"

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

class Modeler : public QWidget
{
public:
    Modeler(VulkanWindow *vulkanWindow);

private:
    QLabel *createLabel(const QString &text);

    QLabel *infoLabel;
    QCheckBox *meshSwitch;
    QLCDNumber *counterLcd;
    QPushButton *newButton;
    QPushButton *quitButton;
    QPushButton *pauseButton;

    int m_count = 128;
private:
    Ui::Modeler *ui;
    Scene _scene;
};

//class Modeler : public QMainWindow
//{
//    Q_OBJECT

//public:
//    explicit Modeler(QWidget *parent = nullptr);
//    ~Modeler();


#endif // MODELER_H
