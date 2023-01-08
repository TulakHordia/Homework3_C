#pragma once
#include "Address.h"
#include "Customer.h"
#include "Product.h"
#include "ShoppingCart.h"
#include "listGen.h"

typedef enum { eNoSort, eName, eTimesSpended, eTotalSpend, eNofSortType } eSort;
static const char* typeSort[eNofSortType] = { "NotSorted" ,"Name", "Number of times Purchase", "Total spend" };

typedef struct
{
	char*		name;
	Address		location;
	Customer*	customerArr;
	int			customerCount;
	LIST		productList;

	//Product**	productArr;
	//int			productCount;

	eSort		sortType;
}SuperMarket;


int			initSuperMarket(SuperMarket* pMarket);
void		printSuperMarket(const SuperMarket* pMarket);
int			addProduct(SuperMarket* pMarket);
int			addNewProduct(SuperMarket* pMarket, const char* barcode);
int			addCustomer(SuperMarket* pMarket);
int			isCustomerInMarket(SuperMarket* pMarket, Customer* pCust);
int			doShopping(SuperMarket* pMarket);
Customer*	doPrintCart(SuperMarket* pMarket);
int			doPayment(SuperMarket* pMarket);
Customer*	getCustomerShopPay(SuperMarket* pMarket);
void		fillCart(SuperMarket* pMarket, ShoppingCart* pCart);

//Prints
void		printProductByType(SuperMarket* pMarket);
void		printAllProducts(const SuperMarket* pMarket);
void		printAllCustomers(const SuperMarket* pMarket);

int			getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode);
Product*	getProductByBarcode(SuperMarket* pMarket, const char* barcode);
Product*	getProductFromUser(SuperMarket* pMarket, char* barcode);
void		getUniquBarcode(char* barcode, SuperMarket* pMarket);
Customer*	FindCustomerByName(SuperMarket* pMarket, const char* name);
Customer*	getCustomerWhoShop(SuperMarket* pMarket);
Product*	getProductAndCount(SuperMarket* pMarket, int* pCount);

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
void		makeCustomersPay(SuperMarket* pMarket);
int			insertBarcodeToTheCorrectPlace(SuperMarket* pMarket, Product* product);
int			countProductsInList(const SuperMarket* pMarket);
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// 
// 
// 
//__________________________________________________________________________
int			writeProductArrToBFile(FILE* fileName, LIST* proArr, int count);
BOOL		readProductArrFromBFile(FILE* fileName, LIST* theList);
//--------------------------------------------------------------------------
int			writeCustomerArrToTxtFile(FILE* fileName, Customer* CustArr, int count);
Customer*	readCustomerArrFromTxtFile(FILE* fileName, int* pCount);
//____________________________________________________________________
int			writeSuperMarketToBFile(const char* fileName, SuperMarket* pmarket);
int			readSupermarketFromBinaryFile(const char* fileName, SuperMarket* pmarket);
//_______________________________________________________________________
int			writeCustomersArrToTxtFile(const char* fileName, SuperMarket* pmarket);
int			readCustomersFromTxtFile(const char* fileName, SuperMarket* pmarket);
//________________________________________________________________________

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
eSort		getSortType();
void		sortCustomers(SuperMarket* pShop);
int			compareCustomerByName(const void* customer1, const void* customer2);
int			compareCustomerByTimesPurchase(const void* customer1, const void* customer2);
int			compareCustomerByTotalSpend(const void* customer1, const void* customer2);
Customer*	searchForCustomerBySort(SuperMarket* pMarket);
Customer*	bsearchByName(Customer* Ptemp, Customer* arr, int size);
Customer*	bsearchByTimes(Customer* Ptemp, Customer* arr, int size);
Customer*	bsearchByTotal(Customer* Ptemp, Customer* arr, int size);
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

void		freeMarket(SuperMarket* pMarket);
void		freeProductArr(Product** arr, int size);

