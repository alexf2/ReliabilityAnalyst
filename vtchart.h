#if !defined(AFX_VTCHART_H__AFD39BD4_00BF_11D2_9887_00504E02C39D__INCLUDED_)
#define AFX_VTCHART_H__AFD39BD4_00BF_11D2_9887_00504E02C39D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CPicture;

/////////////////////////////////////////////////////////////////////////////
// CVtChart wrapper class

class CVtChart : public CWnd
{
protected:
	DECLARE_DYNCREATE(CVtChart)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x5a721580, 0x5af0, 0x11ce, { 0x83, 0x84, 0x0, 0x20, 0xaf, 0x23, 0x37, 0xf2 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:
	BOOL GetAutoIncrement();
	void SetAutoIncrement(BOOL);
	BOOL GetRandomFill();
	void SetRandomFill(BOOL);
	short GetChartType();
	void SetChartType(short);
	short GetColumn();
	void SetColumn(short);
	short GetColumnCount();
	void SetColumnCount(short);
	CString GetColumnLabel();
	void SetColumnLabel(LPCTSTR);
	short GetColumnLabelCount();
	void SetColumnLabelCount(short);
	short GetColumnLabelIndex();
	void SetColumnLabelIndex(short);
	CString GetData();
	void SetData(LPCTSTR);
	CString GetFootnoteText();
	void SetFootnoteText(LPCTSTR);
	BOOL GetRepaint();
	void SetRepaint(BOOL);
	short GetRow();
	void SetRow(short);
	short GetRowCount();
	void SetRowCount(short);
	CString GetRowLabel();
	void SetRowLabel(LPCTSTR);
	short GetRowLabelCount();
	void SetRowLabelCount(short);
	short GetRowLabelIndex();
	void SetRowLabelIndex(short);
	short GetSeriesColumn();
	void SetSeriesColumn(short);
	short GetSeriesType();
	void SetSeriesType(short);
	BOOL GetShowLegend();
	void SetShowLegend(BOOL);
	short GetSsLinkMode();
	void SetSsLinkMode(short);
	CString GetSsLinkRange();
	void SetSsLinkRange(LPCTSTR);
	short GetDrawMode();
	void SetDrawMode(short);
	short GetBorderStyle();
	void SetBorderStyle(short);
	BOOL GetEnabled();
	void SetEnabled(BOOL);
	OLE_HANDLE GetHWnd();
	void SetHWnd(OLE_HANDLE);
	LPDISPATCH GetTitle();
	void SetTitle(LPDISPATCH);
	LPDISPATCH GetFootnote();
	void SetFootnote(LPDISPATCH);
	CString GetTitleText();
	void SetTitleText(LPCTSTR);
	BOOL GetStacking();
	void SetStacking(BOOL);
	short GetTextLengthType();
	void SetTextLengthType(short);
	BOOL GetAllowUserChanges();
	void SetAllowUserChanges(BOOL);
	BOOL GetAllowSelections();
	void SetAllowSelections(BOOL);
	BOOL GetAllowSeriesSelection();
	void SetAllowSeriesSelection(BOOL);
	BOOL GetAllowDynamicRotation();
	void SetAllowDynamicRotation(BOOL);
	short GetActiveSeriesCount();
	void SetActiveSeriesCount(short);
	LPDISPATCH GetBackdrop();
	void SetBackdrop(LPDISPATCH);
	LPDISPATCH GetPrintInformation();
	void SetPrintInformation(LPDISPATCH);
	LPDISPATCH GetLegend();
	void SetLegend(LPDISPATCH);
	LPDISPATCH GetDataGrid();
	void SetDataGrid(LPDISPATCH);
	LPDISPATCH GetPlot();
	void SetPlot(LPDISPATCH);
	long GetTwipsWidth();
	void SetTwipsWidth(long);
	long GetTwipsHeight();
	void SetTwipsHeight(long);
	BOOL GetAllowDithering();
	void SetAllowDithering(BOOL);
	short GetErrorOffset();
	void SetErrorOffset(short);
	BOOL GetDoSetCursor();
	void SetDoSetCursor(BOOL);
	CString GetFileName();
	void SetFileName(LPCTSTR);
	BOOL GetChart3d();
	void SetChart3d(BOOL);
	CString GetSsLinkBook();
	void SetSsLinkBook(LPCTSTR);

// Operations
public:
	void EditPaste();
	void EditCopy();
	void ActivateSelectionDialog();
	void Layout();
	void ToDefaults();
	void PrintSetupDialog();
	void PrintChart();
	void SelectPart(short part, short index1, short index2, short index3, short index4);
	void GetSelectedPart(short* part, short* index1, short* index2, short* index3, short* index4);
	void GetDLLVersion(short* major, short* minor);
	void TwipsToChartPart(long xVal, long yVal, short* part, short* index1, short* index2, short* index3, short* index4);
	void ReadFromFile(LPCTSTR FileName);
	void WritePictureToFile(LPCTSTR FileName, short pictureType, short options);
	void WriteToFile(LPCTSTR FileName);
	void ActivateFormatMenu(long X, long Y);
	void GetMetafile(short options, long* Handle, long* Width, long* Height);
	void CopyDataFromArray(short Top, short Left, short Bottom, short Right, const VARIANT& Array);
	void CopyDataToArray(short Top, short Left, short Bottom, short Right, const VARIANT& Array);
	void Draw(long hDC, short hDCType, long Top, long Left, long Bottom, long Right, BOOL Layout, BOOL Stretch);
	long GetGetBitmap(const VARIANT& options);
	void UseWizard();
	void EditChartData();
	void Refresh();
	void AboutBox();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VTCHART_H__AFD39BD4_00BF_11D2_9887_00504E02C39D__INCLUDED_)
