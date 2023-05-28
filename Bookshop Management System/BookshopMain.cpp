#include "BookshopUI.h"

int main()
{
	//a bookshopUI object call ui is instanstiated
	BookshopUI* ui{ new BookshopUI() };

	//this initialize the bookshop management system
	ui->startProgram();

	delete ui;

	return 0;
}