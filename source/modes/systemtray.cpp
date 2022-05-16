#include "systemtray.h"
#include "passwidget.h"

#include <QSystemTrayIcon>
#include "QDebug"

//----------------------------
SystemTray::SystemTray(QObject* inParent)
            : QObject(inParent)
            , m_ptrPassForm(new PassWidget)
            , m_nameProgram("Path")
            , m_pathImageIcon(":/image/bad.png")
{
    createTrayIcon();
    connectSlots();

    m_sysTrayIcon->setToolTip(m_nameProgram);
    setImageIcon();
    qDebug() << "Create SystemTray";
}

//----------------------------
SystemTray::~SystemTray()
{
    qDebug() << "~SystemTray";
}

//----------------------------
void SystemTray::createTrayIcon()
{
    m_sysTrayIcon = new QSystemTrayIcon(this);
}

//----------------------------
void SystemTray::connectSlots() const
{
    connect(m_sysTrayIcon, &QSystemTrayIcon::activated, this, &SystemTray::showPassAndHideTray);
}

//----------------------------
void SystemTray::setImageIcon()
{
    m_sysTrayIcon->setIcon(QPixmap(m_pathImageIcon));
}

//----------------------------
void SystemTray::slotStatusProgressBar(const QString& inStatus)
{
    QString status = QString(m_nameProgram + "\n"
                             "Процесс: %1")
                                .arg(inStatus);
    m_sysTrayIcon->setToolTip(status);
}

//----------------------------
void SystemTray::showPassAndHideTray() const
{
    m_sysTrayIcon->hide();
    m_ptrPassForm->show();
}

//----------------------------
void SystemTray::showTrayIcon() const
{
    m_sysTrayIcon->show();
}

//----------------------------
void SystemTray::setPointWidget(QWidget* ptrWidget)
{
    m_ptrPassForm->setPointWidget(ptrWidget);
}

