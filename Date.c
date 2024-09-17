#define _CRT_SECURE_NO_WARNINGS
#include "date.h"
#include <stdio.h>

Date createDate(int day, int month, int year) {
    Date date;
    date.day = day;
    date.month = month;
    date.year = year;
    return date;
}

int compareDates(Date date1, Date date2) {
    if (date1.year != date2.year) {
        printf("not the same year");
        return -1;
    }
    if (date1.month != date2.month) {
        printf("not the same month");
        return -1;
    }
    if (date1.day != date2.day) {
        printf("not the same day");
        return -1;
    }
    return 0;
}

void printDate(Date date) {
    printf("%02d-%02d-%04d\n", date.day, date.month, date.year);
}
