#ifndef __COMP__
#define __COMP__

#include "Flight.h"
#include "AirportManager.h"
#include "GeneralList.h"

typedef enum { eNone, eHour, eDate,eSorceCode,eDestCode, eNofSortOpt } eSortOption;
static const char* sortOptStr[eNofSortOpt];

//================================= 

typedef struct
{
	char*		name;
	int			flightCount;
	Flight**	flightArr;
	eSortOption	sortOpt;
	LIST		flightDateList;
}Company;

//================================= 

int			initCompanyFromFile(Company* pComp, AirportManager* pManaer, const char* fileName);
void		initCompany(Company* pComp, AirportManager* pManaer);
void		initDateList(Company* pComp);
int			isUniqueDate(const Company* pComp, int index);
int			addFlight(Company* pComp,const AirportManager* pManager);
void		printCompany(const Company* pComp, const char* str, ...);
void		printFlightsCount(const Company* pComp);
int			saveCompanyToFile(const Company* pComp, const char* fileName);
int			loadCompanyFromFile(Company* pComp, const char* fileName);
void		sortFlight(Company* pComp);
void		findFlight(const Company* pComp);
void		freeCompany(Company* pComp);
eSortOption showSortMenu();

//================================= 

#endif

