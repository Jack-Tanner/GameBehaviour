
// Editor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Editor.h"
#include "MainFrm.h"

#include "EditorDoc.h"
#include "EditorView.h"

#include <gl/GL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Windows.h>

#include <string>
#include <sstream>
#include <SpriteEntity.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEditorApp

BEGIN_MESSAGE_MAP(CEditorApp, CWinAppEx)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CEditorApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CEditorApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CEditorApp construction

CEditorApp::CEditorApp()
{
	m_bHiColorIcons = TRUE;
	strCurrentFile = L"untitled.json";

	m_vLookat = glm::vec3( 0.0f, 0.0f, 0.0f );
	m_fZoom = 1.0f;
	m_pSelectedEntity = nullptr;
	
	m_bUpdateClassView = false;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Editor.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CEditorApp object

CEditorApp theApp;


// CEditorApp initialization

BOOL CEditorApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CEditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();


	m_Renderer.Initialise();

	GLFont* m_pFont = m_Renderer.GetFontManager().AddFont( "Assets/Fonts/arial.ttf", 36 );


	float xRatio = 100.0f;
	float yRatio = 100.0f;
	float x = ( 1280 * 0.5f );
	float y = ( 720 * 0.5f );

	glm::mat4 projMatrix = glm::orthoLH( -x, x, -y, y, 1.0f, 100000000.0f );
	glm::vec3 eyePos( 0.0f, 0.0f, 0.0f );
	glm::vec3 lookAt( 0.0f, 0.0f, 1.0f );
	glm::vec3 Up( 0.0f, 1.0f, 0.0f );

	glm::mat4 viewMatrix = glm::lookAtLH( eyePos, lookAt, Up );

	BaseShader* pBasicShader = m_Renderer.GetShaderManager().CreateShaderFromFile( "Basic", "BasicVertexColour_Vert.glsl", "BasicVertexColour_Frag.glsl" );
	m_Renderer.GetShaderManager().UseShader( pBasicShader );
	pBasicShader->SetViewMatrix( viewMatrix );
	pBasicShader->SetProjectionMatrix( projMatrix );
	pBasicShader->SetWorldMatrix( glm::mat4( 1.0f ) );

	BaseShader* pTexturedShader = m_Renderer.GetShaderManager().CreateShaderFromFile( "BasicTextured", "BasicTextured_Vert.glsl", "BasicTextured_Frag.glsl" );
	m_Renderer.GetShaderManager().UseShader( pTexturedShader );
	pTexturedShader->SetViewMatrix( viewMatrix );
	pTexturedShader->SetProjectionMatrix( projMatrix );
	GLuint uTexLocation = pTexturedShader->GetUniformLocation( "texColour" );
	pTexturedShader->SetUniform( uTexLocation, 0 );

	return TRUE;
}

int CEditorApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	for ( auto& pEnt : m_pEditorEntities )
	{
		if ( pEnt->pClickableBox )
		{
			delete pEnt->pClickableBox;
		}
	}

	return CWinAppEx::ExitInstance();
}

