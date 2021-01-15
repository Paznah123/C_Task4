#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "Company.h"
#include "AirportManager.h"
#include "General.h"
#include "main.h"

//================================= 

const char* str[eNofOptions] = { "Add Flight", "Add Airport",
									"PrintCompany", "Print all Airports",
										"Print flights between origin-destination",
														"Sort Flights", "Search Flight" };
//================================= 

int main(int argc, char* argv[])
{
	if (argc != 3) {
		printf("Error in params\n");
		system("pause");
		return -1;
	}

	AirportManager	manager;
	Company			company;

	initManagerAndCompany(&manager, &company, argv[1], argv[2]);

	int option;
	int stop = 0;

	do
	{
		option = menu();
		switch (option)
		{
			case eAddFlight:
				if (!addFlight(&company, &manager))
					printf("Error adding flight\n");
				break;

			case eAddAirport:
				if (!addAirport(&manager))
					printf("Error adding airport\n");
				break;

			case ePrintCompany:
				printCompany(&company, "Hachi", "Babit", "Ba", "Olam", NULL);
				break;

			case ePrintAirports:
				printAirports(&manager);
				break;
		
			case ePrintFlightOrigDest:
				printFlightsCount(&company);
				break;
		
			case eSortFlights:
				sortFlight(&company);
				break;

			case eSearchFlight:
				findFlight(&company);
				break;

			case EXIT:
				printf("Bye bye\n");
				stop = 1;
				break;

			default:
				printf("Wrong option\n");
				break;
		}
	} while (!stop);


	saveManagerToFile(&manager, argv[1]);
	saveCompanyToFile(&company, argv[2]);

	freeManager(&manager);
	freeCompany(&company);

	system("pause");
	return 1;
}

//================================= 

int menu()
{
	int option;
	printf("\n======================\n");
	printf("Please choose one of the following options\n");

	for (int i = 0; i < eNofOptions - 1; i++)
		printf("- %d - %s\n", i + 1, str[i]);

	printf("%d - Quit\n", EXIT);
	printf("====================== ");
	scanf("%d", &option);
	
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

//================================= 

int initManagerAndCompany(AirportManager* pManager ,Company* pCompany, const char* manager_fileName, const char* company_fileName)
{
	int res = initManager(pManager, manager_fileName);
	if (!res)
	{
		printf("error init manager\n");
		return 0;
	}

	if (res == FROM_FILE)
		return initCompanyFromFile(pCompany,pManager, company_fileName);
	else
		initCompany(pCompany, pManager);
	return 1;
}