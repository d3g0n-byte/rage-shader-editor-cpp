#pragma once
#include <vector>
#include <string>
//#include "fxc_structures.h"


namespace ShaderModel3 {
	class CommentsLayout {
	public:
		std::vector<unsigned> pointer;
		std::vector<unsigned> size;
		std::vector<bool> usedPointer;

		void Add(unsigned int _size);
		void Add(std::string _str);
		void CreateLayout(unsigned startPos);
	};

	class Version {
	public:
		unsigned char version2;
		unsigned char version1;
	};

	class CommentsVarAttr
	{
		unsigned short type1;
		unsigned short type2;
		unsigned short count1;
		unsigned short count2;// основной
		unsigned short arrayCount;
		unsigned short _fa;
		unsigned short _fc;
	};

	class CommentsVariable {
	public:
		unsigned pName;
		unsigned short registerType;
		unsigned short index;
		unsigned short size;
		unsigned short _fa;
		unsigned pAttr;
		unsigned int _f14;
	
		std::string _name;
		CommentsVarAttr _attr;
	};
	class Comments {
	public:
		unsigned int _f0;
		unsigned pCreator; // = pos + 12
		Version version;
		unsigned short shaderType;
		unsigned int paramCount;
		unsigned int _f10;
		unsigned int _f14;
		unsigned pProfile; //
		unsigned pVariable;

		std::string _creator;
		std::string _profile; //
		std::vector<CommentsVariable> _variable;
	};

	class Shader: public Version {
	public:
		unsigned short shaderType;   // 0xffff - pixel, 0xfeffff - vertex
		unsigned short _f4;   // 0xfeffff
		unsigned short bytecodePosition;//= pos + 8
		unsigned CTAB;//
		Comments comments;
		std::vector<unsigned char> bytecode;

		Shader() {};

		//void BuildComments(std::vector<ShaderVariable> *variables);
	};
}