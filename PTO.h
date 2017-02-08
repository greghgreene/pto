#ifndef PTO_H_INCLUDED
#define PTO_H_INCLUDED

#include "Date.h"

typedef enum
{
    ePTOSuccess,
    ePTOFailure
} PTOResultType;

typedef enum
{
    ePTONormal,
    ePTOPatent,
    ePTOService
} PTOType;

// Generic PTO class.  This is for PTOs that don't accrue bi-weekly
// such as patent and service days.
class PTO
{
protected:
    float  fHours;
    Date   DateUpdated;

public:
    PTO (void);
    PTO (float fHours);
    PTO (float fHours, int nYear, int nMonth, int nDay);

    void           Print (void);
    float          Get (void);
    PTOResultType  Add (float fNewHours);
    PTOResultType  Set (float fCurrHours);
    PTOResultType  Set (float fCurrHours, char* sDateString);
    PTOResultType  Use (float fNewHours);
    char*          GetDate (void);
};

// PTOs that accrue (bi-weekly)
class PTOAccrue:  public PTO
{
private:
    float fAccrueBiweekly = 7.7;  // Accrue for employees with 20+ years


public:
    PTOAccrue (void);
    PTOAccrue (float fHours);
    PTOAccrue (float fHours, int nYear, int nMonth, int nDay);

    PTOResultType  Add (float fNewHours);
    PTOResultType  Use (float fNewHours);
    PTOResultType  UpdateAccrued (void); // Updates the accrued PTO hours
};

#endif // PTO_H_INCLUDED
