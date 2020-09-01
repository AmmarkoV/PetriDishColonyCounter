/***************************************************************
 * Name:      PetriDishCounterMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Ammar Qammaz (ammarkov at gmail dot com)
 * Created:   2020-08-31
 * Copyright: Ammar Qammaz (http://ammar.gr)
 * License:
 **************************************************************/

#include "PetriDishCounterMain.h"
#include <wx/msgdlg.h>
#include <wx/filename.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/wx.h>
#include <wx/utils.h>
#include <wx/dir.h>

//(*InternalHeaders(PetriDishCounterFrame)
#include <wx/bitmap.h>
#include <wx/font.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/wfstream.h>

#include "cv.h"

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

//(*IdInit(PetriDishCounterFrame)
const long PetriDishCounterFrame::ID_STATICBOX1 = wxNewId();
const long PetriDishCounterFrame::ID_STATICBITMAP1 = wxNewId();
const long PetriDishCounterFrame::ID_STATICBOX2 = wxNewId();
const long PetriDishCounterFrame::ID_STATICBOX3 = wxNewId();
const long PetriDishCounterFrame::ID_STATICTEXT1 = wxNewId();
const long PetriDishCounterFrame::ID_SPINCTRLDOUBLE1 = wxNewId();
const long PetriDishCounterFrame::ID_STATICTEXT2 = wxNewId();
const long PetriDishCounterFrame::ID_BUTTON1 = wxNewId();
const long PetriDishCounterFrame::ID_BUTTON2 = wxNewId();
const long PetriDishCounterFrame::ID_BUTTON3 = wxNewId();
const long PetriDishCounterFrame::ID_SPINCTRLDOUBLE2 = wxNewId();
const long PetriDishCounterFrame::ID_SPINCTRLDOUBLE3 = wxNewId();
const long PetriDishCounterFrame::ID_SPINCTRLDOUBLE4 = wxNewId();
const long PetriDishCounterFrame::ID_STATICTEXT3 = wxNewId();
const long PetriDishCounterFrame::ID_STATICTEXT4 = wxNewId();
const long PetriDishCounterFrame::ID_SPINCTRLDOUBLE5 = wxNewId();
const long PetriDishCounterFrame::ID_CHECKBOX1 = wxNewId();
const long PetriDishCounterFrame::ID_SPINCTRLDOUBLE6 = wxNewId();
const long PetriDishCounterFrame::ID_CHECKBOX2 = wxNewId();
const long PetriDishCounterFrame::ID_CHECKBOX3 = wxNewId();
const long PetriDishCounterFrame::ID_CHECKBOX4 = wxNewId();
const long PetriDishCounterFrame::ID_SPINCTRLDOUBLE7 = wxNewId();
const long PetriDishCounterFrame::ID_CHECKBOX5 = wxNewId();
const long PetriDishCounterFrame::ID_CHECKBOX6 = wxNewId();
const long PetriDishCounterFrame::ID_STATICLINE1 = wxNewId();
const long PetriDishCounterFrame::ID_BUTTON4 = wxNewId();
const long PetriDishCounterFrame::ID_BUTTON5 = wxNewId();
const long PetriDishCounterFrame::commandImportFile = wxNewId();
const long PetriDishCounterFrame::commandImportFolder = wxNewId();
const long PetriDishCounterFrame::commandLiveFromCamera = wxNewId();
const long PetriDishCounterFrame::idMenuQuit = wxNewId();
const long PetriDishCounterFrame::viewRawImage = wxNewId();
const long PetriDishCounterFrame::viewNormalWithOverlay = wxNewId();
const long PetriDishCounterFrame::viewInternal = wxNewId();
const long PetriDishCounterFrame::idMenuAbout = wxNewId();
const long PetriDishCounterFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(PetriDishCounterFrame,wxFrame)
    //(*EventTable(PetriDishCounterFrame)
    //*)
    EVT_PAINT(PetriDishCounterFrame::OnPaint)
    EVT_MOTION(PetriDishCounterFrame::OnMotion)
END_EVENT_TABLE()

