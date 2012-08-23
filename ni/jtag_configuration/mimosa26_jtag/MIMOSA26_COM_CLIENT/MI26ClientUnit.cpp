//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MI26ClientUnit.h"



USEUNIT("MI26LIB_TLB.cpp");
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMI26Client *FormMI26Client;

//---------------------------------------------------------------------------
__fastcall TFormMI26Client::TFormMI26Client(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormMI26Client::ButtonInfoClick(TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }
        if(MI26MasterConf.IsBound())
        {

                try {
                        WideString strValue;
                        OleCheck(MI26MasterConf.get_Info(&strValue));
                        Edit1->Text = strValue;
                }
                catch (EOleError &eOSE) {

                        Memo1->Lines->Add(eOSE.Message);
                }
        }
}

//---------------------------------------------------------------------------
void __fastcall TFormMI26Client::FormClose(TObject *Sender,
      TCloseAction &Action)
{
     MI26MasterConf.Unbind();

}
//---------------------------------------------------------------------------
void __fastcall TFormMI26Client::ButtonResetClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                WideString MsgStr;
                OleCheck(MI26MasterConf.MasterConfReset(&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }

}
//---------------------------------------------------------------------------
void __fastcall TFormMI26Client::ButtonStartClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                WideString MsgStr;
                OleCheck(MI26MasterConf.MasterConfStart(&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }

}
//---------------------------------------------------------------------------
void __fastcall TFormMI26Client::ButtonReadClick(TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                WideString MsgStr;
                OleCheck(MI26MasterConf.MasterConfReadBack(&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }

}

//---------------------------------------------------------------------------
void __fastcall TFormMI26Client::ButtonReadBiasClick(TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                WideString MsgStr;
                OleCheck(MI26MasterConf.MasterConfReadBiasBack(&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }

}
//---------------------------------------------------------------------------
void __fastcall TFormMI26Client::ButtonUpdateBiasClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                WideString MsgStr;
                OleCheck(MI26MasterConf.MasterConfUpdateBias(&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }

}
//---------------------------------------------------------------------------
void __fastcall TFormMI26Client::ButtonUpdateAllClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                WideString MsgStr;
                OleCheck(MI26MasterConf.MasterConfUpdateAll(&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------
void __fastcall TFormMI26Client::ButtonFileLoadDialogClick(
      TObject *Sender)
{
  OpenDialog->Options.Clear();
  OpenDialog->InitialDir = AnsiString("C:\\CCMOS_SCTRL\\MIMOSA26_JTAG\config_files");
  OpenDialog->Title      = "Load Master Configuration";
  OpenDialog->Filter     = "Master Configuration (*.mcf)|*.MCF";
  OpenDialog->Options << ofFileMustExist << ofHideReadOnly << ofNoChangeDir;



  if (OpenDialog->Execute()) {
        EditLoadFileName->Text =  OpenDialog->FileName;
        Memo1->Lines->Add(AnsiString("Load Configuration File: '")+ FormMI26Client->OpenDialog->FileName + AnsiString("'"));
        ButtonLoadFile->Enabled = 1;
  }

}
//---------------------------------------------------------------------------
void __fastcall TFormMI26Client::ButtonLoadFileClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                WideString FileName = EditLoadFileName->Text;
                WideString MsgStr;
                OleCheck(MI26MasterConf.MasterConfLoadFile(FileName,&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }

        ButtonSaveAll->Enabled = true;
        
}
//---------------------------------------------------------------------------
void __fastcall TFormMI26Client::ButtonFileSaveDialogClick(
      TObject *Sender)
{
  SaveDialog->Options.Clear();
  SaveDialog->InitialDir = AnsiString("C:\\CCMOS_SCTRL\\MIMOSA26_JTAG_INPOUT32\\config_files");
  SaveDialog->Title      = "Load Master Configuration";
  SaveDialog->Filter     = "Master Configuration (*.mcf)|*.MCF";
  SaveDialog->Options << ofFileMustExist << ofHideReadOnly << ofNoChangeDir;



  if (SaveDialog->Execute()) {
    EditSaveFileName->Text =  SaveDialog->FileName;
    Memo1->Lines->Add(AnsiString("Save Configuration File: '")+ FormMI26Client->SaveDialog->FileName + AnsiString("'"));
    ButtonSaveFile->Enabled = 1;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMI26Client::ButtonSaveFileClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                WideString FileName = EditSaveFileName->Text;
                WideString MsgStr;
                OleCheck(MI26MasterConf.MasterConfSaveFile(FileName,&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------
void __fastcall TFormMI26Client::ButtonPortParallelAddrUpdateClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                AnsiString Str= AnsiString("0x") + EditParallelPortAddr->Text;
                int Addr = Str.ToInt();
                long rb_Addr = 0;
                WideString MsgStr;
                OleCheck(MI26MasterConf.MasterConfSetParallelPortAddr(Addr,&rb_Addr,&MsgStr));
                EditParallelPortAddrRb->Text = Str.IntToHex(rb_Addr,4);
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonBiasRegWriteClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                int regnum = EditBiasRegNum->Text.ToIntDef(0);
                int regval = EditBiasRegValue->Text.ToIntDef(0);
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfSetBias(regnum,regval,&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonRegReadClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                int regnum = EditBiasRegNum->Text.ToIntDef(0);
                long regval= 0;
                long rb_regval= 0;
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfGetBias(regnum,&regval,&rb_regval,&MsgStr));
                EditBiasRegValue->Text = AnsiString(regval);
                EditBiasRegValueRb->Text = AnsiString(rb_regval);
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonRomodeRegWriteClick(TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                int regnum = EditRomodeRegNum->Text.ToIntDef(0);
                int regval = EditRomodeRegValue->Text.ToIntDef(0);
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfSetRoMode(regnum,regval,&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonRomodeRegReadClick(TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                int regnum = EditRomodeRegNum->Text.ToIntDef(0);
                long regval= 0;
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfGetRoMode(regnum,&regval,&MsgStr));
                EditRomodeRegValueRb->Text = AnsiString(regval);
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonDiscriRegWriteClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                int regnum = EditDiscriRegNum->Text.ToIntDef(0);
                AnsiString Str =   AnsiString("0x") +  EditDiscriRegValue->Text.LowerCase();
                long regval= (long ) Str.ToIntDef(0);
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfSetDiscri(regnum,regval,&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonDiscriRegReadClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                int regnum = EditDiscriRegNum->Text.ToIntDef(0);
                long regval= 0;
                long rb_regval= 0;
                AnsiString Str;
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfGetDiscri(regnum,&regval,&rb_regval,&MsgStr));
                EditDiscriRegValue->Text  = Str.IntToHex(regval,8);
                EditDiscriRegValueRb->Text  = Str.IntToHex(rb_regval,8);
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonLinePat0RegWriteClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {

                int regnum = EditLinePat0RegNum->Text.ToIntDef(0);
                AnsiString Str =   AnsiString("0x") +  EditLinePat0RegValue->Text.LowerCase();
                long regval= Str.ToIntDef(0);
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfSetLinePat0(regnum,regval,&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonPat0RegReadClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                AnsiString Str;
                int regnum = EditLinePat0RegNum->Text.ToIntDef(0);
                long regval= 0;
                long rb_regval= 0;
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfGetLinePat0(regnum,&regval,&rb_regval,&MsgStr));
                EditLinePat0RegValue->Text  =  Str.IntToHex(regval,8);
                EditLinePat0RegValueRb->Text  =  Str.IntToHex(rb_regval,8);
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonLinePat1RegWriteClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {

                int regnum = EditLinePat1RegNum->Text.ToIntDef(0);
                AnsiString Str =   AnsiString("0x") +  EditLinePat1RegValue->Text.LowerCase();
                long regval= Str.ToIntDef(0);
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfSetLinePat1(regnum,regval,&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonLinePat1RegReadClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                AnsiString Str;
                int regnum = EditLinePat1RegNum->Text.ToIntDef(0);
                long regval= 0;
                long rb_regval= 0;
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfGetLinePat1(regnum,&regval, &rb_regval, &MsgStr));
                EditLinePat1RegValue->Text  =  Str.IntToHex(regval,8);
                EditLinePat1RegValueRb->Text  =  Str.IntToHex(rb_regval,8);
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------


void __fastcall TFormMI26Client::ButtonCtrlRegWriteClick(TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {

                int regnum = EditCtrlPixRegNum->Text.ToIntDef(0);
                long regval= EditCtrlPixRegValue->Text.ToIntDef(0);
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfSetCtrl(regnum,regval, &MsgStr));
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonCtrlRegReadClick(TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                AnsiString Str;
                int regnum = EditCtrlPixRegNum->Text.ToIntDef(0);
                long regval= 0;
                long rb_regval= 0;
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfGetCtrl(regnum,&regval, &rb_regval, &MsgStr));
                EditCtrlPixRegValue->Text  = AnsiString(regval);
                EditCtrlPixRegValueRb->Text  =  AnsiString(rb_regval);
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonDevNumSelectClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                int DevNum = EditDevNum->Text.ToIntDef(0);
                WideString MsgStr;
                OleCheck(MI26MasterConf.MasterConfSetDevNum(DevNum,&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonDevNumReadClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                long DevNum;
                WideString MsgStr;
                OleCheck(MI26MasterConf.MasterConfGetDevNum(&DevNum,&MsgStr));
                EditDevNumRb->Text = AnsiString(DevNum);
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonTest1PadRegWriteClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                int regvalue = EditTest1PadRegValue->Text.ToIntDef(0);
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfSetTest1Pad(regvalue,&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------


void __fastcall TFormMI26Client::ButtonTest1PadRegReadClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                long regvalue;
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfGetTest1Pad(&regvalue,&MsgStr));
                EditTest1PadRegValueRb->Text = AnsiString(regvalue);
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonTest2PadRegWriteClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                int regvalue = EditTest2PadRegValue->Text.ToIntDef(0);
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfSetTest2Pad(regvalue,&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonTest2PadRegReadClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                long regvalue;
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfGetTest2Pad(&regvalue,&MsgStr));
                EditTest2PadRegValueRb->Text = AnsiString(regvalue);
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonHeaderTrailerRegWriteClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {

                int regnum = EditHeaderTrailerRegNum->Text.ToIntDef(0);
                AnsiString Str =   AnsiString("0x") +  EditHeaderTrailerRegValue->Text.LowerCase();
                long regval= Str.ToIntDef(0);
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfSetHeaderTrailer(regnum,regval,&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonHeaderTrailerRegReadClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                AnsiString Str;
                int regnum = EditHeaderTrailerRegNum->Text.ToIntDef(0);
                long regval= 0;
                long rb_regval= 0;
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfGetHeaderTrailer(regnum,&regval, &rb_regval, &MsgStr));
                EditHeaderTrailerRegValue->Text  = Str.IntToHex(regval,4);
                EditHeaderTrailerRegValueRb->Text  =  Str.IntToHex(rb_regval,4);
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonCtrlSuzeRegWriteClick(TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {

                int regnum = EditCtrlSuzeRegNum->Text.ToIntDef(0);
                long regval= EditCtrlSuzeRegValue->Text.ToIntDef(0);
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfSetCtrlSuze(regnum,regval,&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonCtrlSuzeRegReadClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                AnsiString Str;
                int regnum = EditCtrlSuzeRegNum->Text.ToIntDef(0);
                long regval= 0;
                long rb_regval= 0;
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfGetCtrlSuze(regnum,&regval, &rb_regval, &MsgStr));
                EditCtrlSuzeRegValue->Text  = AnsiString(regval);
                EditCtrlSuzeRegValueRb->Text  =  AnsiString(rb_regval);
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonSeqSuzeRegWriteClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {

                int regnum = EditSeqSuzeRegNum->Text.ToIntDef(0);
                AnsiString Str =   AnsiString("0x") +  EditSeqSuzeRegValue->Text.LowerCase();
                long regval= Str.ToIntDef(0);
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfSetSeqSuze(regnum,regval,&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonSeqSuzeRegReadClick(TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                AnsiString Str;
                int regnum = EditSeqSuzeRegNum->Text.ToIntDef(0);
                long regval= 0;
                long rb_regval= 0;
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfGetSeqSuze(regnum,&regval, &rb_regval, &MsgStr));
                EditSeqSuzeRegValue->Text  = Str.IntToHex(regval,4);
                EditSeqSuzeRegValueRb->Text  =  Str.IntToHex(rb_regval,4);
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonCtrl8B10BReg0RegWriteClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {

                int regnum = EditCtrl8B10BReg0RegNum->Text.ToIntDef(0);
                AnsiString Str =   AnsiString("0x") +  EditCtrl8B10BReg0RegValue->Text.LowerCase();
                long regval= Str.ToIntDef(0);
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfSet8B10BReg0(regnum,regval,&MsgStr)); //Mimosa26ConfSetSeqSuze(regnum,regval,&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------


void __fastcall TFormMI26Client::ButtonCtrl8B10BReg0RegReadClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                AnsiString Str;
                int regnum = EditCtrl8B10BReg0RegNum->Text.ToIntDef(0);
                long regval= 0;
                long rb_regval= 0;
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfGet8B10BReg0(regnum,&regval, &rb_regval, &MsgStr));
                EditCtrl8B10BReg0RegValue->Text  = Str.IntToHex(regval,4);
                EditCtrl8B10BReg0RegValueRb->Text  =  Str.IntToHex(rb_regval,4);
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------


void __fastcall TFormMI26Client::ButtonCtrl8B10BReg1EncodedDataRegReadClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                AnsiString Str;
                int regnum = EditCtrl8B10BReg1EncodedDataRegNum->Text.ToIntDef(0);
                long regval= 0;
                long rb_regval= 0;
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfGet8B10BReg1EncodedData(regnum,&rb_regval,&MsgStr));
                EditCtrl8B10BReg1EncodedDataRegValueRb->Text  =  Str.IntToHex(rb_regval,4);
                Memo1->Lines->Add(MsgStr);
        }
}

//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonCtrl8B10BReg1RawDataRegReadClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                AnsiString Str;
                int regnum = EditCtrl8B10BReg1RawDataRegNum->Text.ToIntDef(0);
                long regval= 0;
                long rb_regval= 0;
                WideString MsgStr;
                OleCheck(MI26MasterConf.Mimosa26ConfGet8B10BReg1RawData(regnum,&rb_regval, &MsgStr));
                EditCtrl8B10BReg1RawDataRegValueRb->Text  =  Str.IntToHex(rb_regval,4);
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonCtrl8B10BReg1FlagsRegReadClick(
      TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                AnsiString Str;
                int regnum = EditCtrl8B10BReg1FlagsRegNum->Text.ToIntDef(0);
                long regval= 0;
                long rb_regval= 0;
                WideString MsgStr;
                try {
                        OleCheck(MI26MasterConf.Mimosa26ConfGet8B10BReg1Flags(regnum,&rb_regval, &MsgStr));
                }

                catch (EOleError &eOSE) {

                        Memo1->Lines->Add(eOSE.Message);

                }

                EditCtrl8B10BReg1FlagsRegValueRb->Text  =  Str.IntToHex(rb_regval,4);
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMI26Client::ButtonSaveAllClick(TObject *Sender)
{
        if(!MI26MasterConf.IsBound())
        {
                OleCheck(CoMI26MasterConf::Create(MI26MasterConf));
        }

        if(MI26MasterConf.IsBound())
        {
                WideString MsgStr;
                OleCheck(MI26MasterConf.MasterConfSaveAllFile(&MsgStr));
                Memo1->Lines->Add(MsgStr);
        }
}
//---------------------------------------------------------------------------

