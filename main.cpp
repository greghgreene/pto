#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

#include "PTODatabase.h"

typedef enum
{
   eSuccess,
   eFailure
} ResultType;

static class PTODatabase gDb;

void PrintUsage(void)
{
   printf("Options: \n");
   printf("\t1:  Create New Database\n");
   printf("\t2:  Read Existing Database\n");
   printf("\t3:  Add PTO Hours\n");
   printf("\t4:  Use PTO Hours\n");
   printf("\t5:  Print PTO History\n");
   printf("\t6:  Print Current PTO Hours\n");
   printf("\t7:  Save Database\n");
   printf("\t0:  Exit\n");
}

ResultType HandleOption(int option, bool debug)
{
   PTODBResultType PTOrs = ePTODBSuccess;
   char exit_response;

   switch (option)
   {
      case 1:
         PTOrs = gDb.Create();       break;
      case 2:
         PTOrs = gDb.Read();         break;
      case 3:
         PTOrs = gDb.AddPTOHours();  break;
      case 4:
         PTOrs = gDb.UsePTOHours();  break;
      case 5:
         gDb.PrintHistory(); break;
      case 6:
         gDb.PrintCurrent(); break;
      case 7:
         PTOrs = gDb.Save();         break;
      case 0:
         if (gDb.Modified() == true)
         {
            printf("Database has not been saved.  Save before exiting (y/n)? ");
            cin >> exit_response;

            if (exit_response == 'y')
            {
               gDb.Save();
            }
         }
         break;

      default:
         printf("Invalid option:  %d\n", option);
         PTOrs = ePTODBFailure;
   }

   if (PTOrs == ePTODBSuccess)
   {
      return eSuccess;
   }
   else
   {
      printf("HandleOption failed:  %d\n", (int)PTOrs);
      gDb.PrintError(PTOrs);
      return eFailure;
   }
}

int main(int argc, char **argv)
{
   ResultType ret;
   int        i;
   int        option = -1;
   bool       debug  = false;

   if (argc > 1)
   {
      for (i = 1; i < argc; i++)
      {
         if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0))
         {
            PrintUsage();
         }
         else if ((strcmp(argv[i], "-d") == 0) || (strcmp(argv[i], "--debug") == 0))
         {
            debug = true;
         }
      }

   }

   do
   {
      PrintUsage();
      printf("Enter option: ");
      cin >> option;
      ret = HandleOption(option, debug);

      if (ret != eSuccess)
      {
         printf("Failure:  %d\n", (int)ret);
      }

   }
   while (option != 0);

   return eSuccess;
}
