#ifndef DATE_H
#define DATE_H

typedef struct {
    int day;
    int month;
    int year;
} Date;

Date createDate(int day, int month, int year);
int compareDates(Date date1, Date date2);
void printDate(Date date);

#endif

