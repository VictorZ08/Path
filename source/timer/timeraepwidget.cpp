#include "ui_timeraepwidget.h"
#include "timeraepwidget.h"

#include "sortfiles.h"
#include "timer.h"

#include <QDebug>

//-------------------------------
TimerAepWidget::TimerAepWidget(SystemTray* inSysTray,
                               QWidget* inParent)
            : TimerInterface(inSysTray, inParent)
            , ui(new Ui::TimerAepWidget)
{
    ui->setupUi(this);

    setParentWiget(this);;
    connectSlots();
    initEventFiter();

    ui->m_loadSets_tw->setAttribute(Qt::WA_AcceptDrops, true);
    statusGeneratesFiles("color: rgb(0, 0, 0)",
                         "Статус:-");

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

    connect(this, SIGNAL(emitPreviewTime()),
            this, SLOT(m_previewTime_le_changed()));
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
}

/**
    @brief TimerAepWidget::m_clear_pb_clicked
    Очищает форму
*/
void TimerAepWidget::m_clear_pb_clicked()
{
    clearWiget();
    statusGeneratesFiles("color: rgb(0, 0, 0)", "Статус:-");
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
    QVector<QDateTime>::iterator itDateTime = getDateTime().begin();

    Set& sets = getSetsInTree();
    for(auto &set: sets.getSetsAep()) {
        sortFilesToComplectAep(set);
        Timer::setDateTimeFile(set.first, *itDateTime++);
        for(auto &files: set.second) {
            Timer::setDateTimeFile(files, *itDateTime++);
        }
    }
    statusGeneratesFiles("color: rgb(255, 255, 255)",
                         "Статус: Готов");
}

