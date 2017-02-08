#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

#include "PTO.h"

// PTO APIs
PTO::PTO(void) : DateUpdated()
{
   fHours = 0.0;
}

PTO::PTO(float fHoursNew) : DateUpdated()
{
   fHours = fHoursNew;
}

PTO::PTO(float fHoursNew, int nYearNew, int nMonthNew, int nDayNew) : DateUpdated(nYearNew, nMonthNew, nDayNew)
{
   fHours = fHoursNew;
}

void PTO::Print(void)
{
   cout << "PTO: " << fHours << " ";
   DateUpdated.Print();
   cout << endl;
}

float PTO::Get(void)
{
   return fHours;
}

PTOResultType PTO::Add(float fNewHours)
{
   fHours = fHours + fNewHours;
   DateUpdated.SetNow();
   return ePTOSuccess;
}

PTOResultType PTO::Set(float fCurrHours)
{
   fHours = fCurrHours;
   DateUpdated.SetNow();
   return ePTOSuccess;
}

PTOResultType PTO::Set(float fCurrHours, char *sDateString)
{
   fHours = fCurrHours;
   DateUpdated.SetString(sDateString);
   return ePTOSuccess;
}

PTOResultType PTO::Use(float fNewHours)
{
   fHours = fHours - fNewHours;
   DateUpdated.SetNow();

   if (fHours < 0.0)
   {
      printf("%s:  negative hours", __func__);
      return ePTOFailure;
   }
   else
   {
      return ePTOSuccess;
   }
}

char* PTO::GetDate(void)
{
   return DateUpdated.GetString();
}

// PTOAccrue APIs
PTOAccrue::PTOAccrue(void) : PTO()
{
}

PTOAccrue::PTOAccrue(float fHours) : PTO(fHours)
{
}

PTOAccrue::PTOAccrue(float fHours, int nYear, int nMonth, int nDay) : PTO(fHours, nYear, nMonth, nDay)
{
}

PTOResultType PTOAccrue::UpdateAccrued(void)
{
   int     nDays;
   int     nBiweeks;
   float   fAccruedPTOHours;
   Date    Now;

   // Subtract the date last updated from the current date to get the number of days since the last update
   nDays = Now - DateUpdated;

   nBiweeks = (int)((float)nDays / 14);
   fAccruedPTOHours = nBiweeks * fAccrueBiweekly;
   fHours = fHours + fAccruedPTOHours;
   //printf("%d %d %f %f\n", nDays, nBiweeks, fAccruedPTOHours, fHours);
   return ePTOSuccess;
}

PTOResultType PTOAccrue::Add(float fNewHours)
{
   PTOResultType res;

   res = this->UpdateAccrued();

   if (res != ePTOSuccess)
   {
      return res;
   }

   DateUpdated.SetNow();
   fHours = fHours + fNewHours;

   return ePTOSuccess;
}

PTOResultType PTOAccrue::Use(float fNewHours)
{
   PTOResultType res;

   res = this->UpdateAccrued();

   if (res != ePTOSuccess)
   {
      return res;
   }

   DateUpdated.SetNow();
   fHours = fHours - fNewHours;

   if (fHours < 0.0)
   {
      printf("%s:  negative hours", __func__);
      return ePTOFailure;
   }
   else
   {
      return ePTOSuccess;
   }
}
