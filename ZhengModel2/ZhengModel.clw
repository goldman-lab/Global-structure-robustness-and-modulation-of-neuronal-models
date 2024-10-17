; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CNeuronPage
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ZhengModel.h"
LastPage=0

ClassCount=11
Class1=CZhengModelApp
Class2=CZhengModelDoc
Class3=CZhengModelView
Class4=CMainFrame

ResourceCount=7
Resource1=IDD_DISPLAYPAGE
Resource2=IDD_NEURONPAGE
Resource3=IDR_ZHENGMTYPE
Class5=CAboutDlg
Class6=CChildFrame
Class7=CTabCtrls
Resource4=IDD_RUNPAGE
Resource5=IDR_MAINFRAME
Class8=CDisplayPage
Class9=CNeuronPage
Class10=CRunPage
Resource6=IDD_ABOUTBOX
Class11=CPlotsDialog
Resource7=IDD_PLOTSDIALOG

[CLS:CZhengModelApp]
Type=0
HeaderFile=ZhengModel.h
ImplementationFile=ZhengModel.cpp
Filter=N

[CLS:CZhengModelDoc]
Type=0
HeaderFile=ZhengModelDoc.h
ImplementationFile=ZhengModelDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC

[CLS:CZhengModelView]
Type=0
HeaderFile=ZhengModelView.h
ImplementationFile=ZhengModelView.cpp
Filter=W
BaseClass=CView
VirtualFilter=VWC
LastObject=CZhengModelView

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CMDIFrameWnd
VirtualFilter=fWC
LastObject=CMainFrame


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M

[CLS:CAboutDlg]
Type=0
HeaderFile=ZhengModel.cpp
ImplementationFile=ZhengModel.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_ZHENGMTYPE]
Type=1
Class=CZhengModelView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_WINDOW_NEW
Command18=ID_WINDOW_CASCADE
Command19=ID_WINDOW_TILE_HORZ
Command20=ID_WINDOW_ARRANGE
Command21=ID_APP_ABOUT
CommandCount=21

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[CLS:CTabCtrls]
Type=0
HeaderFile=TabCtrls.h
ImplementationFile=TabCtrls.cpp
BaseClass=CPropertySheet
Filter=W
LastObject=CTabCtrls

[DLG:IDD_RUNPAGE]
Type=1
Class=CRunPage
ControlCount=38
Control1=IDC_STATIC,static,1342308352
Control2=IDC_DTMIN_EDIT,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_TMAX_EDIT,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_SAVE_RADIO,button,1342308361
Control7=IDC_SAVE_RADIOdummy,button,1342177289
Control8=IDC_STATIC,static,1342308352
Control9=IDC_DISPLAY_RADIO,button,1342308361
Control10=IDC_DISPLAY_RADIOdummy,button,1342177289
Control11=IDC_STATIC,static,1342308352
Control12=IDC_MAX_INJECT_SOMA_EDIT,edit,1350631552
Control13=IDC_STATIC,static,1342177280
Control14=IDC_INJECT_ON_EDIT,edit,1350631552
Control15=IDC_STATIC,static,1342177280
Control16=IDC_INJECT_OFF_EDIT,edit,1350631552
Control17=IDC_RUN_BUTTON,button,1342177280
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_END_RUN_BUTTON,button,1342242816
Control24=IDC_PAUSE_BUTTON,button,1342242816
Control25=IDC_MAX_INJECT_HILLOCK_EDIT,edit,1350631552
Control26=IDC_STATIC,static,1342308352
Control27=IDC_STATIC,static,1342308352
Control28=IDC_STATIC,static,1342308352
Control29=IDC_DTMAX_EDIT,edit,1350631552
Control30=IDC_STATIC,static,1342308352
Control31=IDC_APPLY_BUTT,button,1342242817
Control32=IDC_STATIC,static,1342177280
Control33=IDC_INJECT_ON_EDIT2,edit,1350631552
Control34=IDC_STATIC,static,1342308352
Control35=IDC_STATIC,static,1342308352
Control36=IDC_MAX_INJECT_SOMA_EDIT2,edit,1350631552
Control37=IDC_STATIC,static,1342308352
Control38=IDC_MAX_INJECT_HILLOCK_EDIT2,edit,1350631552

