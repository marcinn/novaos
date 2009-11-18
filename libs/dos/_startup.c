#include <dos/boot.h>
#include <system.h>

struct SysBase _SysBase;
struct SysBase *SysBase = &_SysBase;

void _start()
{
    SysBase->sb_KrnlVTOP = peekl(LINEAR_DS,0x1000);
    SysBase->sb_KrnlPTOV = (0xC00000L - 0x10080L);
}
