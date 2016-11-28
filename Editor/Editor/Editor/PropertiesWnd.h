
#pragma once

class CPropertiesWnd : public CDockablePane
{
// Construction
public:
	CPropertiesWnd();

	void AdjustLayout();

	void InitPropList();

// Attributes
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

protected:
	CFont m_fntPropList;
	CMFCPropertyGridCtrl m_wndPropList;

// Implementation
public:
	virtual ~CPropertiesWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnPropertyChanged( __in WPARAM wparam, __in LPARAM lparam );

	DECLARE_MESSAGE_MAP()


	void SetPropListFont();


	void ApplyChanges( CMFCPropertyGridProperty* pProp );


	void InitBaseEntitiyPropList();
	void ApplyBaseEntityPropList( CMFCPropertyGridProperty* pProp );

	void InvalidateProperties();

private:
	// PROPERTIES
	CMFCPropertyGridProperty* m_pPositionPropX;
	CMFCPropertyGridProperty* m_pPositionPropY;
	CMFCPropertyGridProperty* m_pIDProp;
	CMFCPropertyGridProperty* m_pNameProp;

	CMFCPropertyGridProperty* m_pScalePropX;
	CMFCPropertyGridProperty* m_pScalePropY;
	CMFCPropertyGridProperty* m_pRotationProp;
	


};
