#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Supermarket.h"
#include "Product.h"
#include "Customer.h"
#include "Address.h"
#include "General.h"
#include "ShoppingCart.h"

int		initSuperMarket(SuperMarket* pMarket)
{
	pMarket->customerCount = 0;
	pMarket->customerArr = NULL;
	pMarket->productCount = 0;
	pMarket->productArr = NULL;
	pMarket->name = getStrExactLength("Enter market name");
	pMarket->sortType = eNoSort;
	return initAddress(&pMarket->location);
}

void	printSuperMarket(const SuperMarket* pMarket)
{
	printf("Super Market Name: %s\t", pMarket->name);
	printf("Address: ");
	printAddress(&pMarket->location);
	printf("\n");
	printAllProducts(pMarket);
	printf("\n");
	printAllCustomers(pMarket);
}

int		addProduct(SuperMarket* pMarket)
{
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = getProductFromUser(pMarket,barcode);
	if (pProd != NULL) //This barcode exist in stock
		updateProductCount(pProd);
	else { //new product to stock
		if(!addNewProduct(pMarket, barcode))
			return 0;
	}

	return 1;
}

int		addNewProduct(SuperMarket* pMarket,const char* barcode)
{
	pMarket->productArr = (Product**)realloc(pMarket->productArr, (pMarket->productCount + 1) * sizeof(Product*));
	if (!pMarket->productArr)
		return 0;
	
	Product* pProd = (Product*)calloc(1, sizeof(Product));
	if (!pProd)
	{
		free(pMarket->productArr);
		return 0;
	}

	strcpy(pProd->barcode, barcode);
	initProductNoBarcode(pProd);
	
	pMarket->productArr[pMarket->productCount] = pProd;
	pMarket->productCount++;
	return 1;
}

int		addCustomer(SuperMarket* pMarket)
{
	Customer cust;
	if (!initCustomer(&cust))
		return 0;

	if (isCustomerInMarket(pMarket, &cust))
	{
		printf("This customer already in market\n");
		free(cust.name);
		return 0;
	}

	pMarket->customerArr = (Customer*)realloc(pMarket->customerArr, (pMarket->customerCount + 1) * sizeof(Customer));
	if (!pMarket->customerArr)
	{
		free(cust.name);
		return 0;
	}
	pMarket->customerArr[pMarket->customerCount] = cust;
	pMarket->customerCount++;
	pMarket->sortType = eNoSort;
	return 1;
}

int		isCustomerInMarket(SuperMarket* pMarket, Customer* pCust)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (strcmp(pMarket->customerArr[i].name, pCust->name) == 0)
			return 1;
	}
	return 0;
}

int	doShopping(SuperMarket* pMarket)
{
	Customer* pCustomer = getCustomerShopPay(pMarket);
	if (!pCustomer)
		return 0;

	if (pCustomer->pCart == NULL)
	{
		pCustomer->pCart = (ShoppingCart*)malloc(sizeof(ShoppingCart));
		if (!pCustomer->pCart)
			return 0;
		initCart(pCustomer->pCart);
	}
	fillCart(pMarket, pCustomer->pCart);
	printf("---------- Shopping ended ----------\n");
	return 1;
}

Customer*	doPrintCart(SuperMarket* pMarket)
{
	Customer* pCustomer = getCustomerShopPay(pMarket);
	if (!pCustomer)
		return NULL;
	if (pCustomer->pCart == NULL)
	{
		printf("Customer cart is empty\n");
		return NULL;
	}
	printShoppingCart(pCustomer->pCart);
	return pCustomer;
}

int	doPayment(SuperMarket* pMarket)
{
	Customer* pCustomer = doPrintCart(pMarket);
	if(!pCustomer)
		return 0;
	pay(pCustomer);
	return 1;
}

Customer*	getCustomerShopPay(SuperMarket* pMarket)
{
	if (pMarket->customerCount == 0)
	{
		printf("No customer listed to market\n");
		return NULL;
	}

	if (pMarket->productCount == 0)
	{
		printf("No products in market - cannot shop\n");
		return NULL;
	}

	Customer* pCustomer = getCustomerWhoShop(pMarket);
	if (!pCustomer)
	{
		printf("this customer not listed\n");
		return NULL;
	}

	return pCustomer;
}