[DLG:IDD_NEURONPAGE]
Type=1
Class=CNeuronPage
ControlCount=114
Control1=IDC_RESISTIVITY_EDIT,edit,1350631552
Control2=IDC_COMPART_COMBO,combobox,1344339971
Control3=IDC_C_EDIT,edit,1350631552
Control4=IDC_RADIUS_EDIT,edit,1350631552
Control5=IDC_LENGTH_EDIT,edit,1350631552
Control6=IDC_INIT_V_EDIT,edit,1350631552
Control7=IDC_INIT_Ca_EDIT,edit,1350631552
Control8=IDC_CHAN_COMBO,combobox,1344339971
Control9=IDC_E_EDIT,edit,1350631552
Control10=IDC_CHAN_NUM_M_EDIT,edit,1350631552
Control11=IDC_CHAN_NUM_H_EDIT,edit,1350631552
Control12=IDC_SENS_COMBO,combobox,1344339971
Control13=IDC_SENS_VALUEMAX_EDIT,edit,1350631552
Control14=IDC_SENS_TAU_M_EDIT,edit,1350631552
Control15=IDC_SENS_TAU_H_EDIT,edit,1350631552
Control16=IDC_SENS_Z_M_EDIT,edit,1350631552
Control17=IDC_SENS_Z_H_EDIT,edit,1350631552
Control18=IDC_SENS_NUM_M_EDIT,edit,1350631552
Control19=IDC_SENS_NUM_H_EDIT,edit,1350631552
Control20=IDC_Leak_GMAX0_EDIT,edit,1350631552
Control21=IDC_Na_GMAX0_EDIT,edit,1350631552
Control22=IDC_CaT_GMAX0_EDIT,edit,1350631552
Control23=IDC_CaP_GMAX0_EDIT,edit,1350631552
Control24=IDC_A_GMAX0_EDIT,edit,1350631552
Control25=IDC_KCa_GMAX0_EDIT,edit,1350631552
Control26=IDC_Kd_GMAX0_EDIT,edit,1350631552
Control27=IDC_h_GMAX0_EDIT,edit,1350631552
Control28=IDC_DC_TARGET_EDIT,edit,1350631552
Control29=IDC_SLOW_TARGET_EDIT,edit,1350631552
Control30=IDC_FAST_TARGET_EDIT,edit,1350631552
Control31=IDC_APPLY_BUTT,button,1342242817
Control32=IDC_STATIC,static,1342308352
Control33=IDC_STATIC,button,1342177287
Control34=IDC_STATIC,static,1342308352
Control35=IDC_REMOVE_CHAN_BUTT,button,1342242816
Control36=IDC_STATIC,static,1342308352
Control37=IDC_STATIC,static,1342308352
Control38=IDC_STATIC,static,1342308352
Control39=IDC_STATIC,static,1342308352
Control40=IDC_STATIC,static,1342308352
Control41=IDC_STATIC,static,1342308352
Control42=IDC_STATIC,static,1342308352
Control43=IDC_STATIC,static,1342308352
Control44=IDC_STATIC,static,1342308352
Control45=IDC_STATIC,static,1342308352
Control46=IDC_STATIC,static,1342308352
Control47=IDC_STATIC,static,1342308352
Control48=IDC_STATIC,button,1342177287
Control49=IDC_STATIC,static,1342308352
Control50=IDC_STATIC,static,1342308352
Control51=IDC_STATIC,static,1342308352
Control52=IDC_AXIAL_CONDUCT_EDIT,edit,1484849280
Control53=IDC_AREA_EDIT,edit,1484849280
Control54=IDC_STATIC,static,1342308352
Control55=IDC_STATIC,static,1342308352
Control56=IDC_STATIC,static,1342308352
Control57=IDC_STATIC,static,1342308352
Control58=IDC_STATIC,static,1342308352
Control59=IDC_STATIC,static,1342308352
Control60=IDC_STATIC,button,1342177287
Control61=IDC_STATIC,static,1342308352
Control62=IDC_STATIC,static,1342308352
Control63=IDC_STATIC,static,1342308352
Control64=IDC_STATIC,static,1342308352
Control65=IDC_STATIC,static,1342308352
Control66=IDC_STATIC,static,1342308352
Control67=IDC_STATIC,static,1342308352
Control68=IDC_STATIC,static,1342308352
Control69=IDC_STATIC,static,1342308352
Control70=IDC_STATIC,static,1342308352
Control71=IDC_STATIC,static,1342308352
Control72=IDC_STATIC,static,1342308352
Control73=IDC_STATIC,static,1342308352
Control74=IDC_STATIC,static,1342308352
Control75=IDC_STATIC,button,1342177287
Control76=IDC_STATIC,static,1342308352
Control77=IDC_STATIC,static,1342308352
Control78=IDC_STATIC,static,1342308352
Control79=IDC_STATIC,static,1342308352
Control80=IDC_STATIC,static,1342308352
Control81=IDC_STATIC,static,1342308352
Control82=IDC_STATIC,static,1342308352
Control83=IDC_REMOVE_SENS_BUTT,button,1342373888
Control84=IDC_STATIC,static,1342308352
Control85=IDC_STATIC,static,1342308352
Control86=IDC_STATIC,static,1342308352
Control87=IDC_STATIC,button,1342177287
Control88=IDC_STATIC,static,1342308352
Control89=IDC_STATIC,static,1342308352
Control90=IDC_STATIC,static,1342308352
Control91=IDC_Leak_GMAX_STATIC,static,1342312460
Control92=IDC_Na_GMAX_STATIC,static,1342312460
Control93=IDC_h_GMAX_STATIC,static,1342312460
Control94=IDC_CaT_GMAX_STATIC,static,1342312460
Control95=IDC_CaP_GMAX_STATIC,static,1342312460
Control96=IDC_A_GMAX_STATIC,static,1342312460
Control97=IDC_KCa_GMAX_STATIC,static,1342312460
Control98=IDC_Kd_GMAX_STATIC,static,1342312460
Control99=IDC_DC_VALUE_STATIC,static,1342312460
Control100=IDC_SLOW_VALUE_STATIC,static,1342312460
Control101=IDC_FAST_VALUE_STATIC,static,1342312460
Control102=IDC_STATIC,button,1342177287
Control103=IDC_STATIC,static,1342308352
Control104=IDC_SYN_COMBO,combobox,1344339971
Control105=IDC_REMOVE_SYN_BUTT,button,1342242816
Control106=IDC_STATIC,static,1342308352
Control107=IDC_STATIC,static,1342308352
Control108=IDC_STATIC,static,1342308352
Control109=IDC_STATIC,static,1342308352
Control110=IDC_STATIC,static,1342308352
Control111=IDC_STATIC,static,1342308352
Control112=IDC_SYN_E_EDIT,edit,1350631552
Control113=IDC_SYN_TAUDECAY_EDIT,edit,1350631552
Control114=IDC_SYN_GSTEP_EDIT,edit,1350631552

