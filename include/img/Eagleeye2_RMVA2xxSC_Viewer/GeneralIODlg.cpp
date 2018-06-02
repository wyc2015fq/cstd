// GeneralIODlg.cpp : implementation file
//

#include "stdafx.h"
#include "Eagleeye2_Viewer.h"
#include "GeneralIODlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneralIODlg dialog


CGeneralIODlg::CGeneralIODlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGeneralIODlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGeneralIODlg)
	m_bIsPort1IN = TRUE;
	m_bIsPort2IN = TRUE;
	m_bIsPort3IN = TRUE;
	m_bIsPort4IN = TRUE;
	m_bIsPort5IN = TRUE;
	m_bIsPort6IN = TRUE;
	m_bIsPort7IN = TRUE;
	m_bIsPort8IN = TRUE;
	m_bIsPort9IN = TRUE;
	m_bIsPort10IN = TRUE;
	m_bIsFlashSynEnable = FALSE; //disable flash syn
	m_bPort10OUTValue = FALSE;
	m_bPort1OUTValue = FALSE;
	m_bPort2OUTValue = FALSE;
	m_bPort3OUTValue = FALSE;
	m_bPort4OUTValue = FALSE;
	m_bPort5OUTValue = FALSE;
	m_bPort6OUTValue = FALSE;
	m_bPort7OUTValue = FALSE;
	m_bPort8OUTValue = FALSE;
	m_bPort9OUTValue = FALSE;
	m_bIsPortDirAdj = FALSE;
	m_bIsPortOutAdj = FALSE;
	m_bIsFlashSynAdj = FALSE;
	//}}AFX_DATA_INIT
	//default 0 -> input
	m_pioIndicator.pio1 = 0;
	m_pioIndicator.pio2 = 0;
	m_pioIndicator.pio3 = 0;
	m_pioIndicator.pio4 = 0;
	m_pioIndicator.pio5 = 0;
	m_pioIndicator.pio6 = 0;
	m_pioIndicator.pio7 = 0;
	m_pioIndicator.pio8 = 0;
	m_pioIndicator.pio9 = 0;
	m_pioIndicator.pio10 = 0;
}


void CGeneralIODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralIODlg)
	DDX_Control(pDX, IDC_FLASHSYNENABLE, m_ctrlFlashSynEnable);
	DDX_Control(pDX, IDC_FLASHSYNDISABLE, m_ctrlFlashSynDisable);
	DDX_Control(pDX, IDC_PORT9OUT, m_ctrlPort9OUT);
	DDX_Control(pDX, IDC_PORT9IN, m_ctrlPort9IN);
	DDX_Control(pDX, IDC_PORT8OUT, m_ctrlPort8OUT);
	DDX_Control(pDX, IDC_PORT8IN, m_ctrlPort8IN);
	DDX_Control(pDX, IDC_PORT7OUT, m_ctrlPort7OUT);
	DDX_Control(pDX, IDC_PORT7IN, m_ctrlPort7IN);
	DDX_Control(pDX, IDC_PORT6OUT, m_ctrlPort6OUT);
	DDX_Control(pDX, IDC_PORT6IN, m_ctrlPort6IN);
	DDX_Control(pDX, IDC_PORT5OUT, m_ctrlPort5OUT);
	DDX_Control(pDX, IDC_PORT5IN, m_ctrlPort5IN);
	DDX_Control(pDX, IDC_PORT4OUT, m_ctrlPort4OUT);
	DDX_Control(pDX, IDC_PORT4IN, m_ctrlPort4IN);
	DDX_Control(pDX, IDC_PORT3OUT, m_ctrlPort3OUT);
	DDX_Control(pDX, IDC_PORT3IN, m_ctrlPort3IN);
	DDX_Control(pDX, IDC_PORT2OUT, m_ctrlPort2OUT);
	DDX_Control(pDX, IDC_PORT2IN, m_ctrlPort2IN);
	DDX_Control(pDX, IDC_PORT1OUT, m_ctrlPort1OUT);
	DDX_Control(pDX, IDC_PORT1IN, m_ctrlPort1IN);
	DDX_Control(pDX, IDC_PORT10OUT, m_ctrlPort10OUT);
	DDX_Control(pDX, IDC_PORT10IN, m_ctrlPort10IN);
	DDX_Text(pDX, IDC_PORT10OUTVALUE, m_bPort10OUTValue);
	DDX_Text(pDX, IDC_PORT1OUTVALUE, m_bPort1OUTValue);
	DDX_Text(pDX, IDC_PORT2OUTVALUE, m_bPort2OUTValue);
	DDX_Text(pDX, IDC_PORT3OUTVALUE, m_bPort3OUTValue);
	DDX_Text(pDX, IDC_PORT4OUTVALUE, m_bPort4OUTValue);
	DDX_Text(pDX, IDC_PORT5OUTVALUE, m_bPort5OUTValue);
	DDX_Text(pDX, IDC_PORT6OUTVALUE, m_bPort6OUTValue);
	DDX_Text(pDX, IDC_PORT7OUTVALUE, m_bPort7OUTValue);
	DDX_Text(pDX, IDC_PORT8OUTVALUE, m_bPort8OUTValue);
	DDX_Text(pDX, IDC_PORT9OUTVALUE, m_bPort9OUTValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGeneralIODlg, CDialog)
	//{{AFX_MSG_MAP(CGeneralIODlg)
	ON_BN_CLICKED(IDC_PORT1IN, OnPort1in)
	ON_BN_CLICKED(IDC_PORT1OUT, OnPort1out)
	ON_BN_CLICKED(IDC_FLASHSYNDISABLE, OnFlashsyndisable)
	ON_BN_CLICKED(IDC_FLASHSYNENABLE, OnFlashsynenable)
	ON_BN_CLICKED(IDC_PORT2IN, OnPort2in)
	ON_BN_CLICKED(IDC_PORT2OUT, OnPort2out)
	ON_BN_CLICKED(IDC_PORT3IN, OnPort3in)
	ON_BN_CLICKED(IDC_PORT3OUT, OnPort3out)
	ON_BN_CLICKED(IDC_PORT4IN, OnPort4in)
	ON_BN_CLICKED(IDC_PORT4OUT, OnPort4out)
	ON_BN_CLICKED(IDC_PORT5IN, OnPort5in)
	ON_BN_CLICKED(IDC_PORT5OUT, OnPort5out)
	ON_BN_CLICKED(IDC_PORT6IN, OnPort6in)
	ON_BN_CLICKED(IDC_PORT6OUT, OnPort6out)
	ON_BN_CLICKED(IDC_PORT7IN, OnPort7in)
	ON_BN_CLICKED(IDC_PORT7OUT, OnPort7out)
	ON_BN_CLICKED(IDC_PORT8IN, OnPort8in)
	ON_BN_CLICKED(IDC_PORT8OUT, OnPort8out)
	ON_BN_CLICKED(IDC_PORT9IN, OnPort9in)
	ON_BN_CLICKED(IDC_PORT9OUT, OnPort9out)
	ON_BN_CLICKED(IDC_PORT10IN, OnPort10in)
	ON_BN_CLICKED(IDC_PORT10OUT, OnPort10out)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneralIODlg message handlers

