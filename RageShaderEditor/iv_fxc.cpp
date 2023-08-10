#include "iv_fxc.h"
typedef std::pair<const std::string, boost::property_tree::ptree> XmlChild;

void IV::ToXML(std::vector<unsigned char> *fxc_data, std::string fileName, std::string folder) {
	int platform = CheckFxcPlatform(fxc_data);
	//if (platform != 2) exit(0);

	unsigned int posInFxc = 5;
	Fxc_IV fxc_file;
	memmove(&fxc_file.magic, &(*fxc_data)[0], 0x4);

	std::vector<Fragment>* tmpFrag;
	for (unsigned char x = 0; x < 2; x++) {
		if (x == 0) {
			tmpFrag = &fxc_file.vertexFragment;
			memmove(&fxc_file.vertexFragmentCount, &(*fxc_data)[4], 0x1);
			(*tmpFrag).resize(fxc_file.vertexFragmentCount);
		}
		else {
			tmpFrag = &fxc_file.pixelFragment;
			memmove(&fxc_file.pixelFragmentCount, &(*fxc_data)[posInFxc], 0x6);
			posInFxc+=6;
			fxc_file.pixelFragmentCount--; // Kоличество всегда +1. —делано дл€ того, чтобы можно было не использовать шейдер дл€ этой техники если указать 0.\
											  “акое встречал только в Red Dead Redemption
			(*tmpFrag).resize(fxc_file.pixelFragmentCount);
		}
		for (unsigned char i = 0; i < (*tmpFrag).size(); i++) {
			memmove(&(*tmpFrag)[i].variablesCount, &(*fxc_data)[posInFxc], 0x1);
			posInFxc++;
			(*tmpFrag)[i].variable.resize((*tmpFrag)[i].variablesCount);
			for (unsigned char j = 0; j < (*tmpFrag)[i].variablesCount; j++) {
				memmove(&(*tmpFrag)[i].variable[j].type, &(*fxc_data)[posInFxc], 0x4);
				posInFxc += 4;
				(*tmpFrag)[i].variable[j].name = ReadStringWithSize((char*)&(*fxc_data)[posInFxc], &posInFxc);
			}
			memmove(&(*tmpFrag)[i].shaderSize, &(*fxc_data)[posInFxc], 0x2);
			posInFxc += 2;
			memmove(&(*tmpFrag)[i].shaderSize2, &(*fxc_data)[posInFxc], 0x2);
			posInFxc += 2;
			(*tmpFrag)[i].shader.bytecode.resize((*tmpFrag)[i].shaderSize);
			memmove(&(*tmpFrag)[i].shader.bytecode[0], &(*fxc_data)[posInFxc], (*tmpFrag)[i].shaderSize);
			posInFxc += (*tmpFrag)[i].shaderSize;
		}
	}
	
	std::vector<Variable>* tmpVar;
	for (size_t x = 0; x < 2; x++) {
		if (x == 0) {
			tmpVar = &fxc_file.globalVariable;
			memmove(&fxc_file.globalVariablesCount, &(*fxc_data)[posInFxc], 0x1);
			posInFxc++;
			(*tmpVar).resize(fxc_file.globalVariablesCount);
		}
		else {
			tmpVar = &fxc_file.localVariable;
			memmove(&fxc_file.localVariablesCount, &(*fxc_data)[posInFxc], 0x1);
			posInFxc++;
			(*tmpVar).resize(fxc_file.localVariablesCount);
		}
		for (unsigned char i = 0; i < (*tmpVar).size(); i++) {
			memmove(&(*tmpVar)[i].type, &(*fxc_data)[posInFxc], 0x2);
			posInFxc += 2;
			(*tmpVar)[i].name1 = ReadStringWithSize((char*)&(*fxc_data)[posInFxc], &posInFxc);
			(*tmpVar)[i].name2 = ReadStringWithSize((char*)&(*fxc_data)[posInFxc], &posInFxc);
			memmove(&(*tmpVar)[i].annotationCount, &(*fxc_data)[posInFxc], 0x1);
			posInFxc++;
			(*tmpVar)[i].annotation.resize((*tmpVar)[i].annotationCount);
			for (size_t j = 0; j < (*tmpVar)[i].annotationCount; j++) {
				(*tmpVar)[i].annotation[j].name = ReadStringWithSize((char*)&(*fxc_data)[posInFxc], &posInFxc);
				memmove(&(*tmpVar)[i].annotation[j].valueType, &(*fxc_data)[posInFxc], 0x1);
				posInFxc++;
				switch ((*tmpVar)[i].annotation[j].valueType) {
				case 0:
					memmove(&(*tmpVar)[i].annotation[j].value.intValue, &(*fxc_data)[posInFxc], 0x4);
					posInFxc+= 4;
					break;
				case 1:
					memmove(&(*tmpVar)[i].annotation[j].value.floatValue, &(*fxc_data)[posInFxc], 0x4);
					posInFxc+= 4;
					break;
				case 2:
					(*tmpVar)[i].annotation[j].value.stringValue = ReadStringWithSize((char*)&(*fxc_data)[posInFxc], &posInFxc);
					break;
				default:
					break;
				}
			}
			memmove(&(*tmpVar)[i].valueCount, &(*fxc_data)[posInFxc], 0x1);
			posInFxc++;
			(*tmpVar)[i].value.resize((*tmpVar)[i].valueCount);
			if ((*tmpVar)[i].valueCount != 0) {
				memmove(&(*tmpVar)[i].value[0], &(*fxc_data)[posInFxc], 0x4 * (*tmpVar)[i].valueCount);
				posInFxc += 0x4 * (*tmpVar)[i].valueCount;
			}
		}

	}

	memmove(&fxc_file.techniquesCount, &(*fxc_data)[posInFxc], 0x1);
	posInFxc++;
	fxc_file.technique.resize(fxc_file.techniquesCount);
	for (unsigned char i = 0; i < fxc_file.techniquesCount; i++) {
		fxc_file.technique[i].name = ReadStringWithSize((char*)&(*fxc_data)[posInFxc], &posInFxc);
		memmove(&fxc_file.technique[i].passCount, &(*fxc_data)[posInFxc], 0x1);
		posInFxc++;
		fxc_file.technique[i].pass.resize(fxc_file.technique[i].passCount);
		for (unsigned char j = 0; j < fxc_file.technique[i].passCount; j++) {
			memmove(&fxc_file.technique[i].pass[j].vs, &(*fxc_data)[posInFxc], 0x3);
			posInFxc+=0x3;
			fxc_file.technique[i].pass[j].value.resize(fxc_file.technique[i].pass[j].valueCount);
			for (unsigned char n = 0; n < fxc_file.technique[i].pass[j].valueCount; n++) {
				memmove(&fxc_file.technique[i].pass[j].value[n].type, &(*fxc_data)[posInFxc], 0x8);
				posInFxc += 0x8;
			}
		}
	}
	// и имена. »х нет в оригинальном fxc файле. ƒобавлены чтобы соотвествовать fxc файлам остальных игр.
	if ((*fxc_data).size() > posInFxc) { // провер€ем есть ли мена
		for (unsigned char i = 0; i < fxc_file.vertexFragmentCount; i++) {
			fxc_file.vertexFragment[i].name = ReadStringWithSize((char*)&(*fxc_data)[posInFxc], &posInFxc);
		}
		for (unsigned char i = 0; i < fxc_file.pixelFragmentCount; i++) {
			fxc_file.pixelFragment[i].name = ReadStringWithSize((char*)&(*fxc_data)[posInFxc], &posInFxc);
		}
	}
	else {
		for (unsigned char i = 0; i < fxc_file.vertexFragmentCount; i++) {
			fxc_file.vertexFragment[i].name = fileName + "_vs" + std::to_string((unsigned long long)i);
		}
		for (unsigned char i = 0; i < fxc_file.pixelFragmentCount; i++) {
			fxc_file.pixelFragment[i].name = fileName + "_ps" + std::to_string((unsigned long long)i);
		}
	}

	// пишем xml шейдер
	XmlWritter xml;
	xml.WriteGroupStart("Effect");
	switch (platform) {
	case 1:
		xml.WriteValue("Platform", "pc");
		break;
	case 2:
		xml.WriteValue("Platform", "xenon");
		break;
	}
	xml.WriteGroupStart("Shaders", fxc_file.pixelFragmentCount + fxc_file.vertexFragmentCount);
	std::vector<std::string> vsAsmBuffer;
	std::vector<std::string> psAsmBuffer;
	vsAsmBuffer.resize(fxc_file.vertexFragmentCount);
	psAsmBuffer.resize(fxc_file.pixelFragmentCount);
	std::vector<std::string>* tmpShaderBuffer;
	for (unsigned char x = 0; x < 2; x++) {
		std::string fragName;
		if (x == 0) {
			tmpFrag = &fxc_file.vertexFragment;
			fragName = "VertexShaders";
			tmpShaderBuffer = &vsAsmBuffer;
		}
		else {
			tmpFrag = &fxc_file.pixelFragment;
			fragName = "PixelShaders";
			tmpShaderBuffer = &psAsmBuffer;
		}
		xml.WriteGroupStart(fragName, (*tmpFrag).size());
		for (unsigned char i = 0; i < (*tmpFrag).size(); i++) {
			//std::string filePath =;

			LPD3DXBUFFER shader = NULL;
			auto d3dx9x = LoadLibraryW((L"d3dx9x.dll"));
			if (!d3dx9x) {
				printf("we expect d3dx9x.dll in your root directory\n");
				exit(0);
			}
			auto D3DX9XDisassembleShader = (HRESULT (*)(CONST DWORD * pShader, BOOL EnableColorCode, LPCSTR pComments, LPD3DXBUFFER * ppDisassembly))GetProcAddress(d3dx9x, "D3DX9XDisassembleShader");
			HRESULT hr = D3DX9XDisassembleShader((DWORD*)&(*tmpFrag)[i].shader.bytecode[0], 0, NULL, &shader);

			if (FAILED(hr)) {
				printf("ERR");
			}
			(*tmpShaderBuffer)[i].resize(shader->GetBufferSize()-1);
			memmove(&(*tmpShaderBuffer)[i][0], shader->GetBufferPointer(), shader->GetBufferSize());

			std::vector<unsigned char> tmp;
			tmp.resize(shader->GetBufferSize());
			memmove(&tmp[0], shader->GetBufferPointer(), shader->GetBufferSize());

			xml.WriteGroupStart("Item");
			xml.WriteValue("Name", (*tmpFrag)[i].name);
			xml.WriteValue("File", fileName + '\\' + (*tmpFrag)[i].name + ".asm");
			xml.WriteGroupStart("Variables", (*tmpFrag)[i].variablesCount);
			for (unsigned char j = 0; j < (*tmpFrag)[i].variablesCount; j++) {
				xml.WriteGroupStart("Item");
				xml.WriteValue("Type", GetVariableType((*tmpFrag)[i].variable[j].type));
				xml.WriteValue("Name", (*tmpFrag)[i].variable[j].name);
				xml.WriteValue("Index", (*tmpFrag)[i].variable[j].index);
				xml.WriteGroupEnd();
			}
			xml.WriteGroupEnd();
			xml.WriteGroupEnd();
		}
		xml.WriteGroupEnd();
	}
	xml.WriteGroupEnd(); // shaders
	for (unsigned char x = 0; x < 2; x++) {
		std::string fragName;
		if (x == 0) {
			tmpVar = &fxc_file.globalVariable;
			fragName = "GlobalVariables";
		}
		else {
			tmpVar = &fxc_file.localVariable;
			fragName = "LocalVariables";
		}
		xml.WriteGroupStart(fragName, (*tmpVar).size());
		for (unsigned char i = 0; i < (*tmpVar).size(); i++) {
			xml.WriteGroupStart("Item");
			xml.WriteValue("Type", GetVariableType((*tmpVar)[i].type));
			xml.WriteValue("ArrayCount", (*tmpVar)[i].arrayCount);
			xml.WriteValue("FirstName", (*tmpVar)[i].name1);
			xml.WriteValue("SecondName", (*tmpVar)[i].name2);
			xml.WriteGroupStart("Annotations", (*tmpVar)[i].annotationCount);
			for (unsigned char j = 0; j < (*tmpVar)[i].annotationCount; j++) {
				xml.WriteGroupStart("Item");
				xml.WriteValue("Name", (*tmpVar)[i].annotation[j].name);
				switch ((*tmpVar)[i].annotation[j].valueType) {
				case 0:
					xml.WriteValue("Type", "int");
					xml.WriteValue("Value", (*tmpVar)[i].annotation[j].value.intValue);
					break;
				case 1:
					float val;
					memmove(&val, &(*tmpVar)[i].annotation[j].value.intValue, 0x4);
					xml.WriteValue("Type", "float");
					xml.WriteValue("Value", val);
					break;
				case 2:
					xml.WriteValue("Type", "string");
					xml.WriteValue("Value", (*tmpVar)[i].annotation[j].value.stringValue);
					break;
				}
				xml.WriteGroupEnd();
			}
			xml.WriteGroupEnd();
			if ((*tmpVar)[i].valueCount != 0) {
				std::stringstream values;
				//values.setf(std::ios::fixed);
				//values.precision(8);
				float tmpVal;
				if ((*tmpVar)[i].type == 1 || (*tmpVar)[i].type == 7) {
					for (unsigned char j = 0; j < (*tmpVar)[i].valueCount; j++) {
						if (j != 0) values << ";";
						values << (*tmpVar)[i].value[j];
					}
				}
				else if ((*tmpVar)[i].type == 6) {
					if (platform == 1) {
						for (unsigned char j = 0; j < (*tmpVar)[i].valueCount; j += 2) {
							if (j != 0) values << ";";
							values << GetSamplerStateTypePC((*tmpVar)[i].value[j]) << '=';
							if ((*tmpVar)[i].value[j] >= 0 && (*tmpVar)[i].value[j] <= 2)
								values << GetTextureAddressPC((*tmpVar)[i].value[j + 1]);
							else if ((*tmpVar)[i].value[j] >= 4 && (*tmpVar)[i].value[j] <= 6)
								values << GetTextureFilterTypePC((*tmpVar)[i].value[j + 1]);
							else {
								memmove(&tmpVal, &(*tmpVar)[i].value[j + 1], 0x4);
								values << tmpVal;
							}
						}
					}
					else if (platform == 2) {
						for (unsigned char j = 0; j < (*tmpVar)[i].valueCount; j += 2) {
							//if (j != 0) values << ";";
							//(*tmpVar)[i].value[j]++;
							//(*tmpVar)[i].value[j + 1]++;
							//values << GetSamplerStateTypePC((*tmpVar)[i].value[j]) << '=';
							//if ((*tmpVar)[i].value[j] >= 0 && (*tmpVar)[i].value[j] <= 2)
							//	values << GetTextureAddressPC((*tmpVar)[i].value[j] + 1);
							//else if ((*tmpVar)[i].value[j] >= 4 && (*tmpVar)[i].value[j] <= 6)
							//	values << GetTextureFilterTypePC((*tmpVar)[i].value[j] + 1);
							//else {
							//	memmove(&tmpVal, &(*tmpVar)[i].value[j + 1], 0x4);
							//	values << tmpVal;
							//}
							if (j != 0) values << ";";
							values << (*tmpVar)[i].value[j] << '=';
							if ((*tmpVar)[i].value[j] >= 0 && (*tmpVar)[i].value[j] <= 2)
								values << (*tmpVar)[i].value[j + 1];
							else if ((*tmpVar)[i].value[j] >= 4 && (*tmpVar)[i].value[j] <= 6)
								values << (*tmpVar)[i].value[j + 1];
							else {
								memmove(&tmpVal, &(*tmpVar)[i].value[j + 1], 0x4);
								values << tmpVal;
							}
						}
					}
				}
				else {
					for (unsigned char j = 0; j < (*tmpVar)[i].valueCount; j++) {
						memmove(&tmpVal, &(*tmpVar)[i].value[j], 0x4);
						if (j != 0) values << ";";
						values << tmpVal;
					}
				}
				xml.WriteValue("Values", values.str());
			}
			else xml.WriteValue("Values", "none");
			xml.WriteGroupEnd();

		}
		xml.WriteGroupEnd();

	}
	xml.WriteGroupStart("Techniques", fxc_file.techniquesCount);
	for (unsigned char i = 0; i < fxc_file.techniquesCount; i++) {
		xml.WriteGroupStart("Item");
		xml.WriteValue("Name", fxc_file.technique[i].name);
		xml.WriteGroupStart("Passes", fxc_file.technique[i].passCount);
		for (unsigned char j = 0; j < fxc_file.technique[i].passCount; j++) {
			xml.WriteGroupStart("Item");
			xml.WriteValue("VertexShader", fxc_file.vertexFragment[fxc_file.technique[i].pass[j].vs].name);
			xml.WriteValue("PixelShader", fxc_file.technique[i].pass[j].ps != 0? fxc_file.pixelFragment[fxc_file.technique[i].pass[j].ps - 1].name : "null");
			xml.WriteGroupStart("Params", fxc_file.technique[i].pass[j].valueCount);
			for (unsigned char n = 0; n < fxc_file.technique[i].pass[j].valueCount; n++) {
				xml.WriteGroupStart("Item");
				xml.WriteValue("Type", fxc_file.technique[i].pass[j].value[n].type);
				xml.WriteValue("Value", fxc_file.technique[i].pass[j].value[n].value);
				xml.WriteGroupEnd();
			}
			xml.WriteGroupEnd();
			xml.WriteGroupEnd();

		}
		xml.WriteGroupEnd();
		xml.WriteGroupEnd();

	}
	xml.WriteGroupEnd();

	CreateFolder(folder + '\\' + fileName);
	std::ofstream asmOut; 
	for (unsigned char i = 0; i < fxc_file.vertexFragmentCount; i++) {
		asmOut.open(folder + '\\' + fileName + '\\' + fxc_file.vertexFragment[i].name + ".asm");
		asmOut << vsAsmBuffer[i];
		asmOut.close();
	}
	for (unsigned char i = 0; i < fxc_file.pixelFragmentCount; i++) {
		asmOut.open(folder + '\\' + fileName + '\\' + fxc_file.pixelFragment[i].name + ".asm");
		asmOut << psAsmBuffer[i];
		asmOut.close();
	}
	xml.Save(folder + '\\' + fileName + ".xml");

}

