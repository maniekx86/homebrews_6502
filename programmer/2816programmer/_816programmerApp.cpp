/***************************************************************
 * Name:      _816programmerApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2023-08-17
 * Copyright:  ()
 * License:
 **************************************************************/

#include "_816programmerApp.h"

//(*AppHeaders
#include "_816programmerMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(_816programmerApp);

bool _816programmerApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
        _816programmerFrame* Frame = new _816programmerFrame(0);
        Frame->Show();
        SetTopWindow(Frame);
    }
    //*)

    return wxsOK;

}
