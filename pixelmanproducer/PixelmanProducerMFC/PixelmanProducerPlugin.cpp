#include "stdafx.h"
#include "../Pixelman_2008_03_30/_other_/headers/MpxPluginMgrApi.h"
#include "medipixChipId.h"
#include "PixelmanProducerMFCDlg.h"

#define PLUGIN_NAME "PixelmanProducerMFC"
#define WINVER 0x0500



//Variablendeklaration

//DEVID *mpxDevId;
DEVID devId;	
int mpxCount;
medipixChipId* mpxDevId;


//Prototypen für Funktionen
void DialogBoxInit(unsigned int par);
void DialogBoxDelete(CWnd* sender);
void MgrDeleteCallback(CBPARAM par);
int mpxCtrlLoadPixelsCfgAscii(DEVID devId, const char *maskBitFile,
						   const char *testBitFile, const char *thlFile,
						   const char *thhOrModeFile, BOOL loadDacs);
int mpxCtrlPerformFrameAcq(DEVID devId, int numberOfFrames, 
						   double timeOfEachAcq, u32 fileFlags,
						   const char *fileName);
int mpxCtrlGetFrame16(DEVID devId, i16 *buffer, u32 size,
							 u32 frameNumber);
int mpxCtrlTriggerType(DEVID devId, int trigger);




PLUGIN_INIT
{	
	//mgr->registerCallback(MPXMGR_NAME, MPXMGR_CB_EXIT, MgrDeleteCallback);    

	//char buffer[200];
	
		

	
	mgr->mpxCtrlGetFirstMpx( &devId, &mpxCount );
	if (mpxCount > 0)
	{	
		mpxDevId = new medipixChipId[mpxCount];
		mpxDevId[0].deviceId = devId;
		mpxDevId[0].chipNo = 0;
		mgr->mpxCtrlGetDevInfo(mpxDevId[0].deviceId, &(mpxDevId[0].deviceInfo));
		static int chipSize = mpxDevId[0].deviceInfo.rowLen * mpxDevId[0].deviceInfo.rowLen;
		mpxDevId[0].databuffer = new i16[chipSize];
		memset(mpxDevId[0].databuffer, 0, chipSize * sizeof(i16));
		mpxDevId[0].sizeOfDataBuffer = chipSize;
		
		int chipNo = 0;
		while(mgr->mpxCtrlGetNextMpx(&devId) == 0 || chipNo != mpxCount-1 )
		{	//static int chipSize = 0;
			++chipNo;
			mpxDevId[chipNo].deviceId = devId;
			mpxDevId[chipNo].chipNo = chipNo;
			mgr->mpxCtrlGetDevInfo(mpxDevId[chipNo].deviceId, &(mpxDevId[chipNo].deviceInfo));
			chipSize = mpxDevId[chipNo].deviceInfo.rowLen * mpxDevId[chipNo].deviceInfo.rowLen;
			
			//memset(mpxDevId[chipNo].databuffer, 0, sizeof(i16)*chipSize);
			mpxDevId[chipNo].databuffer = new i16[chipSize];
			memset(mpxDevId[chipNo].databuffer, 0, chipSize * sizeof(i16));
			mpxDevId[chipNo].sizeOfDataBuffer = chipSize;//(mpxDevId[chipNo].deviceInfo.rowLen)*(mpxDevId[chipNo].deviceInfo.rowLen);		
			
		}

		return mgr->addMenuItem(PLUGIN_NAME,"TimePixProducer","TimePixProducer", DialogBoxInit, 0, 0, 0);
	}
	else
	{
		MessageBox(0, "No chips detected", "Error", 0);
		return 1;
	}
}



void DialogBoxInit(unsigned int par)
{
	//MessageBox(NULL, "Hallo´", "Test", NULL);
	

	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	BOOL ret = FALSE;
	
	CPixelmanProducerMFCDlg* pMainWnd = new CPixelmanProducerMFCDlg(NULL);
	pMainWnd->DialogBoxDelete=&DialogBoxDelete;
	pMainWnd->mpxCount = mpxCount;
	pMainWnd->mpxDevId = mpxDevId;
	pMainWnd->mpxCtrlLoadPixelsCfgAscii = &mpxCtrlLoadPixelsCfgAscii;
	pMainWnd->mpxCtrlPerformFrameAcq = &mpxCtrlPerformFrameAcq;
	pMainWnd->mpxCtrlGetFrame16 = &mpxCtrlGetFrame16;
	pMainWnd->mpxCtrlTriggerType = &mpxCtrlTriggerType;
	ret = pMainWnd->Create(IDD_PIXELMANPRODUCERMFC_DIALOG, CWnd::GetDesktopWindow());
	pMainWnd->SetWindowText("Pixelman Eudaq-Producer");
	pMainWnd->ShowWindow(SW_SHOW);

}

void DialogBoxDelete(CWnd* sender)//,int * n_inited3)
{
	/*Test ob Zuordnung des Arrays mit Pointern auf die verschiedenen Instanzen
	des Plugins stimmt. Für jeden Chip muss eines vorhanden sein.
	char buffer[500];
	sprintf(buffer,"%i = %u ?", chipNo, pArrMainWnd[chipNo]->chipID);
	MessageBox(NULL, buffer, "Test",0);
	*/
	delete sender;
}

int mpxCtrlLoadPixelsCfgAscii(DEVID devId, const char *maskBitFile,
						   const char *testBitFile, const char *thlFile,
						   const char *thhOrModeFile, BOOL loadDacs)
{
	return mgr->mpxCtrlLoadPixelsCfgAscii( devId, maskBitFile, testBitFile, thlFile, thhOrModeFile, loadDacs);
}

int mpxCtrlPerformFrameAcq(DEVID devId, int numberOfFrames, 
						   double timeOfEachAcq, u32 fileFlags,
						   const char *fileName)
{
	return mgr->mpxCtrlPerformFrameAcq(devId, numberOfFrames, timeOfEachAcq, fileFlags, fileName);
}

/*void MgrDeleteCallback(CBPARAM par)
{
	//pMainWnd->DialogBoxDelete(pMainWnd);
	//delete *pMainWnd;
}
*/

int mpxCtrlGetFrame16(DEVID devId, i16 *buffer, u32 size,
							 u32 frameNumber)
{
	return mgr->mpxCtrlGetFrame16(devId, buffer, size, frameNumber);
}

int mpxCtrlTriggerType(DEVID devId, int trigger)
{
	return mgr->mpxCtrlTrigger(devId, trigger);
}