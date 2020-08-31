/***************************************************************
 * Name:      PetriDishCounterApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Ammar Qammaz (ammarkov at gmail dot com)
 * Created:   2020-08-31
 * Copyright: Ammar Qammaz (http://ammar.gr)
 * License:
 **************************************************************/

#include "PetriDishCounterApp.h"

//(*AppHeaders
#include "PetriDishCounterMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(PetriDishCounterApp);

bool PetriDishCounterApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	PetriDishCounterFrame* Frame = new PetriDishCounterFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
