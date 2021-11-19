﻿
// GiaoDienClientSocketDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "GiaoDienClientSocket.h"
#include "GiaoDienClientSocketDlg.h"
#include "afxdialogex.h"
#include "ClientScreen.h"
#include "LogNoti.h"
#include <cstring>
#include <string>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HANDLE checkServerThread;
HANDLE receiveLog;
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGiaoDienClientSocketDlg dialog

const int mPort = 5900;

CGiaoDienClientSocketDlg::CGiaoDienClientSocketDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GIAODIENCLIENTSOCKET_DIALOG, pParent)
	, mIpAddress(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGiaoDienClientSocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IP, txtIP);
	DDX_Control(pDX, IDC_Port, userName);
	//DDX_Control(pDX, IDC_LIST1, listUserLog);
	//DDX_Control(pDX, IDC_EDIT2, txtMsg);
	//DDX_Text(pDX, IDC_EDIT1, mIpAddress);
	//DDX_Text(pDX, IDC_EDIT3, mPort);
}

BEGIN_MESSAGE_MAP(CGiaoDienClientSocketDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_SEND, &CGiaoDienClientSocketDlg::OnBnClickedSend)
	ON_BN_CLICKED(BTN_CONNECT, &CGiaoDienClientSocketDlg::OnBnClickedConnect)
	ON_BN_CLICKED(BTN_REGISTER, &CGiaoDienClientSocketDlg::OnBnClickedRegister)
END_MESSAGE_MAP()


// CGiaoDienClientSocketDlg message handlers

BOOL CGiaoDienClientSocketDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGiaoDienClientSocketDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGiaoDienClientSocketDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGiaoDienClientSocketDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGiaoDienClientSocketDlg::OnOK()
{
	OnBnClickedSend();
}

void CGiaoDienClientSocketDlg::OnClose() {
	//kil thread
	TerminateThread(checkServerThread, 0);
	TerminateThread(receiveLog, 0);

	//tắt window
	CDialog::OnClose();
}

void CGiaoDienClientSocketDlg::OnBnClickedSend()
{
	//// TODO: Add your control notification handler code here
}




void CGiaoDienClientSocketDlg::OnBnClickedConnect()
{
	// TODO: Add your control notification handler code here
	txtIP.GetWindowText(mIpAddress);

	//if (AfxSocketInit() == FALSE)
	//{
	//	MessageBox(_T("Init socket failed!"));
	//	return;
	//}

	//if (mClientSocket.Create() == FALSE)
	//{
	//	MessageBox(_T("Socket create failed!"));
	//	return;
	//}

	//if (mClientSocket.Connect(mIpAddress, 9090) == FALSE)
	//{
	//	MessageBox(_T("Socket connect failed!"));
	//	return;
	//}

	std::string server_ip_string = CStringA(mIpAddress);
	// Init socket

	res = WSAStartup(MAKEWORD(2, 2), &w);
	if (res < 0)
	{
		printf("\nCannot Initialize socket lib");
	}
	//Open a socket - listener
	int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nSocket < 0)
	{
		//errno is a system global variable which gets updated
		//with the last API call return value/result.
		printf("\nCannot Initialize listener socket:%d", errno);
	}

	srv.sin_family = AF_INET;
	srv.sin_addr.s_addr = inet_addr(server_ip_string.c_str());
	srv.sin_port = htons(PORT);
	memset(&(srv.sin_zero), 0, 8);

	u_long optval = 0;

	res = ioctlsocket(nSocket, FIONBIO, &optval);
	if (res < 0)
	{
		printf("\nCannot connect to server:%d", errno);
		
		WSACleanup();
	}
	else {
		
		printf("Connect to server\n");
		char receive_buffer[256] = { 0 };
		string send_buffer;
		recv(nSocket, receive_buffer, 255, 0);
		//std::cout << "press any key to see the message from server ";
		//getchar();
		if (string(receive_buffer).compare("full") == 0) {
			cout << "Full queue!!!" << endl;
			getchar();
			cout << "Press any key to exit" << endl;
			WSACleanup();
			exit(EXIT_FAILURE);
		}
		cout << endl << receive_buffer << endl;

		int index = 0;


		// Name input
		CString name;
		GetDlgItemText(IDC_UserName, name);
		std::string name_string = CStringA(name);

		while (1) {

			if (recv(nSocket, receive_buffer, 256, 0) == -1) {
				break;
			}
			cout << endl << receive_buffer << endl;
			if (string(receive_buffer).compare("full") == 0) {
				cout << "Full queue!!!" << endl;
				// Message
				cout << "Press any key to exit" << endl;
				Sleep(3000);
				WSACleanup();
				exit(EXIT_FAILURE);
			}
			else if (string(receive_buffer).compare("Registration Completed Successfully") == 0) {
				// Message
				break;
			}
			send(nSocket, name_string.c_str(), 256, 0);
		}

		char receive_buffer1[256] = { 0, };
		vector<string> res;
		send_buffer = "";


		/*	while (1) {

				if (recv(nSocket, receive_buffer1, 256, 0) == -1) {
					break;
				}
				cout << endl << receive_buffer1 << endl;

				res = split(receive_buffer1, ",");
				index = 0;
				for (auto i : res) {
					cout << "Word: " << index++ << " - " << i << endl;
				}
				if (res.size() > 0 && res[6].compare("Your turn") == 0) {

					cout << "Input:" << endl;
					getline(cin, send_buffer);
					send_buffer.append(",").append("1");
					send(nSocket, send_buffer.c_str(), 256, 0);
					cout << "Sended.." << endl;
				}

				else {
					if (res.size() >= 8 && res[7] != "" && res[7].find("Lost") != std::string::npos) {
						cout << "Wait..." << endl;
						break;
					}
					if (res.size() >= 8 && res[7] != "" && res[7].find("Congratulations") != std::string::npos) {
						cout << "~~~ You win. End game ~~~" << endl;
						break;
					}
				}
			}
		}

		*/
		//MessageBox(textInput);

		CString mess;
		mess.Format(_T("Socket connect sucessfull with username %s"), name_string);
		MessageBox(mess);
		mClientSocket.SetSocketListener(this);
	}
}

