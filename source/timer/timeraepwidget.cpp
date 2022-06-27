#include "ui_timeraepwidget.h"
#include "timeraepwidget.h"
#include "verification/loggerwidget.h"
#include "verification/dataverification.h"

#include "sortfiles.h"
#include "timer.h"

#include <QDebug>

static double m_step = 0.0;

//-------------------------------
TimerAepWidget::TimerAepWidget(SystemTray* inSysTray,
                               QWidget* inParent)
            : TimerInterface(inSysTray, inParent)
            , ui(new Ui::TimerAepWidget)
            , m_logger(new LoggerWidget(inSysTray, this))
{
    ui->setupUi(this);

    setParentWiget(this);;
    connectSlots();
    initEventFiter();

    ui->m_loadSets_tw->setAttribute(Qt::WA_AcceptDrops, true);
    ui->m_timeSet_le->setText(QString::number(kTimeSetAep));
    ui->m_minTimeModes_le->setText(QString::number(kMinTimeModeAep));
    ui->m_maxTimeModes_le->setText(QString::number(kMaxTimeModeAep));

    ui->m_status_prb->setValue(0);

    qDebug()<<"Create TimerAepWidget";
}

//-------------------------------
TimerAepWidget::~TimerAepWidget()
{
    delete ui;
    qDebug()<<"~TimerAepWidget";
}

/**
    @brief TimerAepWidget::connectSlots
    Установка сигнал - слотов
*/
void TimerAepWidget::connectSlots() const
{
    connect(ui->m_clear_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_clear_pb_clicked()));

    connect(ui->m_back_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_back_pb_clicked()));

    connect(ui->m_start_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_start_pb_clicked()));

    connect(ui->m_editTime_dte, SIGNAL(dateTimeChanged(const QDateTime &)),
            this, SLOT(m_previewTime_le_changed()));

    connect(ui->m_timeSet_le, SIGNAL(textChanged(const QString &)),
            this, SLOT(m_previewTime_le_changed()));

    connect(ui->m_minTimeModes_le, SIGNAL(textChanged(const QString &)),
            this, SLOT(m_previewTime_le_changed()));

    connect(ui->m_maxTimeModes_le, SIGNAL(textChanged(const QString &)),
            this, SLOT(m_previewTime_le_changed()));

    connect(ui->m_fixedTime_ckb, SIGNAL(stateChanged(int)),
            this, SLOT(m_previewTime_le_changed()));

    connect(ui->m_report_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_report_pb_clicked()));

    connect(ui->m_startCheckData_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_startCheckData_pb_clicked()));

    connect(this, SIGNAL(emitPreviewTime()),
            this, SLOT(m_previewTime_le_changed()));

    connect(this, SIGNAL(emitStatus_prb()), this,
            SLOT(m_progress_prb_tempStart()), Qt::DirectConnection);

    connect(m_logger, SIGNAL(emitBackUi()),
            this, SLOT(showForm()));
}

/**
    @brief TimerAepWidget::initEventFiter
    Установка эвент фильтра, что бы при нажатии на пробел
    (приложение сворачивается в tree), фокус не смещался на виджеты
*/
void TimerAepWidget::initEventFiter()
{
    ui->m_clear_pb->installEventFilter(this);
    ui->m_loadSets_tw->installEventFilter(this);
    ui->m_back_pb->installEventFilter(this);
    ui->m_timeSet_le->installEventFilter(this);
    ui->m_editTime_dte->installEventFilter(this);
    ui->m_previewTime_le->setEnabled(false);
    ui->m_start_pb->installEventFilter(this);
    ui->m_startCheckData_pb->installEventFilter(this);
    ui->m_report_pb->installEventFilter(this);
    ui->m_minTimeModes_le->installEventFilter(this);
    ui->m_maxTimeModes_le->installEventFilter(this);
    ui->m_fixedTime_ckb->installEventFilter(this);
}

/**
    @brief TimerAepWidget::m_clear_pb_clicked
    Очищает форму
*/
void TimerAepWidget::m_clear_pb_clicked()
{
    clearWiget();
    ui->m_timeSet_le->setText(QString::number(kTimeSetAep));
    ui->m_minTimeModes_le->setText(QString::number(kMinTimeModeAep));
    ui->m_maxTimeModes_le->setText(QString::number(kMaxTimeModeAep));
    ui->m_status_prb->setValue(0);
}

/**
    @brief TimerAepWidget::m_back_pb_clicked
    Возвращает в начальную форму
*/
void TimerAepWidget::m_back_pb_clicked()
{
    this->hide();
    emit emitBackUi();
}

/**
    @brief TimerAepWidget::m_previewTime_le_changed
    Выводит предварительные значения даты и времени
*/
void TimerAepWidget::m_previewTime_le_changed()
{
    previewTime();
}

/**
    @brief TimerAepWidget::m_start_pb_clicked
    Запуск перетасовки файлов и присвоения им время
*/
void TimerAepWidget::m_start_pb_clicked()
{
    m_step = 0;
    QVector<QDateTime>::iterator itDateTime = getDateTime().begin();

    Set& setsTree = getSetsInTree();
    ui->m_status_prb->setMaximum(setsTree.getSetsAep().count());

    shuffleFiles(setsTree.getSetsAep());
    sortFilesToComplectAep(setsTree);
    for(auto& set: setsTree.getSetsAep()) {
        emit emitStatus_prb();
        setDateTimeFiles(set.first, *itDateTime++);
        for(auto& files: set.second) {
            setDateTimeFiles(files, *itDateTime++);
        }
    }
    statusGeneratesFiles("color: rgb(255, 255, 255)",
                         "Статус: Готов");
}

/**
    @brief TimerAepWidget::m_progress_prb_tempStart
    Увеличивает состояние прогрессбар
*/
void TimerAepWidget::m_progress_prb_tempStart()
{
    ++m_step;
    ui->m_status_prb->setValue(m_step);
}

void TimerAepWidget::m_report_pb_clicked()
{
    this->hide();
    m_logger->show();
}

void TimerAepWidget::m_startCheckData_pb_clicked()
{
    QStringList reportError;
    DataVerificationAep dv;
    Set& setsTree = getSetsInTree();
    ui->m_status_prb->setMaximum(setsTree.getSetsAep().count());
    for(auto& set: setsTree.getSetsAep()) {
        dv.pushData(set.second);
        reportError.append(dv.checkFiles());
        emit emitStatus_prb();
    }
}

/**
    @brief TimerAepWidget::showForm
    Вывод формы
*/
void TimerAepWidget::showForm()
{
    this->show();
}
