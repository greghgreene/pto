#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

using namespace std;

#include "Date.h"

// Determine the number of days between 2 dates.
int operator-(const Date& end, const Date& start)
{
   int nDaysAdd;
   int nDaysSub;
   int nTotalDays;
   int nOffsetMonths[12] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

   nDaysAdd = end.nDay + nOffsetMonths[end.nMonth - 1];
   nDaysSub = start.nDay + nOffsetMonths[start.nMonth - 1];
   nTotalDays = (int)(((double)(end.nYear - start.nYear)) * 365.2422 + ((double)nDaysAdd - nDaysSub + 1));

   return nTotalDays;
}

Date::Date(void)
{
   this->SetNow();
   snprintf(sDate, sizeof(sDate), "%02d%02d%4d", nMonth, nDay, nYear);
}

Date::Date(int nYearNew, int nMonthNew, int nDayNew)
{
   nYear = nYearNew;
   nMonth = nMonthNew;
   nDay = nDayNew;

   snprintf(sDate, sizeof(sDate), "%02d%02d%4d", nMonth, nDay, nYear);
}

void Date::SetNow(void)
{
   time_t t;
   struct tm *info;

   time(&t);
   info = localtime(&t);

   nYear = info->tm_year + 1900;
   nMonth = info->tm_mon + 1;
   nDay = info->tm_mday;

   snprintf(sDate, sizeof(sDate), "%02d%02d%4d", nMonth, nDay, nYear);
}

char* Date::GetString(void)
{
   return sDate;
}

void Date::SetString(char *str)
{
   int result;

   strncpy(sDate, str, DATE_STRING_SIZE);
   result = sscanf(sDate, "%02d%02d%4d", &nMonth, &nDay, &nYear);
   if (3 != result)
   {
      printf("%s:  sscanf failed:  %d\n", __func__, result);
   }
}

void Date::Print(void)
{
   printf("%02d/%02d/%4d", nMonth, nDay, nYear);
}
