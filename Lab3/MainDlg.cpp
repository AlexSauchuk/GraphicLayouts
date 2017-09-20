
// MainDlg.cpp : ���� ����������
//

#include "stdafx.h"
#include "CMainApp.h"
#include "MainDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���������� ���� CAboutDlg ������������ ��� �������� �������� � ����������

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ������ ����������� ����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

// ����������
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_LBUTTONUP()
//	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LAB3_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DRAWING_AREA, m_PictureControl);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
//	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
//	ON_WM_QUERYDRAGICON()
//	ON_STN_CLICKED(IDC_DRAWING_AREA, &CMainDlg::OnStnClickedDrawingArea)
ON_COMMAND(ID_ABOUT, &CMainDlg::OnAbout)
ON_WM_DESTROY()
ON_COMMAND(ID_ADDSHAPE, &CMainDlg::OnAddShape)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// ����������� ��������� CMainDlg

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���������� ������ "� ���������..." � ��������� ����.

	// IDM_ABOUTBOX ������ ���� � �������� ��������� �������.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	ChangeMenuItemCaption(ID_ABOUT, IDS_ABOUTBOX);
	ChangeMenuItemCaption(ID_ADDSHAPE, IDS_ADDSHAPE);

	// ������ ������ ��� ����� ����������� ����.  ����� ������ ��� �������������,
	//  ���� ������� ���� ���������� �� �������� ����������
	SetIcon(m_hIcon, TRUE);			// ������� ������
	SetIcon(m_hIcon, FALSE);		// ������ ������

	// TODO: �������� �������������� �������������

	shapesManager = new ShapesManager(IDB_TURN);
	return TRUE;  // ������� �������� TRUE, ���� ����� �� ������� �������� ����������
}

void CMainDlg::OnPaint()
{

	if (IsIconic())
	{
		CPaintDC dc(this); // �������� ���������� ��� ���������
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ������������ ������ �� ������ ����������� ��������������
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ��������� ������
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
		Draw();
}

void CMainDlg::OnAddShape()
{
	vertexes.push_back({ 20, 40 });
	vertexes.push_back({ 60, 60 });
	vertexes.push_back({ 10, 80 });
	shapesManager->AddShape(vertexes);
	vertexes.clear();

	vertexes.push_back({ 80, 90 });
	vertexes.push_back({ 150, 90 });
	vertexes.push_back({ 150, 150 });
	vertexes.push_back({ 80, 150 });
	shapesManager->AddShape(vertexes);
	vertexes.clear();
	InvalidateRect(NULL, false);
}

void CMainDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (shapesManager->CheckShapeSelect(point)) {
		currentPoint = new CPoint(point);
		isNothingSelect = false;
		InvalidateRect(NULL, FALSE);
	}
	else if (!isNothingSelect)
	{
		isNothingSelect = true;
		InvalidateRect(NULL, FALSE);
	}
 
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMainDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (currentPoint != NULL || prevPoint != NULL) 
	{
		DeletePoint(&currentPoint);
		DeletePoint(&prevPoint);
		InvalidateRect(NULL, FALSE);
	}
	
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CMainDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (currentPoint != NULL)
	{
		if (prevPoint != NULL)
			delete prevPoint;
		prevPoint = currentPoint;
		currentPoint = new CPoint(point);
		shapesManager->ChangeSelectShapePosition({ currentPoint->x - prevPoint->x, currentPoint->y - prevPoint->y});
		InvalidateRect(FALSE, NULL);
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void CMainDlg::OnAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CMainDlg::OnDestroy()
{
	delete shapesManager;
	vertexes.clear();
	std::vector<POINT>().swap(vertexes);
	CDialogEx::OnDestroy();
}

void CMainDlg::Draw()
{
	CPaintDC dc(&m_PictureControl);
	CDC compatibleDC;
	CRect rect;
	CBitmap bitmap;

	GetClientRect(rect);
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	compatibleDC.CreateCompatibleDC(&dc);
	CBitmap* oldBitmap = compatibleDC.SelectObject(&bitmap);

	compatibleDC.FillRect(rect, WHITE_BRUSH);
	shapesManager->Draw(&compatibleDC);
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &compatibleDC, 0, 0, SRCCOPY);

	shapesManager->DrawTurnIcon(&dc);
	compatibleDC.SelectObject(oldBitmap);
	
	bitmap.DeleteObject();
	compatibleDC.DeleteDC();
	CDialogEx::OnPaint();
}

void CMainDlg::ChangeMenuItemCaption(int menuItemId, int stringId)
{
	CMenu* mainMenu = GetMenu();
	CString str;
	str.LoadStringW(stringId);
	mainMenu->ModifyMenuW(menuItemId, MF_BYCOMMAND, menuItemId, str);
}

void CMainDlg::DeletePoint(CPoint** point)
{
	delete *point;
	*point = NULL;
}
