#define WIN32_LEAN_AND_MEAN		// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#include <DbgHelp.h>

class CMiniDump
{
public:
	static BOOL Begin(VOID);
	static BOOL End(VOID);
};