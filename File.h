#pragma once

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Guid/FileInfo.h>
#include <Protocol/SimpleFileSystem.h>

EFI_STATUS GetFileHandle(
	EFI_HANDLE ImageHandle,
	CHAR16* FileName,
	EFI_FILE_PROTOCOL** File
);

EFI_STATUS ReadFile(
	EFI_FILE_PROTOCOL* File,
	EFI_PHYSICAL_ADDRESS* FileBase
);