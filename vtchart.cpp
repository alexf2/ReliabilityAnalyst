// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "vtchart.h"

// Dispatch interfaces referenced by this interface
#include "Picture.h"

/////////////////////////////////////////////////////////////////////////////
// CVtChart

IMPLEMENT_DYNCREATE(CVtChart, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CVtChart properties

BOOL CVtChart::GetAutoIncrement()
{
	BOOL result;
	GetProperty(0x1, VT_BOOL, (void*)&result);
	return result;
}

void CVtChart::SetAutoIncrement(BOOL propVal)
{
	SetProperty(0x1, VT_BOOL, propVal);
}

BOOL CVtChart::GetRandomFill()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}

void CVtChart::SetRandomFill(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}

short CVtChart::GetChartType()
{
	short result;
	GetProperty(0x3, VT_I2, (void*)&result);
	return result;
}

void CVtChart::SetChartType(short propVal)
{
	SetProperty(0x3, VT_I2, propVal);
}

short CVtChart::GetColumn()
{
	short result;
	GetProperty(0x4, VT_I2, (void*)&result);
	return result;
}

void CVtChart::SetColumn(short propVal)
{
	SetProperty(0x4, VT_I2, propVal);
}

short CVtChart::GetColumnCount()
{
	short result;
	GetProperty(0x5, VT_I2, (void*)&result);
	return result;
}

void CVtChart::SetColumnCount(short propVal)
{
	SetProperty(0x5, VT_I2, propVal);
}

CString CVtChart::GetColumnLabel()
{
	CString result;
	GetProperty(0x6, VT_BSTR, (void*)&result);
	return result;
}

void CVtChart::SetColumnLabel(LPCTSTR propVal)
{
	SetProperty(0x6, VT_BSTR, propVal);
}

short CVtChart::GetColumnLabelCount()
{
	short result;
	GetProperty(0x7, VT_I2, (void*)&result);
	return result;
}

void CVtChart::SetColumnLabelCount(short propVal)
{
	SetProperty(0x7, VT_I2, propVal);
}

short CVtChart::GetColumnLabelIndex()
{
	short result;
	GetProperty(0x8, VT_I2, (void*)&result);
	return result;
}

void CVtChart::SetColumnLabelIndex(short propVal)
{
	SetProperty(0x8, VT_I2, propVal);
}

CString CVtChart::GetData()
{
	CString result;
	GetProperty(0x9, VT_BSTR, (void*)&result);
	return result;
}

void CVtChart::SetData(LPCTSTR propVal)
{
	SetProperty(0x9, VT_BSTR, propVal);
}

CString CVtChart::GetFootnoteText()
{
	CString result;
	GetProperty(0xa, VT_BSTR, (void*)&result);
	return result;
}

void CVtChart::SetFootnoteText(LPCTSTR propVal)
{
	SetProperty(0xa, VT_BSTR, propVal);
}

BOOL CVtChart::GetRepaint()
{
	BOOL result;
	GetProperty(0xb, VT_BOOL, (void*)&result);
	return result;
}

void CVtChart::SetRepaint(BOOL propVal)
{
	SetProperty(0xb, VT_BOOL, propVal);
}

short CVtChart::GetRow()
{
	short result;
	GetProperty(0xc, VT_I2, (void*)&result);
	return result;
}

void CVtChart::SetRow(short propVal)
{
	SetProperty(0xc, VT_I2, propVal);
}

short CVtChart::GetRowCount()
{
	short result;
	GetProperty(0xd, VT_I2, (void*)&result);
	return result;
}

void CVtChart::SetRowCount(short propVal)
{
	SetProperty(0xd, VT_I2, propVal);
}

CString CVtChart::GetRowLabel()
{
	CString result;
	GetProperty(0xe, VT_BSTR, (void*)&result);
	return result;
}

void CVtChart::SetRowLabel(LPCTSTR propVal)
{
	SetProperty(0xe, VT_BSTR, propVal);
}

short CVtChart::GetRowLabelCount()
{
	short result;
	GetProperty(0xf, VT_I2, (void*)&result);
	return result;
}

void CVtChart::SetRowLabelCount(short propVal)
{
	SetProperty(0xf, VT_I2, propVal);
}

short CVtChart::GetRowLabelIndex()
{
	short result;
	GetProperty(0x10, VT_I2, (void*)&result);
	return result;
}