void	printAllProducts(const SuperMarket* pMarket)
{
	printf("There are %d products\n", pMarket->productCount);
	printf("%-20s %-10s\t", "Name", "Barcode");
	printf("%-20s %-10s %s\n", "Type", "Price", "Count In Stoke");
	printf("--------------------------------------------------------------------------------\n");

	for (int i = 0; i < pMarket->productCount; i++)
		printProduct(pMarket->productArr[i]);
}

void	printAllCustomers(const SuperMarket* pMarket)
{
	printf("There are %d listed customers\n", pMarket->customerCount);
	for (int i = 0; i < pMarket->customerCount; i++)
		printCustomer(&pMarket->customerArr[i]);
}

int writeProductArrToBFile(FILE* fileName, Product** proArr, int count)
{
	if (fwrite(&count, sizeof(int), 1, fileName) != 1)
	{
		return 0;
	}
	for (int i = 0; i < count; i++)
	{
		if (!writeProductInBinFile(fileName, proArr[i]))
		{
			return 0;
		}
	}
	return 1;
}

Product** readProductArrFromBFile(FILE* fileName, int* pCount)
{
	Product** arr = NULL;

	if (fread(pCount, sizeof(int), 1, fileName) != 1)
	{
		return NULL;
	}
	arr = (Product**)calloc(*pCount , sizeof(Product*));
	if (!arr)
	{
		return NULL;
	}
	
	for (int i = 0; i < *pCount; i++)
	{
		arr[i]= (Product*)malloc(sizeof(Product));
		if (!arr[i])
		{
			freeProductArr(arr, i);
			return NULL;
		}
		if (!readProductInBinFile(fileName,arr[i]))
		{
			freeProductArr(arr, i);
			free(arr);
			return NULL;
		}
	}
	return arr;
}

int writeCustomerArrToTxtFile(FILE* fileName, Customer* CustArr, int count)
{
	fprintf(fileName, "%d\n", count);
	for (int i = 0; i < count; i++)
		writeCustomerInTxtFile(fileName, &CustArr[i]);

	return 1;
}

Customer* readCustomerArrFromTxtFile(FILE* fileName, int* pCount)
{
	Customer* arr = NULL;
	if (fscanf(fileName, "%d", pCount) != 1)
	{
		return NULL;
	}
	arr = (Customer*)malloc(*pCount * sizeof(Customer));
	if (!arr)
	{
		return NULL;
	}
	for (int i = 0; i < *pCount; i++)
	{
		fgetc(fileName);
		if (!readCustomerInTxtFile(fileName, &arr[i]))
		{
			free(arr);
			return NULL;
		}
		
	}
	return arr;
}

int writeSuperMarketToBFile(const char* fileName, SuperMarket* pmarket)
{
	FILE* fp;
	fp = fopen(fileName, "wb");
	if (!fp)
		return 0;
	int len =(int) strlen(pmarket->name) + 1;
	if (fwrite(&len, sizeof(int), 1, fp) != 1)
	{
		fclose(fp);
		return 0;
	}
	if (fwrite(pmarket->name, sizeof(char), len, fp) != len)
	{
		fclose(fp);
		return 0;
	}
	if (writeAddressInBinFile(fp, &pmarket->location) == 0) 
	{
		fclose(fp);
		return 0;
	}
	if (writeProductArrToBFile(fp, pmarket->productArr, pmarket->productCount)==0)
	{
		fclose(fp);
			return 0;
	}
	fclose(fp);
	return 1;

}

int readSuperMarketToBFile(const char* fileName, SuperMarket* pmarket)
{
	int len;
	FILE* fp = fopen(fileName, "rb");
	if (!fp)
		return 0;
	if (fread(&len, sizeof(int), 1, fp) != 1)
	{
		fclose(fp);
		return 0;
	}
	pmarket->name = (char*)malloc(len * sizeof(char));
	if (!pmarket->name)
	{
		fclose(fp);
		return 0;
	}
	if (fread(pmarket->name, sizeof(char), len, fp) != len)
	{
		free(pmarket->name);
		fclose(fp);
		return 0;
	}

	if (readAddressInBinFile(fp, &pmarket->location) == 0)
	{
		free(pmarket->name);
		fclose(fp);
		return 0;
	}
	pmarket->productArr = readProductArrFromBFile(fp, &pmarket->productCount);
	if (!pmarket->productArr) 
	{
		free(pmarket->name);
		freeAddress(&pmarket->location);
		fclose(fp);
		return 0;
	}
	pmarket->sortType = eNoSort;
	fclose(fp);
	return 1;
}