BOOL CEditorApp::OnIdle( LONG lCount )
{
	static double fFrameDelta = 0;
	static LARGE_INTEGER uFrameTime;
	static bool bOnce = false;
	if ( !bOnce )
	{
		bOnce = true;
		QueryPerformanceCounter( &uFrameTime );
	}	

	LARGE_INTEGER uFrequency, uNow;
	QueryPerformanceFrequency( &uFrequency );
	QueryPerformanceCounter( &uNow );

	fFrameDelta = ( ( (double)uNow.QuadPart - ( double ) uFrameTime.QuadPart ) ) / ( double ) uFrequency.QuadPart;
	QueryPerformanceCounter( &uFrameTime );

	m_World.Update( ( float ) fFrameDelta );
	//m_PhysicsManager.Update( (float)fFrameDelta );
	
	if ( m_bUpdateClassView )
	{
		m_bUpdateClassView = false;
		CMainFrame* pMainframe = ( CMainFrame* ) AfxGetMainWnd();
		pMainframe->m_wndClassView.FillClassView();
	}

	HGLRC   hrc;
	hrc = ::wglGetCurrentContext();
	if ( hrc != NULL )
	{
		if ( m_pSelectedEntity && m_pSelectedEntity->pClickableBox )
		{
			const glm::vec2& vPos = m_pSelectedEntity->pClickableBox->GetPosition();
			const glm::vec2& vSize = m_pSelectedEntity->pClickableBox->GetSize();

			glm::vec2 vCorners[4];
			vCorners[0] = glm::vec2( vPos.x - vSize.x, vPos.y - vSize.y );
			vCorners[1] = glm::vec2( vPos.x + vSize.x, vPos.y - vSize.y );
			vCorners[2] = glm::vec2( vPos.x + vSize.x, vPos.y + vSize.y );
			vCorners[3] = glm::vec2( vPos.x - vSize.x, vPos.y + vSize.y );
			m_Renderer.DrawDebugQuad( vCorners, glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f ) );
		}
		
		m_Renderer.Render();
	}

	std::stringstream ss;
	ss << roundf( ( (float)fFrameDelta * 1000.0f ) * 100.0f ) / 100.0f << "ms";
	std::string strFPS = ss.str();
	//m_Renderer.GetFontManager().PrintString( m_Renderer.GetFontManager().GetFont( "Assets/Fonts/arial.ttf", 36 ), strFPS, -630.0f, 320.0f, glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );

	SwapBuffers( wglGetCurrentDC() );

	return TRUE;
}

int CEditorApp::Run()
{
	bool bIdle = false;
	MSG msg;
	LONG lIdleCount = 0;
	for ( ;; )
	{
		while ( bIdle &&
			!::PeekMessage( &msg, NULL, NULL, NULL, PM_NOREMOVE ) )
		{
			if ( !OnIdle( lIdleCount++ ) )
				bIdle = FALSE; 
		}

		do
		{
			if ( !PumpMessage() )
			{
	//			m_Renderer.Shutdown();
				return ExitInstance();
			}

			OnIdle( 1 );

			if ( IsIdleMessage( &msg ) )
			{
				bIdle = TRUE;
				lIdleCount = 0;
			}

		} while ( ::PeekMessage( &msg, NULL, NULL, NULL, PM_NOREMOVE ) );

	}

	m_Renderer.Shutdown();

	return 0;
}

void CEditorApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CEditorApp::LoadCustomState()
{
}

void CEditorApp::SaveCustomState()
{
}

