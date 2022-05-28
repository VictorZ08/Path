#ifndef TIMER_H
#define TIMER_H

class QDateTime;
class QFileInfo;

const QDateTime getWorkDateTime();
void setDateTimeFiles(const QFileInfo& inFiles,
                      const QDateTime& inDateTime);

#endif // TIMER_H
