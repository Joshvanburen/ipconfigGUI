/*
\author	Josh Van Buren
\brief Ipconfig GUI
\version	1.0
\date	May 25, 2020
*/

///Include the header
#include "ipconfigGUI.h"

///Include the icons
#include "ipconfigGUI.xpm"

//Macro to timplement the application
IMPLEMENT_APP(ipconfigGUIApp)

///Onl initialization
bool ipconfigGUIApp::OnInit()
{
	//Create an instance of the frame class
    ipconfigGUIFrame * tFrame = new ipconfigGUIFrame(L"Ipconfig GUI");

	//Center the frame
	tFrame->Centre();
	
	//Show the frame
	tFrame->Show();

	//Return
    return true;
} //End of the app class

///Frame constructor
ipconfigGUIFrame::ipconfigGUIFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(450, 360))
{
	//Get the adapter information
	getAdapterInfo();
	
	//Panel for the frame
	wxPanel * fPanel = new wxPanel(this, wxID_ANY);
	
	//Set the key event handler for the panel
	fPanel->Connect(wxEVT_CHAR, wxKeyEventHandler(ipconfigGUIFrame::panelKeyDown), NULL, this);
	
	//Creates the menu bar
	wxMenuBar * menubar = new wxMenuBar;
	
	//Creates the toolbar menus
	wxMenu * fileMenu = new wxMenu();
	wxMenu * editMenu = new wxMenu();
	wxMenu * helpMenu = new wxMenu();
	
	//Add the file menu to the menu bar
	menubar->Append(fileMenu, L"&File");
	menubar->Append(editMenu, L"&Edit");
	menubar->Append(helpMenu, L"&Help");
	
	//Add items to the file menu
	fileMenu->Append(wxID_EXIT, L"E&xit\tAlt-F4", L"Exits the program");
	
	//Add items to the exit menu
	editMenu->Append(idRelease, L"Relea&se\tCtrl+S", L"Releases the IP address of the selected adapter");
	editMenu->Append(idRenew, L"Re&new\tCtrl+N", L"Renews the IP address of the selected adapter");
	editMenu->AppendSeparator();
	editMenu->Append(idReleaseAll, L"Rele&ase All\tCtrl+A", L"Releases the IP address of all adapters");
	editMenu->Append(idRenewAll, L"Rene&w All\tCtrl+W", L"Renews the IP address of the all adapters");
	editMenu->AppendSeparator();
	editMenu->Append(idRefresh, L"&Refresh\tCtrl+R", L"Refreshes the adapter data");
	editMenu->AppendSeparator();
	editMenu->Append(idFlushDNS, L"&Flush DNS\tCtrl+F", L"Flushes the DNS resolver cache");
	editMenu->Append(idRegisterDNS, L"Register &DNS\tCtrl+D", L"Attempts to register the address in DNS");
	editMenu->AppendSeparator();
	editMenu->Append(idShowConfig, L"Show &Ipconfig Output", L"Shows full ipconfig output");
	
	//Add items to the help menu
	helpMenu->Append(wxID_ABOUT, L"&About...\tF1", L"Show about dialog");
	
	//Sets the menu bar
	SetMenuBar(menubar);
	
	//Initialize the combo box, clear it
	wxComboBox * adapterDescCombo = new wxComboBox(fPanel, idCombo);
	adapterDescCombo->Clear();
	
	//Add the items to the combo box
	for(int i = 0; i < adaptersInfo.size(); i++)
	{
		//Add the description
		adapterDescCombo->Append(adaptersInfo.at(i).description);
	} //Ends the for
	
	//Set the selection
	adapterDescCombo->SetSelection(0);
	
	//Create the buttons
	wxButton * okButton = new wxButton(fPanel, wxID_OK, L"&OK");
	wxButton * releaseButton = new wxButton(fPanel, idRelease, L"Relea&se");
	wxButton * releaseAllButton = new wxButton(fPanel, idReleaseAll, L"Rele&ase All");
	wxButton * renewButton = new wxButton(fPanel, idRenew, L"Re&new");
	wxButton * renewAllButton = new wxButton(fPanel, idRenewAll, L"Rene&w All");
	wxButton * refreshButton = new wxButton(fPanel, idRefresh, L"&Refresh");
	
	//Create labels for the information
	wxStaticText * compNameLabel = new wxStaticText(fPanel, wxID_ANY, L"Computer Name:", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	wxStaticText * adapterNameLabel = new wxStaticText(fPanel, wxID_ANY, L"Adapter Name:", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	wxStaticText * ipV4Label = new wxStaticText(fPanel, wxID_ANY, L"IPV4 Address:", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	wxStaticText * ipV6Label = new wxStaticText(fPanel, wxID_ANY, L"IPV6 Address:", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	wxStaticText * macLabel = new wxStaticText(fPanel, wxID_ANY, L"MAC Address:", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	
	//Create the text controls to hold the information
	wxTextCtrl * compNameInfoText = new wxTextCtrl(fPanel, idCName, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, wxTextCtrlNameStr);
	wxTextCtrl * ipV4InfoText = new wxTextCtrl(fPanel, idIPV4, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, wxTextCtrlNameStr);
	wxTextCtrl * ipV6InfoText = new wxTextCtrl(fPanel, idIPV6, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, wxTextCtrlNameStr);
	wxTextCtrl * macInfoText = new wxTextCtrl(fPanel, idMAC, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, wxTextCtrlNameStr);
	
	//Set the tool tips for all of the controls
	okButton->SetToolTip(L"Close (Alt+O)");
	releaseButton->SetToolTip(L"Releases the IP address of the selected adapter (Alt+S)");
	renewButton->SetToolTip(L"Renews the IP address of the selected adapter (Alt+N)");
	releaseAllButton->SetToolTip(L"Releases the IP address of all adapters (Alt+A)");
	renewAllButton->SetToolTip(L"Renews the IP address of all adapters (Alt+W)");
	refreshButton->SetToolTip(L"Refreshes the adapter data (Alt+R)");
	compNameInfoText->SetToolTip(L"Computer Name");
	ipV4InfoText->SetToolTip(L"IPV4 Address");
	ipV6InfoText->SetToolTip(L"IPV6 Address");
	macInfoText->SetToolTip(L"MAC Address");
	
	//Set the text for the computer name label
	setComputerNameText();
	
	//Create the box sizers
	wxBoxSizer * vboxMain = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer * vboxLeft = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer * vboxRight = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer * hbox1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * hbox2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * hbox3 = new wxBoxSizer(wxHORIZONTAL);
	
	//Add items to the sizers
	vboxLeft->Add(compNameLabel, 1, wxALL | wxEXPAND | wxALIGN_LEFT, 5);
	vboxLeft->Add(adapterNameLabel, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);
	vboxLeft->Add(ipV4Label, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);
	vboxLeft->Add(ipV6Label, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);
	vboxLeft->Add(macLabel, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);
	vboxRight->Add(compNameInfoText, 1, wxEXPAND | wxALL | wxALIGN_LEFT, 5);
	vboxRight->Add(adapterDescCombo, 1, wxEXPAND | wxALL | wxALIGN_LEFT, 5);
	vboxRight->Add(ipV4InfoText, 1, wxEXPAND | wxALL | wxALIGN_LEFT, 5);
	vboxRight->Add(ipV6InfoText, 1, wxEXPAND | wxALL | wxALIGN_LEFT, 5);
	vboxRight->Add(macInfoText, 1, wxEXPAND | wxALL | wxALIGN_LEFT, 5);
	hbox1->Add(vboxLeft, 1, wxALL | wxEXPAND, 5);
	hbox1->Add(vboxRight, 1, wxALL | wxEXPAND, 5);
	hbox2->Add(okButton, 1, wxALL | wxEXPAND, 5);
	hbox2->Add(releaseButton, 1, wxALL | wxEXPAND, 5);
	hbox2->Add(renewButton, 1, wxALL | wxEXPAND, 5);
	hbox3->Add(refreshButton, 1, wxALL | wxEXPAND, 5);
	hbox3->Add(releaseAllButton, 1, wxALL | wxEXPAND, 5);
	hbox3->Add(renewAllButton, 1, wxALL | wxEXPAND, 5);
	
	//Add the horizontal sizers to the vertical one
	vboxMain->Add(hbox1, 1, wxALL, 5);
	vboxMain->Add(hbox2, 0, wxALL | wxALIGN_CENTER, 5);
	vboxMain->Add(hbox3, 0, wxALL | wxALIGN_CENTER, 5);
	
	//Set the sizer for the panel
	fPanel->SetSizer(vboxMain);
	
	//Fit the sizer to the panel
	vboxMain->Fit(fPanel);
	
	//Set the icon
	SetIcon(wxIcon(ipconfigGUI_xpm));
	
	//Create the status bar
    CreateStatusBar(1);
    SetStatusText(L"Ipconfig GUI");
	
	//Refresh the data
	refreshData();
} //Ends the constructor

///Function that gets the computer name
wstring ipconfigGUIFrame::getComputerName()
{
	//Return string
	wstring retString;
	
	//Buffer size
	unsigned long cBuffSize = 255;
	
	//Create a buffer to get the computer name
	wchar_t * cNameBuff = new wchar_t[cBuffSize];
	
	//Get the computer name
	if(GetComputerName(cNameBuff, &cBuffSize) != 0)
	{
		//Set the string
		retString = wstring(cNameBuff);
	} //Ends the if
	//Else, failed, return unknown
	else
	{
		//Set the string
		retString = wstring(L"Unknown");
	} //Ends the else
		
	//Return
	return retString;
} //Ends the function

//Function that sets the text for the computer name text control
void ipconfigGUIFrame::setComputerNameText()
{
	//Get the computer name
	wxString compName(getComputerName());
	
	//Find the text control
	wxTextCtrl * compNameInfoText = (wxTextCtrl *)FindWindow(idCName);
	
	//Clear the control
	compNameInfoText->Clear();
	
	//Set the value
	compNameInfoText->SetValue(compName);
	
} //Ends the function

///Function that populates the adapter information
void ipconfigGUIFrame::getAdapterInfo()
{
	//Erase the buffer
	adaptersInfo.clear();
	
	//Interface info 
	PIP_INTERFACE_INFO interfaceInfo;
	
	//Free the memory
	if(interfaceInfo) { HeapFree(GetProcessHeap(), 0, (interfaceInfo)); }
	
	//Return value
	unsigned long retVal = 0;

	//Loop counter
	unsigned int i = 0;

	//Set the flags to pass to GetAdaptersAddresses
	unsigned long adaptFlags = GAA_FLAG_INCLUDE_PREFIX;

	//Default to unspecified address family (both)
	unsigned long adaptFamily = AF_UNSPEC;

	//Pointer to structure of adapter addresses
	PIP_ADAPTER_ADDRESSES adaptAddresses = NULL;
	
	//Length of that buffer
	unsigned long outBufLen = 15000;

	//Wide character string and size
	wchar_t buff[100];
	unsigned long bufflen = 100;

	//Pointer to structure of adapter addresses
	PIP_ADAPTER_ADDRESSES currAddress = NULL;
	
	//Pointer to unicast address
	PIP_ADAPTER_UNICAST_ADDRESS unicastAddress = NULL;
	
	//Allocate some memory
	adaptAddresses = (IP_ADAPTER_ADDRESSES *) HeapAlloc(GetProcessHeap(), 0, (outBufLen));

	//Try to get the adapter information
	retVal = GetAdaptersAddresses(adaptFamily, adaptFlags, NULL, adaptAddresses, &outBufLen);
	
	//If not enough memory
	if(retVal == ERROR_BUFFER_OVERFLOW)
	{
		//Free the memory
		HeapFree(GetProcessHeap(), 0, (adaptAddresses));
		adaptAddresses = NULL;
		
		//Return
		return;
	} //Ends the if
	
	//If successful
	if (retVal == NO_ERROR)
	{
		//Current adapter information
		currAddress = adaptAddresses;
		
		//Loop through all of the adapters
		while(currAddress)
		{
			//Create the struct to store the information
			localAdapterInfo loopAdapt;
			
			//Get the adapter name as wide string
			wchar_t * adaptNameW = narrowStringToWide(currAddress->AdapterName);
			
			//Set the propterties of the structure
			loopAdapt.adaptName = wstring(adaptNameW);
			loopAdapt.friendlyName = wstring(currAddress->FriendlyName);
			loopAdapt.dnsSuffix = wstring(currAddress->DnsSuffix);
			loopAdapt.description = wstring(currAddress->Description);
			loopAdapt.releaseable = false;
			
			//Get the first unicast address
			unicastAddress = currAddress->FirstUnicastAddress;
			
			//If it's not null
			if(unicastAddress != NULL)
			{
				//Loop through the addresses
				for(i = 0; unicastAddress != NULL; i++)
				{
					//If v4 address
					if (unicastAddress->Address.lpSockaddr->sa_family == AF_INET)
					{
						//Get the socket address
						sockaddr_in *sa_in = (sockaddr_in *)unicastAddress->Address.lpSockaddr;
						
						//Get the v4 address
						InetNtopW(AF_INET,&(sa_in->sin_addr),buff,bufflen);
						
						//Store the address
						loopAdapt.ipV4Addr = wstring(buff);
					} //Ends the if
					//If v6 address
					else if(unicastAddress->Address.lpSockaddr->sa_family == AF_INET6)
					{
						//Get the socket address
						sockaddr_in6 *sa_in6 = (sockaddr_in6 *)unicastAddress->Address.lpSockaddr;
						
						//Get the v6 address
						InetNtopW(AF_INET6,&(sa_in6->sin6_addr),buff,bufflen);
						
						//Store the address
						loopAdapt.ipV6Addr = wstring(buff);
					} //Ends the if
					//Else, do nothing
					else {}
					
					//Move on to the next address
					unicastAddress = unicastAddress->Next;
				} //Ends the for
			} //Ends the if	

			//If the physical address exists
			if(currAddress->PhysicalAddressLength != 0)
			{
				//Loop through the bytes of the address
				for (i = 0; i < (int)currAddress->PhysicalAddressLength; i++)
				{
					//If it's the end of the address
					if(i == (currAddress->PhysicalAddressLength - 1))
					{
						//Create a temp buffer
						wchar_t * tempBuff = new wchar_t[50];
						
						//Get the value
						swprintf(tempBuff, L"%.2X\n", (int) currAddress->PhysicalAddress[i]);
						
						//Add to the string
						loopAdapt.macAddress = loopAdapt.macAddress + wstring(tempBuff);
						
						//Delete the temporary buffer
						delete[] tempBuff;
					} //Ends the if
					//Else, not the end
					else
					{
						//Create a temp buffer
						wchar_t * tempBuff = new wchar_t[50];
						
						//Get the value
						swprintf(tempBuff, L"%.2X-", (int) currAddress->PhysicalAddress[i]);
						
						//Add to the string
						loopAdapt.macAddress = loopAdapt.macAddress + wstring(tempBuff);
						
						//Delete the temporary buffer
						delete[] tempBuff;						
					} //Ends the else
				} //Ends the for
			
			} //Ends the if
			//Else, set to nothing
			else { loopAdapt.macAddress = wstring(L"NULL"); } 
			
			//Push back the object onto the vector
			adaptersInfo.push_back(loopAdapt);
			
			//Next address
			currAddress = currAddress->Next;
		} //Ends the while
	} //Ends the if
	
	//Free the memory
	if(adaptAddresses) { HeapFree(GetProcessHeap(), 0, (adaptAddresses)); }
	
	//Pointer to the structure to get the information from the getinterfaceinfo function
	interfaceInfo = NULL;
	
	//Buffer length
	unsigned long iInfoSize = 0;
	
	//Call to get memory size needed
	retVal = GetInterfaceInfo(interfaceInfo, &iInfoSize);
	
	//If not enough memory
	if(retVal == ERROR_INSUFFICIENT_BUFFER)
	{
		//Allocate the memory
		interfaceInfo = (IP_INTERFACE_INFO *) HeapAlloc(GetProcessHeap(), 0, (iInfoSize));
		
		//Get the interface information
		retVal = GetInterfaceInfo(interfaceInfo, &iInfoSize);
		
		//Loop through the adapters
		for(i = 0; i < interfaceInfo -> NumAdapters; i++)
		{
			//Create the wstring
			wstring interfaceName = wstring(interfaceInfo->Adapter[i].Name);
			
			//Loop through the adapter addresses and see if we find a match
			for(int j = 0; j < adaptersInfo.size(); j++)
			{
				//If the adapter is found, it's releaseable
				if(interfaceName.find(adaptersInfo.at(j).adaptName) != wstring::npos)
				{
					//The adapter is releaseable
					adaptersInfo.at(j).releaseable = true;
				} //Ends the if
			} //Ends the for loop
		} //Ends the for loop
	} //Ends the if
} //Ends the function

///Function that refreshes the data in the window
void ipconfigGUIFrame::refreshData()
{
	//Get all new data
	getAdapterInfo();
	
	//Find the combo box
	wxComboBox * adapterDescCombo = (wxComboBox *)FindWindow(idCombo);
	
	//Get the currently selected string
	wxString currAdapter = adapterDescCombo->GetStringSelection();
	
	//Clears the combo box
	adapterDescCombo->Clear();
	
	//Add the items to the combo box
	for(int i = 0; i < adaptersInfo.size(); i++)
	{
		//Add the description
		adapterDescCombo->Append(adaptersInfo.at(i).description);
	} //Ends the for
	
	//Has the selection been found
	bool selFound = false;
	
	//Look for the adapter that was selected
	for(int j = 0; j < adapterDescCombo->GetCount(); j++)
	{
		//Look for a string match
		if(currAdapter.CmpNoCase(adapterDescCombo->GetString(j)) == 0)
		{
			//Set the selection
			adapterDescCombo->SetSelection(j);
			
			//Selection found
			selFound = true;
		} //Ends the if
	} //Ends the for
	
	//If it's not found, set the selection
	if(selFound == false)
	{
		//Set the selection, get the name of the current selection
		adapterDescCombo->SetSelection(0);
		currAdapter = adapterDescCombo->GetStringSelection();
	} //Ends the if
	
	//Find the adapter in the list
	for(int i = 0; i < adaptersInfo.size(); i++)
	{
		//Create a wxstring for comparison
		wxString compareString = wxString(adaptersInfo.at(i).description);
		
		//Compare the strings
		if(currAdapter.CmpNoCase(compareString) == 0)
		{
			//Find the text controls 
			wxTextCtrl * ipV4InfoText = (wxTextCtrl *)FindWindow(idIPV4);
			wxTextCtrl * ipV6InfoText = (wxTextCtrl *)FindWindow(idIPV6);
			wxTextCtrl * macInfoText = (wxTextCtrl *)FindWindow(idMAC);
			
			//Clear the text controls
			ipV4InfoText->Clear();
			ipV6InfoText->Clear();
			macInfoText->Clear();
			
			//Set the data
			ipV4InfoText->SetValue(wxString(adaptersInfo.at(i).ipV4Addr));
			ipV6InfoText->SetValue(wxString(adaptersInfo.at(i).ipV6Addr));
			macInfoText->SetValue(wxString(adaptersInfo.at(i).macAddress));
			
			//Get the menu bar from the frame
			wxMenuBar * fMenu = GetMenuBar();
			
			//Look for the edit menu
			int eMenuIndex = fMenu->FindMenu(L"Edit");
			
			//Gets the edit menu
			wxMenu * editMenu = fMenu->GetMenu(eMenuIndex);
			
			//Find the buttons needed 
			wxButton * releaseButton = (wxButton *)FindWindow(idRelease);
			wxButton * renewButton = (wxButton *)FindWindow(idRenew);
			
			//If the adapter is not releaseable, disable the button and menu selections
			if(adaptersInfo.at(i).releaseable == false)
			{
				//Disable the buttons
				releaseButton->Enable(false);
				renewButton->Enable(false);
				
				//Disable the menu items
				editMenu->Enable(idRelease, false);
				editMenu->Enable(idRenew, false);
			} //Ends the if
			//Else, enable the button and menu selections
			else
			{
				//Enable the buttons
				releaseButton->Enable(true);
				renewButton->Enable(true);
				
				//Enable the menu items
				editMenu->Enable(idRelease, true);
				editMenu->Enable(idRenew, true);			
			} //Ends the else
		} //Ends the if
	} //Ends the for
	
	//Set the text for the computer name
	setComputerNameText();
	
	//Calls the function to refresh the child window
	refreshChildWindow();
} //Ends the function

///Function that searches for child windows and calls the refresh method
void ipconfigGUIFrame::refreshChildWindow()
{
	//If the child exists
	if(childExists) { ((ipconfigFrame *)childRef)->refreshFromParent(); }
} //Ends the function

///Handles key events for the main panel
void ipconfigGUIFrame::panelKeyDown(wxKeyEvent& event)
{
	//If F5 has been pressed
	if(event.GetKeyCode() == WXK_F5)
		//Call the method to refresh
		refreshData();

	//Regular event handling
	event.Skip();
} //Ends the function

///Function that attempts to flush the dns cache
void ipconfigGUIFrame::flushDNS(wxCommandEvent & event)
{
	//Path to the DLL
	wchar_t * dnsAPIDLLPath = new wchar_t[512];
	ExpandEnvironmentStrings(L"%SystemRoot%\\system32\\dnsapi.dll", dnsAPIDLLPath, 512);
	
	//Load a handle to the dnsapi dll
	HINSTANCE dnsAPIDLL = LoadLibrary(dnsAPIDLLPath);
	
	//Delete the pointer
	delete[] dnsAPIDLLPath;
	
	//If failed
	if(dnsAPIDLL == NULL)
	{
		//Show error
		wxMessageBox(L"Error flushing the DNS resolver cache.", L"Error Flushing DNS", wxICON_ERROR | wxSTAY_ON_TOP, this);
	} //Ends the if
	//Else, call the function
	else
	{
		//Define a type for the function
		typedef unsigned long (*DnsFlushResolverCachePtr)();
		
		//Function pointer
		DnsFlushResolverCachePtr funcPtr = (DnsFlushResolverCachePtr)(GetProcAddress(dnsAPIDLL, "DnsFlushResolverCache"));
		
		//If error
		if(funcPtr == NULL)
		{
			//Show error
			wxMessageBox(L"Error flushing the DNS resolver cache.", L"Error Flushing DNS", wxICON_ERROR | wxSTAY_ON_TOP, this);
		} //Ends the if
		//Else, keep going
		else
		{
			//Call the function
			unsigned long res = funcPtr();
			
			//If successful
			if(res == 1)
			{
				//Success
				wxMessageBox(L"The DNS resolveer cache has been successfully flushed.", L"Successfully Flushed DNS", wxICON_INFORMATION | wxSTAY_ON_TOP, this);
			}
			//Else, error
			else
			{
				//Array for the error message
				wchar_t buf[512];
				
				//Get the error code and format it
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 0 , buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
				
				//Show the error
				wxMessageBox(wxString(buf), L"Error Flushing DNS", wxICON_ERROR | wxSTAY_ON_TOP, this);
			} //Else, error
		} //Ends the else
	} //Ends the else
} //Ends the function

///Function that attempts to register the address in DNS
void ipconfigGUIFrame::registerDNS(wxCommandEvent & event)
{
	//Path to the DLL
	wchar_t * dnsAPIDLLPath = new wchar_t[512];
	ExpandEnvironmentStrings(L"%SystemRoot%\\system32\\dnsapi.dll", dnsAPIDLLPath, 512);
	
	//Load a handle to the dnsapi dll
	HINSTANCE dnsAPIDLL = LoadLibrary(dnsAPIDLLPath);
	
	//Delete the pointer 
	delete[] dnsAPIDLLPath;
	
	//If failed
	if(dnsAPIDLL == NULL)
	{
		//Show error
		wxMessageBox(L"Error registering the DNS address.", L"Error Registering DNS", wxICON_ERROR | wxSTAY_ON_TOP, this);
	} //Ends the if
	//Else, call the function
	else
	{
		//Define a type for the function
		typedef unsigned long (*DnsAsyncRegisterInitPtr)();
		
		//Function pointer
		DnsAsyncRegisterInitPtr funcPtr = (DnsAsyncRegisterInitPtr)(GetProcAddress(dnsAPIDLL, "DnsAsyncRegisterInit"));

		//If error
		if(funcPtr == NULL)
		{
			//Show error
			//wxMessageBox(L"Error registering the DNS address.", L"Error Registering DNS", wxICON_ERROR | wxSTAY_ON_TOP, this);
		} //Ends the if
		//Else, keep going
		else
		{
			//Call the function
			unsigned long res = funcPtr();
				
			//Show the error
			//wxMessageBox(L"Error registering the DNS address.", L"Error Registering DNS", wxICON_ERROR | wxSTAY_ON_TOP, this);
		} //Ends the else
	} //Ends the else
		
	//Process information, startup information
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION processInfo;
	
	//Sets up the memory
	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	ZeroMemory(&processInfo, sizeof(processInfo));
	
	//Don't show the window
	startupInfo.dwFlags = STARTF_USESHOWWINDOW;
	
	//Process command line
	wchar_t * procCMDLine = new wchar_t[512];
	ExpandEnvironmentStrings(L"%SystemRoot%\\system32\\ipconfig.exe \/registerdns", procCMDLine, 512);
	
	//Runs the system command
	CreateProcess(NULL, procCMDLine, NULL, NULL, 0, CREATE_NO_WINDOW, NULL, 
		NULL, &startupInfo, &processInfo);
		
	//Wait for the process to exit.
	WaitForSingleObject(processInfo.hProcess, 0);
	
	//Close our reference to the initial thread
	CloseHandle(processInfo.hThread);

	//And close our reference to the process.
	CloseHandle(processInfo.hProcess);
	
	//Free the pointer
	delete[] procCMDLine;
	
	//Show message
	wxMessageBox(L"Registration of the DNS resource records for all adapters of this computer has been initiated. Any errors will be reported in the Event Viewer in 15 minutes.",
		L"Attempting to Register DNS Resource Records", wxICON_INFORMATION | wxSTAY_ON_TOP, this);
} //Ends the function

///Method that handles the quit event
void ipconfigGUIFrame::OnExit(wxCommandEvent & WXUNUSED(event))
{
	//Closes the window
    Close(true);
} //Ends the function

///Releases the IP for the selected adapter
void ipconfigGUIFrame::releaseIP(wxCommandEvent& event)
{
	//Find the button
	wxButton * releaseButton = (wxButton *)FindWindow(idRelease);
	
	//Find the combo box
	wxComboBox * adapterDescCombo = (wxComboBox *)FindWindow(idCombo);
	
	//Disable the release button
	releaseButton->Enable(false);
	
	//Get the menu bar from the frame
	wxMenuBar * fMenu = GetMenuBar();
	
	//Look for the edit menu
	int eMenuIndex = fMenu->FindMenu(L"Edit");
	
	//Gets the edit menu
	wxMenu * editMenu = fMenu->GetMenu(eMenuIndex);
	
	//Disable the menu item
	editMenu->Enable(idRelease, false);
	
	//Get the currently selected adapter
	wstring currAdapter = adapterDescCombo->GetStringSelection().wc_str();
	
	//Adapter name for the match
	wstring matchName;
	
	//Has the selection been found
	bool selFound = false;
	
	//Look for the adapter that was selected
	for(int i = 0; i < adaptersInfo.size(); i++)
	{
		//Look for a string match
		if(currAdapter.find(adaptersInfo.at(i).description) != wstring::npos)
		{
			//Get the adapter name
			matchName = adaptersInfo.at(i).adaptName;
			
			//Selection found
			selFound = true;
		} //Ends the if
	} //Ends the for
	
	//Pointer to the structure to get the information from the getinterfaceinfo function
	PIP_INTERFACE_INFO interfaceInfo = NULL;
	
	//Buffer length
	unsigned long iInfoSize = 0;
	
	//Call to get memory size needed
	unsigned long retVal = GetInterfaceInfo(interfaceInfo, &iInfoSize);
	
	//If not enough memory
	if(retVal == ERROR_INSUFFICIENT_BUFFER)
	{
		//Allocate the memory
		interfaceInfo = (IP_INTERFACE_INFO *) HeapAlloc(GetProcessHeap(), 0, (iInfoSize));
		
		//Get the interface information
		retVal = GetInterfaceInfo(interfaceInfo, &iInfoSize);
		
		//Loop through the adapters
		for(int j = 0; j < interfaceInfo -> NumAdapters; j++)
		{
			//Create the wstring
			wstring interfaceName = wstring(interfaceInfo->Adapter[j].Name);
			
			//If match found
			if(interfaceName.find(matchName) != wstring::npos)
			{
				//The adapter is releaseable, attempt to release it.
				retVal = IpReleaseAddress(&interfaceInfo->Adapter[j]);
				
				//If error
				if(retVal != NO_ERROR)
				{
					//If the IP has already been released, i.e. has 169 address
					if(retVal == 1228){}
					//Else if the adapter actually cannot be released
					else if(retVal == 2){}
					//Else, show the error
					else
					{
						//Buffer to hold the message
						wchar_t * sBuff;
						
						//Format the error
						FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, retVal, 0,
							(wchar_t *)&sBuff, 0, NULL);
							
						//String to display
						wxString eMessage = wxString(L"Error releasing IP address ") + wxString(sBuff);
						
						//Show the error
						//wxMessageBox(wxString::Format(L"Error releasing IP address %ld", retVal), L"Error Releasing IP Address", wxICON_ERROR | wxSTAY_ON_TOP, this);
						wxMessageBox(eMessage, L"Error Releasing IP Address", wxICON_ERROR | wxSTAY_ON_TOP, this);
					} //Ends the else
				} //Ends the if
				//Else, sleep so the IP can refresh
				else
				{
					//Sleep for eight seconds
					Sleep(8000);
				} //Ends the else
			} //Ends the if
		} //Ends the for loop
	} //Ends the if	
	
	//Calls the function to refresh the data
	refreshData();
} //Ends the function

///Renews the IP for the selected adapter
void ipconfigGUIFrame::renewIP(wxCommandEvent& event)
{
	//Find the button
	wxButton * renewButton = (wxButton *)FindWindow(idRenew);
	
	//Find the combo box
	wxComboBox * adapterDescCombo = (wxComboBox *)FindWindow(idCombo);
	
	//Disable the renew button
	renewButton->Enable(false);
	
	//Get the menu bar from the frame
	wxMenuBar * fMenu = GetMenuBar();
	
	//Look for the edit menu
	int eMenuIndex = fMenu->FindMenu(L"Edit");
	
	//Gets the edit menu
	wxMenu * editMenu = fMenu->GetMenu(eMenuIndex);
	
	//Disable the menu item
	editMenu->Enable(idRenew, false);
	
	//Get the currently selected adapter
	wstring currAdapter = adapterDescCombo->GetStringSelection().wc_str();
	
	//Adapter name for the match
	wstring matchName;
	
	//Has the selection been found
	bool selFound = false;
	
	//Look for the adapter that was selected
	for(int i = 0; i < adaptersInfo.size(); i++)
	{
		//Look for a string match
		if(currAdapter.find(adaptersInfo.at(i).description) != wstring::npos)
		{
			//Get the adapter name
			matchName = adaptersInfo.at(i).adaptName;
			
			//Selection found
			selFound = true;
		} //Ends the if
	} //Ends the for
	
	//Pointer to the structure to get the information from the getinterfaceinfo function
	PIP_INTERFACE_INFO interfaceInfo = NULL;
	
	//Buffer length
	unsigned long iInfoSize = 0;
	
	//Call to get memory size needed
	unsigned long retVal = GetInterfaceInfo(interfaceInfo, &iInfoSize);
	
	//If not enough memory
	if(retVal == ERROR_INSUFFICIENT_BUFFER)
	{
		//Allocate the memory
		interfaceInfo = (IP_INTERFACE_INFO *) HeapAlloc(GetProcessHeap(), 0, (iInfoSize));
		
		//Get the interface information
		retVal = GetInterfaceInfo(interfaceInfo, &iInfoSize);
		
		//Loop through the adapters
		for(int j = 0; j < interfaceInfo -> NumAdapters; j++)
		{
			//Create the wstring
			wstring interfaceName = wstring(interfaceInfo->Adapter[j].Name);
			
			//If match found
			if(interfaceName.find(matchName) != wstring::npos)
			{
				//The adapter is renewable, attempt to renew it.
				retVal = IpRenewAddress(&interfaceInfo->Adapter[j]);
				
				//If error
				if(retVal != NO_ERROR)
				{
					//Buffer to hold the message
					wchar_t * sBuff;
					
					//Format the error
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, retVal, 0,
						(wchar_t *)&sBuff, 0, NULL);
						
					//String to display
					wxString eMessage = wxString(L"Error renewing IP address ") + wxString(sBuff);
					
					//Show the error
					//wxMessageBox(wxString::Format(L"Error renewing IP address %ld", retVal), L"Error Renewing IP Address", wxICON_ERROR | wxSTAY_ON_TOP, this);
					wxMessageBox(eMessage, L"Error Renewing IP Address", wxICON_ERROR | wxSTAY_ON_TOP, this);
				} //Ends the if
			} //Ends the if
		} //Ends the for loop
	} //Ends the if	
	
	//Calls the function to refresh the data
	refreshData();
} //Ends the function

///Releases the IP for all adapters
void ipconfigGUIFrame::releaseAllIP(wxCommandEvent& event)
{	
	//Find the button
	wxButton * releaseAllButton = (wxButton *)FindWindow(idReleaseAll);
	
	//Disable the release all button
	releaseAllButton->Enable(false);
	
	//Get the menu bar from the frame
	wxMenuBar * fMenu = GetMenuBar();
	
	//Look for the edit menu
	int eMenuIndex = fMenu->FindMenu(L"Edit");
	
	//Gets the edit menu
	wxMenu * editMenu = fMenu->GetMenu(eMenuIndex);
	
	//Disable the menu item
	editMenu->Enable(idReleaseAll, false);
	
	//Pointer to the structure to get the information from the getinterfaceinfo function
	PIP_INTERFACE_INFO interfaceInfo = NULL;
	
	//Buffer length
	unsigned long iInfoSize = 0;
	
	//Call to get memory size needed
	unsigned long retVal = GetInterfaceInfo(interfaceInfo, &iInfoSize);
	
	//If not enough memory
	if(retVal == ERROR_INSUFFICIENT_BUFFER)
	{
		//Allocate the memory
		interfaceInfo = (IP_INTERFACE_INFO *) HeapAlloc(GetProcessHeap(), 0, (iInfoSize));
		
		//Get the interface information
		retVal = GetInterfaceInfo(interfaceInfo, &iInfoSize);
		
		//Loop through the adapters
		for(int j = 0; j < interfaceInfo -> NumAdapters; j++)
		{
			//The adapter is releaseable, attempt to release it.
			retVal = IpReleaseAddress(&interfaceInfo->Adapter[j]);
			
			//If error
			if(retVal != NO_ERROR)
			{
				//If the IP has already been released, i.e. has 169 address
				if(retVal == 1228){}
				//Else if the adapter actually cannot be released
				else if(retVal == 2){}
				//Else, show the error
				else
				{
					//Buffer to hold the message
					wchar_t * sBuff;
					
					//Format the error
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, retVal, 0,
						(wchar_t *)&sBuff, 0, NULL);
						
					//String to display
					wxString eMessage = wxString(L"Error releasing IP address ") + wxString(sBuff);
					
					//Show the error
					//wxMessageBox(wxString::Format(L"Error releasing IP address %ld", retVal), L"Error Releasing IP Address", wxICON_ERROR | wxSTAY_ON_TOP, this);
					wxMessageBox(eMessage, L"Error Releasing IP Address", wxICON_ERROR | wxSTAY_ON_TOP, this);
				} //Ends the else
			} //Ends the if
		} //Ends the for loop
	} //Ends the if

	//Process information, startup information
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION processInfo;
	
	//Sets up the memory
	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	ZeroMemory(&processInfo, sizeof(processInfo));
	
	//Don't show the window
	startupInfo.dwFlags = STARTF_USESHOWWINDOW;
	
	//Process command line
	wchar_t * procCMDLine = new wchar_t[512];
	ExpandEnvironmentStrings(L"%SystemRoot%\\system32\\ipconfig.exe \/release6", procCMDLine, 512);
	
	//Runs the system command
	CreateProcess(NULL, procCMDLine, NULL, NULL, 0, CREATE_NO_WINDOW, NULL, 
		NULL, &startupInfo, &processInfo);
		
	//Wait for the process to exit.
	WaitForSingleObject(processInfo.hProcess, 0);
	
	//Close our reference to the initial thread
	CloseHandle(processInfo.hThread);

	//And close our reference to the process.
	CloseHandle(processInfo.hProcess);
	
	//Free the pointer
	delete[] procCMDLine;
	
	//Enable the release all button
	releaseAllButton->Enable(true);
	
	//Disable the menu item
	editMenu->Enable(idReleaseAll, true);
	
	//Sleep for eight seconds
	Sleep(8000);
	
	//Calls the function to refresh the data
	refreshData();
} //Ends the function

///Renews the IP for all adapters
void ipconfigGUIFrame::renewAllIP(wxCommandEvent& event)
{
	//Find the button
	wxButton * renewAllButton = (wxButton *)FindWindow(idRenewAll);
	
	//Disable the renew all button
	renewAllButton->Enable(false);
	
	//Get the menu bar from the frame
	wxMenuBar * fMenu = GetMenuBar();
	
	//Look for the edit menu
	int eMenuIndex = fMenu->FindMenu(L"Edit");
	
	//Gets the edit menu
	wxMenu * editMenu = fMenu->GetMenu(eMenuIndex);
	
	//Disable the menu item
	editMenu->Enable(idRenewAll, false);
	
	//Pointer to the structure to get the information from the getinterfaceinfo function
	PIP_INTERFACE_INFO interfaceInfo = NULL;
	
	//Buffer length
	unsigned long iInfoSize = 0;
	
	//Call to get memory size needed
	unsigned long retVal = GetInterfaceInfo(interfaceInfo, &iInfoSize);
	
	//If not enough memory
	if(retVal == ERROR_INSUFFICIENT_BUFFER)
	{
		//Allocate the memory
		interfaceInfo = (IP_INTERFACE_INFO *) HeapAlloc(GetProcessHeap(), 0, (iInfoSize));
		
		//Get the interface information
		retVal = GetInterfaceInfo(interfaceInfo, &iInfoSize);
		
		//Loop through the adapters
		for(int j = 0; j < interfaceInfo -> NumAdapters; j++)
		{
			//The adapter is renewable, attempt to renew it.
			retVal = IpRenewAddress(&interfaceInfo->Adapter[j]);
			
			//If error
			if(retVal != NO_ERROR)
			{
				//If the adapter cannot actually be renewed, don't show error on the all function
				if(retVal == 2) {}
				//Else, show error
				else
				{
					//Buffer to hold the message
					wchar_t * sBuff;
					
					//Format the error
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, retVal, 0,
						(wchar_t *)&sBuff, 0, NULL);
						
					//String to display
					wxString eMessage = wxString(L"Error renewing IP address ") + wxString(sBuff);
					
					//Show the error
					//wxMessageBox(wxString::Format(L"Error renewing IP address %ld", retVal), L"Error Renewing IP Address", wxICON_ERROR | wxSTAY_ON_TOP, this);
					wxMessageBox(eMessage, L"Error Renewing IP Address", wxICON_ERROR | wxSTAY_ON_TOP, this);
				} //Ends the else
			} //Ends the if
		} //Ends the for loop
	} //Ends the if	
	
	//Process information, startup information
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION processInfo;
	
	//Sets up the memory
	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	ZeroMemory(&processInfo, sizeof(processInfo));
	
	//Don't show the window
	startupInfo.dwFlags = STARTF_USESHOWWINDOW;
	
	//Process command line
	wchar_t * procCMDLine = new wchar_t[512];
	ExpandEnvironmentStrings(L"%SystemRoot%\\system32\\ipconfig.exe \/renew6", procCMDLine, 512);
	
	//Runs the system command
	CreateProcess(NULL, procCMDLine, NULL, NULL, 0, CREATE_NO_WINDOW, NULL, 
		NULL, &startupInfo, &processInfo);
		
	//Wait for the process to exit.
	WaitForSingleObject(processInfo.hProcess, 0);
	
	//Close our reference to the initial thread
	CloseHandle(processInfo.hThread);

	//And close our reference to the process.
	CloseHandle(processInfo.hProcess);
	
	//Free the pointer
	delete[] procCMDLine;	
	
	//Enable the renew all button
	renewAllButton->Enable(true);
	
	//Re-enable the menu item
	editMenu->Enable(idRenewAll, true);
	
	//Calls the function to refresh the data
	refreshData();
} //Ends the function

///Refreshes the adapter data
void ipconfigGUIFrame::refresh(wxCommandEvent& event)
{
	//Calls the function to refresh the data
	refreshData();
} //Ends the function

///Function that shows a window with full ipconfig /all output
void ipconfigGUIFrame::showIPConfigInfo(wxCommandEvent & event)
{
	//Get the menu bar from the frame
	wxMenuBar * fMenu = GetMenuBar();
	
	//Look for the edit menu
	int eMenuIndex = fMenu->FindMenu(L"Edit");
	
	//Gets the edit menu
	wxMenu * editMenu = fMenu->GetMenu(eMenuIndex);
	
	//Disable the menu item to show the output
	editMenu->Enable(idShowConfig, false);
	
	//Create the frame
	ipconfigFrame * iFrame = new ipconfigFrame(this, L"IPConfig");
	
	//Child exists, set refrence
	childExists = true;
	childRef = iFrame;
	
	//Center the frame
	iFrame->Centre();
	
	//Show the frame
	iFrame->Show();
} //Ends the function

///Function that shows the about dialog
void ipconfigGUIFrame::about(wxCommandEvent& event)
{
	//Creates the dialog and sets the properties
	wxAboutDialogInfo aboutInfo;
	aboutInfo.SetName(L"Ipconfig GUI");
	aboutInfo.SetVersion(L"1.0");
	aboutInfo.SetDescription(L"IP Information in a GUI");
	//aboutInfo.SetCopyright("(C) 2011");
	aboutInfo.SetWebSite("https://github.com/Joshvanburen");
	aboutInfo.AddDeveloper("Josh Van Buren");
	aboutInfo.SetIcon(wxIcon(ipconfigGUI_xpm));

	//Displays the box
	wxAboutBox(aboutInfo);
}

//Function that alerts the parent window that the child has closedir
void ipconfigGUIFrame::childClosed()
{
	//Child closed
	childExists = false;
	
	//Get the menu bar from the frame
	wxMenuBar * fMenu = GetMenuBar();
	
	//Look for the edit menu
	int eMenuIndex = fMenu->FindMenu(L"Edit");
	
	//Gets the edit menu
	wxMenu * editMenu = fMenu->GetMenu(eMenuIndex);
	
	//Re-enable the menu item
	editMenu->Enable(idShowConfig, true);
} //Ends the function

///Frame constructor
ipconfigFrame::ipconfigFrame(wxWindow * parent, const wxString& title)
       : wxFrame(parent, wxID_ANY, title, wxDefaultPosition, wxSize(675, 500))
{
	//Panel for the frame
	wxPanel * fPanel = new wxPanel(this, wxID_ANY);
	
	//Set the key event handler for the panel
	fPanel->Connect(wxEVT_CHAR, wxKeyEventHandler(ipconfigFrame::panelKeyDown), NULL, this);
	
	//Create the buttons
	okButton = new wxButton(fPanel, wxID_OK, L"&OK");
	refreshButton = new wxButton(fPanel, idRefresh, L"&Refresh");
	copyClipboardButton = new wxButton(fPanel, idCopyToClipboard, L"&Copy");
	
	//Set the tool tips for all of the controls
	okButton->SetToolTip(L"Close (Alt+O)");
	refreshButton->SetToolTip(L"Refreshes the adapter data (Alt+R)");
	copyClipboardButton->SetToolTip(L"Copy the text to the clipboard (Alt+C)");
	
	//Create the text control
	ipconfigOutput = new wxTextCtrl(fPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE, wxDefaultValidator, wxTextCtrlNameStr);
	
	//Get the font for the control
	wxFont iOFont = ipconfigOutput->GetFont();
	
	//Set the family for the font
	iOFont.SetFamily(wxFONTFAMILY_TELETYPE);
	
	//Set the font
	ipconfigOutput->SetFont(iOFont);
	
	//Create the box sizers
	wxBoxSizer * vboxMain = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer * hbox1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * hbox2 = new wxBoxSizer(wxHORIZONTAL);
	
	//Add items to the sizers
	hbox1->Add(ipconfigOutput, 1, wxEXPAND | wxALL | wxALIGN_LEFT, 5);
	hbox2->Add(okButton, 1, wxALL | wxEXPAND, 5);
	hbox2->Add(refreshButton, 1, wxALL | wxEXPAND, 5);
	hbox2->Add(copyClipboardButton, 1, wxALL | wxEXPAND, 5);
	
	//Add the horizontal sizers to the vertical one
	vboxMain->Add(hbox1, 1, wxALL | wxEXPAND, 5);
	vboxMain->Add(hbox2, 0, wxALL | wxALIGN_CENTER, 5);
	
	//Set the sizer for the panel
	fPanel->SetSizer(vboxMain);
	
	//Fit the sizer to the panel
	vboxMain->Fit(fPanel);
	
	//Set the icon
	SetIcon(wxIcon(ipconfigGUI_xpm));
	
	//Create the status bar
    //CreateStatusBar(1);
    //SetStatusText(L"Full Ipconfig Information");
	
	//Refresh the data
	refreshData();
} //Ends the function

///Function that handles the refresh call from the parent
void ipconfigFrame::refreshFromParent()
{
	//Calls the refresh method
	refreshData();
} //Ends the function

///Function that gets the ipconfig output
wstring ipconfigFrame::getIPConfigOutput()
{
	//String to return eventually
	wstring returnString;
	
	//Handles for the read and write info for the child process
	HANDLE childStdOutRead = NULL;
	HANDLE childStdOutWrite = NULL;
	
	//Security attributes for the pipes to be created to run the command
	SECURITY_ATTRIBUTES secAttr;
	
	//Set the security attributes
	secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	secAttr.bInheritHandle = 1;
	secAttr.lpSecurityDescriptor = NULL;
	
	//Create the pipe for out
	if(CreatePipe(&childStdOutRead, &childStdOutWrite, &secAttr, 0) != 0)
	{
		//Ensure read handle not inherited
		if(SetHandleInformation(childStdOutRead, HANDLE_FLAG_INHERIT, 0) != 0)
		{
			//Process information, startup information
			STARTUPINFO startupInfo;
			PROCESS_INFORMATION processInfo;
			
			//Sets up the memory
			ZeroMemory(&startupInfo, sizeof(startupInfo));
			startupInfo.cb = sizeof(startupInfo);
			ZeroMemory(&processInfo, sizeof(processInfo));
			
			//Startup handles and info
			startupInfo.hStdError = childStdOutWrite;
			startupInfo.hStdOutput = childStdOutWrite;
			startupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);;
			startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
			
			//Process command line
			wchar_t * procCMDLine = new wchar_t[512];
			ExpandEnvironmentStrings(L"%SystemRoot%\\system32\\ipconfig.exe \/all", procCMDLine, 512);
			
			//Dialog result
			int dialogRes = 0;
			
			//Create the process
			unsigned long res = CreateProcess(NULL, procCMDLine, NULL, NULL, 1, CREATE_UNICODE_ENVIRONMENT | CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInfo);

			//Wait for the process to return, max tens seconds
			WaitForSingleObject(processInfo.hProcess, 10000);
			
			//Close the read handle
			CloseHandle(childStdOutWrite);
			
			//Failed to launch the process
			if(res == 0)
			{
				//Array for the error message
				wchar_t buf[512];
				
				//Get the error code and format it
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 0 , buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
				
				//Show the error
				//dialogRes = wxMessageBox(wxString(buf), L"Error Creating the Process", wxICON_ERROR | wxSTAY_ON_TOP, NULL);
				
				//Error string
				returnString = wstring(L"Error running ipconfig /all.");
			} //Ends the if
			else
			{
				//Variable to hold the exit code
				unsigned long eCode;
				
				//Get the return code
				res = GetExitCodeProcess(processInfo.hProcess, &eCode);
				
				//If failed, get the error code
				if(res == 0)
				{
					//Array for the error message
					wchar_t buf[512];
					
					//Get the error code and format it
					FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 0 , buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
					
					//Show the error
					//dialogRes = wxMessageBox(wxString(buf), L"Error Getting the Return Code", wxICON_ERROR | wxSTAY_ON_TOP, NULL);
					
					//Error string
					returnString = wstring(L"Error running ipconfig /all.");
				} //Ends the if
				//Else, show the error code
				else
				{
					//Buffer to read from the output loop
					char readBuf[4096] {};
					
					//Bytes read, available
					unsigned long bytesRead, bytesAvail = 0;
					
					//Wide string to store the output
					string ipcOutput;

					//Read some data
					res = ReadFile(childStdOutRead, readBuf, 4095, &bytesRead, NULL);
					
					//While successful read more
					while(res == 1)
					{
						//Set the null terminator
						readBuf[bytesRead] = '\0';
						
						//Temp string
						string tempString(readBuf);
						
						//Add to the output
						ipcOutput = ipcOutput + tempString;
						
						//Read more
						res = ReadFile(childStdOutRead, readBuf, 4095, &bytesRead, NULL);
					} //Ends the while
					
					//Create the string to return
					returnString = wstring(narrowStringToWide((char *)ipcOutput.c_str()));
					
					//dialogRes = wxMessageBox(ipOutput, L"Ipconfig Output", wxICON_INFORMATION | wxSTAY_ON_TOP, NULL);
					
				} //Ends the else
				
				//Close our reference to the initial thread
				CloseHandle(processInfo.hThread);

				//And close our reference to the process.
				CloseHandle(processInfo.hProcess);	
			} //Ends the else
			
			//Close the write handles
			CloseHandle(childStdOutRead);
		} //Ends the if
		//Else, error
		else
		{
			//Show error
			//wxMessageBox(L"Error running ipconfig.", L"Error", wxICON_ERROR | wxSTAY_ON_TOP, this);
			
			//Error string
			returnString = wstring(L"Error running ipconfig /all.");
		} //Ends the if
	} //Ends the if
	//Else, error
	else
	{
		//Show error
		//wxMessageBox(L"Error running ipconfig.", L"Error", wxICON_ERROR | wxSTAY_ON_TOP, this);
		
		//Error string
		returnString = wstring(L"Error running ipconfig /all.");
	} //Ends the else
		
	//Return the return string
	return returnString;
} //Ends the function

