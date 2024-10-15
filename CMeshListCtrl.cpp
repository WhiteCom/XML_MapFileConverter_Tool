// CMeshListCtrl.cpp : implementation file
//

#include "pch.h"
#include "GXMeshTexture_Converter_Tool.h"
#include "GXMeshTexture_Converter_ToolDlg.h"
#include "CMeshListCtrl.h"


// CMeshListCtrl

IMPLEMENT_DYNAMIC(CMeshListCtrl, CListCtrl)

CMeshListCtrl::CMeshListCtrl()
{

}

CMeshListCtrl::~CMeshListCtrl()
{
}

BEGIN_MESSAGE_MAP(CMeshListCtrl, CListCtrl)
	ON_WM_DROPFILES(IDC_MESH_FILE_LISTBOX, OnDropFiles)
END_MESSAGE_MAP()

//
// Begin - CMeshListCtrl message handlers
//

void CMeshListCtrl::OnDropFiles(HDROP hDropInfo)
{
	TCHAR szPathName[MAX_PATH] = { 0, };

	// ���� ����Ʈ �ڽ� ���빰 �ʱ�ȭ
	CGXMeshTextureConverterToolDlg* pDlg = (CGXMeshTextureConverterToolDlg*)(AfxGetMainWnd());

	pDlg->AllClearListBox(IDC_MESH_FILE_LISTBOX);

	UINT uCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, szPathName, MAX_PATH);
	 
	for (UINT i = 0; i < uCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szPathName, MAX_PATH);

		// ������ ���� ����Ʈ�ڽ� ��Ʈ�ѿ� �߰�
		CListBox* pListBox = (CListBox*)(pDlg->GetDlgItem(IDC_MESH_FILE_LISTBOX));
		if (pListBox == NULL)
		{
			MessageBox(_T("IDC_MESH_FILE_LISTBOX ��Ʈ���� �������� ���߽��ϴ�."), _T("����"), MB_OK);
			break;
		}
		
		// �巡�� �� ����, ���ϸ� �����ϱ� 
		std::wstring filePath = L"";
		filePath += szPathName;
		std::wstring realFileName = L"";

		if (!filePath.empty())
		{
			// Path ����� �ؽ�Ʈ ����
			std::wstring::size_type pos_slush; // '/' ���� ã��� 
			std::wstring::size_type pos_reverse_slush; // '\' ���� ã���

			pos_slush = filePath.rfind(L"/");
			pos_reverse_slush = filePath.rfind(L"\\");

			if (pos_slush == std::wstring::npos && pos_reverse_slush != std::wstring::npos)
			{
				// ���� Path�� '\' �ִ°��
				realFileName = filePath.substr(pos_reverse_slush + 1);
			}
			else if (pos_slush != std::string::npos && pos_reverse_slush == std::string::npos)
			{
				// ���� Path�� '/' �ִ°��
				realFileName = filePath.substr(pos_slush + 1);
			}
			else
			{
				// ���� URL�� '/', '\' �� �� ���� ���
				realFileName = filePath;
			}
		}

		// ���� Ȯ���� üũ
		std::wstring::size_type pos_GX_extension;
		std::wstring::size_type pos_GB_extension;

		pos_GX_extension = realFileName.find(L"gx");
		pos_GB_extension = realFileName.find(L"gb");

		// Ȯ���ڿ� �´� ���״Ͼ ����
		if (pos_GX_extension != std::wstring::npos)
		{
			// GX File
			pDlg->AddGXFileList(filePath.c_str()); // �����̳ʿ� ���� �� �ش� ���ҽ� �����ϴ� �����θ� ����
		}
		else if (pos_GB_extension != std::wstring::npos)
		{
			// GB File
			pDlg->AddGBFileList(filePath.c_str()); // �����̳ʿ� ���� �� �ش� ���ҽ� �����ϴ� �����θ� ����
		}
		else
		{
			// � Ȯ���ڿ��� �ش����� �ʴ°��
			MessageBox(_T("gx �Ǵ� gb ������ �ƴմϴ�. �ùٸ� ������ �߰����ּ���"), _T("�˸�"), MB_OK);
			return;
		}

		// ����Ʈ �ڽ��� ���ϸ� �߰�
		pListBox->AddString(realFileName.c_str());
	}

	// ����Ʈ �� ���� ���� ����
	pDlg->UpdateListBoxFileCount(IDC_MESH_FILE_LISTBOX);

	::DragFinish(hDropInfo);

	CListCtrl::OnDropFiles(hDropInfo);
}

//
// End - CMeshListCtrl message handlers
//


