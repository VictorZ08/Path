#include "ui_searchfileswidget.h"
#include "searchfileswidget.h"
#include "find.h"
#include "database.h"
#include "modes/systemtray.h"

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QDesktopServices>
#include <QKeyEvent>
#include <QFileInfo>
#include <QDateTime>
#include <QDebug>

bool cmpDateTime(const QString& s1, const QString& s2)
{
    QFileInfo src, dst;
    src = s1; dst = s2;
    return src.lastModified() > dst.lastModified();
}

/**
    @brief SearchFilesWidget::SearchFilesWidget
    Конструктор
    @param inSysTray указатель на системный трай
    @param inParent указатель на родительский объект,
    требуется для удаления
*/
SearchFilesWidget::SearchFilesWidget(SystemTray* inSysTray,
                                     QWidget* inParent)
            : QDialog(inParent)
            , ui(new Ui::SearchFilesWidget)
            , m_sysTray{inSysTray}
            , m_Find(new Find(inParent))
            , m_DataBase(new DataBase(inParent))
{
    ui->setupUi(this);

    ui->m_viewerFoundFieles_tb->setOpenLinks(false);
    ui->m_viewerFoundFieles_tb->setOpenExternalLinks(false);

    ui->m_progress_prb->setValue(m_nStep);

    connectSlots();
    initEventFiter();

    m_Find->moveToThread(&m_threadFind);
    m_DataBase->createConnectDB();
    statusLoadDB();
    m_DataBase->disconnectDB();
    m_DataBase->moveToThread(&m_threadDB);
    qDebug()<<"Create SearchFilesWidget";
}

/**
    @brief SearchFilesWidget::~SearchFilesWidget()
    Деструктор
*/
SearchFilesWidget::~SearchFilesWidget()
{
    delete ui;
    qDebug()<<"~SearchFilesWidget";
}

/**
    @brief SearchFilesWidget::connectSlots
    Установка сигнал - слотов
*/
void SearchFilesWidget::connectSlots() const
{
    connect(ui->m_search_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_search_pb_clicked()));

    connect(ui->m_clear_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_clear_pb_clicked()));

    connect(ui->m_updateBD_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_updateBD_pb_clicked()));

    connect(ui->m_space_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_space_pb_clicked()));

    connect(ui->m_viewerFoundFieles_tb, SIGNAL(anchorClicked(QUrl)),
            this, SLOT(m_viewerFoundFieles_tb_anClicked(QUrl)));

    connect(ui->m_back_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_back_pb_clicked()));

    connect(this, SIGNAL(statusProgressBar(const QString &)),
            m_sysTray, SLOT(slotStatusProgressBar(const QString &)), Qt::DirectConnection);

    connect(m_Find, SIGNAL(threadFinished()), this,
            SLOT(threadFindFinished()), Qt::DirectConnection);

    connect(m_DataBase, SIGNAL(threadFinished()), this,
            SLOT(threadDataBaseFinished()), Qt::DirectConnection);

    connect(m_Find, SIGNAL(tempStart()), this,
            SLOT(m_progress_prb_tempStart()), Qt::DirectConnection);

    connect(m_DataBase, SIGNAL(tempStart()), this,
            SLOT(m_progress_prb_tempStart()), Qt::DirectConnection);

    connect(&m_threadFind, SIGNAL(started()), m_Find,
            SLOT(slotThreadFind()));

    connect(&m_threadDB, SIGNAL(started()), m_DataBase,
            SLOT(slotThreadDB()));
}

/**
    @brief SearchFilesWidget::initEventFiter
    Установка эвент фильтра, что бы при нажатии на пробел
    (приложение сворачивается в tree), фокус не смещался на виджеты
*/
void SearchFilesWidget::initEventFiter()
{
    ui->m_search_pb->installEventFilter(this);
    ui->m_clear_pb->installEventFilter(this);
    ui->m_updateBD_pb->installEventFilter(this);
    ui->m_back_pb->installEventFilter(this);
    ui->m_searchWord_le->installEventFilter(this);
    ui->m_viewerFoundFieles_tb->installEventFilter(this);
    ui->m_loadDirectoriesDB_te->installEventFilter(this);
}

/**
    @brief SearchFilesWidget::statusLoadDB
    Статус загрузки базы данных
*/
void SearchFilesWidget::statusLoadDB()
{
    QString countValues = QString("В базе: %1 файлов")
            .arg(m_DataBase->countFilesDB());
    ui->label->setText(countValues);

    QString updateCountLoad = QString("Добавлено в базу: %1 файлов")
            .arg(m_Find->getHashExelFiles().count());
    ui->addLabel->setText(updateCountLoad);

    QString updateDate = QString("Последнее обновление БД: %1 ")
            .arg(m_DataBase->getDateUpdateServerDB());
    ui->labelDateTime->setText(updateDate);
}

/**
    @brief SearchFilesWidget::m_updateBD_pb_clicked
    Обновление базы данных
*/
void SearchFilesWidget::m_updateBD_pb_clicked()
{
    ui->m_viewerFoundFieles_tb->clear();
    ui->m_progress_prb->setValue(0);
    QString inputStringUrl = ui->m_loadDirectoriesDB_te->toPlainText();
    QStringList urlFoldersOrder = splitString(inputStringUrl);
    ui->m_progress_prb->setMaximum(urlFoldersOrder.count());
    m_Find->setUrlFolders(urlFoldersOrder);
    m_threadFind.start();
    qDebug()<<"start threadFind!";
}

