; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "diplom.h"
LastPage=0

ClassCount=6
Class1=TDiplomApp
Class2=TDiplomDoc
Class3=TDiplomView
Class4=TMainFrame

ResourceCount=14
Resource1=IDD_ABOUTBOX
Resource2=IDD_ABOUTBOX (English (U.S.))
Resource3=IDD_DLG_DLGBARH (English (U.S.))
Class5=CAboutDlg
Class6=TChildFrame
Resource4=IDR_DIPLOMTYPE
Resource5=IDD_PROPPAGE_FILL
Resource6="IDD_DLG_ALN"
Resource7=IDR_MAINFRAME
Resource8=IDD_DLG_BRIG
Resource9=IDD_PROPPAGE_PC
Resource10=IDD_DLG_ALN
Resource11=IDD_PROPPAGE_LINE
Resource12=IDD_DLG_SCALE
Resource13=IDD_DIALOG_SCSUB
Resource14=IDD_PROPPAGE_TXT

[CLS:TDiplomApp]
Type=0
HeaderFile=diplom.h
ImplementationFile=diplom.cpp
Filter=W
LastObject=ID_APP_EXIT
BaseClass=CWinApp
VirtualFilter=AC

[CLS:TDiplomDoc]
Type=0
HeaderFile=diplomDoc.h
ImplementationFile=diplomDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC

[CLS:TDiplomView]
Type=0
HeaderFile=diplomView.h
ImplementationFile=diplomView.cpp
Filter=W
BaseClass=CScrollView
VirtualFilter=VWC

