#include "CustomFileUtils.h"
#include "platform/CCCommon.h"
#include <Shlobj.h>
#include <cstdlib>

NS_CC_BEGIN

//using namespace std;

#define CC_MAX_PATH  512

// The root path of resources, the character encoding is UTF-8.
// UTF-8 is the only encoding supported by cocos2d-x API.
static std::string s_resourcePath = "";

// D:\aaa\bbb\ccc\ddd\abc.txt --> D:/aaa/bbb/ccc/ddd/abc.txt
static inline std::string convertPathFormatToUnixStyle(const std::string& path)
{
    std::string ret = path;
    int len = ret.length();
    for (int i = 0; i < len; ++i)
    {
        if (ret[i] == '\\')
        {
            ret[i] = '/';
        }
    }
    return ret;
}

static void _checkPath()
{
    if (0 == s_resourcePath.length())
    {
        WCHAR *pUtf16ExePath = nullptr;
        _get_wpgmptr(&pUtf16ExePath);

        // We need only directory part without exe
        WCHAR *pUtf16DirEnd = wcsrchr(pUtf16ExePath, L'\\');

        char utf8ExeDir[CC_MAX_PATH] = { 0 };
        int nNum = WideCharToMultiByte(CP_UTF8, 0, pUtf16ExePath, pUtf16DirEnd-pUtf16ExePath+1, utf8ExeDir, sizeof(utf8ExeDir), nullptr, nullptr);

        s_resourcePath = convertPathFormatToUnixStyle(utf8ExeDir);
    }
}

FileUtilsWin32_mod::FileUtilsWin32_mod()
{
}

bool FileUtilsWin32_mod::init()
{
    _checkPath();
    _defaultResRootPath = s_resourcePath;
    return FileUtils::init();
}

bool FileUtilsWin32_mod::isFileExistInternal(const std::string& strFilePath) const
{
    if (0 == strFilePath.length())
    {
        return false;
    }
    
    std::string strPath = strFilePath;
    if (!isAbsolutePath(strPath))
    { // Not absolute path, add the default root path at the beginning.
        strPath.insert(0, _defaultResRootPath);
    }

    WCHAR utf16Buf[CC_MAX_PATH] = {0};
    MultiByteToWideChar(CP_UTF8, 0, strPath.c_str(), -1, utf16Buf, sizeof(utf16Buf)/sizeof(utf16Buf[0]));

    DWORD attr = GetFileAttributesW(utf16Buf);
    if(attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY))
        return false;   //  not a file
    return true;
}

bool FileUtilsWin32_mod::isAbsolutePath(const std::string& strPath) const
{
    if (   (strPath.length() > 2 
        && ( (strPath[0] >= 'a' && strPath[0] <= 'z') || (strPath[0] >= 'A' && strPath[0] <= 'Z') )
        && strPath[1] == ':') || (strPath[0] == '/' && strPath[1] == '/'))
    {
        return true;
    }
    return false;
}

static Data getData(const std::string& filename, bool forString)
{
    if (filename.empty())
    {
        return Data::Null;
    }

    unsigned char *buffer = nullptr;

    size_t size = 0;
    do
    {
        // read the file from hardware
        std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);

        WCHAR wszBuf[CC_MAX_PATH] = {0};
        MultiByteToWideChar(CP_UTF8, 0, fullPath.c_str(), -1, wszBuf, sizeof(wszBuf)/sizeof(wszBuf[0]));

        HANDLE fileHandle = ::CreateFileW(wszBuf, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, nullptr);
        CC_BREAK_IF(fileHandle == INVALID_HANDLE_VALUE);
        
        size = ::GetFileSize(fileHandle, nullptr);

        if (forString)
        {
            buffer = (unsigned char*) malloc(size + 1);
            buffer[size] = '\0';
        }
        else
        {
            buffer = (unsigned char*) malloc(size);
        }
        DWORD sizeRead = 0;
        BOOL successed = FALSE;
        successed = ::ReadFile(fileHandle, buffer, size, &sizeRead, nullptr);
        ::CloseHandle(fileHandle);

        if (!successed)
        {
            // should determine buffer value, or it will cause memory leak
            if (buffer)
            {
                free(buffer);
                buffer = nullptr;
            }    
        }
    } while (0);
    
    Data ret;

    if (buffer == nullptr || size == 0)
    {
        std::string msg = "Get data from file(";
        // Gets error code.
        DWORD errorCode = ::GetLastError();
        char errorCodeBuffer[20] = {0};
        snprintf(errorCodeBuffer, sizeof(errorCodeBuffer), "%d", errorCode);

        msg = msg + filename + ") failed, error code is " + errorCodeBuffer;
        CCLOG("%s", msg.c_str());

        if (buffer)
            free(buffer);
    }
    else
    {
        ret.fastSet(buffer, size);
    }
    return ret;
}

std::string FileUtilsWin32_mod::getStringFromFile(const std::string& filename)
{
    Data data = getData(filename, true);
	if (data.isNull())
	{
		return "";
	}

    std::string ret((const char*)data.getBytes());
    return ret;
}
    
