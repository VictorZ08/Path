#include "timerinterface.h"
#include "treeitem.h"
#include "sortfiles.h"
#include "random/random.h"
#include "timer.h"
#include "modes/systemtray.h"
#include "verification/loggerwidget.h"
#include "verification/dataverification.h"

#include <QMimeData>
#include <QDragEnterEvent>
#include <QCoreApplication>
#include <QTreeWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QDateTimeEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>
#include <QDir>

#include <QDebug>

TimerInterface::TimerInterface(SystemTray* inSysTray,
                               QWidget* inParent)
        : QDialog(inParent)
        , m_sysTray{inSysTray}
        , m_beginWorkTime{getWorkDateTime()}
{
    qDebug()<<"Create TimerInterface";
}

TimerInterface::~TimerInterface()
{
    qDebug()<<"~TimerInterface";
}

/**
    @brief TimerInterface::eventFilter
    При нажатии на пробел, скрывает программу в tree, а так же
    при перетаскивании файлов в tw проверяет их на ошибки
    @param inTarget Объект от наведения курсора
    @param inEvent Сравнивает тип нажатой клавиши
*/
bool TimerInterface::eventFilter(QObject* inTarget,
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
    if(inEvent->type() == QEvent::DragEnter) {
        if(inTarget->objectName() == m_loadSets_tw->objectName()) {
            dragEventTreeWidget(inEvent);
            return true;
        }
    }
    else if(inEvent->type() == QEvent::Drop &&
            m_checkStatusLoadTree == false)
    {
        dropEventTreeWidget();
        emit emitPreviewTime();
        return true;
    }
return false;
}

/**
    @brief TimerInterface::dragEventTreeWidget
    Проверяет сбрасываемые файлы в tw, если формат файлов
    не совпадает, выходим
    @param inEvent событие при сбросе в tw
*/
void TimerInterface::dragEventTreeWidget(QEvent* inEvent)
{
    QDragEnterEvent* dragEvent = dynamic_cast<QDragEnterEvent* >(inEvent);
    QList<QUrl> urlList = dragEvent->mimeData()->urls();
    m_tempBufferLoadData.clear();

    QString strObjectName = this->objectName();
    if(strObjectName.contains("pemi", Qt::CaseInsensitive)){
        m_checkStatusLoadTree = m_tempBufferLoadData.addSetsPemi(urlList);
    }
    else if(strObjectName.contains("aep", Qt::CaseInsensitive)){
        m_checkStatusLoadTree = m_tempBufferLoadData.addSetsAep(urlList);
    }
    if(m_checkStatusLoadTree == true) {
        return;
    }
    dragEvent->setDropAction(Qt::LinkAction);
    dragEvent->acceptProposedAction();
}

/**
    @brief TimerInterface::dragEnterEvent
    Разрешпет сбрасывать файлы
    @param inEvent событие при сбросе в qlineedit
*/
void TimerInterface::dragEnterEvent(QDragEnterEvent* inEvent)
{
     inEvent->setDropAction(Qt::LinkAction);
     inEvent->acceptProposedAction();
}

/**
    @brief TimerInterface::dropEvent
    Сбрасывает url в qlineedit
    @param inEvent событие при сбросе в qlineedit
*/
void TimerInterface::dropEvent(QDropEvent* inEvent)
{
    QList<QUrl> urls = inEvent->mimeData()->urls();
    for(auto& url: urls)
        m_saveSets_le->setText(url.toLocalFile());
}

/**
    @brief TimerInterface::dropEventTreeWidget
    Отображает в tw сброшенные данные, а также
    проверяет уже их на наличие ошибок.
*/
void TimerInterface::dropEventTreeWidget()
{
    m_loadSets_tw->clear();
    QString strObjectName = this->objectName();
    if(strObjectName.contains("pemi", Qt::CaseInsensitive)){
        m_loadSetsTree.addSetsPemi(m_tempBufferLoadData);
        TreeItem::drawingFilesPemiInTree(m_loadSetsTree, m_loadSets_tw);
    }
    else if(strObjectName.contains("aep", Qt::CaseInsensitive)){
        m_loadSetsTree.addSetsAep(m_tempBufferLoadData);
        TreeItem::drawingFilesAepInTree(m_loadSetsTree, m_loadSets_tw);
    }
}