[CLS:TMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CMDIFrameWnd
VirtualFilter=fWC
LastObject=ID_APP_EXIT


[CLS:TChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=W
BaseClass=CMDIChildWnd
VirtualFilter=mfWC
LastObject=ID_APP_EXIT

[CLS:CAboutDlg]
Type=0
HeaderFile=diplom.cpp
ImplementationFile=diplom.cpp
Filter=T
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=?
CommandCount=0

[MNU:IDR_DIPLOMTYPE]
Type=1
Class=?
CommandCount=0

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_MODE_BRIG
Command2=ID_WINDOW_CASCADE
Command3=ID_EDIT_DUP
Command4=ID_EDIT_CLEAR
Command5=ID_CONTEXT_HELP
Command6=ID_WINDOW_TILE_HORZ
Command7=ID_FILE_NEW
Command8=ID_FILE_OPEN
Command9=ID_FILE_PRINT
Command10=ID_WINDOW_ARRANGE
Command11=ID_FILE_SAVE
Command12=ID_ED_UNHILITE
Command13=ID_WINDOW_TILE_VERT
Command14=ID_EDIT_REDO
Command15=ID_EDIT_UNDO
Command16=ID_EDIT_CUT
Command17=ICM_ED_STOP
Command18=ID_CONTEXT_HELP
Command19=ICM_ED_SINGLESTEP
Command20=ID_FILE_SAVE
Command21=ID_FILE_OPEN
Command22=ID_FILE_CLOSE
Command23=ICM_ED_STEPMODE
Command24=ID_NEXT_PANE
Command25=ID_PRIV_PANE
Command26=ID_EDIT_COPY
Command27=ID_EDIT_PASTE
Command28=ID_EDIT_CUT
Command29=ID_APP_EXIT
Command30=ID_EDIT_UNDO
CommandCount=30

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DLG_DLGBARH (English (U.S.))]
Type=1
Class=?
ControlCount=0

[DLG:"IDD_DLG_ALN"]
Type=1
ControlCount=25
Control1=IDC_RAD_A,button,1342373897
Control2=IDC_RAD_D,button,1342242825
Control3=IDC_BORSHADE3,BorShade,1342341126
Control4=IDC_BTN_A1_H,button,1342378121
Control5=IDC_BTN_A2_H,button,1342247049
Control6=IDC_BTN_A3_H,button,1342247049
Control7=IDC_BTN_A4_H,button,1342247049
Control8=IDC_GROUPBOX2,button,1342308359
Control9=IDC_BTN_A1_V,button,1342378121
Control10=IDC_BTN_A2_V,button,1342247049
Control11=IDC_BTN_A3_V,button,1342247049
Control12=IDC_BTN_A4_V,button,1342247049
Control13=IDC_GROUPBOX3,button,1342308359
Control14=IDC_BTN_D1_H,button,1342378121
Control15=IDC_BTN_D2_H,button,1342247049
Control16=IDC_BTN_D3_H,button,1342247049
Control17=IDC_GROUPBOX4,button,1342308359
Control18=IDC_BTN_D1_V,button,1342378121
Control19=IDC_BTN_D2_V,button,1342247049
Control20=IDC_BTN_D3_V,button,1342247049
Control21=IDOK,BorBtn,1342242817
Control22=IDCANCEL,BorBtn,1342242816
Control23=IDC_GROUPBOX1,button,1342308359
Control24=IDC_BORSHADE1,BorShade,1342341126
Control25=IDC_BORSHADE4,BorShade,1342341126

[DLG:IDD_DLG_ALN]
Type=1
Class=?
ControlCount=25
Control1=IDC_RAD_A,button,1342373897
Control2=IDC_RAD_D,button,1342242825
Control3=IDC_BORSHADE3,BorShade,1342341126
Control4=IDC_GROUPBOX1,button,1342308359
Control5=IDC_BTN_A1_H,button,1342378121
Control6=IDC_BTN_A2_H,button,1342247049
Control7=IDC_BTN_A3_H,button,1342247049
Control8=IDC_BTN_A4_H,button,1342247049
Control9=IDC_GROUPBOX2,button,1342308359
Control10=IDC_BTN_A1_V,button,1342378121
Control11=IDC_BTN_A2_V,button,1342247049
Control12=IDC_BTN_A3_V,button,1342247049
Control13=IDC_BTN_A4_V,button,1342247049
Control14=IDC_GROUPBOX3,button,1342308359
Control15=IDC_BTN_D1_H,button,1342378121
Control16=IDC_BTN_D2_H,button,1342247049
Control17=IDC_BTN_D3_H,button,1342247049
Control18=IDC_BTN_D1_V,button,1342247049
Control19=IDC_BTN_D2_V,button,1342247049
Control20=IDC_BTN_D3_V,button,1342247049
Control21=IDC_BORSHADE4,BorShade,1342341126
Control22=IDOK,BorBtn,1342242817
Control23=IDCANCEL,BorBtn,1342242816
Control24=IDC_BORSHADE1,BorShade,1342341126
Control25=IDC_GROUPBOX4,button,1342308359

[DLG:IDD_PROPPAGE_PC]
Type=1
Class=?
ControlCount=15
Control1=IDC_ED_RANG,edit,1350639744
Control2=IDC_SPIN_PASS,msctls_updown32,1342177334
Control3=IDC_ED_NUMBER,edit,1350639744
Control4=IDC_SPIN_PASS2,msctls_updown32,1342177334
Control5=IDC_STATIC,button,1342177287
Control6=IDC_ED_L,edit,1350631552
Control7=IDC_ED_M,edit,1350631552
Control8=IDC_ED_D,edit,1350631552
Control9=IDC_ED_NAME,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352

[DLG:IDD_PROPPAGE_TXT]
Type=1
Class=?
ControlCount=3
Control1=IDC_ED_TEXT,edit,1350635716
Control2=IDC_STATIC,static,1342308352
Control3=IDC_CHECK1,button,1342242819

[DLG:IDD_PROPPAGE_LINE]
Type=1
Class=?
ControlCount=7
Control1=IDC_COMBO_PATTERN,combobox,1344339987
Control2=IDC_COMBO_WEIGHT,combobox,1344339987
Control3=IDC_COMBO_CLR,combobox,1344339987
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_BTN_CLREXT,button,1342246656

[DLG:IDD_PROPPAGE_FILL]
Type=1
Class=?
ControlCount=8
Control1=IDC_COMBO_PATTERN,combobox,1344339987
Control2=IDC_COMBO_FONCLR,combobox,1344339987
Control3=IDC_COMBO_CLR,combobox,1344339987
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_BTN_CLREXT,button,1342246656
Control8=IDC_BTN_CLREXTBK,button,1342246656

[DLG:IDD_DLG_BRIG]
Type=1
Class=?
ControlCount=9
Control1=IDC_RAD_PRIOR,button,1342308361
Control2=IDC_RAD_NOPRIOR,button,1342177289
Control3=IDC_RAD_LABELED,button,1342177289
Control4=IDC_STATIC,button,1342308359
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_NUMBER,edit,1350631552
Control7=IDC_SPIN1,msctls_updown32,1342177334
Control8=IDOK,button,1342242817
Control9=IDCANCEL,button,1342242816

[DLG:IDD_DLG_SCALE]
Type=1
ControlCount=12
Control1=IDC_RADIO1,button,1342308361
Control2=IDC_RADIO2,button,1342177289
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_SLIDER1,msctls_trackbar32,1342242817
Control7=IDC_STATIC,button,1342177287
Control8=IDC_RAD_SELF,button,1342308361
Control9=IDC_RAD_GLOB,button,1342177289
Control10=IDC_STATIC,button,1342308359
Control11=IDOK,button,1342242817
Control12=IDCANCEL,button,1342242816

[DLG:IDD_DIALOG_SCSUB]
Type=1
ControlCount=6
Control1=IDC_LIST_DP,listbox,1353777419
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

