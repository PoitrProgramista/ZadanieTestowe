// ZadanieTestoweDoc.h : interface of the CZadanieTestoweDoc class
//

#pragma once
#include <vector>
#include "shapefil.h"

class CZadanieTestoweDoc : public CDocument
{
protected: // create from serialization only
	CZadanieTestoweDoc();
	DECLARE_DYNCREATE(CZadanieTestoweDoc)

	// Attributes
protected:
	std::vector<SHPObject*> entities;
	std::vector<POINT> partToPrint;
	CPoint translationVector;
	CPoint previousPointerLocation;
	int entityCount;
	int zoomFactor;
	int accuracy{ 10000 };
	bool fileGood = false;

	struct
	{
		double x;
		double y;
	}bias;

	// Operations
public:
	int getEntityCount();
	int getPartCount(int entity);
	int getVertexCount(int entity);
	int getZoomFactor();
	CPoint getTranslation();
	CPoint getPreviousPointerLocation();
	POINT calculateCoordinantes(double x, double y);
	std::vector<POINT>& loadPartToPrint(int entityNumber, int partNumber);
	void setZoomFactor(int zoomFactor);
	void addTranslation(CPoint vector);
	void setPreviousPointerLocation(CPoint point);
	void zoomInOnCursor(HWND windowHandle, CPoint cursorPosition);
	void zoomOutOnCursor(HWND windowHandle, CPoint cursorPosition);
	bool loadShapeFile(const char* fileName);
	void clearShape();

	// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

	// Implementation
public:
	virtual ~CZadanieTestoweDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileOpen();
};
