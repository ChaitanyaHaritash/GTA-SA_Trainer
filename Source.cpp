#include "Windows.h"
#include <iostream>
#include "psapi.h"
#include <TlHelp32.h>
#include <tchar.h>
#include "error.h"
#pragma (lib , "psapi.lib")


using namespace std;

///////Code Starts Here//////////

//Win32Api Parameters Definations 
HANDLE openProc;
DWORD pid;
unsigned long n;


DWORD BaseAddr_CPed = 0x00B7CD98;	//Cped
DWORD BaseAddr_Money = 0x00B7CE50;	//Money
DWORD BaseAddr_Fat = 0x00B793D4;	//Fats
DWORD BaseAddr_HealthBar = 0x00B793E0;	//Health Bar
DWORD BaseAddr_Armour = 0x00969133;		//Armour
DWORD BaseAddr_WantedLevel = 0x00BAA420; //Wanted level
DWORD BaseAddr_WantedClear = 0x00969135; //Clear all Wanted Levels
uintptr_t offset_Health = 0x540;		//Offset Health

//Return Default Values 
DWORD least_money;
DWORD least_health;
float Default_HealthBar;
DWORD armour_status;
DWORD wanted_level;

//My Definations for values
int mymoney = 999999999;
int myhealth = 1;
float myfat = 2999.55;
float myhealthbar = 10000.000000;
int myclearwanted = 0;



//Get Image Base Address
DWORD dwGetModule(DWORD dwProcID, TCHAR* cModuleName) {

	DWORD dwModuleBase = 0;
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcID);

	MODULEENTRY32 Me32;
	Me32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hSnapShot, &Me32)) {
		do
		{
			if (_tcsicmp(Me32.szModule, cModuleName) == 0) {
				dwModuleBase = (DWORD)Me32.modBaseAddr;
				break;
			}
		} while (Module32Next(hSnapShot, &Me32));
	}

	CloseHandle(hSnapShot);

	return dwModuleBase;

}


//Increase Fat in Status helpful in some missons
int GetFat() {
	//ReadProcessMemory(openProc, LPVOID(BaseAddr_Health), &least_health, sizeof(least_health), &n);
	if (!WriteProcessMemory(openProc, LPVOID(BaseAddr_Fat), &myfat, sizeof(myfat), 0)) {
			printf("\nFailed To Change FATs Status\n");
	}
	printf("\nFAT is set to :%f ;) ", myfat);
	
	return 0;
}

//Armour (wont appear in player menu but its activated)
int GetArmour() {
	ReadProcessMemory(openProc, LPVOID(BaseAddr_Armour), &armour_status, sizeof(armour_status), &n);
	if (armour_status != 1) {
		if (!WriteProcessMemory(openProc, LPVOID(BaseAddr_Armour), &myhealth, sizeof(myhealth), 0)) {
			printf("\nFailed To Change Armour Status : %s\n", GetLastErrorStdStr().c_str());
		}
	}
	
	printf("\nArmour is set to :%d ;) ", armour_status);
	
	return 0;
}

//Get Money if less than $5000, CJ only help those who are lazy af to earn even $5000 lol jk
int GetMoney() {
	ReadProcessMemory(openProc, LPVOID(BaseAddr_Money), &least_money, sizeof(least_money), &n);
	if (least_money < 5000) {
		if (!WriteProcessMemory(openProc, LPVOID(BaseAddr_Money), &mymoney, sizeof(mymoney), 0)) {
			printf("\nFailed To Change Money Status\n");
		}
		printf("\nMoney is set to :%d ;) ", mymoney);
	}
	return 0;
}


//Health Bar Increase (you may need to die once to activate it next respawn)
int GetHealthBar() {
	ReadProcessMemory(openProc, LPVOID(BaseAddr_HealthBar), &Default_HealthBar, sizeof(Default_HealthBar), &n);
	//printf("%f", Default_HealthBar);
	if (Default_HealthBar <= 569.000000){
		if (!WriteProcessMemory(openProc, LPVOID(BaseAddr_HealthBar), &myhealthbar, sizeof(myhealthbar), 0)) {
			printf("\nFailed To Change Health Bar Status : %s\n", GetLastErrorStdStr().c_str());
		}
		else { printf("\nHealth Bar Status Changed to %f",myhealthbar); }
	}
	return 0;
}

// Wanted level - Clear
int WantedLevelClear() {
	ReadProcessMemory(openProc, LPVOID(BaseAddr_WantedLevel), &wanted_level, sizeof(wanted_level), &n);
	//printf("%d\n",wanted_level);
	if (wanted_level > 0) {
		if (!WriteProcessMemory(openProc, LPVOID(BaseAddr_WantedClear), &myclearwanted, sizeof(myclearwanted), 0)) {
			printf("\nFailed To Change Armour Status : %s\n", GetLastErrorStdStr().c_str());
			ExitProcess(0);
		}
	}
	return 0;
}

int main() {
	HWND handle = FindWindow(0, "GTA: San Andreas");
	if (handle == NULL) {
		printf("\n[-] Window is not Valid\n");
		ExitProcess(0);
	}
	GetWindowThreadProcessId(handle, &pid);
	openProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	GetMoney();
	GetFat();
	GetArmour();
	GetHealthBar();
	WantedLevelClear();
}