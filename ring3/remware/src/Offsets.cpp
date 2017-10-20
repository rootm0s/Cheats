#include "Offsets.h"
#include <iostream>

std::string COffsets::toHex(DWORD offset) {
    std::stringstream ss;
    ss << std::hex << offset;
    std::string x = ss.str();
    for (auto &c : x) c = toupper(c);

    return x;
}
void COffsets::getOffsets(PMemory* m) {
    mem = m;

    modClient = mem->GetModule("client.dll");
    modEngine = mem->GetModule("engine.dll");
}

