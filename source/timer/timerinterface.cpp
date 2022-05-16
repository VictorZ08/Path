#include "timerinterface.h"
#include "treeitem.h"
#include "sortfiles.h"
#include "random/random.h"
#include "timer.h"
#include "modes/systemtray.h"

#include <QMimeData>
#include <QDragEnterEvent>
#include <QCoreApplication>
#include <QTreeWidget>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QLabel>
#include <QDir>

#include <QDebug>

constexpr quint64 kTimeSet = 50;

TimerInterface::TimerInterface(SystemTray* inSysTray,
                               QWidget* inParent)
        : QDialog(inParent)
        , m_sysTray{inSysTray}
        , m_beginWorkTime{Timer::getWorkDateTime()}
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
    quint64 timeSet = m_timeSet_le->text().toInt();
    m_beginWorkTime = m_editTime_dte->dateTime();
    m_dateTime.clear();

    QString strObjectName = this->objectName();
    if(strObjectName.contains("pemi", Qt::CaseInsensitive)){
        quint64 numSets;
        if(m_numSets_le != nullptr)
            numSets = m_numSets_le->text().toInt();
        else
            numSets = m_loadSetsTree.getSetsAep().count();

        m_dateTime = Random::randTimePemi(m_beginWorkTime,
                                          numSets,
                                          timeSet);
    }
    else if(strObjectName.contains("aep", Qt::CaseInsensitive)){
        if(m_loadSetsTree.getSetsAep().count() != 0) {

            quint64 numFilesInSet = m_loadSetsTree.getSetsAep().at(0).second.count();
            quint64 numSets;
            if(m_numSets_le != nullptr)
                numSets = m_numSets_le->text().toInt();
            else
                numSets = m_loadSetsTree.getSetsAep().count();

            m_dateTime = Random::randTimeAep(m_beginWorkTime,
                                             numFilesInSet,
                                             numSets,
                                             timeSet);
        }
    }
    if(!m_dateTime.isEmpty()) {
        m_previewTime_le->setText((m_dateTime.back()).toString("dd.MM.yyyy hh:mm"));
    }
    statusGeneratesFiles("color: rgb(0, 0, 0)", "Статус:-");
}

/**
    @brief TimerInterface::setParentWiget
    Устанавливает укакзатели на виджеты
*/
void TimerInterface::setParentWiget(const QObject* inObj)
{
    m_loadSets_tw = inObj->findChild<QTreeWidget *>("m_loadSets_tw");
    m_timeSet_le = inObj->findChild<QLineEdit *>("m_timeSet_le");
    m_previewTime_le = inObj->findChild<QLineEdit *>("m_previewTime_le");
    m_numSets_le = inObj->findChild<QLineEdit *>("m_numSets_le");
    m_saveSets_le = inObj->findChild<QLineEdit *>("m_saveSets_le");
    m_editTime_dte = inObj->findChild<QDateTimeEdit *>("m_editTime_dte");
    m_status_l = inObj->findChild<QLabel *>("m_status_l");

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
    m_timeSet_le->setText(QString::number(kTimeSet));
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
    m_timeSet_le->setText(QString::number(kTimeSet));
}

/**
    @brief TimerInterface::statusGeneratesFiles
    Выводит строку стату начала или
    завершения работы генерации файлов
    @param inColor Цвет текста
    @param inStatus Текст
*/
void TimerInterface::statusGeneratesFiles(const QString& inColor,
                                          const QString& inStatus)
{
    m_status_l->setStyleSheet(inColor);
    m_status_l->setText(inStatus);
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
    @brief TimerInterface::closeEvent
    Закрывает приложение
    @param inEvent Не используется
*/
void TimerInterface::closeEvent(QCloseEvent* inEvent)
{
    Q_UNUSED(inEvent);
    QCoreApplication::quit();
}
