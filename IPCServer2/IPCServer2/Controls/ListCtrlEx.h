#pragma once

// Sourced From: https://www.technical-recipes.com/2014/making-a-mfc-list-control-editable/

#include "InPlaceEdit.h"

class CListCtrlEx : public CListCtrl
{
public:
	int GetRowFromPoint( CPoint &point, int *col ) const;
	CEdit* EditSubLabel( int nItem, int nCol );

	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	void OnLButtonDown(UINT nFlags, CPoint point);

	bool GetClicked();
	void SetClicked(bool Value);

protected:
	bool m_fClicked;
};