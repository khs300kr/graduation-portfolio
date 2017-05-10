#pragma once

#include "resource.h"
// server
void ReadPacket(SOCKET sock);
void ProcessPacket(char *ptr);
