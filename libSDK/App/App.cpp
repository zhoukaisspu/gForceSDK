// App.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"

#include <OYM_NIF.h>
#include <DiscoveryService.h>
#include "AdapterManager.h"

bool ctrlhandler( DWORD fdwctrltype )
{
    switch( fdwctrltype )
    {
	    // handle the ctrl-c signal.
	    case CTRL_C_EVENT:
	        printf( "ctrl-c event\n\n" );
	        return( true );
	    // ctrl-close: confirm that the user wants to exit.
	    case CTRL_CLOSE_EVENT:
	        printf( "ctrl-close event\n\n" );
	        return( false );
	    // pass other signals to the next handler.
	    case CTRL_BREAK_EVENT:
	        printf( "ctrl-break event\n\n" );
	        return false;
	    case CTRL_LOGOFF_EVENT:
	        printf( "ctrl-logoff event\n\n" );
	        return false;
	    case CTRL_SHUTDOWN_EVENT:
	        printf( "ctrl-shutdown event\n\n" );
	        return false;
	    default:
	        return false;
    }
}

int _tmain(int charc, char* argv[]) {
	OYM_STATUS status;
	OYM_UINT8 loop = 0;
	OYM_AdapterManager* am = new OYM_AdapterManager();

	status = am->Init();
	if (!OYM_SUCCEEDED)
	{
		return OYM_FAIL;
	}
#if 0
	status = am->StartScan();
	if (!OYM_SUCCEEDED)
	{
		return OYM_FAIL;
		printf("main thread! status = %d\n", status);
	}
#endif

	if( SetConsoleCtrlHandler( (PHANDLER_ROUTINE) ctrlhandler, true ) )
    {
        while( 1 )
		{ 
			Sleep(1000);
			loop++;
			printf("loop = %d\n", loop);
			if(loop == 5)
			{
				printf("Deinit \n");
				am->Deinit();
			}
		}
    }
	else
	{
    	printf( "\nerror: could not set control handler");
	}
	
	return 0;
}