///Function called to refresh the data in the frame
void ipconfigFrame::refreshData()
{
	//Get the ipconfig output
	wstring ipO = getIPConfigOutput();
	
	//Create a wxString from the output
	wxString controlText(ipO);
	
	//Set the control value
	ipconfigOutput->SetValue(controlText);
} //Ends the function

///Handles key events for the main panel
void ipconfigFrame::panelKeyDown(wxKeyEvent& event)
{
	//If F5 has been pressed
	if(event.GetKeyCode() == WXK_F5)
		//Call the method to refresh
		refreshData();

	//Regular event handling
	event.Skip();
} //Ends the function

///Refreshes the ipconfig data
void ipconfigFrame::refresh(wxCommandEvent& event)
{
	//Calls the function to refresh the data
	refreshData();
} //Ends the function

//Functio that copies the text to the clipboard
void ipconfigFrame::copyClipboard(wxCommandEvent & event)
{
	//Try to open the clipboard
	if(wxTheClipboard->Open())
	{
		//Clear the clipboard
		wxTheClipboard->Clear();
		
		//Set the text
		wxTheClipboard->SetData(new wxTextDataObject(ipconfigOutput->GetValue()));
		
		//Flush the clipboard
		wxTheClipboard->Flush();
		
		//Close the clipboard
		wxTheClipboard->Close();
	} //Ends the if
} //Ends the function

///Method that handles the quit event
void ipconfigFrame::OnExit(wxCommandEvent & WXUNUSED(event))
{
	//Closes the window
    Close(true);
} //Ends the function

///Method that handles the onclose event
void ipconfigFrame::onClose(wxCloseEvent& evt)
{
	//Send the message to the parent
	((ipconfigGUIFrame *)GetParent())->childClosed();
	
	//Close window
	evt.Skip();	
} //Ends the function

//Function that converts a narrow string to a wide string
wchar_t * narrowStringToWide(char * narrowString)
{
	//Get the size of the narrow string
	size_t nSize = strlen(narrowString) + 1;
	
	//Create a new wide string
	wchar_t * wideString = new wchar_t[nSize];
	
	//Copy the string
	mbstowcs(wideString, narrowString, nSize);
	
	//Return the wide string
	return wideString;
} //Ends the function