[DLG:IDD_DISPLAYPAGE]
Type=1
Class=CDisplayPage
ControlCount=29
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_COMBO1,combobox,1344340226
Control9=IDC_GRAPH1_COMBO,combobox,1344339970
Control10=IDC_COMBO3,combobox,1344340226
Control11=IDC_COMBO4,combobox,1344340226
Control12=IDC_COMBO5,combobox,1344340226
Control13=IDC_COMBO6,combobox,1344340226
Control14=IDC_NUM_PLOTS_EDIT,edit,1350631552
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_MIN1_EDIT,edit,1350631552
Control18=IDC_MAX1_EDIT,edit,1350631552
Control19=IDC_MIN2_EDIT,edit,1350631552
Control20=IDC_MAX2_EDIT,edit,1350631552
Control21=IDC_MIN3_EDIT,edit,1350631552
Control22=IDC_MAX3_EDIT,edit,1350631552
Control23=IDC_MIN4_EDIT,edit,1350631552
Control24=IDC_MAX4_EDIT,edit,1350631552
Control25=IDC_MIN5_EDIT,edit,1350631552
Control26=IDC_MAX5_EDIT,edit,1350631552
Control27=IDC_MIN6_EDIT,edit,1350631552
Control28=IDC_MAX6_EDIT,edit,1350631552
Control29=IDC_APPLY_BUTT,button,1342242816

[CLS:CDisplayPage]
Type=0
HeaderFile=DisplayPage.h
ImplementationFile=DisplayPage.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=CDisplayPage
VirtualFilter=idWC

[CLS:CNeuronPage]
Type=0
HeaderFile=NeuronPage.h
ImplementationFile=NeuronPage.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=IDC_REMOVE_SYN_BUTT
VirtualFilter=idWC

[CLS:CRunPage]
Type=0
HeaderFile=RunPage.h
ImplementationFile=RunPage.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=CRunPage
VirtualFilter=idWC

[CLS:CPlotsDialog]
Type=0
HeaderFile=PlotsDialog.h
ImplementationFile=PlotsDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CPlotsDialog
VirtualFilter=dWC

[DLG:IDD_PLOTSDIALOG]
Type=1
Class=CPlotsDialog
ControlCount=1
Control1=IDC_PLOTSPICTURE,static,1342308358

