
// EditorView.cpp : implementation of the CEditorView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Editor.h"
#endif

#include "EditorDoc.h"
#include "EditorView.h"
#include "MainFrm.h"

#include <gl/GL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ShaderManager.h>
#include <Debug.h>
#include <sstream>
#include <SpriteEntity.h>
#include <CollidableSpriteEntity.h>
#include <TextureManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

unsigned char threeto8[8] =
{
	0, 0111 >> 1, 0222 >> 1, 0333 >> 1, 0444 >> 1, 0555 >> 1, 0666 >> 1, 0377
};

unsigned char twoto8[4] =
{
	0, 0x55, 0xaa, 0xff
};

unsigned char oneto8[2] =
{
	0, 255
};

static int defaultOverride[13] =
{
	0, 3, 24, 27, 64, 67, 88, 173, 181, 236, 247, 164, 91
};

static PALETTEENTRY defaultPalEntry[20] =
{
	{ 0,   0,   0,    0 },
	{ 0x80,0,   0,    0 },
	{ 0,   0x80,0,    0 },
	{ 0x80,0x80,0,    0 },
	{ 0,   0,   0x80, 0 },
	{ 0x80,0,   0x80, 0 },
	{ 0,   0x80,0x80, 0 },
	{ 0xC0,0xC0,0xC0, 0 },

	{ 192, 220, 192,  0 },
	{ 166, 202, 240,  0 },
	{ 255, 251, 240,  0 },
	{ 160, 160, 164,  0 },

	{ 0x80,0x80,0x80, 0 },
	{ 0xFF,0,   0,    0 },
	{ 0,   0xFF,0,    0 },
	{ 0xFF,0xFF,0,    0 },
	{ 0,   0,   0xFF, 0 },
	{ 0xFF,0,   0xFF, 0 },
	{ 0,   0xFF,0xFF, 0 },
	{ 0xFF,0xFF,0xFF, 0 }
};

// CEditorView

IMPLEMENT_DYNCREATE(CEditorView, CView)

BEGIN_MESSAGE_MAP( CEditorView, CView )
	// Standard printing commands
	ON_COMMAND( ID_FILE_PRINT, &CView::OnFilePrint )
	ON_COMMAND( ID_FILE_PRINT_DIRECT, &CView::OnFilePrint )
	ON_COMMAND( ID_FILE_PRINT_PREVIEW, &CEditorView::OnFilePrintPreview )
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_COMMAND( ID_EDIT_COPY, OnEditCopy )
	ON_COMMAND( ID_EDIT_PASTE, OnEditPaste )	
	ON_COMMAND( ID_EDIT_DELETE, OnDeletePressed )
	ON_COMMAND( ID_NEW_SPRITEENTITY, OnNewSpriteEntity )
END_MESSAGE_MAP()


// CEditorView construction/destruction

CEditorView::CEditorView()
{
	m_pDC = NULL;
	m_pOldPalette = NULL;
	m_bMouseRDown = false;
}

CEditorView::~CEditorView()
{
}

BOOL CEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

int CEditorView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CView::OnCreate( lpCreateStruct ) == -1 )
		return -1;

	PIXELFORMATDESCRIPTOR pfd2;
	int         n;
	HGLRC       hrc;

	m_pDC = new CClientDC( this );

	ASSERT( m_pDC != NULL );

	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof( PIXELFORMATDESCRIPTOR ),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |          // support OpenGL
		PFD_DOUBLEBUFFER,             // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;

	if ( ( pixelformat = ChoosePixelFormat( m_pDC->GetSafeHdc(), &pfd ) ) == 0 )
	{
		MessageBox( L"ChoosePixelFormat failed" );
		return 0;
	}

	if ( SetPixelFormat( m_pDC->GetSafeHdc(), pixelformat, &pfd ) == FALSE )
	{
		MessageBox( L"SetPixelFormat failed" );
		return 0;
	}

	n = ::GetPixelFormat( m_pDC->GetSafeHdc() );
	::DescribePixelFormat( m_pDC->GetSafeHdc(), n, sizeof( pfd2 ), &pfd2 );

	CreateRGBPalette();

	hrc = wglCreateContext( m_pDC->GetSafeHdc() );
	wglMakeCurrent( m_pDC->GetSafeHdc(), hrc );

	GetClientRect( &m_oldRect );

	return 0;
}

