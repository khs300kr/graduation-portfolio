#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{ 
	CSystemClass* System; 
	bool result;
	
	// system 객체를 생성한다. 
	System = new CSystemClass; 
	if(!System) { return 0; } 
	
	// system 객체를 초기화하고 run을 호출한다. 
	result = System->Initialize(); 
	if(result) 
	{ 
		System->Run(); 
	} 
	
	// system객체를 종료하고 메모리를 반환한다 

	System->Shutdown(); 
	delete System; 
	System = 0; 


	return 0; 
}