#include <string>
#include <vector>

bool AllBoolsUsed(bool* arr, unsigned count) {
	for (unsigned a = 0; a < count; a++)
		if (arr[a] == false) return false;
	return true;
}

std::string ReadStringWithSize(char* arr, unsigned* pos = NULL) {
	std::string retVal;
	retVal.resize((unsigned char)arr[0] - 1);
	memmove((void*)retVal.c_str(), arr + 1, (unsigned char)arr[0] - 1);
	if (pos != NULL) *pos += (unsigned char)arr[0] + 1;
	return retVal;
}

std::string GetFolderName(const std::string& fname)
{
	size_t pos = fname.find_last_of("\\/");
	return (std::string::npos == pos)
		? ""
		: fname.substr(0, pos);
}

std::string GetFileName(std::string path) {
	return path.substr(path.find_last_of("/\\") + 1);
}

std::string GetFileNameWithoutExtension(std::string path) {
	size_t sep = path.find_last_of("\\/");
	if (sep != std::string::npos)
		path = path.substr(sep + 1, path.size() - sep - 1);

	size_t dot = path.find_last_of(".");
	std::string name;
	if (dot != std::string::npos)
	{
		name = path.substr(0, dot);
		std::string ext = path.substr(dot, path.size() - dot);
	}
	else
	{
		name = path;
		std::string ext = "";
	}
	return name;
}

std::string GetExtension(std::string path) {
	size_t dot = path.find_last_of(".") + 1;
	if (dot != std::string::npos) return path.substr(dot, path.size() - dot);
	else return "";

}

std::string GetVariableType(int in) {
	switch (in) {
	case 1: return "int";
	case 2: return "float";
	case 3: return "float2";
	case 4: return "float3";
	case 5: return "float4";
	case 6: return "sampler";
	case 7: return "bool";
	case 8: return "float4x3";
	case 9: return "float4x4";
	}
}
std::string GetShaderType(int in) {
	switch (in) {
	case 0:return "VertexShaders";
	case 1:return "PixelShaders";
	case 2:return "ComputeShaders";
	case 3:return "DomainShaders";
	case 4:return "GeometryShaders";
	case 5:return "HullShaders";
	}
}

std::string GetSamplerStateTypePC(unsigned in) {
	switch (in) {
	case 0: return "AddressU";
	case 1: return "AddressV";
	case 2: return "AddressW";
	case 3: return "BorderColor";
	case 4: return "MagFilter";
	case 5: return "MinFilter";
	case 6: return "MipFilter";
	case 7: return "MipMapLodBias";
	case 8: return "MaxMipLevel";
	case 9: return "MaxAnisotropy";
	case 10: return "SrgbTexture";
	case 11: return "ElementIndex";
	case 12: return "DMapOffset";
	default: return std::to_string((unsigned long long)in);
	}
}
std::string GetTextureAddressPC(unsigned in) {
	switch (in) {
	case 1: return "Wrap";
	case 2: return "Mirror";
	case 3: return "Clamp";
	case 4: return "Border";
	case 5: return "MirrorOnce";
	default: return std::to_string((unsigned long long)in);
	}
}
std::string GetTextureFilterTypePC(unsigned in) {
	switch (in) {
	case 0: return "None";
	case 1: return "Point";
	case 2: return "Linear";
	case 3: return "Anisotropic";
	case 6: return "PyramidalQuad";
	case 7: return "GaussianQuad";
	case 8: return "ConvolutionMono";
	default: return std::to_string((unsigned long long)in);
	}
}

bool IsPathExist(const std::string& s) {
	struct stat buffer;
	return (stat(s.c_str(), &buffer) == 0);
}

void CreateFolder(std::string path) {
	if (!IsPathExist(path)) system(("mkdir \"" + path + "\"").c_str());
}

int GetIntFromVariableType(std::string paramtype) {
	if (paramtype == "int") return 1;
	else if (paramtype == "float") return 2;
	else if (paramtype == "float2") return 3;
	else if (paramtype == "float3") return 4;
	else if (paramtype == "float4") return 5;
	else if (paramtype == "sampler") return 6;
	else if (paramtype == "bool") return 7;
	else if (paramtype == "float4x3") return 8;
	else if (paramtype == "float4x4") return 9;
	else return 0;
}

int GetIntFromSamplerStateTypePC(std::string paramtype) {
	if (paramtype == "AddressU") return 0;
	else if (paramtype == "AddressV") return 1;
	else if (paramtype == "AddressW") return 2;
	else if (paramtype == "BorderColor") return 3;
	else if (paramtype == "MagFilter") return 4;
	else if (paramtype == "MinFilter") return 5;
	else if (paramtype == "MipFilter") return 6;
	else if (paramtype == "MipMapLodBias") return 7;
	else if (paramtype == "MaxMipLevel") return 8;
	else if (paramtype == "MaxAnisotropy") return 9;
	else if (paramtype == "SrgbTexture") return 10;
	else if (paramtype == "ElementIndex") return 11;
	else if (paramtype == "DMapOffset") return 12;
	return 0;
}

int GetIntFromTextureAddressPC(std::string paramtype) {
	if (paramtype == "Wrap") return 1;
	else if (paramtype == "Mirror") return 2;
	else if (paramtype == "Clamp") return 3;
	else if (paramtype == "Border") return 4;
	else if (paramtype == "MirrorOnce") return 5;
	return 1;
}

int GetIntFromTextureFilterTypePC(std::string in) {
	int out;
	if (in == "None") return 0;
	else	if (in == "Point") return 1;
	else	if (in == "Linear") return 2;
	else	if (in == "Anisotropic") return 3;
	else	if (in == "PyramidalQuad") return 6;
	else	if (in == "GaussianQuad") return 7;
	else	if (in == "ConvolutionMono") return 8;
	return 0;
}

int CheckFxcPlatform(std::vector<unsigned char>* fxcData) {
	unsigned posInFxc = 5;
	//unsigned tmpInt;
	unsigned char tmpChar;
	unsigned short tmpShort;
	memmove(&tmpChar, &(*fxcData)[4], 0x1);
	if (tmpChar != 0) {
		memmove(&tmpChar, &(*fxcData)[posInFxc], 0x1);
		posInFxc++;
		unsigned char paramCount = tmpChar;
		for (unsigned char i = 0; i < paramCount; i++) {
			posInFxc+=0x4; // skip type & index
			memmove(&tmpChar, &(*fxcData)[posInFxc], 0x1);
			posInFxc++;
			//memmove(&tmpChar, &(*fxcData)[posInFxc], tmpChar);
			posInFxc += tmpChar;
		}
		posInFxc += 0x4;
		memmove(&tmpShort, &(*fxcData)[posInFxc], 0x2);
		posInFxc += 2;
		if (tmpShort == 768) return 1; // pc
		else if (tmpShort == 10768) return 2; // xenon
		else {
			printf("Unk shader bytecode");
			exit(0);
		}
	}
}