void CVtChart::SetRowLabelIndex(short propVal)
{
	SetProperty(0x10, VT_I2, propVal);
}

short CVtChart::GetSeriesColumn()
{
	short result;
	GetProperty(0x11, VT_I2, (void*)&result);
	return result;
}

void CVtChart::SetSeriesColumn(short propVal)
{
	SetProperty(0x11, VT_I2, propVal);
}

short CVtChart::GetSeriesType()
{
	short result;
	GetProperty(0x12, VT_I2, (void*)&result);
	return result;
}

void CVtChart::SetSeriesType(short propVal)
{
	SetProperty(0x12, VT_I2, propVal);
}

BOOL CVtChart::GetShowLegend()
{
	BOOL result;
	GetProperty(0x13, VT_BOOL, (void*)&result);
	return result;
}

void CVtChart::SetShowLegend(BOOL propVal)
{
	SetProperty(0x13, VT_BOOL, propVal);
}

short CVtChart::GetSsLinkMode()
{
	short result;
	GetProperty(0x14, VT_I2, (void*)&result);
	return result;
}

void CVtChart::SetSsLinkMode(short propVal)
{
	SetProperty(0x14, VT_I2, propVal);
}

CString CVtChart::GetSsLinkRange()
{
	CString result;
	GetProperty(0x15, VT_BSTR, (void*)&result);
	return result;
}

void CVtChart::SetSsLinkRange(LPCTSTR propVal)
{
	SetProperty(0x15, VT_BSTR, propVal);
}

short CVtChart::GetDrawMode()
{
	short result;
	GetProperty(0x17, VT_I2, (void*)&result);
	return result;
}

void CVtChart::SetDrawMode(short propVal)
{
	SetProperty(0x17, VT_I2, propVal);
}

short CVtChart::GetBorderStyle()
{
	short result;
	GetProperty(DISPID_BORDERSTYLE, VT_I2, (void*)&result);
	return result;
}

void CVtChart::SetBorderStyle(short propVal)
{
	SetProperty(DISPID_BORDERSTYLE, VT_I2, propVal);
}

BOOL CVtChart::GetEnabled()
{
	BOOL result;
	GetProperty(DISPID_ENABLED, VT_BOOL, (void*)&result);
	return result;
}

void CVtChart::SetEnabled(BOOL propVal)
{
	SetProperty(DISPID_ENABLED, VT_BOOL, propVal);
}

OLE_HANDLE CVtChart::GetHWnd()
{
	OLE_HANDLE result;
	GetProperty(DISPID_HWND, VT_I2, (void*)&result);
	return result;
}

void CVtChart::SetHWnd(OLE_HANDLE propVal)
{
	SetProperty(DISPID_HWND, VT_I2, propVal);
}

LPDISPATCH CVtChart::GetTitle()
{
	LPDISPATCH result;
	GetProperty(0x1a, VT_DISPATCH, (void*)&result);
	return result;
}

void CVtChart::SetTitle(LPDISPATCH propVal)
{
	SetProperty(0x1a, VT_DISPATCH, propVal);
}

LPDISPATCH CVtChart::GetFootnote()
{
	LPDISPATCH result;
	GetProperty(0x1b, VT_DISPATCH, (void*)&result);
	return result;
}

void CVtChart::SetFootnote(LPDISPATCH propVal)
{
	SetProperty(0x1b, VT_DISPATCH, propVal);
}

CString CVtChart::GetTitleText()
{
	CString result;
	GetProperty(0x1c, VT_BSTR, (void*)&result);
	return result;
}

void CVtChart::SetTitleText(LPCTSTR propVal)
{
	SetProperty(0x1c, VT_BSTR, propVal);
}

BOOL CVtChart::GetStacking()
{
	BOOL result;
	GetProperty(0x1d, VT_BOOL, (void*)&result);
	return result;
}

void CVtChart::SetStacking(BOOL propVal)
{
	SetProperty(0x1d, VT_BOOL, propVal);
}

short CVtChart::GetTextLengthType()
{
	short result;
	GetProperty(0x1e, VT_I2, (void*)&result);
	return result;
}

void CVtChart::SetTextLengthType(short propVal)
{
	SetProperty(0x1e, VT_I2, propVal);
}