BOOL CGeneralIODlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	//Flash lamp syn out control
	if(m_bIsFlashSynEnable){
		m_ctrlFlashSynEnable.SetCheck(TRUE);
		m_ctrlFlashSynDisable.SetCheck(FALSE);
	}
	else{
		m_ctrlFlashSynEnable.SetCheck(FALSE);
		m_ctrlFlashSynDisable.SetCheck(TRUE);
	}

	//PIO control display
	if( m_bIsPort1IN){
		m_ctrlPort1IN.SetCheck(TRUE);
		m_ctrlPort1OUT.SetCheck(FALSE);
		GetDlgItem(IDC_PORT1OUTVALUE)->EnableWindow(FALSE);
	}
	else{
		m_ctrlPort1IN.SetCheck(FALSE);
		m_ctrlPort1OUT.SetCheck(TRUE);
		GetDlgItem(IDC_PORT1OUTVALUE)->EnableWindow(TRUE);
	}

	if( m_bIsPort2IN){
		m_ctrlPort2IN.SetCheck(TRUE);
		m_ctrlPort2OUT.SetCheck(FALSE);
		GetDlgItem(IDC_PORT2OUTVALUE)->EnableWindow(FALSE);
	}
	else{
		m_ctrlPort2IN.SetCheck(FALSE);
		m_ctrlPort2OUT.SetCheck(TRUE);
		GetDlgItem(IDC_PORT2OUTVALUE)->EnableWindow(TRUE);
	}

	if( m_bIsPort3IN){
		m_ctrlPort3IN.SetCheck(TRUE);
		m_ctrlPort3OUT.SetCheck(FALSE);
		GetDlgItem(IDC_PORT3OUTVALUE)->EnableWindow(FALSE);
	}
	else{
		m_ctrlPort3IN.SetCheck(FALSE);
		m_ctrlPort3OUT.SetCheck(TRUE);
		GetDlgItem(IDC_PORT3OUTVALUE)->EnableWindow(TRUE);
	}

	if( m_bIsPort4IN){
		m_ctrlPort4IN.SetCheck(TRUE);
		m_ctrlPort4OUT.SetCheck(FALSE);
		GetDlgItem(IDC_PORT4OUTVALUE)->EnableWindow(FALSE);
	}
	else{
		m_ctrlPort4IN.SetCheck(FALSE);
		m_ctrlPort4OUT.SetCheck(TRUE);
		GetDlgItem(IDC_PORT4OUTVALUE)->EnableWindow(TRUE);
	}

	if( m_bIsPort5IN){
		m_ctrlPort5IN.SetCheck(TRUE);
		m_ctrlPort5OUT.SetCheck(FALSE);
		GetDlgItem(IDC_PORT5OUTVALUE)->EnableWindow(FALSE);
	}
	else{
		m_ctrlPort5IN.SetCheck(FALSE);
		m_ctrlPort5OUT.SetCheck(TRUE);
		GetDlgItem(IDC_PORT5OUTVALUE)->EnableWindow(TRUE);
	}

	if( m_bIsPort6IN){
		m_ctrlPort6IN.SetCheck(TRUE);
		m_ctrlPort6OUT.SetCheck(FALSE);
		GetDlgItem(IDC_PORT6OUTVALUE)->EnableWindow(FALSE);
	}
	else{
		m_ctrlPort6IN.SetCheck(FALSE);
		m_ctrlPort6OUT.SetCheck(TRUE);
		GetDlgItem(IDC_PORT6OUTVALUE)->EnableWindow(TRUE);
	}

	if( m_bIsPort7IN){
		m_ctrlPort7IN.SetCheck(TRUE);
		m_ctrlPort7OUT.SetCheck(FALSE);
		GetDlgItem(IDC_PORT7OUTVALUE)->EnableWindow(FALSE);
	}
	else{
		m_ctrlPort7IN.SetCheck(FALSE);
		m_ctrlPort7OUT.SetCheck(TRUE);
		GetDlgItem(IDC_PORT7OUTVALUE)->EnableWindow(TRUE);
	}

	if( m_bIsPort8IN){
		m_ctrlPort8IN.SetCheck(TRUE);
		m_ctrlPort8OUT.SetCheck(FALSE);
		GetDlgItem(IDC_PORT8OUTVALUE)->EnableWindow(FALSE);
	}
	else{
		m_ctrlPort8IN.SetCheck(FALSE);
		m_ctrlPort8OUT.SetCheck(TRUE);
		GetDlgItem(IDC_PORT8OUTVALUE)->EnableWindow(TRUE);
	}

	if( m_bIsPort9IN){
		m_ctrlPort9IN.SetCheck(TRUE);
		m_ctrlPort9OUT.SetCheck(FALSE);
		GetDlgItem(IDC_PORT9OUTVALUE)->EnableWindow(FALSE);
	}
	else{
		m_ctrlPort9IN.SetCheck(FALSE);
		m_ctrlPort9OUT.SetCheck(TRUE);
		GetDlgItem(IDC_PORT9OUTVALUE)->EnableWindow(TRUE);
	}

	if( m_bIsPort10IN){
		m_ctrlPort10IN.SetCheck(TRUE);
		m_ctrlPort10OUT.SetCheck(FALSE);
		GetDlgItem(IDC_PORT10OUTVALUE)->EnableWindow(FALSE);
	}
	else{
		m_ctrlPort10IN.SetCheck(FALSE);
		m_ctrlPort10OUT.SetCheck(TRUE);
		GetDlgItem(IDC_PORT10OUTVALUE)->EnableWindow(TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGeneralIODlg::OnFlashsyndisable() 
{
	// TODO: Add your control notification handler code here
	m_bIsFlashSynAdj = TRUE;
	m_bIsFlashSynEnable = FALSE;
	m_ctrlFlashSynEnable.SetCheck(FALSE);
	m_ctrlFlashSynDisable.SetCheck(TRUE);

}

void CGeneralIODlg::OnFlashsynenable() 
{
	// TODO: Add your control notification handler code here
	m_bIsFlashSynAdj = TRUE;
	m_bIsFlashSynEnable = TRUE;
	m_ctrlFlashSynEnable.SetCheck(TRUE);
	m_ctrlFlashSynDisable.SetCheck(FALSE);	
}

void CGeneralIODlg::OnPort1in() 
{
	// TODO: Add your control notification handler code here
	m_pioIndicator.pio1 = 0;
	m_bIsPort1IN = TRUE;
	m_ctrlPort1IN.SetCheck(TRUE);
	m_ctrlPort1OUT.SetCheck(FALSE);
	GetDlgItem(IDC_PORT1OUTVALUE)->EnableWindow(FALSE);
}

void CGeneralIODlg::OnPort1out() 
{
	// TODO: Add your control notification handler code here
	m_pioIndicator.pio1 = 1;
	m_bIsPort1IN = FALSE;
	m_ctrlPort1IN.SetCheck(FALSE);
	m_ctrlPort1OUT.SetCheck(TRUE);
	GetDlgItem(IDC_PORT1OUTVALUE)->EnableWindow(TRUE);
}

void CGeneralIODlg::OnPort2in() 
{
	// TODO: Add your control notification handler code here
	m_pioIndicator.pio2 = 0;
	m_bIsPort2IN = TRUE;
	m_ctrlPort2IN.SetCheck(TRUE);
	m_ctrlPort2OUT.SetCheck(FALSE);
	GetDlgItem(IDC_PORT2OUTVALUE)->EnableWindow(FALSE);
}

void CGeneralIODlg::OnPort2out() 
{
	// TODO: Add your control notification handler code here
	m_pioIndicator.pio2 = 1;
	m_bIsPort2IN = FALSE;
	m_ctrlPort2IN.SetCheck(FALSE);
	m_ctrlPort2OUT.SetCheck(TRUE);
	GetDlgItem(IDC_PORT2OUTVALUE)->EnableWindow(TRUE);
}

void CGeneralIODlg::OnPort3in() 
{
	// TODO: Add your control notification handler code here
	m_pioIndicator.pio3 = 0;
	m_bIsPort3IN = TRUE;
	m_ctrlPort3IN.SetCheck(TRUE);
	m_ctrlPort3OUT.SetCheck(FALSE);
	GetDlgItem(IDC_PORT3OUTVALUE)->EnableWindow(FALSE);
}

void CGeneralIODlg::OnPort3out() 
{
	// TODO: Add your control notification handler code here
	m_pioIndicator.pio3 = 1;
	m_bIsPort3IN = FALSE;
	m_ctrlPort3IN.SetCheck(FALSE);
	m_ctrlPort3OUT.SetCheck(TRUE);
	GetDlgItem(IDC_PORT3OUTVALUE)->EnableWindow(TRUE);
}

void CGeneralIODlg::OnPort4in() 
{
	// TODO: Add your control notification handler code here
	m_pioIndicator.pio4 = 0;
	m_bIsPort4IN = TRUE;
	m_ctrlPort4IN.SetCheck(TRUE);
	m_ctrlPort4OUT.SetCheck(FALSE);
	GetDlgItem(IDC_PORT4OUTVALUE)->EnableWindow(FALSE);
}

void CGeneralIODlg::OnPort4out() 
{
	// TODO: Add your control notification handler code here
	m_pioIndicator.pio4 = 1;
	m_bIsPort4IN = FALSE;
	m_ctrlPort4IN.SetCheck(FALSE);
	m_ctrlPort4OUT.SetCheck(TRUE);
	GetDlgItem(IDC_PORT4OUTVALUE)->EnableWindow(TRUE);
}

void CGeneralIODlg::OnPort5in() 
{
	// TODO: Add your control notification handler code here
	m_pioIndicator.pio5 = 0;
	m_bIsPort5IN = TRUE;
	m_ctrlPort5IN.SetCheck(TRUE);
	m_ctrlPort5OUT.SetCheck(FALSE);
	GetDlgItem(IDC_PORT5OUTVALUE)->EnableWindow(FALSE);
}

void CGeneralIODlg::OnPort5out() 
{
	// TODO: Add your control notification handler code here
	m_pioIndicator.pio5 = 1;
	m_bIsPort5IN = FALSE;
	m_ctrlPort5IN.SetCheck(FALSE);
	m_ctrlPort5OUT.SetCheck(TRUE);
	GetDlgItem(IDC_PORT5OUTVALUE)->EnableWindow(TRUE);
}

void CGeneralIODlg::OnPort6in() 
{
	// TODO: Add your control notification handler code here
	m_pioIndicator.pio6 = 0;
	m_bIsPort6IN = TRUE;
	m_ctrlPort6IN.SetCheck(TRUE);
	m_ctrlPort6OUT.SetCheck(FALSE);
	GetDlgItem(IDC_PORT6OUTVALUE)->EnableWindow(FALSE);
}

void CGeneralIODlg::OnPort6out() 
{
	// TODO: Add your control notification handler code here
	m_pioIndicator.pio6 = 1;
	m_bIsPort6IN = FALSE;
	m_ctrlPort6IN.SetCheck(FALSE);
	m_ctrlPort6OUT.SetCheck(TRUE);
	GetDlgItem(IDC_PORT6OUTVALUE)->EnableWindow(TRUE);
}

void CGeneralIODlg::OnPort7in() 
{
	// TODO: Add your control notification handler code here
	m_pioIndicator.pio7 = 0;
	m_bIsPort7IN = TRUE;
	m_ctrlPort7IN.SetCheck(TRUE);
	m_ctrlPort7OUT.SetCheck(FALSE);
	GetDlgItem(IDC_PORT7OUTVALUE)->EnableWindow(FALSE);
}

void CGeneralIODlg::OnPort7out() 
{
	// TODO: Add your control notification handler code here
	m_pioIndicator.pio7 = 1;
	m_bIsPort7IN = FALSE;
	m_ctrlPort7IN.SetCheck(FALSE);
	m_ctrlPort7OUT.SetCheck(TRUE);
	GetDlgItem(IDC_PORT7OUTVALUE)->EnableWindow(TRUE);
}

void CGeneralIODlg::OnPort8in() 
{
	// TODO: Add your control notification handler code here
	m_pioIndicator.pio8 = 0;
	m_bIsPort8IN = TRUE;
	m_ctrlPort8IN.SetCheck(TRUE);
	m_ctrlPort8OUT.SetCheck(FALSE);
	GetDlgItem(IDC_PORT8OUTVALUE)->EnableWindow(FALSE);
}

void CGeneralIODlg::OnPort8out() 
{
	// TODO: Add your control notification handler code here
	m_pioIndicator.pio8 = 1;
	m_bIsPort8IN = FALSE;
	m_ctrlPort8IN.SetCheck(FALSE);
	m_ctrlPort8OUT.SetCheck(TRUE);
	GetDlgItem(IDC_PORT8OUTVALUE)->EnableWindow(TRUE);
}

void CGeneralIODlg::OnPort9in() 
{
	// TODO: Add your control notification handler code here
	m_pioIndicator.pio9 = 0;
	m_bIsPort9IN = TRUE;
	m_ctrlPort9IN.SetCheck(TRUE);
	m_ctrlPort9OUT.SetCheck(FALSE);
	GetDlgItem(IDC_PORT9OUTVALUE)->EnableWindow(FALSE);
}

void CGeneralIODlg::OnPort9out() 
{
	// TODO: Add your control notification handler code here
	m_pioIndicator.pio9 = 1;
	m_bIsPort9IN = FALSE;
	m_ctrlPort9IN.SetCheck(FALSE);
	m_ctrlPort9OUT.SetCheck(TRUE);
	GetDlgItem(IDC_PORT9OUTVALUE)->EnableWindow(TRUE);
}

void CGeneralIODlg::OnPort10in() 
{
	// TODO: Add your control notification handler code here
	m_pioIndicator.pio10 = 0;
	m_bIsPort10IN = TRUE;
	m_ctrlPort10IN.SetCheck(TRUE);
	m_ctrlPort10OUT.SetCheck(FALSE);
	GetDlgItem(IDC_PORT10OUTVALUE)->EnableWindow(FALSE);
}

void CGeneralIODlg::OnPort10out() 
{
	// TODO: Add your control notification handler code here
	m_pioIndicator.pio10 = 1;
	m_bIsPort10IN = FALSE;
	m_ctrlPort10IN.SetCheck(FALSE);
	m_ctrlPort10OUT.SetCheck(TRUE);
	GetDlgItem(IDC_PORT10OUTVALUE)->EnableWindow(TRUE);
}

void CGeneralIODlg::OnOK() 
{
	// TODO: Add extra validation here
	UINT tmppioIndicator;

	tmppioIndicator = *(UINT*)(&m_pioIndicator);

	UpdateData(TRUE);
	m_pioIndicator.pio1outvalue = m_bPort1OUTValue;
	m_pioIndicator.pio2outvalue = m_bPort2OUTValue;
	m_pioIndicator.pio3outvalue = m_bPort3OUTValue;
	m_pioIndicator.pio4outvalue = m_bPort4OUTValue;
	m_pioIndicator.pio5outvalue = m_bPort5OUTValue;
	m_pioIndicator.pio6outvalue = m_bPort6OUTValue;
	m_pioIndicator.pio7outvalue = m_bPort7OUTValue;
	m_pioIndicator.pio8outvalue = m_bPort8OUTValue;
	m_pioIndicator.pio9outvalue = m_bPort9OUTValue;
	m_pioIndicator.pio10outvalue = m_bPort10OUTValue;

	CDialog::OnOK();
}