void CEditorView::OnDraw(CDC* pDC)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}


// CEditorView printing

void CEditorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	if ( m_bMouseRDown )
	{
		m_bMouseRDown = false;

		ShowCursor( TRUE );
		CPoint projPoint = m_MouseRClickPos;
		ClientToScreen( &projPoint );

		ClipCursor( NULL );
	}

}

void CEditorView::OnRButtonDown( UINT nFlags, CPoint point )
{
	if ( !m_bMouseLDown )
	{
		m_bMouseRDown = true;
		m_MouseRClickPos = point;
		ShowCursor( FALSE );

		RECT clip;
		GetClientRect( &clip );
		clip.left += 5;
		clip.top += 30;
		clip.right -= 5;
		clip.bottom -= 5;
		ClientToScreen( &clip );
		ClipCursor( &clip );
	}

}

void CEditorView::OnLButtonUp( UINT nFlags, CPoint point )
{
	if ( m_bMouseLDown )
	{
		m_bMouseLDown = false;
		
	}
}

void CEditorView::OnLButtonDown( UINT nFlags, CPoint point )
{
	if ( !m_bMouseRDown )
	{
		RECT clientRect;
		GetClientRect( &clientRect );
		glm::vec3 vCenter( theApp.m_vLookat.x, theApp.m_vLookat.y, 1.0f );
		glm::vec3 vUp( 0.0f, 1.0f, 0.0f );
		glm::mat4 mView = glm::lookAtLH( theApp.m_vLookat, vCenter, vUp );

		float xRatio = 100.0f;
		float yRatio = 100.0f;
		float x = ( 1280 * 0.5f ) * theApp.m_fZoom;
		float y = ( 720 * 0.5f ) * theApp.m_fZoom;
		glm::mat4 mProj = glm::orthoLH( -x, x, -y, y, 1.0f, 100000000.0f );

		glm::mat4 mVP = mProj * mView;
		glm::mat4 mVPInverse = glm::inverse( mVP );

		glm::vec2 vMousePos;
		vMousePos.x = point.x / ( clientRect.right * 0.5f ) - 1.0f;
		vMousePos.y = point.y / ( clientRect.bottom * 0.5f ) - 1.0f;


		glm::vec4 vScreenPos( vMousePos.x, -vMousePos.y, 1.0f, 1.0f );
		glm::vec4 worldPos = mVPInverse * vScreenPos;
		vMousePos.x = worldPos.x;
		vMousePos.y = worldPos.y;


		bool bFound = false;
		for ( auto& pEnt : theApp.m_pEditorEntities )
		{
			if ( pEnt && pEnt->pClickableBox )
			{
				glm::vec2 min, max;
				min.x = pEnt->pClickableBox->GetPosition().x - pEnt->pClickableBox->GetSize().x;
				min.y = pEnt->pClickableBox->GetPosition().y - pEnt->pClickableBox->GetSize().y;

				max.x = pEnt->pClickableBox->GetPosition().x + pEnt->pClickableBox->GetSize().x;
				max.y = pEnt->pClickableBox->GetPosition().y + pEnt->pClickableBox->GetSize().y;

				if ( worldPos.x > min.x && worldPos.x < max.x &&
					worldPos.y > min.y && worldPos.y < max.y )
				{
					m_bMouseLDown = true;
					bFound = true;
					theApp.m_pSelectedEntity = pEnt;
					m_ClickOffset = pEnt->pClickableBox->GetPosition() - vMousePos;

					break;
				}
			}
		}

		if( !bFound )
			theApp.m_pSelectedEntity = nullptr;

		CMainFrame* pMainframe = ( CMainFrame* ) AfxGetMainWnd();
		if ( pMainframe )
		{
			pMainframe->m_wndProperties.InitPropList();
		}
	}

}