BOOL CVtChart::GetAllowUserChanges()
{
	BOOL result;
	GetProperty(0x1f, VT_BOOL, (void*)&result);
	return result;
}

void CVtChart::SetAllowUserChanges(BOOL propVal)
{
	SetProperty(0x1f, VT_BOOL, propVal);
}

BOOL CVtChart::GetAllowSelections()
{
	BOOL result;
	GetProperty(0x20, VT_BOOL, (void*)&result);
	return result;
}

void CVtChart::SetAllowSelections(BOOL propVal)
{
	SetProperty(0x20, VT_BOOL, propVal);
}

BOOL CVtChart::GetAllowSeriesSelection()
{
	BOOL result;
	GetProperty(0x21, VT_BOOL, (void*)&result);
	return result;
}

void CVtChart::SetAllowSeriesSelection(BOOL propVal)
{
	SetProperty(0x21, VT_BOOL, propVal);
}

BOOL CVtChart::GetAllowDynamicRotation()
{
	BOOL result;
	GetProperty(0x22, VT_BOOL, (void*)&result);
	return result;
}

void CVtChart::SetAllowDynamicRotation(BOOL propVal)
{
	SetProperty(0x22, VT_BOOL, propVal);
}

short CVtChart::GetActiveSeriesCount()
{
	short result;
	GetProperty(0x23, VT_I2, (void*)&result);
	return result;
}

void CVtChart::SetActiveSeriesCount(short propVal)
{
	SetProperty(0x23, VT_I2, propVal);
}

LPDISPATCH CVtChart::GetBackdrop()
{
	LPDISPATCH result;
	GetProperty(0x24, VT_DISPATCH, (void*)&result);
	return result;
}

void CVtChart::SetBackdrop(LPDISPATCH propVal)
{
	SetProperty(0x24, VT_DISPATCH, propVal);
}

LPDISPATCH CVtChart::GetPrintInformation()
{
	LPDISPATCH result;
	GetProperty(0x25, VT_DISPATCH, (void*)&result);
	return result;
}

void CVtChart::SetPrintInformation(LPDISPATCH propVal)
{
	SetProperty(0x25, VT_DISPATCH, propVal);
}

LPDISPATCH CVtChart::GetLegend()
{
	LPDISPATCH result;
	GetProperty(0x26, VT_DISPATCH, (void*)&result);
	return result;
}

void CVtChart::SetLegend(LPDISPATCH propVal)
{
	SetProperty(0x26, VT_DISPATCH, propVal);
}

LPDISPATCH CVtChart::GetDataGrid()
{
	LPDISPATCH result;
	GetProperty(0x27, VT_DISPATCH, (void*)&result);
	return result;
}

void CVtChart::SetDataGrid(LPDISPATCH propVal)
{
	SetProperty(0x27, VT_DISPATCH, propVal);
}

LPDISPATCH CVtChart::GetPlot()
{
	LPDISPATCH result;
	GetProperty(0x28, VT_DISPATCH, (void*)&result);
	return result;
}

void CVtChart::SetPlot(LPDISPATCH propVal)
{
	SetProperty(0x28, VT_DISPATCH, propVal);
}

long CVtChart::GetTwipsWidth()
{
	long result;
	GetProperty(0x29, VT_I4, (void*)&result);
	return result;
}

void CVtChart::SetTwipsWidth(long propVal)
{
	SetProperty(0x29, VT_I4, propVal);
}

long CVtChart::GetTwipsHeight()
{
	long result;
	GetProperty(0x2a, VT_I4, (void*)&result);
	return result;
}

void CVtChart::SetTwipsHeight(long propVal)
{
	SetProperty(0x2a, VT_I4, propVal);
}

BOOL CVtChart::GetAllowDithering()
{
	BOOL result;
	GetProperty(0x2b, VT_BOOL, (void*)&result);
	return result;
}

void CVtChart::SetAllowDithering(BOOL propVal)
{
	SetProperty(0x2b, VT_BOOL, propVal);
}

short CVtChart::GetErrorOffset()
{
	short result;
	GetProperty(0x2c, VT_I2, (void*)&result);
	return result;
}

void CVtChart::SetErrorOffset(short propVal)
{
	SetProperty(0x2c, VT_I2, propVal);
}

