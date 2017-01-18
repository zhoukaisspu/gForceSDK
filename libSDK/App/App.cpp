// App.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"

#include <OYM_NIF.h>
#include <DiscoveryService.h>
#include "AdapterManager.h"

int _tmain(int charc, char* argv[]) {
	OYM_STATUS status;

	OYM_AdapterManager* am = new OYM_AdapterManager();
	status = am->Init();
	if (!OYM_SUCCEEDED)
	{
		return OYM_FAIL;
	}

	status = am->StartScan();
	if (!OYM_SUCCEEDED)
	{
		return OYM_FAIL;
		printf("main thread! status = %d\n", status);
	}

	while (1) {
		Sleep(5000);
		printf("main thread!\n");
	};
}
