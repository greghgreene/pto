#ifndef DATE_H_INCLUDED
#define DATE_H_INCLUDED

#define DATE_STRING_SIZE 9

class Date
{
protected:
    int    nYear;
    int    nMonth;
    int    nDay;
    char   sDate[DATE_STRING_SIZE];
public:
    Date (void);
    Date (int nYearNew, int nMonthNew, int nDayNew);
    friend int operator- (const Date& start, const Date& end);

    void  SetNow (void);
    char* GetString (void);
    void  SetString (char *);
    void  Print (void);
};

#endif // DATE_H_INCLUDED