struct AddonXmlShaderInfo {
	std::string filePath;
	std::string entryPoint;
};
struct AddonXmlVariableInfo {
	std::string values;
};

void IV::ToFXC(std::string filePath) {
	// читаем xml
	//char buffer[MAX_PATH + 1];
	//int count = ::GetModuleFileNameA(NULL, buffer, MAX_PATH);
	//std::string exePath = std::string(buffer, (count > 0) ? count : 0);

	std::ifstream t(filePath);
	if (!t.is_open()) {
		return;
	}
	t.seekg(0, std::ios::end);
	size_t size = t.tellg();
	std::string xmlCode(size, ' ');
	t.seekg(0);
	t.read(&xmlCode[0], size);
	t.close();

	std::stringstream stream(xmlCode);

	Fxc_IV fxc_file;
	std::vector<AddonXmlShaderInfo> vsAddonInfo;
	std::vector<AddonXmlShaderInfo> psAddonInfo;
	std::vector<AddonXmlVariableInfo> globalVarAddonInfo;
	std::vector<AddonXmlVariableInfo> localVarAddonInfo;
	int platform;

	try {
		boost::property_tree::ptree propertyTree;
		boost::property_tree::read_xml(stream, propertyTree);
		std::stringstream output_stream;
		boost::property_tree::write_xml(output_stream, propertyTree);
		std::string outputXmlCode = output_stream.str();
		BOOST_FOREACH (auto& a, propertyTree.get_child("Effect")) {
			if (a.first == "Platform") {
				if (a.second.data() == "pc") platform = 1;
				else if (a.second.data() == "xenon") platform = 2;
			}
			else if (a.first == "Shaders") {
				BOOST_FOREACH(auto& b , a.second.get_child("")) {
					if ((b.first == "VertexShaders" || b.first == "PixelShaders")) {
						std::vector<Fragment>* tmpFrag;
						unsigned char* fragsCount;
						std::vector<AddonXmlShaderInfo>* tmpAddonInfo;
						if (b.first == "VertexShaders") {
							tmpFrag = &fxc_file.vertexFragment;
							fragsCount = &fxc_file.vertexFragmentCount;
							tmpAddonInfo = &vsAddonInfo;
						}
						else /*if (b.first == "PixelShaders")*/ {
							tmpFrag = &fxc_file.pixelFragment;
							fragsCount = &fxc_file.pixelFragmentCount;
							tmpAddonInfo = &psAddonInfo;
						}
						BOOST_FOREACH(auto& x, b.second.get_child("")) {
							(*fragsCount)++;
							(*tmpFrag).push_back(Fragment());
							(*tmpAddonInfo).push_back(AddonXmlShaderInfo());
							int currentFragment = (*fragsCount) - 1;
							(*tmpFrag)[currentFragment].variablesCount = 0;
							BOOST_FOREACH(auto& c , x.second) {
								if (c.first == "Name") (*tmpFrag)[currentFragment].name = c.second.data();
								else if (c.first == "File") (*tmpAddonInfo)[currentFragment].filePath = c.second.data();
								else if (c.first == "EntryPoint") (*tmpAddonInfo)[currentFragment].entryPoint = c.second.data();
								else if (c.first == "Variables") {
									BOOST_FOREACH(auto& d , c.second.get_child("")) {
										(*tmpFrag)[currentFragment].variablesCount++;
										(*tmpFrag)[currentFragment].variable.push_back(ShaderVariable());
										int currentVariable = (*tmpFrag)[currentFragment].variablesCount - 1;
										BOOST_FOREACH(auto& e , d.second) {
											if (e.first == "Type")
												(*tmpFrag)[currentFragment].variable[currentVariable].type = GetIntFromVariableType(e.second.data());
											else if (e.first == "Name")
												(*tmpFrag)[currentFragment].variable[currentVariable].name = e.second.data();
											else if (e.first == "Index")
												(*tmpFrag)[currentFragment].variable[currentVariable].index = std::stoi(e.second.data());
										}
									}

								}
							}//printf("c");
						}
					}
				}
			}
			else if ((a.first == "GlobalVariables" || a.first == "LocalVariables")) {
				std::vector<Variable>* tmpVar;
				//std::vector<AddonXmlVariableInfo>* tmpVarAddonInfo;
				unsigned char* tmpVarCount;
				if (a.first == "GlobalVariables") {
					tmpVar = &fxc_file.globalVariable;
					//tmpVarAddonInfo = &globalVarAddonInfo;
					tmpVarCount = &fxc_file.globalVariablesCount;
				}
				else {
					tmpVar = &fxc_file.localVariable;
					//tmpVarAddonInfo = &localVarAddonInfo;
					tmpVarCount = &fxc_file.localVariablesCount;
				}
				BOOST_FOREACH(auto& b , a.second.get_child("")) {
					(*tmpVarCount)++;
					(*tmpVar).push_back(Variable());
					//(*tmpVarAddonInfo).push_back(AddonXmlVariableInfo());
					unsigned char currentVar = *tmpVarCount - 1;
					BOOST_FOREACH(auto& c , b.second) {
						if (c.first == "Type") (*tmpVar)[currentVar].type = GetIntFromVariableType(c.second.data());
						else if (c.first == "ArrayCount") (*tmpVar)[currentVar].arrayCount = std::stoi(c.second.data());
						else if (c.first == "FirstName") (*tmpVar)[currentVar].name1 = c.second.data();
						else if (c.first == "SecondName") (*tmpVar)[currentVar].name2 = c.second.data();
						else if (c.first == "Annotations") {
							(*tmpVar)[currentVar].annotationCount = 0;
							unsigned char currentAnnotation;
							BOOST_FOREACH(auto& d , c.second.get_child("")) {
								(*tmpVar)[currentVar].annotationCount++;
								currentAnnotation = (*tmpVar)[currentVar].annotationCount - 1;
								(*tmpVar)[currentVar].annotation.push_back(Annotation());
								BOOST_FOREACH(auto& e , d.second) {
									if (e.first == "Name") (*tmpVar)[currentVar].annotation[currentAnnotation].name = e.second.data();
									else if (e.first == "Type") {
										if (e.second.data() == "int")(*tmpVar)[currentVar].annotation[currentAnnotation].valueType = 0;
										else if (e.second.data() == "float")(*tmpVar)[currentVar].annotation[currentAnnotation].valueType = 1;
										else if (e.second.data() == "string")(*tmpVar)[currentVar].annotation[currentAnnotation].valueType = 2;
									}
									else if (e.first == "Value") {
										if ((*tmpVar)[currentVar].annotation[currentAnnotation].valueType == 0)
											(*tmpVar)[currentVar].annotation[currentAnnotation].value.intValue = std::stoi(e.second.data());
										else if ((*tmpVar)[currentVar].annotation[currentAnnotation].valueType == 1)
											(*tmpVar)[currentVar].annotation[currentAnnotation].value.floatValue = std::stof(e.second.data());
										else if ((*tmpVar)[currentVar].annotation[currentAnnotation].valueType == 2)
											(*tmpVar)[currentVar].annotation[currentAnnotation].value.stringValue = e.second.data();
									}
								}
							}
						}
						else if (c.first == "Values") {
							std::string tmpStr = c.second.data();
							if (tmpStr == "none")
								(*tmpVar)[currentVar].valueCount = 0;
							else {
								std::replace(tmpStr.begin(), tmpStr.end(), '=', ';');
								std::vector<std::string> variable;
								boost::split(variable, tmpStr, boost::is_any_of(";"));
								(*tmpVar)[currentVar].valueCount = variable.size();
								(*tmpVar)[currentVar].value.resize((*tmpVar)[currentVar].valueCount);
								if ((*tmpVar)[currentVar].type == 1 || (*tmpVar)[currentVar].type == 7) {
									for (unsigned char i = 0; i < (*tmpVar)[currentVar].valueCount; i++)
										(*tmpVar)[currentVar].value[i] = std::stoi(variable[i]);
								}
								else if ((*tmpVar)[currentVar].type == 6) {
									if (platform == 1) {
										for (unsigned char i = 0; i < (*tmpVar)[currentVar].valueCount; i += 2) {
											(*tmpVar)[currentVar].value[i] = GetIntFromSamplerStateTypePC(variable[i]);
											if ((*tmpVar)[currentVar].value[i] >= 0 && (*tmpVar)[currentVar].value[i] <= 2)
												(*tmpVar)[currentVar].value[i + 1] = GetIntFromTextureAddressPC(variable[i + 1]);
											else if ((*tmpVar)[currentVar].value[i] >= 4 && (*tmpVar)[currentVar].value[i] <= 6)
												(*tmpVar)[currentVar].value[i + 1] = GetIntFromTextureFilterTypePC(variable[i + 1]);
											else {
												float tmpFloat = std::stof(variable[i + 1]);
												memmove(&(*tmpVar)[currentVar].value[i + 1], &tmpFloat, 0x4);
											}
										}
									}
									else {
										for (unsigned char i = 0; i < (*tmpVar)[currentVar].valueCount; i += 2) {
											(*tmpVar)[currentVar].value[i] = std::stoi(variable[i]);
											if ((*tmpVar)[currentVar].value[i] >= 0 && (*tmpVar)[currentVar].value[i] <= 2)
												(*tmpVar)[currentVar].value[i + 1] = stoi(variable[i + 1]);
											else if ((*tmpVar)[currentVar].value[i] >= 4 && (*tmpVar)[currentVar].value[i] <= 6)
												(*tmpVar)[currentVar].value[i + 1] = stoi(variable[i + 1]);
											else {
												float tmpFloat = std::stof(variable[i + 1]);
												memmove(&(*tmpVar)[currentVar].value[i + 1], &tmpFloat, 0x4);
											}
										}
									}
								}
								else {
									float tmpVal;
									for (unsigned char i = 0; i < (*tmpVar)[currentVar].valueCount; i++) {
										tmpVal = std::stof(variable[i]);
										memmove(&(*tmpVar)[currentVar].value[i],& tmpVal, 0x4);
									}
								}
							}
						}

					}
				}
			}
			else if (a.first == "Techniques") {				
				BOOST_FOREACH(auto& b , a.second.get_child("")) {
					fxc_file.techniquesCount++;
					fxc_file.technique.push_back(Technique());
					unsigned char currentTeq = fxc_file.techniquesCount - 1;
					BOOST_FOREACH(auto& c , b.second) {
						if (c.first == "Name") fxc_file.technique[currentTeq].name = c.second.data();
						else if (c.first == "Passes" ) {
							fxc_file.technique[currentTeq].passCount = 0;
							unsigned char currentPass;
							BOOST_FOREACH(auto& d , c.second.get_child("")) {
								fxc_file.technique[currentTeq].passCount++;
								fxc_file.technique[currentTeq].pass.push_back(Pass());
								currentPass = fxc_file.technique[currentTeq].passCount - 1;
								BOOST_FOREACH(auto& e , d.second) {
									if (e.first == "VertexShader") {
										for (unsigned char i = 0; i < fxc_file.vertexFragment.size(); i++)
											if (fxc_file.vertexFragment[i].name == e.second.data()) {
												fxc_file.technique[currentTeq].pass[currentPass].vs = i;
												break;
											}
									}
									else if (e.first == "PixelShader") {
										if (e.second.data() == "null") fxc_file.technique[currentTeq].pass[currentPass].ps = 0;
										else {
											for (unsigned char i = 0; i < fxc_file.pixelFragment.size(); i++)
												if (fxc_file.pixelFragment[i].name == e.second.data()) {
													fxc_file.technique[currentTeq].pass[currentPass].ps = i + 1;
													break;
												}
										}
									}
									else if (e.first == "Params") {
										fxc_file.technique[currentTeq].pass[currentPass].valueCount = 0;
										unsigned char currentParam;
										BOOST_FOREACH(auto& f , e.second.get_child("")) {
											fxc_file.technique[currentTeq].pass[currentPass].valueCount++;
											currentParam = fxc_file.technique[currentTeq].pass[currentPass].valueCount - 1;
											fxc_file.technique[currentTeq].pass[currentPass].value.push_back(PassValue());
											BOOST_FOREACH(auto& g , f.second) {
												if (g.first == "Type") 
													fxc_file.technique[currentTeq].pass[currentPass].value[currentParam].type = std::stoi(g.second.data());
												else if (g.first == "Value") 
													fxc_file.technique[currentTeq].pass[currentPass].value[currentParam].value = std::stoi(g.second.data());

											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

	}
	catch (boost::property_tree::xml_parser_error) {
		throw;
	}

	// собераем шейдеры в кучу
	for (unsigned char x = 0; x < 2; x++) {
		std::vector<Fragment>* tmpFrag;
		std::vector<AddonXmlShaderInfo>* tmpShaderInfo;
		std::string profile;
		if (x == 0) {
			tmpFrag = &fxc_file.vertexFragment;
			tmpShaderInfo = &vsAddonInfo;
			profile = "vs_3_0";
		}
		else {
			tmpFrag = &fxc_file.pixelFragment;
			tmpShaderInfo = &psAddonInfo;
			profile = "ps_3_0";
		}

		for (unsigned char i = 0; i < (*tmpFrag).size(); i++) {
			std::string shaderPath = GetFolderName(filePath) + '\\' + (*tmpShaderInfo)[i].filePath;
			std::string ext = GetExtension((*tmpShaderInfo)[i].filePath);
			if (ext == "asm") {
				t.open(shaderPath);
				if (!t.is_open()) {
					printf("file on path %s not found\n", shaderPath.c_str());
					system("pause");
					exit(0);
				}
				t.seekg(0, std::ios::end);
				size = t.tellg();
				std::string asmShader(size, ' ');
				t.seekg(0);
				t.read(&asmShader[0], size);
				t.close();
				std::replace(asmShader.begin(), asmShader.end(), ';', '\n');

				LPD3DXBUFFER shaderByteCode = NULL;
				LPD3DXBUFFER error = NULL;

				HRESULT hr;
				if(platform == 1)
					hr = D3DXAssembleShader(asmShader.c_str(), asmShader.length(), 0, 0, 0, &shaderByteCode, &error);
				else {
					auto d3dx9x = LoadLibraryW((L"d3dx9x.dll"));
					if (!d3dx9x) {
						printf("we expect d3dx9x.dll in your root directory\n");
						system("pause");
						exit(0);
					}
					auto D3DX9XAssembleShader = (HRESULT(*)(LPCSTR pSrcData, UINT SrcDataLen, CONST D3DXMACRO * pDefines, LPD3DXINCLUDE pInclude, DWORD Flags, LPD3DXBUFFER * ppShader, LPD3DXBUFFER * ppErrorMsgs))GetProcAddress(d3dx9x, "D3DX9XAssembleShader");
					hr = D3DX9XAssembleShader(asmShader.c_str(), asmShader.length(), 0, 0, 0, &shaderByteCode, &error);
				}
				if (FAILED(hr)) {
					std::string errText;
					errText.resize(error->GetBufferSize() - 1);
					memmove(&errText[0], error->GetBufferPointer(), error->GetBufferSize() - 1);
					size_t pos = errText.find("):");
					if(pos +1  < errText.length())
						errText = errText.erase(0, pos + 1);
					printf("Error in shader %s %s", GetFileName((*tmpShaderInfo)[i].filePath).c_str(), errText.c_str());
					system("pause");
					exit(0);
				}
				(*tmpFrag)[i].shaderSize = (*tmpFrag)[i].shaderSize2 = shaderByteCode->GetBufferSize();
				(*tmpFrag)[i].shader.bytecode.resize((*tmpFrag)[i].shaderSize);
				memmove(&(*tmpFrag)[i].shader.bytecode[0], shaderByteCode->GetBufferPointer(), (*tmpFrag)[i].shaderSize);
			}
			else if (ext == "hlsl" || ext == "fx") {
				t.open(shaderPath);
				if (!t.is_open()) {
					printf("file on path %s not found\n", shaderPath.c_str());
					system("pause");
					exit(0);
				}
				t.seekg(0, std::ios::end);
				size = t.tellg();
				std::string hlslCode(size, ' ');
				t.seekg(0);
				t.read(&hlslCode[0], size);
				t.close();
				LPD3DXBUFFER shaderByteCode = NULL;
				LPD3DXBUFFER error = NULL;
				HRESULT hr;
				if (platform == 1)
					hr = D3DXCompileShader(hlslCode.c_str(), hlslCode.length(), NULL, NULL, (*tmpShaderInfo)[i].entryPoint.c_str(), profile.c_str(), NULL, &shaderByteCode, &error, NULL);
				else {
					auto d3dx9x = LoadLibraryW((L"d3dx9x.dll"));
					if (!d3dx9x) {
						printf("we expect d3dx9x.dll in your root directory\n");
						system("pause");
						exit(0);
					}
					auto D3DX9XCompileShader = (HRESULT(*)(LPCSTR pSrcData, UINT SrcDataLen, D3DXMACRO * pDefines, LPD3DXINCLUDE pInclude, LPCSTR pFunctionName, LPCSTR pProfile, DWORD Flags, LPD3DXBUFFER * ppShader, LPD3DXBUFFER * ppErrorMsgs, LPD3DXCONSTANTTABLE * ppConstantTable))GetProcAddress(d3dx9x, "D3DX9XCompileShader");
					hr = D3DX9XCompileShader(hlslCode.c_str(), hlslCode.length(), NULL, NULL, (*tmpShaderInfo)[i].entryPoint.c_str(), profile.c_str(), NULL, &shaderByteCode, &error, NULL);
				}
				if (FAILED(hr)) {
					std::string errText;
					errText.resize(error->GetBufferSize() - 1);
					memmove(&errText[0], error->GetBufferPointer(), error->GetBufferSize() - 1);
					size_t pos = errText.find("):");
					if (pos + 1 < errText.length())
						errText = errText.erase(0, pos + 1);
					printf("Error in shader %s %s", GetFileName((*tmpShaderInfo)[i].filePath).c_str(), errText.c_str());
					system("pause");
					exit(0);
				}
				(*tmpFrag)[i].shaderSize = (*tmpFrag)[i].shaderSize2 = shaderByteCode->GetBufferSize();
				(*tmpFrag)[i].shader.bytecode.resize((*tmpFrag)[i].shaderSize);
				memmove(&(*tmpFrag)[i].shader.bytecode[0], shaderByteCode->GetBufferPointer(), (*tmpFrag)[i].shaderSize);
			}
			else {
				t.open(shaderPath, std::ios::binary);
				if (!t.is_open()) {
					printf("file on path %s not found\n", shaderPath.c_str());
					system("pause");
					exit(0);
				}
				t.seekg(0, std::ios::end);
				size = t.tellg();
				//std::vector<unsigned char> byteCode;
				(*tmpFrag)[i].shader.bytecode.resize(size);
				(*tmpFrag)[i].shaderSize = (*tmpFrag)[i].shaderSize2 = size;
				t.seekg(0);
				t.read((char*)&(*tmpFrag)[i].shader.bytecode[0], size);
				t.close();
			}
		}

	}
	fxc_file.pixelFragmentCount++;
	// получаем размер дл€ fxc шейдера
	unsigned int fxcSize = fxc_file.GetSize();

	unsigned char tmpChar;
	fxc_file.magic = 1635280754;

	std::vector<unsigned char> fxcFileBuffer(fxcSize * 2, 0xcd);
	memmove(&fxcFileBuffer[0], &fxc_file.magic, 0x4);
	std::vector<Fragment>* tmpFrag;
	unsigned posInFxc = 4;
	for (unsigned char x = 0; x < 2; x++) {
		if (x == 0) {
			tmpFrag = &fxc_file.vertexFragment;
		}
		else {
			tmpFrag = &fxc_file.pixelFragment;
		}
		tmpChar = (*tmpFrag).size();
		if (x != 0) tmpChar++;
		memmove(&fxcFileBuffer[posInFxc], &tmpChar, 0x1);
		posInFxc += 1;
		if (x != 0) {
			memmove(&fxcFileBuffer[posInFxc], &fxc_file.unk1, 0x5);
			posInFxc += 0x5;
		}
		for (unsigned char i = 0; i < (*tmpFrag).size(); i++) {
			memmove(&fxcFileBuffer[posInFxc], &(*tmpFrag)[i].variablesCount, 0x1);
			posInFxc++;
			for (unsigned char j = 0; j < (*tmpFrag)[i].variablesCount; j++) {
				memmove(&fxcFileBuffer[posInFxc], &(*tmpFrag)[i].variable[j].type, 0x4);
				posInFxc += 4;
				tmpChar = (*tmpFrag)[i].variable[j].name.length() + 1;
				memmove(&fxcFileBuffer[posInFxc], &tmpChar, 0x1);
				posInFxc ++;
				memmove(&fxcFileBuffer[posInFxc], (*tmpFrag)[i].variable[j].name.c_str(), (*tmpFrag)[i].variable[j].name.length() + 1);
				posInFxc += (*tmpFrag)[i].variable[j].name.length() + 1;
			}
			memmove(&fxcFileBuffer[posInFxc], &(*tmpFrag)[i].shaderSize, 0x4);
			posInFxc += 4;
			memmove(&fxcFileBuffer[posInFxc], &(*tmpFrag)[i].shader.bytecode[0], (*tmpFrag)[i].shaderSize2);
			posInFxc += (*tmpFrag)[i].shaderSize2;
		}
	}
	std::vector<Variable>* tmpVar;
	for (unsigned char x = 0; x < 2; x++) {
		if (x == 0) {
			tmpVar = &fxc_file.globalVariable;
		}
		else {
			tmpVar = &fxc_file.localVariable;
		}
		tmpChar = (*tmpVar).size();
		memmove(&fxcFileBuffer[posInFxc], &tmpChar, 0x1);
		posInFxc += 1;
		for (unsigned char i = 0; i < (*tmpVar).size(); i++) {
			memmove(&fxcFileBuffer[posInFxc], &(*tmpVar)[i].type, 0x2);
			posInFxc += 2;
			tmpChar = (*tmpVar)[i].name1.length() + 1;
			memmove(&fxcFileBuffer[posInFxc], &tmpChar, 0x1);
			posInFxc++;
			memmove(&fxcFileBuffer[posInFxc], (*tmpVar)[i].name1.c_str(), (*tmpVar)[i].name1.length() + 1);
			posInFxc += (*tmpVar)[i].name1.length() + 1;
			tmpChar = (*tmpVar)[i].name2.length() + 1;
			memmove(&fxcFileBuffer[posInFxc], &tmpChar, 0x1);
			posInFxc++;
			memmove(&fxcFileBuffer[posInFxc], (*tmpVar)[i].name2.c_str(), (*tmpVar)[i].name2.length() + 1);
			posInFxc += (*tmpVar)[i].name2.length() + 1;
			memmove(&fxcFileBuffer[posInFxc], &(*tmpVar)[i].annotationCount, 0x1);
			posInFxc++;
			for (unsigned char j = 0; j < (*tmpVar)[i].annotationCount; j++) {
				tmpChar = (*tmpVar)[i].annotation[j].name.length() + 1;
				memmove(&fxcFileBuffer[posInFxc], &tmpChar, 0x1);
				posInFxc++;
				memmove(&fxcFileBuffer[posInFxc], (*tmpVar)[i].annotation[j].name.c_str(), (*tmpVar)[i].name2.length() + 1);
				posInFxc += (*tmpVar)[i].annotation[j].name.length() + 1;
				memmove(&fxcFileBuffer[posInFxc], &(*tmpVar)[i].annotation[j].valueType, 0x1);
				posInFxc++;
				if ((*tmpVar)[i].annotation[j].valueType == 0) {
					memmove(&fxcFileBuffer[posInFxc], &(*tmpVar)[i].annotation[j].value.intValue, 0x4);
					posInFxc+=4;
				}
				else if ((*tmpVar)[i].annotation[j].valueType == 1) {
					memmove(&fxcFileBuffer[posInFxc], &(*tmpVar)[i].annotation[j].value.floatValue, 0x4);
					posInFxc += 4;
				}
				else if ((*tmpVar)[i].annotation[j].valueType == 2) {
					tmpChar = (*tmpVar)[i].annotation[j].value.stringValue.length() + 1;
					memmove(&fxcFileBuffer[posInFxc], &tmpChar, 0x1);
					posInFxc++;
					memmove(&fxcFileBuffer[posInFxc], (*tmpVar)[i].annotation[j].value.stringValue.c_str(), (*tmpVar)[i].annotation[j].value.stringValue.length() + 1);
					posInFxc += (*tmpVar)[i].annotation[j].value.stringValue.length() + 1;
				}
			}
			memmove(&fxcFileBuffer[posInFxc], &(*tmpVar)[i].valueCount, 0x1);
			posInFxc++;
			if ((*tmpVar)[i].valueCount != 0) {
				memmove(&fxcFileBuffer[posInFxc], &(*tmpVar)[i].value[0], (*tmpVar)[i].valueCount * 0x4);
				posInFxc+= (*tmpVar)[i].valueCount * 0x4;
			}
		}
	}
	memmove(&fxcFileBuffer[posInFxc], &fxc_file.techniquesCount, 0x1);
	posInFxc++;
	for (unsigned char i = 0; i < fxc_file.techniquesCount; i++) {
		tmpChar = fxc_file.technique[i].name.length() + 1;
		memmove(&fxcFileBuffer[posInFxc], &tmpChar, 0x1);
		posInFxc++;
		memmove(&fxcFileBuffer[posInFxc], fxc_file.technique[i].name.c_str(), tmpChar);
		posInFxc += tmpChar;
		memmove(&fxcFileBuffer[posInFxc], &fxc_file.technique[i].passCount, 0x1);
		posInFxc++;
		for (unsigned char j = 0; j < fxc_file.technique[i].passCount; j++) {
			memmove(&fxcFileBuffer[posInFxc], &fxc_file.technique[i].pass[j].vs, 0x2);
			posInFxc+=2;
			memmove(&fxcFileBuffer[posInFxc], &fxc_file.technique[i].pass[j].valueCount, 0x1);
			posInFxc++;
			if (fxc_file.technique[i].pass[j].valueCount != 0) {
				memmove(&fxcFileBuffer[posInFxc], &fxc_file.technique[i].pass[j].value[0].type, fxc_file.technique[i].pass[j].valueCount * 0x8);
				posInFxc+= fxc_file.technique[i].pass[j].valueCount * 0x8;
			}


		}
	}
	for (unsigned char i = 0; i < fxc_file.vertexFragmentCount; i++) {
		tmpChar = fxc_file.vertexFragment[i].name.length() + 1;
		memmove(&fxcFileBuffer[posInFxc], &tmpChar, 0x1);
		posInFxc++;
		memmove(&fxcFileBuffer[posInFxc], fxc_file.vertexFragment[i].name.c_str(), tmpChar);
		posInFxc += tmpChar;
	}
	for (unsigned char i = 0; i < fxc_file.pixelFragmentCount -1; i++) {
		tmpChar = fxc_file.pixelFragment[i].name.length() + 1;
		memmove(&fxcFileBuffer[posInFxc], &tmpChar, 0x1);
		posInFxc++;
		memmove(&fxcFileBuffer[posInFxc], fxc_file.pixelFragment[i].name.c_str(), tmpChar);
		posInFxc += tmpChar;
	}

	fxcFileBuffer.resize(posInFxc);
	std::ofstream fxcOut(GetFolderName(filePath) + '\\' + GetFileNameWithoutExtension(filePath) + ".fxc", std::ios::binary);
	fxcOut.write((char*)&fxcFileBuffer[0], fxcFileBuffer.size());
	fxcOut.close();
}