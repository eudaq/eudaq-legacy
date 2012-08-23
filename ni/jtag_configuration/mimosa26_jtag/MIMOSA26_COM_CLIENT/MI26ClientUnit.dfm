object FormMI26Client: TFormMI26Client
  Left = 447
  Top = 177
  Width = 521
  Height = 921
  Caption = 'COM CLIENT EXAMPLE FOR MIMOSA26 8B10B TEST'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object LabelMasterConfLoadFileName: TLabel
    Left = 16
    Top = 8
    Width = 107
    Height = 12
    Caption = 'Master Configuration File:'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object LabelMasterConfSaveFileName: TLabel
    Left = 16
    Top = 48
    Width = 107
    Height = 12
    Caption = 'Master Configuration File:'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object LabelMasterConfParallelPortAddr: TLabel
    Left = 16
    Top = 128
    Width = 89
    Height = 12
    Caption = 'Parallel Port Address:'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object LabelBiasReg: TLabel
    Left = 16
    Top = 240
    Width = 60
    Height = 12
    Caption = 'Bias Registers'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object LabelMasterConfBiasRegNum: TLabel
    Left = 152
    Top = 216
    Width = 70
    Height = 12
    Caption = 'Register Number'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object LabelMasterConfBiasRegValue: TLabel
    Left = 240
    Top = 216
    Width = 61
    Height = 12
    Caption = 'Register Value'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object LabelRomode: TLabel
    Left = 16
    Top = 280
    Width = 98
    Height = 12
    Caption = 'Readout Mode Register'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object LabelDiscri: TLabel
    Left = 16
    Top = 320
    Width = 92
    Height = 12
    Caption = 'Discriminator Register'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object LabelLinePat0: TLabel
    Left = 16
    Top = 360
    Width = 93
    Height = 12
    Caption = 'Line Pattern Register 0'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object LabelLinePat1: TLabel
    Left = 16
    Top = 400
    Width = 93
    Height = 12
    Caption = 'Line Pattern Register 1'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object LabelCtrlPix: TLabel
    Left = 16
    Top = 440
    Width = 82
    Height = 12
    Caption = 'Control Pix Register'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object LabelMasterConfTest1Pad: TLabel
    Left = 16
    Top = 160
    Width = 80
    Height = 12
    Caption = 'Test Pad 1 Register'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object LabelMasterConfTest2Pad: TLabel
    Left = 240
    Top = 168
    Width = 80
    Height = 12
    Caption = 'Test Pad 2 Register'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object LabelDevNum: TLabel
    Left = 16
    Top = 96
    Width = 65
    Height = 12
    Caption = 'Device Number'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object LabelConnectStatus: TLabel
    Left = 16
    Top = 856
    Width = 27
    Height = 12
    Caption = 'Status'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object LabelMasterConfHeaderTrailer: TLabel
    Left = 16
    Top = 480
    Width = 94
    Height = 12
    Caption = 'Header Trailer Register'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object LabelMasterConfCtrlSuze: TLabel
    Left = 16
    Top = 520
    Width = 90
    Height = 12
    Caption = 'Control Suze Register'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object LabelMasterConfSeqSuze: TLabel
    Left = 16
    Top = 560
    Width = 104
    Height = 12
    Caption = 'Sequencer Suze Register'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object LabelMasterConfCtrl8B10BReg0: TLabel
    Left = 16
    Top = 600
    Width = 103
    Height = 12
    Caption = 'Control 8B10B Register 0'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object LabelMasterConf8B10BReg1EncodedData: TLabel
    Left = 16
    Top = 680
    Width = 147
    Height = 12
    Caption = 'Control 8B10B Register 1 (Encoded)'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label1: TLabel
    Left = 16
    Top = 640
    Width = 153
    Height = 12
    Caption = 'Control 8B10B Register 1 (Raw Data)'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label12: TLabel
    Left = 16
    Top = 720
    Width = 134
    Height = 12
    Caption = 'Control 8B10B Register 1 (Flags)'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Edit1: TEdit
    Left = 64
    Top = 856
    Width = 321
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
  end
  object ButtonInfo: TButton
    Left = 8
    Top = 816
    Width = 73
    Height = 25
    Caption = 'Connect'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = ButtonInfoClick
  end
  object ButtonLoadFile: TButton
    Left = 408
    Top = 24
    Width = 73
    Height = 25
    Caption = 'Load File'
    Enabled = False
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = ButtonLoadFileClick
  end
  object EditLoadFileName: TEdit
    Left = 16
    Top = 24
    Width = 337
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
  end
  object ButtonSaveFile: TButton
    Left = 408
    Top = 64
    Width = 89
    Height = 25
    Caption = 'Save File'
    Enabled = False
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    OnClick = ButtonSaveFileClick
  end
  object EditSaveFileName: TEdit
    Left = 16
    Top = 64
    Width = 337
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
  end
  object ButtonReset: TButton
    Left = 88
    Top = 816
    Width = 65
    Height = 25
    Caption = 'Reset'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    OnClick = ButtonResetClick
  end
  object ButtonStart: TButton
    Left = 160
    Top = 816
    Width = 73
    Height = 25
    Caption = 'Start'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 7
    OnClick = ButtonStartClick
  end
  object ButtonRead: TButton
    Left = 248
    Top = 816
    Width = 73
    Height = 25
    Caption = 'Read'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 8
    OnClick = ButtonReadClick
  end
  object ButtonUpdateBias: TButton
    Left = 336
    Top = 816
    Width = 73
    Height = 25
    Caption = 'Update Bias'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 9
    OnClick = ButtonUpdateBiasClick
  end
  object ButtonUpdateAll: TButton
    Left = 416
    Top = 816
    Width = 81
    Height = 25
    Caption = 'Update All'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 10
    OnClick = ButtonUpdateAllClick
  end
  object Memo1: TMemo
    Left = 16
    Top = 752
    Width = 481
    Height = 49
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    Lines.Strings = (
      '')
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 11
  end
  object ButtonFileLoadDialog: TButton
    Left = 360
    Top = 24
    Width = 33
    Height = 25
    Caption = '...'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 12
    OnClick = ButtonFileLoadDialogClick
  end
  object ButtonFileSaveDialog: TButton
    Left = 360
    Top = 64
    Width = 33
    Height = 25
    Caption = '...'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 13
    OnClick = ButtonFileSaveDialogClick
  end
  object EditParallelPortAddr: TEdit
    Left = 120
    Top = 128
    Width = 33
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 14
    Text = '378'
  end
  object ButtonPortParallelAddrUpdate: TButton
    Left = 200
    Top = 128
    Width = 81
    Height = 25
    Caption = 'Update  Address'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 15
    OnClick = ButtonPortParallelAddrUpdateClick
  end
  object EditBiasRegNum: TEdit
    Left = 176
    Top = 240
    Width = 25
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 16
    Text = '0'
  end
  object EditBiasRegValue: TEdit
    Left = 240
    Top = 240
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 17
    Text = '0'
  end
  object ButtonBiasRegWrite: TButton
    Left = 384
    Top = 240
    Width = 40
    Height = 25
    Caption = 'Write'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 18
    OnClick = ButtonBiasRegWriteClick
  end
  object ButtonRegRead: TButton
    Left = 432
    Top = 240
    Width = 40
    Height = 25
    Caption = 'Read'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 19
    OnClick = ButtonRegReadClick
  end
  object EditRomodeRegNum: TEdit
    Left = 176
    Top = 280
    Width = 25
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 20
    Text = '0'
  end
  object EditRomodeRegValue: TEdit
    Left = 240
    Top = 280
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 21
    Text = '0'
  end
  object ButtonRomodeRegWrite: TButton
    Left = 384
    Top = 280
    Width = 40
    Height = 25
    Caption = 'Write'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 22
    OnClick = ButtonRomodeRegWriteClick
  end
  object ButtonRomodeRegRead: TButton
    Left = 432
    Top = 280
    Width = 40
    Height = 25
    Caption = 'Read'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 23
    OnClick = ButtonRomodeRegReadClick
  end
  object EditDiscriRegNum: TEdit
    Left = 176
    Top = 320
    Width = 25
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 24
    Text = '0'
  end
  object EditDiscriRegValue: TEdit
    Left = 240
    Top = 320
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 25
    Text = '0'
  end
  object ButtonDiscriRegWrite: TButton
    Left = 384
    Top = 320
    Width = 40
    Height = 25
    Caption = 'Write'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 26
    OnClick = ButtonDiscriRegWriteClick
  end
  object ButtonDiscriRegRead: TButton
    Left = 432
    Top = 320
    Width = 40
    Height = 25
    Caption = 'Read'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 27
    OnClick = ButtonDiscriRegReadClick
  end
  object EditLinePat0RegNum: TEdit
    Left = 176
    Top = 360
    Width = 25
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 28
    Text = '0'
  end
  object EditLinePat0RegValue: TEdit
    Left = 240
    Top = 360
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 29
    Text = '0'
  end
  object ButtonLinePat0RegWrite: TButton
    Left = 384
    Top = 360
    Width = 40
    Height = 25
    Caption = 'Write'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 30
    OnClick = ButtonLinePat0RegWriteClick
  end
  object ButtonPat0RegRead: TButton
    Left = 432
    Top = 360
    Width = 40
    Height = 25
    Caption = 'Read'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 31
    OnClick = ButtonPat0RegReadClick
  end
  object EditLinePat1RegNum: TEdit
    Left = 176
    Top = 400
    Width = 25
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 32
    Text = '0'
  end
  object EditLinePat1RegValue: TEdit
    Left = 240
    Top = 400
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 33
    Text = '0'
  end
  object ButtonLinePat1RegWrite: TButton
    Left = 384
    Top = 400
    Width = 40
    Height = 25
    Caption = 'Write'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 34
    OnClick = ButtonLinePat1RegWriteClick
  end
  object ButtonLinePat1RegRead: TButton
    Left = 432
    Top = 400
    Width = 40
    Height = 25
    Caption = 'Read'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 35
    OnClick = ButtonLinePat1RegReadClick
  end
  object EditCtrlPixRegNum: TEdit
    Left = 176
    Top = 440
    Width = 25
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 36
    Text = '0'
  end
  object EditCtrlPixRegValue: TEdit
    Left = 240
    Top = 440
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 37
    Text = '0'
  end
  object ButtonCtrlRegWrite: TButton
    Left = 384
    Top = 440
    Width = 40
    Height = 25
    Caption = 'Write'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 38
    OnClick = ButtonCtrlRegWriteClick
  end
  object ButtonCtrlRegRead: TButton
    Left = 432
    Top = 440
    Width = 40
    Height = 25
    Caption = 'Read'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 39
    OnClick = ButtonCtrlRegReadClick
  end
  object EditBiasRegValueRb: TEdit
    Left = 312
    Top = 240
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 40
    Text = '0'
  end
  object EditRomodeRegValueRb: TEdit
    Left = 312
    Top = 280
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 41
    Text = '0'
  end
  object EditDiscriRegValueRb: TEdit
    Left = 312
    Top = 320
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 42
    Text = '0'
  end
  object EditLinePat0RegValueRb: TEdit
    Left = 312
    Top = 360
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 43
    Text = '0'
  end
  object EditLinePat1RegValueRb: TEdit
    Left = 312
    Top = 400
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 44
    Text = '0'
  end
  object EditCtrlPixRegValueRb: TEdit
    Left = 312
    Top = 440
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 45
    Text = '0'
  end
  object EditParallelPortAddrRb: TEdit
    Left = 160
    Top = 128
    Width = 33
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 46
    Text = '378'
  end
  object EditTest1PadRegValue: TEdit
    Left = 32
    Top = 176
    Width = 25
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 47
    Text = '0'
  end
  object ButtonTest1PadRegWrite: TButton
    Left = 104
    Top = 176
    Width = 40
    Height = 25
    Caption = 'Write'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 48
    OnClick = ButtonTest1PadRegWriteClick
  end
  object ButtonTest1PadRegRead: TButton
    Left = 152
    Top = 176
    Width = 40
    Height = 25
    Caption = 'Read'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 49
    OnClick = ButtonTest1PadRegReadClick
  end
  object EditTest1PadRegValueRb: TEdit
    Left = 64
    Top = 176
    Width = 25
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 50
    Text = '0'
  end
  object EditTest2PadRegValue: TEdit
    Left = 248
    Top = 184
    Width = 25
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 51
    Text = '0'
  end
  object ButtonTest2PadRegWrite: TButton
    Left = 328
    Top = 184
    Width = 40
    Height = 25
    Caption = 'Write'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 52
    OnClick = ButtonTest2PadRegWriteClick
  end
  object ButtonTest2PadRegRead: TButton
    Left = 376
    Top = 184
    Width = 40
    Height = 25
    Caption = 'Read'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 53
    OnClick = ButtonTest2PadRegReadClick
  end
  object EditTest2PadRegValueRb: TEdit
    Left = 288
    Top = 184
    Width = 25
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 54
    Text = '0'
  end
  object EditDevNum: TEdit
    Left = 120
    Top = 96
    Width = 25
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 55
    Text = '0'
  end
  object ButtonDevNumSelect: TButton
    Left = 184
    Top = 96
    Width = 65
    Height = 25
    Caption = 'Select Device'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 56
    OnClick = ButtonDevNumSelectClick
  end
  object EditDevNumRb: TEdit
    Left = 152
    Top = 96
    Width = 25
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 57
    Text = '0'
  end
  object ButtonDevNumRead: TButton
    Left = 256
    Top = 96
    Width = 57
    Height = 25
    Caption = 'Get Device'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 58
    OnClick = ButtonDevNumReadClick
  end
  object EditHeaderTrailerRegNum: TEdit
    Left = 176
    Top = 480
    Width = 25
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 59
    Text = '0'
  end
  object EditHeaderTrailerRegValue: TEdit
    Left = 240
    Top = 480
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 60
    Text = '0'
  end
  object ButtonHeaderTrailerRegWrite: TButton
    Left = 384
    Top = 480
    Width = 40
    Height = 25
    Caption = 'Write'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 61
    OnClick = ButtonHeaderTrailerRegWriteClick
  end
  object ButtonHeaderTrailerRegRead: TButton
    Left = 432
    Top = 480
    Width = 40
    Height = 25
    Caption = 'Read'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 62
    OnClick = ButtonHeaderTrailerRegReadClick
  end
  object EditHeaderTrailerRegValueRb: TEdit
    Left = 312
    Top = 480
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 63
    Text = '0'
  end
  object EditCtrlSuzeRegNum: TEdit
    Left = 176
    Top = 520
    Width = 25
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 64
    Text = '0'
  end
  object EditCtrlSuzeRegValue: TEdit
    Left = 240
    Top = 520
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 65
    Text = '0'
  end
  object ButtonCtrlSuzeRegWrite: TButton
    Left = 384
    Top = 520
    Width = 40
    Height = 25
    Caption = 'Write'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 66
    OnClick = ButtonCtrlSuzeRegWriteClick
  end
  object ButtonCtrlSuzeRegRead: TButton
    Left = 432
    Top = 520
    Width = 40
    Height = 25
    Caption = 'Read'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 67
    OnClick = ButtonCtrlSuzeRegReadClick
  end
  object EditCtrlSuzeRegValueRb: TEdit
    Left = 312
    Top = 520
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 68
    Text = '0'
  end
  object EditSeqSuzeRegNum: TEdit
    Left = 176
    Top = 560
    Width = 25
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 69
    Text = '0'
  end
  object EditSeqSuzeRegValue: TEdit
    Left = 240
    Top = 560
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 70
    Text = '0'
  end
  object ButtonSeqSuzeRegWrite: TButton
    Left = 384
    Top = 560
    Width = 40
    Height = 25
    Caption = 'Write'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 71
    OnClick = ButtonSeqSuzeRegWriteClick
  end
  object ButtonSeqSuzeRegRead: TButton
    Left = 432
    Top = 560
    Width = 40
    Height = 25
    Caption = 'Read'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 72
    OnClick = ButtonSeqSuzeRegReadClick
  end
  object EditSeqSuzeRegValueRb: TEdit
    Left = 312
    Top = 560
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 73
    Text = '0'
  end
  object EditCtrl8B10BReg0RegNum: TEdit
    Left = 176
    Top = 600
    Width = 25
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 74
    Text = '0'
  end
  object EditCtrl8B10BReg0RegValue: TEdit
    Left = 240
    Top = 600
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 75
    Text = '0'
  end
  object ButtonCtrl8B10BReg0RegWrite: TButton
    Left = 384
    Top = 600
    Width = 40
    Height = 25
    Caption = 'Write'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 76
    OnClick = ButtonCtrl8B10BReg0RegWriteClick
  end
  object ButtonCtrl8B10BReg0RegRead: TButton
    Left = 432
    Top = 600
    Width = 40
    Height = 25
    Caption = 'Read'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 77
    OnClick = ButtonCtrl8B10BReg0RegReadClick
  end
  object EditCtrl8B10BReg0RegValueRb: TEdit
    Left = 312
    Top = 600
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 78
    Text = '0'
  end
  object EditCtrl8B10BReg1EncodedDataRegNum: TEdit
    Left = 176
    Top = 680
    Width = 25
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 79
    Text = '0'
  end
  object ButtonCtrl8B10BReg1EncodedDataRegRead: TButton
    Left = 384
    Top = 680
    Width = 65
    Height = 25
    Caption = 'Read'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 80
    OnClick = ButtonCtrl8B10BReg1EncodedDataRegReadClick
  end
  object EditCtrl8B10BReg1EncodedDataRegValueRb: TEdit
    Left = 240
    Top = 680
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 81
    Text = '0'
  end
  object EditCtrl8B10BReg1RawDataRegNum: TEdit
    Left = 176
    Top = 640
    Width = 25
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 82
    Text = '0'
  end
  object ButtonCtrl8B10BReg1RawDataRegRead: TButton
    Left = 384
    Top = 640
    Width = 65
    Height = 25
    Caption = 'Read'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 83
    OnClick = ButtonCtrl8B10BReg1RawDataRegReadClick
  end
  object EditCtrl8B10BReg1RawDataRegValueRb: TEdit
    Left = 240
    Top = 640
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 84
    Text = '0'
  end
  object ButtonCtrl8B10BReg1FlagsRegRead: TButton
    Left = 384
    Top = 720
    Width = 65
    Height = 25
    Caption = 'Read'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 85
    OnClick = ButtonCtrl8B10BReg1FlagsRegReadClick
  end
  object EditCtrl8B10BReg1FlagsRegValueRb: TEdit
    Left = 240
    Top = 720
    Width = 65
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 86
    Text = '0'
  end
  object EditCtrl8B10BReg1FlagsRegNum: TEdit
    Left = 176
    Top = 720
    Width = 25
    Height = 20
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 87
    Text = '0'
  end
  object ButtonSaveAll: TButton
    Left = 408
    Top = 104
    Width = 89
    Height = 25
    Caption = 'Save File'
    Enabled = False
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 88
    OnClick = ButtonSaveAllClick
  end
  object ButtonReadBias: TButton
    Left = 424
    Top = 856
    Width = 73
    Height = 25
    Caption = 'Read Bias'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 89
    OnClick = ButtonReadBiasClick
  end
  object OpenDialog: TOpenDialog
    Left = 336
    Top = 136
  end
  object SaveDialog: TSaveDialog
    Left = 336
    Top = 104
  end
end
