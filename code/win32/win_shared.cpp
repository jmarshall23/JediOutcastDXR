// leave this as first line for PCH reasons...
//
#include "../server/exe_headers.h"

#include "../game/q_shared.h"
#include "../qcommon/qcommon.h"
#include "win_local.h"

#include <intrin.h>
#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>

/*
================
Sys_Milliseconds
================
*/
int Sys_Milliseconds(void)
{
    static int sys_timeBase = timeGetTime();
    int sys_curtime = timeGetTime() - sys_timeBase;
    return sys_curtime;
}

/*
** --------------------------------------------------------------------------------
**
** PROCESSOR STUFF
**
** --------------------------------------------------------------------------------
*/
static void CPUID(int func, unsigned regs[4])
{
    int cpuInfo[4];
    __cpuid(cpuInfo, func);
    regs[0] = (unsigned)cpuInfo[0];
    regs[1] = (unsigned)cpuInfo[1];
    regs[2] = (unsigned)cpuInfo[2];
    regs[3] = (unsigned)cpuInfo[3];
}

static int IsPentium(void)
{
#if defined(_M_IX86) || defined(_M_X64)
    // Pure intrinsic code cannot reproduce the old EFLAGS.ID-bit toggle test.
    // On supported MSVC Windows targets, assuming CPUID exists is reasonable.
    return qtrue;
#else
    return qfalse;
#endif
}

static int Is3DNOW(void)
{
    unsigned regs[4];

    // Check max extended CPUID leaf.
    CPUID(0x80000000, regs);
    if (regs[0] < 0x80000001)
        return qfalse;

    // Bit 31 of EDX = 3DNow!
    CPUID(0x80000001, regs);
    return (regs[3] & (1u << 31)) ? qtrue : qfalse;
}

static int IsKNI(void)
{
    unsigned regs[4];

    // KNI = SSE = bit 25 of EDX in CPUID leaf 1
    CPUID(1, regs);
    if ((regs[3] & (1u << 25)) == 0)
        return qfalse;

#if defined(_M_X64)
    // x64 Windows guarantees SSE/SSE2 OS support.
    return qtrue;
#else
    // On old 32-bit systems, probe an SSE instruction under SEH.
    __try
    {
        __m128 z = _mm_setzero_ps();
        (void)z;
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        return qfalse;
    }

    return qtrue;
#endif
}

static int IsWIL(void)
{
    unsigned regs[4];

    // WIL = Willamette = SSE2 = bit 26 of EDX in CPUID leaf 1
    CPUID(1, regs);
    if ((regs[3] & (1u << 26)) == 0)
        return qfalse;

#if defined(_M_X64)
    // x64 Windows guarantees SSE2 OS support.
    return qtrue;
#else
    // On old 32-bit systems, probe an SSE2 instruction under SEH.
    __try
    {
        __m128d z = _mm_setzero_pd();
        (void)z;
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        return qfalse;
    }

    return qtrue;
#endif
}

static int IsMMX(void)
{
    unsigned regs[4];

    // Bit 23 of EDX = MMX
    CPUID(1, regs);
    return (regs[3] & (1u << 23)) ? qtrue : qfalse;
}

int Sys_GetProcessorId(void)
{
#if defined _M_ALPHA
    return CPUID_AXP;
#elif !defined ID_PC
    return CPUID_GENERIC;
#else

    if (!IsPentium())
        return CPUID_INTEL_UNSUPPORTED;

    if (!IsMMX())
        return CPUID_INTEL_PENTIUM;

    if (Is3DNOW())
        return CPUID_AMD_3DNOW;

    if (IsKNI())
    {
        if (IsWIL())
            return CPUID_INTEL_WILLIAMETTE;

        return CPUID_INTEL_KATMAI;
    }

    return CPUID_INTEL_MMX;

#endif
}

//============================================

char* Sys_GetCurrentUser(void)
{
    static char s_userName[1024];
    unsigned long size = sizeof(s_userName);

    if (!GetUserName(s_userName, &size))
        strcpy(s_userName, "player");

    if (!s_userName[0])
        strcpy(s_userName, "player");

    return s_userName;
}