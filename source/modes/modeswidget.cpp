#include "ui_modeswidget.h"
#include "modeswidget.h"

#include "generator/generatoraepwidget.h"
#include "generator/generatorpemiwidget.h"
#include "timer/timerpemiwidget.h"
#include "timer/timeraepwidget.h"
#include "search/searchfileswidget.h"
#include "systemtray.h"

#include <QKeyEvent>
#include "QDebug"

ModesWidget::ModesWidget(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::ModesWidget)
{
    ui->setupUi(this);

    createSystemTray();
    createWidgets();
    initEventFiter();
    connectSlots();


    qDebug()<<"Create ModesWidget";
}

ModesWidget::~ModesWidget()
{
    delete ui;
    qDebug()<<"~ModesWidget";
}

//-------------------------------
void ModesWidget::connectSlots() const
{
    connect(ui->m_generAEP_pb, SIGNAL (clicked(bool)),
            this, SLOT(m_generAEP_pb_clicked()));

    connect(ui->m_timerAEP_pb, SIGNAL (clicked(bool)),
            this, SLOT(m_timerAEP_pb_clicked()));

    connect(ui->m_generPEMI_pb, SIGNAL (clicked(bool)),
            this, SLOT(m_generPEMI_pb_clicked()));

    connect(ui->m_timerPEMI_pb, SIGNAL (clicked(bool)),
            this, SLOT(m_timerPEMI_pb_clicked()));

    connect(ui->m_search_pb, SIGNAL (clicked(bool)),
            this, SLOT(m_search_pb_clicked()));

    connect(m_generatorAepWidget, SIGNAL(emitBackUi()),
            this, SLOT(showModes()));

    connect(m_generatorPemiWidget, SIGNAL(emitBackUi()),
            this, SLOT(showModes()));

    connect(m_timerPemiWidget, SIGNAL(emitBackUi()),
            this, SLOT(showModes()));

    connect(m_timerAepWidget, SIGNAL(emitBackUi()),
            this, SLOT(showModes()));

    connect(m_searchFileWidget, SIGNAL(emitBackUi()),
            this, SLOT(showModes()));
}

//-------------------------------
void ModesWidget::initEventFiter()
{
    ui->m_generAEP_pb->installEventFilter(this);
    ui->m_generPEMI_pb->installEventFilter(this);
    ui->m_timerAEP_pb->installEventFilter(this);
    ui->m_timerPEMI_pb->installEventFilter(this);
    ui->m_search_pb->installEventFilter(this);
}

//-------------------------------
void ModesWidget::createSystemTray()
{
    m_sysTray = new SystemTray(this);
    m_sysTray->setPointWidget(this);
    m_sysTray->showTrayIcon();
}

//-------------------------------
void ModesWidget::createWidgets()
{
    m_timerPemiWidget = new TimerPemiWidget(m_sysTray, this);
    m_timerAepWidget = new TimerAepWidget(m_sysTray, this);
    m_generatorAepWidget = new GeneratorAepWidget(m_sysTray, this);
    m_generatorPemiWidget = new GeneratorPemiWidget(m_sysTray, this);
    m_searchFileWidget = new SearchFilesWidget(m_sysTray, this);
}

//-------------------------------
void ModesWidget::m_generAEP_pb_clicked()
{
    this->hide();
    m_generatorAepWidget->show();
}

//-------------------------------
void ModesWidget::m_timerAEP_pb_clicked()
{
    this->hide();
    m_timerAepWidget->show();
}

//-------------------------------
void ModesWidget::m_generPEMI_pb_clicked()
{
    this->hide();
    m_generatorPemiWidget->show();
}

//-------------------------------
void ModesWidget::m_timerPEMI_pb_clicked()
{
    this->hide();
    m_timerPemiWidget->show();
}

//-------------------------------
void ModesWidget::m_search_pb_clicked()
{
    this->hide();
    m_searchFileWidget->show();
}

//-------------------------------
void ModesWidget::showModes()
{
    this->show();
}

//-------------------------------
bool ModesWidget::eventFilter(QObject* inTarget,
                              QEvent* inEvent)
{
    if (inEvent->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = dynamic_cast<QKeyEvent* >(inEvent);
          if (keyEvent->key() == Qt::Key_Space) {
              m_sysTray->setPointWidget(this);
              m_sysTray->showTrayIcon();
              this->hide();
              return true;
          }
     }
return false;
}
