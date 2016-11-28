
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "Editor.h"

#include <codecvt>
#include <SpriteEntity.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
	InvalidateProperties();
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE( AFX_WM_PROPERTY_CHANGED, OnPropertyChanged )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd () == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	InvalidateProperties();
	m_wndPropList.RemoveAll();
	
	if ( theApp.m_pSelectedEntity != nullptr )
	{
		InitBaseEntitiyPropList();
	}
	else
	{
		m_wndPropList.Invalidate();
	}

}

void CPropertiesWnd::ApplyChanges( CMFCPropertyGridProperty* pProp )
{
	if ( pProp && theApp.m_pSelectedEntity )
	{
		ApplyBaseEntityPropList( pProp );
	}
}

void CPropertiesWnd::InitBaseEntitiyPropList()
{
	BaseEntity* pEnt = nullptr;
	if ( theApp.m_pSelectedEntity && theApp.m_pSelectedEntity->pParentEntity )
	{
		pEnt = theApp.m_pSelectedEntity->pParentEntity;
	}

	if ( !pEnt )
		return;

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty( _T( "Base" ) );
	m_wndPropList.AddProperty( pGroup1 );

	const std::string& strName = pEnt->GetName();
	m_pNameProp = new CMFCPropertyGridProperty( _T( "Name" ), ( _variant_t ) strName.c_str(), _T( "Entity Name" ) );
	m_wndPropList.AddProperty( m_pNameProp );

	m_pIDProp = new CMFCPropertyGridProperty( _T( "ID" ), ( _variant_t ) pEnt->GetID(), _T( "Entity ID" ) );
	m_pIDProp->AllowEdit( FALSE );
	m_wndPropList.AddProperty( m_pIDProp );


	pGroup1 = new CMFCPropertyGridProperty( _T( "Transform" ) );
	m_wndPropList.AddProperty( pGroup1 );

	CMFCPropertyGridProperty* pPosition = new CMFCPropertyGridProperty( _T( "Position" ), 0, TRUE );
	m_pPositionPropX = new CMFCPropertyGridProperty( _T( "X" ), ( _variant_t ) pEnt->GetTransform().m_Position.x, _T( "X Position" ) );
	pPosition->AddSubItem( m_pPositionPropX );

	m_pPositionPropY = new CMFCPropertyGridProperty( _T( "Y" ), ( _variant_t ) pEnt->GetTransform().m_Position.y, _T( "Y Position" ) );
	pPosition->AddSubItem( m_pPositionPropY );
	pPosition->Expand();
	m_wndPropList.AddProperty( pPosition );

	CMFCPropertyGridProperty* pScale = new CMFCPropertyGridProperty( _T( "Scale" ), 0, TRUE );
	m_pScalePropX = new CMFCPropertyGridProperty( _T( "X" ), ( _variant_t ) pEnt->GetTransform().m_Scale.x, _T( "X Scale" ) );
	pScale->AddSubItem( m_pScalePropX );

	m_pScalePropY = new CMFCPropertyGridProperty( _T( "Y" ), ( _variant_t ) pEnt->GetTransform().m_Scale.y, _T( "Y Scale" ) );
	pScale->AddSubItem( m_pScalePropY );
	pScale->Expand();
	m_wndPropList.AddProperty( pScale );

	CMFCPropertyGridProperty* pRotation = new CMFCPropertyGridProperty( _T( "Rotation" ), 0, TRUE );
	m_pRotationProp = new CMFCPropertyGridProperty( _T( "Rotation" ), ( _variant_t ) pEnt->GetTransform().m_Rotation, _T( "Rotation" ) );
	pRotation->AddSubItem( m_pRotationProp );
	pRotation->Expand();
	m_wndPropList.AddProperty( pRotation );


	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty( _T( "Basic" ) );
	m_wndPropList.AddProperty( pGroup2 );
}