Data FileUtilsWin32_mod::getDataFromFile(const std::string& filename)
{
    return getData(filename, false);
}

unsigned char* FileUtilsWin32_mod::getFileData(const std::string& filename, const char* mode, ssize_t* size)
{
    unsigned char * pBuffer = nullptr;
    *size = 0;
    do
    {
        // read the file from hardware
        std::string fullPath = fullPathForFilename(filename);

        WCHAR wszBuf[CC_MAX_PATH] = {0};
        MultiByteToWideChar(CP_UTF8, 0, fullPath.c_str(), -1, wszBuf, sizeof(wszBuf)/sizeof(wszBuf[0]));

        HANDLE fileHandle = ::CreateFileW(wszBuf, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, nullptr);
        CC_BREAK_IF(fileHandle == INVALID_HANDLE_VALUE);
        
        *size = ::GetFileSize(fileHandle, nullptr);

        pBuffer = (unsigned char*) malloc(*size);
        DWORD sizeRead = 0;
        BOOL successed = FALSE;
        successed = ::ReadFile(fileHandle, pBuffer, *size, &sizeRead, nullptr);
        ::CloseHandle(fileHandle);

        if (!successed)
        {
            free(pBuffer);
            pBuffer = nullptr;
        }
    } while (0);
    
    if (! pBuffer)
    {
        std::string msg = "Get data from file(";
        // Gets error code.
        DWORD errorCode = ::GetLastError();
        char errorCodeBuffer[20] = {0};
        snprintf(errorCodeBuffer, sizeof(errorCodeBuffer), "%d", errorCode);

        msg = msg + filename + ") failed, error code is " + errorCodeBuffer;
        CCLOG("%s", msg.c_str());
    }
    return pBuffer;
}

std::string FileUtilsWin32_mod::getPathForFilename(const std::string& filename, const std::string& resolutionDirectory, const std::string& searchPath) const
{
    std::string unixFileName = convertPathFormatToUnixStyle(filename);
    std::string unixResolutionDirectory = convertPathFormatToUnixStyle(resolutionDirectory);
    std::string unixSearchPath = convertPathFormatToUnixStyle(searchPath);

    return FileUtils::getPathForFilename(unixFileName, unixResolutionDirectory, unixSearchPath);
}

std::string FileUtilsWin32_mod::getFullPathForDirectoryAndFilename(const std::string& strDirectory, const std::string& strFilename) const
{
    std::string unixDirectory = convertPathFormatToUnixStyle(strDirectory);
    std::string unixFilename = convertPathFormatToUnixStyle(strFilename);
    
    return FileUtils::getFullPathForDirectoryAndFilename(unixDirectory, unixFilename);
}

std::string FileUtilsWin32_mod::getWritablePath() const
{
    if (_writablePath.length())
    {
        return _writablePath;
    }

    // Get full path of executable, e.g. c:\Program Files (x86)\My Game Folder\MyGame.exe
    char full_path[CC_MAX_PATH + 1];
    ::GetModuleFileNameA(nullptr, full_path, CC_MAX_PATH + 1);

    // Debug app uses executable directory; Non-debug app uses local app data directory
//#ifndef _DEBUG
        // Get filename of executable only, e.g. MyGame.exe
        char *base_name = strrchr(full_path, '\\');

        if(base_name)
        {
            char app_data_path[CC_MAX_PATH + 1];

            // Get local app data directory, e.g. C:\Documents and Settings\username\Local Settings\Application Data
            if (SUCCEEDED(SHGetFolderPathA(nullptr, CSIDL_LOCAL_APPDATA, nullptr, SHGFP_TYPE_CURRENT, app_data_path)))
            {
                std::string ret((char*)app_data_path);

                // Adding executable filename, e.g. C:\Documents and Settings\username\Local Settings\Application Data\MyGame.exe
                ret += base_name;

                // Remove ".exe" extension, e.g. C:\Documents and Settings\username\Local Settings\Application Data\MyGame
                ret = ret.substr(0, ret.rfind("."));

                ret += "\\";

                // Create directory
                if (SUCCEEDED(SHCreateDirectoryExA(nullptr, ret.c_str(), nullptr)))
                {
                    return convertPathFormatToUnixStyle(ret);
                }
            }
        }
//#endif // not defined _DEBUG

    // If fetching of local app data directory fails, use the executable one
    std::string ret((char*)full_path);

    // remove xxx.exe
    ret =  ret.substr(0, ret.rfind("\\") + 1);

    ret = convertPathFormatToUnixStyle(ret);

    return ret;
}














#ifndef _MSC_VER
CustomFileUtils::CustomFileUtils()
{
}
#endif

void CustomFileUtils::addAutoSearchArchive(const std::string& path)
{
	if (!this->isFileExist(path)) return;
	unzFile file = nullptr;
#ifndef _MSC_VER
	file = unzOpen(FileUtils::getInstance()->getSuitableFOpen(path).c_str());
#else
	file = unzOpen(path.c_str());
#endif
	unz_file_info file_info;
	do {
		unz_file_pos entry;
		if (unzGetFilePos(file, &entry) == UNZ_OK) {
			char filename_inzip[1024];
			if (unzGetCurrentFileInfo(file, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0) == UNZ_OK) {
				_autoSearchArchive[filename_inzip] = std::make_pair(file, entry);
			}
		}
	} while (unzGoToNextFile(file) == UNZ_OK);
}

