#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "General.h"
#include "Macros.h"

//================================= getters

char* getStrExactName(const char* msg)
{
	char* str;
	char temp[MAX_STR_LEN];
	printf("%s\t", msg);
	myGets(temp, MAX_STR_LEN,stdin);

	str = getDynStr(temp);
	return str;
}

char* getDynStr(char* str)
{
	char* theStr;
	theStr = (char*)malloc((strlen(str) + 1) * sizeof(char));
	CHECK_RETURN_NULL(theStr);

	strcpy(theStr, str);
	return theStr;
}

char*  myGets(char* buffer, int size, FILE* fp)
{
	if (buffer != NULL && size > 0)
	{
		if (fgets(buffer, size, fp))
		{
			buffer[strcspn(buffer, "\n")] = '\0';
			return buffer;
		}
		buffer[0] = '\0';
	}
	return NULL;
}

//================================= string

char**	splitCharsToWords(char* str, int* pCount, int* pTotalLength)
{
	char temp[255];
	char* delimiters = " ";
	char* word;
	int count = 0;

	strcpy(temp, str);
	char** wordsArray = NULL;
	*pTotalLength = 0;

	word = strtok(temp, delimiters);
	while (word != NULL)
	{
		wordsArray = (char**)realloc(wordsArray,(count + 1)*sizeof(char*));
		if (!wordsArray)
			return 0;
		wordsArray[count] = getDynStr(word);
		count++;
		*pTotalLength += (int)strlen(word);
		word = strtok(NULL, delimiters);
	}
	*pCount = count;
	return wordsArray;
}

char* appendToString( char* str, const char* firstAppend, va_list list) {
	const char* strTemp;
	char* longStr = NULL;
	int combineLength = 0;

	strTemp = firstAppend;
	while (strTemp != NULL)
	{
		if (combineLength == 0)
		{
			combineLength = (int)strlen(str) + (int)strlen(strTemp) + 2;
			longStr = (char*)realloc(longStr, combineLength * sizeof(char));
			if (!longStr)
				return;

			strcpy(longStr, str);
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

	return longStr;
}

//================================= 

void generalArrayFunction(void* arr, int size, int typeSize, void(*func)(void* element))
{
	for (int i = 0; i < size; i++)
		func((char*)(arr)+i*typeSize);
}

//================================= print

void	printStr(const void* str)
{
	puts(str);
}

//================================= free

void	freePtr(void* str)
{
	free(str);
}
