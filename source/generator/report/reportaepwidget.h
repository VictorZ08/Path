#ifndef REPORTAEPWIDGET_H
#define REPORTAEPWIDGET_H

#include <timer/timerinterface.h>

QT_BEGIN_NAMESPACE
namespace Ui {class ReportAepWidget;}
QT_END_NAMESPACE

/**
    @author Victor Zaytcev
*/
class ReportAepWidget final : public TimerInterface
{
    Q_OBJECT

public:
    explicit ReportAepWidget(SystemTray* inSysTray = nullptr,
                             QWidget* inParent  = nullptr);
    ~ReportAepWidget();
    void openInfoOut(QFileInfoList& inOutSets,
                     QFileInfoList& inSetsInTree);

protected:
    void initEventFilter();
    void outputNameFile(QFileInfo& inNameFolderOut,
                        QFileInfoList::ConstIterator it);
    void outputDataFile(QFileInfo& inNameFolderOut,
                        QFileInfoList::ConstIterator it);

    virtual void closeEvent(QCloseEvent* inEvent) override;

signals:
    void emitBackUi();

private:
    Ui::ReportAepWidget* ui = nullptr;
};

#endif // REPORTAEPWIDGET_H
