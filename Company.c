#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "Company.h"
#include "Airport.h"
#include "Flight.h"
#include "General.h"
#include "fileHelper.h"
#include "Macros.h"

#define DETAIL_PRINT

static const char* sortOptStr[eNofSortOpt] = {
	"None","Hour", "Date", "Airport takeoff code", "Airport landing code" };

//================================= init

int		initCompanyFromFile(Company* pComp, AirportManager* pManaer, const char* fileName)
{
	L_init(&pComp->flightDateList);
	if (loadCompanyFromFile(pComp, fileName))
	{
		initDateList(pComp);
		return 1;
	}
	return 0;
}

void	initCompany(Company* pComp,AirportManager* pManaer)
{
	printf("-----------  Init Airline Company\n");
	L_init(&pComp->flightDateList);
	
	pComp->name = getStrExactName("Enter company name");
	pComp->flightArr = NULL;
	pComp->flightCount = 0;
}

void	initDateList(Company* pComp)
{	
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if(isUniqueDate(pComp,i))
		{
			char* sDate = createDateString(&pComp->flightArr[i]->date);
			L_insert(&(pComp->flightDateList.head), sDate);
		}
	}
}

//================================= add

int		addFlight(Company* pComp, const AirportManager* pManager)
{
	if (pManager->count < 2)
	{
		printf("There are not enough airports to set a flight\n");
		return 0;
	}
	pComp->flightArr = (Flight**)realloc(pComp->flightArr, (pComp->flightCount + 1) * sizeof(Flight*));
	CHECK_RETURN_0(pComp->flightArr);

	pComp->flightArr[pComp->flightCount] = (Flight*)calloc(1, sizeof(Flight));
	CHECK_RETURN_0(pComp->flightArr[pComp->flightCount]);

	initFlight(pComp->flightArr[pComp->flightCount], pManager);
	if (isUniqueDate(pComp, pComp->flightCount))
	{
		char* sDate = createDateString(&pComp->flightArr[pComp->flightCount]->date);
		L_insert(&(pComp->flightDateList.head), sDate);
	}
	pComp->flightCount++;
	return 1;
}

//================================= checkers

int		isUniqueDate(const Company* pComp, int index)
{
	Date* pCheck = &pComp->flightArr[index]->date;
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (i == index)
			continue;
		if (equalDate(&pComp->flightArr[i]->date,pCheck))
			return 0;
	}
	return 1;
}

//================================= file

int		saveCompanyToFile(const Company* pComp, const char* fileName) {
	FILE* fp;
	fp = fopen(fileName, "wb");
	CHECK_MSG_RETURN_0(fp, "Error open copmpany file to write\n");
	
	unsigned short fCount = pComp->flightCount << 7;
	unsigned short sortOpt = pComp->sortOpt << 4;
	unsigned short nameLength = (short)strlen(pComp->name);

	unsigned short pCompData = fCount | sortOpt | nameLength;

	fwrite(&pCompData, sizeof(short), 1, fp);
	fwrite(pComp->name, sizeof(char), nameLength + 1, fp);

	for (int i = 0; i < pComp->flightCount; i++)
	{
		fwrite(&pComp->flightArr[i]->originCode, sizeof(char), CODE_LENGTH, fp);
		fwrite(&pComp->flightArr[i]->destCode, sizeof(char), CODE_LENGTH, fp);

		if (!saveFlightToFile(pComp->flightArr[i], fp))
			return 0;
	}
	fclose(fp);
	return 1;
}

int loadCompanyFromFile(Company* pComp, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "rb");
	CHECK_MSG_RETURN_0(fp, "Error open company file\n")
	pComp->flightArr = NULL;

	unsigned short pCompData = 0;

	fread(&pCompData, sizeof(short), 1, fp);
	int length = pCompData & 0xF;
	pComp->flightCount = pCompData >> 7;
	pCompData <<= 8;

	pComp->sortOpt = pCompData >> 15;
	pComp->name = (char*)malloc(sizeof(char) * (length + 1));
	fread(pComp->name, sizeof(char), length + 1, fp);

	pComp->flightArr = (Flight**)malloc(pComp->flightCount * sizeof(Flight*));
	for (int i = 0; i < pComp->flightCount; i++)
	{
		pComp->flightArr[i] = (Flight*)calloc(1, sizeof(Flight));
		CHECK_RETURN_0(pComp->flightArr[i]);
		if (!loadFlightFromFile(pComp->flightArr[i], fp))
			return 0;
	}
	fclose(fp);
	return 1;
}

//================================= list

void	sortFlight(Company* pComp)
{
	pComp->sortOpt = showSortMenu();
	int(*compare)(const void* air1, const void* air2) = NULL;

	switch (pComp->sortOpt)
	{
		case eHour:
			compare = compareByHour;
			break;
		case eDate:
			compare = compareByDate;
			break;
		case eSorceCode:
			compare = compareByCodeOrig;
			break;
		case eDestCode:
			compare = compareByCodeDest;
			break;
	}

	if (compare != NULL)
		qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compare);
}

