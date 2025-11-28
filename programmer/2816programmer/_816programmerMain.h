/***************************************************************
 * Name:      _816programmerMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2023-08-17
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef _816PROGRAMMERMAIN_H
#define _816PROGRAMMERMAIN_H

//(*Headers(_816programmerFrame)
#include <wx/button.h>
#include <wx/combobox.h>
#include <wx/filepicker.h>
#include <wx/frame.h>
#include <wx/gauge.h>
#include <wx/gbsizer.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>
#include <wx/timer.h>
//*)

class _816programmerFrame: public wxFrame
{
    public:

        _816programmerFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~_816programmerFrame();

    private:

        //(*Handlers(_816programmerFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnConnectButtonClick(wxCommandEvent& event);
        void OnTimer1Trigger(wxTimerEvent& event);
        void OnstartButtonClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(_816programmerFrame)
        static const long ID_BUTTON1;
        static const long ID_RADIOBOX1;
        static const long ID_GAUGE1;
        static const long ID_STATICTEXT3;
        static const long ID_FILEPICKERCTRL1;
        static const long ID_FILEPICKERCTRL2;
        static const long ID_STATICTEXT4;
        static const long ID_STATICTEXT5;
        static const long ID_BUTTON2;
        static const long ID_RADIOBOX2;
        static const long ID_PANEL2;
        static const long ID_COMBOBOX1;
        static const long ID_STATICTEXT2;
        static const long ID_PANEL1;
        static const long ID_STATICTEXT1;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        static const long ID_TIMER1;
        //*)

        //(*Declarations(_816programmerFrame)
        wxButton* ConnectButton;
        wxButton* startButton;
        wxComboBox* SerialList;
        wxFilePickerCtrl* readFile;
        wxFilePickerCtrl* writeverifyFile;
        wxGauge* progressGauge;
        wxPanel* Panel1;
        wxPanel* Panel2;
        wxRadioBox* ActionSelector;
        wxRadioBox* ROMTypeSelector;
        wxStaticText* SerialState;
        wxStaticText* StaticText1;
        wxStaticText* StaticText3;
        wxStaticText* StaticText4;
        wxStaticText* progressText;
        wxStatusBar* StatusBar1;
        wxTimer Timer1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // _816PROGRAMMERMAIN_H
