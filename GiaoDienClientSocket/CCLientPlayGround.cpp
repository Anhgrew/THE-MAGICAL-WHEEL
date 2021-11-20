// CCLientPlayGround.cpp : implementation file
//

#include "pch.h"
#include "GiaoDienClientSocket.h"
#include "CCLientPlayGround.h"
#include "afxdialogex.h"

void HiddenRemain(std::string guessWord, std::string& keyWord, std::string& msg, std::string& disWord);
// CCLientPlayGround dialog

IMPLEMENT_DYNAMIC(CCLientPlayGround, CDialogEx)

CCLientPlayGround::CCLientPlayGround(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENTPLAYGROUND, pParent)
{

}

CCLientPlayGround::~CCLientPlayGround()
{
}

void CCLientPlayGround::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTPG, listPlayGround);
	DDX_Control(pDX, IDC_GUESSWORD, txtGuessWord);
	DDX_Control(pDX, IDC_KEYWORD, txtKeyWord);
	DDX_Control(pDX, IDC_SCORE, txtScore);
}


BEGIN_MESSAGE_MAP(CCLientPlayGround, CDialogEx)
	ON_BN_CLICKED(IDC_SENDANSWER, &CCLientPlayGround::OnBnClickedSendanswer)
END_MESSAGE_MAP()

BOOL CCLientPlayGround::OnInitDialog() {
	// Init if required, can add later


	//set readonly when turn < 2
	/*if (num_turn < 2) {
		txtKeyWord.SetReadOnly(TRUE);
	} else {
		txtKeyWord.SetReadOnly(FALSE);
	}*/
}

// CCLientPlayGround message handlers


void CCLientPlayGround::OnBnClickedSendanswer()
{
	// TODO: Add your control notification handler code here
	CString responseMsg;
	CString guessW;
	CString keyW;

	txtGuessWord.GetWindowTextW(guessW);
	txtKeyWord, GetWindowTextW(keyW);

	//Send response to Server, with format: keyword,guessword
	responseMsg = keyW + "," + guessW;
	CT2A buff(responseMsg, CP_UTF8);
	//->send to server

	txtGuessWord.SetWindowTextW(_T(""));
	txtGuessWord.SetWindowTextW(_T(""));

	//Receive from Server

	std::string gW = CT2A(guessW);
	std::string kW = CT2A(keyW);
	std::string msg;
	//HiddenRemain(gW, keyword, msg, disWord);
	
	// Display on list chat result
	listPlayGround.AddString((LPCTSTR)strconverter.from_bytes(msg).c_str());
	listPlayGround.AddString(CString("The Remain words: ") + disWord.c_str());
	listPlayGround.SetCurSel(listPlayGround.GetCount() - 1);

	//// Show Score
	int score;
	CString sc;
	sc.Format(_T("%d"), score);
	txtScore.SetWindowTextW(sc);

	//Handle when user make wrong guess word/wrong keyword

}

void HiddenRemain(std::string guessWord, std::string& keyWord, std::string& msg, std::string& disWord) {
	std::vector<size_t> positions;
	size_t pos = keyWord.find(guessWord, 0);
	size_t spc = keyWord.find(" ", 0);

	while (pos != std::string::npos) {
		positions.push_back(pos);
		disWord.replace(pos, 1, guessWord);
		pos = keyWord.find(guessWord, pos + 1);
	}
	while (spc != std::string::npos) {
		disWord.replace(spc, 1, " ");
		spc = keyWord.find(" ", spc + 1);
	}

	msg = "Character '" + guessWord + "' has " + std::to_string(positions.size()) + " occurences.";

}
