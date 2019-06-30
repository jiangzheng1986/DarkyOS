#ifndef TIME_H
#define TIME_H

typedef struct
{
    unsigned char byYear;
    unsigned char byMonth;
    unsigned char byDay;
    unsigned char byHour;
    unsigned char byMinute;
    unsigned char bySecond;
}SystemTime;

void GetLocalTime(SystemTime *pLocalTime);

#endif
