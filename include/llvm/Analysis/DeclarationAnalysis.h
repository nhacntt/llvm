#ifndef LLVM_ANALYSIS_DECLARATION_ANALYSIS_H
#define LLVM_ANALYSIS_DECLARATION_ANALYSIS_H

#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/ErrorOr.h>

#include <string>
#include <vector>

using namespace std;

namespace llvm {

struct Location {
	string SourceFile;
	unsigned LineNum;
	unsigned ColNum;
	
	//Constructors
	Location() = default;

	Location(const Location&) = default;

	Location(string SourceFile, unsigned LineNum, unsigned ColNum):
		SourceFile(SourceFile),
		LineNum(LineNum),
		ColNum(ColNum){};

};


struct Declaration {
	Location Loc;
	string Name;
	llvm::SmallVector<string,4> Annotations;
	
	//Constructors
	Declaration() = default;

	Declaration(const Declaration&) = default;

	Declaration(Location Loc, string Name, llvm::SmallVector<string,4> Annotations):
		Loc(Loc),
		Name(Name),
		Annotations(Annotations){};

};

//Information of a parameter's declaration
struct ParamDecl : public Declaration {
	string Type;
	
	//Constructor
	ParamDecl() = default;

	ParamDecl(const Declaration& D, string Type):
		Declaration(D),
		Type(Type){};

};

//Information of a function's declaration
struct FuncDecl : public Declaration {
	string MangledName;				//Mangled Name
	string RType;					//Return Type
	vector<ParamDecl> ParamVec;		//Function's parameters
	bool hasAnnotation;

	//Constructor
	FuncDecl()=default;
	
	FuncDecl(const Declaration& D, string MangledName, string RType, vector<ParamDecl> ParamVec):
		Declaration(D),
		MangledName(MangledName),
		RType(RType),
		ParamVec(ParamVec){

		hasAnnotation = !Annotations.empty();
		if (hasAnnotation) return;
		for (const auto& P:ParamVec){
			hasAnnotation = not P.Annotations.empty();
			if (hasAnnotation) return;
		}
	}
};

struct FuncDeclList{

	vector<FuncDecl> List;
	llvm::ErrorOr<FuncDeclList> fromYAML(string FileName);
	void toYAML(llvm::raw_ostream &OS);
};

} //namespace

#endif