PetriDishCounterFrame::PetriDishCounterFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(PetriDishCounterFrame)
    wxMenu* Menu1;
    wxMenu* Menu2;
    wxMenuBar* MenuBar1;
    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem2;

    Create(parent, id, _("Petri Dish Counter "), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    SetClientSize(wxSize(1280,720));
    StaticBox1 = new wxStaticBox(this, ID_STATICBOX1, _("Input Feed"), wxPoint(8,8), wxSize(968,658), 0, _T("ID_STATICBOX1"));
    StaticBitmap1 = new wxStaticBitmap(this, ID_STATICBITMAP1, wxBitmap(wxImage(_T("/home/ammar/Documents/Programming/PetriDishColonyCounter/data/default.png")).Rescale(wxSize(950,600).GetWidth(),wxSize(950,600).GetHeight())), wxPoint(16,48), wxSize(950,600), 0, _T("ID_STATICBITMAP1"));
    StaticBox2 = new wxStaticBox(this, ID_STATICBOX2, _("Filtering Controls"), wxPoint(984,192), wxSize(280,474), 0, _T("ID_STATICBOX2"));
    StaticBox3 = new wxStaticBox(this, ID_STATICBOX3, _("Current Count"), wxPoint(984,8), wxSize(280,184), 0, _T("ID_STATICBOX3"));
    ResultText = new wxStaticText(this, ID_STATICTEXT1, _("0"), wxPoint(1000,48), wxSize(208,104), 0, _T("ID_STATICTEXT1"));
    wxFont ResultTextFont(57,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    ResultText->SetFont(ResultTextFont);
    MinThreshold = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE1, _T("5"), wxPoint(992,248), wxSize(130,34), 0, 0, 100, 5, 1, _T("ID_SPINCTRLDOUBLE1"));
    MinThreshold->SetValue(_T("5"));
    StaticText1 = new wxStaticText(this, ID_STATICTEXT2, _("Threshold"), wxPoint(1088,224), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    ButtonPrevious = new wxButton(this, ID_BUTTON1, _("Previous"), wxPoint(1000,624), wxSize(112,34), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    ButtonNext = new wxButton(this, ID_BUTTON2, _("Next"), wxPoint(1144,624), wxSize(112,34), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    ButtonEnter = new wxButton(this, ID_BUTTON3, _("Sample is OK"), wxPoint(1000,560), wxSize(256,58), 0, wxDefaultValidator, _T("ID_BUTTON3"));
    SpinCtrlDouble2 = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE2, _T("5"), wxPoint(992,312), wxSize(130,34), 0, 0, 100, 5, 1, _T("ID_SPINCTRLDOUBLE2"));
    SpinCtrlDouble2->SetValue(_T("5"));
    MaxThreshold = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE3, _T("45"), wxPoint(1128,248), wxSize(130,34), 0, 0, 100, 45, 1, _T("ID_SPINCTRLDOUBLE3"));
    MaxThreshold->SetValue(_T("45"));
    SpinCtrlDouble1 = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE4, _T("44"), wxPoint(1128,312), wxSize(130,34), 0, 0, 100, 5, 44, _T("ID_SPINCTRLDOUBLE4"));
    SpinCtrlDouble1->SetValue(_T("44"));
    StaticText2 = new wxStaticText(this, ID_STATICTEXT3, _("Time Saved"), wxPoint(1176,160), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    StaticText3 = new wxStaticText(this, ID_STATICTEXT4, _("Speed information"), wxPoint(1000,160), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    SpinCtrlDouble3 = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE5, _T("0.6"), wxPoint(1128,360), wxSize(128,34), 0, 0, 1, 0.6, 0.1, _T("ID_SPINCTRLDOUBLE5"));
    SpinCtrlDouble3->SetValue(_T("0.6"));
    CheckBoxCircularity = new wxCheckBox(this, ID_CHECKBOX1, _("Circularity"), wxPoint(1000,368), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    CheckBoxCircularity->SetValue(true);
    SpinCtrlDouble4 = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE6, _T("0.01"), wxPoint(1128,408), wxSize(128,34), 0, 0, 1, 0, 0.01, _T("ID_SPINCTRLDOUBLE6"));
    SpinCtrlDouble4->SetValue(_T("0.01"));
    CheckBoxInertia = new wxCheckBox(this, ID_CHECKBOX2, _("Inertia"), wxPoint(1000,414), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    CheckBoxInertia->SetValue(true);
    CheckBoxFilterByArea = new wxCheckBox(this, ID_CHECKBOX3, _("Filter by Area"), wxPoint(1064,288), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
    CheckBoxFilterByArea->SetValue(true);
    CheckBoxConvexity = new wxCheckBox(this, ID_CHECKBOX4, _("Convexity"), wxPoint(1000,462), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
    CheckBoxConvexity->SetValue(false);
    SpinCtrlDouble5 = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE7, _T("0.87"), wxPoint(1128,456), wxSize(128,34), 0, 0, 1, 0, 0.01, _T("ID_SPINCTRLDOUBLE7"));
    SpinCtrlDouble5->SetValue(_T("0.87"));
    CheckBoxDilate = new wxCheckBox(this, ID_CHECKBOX5, _("Dilate"), wxPoint(1000,504), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX5"));
    CheckBoxDilate->SetValue(true);
    CheckBoxErode = new wxCheckBox(this, ID_CHECKBOX6, _("Erode"), wxPoint(1128,504), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX6"));
    CheckBoxErode->SetValue(false);
    StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxPoint(1000,552), wxSize(256,0), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
    ButtonIncrement = new wxButton(this, ID_BUTTON4, _("+"), wxPoint(1216,40), wxSize(40,34), 0, wxDefaultValidator, _T("ID_BUTTON4"));
    ButtonSubtract = new wxButton(this, ID_BUTTON5, _("-"), wxPoint(1216,104), wxSize(40,34), 0, wxDefaultValidator, _T("ID_BUTTON5"));
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem3 = new wxMenuItem(Menu1, commandImportFile, _("Import File"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem3);
    MenuItem4 = new wxMenuItem(Menu1, commandImportFolder, _("Import Folder"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem4);
    MenuItem5 = new wxMenuItem(Menu1, commandLiveFromCamera, _("Import From Camera"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem5);
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu3 = new wxMenu();
    MenuItem6 = new wxMenuItem(Menu3, viewRawImage, _("Input Image"), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(MenuItem6);
    MenuItem7 = new wxMenuItem(Menu3, viewNormalWithOverlay, _("Image with overlay"), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(MenuItem7);
    MenuItem8 = new wxMenuItem(Menu3, viewInternal, _("Internal image"), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(MenuItem8);
    MenuBar1->Append(Menu3, _("View"));
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

    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PetriDishCounterFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PetriDishCounterFrame::OnAbout);
    //*)
    Connect(commandImportFile,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PetriDishCounterFrame::OnLoadFromFile);
    Connect(commandImportFolder,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PetriDishCounterFrame::OnLoadFromFolder);
    Connect(commandLiveFromCamera,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PetriDishCounterFrame::OnLiveCameraStream);
}

void PetriDishCounterFrame::triggerProcessing()
{
   int result = processLoadedImage(0);

   char numberShown[128];
   snprintf(numberShown,128,"%u",result);
   ResultText->SetLabel(wxString::FromUTF8(numberShown));
   //accessRGBPixels(unsigned int * width,unsigned int * height);
}


void PetriDishCounterFrame::render(wxDC& dc)
{
    /*
  if ( (rgbFrame!=0) && (live_feeds[0].bmp!=0) )
   { dc.DrawBitmap(*live_feeds[0].bmp,feed_0_x,feed_0_y,0); } //FEED 1

  if ( (depthFrame!=0) && (live_feeds[1].bmp!=0) )
   { dc.DrawBitmap(*live_feeds[1].bmp,feed_1_x,feed_1_y,0); } //FEED 2



   if (recording)
   { //DRAW RECORDING DECAL ON LEFT FEED
     wxPen red(wxColour(255,0,0),1,wxSOLID);
     dc.SetPen(red);
     dc.SetBrush(*wxRED_BRUSH); // *wxTRANSPARENT_BRUSH
     dc.DrawCircle(50,50,10); //Recording Mark ON!
   }

   DrawAFPoints(dc ,feed_0_x,feed_0_y);
   DrawFeaturesAtFeed(dc,feed_0_x,feed_0_y,ListCtrlPoints);

 wxSleep(0.01);
 wxYieldIfNeeded();*/
}

void PetriDishCounterFrame::OnPaint(wxPaintEvent& evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void PetriDishCounterFrame::OnMotion(wxMouseEvent& event)
{
  int x=event.GetX();
  int y=event.GetY();
}

PetriDishCounterFrame::~PetriDishCounterFrame()
{
    //(*Destroy(PetriDishCounterFrame)
    //*)
}

void PetriDishCounterFrame::OnLoadFromFile(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, _("Open JPEG file"), "", "", "JPEG files (*.jpg)|*.jpg", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...
    // proceed loading the file chosen by the user;
    // this can be done with e.g. wxWidgets input streams:

    fprintf(stderr,"File requested to open is : %s \n",openFileDialog.GetPath().mb_str().data());

    wxFileInputStream input_stream(openFileDialog.GetPath());
    if (!input_stream.IsOk())
    {
        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    } else
    {
      int result = loadAnImage(openFileDialog.GetPath().mb_str().data());
      if (!result)
      {
        wxMessageBox(_("Failed loading image.."), _("Error"), wxICON_ERROR, this);
      } else
      {
        triggerProcessing();
      }
    }
}

void PetriDishCounterFrame::OnLoadFromFolder(wxCommandEvent& event)
{

 wxDirDialog openDirectoryDialog(this, _("Choose input directory"), "", wxDD_DEFAULT_STYLE|wxDD_DIR_MUST_EXIST);
    if (openDirectoryDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...
    // proceed loading the file chosen by the user;
    // this can be done with e.g. wxWidgets input streams:

    fprintf(stderr,"Directory requested to open is : %s \n",openDirectoryDialog.GetPath().mb_str().data());


}

void PetriDishCounterFrame::OnLiveCameraStream(wxCommandEvent& event)
{
    Close();
}

void PetriDishCounterFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void PetriDishCounterFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}
