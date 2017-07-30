#include "stdafx.h"
#include "IniHelper.h"

IniHelper::IniHelper()
{
}

IniHelper::~IniHelper()
{
}

void IniHelper::setFile(const std::string& fileName)
{
	_aFileName = fileName;
}
void IniHelper::setFile(const std::wstring& fileName)
{
	_wFileName = fileName;
}

//BOOL IniHelper::isFileExists(const std::string& fileName)
//{
//	return PathFileExistsA(fileName.c_str());
//}
//
//BOOL IniHelper::isFileExists(const std::wstring& fileName)
//{
//	return PathFileExistsW(fileName.c_str());
//}
//
//BOOL IniHelper::isFileExistsA()
//{
//	return isFileExists(_aFileName);
//}
//
//BOOL IniHelper::isFileExistsW()
//{
//	return isFileExists(_wFileName);
//}

#pragma region PrivateProfileString
void IniHelper::writePrivateProfileString(const std::string& appName, const std::string& key, const std::string& value)
{
	WritePrivateProfileStringA(appName.c_str(), key.c_str(), value.c_str(), _aFileName.c_str());
}

void IniHelper::writePrivateProfileString(const std::wstring& appName, const std::wstring& key, const std::wstring& value)
{
	WritePrivateProfileStringW(appName.c_str(), key.c_str(), value.c_str(), _wFileName.c_str());
}

std::string IniHelper::getPrivateProfileString(const std::string& appName, const std::string& key, const std::string& defaultValue)
{
	std::string buf;
	buf.reserve(CONSTANTS::MAX_LOADSTRING);
	char* ptr = &buf[0];
	//char* ptr = buf.data();
	GetPrivateProfileStringA(appName.c_str(),key.c_str(), defaultValue.c_str(), ptr, CONSTANTS::MAX_LOADSTRING, _aFileName.c_str());
	return buf;
}

std::wstring IniHelper::getPrivateProfileString(const std::wstring& appName, const std::wstring& key, const std::wstring& defaultValue)
{
	std::wstring buf;
	buf.reserve(CONSTANTS::MAX_LOADSTRING);
	wchar_t* ptr = &buf[0];
	//wchar_t* ptr = buf.data();
	GetPrivateProfileStringW(appName.c_str(), key.c_str(), defaultValue.c_str(), ptr, CONSTANTS::MAX_LOADSTRING, _wFileName.c_str());
	return buf;
}
#pragma endregion

#pragma region ProfileString
void IniHelper::writeProfileString(const std::string& appName, const std::string& key, const std::string& value)
{
	WriteProfileStringA(appName.c_str(), key.c_str(), value.c_str());
}

void IniHelper::writeProfileString(const std::wstring& appName, const std::wstring& key, const std::wstring& value)
{
	WriteProfileStringW(appName.c_str(), key.c_str(), value.c_str());
}

std::string IniHelper::getProfileString(const std::string& appName, const std::string& key, const std::string& defaultValue)
{
	std::string buf;
	buf.reserve(CONSTANTS::MAX_LOADSTRING);
	char* ptr = &buf[0];
	//char* ptr = buf.data();
	GetProfileStringA(appName.c_str(), key.c_str(), defaultValue.c_str(), ptr, CONSTANTS::MAX_LOADSTRING);
	return buf;
}

std::wstring IniHelper::getProfileString(const std::wstring& appName, const std::wstring& key, const std::wstring& defaultValue)
{
	std::wstring buf;
	buf.reserve(CONSTANTS::MAX_LOADSTRING);
	wchar_t* ptr = &buf[0];
	//wchar_t* ptr = buf.data();
	GetProfileStringW(appName.c_str(), key.c_str(), defaultValue.c_str(), ptr, CONSTANTS::MAX_LOADSTRING);
	return buf;
}
#pragma endregion

