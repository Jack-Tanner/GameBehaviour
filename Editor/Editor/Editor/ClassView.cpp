
#include "stdafx.h"
#include "MainFrm.h"
#include "ClassView.h"
#include "Resource.h"
#include "Editor.h"

#include <locale>
#include <codecvt>
#include <string>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClassView::CClassView()
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

CClassView::~CClassView()
{
}

BEGIN_MESSAGE_MAP( CClassView, CDockablePane )
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND( ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction )
	ON_COMMAND( ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable )
	ON_COMMAND( ID_CLASS_DEFINITION, OnClassDefinition )
	ON_COMMAND( ID_CLASS_PROPERTIES, OnClassProperties )
	ON_COMMAND( ID_NEW_FOLDER, OnNewFolder )
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE( ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort )
	ON_UPDATE_COMMAND_UI_RANGE( ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView message handlers

int CClassView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndClassView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}


	OnChangeVisualStyle();

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	// Fill in some static tree view data (dummy code, nothing magic here)
	//FillClassView();

	return 0;
}

void CClassView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CClassView::FillClassView()
{
	m_wndClassView.DeleteAllItems();


	HTREEITEM hRoot = m_wndClassView.InsertItem(_T("Entities"), 0, 0);
	m_wndClassView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	
	HTREEITEM hClass;
	const std::vector<BaseEntity*>& pEntities = theApp.m_World.GetEntityList();
	for ( BaseEntity* pEnt : pEntities )
	{
		if ( pEnt )
		{
			std::stringstream ss;
			ss << pEnt->GetName() << " [" << pEnt->GetID() << "]";
			

			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::wstring wideName = converter.from_bytes( ss.str() );

			hClass = m_wndClassView.InsertItem( wideName.c_str(), 1, 1, hRoot );
		}
	}

	m_wndClassView.Expand( hRoot, TVE_EXPAND );

}

void CClassView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void CClassView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_wndClassView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + 1, rectClient.Width() - 2, rectClient.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CClassView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CClassView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;
}

void CClassView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CClassView::OnDblClick( NMHDR * pNMHDR, LRESULT * pResult )
{
}

void CClassView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("Add member function..."));
}

void CClassView::OnClassAddMemberVariable()
{
	// TODO: Add your command handler code here
}

void CClassView::OnClassDefinition()
{
	// TODO: Add your command handler code here
}

void CClassView::OnClassProperties()
{
	// TODO: Add your command handler code here
}

void CClassView::OnNewFolder()
{
	AfxMessageBox(_T("New Folder..."));
}

void CClassView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndClassView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CClassView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndClassView.SetFocus();
}

void CClassView::OnChangeVisualStyle()
{
	m_ClassViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndClassView.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

}

BEGIN_MESSAGE_MAP( CEntityTree, CViewTree )
	//ON_NOTIFY_REFLECT( TVN_ITEMCHANGED, OnSelChangedTreeCtrl )
	ON_NOTIFY_REFLECT( NM_DBLCLK, OnSelChangedTreeCtrl )
END_MESSAGE_MAP()

BOOL CEntityTree::SelectItem( HTREEITEM hItem )
{
	return CViewTree::SelectItem( hItem );
}

void CEntityTree::OnSelChangedTreeCtrl( NMHDR * pNMHDR, LRESULT * pResult )
{
	HTREEITEM item = GetSelectedItem();
	if ( item )
	{
		CString strText = GetItemText( item );
		CT2CA pszConvertedAnsiString( strText );
		std::string strName( pszConvertedAnsiString );

		std::size_t uIDPos = strName.find( '[', 0 );
		if ( uIDPos != std::string::npos )
		{
			std::string strID = strName.substr( uIDPos, strName.size() - uIDPos );
			std::stringstream ss;
			ss << strID;
			char temp;
			unsigned int uID = -1;
			ss >> temp >> uID;

			if ( uID != -1 )
			{
				for ( EditorEntity* pEnt : theApp.m_pEditorEntities )
				{
					if ( pEnt && pEnt->pParentEntity && pEnt->pParentEntity->GetID() == uID )
					{
						theApp.m_pSelectedEntity = pEnt;
						glm::vec2 newLookAt = pEnt->pParentEntity->GetTransform().m_Position;
						theApp.m_vLookat = glm::vec3( newLookAt.x, newLookAt.y, 0.0f );

						glm::vec3 vCenter( theApp.m_vLookat.x, theApp.m_vLookat.y, 1.0f );
						glm::vec3 vUp( 0.0f, 1.0f, 0.0f );
						glm::mat4 mLookAt = glm::lookAtLH( theApp.m_vLookat, vCenter, vUp );

						ShaderManager& shaderMan = theApp.m_Renderer.GetShaderManager();

						for ( unsigned int x = 0; x < shaderMan.GetNumShaders(); x++ )
						{
							BaseShader* pShader = shaderMan.GetShader( x );
							shaderMan.UseShader( pShader );
							pShader->SetViewMatrix( mLookAt );
						}

						CMainFrame* pMainframe = ( CMainFrame* ) AfxGetMainWnd();
						if ( pMainframe )
						{
							pMainframe->m_wndProperties.InitPropList();
						}

						break;
					}
				}
			}
		}
	}

}
