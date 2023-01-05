#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Customer.h"
#include "General.h"

int	initCustomer(Customer* pCustomer)
{

	do {
		pCustomer->name = getStrExactLength("Enter customer name\n");
		if (!pCustomer->name)
			return 0;
	} while (checkEmptyString(pCustomer->name));
	pCustomer->shopTimes = 0;
	pCustomer->totalSpend = 0;
	pCustomer->pCart = NULL;
	return 1;
}

void	printCustomer(const Customer* pCustomer)
{
	printf("Name: %s  shop %d Times and total Spend : %6.3f     \t", pCustomer->name,pCustomer->shopTimes,pCustomer->totalSpend);
	
	if (pCustomer->pCart == NULL)
		printf("Shopping cart is empty!\n");
	else 
	{
		printf("Doing shopping now!!!\n");
		//printShoppingCart(pCustomer->pCart);
	}
}


void	pay(Customer* pCustomer)
{
	if (!pCustomer->pCart)
		return;
	printf("---------- Cart info and bill for %s ----------\n", pCustomer->name);
	printShoppingCart(pCustomer->pCart);
	pCustomer->shopTimes = pCustomer->shopTimes + 1;
	pCustomer->totalSpend = pCustomer->totalSpend + getTotalPrice(pCustomer->pCart);
	printf("!!! --- Payment was recived!!!! --- \n");
	freeShoppingCart(pCustomer->pCart);
	free(pCustomer->pCart);
	pCustomer->pCart = NULL;
}




int isCustomer(const Customer* pCust, const char* name)
{
	if (strcmp(pCust->name, name) == 0)
		return 1;
	return 0;
}

void	freeCustomer(Customer* pCust)
{
	if (pCust->pCart)
		pay(pCust); //will free every thing
	free(pCust->name);
}

void writeCustomerInTxtFile(FILE* file, Customer* pCust)
{
	fprintf(file, "%s\n%d %f\n", pCust->name,pCust->shopTimes ,pCust->totalSpend);
}

int readCustomerInTxtFile(FILE* file, Customer* pCust)
{
	char temp[MAX_STR_LEN];
	fgets(temp, MAX_STR_LEN, file);
	temp[strlen(temp) - 1] = '\0';
	pCust->name = _strdup(temp);
	if (!pCust->name)
		return 0;
	
	if (fscanf(file,"%d %f", &pCust->shopTimes, &pCust->totalSpend) != 2) 
	{
		free(pCust->name);
		return 0;
	}
	pCust->pCart = NULL;
	return 1;
}
