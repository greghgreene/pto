#include <errno.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

#include "PTODatabase.h"

// PTO Record APIs
PTODBResultType PTORecord::Set(float fHoursNormal, float fHoursPatent, float fHoursService)
{
   PTOResultType res;

   res = PTONormal.Set(fHoursNormal);
   if (res != ePTOSuccess)
   {
      return ePTODBFailure;
   }

   res = PTOPatent.Set(fHoursPatent);
   if (res != ePTOSuccess)
   {
      return ePTODBFailure;
   }

   res = PTOService.Set(fHoursService);
   if (res != ePTOSuccess)
   {
      return ePTODBFailure;
   }

   TransactionDate.SetNow();

   return ePTODBSuccess;
}

PTODBResultType PTORecord::AddPTOHours(float fHours, PTOType type)
{
   PTOResultType res;

   switch (type)
   {
      case ePTONormal:
         res = PTONormal.Add(fHours);
         break;

      case ePTOPatent:
         res = PTOPatent.Add(fHours);
         break;

      case ePTOService:
         res = PTOService.Add(fHours);
         break;

      default:
         printf("%s:  invalid PTO type", __func__);
         break;
   }

   if (res == ePTOSuccess)
   {
      return ePTODBSuccess;
   }
   else
   {
      return ePTODBFailure;
   }
}

PTODBResultType PTORecord::UsePTOHours(float fHours, PTOType type)
{
   PTOResultType res;

   switch (type)
   {
      case ePTONormal:
         res = PTONormal.Use(fHours);
         break;

      case ePTOPatent:
         res = PTOPatent.Use(fHours);
         break;

      case ePTOService:
         res = PTOService.Use(fHours);
         break;

      default:
         break;
   }

   if (res == ePTOSuccess)
   {
      return ePTODBSuccess;
   }
   else
   {
      return ePTODBFailure;
   }
}

PTODBResultType PTORecord::UpdateAccruedHours()
{
   PTOResultType result;

   result = PTONormal.UpdateAccrued();
   if (ePTOSuccess == result)
   {
      return ePTODBSuccess;
   }
   else
   {
      return ePTODBFailure;
   }
}

void PTORecord::Print(void)
{
   //PTONormal.UpdateAccrued();

   printf("%s:  normal %.2f %s patent %.2f %s service %.2f %s\n",
          TransactionDate.GetString(),
          PTONormal.Get(), PTONormal.GetDate(),
          PTOPatent.Get(), PTOPatent.GetDate(),
          PTOService.Get(), PTOService.GetDate());
}

PTODBResultType PTORecord::Read(FILE *fp)
{
   int     rc;
   char    sD[DATE_STRING_SIZE];
   char    sN[DATE_STRING_SIZE];
   char    sP[DATE_STRING_SIZE];
   char    sS[DATE_STRING_SIZE];
   float   fN;
   float   fP;
   float   fS;

   rc = fscanf(fp, "%s %f %s %f %s %f %s", sD, &fN, sN, &fP, sP, &fS, sS);

   if (rc > 0)
   {
      TransactionDate.SetString(sD);
      PTONormal.Set(fN, sN);
      PTOPatent.Set(fP, sP);
      PTOService.Set(fS, sS);
      return ePTODBSuccess;
   }
   else
   {
      return ePTODBFailure;
   }
}

PTODBResultType PTORecord::Save(FILE *fp)
{
   fprintf(fp, "%s %.2f %s %.2f %s %.2f %s\n", TransactionDate.GetString(),
           PTONormal.Get(), PTONormal.GetDate(),
           PTOPatent.Get(), PTOPatent.GetDate(),
           PTOService.Get(), PTOService.GetDate());

   return ePTODBSuccess;
}

/****************************************************************************************************
 * PTODatabase APIs
 ****************************************************************************************************/
PTODBResultType PTODatabase::Create(void)
{
   float fHoursNormal;
   float fHoursPatent;
   float fHoursService;

   if (strlen(sDbFileName) != 0)
   {
      printf("PTODatabase::Create:  database file name is not empty\n");
      return ePTODBFailure;
   }

   printf("Enter database file name:  ");
   fscanf(stdin, "%s", sDbFileName);

   printf("Enter the initial PTO hours (normal, patent, service):  ");
   fscanf(stdin, "%f %f %f", &fHoursNormal, &fHoursPatent, &fHoursService);

   database[nNumEntries].Set(fHoursNormal, fHoursPatent, fHoursService);
   bModified = true;
   current = database[nNumEntries];
   nNumEntries++;

   return ePTODBSuccess;
}

