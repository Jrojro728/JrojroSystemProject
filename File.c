#include "File.h"

EFI_STATUS GetFileHandle(EFI_HANDLE ImageHandle, CHAR16* FileName, EFI_FILE_PROTOCOL** File)
{
	EFI_STATUS Status = EFI_SUCCESS;

	UINTN HandleCount = 0;
	EFI_HANDLE* HandleBuffer = NULL;
	Status = gBS->LocateHandleBuffer(
		ByProtocol,
		&gEfiSimpleFileSystemProtocolGuid,
		NULL,
		&HandleCount,
		&HandleBuffer
	);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	Status = gBS->OpenProtocol(
		HandleBuffer[0],
		&gEfiSimpleFileSystemProtocolGuid,
		(void**)&FileSystem,
		ImageHandle,
		NULL,
		EFI_OPEN_PROTOCOL_GET_PROTOCOL
	);
	
	EFI_FILE_PROTOCOL* Root;
	Status = FileSystem->OpenVolume(
		FileSystem,
		&Root
	);

	Root->Open(
		Root,
		File,
		FileName,
		EFI_FILE_MODE_WRITE | EFI_FILE_MODE_READ,
		0
	);

	return Status;
}

EFI_STATUS ReadFile(EFI_FILE_PROTOCOL* File, EFI_PHYSICAL_ADDRESS* FileBase)
{
	EFI_STATUS Status = EFI_SUCCESS;
	EFI_FILE_INFO* FileInfo;

	UINTN InfoSize = sizeof(EFI_FILE_INFO) + 128;
	Status = gBS->AllocatePool(
		EfiLoaderData,
		InfoSize,
		(VOID**)&FileInfo
	);

	Status = File->GetInfo(
		File,
		&gEfiFileInfoGuid,
		&InfoSize,
		FileInfo
	);

	UINTN FilePageSize = (FileInfo->FileSize >> 12) + 1;

	EFI_PHYSICAL_ADDRESS FileBufferAddress;
	Status = gBS->AllocatePages(
		AllocateAnyPages,
		EfiLoaderData,
		FilePageSize,
		&FileBufferAddress
	);

	UINTN ReadSize = FileInfo->FileSize;
	Status = File->Read(
		File,
		&ReadSize,
		(VOID*)FileBufferAddress
	);

	gBS->FreePool(FileInfo);
	*FileBase = FileBufferAddress;

	return Status;
}