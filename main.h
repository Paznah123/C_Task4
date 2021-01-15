#pragma once

#define EXIT			-1

//#define MANAGER_FILE_NAME "airport_authority.txt"
//#define COMPANY_FILE_NAME "company.bin"

//================================= 

typedef enum
{
	eAddFlight = 1, eAddAirport, ePrintCompany, ePrintAirports,ePrintFlightOrigDest,
	eSortFlights, eSearchFlight,
	eNofOptions
} eMenuOptions;

//================================= 

const char* str[eNofOptions];

int menu();
int initManagerAndCompany(AirportManager* pManager, Company* pCompany, const char* manager_fileName, const char* company_fileName);

//================================= 
