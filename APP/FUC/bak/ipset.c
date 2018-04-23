
#include "mainmenu.h"
#include "sys.h"


void App_SETAPP(void) 
{
  appxsel =0 , appysel=0;
	
	
  hWinSetapp = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, WM_HBKWIN, 0, 0);
	hWin_msgto   = hWinSetapp;
}


