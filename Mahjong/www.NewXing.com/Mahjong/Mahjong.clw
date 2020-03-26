; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSetMaj
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Mahjong.h"

ClassCount=3
Class1=CMahjongApp
Class2=CMahjongDlg

ResourceCount=2
Resource1=IDR_MAINFRAME
Class3=CSetMaj
Resource2=IDD_MAHJONG_DIALOG

[CLS:CMahjongApp]
Type=0
HeaderFile=Mahjong.h
ImplementationFile=Mahjong.cpp
Filter=N

[CLS:CMahjongDlg]
Type=0
HeaderFile=MahjongDlg.h
ImplementationFile=MahjongDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CMahjongDlg



[DLG:IDD_MAHJONG_DIALOG]
Type=1
Class=CMahjongDlg
ControlCount=0

[CLS:CSetMaj]
Type=0
HeaderFile=SetMaj.h
ImplementationFile=SetMaj.cpp
BaseClass=CDialog
Filter=D
LastObject=CSetMaj
VirtualFilter=dWC

