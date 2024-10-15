
// GXMeshTexture_Converter_Tool.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include <vector>
#include <string>
#include <algorithm>

// CGXMeshTextureConverterToolApp:
// See GXMeshTexture_Converter_Tool.cpp for the implementation of this class
//

//
// Main Dialog
//
class CGXMeshTextureConverterToolApp : public CWinApp
{
public:
	CGXMeshTextureConverterToolApp();

// Overrides
public:
	virtual BOOL InitInstance();
	
// Implementation
	DECLARE_MESSAGE_MAP()
};

extern CGXMeshTextureConverterToolApp theApp;