PTODBResultType PTODatabase::Read(void)
{
   FILE *fp;

   if (strlen(sDbFileName) != 0)
   {
      printf("PTODabase::Read:  database file name is not empty\n");
      return ePTODBFailure;
   }

   printf("Enter database file name:  ");
   fscanf(stdin, "%s", sDbFileName);

   fp = fopen(sDbFileName, "r");
   if (fp == NULL)
   {
      perror("PTODatabase::Read:  fopen ");
      return ePTODBFailure;
   }

   nNumEntries = 0;

   while (database[nNumEntries].Read(fp) == ePTODBSuccess)
   {
      nNumEntries++;
   }

   fclose(fp);
   current = database[nNumEntries - 1];
   current.UpdateAccruedHours();

   return ePTODBSuccess;
}

PTODBResultType PTODatabase::Save(void)
{
   FILE *fp;
   int   i;

   fp = fopen(sDbFileName, "w");
   if (fp == NULL)
   {
      perror("PTODatabase::Save:  fopen ");
      return ePTODBFailure;
   }

   for (i = 0; i < nNumEntries; i++)
   {
      database[i].Save(fp);
   }

   fclose(fp);

   bModified = false;
   return ePTODBSuccess;
}

PTODBResultType PTODatabase::AddPTOHours(void)
{
   int             nType;
   float           fHours;
   PTODBResultType res;

   printf("Enter 1 for Normal, 2 for Patent, and 3 for Service:  ");
   fscanf(stdin, "%d", &nType);
   printf("Enter number of hours to add:  ");
   fscanf(stdin, "%f", &fHours);

   database[nNumEntries] = current;

   switch (nType)
   {
      case 1:
         res = database[nNumEntries].AddPTOHours(fHours, ePTONormal);
         break;
      case 2:
         res = database[nNumEntries].AddPTOHours(fHours, ePTOPatent);
         break;
      case 3:
         res = database[nNumEntries].AddPTOHours(fHours, ePTOService);
         break;
      default:
         res = ePTODBFailure;
   }

   if (res == ePTODBSuccess)
   {
      current = database[nNumEntries];
      nNumEntries++;
      bModified = true;
      return ePTODBSuccess;
   }
   else
   {
      return ePTODBFailure;
   }
}

PTODBResultType PTODatabase::UsePTOHours(void)
{
   int             nType;
   float           fHours;
   PTODBResultType res;

   printf("Enter 1 for Normal, 2 for Patent, and 3 for Service:  ");
   fscanf(stdin, "%d", &nType);
   printf("Enter number of hours to use:  ");
   fscanf(stdin, "%f", &fHours);

   database[nNumEntries] = current;

   switch (nType)
   {
      case 1:
         res = database[nNumEntries].UsePTOHours(fHours, ePTONormal);
         break;
      case 2:
         res = database[nNumEntries].UsePTOHours(fHours, ePTOPatent);
         break;
      case 3:
         res = database[nNumEntries].UsePTOHours(fHours, ePTOService);
         break;
      default:
         res = ePTODBFailure;
   }

   if (res == ePTODBSuccess)
   {
      current = database[nNumEntries];
      nNumEntries++;
      bModified = true;
      return ePTODBSuccess;
   }
   else
   {
      return ePTODBFailure;
   }
}

bool PTODatabase::Modified(void)
{
   return bModified;
}

void PTODatabase::PrintHistory(void)
{
   int i;

   printf("\n\n");
   for (i = 0; i < nNumEntries; i++)
   {
      database[i].Print();
   }
   printf("\n\n");
}

void PTODatabase::PrintCurrent(void)
{
   printf("\n\n");
   current.Print();
   printf("\n\n");
}

void PTODatabase::PrintError(PTODBResultType res)
{
   switch (res)
   {
      case ePTODBSuccess:
         printf("Success\n"); break;
      case ePTODBFailure:
         printf("Failure\n"); break;
      default:
         printf("Unknown\n"); break;
   }
}
