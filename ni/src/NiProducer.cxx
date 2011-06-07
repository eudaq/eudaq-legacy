#include "NiController.hh"
#include "eudaq/Producer.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/Utils.hh"
#include "eudaq/Logger.hh"
#include "eudaq/OptionParser.hh"
#include "eudaq/counted_ptr.hh"

#include <stdio.h>
#include <stdlib.h>

using eudaq::RawDataEvent;

class NiProducer: public eudaq::Producer {
public:
	NiProducer(const std::string & runcontrol) :
		eudaq::Producer("NI", runcontrol), done(false), running(false), stopping(false) {

		configure = false;

		std::cout << "NI Producer was started successful " << std::endl;
	}
	void MainLoop() {
		//r=0; t=0;
		do {
			/*
			 * waiting when button was press
			 * if (!m_running) 			continue;
			 */
			if (!running) {
				//printf("next");
				eudaq::mSleep(1000);
				continue;
			}
			if (running) {
				eudaq::RawDataEvent ev("NI", m_run, ++m_ev);
				//t++;
				datalength1 = ni_control->DataTransportClientSocket_ReadLength("priv");
				Buffer1 = ni_control->DataTransportClientSocket_ReadData(datalength1);
				ev.AddBlock(0, Buffer1, datalength1);
				datalength2 = ni_control->DataTransportClientSocket_ReadLength("priv");
				Buffer2 = ni_control->DataTransportClientSocket_ReadData(datalength2);
				ev.AddBlock(1, Buffer2, datalength2);

				//if (t == 1000) {
				//	r++;
				//	printf("r = %d \n", r);
				//	t = 0;
				//}
				/*
				 *
				 *  D A T A    U N C O M P R E S S
				 *
				 */
/*
				for (int i = 0; i < 3500; i++)
					data[i] = 0;
				int i = 0;
				int k = 0;
				NumOfData = 0;
				while (i < datalength) {
					data[k] = 0xFF & Buffer[i + 1];
					data[k] <<= 8;
					data[k] += 0xFF & Buffer[i];
					//printf("w=0x%x ", data[k] );
					i = i + 2;
					k++;
				}
				//printf(" \n" );
				while (1) {
					if (NumOfData > ((datalength / 2) - 2))
						break;
					Header0 = data[NumOfData++];
					Header1 = data[NumOfData++];

					if ((Header0 == 0x5555) & (Header1 == 0x5551))		NumOfDetector = 1;
					if ((Header0 == 0x5555) & (Header1 == 0x5552))		NumOfDetector = 2;
					if ((Header0 == 0x5555) & (Header1 == 0x5553))		NumOfDetector = 3;
					if ((Header0 == 0x5555) & (Header1 == 0x5554))		NumOfDetector = 4;
					if ((Header0 == 0x5555) & (Header1 == 0x5555))		NumOfDetector = 5;
					if ((Header0 == 0x5555) & (Header1 == 0x5556))		NumOfDetector = 6;
					MIMOSA_Counter0 = data[NumOfData++];
					MIMOSA_Counter1 = data[NumOfData++];
					MIMOSA_Datalen0 = data[NumOfData++];
					MIMOSA_Datalen1 = data[NumOfData++];
					MIMOSA_Datalen = MIMOSA_Datalen0 + MIMOSA_Datalen1;
					printf("Header0=%x  ", Header0);
					printf("Header1=%x  ", Header1);
					printf("MIMOSA_Counter0=%x  ", MIMOSA_Counter0);
					printf("MIMOSA_Counter1=%x  ", MIMOSA_Counter1);
					printf("MIMOSA_Datalen0=%x  ", MIMOSA_Datalen0);
					printf("MIMOSA_Datalen1=%x  \n", MIMOSA_Datalen1);
					printf(" \n", Header1);
					/*
					 *
					 * R E A D   S T A T U S
					 *
					 */
/*
					NumOfLengt = 0;
					MIMOSA_DatalenTmp = NumOfData + MIMOSA_Datalen;
					while (1) {
						if (NumOfData > ((datalength / 2) - 2))
							break;
						if (NumOfData >= MIMOSA_DatalenTmp)
							break;
						StatusLine = data[NumOfData++];
						OVF = StatusLine >> 15;
						NumOfState = StatusLine & 0xF;
						AddLine = (StatusLine >> 4) & 0x7FF;
						printf("OVF=%d ", OVF);
						printf("AddLine=%d ", AddLine);
						printf("NumOfState=%d  \n", NumOfState);
						/*
						 * R E A D   S T A T E
						 */
						//*
/*
						for (int k = 0; k < NumOfState; k++) {
							if (NumOfData > ((datalength / 2) - 2))
								break;
							if (NumOfData >= MIMOSA_DatalenTmp)
								break;
							State = data[NumOfData++];
							AddColum = (State >> 2) & 0x7FF;
							HitPix = State & 0x3;
							printf("                              AddColum=%d ", AddColum);
							printf("HitPix=%d \n", HitPix);
						}
					}
					MIMOSA_Trailer0 = data[NumOfData++];
					MIMOSA_Trailer1 = data[NumOfData++];
					printf("Tr0=%x   ", MIMOSA_Trailer0);
					printf("Tr1=%x \n", MIMOSA_Trailer1);

				}
				//printf("NumOfData = %d  \n", NumOfData );
				//printf("t = %d \n", t);


				if (t == 8680) {
					r++;
					printf("r = %d \n", r);
					//printf(" Data=%d ", datalengthAll);
					datalengthAll = 0;
					t = 0;
				}
				datalength = 0;
*/
				SendEvent(ev);
				//eudaq::mSleep(1000);
			}
		} while (!done);
	}
	virtual void OnConfigure(const eudaq::Configuration & param) {

		unsigned char configur[5] = "conf";

		try {
			if (!configure){
				ni_control = new NiController();
				ni_control->GetProduserHostInfo();
				ni_control->ConfigClientSocket_Open();
				ni_control->DatatransportClientSocket_Open();
				std::cout << " " << std::endl;
				configure = true;
			}

			TriggerType = 	param.Get("TriggerType", 255);
			Det = 			param.Get("Det", 255);
			Mode = 			param.Get("Mode", 255);
			NiVersion = 	param.Get("NiVersion", 255);
			NumBoards = 	param.Get("NumBoards", 255);
			FPGADownload = 	param.Get("FPGADownload", 255);
			for(unsigned char i=0; i<6; i++){
				MimosaID[i] = param.Get("MimosaID_" + to_string(i+1), 255);
				MimosaEn[i] = param.Get("MimosaEn_" + to_string(i+1), 255);
			}
			OneFrame = param.Get("OneFrame", 255);

			std::cout << "...Configured (" << param.Name() << ")" << std::endl;

			//TriggerType = param.Get("TriggerType", 255);

			//EUDAQ_INFO("Configured (" ")");
			//EUDAQ_ERROR("Config exception: " );
			conf_parameters[0] = NiVersion;
			conf_parameters[1] = TriggerType;
			conf_parameters[2] = Det;
			conf_parameters[3] = MimosaEn[1];
			conf_parameters[4] = MimosaEn[2];
			conf_parameters[5] = MimosaEn[3];
			conf_parameters[6] = MimosaEn[4];
			conf_parameters[7] = MimosaEn[5];
			conf_parameters[8] = NumBoards;
			conf_parameters[9] = FPGADownload;
			for (int z=0; z<10; z++) printf("En[%d]=%d \n", z, conf_parameters[z]);
			for (int z=0; z<6; z++) printf("ID[%d]=%d \n", z, MimosaID[z]);

			ni_control->ConfigClientSocket_Send(configur, sizeof(configur) );
			ni_control->ConfigClientSocket_Send(conf_parameters, sizeof(conf_parameters) );

			std::cout << "... was Configured " << param.Name() << " " << std::endl;
			EUDAQ_INFO("Configured (" + param.Name() + ")");
			SetStatus(eudaq::Status::LVL_OK, "Configured (" + param.Name() + ")");
		} catch (const std::exception & e) {
			printf("Caught exception: %s\n", e.what());
			SetStatus(eudaq::Status::LVL_ERROR, "Configuration Error");
		} catch (...) {
			printf("Unknown exception\n");
			SetStatus(eudaq::Status::LVL_ERROR, "Configuration Error");
		}
	}
	virtual void OnStartRun(unsigned param) {
		try {
			m_run = param;
			m_ev = 0;
			std::cout << "Start Run: " << param << std::endl;

			eudaq::RawDataEvent ev(RawDataEvent::BORE("NI", m_run));

			ev.SetTag("DET", "MIMOSA26");
			ev.SetTag("MODE", "ZS2");
			ev.SetTag("BOARDS", NumBoards);
			for(unsigned char i=0; i<6; i++)
				ev.SetTag("ID" + to_string(i), to_string(MimosaID[i]) );
			for(unsigned char i=0; i<6; i++)
							ev.SetTag("MIMOSA_EN" + to_string(i), to_string(MimosaEn[i]) );
			SendEvent(ev);
			eudaq::mSleep(500);

			ni_control->Start();
			running = true;

			SetStatus(eudaq::Status::LVL_OK, "Started");
		} catch (const std::exception & e) {
			printf("Caught exception: %s\n", e.what());
			SetStatus(eudaq::Status::LVL_ERROR, "Start Error");
		} catch (...) {
			printf("Unknown exception\n");
			SetStatus(eudaq::Status::LVL_ERROR, "Start Error");
		}
	}
	virtual void OnStopRun() {
		try {
			std::cout << "Stop Run" << std::endl;

			ni_control->Stop();
			eudaq::mSleep(500);
			running = false;
			eudaq::mSleep(100);
			// Send an EORE after all the real events have been sent
			// You can also set tags on it (as with the BORE) if necessary
			SetStatus(eudaq::Status::LVL_OK, "Stopped");
			SendEvent(eudaq::RawDataEvent::EORE("NI", m_run, ++m_ev));

		} catch (const std::exception & e) {
			printf("Caught exception: %s\n", e.what());
			SetStatus(eudaq::Status::LVL_ERROR, "Stop Error");
		} catch (...) {
			printf("Unknown exception\n");
			SetStatus(eudaq::Status::LVL_ERROR, "Stop Error");
		}
	}
	virtual void OnTerminate() {
		std::cout << "Terminate (press enter)" << std::endl;
		done = true;
		ni_control->DatatransportClientSocket_Close();
		ni_control->ConfigClientSocket_Close();
		eudaq::mSleep(1000);
	}
private:
	unsigned m_run, m_ev;
	bool done, running, stopping,  configure;
	struct timeval tv;
	counted_ptr<NiController> ni_control;

