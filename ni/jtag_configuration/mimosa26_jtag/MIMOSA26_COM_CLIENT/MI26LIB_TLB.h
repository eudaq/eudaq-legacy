// ************************************************************************ //
// AVERTISSEMENT                                                                 
// -------                                                                    
// Les types déclarés dans ce fichier ont été générés à partir de données lues 
// depuis la bibliothèque de types. Si cette dernière (via une autre bibliothèque de types 
// s'y référant) est explicitement ou indirectement ré-importée, ou la commande "Rafraîchir"  
// de l'éditeur de bibliothèque de types est activée lors de la modification de la bibliothèque 
// de types, le contenu de ce fichier sera régénéré et toutes les modifications      
// manuellement apportées seront perdues.                                     
// ************************************************************************ //

// C++ TLBWRTR : $Revision:   1.151.1.0.1.27  $
// Fichier généré le 01/02/2010 09:50:19 depuis la bibliothèque de types ci-dessous.

// ************************************************************************  //
// Bibl. types : D:\dd\sdev\src\com\c\dev\prj\win\jtag_mimostar\mimosa_26\mimosa_26_inpout32_COM_v1.7_powerpulse\MI26LIB.tlb (1)
// LIBID: {C4745228-8659-4D23-B3DD-BDF9C188AB77}
// LCID: 0
// Fichier d'aide : 
// Chaîne d'aide : FormMimosa23MasterConf_main_inpout32 (bibliothèque)
// DepndLst: 
//   (1) v2.0 stdole, (C:\WINDOWS\system32\STDOLE2.TLB)
// ************************************************************************ //
#ifndef   MI26LIB_TLBH
#define   MI26LIB_TLBH

#pragma option push -b -w-inl

#include <utilcls.h>
#if !defined(__UTILCLS_H_VERSION) || (__UTILCLS_H_VERSION < 0x0600)
//
// Le code généré par l'utilitaire TLIBIMP ou la fonction d'importation de bibliothèque de types 
// ou d'ActiveX de C++Builder repose sur des versions spécifiques
// du fichier en-tête UTILCLS.H situé dans le répertoire INCLUDE\VCL. Si une 
// version antérieure du fichier est détectée, vous devrez appliquer une mise à jour ou un correctif.
//
#error "Ce fichier requiert une version plus récente du fichier en-tête UTILCLS.H" \
       "Vous devez appliquer une mise à jour/correctif à votre copie de C++Builder"
#endif
#include <olectl.h>
#include <ocidl.h>
#if defined(USING_ATLVCL) || defined(USING_ATL)
#if !defined(__TLB_NO_EVENT_WRAPPERS)
#include <atl/atlmod.h>
#endif
#endif


// *********************************************************************//
// Référence Forward à certains types VCL (pour éviter d'inclure STDVCL.HPP)    
// *********************************************************************//
namespace Stdvcl {class IStrings; class IStringsDisp;}
using namespace Stdvcl;
typedef TComInterface<IStrings> IStringsPtr;
typedef TComInterface<IStringsDisp> IStringsDispPtr;

