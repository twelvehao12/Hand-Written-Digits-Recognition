
// digitRecDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "digitRec.h"
#include "digitRecDlg.h"
#include "afxdialogex.h"

#include <opencv2/opencv.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CdigitRecDlg 对话框



CdigitRecDlg::CdigitRecDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIGITREC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdigitRecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, FILEBROWSE, mFilePath);	// 从控件提取输入图片路径到mFilePath变量
}

BEGIN_MESSAGE_MAP(CdigitRecDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(SHOWIMAGEBUTTON, &CdigitRecDlg::OnBnClickedShowimagebutton)
	ON_BN_CLICKED(RUNBUTTON, &CdigitRecDlg::OnBnClickedRunbutton)
END_MESSAGE_MAP()


// CdigitRecDlg 消息处理程序

BOOL CdigitRecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CdigitRecDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CdigitRecDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CdigitRecDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CdigitRecDlg::OnBnClickedShowimagebutton()
{
	// opencv显示图像
	UpdateData(TRUE);
	if (mFilePath.IsEmpty())
	{
		MessageBox(_T("图像未准备好，请重新选择图像"));
		UpdateData(FALSE);
		return;
	}
	cv::Mat mImg = cv::imread(
		std::string(CT2A(mFilePath.GetString())),
		cv::IMREAD_ANYCOLOR
	);
	UpdateData(FALSE);

	cv::resize(mImg, mImg, cv::Size(512, 512), 0.0, 0.0, cv::INTER_CUBIC);

	cv::imshow("图片(点击任意按键退出)", mImg);
	cv::waitKey(0);
	cv::destroyAllWindows();
}


void CdigitRecDlg::OnBnClickedRunbutton()
{
	// opencv加载训练好的模型推导
	std::string modelpath = "./ResNet18-digits.onnx";

	auto net = cv::dnn::readNetFromONNX(modelpath);	// 读取模型

	UpdateData(TRUE);
	if (mFilePath.IsEmpty())
	{
		MessageBox(_T("图像未准备好，请重新选择图像"));
		UpdateData(FALSE);
		return;
	}
	cv::Mat image = cv::imread(
		std::string(CT2A(mFilePath.GetString())),
		cv::IMREAD_GRAYSCALE
	);
	UpdateData(FALSE);
	cv::resize(image, image, cv::Size(28, 28), 0.0, 0.0, cv::INTER_CUBIC);

	cv::Mat blob = cv::dnn::blobFromImage(image, 1, cv::Size(28, 28));
	blob /= 255;	// 归一化

	net.setInput(blob);
	cv::Mat out = net.forward();

	// argmax 获取识别结果
	double minValue, maxValue;
	cv::Point minIdx, maxIdx;
	cv::minMaxLoc(out, &minValue, &maxValue, &minIdx, &maxIdx);	
	// 

	std::string res = std::to_string(maxIdx.x);	// 结果转换成string

	MessageBox(CString(res.c_str()), _T("识别结果"));
}

