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
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>
//*)

#include "cv.h"
#include <wx/dc.h>

class PetriDishCounterFrame: public wxFrame
{
    public:

        PetriDishCounterFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~PetriDishCounterFrame();

    private:
        void triggerProcessing();

        void OnLoadFromFile(wxCommandEvent& event);
        void OnLoadFromFolder(wxCommandEvent& event);
        void OnLiveCameraStream(wxCommandEvent& event);

        //(*Handlers(PetriDishCounterFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnButtonIncrementClick(wxCommandEvent& event);
        void OnButtonSubtractClick(wxCommandEvent& event);
        void OnMinThresholdChange(wxSpinDoubleEvent& event);
        void OnMaxThresholdChange(wxSpinDoubleEvent& event);
        void OnSpinCtrlDouble2Change(wxSpinDoubleEvent& event);
        void OnSpinCtrlDouble1Change(wxSpinDoubleEvent& event);
        void OnSpinCtrlDouble3Change(wxSpinDoubleEvent& event);
        void OnSpinCtrlDouble4Change(wxSpinDoubleEvent& event);
        void OnSpinCtrlDouble5Change(wxSpinDoubleEvent& event);
        //*)

        //(*Identifiers(PetriDishCounterFrame)
        static const long ID_STATICBOX1;
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
        static const long commandLiveFromCamera;
        static const long idMenuQuit;
        static const long viewRawImage;
        static const long viewNormalWithOverlay;
        static const long viewInternal;
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
        wxMenu* Menu3;
        wxMenuItem* MenuItem3;
        wxMenuItem* MenuItem4;
        wxMenuItem* MenuItem5;
        wxMenuItem* MenuItem6;
        wxMenuItem* MenuItem7;
        wxMenuItem* MenuItem8;
        wxSpinCtrlDouble* Circularity;
        wxSpinCtrlDouble* Convexity;
        wxSpinCtrlDouble* Inertia;
        wxSpinCtrlDouble* MaxArea;
        wxSpinCtrlDouble* MaxThreshold;
        wxSpinCtrlDouble* MinArea;
        wxSpinCtrlDouble* MinThreshold;
        wxStaticBox* StaticBox1;
        wxStaticBox* StaticBox2;
        wxStaticBox* StaticBox3;
        wxStaticLine* StaticLine1;
        wxStaticText* CorrectionsText;
        wxStaticText* ResultText;
        wxStaticText* SpeedText;
        wxStaticText* StaticText1;
        wxStatusBar* StatusBar1;
        //*)

        //This is our output!
        unsigned int colonyCounter;
        signed int userChangeToColonyCounter;

        unsigned int mousePositionX;
        unsigned int mousePositionY;

        wxBitmap * visualizationBitmap;
        wxImage * visualizationImage;
        struct processingInformation settings;

        void render(wxDC& dc);
        void OnPaint(wxPaintEvent& evt);
        void OnMotion(wxMouseEvent& event);
        void OnLeftClick(wxMouseEvent& event);
        void OnRightClick(wxMouseEvent& event);


        void OnSwitchVisualizationToRGB(wxCommandEvent& event);
        void OnSwitchVisualizationToRGBWithOverlay(wxCommandEvent& event);
        void OnSwitchVisualizationToInternal(wxCommandEvent& event);


        DECLARE_EVENT_TABLE()
};

#endif // PETRIDISHCOUNTERMAIN_H
