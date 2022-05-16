#ifndef TIMERINTERFACE_H
#define TIMERINTERFACE_H

#include <set.h>

#include <QDialog>
#include <QDateTime>

class QTreeWidget;
class QDateTimeEdit;
class QLineEdit;
class QLabel;
class SystemTray;
class Set;

/**
    @author Victor Zaytcev
*/
class TimerInterface : public QDialog
{
    Q_OBJECT
public:
    explicit TimerInterface(SystemTray* inSysTray,
                            QWidget* inParent = nullptr);
    virtual ~TimerInterface();

protected:
    void previewTime();
    void clearWiget();
    void initCurrentTimeWidget();
    void setParentWiget(const QObject* inObj);
    void statusGeneratesFiles(const QString& inColor,
                              const QString& inStatus);
    void dragEventTreeWidget(QEvent* inEvent);
    void dropEventTreeWidget();

    size_t getCountSetsInTree();
    QVector<QDateTime>& getDateTime() { return m_dateTime;};
    Set& getSetsInTree() { return m_loadSetsTree;}

    virtual void dragEnterEvent(QDragEnterEvent* inEvent) override;
    virtual void dropEvent(QDropEvent* inEvent) override;
    virtual void closeEvent(QCloseEvent* inEvent) override;
    virtual bool eventFilter(QObject* inTarget,
                             QEvent* inEvent) override;

signals:
    void emitPreviewTime();

private:
    QTreeWidget* m_loadSets_tw    = nullptr;
    QDateTimeEdit* m_editTime_dte = nullptr;
    QLineEdit* m_timeSet_le       = nullptr;
    QLineEdit *m_previewTime_le   = nullptr;
    QLineEdit* m_numSets_le       = nullptr;
    QLineEdit* m_saveSets_le      = nullptr;
    QLabel* m_status_l            = nullptr;
    SystemTray* m_sysTray         = nullptr;

    QVector<QDateTime>  m_dateTime;
    QDateTime           m_beginWorkTime;
    Set                 m_loadSetsTree;
    Set                 m_tempBufferLoadData;//?????

    bool m_checkStatusLoadTree = true;
};

#endif // TIMERINTERFACE_H