int writeCustomersArrToTxtFile(const char* fileName, SuperMarket* pmarket)
{
	FILE* fp;
	fp = fopen(fileName, "w");
	if (!fp)
		return 0;
	if(writeCustomerArrToTxtFile(fp,pmarket->customerArr,pmarket->customerCount)==0)
	{
		fclose(fp);
		return 0;
	}
	fclose(fp);
	return 1;
}

int readCustomersArrToTxtFile(const char* fileName, SuperMarket* pmarket)
{
	FILE* fp = fopen(fileName, "r");
	if (!fp)
		return 0;
	pmarket->customerArr = readCustomerArrFromTxtFile(fp, &pmarket->customerCount);
	if (!pmarket->customerArr) 
	{
		fclose(fp);
		return 0;
	}
	fclose(fp);
	return 1;
}


Customer* getCustomerWhoShop(SuperMarket* pMarket)
{
	printAllCustomers(pMarket);
	char name[MAX_STR_LEN];
	getsStrFixSize(name, sizeof(name), "Who is shopping? Enter cutomer name\n");

	Customer* pCustomer = FindCustomerByName(pMarket, name);
	
	return pCustomer;
}


void fillCart(SuperMarket* pMarket,ShoppingCart* pCart)
{
	printAllProducts(pMarket);
	char op;
	while (1)
	{
		printf("Do you want to shop for a product? y/Y, anything else to exit!!\t");
		do {
			scanf("%c", &op);
		} while (isspace(op));
		getchar(); //clean the enter
		if (op != 'y' && op != 'Y')
			break;
		int count;
		Product* pProd = getProductAndCount(pMarket, &count);
		if(pProd)
		{
			if (!addItemToCart(pCart, pProd->barcode, pProd->price, count))
			{
				printf("Error adding item\n");
				return;
			}
			pProd->count -= count; //item bought!!!
		}
	}
}

Product* getProductAndCount(SuperMarket* pMarket, int* pCount)
{
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = getProductFromUser(pMarket, barcode);
	if (pProd == NULL)
	{
		printf("No such product\n");
		return NULL;
	} 
	
	if (pProd->count == 0)
	{
		printf("This product out of stock\n");
		return NULL;
	}
	
	int count;
	do {
		printf("How many items do you want? max %d\n", pProd->count);
		scanf("%d", &count);
	} while (count <= 0 || count > pProd->count);
	*pCount = count;
	return pProd;
}

void	printProductByType(SuperMarket* pMarket)
{
	if (pMarket->productCount == 0)
	{
		printf("No products in market\n");
		return;
	}
	eProductType	type = getProductType();
	int count = 0;
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (pMarket->productArr[i]->type == type)
		{
			count++;
			printProduct(pMarket->productArr[i]);
		}
	}
	if (count == 0)
		printf("There are no product of type %s in market %s\n", getProductTypeStr(type), pMarket->name);
}

Product* getProductFromUser(SuperMarket* pMarket, char* barcode)
{
	getBorcdeCode(barcode);
	return getProductByBarcode(pMarket, barcode);
}

void	freeMarket(SuperMarket* pMarket)
{
	free(pMarket->name);
	freeAddress(&pMarket->location);
	for (int i = 0; i < pMarket->productCount; i++)
	{
		freeProduct(pMarket->productArr[i]);
		free(pMarket->productArr[i]);
	}
	free(pMarket->productArr);
	

	for (int i = 0; i < pMarket->customerCount; i++)
		freeCustomer(&pMarket->customerArr[i]);
	free(pMarket->customerArr);
}

void	getUniquBarcode(char* barcode, SuperMarket* pMarket)
{
	int cont = 1;
	while (cont)
	{
		getBorcdeCode(barcode);
		int index = getProductIndexByBarcode(pMarket, barcode);
		if (index == -1)
			cont = 0;
		else
			printf("This product already in market\n");
	}
}

int getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (isProduct(pMarket->productArr[i], barcode))
			return i;
	}
	return -1;

}


Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (isProduct(pMarket->productArr[i], barcode))
			return pMarket->productArr[i];
	}
	return NULL;

}