BOOL CVtChart::GetDoSetCursor()
{
	BOOL result;
	GetProperty(0x2d, VT_BOOL, (void*)&result);
	return result;
}

void CVtChart::SetDoSetCursor(BOOL propVal)
{
	SetProperty(0x2d, VT_BOOL, propVal);
}

CString CVtChart::GetFileName()
{
	CString result;
	GetProperty(0x2e, VT_BSTR, (void*)&result);
	return result;
}

void CVtChart::SetFileName(LPCTSTR propVal)
{
	SetProperty(0x2e, VT_BSTR, propVal);
}

BOOL CVtChart::GetChart3d()
{
	BOOL result;
	GetProperty(0x2f, VT_BOOL, (void*)&result);
	return result;
}

void CVtChart::SetChart3d(BOOL propVal)
{
	SetProperty(0x2f, VT_BOOL, propVal);
}

CString CVtChart::GetSsLinkBook()
{
	CString result;
	GetProperty(0x30, VT_BSTR, (void*)&result);
	return result;
}

void CVtChart::SetSsLinkBook(LPCTSTR propVal)
{
	SetProperty(0x30, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CVtChart operations

void CVtChart::EditPaste()
{
	InvokeHelper(0x31, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CVtChart::EditCopy()
{
	InvokeHelper(0x32, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CVtChart::ActivateSelectionDialog()
{
	InvokeHelper(0x33, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CVtChart::Layout()
{
	InvokeHelper(0x34, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CVtChart::ToDefaults()
{
	InvokeHelper(0x35, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CVtChart::PrintSetupDialog()
{
	InvokeHelper(0x36, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CVtChart::PrintChart()
{
	InvokeHelper(0x37, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CVtChart::SelectPart(short part, short index1, short index2, short index3, short index4)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0x38, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 part, index1, index2, index3, index4);
}

void CVtChart::GetSelectedPart(short* part, short* index1, short* index2, short* index3, short* index4)
{
	static BYTE parms[] =
		VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2;
	InvokeHelper(0x39, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 part, index1, index2, index3, index4);
}

void CVtChart::GetDLLVersion(short* major, short* minor)
{
	static BYTE parms[] =
		VTS_PI2 VTS_PI2;
	InvokeHelper(0x3a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 major, minor);
}

void CVtChart::TwipsToChartPart(long xVal, long yVal, short* part, short* index1, short* index2, short* index3, short* index4)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2;
	InvokeHelper(0x3b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 xVal, yVal, part, index1, index2, index3, index4);
}

void CVtChart::ReadFromFile(LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName);
}

void CVtChart::WritePictureToFile(LPCTSTR FileName, short pictureType, short options)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I2 VTS_I2;
	InvokeHelper(0x3d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, pictureType, options);
}

void CVtChart::WriteToFile(LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName);
}

void CVtChart::ActivateFormatMenu(long X, long Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x3f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y);
}

void CVtChart::GetMetafile(short options, long* Handle, long* Width, long* Height)
{
	static BYTE parms[] =
		VTS_I2 VTS_PI4 VTS_PI4 VTS_PI4;
	InvokeHelper(0x40, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 options, Handle, Width, Height);
}

void CVtChart::CopyDataFromArray(short Top, short Left, short Bottom, short Right, const VARIANT& Array)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_VARIANT;
	InvokeHelper(0x41, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Top, Left, Bottom, Right, &Array);
}

void CVtChart::CopyDataToArray(short Top, short Left, short Bottom, short Right, const VARIANT& Array)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_VARIANT;
	InvokeHelper(0x42, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Top, Left, Bottom, Right, &Array);
}

void CVtChart::Draw(long hDC, short hDCType, long Top, long Left, long Bottom, long Right, BOOL Layout, BOOL Stretch)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_BOOL;
	InvokeHelper(0x43, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 hDC, hDCType, Top, Left, Bottom, Right, Layout, Stretch);
}

long CVtChart::GetGetBitmap(const VARIANT& options)
{
	long result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x46, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
		&options);
	return result;
}

void CVtChart::UseWizard()
{
	InvokeHelper(0x44, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CVtChart::EditChartData()
{
	InvokeHelper(0x45, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CVtChart::Refresh()
{
	InvokeHelper(DISPID_REFRESH, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CVtChart::AboutBox()
{
	InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