/**
    @brief SearchFilesWidget::splitString
    Расщепляем строку
    @param inString входная строка
*/
QStringList SearchFilesWidget::splitString(const QString& inString)
{
    QStringList strIn, strOut;
    strIn = inString.split('\n');
    strOut.reserve(strIn.count());
    for(auto& i: strIn)
        strOut.append(QUrl(i).toLocalFile());
    return strOut;
}

/**
    @brief SearchFilesWidget::m_progress_prb_tempStart
    Выводит прогресс поиска или загрузки в БД
*/
void SearchFilesWidget::m_progress_prb_tempStart()
{
    ++m_nStep;
    ui->m_progress_prb->setValue(m_nStep);
    if(m_sysTray != nullptr)
        emit statusProgressBar(ui->m_progress_prb->text());

    qDebug()<< ui->m_progress_prb->text();
}

/**
    @brief SearchFilesWidget::m_space_pb_clicked
    Виртуальная кнопка пробела, т.к. пробел занят на
    сворачивание в трай
*/
void SearchFilesWidget::m_space_pb_clicked()
{
    ui->m_searchWord_le->insert(" ");
}

/**
    @brief SearchFilesWidget::m_search_pb_clicked
    Поиск и сортировка по дате, выводит в textbrowser
*/
void SearchFilesWidget::m_search_pb_clicked()
{
    if(ui->m_searchWord_le->text().isEmpty() == false) {
        ui->m_viewerFoundFieles_tb->clear();
        QStringList listUrl = searchWordInDB();
        qSort(listUrl.begin(), listUrl.end(),cmpDateTime);
        outUrlInBrowser(listUrl);
    }
    return;
}

/**
    @brief SearchFilesWidget::searchWordInDB
    Поиск по слову в БД
*/
QStringList SearchFilesWidget::searchWordInDB()
{
    m_DataBase->createConnectDB();
    QString str = "SELECT pathFiles FROM tableExelFies WHERE oneStringInFile LIKE '%%1%' ";
    QString str1 = str.arg(ui->m_searchWord_le->text().toLower());

    QSqlQuery query;
    if(!query.exec(str1)) {
        qDebug() << "Unable to execute query";
        //return;
    }
    QStringList stringUrl;
    int fieldNo = query.record().indexOf("pathFiles");
    while (query.next())
        stringUrl.append(query.value(fieldNo).toString());

    return stringUrl;
}

/**
    @brief SearchFilesWidget::outUrlInBrowser
    Вывод списка в браузер
    @param inStringUrl список
*/
void SearchFilesWidget::outUrlInBrowser(const QStringList& inStringUrl)
{
    foreach(auto url, inStringUrl) {
        QString hyperLink = QString::fromStdString(url.toStdString());
        hyperLink = QString::fromStdString("<a href = \"") + hyperLink +
                            QString::fromStdString("/\" >") + hyperLink +
                            QString::fromStdString("<//a>");

        QFileInfo file = url;
        ui->m_viewerFoundFieles_tb->append(file.lastModified().date().toString("dd.MM.yyyy"));
        ui->m_viewerFoundFieles_tb->append(hyperLink);
    }
    m_DataBase->disconnectDB();
}

/**
    @brief SearchFilesWidget::threadFindFinished
    Завершает поток поиска
*/
void SearchFilesWidget::threadFindFinished()
{
    m_threadFind.quit();
    m_threadFind.wait();
    qDebug()<<"quit threadFind!";

    ui->m_progress_prb->setValue(0);
    ui->m_progress_prb->setMaximum(m_Find->getHashExelFiles().count());
    m_DataBase->setHash(m_Find->getHashExelFiles());
    m_threadDB.start();
    qDebug()<<"start threadDB!";
}

/**
    @brief SearchFilesWidget::threadDataBaseFinished
    Завершает поток БД
*/
void SearchFilesWidget::threadDataBaseFinished()
{
    m_threadDB.quit();
    m_threadDB.wait();
    statusLoadDB();
    m_DataBase->disconnectDB();
    qDebug()<<"quit threadDB!";
}

/**
    @brief SearchFilesWidget::m_clear_pb_clicked
    Очищает контейнеры, устанавливаем прогресс
*/
void SearchFilesWidget::m_clear_pb_clicked()
{
    m_nStep = 0;
    ui->m_loadDirectoriesDB_te->clear();
    ui->m_viewerFoundFieles_tb->clear();
    ui->m_progress_prb->setValue(0);
}

//-------------------------------
bool SearchFilesWidget::eventFilter(QObject* inTarget,
                                   QEvent* inEvent)
{
    if (inEvent->type() == QEvent::KeyPress) {
          QKeyEvent *keyEvent = dynamic_cast<QKeyEvent*>(inEvent);
          if (keyEvent->key() == Qt::Key_Space) {
              this->hide();
              m_sysTray->setPointWidget(this);
              m_sysTray->showTrayIcon();
              return true;
          }
    }
return false;
}

//-------------------------------
void SearchFilesWidget::closeEvent(QCloseEvent* event)
{
    //Q_UNUSED(event)
    QCoreApplication::quit();
}

//--------------------------------------------
void SearchFilesWidget::m_back_pb_clicked()
{
    this->hide();
    emit emitBackUi();
}

/**
    @brief SearchFilesWidget::m_viewerFoundFieles_tb_anClicked
    Просмотр найденных файлов
    @param inLink файлы
*/
void SearchFilesWidget::m_viewerFoundFieles_tb_anClicked(const QUrl& inLink)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(inLink.toString()));
}
