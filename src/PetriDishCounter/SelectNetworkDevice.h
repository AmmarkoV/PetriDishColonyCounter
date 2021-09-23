#ifndef SELECTNETWORKDEVICE_H
#define SELECTNETWORKDEVICE_H

//(*Headers(SelectNetworkDevice)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class SelectNetworkDevice: public wxDialog
{
	public:

		SelectNetworkDevice(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~SelectNetworkDevice();

		//(*Declarations(SelectNetworkDevice)
		wxButton* ButtonCancel;
		wxButton* ButtonConnect;
		wxStaticText* StaticText1;
		wxTextCtrl* URI;
		//*)

	protected:

		//(*Identifiers(SelectNetworkDevice)
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT1;
		//*)

	private:

		//(*Handlers(SelectNetworkDevice)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
