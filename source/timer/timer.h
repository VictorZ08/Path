#ifndef TIMER_H
#define TIMER_H

class QDateTime;
class QFileInfo;

class Timer
{
public:
    static const QDateTime getWorkDateTime();
    static void setDateTimeFile(const QFileInfo& inNameFile,
                                const QDateTime& inDateTime);
private:
    Timer() {};
};

#endif // TIMER_H