Customer* FindCustomerByName(SuperMarket* pMarket, const char* name)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (isCustomer(&pMarket->customerArr[i], name))
			return &pMarket->customerArr[i];
	}
	return  NULL;
}
void freeProductArr(Product** arr, int size)
{
	for (int i = 0; i < size; i++)
		freeProduct(arr[i]);
}

//////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// Sorting ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
eSort getSortType()
{
	int option;
	printf("\n\n");
	do {
		printf("Please enter one of the following types\n");
		for (int i = 1; i < eNofSortType; i++)
			printf("%d for %s\n", i, typeSort[i]);
		scanf("%d", &option);
	} while (option < 1 || option >= eNofSortType);
	getchar();
	return (eSort)option;
}

void sortCustomers(SuperMarket* pShop)
{
	eSort sort = getSortType();

	if (sort == eName)
	{
		qsort(pShop->customerArr, pShop->customerCount, sizeof(Customer), compareCustomerByName);
	}
	else if (sort == eTimesSpended)
	{
		qsort(pShop->customerArr, pShop->customerCount, sizeof(Customer), compareCustomerByTimesPurchase);
	}
	else if (sort == eTotalSpend) {
		qsort(pShop->customerArr, pShop->customerCount, sizeof(Customer), compareCustomerByTotalSpend);
	}
	pShop->sortType = sort;
}

Customer* searchForCustomerBySort(SuperMarket* pMarket)
{
	Customer tempCustomer;
	Customer* tempResult;

	if (pMarket->sortType == eNoSort)
	{
		printf("Cannot use this type of sort, nothing is sorted.\n");
		return NULL;
	}
	if (pMarket->sortType == eName)
		tempResult = bsearchByName(&tempCustomer, pMarket->customerArr, pMarket->customerCount);
	else if (pMarket->sortType == eTimesSpended)
		tempResult = bsearchByTimes(&tempCustomer, pMarket->customerArr, pMarket->customerCount);
	else
		tempResult = bsearchByTotal(&tempCustomer, pMarket->customerArr, pMarket->customerCount);
	if (!tempResult)
	{
		printf("Did not find the customer.");
		return NULL;
	}
	printf("Customer found:\n");
	printCustomer(tempResult);
	return tempResult;
}

Customer* bsearchByName(Customer* pTemp, Customer* arr, int size)
{
	pTemp->name = getStrExactLength("Enter the customers name: (String)\n");
	return (Customer*)bsearch(pTemp, arr, size, sizeof(Customer), compareCustomerByName);
}
Customer* bsearchByTimes(Customer* pTemp, Customer* arr, int size)
{
	printf("Enter the amount of times the customer shopped: (Int)\n");
	scanf("%d", &pTemp->shopTimes);
	return (Customer*)bsearch(pTemp, arr, size, sizeof(Customer), compareCustomerByTimesPurchase);
}
Customer* bsearchByTotal(Customer* pTemp, Customer* arr, int size)
{
	printf("Enter the exact amount the customer spent: (Float)\n");
	scanf("%f", &pTemp->totalSpend);
	return (Customer*)bsearch(pTemp, arr, size, sizeof(Customer), compareCustomerByTotalSpend);
}
int compareCustomerByName(const void* customer1, const void* customer2)
{
	const Customer* cus1 = (const Customer*) customer1;
	const Customer* cus2 = (const Customer*) customer2;
	return strcmp(cus1->name, cus2->name);
}

int compareCustomerByTimesPurchase(const void* customer1, const void* customer2)
{
	const Customer* cus1 = (const Customer*)customer1;
	const Customer* cus2 = (const Customer*)customer2;
	if (cus1->shopTimes > cus2->shopTimes)
		return 1;
	else if (cus1->shopTimes < cus2->shopTimes)
		return -1;
	else
		return 0;
}

int compareCustomerByTotalSpend(const void* customer1, const void* customer2)
{
	const Customer* cus1 = (const Customer*)customer1;
	const Customer* cus2 = (const Customer*)customer2;
	if (cus1->totalSpend > cus2->totalSpend)
		return 1;
	else if (cus1->totalSpend < cus2->totalSpend)
		return -1;
	else
		return 0;
}