std::string CustomFileUtils::fullPathForFilename(const std::string &filename) const
{
	auto it = _autoSearchArchive.find(filename);
	if (_autoSearchArchive.end() != it) {
		return filename;
	}
	return FileUtilsInherit::fullPathForFilename(filename);
}

unsigned char* CustomFileUtils::getFileData(const std::string& filename, const char* mode, ssize_t *size)
{
	unsigned char* ret = getFileDataFromArchive(filename, size);
	if (ret) return ret;
	return FileUtilsInherit::getFileData(filename, mode, size);
}

bool CustomFileUtils::isFileExistInternal(const std::string& strFilePath) const
{
	auto it = _autoSearchArchive.find(strFilePath);
	if (_autoSearchArchive.end() != it) {
		return true;
	}
	return FileUtilsInherit::isFileExistInternal(strFilePath);
}

bool CustomFileUtils::isDirectoryExistInternal(const std::string& dirPath) const
{
	//for (auto &it : _autoSearchArchive) {
	for (auto& p_it = _autoSearchArchive.begin(); p_it != _autoSearchArchive.end(); ++p_it)
	{
		const auto& it = *p_it;
		if (it.first.size() <= dirPath.size()) continue;
		if (!strncmp(it.first.c_str(), dirPath.c_str(), dirPath.size()) && it.first[dirPath.size()] == '/') {
			return true;
		}
	}
	return FileUtilsInherit::isDirectoryExistInternal(dirPath);
}

unsigned char* CustomFileUtils::getFileDataFromArchive(const std::string& filename, ssize_t *size)
{
	auto it = _autoSearchArchive.find(filename);
	if (_autoSearchArchive.end() != it) {
		pthread_mutex_lock(&_lock);
		if (unzGoToFilePos(it->second.first, &it->second.second) != UNZ_OK) {
			pthread_mutex_unlock(&_lock); //FIXME:added
			return nullptr;
		}
		unz_file_info fileInfo;
		if (unzGetCurrentFileInfo(it->second.first, &fileInfo, NULL, 0, NULL, 0, NULL, 0) != UNZ_OK) return nullptr;
		unsigned char *buffer = (unsigned char*)malloc(fileInfo.uncompressed_size);
		int readedSize = unzReadCurrentFile(it->second.first, buffer, static_cast<unsigned>(fileInfo.uncompressed_size));
		pthread_mutex_unlock(&_lock);
		CCASSERT(readedSize == 0 || readedSize == (int)fileInfo.uncompressed_size, "the file size is wrong");
		*size = fileInfo.uncompressed_size;
		return buffer;
	}
	return nullptr;
}

cocos2d::Data CustomFileUtils::getDataFromFile(const std::string& filename)
{
	ssize_t size;
	unsigned char* buffer = getFileDataFromArchive(filename, &size);
	if (buffer) {
		Data ret;
		ret.fastSet(buffer, size);
		return ret;
	}
	return FileUtilsInherit::getDataFromFile(filename);
}

std::string CustomFileUtils::getStringFromFile(const std::string& filename)
{
	Data data = getDataFromFile(filename);
	if (data.isNull())
		return "";

	std::string ret((const char*)data.getBytes());
	return ret;
}

NS_CC_END

cocos2d::CustomFileUtils *TVPCreateCustomFileUtils() {
	cocos2d::CustomFileUtils *ret = new cocos2d::CustomFileUtils; //FIXME:
	ret->init();
	return ret;
}

#include "StorageImpl.h"
#include "Platform.h"
static bool TVPCopyFolder(const std::string &from, const std::string &to) {
	if (!TVPCheckExistentLocalFolder(to) && !TVPCreateFolders(to)) {
		return false;
	}

	bool success = true;
	TVPListDir(from, [&](const std::string &_name, int mask) {
		if (_name == "." || _name == "..") return;
		if (!success) return;
		if (mask & S_IFREG) {
			success = TVPCopyFile(from + "/" + _name, to + "/" + _name);
		}
		else if (mask & S_IFDIR) {
			success = TVPCopyFolder(from + "/" + _name, to + "/" + _name);
		}
	});
	return success;
}

bool TVPCopyFile(const std::string &from, const std::string &to)
{
	FILE * ffrom = fopen(from.c_str(), "rb");
	if (!ffrom) { // try folder copy
		return TVPCopyFolder(from, to);
	}
	FILE * fto = fopen(to.c_str(), "wb");
	if (!fto) {
		if (ffrom) fclose(ffrom);
		return false;
	}
	const int bufSize = 1 * 1024 * 1024;
	std::vector<char> buffer; buffer.resize(bufSize);
	int readed = 0;
	while ((readed = fread(&buffer.front(), 1, bufSize, ffrom))) {
		fwrite(&buffer.front(), 1, readed, fto);
	}
	fclose(ffrom);
	fclose(fto);
	return true;
}
