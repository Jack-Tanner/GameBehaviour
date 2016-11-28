
#pragma once

#include "ViewTree.h"

class CEntityTree : public CViewTree
{
public:

	BOOL SelectItem( HTREEITEM hItem );

protected:
	afx_msg void OnSelChangedTreeCtrl( NMHDR* pNMHDR, LRESULT* pResult );
	DECLARE_MESSAGE_MAP()

};

class CClassView : public CDockablePane
{
public:
	CClassView();
	virtual ~CClassView();

	void AdjustLayout();
	void OnChangeVisualStyle();

	void FillClassView();

	

protected:
	CEntityTree m_wndClassView;
	CImageList m_ClassViewImages;
	UINT m_nCurrSort;



// Overrides
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClassAddMemberFunction();
	afx_msg void OnClassAddMemberVariable();
	afx_msg void OnClassDefinition();
	afx_msg void OnClassProperties();
	afx_msg void OnNewFolder();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	afx_msg void OnSort(UINT id);
	afx_msg void OnUpdateSort(CCmdUI* pCmdUI);
	afx_msg void OnDblClick( NMHDR *pNMHDR, LRESULT *pResult );
	DECLARE_MESSAGE_MAP()
};

