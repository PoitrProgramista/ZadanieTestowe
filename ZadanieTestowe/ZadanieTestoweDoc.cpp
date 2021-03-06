// ZadanieTestoweDoc.cpp : implementation of the CZadanieTestoweDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ZadanieTestowe.h"
#endif

#include "ZadanieTestoweDoc.h"

#include <propkey.h>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CZadanieTestoweDoc

IMPLEMENT_DYNCREATE(CZadanieTestoweDoc, CDocument)

BEGIN_MESSAGE_MAP(CZadanieTestoweDoc, CDocument)

	ON_COMMAND(ID_FILE_OPEN32771, &CZadanieTestoweDoc::OnFileOpen)
END_MESSAGE_MAP()

// CZadanieTestoweDoc construction/destruction

CZadanieTestoweDoc::CZadanieTestoweDoc()
{
}

CZadanieTestoweDoc::~CZadanieTestoweDoc()
{
	clearShape();
}

BOOL CZadanieTestoweDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

// CZadanieTestoweDoc serialization

void CZadanieTestoweDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CZadanieTestoweDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CZadanieTestoweDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CZadanieTestoweDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CZadanieTestoweDoc diagnostics

#ifdef _DEBUG
void CZadanieTestoweDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CZadanieTestoweDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

int CZadanieTestoweDoc::getEntityCount()
{
	return entityCount;
}

int CZadanieTestoweDoc::getPartCount(int entity)
{
	return entities[entity]->nParts;
}

int CZadanieTestoweDoc::getVertexCount(int entity)
{
	return entities[entity]->nVertices;
}

void CZadanieTestoweDoc::setZoomFactor(int zoomFactor)
{
	this->zoomFactor = zoomFactor;
}

int CZadanieTestoweDoc::getZoomFactor()
{
	return zoomFactor;
}

void CZadanieTestoweDoc::addTranslation(CPoint vector)
{
	this->translationVector += vector;
}

CPoint CZadanieTestoweDoc::getTranslation()
{
	return translationVector;
}

void CZadanieTestoweDoc::setPreviousPointerLocation(CPoint point)
{
	previousPointerLocation = point;
}

CPoint CZadanieTestoweDoc::getPreviousPointerLocation()
{
	return previousPointerLocation;
}

std::vector<POINT>& CZadanieTestoweDoc::loadPartToPrint(int currentEntity, int currentPartNumber)
{
	int partOffset = entities[currentEntity]->panPartStart[currentPartNumber];
	int nextPartNumber = currentPartNumber + 1;
	int currentEntityPartCount = getPartCount(currentEntity);
	int currentEntityVertexCount = getVertexCount(currentEntity);
	int partVertexCount = 0;

	partToPrint.clear();

	if (nextPartNumber == currentEntityPartCount)
	{
		partVertexCount = currentEntityVertexCount - partOffset;
	}
	else
	{
		int nextPartOffset = entities[currentEntity]->panPartStart[nextPartNumber];

		partVertexCount = nextPartOffset - partOffset;
	}

	for (auto i = 0; i < partVertexCount; i++)
	{
		double x = entities[currentEntity]->padfX[partOffset + i];
		double y = entities[currentEntity]->padfY[partOffset + i];

		POINT point = calculateCoordinantes(x, y);

		partToPrint.emplace_back(point);
	}
	return partToPrint;
}

POINT CZadanieTestoweDoc::calculateCoordinantes(double x, double y)
{
	POINT point;

	point.x = ( x - bias.x ) * accuracy;
	point.x /= zoomFactor;
	point.x += translationVector.x;

	point.y = -( y - bias.y ) * accuracy;
	point.y /= zoomFactor;
	point.y += translationVector.y;

	return point;
}

void CZadanieTestoweDoc::zoomInOnCursor(HWND windowHandle, CPoint cursorPosition)
{
	POINT clientCursorPosition{ cursorPosition };
	LPPOINT coodrinantes = &clientCursorPosition;
	ScreenToClient(windowHandle, coodrinantes);

	translationVector.x += translationVector.x - clientCursorPosition.x;
	translationVector.y += translationVector.y - clientCursorPosition.y;
}

void CZadanieTestoweDoc::zoomOutOnCursor(HWND windowHandle, CPoint cursorPosition)
{
	POINT clientCursorPosition{ cursorPosition };
	LPPOINT coodrinantes = &clientCursorPosition;
	ScreenToClient(windowHandle, coodrinantes);

	translationVector.x -= ( translationVector.x - clientCursorPosition.x ) / 2;
	translationVector.y -= ( translationVector.y - clientCursorPosition.y ) / 2;
}

// CZadanieTestoweDoc commands
bool CZadanieTestoweDoc::loadShapeFile(const char* fileName)
{
	SHPHandle shapeFile = SHPOpen(fileName, "rb");

	if (shapeFile == NULL)
	{
		AfxMessageBox(_T("Please select a ShapeFile in folder \"plik\""));
		return false;
	}

	SHPGetInfo(shapeFile, &entityCount, nullptr, nullptr, nullptr);

	clearShape();

	entities.reserve(entityCount);

	for (auto i = 0; i < entityCount; ++i)
		entities.emplace_back(SHPReadObject(shapeFile, i));

	SHPClose(shapeFile);

	double originX = entities[0]->padfX[0];
	double originY = entities[0]->padfY[0];

	bias.x = originX;
	bias.y = originY;

	return true;
}

void CZadanieTestoweDoc::OnFileOpen()
{
	CFileDialog openedFile(TRUE, NULL, NULL);
	openedFile.DoModal();
	std::string convertedFileName = CT2A(openedFile.GetFileName());
	std::string fileName = "plik/" + convertedFileName;
	fileGood = loadShapeFile(fileName.c_str());
	if (fileGood)
		UpdateAllViews(NULL);
}

void CZadanieTestoweDoc::clearShape()
{
	for (SHPObject* entity : entities)
	{
		SHPDestroyObject(entity);
	}
	entities.clear();
	translationVector = { 600,750 };
	zoomFactor = 256;
}