namespace Mi26lib_tlb
{

// *********************************************************************//
// Chaîne d'aide : FormMimosa23MasterConf_main_inpout32 (bibliothèque)
// Version :    1.0
// *********************************************************************//


// *********************************************************************//
// GUIDS déclarés dans la bibliothèque de types. Préfixes utilisés :    
//   Bibliothèques de types : LIBID_xxxx                                      
//   CoClasses              : CLSID_xxxx                                      
//   DISPInterfaces         : DIID_xxxx                                       
//   Non-DISP interfaces    : IID_xxxx                                        
// *********************************************************************//
extern "C" const __declspec(selectany) GUID LIBID_MI26LIB = {0xC4745228, 0x8659, 0x4D23,{ 0xB3, 0xDD, 0xBD,0xF9, 0xC1, 0x88,0xAB, 0x77} };
extern "C" const __declspec(selectany) GUID IID_IMI26MasterConf = {0x485C7AE6, 0xA7C1, 0x4901,{ 0x8C, 0x83, 0x42,0x1A, 0xBE, 0x54,0x59, 0x94} };
extern "C" const __declspec(selectany) GUID CLSID_MI26MasterConf = {0x1ED8AA40, 0x2D7D, 0x4B3D,{ 0x8A, 0xB4, 0x62,0xFE, 0x91, 0xB8,0xB1, 0xB1} };

// *********************************************************************//
// Déclaration Forward des types définis dans la bibliothèque de types    
// *********************************************************************//
interface DECLSPEC_UUID("{485C7AE6-A7C1-4901-8C83-421ABE545994}") IMI26MasterConf;
typedef TComInterface<IMI26MasterConf, &IID_IMI26MasterConf> IMI26MasterConfPtr;


// *********************************************************************//
// Déclaration des CoClasses définies dans la bibliothèque de types    
// (REMARQUE : On affecte chaque CoClasse à son Interface par défaut)              
//                                                                        
// Les macros LIBID_OF_ affectent un LIBID_OF_CoClassName au GUID de 
// cette Bibliothèque de types. Cela simplifie la mise à jour des macros lorsque le nom 
// de CoClasse change.                                                           
// *********************************************************************//
typedef IMI26MasterConf MI26MasterConf;
typedef IMI26MasterConfPtr MI26MasterConfPtr;

#define LIBID_OF_MI26MasterConf (&LIBID_MI26LIB)
// *********************************************************************//
// Interface   : IMI26MasterConf
// Indicateurs : (4416) Dual OleAutomation Dispatchable
// GUID        : {485C7AE6-A7C1-4901-8C83-421ABE545994}
// *********************************************************************//
interface IMI26MasterConf  : public IDispatch
{
public:
  virtual HRESULT STDMETHODCALLTYPE get_Info(BSTR* Value/*[out,retval]*/) = 0; // [1]
  virtual HRESULT STDMETHODCALLTYPE MasterConfLoadFile(BSTR FileName/*[in]*/, 
                                                       BSTR* Msg/*[out,retval]*/) = 0; // [2]
  virtual HRESULT STDMETHODCALLTYPE MasterConfSaveFile(BSTR FileName/*[in]*/, 
                                                       BSTR* Msg/*[out,retval]*/) = 0; // [3]
  virtual HRESULT STDMETHODCALLTYPE MasterConfUpdateAll(BSTR* Msg/*[out,retval]*/) = 0; // [4]
  virtual HRESULT STDMETHODCALLTYPE MasterConfUpdateBias(BSTR* Msg/*[out,retval]*/) = 0; // [5]
  virtual HRESULT STDMETHODCALLTYPE MasterConfReadBack(BSTR* Msg/*[out,retval]*/) = 0; // [6]
  virtual HRESULT STDMETHODCALLTYPE MasterConfReset(BSTR* Msg/*[out,retval]*/) = 0; // [7]
  virtual HRESULT STDMETHODCALLTYPE MasterConfStart(BSTR* Msg/*[out,retval]*/) = 0; // [8]
  virtual HRESULT STDMETHODCALLTYPE MasterConfSetParallelPortAddr(int Addr/*[in]*/, 
                                                                  long* Rb_Addr/*[out]*/, 
                                                                  BSTR* Msg/*[out,retval]*/) = 0; // [9]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfSetBias(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                        BSTR* Msg/*[out,retval]*/) = 0; // [10]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfGetBias(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                        long* Rb_RegValue/*[out]*/, 
                                                        BSTR* Msg/*[out,retval]*/) = 0; // [11]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfSetRoMode(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                          BSTR* Msg/*[out,retval]*/) = 0; // [12]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfGetRoMode(long RegNum/*[in]*/, 
                                                          long* RegValue/*[out]*/, 
                                                          BSTR* Msg/*[out,retval]*/) = 0; // [13]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfSetDiscri(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                          BSTR* Msg/*[out,retval]*/) = 0; // [14]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfGetDiscri(long RegNum/*[in]*/, 
                                                          long* RegValue/*[out]*/, 
                                                          long* Rb_RegValue/*[out]*/, 
                                                          BSTR* Msg/*[out,retval]*/) = 0; // [15]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfSetLinePat0(long RegNum/*[in]*/, 
                                                            long RegValue/*[in]*/, 
                                                            BSTR* Msg/*[out,retval]*/) = 0; // [16]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfGetLinePat0(long RegNum/*[in]*/, 
                                                            long* RegValue/*[out]*/, 
                                                            long* Rb_RegValue/*[out]*/, 
                                                            BSTR* Msg/*[out,retval]*/) = 0; // [17]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfSetLinePat1(long RegNum/*[in]*/, 
                                                            long RegValue/*[in]*/, BSTR* Msg/*[in]*/) = 0; // [18]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfGetLinePat1(long RegNum/*[in]*/, 
                                                            long* RegValue/*[out]*/, 
                                                            long* Rb_RegValue/*[out]*/, 
                                                            BSTR* Msg/*[out,retval]*/) = 0; // [19]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfShowWindow(long ShowWin/*[in]*/, 
                                                           BSTR* Msg/*[out,retval]*/) = 0; // [21]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfSetCtrl(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                        BSTR* Msg/*[out,retval]*/) = 0; // [22]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfGetCtrl(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                        long* Rb_RegValue/*[out]*/, 
                                                        BSTR* Msg/*[out,retval]*/) = 0; // [23]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfSetTest1Pad(long RegValue/*[in]*/, 
                                                            BSTR* Msg/*[out,retval]*/) = 0; // [24]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfGetTest1Pad(long* RegValue/*[out]*/, 
                                                            BSTR* Msg/*[out,retval]*/) = 0; // [25]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfSetTest2Pad(long RegValue/*[in]*/, 
                                                            BSTR* Msg/*[out,retval]*/) = 0; // [26]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfGetTest2Pad(long* RegValue/*[out]*/, 
                                                            BSTR* Msg/*[out,retval]*/) = 0; // [27]
  virtual HRESULT STDMETHODCALLTYPE MasterConfUpdateLinePat(BSTR* Msg/*[out,retval]*/) = 0; // [20]
  virtual HRESULT STDMETHODCALLTYPE MasterConfSetDevNum(long DevNum/*[in]*/, 
                                                        BSTR* Msg/*[out,retval]*/) = 0; // [28]
  virtual HRESULT STDMETHODCALLTYPE MasterConfGetDevNum(long* DevNum/*[out]*/, 
                                                        BSTR* Msg/*[out,retval]*/) = 0; // [29]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfSetHeaderTrailer(long RegNum/*[in]*/, 
                                                                 long RegValue/*[in]*/, 
                                                                 BSTR* Msg/*[out,retval]*/) = 0; // [30]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfGetHeaderTrailer(long RegNum/*[in]*/, 
                                                                 long* RegValue/*[out]*/, 
                                                                 long* Rb_RegValue/*[out]*/, 
                                                                 BSTR* Msg/*[out,retval]*/) = 0; // [31]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfSetSeqPix(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                          BSTR* Msg/*[out,retval]*/) = 0; // [32]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfGetSeqPix(long RegNum/*[in]*/, 
                                                          long* RegValue/*[out]*/, 
                                                          long* Rb_RegValue/*[out]*/, 
                                                          BSTR* Msg/*[out,retval]*/) = 0; // [33]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfSetSeqSuze(long RegNum/*[in]*/, 
                                                           long RegValue/*[in]*/, 
                                                           BSTR* Msg/*[out,retval]*/) = 0; // [34]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfGetSeqSuze(long RegNum/*[in]*/, 
                                                           long* RegValue/*[out]*/, 
                                                           long* Rb_RegValue/*[out]*/, 
                                                           BSTR* Msg/*[out,retval]*/) = 0; // [35]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfSetCtrlSuze(long RegNum/*[in]*/, 
                                                            long RegValue/*[in]*/, 
                                                            BSTR* Msg/*[out,retval]*/) = 0; // [36]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfGetCtrlSuze(long RegNum/*[in]*/, 
                                                            long* RegValue/*[out]*/, 
                                                            long* Rb_RegValue/*[out]*/, 
                                                            BSTR* Msg/*[out,retval]*/) = 0; // [37]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfSet8B10BReg0(long RegNum/*[in]*/, 
                                                             long RegValue/*[in]*/, 
                                                             BSTR* Msg/*[out,retval]*/) = 0; // [38]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfGet8B10BReg0(long RegNum/*[in]*/, 
                                                             long* RegValue/*[out]*/, 
                                                             long* Rb_RegValue/*[out]*/, 
                                                             BSTR* Msg/*[out,retval]*/) = 0; // [39]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfGet8B10BReg1EncodedData(long RegNum/*[in]*/, 
                                                                        long* Rb_RegValue/*[out]*/, 
                                                                        BSTR* Msg/*[out,retval]*/) = 0; // [41]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfGet8B10BReg1Flags(long RegNum/*[in]*/, 
                                                                  long* Rb_RegValue/*[out]*/, 
                                                                  BSTR* Msg/*[out,retval]*/) = 0; // [42]
  virtual HRESULT STDMETHODCALLTYPE Mimosa26ConfGet8B10BReg1RawData(long RegNum/*[in]*/, 
                                                                    long* Rb_RegValue/*[out]*/, 
                                                                    BSTR* Msg/*[out,retval]*/) = 0; // [40]
  virtual HRESULT STDMETHODCALLTYPE MasterConfSaveAllFile(BSTR* Msg/*[out,retval]*/) = 0; // [43]
  virtual HRESULT STDMETHODCALLTYPE MasterConfReadBiasBack(BSTR* Msg/*[out,retval]*/) = 0; // [44]

