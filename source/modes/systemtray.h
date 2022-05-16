#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QObject>
#include <QSharedPointer>

class PassWidget;
class QSystemTrayIcon;

class SystemTray : public QObject
{
    Q_OBJECT
public:
    SystemTray(QObject* inParent = nullptr);
    ~SystemTray();
    void showTrayIcon() const;
    void setPointWidget(QWidget* ptrWidget);

public slots:
    void slotStatusProgressBar(const QString& inStatus);

protected:
    void connectSlots() const;
    void setImageIcon();
    void createTrayIcon();
    void showPassAndHideTray() const;

private:
    QSystemTrayIcon* m_sysTrayIcon  = nullptr;
    QSharedPointer<PassWidget>   m_ptrPassForm;

    const QString m_nameProgram;
    const QString m_pathImageIcon;
};

#endif // SYSTEMTRAY_H
