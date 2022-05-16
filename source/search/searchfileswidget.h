#ifndef SEARCHFILESWIDGET_H
#define SEARCHFILESWIDGET_H

#include <QDialog>
#include <QThread>

class Find;
class DataBase;
class MainWindow_Form;
class SystemTray;

namespace Ui {
class SearchFilesWidget;
}

/**
    @author Victor Zaytcev
*/
class SearchFilesWidget final : public QDialog
{
    Q_OBJECT

public:
    explicit SearchFilesWidget(SystemTray* inSysTray = nullptr,
                               QWidget* inParent = nullptr);
    ~SearchFilesWidget();

protected:
    void connectSlots() const;
    void initEventFiter();
    void statusLoadDB();

    QStringList splitString(const QString& inString);

    virtual bool eventFilter(QObject* inTarget,
                             QEvent* inEvent) override;
    virtual void closeEvent(QCloseEvent* inEvent) override;

protected slots:
    void m_search_pb_clicked();
    void m_clear_pb_clicked();
    void m_progress_prb_tempStart();
    void m_space_pb_clicked();
    void m_updateBD_pb_clicked();
    void m_back_pb_clicked();
    void m_viewerFoundFieles_tb_anClicked(const QUrl& inLink);
    void threadFindFinished();//!!!name
    void threadDataBaseFinished();//!!!name

signals:
    void emitBackUi();
    void statusProgressBar(const QString& );


private:
    QStringList searchWordInDB();
    void outUrlInBrowser(const QStringList& inStringUrl);

private:
    Ui::SearchFilesWidget* ui = nullptr;
    SystemTray* m_sysTray = nullptr;
    Find* m_Find = nullptr;
    DataBase* m_DataBase = nullptr;

    QThread         m_threadFind;
    QThread         m_threadDB;

    int m_nStep = 0;
};

#endif