#if !defined(__TLB_NO_INTERFACE_WRAPPERS)

  BSTR __fastcall get_Info(void)
  {
    BSTR Value = 0;
    OLECHECK(this->get_Info((BSTR*)&Value));
    return Value;
  }

  BSTR __fastcall MasterConfLoadFile(BSTR FileName/*[in]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->MasterConfLoadFile(FileName, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall MasterConfSaveFile(BSTR FileName/*[in]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->MasterConfSaveFile(FileName, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall MasterConfUpdateAll(void)
  {
    BSTR Msg = 0;
    OLECHECK(this->MasterConfUpdateAll((BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall MasterConfUpdateBias(void)
  {
    BSTR Msg = 0;
    OLECHECK(this->MasterConfUpdateBias((BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall MasterConfReadBack(void)
  {
    BSTR Msg = 0;
    OLECHECK(this->MasterConfReadBack((BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall MasterConfReset(void)
  {
    BSTR Msg = 0;
    OLECHECK(this->MasterConfReset((BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall MasterConfStart(void)
  {
    BSTR Msg = 0;
    OLECHECK(this->MasterConfStart((BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall MasterConfSetParallelPortAddr(int Addr/*[in]*/, long* Rb_Addr/*[out]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->MasterConfSetParallelPortAddr(Addr, Rb_Addr, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfSetBias(long RegNum/*[in]*/, long RegValue/*[in]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfSetBias(RegNum, RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfGetBias(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                      long* Rb_RegValue/*[out]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfGetBias(RegNum, RegValue, Rb_RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfSetRoMode(long RegNum/*[in]*/, long RegValue/*[in]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfSetRoMode(RegNum, RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfGetRoMode(long RegNum/*[in]*/, long* RegValue/*[out]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfGetRoMode(RegNum, RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfSetDiscri(long RegNum/*[in]*/, long RegValue/*[in]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfSetDiscri(RegNum, RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfGetDiscri(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                        long* Rb_RegValue/*[out]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfGetDiscri(RegNum, RegValue, Rb_RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfSetLinePat0(long RegNum/*[in]*/, long RegValue/*[in]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfSetLinePat0(RegNum, RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfGetLinePat0(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                          long* Rb_RegValue/*[out]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfGetLinePat0(RegNum, RegValue, Rb_RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfGetLinePat1(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                          long* Rb_RegValue/*[out]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfGetLinePat1(RegNum, RegValue, Rb_RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfShowWindow(long ShowWin/*[in]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfShowWindow(ShowWin, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfSetCtrl(long RegNum/*[in]*/, long RegValue/*[in]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfSetCtrl(RegNum, RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfGetCtrl(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                      long* Rb_RegValue/*[out]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfGetCtrl(RegNum, RegValue, Rb_RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfSetTest1Pad(long RegValue/*[in]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfSetTest1Pad(RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfGetTest1Pad(long* RegValue/*[out]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfGetTest1Pad(RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfSetTest2Pad(long RegValue/*[in]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfSetTest2Pad(RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfGetTest2Pad(long* RegValue/*[out]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfGetTest2Pad(RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall MasterConfUpdateLinePat(void)
  {
    BSTR Msg = 0;
    OLECHECK(this->MasterConfUpdateLinePat((BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall MasterConfSetDevNum(long DevNum/*[in]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->MasterConfSetDevNum(DevNum, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall MasterConfGetDevNum(long* DevNum/*[out]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->MasterConfGetDevNum(DevNum, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfSetHeaderTrailer(long RegNum/*[in]*/, long RegValue/*[in]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfSetHeaderTrailer(RegNum, RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfGetHeaderTrailer(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                               long* Rb_RegValue/*[out]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfGetHeaderTrailer(RegNum, RegValue, Rb_RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfSetSeqPix(long RegNum/*[in]*/, long RegValue/*[in]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfSetSeqPix(RegNum, RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfGetSeqPix(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                        long* Rb_RegValue/*[out]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfGetSeqPix(RegNum, RegValue, Rb_RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfSetSeqSuze(long RegNum/*[in]*/, long RegValue/*[in]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfSetSeqSuze(RegNum, RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfGetSeqSuze(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                         long* Rb_RegValue/*[out]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfGetSeqSuze(RegNum, RegValue, Rb_RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfSetCtrlSuze(long RegNum/*[in]*/, long RegValue/*[in]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfSetCtrlSuze(RegNum, RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfGetCtrlSuze(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                          long* Rb_RegValue/*[out]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfGetCtrlSuze(RegNum, RegValue, Rb_RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfSet8B10BReg0(long RegNum/*[in]*/, long RegValue/*[in]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfSet8B10BReg0(RegNum, RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfGet8B10BReg0(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                           long* Rb_RegValue/*[out]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfGet8B10BReg0(RegNum, RegValue, Rb_RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfGet8B10BReg1EncodedData(long RegNum/*[in]*/, 
                                                      long* Rb_RegValue/*[out]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfGet8B10BReg1EncodedData(RegNum, Rb_RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfGet8B10BReg1Flags(long RegNum/*[in]*/, long* Rb_RegValue/*[out]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfGet8B10BReg1Flags(RegNum, Rb_RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall Mimosa26ConfGet8B10BReg1RawData(long RegNum/*[in]*/, long* Rb_RegValue/*[out]*/)
  {
    BSTR Msg = 0;
    OLECHECK(this->Mimosa26ConfGet8B10BReg1RawData(RegNum, Rb_RegValue, (BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall MasterConfSaveAllFile(void)
  {
    BSTR Msg = 0;
    OLECHECK(this->MasterConfSaveAllFile((BSTR*)&Msg));
    return Msg;
  }

  BSTR __fastcall MasterConfReadBiasBack(void)
  {
    BSTR Msg = 0;
    OLECHECK(this->MasterConfReadBiasBack((BSTR*)&Msg));
    return Msg;
  }


  __property   BSTR            Info = {read = get_Info};

#endif //   __TLB_NO_INTERFACE_WRAPPERS

};

#if !defined(__TLB_NO_INTERFACE_WRAPPERS)
// *********************************************************************//
// SmartIntf: TCOMIMI26MasterConf
// Interface: IMI26MasterConf
// *********************************************************************//
template <class T /* IMI26MasterConf */ >
class TCOMIMI26MasterConfT : public TComInterface<IMI26MasterConf>, public TComInterfaceBase<IUnknown>
{
public:
  TCOMIMI26MasterConfT() {}
  TCOMIMI26MasterConfT(IMI26MasterConf *intf, bool addRef = false) : TComInterface<IMI26MasterConf>(intf, addRef) {}
  TCOMIMI26MasterConfT(const TCOMIMI26MasterConfT& src) : TComInterface<IMI26MasterConf>(src) {}
  TCOMIMI26MasterConfT& operator=(const TCOMIMI26MasterConfT& src) { Bind(src, true); return *this;}

  HRESULT         __fastcall get_Info(BSTR* Value/*[out,retval]*/);
  BSTR            __fastcall get_Info(void);
  HRESULT         __fastcall MasterConfLoadFile(BSTR FileName/*[in]*/, BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfLoadFile(BSTR FileName/*[in]*/);
  HRESULT         __fastcall MasterConfSaveFile(BSTR FileName/*[in]*/, BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfSaveFile(BSTR FileName/*[in]*/);
  HRESULT         __fastcall MasterConfUpdateAll(BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfUpdateAll(void);
  HRESULT         __fastcall MasterConfUpdateBias(BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfUpdateBias(void);
  HRESULT         __fastcall MasterConfReadBack(BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfReadBack(void);
  HRESULT         __fastcall MasterConfReset(BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfReset(void);
  HRESULT         __fastcall MasterConfStart(BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfStart(void);
  HRESULT         __fastcall MasterConfSetParallelPortAddr(int Addr/*[in]*/, long* Rb_Addr/*[out]*/, 
                                                           BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfSetParallelPortAddr(int Addr/*[in]*/, long* Rb_Addr/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetBias(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                 BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetBias(long RegNum/*[in]*/, long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetBias(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                 long* Rb_RegValue/*[out]*/, 
                                                 BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetBias(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                 long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetRoMode(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                   BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetRoMode(long RegNum/*[in]*/, long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetRoMode(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                   BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetRoMode(long RegNum/*[in]*/, long* RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetDiscri(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                   BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetDiscri(long RegNum/*[in]*/, long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetDiscri(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                   long* Rb_RegValue/*[out]*/, 
                                                   BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetDiscri(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                   long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetLinePat0(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                     BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetLinePat0(long RegNum/*[in]*/, long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetLinePat0(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                     long* Rb_RegValue/*[out]*/, 
                                                     BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetLinePat0(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                     long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetLinePat1(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                     BSTR* Msg/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetLinePat1(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                     long* Rb_RegValue/*[out]*/, 
                                                     BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetLinePat1(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                     long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfShowWindow(long ShowWin/*[in]*/, BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfShowWindow(long ShowWin/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfSetCtrl(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                 BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetCtrl(long RegNum/*[in]*/, long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetCtrl(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                 long* Rb_RegValue/*[out]*/, 
                                                 BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetCtrl(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                 long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetTest1Pad(long RegValue/*[in]*/, 
                                                     BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetTest1Pad(long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetTest1Pad(long* RegValue/*[out]*/, 
                                                     BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetTest1Pad(long* RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetTest2Pad(long RegValue/*[in]*/, 
                                                     BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetTest2Pad(long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetTest2Pad(long* RegValue/*[out]*/, 
                                                     BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetTest2Pad(long* RegValue/*[out]*/);
  HRESULT         __fastcall MasterConfUpdateLinePat(BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfUpdateLinePat(void);
  HRESULT         __fastcall MasterConfSetDevNum(long DevNum/*[in]*/, BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfSetDevNum(long DevNum/*[in]*/);
  HRESULT         __fastcall MasterConfGetDevNum(long* DevNum/*[out]*/, BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfGetDevNum(long* DevNum/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetHeaderTrailer(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                          BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetHeaderTrailer(long RegNum/*[in]*/, long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetHeaderTrailer(long RegNum/*[in]*/, 
                                                          long* RegValue/*[out]*/, 
                                                          long* Rb_RegValue/*[out]*/, 
                                                          BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetHeaderTrailer(long RegNum/*[in]*/, 
                                                          long* RegValue/*[out]*/, 
                                                          long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetSeqPix(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                   BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetSeqPix(long RegNum/*[in]*/, long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetSeqPix(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                   long* Rb_RegValue/*[out]*/, 
                                                   BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetSeqPix(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                   long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetSeqSuze(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                    BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetSeqSuze(long RegNum/*[in]*/, long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetSeqSuze(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                    long* Rb_RegValue/*[out]*/, 
                                                    BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetSeqSuze(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                    long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetCtrlSuze(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                     BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetCtrlSuze(long RegNum/*[in]*/, long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetCtrlSuze(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                     long* Rb_RegValue/*[out]*/, 
                                                     BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetCtrlSuze(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                     long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSet8B10BReg0(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                      BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSet8B10BReg0(long RegNum/*[in]*/, long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGet8B10BReg0(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                      long* Rb_RegValue/*[out]*/, 
                                                      BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGet8B10BReg0(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                      long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfGet8B10BReg1EncodedData(long RegNum/*[in]*/, 
                                                                 long* Rb_RegValue/*[out]*/, 
                                                                 BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGet8B10BReg1EncodedData(long RegNum/*[in]*/, 
                                                                 long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfGet8B10BReg1Flags(long RegNum/*[in]*/, 
                                                           long* Rb_RegValue/*[out]*/, 
                                                           BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGet8B10BReg1Flags(long RegNum/*[in]*/, 
                                                           long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfGet8B10BReg1RawData(long RegNum/*[in]*/, 
                                                             long* Rb_RegValue/*[out]*/, 
                                                             BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGet8B10BReg1RawData(long RegNum/*[in]*/, 
                                                             long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall MasterConfSaveAllFile(BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfSaveAllFile(void);
  HRESULT         __fastcall MasterConfReadBiasBack(BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfReadBiasBack(void);

  __property   BSTR            Info = {read = get_Info};
};
typedef TCOMIMI26MasterConfT<IMI26MasterConf> TCOMIMI26MasterConf;

// *********************************************************************//
// DispIntf :  IMI26MasterConf
// Flags :     (4416) Dual OleAutomation Dispatchable
// GUID :      {485C7AE6-A7C1-4901-8C83-421ABE545994}
// *********************************************************************//
template<class T>
class IMI26MasterConfDispT : public TAutoDriver<IMI26MasterConf>
{
public:
  IMI26MasterConfDispT(){}

  IMI26MasterConfDispT(IMI26MasterConf *pintf)
  {
    TAutoDriver<IMI26MasterConf>::Bind(pintf, false);
  }

  IMI26MasterConfDispT(IMI26MasterConfPtr pintf)
  {
    TAutoDriver<IMI26MasterConf>::Bind(pintf, true);
  }

  IMI26MasterConfDispT& operator=(IMI26MasterConf *pintf)
  {
    TAutoDriver<IMI26MasterConf>::Bind(pintf, false);
    return *this;
  }

  IMI26MasterConfDispT& operator=(IMI26MasterConfPtr pintf)
  {
    TAutoDriver<IMI26MasterConf>::Bind(pintf, true);
    return *this;
  }

  HRESULT BindDefault()
  {
    return OLECHECK(Bind(CLSID_MI26MasterConf));
  }

  HRESULT BindRunning()
  {
    return BindToActive(CLSID_MI26MasterConf);
  }

  HRESULT         __fastcall get_Info(BSTR* Value/*[out,retval]*/);
  BSTR            __fastcall get_Info(void);
  HRESULT         __fastcall MasterConfLoadFile(BSTR FileName/*[in]*/, BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfLoadFile(BSTR FileName/*[in]*/);
  HRESULT         __fastcall MasterConfSaveFile(BSTR FileName/*[in]*/, BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfSaveFile(BSTR FileName/*[in]*/);
  HRESULT         __fastcall MasterConfUpdateAll(BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfUpdateAll(void);
  HRESULT         __fastcall MasterConfUpdateBias(BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfUpdateBias(void);
  HRESULT         __fastcall MasterConfReadBack(BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfReadBack(void);
  HRESULT         __fastcall MasterConfReset(BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfReset(void);
  HRESULT         __fastcall MasterConfStart(BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfStart(void);
  HRESULT         __fastcall MasterConfSetParallelPortAddr(int Addr/*[in]*/, long* Rb_Addr/*[out]*/
                                                           , BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfSetParallelPortAddr(int Addr/*[in]*/, long* Rb_Addr/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetBias(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                 BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetBias(long RegNum/*[in]*/, long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetBias(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                 long* Rb_RegValue/*[out]*/, 
                                                 BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetBias(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                 long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetRoMode(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                   BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetRoMode(long RegNum/*[in]*/, long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetRoMode(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                   BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetRoMode(long RegNum/*[in]*/, long* RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetDiscri(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                   BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetDiscri(long RegNum/*[in]*/, long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetDiscri(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                   long* Rb_RegValue/*[out]*/, 
                                                   BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetDiscri(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                   long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetLinePat0(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                     BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetLinePat0(long RegNum/*[in]*/, long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetLinePat0(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                     long* Rb_RegValue/*[out]*/, 
                                                     BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetLinePat0(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                     long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetLinePat1(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                     BSTR* Msg/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetLinePat1(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                     long* Rb_RegValue/*[out]*/, 
                                                     BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetLinePat1(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                     long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfShowWindow(long ShowWin/*[in]*/, BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfShowWindow(long ShowWin/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfSetCtrl(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                 BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetCtrl(long RegNum/*[in]*/, long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetCtrl(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                 long* Rb_RegValue/*[out]*/, 
                                                 BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetCtrl(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                 long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetTest1Pad(long RegValue/*[in]*/, 
                                                     BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetTest1Pad(long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetTest1Pad(long* RegValue/*[out]*/, 
                                                     BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetTest1Pad(long* RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetTest2Pad(long RegValue/*[in]*/, 
                                                     BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetTest2Pad(long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetTest2Pad(long* RegValue/*[out]*/, 
                                                     BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetTest2Pad(long* RegValue/*[out]*/);
  HRESULT         __fastcall MasterConfUpdateLinePat(BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfUpdateLinePat(void);
  HRESULT         __fastcall MasterConfSetDevNum(long DevNum/*[in]*/, BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfSetDevNum(long DevNum/*[in]*/);
  HRESULT         __fastcall MasterConfGetDevNum(long* DevNum/*[out]*/, BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfGetDevNum(long* DevNum/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetHeaderTrailer(long RegNum/*[in]*/, long RegValue/*[in]*/
                                                          , BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetHeaderTrailer(long RegNum/*[in]*/, long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetHeaderTrailer(long RegNum/*[in]*/, 
                                                          long* RegValue/*[out]*/, 
                                                          long* Rb_RegValue/*[out]*/, 
                                                          BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetHeaderTrailer(long RegNum/*[in]*/, 
                                                          long* RegValue/*[out]*/, 
                                                          long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetSeqPix(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                   BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetSeqPix(long RegNum/*[in]*/, long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetSeqPix(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                   long* Rb_RegValue/*[out]*/, 
                                                   BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetSeqPix(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                   long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetSeqSuze(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                    BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetSeqSuze(long RegNum/*[in]*/, long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetSeqSuze(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                    long* Rb_RegValue/*[out]*/, 
                                                    BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetSeqSuze(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                    long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSetCtrlSuze(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                     BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSetCtrlSuze(long RegNum/*[in]*/, long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGetCtrlSuze(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                     long* Rb_RegValue/*[out]*/, 
                                                     BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGetCtrlSuze(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                     long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfSet8B10BReg0(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                      BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfSet8B10BReg0(long RegNum/*[in]*/, long RegValue/*[in]*/);
  HRESULT         __fastcall Mimosa26ConfGet8B10BReg0(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                      long* Rb_RegValue/*[out]*/, 
                                                      BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGet8B10BReg0(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                      long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfGet8B10BReg1EncodedData(long RegNum/*[in]*/, 
                                                                 long* Rb_RegValue/*[out]*/, 
                                                                 BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGet8B10BReg1EncodedData(long RegNum/*[in]*/, 
                                                                 long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfGet8B10BReg1Flags(long RegNum/*[in]*/, 
                                                           long* Rb_RegValue/*[out]*/, 
                                                           BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGet8B10BReg1Flags(long RegNum/*[in]*/, 
                                                           long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall Mimosa26ConfGet8B10BReg1RawData(long RegNum/*[in]*/, 
                                                             long* Rb_RegValue/*[out]*/, 
                                                             BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall Mimosa26ConfGet8B10BReg1RawData(long RegNum/*[in]*/, 
                                                             long* Rb_RegValue/*[out]*/);
  HRESULT         __fastcall MasterConfSaveAllFile(BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfSaveAllFile(void);
  HRESULT         __fastcall MasterConfReadBiasBack(BSTR* Msg/*[out,retval]*/);
  BSTR            __fastcall MasterConfReadBiasBack(void);

  __property   BSTR            Info = {read = get_Info};
};
typedef IMI26MasterConfDispT<IMI26MasterConf> IMI26MasterConfDisp;

// *********************************************************************//
// SmartIntf: TCOMIMI26MasterConf
// Interface: IMI26MasterConf
// *********************************************************************//
template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::get_Info(BSTR* Value/*[out,retval]*/)
{
  return (*this)->get_Info(Value);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::get_Info(void)
{
  BSTR Value = 0;
  OLECHECK(this->get_Info((BSTR*)&Value));
  return Value;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::MasterConfLoadFile(BSTR FileName/*[in]*/, BSTR* Msg/*[out,retval]*/)
{
  return (*this)->MasterConfLoadFile(FileName, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::MasterConfLoadFile(BSTR FileName/*[in]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->MasterConfLoadFile(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::MasterConfSaveFile(BSTR FileName/*[in]*/, BSTR* Msg/*[out,retval]*/)
{
  return (*this)->MasterConfSaveFile(FileName, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::MasterConfSaveFile(BSTR FileName/*[in]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->MasterConfSaveFile(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::MasterConfUpdateAll(BSTR* Msg/*[out,retval]*/)
{
  return (*this)->MasterConfUpdateAll(Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::MasterConfUpdateAll(void)
{
  BSTR Msg = 0;
  OLECHECK(this->MasterConfUpdateAll((BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::MasterConfUpdateBias(BSTR* Msg/*[out,retval]*/)
{
  return (*this)->MasterConfUpdateBias(Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::MasterConfUpdateBias(void)
{
  BSTR Msg = 0;
  OLECHECK(this->MasterConfUpdateBias((BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::MasterConfReadBack(BSTR* Msg/*[out,retval]*/)
{
  return (*this)->MasterConfReadBack(Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::MasterConfReadBack(void)
{
  BSTR Msg = 0;
  OLECHECK(this->MasterConfReadBack((BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::MasterConfReset(BSTR* Msg/*[out,retval]*/)
{
  return (*this)->MasterConfReset(Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::MasterConfReset(void)
{
  BSTR Msg = 0;
  OLECHECK(this->MasterConfReset((BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::MasterConfStart(BSTR* Msg/*[out,retval]*/)
{
  return (*this)->MasterConfStart(Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::MasterConfStart(void)
{
  BSTR Msg = 0;
  OLECHECK(this->MasterConfStart((BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::MasterConfSetParallelPortAddr(int Addr/*[in]*/, long* Rb_Addr/*[out]*/, 
                                                       BSTR* Msg/*[out,retval]*/)
{
  return (*this)->MasterConfSetParallelPortAddr(Addr, Rb_Addr, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::MasterConfSetParallelPortAddr(int Addr/*[in]*/, long* Rb_Addr/*[out]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->MasterConfSetParallelPortAddr(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetBias(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                             BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfSetBias(RegNum, RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetBias(long RegNum/*[in]*/, long RegValue/*[in]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfSetBias(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetBias(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                             long* Rb_RegValue/*[out]*/, BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfGetBias(RegNum, RegValue, Rb_RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetBias(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                             long* Rb_RegValue/*[out]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfGetBias(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetRoMode(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                               BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfSetRoMode(RegNum, RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetRoMode(long RegNum/*[in]*/, long RegValue/*[in]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfSetRoMode(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetRoMode(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                               BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfGetRoMode(RegNum, RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetRoMode(long RegNum/*[in]*/, long* RegValue/*[out]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfGetRoMode(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetDiscri(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                               BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfSetDiscri(RegNum, RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetDiscri(long RegNum/*[in]*/, long RegValue/*[in]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfSetDiscri(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetDiscri(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                               long* Rb_RegValue/*[out]*/, BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfGetDiscri(RegNum, RegValue, Rb_RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetDiscri(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                               long* Rb_RegValue/*[out]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfGetDiscri(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetLinePat0(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                 BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfSetLinePat0(RegNum, RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetLinePat0(long RegNum/*[in]*/, long RegValue/*[in]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfSetLinePat0(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetLinePat0(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                 long* Rb_RegValue/*[out]*/, 
                                                 BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfGetLinePat0(RegNum, RegValue, Rb_RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetLinePat0(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                 long* Rb_RegValue/*[out]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfGetLinePat0(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetLinePat1(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                 BSTR* Msg/*[in]*/)
{
  return (*this)->Mimosa26ConfSetLinePat1(RegNum, RegValue, Msg);
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetLinePat1(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                 long* Rb_RegValue/*[out]*/, 
                                                 BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfGetLinePat1(RegNum, RegValue, Rb_RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetLinePat1(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                 long* Rb_RegValue/*[out]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfGetLinePat1(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfShowWindow(long ShowWin/*[in]*/, BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfShowWindow(ShowWin, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfShowWindow(long ShowWin/*[in]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfShowWindow(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetCtrl(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                             BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfSetCtrl(RegNum, RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetCtrl(long RegNum/*[in]*/, long RegValue/*[in]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfSetCtrl(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetCtrl(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                             long* Rb_RegValue/*[out]*/, BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfGetCtrl(RegNum, RegValue, Rb_RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetCtrl(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                             long* Rb_RegValue/*[out]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfGetCtrl(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetTest1Pad(long RegValue/*[in]*/, BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfSetTest1Pad(RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetTest1Pad(long RegValue/*[in]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfSetTest1Pad(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetTest1Pad(long* RegValue/*[out]*/, BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfGetTest1Pad(RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetTest1Pad(long* RegValue/*[out]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfGetTest1Pad(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetTest2Pad(long RegValue/*[in]*/, BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfSetTest2Pad(RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetTest2Pad(long RegValue/*[in]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfSetTest2Pad(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetTest2Pad(long* RegValue/*[out]*/, BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfGetTest2Pad(RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetTest2Pad(long* RegValue/*[out]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfGetTest2Pad(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::MasterConfUpdateLinePat(BSTR* Msg/*[out,retval]*/)
{
  return (*this)->MasterConfUpdateLinePat(Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::MasterConfUpdateLinePat(void)
{
  BSTR Msg = 0;
  OLECHECK(this->MasterConfUpdateLinePat((BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::MasterConfSetDevNum(long DevNum/*[in]*/, BSTR* Msg/*[out,retval]*/)
{
  return (*this)->MasterConfSetDevNum(DevNum, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::MasterConfSetDevNum(long DevNum/*[in]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->MasterConfSetDevNum(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::MasterConfGetDevNum(long* DevNum/*[out]*/, BSTR* Msg/*[out,retval]*/)
{
  return (*this)->MasterConfGetDevNum(DevNum, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::MasterConfGetDevNum(long* DevNum/*[out]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->MasterConfGetDevNum(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetHeaderTrailer(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                      BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfSetHeaderTrailer(RegNum, RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetHeaderTrailer(long RegNum/*[in]*/, long RegValue/*[in]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfSetHeaderTrailer(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetHeaderTrailer(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                      long* Rb_RegValue/*[out]*/, 
                                                      BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfGetHeaderTrailer(RegNum, RegValue, Rb_RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetHeaderTrailer(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                      long* Rb_RegValue/*[out]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfGetHeaderTrailer(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetSeqPix(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                               BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfSetSeqPix(RegNum, RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetSeqPix(long RegNum/*[in]*/, long RegValue/*[in]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfSetSeqPix(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetSeqPix(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                               long* Rb_RegValue/*[out]*/, BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfGetSeqPix(RegNum, RegValue, Rb_RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetSeqPix(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                               long* Rb_RegValue/*[out]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfGetSeqPix(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetSeqSuze(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfSetSeqSuze(RegNum, RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetSeqSuze(long RegNum/*[in]*/, long RegValue/*[in]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfSetSeqSuze(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetSeqSuze(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                long* Rb_RegValue/*[out]*/, 
                                                BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfGetSeqSuze(RegNum, RegValue, Rb_RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetSeqSuze(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                long* Rb_RegValue/*[out]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfGetSeqSuze(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetCtrlSuze(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                 BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfSetCtrlSuze(RegNum, RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSetCtrlSuze(long RegNum/*[in]*/, long RegValue/*[in]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfSetCtrlSuze(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetCtrlSuze(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                 long* Rb_RegValue/*[out]*/, 
                                                 BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfGetCtrlSuze(RegNum, RegValue, Rb_RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGetCtrlSuze(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                 long* Rb_RegValue/*[out]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfGetCtrlSuze(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSet8B10BReg0(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                  BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfSet8B10BReg0(RegNum, RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfSet8B10BReg0(long RegNum/*[in]*/, long RegValue/*[in]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfSet8B10BReg0(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGet8B10BReg0(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                  long* Rb_RegValue/*[out]*/, 
                                                  BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfGet8B10BReg0(RegNum, RegValue, Rb_RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGet8B10BReg0(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                  long* Rb_RegValue/*[out]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfGet8B10BReg0(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGet8B10BReg1EncodedData(long RegNum/*[in]*/, 
                                                             long* Rb_RegValue/*[out]*/, 
                                                             BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfGet8B10BReg1EncodedData(RegNum, Rb_RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGet8B10BReg1EncodedData(long RegNum/*[in]*/, 
                                                             long* Rb_RegValue/*[out]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfGet8B10BReg1EncodedData(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGet8B10BReg1Flags(long RegNum/*[in]*/, 
                                                       long* Rb_RegValue/*[out]*/, 
                                                       BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfGet8B10BReg1Flags(RegNum, Rb_RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGet8B10BReg1Flags(long RegNum/*[in]*/, 
                                                       long* Rb_RegValue/*[out]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfGet8B10BReg1Flags(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGet8B10BReg1RawData(long RegNum/*[in]*/, 
                                                         long* Rb_RegValue/*[out]*/, 
                                                         BSTR* Msg/*[out,retval]*/)
{
  return (*this)->Mimosa26ConfGet8B10BReg1RawData(RegNum, Rb_RegValue, Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::Mimosa26ConfGet8B10BReg1RawData(long RegNum/*[in]*/, 
                                                         long* Rb_RegValue/*[out]*/)
{
  BSTR Msg = 0;
  OLECHECK(this->Mimosa26ConfGet8B10BReg1RawData(, (BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::MasterConfSaveAllFile(BSTR* Msg/*[out,retval]*/)
{
  return (*this)->MasterConfSaveAllFile(Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::MasterConfSaveAllFile(void)
{
  BSTR Msg = 0;
  OLECHECK(this->MasterConfSaveAllFile((BSTR*)&Msg));
  return Msg;
}

template <class T> HRESULT __fastcall
TCOMIMI26MasterConfT<T>::MasterConfReadBiasBack(BSTR* Msg/*[out,retval]*/)
{
  return (*this)->MasterConfReadBiasBack(Msg);
}

template <class T> BSTR __fastcall
TCOMIMI26MasterConfT<T>::MasterConfReadBiasBack(void)
{
  BSTR Msg = 0;
  OLECHECK(this->MasterConfReadBiasBack((BSTR*)&Msg));
  return Msg;
}

// *********************************************************************//
// DispIntf :  IMI26MasterConf
// Flags :     (4416) Dual OleAutomation Dispatchable
// GUID :      {485C7AE6-A7C1-4901-8C83-421ABE545994}
// *********************************************************************//
template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::get_Info(BSTR* Value/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Info"), DISPID(1));
  TAutoArgs<0> _args;
  return OutRetValSetterPtr(Value /*[VT_BSTR:1]*/, _args, OlePropertyGet(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::get_Info(void)
{
  BSTR Value;
  this->get_Info((BSTR*)&Value);
  return Value;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::MasterConfLoadFile(BSTR FileName/*[in]*/, BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("MasterConfLoadFile"), DISPID(2));
  TAutoArgs<1> _args;
  _args[1] = FileName /*[VT_BSTR:0]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::MasterConfLoadFile(BSTR FileName/*[in]*/)
{
  BSTR Msg;
  this->MasterConfLoadFile(FileName, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::MasterConfSaveFile(BSTR FileName/*[in]*/, BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("MasterConfSaveFile"), DISPID(3));
  TAutoArgs<1> _args;
  _args[1] = FileName /*[VT_BSTR:0]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::MasterConfSaveFile(BSTR FileName/*[in]*/)
{
  BSTR Msg;
  this->MasterConfSaveFile(FileName, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::MasterConfUpdateAll(BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("MasterConfUpdateAll"), DISPID(4));
  TAutoArgs<0> _args;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::MasterConfUpdateAll(void)
{
  BSTR Msg;
  this->MasterConfUpdateAll((BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::MasterConfUpdateBias(BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("MasterConfUpdateBias"), DISPID(5));
  TAutoArgs<0> _args;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::MasterConfUpdateBias(void)
{
  BSTR Msg;
  this->MasterConfUpdateBias((BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::MasterConfReadBack(BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("MasterConfReadBack"), DISPID(6));
  TAutoArgs<0> _args;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::MasterConfReadBack(void)
{
  BSTR Msg;
  this->MasterConfReadBack((BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::MasterConfReset(BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("MasterConfReset"), DISPID(7));
  TAutoArgs<0> _args;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::MasterConfReset(void)
{
  BSTR Msg;
  this->MasterConfReset((BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::MasterConfStart(BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("MasterConfStart"), DISPID(8));
  TAutoArgs<0> _args;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::MasterConfStart(void)
{
  BSTR Msg;
  this->MasterConfStart((BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::MasterConfSetParallelPortAddr(int Addr/*[in]*/, long* Rb_Addr/*[out]*/, 
                                                       BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("MasterConfSetParallelPortAddr"), DISPID(9));
  TAutoArgs<2> _args;
  _args[1] = Addr /*[VT_INT:0]*/;
  _args[2] = Rb_Addr /*[VT_I4:1]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::MasterConfSetParallelPortAddr(int Addr/*[in]*/, long* Rb_Addr/*[out]*/)
{
  BSTR Msg;
  this->MasterConfSetParallelPortAddr(Addr, Rb_Addr, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetBias(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                             BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfSetBias"), DISPID(10));
  TAutoArgs<2> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:0]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetBias(long RegNum/*[in]*/, long RegValue/*[in]*/)
{
  BSTR Msg;
  this->Mimosa26ConfSetBias(RegNum, RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetBias(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                             long* Rb_RegValue/*[out]*/, BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfGetBias"), DISPID(11));
  TAutoArgs<3> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:1]*/;
  _args[3] = Rb_RegValue /*[VT_I4:1]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetBias(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                             long* Rb_RegValue/*[out]*/)
{
  BSTR Msg;
  this->Mimosa26ConfGetBias(RegNum, RegValue, Rb_RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetRoMode(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                               BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfSetRoMode"), DISPID(12));
  TAutoArgs<2> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:0]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetRoMode(long RegNum/*[in]*/, long RegValue/*[in]*/)
{
  BSTR Msg;
  this->Mimosa26ConfSetRoMode(RegNum, RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetRoMode(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                               BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfGetRoMode"), DISPID(13));
  TAutoArgs<2> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:1]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetRoMode(long RegNum/*[in]*/, long* RegValue/*[out]*/)
{
  BSTR Msg;
  this->Mimosa26ConfGetRoMode(RegNum, RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetDiscri(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                               BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfSetDiscri"), DISPID(14));
  TAutoArgs<2> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:0]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetDiscri(long RegNum/*[in]*/, long RegValue/*[in]*/)
{
  BSTR Msg;
  this->Mimosa26ConfSetDiscri(RegNum, RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetDiscri(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                               long* Rb_RegValue/*[out]*/, BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfGetDiscri"), DISPID(15));
  TAutoArgs<3> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:1]*/;
  _args[3] = Rb_RegValue /*[VT_I4:1]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetDiscri(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                               long* Rb_RegValue/*[out]*/)
{
  BSTR Msg;
  this->Mimosa26ConfGetDiscri(RegNum, RegValue, Rb_RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetLinePat0(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                 BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfSetLinePat0"), DISPID(16));
  TAutoArgs<2> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:0]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetLinePat0(long RegNum/*[in]*/, long RegValue/*[in]*/)
{
  BSTR Msg;
  this->Mimosa26ConfSetLinePat0(RegNum, RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetLinePat0(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                 long* Rb_RegValue/*[out]*/, 
                                                 BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfGetLinePat0"), DISPID(17));
  TAutoArgs<3> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:1]*/;
  _args[3] = Rb_RegValue /*[VT_I4:1]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetLinePat0(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                 long* Rb_RegValue/*[out]*/)
{
  BSTR Msg;
  this->Mimosa26ConfGetLinePat0(RegNum, RegValue, Rb_RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetLinePat1(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                 BSTR* Msg/*[in]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfSetLinePat1"), DISPID(18));
  TAutoArgs<3> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:0]*/;
  _args[3] = Msg /*[VT_BSTR:1]*/;
  return OleFunction(_dispid, _args);
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetLinePat1(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                 long* Rb_RegValue/*[out]*/, 
                                                 BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfGetLinePat1"), DISPID(19));
  TAutoArgs<3> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:1]*/;
  _args[3] = Rb_RegValue /*[VT_I4:1]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetLinePat1(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                 long* Rb_RegValue/*[out]*/)
{
  BSTR Msg;
  this->Mimosa26ConfGetLinePat1(RegNum, RegValue, Rb_RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfShowWindow(long ShowWin/*[in]*/, BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfShowWindow"), DISPID(21));
  TAutoArgs<1> _args;
  _args[1] = ShowWin /*[VT_I4:0]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfShowWindow(long ShowWin/*[in]*/)
{
  BSTR Msg;
  this->Mimosa26ConfShowWindow(ShowWin, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetCtrl(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                             BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfSetCtrl"), DISPID(22));
  TAutoArgs<2> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:0]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetCtrl(long RegNum/*[in]*/, long RegValue/*[in]*/)
{
  BSTR Msg;
  this->Mimosa26ConfSetCtrl(RegNum, RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetCtrl(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                             long* Rb_RegValue/*[out]*/, BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfGetCtrl"), DISPID(23));
  TAutoArgs<3> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:1]*/;
  _args[3] = Rb_RegValue /*[VT_I4:1]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetCtrl(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                             long* Rb_RegValue/*[out]*/)
{
  BSTR Msg;
  this->Mimosa26ConfGetCtrl(RegNum, RegValue, Rb_RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetTest1Pad(long RegValue/*[in]*/, BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfSetTest1Pad"), DISPID(24));
  TAutoArgs<1> _args;
  _args[1] = RegValue /*[VT_I4:0]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetTest1Pad(long RegValue/*[in]*/)
{
  BSTR Msg;
  this->Mimosa26ConfSetTest1Pad(RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetTest1Pad(long* RegValue/*[out]*/, BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfGetTest1Pad"), DISPID(25));
  TAutoArgs<1> _args;
  _args[1] = RegValue /*[VT_I4:1]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetTest1Pad(long* RegValue/*[out]*/)
{
  BSTR Msg;
  this->Mimosa26ConfGetTest1Pad(RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetTest2Pad(long RegValue/*[in]*/, BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfSetTest2Pad"), DISPID(26));
  TAutoArgs<1> _args;
  _args[1] = RegValue /*[VT_I4:0]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetTest2Pad(long RegValue/*[in]*/)
{
  BSTR Msg;
  this->Mimosa26ConfSetTest2Pad(RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetTest2Pad(long* RegValue/*[out]*/, BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfGetTest2Pad"), DISPID(27));
  TAutoArgs<1> _args;
  _args[1] = RegValue /*[VT_I4:1]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetTest2Pad(long* RegValue/*[out]*/)
{
  BSTR Msg;
  this->Mimosa26ConfGetTest2Pad(RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::MasterConfUpdateLinePat(BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("MasterConfUpdateLinePat"), DISPID(20));
  TAutoArgs<0> _args;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::MasterConfUpdateLinePat(void)
{
  BSTR Msg;
  this->MasterConfUpdateLinePat((BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::MasterConfSetDevNum(long DevNum/*[in]*/, BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("MasterConfSetDevNum"), DISPID(28));
  TAutoArgs<1> _args;
  _args[1] = DevNum /*[VT_I4:0]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::MasterConfSetDevNum(long DevNum/*[in]*/)
{
  BSTR Msg;
  this->MasterConfSetDevNum(DevNum, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::MasterConfGetDevNum(long* DevNum/*[out]*/, BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("MasterConfGetDevNum"), DISPID(29));
  TAutoArgs<1> _args;
  _args[1] = DevNum /*[VT_I4:1]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::MasterConfGetDevNum(long* DevNum/*[out]*/)
{
  BSTR Msg;
  this->MasterConfGetDevNum(DevNum, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetHeaderTrailer(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                      BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfSetHeaderTrailer"), DISPID(30));
  TAutoArgs<2> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:0]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetHeaderTrailer(long RegNum/*[in]*/, long RegValue/*[in]*/)
{
  BSTR Msg;
  this->Mimosa26ConfSetHeaderTrailer(RegNum, RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetHeaderTrailer(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                      long* Rb_RegValue/*[out]*/, 
                                                      BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfGetHeaderTrailer"), DISPID(31));
  TAutoArgs<3> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:1]*/;
  _args[3] = Rb_RegValue /*[VT_I4:1]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetHeaderTrailer(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                      long* Rb_RegValue/*[out]*/)
{
  BSTR Msg;
  this->Mimosa26ConfGetHeaderTrailer(RegNum, RegValue, Rb_RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetSeqPix(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                               BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfSetSeqPix"), DISPID(32));
  TAutoArgs<2> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:0]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetSeqPix(long RegNum/*[in]*/, long RegValue/*[in]*/)
{
  BSTR Msg;
  this->Mimosa26ConfSetSeqPix(RegNum, RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetSeqPix(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                               long* Rb_RegValue/*[out]*/, BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfGetSeqPix"), DISPID(33));
  TAutoArgs<3> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:1]*/;
  _args[3] = Rb_RegValue /*[VT_I4:1]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetSeqPix(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                               long* Rb_RegValue/*[out]*/)
{
  BSTR Msg;
  this->Mimosa26ConfGetSeqPix(RegNum, RegValue, Rb_RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetSeqSuze(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfSetSeqSuze"), DISPID(34));
  TAutoArgs<2> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:0]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetSeqSuze(long RegNum/*[in]*/, long RegValue/*[in]*/)
{
  BSTR Msg;
  this->Mimosa26ConfSetSeqSuze(RegNum, RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetSeqSuze(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                long* Rb_RegValue/*[out]*/, 
                                                BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfGetSeqSuze"), DISPID(35));
  TAutoArgs<3> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:1]*/;
  _args[3] = Rb_RegValue /*[VT_I4:1]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetSeqSuze(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                long* Rb_RegValue/*[out]*/)
{
  BSTR Msg;
  this->Mimosa26ConfGetSeqSuze(RegNum, RegValue, Rb_RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetCtrlSuze(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                 BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfSetCtrlSuze"), DISPID(36));
  TAutoArgs<2> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:0]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSetCtrlSuze(long RegNum/*[in]*/, long RegValue/*[in]*/)
{
  BSTR Msg;
  this->Mimosa26ConfSetCtrlSuze(RegNum, RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetCtrlSuze(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                 long* Rb_RegValue/*[out]*/, 
                                                 BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfGetCtrlSuze"), DISPID(37));
  TAutoArgs<3> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:1]*/;
  _args[3] = Rb_RegValue /*[VT_I4:1]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGetCtrlSuze(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                 long* Rb_RegValue/*[out]*/)
{
  BSTR Msg;
  this->Mimosa26ConfGetCtrlSuze(RegNum, RegValue, Rb_RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSet8B10BReg0(long RegNum/*[in]*/, long RegValue/*[in]*/, 
                                                  BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfSet8B10BReg0"), DISPID(38));
  TAutoArgs<2> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:0]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfSet8B10BReg0(long RegNum/*[in]*/, long RegValue/*[in]*/)
{
  BSTR Msg;
  this->Mimosa26ConfSet8B10BReg0(RegNum, RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGet8B10BReg0(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                  long* Rb_RegValue/*[out]*/, 
                                                  BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfGet8B10BReg0"), DISPID(39));
  TAutoArgs<3> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = RegValue /*[VT_I4:1]*/;
  _args[3] = Rb_RegValue /*[VT_I4:1]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGet8B10BReg0(long RegNum/*[in]*/, long* RegValue/*[out]*/, 
                                                  long* Rb_RegValue/*[out]*/)
{
  BSTR Msg;
  this->Mimosa26ConfGet8B10BReg0(RegNum, RegValue, Rb_RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGet8B10BReg1EncodedData(long RegNum/*[in]*/, 
                                                             long* Rb_RegValue/*[out]*/, 
                                                             BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfGet8B10BReg1EncodedData"), DISPID(41));
  TAutoArgs<2> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = Rb_RegValue /*[VT_I4:1]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGet8B10BReg1EncodedData(long RegNum/*[in]*/, 
                                                             long* Rb_RegValue/*[out]*/)
{
  BSTR Msg;
  this->Mimosa26ConfGet8B10BReg1EncodedData(RegNum, Rb_RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGet8B10BReg1Flags(long RegNum/*[in]*/, 
                                                       long* Rb_RegValue/*[out]*/, 
                                                       BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfGet8B10BReg1Flags"), DISPID(42));
  TAutoArgs<2> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = Rb_RegValue /*[VT_I4:1]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGet8B10BReg1Flags(long RegNum/*[in]*/, 
                                                       long* Rb_RegValue/*[out]*/)
{
  BSTR Msg;
  this->Mimosa26ConfGet8B10BReg1Flags(RegNum, Rb_RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGet8B10BReg1RawData(long RegNum/*[in]*/, 
                                                         long* Rb_RegValue/*[out]*/, 
                                                         BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("Mimosa26ConfGet8B10BReg1RawData"), DISPID(40));
  TAutoArgs<2> _args;
  _args[1] = RegNum /*[VT_I4:0]*/;
  _args[2] = Rb_RegValue /*[VT_I4:1]*/;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::Mimosa26ConfGet8B10BReg1RawData(long RegNum/*[in]*/, 
                                                         long* Rb_RegValue/*[out]*/)
{
  BSTR Msg;
  this->Mimosa26ConfGet8B10BReg1RawData(RegNum, Rb_RegValue, (BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::MasterConfSaveAllFile(BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("MasterConfSaveAllFile"), DISPID(43));
  TAutoArgs<0> _args;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::MasterConfSaveAllFile(void)
{
  BSTR Msg;
  this->MasterConfSaveAllFile((BSTR*)&Msg);
  return Msg;
}

template <class T> HRESULT __fastcall
IMI26MasterConfDispT<T>::MasterConfReadBiasBack(BSTR* Msg/*[out,retval]*/)
{
  _TDispID _dispid(*this, OLETEXT("MasterConfReadBiasBack"), DISPID(44));
  TAutoArgs<0> _args;
  return OutRetValSetterPtr(Msg /*[VT_BSTR:1]*/, _args, OleFunction(_dispid, _args));
}

template <class T> BSTR __fastcall
IMI26MasterConfDispT<T>::MasterConfReadBiasBack(void)
{
  BSTR Msg;
  this->MasterConfReadBiasBack((BSTR*)&Msg);
  return Msg;
}

// *********************************************************************//
// Les typedefs suivants exposent les classes (CoCoClassName) qui  
// fournissent les méthodes statiques Create() et CreateRemote(LPWSTR machineName)  
// pour créer une instance d'objet exposé. Ces fonctions peuvent    
// être utilisées par les clients désirant automatiser les CoClasses exposées 
// par cette  bibliothèque de types.                                                          
// *********************************************************************//

// *********************************************************************//
// CREATEUR INTERFACE DEFAUT DE COCLASSE
// CoClasse  : MI26MasterConf
// Interface : TCOMIMI26MasterConf
// *********************************************************************//
typedef TCoClassCreatorT<TCOMIMI26MasterConf, IMI26MasterConf, &CLSID_MI26MasterConf, &IID_IMI26MasterConf> CoMI26MasterConf;
#endif  //   __TLB_NO_INTERFACE_WRAPPERS


};     // namespace Mi26lib_tlb

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace Mi26lib_tlb;
#endif

#pragma option pop

#endif // MI26LIB_TLBH
