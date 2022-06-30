#include "ui_reportaepwidget.h"
#include "reportaepwidget.h"
#include "modes/systemtray.h"

#include <QDesktopWidget>
#include <QKeyEvent>
#include "QDebug"

/**
    @brief ReportAepWidget::ReportAepWidget
    Конструктор
    @param inSysTray указатель на системный трай
    @param inParent указатель на родительский объект,
    требуется для удаления
*/
ReportAepWidget::ReportAepWidget(SystemTray* inSysTray,
                                 QWidget* inParent)
                        : TimerInterface(inSysTray, inParent)
                        , ui(new Ui::ReportAepWidget)
{
    ui->setupUi(this);

    initEventFilter();

    QDesktopWidget desk;
    QRect QScreen = desk.availableGeometry();
    setFixedSize(750, QScreen.height()-70);
    ui->m_outTextInfo_te->setFixedSize(720,QScreen.height()-130);
    ui->m_outTextInfo_te->setReadOnly(true);
    ui->m_outTextInfo_te->installEventFilter(this);
    qDebug()<<"Create Report";
}

/**
    @brief ReportAepWidget::~ReportAepWidget()
    Деструктор
*/
ReportAepWidget::~ReportAepWidget()
{
    delete ui;
    qDebug()<<"~Report";
}

/**
    @brief ReportAepWidget::initEventFiter
    Установка эвент фильтра, что бы при нажатии на пробел
    (приложение сворачивается в tree), фокус не смещался на виджеты
*/
void ReportAepWidget::initEventFilter()
{
    ui->m_outTextInfo_te->installEventFilter(this);
}

/**
    @brief ReportAepWidget::openInfoOut()
    Открывает файлы (исходные и сгенерированные)
    @param inOutSets файлы сгенерированные
    @param inInSets файлы загруженные в трей
*/
void ReportAepWidget::openInfoOut(QFileInfoList& inOutSets,
                                          QFileInfoList& inSetsInTree)
{
    ui->m_outTextInfo_te->clear();
    QFileInfoList::ConstIterator it = inSetsInTree.begin();
    for(auto& file: inOutSets) {
        if(file.isDir())
            outputNameFile(file, it);
        else if(file.isFile()) {
            outputNameFile(file, it);
            outputDataFile(file, it);
        }
        ++it;
        if(it == inSetsInTree.end()) {
            it = inSetsInTree.begin();
        }
    }
}

/**
    @brief ReportAepWidget::outputNameFile()
    Выводит и подкрашивает имя файла или каталога
    @param inNameFolderOut файл или каталог
    @param it файл или каталог из трея
*/
void ReportAepWidget::outputNameFile(QFileInfo& inNameFolderOut,
                                     QFileInfoList::ConstIterator it)
{
    ui->m_outTextInfo_te->setTextColor(QColor(255,255,255));
    ui->m_outTextInfo_te->insertPlainText("\t======"+inNameFolderOut.baseName()+"=====");
    ui->m_outTextInfo_te->insertPlainText("\t\t\t======"+it->baseName()+"=====\n");
}

/**
    @brief ReportAepWidget::outputDataFile()
    Открывает и читает файл и выводит данные в виджет
    @param inNameFolderOut файл или каталог
    @param it файл или каталог из трея
*/
void ReportAepWidget::outputDataFile(QFileInfo& inNameFolderOut,
                                     QFileInfoList::ConstIterator it)
{
    QTextCursor textCursot = ui->m_outTextInfo_te->textCursor();
    ui->m_outTextInfo_te->setTextColor(QColor(0,0,0));
    QFile fileOut(inNameFolderOut.absoluteFilePath());
    QFile fileIn(it->absoluteFilePath());
    if (!fileOut.open(QFile::ReadOnly | QFile::Text)) {
        qDebug()<<"Не удаётся открыть созданный файл!";
    }
    if (!fileIn.open(QFile::ReadOnly | QFile::Text)) {
        qDebug()<<"Не удаётся открыть загруженный файл!";
    }
    bool flag = false;
    while (!fileOut.atEnd()) {
       ui->m_outTextInfo_te->insertPlainText(fileOut.readLine());
       textCursot.movePosition(QTextCursor::Up, QTextCursor::KeepAnchor);
       textCursot.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
       ui->m_outTextInfo_te->setTextCursor(textCursot);
       if(flag == true )
          ui->m_outTextInfo_te->insertPlainText("\t|\t" + fileIn.readLine());
       else {
           ui->m_outTextInfo_te->insertPlainText("\t\t\t\t|\t" + fileIn.readLine());
           flag = true;
       }
    }
    fileOut.close();
    fileIn.close();
    ui->m_outTextInfo_te->append("");
}


/**
    @brief ReportAepWidget::closeEvent
    Закрывает приложение
    @param inEvent Не используется
*/
void ReportAepWidget::closeEvent(QCloseEvent* event)
{
    this->hide();
    emit emitBackUi();
}
