#ifndef TIMERINTERFACE_H
#define TIMERINTERFACE_H

#include <set.h>

#include <QDialog>
#include <QDateTime>

struct sCoordSerchTable;

class QTreeWidget;
class QDateTimeEdit;
class QProgressBar;
class QLineEdit;
class QLabel;
class QCheckBox;
class SystemTray;
class Set;
class LoggerWidget;

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

    bool getStatusLoadTree() const { return m_checkStatusLoadTree;}

protected:
    void previewTime();
    void clearWiget();
    void initCurrentTimeWidget();
    void setParentWiget(const QObject* inObj);
    void dragEventTreeWidget(QEvent* inEvent);
    void dropEventTreeWidget();
    void progressTempStart();
    void reportCheck(LoggerWidget* logger);
    void startCheckData();

    size_t getCountSetsInTree();
    QVector<QDateTime>& getDateTime() { return m_dateTime;};
    Set& getSetsInTree() { return m_loadSetsTree;}

    virtual void dragEnterEvent(QDragEnterEvent* inEvent) override;
    virtual void dropEvent(QDropEvent* inEvent) override;
    virtual void closeEvent(QCloseEvent* inEvent) override;
    virtual bool eventFilter(QObject* inTarget,
                             QEvent* inEvent) override;

    static constexpr int kTimeSetAep = 10;
    static constexpr int kMinTimeModeAep = 5;
    static constexpr int kMaxTimeModeAep = 8;
    static constexpr int kTimeSetPemi = 50;
    double m_step = 0.0;

signals:
    void emitPreviewTime();
    void emitStatus_prb();

private:
    QTreeWidget* m_loadSets_tw    = nullptr;
    QDateTimeEdit* m_editTime_dte = nullptr;
    QLineEdit* m_timeSet_le       = nullptr;
    QLineEdit* m_minTimeModes_le  = nullptr;
    QLineEdit* m_maxTimeModes_le  = nullptr;
    QLineEdit *m_previewTime_le   = nullptr;
    QLineEdit* m_numSets_le       = nullptr;
    QLineEdit* m_saveSets_le      = nullptr;
    QLabel* m_status_l            = nullptr;
    QCheckBox* m_fixedTime_ckb    = nullptr;
    SystemTray* m_sysTray         = nullptr;
    QProgressBar* m_status_prb    = nullptr;
    QPushButton* m_startCheckData_pb = nullptr;
    QPushButton* m_reportCheck_pb    = nullptr;

    QVector<QDateTime>  m_dateTime;
    QDateTime           m_beginWorkTime;
    Set                 m_loadSetsTree;
    Set                 m_tempBufferLoadData;//?????

    QStringList m_reportError;
    bool m_checkStatusLoadTree = true;
};

#endif // TIMERINTERFACE_H