void CPropertiesWnd::ApplyBaseEntityPropList( CMFCPropertyGridProperty* pProp )
{
	BaseEntity* pEnt = theApp.m_pSelectedEntity->pParentEntity;
	if ( !pEnt )
		return;

	if ( pProp == m_pPositionPropX )
	{
		float fXValue = m_pPositionPropX->GetValue().fltVal;
		pEnt->GetTransform().m_Position.x = fXValue;
		theApp.m_pSelectedEntity->pClickableBox->SetPosition( pEnt->GetTransform().m_Position );
	}
	else if ( pProp == m_pPositionPropY )
	{
		float fYValue = m_pPositionPropY->GetValue().fltVal;
		pEnt->GetTransform().m_Position.y = fYValue;
		theApp.m_pSelectedEntity->pClickableBox->SetPosition( pEnt->GetTransform().m_Position );
	}
	else if ( pProp == m_pNameProp )
	{
		std::wstring strwName( m_pNameProp->GetValue().bstrVal );
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::string strName = converter.to_bytes( strwName );
		pEnt->SetName( strName );

		theApp.m_bUpdateClassView = true;
	}
	else if ( pProp == m_pRotationProp )
	{
		float fRotationValue = m_pRotationProp->GetValue().fltVal;
		pEnt->GetTransform().m_Rotation = fRotationValue;
		// TODO : OOBB me please.
	}
	else if ( pProp == m_pScalePropX )
	{
		float fValue = m_pScalePropX->GetValue().fltVal;
		pEnt->GetTransform().m_Scale.x = fValue;

		glm::vec2 vSize;
		SpriteEntity* pSpriteEnt = dynamic_cast< SpriteEntity* >( pEnt );
		if ( pSpriteEnt )
		{
			BaseTexture* pTexture = pSpriteEnt->GetTexture();
			if ( pTexture )
			{
				vSize.x = pTexture->GetWidth() * pEnt->GetTransform().m_Scale.x;
				vSize.y = pTexture->GetHeight() * pEnt->GetTransform().m_Scale.y;

				theApp.m_pSelectedEntity->pClickableBox->SetSize( vSize );
			}
		}
	}
	else if ( pProp == m_pScalePropY )
	{
		float fValue = m_pScalePropY->GetValue().fltVal;
		pEnt->GetTransform().m_Scale.y = fValue;

		glm::vec2 vSize;
		SpriteEntity* pSpriteEnt = dynamic_cast< SpriteEntity* >( pEnt );
		if ( pSpriteEnt )
		{
			BaseTexture* pTexture = pSpriteEnt->GetTexture();
			if ( pTexture )
			{
				vSize.x = pTexture->GetWidth() * pEnt->GetTransform().m_Scale.x;
				vSize.y = pTexture->GetHeight() * pEnt->GetTransform().m_Scale.y;

				theApp.m_pSelectedEntity->pClickableBox->SetSize( vSize );
			}
		}
	}
}

