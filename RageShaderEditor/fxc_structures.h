#pragma once
#include <vector>
#include <string>
#include "ShaderModel3.h"

struct Base {
public:
	unsigned magic;

	Base() {};
};

class PassValue {
public:
	unsigned int type;
	unsigned int value;

	PassValue() {};
};

class Pass {
public:
	unsigned char vs;
	unsigned char ps;
	unsigned char valueCount;
	std::vector<PassValue> value;

	Pass() {};
};

class Technique {
public:
	std::string name;
	unsigned char passCount;
	std::vector<Pass> pass;

	Technique() {};
};

class AnnotationValue {
public:
	int intValue;       // 0
	float floatValue;   // 1
	std::string stringValue; // 2

	AnnotationValue() {};
};

class Annotation {
public:
	std::string name;
	unsigned char valueType;
	AnnotationValue value;

	Annotation() {};
};

//class VariableValue {
//public:
//	float floatValue;
//	int intValue;
//
//	VariableValue() {};
//};

class Variable {
public:
	unsigned char type;
	unsigned char arrayCount;
	std::string name1;
	std::string name2;
	unsigned char annotationCount;
	std::vector<Annotation> annotation;
	unsigned char valueCount;
	std::vector<unsigned> value;

	Variable() {};
};

class ShaderVariable {
public:
	unsigned short type;
	unsigned short index;
	std::string name;

	ShaderVariable() {};
};

class Fragment {
public:
	std::string name;			// в оригинальном шейдера нет имени, но для удобства я добавил.
	unsigned char variablesCount;
	std::vector<ShaderVariable> variable;
	unsigned short shaderSize;
	unsigned short shaderSize2;
	ShaderModel3::Shader shader;
	
	Fragment() {};
};

class Fxc_IV : public Base {
public:
	unsigned char vertexFragmentCount;
	std::vector<Fragment> vertexFragment;
	unsigned char pixelFragmentCount;
	unsigned char unk1;
	unsigned int unk2;
	std::vector<Fragment> pixelFragment;
	unsigned char globalVariablesCount;
	std::vector<Variable> globalVariable;
	unsigned char localVariablesCount;
	std::vector<Variable> localVariable;
	unsigned char techniquesCount;
	std::vector<Technique> technique;

	Fxc_IV() {};

	unsigned int GetSize();

};