/**
    @brief TimerInterface::previewTime
    Выводит предварительные значения даты и времени
*/
void TimerInterface::previewTime()
{
    int timeSet = m_timeSet_le->text().toInt();
    m_beginWorkTime = m_editTime_dte->dateTime();
    int minTimeModes = m_minTimeModes_le->text().toInt();
    int maxTimeModes = m_maxTimeModes_le->text().toInt();
    bool checkFixedTime = m_fixedTime_ckb->isChecked();
    m_dateTime.clear();

    QString strObjectName = this->objectName();
    if(strObjectName.contains("pemi", Qt::CaseInsensitive)){
        quint64 numSets;
        if(m_numSets_le != nullptr)
            numSets = m_numSets_le->text().toInt();
        else
            numSets = m_loadSetsTree.getSetsPemi().count();

        m_dateTime = Random::randTimePemi(m_beginWorkTime,
                                          numSets,
                                          timeSet);
    }
    else if(strObjectName.contains("aep", Qt::CaseInsensitive)){
        if(m_loadSetsTree.getSetsAep().count() != 0) {

            int numFilesInSet = m_loadSetsTree.getSetsAep().at(0).second.count();
            int numSets;
            if(m_numSets_le != nullptr)
                numSets = m_numSets_le->text().toInt();
            else
                numSets = m_loadSetsTree.getSetsAep().count();

            if(checkFixedTime == true) {
                size_t sumFiles = m_loadSetsTree.countFilesAllSetsAep();
                qDebug()<<sumFiles;
                m_dateTime = Random::randTimeFixed(m_beginWorkTime,
                                                   sumFiles);
                qDebug()<<m_dateTime.back();
            }
            else {
                m_dateTime = Random::randTimeAep(m_beginWorkTime,
                                                 numFilesInSet,
                                                 numSets,
                                                 minTimeModes,
                                                 maxTimeModes,
                                                 timeSet);
            }
        }
    }

    if(!m_dateTime.isEmpty()) {
        m_previewTime_le->setText((m_dateTime.back()).toString("dd.MM.yyyy hh:mm"));
    }
}

/**
    @brief TimerInterface::setParentWiget
    Устанавливает укакзатели на виджеты
*/
void TimerInterface::setParentWiget(const QObject* inObj)
{
    m_loadSets_tw = inObj->findChild<QTreeWidget *>("m_loadSets_tw");
    m_timeSet_le = inObj->findChild<QLineEdit *>("m_timeSet_le");
    m_minTimeModes_le = inObj->findChild<QLineEdit *>("m_minTimeModes_le");
    m_maxTimeModes_le = inObj->findChild<QLineEdit *>("m_maxTimeModes_le");
    m_previewTime_le = inObj->findChild<QLineEdit *>("m_previewTime_le");
    m_numSets_le = inObj->findChild<QLineEdit *>("m_numSets_le");
    m_saveSets_le = inObj->findChild<QLineEdit *>("m_saveSets_le");
    m_editTime_dte = inObj->findChild<QDateTimeEdit *>("m_editTime_dte");
    m_fixedTime_ckb = inObj->findChild<QCheckBox *>("m_fixedTime_ckb");    
    m_startCheckData_pb = inObj->findChild<QPushButton *>("m_startCheckData_pb");
    m_reportCheck_pb = inObj->findChild<QPushButton *>("m_reportCheck_pb");
    m_status_prb = inObj->findChild<QProgressBar *>("m_status_prb");

    initCurrentTimeWidget();
}

/**
    @brief TimerInterface::initCurrentTimeWidget
    Инициализация начала работы и время на комплект
*/
void TimerInterface::initCurrentTimeWidget()
{
    m_previewTime_le->setText("");
    m_editTime_dte->setDateTime(m_beginWorkTime);
}

/**
    @brief TimerInterface::clearWiget
    Очищает переменные и виджеты
*/
void TimerInterface::clearWiget()
{
    m_loadSets_tw->clear();
    m_dateTime.clear();
    m_loadSetsTree.clear();
    m_checkStatusLoadTree = true;
}

/**
    @brief TimerInterface::countSetsInTree
    Выводит количество комплектов в tw
*/
size_t TimerInterface::getCountSetsInTree()
{
    QString strObjectName = this->objectName();
    if(strObjectName.contains("pemi", Qt::CaseInsensitive)){
        return m_loadSetsTree.countSetsPemi();
    }
    else
        return m_loadSetsTree.countSetsAep();
}

/**
    @brief TimerInterface::progressTempStart
    Увеличивает состояние прогрессбар
*/
void TimerInterface::progressTempStart()
{
    ++m_step;
    m_status_prb->setValue(m_step);
}

/**
    @brief TimerInterface::reportCheck
    Вывод отчета
    @param logger получаем указатель на виджет форму
*/
void TimerInterface::reportCheck(LoggerWidget* logger)
{
    this->hide();
    logger->setError(m_reportError);
    logger->show();
}

/**
    @brief TimerInterface::startCheckData
    Проверка данных в tree на ошибки
*/
void TimerInterface::startCheckData()
{
    if(getStatusLoadTree() == true)
        return;

    m_step = 0;
    DataVerificationAep dv;
    Set& setsTree = getSetsInTree();
    m_status_prb->setMaximum(setsTree.getSetsAep().count());
    for(auto& set: setsTree.getSetsAep()) {
        dv.checkFiles(set.second);
        emit emitStatus_prb();
    }

    m_reportError = dv.getData();
    if(m_reportError.isEmpty())
        m_reportCheck_pb->setStyleSheet("background-color: green;");
    else
        m_reportCheck_pb->setStyleSheet("background-color: red;");
}

/**
    @brief TimerInterface::closeEvent
    Закрывает приложение
    @param inEvent Не используется
*/
void TimerInterface::closeEvent(QCloseEvent* inEvent)
{
    Q_UNUSED(inEvent);
    QCoreApplication::quit();
}
