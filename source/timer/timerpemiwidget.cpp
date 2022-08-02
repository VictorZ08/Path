#include "ui_timerpemiwidget.h"
#include "timerpemiwidget.h"
#include "verification/loggerwidget.h"

#include "sortfiles.h"
#include "timer.h"

#include "chronometr.h"//Удалить после измерений
#include <QDebug>

TimerPemiWidget::TimerPemiWidget(SystemTray* inSysTray,
                                 QWidget* inParent)
            : TimerInterface(inSysTray, inParent)
            , ui{new Ui::TimerPemiWidget}
            , m_logger(new LoggerWidget(inSysTray, this))
{
    ui->setupUi(this);

    setParentWiget(this);
    connectSlots();
    initEventFiter();

    ui->m_loadSets_tw->setAttribute(Qt::WA_AcceptDrops, true);
    qDebug()<<"Create TimerPemiWidget";
}

//-------------------------------
TimerPemiWidget::~TimerPemiWidget()
{
    delete ui;
    qDebug()<<"~TimerPemiWidget";
}

/**
    @brief TimerPemiWidget::connectSlots
    Установка сигнал - слотов
*/
void TimerPemiWidget::connectSlots() const
{
    connect(ui->m_clear_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_clear_pb_clicked()));

    connect(ui->m_back_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_back_pb_clicked()));

    connect(ui->m_start_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_start_pb_clicked()));

    connect(ui->m_editTime_dte, SIGNAL(dateTimeChanged(const QDateTime& )),
            this, SLOT(m_previewTime_le_changed()));

    connect(ui->m_timeSet_le, SIGNAL(textChanged(const QString& )),
            this, SLOT(m_previewTime_le_changed()));

    connect(this, SIGNAL(emitPreviewTime()),
            this, SLOT(m_previewTime_le_changed()));

    connect(ui->m_fixedTime_ckb, SIGNAL(stateChanged(int)),
            this, SLOT(m_previewTime_le_changed()));

    connect(ui->m_startCheckData_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_startCheckData_pb_clicked()));

    connect(ui->m_reportCheck_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_reportCheck_pb_clicked()));

    connect(this, SIGNAL(emitStatus_prb()), this,
            SLOT(m_progress_prb_tempStart()), Qt::DirectConnection);

    connect(m_logger, SIGNAL(emitBackUi()),
            this, SLOT(showForm()));
}

/**
    @brief TimerPemiWidget::initEventFiter
    Установка эвент фильтра, что бы при нажатии на пробел
    (приложение сворачивается в tree), фокус не смещался на виджеты
*/
void TimerPemiWidget::initEventFiter()
{
    ui->m_clear_pb->installEventFilter(this);
    ui->m_loadSets_tw->installEventFilter(this);
    ui->m_back_pb->installEventFilter(this);
    ui->m_timeSet_le->installEventFilter(this);
    ui->m_editTime_dte->installEventFilter(this);
    ui->m_previewTime_le->setEnabled(false);
    ui->m_start_pb->installEventFilter(this);
}

/**
    @brief TimerPemiWidget::m_clear_pb_clicked
    Очищает форму
*/
void TimerPemiWidget::m_clear_pb_clicked()
{
    clearWiget();

    ui->m_timeSet_le->setText(QString::number(kTimeSetPemi));
    ui->m_status_prb->setValue(0);
}

/**
    @brief TimerPemiWidget::m_previewTime_le_changed
    Выводит предварительные значения даты и времени
*/
void TimerPemiWidget::m_previewTime_le_changed()
{
    previewTime();
}

/**
    @brief TimerPemiWidget::m_startCheckData_pb_clicked
    Проверка данных в tree на ошибки
*/
void TimerPemiWidget::m_startCheckData_pb_clicked()
{
    startCheckData();
}

/**
    @brief TimerPemiWidget::m_reportCheck_pb_clicked
    Вывод отчета
*/
void TimerPemiWidget::m_reportCheck_pb_clicked()
{
    reportCheck(m_logger);
}

/**
    @brief TimerPemiWidget::m_progress_prb_tempStart
    Увеличивает состояние прогрессбар
*/
void TimerPemiWidget::m_progress_prb_tempStart()
{
    progressTempStart();
}

/**
    @brief TimerPemiWidget::showForm
    Вывод формы
*/
void TimerPemiWidget::showForm()
{
    this->show();
}

/**
    @brief TimerPemiWidget::m_back_pb_clicked
    Возвращает в начальную форму
*/
void TimerPemiWidget::m_back_pb_clicked()
{
    this->hide();
    emit emitBackUi();
}

/**
    @brief TimerPemiWidget::m_start_pb_clicked
    Запуск перетасовки файлов и присвоения им время
*/
void TimerPemiWidget::m_start_pb_clicked()
{
    Set& setsTree = getSetsInTree();
    ui->m_status_prb->setMaximum(setsTree.getSetsPemi().count());

    QVector<QDateTime>::iterator itDT = getDateTime().begin();
    shuffleFiles(setsTree.getSetsPemi());
    for(auto& set : setsTree.getSetsPemi()) {
        setDateTimeFiles(set, *itDT++);
        emit emitStatus_prb();
    }
}

/**
    @brief TimerPemiWidget::testBenchmark
    Тест на произвоительность метода
*/
void TimerPemiWidget::testBenchmark()
{

}
