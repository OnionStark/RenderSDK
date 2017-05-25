#include "Common/FileUtils.h"

i64 GetFileSizeInBytes(HANDLE hFile)
{
	LARGE_INTEGER sizeInBytes;
	VerifyWinAPIResult(GetFileSizeEx(hFile, &sizeInBytes));
	return sizeInBytes.QuadPart;
}

bool LoadDataFromFile(const wchar_t* pFilePath, FileMode fileMode, std::vector<char>& loadedData)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	const DWORD numberOfBytesToRead = (DWORD)GetFileSizeInBytes(hFile);
	
	if (fileMode == FileMode::Text)
		loadedData.resize(numberOfBytesToRead + 1, '\0');
	else
	{
		assert(fileMode == FileMode::Binary);
		loadedData.resize(numberOfBytesToRead, '0');
	}
	
	DWORD numberOfBytesRead = 0;
	VerifyWinAPIResult(ReadFile(hFile, &loadedData[0], numberOfBytesToRead, &numberOfBytesRead, nullptr));
	VerifyWinAPIResult(CloseHandle(hFile));
	
	if (fileMode == FileMode::Text)
	{
		assert(numberOfBytesRead <= numberOfBytesToRead);
		loadedData.resize(numberOfBytesRead + 1);
	}
	else
	{
		assert(fileMode == FileMode::Binary);
		assert(numberOfBytesRead == numberOfBytesToRead);
	}
	return true;
}
