#include "timer.h"

#include <QDateTime>
#include <QFileInfo>
#include <qt_windows.h>


const QDateTime Timer::getWorkDateTime()
{
    if(QTime::currentTime() < QTime(20,15) &&
       QTime::currentTime() < QTime(8,15))
       return QDateTime(QDate::currentDate(), QTime(20, 15));
    else
       return QDateTime(QDate::currentDate(), QTime(8, 15));
}

//-------------------------------
void Timer::setDateTimeFile(const QFileInfo& inNameFile, const QDateTime &inDateTime)
{
    SYSTEMTIME st;
    FILETIME ft;
    TIME_ZONE_INFORMATION tzi;
    HANDLE hFile;

    LPCTSTR stdPath = (wchar_t*)inNameFile.filePath().utf16();
    hFile = CreateFile(stdPath,
                       FILE_WRITE_ATTRIBUTES,
                       FILE_SHARE_WRITE|FILE_SHARE_READ,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_BACKUP_SEMANTICS,
                       NULL);

    FileTimeToSystemTime(&ft, &st);
    st.wYear   = inDateTime.toString("yyyy").toInt();
    st.wMonth  = inDateTime.toString("MM").toInt();
    st.wDay    = inDateTime.toString("dd").toInt();
    st.wHour   = inDateTime.toString("hh").toInt();
    st.wMinute = inDateTime.toString("mm").toInt();
    /*Извлекает текущие настройки часового пояса*/
    GetTimeZoneInformation(&tzi);
    /*Преобразует местное время в UTC-время*/
    TzSpecificLocalTimeToSystemTime(&tzi,&st,&st);
    /*Преобразует системное время в файловое*/
    SystemTimeToFileTime(&st, &ft);
    SetFileTime(hFile, &ft, &ft, &ft);
    CloseHandle(hFile);
}

