#pragma once

#define HIGH 1
#define LOW 0
typedef int parPort;


class TimePixDAQStatus
{
public:
	TimePixDAQStatus(parPort port);
	virtual ~TimePixDAQStatus();

	int parPortAddress;
	int parPortStatusReg;
	int parPortControlReg;
	
	void parPortSetBusyLineHigh();
	void parPortSetBusyLineLow();
	int parPortCheckBusyLine();

	void parPortUpdateAddress(parPort port);

	int parPortCheckTriggerLine();

protected:
	int _Inp32(short par); //wraps the Inp32 function from
							//inpout32.dll for error handling

};