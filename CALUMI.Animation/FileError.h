#pragma once

#ifdef CALUMIANIMATION_EXPORTS // This macro is typically defined by Visual Studio for DLL projects
#define CALUMIANIMATION_API __declspec(dllexport)
#else
#define CALUMIANIMATION_API __declspec(dllimport)
#endif

#include <string>
#include <filesystem>
#include <print>
#include <format>

enum CALUMIANIMATION_API FileErrorCode
{
	FileNotFound,
	PermissionDenied,
	NotAFile,
	ReadFailure,
	IncorrectFileType,
	FileTooLarge,
	FileTooSmall,
	UnknownErrorCode,
	WriteFailure
};

struct CALUMIANIMATION_API FileError
{
	FileErrorCode fileCode;
	std::filesystem::path path;
	std::string errorMessage;

	friend std::ostream& operator<<(std::ostream& os, FileError error);

	std::string ToString();
};
