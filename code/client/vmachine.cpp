// vmachine.cpp -- wrapper to fake virtual machine for client

#include "vmachine.h"
#include <windows.h>
#include <stdint.h>
#pragma warning (disable : 4514)
/*
==============================================================

VIRTUAL MACHINE

==============================================================
*/
intptr_t	VM_Call(intptr_t callnum, ... )
{
	return cgvm.entryPoint( (&callnum)[0], (&callnum)[1], (&callnum)[2], (&callnum)[3],
		(&callnum)[4], (&callnum)[5], (&callnum)[6], (&callnum)[7],
		(&callnum)[8],  (&callnum)[9] );
	
}

/*
============
VM_DllSyscall

we pass this to the cgame dll to call back into the client
============
*/
extern intptr_t CL_CgameSystemCalls(intptr_t*args );
extern int CL_UISystemCalls( int *args );

intptr_t VM_DllSyscall( intptr_t arg, ... ) {
//	return cgvm->systemCall( &arg );
	return CL_CgameSystemCalls( &arg );
}
