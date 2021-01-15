#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Flight.h"
#include "General.h"
#include "fileHelper.h"
#include "Macros.h"

//================================= init

void	initFlight(Flight* pFlight, const AirportManager* pManager)
{
	Airport* pPortOr = setAiportToFlight(pManager, "Enter code of origin airport:");
	strcpy(pFlight->originCode, pPortOr->code);
	int same;
	Airport* pPortDes;
	do {
		pPortDes = setAiportToFlight(pManager, "Enter code of destination airport:");
		same = isSameAirport(pPortOr, pPortDes);
		if (same)
			printf("Same origin and destination airport\n");
	} while (same);
	strcpy(pFlight->destCode, pPortDes->code);
	getCorrectDate(&pFlight->date);
	pFlight->hour = getFlightHour();
}

//================================= getters

int getFlightHour()
{
	int h;
	do {
		printf("Enter flight hour [0-23]:\t");
		scanf("%d", &h);
	} while (h < 0 || h>23);
	return h;
}

Airport* setAiportToFlight(const AirportManager* pManager, const char* msg)
{
	char code[MAX_STR_LEN];
	Airport* port;
	do
	{
		printf("%s\t", msg);
		myGets(code, MAX_STR_LEN, stdin);
		port = findAirportByCode(pManager, code);
		if (port == NULL)
			printf("No airport in this country - try again\n");
	} while(port == NULL);

	return port;
}

//================================= checkers

int		isFlightInRoute(const Flight* pFlight, const char* codeSource, const char* codeDest)
{
	if ((strcmp(pFlight->originCode, codeSource) == 0) &&
		(strcmp(pFlight->destCode, codeDest) == 0))
		return 1;

	return 0;
}


int		countFlightsInRoute(Flight** arr, int size, const char* codeSource,
	const char* codeDest)
{
	int count = 0;
	for (int i = 0; i < size; i++)
	{
		if (isFlightInRoute(arr[i], codeSource, codeDest))
			count++;
	}
	return count;
}

//================================= file

int saveFlightToFile(const Flight* pF, FILE* fp)
{

	int year = pF->date.year << 14;
	int month = pF->date.month << 10;
	int day = pF->date.day << 5;
	int hour = pF->hour;

	int flightData = year | month | day | hour;
	if (fwrite(&flightData, sizeof(int), 1, fp) != 1)
	{
		printf("Error write flight\n");
		return 0;
	}
	return 1;
}

int loadFlightFromFile(Flight* pF, FILE* fp)
{
	char newOriginCode[CODE_LENGTH + 1] = { 0 };
	char newDestCode[CODE_LENGTH + 1] = { 0 };
	CHECK_0_MSG_CLOSE_FILE(fread(newOriginCode, sizeof(char), CODE_LENGTH, fp), fp, "Error reding origin code");
	CHECK_0_MSG_CLOSE_FILE(fread(newDestCode, sizeof(char), CODE_LENGTH, fp), fp, "Error reding destination code");

	strcpy(pF->originCode, newOriginCode);
	strcpy(pF->destCode, newDestCode);

	unsigned int flightData = 0;
	CHECK_0_MSG_CLOSE_FILE(fread(&flightData, sizeof(int), 1, fp), fp, "Error reding flight data");

	pF->date.year = flightData >> 14;
	flightData <<= 18;
	pF->date.month = flightData >> 28;
	flightData <<= 4;
	pF->date.day = flightData >> 27;
	flightData <<= 5;
	pF->hour = flightData >> 27;

	return 1;
}

//================================= compare

int	compareByCodeOrig(const void* flight1, const void* flight2)
{
	const Flight* pFlight1 = *(const Flight**)flight1;
	const Flight* pFlight2 = *(const Flight**)flight2;
	return strcmp(pFlight1->originCode, pFlight2->originCode);
}

int	compareByCodeDest(const void* flight1, const void* flight2)
{
	const Flight* pFlight1 = *(const Flight**)flight1;
	const Flight* pFlight2 = *(const Flight**)flight2;
	return strcmp(pFlight1->destCode, pFlight2->destCode);
}

int	compareByHour(const void* flight1, const void* flight2)
{
	const Flight* pFlight1 = *(const Flight**)flight1;
	const Flight* pFlight2 = *(const Flight**)flight2;
	return (pFlight1->hour - pFlight2->hour);
}

int		compareByDate(const void* flight1, const void* flight2)
{
	const Flight* pFlight1 = *(const Flight**)flight1;
	const Flight* pFlight2 = *(const Flight**)flight2;

	Date d1 = pFlight1->date;
	Date d2 = pFlight2->date;

	if (d1.year > d2.year)
		return 1;
	if (d1.year < d2.year)
		return -1;

	if (d1.month > d2.month)
		return 1;
	if (d1.month < d2.month)
		return -1;

	if (d1.day > d2.day)
		return 1;
	if (d1.day < d2.day)
		return -1;

	return 0;
}

//================================= print

void	printFlight(const Flight* pFlight)
{
	printf("Flight - %s -> %s | ", pFlight->originCode, pFlight->destCode);
	printf("Hour: %d\t", pFlight->hour);
	printDate(&pFlight->date);
}

void	printFlightV(const void* val)
{
	const Flight* pFlight = *(const Flight**)val;
	printFlight(pFlight);
}

//================================= free

void	freeFlight(void* val)
{
	Flight* pFlight = *(Flight**)val;
	free(pFlight);
}