void CEditorView::OnMouseLeave()
{

}

void CEditorView::OnMouseMove( UINT nFlags, CPoint point )
{
	CView::OnMouseMove( nFlags, point );
	if ( m_bMouseRDown )
	{
		CPoint mouseDelta = point - m_MouseRClickPos;
		m_MouseRClickPos = point;

		theApp.m_vLookat.x -= mouseDelta.x * theApp.m_fZoom;
		theApp.m_vLookat.y += mouseDelta.y * theApp.m_fZoom;
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
		
	}
	else if ( m_bMouseLDown && theApp.m_pSelectedEntity )
	{
		RECT clientRect;
		GetClientRect( &clientRect );
		glm::vec3 vCenter( theApp.m_vLookat.x, theApp.m_vLookat.y, 1.0f );
		glm::vec3 vUp( 0.0f, 1.0f, 0.0f );
		glm::mat4 mView = glm::lookAtLH( theApp.m_vLookat, vCenter, vUp );

		float xRatio = 100.0f;
		float yRatio = 100.0f;
		float x = ( 1280 * 0.5f ) * theApp.m_fZoom;
		float y = ( 720 * 0.5f ) * theApp.m_fZoom;
		glm::mat4 mProj = glm::orthoLH( -x, x, -y, y, 1.0f, 100000000.0f );

		glm::mat4 mVP = mProj * mView;
		glm::mat4 mVPInverse = glm::inverse( mVP );

		glm::vec2 vMousePos;
		vMousePos.x = point.x / ( clientRect.right * 0.5f ) - 1.0f;
		vMousePos.y = point.y / ( clientRect.bottom * 0.5f ) - 1.0f;


		glm::vec4 vScreenPos( vMousePos.x, -vMousePos.y, 1.0f, 1.0f );
		glm::vec4 worldPos = mVPInverse * vScreenPos;
		vMousePos.x = worldPos.x;
		vMousePos.y = worldPos.y;

		theApp.m_pSelectedEntity->pClickableBox->SetPosition( vMousePos + m_ClickOffset );
		theApp.m_pSelectedEntity->pParentEntity->GetTransform().m_Position = vMousePos + m_ClickOffset;

		CMainFrame* pMainframe = ( CMainFrame* ) AfxGetMainWnd();
		if ( pMainframe )
		{
			pMainframe->m_wndProperties.InitPropList();
		}
	}
}

BOOL CEditorView::OnMouseWheel( UINT fFlags, short zDelta, CPoint point )
{
	theApp.m_fZoom -= ( zDelta / 120.0f ) * 0.5f;
	if ( theApp.m_fZoom < 0.5f )
		theApp.m_fZoom = 0.5f;

	float xRatio = 100.0f;
	float yRatio = 100.0f;
	float x = ( 1280 * 0.5f ) * theApp.m_fZoom;
	float y = ( 720 * 0.5f ) * theApp.m_fZoom;

	glm::mat4 projMatrix = glm::orthoLH( -x, x, -y, y, 1.0f, 100000000.0f );

	ShaderManager& shaderMan = theApp.m_Renderer.GetShaderManager();

	for ( unsigned int x = 0; x < shaderMan.GetNumShaders(); x++ )
	{
		BaseShader* pShader = shaderMan.GetShader( x );
		shaderMan.UseShader( pShader );
		pShader->SetProjectionMatrix( projMatrix );
	}

	return 0;
}

