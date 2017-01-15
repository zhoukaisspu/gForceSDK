// App.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"

#include <OYM_NIF.h>
#include <DiscoveryService.h>
#include "AdapterManager.h"

int _tmain(int charc, char* argv[]) {
	OYM_STATUS status;

	OYM_NPI_Interface NPI_intface;
	status = NPI_intface.Init();
	if (status != OYM_SUCCESS)
	{
		printf("NPI_intface error!");
		return 1;
	}

	OYM_AdapterManager am(&NPI_intface);
	am.Init();

	while (1) {
		Sleep(5000);
		printf("main thread!\n");
	};
}
