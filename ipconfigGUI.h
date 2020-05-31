/*
Josh Van Buren
Header file for ipconiggui
*/

#ifndef IPCONFIGGUI_H
#define IPCONFIGGUI_H

//Use unicode
#define UNICODE

//Includes
#include <winsock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <vector>
#include <wx/wx.h>
#include <wx/clipbrd.h>
#include <wx/aboutdlg.h>

//Use the standard namespace
using namespace std;

//Structure for adapter info
typedef struct localAdapterInfo  {
    wstring adaptName;
    wstring friendlyName;
    wstring ipV4Addr;
	wstring ipV6Addr;
	wstring dnsSuffix;
	wstring description;
	wstring macAddress;
	bool releaseable;
} localAdapterInfo;

//Class for the application
class ipconfigGUIApp : public wxApp
{
	//Public declarations
	public:
		virtual bool OnInit();
}; //Ends the class

//Class for the frame
class ipconfigGUIFrame : public wxFrame
{
	//Public declarations
	public:
		ipconfigGUIFrame(const wxString& title);
		void childClosed();
		
	//Private delcarations
	private:
		wstring getComputerName();
		void setComputerNameText();
		void getAdapterInfo();
		void refreshData();
		void refreshChildWindow();
		void panelKeyDown(wxKeyEvent& event);
		void OnExit(wxCommandEvent & event);
		void releaseIP(wxCommandEvent & event);
		void renewIP(wxCommandEvent & event);
		void releaseAllIP(wxCommandEvent & event);
		void renewAllIP(wxCommandEvent & event);
		void refresh(wxCommandEvent & event);
		void flushDNS(wxCommandEvent & event);
		void registerDNS(wxCommandEvent & event);
		void showIPConfigInfo(wxCommandEvent & event);
		void about(wxCommandEvent& event);
		vector<localAdapterInfo> adaptersInfo;
		vector<wstring> interfaceNames;
		bool childExists;
		wxWindow * childRef;
		
	//Declare the event table
	DECLARE_EVENT_TABLE();
	
	//Enumeration for events
	enum
	{
		//Event values
		idCName = 1000,
		idIPV4,
		idIPV6,
		idMAC,
		idRefresh,
		idRelease,
		idReleaseAll,
		idRenew,
		idRenewAll,
		idCombo,
		idFlushDNS,
		idRegisterDNS,
		idShowConfig
	};
}; //Ends the class

BEGIN_EVENT_TABLE(ipconfigGUIFrame, wxFrame)
	EVT_MENU    (wxID_EXIT, ipconfigGUIFrame::OnExit)
	EVT_MENU    (idRelease, ipconfigGUIFrame::releaseIP)
	EVT_MENU    (idRenew, ipconfigGUIFrame::renewIP)
	EVT_MENU    (idReleaseAll, ipconfigGUIFrame::releaseAllIP)
	EVT_MENU    (idRenewAll, ipconfigGUIFrame::renewAllIP)
	EVT_MENU    (idRefresh, ipconfigGUIFrame::refresh)
	EVT_MENU    (idFlushDNS, ipconfigGUIFrame::flushDNS)
	EVT_MENU    (idRegisterDNS, ipconfigGUIFrame::registerDNS)
	EVT_MENU	(idShowConfig, ipconfigGUIFrame::showIPConfigInfo)
	EVT_MENU	(wxID_ABOUT, ipconfigGUIFrame::about)
	EVT_BUTTON    (wxID_OK, ipconfigGUIFrame::OnExit)
	EVT_BUTTON    (idRelease, ipconfigGUIFrame::releaseIP)
	EVT_BUTTON    (idRenew, ipconfigGUIFrame::renewIP)
	EVT_BUTTON    (idReleaseAll, ipconfigGUIFrame::releaseAllIP)
	EVT_BUTTON    (idRenewAll, ipconfigGUIFrame::renewAllIP)
	EVT_BUTTON    (idRefresh, ipconfigGUIFrame::refresh)
	EVT_COMBOBOX	(idCombo, ipconfigGUIFrame::refresh)
END_EVENT_TABLE() //End event table

//Class for the ipconfig info
class ipconfigFrame : public wxFrame
{
	//Public variable declarations
	public:
		ipconfigFrame(wxWindow * parent, const wxString& title);
		void refreshFromParent();
		
	//Private variable declarations
	private:
		wstring getIPConfigOutput();
		void refreshData();
		void panelKeyDown(wxKeyEvent& event);
		void refresh(wxCommandEvent & event);
		void copyClipboard(wxCommandEvent & event);
		void OnExit(wxCommandEvent & event);
		void onClose(wxCloseEvent& evt);
		wxButton * okButton;
		wxButton * refreshButton;
		wxButton * copyClipboardButton;
		wxTextCtrl * ipconfigOutput;
		
	//Declare the event table
	DECLARE_EVENT_TABLE();
	
	//Enumeration for events
	enum
	{
		//Event values
		idRefresh = 2000,
		idCopyToClipboard
	};
};


BEGIN_EVENT_TABLE(ipconfigFrame, wxFrame)
	EVT_BUTTON    (wxID_OK, ipconfigFrame::OnExit)
	EVT_BUTTON    (idRefresh, ipconfigFrame::refresh)
	EVT_BUTTON    (idCopyToClipboard, ipconfigFrame::copyClipboard)
	EVT_CLOSE     (ipconfigFrame::onClose)
END_EVENT_TABLE() //End event table

///Function to convert a narrow string to wide
wchar_t * narrowStringToWide(char * narrowString);

#endif