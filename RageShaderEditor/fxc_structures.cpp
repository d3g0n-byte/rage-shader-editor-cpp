#include "fxc_structures.h"

unsigned Fxc_IV::GetSize() {
	unsigned int retVal = 4; // magic
	std::vector<Fragment>* tmpFrag;
	retVal += 2; // fragCount
	for (char x = 0; x < 2; x++) {
		if (x == 0) {
			tmpFrag = &vertexFragment;
		}
		else {
			tmpFrag = &pixelFragment;
			retVal += 5; // two unk values
		}
		retVal += (*tmpFrag).size(); // vars count for each frag
		for (unsigned char i = 0; i < (*tmpFrag).size(); i++) {
			retVal += (*tmpFrag)[i].variablesCount * 0x5; // type & index of each var and name size
			for (unsigned char j = 0; j < (*tmpFrag)[i].variablesCount; j++) {
				retVal += (*tmpFrag)[i].variable[j].name.length() + 1;
			}
			retVal += (*tmpFrag)[i].shaderSize;
			retVal += 0x4; // shaderSize
		}
	}
	std::vector<Variable>* tmpVariable;
	retVal += 2; // varsCount
	for (char x = 0; x < 2; x++) {
		if (x == 0) {
			tmpVariable = &globalVariable;
		}
		else {
			tmpVariable = &localVariable;
		}
		retVal += 6 * (*tmpVariable).size(); // type, arrCount, name&name2 size, annotations&values sount
		for (unsigned char i = 0; i < (*tmpVariable).size(); i++) {
			retVal += (*tmpVariable)[i].name1.length() + 1;
			retVal += (*tmpVariable)[i].name2.length() + 1;
			for (unsigned char j = 0; j < (*tmpVariable)[i].annotation.size(); j++) {
				retVal += (*tmpVariable)[i].annotation[j].name.length() + 3; // name + size
				if ((*tmpVariable)[i].annotation[j].valueType == 0 || (*tmpVariable)[i].annotation[j].valueType == 1) { // num value
					retVal += 4;
				}
				else {
					retVal += 2 + (*tmpVariable)[i].annotation[j].value.stringValue.length(); // string value
				}
			}
			retVal += (*tmpVariable)[i].valueCount * 0x4;
		}
	}
	retVal++; // tecCount
	retVal += techniquesCount; // passesCount
	for (unsigned char i = 0; i < techniquesCount; i++) {
		retVal += technique[i].name.length() + 0x2;
		retVal += technique[i].passCount * 0x3; // vs, ps and paramCount
		for (unsigned char j = 0; j < technique[i].passCount; j++) {
			technique[i].pass[j].valueCount * 0x8;
		}
	}
	return retVal;
}