void CGiaoDienClientSocketDlg::OnAccepted(CString ipAdress, int port) { }

vector<string> CGiaoDienClientSocketDlg::split(string s, string delimiter)
	{
	vector<std::string> res;
	int last = 0; int next = 0;
	while ((next = s.find(delimiter, last)) != string::npos) {
		res.push_back(s.substr(last, next - last));
		last = next + 1;
	}
	res.push_back(s.substr(last));
	return res;
}


void CGiaoDienClientSocketDlg::OnRecept(CString message)
{
	if (message.GetLength() > 0)
	{
		listUserLog.AddString(CString("Other: ") + message);
		listUserLog.SetCurSel(listUserLog.GetCount() - 1);
	}
}


void CGiaoDienClientSocketDlg::OnBnClickedRegister()
{
	// TODO: Add your control notification handler code here
	LogNoti noti;

	noti.DoModal();
}


//hàm để đóng hết tất cả nút
void CGiaoDienClientSocketDlg::CloseAllButtons()
{
	GetDlgItem(IDC_IP)->EnableWindow(FALSE);
	GetDlgItem(BTN_REGISTER)->EnableWindow(FALSE);
	GetDlgItem(BTN_REFRESH)->EnableWindow(FALSE);
}

//thread để check xem server có bị disconnect không
//DWORD WINAPI CheckServer(LPVOID socket) {
//	SOCKET server = (SOCKET)socket;
//	int iResult;
//	char buf[2];
//
//	while (1) {
//		iResult = recv(server, buf, 2, MSG_PEEK);
//		if (iResult == 0 || iResult == SOCKET_ERROR) {
//			//MessageBox(_T("Server disconnected!"), _T("Server disconnected!"), MB_OK | NULL);
//			//CloseAllButtons();
//			//ServerNotiDlg notif;
//			//notif.DoModal();
//			return 0;
//		}
//	}
//	return 0;
//}

//DWORD WINAPI ReceiveLog(LPVOID socket) {
//
//	SOCKET notiSocket = (SOCKET)socket;
//
//	int iResult;
//	char* buf;
//
//	while (1) {
//		iResult = recvMsg(notiSocket, buf);
//		if (iResult == 0 || iResult == SOCKET_ERROR) {
//			return 0;
//		}
//		//pop up window
//
//		string noti(buf);
//		LogNoti logWindow;
//		logWindow.text = noti;
//		logWindow.DoModal();
//		strcpy(buf, "");
//	}
//	return 0;
//}