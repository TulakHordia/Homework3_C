#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "General.h"
#include "Supermarket.h"

#define BIN_FILE_NAME "SuperMarket.bin"
#define Text_FILE_NAME "Customers.txt"

int menu();

const char* menuStrings[eNofOptions] = { "Show Supermarket.", "Add Product.",
								"Add Customer.", "Customer Shopping.","Print Shopping Cart.","Customer Pay.",
								"Print product by type.","Search customer by type.","Sort the customers." };

int main()
{
	SuperMarket	market;
	int success = 1;
	printf("try to load the files\n");
	if (readSuperMarketToBFile(BIN_FILE_NAME, &market) == 0)
	{
		success = 0;
	}
	printAllProducts(&market);
	if (readCustomersArrToTxtFile(Text_FILE_NAME, &market)==0)
	{
		success = 0;
	}
	if (success == 0)
	{
		if (!initSuperMarket(&market))
		{
			printf("error init  Super Market");
			return 0;
		}
	}
	int option;
	int stop = 0;
	
	do
	{
		option = menu();
		switch (option)
		{
		case eShowSuperMarket:
			printSuperMarket(&market);
			break;

		case eAddProduct:
			if (!addProduct(&market))
				printf("Error adding product\n");
			break;

		case eAddCustomer:
			if (!addCustomer(&market))
				printf("Error adding customer\n");
			break;

		case eCustomerDoShopping:
			if(!doShopping(&market))
				printf("Error in shopping\n");
			break;
		case ePrintCart:
			doPrintCart(&market);
			break;

		case eCustomerPay:
			if(!doPayment(&market))
				printf("Error in payment\n");
			break;

		case ePrintProductByType:
			printProductByType(&market);
			break;

		case eSearchCustomer:
			searchForCustomerBySort(&market);
			break;

		case eSortCustomers:
			sortCustomers(&market);
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

	if (writeSuperMarketToBFile(BIN_FILE_NAME,&market) == 0)
	{
		printf("Failed saving to binary!");
	 }
	if (writeCustomersArrToTxtFile(Text_FILE_NAME, &market) == 0)
	{
		printf("Failed saving to txt!");
	}
	freeMarket(&market);
	
	system("pause");
	return 1;
}

int menu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for(int i = 0 ; i < eNofOptions ; i++)
		printf("%d - %s\n",i, menuStrings[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

