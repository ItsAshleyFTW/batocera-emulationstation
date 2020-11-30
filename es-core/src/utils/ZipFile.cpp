#include "ZipFile.h"
#include <time.h>
#include "LocaleES.h"

#if WIN32
#include <Windows.h>
#endif

#include <functional>
#include <iterator>
#include "zip_file.hpp"

namespace Utils
{
	namespace Zip
	{
		unsigned int ZipFile::computeCRC(unsigned int crc, const void* ptr, size_t buf_len)
		{
			return (mz_uint32)mz_crc32((mz_uint32)crc, (const mz_uint8 *)ptr, buf_len);
		}

		ZipFile::ZipFile()
		{
			mZipFile = nullptr;
		}

		ZipFile::~ZipFile()
		{
			if (mZipFile != nullptr)
				delete mZipFile;
		}

		bool ZipFile::load(const std::string &filename)
		{
			try
			{
				if (mZipFile != nullptr)
					delete mZipFile;

				miniz_cpp::zip_file* pFile = new miniz_cpp::zip_file();
				pFile->load(filename);

				mZipFile = pFile;
			}
			catch (...)
			{

			}

			return mZipFile != nullptr;
		}

		bool ZipFile::extract(const std::string &member, const std::string &path)
		{
			if (mZipFile == nullptr)
				return false;

			try
			{
				miniz_cpp::zip_file* pFile = (miniz_cpp::zip_file*) mZipFile;
				pFile->extract(member, path);
			}
			catch (...)
			{
				return false;
			}

			return true;
		}

		struct OpaqueWrapper
		{
			zip_callback callback;
			void* opaque;
		};

		void ZipFile::readBuffered(const std::string &name, zip_callback pCallback, void* pOpaque)
		{
			if (mZipFile == nullptr)
				return;

			OpaqueWrapper w;
			w.opaque = pOpaque;
			w.callback = pCallback;

			mz_file_write_func func = [](void *pOpaque, mz_uint64 file_ofs, const void *pBuf, size_t n) 
			{ 
				OpaqueWrapper* wrapper = (OpaqueWrapper*)pOpaque;
				return wrapper->callback(wrapper->opaque, file_ofs, pBuf, n);
			};

			miniz_cpp::zip_file* pFile = (miniz_cpp::zip_file*) mZipFile;

			pFile->read(name, func, &w);
		}

		std::vector<std::string> ZipFile::namelist()
		{
			return ((miniz_cpp::zip_file*) mZipFile)->namelist();
		}

		std::string ZipFile::getFileCrc(const std::string &name)
		{
			if (mZipFile == nullptr)
				return "" ;

			auto info = ((miniz_cpp::zip_file*) mZipFile)->getinfo(name);

			char hex[10];
			auto len = snprintf(hex, sizeof(hex) - 1, "%08X", info.crc);
			hex[len] = 0;

			return hex;
		}

	} // Zip::

} // Utils::
