#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Product.h"
#include "General.h"

#define MIN_DIG 3
#define MAX_DIG 5

void	initProduct(Product* pProduct)
{
	initProductNoBarcode(pProduct);
	getBorcdeCode(pProduct->barcode);
}

void	initProductNoBarcode(Product* pProduct)
{
	initProductName(pProduct);
	pProduct->type = getProductType();
	pProduct->price = getPositiveFloat("Enter product price\t");
	pProduct->count = getPositiveInt("Enter product number of items\t");
}

void initProductName(Product* pProduct)
{
	do {
		printf("enter product name up to %d chars\n", NAME_LENGTH);
		myGets(pProduct->name, sizeof(pProduct->name));
	} while (checkEmptyString(pProduct->name));
}

void	printProduct(const Product* pProduct)
{
	printf("%-20s %-10s\t", pProduct->name, pProduct->barcode);
	printf("%-20s %5.2f %10d\n", typeStr[pProduct->type], pProduct->price, pProduct->count);
}

void getBorcdeCode(char* code)
{
	char temp[MAX_STR_LEN];
	char msg[MAX_STR_LEN];
	sprintf(msg,"Code should be of %d length exactly\n"
				"UPPER CASE letter and digits\n"
				"Must have %d to %d digits\n"
				"First and last chars must be UPPER CASE letter\n"
				"For example A12B40C\n",
				BARCODE_LENGTH, MIN_DIG, MAX_DIG);
	int ok = 1;
	int digCount = 0;
	do {
		ok = 1;
		digCount = 0;
		printf("Enter product barcode "); 
		getsStrFixSize(temp, MAX_STR_LEN, msg);
		if (strlen(temp) != BARCODE_LENGTH)
		{
			puts(msg);
			ok = 0;
		}
		else {
			//check and first upper letters
			if(!isupper(temp[0]) || !isupper(temp[BARCODE_LENGTH-1]))
			{
				puts("First and last must be upper case letters\n");
				ok = 0;
			} else {
				for (int i = 1; i < BARCODE_LENGTH - 1; i++)
				{
					if (!isupper(temp[i]) && !isdigit(temp[i]))
					{
						puts("Only upper letters and digits\n");
						ok = 0;
						break;
					}
					if (isdigit(temp[i]))
						digCount++;
				}
				if (digCount < MIN_DIG || digCount > MAX_DIG)
				{
					puts("Incorrect number of digits\n");
					ok = 0;
				}
			}
		}
		
	} while (!ok);

	strcpy(code, temp);
}


eProductType getProductType()
{
	int option;
	printf("\n\n");
	do {
		printf("Please enter one of the following types\n");
		for (int i = 0; i < eNofProductType; i++)
			printf("%d for %s\n", i, typeStr[i]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNofProductType);
	getchar();
	return (eProductType)option;
}

const char* getProductTypeStr(eProductType type)
{
	if (type < 0 || type >= eNofProductType)
		return NULL;
	return typeStr[type];
}

int		isProduct(const Product* pProduct, const char* barcode)
{
	if (strcmp(pProduct->barcode, barcode) == 0)
		return 1;
	return 0;
}

void	updateProductCount(Product* pProduct)
{
	int count;
	do {
		printf("How many items to add to stock?");
		scanf("%d", &count);
	} while (count < 1);
	pProduct->count += count;
}


void	freeProduct(Product* pProduct)
{
	//nothing to free!!!!
}

int writeProductInBinFile(FILE* file, Product* prod)
{
	/*int len = (int)strlen(prod->barcode) + 1;
	if (fwrite(&len, sizeof(int), 1, file) != 1)
		return 0;
	if (fwrite(prod->barcode, sizeof(char), len, file) !=len)
		return 0;
	len = (int)strlen(prod->name) + 1;
	if (fwrite(&len, sizeof(int), 1, file) != 1)
		return 0;
	if (fwrite(prod->name, sizeof(char), len, file) != len)
		return 0;
	if (fwrite(&prod->type, sizeof(eProductType), 1, file) != 1)
		return 0;
	if (fwrite(&prod->price, sizeof(float), 1, file) != 1)
		return 0;
	if (fwrite(&prod->count, sizeof(int), 1, file) != 1)
		return 0;*/
	if (fwrite(prod, sizeof(Product), 1, file) != 1)
		return 0;
	return 1;
}

int readProductInBinFile(FILE* file, Product* prod)
{
	/*int len;
	if (fread(&len, sizeof(int), 1, file) != 1)
		return 0;
	if (fread(prod->barcode, sizeof(char), len, file) != len) 
	{
		return 0;
	}
	if (fread(&len, sizeof(int), 1, file) != 1)
		return 0;
	if (fread(prod->name, sizeof(char), len, file) != len)
	{
		return 0;
	}
	if (fread(&prod->type, sizeof(eProductType), 1, file) != 1)
		return 0;
	if (fread(&prod->price, sizeof(float), 1, file) != 1)
		return 0;
	if (fread(&prod->count, sizeof(int), 1, file) != 1)
		return 0;*/
	
	if (fread(prod, sizeof(Product), 1, file) != 1)
		return 0;

	return 1;
}
