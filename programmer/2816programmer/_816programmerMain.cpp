/***************************************************************
 * Name:      _816programmerMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2023-08-17
 * Copyright:  ()
 * License:
 **************************************************************/

#include "_816programmerMain.h"
#include <wx/msgdlg.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <libserial/SerialPort.h>

using namespace LibSerial;

SerialPort serial_port;

std::vector<std::string> getSerialPorts() {
    std::vector<std::string> serialPorts;

    // Iterate through /dev directory and find entries that start with "ttyS", "ttyUSB", or "ttyACM"
    for (const auto& entry : std::filesystem::directory_iterator("/dev")) {
        const std::string& entryName = entry.path().string();
        if (entryName.find("/dev/ttyS") != std::string::npos ||
            entryName.find("/dev/ttyUSB") != std::string::npos ||
            entryName.find("/dev/ttyACM") != std::string::npos) {
            serialPorts.push_back(entryName);
        }
    }

    return serialPorts;
}


//(*InternalHeaders(_816programmerFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(_816programmerFrame)
const long _816programmerFrame::ID_BUTTON1 = wxNewId();
const long _816programmerFrame::ID_RADIOBOX1 = wxNewId();
const long _816programmerFrame::ID_GAUGE1 = wxNewId();
const long _816programmerFrame::ID_STATICTEXT3 = wxNewId();
const long _816programmerFrame::ID_FILEPICKERCTRL1 = wxNewId();
const long _816programmerFrame::ID_FILEPICKERCTRL2 = wxNewId();
const long _816programmerFrame::ID_STATICTEXT4 = wxNewId();
const long _816programmerFrame::ID_STATICTEXT5 = wxNewId();
const long _816programmerFrame::ID_BUTTON2 = wxNewId();
const long _816programmerFrame::ID_RADIOBOX2 = wxNewId();
const long _816programmerFrame::ID_PANEL2 = wxNewId();
const long _816programmerFrame::ID_COMBOBOX1 = wxNewId();
const long _816programmerFrame::ID_STATICTEXT2 = wxNewId();
const long _816programmerFrame::ID_PANEL1 = wxNewId();
const long _816programmerFrame::ID_STATICTEXT1 = wxNewId();
const long _816programmerFrame::idMenuQuit = wxNewId();
const long _816programmerFrame::idMenuAbout = wxNewId();
const long _816programmerFrame::ID_STATUSBAR1 = wxNewId();
const long _816programmerFrame::ID_TIMER1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(_816programmerFrame,wxFrame)
    //(*EventTable(_816programmerFrame)
    //*)
END_EVENT_TABLE()

