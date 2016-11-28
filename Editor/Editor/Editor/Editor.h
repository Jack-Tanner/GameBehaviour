
// Editor.h : main header file for the Editor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#define EDITOR_ONLY 1

#include "resource.h"       // main symbols

#include <Renderer.h>
#include <PhysicsManager.h>
#include <World.h>
#include <AABBShape.h>

struct EditorEntity
{
	AABBShape* pClickableBox;
	BaseEntity* pParentEntity;
};

// CEditorApp:
// See Editor.cpp for the implementation of this class
//

class CEditorApp : public CWinAppEx
{
public:
	CEditorApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle( LONG lCount ); // return TRUE if more idle processing
	virtual int Run();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	Renderer	m_Renderer;
	PhysicsManager m_PhysicsManager;
	World		m_World;

	glm::vec3	m_vLookat;
	float		m_fZoom;

	bool		m_bUpdateClassView;

	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileNew();

	DECLARE_MESSAGE_MAP()

public:
	std::wstring strCurrentFile;
	std::vector<EditorEntity*> m_pEditorEntities;

	EditorEntity* m_pSelectedEntity;

	
};

extern CEditorApp theApp;
