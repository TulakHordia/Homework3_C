#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Address.h"
#include "General.h"

#define ELEMENT_SEP "#"
#define WORD_SEP "  "

int	initAddress(Address* pAd)
{
	char allData[MAX_STR_LEN];
	char** elements=NULL;
	int count;
	int ok = 1;
	int totalLength;
	char msg[MAX_STR_LEN];
	sprintf(msg, "Enter address data\nFormat: street%shouse number%scity\nstreet and city can have spaces\n", 
			ELEMENT_SEP, ELEMENT_SEP);
	do {

		getsStrFixSize(allData, sizeof(allData), msg);
		int countSep = countCharInString(allData, ELEMENT_SEP[0]);
		if (countSep > 2)
		{
			printf("Too many separators in address\n");
			ok = 0;
		} else {
			elements = splitCharsToWords(allData, &count, ELEMENT_SEP, &totalLength);
			if (!elements)
				continue;
			ok = checkElements(elements, count);
			if (!ok)
			{
				printf("!!!incorrect address format!!!\n");
				freeElements(elements, count);
			}
		}
	} while (!ok);

	//get the house number
	sscanf(elements[1], "%d", &pAd->num);
	pAd->street = fixAddressParam(elements[0]);
	if (!pAd->street)
	{
		freeElements(elements, count);
		return 0;
	}

	pAd->city = fixAddressParam(elements[2]);
	if (!pAd->city)
	{
		free(pAd->street);
		freeElements(elements, count);
		return 0;
	}

	freeElements(elements, count);
	return 1;
}

char*	fixAddressParam(char* param)
{
	char* fixParamStr;
	char** wordsArray = NULL;
	int totalLength;
	int count;

	wordsArray = splitCharsToWords(param, &count," ", &totalLength);
	if (!wordsArray)
		return NULL;
	//add size for the WORD_SEP between words and for '\0'
	size_t length = (count - 1) * strlen(WORD_SEP) + 1;
	fixParamStr = (char*)calloc(totalLength + length, sizeof(char));
	if (!fixParamStr)
	{
		freeElements(wordsArray, count);
		return NULL;
	}

	if(count == 1)
		wordsArray[0][0] = toupper(wordsArray[0][0]); //upper
	else {
		for (int i = 0; i < count - 1; i++)
		{
			wordsArray[i][0] = toupper(wordsArray[i][0]); //upper
		}
		wordsArray[count - 1][0] = tolower(wordsArray[count - 1][0]); //lower
	}
	for (int i = 0; i < count - 1; i++)
	{
		strcat(fixParamStr, wordsArray[i]);
		strcat(fixParamStr, WORD_SEP);
	}
	strcat(fixParamStr, wordsArray[count - 1]);

	freeElements(wordsArray, count);
	return fixParamStr;
}


int checkElements(char**  elements, int count)
{
	if (count != 3)
		return 0;
	//check second word is a number;
	char* streetNum = elements[1];
	while (*streetNum)
	{
		if (!isdigit(*streetNum))
			return 0;
		streetNum++;
	}

	if (checkEmptyString(elements[0]))
		return 0;
	if (checkEmptyString(elements[2]))
		return 0;

	return 1;
}

void freeElements(char**  elements, int count)
{
	for (int i = 0; i < count; i++)
		free(elements[i]);
	free(elements);
}

int writeAddressInBinFile(FILE* file, Address* pAdd)
{
	if (fwrite(&pAdd->num, sizeof(int), 1, file) != 1)
		return 0;
	int len = (int)strlen(pAdd->street) + 1;
	if (fwrite(&len, sizeof(int), 1, file) != 1)
		return 0;
	if (fwrite(pAdd->street, sizeof(char), len, file) != len)
		return 0;

	len = (int)strlen(pAdd->city) + 1;

	if (fwrite(&len, sizeof(int), 1, file) != 1)
	{
		return 0;
	}

	if (fwrite(pAdd->city, sizeof(char), len, file) != len)
	{
		return 0;
	}

	return 1;
}

int readAddressInBinFile(FILE* file, Address* pAdd)
{
	int len;
	if (fread(&pAdd->num, sizeof(int), 1, file) != 1)
		return 0;
	if (fread(&len, sizeof(int), 1, file) != 1)
		return 0;
	pAdd->street = (char*)malloc(len*sizeof(char));
	if (!pAdd->street) 
	{
		return 0;
	}
	if (fread(pAdd->street ,sizeof(char), len, file) != len)
	{
		free(pAdd->street);
		return 0;
	}
	if (fread(&len, sizeof(int), 1, file) != 1)
	{
		free(pAdd->street);
		return 0;
	}
	pAdd->city = (char*)malloc(len * sizeof(char));
	if (!pAdd->city)
	{
		return 0;
	}
	if (fread(pAdd->city, sizeof(char), len, file) != len)
	{
		free(pAdd->street);
		free(pAdd->city);
		return 0;
	}
	return 1;
}

void printAddress(const Address* pAd)
{
	printf("%s %d, %s\n", pAd->street, pAd->num, pAd->city);
}

void	freeAddress(Address* pAd)
{
	free(pAd->city);
	free(pAd->street);
}