_816programmerFrame::_816programmerFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(_816programmerFrame)
    wxGridBagSizer* GridBagSizer1;
    wxMenu* Menu1;
    wxMenu* Menu2;
    wxMenuBar* MenuBar1;
    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem2;

    Create(parent, wxID_ANY, _("2816/256 Programmer"), wxDefaultPosition, wxDefaultSize, wxCLOSE_BOX|wxMINIMIZE_BOX, _T("wxID_ANY"));
    SetClientSize(wxSize(359,542));
    GridBagSizer1 = new wxGridBagSizer(0, 0);
    Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxSize(349,494), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    ConnectButton = new wxButton(Panel1, ID_BUTTON1, _("Connect"), wxPoint(224,17), wxSize(112,34), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    Panel2 = new wxPanel(Panel1, ID_PANEL2, wxPoint(0,80), wxSize(347,408), wxBORDER_SIMPLE|wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    Panel2->Disable();
    wxString __wxRadioBoxChoices_1[3] =
    {
        _("Read EEPROM"),
        _("Verify EEPROM"),
        _("Write EEPROM")
    };
    ActionSelector = new wxRadioBox(Panel2, ID_RADIOBOX1, _("Action"), wxPoint(8,104), wxSize(328,96), 3, __wxRadioBoxChoices_1, 1, wxRA_HORIZONTAL, wxDefaultValidator, _T("ID_RADIOBOX1"));
    progressGauge = new wxGauge(Panel2, ID_GAUGE1, 2048, wxPoint(16,376), wxSize(312,16), 0, wxDefaultValidator, _T("ID_GAUGE1"));
    progressText = new wxStaticText(Panel2, ID_STATICTEXT3, _("Progress: -"), wxPoint(16,368), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    readFile = new wxFilePickerCtrl(Panel2, ID_FILEPICKERCTRL1, wxEmptyString, wxEmptyString, wxEmptyString, wxPoint(16,280), wxSize(312,34), wxFLP_OVERWRITE_PROMPT|wxFLP_SAVE|wxFLP_USE_TEXTCTRL, wxDefaultValidator, _T("ID_FILEPICKERCTRL1"));
    writeverifyFile = new wxFilePickerCtrl(Panel2, ID_FILEPICKERCTRL2, wxEmptyString, wxEmptyString, wxEmptyString, wxPoint(16,224), wxSize(312,34), wxFLP_FILE_MUST_EXIST|wxFLP_OPEN|wxFLP_USE_TEXTCTRL, wxDefaultValidator, _T("ID_FILEPICKERCTRL2"));
    StaticText3 = new wxStaticText(Panel2, ID_STATICTEXT4, _("File to Write/Verify EEPROM"), wxPoint(16,208), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    StaticText4 = new wxStaticText(Panel2, ID_STATICTEXT5, _("File to read EEPROM"), wxPoint(16,264), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    startButton = new wxButton(Panel2, ID_BUTTON2, _("Start"), wxPoint(72,328), wxSize(208,40), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    wxString __wxRadioBoxChoices_2[2] =
    {
        _("28C16 (2 KB)"),
        _("28C256 (32 KB) (Only 16 KB reads/writes)")
    };
    ROMTypeSelector = new wxRadioBox(Panel2, ID_RADIOBOX2, _("EEPROM Type"), wxPoint(10,16), wxSize(320,79), 2, __wxRadioBoxChoices_2, 1, wxRA_HORIZONTAL, wxDefaultValidator, _T("ID_RADIOBOX2"));
    SerialList = new wxComboBox(Panel1, ID_COMBOBOX1, wxEmptyString, wxPoint(16,16), wxSize(199,31), 0, 0, wxCB_READONLY, wxDefaultValidator, _T("ID_COMBOBOX1"));
    SerialState = new wxStaticText(Panel1, ID_STATICTEXT2, _("State: disconnected"), wxPoint(16,56), wxSize(311,17), 0, _T("ID_STATICTEXT2"));
    GridBagSizer1->Add(Panel1, wxGBPosition(2, 0), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("maniek86\'s 2816 (and 28256) EEPROM programmer"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    GridBagSizer1->Add(StaticText1, wxGBPosition(0, 0), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(GridBagSizer1);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    Timer1.SetOwner(this, ID_TIMER1);
    Timer1.Start(5, false);
    Layout();

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&_816programmerFrame::OnConnectButtonClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&_816programmerFrame::OnstartButtonClick);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&_816programmerFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&_816programmerFrame::OnAbout);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&_816programmerFrame::OnTimer1Trigger);
    //*)

    std::vector<std::string> serialPorts = getSerialPorts();

    wxArrayString wxStringArray;
    for (const auto& str : serialPorts) {
        wxStringArray.Add(wxString(str.c_str(), wxConvUTF8));
    }


    if (serialPorts.empty()) {
        std::cout << "No serial ports found." << std::endl;
    } else {
        std::cout << "Detected serial ports:" << std::endl;
        for (const std::string& port : serialPorts) {
            std::cout << port << std::endl;
            SerialList->Set(wxStringArray);
        }
    }

}

_816programmerFrame::~_816programmerFrame()
{
    //(*Destroy(_816programmerFrame)
    //*)
}

void _816programmerFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void _816programmerFrame::OnAbout(wxCommandEvent& event)
{
    //wxString msg = wxbuildinfo(long_f);
    wxMessageBox("2816 / 28256 gui programmer by maniek86", _("Welcome to..."));
}

bool isSerialPortOpen=false;
int currentOperation = 0;
int connectionState = 0;
int connectionTime;

bool isVerifing=false;
bool isWriting=false;
bool isFastWrite=false;

std::ofstream fileToWrite;
std::ifstream fileToRead;

int currentReadPos=0;
uint8_t readedFile[0x10000];

uint8_t diskFile[0x10000];

enum eeprom_types {
    AT28C16,
    AT28C128, // 256 but only half is programmed
    AT28C256
} eeprom_type;

uint16_t getEEPROMSize() {
    if(eeprom_type == AT28C16) return 2048;
    if(eeprom_type == AT28C128) return 16384;
    if(eeprom_type == AT28C256) return 32768;

    return 0;
}

uint16_t getEEPROMPageSize() {
    if(eeprom_type == AT28C16) return 16;
    if(eeprom_type == AT28C128) return 64;
    if(eeprom_type == AT28C256) return 64;
}

uint16_t getEEPROMPages() {
    return getEEPROMSize() / getEEPROMPageSize();
}

void _816programmerFrame::OnConnectButtonClick(wxCommandEvent& event) {
    if(connectionState==1) return;
    if(isSerialPortOpen) {
        if(currentOperation!=0) return;

        serial_port.Close();
        ConnectButton->SetLabel("Connect");
        SerialState->SetLabel("State: disconnected");
        Panel2->Enable(false);
        SerialList->Enable(true);
        isSerialPortOpen = false;
        connectionState = 0;

        return;
    }
    try {
        // Open the Serial Port at the desired hardware port.
        std::string serialPortPath(SerialList->GetValue().mb_str());
        serial_port.Open(serialPortPath.c_str());
    }
    catch (const OpenFailed&) {
        std::cerr << "The serial port did not open correctly." << std::endl ;
        wxMessageBox("Cannot open serial port!", "Error", wxOK | wxICON_ERROR);
        return;
    }
    serial_port.SetBaudRate(BaudRate::BAUD_115200) ;
    serial_port.SetCharacterSize(CharacterSize::CHAR_SIZE_8) ;
    serial_port.SetFlowControl(FlowControl::FLOW_CONTROL_NONE) ;
    serial_port.SetParity(Parity::PARITY_NONE) ;
    serial_port.SetStopBits(StopBits::STOP_BITS_1) ;
    connectionState = 1;
    connectionTime = 200;
    SerialList->Enable(false);
    ConnectButton->Enable(false);
    SerialState->SetLabel("State: connecting");
    isSerialPortOpen = true;
    serial_port.WriteByte('D'); // detect command
}

void writeReadCommand(uint8_t page) {

    serial_port.WriteByte('F');
    serial_port.WriteByte(page);

}

void writeWriteCommand(uint16_t addr, uint8_t data) {

    serial_port.WriteByte('W');
    uint8_t addr1 = addr / 256;
    uint8_t addr2 = addr % 256;
    serial_port.WriteByte(addr1);
    serial_port.WriteByte(addr2);
    serial_port.WriteByte(data);

}

void updateProgrammerMode() {
    char buf[2];
    buf[0]='M';
    if(eeprom_type == AT28C16) buf[1]='0';
    if(eeprom_type == AT28C128) buf[1]='1';
    if(eeprom_type == AT28C256) buf[1]='1';
    serial_port.WriteByte(buf[0]);
    serial_port.WriteByte(buf[1]);
    serial_port.DrainWriteBuffer() ;
}

void writePage(uint8_t page) {

    if(page==getEEPROMPages()) return;

    int filePos = getEEPROMPageSize() * page;

    serial_port.WriteByte('P');
    serial_port.WriteByte(page);

    for(int i=0;i<getEEPROMPageSize();i++) {
        serial_port.WriteByte(diskFile[filePos + i]);
    }

}

void _816programmerFrame::OnTimer1Trigger(wxTimerEvent& event) {

    unsigned char cmd;
    unsigned char data;

    int wait=0;

    if(connectionState==1) {
        connectionTime--;

        if(connectionTime<=0) {
            connectionState = 0;
            serial_port.Close();
            ConnectButton->Enable(true);
            SerialList->Enable(true);
            isSerialPortOpen = false;
            SerialState->SetLabel("State: disconnected");
            wxMessageBox("Connection timed out", "Error", wxOK | wxICON_ERROR);
        }
    }

    if(currentOperation==6) { // finish operation
        progressGauge->SetValue(getEEPROMSize());
        std::string donestring = "Progress: read EEPROM done ("+std::to_string(getEEPROMSize())+"/"+std::to_string(getEEPROMSize())+")";
        wxString mydonestring(donestring);
        progressText->SetLabel(mydonestring);

        currentOperation = 0;

        if(isVerifing) {
            if(isWriting) { // verify after write
                int badPos = -1;
                for(int i=0; i < getEEPROMPageSize(); i++) {
                    if(readedFile[i]!=diskFile[i]) {
                        badPos = i;
                        break;
                    }
                }
                if(badPos==-1) {
                    wxMessageBox("Write and Verify OK!", "Write", wxOK);
                    progressText->SetLabel("Progress: write OK");
                } else {
                    std::string ststring = "Write failed (byte at pos "+std::to_string(badPos)+")";

                    wxString mystring(ststring);
                    wxMessageBox(mystring, "Verify", wxOK);
                    std::string ststring2 = "Progress: write failed (pos "+std::to_string(badPos)+")";

                    wxString mystring2(ststring2);
                    progressText->SetLabel(mystring2);
                }

            } else { // just verify
                int badPos = -1;
                for(int i=0; i < getEEPROMSize(); i++) {
                    if(readedFile[i]!=diskFile[i]) {
                        badPos = i;
                        break;
                    }
                }
                if(badPos==-1) {
                    wxMessageBox("Verify OK!", "Verify", wxOK);
                    progressText->SetLabel("Progress: verify OK");
                } else {
                    std::string ststring = "Verify failed (byte at pos "+std::to_string(badPos)+")";

                    wxString mystring(ststring);
                    wxMessageBox(mystring, "Verify", wxOK);
                    std::string ststring2 = "Progress: verify failed (pos "+std::to_string(badPos)+")";

                    wxString mystring2(ststring2);
                    progressText->SetLabel(mystring2);
                }
            }
        } else { // read to file
            fileToWrite.write((char*)readedFile, getEEPROMSize());
            fileToWrite.close();
        }

        ActionSelector->Enable(true);
        writeverifyFile->Enable(true);
        readFile->Enable(true);
        startButton->Enable(true);
        ConnectButton->Enable(true);
        ROMTypeSelector->Enable(true);

    }


    if(isSerialPortOpen&&serial_port.IsDataAvailable()) {
        serial_port.ReadByte(cmd,5);
        if(cmd=='$') { // response to 'D' detect command
            ConnectButton->SetLabel("Disconnect");
            ConnectButton->Enable(true);
            SerialState->SetLabel("State: connected");
            SerialList->Enable(false);
            Panel2->Enable(true);
            connectionState = 2;

        }


        if(cmd=='F') {
            uint8_t recData[255];


            for(int i=0;i<getEEPROMPageSize();i++) {
                while(!serial_port.IsDataAvailable()) {
                    wait++;
                    if(wait>1000) return;
                    usleep(100);
                }

                serial_port.ReadByte(data,5);
                recData[i] = data;
            }

            if(currentOperation!=5) return;
            if(currentReadPos>=getEEPROMPages()) return;

            for(int i=0;i < getEEPROMPageSize();i++) {
                readedFile[currentReadPos*getEEPROMPageSize()+i]=recData[i];
            }



            if(currentReadPos>=getEEPROMPages()-1) {
                currentOperation = 6;
            }

            currentReadPos++;

            if(currentReadPos<=getEEPROMPages()) {
                progressGauge->SetValue(currentReadPos*getEEPROMPageSize());
                progressText->SetLabel("Progress: reading "+std::to_string((int)(100/(float)getEEPROMSize()*currentReadPos*getEEPROMPageSize()))+"% ("+std::to_string(currentReadPos*getEEPROMPageSize())+"/"+std::to_string(getEEPROMSize())+")");

                writeReadCommand(currentReadPos);
            }
        }

        // write (page)
        if(cmd=='P') { //
            if(currentOperation!=10) return;
            if(currentReadPos>=getEEPROMPages()) return;

            readedFile[currentReadPos]=data;

            if(currentReadPos>=getEEPROMPages()-1) {
                currentOperation = 5;
                currentReadPos = 0;

                writeReadCommand(0);
                return;
            }

            currentReadPos++;

            if(currentReadPos<=getEEPROMPages()) {
                progressGauge->SetValue(currentReadPos*getEEPROMPageSize());
                progressText->SetLabel("Progress: writing "+std::to_string((int)(100/(float)getEEPROMSize()*currentReadPos*getEEPROMPageSize()))+"% ("+std::to_string(currentReadPos*getEEPROMPageSize())+"/"+std::to_string(getEEPROMSize())+")");
                writePage(currentReadPos);
            }

        }
    }
}

bool checkSize_msg(uint64_t fsize) {
    if(eeprom_type == AT28C16) {
        if(fsize==2048) {
            return true;
        } else {
            wxMessageBox("File is not exactly 2048 bytes in size", "Error", wxOK | wxICON_ERROR);
        }
    }

    if(eeprom_type == AT28C128) {
        if(fsize==16384) {
            return true;
        } else {
            wxMessageBox("File is not exactly 16384 bytes in size", "Error", wxOK | wxICON_ERROR);
        }
    }

    if(eeprom_type == AT28C256) {
        if(fsize==32768) {
            return true;
        } else {
            wxMessageBox("File is not exactly 32768 bytes in size", "Error", wxOK | wxICON_ERROR);
        }
    }

    return false;
}




void _816programmerFrame::OnstartButtonClick(wxCommandEvent& event) {
    bool disableControls=false;
    if(currentOperation!=0) return;
    int sel=ActionSelector->GetSelection();
    int type=ROMTypeSelector->GetSelection();

    if(type==0) {
        eeprom_type = AT28C16;
    } else if(type==1) {
        eeprom_type = AT28C128;
    } else if(type==2) {
        eeprom_type = AT28C256;
    }

    progressGauge->SetValue(0);

    if(sel==0) { // read eeprom
        std::string fpath(readFile->GetPath().mb_str());

        if(fpath=="") {
            wxMessageBox("Please select output file", "Error", wxOK | wxICON_ERROR);
            return;
        }
        fileToWrite.open(fpath, std::ios::out | std::ios::binary | std::ios::trunc);

        if(fileToWrite.bad()) {
            wxMessageBox("Cannot open output file", "Error", wxOK | wxICON_ERROR);
            return;
        }

        currentReadPos = 0;

        for(int i=0; i<getEEPROMSize(); i++) {
            readedFile[i] = 0;
        }

        isVerifing=false;
        isWriting=false;

        updateProgrammerMode();
        progressGauge->SetRange(getEEPROMSize());

        currentOperation = 5;

        writeReadCommand(0);

        disableControls = true;
    }

    if(sel==1) { // verify file
        std::string fpath(writeverifyFile->GetPath().mb_str());

        if(fpath=="") {
            wxMessageBox("Please select input file", "Error", wxOK | wxICON_ERROR);
            return;
        }
        fileToRead.open(fpath, std::ios::in | std::ios::binary);

        if(fileToRead.bad()) {
            wxMessageBox("Cannot open input file", "Error", wxOK | wxICON_ERROR);
            return;
        }

        uint64_t begin = fileToRead.tellg();
        fileToRead.seekg (0, std::ios::end);
        uint64_t end = fileToRead.tellg();
        uint64_t fsize = end - begin;
        if(!checkSize_msg(fsize)) {
            fileToRead.close();
            return;
        }
        fileToRead.seekg (0, std::ios::beg);

        fileToRead.read(reinterpret_cast<char*>(diskFile), getEEPROMSize());

        fileToRead.close();

        currentReadPos = 0;

        for(int i=0; i<getEEPROMSize(); i++) {
            readedFile[i] = 0;
        }

        updateProgrammerMode();
        progressGauge->SetRange(getEEPROMSize());

        isVerifing=true;
        isWriting=false;

        currentOperation = 5;

        writeReadCommand(0);

        disableControls = true;
    }

    // write file
    if(sel==2) { // verify file
        std::string fpath(writeverifyFile->GetPath().mb_str());

        if(fpath=="") {
            wxMessageBox("Please select input file", "Error", wxOK | wxICON_ERROR);
            return;
        }
        fileToRead.open(fpath, std::ios::in | std::ios::binary);

        if(fileToRead.bad()) {
            wxMessageBox("Cannot open input file", "Error", wxOK | wxICON_ERROR);
            return;
        }

        uint64_t begin = fileToRead.tellg();
        fileToRead.seekg (0, std::ios::end);
        uint64_t end = fileToRead.tellg();
        uint64_t fsize = end - begin;
        if(!checkSize_msg(fsize)) {
            fileToRead.close();
            return;
        }
        fileToRead.seekg (0, std::ios::beg);

        fileToRead.read(reinterpret_cast<char*>(diskFile), getEEPROMSize());

        fileToRead.close();

        currentReadPos = 0;

        for(int i=0; i<getEEPROMSize(); i++) {
            readedFile[i] = 0;
        }

        updateProgrammerMode();
        progressGauge->SetRange(getEEPROMSize());

        isVerifing=true;
        isWriting=true;

        currentOperation = 10;

        writePage(0);

        disableControls = true;

    }
    if(disableControls) {
        ROMTypeSelector->Enable(false);
        ActionSelector->Enable(false);
        writeverifyFile->Enable(false);
        readFile->Enable(false);
        startButton->Enable(false);
        ConnectButton->Enable(false);
    }
}