void CEditorView::OnEditCopy()
{
	if ( GetFocus() == this )
	{
		if ( theApp.m_pSelectedEntity && theApp.m_pSelectedEntity->pParentEntity )
		{
			theApp.m_pSelectedEntity->pParentEntity->SerializeOut( m_JsonCopy );
		}
	}
}

void CEditorView::OnEditPaste()
{
	if ( GetFocus() == this )
	{
		if ( m_JsonCopy.empty() == false )
		{
			BaseEntity* pNewEnt = nullptr;
			unsigned int uType = m_JsonCopy["type"];

			switch ( uType )
			{
			case eSprite:
				pNewEnt = new SpriteEntity();
				break;
			case eCollidableSprite:
				pNewEnt = new CollidableSpriteEntity();
				break;
			case eNone:
			case eMax:
			default:
				DebugBreak();
				break;
			}

			if ( pNewEnt != nullptr )
			{
				unsigned int uNextID = theApp.m_World.GetNextID();
				m_JsonCopy["id"] = uNextID;
				pNewEnt->SerializeIn( m_JsonCopy );
				theApp.m_World.AddEntity( pNewEnt );

				EditorEntity* pEditorEnt = new EditorEntity();
				pEditorEnt->pParentEntity = pNewEnt;
				glm::vec2 vSize;

				SpriteEntity* pSpriteEnt = dynamic_cast< SpriteEntity* >( pNewEnt );
				if ( pSpriteEnt )
				{
					BaseTexture* pTexture = pSpriteEnt->GetTexture();
					if ( pTexture )
					{
						vSize.x = pTexture->GetWidth() * pNewEnt->GetTransform().m_Scale.x;
						vSize.y = pTexture->GetHeight() * pNewEnt->GetTransform().m_Scale.y;
					}
				}

				pEditorEnt->pClickableBox = new AABBShape( pNewEnt->GetTransform().m_Position, vSize );

				theApp.m_pEditorEntities.push_back( pEditorEnt );

				CMainFrame* pMainframe = ( CMainFrame* ) AfxGetMainWnd();
				pMainframe->m_wndClassView.FillClassView();

			}
		}
	}
}

void CEditorView::OnDeletePressed()
{
	if ( GetFocus() == this )
	{
		if ( theApp.m_pSelectedEntity )
		{
			if ( theApp.m_pSelectedEntity->pParentEntity )
			{
				for ( auto& it = theApp.m_pEditorEntities.begin(); it != theApp.m_pEditorEntities.end(); it++ )
				{
					if ( ( *it )->pParentEntity == theApp.m_pSelectedEntity->pParentEntity )
					{
						theApp.m_pEditorEntities.erase( it );
						break;
					}
				}

				theApp.m_World.RemoveEntity( theApp.m_pSelectedEntity->pParentEntity );
				delete theApp.m_pSelectedEntity->pParentEntity;
				delete theApp.m_pSelectedEntity->pClickableBox;

				theApp.m_pSelectedEntity = nullptr;

			}
		}
	}
}

void CEditorView::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if ( nChar == VK_DELETE )
		OnDeletePressed();

	CView::OnKeyDown( nChar, nRepCnt, nFlags );
	
}

void CEditorView::OnNewSpriteEntity()
{
	SpriteInitialiser pInit;
	pInit.m_pszShader = "BasicTextured";
	pInit.m_pszTexture = "bricks.png";
	pInit.m_Transform.m_Position = theApp.m_vLookat;

	TextureManager::GetInstance()->AddTexture( "bricks.png" );

	SpriteEntity* pEntity = theApp.m_World.AddEntity<SpriteEntity>( &pInit );
	if ( pEntity )
	{
		EditorEntity* pEditorEnt = new EditorEntity();
		pEditorEnt->pParentEntity = pEntity;
		glm::vec2 vSize;
		if ( pEntity )
		{
			BaseTexture* pTexture = pEntity->GetTexture();
			if ( pTexture )
			{
				vSize.x = pTexture->GetWidth();
				vSize.y = pTexture->GetHeight();
			}
		}

		pEditorEnt->pClickableBox = new AABBShape( pEntity->GetTransform().m_Position, vSize );

		theApp.m_pEditorEntities.push_back( pEditorEnt );
		CMainFrame* pMainframe = ( CMainFrame* ) AfxGetMainWnd();
		pMainframe->m_wndClassView.FillClassView();

		theApp.m_pSelectedEntity = pEditorEnt;
	}
}


void CEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CEditorView::OnSize( UINT nType, int cx, int cy )
{
	CView::OnSize( nType, cx, cy );

	if ( cy > 0 )
	{
		glViewport( 0, 0, cx, cy );

		m_oldRect.right = cx;
		m_oldRect.bottom = cy;
	}
}

unsigned char ComponentFromIndex( int i, UINT nbits, UINT shift )
{
	unsigned char val;

	val = ( unsigned char ) ( i >> shift );
	switch ( nbits )
	{

	case 1:
		val &= 0x1;
		return oneto8[val];
	case 2:
		val &= 0x3;
		return twoto8[val];
	case 3:
		val &= 0x7;
		return threeto8[val];

	default:
		return 0;
	}
}


#pragma warning(disable : 4244)
void CEditorView::CreateRGBPalette()
{
	PIXELFORMATDESCRIPTOR pfd;
	LOGPALETTE *pPal;
	int n, i;

	n = ::GetPixelFormat( m_pDC->GetSafeHdc() );
	::DescribePixelFormat( m_pDC->GetSafeHdc(), n, sizeof( pfd ), &pfd );

	if ( pfd.dwFlags & PFD_NEED_PALETTE )
	{
		n = 1 << pfd.cColorBits;
		pPal = ( PLOGPALETTE ) new char[sizeof( LOGPALETTE ) + n * sizeof( PALETTEENTRY )];

		ASSERT( pPal != NULL );

		pPal->palVersion = 0x300;
		pPal->palNumEntries = n;
		for ( i = 0; i<n; i++ )
		{
			pPal->palPalEntry[i].peRed =
				ComponentFromIndex( i, pfd.cRedBits, pfd.cRedShift );
			pPal->palPalEntry[i].peGreen =
				ComponentFromIndex( i, pfd.cGreenBits, pfd.cGreenShift );
			pPal->palPalEntry[i].peBlue =
				ComponentFromIndex( i, pfd.cBlueBits, pfd.cBlueShift );
			pPal->palPalEntry[i].peFlags = 0;
		}

		/* fix up the palette to include the default GDI palette */
		if ( ( pfd.cColorBits == 8 ) &&
			( pfd.cRedBits == 3 ) && ( pfd.cRedShift == 0 ) &&
			( pfd.cGreenBits == 3 ) && ( pfd.cGreenShift == 3 ) &&
			( pfd.cBlueBits == 2 ) && ( pfd.cBlueShift == 6 )
			)
		{
			for ( i = 1; i <= 12; i++ )
				pPal->palPalEntry[defaultOverride[i]] = defaultPalEntry[i];
		}

		m_cPalette.CreatePalette( pPal );
		delete[] pPal;

		m_pOldPalette = m_pDC->SelectPalette( &m_cPalette, FALSE );
		m_pDC->RealizePalette();
	}
}
#pragma warning(default : 4244)

void CEditorView::OnDestroy()
{
	HGLRC   hrc;

	hrc = ::wglGetCurrentContext();

	::wglMakeCurrent( NULL, NULL );

	if ( hrc )
		::wglDeleteContext( hrc );

	if ( m_pOldPalette )
		m_pDC->SelectPalette( m_pOldPalette, FALSE );

	if ( m_pDC )
		delete m_pDC;

	CView::OnDestroy();
}


// CEditorView diagnostics

#ifdef _DEBUG
void CEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEditorDoc* CEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEditorDoc)));
	return (CEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CEditorView message handlers
