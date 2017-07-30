#pragma once
#include <string>
#include <cstdlib>
#include "Shlwapi.h"

namespace CONSTANTS {
	static const size_t MAX_LOADSTRING = 256;
	static const size_t INT_TO_STR_BUFF_SIZE = 16;

}


class IniHelper
{
public:
	IniHelper();
	virtual ~IniHelper();
	void setFile(const std::string& fileName);
	void setFile(const std::wstring& fileName);

	/*BOOL isFileExists(const std::string& fileName);
	BOOL isFileExists(const std::wstring& fileName);
	BOOL isFileExistsA();
	BOOL isFileExistsW();*/

	void writePrivateProfileString(const std::string& appName, const std::string& key, const std::string& value);
	void writePrivateProfileString(const std::wstring& appName, const std::wstring& key, const std::wstring& value);
	std::string getPrivateProfileString(const std::string& appName, const std::string& key, const std::string& defaultValue);
	std::wstring getPrivateProfileString(const std::wstring& appName, const std::wstring& key, const std::wstring& defaultValue);

	void writeProfileString(const std::string& appName, const std::string& key, const std::string& value);
	void writeProfileString(const std::wstring& appName, const std::wstring& key, const std::wstring& value);
	std::string getProfileString(const std::string& appName, const std::string& key, const std::string& defaultValue);
	std::wstring getProfileString(const std::wstring& appName, const std::wstring& key, const std::wstring& defaultValue);

	void writePrivateProfileInt(const std::string& appName, const std::string& key, INT value);
	void writePrivateProfileInt(const std::wstring& appName, const std::wstring& key, INT value);
	UINT getPrivateProfileInt(const std::string& appName, const std::string& key, INT defaultValue);
	UINT getPrivateProfileInt(const std::wstring& appName, const std::wstring& key, INT defaultValue);

	void writeProfileInt(const std::string& appName, const std::string& key, INT value);
	void writeProfileInt(const std::wstring& appName, const std::wstring& key, INT value);
	UINT getProfileInt(const std::string& appName, const std::string& key, INT defaultValue);
	UINT getProfileInt(const std::wstring& appName, const std::wstring& key, INT defaultValue);

	void writePrivateProfileFloat(const std::string& appName, const std::string& key, float value);
	void writePrivateProfileFloat(const std::wstring& appName, const std::wstring& key, float value);
	float getPrivateProfileFloat(const std::string& appName, const std::string& key, float defaultValue);
	float getPrivateProfileFloat(const std::wstring& appName, const std::wstring& key, float defaultValue);


private:
	std::string _aFileName;
	std::wstring _wFileName;
};