	char *Buffer1;
	unsigned int datalength1;
	char *Buffer2;
	unsigned int datalength2;
	int r ;
	int t ;


	unsigned int Header0;
	unsigned int Header1;
	unsigned int MIMOSA_Counter0;
	unsigned int MIMOSA_Counter1;
	unsigned int MIMOSA_Datalen0;
	unsigned int MIMOSA_Datalen1;
	unsigned int MIMOSA_Datalen;
	unsigned int MIMOSA_Trailer0;
	unsigned int MIMOSA_Trailer1;
	unsigned int StatusLine;
	unsigned int OVF;
	unsigned int NumOfState;
	unsigned int AddLine;
	unsigned int State;
	unsigned int AddColum;
	unsigned int HitPix;

	unsigned int data[7000];
	unsigned char NumOfDetector;
	unsigned int NumOfData;
	unsigned int NumOfLengt;
	unsigned int MIMOSA_DatalenTmp;
	int datalengthAll;

	unsigned TriggerType;
	unsigned Det;
	unsigned Mode;
	unsigned NiVersion;
	unsigned NumBoards;
	unsigned FPGADownload;
	unsigned MimosaID[6];
	unsigned MimosaEn[6];
	bool OneFrame;

	unsigned char conf_parameters[10];

};
// ----------------------------------------------------------------------
int main(int /*argc*/, const char ** argv) {
	std::cout << "Start Producer \n" << std::endl;

	eudaq::OptionParser op("EUDAQ NI Producer", "0.1", "The Producer task for the NI crate");
	eudaq::Option<std::string> rctrl(op, "r", "runcontrol", "tcp://localhost:44000", "address", "The address of the RunControl application");
	eudaq::Option<std::string> level(op, "l", "log-level", "NONE", "level", "The minimum level for displaying log messages locally");
	eudaq::Option<std::string> name(op, "n", "name", "NI", "string", "The name of this Producer");
	try {
		op.Parse(argv);
		EUDAQ_LOG_LEVEL(level.Value());
		NiProducer producer(rctrl.Value());
		producer.MainLoop();
		eudaq::mSleep(500);
	} catch (...) {
		return op.HandleMainException();
	}
	return 0;
}
