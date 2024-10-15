#include "GxDomHelper.h"

// To do...


#include "GxDomHelper.h"
//#include "Common/GxException.h"
//#include "Common/GxMathConverter.h"

//
// class GxException
//
GxException::GxException(TCHAR const* format, ...)
{
	TCHAR errorMessageOutput[1024];

	va_list va;
	va_start(va, format);
	_vsntprintf(errorMessageOutput, 1024 * 2 - 1, format, va);
	va_end(va);

	errorMessage_ = errorMessageOutput;
}

void GxException::attachSourceString(std::wstring sourceString)
{
	if (!sourceString.empty())
	{
		if (sourceString.find('\\') != std::wstring::npos)
		{
			sourceString = _T("\"") + sourceString.substr(
				sourceString.rfind('\\') + 1, std::wstring::npos);
		}
		errorMessage_ += (_T("\nSource : ") + sourceString);
	}
}

void GxException::attachLineString(const std::wstring& lineString)
{
	if (!lineString.empty())
	{
		errorMessage_ += (_T("\nLine : ") + lineString);
	}
}

void GxException::attachCompileDateString(const std::wstring& compileDateString)
{
	if (!compileDateString.empty())
	{
		errorMessage_ += (_T("\nCompile Date : ") + compileDateString);
	}
}

//
// namespace g3d
//

namespace g3d
{
	// util function
	std::string toString(int value)
	{
		char temp[32];
		sprintf(temp, "%d", temp);
		return temp;
	}

	class GxDomStreamFileLoader : public gx::StreamFileLoader
	{
	public:
		GxDomStreamFileLoader()
		{

		}
		~GxDomStreamFileLoader()
		{
			for (unsigned int i = 0; i < data_.size(); ++i)
				delete[] data_[i];
		}
		virtual bool load(const std::string& strFile, const char*& stream, size_t& size)
		{
			FILE* pFile = fopen(strFile.c_str(), "rb");
			gx::ScopedFileCloser fileCloser(pFile);

			// printf( "load : %s\n", path.c_str() );

			if (pFile)
			{
				// read file into memory
				fseek(pFile, 0, SEEK_END);
				const long endPos = ftell(pFile);
				fseek(pFile, 0, SEEK_SET);
				size_t fileSize = static_cast<size_t>(endPos);

				char* data = new char[fileSize];
				fread(data, 1, fileSize, pFile);

				stream = data;
				size = fileSize;

				data_.push_back(data);

				return true;
			}

			return false;
		}

		virtual void onLoadFailed(const std::wstring& url, gx::ELoadResult)
		{
			failedUrls_.push_back(url);
		}

	public:
		std::vector<std::wstring> failedUrls_;

	private:
		std::vector< char* > data_;
	};

	GxDomWrapper::GxDomWrapper()
		: gxDom_(gx::newObject< gx::Group >())
	{
		if (gxDom_) gxDom_->grab();
	}

	GxDomWrapper::~GxDomWrapper()
	{
		if (gxDom_)
		{
			int result = gxDom_->drop();
			GX_EXPRESSION_CHECK(result == 0);
		}
		gxDom_ = 0;
	}

	bool GxDomWrapper::load(const std::wstring& fullPath)
	{
		if (!isValid())
		{
			errorString_ = TEXT("gx dom is not valid.");
			return false;
		}

		if (gxDom_)
		{
			GxDomStreamFileLoader loader;

			gx::XmlStream xmlStream;

			std::string fullPath_a;
			fullPath_a.assign(fullPath.begin(), fullPath.end());

			gx::ELoadResult result = xmlStream.load(gxDom_, fullPath_a, NULL, &loader);
			if (gx::LoadResult_Succeed == result)
			{
				errorString_.clear();
				return true;
			}
			else
			{
				switch (result)
				{
				case gx::LoadResult_LoadFileFail:
					errorString_ = _T("LoadFileFail.");
					break;
				case gx::LoadResult_IncompleteBinary:
					errorString_ = _T("IncompleteBinary.");
					break;
				case gx::LoadResult_XmlLoadFail:
					errorString_ = _T("XmlLoadFail.");
					break;
				case gx::LoadResult_VersionMismatch:
					errorString_ = _T("VersionMismatch.");
					break;
				case gx::LoadResult_UpdateFail:
					errorString_ = _T("UpdateFail.");
					break;
				case gx::LoadResult_ParseFail:
					errorString_ = _T("ParseFail.");
					break;
				default:
					errorString_ = _T("gx dom load failed.");
					break;
				}

				if (result == gx::LoadResult_ParseFail)
				{
					if (!loader.failedUrls_.empty())
					{
						errorString_ += _T("couldn't resolve template\n");
						const size_t numUrl = loader.failedUrls_.size();
						for (size_t i = 0; i < numUrl; ++i)
						{
							errorString_ += loader.failedUrls_[i];
							errorString_ += _T("\n");
						}
					}
					return true;
				}

				return false;
			}
		}

		errorString_ = _T("unknown fail.");

		return false;
	}

	bool GxDomWrapper::save(const std::wstring& fullPath, bool binary)
	{
		if (!isValid())
		{
			errorString_ = _T("gx dom is not valid.");
			return false;
		}

		if (gxDom_)
		{
			gx::XmlStream xmlStream;

			std::string fullPath_a;
			fullPath_a.assign(fullPath.begin(), fullPath.end());

			if (S_OK == xmlStream.save(gxDom_, fullPath_a, binary))
			{
				errorString_.clear();
				return true;
			}
			else
			{
				errorString_ = _T("gx dom save failed.");
				return false;
			}
		}

		errorString_ = _T("unknown fail.");

		return false;
	}

	gx::Attr* makeAttr(const std::string& key, const std::string& value)
	{
		gx::Attr* gxDomAttr = gx::newObject< gx::Attr >();
		GX_NULL_POINTER_CHECK(gxDomAttr);
		gxDomAttr->setName(key);
		gxDomAttr->setValue(value);
		return gxDomAttr;
	}

	gx::Attr* makeAttr(const std::string& key, int value)
	{
		return makeAttr(key, toString(value));
	}

	gx::Attr* makeAttr(const std::string& key, float value)
	{
		return makeAttr(key, toString(value));
	}

	gx::Attr* makeAttr(const std::string& key, bool value)
	{
		return makeAttr(key, toString(value));
	}
}