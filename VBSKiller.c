#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>

STATIC CONST EFI_GUID gMicrosoftVendorGuid = {0x77fa9abd, 0x0359, 0x4d32, {0xbd, 0x60, 0x28, 0xf4, 0xe7, 0x8f, 0x78, 0x4b}};
STATIC CONST CHAR16 mVbsVarName[] = L"VbsPolicyDisabled";
#define VBS_VAR_ATTR (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS)
STATIC CONST CHAR16 mWinBootFile[] = L"BOOTMGFW.EFI";
#define WIN_BOOT_FILE_LEN 12

STATIC
BOOLEAN
IsWindowsBootFile(
    IN CHAR16 *PathName)
{
    UINTN Len;
    UINTN Index;
    CHAR16 CharA, CharB;
    if (PathName == NULL)
    {
        return FALSE;
    }
    Len = StrLen(PathName);
    if (Len < WIN_BOOT_FILE_LEN)
    {
        return FALSE;
    }
    PathName += (Len - WIN_BOOT_FILE_LEN);
    for (Index = 0; Index < WIN_BOOT_FILE_LEN; Index++)
    {
        CharA = PathName[Index];
        CharB = mWinBootFile[Index];
        if (CharA >= L'a' && CharA <= L'z')
        {
            CharA -= (L'a' - L'A');
        }
        if (CharA != CharB)
        {
            return FALSE;
        }
    }
    return TRUE;
}

STATIC
VOID DisableVBS(
    VOID)
{
    EFI_STATUS Status;
    UINT8 TargetValue = 1;
    UINT8 CurrentValue = 0;
    UINTN Size = sizeof(CurrentValue);
    Status = gRT->GetVariable((CHAR16 *)mVbsVarName, (EFI_GUID *)&gMicrosoftVendorGuid, NULL, &Size, &CurrentValue);
    if (!EFI_ERROR(Status) && CurrentValue == TargetValue)
    {
        return;
    }
    Status = gRT->SetVariable((CHAR16 *)mVbsVarName, (EFI_GUID *)&gMicrosoftVendorGuid, VBS_VAR_ATTR, sizeof(TargetValue), &TargetValue);
    if (EFI_ERROR(Status))
    {
        gRT->SetVariable((CHAR16 *)mVbsVarName, (EFI_GUID *)&gMicrosoftVendorGuid, 0, 0, NULL);
        Status = gRT->SetVariable((CHAR16 *)mVbsVarName, (EFI_GUID *)&gMicrosoftVendorGuid, VBS_VAR_ATTR, sizeof(TargetValue), &TargetValue);
    }
    if (EFI_ERROR(Status))
    {
        Print(L"[Error] VBS Disable failed: %r\n", Status);
    } else {
        Print(L"[Info] VBS Disable succeeded: %r\n", Status);
    }
}

STATIC
VOID BootWindows(
    VOID)
{
    EFI_BOOT_MANAGER_LOAD_OPTION *BootOptions;
    UINTN BootOptionCount;
    UINTN Index;
    EFI_DEVICE_PATH_PROTOCOL *Node;
    FILEPATH_DEVICE_PATH *FilePathNode;
    BOOLEAN Found;
    BootOptions = EfiBootManagerGetLoadOptions(&BootOptionCount, LoadOptionTypeBoot);
    if (BootOptions == NULL)
    {
        return;
    }
    for (Index = 0; Index < BootOptionCount; Index++)
    {
        if ((BootOptions[Index].Attributes & LOAD_OPTION_ACTIVE) == 0)
        {
            continue;
        }
        Found = FALSE;
        Node = BootOptions[Index].FilePath;
        while (!IsDevicePathEnd(Node))
        {
            if ((DevicePathType(Node) == MEDIA_DEVICE_PATH) &&
                (DevicePathSubType(Node) == MEDIA_FILEPATH_DP))
            {
                FilePathNode = (FILEPATH_DEVICE_PATH *)Node;
                if (IsWindowsBootFile(FilePathNode->PathName))
                {
                    Found = TRUE;
                    break;
                }
            }
            Node = NextDevicePathNode(Node);
        }
        if (Found)
        {
            Print(L"[Boot] Windows found: %s\n", BootOptions[Index].Description);
            EfiBootManagerBoot(&BootOptions[Index]);
        }
    }
    Print(L"[Error] Windows boot loader not found.\n");
    EfiBootManagerFreeLoadOptions(BootOptions, BootOptionCount);
}

EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable)
{
    UINTN Index;
    EFI_INPUT_KEY Key;
    DisableVBS();
    BootWindows();
    Print(L"\nPress any key to exit...\n");
    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &Index);
    gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);
    return EFI_SUCCESS;
}
