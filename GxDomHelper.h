#pragma once

#include <string>
#include <tchar.h>
#include <gx.h>
#include <vector>
#include <set>

namespace g3d
{

}
#define _GX_STR(x) _GX_VAL(x)
#define _GX_VAL(x) L#x

//
// class GxException
//
class GxException
{
public:
	GxException() {}
	GxException(TCHAR const* format, ...);

	//void attachProgressState();
	void attachSourceString(std::wstring sourceString);
	void attachLineString(const std::wstring& lineString);
	void attachCompileDateString(const std::wstring& compileDateString);

public:
	std::wstring errorMessage_;
};

#define GX_EXCEPTION_STAMP( exception ) \
	{ \
	exception.errorMessage_ += _T("\n") ; \
	exception.errorMessage_ += _T("\n") ; \
	exception.attachSourceString( _GX_STR(__FILE__) ) ; \
	exception.attachLineString( _GX_STR(__LINE__) ) ; \
	exception.attachCompileDateString( _GX_STR(__DATE__) ) ; \
	}


#define GX_THROW_EXCEPTION( msg ) \
	{ \
	GxException exception( msg ) ; \
	GX_EXCEPTION_STAMP( exception ) ; \
	throw exception ; \
	}

#define GX_NULL_POINTER_CHECK( p ) \
	{ \
	if ( !p ) GX_THROW_EXCEPTION( _T("\"" #p "\" is null pointer.") ) ; \
	}

#define GX_EXPRESSION_CHECK( p ) \
	{ \
	if ( !( p ) ) GX_THROW_EXCEPTION( _T("\"" #p "\" is false.") ) ; \
	}

#ifndef __GRAN3D_MAX_PLUGIN_GX_DOM_HELPER_H__
#define __GRAN3D_MAX_PLUGIN_GX_DOM_HELPER_H__


//
// namespace g3d
//



namespace g3d
{
	//
	// class GxDomWrapper
	//
	class GxDomWrapper
	{
	public:
		GxDomWrapper();
		virtual ~GxDomWrapper();

		bool isValid() const { return gxDom_ != 0; }
		gx::Group* get() const { return gxDom_; }

		bool load(const std::wstring& fullPath);
		bool save(const std::wstring& fullPath, bool binary = false);

		const std::wstring& getErrorString() const { return errorString_; }

	protected:
		gx::Group* gxDom_;
		std::wstring errorString_;
	};

	gx::Attr* makeAttr(const std::string& key, const std::string& value);
	gx::Attr* makeAttr(const std::string& key, bool value);
	gx::Attr* makeAttr(const std::string& key, int value);
	gx::Attr* makeAttr(const std::string& key, float value);

	// typedef
	typedef const gx::gxGroupVector GxDomGroupVec;
	typedef const gx::gxMaterialVector GxDomMaterialVec;
	typedef const gx::gxGeometryDataVector GxDomGeometryVec;
	typedef const gx::gxLightDataVector GxDomLightVec;
	typedef const gx::gxCameraDataVector GxDomCameraVec;
	typedef const gx::gxAttrVector GxDomAttrVec;
	typedef const gx::gxBindCtrlerVector GxDomBindCtrlVec;
	typedef const gx::gxNoteTrackVector GxDomNoteTrackVec;
	
}

#endif // __GRAN3D_MAX_PLUGIN_GX_DOM_HELPER_H__

// To do...
// gx 파일 gx::dom 이용하여 파싱하는 클래스 (맥스 플러그인 테스트용)
class CGxDomHelper
{
public:
	// To do...
	// CC Max Plugin GxImportExport 프로젝트 내
	// GxDomHelper.h, cpp 파일 참고
	// 
};