#include "widget.h"
#include "ui_widget.h"
#include "src/opencvworker/opencvworker.h"
#include <QTimer>
#include <QThread>
#include <QObject>

widget::widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget)
{
    ui->setupUi(this);
    ui->labelView->setScaledContents(true);
    setup();
}

widget::~widget()
{
    thread->quit();
    while(!thread->isFinished());

    delete thread;
    delete ui;
}

void widget::setup()
{
    thread = new QThread();
    OpenCvWorker *worker = new OpenCvWorker();
    QTimer *workerTrigger = new QTimer();
    workerTrigger->setInterval(0);

    //QTimer trigger receive frame
    connect(workerTrigger, SIGNAL(timeout()), worker, SLOT(receiveGrabFrame()));
    connect(this, SIGNAL(sendSetup(int)), worker, SLOT(receiveSetup(int))); //
    connect(this, SIGNAL(sendToggleStream()), worker, SLOT(receiveToggleStream()));

    //Buttons
    connect(ui->pushButtonPlay, SIGNAL(clicked(bool)), this, SLOT(receiveToggleStream()));
    connect(ui->checkBox, SIGNAL(toggled(bool)), worker, SLOT(receiveCheckBox()));
    connect(ui->spinBoxHMIN, SIGNAL(valueChanged(int)), worker, SLOT(receiveHMIN(int)));
    connect(ui->spinBoxSMIN, SIGNAL(valueChanged(int)), worker, SLOT(receiveSMIN(int)));
    connect(ui->spinBoxVMIN, SIGNAL(valueChanged(int)), worker, SLOT(receiveVMIN(int)));
    connect(ui->spinBoxHMAX, SIGNAL(valueChanged(int)), worker, SLOT(receiveHMAX(int)));
    connect(ui->spinBoxSMAX, SIGNAL(valueChanged(int)), worker, SLOT(receiveSMAX(int)));
    connect(ui->spinBoxVMAX, SIGNAL(valueChanged(int)), worker, SLOT(receiveVMAX(int)));


    //Receive frame signal to display the image
    connect(worker, SIGNAL(sendFrame(QImage)), this, SLOT(receiveFrame(QImage)));

    workerTrigger->start();
    worker->moveToThread(thread);
    workerTrigger->moveToThread(thread);

    thread->start();

    emit sendSetup(0);
}

void widget::receiveFrame(QImage frame)
{
    ui->labelView->setPixmap(QPixmap::fromImage(frame));
}

void widget::receiveToggleStream()
{
    if(!ui->pushButtonPlay->text().compare(">")) ui->pushButtonPlay->setText("||");
    else ui->pushButtonPlay->setText(">");

    emit sendToggleStream();
}
