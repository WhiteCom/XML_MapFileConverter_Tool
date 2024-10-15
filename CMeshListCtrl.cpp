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

	// 기존 리스트 박스 내용물 초기화
	CGXMeshTextureConverterToolDlg* pDlg = (CGXMeshTextureConverterToolDlg*)(AfxGetMainWnd());

	pDlg->AllClearListBox(IDC_MESH_FILE_LISTBOX);

	UINT uCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, szPathName, MAX_PATH);
	 
	for (UINT i = 0; i < uCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szPathName, MAX_PATH);

		// 삽입한 파일 리스트박스 컨트롤에 추가
		CListBox* pListBox = (CListBox*)(pDlg->GetDlgItem(IDC_MESH_FILE_LISTBOX));
		if (pListBox == NULL)
		{
			MessageBox(_T("IDC_MESH_FILE_LISTBOX 컨트롤을 가져오지 못했습니다."), _T("오류"), MB_OK);
			break;
		}
		
		// 드래그 한 파일, 파일명만 추출하기 
		std::wstring filePath = L"";
		filePath += szPathName;
		std::wstring realFileName = L"";

		if (!filePath.empty())
		{
			// Path 요소의 텍스트 추출
			std::wstring::size_type pos_slush; // '/' 문자 찾기용 
			std::wstring::size_type pos_reverse_slush; // '\' 문자 찾기용

			pos_slush = filePath.rfind(L"/");
			pos_reverse_slush = filePath.rfind(L"\\");

			if (pos_slush == std::wstring::npos && pos_reverse_slush != std::wstring::npos)
			{
				// 현재 Path에 '\' 있는경우
				realFileName = filePath.substr(pos_reverse_slush + 1);
			}
			else if (pos_slush != std::string::npos && pos_reverse_slush == std::string::npos)
			{
				// 현재 Path에 '/' 있는경우
				realFileName = filePath.substr(pos_slush + 1);
			}
			else
			{
				// 현재 URL에 '/', '\' 둘 다 없는 경우
				realFileName = filePath;
			}
		}

		// 파일 확장자 체크
		std::wstring::size_type pos_GX_extension;
		std::wstring::size_type pos_GB_extension;

		pos_GX_extension = realFileName.find(L"gx");
		pos_GB_extension = realFileName.find(L"gb");

		// 확장자에 맞는 컨테니어에 삽입
		if (pos_GX_extension != std::wstring::npos)
		{
			// GX File
			pDlg->AddGXFileList(filePath.c_str()); // 컨테이너에 삽입 시 해당 리소스 존재하는 절대경로명 삽입
		}
		else if (pos_GB_extension != std::wstring::npos)
		{
			// GB File
			pDlg->AddGBFileList(filePath.c_str()); // 컨테이너에 삽입 시 해당 리소스 존재하는 절대경로명 삽입
		}
		else
		{
			// 어떤 확장자에도 해당하지 않는경우
			MessageBox(_T("gx 또는 gb 파일이 아닙니다. 올바른 파일을 추가해주세요"), _T("알림"), MB_OK);
			return;
		}

		// 리스트 박스에 파일명 추가
		pListBox->AddString(realFileName.c_str());
	}

	// 리스트 내 파일 갯수 갱신
	pDlg->UpdateListBoxFileCount(IDC_MESH_FILE_LISTBOX);

	::DragFinish(hDropInfo);

	CListCtrl::OnDropFiles(hDropInfo);
}

//
// End - CMeshListCtrl message handlers
//


