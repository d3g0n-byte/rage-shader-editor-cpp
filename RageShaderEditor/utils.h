#pragma once
#include <vector>

bool AllBoolsUsed(bool* arr, unsigned count);
std::string ReadStringWithSize(char* arr, unsigned* pos = NULL);
std::string GetFolderName(const std::string& fname);
std::string GetFileName(std::string path);
std::string GetFileNameWithoutExtension(std::string path);
std::string GetExtension(std::string path);
std::string GetVariableType(int type);
std::string GetShaderType(int type);
std::string GetSamplerStateTypePC(unsigned in);
std::string GetTextureAddressPC(unsigned in);
std::string GetTextureFilterTypePC(unsigned in);
bool IsPathExist(const std::string& s);
void CreateFolder(std::string path);
int GetIntFromVariableType(std::string paramtype);
int GetIntFromSamplerStateTypePC(std::string paramtype);
int GetIntFromTextureAddressPC(std::string paramtype);
int GetIntFromTextureFilterTypePC(std::string in);
int CheckFxcPlatform(std::vector<unsigned char>* fxcData);