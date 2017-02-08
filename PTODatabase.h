#ifndef PTODATABASE_H_INCLUDED
#define PTODATABASE_H_INCLUDED

#include "Date.h"
#include "PTO.h"

#define MAX_PTO_RECORDS 1000
#define MAX_DB_FILENAME 100

typedef enum
{
   ePTODBSuccess,
   ePTODBFailure
} PTODBResultType;

// A PTO record consists of each type of PTO
class PTORecord
{
 protected:
    PTOAccrue  PTONormal = 0.0;
    PTO        PTOPatent = 0.0;
    PTO        PTOService = 0.0;
    Date       TransactionDate;
 public:
    PTODBResultType Set(float fHoursNormal, float fHoursPatent, float fHoursService);
    PTODBResultType Read(FILE *fp);
    PTODBResultType Save(FILE *fp);

    PTODBResultType AddPTOHours(float fHours, PTOType type);
    PTODBResultType UsePTOHours(float fHours, PTOType type);
    PTODBResultType UpdateAccruedHours(void);
    void            Print(void);
};

class PTODatabase
{
 protected:
    int        nNumEntries = 0;
    char       sDbFileName[MAX_DB_FILENAME] = { 0 };
    bool       bModified = false;
    PTORecord  database[MAX_PTO_RECORDS];
    PTORecord  current;

 public:
    PTODBResultType Create(void);
    PTODBResultType CreateRecord(void);
    PTODBResultType Read(void);
    PTODBResultType Save(void);

    PTODBResultType AddPTOHours(void);
    PTODBResultType UsePTOHours(void);

    bool            Modified(void);
    void            PrintHistory(void);
    void            PrintCurrent(void);
    void            PrintError(PTODBResultType);
};

#endif // PTODATABASE_H_INCLUDED
