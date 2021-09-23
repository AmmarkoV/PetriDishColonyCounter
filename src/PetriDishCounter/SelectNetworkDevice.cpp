#include "SelectNetworkDevice.h"

//(*InternalHeaders(SelectNetworkDevice)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(SelectNetworkDevice)
const long SelectNetworkDevice::ID_BUTTON1 = wxNewId();
const long SelectNetworkDevice::ID_BUTTON2 = wxNewId();
const long SelectNetworkDevice::ID_TEXTCTRL1 = wxNewId();
const long SelectNetworkDevice::ID_STATICTEXT1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(SelectNetworkDevice,wxDialog)
	//(*EventTable(SelectNetworkDevice)
	//*)
END_EVENT_TABLE()

SelectNetworkDevice::SelectNetworkDevice(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(SelectNetworkDevice)
	Create(parent, id, _("Connect To Network Device"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(486,152));
	ButtonConnect = new wxButton(this, ID_BUTTON1, _("Connect"), wxPoint(24,88), wxSize(328,34), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	ButtonCancel = new wxButton(this, ID_BUTTON2, _("Cancel"), wxPoint(368,88), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	URI = new wxTextCtrl(this, ID_TEXTCTRL1, _("192.168.1.33"), wxPoint(24,40), wxSize(432,34), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Address"), wxPoint(24,16), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	//*)
}

SelectNetworkDevice::~SelectNetworkDevice()
{
	//(*Destroy(SelectNetworkDevice)
	//*)
}

