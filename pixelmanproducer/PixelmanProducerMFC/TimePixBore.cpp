#include "stdafx.h"
#include "TimePixBore.h"
#include "medipixChipId.h"

TimePixBore::TimePixBore(DevInfo *param, double timeToEndOfShutter, DACTYPE *dacVals)
{
	version = VERSION;
	m_rowLen = param->rowLen;                  
	m_numberOfRows = param->numberOfRows;
	m_mpxType = param->mpxType;                       
	m_chipboardID[MPX_MAX_CHBID] = param->chipboardID[MPX_MAX_CHBID];   
	m_ifaceName = param->ifaceName;  
	m_clockTimepix = param->clockTimepix;	
	m_timeToEndOfShutter = timeToEndOfShutter;
	m_dacVals = dacVals;
	

}