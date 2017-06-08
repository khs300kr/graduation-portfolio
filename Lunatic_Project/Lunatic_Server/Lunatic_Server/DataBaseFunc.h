#pragma once
#define ID_LEN			10
#define PASSWORD_LEN	10

void Init_DB(void);
void Close_DB(void);
//
void Client_Login(char id[], char password[], int ci);