#pragma region PrivateProfileInt
void IniHelper::writePrivateProfileInt(const std::string& appName, const std::string& key, INT value)
{
	char buf[CONSTANTS::INT_TO_STR_BUFF_SIZE];
	sprintf_s(buf, "%d", value);
	WritePrivateProfileStringA(appName.c_str(), key.c_str(), buf, _aFileName.c_str());
}

void IniHelper::writePrivateProfileInt(const std::wstring& appName, const std::wstring& key, INT value)
{
	wchar_t buf[CONSTANTS::INT_TO_STR_BUFF_SIZE];
	swprintf(buf, L"%d", value);
	WritePrivateProfileStringW(appName.c_str(), key.c_str(), buf, _wFileName.c_str());
}

UINT IniHelper::getPrivateProfileInt(const std::string& appName, const std::string& key, INT defaultValue)
{
	return GetPrivateProfileIntA(appName.c_str(), key.c_str(), defaultValue, _aFileName.c_str());
}

UINT IniHelper::getPrivateProfileInt(const std::wstring& appName, const std::wstring& key, INT defaultValue)
{
	return GetPrivateProfileIntW(appName.c_str(), key.c_str(), defaultValue, _wFileName.c_str());
}
#pragma endregion

#pragma region ProfileInt
void IniHelper::writeProfileInt(const std::string& appName, const std::string& key, INT value)
{
	throw std::logic_error("Function not yet implemented");
}

void IniHelper::writeProfileInt(const std::wstring& appName, const std::wstring& key, INT value)
{
	throw std::logic_error("Function not yet implemented");
}

UINT IniHelper::getProfileInt(const std::string& appName, const std::string& key, INT defaultValue)
{
	throw std::logic_error("Function not yet implemented");
	return defaultValue;
}

UINT IniHelper::getProfileInt(const std::wstring& appName, const std::wstring& key, INT defaultValue)
{
	throw std::logic_error("Function not yet implemented");
	return defaultValue;
}
#pragma endregion

#pragma region PrivateProfileFloat
void IniHelper::writePrivateProfileFloat(const std::string& appName, const std::string& key, float value)
{
	char buf[CONSTANTS::INT_TO_STR_BUFF_SIZE];
	sprintf_s(buf, "%f", value);
	WritePrivateProfileStringA(appName.c_str(), key.c_str(), buf, _aFileName.c_str());
}

void IniHelper::writePrivateProfileFloat(const std::wstring& appName, const std::wstring& key, float value)
{
	wchar_t buf[CONSTANTS::INT_TO_STR_BUFF_SIZE];
	swprintf(buf, L"%f", value);
	WritePrivateProfileStringW(appName.c_str(), key.c_str(), buf, _wFileName.c_str());
}

float IniHelper::getPrivateProfileFloat(const std::string& appName, const std::string& key, float defaultValue)
{
	char out[CONSTANTS::INT_TO_STR_BUFF_SIZE];
	char in[CONSTANTS::INT_TO_STR_BUFF_SIZE];
	sprintf(in, "%f", defaultValue);
	GetPrivateProfileStringA(appName.c_str(), key.c_str(), in, out, CONSTANTS::MAX_LOADSTRING, _aFileName.c_str());
	float value;
	char* pEnd;
	value = strtof(out, &pEnd);
	return value;
}

float IniHelper::getPrivateProfileFloat(const std::wstring& appName, const std::wstring& key, float defaultValue)
{
	wchar_t out[CONSTANTS::INT_TO_STR_BUFF_SIZE];
	wchar_t in[CONSTANTS::INT_TO_STR_BUFF_SIZE];
	swprintf(in, L"%f", defaultValue);
	GetPrivateProfileStringW(appName.c_str(), key.c_str(), in, out, CONSTANTS::MAX_LOADSTRING, _wFileName.c_str());
	float value;
	wchar_t* pEnd;
	value = wcstof(out, &pEnd);
	return value;
}
#pragma endregion
