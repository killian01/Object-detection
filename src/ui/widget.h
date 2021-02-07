#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>

namespace Ui {
class widget;
}

class widget : public QWidget
{
    Q_OBJECT

private:
    Ui::widget *ui;
    QThread *thread;

    void setup();

public:
    explicit widget(QWidget *parent = nullptr);
    ~widget();

signals:
    void sendSetup(int device);
    void sendToggleStream();

private slots:
    void receiveFrame(QImage frame);
    void receiveToggleStream();

};

#endif // WIDGET_H
