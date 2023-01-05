#pragma once
#include "ShoppingCart.h"

typedef struct
{
	char*			name;
	int				shopTimes;
	float			totalSpend;
	ShoppingCart*	pCart;
}Customer;

int		initCustomer(Customer* pCustomer);
void	printCustomer(const Customer* pCustomer);

int		isCustomer(const Customer* pCust, const char* name);

void	pay(Customer* pCustomer);

void	freeCustomer(Customer* pCust);

void writeCustomerInTxtFile(FILE* file, Customer* pCust);
int readCustomerInTxtFile(FILE* file, Customer* pCust);

