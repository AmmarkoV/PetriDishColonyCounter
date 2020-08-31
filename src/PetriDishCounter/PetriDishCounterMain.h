/***************************************************************
 * Name:      PetriDishCounterMain.h
 * Purpose:   Defines Application Frame
 * Author:    Ammar Qammaz (ammarkov at gmail dot com)
 * Created:   2020-08-31
 * Copyright: Ammar Qammaz (http://ammar.gr)
 * License:
 **************************************************************/

#ifndef PETRIDISHCOUNTERMAIN_H
#define PETRIDISHCOUNTERMAIN_H

//(*Headers(PetriDishCounterFrame)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/spinctrl.h>
#include <wx/statbmp.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>
//*)

class PetriDishCounterFrame: public wxFrame
{
    public:

        PetriDishCounterFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~PetriDishCounterFrame();

    private:

        //(*Handlers(PetriDishCounterFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        //*)

        //(*Identifiers(PetriDishCounterFrame)
        static const long ID_STATICBOX1;
        static const long ID_STATICBITMAP1;
        static const long ID_STATICBOX2;
        static const long ID_STATICBOX3;
        static const long ID_STATICTEXT1;
        static const long ID_SPINCTRLDOUBLE1;
        static const long ID_STATICTEXT2;
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_BUTTON3;
        static const long ID_SPINCTRLDOUBLE2;
        static const long ID_SPINCTRLDOUBLE3;
        static const long ID_SPINCTRLDOUBLE4;
        static const long ID_STATICTEXT3;
        static const long ID_STATICTEXT4;
        static const long ID_SPINCTRLDOUBLE5;
        static const long ID_CHECKBOX1;
        static const long ID_SPINCTRLDOUBLE6;
        static const long ID_CHECKBOX2;
        static const long ID_CHECKBOX3;
        static const long ID_CHECKBOX4;
        static const long ID_SPINCTRLDOUBLE7;
        static const long ID_CHECKBOX5;
        static const long ID_CHECKBOX6;
        static const long ID_STATICLINE1;
        static const long ID_BUTTON4;
        static const long ID_BUTTON5;
        static const long commandImportFile;
        static const long commandImportFolder;
        static const long ID_MENUITEM1;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(PetriDishCounterFrame)
        wxButton* ButtonEnter;
        wxButton* ButtonIncrement;
        wxButton* ButtonNext;
        wxButton* ButtonPrevious;
        wxButton* ButtonSubtract;
        wxCheckBox* CheckBoxCircularity;
        wxCheckBox* CheckBoxConvexity;
        wxCheckBox* CheckBoxDilate;
        wxCheckBox* CheckBoxErode;
        wxCheckBox* CheckBoxFilterByArea;
        wxCheckBox* CheckBoxInertia;
        wxMenuItem* MenuItem3;
        wxMenuItem* MenuItem4;
        wxMenuItem* MenuItem5;
        wxSpinCtrlDouble* MaxThreshold;
        wxSpinCtrlDouble* MinThreshold;
        wxSpinCtrlDouble* SpinCtrlDouble1;
        wxSpinCtrlDouble* SpinCtrlDouble2;
        wxSpinCtrlDouble* SpinCtrlDouble3;
        wxSpinCtrlDouble* SpinCtrlDouble4;
        wxSpinCtrlDouble* SpinCtrlDouble5;
        wxStaticBitmap* StaticBitmap1;
        wxStaticBox* StaticBox1;
        wxStaticBox* StaticBox2;
        wxStaticBox* StaticBox3;
        wxStaticLine* StaticLine1;
        wxStaticText* ResultText;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxStatusBar* StatusBar1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // PETRIDISHCOUNTERMAIN_H
