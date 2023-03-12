#include <efi.h>
#include <efilib.h>
// #include <elf.h>
#include <elf.h>

EFI_FILE_HANDLE getVolume(EFI_HANDLE image) {
    EFI_GUID lipGUID = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_LOADED_IMAGE *loadedImage = NULL;

    // Get image file from handle
    uefi_call_wrapper(BS->HandleProtocol, 3, image, &lipGUID, (void**) &loadedImage);

    // Get volume from the image handle
    return LibOpenRoot(loadedImage->DeviceHandle);
}

int memcmp(const void* a, const void* b, UINT64 n) {
    const unsigned char *x = a, *y = b;
    for (UINT64 i = 0; i < n; i++) {
        if (x[i] < y[i]) return -1;
        if (x[i] > y[i]) return 1;
    }
    return 0;
}

BOOLEAN validateElfHeader(Elf64_Ehdr *header) {
    return memcmp(&header->e_ident[EI_MAG0], ELFMAG0, SELFMAG) &&    // Magic bytes match
    header->e_ident[EI_CLASS] == ELFCLASS64 &&                      // 64-bit elf
    header->e_ident[EI_DATA] == ELFDATA2LSB &&                      // Using 2's complement w/ LSB ordering
    header->e_type == ET_EXEC &&                                    // Elf is executable
    header->e_machine == 62 &&                                      // Platform is EN_AMD64 (define not in elf.h header anymore for some reason)
    header->e_version == EV_CURRENT;                                // File's elf version same as header's elf version
}

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);
    Print(L"Starting pamOS bootloader\n");
    ST = SystemTable;

    // Get image handle
    EFI_FILE_HANDLE volume = getVolume(ImageHandle);

    // Open file from handle
    CHAR16 *fileName = L"kernel.elf";
    EFI_FILE_HANDLE kernelFileHandle;
    EFI_STATUS status = uefi_call_wrapper(volume->Open, 5, volume, &kernelFileHandle, fileName, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);
    if (EFI_ERROR(status)) {
        Print(L"Error loading kernel.elf\r\n");
        return status;
    }

    // Read elf header
    Elf64_Ehdr elfHeader;
    UINT64 headerSize = sizeof(elfHeader);
    status = uefi_call_wrapper(kernelFileHandle->Read, 3, kernelFileHandle, &headerSize, &elfHeader);  // Read from file into struct
    if (EFI_ERROR(status)) {
        Print(L"Unable to read elf header.\r\n");
        return status;
    }

    // Validate elf header
    if (!validateElfHeader(&elfHeader)) {
        Print(L"Kernel has an invalid elf header.\r\n");
        return 1;
    }
    Print(L"Kernel file has been validated.\r\n");

    // File is valid elf, load program headers
    Elf64_Phdr *programHeaders;
    uefi_call_wrapper(kernelFileHandle->SetPosition, 2, kernelFileHandle, elfHeader.e_phoff);  // Set cursor to start of phdr
    UINT64 phdrsSize = elfHeader.e_phentsize * elfHeader.e_phnum;  // Phdr size * # of phdrs
    programHeaders = AllocatePool(phdrsSize);  // malloc
    uefi_call_wrapper(kernelFileHandle->Read, 3, kernelFileHandle, &phdrsSize, programHeaders);

    // Load binary for each phdr
    for (Elf64_Phdr *currHeader = programHeaders; (char*)currHeader < (char*)programHeaders + phdrsSize; currHeader = (Elf64_Phdr*)((char*)currHeader + elfHeader.e_phentsize)) {
        if (currHeader->p_type != PT_LOAD) continue;  // Not a loadable segment, ignore
        int pageCount = (currHeader->p_memsz + 0x1000 - 1) / 0x1000;  // Round up to nearest multiple of 4096 (4096 bytes in a page, get whole number of pages)
        Elf64_Addr segmentAddr = currHeader->p_paddr;
        uefi_call_wrapper(BS->AllocatePages, 4, AllocateAddress, EfiLoaderData, pageCount, &segmentAddr);  // Allocate pages at address expected by segment

        uefi_call_wrapper(kernelFileHandle->SetPosition, 2, kernelFileHandle, currHeader->p_offset);  // Set cursor at start of segment data
        UINT64 size = currHeader->p_filesz;  // Number of data bytes (segment could be 100 bytes, but only 80 bytes actually have data)
        uefi_call_wrapper(kernelFileHandle->Read, 3, kernelFileHandle, &size, (void*)segmentAddr);  // Read data into allocated pages
    }

    // Get memory map
    EFI_MEMORY_DESCRIPTOR *map;
    UINT64 mapEntryCount, mapKey, descriptorSize;
    UINT32 descriptorVersion;
    map = LibMemoryMap(&mapEntryCount, &mapKey, &descriptorSize, &descriptorVersion);

    // Locate GOP
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    EFI_GUID gopGUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGUID, NULL, (void**)&gop);
    if (EFI_ERROR(status)) {
        Print(L"Unable to locate the GOP.\r\n");
        return status;
    }

    Print(L"DEBUG: Frame buffer: 0x%x\r\n", &gop->Mode->FrameBufferBase);
    Print(L"DEBUG: Frame horizontal: %d\r\n", gop->Mode->Info->HorizontalResolution);
    Print(L"DEBUG: Frame vertical: %d\r\n", gop->Mode->Info->VerticalResolution);
    Print(L"DEBUG: Frame PPSL: %d\r\n", gop->Mode->Info->PixelsPerScanLine);


    Print(L"Press any key to enter the kernel.\r\n");

    // Clear input buffer
    uefi_call_wrapper(ST->ConIn->Reset, 2, ST->ConIn, FALSE);

    // Block until key press received
    EFI_INPUT_KEY key;
    while ((status = uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2, ST->ConIn, &key)) == EFI_NOT_READY);

    Print(L"Entering kernel...\r\n");

    // Exit boot services
    uefi_call_wrapper(BS->ExitBootServices, 2, ImageHandle, mapKey);

    int (*kernelEntry)() = ((__attribute__((sysv_abi)) int(*)(UINT64, UINT32, UINT32, UINT32))elfHeader.e_entry);
    kernelEntry(gop->Mode->FrameBufferBase, gop->Mode->Info->HorizontalResolution, gop->Mode->Info->VerticalResolution, gop->Mode->Info->PixelsPerScanLine);

    Print(L"Exited kernel.\r\n");
    for(;;);
    return EFI_SUCCESS;
}
