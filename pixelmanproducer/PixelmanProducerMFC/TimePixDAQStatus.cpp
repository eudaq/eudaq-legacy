#include "afxwin.h"
#include "TimePixDAQStatus.h"

short _stdcall Inp32(short PortAddress);
void _stdcall Out32(short PortAddress, short data);

TimePixDAQStatus::TimePixDAQStatus(parPort port)
{
	parPortAddress = port;
	parPortStatusReg = parPortAddress+1;
	parPortControlReg = parPortAddress+2;
}

TimePixDAQStatus::~TimePixDAQStatus()
{
		
}

void TimePixDAQStatus::parPortSetBusyLineHigh()
{
	int controlRegister = _Inp32( parPortControlReg );
	//int new_register = 0;
	controlRegister |= 0x01;
	
	Out32(parPortControlReg, controlRegister);
}

void TimePixDAQStatus::parPortSetBusyLineLow()
{
	int controlRegister = _Inp32( parPortControlReg );
	//int new_register = 0;		
	controlRegister &= 0xFE;
	
	Out32(parPortControlReg, controlRegister);
	
}

//checks Status of Trigger Line, 1 is active
int TimePixDAQStatus::parPortCheckBusyLine()
{
	int controlRegister = _Inp32( parPortControlReg );

	if (controlRegister & 0x01)
		return 1;
	else
		return 0;
}

int TimePixDAQStatus::parPortCheckTriggerLine()
{
	int statusRegister = _Inp32( parPortStatusReg );
	
	if (statusRegister & 0x80)
		return 1;
	else return 0;
	
}


void TimePixDAQStatus::parPortUpdateAddress(parPort port)
{
	parPortAddress = port;
	parPortStatusReg = parPortAddress+1;
	parPortControlReg = parPortAddress+2;
}

int TimePixDAQStatus::_Inp32(short par)
{	static int retval;
	retval = Inp32(par);
	if(retval == 0xFF)
		return AfxMessageBox("Please enter valid Port Address", MB_ICONERROR, 0);//MessageBox(NULL, "Please enter valid Port Address", "I/O Error", MB_ICONERROR);
	else
		return retval;
}