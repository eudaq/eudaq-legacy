//---------------------------------------------------------------------------

#ifndef MI26ClientUnitH
#define MI26ClientUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include "MI26LIB_TLB.h"
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFormMI26Client : public TForm
{
__published:	// Composants gérés par l'EDI
        TEdit *Edit1;
        TButton *ButtonInfo;
        TButton *ButtonLoadFile;
        TEdit *EditLoadFileName;
        TButton *ButtonSaveFile;
        TEdit *EditSaveFileName;
        TButton *ButtonReset;
        TButton *ButtonStart;
        TButton *ButtonRead;
        TButton *ButtonUpdateBias;
        TButton *ButtonUpdateAll;
        TMemo *Memo1;
        TButton *ButtonFileLoadDialog;
        TOpenDialog *OpenDialog;
        TSaveDialog *SaveDialog;
        TButton *ButtonFileSaveDialog;
        TEdit *EditParallelPortAddr;
        TLabel *LabelMasterConfLoadFileName;
        TLabel *LabelMasterConfSaveFileName;
        TLabel *LabelMasterConfParallelPortAddr;
        TButton *ButtonPortParallelAddrUpdate;
        TLabel *LabelBiasReg;
        TEdit *EditBiasRegNum;
        TEdit *EditBiasRegValue;
        TButton *ButtonBiasRegWrite;
        TLabel *LabelMasterConfBiasRegNum;
        TLabel *LabelMasterConfBiasRegValue;
        TButton *ButtonRegRead;
        TLabel *LabelRomode;
        TEdit *EditRomodeRegNum;
        TEdit *EditRomodeRegValue;
        TButton *ButtonRomodeRegWrite;
        TButton *ButtonRomodeRegRead;
        TLabel *LabelDiscri;
        TEdit *EditDiscriRegNum;
        TEdit *EditDiscriRegValue;
        TButton *ButtonDiscriRegWrite;
        TButton *ButtonDiscriRegRead;
        TLabel *LabelLinePat0;
        TEdit *EditLinePat0RegNum;
        TEdit *EditLinePat0RegValue;
        TButton *ButtonLinePat0RegWrite;
        TButton *ButtonPat0RegRead;
        TLabel *LabelLinePat1;
        TEdit *EditLinePat1RegNum;
        TEdit *EditLinePat1RegValue;
        TButton *ButtonLinePat1RegWrite;
        TButton *ButtonLinePat1RegRead;
        TLabel *LabelCtrlPix;
        TEdit *EditCtrlPixRegNum;
        TEdit *EditCtrlPixRegValue;
        TButton *ButtonCtrlRegWrite;
        TButton *ButtonCtrlRegRead;
        TEdit *EditBiasRegValueRb;
        TEdit *EditRomodeRegValueRb;
        TEdit *EditDiscriRegValueRb;
        TEdit *EditLinePat0RegValueRb;
        TEdit *EditLinePat1RegValueRb;
        TEdit *EditCtrlPixRegValueRb;
        TEdit *EditParallelPortAddrRb;
        TLabel *LabelMasterConfTest1Pad;
        TEdit *EditTest1PadRegValue;
        TButton *ButtonTest1PadRegWrite;
        TButton *ButtonTest1PadRegRead;
        TEdit *EditTest1PadRegValueRb;
        TLabel *LabelMasterConfTest2Pad;
        TEdit *EditTest2PadRegValue;
        TButton *ButtonTest2PadRegWrite;
        TButton *ButtonTest2PadRegRead;
        TEdit *EditTest2PadRegValueRb;
        TLabel *LabelDevNum;
        TEdit *EditDevNum;
        TButton *ButtonDevNumSelect;
        TEdit *EditDevNumRb;
        TButton *ButtonDevNumRead;
        TLabel *LabelConnectStatus;
        TLabel *LabelMasterConfHeaderTrailer;
        TEdit *EditHeaderTrailerRegNum;
        TEdit *EditHeaderTrailerRegValue;
        TButton *ButtonHeaderTrailerRegWrite;
        TButton *ButtonHeaderTrailerRegRead;
        TEdit *EditHeaderTrailerRegValueRb;
        TLabel *LabelMasterConfCtrlSuze;
        TEdit *EditCtrlSuzeRegNum;
        TEdit *EditCtrlSuzeRegValue;
        TButton *ButtonCtrlSuzeRegWrite;
        TButton *ButtonCtrlSuzeRegRead;
        TEdit *EditCtrlSuzeRegValueRb;
        TLabel *LabelMasterConfSeqSuze;
        TEdit *EditSeqSuzeRegNum;
        TEdit *EditSeqSuzeRegValue;
        TButton *ButtonSeqSuzeRegWrite;
        TButton *ButtonSeqSuzeRegRead;
        TEdit *EditSeqSuzeRegValueRb;
        TLabel *LabelMasterConfCtrl8B10BReg0;
        TEdit *EditCtrl8B10BReg0RegNum;
        TEdit *EditCtrl8B10BReg0RegValue;
        TButton *ButtonCtrl8B10BReg0RegWrite;
        TButton *ButtonCtrl8B10BReg0RegRead;
        TEdit *EditCtrl8B10BReg0RegValueRb;
        TLabel *LabelMasterConf8B10BReg1EncodedData;
        TEdit *EditCtrl8B10BReg1EncodedDataRegNum;
        TButton *ButtonCtrl8B10BReg1EncodedDataRegRead;
        TEdit *EditCtrl8B10BReg1EncodedDataRegValueRb;
        TLabel *Label1;
        TEdit *EditCtrl8B10BReg1RawDataRegNum;
        TButton *ButtonCtrl8B10BReg1RawDataRegRead;
        TEdit *EditCtrl8B10BReg1RawDataRegValueRb;
        TLabel *Label12;
        TButton *ButtonCtrl8B10BReg1FlagsRegRead;
        TEdit *EditCtrl8B10BReg1FlagsRegValueRb;
        TEdit *EditCtrl8B10BReg1FlagsRegNum;
        TButton *ButtonSaveAll;
        TButton *ButtonReadBias;
        void __fastcall ButtonInfoClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ButtonResetClick(TObject *Sender);
        void __fastcall ButtonStartClick(TObject *Sender);
        void __fastcall ButtonReadClick(TObject *Sender);
        void __fastcall ButtonReadBiasClick(TObject *Sender);
        void __fastcall ButtonUpdateBiasClick(TObject *Sender);
        void __fastcall ButtonUpdateAllClick(TObject *Sender);
        void __fastcall ButtonFileLoadDialogClick(TObject *Sender);
        void __fastcall ButtonLoadFileClick(TObject *Sender);
        void __fastcall ButtonFileSaveDialogClick(TObject *Sender);
        void __fastcall ButtonSaveFileClick(TObject *Sender);
        void __fastcall ButtonPortParallelAddrUpdateClick(TObject *Sender);
        void __fastcall ButtonBiasRegWriteClick(TObject *Sender);
        void __fastcall ButtonRegReadClick(TObject *Sender);
        void __fastcall ButtonRomodeRegWriteClick(TObject *Sender);
        void __fastcall ButtonRomodeRegReadClick(TObject *Sender);
        void __fastcall ButtonDiscriRegWriteClick(
          TObject *Sender);
        void __fastcall ButtonDiscriRegReadClick(
          TObject *Sender);
        void __fastcall ButtonLinePat0RegWriteClick(
          TObject *Sender);
        void __fastcall ButtonPat0RegReadClick(
          TObject *Sender);
        void __fastcall ButtonLinePat1RegWriteClick(
          TObject *Sender);
        void __fastcall ButtonLinePat1RegReadClick(
          TObject *Sender);
        void __fastcall ButtonCtrlRegWriteClick(TObject *Sender);
        void __fastcall ButtonCtrlRegReadClick(TObject *Sender);
        void __fastcall ButtonDevNumSelectClick(TObject *Sender);
        void __fastcall ButtonDevNumReadClick(TObject *Sender);
        void __fastcall ButtonTest1PadRegWriteClick(TObject *Sender);
        void __fastcall ButtonTest1PadRegReadClick(TObject *Sender);
        void __fastcall ButtonTest2PadRegWriteClick(TObject *Sender);
        void __fastcall ButtonTest2PadRegReadClick(TObject *Sender);
        void __fastcall ButtonHeaderTrailerRegWriteClick(TObject *Sender);
        void __fastcall ButtonHeaderTrailerRegReadClick(TObject *Sender);
        void __fastcall ButtonCtrlSuzeRegWriteClick(TObject *Sender);
        void __fastcall ButtonCtrlSuzeRegReadClick(TObject *Sender);
        void __fastcall ButtonSeqSuzeRegWriteClick(TObject *Sender);
        void __fastcall ButtonSeqSuzeRegReadClick(TObject *Sender);
        void __fastcall ButtonCtrl8B10BReg0RegWriteClick(TObject *Sender);
        void __fastcall ButtonCtrl8B10BReg0RegReadClick(TObject *Sender);
        void __fastcall ButtonCtrl8B10BReg1EncodedDataRegReadClick(TObject *Sender);
        void __fastcall ButtonCtrl8B10BReg1RawDataRegReadClick(TObject *Sender);
        void __fastcall ButtonCtrl8B10BReg1FlagsRegReadClick(
          TObject *Sender);
        void __fastcall ButtonSaveAllClick(TObject *Sender);
        
private:	// Déclarations de l'utilisateur
                TCOMIMI26MasterConf  MI26MasterConf;
public:		// Déclarations de l'utilisateur
        __fastcall TFormMI26Client(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMI26Client *FormMI26Client;
//---------------------------------------------------------------------------
#endif
