// ZadanieTestoweView.cpp : implementation of the CZadanieTestoweView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ZadanieTestowe.h"
#endif

#include "ZadanieTestoweDoc.h"
#include "ZadanieTestoweView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CZadanieTestoweView

IMPLEMENT_DYNCREATE(CZadanieTestoweView, CView)

BEGIN_MESSAGE_MAP(CZadanieTestoweView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CZadanieTestoweView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CZadanieTestoweView construction/destruction

CZadanieTestoweView::CZadanieTestoweView()
{
}

CZadanieTestoweView::~CZadanieTestoweView()
{
}

BOOL CZadanieTestoweView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CView::PreCreateWindow(cs);
}

// CZadanieTestoweView drawing

void CZadanieTestoweView::OnDraw(CDC* pDC)
{
	CZadanieTestoweDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int entityCount = pDoc->getEntityCount();

	for (auto i = 0; i < entityCount; ++i)
	{
		int partCount = pDoc->getPartCount(i);

		for (auto j = 0; j < partCount; ++j)
		{
			std::vector<POINT>& vertices = pDoc->loadPartToPrint(i, j);
			int vertexCount = vertices.size();

			int zoomFactor = pDoc->getZoomFactor();
			CPen lineStyle(PS_SOLID, 2048 / zoomFactor, RGB(0, 0, 200));

			pDC->SelectObject(&lineStyle);
			pDC->Polyline(&vertices[0], vertexCount);

			pDC->SelectObject(&pointStyle);
			CBrush pointFill(RGB(200, 100, 0));
			pDC->SelectObject(&pointFill);

			int pointSize = 2048 / zoomFactor;

			for (POINT verticle : vertices)
			{
				pDC->Ellipse(verticle.x - pointSize, verticle.y - pointSize, verticle.x + pointSize, verticle.y + pointSize);
			}
		}
	}
}

// CZadanieTestoweView printing

void CZadanieTestoweView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CZadanieTestoweView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CZadanieTestoweView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CZadanieTestoweView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CZadanieTestoweView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CZadanieTestoweView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// CZadanieTestoweView diagnostics

#ifdef _DEBUG
void CZadanieTestoweView::AssertValid() const
{
	CView::AssertValid();
}

void CZadanieTestoweView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CZadanieTestoweDoc* CZadanieTestoweView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CZadanieTestoweDoc)));
	return (CZadanieTestoweDoc*)m_pDocument;
}
#endif //_DEBUG

// CZadanieTestoweView message handlers

BOOL CZadanieTestoweView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CZadanieTestoweDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return CView::OnMouseWheel(nFlags, zDelta, pt);

	int currentZoom = pDoc->getZoomFactor();
	HWND windowHandle = GetSafeHwnd();

	if (zDelta > 0)
	{
		if (currentZoom / 2 <= maxZoomIn)
		{
			pDoc->setZoomFactor(currentZoom);
		}
		else
		{
			pDoc->setZoomFactor(currentZoom / 2);
			pDoc->zoomInOnCursor(windowHandle, pt);
		}
	}
	else
	{
		if (currentZoom * 2 >= maxZoomOut)
		{
			pDoc->setZoomFactor(currentZoom);
		}
		else
		{
			pDoc->setZoomFactor(currentZoom * 2);
			pDoc->zoomOutOnCursor(windowHandle, pt);
		}
	}
	Invalidate();
	UpdateWindow();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CZadanieTestoweView::OnMouseMove(UINT nFlags, CPoint point)
{
	CZadanieTestoweDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return CView::OnMouseMove(nFlags, point);

	if (nFlags == MK_LBUTTON)
	{
		CPoint previousPointerLocation = pDoc->getPreviousPointerLocation();
		pDoc->addTranslation(point - previousPointerLocation);

		Invalidate();
		UpdateWindow();
	}
	pDoc->setPreviousPointerLocation(point);
	CView::OnMouseMove(nFlags, point);
}