void CEditorApp::OnFileOpen()
{
	TCHAR szFilters[] = _T( "JSON Files (*.json)|*.json|All Files (*.*)|*.*||" );

	CFileDialog fileDlg( TRUE, _T( "JSON" ), NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters );

	if ( fileDlg.DoModal() == IDOK )
	{
		CString pathName = fileDlg.GetPathName();

		CString fileName = fileDlg.GetFileTitle();
		fileName += ".";
		fileName += fileDlg.GetFileExt();

		for ( auto& pEnt : m_pEditorEntities )
		{
			delete pEnt->pClickableBox;
			delete pEnt;
		}
		m_pSelectedEntity = nullptr;
		m_pEditorEntities.clear();
		m_World.Destroy();

		m_World.SetNextID( 0 );

		strCurrentFile = fileName;
		CT2CA pszConvertedAnsiString( fileName );
		std::string fileStr( pszConvertedAnsiString );
		m_World.SerializeIn( fileStr );

		std::wstring strNewTitle = fileDlg.GetFileTitle();
		strNewTitle += L" - Editor";
		m_pMainWnd->SetWindowTextW( strNewTitle.c_str() );

		m_vLookat = glm::vec3( 0.0f, 0.0f, 0.0f );
		m_fZoom = 1.0f;

		float xRatio = 100.0f;
		float yRatio = 100.0f;
		float x = ( 1280 * 0.5f ) * theApp.m_fZoom;
		float y = ( 720 * 0.5f ) * theApp.m_fZoom;
		glm::mat4 projMatrix = glm::orthoLH( -x, x, -y, y, 1.0f, 100000000.0f );

		glm::vec3 vCenter( theApp.m_vLookat.x, theApp.m_vLookat.y, 1.0f );
		glm::vec3 vUp( 0.0f, 1.0f, 0.0f );
		glm::mat4 mLookAt = glm::lookAtLH( theApp.m_vLookat, vCenter, vUp );

		ShaderManager& shaderMan = theApp.m_Renderer.GetShaderManager();

		for ( unsigned int x = 0; x < shaderMan.GetNumShaders(); x++ )
		{
			BaseShader* pShader = shaderMan.GetShader( x );
			shaderMan.UseShader( pShader );
			pShader->SetProjectionMatrix( projMatrix );
			pShader->SetViewMatrix( mLookAt );
		}

		for ( auto& pEnt : m_World.GetEntityList() )
		{
			EditorEntity* pEditorEnt = new EditorEntity();
			pEditorEnt->pParentEntity = pEnt;
			glm::vec2 vSize;

			SpriteEntity* pSpriteEnt = dynamic_cast< SpriteEntity* >( pEnt );
			if ( pSpriteEnt )
			{
				BaseTexture* pTexture = pSpriteEnt->GetTexture();
				if ( pTexture )
				{
					vSize.x = pTexture->GetWidth() * pEnt->GetTransform().m_Scale.x;
					vSize.y = pTexture->GetHeight() * pEnt->GetTransform().m_Scale.y;
				}
			}

			pEditorEnt->pClickableBox = new AABBShape( pEnt->GetTransform().m_Position, vSize );

			m_pEditorEntities.push_back( pEditorEnt );
		}

		CMainFrame* pMainframe = (CMainFrame*)AfxGetMainWnd();
		pMainframe->m_wndClassView.FillClassView();
	}
}

void CEditorApp::OnFileSave()
{
	TCHAR szFilters[] = _T( "JSON Files (*.json)|*.json|All Files (*.*)|*.*||" );

	CFileDialog fileDlg( FALSE, _T( "JSON" ), strCurrentFile.c_str(),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters );

	if ( fileDlg.DoModal() == IDOK )
	{
		CString pathName = fileDlg.GetPathName();

		CString fileName = fileDlg.GetFileTitle();
		fileName += ".";
		fileName += fileDlg.GetFileExt();

		CT2CA pszConvertedAnsiString( fileName );
		std::string fileStr( pszConvertedAnsiString );
		m_World.SerializeOut( fileStr );
	}
}

void CEditorApp::OnFileNew()
{
	static bool bOnce = false;
	CWinAppEx::OnFileNew();

	if( bOnce )
	{
		TCHAR szFilters[] = _T( "JSON Files (*.json)|*.json|All Files (*.*)|*.*||" );

		CFileDialog fileDlg( FALSE, _T( "JSON" ), strCurrentFile.c_str(),
			OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters );

		if ( fileDlg.DoModal() == IDOK )
		{
			CString pathName = fileDlg.GetPathName();

			CString fileName = fileDlg.GetFileTitle();
			fileName += ".";
			fileName += fileDlg.GetFileExt();

			CT2CA pszConvertedAnsiString( fileName );
			std::string fileStr( pszConvertedAnsiString );
			m_World.SerializeOut( fileStr );

			for ( auto& pEnt : m_pEditorEntities )
			{
				delete pEnt->pClickableBox;
				delete pEnt;
			}

			m_pEditorEntities.clear();

			m_World.Destroy();
			

			m_vLookat = glm::vec3( 0.0f, 0.0f, 0.0f );

			CMainFrame* pMainframe = ( CMainFrame* ) AfxGetMainWnd();
			pMainframe->m_wndClassView.FillClassView();

			m_World.SetNextID( 0 );
		}
	} 

	if ( !bOnce )
		bOnce = true;

}
