// ZadanieTestoweView.h : interface of the CZadanieTestoweView class
//

#pragma once

class CZadanieTestoweView : public CView
{
protected: // create from serialization only
	CZadanieTestoweView();
	DECLARE_DYNCREATE(CZadanieTestoweView)

	// Attributes
public:
	CZadanieTestoweDoc * GetDocument() const;
	CPen pointStyle{ PS_SOLID, 1, RGB(200, 0, 0) };
	const int maxZoomIn{ 1 };
	const int maxZoomOut{ 65536 };

	// Operations
public:

	// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// Implementation
public:
	virtual ~CZadanieTestoweView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in ZadanieTestoweView.cpp
inline CZadanieTestoweDoc* CZadanieTestoweView::GetDocument() const
{
	return reinterpret_cast<CZadanieTestoweDoc*>( m_pDocument );
}
#endif