void	findFlight(const Company* pComp)
{
	int(*compare)(const void* air1, const void* air2) = NULL;
	Flight f = { 0 };
	Flight* pFlight = &f;

	switch (pComp->sortOpt)
	{
		case eHour:
			f.hour = getFlightHour();
			compare = compareByHour;
			break;
		case eDate:
			getCorrectDate(&f.date);
			compare = compareByDate;
			break;
		case eSorceCode:
			getAirportCode(f.originCode);
			compare = compareByCodeOrig;
			break;
		case eDestCode:
			getAirportCode(f.destCode);
			compare = compareByCodeDest;
			break;
	}

	if (compare != NULL)
	{
		Flight** pF = bsearch(&pFlight, pComp->flightArr, pComp->flightCount, sizeof(Flight*), compare);
		if (pF == NULL)
			printf("Flight was not found\n");
		else {
			printf("Flight found, ");
			printFlight(*pF);
		}
	}
	else {
		printf("The search cannot be performed, array not sorted\n");
	}
}

//================================= 

eSortOption showSortMenu()
{
	int opt;
	printf("Base on what field do you want to sort?\n");
	do {
		for (int i = 1; i < eNofSortOpt; i++)
			printf("Enter %d for %s\n", i, sortOptStr[i]);
		scanf("%d", &opt);
	} while (opt < 0 || opt > eNofSortOpt);

	return (eSortOption)opt;
}

//================================= print

void	printFlightsCount(const Company* pComp)
{
	char codeOrigin[CODE_LENGTH + 1];
	char codeDestination[CODE_LENGTH + 1];

	if (pComp->flightCount == 0)
	{
		printf("No flight to search\n");
		return;
	}

	printf("Origin Airport\n");
	getAirportCode(codeOrigin);
	printf("Destination Airport\n");
	getAirportCode(codeDestination);

	int count = countFlightsInRoute(pComp->flightArr, pComp->flightCount, codeOrigin, codeDestination);
	if (count != 0)
		printf("There are %d flights ", count);
	else
		printf("There are No flights ");

	printf("from %s to %s\n", codeOrigin, codeDestination);
}

void	printCompany(const Company* pComp, const char* str, ...)
{
#ifndef DETAIL_PRINT
	printf("Company : %s\n\n", pComp->name);
	printf("Has %d flights\n\n", pComp->flightCount);
#else
	va_list list;
	const char* strTemp;
	char* longStr = NULL;
	int combineLength = 0;

	va_start(list, str);
	strTemp = str;
	while (strTemp != NULL)
	{
		if (combineLength == 0)
		{
			combineLength = (int)strlen(pComp->name) + (int)strlen(strTemp) + 2;
			longStr = (char*)realloc(longStr, combineLength * sizeof(char));
			if (!longStr)
				return;

			strcpy(longStr, pComp->name);
			strcat(longStr, "_");
			strcat(longStr, strTemp);
		}
		else {
			combineLength += (strlen(strTemp) + 2);
			longStr = (char*)realloc(longStr, combineLength * sizeof(char));
			strcat(longStr, "_");
			strcat(longStr, strTemp);
		}
		strTemp = va_arg(list, const char*);
	}

	longStr[combineLength - 1] = '\0';
	printf("Company : %s\n\n", longStr);
	printf("Has %d flights\n\n", pComp->flightCount);
	generalArrayFunction((void*)pComp->flightArr, pComp->flightCount, sizeof(Flight**), printFlightV);
	printf("\nFlight Date List:");
	L_print(&pComp->flightDateList, printStr);
#endif
}

//================================= 

char* combiningStrings(const char* str, ...)
{
	va_list list;
	const char* strTemp;
	char* longStr = NULL;
	int combineLength = 0;
	int len;

	va_start(list, str);
	strTemp = str;
	while (strTemp != NULL)
	{
		len = (int)strlen(strTemp);
		longStr = (char*)realloc(longStr, (combineLength + len + 2) * sizeof(char));
		if (!longStr)
			return NULL;

		if (combineLength == 0) {
			strcpy(longStr, strTemp);
			combineLength = len;
		} else {
			strcat(longStr, "_");
			strcat(longStr, strTemp);
			combineLength += (len + 1);
		}
		strTemp = va_arg(list, const char*);
	}
	va_end(list);
	return longStr;
}

//================================= free

void	freeCompany(Company* pComp)
{
	generalArrayFunction((void*)pComp->flightArr, pComp->flightCount, sizeof(Flight**), freeFlight);
	free(pComp->flightArr);
	free(pComp->name);
	L_free(&pComp->flightDateList, freePtr);
}