void CPropertiesWnd::InvalidateProperties()
{
	m_pPositionPropX = nullptr;
	m_pPositionPropY = nullptr;
	m_pIDProp = nullptr;
	m_pNameProp = nullptr;
	m_pScalePropX = nullptr;
	m_pScalePropY = nullptr;
	m_pRotationProp = nullptr;
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

LRESULT CPropertiesWnd::OnPropertyChanged( WPARAM wparam, LPARAM lparam )
{
	CMFCPropertyGridProperty* pProp = ( CMFCPropertyGridProperty* ) lparam;

	ApplyChanges( pProp );

	return 0;
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}




//CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty( _T( "Transform" ) );
//
//pGroup1->AddSubItem( new CMFCPropertyGridProperty( _T( "Position" ), ( _variant_t ) false, _T( "Specifies the window's font will be non-bold and controls will have a 3D border" ) ) );
//
//CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty( _T( "Border" ), _T( "Dialog Frame" ), _T( "One of: None, Thin, Resizable, or Dialog Frame" ) );
//pProp->AddOption( _T( "None" ) );
//pProp->AddOption( _T( "Thin" ) );
//pProp->AddOption( _T( "Resizable" ) );
//pProp->AddOption( _T( "Dialog Frame" ) );
//pProp->AllowEdit( FALSE );
//
//pGroup1->AddSubItem( pProp );
//pGroup1->AddSubItem( new CMFCPropertyGridProperty( _T( "Caption" ), ( _variant_t ) _T( "About" ), _T( "Specifies the text that will be displayed in the window's title bar" ) ) );
//
//m_wndPropList.AddProperty( pGroup1 );
//
//CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty( _T( "Window Size" ), 0, TRUE );
//
//pProp = new CMFCPropertyGridProperty( _T( "Height" ), ( _variant_t ) 250l, _T( "Specifies the window's height" ) );
//pProp->EnableSpinControl( TRUE, 50, 300 );
//pSize->AddSubItem( pProp );
//
//pProp = new CMFCPropertyGridProperty( _T( "Width" ), ( _variant_t ) 150l, _T( "Specifies the window's width" ) );
//pProp->EnableSpinControl( TRUE, 50, 200 );
//pSize->AddSubItem( pProp );
//
//m_wndPropList.AddProperty( pSize );
//
//CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty( _T( "Font" ) );
//
//LOGFONT lf;
//CFont* font = CFont::FromHandle( ( HFONT ) GetStockObject( DEFAULT_GUI_FONT ) );
//font->GetLogFont( &lf );
//
//_tcscpy_s( lf.lfFaceName, _T( "Arial" ) );
//
//pGroup2->AddSubItem( new CMFCPropertyGridFontProperty( _T( "Font" ), lf, CF_EFFECTS | CF_SCREENFONTS, _T( "Specifies the default font for the window" ) ) );
//pGroup2->AddSubItem( new CMFCPropertyGridProperty( _T( "Use System Font" ), ( _variant_t ) true, _T( "Specifies that the window uses MS Shell Dlg font" ) ) );
//
//m_wndPropList.AddProperty( pGroup2 );
//
//CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty( _T( "Misc" ) );
//pProp = new CMFCPropertyGridProperty( _T( "(Name)" ), _T( "Application" ) );
//pProp->Enable( FALSE );
//pGroup3->AddSubItem( pProp );
//
//CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty( _T( "Window Color" ), RGB( 210, 192, 254 ), NULL, _T( "Specifies the default window color" ) );
//pColorProp->EnableOtherButton( _T( "Other..." ) );
//pColorProp->EnableAutomaticButton( _T( "Default" ), ::GetSysColor( COLOR_3DFACE ) );
//pGroup3->AddSubItem( pColorProp );
//
//static const TCHAR szFilter[] = _T( "Icon Files(*.ico)|*.ico|All Files(*.*)|*.*||" );
//pGroup3->AddSubItem( new CMFCPropertyGridFileProperty( _T( "Icon" ), TRUE, _T( "" ), _T( "ico" ), 0, szFilter, _T( "Specifies the window icon" ) ) );
//
//pGroup3->AddSubItem( new CMFCPropertyGridFileProperty( _T( "Folder" ), _T( "c:\\" ) ) );
//
//m_wndPropList.AddProperty( pGroup3 );
//
//CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty( _T( "Hierarchy" ) );
//
//CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty( _T( "First sub-level" ) );
//pGroup4->AddSubItem( pGroup41 );
//
//CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty( _T( "Second sub-level" ) );
//pGroup41->AddSubItem( pGroup411 );
//
//pGroup411->AddSubItem( new CMFCPropertyGridProperty( _T( "Item 1" ), ( _variant_t ) _T( "Value 1" ), _T( "This is a description" ) ) );
//pGroup411->AddSubItem( new CMFCPropertyGridProperty( _T( "Item 2" ), ( _variant_t ) _T( "Value 2" ), _T( "This is a description" ) ) );
//pGroup411->AddSubItem( new CMFCPropertyGridProperty( _T( "Item 3" ), ( _variant_t ) _T( "Value 3" ), _T( "This is a description" ) ) );
//
//pGroup4->Expand( FALSE );
//m_wndPropList.AddProperty( pGroup4 );