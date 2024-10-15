#pragma once


// CMeshListCtrl

class CMeshListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CMeshListCtrl)

public:
	CMeshListCtrl();
	virtual ~CMeshListCtrl();

protected:
	
	afx_msg void OnDropFiles(HDROP hDropInfo);
	DECLARE_MESSAGE_MAP()
};


