#include <stack>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Driver/Options.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/FrontendActions.h"
#include "llvm/Option/OptTable.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/Signals.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;
using namespace llvm::opt;

class DataType {
public:
  std::string kind;
  virtual void printType();
  virtual ~DataType() {};
};

void DataType::printType() {
  llvm::outs() << "No Type";
}

class VoidType : public DataType {
public:
  VoidType();
  void printType();
};

VoidType::VoidType() {
  kind = "Void";
}

void VoidType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"}";
}

class BoolType : public DataType {
public:
  BoolType();
  void printType();
};

BoolType::BoolType() {
  kind = "Bool";
}

void BoolType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"}";
}

class SignedType : public DataType {
public:
  bool constBool;
  bool volatileBool;
};

class UnsignedType : public DataType {
public:
  bool constBool;
  bool volatileBool;
};

class IntType : public SignedType {
public:
  IntType();
  void printType();
};

IntType::IntType() {
  kind = "IntType";
  constBool = false;
  volatileBool = false;
}

void IntType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << ":const \"true\"";
  if (volatileBool == true)
    llvm::outs() << ":volatileBool \"true\"";
  llvm::outs() << "}";
}

class UnsignedIntType : public UnsignedType {
public:
  UnsignedIntType();
  void printType();
};

UnsignedIntType::UnsignedIntType() {
  kind = "UnsignedIntType";
  constBool = false;
  volatileBool = false;
}

void UnsignedIntType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << ":const \"true\"";
  if (volatileBool == true)
    llvm::outs() << ":volatileBool \"true\"";
  llvm::outs() << "}";
}

class FloatType : public SignedType {
public:
  FloatType();
  void printType();
};

FloatType::FloatType() {
  kind = "FloatType";
  constBool = false;
  volatileBool = false;
}

void FloatType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << ":const \"true\"";
  if (volatileBool == true)
    llvm::outs() << ":volatileBool \"true\"";
  llvm::outs() << "}";
}

class UnsignedFloatType : public UnsignedType {
public:
  UnsignedFloatType();
  void printType();
};

UnsignedFloatType::UnsignedFloatType() {
  kind = "UnsignedFloatType";
  constBool = false;
  volatileBool = false;
}

void UnsignedFloatType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << ":const \"true\"";
  if (volatileBool == true)
    llvm::outs() << ":volatileBool \"true\"";
  llvm::outs() << "}";
}

class LongType : public SignedType {
public:
  LongType();
  void printType();
};

LongType::LongType() {
  kind = "LongType";
  constBool = false;
  volatileBool = false;
}

void LongType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << ":const \"true\"";
  if (volatileBool == true)
    llvm::outs() << ":volatileBool \"true\"";
  llvm::outs() << "}";
}

class UnsignedLongType : public UnsignedType {
public:
  UnsignedLongType();
  void printType();
};

UnsignedLongType::UnsignedLongType() {
  kind = "UnsignedLongType";
  constBool = false;
  volatileBool = false;
}

void UnsignedLongType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << ":const \"true\"";
  if (volatileBool == true)
    llvm::outs() << ":volatileBool \"true\"";
  llvm::outs() << "}";
}

class LongLongType : public SignedType {
public:
  LongLongType();
  void printType();
};

LongLongType::LongLongType() {
  kind = "LongLongType";
  constBool = false;
  volatileBool = false;
}

void LongLongType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << ":const \"true\"";
  if (volatileBool == true)
    llvm::outs() << ":volatileBool \"true\"";
  llvm::outs() << "}";
}

class UnsignedLongLongType : public UnsignedType {
public:
  UnsignedLongLongType();
  void printType();
};

UnsignedLongLongType::UnsignedLongLongType() {
  kind = "UnsignedLongLongType";
  constBool = false;
  volatileBool = false;
}

void UnsignedLongLongType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << ":const \"true\"";
  if (volatileBool == true)
    llvm::outs() << ":volatileBool \"true\"";
  llvm::outs() << "}";
}

class DoubleType : public SignedType {
public:
  DoubleType();
  void printType();
};

DoubleType::DoubleType() {
  kind = "DoubleType";
  constBool = false;
  volatileBool = false;
}

void DoubleType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << ":const \"true\"";
  if (volatileBool == true)
    llvm::outs() << ":volatileBool \"true\"";
  llvm::outs() << "}";
}

class UnsignedDoubleType : public UnsignedType {
public:
  UnsignedDoubleType();
  void printType();
};

UnsignedDoubleType::UnsignedDoubleType() {
  kind = "UnsingedDoubleType";
  constBool = false;
  volatileBool = false;
}

void UnsignedDoubleType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << ":const \"true\"";
  if (volatileBool == true)
    llvm::outs() << ":volatileBool \"true\"";
  llvm::outs() << "}";
}

class LongDoubleType : public SignedType {
public:
  LongDoubleType();
  void printType();
};

LongDoubleType::LongDoubleType() {
  kind = "LongDoubleType";
  constBool = false;
  volatileBool = false;
}

void LongDoubleType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << ":const \"true\"";
  if (volatileBool == true)
    llvm::outs() << ":volatileBool \"true\"";
  llvm::outs() << "}";
}

class ShortType : public SignedType {
public:
  ShortType();
  void printType();
};

ShortType::ShortType() {
  kind = "ShortType";
  constBool = false;
  volatileBool = false;
}

void ShortType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << ":const \"true\"";
  if (volatileBool == true)
    llvm::outs() << ":volatileBool \"true\"";
  llvm::outs() << "}";
}

class UnsignedShortType : public UnsignedType {
public:
  UnsignedShortType();
  void printType();
};

UnsignedShortType::UnsignedShortType() {
  kind = "UnsingedShortType";
  constBool = false;
  volatileBool = false;
}

void UnsignedShortType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << ":const \"true\"";
  if (volatileBool == true)
    llvm::outs() << ":volatileBool \"true\"";
  llvm::outs() << "}";
}

class CharType : public SignedType {
public:
  CharType();
  void printType();
};

CharType::CharType() {
  kind = "CharType";
  constBool = false;
  volatileBool = false;
}

void CharType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << ":const \"true\"";
  if (volatileBool == true)
    llvm::outs() << ":volatileBool \"true\"";
  llvm::outs() << "}";
}

class UnsignedCharType : public UnsignedType {
public:
  UnsignedCharType();
  void printType();
};

UnsignedCharType::UnsignedCharType() {
  kind = "UnsignedChartType";
  constBool = false;
  volatileBool = false;
}

void UnsignedCharType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << ":const \"true\"";
  if (volatileBool == true)
    llvm::outs() << ":volatileBool \"true\"";
  llvm::outs() << "}";
}

class PointeeType : public DataType {
public:
  DataType *pointee;
  PointeeType();
  void printType();
};

PointeeType::PointeeType() {
  kind = "PointerType";
}

void PointeeType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\" :Pointee ";
  pointee->printType();
  llvm::outs() << "}";
}

class FuncType : public DataType {
public:
  std::vector<DataType*> parmType;
  DataType *retType;
  FuncType();
  void printType();
};

FuncType::FuncType() {
  kind = "FuncType";
}

void FuncType::printType() {
  int i;
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (parmType.size() != 0 ) {
    llvm::outs() << " :parms-type [";
    for (i = 0; i < (int)parmType.size() - 1; i++) {
      parmType[i]->printType();
      llvm::outs() << " ";
    }
    parmType[(int)parmType.size() - 1]->printType();
    llvm::outs() << "]";
  }
  llvm::outs() << " :ret-type ";
  retType->printType();
  llvm::outs() << "}";
}

class ArrayDataType : public DataType {
public:
  int arraySize;
  DataType *type;
  ArrayDataType();
  void printType();
};

ArrayDataType::ArrayDataType() {
  kind = "ArrayType";
}

void ArrayDataType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\" :array-size " << arraySize << " :type ";
  type->printType();
  llvm::outs() << "}";
}

class StructureType : public DataType {
public:
  std::string name;
  StructureType();
  void printType();
};

StructureType::StructureType() {
  kind = "StructureType";
}

void StructureType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name << "\"}";
}

class UnionType : public DataType {
public:
  std::string name;
  UnionType();
  void printType();
};

UnionType::UnionType() {
  kind = "UnionType";
}

void UnionType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name << "\"}";
}

class RenameType : public DataType {
public:
  std::string typeName;
  DataType *typedefType;
  RenameType();
  void printType();
};

RenameType::RenameType() {
  kind = "TypedefType";
}

void RenameType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\" :type-name \"" << typeName << "\" :typedef-type ";
  typedefType->printType();
  llvm::outs() << "}";
}

class Node {
public:
  std::string kind;
  std::string beginFile;
  int beginLine;
  int beginColumn;
  std::string endFile;
  int endLine;
  int endColumn;
  void PrintLocation();
  virtual void printAST();
  virtual ~Node() {};
};

void Node::PrintLocation() {
  llvm::outs() << " :loc-begin [\"" << beginFile << "\" " << beginLine << " " << beginColumn 
  	   << "] :loc-end [\"" << endFile << "\" " << endLine << " " << endColumn << "]";
}

void Node::printAST() {
  llvm::outs() << "No Kind";
}

class Expression : public Node {
public:
  std::vector<DataType> type;
};

class Reference : public Expression {
public:
  std::string name;
};

class DeclationReferenceExpression : public Reference {
public:
  std::string scope;
  DeclationReferenceExpression();
  
};

DeclationReferenceExpression::DeclationReferenceExpression() {
  kind = "DRE";
}

class MemberReference : public Reference {
public:
  std::string scope;
  MemberReference();
  
};

MemberReference::MemberReference() : scope("member") {
  kind = "Field";
}

class StructReference : public Reference {
public:
  std::vector<DeclationReferenceExpression> structs;
  MemberReference structMember;
};

class Operator : public Expression {
};

class BinOp : public Operator {
public:
  std::string op;
  std::vector<Expression> left;
  std::vector<Expression> right;
  BinOp();
  
};

BinOp::BinOp() {
  kind = "BinOp";
}

class UnOp : public Operator {
public:
  std::string op;
  std::vector<Expression> operand;
  UnOp();
  
};

UnOp::UnOp() {
  kind = "UnOp";
}

class ConditionalOp : public Operator {
public:
  BinOp condition;
  std::vector<Operator> then;
  std::vector<Operator> denial;
  ConditionalOp();
  
};

ConditionalOp::ConditionalOp() {
  kind = "ConditionalOp";
}

class Literal : public Expression {
public:
};

class IntLiteral : public Literal {
public:
  int value;
  IntLiteral();
  
};

IntLiteral::IntLiteral() {
  kind = "IntegerLiteral";
}

class CharLiteral : public Literal {
public:
  char value;
  CharLiteral();
  
};

CharLiteral::CharLiteral() {
  kind = "CharacterLiteral";
}

class FloatLiteral : public Literal {
public:
  double value;
  FloatLiteral();
  
};

FloatLiteral::FloatLiteral() {
  kind = "FloatingLiteral";
}

class ArrayReference : public Reference {
public:
  DeclationReferenceExpression array;
  Literal index;
  ArrayReference();
  
};

ArrayReference::ArrayReference() {
  kind = "ArrayRef";
}

class FunctionCall : public Expression {
public:
  DeclationReferenceExpression func;
  std::vector<DeclationReferenceExpression> parm;
  FunctionCall();
  
};

FunctionCall::FunctionCall() {
  kind = "FuncCall";
}

class Declation : public Node {
public:
  std::string name;
};

class FieldDeclation : public Declation {
public:
  std::string scope;
  DataType type;

  FieldDeclation();
  
};

FieldDeclation::FieldDeclation() : scope("member") {
  kind = "FieldDecl";
}

class ObjectDeclation : public Declation {
public:
  bool autoBool;
  bool staticBool;
  DataType *type;
  std::string displayType;
};

class DeclationOfVariables : public ObjectDeclation {
public:
  std::string scope;
};

class ParameterDeclation : public DeclationOfVariables {
public:
  ParameterDeclation();
};

ParameterDeclation::ParameterDeclation() {
  kind = "ParmDecl";
  autoBool = false;
  staticBool = false;
}


class VariableDeclation : public DeclationOfVariables {
public:
  Expression *init;
  VariableDeclation();
  void printAST();
};

VariableDeclation::VariableDeclation() {
  kind = "VarDecl";
  autoBool = false;
  staticBool = false;
}

void VariableDeclation::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name << "\" :scope \"" << scope << "\"";
  if (autoBool == true) {
    llvm::outs() << ":auto \"true\"";
  }
  if (staticBool == true) {
    llvm::outs() << ":static \"true\"";
  }
  llvm::outs() << " :display-type \"" << displayType << "\" :type ";
  type->printType();
  PrintLocation();
  llvm::outs() << "}\n";
  //if (!init) {
  //  init->printAST();
  //}
}

class FunctionDeclation : public Declation {
public:
  std::vector<ParameterDeclation> parm;
  std::vector<Node> body;
  FunctionDeclation();
  void printAST();
};

FunctionDeclation::FunctionDeclation() {
  kind = "FuncDecl";
}

void FunctionDeclation::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name << "\"";
  PrintLocation();
  llvm::outs() << "}\n";
}

class TypeDeclation : public Declation {
public:
  std::vector<FieldDeclation> member;
};

class StructDeclation : public TypeDeclation {
public:
  StructDeclation();
};

StructDeclation::StructDeclation() {
  kind = "StructDecl";
}

class UnionDeclation : public TypeDeclation {
public:
  UnionDeclation();
};

UnionDeclation::UnionDeclation() {
  kind = "UnionDecl";
}

class Statement : public Node {
};

class RepetitionStatement : public Statement {
public:
  Expression condition;
  std::vector<Expression> body;
};

class WhileStatement : public RepetitionStatement {
public:
  WhileStatement();
};

WhileStatement::WhileStatement() {
  kind = "While";
}

class DoStatement : public RepetitionStatement {
public:
  DoStatement();
};

DoStatement::DoStatement() {
  kind = "Do";
}

class ForStatement : public RepetitionStatement {
public:
  std::vector<Operator> init;
  std::vector<Operator> update;
  ForStatement();
};

ForStatement::ForStatement() {
  kind = "For";
}

class BranchStatement : public Statement {
public:
  Expression condition;
};

class IfStatement : public BranchStatement {
public:
  std::vector<Expression> then;
  std::vector<Expression> denial;
  IfStatement();
};

IfStatement::IfStatement() {
  kind = "If";
}

class SwitchStatement : public BranchStatement {
public:
  std::vector<Expression> body;
  SwitchStatement();
};

SwitchStatement::SwitchStatement() {
  kind = "switch";
}

class LabelStatement : public Statement {
public:
  LabelStatement();
};

LabelStatement::LabelStatement() {
  kind = "Label";
}

class CaseStatement : public Statement {
public:
  Literal value;
  CaseStatement();
};

CaseStatement::CaseStatement() {
  kind = "Case";
}

class GotoStatement : public Statement {
public:
  std::string jump;
  GotoStatement();
};

GotoStatement::GotoStatement() {
  kind = "Goto";
}

class ContinueStatement : public Statement {
public:
  ContinueStatement();
};

ContinueStatement::ContinueStatement() {
  kind = "Continue";
}

class BreakStatement : public Statement {
public:
  BreakStatement();
};

BreakStatement::BreakStatement() {
  kind = "Break";
}

class ReturnStatement : public Statement {
public:
  Expression value;
  ReturnStatement();
};

ReturnStatement::ReturnStatement() {
  kind = "Ret";
}

class MyAstVisitor : public RecursiveASTVisitor<MyAstVisitor> {
private:
  std::vector<std::string> order;
  std::vector<Node *> prog;
  std::vector<Declation *> DpArray;
  std::vector<Expression *> EpArray;
  std::vector<Statement *> SpArray;

public:
  explicit MyAstVisitor(ASTContext *Context, llvm::StringRef InFile) : Context(Context), source_file(InFile) {}
  // TraverseDecl
  bool TraverseDecl(Decl *decl) {
    if (decl != NULL){
      //llvm::outs() << " ?Decl? " << decl->getDeclKindName() << "\n";
      switch (decl->getKind()) {
      case Decl::Field:
        //llvm::outs() << "decl:1";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseDecl(decl);
	break;
      case Decl::Function: 
        //llvm::outs() << "decl:2";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseDecl(decl);
	break;
      case Decl::ParmVar: 
        //llvm::outs() << "decl:3";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseDecl(decl);
	break;
      case Decl::TranslationUnit: 
        //llvm::outs() << "decl:4";
	linefeedflag = 0;
	linefeedbody = 0;
	caseflag = 0;
	casetoji = 0;
	castflag = 0;
	labelflag = 0;
	caselabel = "";
	skipcount = 0;
	RecursiveASTVisitor::TraverseDecl(decl);
	break;
      case Decl::Typedef: 
        //llvm::outs() << "decl:5";
	RecursiveASTVisitor::TraverseDecl(decl);
	break;
      case Decl::Record: 
        //llvm::outs() << "decl:6";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseDecl(decl);
	break;
      case Decl::Var: 
        //llvm::outs() << "decl:7";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseDecl(decl);
	break;
      case Decl::Enum: 
        //llvm::outs() << "decl:8";
	RecursiveASTVisitor::TraverseDecl(decl);
	break;
      case Decl::EnumConstant: 
        //llvm::outs() << "decl:9";
	RecursiveASTVisitor::TraverseDecl(decl);
	break;
	//以下c++関係のもの
      case Decl::ClassTemplate: 
        //llvm::outs() << "decl:10";
      case Decl::CXXRecord: 
        //llvm::outs() << "decl:11";
      case Decl::CXXMethod: 
        //llvm::outs() << "decl:12";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseDecl(decl);
	break;
      case Decl::ClassTemplateSpecialization: 
        //llvm::outs() << "decl:13";
	//RecursiveASTVisitor::TraverseDecl(decl);
	//	llvm::outs() << " ?Decl? " << decl->getDeclKindName();
	break;
      case Decl::ClassTemplatePartialSpecialization: 
        //llvm::outs() << "decl:14";
      case Decl::CXXConstructor: 
        //llvm::outs() << "decl:15";
      case Decl::CXXConversion: 
        //llvm::outs() << "decl:16";
      case Decl::CXXDestructor: 
        //llvm::outs() << "decl:17";
      case Decl::Empty: 
        //llvm::outs() << "decl:18";
      case Decl::FunctionTemplate: 
        //llvm::outs() << "decl:19";
      case Decl::LinkageSpec: 
        //llvm::outs() << "decl:20";
      case Decl::Namespace: 
        //llvm::outs() << "decl:21";
      case Decl::NonTypeTemplateParm: 
        //llvm::outs() << "decl:22";
      case Decl::TemplateTypeParm: 
        //llvm::outs() << "decl:23";
      case Decl::Using: 
        //llvm::outs() << "decl:24";
      case Decl::UsingDirective: 
        //llvm::outs() << "decl:25";
      case Decl::UsingShadow: 
        //llvm::outs() << "decl:26";
	//llvm::outs() << " ?Decl? " << decl->getDeclKindName();
	RecursiveASTVisitor::TraverseDecl(decl);
	break;
      default : 
        //llvm::outs() << "decl:27";
	llvm::outs() << " ?Decl? " << decl->getDeclKindName();
	RecursiveASTVisitor::TraverseDecl(decl);
      }
    }
    return true;
  }
  
  // ClassTemplate
  bool VisitClassTemplateDecl(ClassTemplateDecl *ctdecl) {
    llvm::outs() << "{:kind \"ClassTemplate\"";
    llvm::outs() << " :Template ";
    TraverseDecl(ctdecl->getTemplatedDecl());
    llvm::outs() << "}";
    return false;
  }

  //
  // FieldDecl (Structure Member) 
  bool VisitFieldDecl(FieldDecl *field) {
    QualType fieldtype = field->getType();
    if (labelflag != 0) {
      os << "{:kind \"Field\""
	 << " :name " << "\"" << (std::string)field->getName() << "\""
	 << " :scope " << "\"member\"";
      os << " :type [";
      PrintTypeInfo(fieldtype);
      checkCast();
      os << "]";
      PrintSourceRange(field->getSourceRange());
      os << "}";
      caselabel += os.str();
      os.str("");
      os.clear();
    } else {
      llvm::outs() << "{:kind \"Field\""
		   << " :name " << "\"" << field->getName() << "\""
		   << " :scope " << "\"member\"";
      llvm::outs() << " :type [";
      PrintTypeInfo(fieldtype);
      checkCast();
      llvm::outs() << "]";
      PrintSourceRange(field->getSourceRange());
      llvm::outs() << "}";
    }
    return false;
  }
  
  // FunctionDecl
  bool VisitFunctionDecl(FunctionDecl *Decl) {
    Node *np;
    FunctionDeclation *FD = new FunctionDeclation();
    if (dyn_cast<CXXMethodDecl>(Decl)) {
      CXXMethodDecl *method = dyn_cast<CXXMethodDecl>(Decl);
      last_func = method->getQualifiedNameAsString();
      QualType functype = method->getResultType();
      llvm::outs() << "{:kind \"Method\""
		   << " :name " << "\"" << last_func << "\"";
      checkSpecifier(method->getStorageClass());
      llvm::outs() << " :type [";
      PrintTypeInfo(functype);
      checkCast();
      llvm::outs() << "]";
      PrintSourceRange(method->getSourceRange());
      llvm::outs() << "\n :Parm [";
      if (method->param_size()) {
	linefeedflag = 0;
	for (int i = 0; i < (int)method->param_size(); i++) {
	  TraverseDecl(method->getParamDecl(i));
	}
      }
      llvm::outs() << "]\n :body [";
      linefeedflag = 0;
      linefeedbody = 0;
      TraverseStmt(method->getBody());
      llvm::outs() << "]}";
    } else {
      last_func = Decl->getQualifiedNameAsString();
      QualType functype = Decl->getType();
      llvm::outs() << "{:kind \"Funcdef\""
		   << " :name " << "\"" << last_func << "\"";
      checkSpecifier(Decl->getStorageClass());
      llvm::outs() << " :type [";
      PrintTypeInfo(functype);
      checkCast();
      llvm::outs() << "]";
      PrintSourceRange(Decl->getSourceRange());
      llvm::outs() << "\n :Parm [";
      if (Decl->param_size()) {
	linefeedflag = 0;
	for (int i = 0; i < (int)Decl->param_size(); i++) {
	  TraverseDecl(Decl->getParamDecl(i));
	}
      }
      llvm::outs() << "]\n :body [";
      linefeedflag = 0;
      linefeedbody = 0;
      TraverseStmt(Decl->getBody());
      llvm::outs() << "]}";
    }

    // 修正版
    order.push_back(FD->kind);
    FD->name = Decl->getQualifiedNameAsString();
    Node t = PrintSourceRange(Decl->getSourceRange());
    FD->beginFile = t.beginFile;
    FD->beginLine = t.beginLine;
    FD->beginColumn = t.beginColumn;
    FD->endFile = t.endFile;
    FD->endLine = t.endLine;
    FD->endColumn = t.endColumn;
    //FuncDeclArray.push_back(FD);
    np = FD;
    prog.push_back(np);

    return false;
  }

  // ParmVarDecl
  bool VisitParmVarDecl(ParmVarDecl *Decl) {
    std::string varname = Decl->getNameAsString();
    QualType vartype = Decl->getType();
    llvm::outs() << "{:kind \"Parm\"" 
		 << " :name " << "\"" << varname  << "\"";
    llvm::outs() << " :type [";
    PrintTypeInfo(vartype);
    checkCast();
    llvm::outs() << "]";
    PrintSourceRange(Decl->getSourceRange());
    llvm::outs() << "}";
    return true;
  }

  // RecordDecl (Structure?)
  bool VisitRecordDecl(RecordDecl *record) {
    std::string recordkind;
    if (record->isStruct()) {
      recordkind = "\"Structdef\"";
    } else if (record->isUnion()) {
      recordkind = "\"Uniondef\"";
    } else if (record->isClass()) {//C++
      CXXRecordDecl *cxxrecord = dyn_cast<CXXRecordDecl>(record);
      llvm::outs() << "{:name " << "\"" << (std::string)record->getName() << "\"";
      PrintSourceRange(record->getSourceRange()); 
      llvm::outs() << "\n :Member [";
      linefeedflag = 0;
      if (!(record->field_empty())) {
	RecordDecl::field_iterator itr = record->field_begin();
	while (itr != record->field_end()) {
	  TraverseDecl(itr->getCanonicalDecl());
	  itr++;
	} 
      }
      if (cxxrecord->method_begin() != cxxrecord->method_end()) {
	CXXRecordDecl::method_iterator mitr = cxxrecord->method_begin();
	while (mitr != cxxrecord->method_end()) {
	  TraverseDecl(mitr->getCanonicalDecl());
	  mitr++;
	}
      }
      llvm::outs() << "]}";
    }
    if (!(record->isClass())) {
      llvm::outs() << "{:kind " << recordkind
		   << " :name " << "\"" << (std::string)record->getName() << "\"";
      PrintSourceRange(record->getSourceRange()); 
      llvm::outs() << "\n :Member [";
      linefeedflag = 0;
      if (!(record->field_empty())) {
	RecordDecl::field_iterator itr = record->field_begin();
	while (itr != record->field_end()) {
	  TraverseDecl(itr->getCanonicalDecl());
	  itr++;
	} 
      }
    llvm::outs() << "]}";
    }
    return false;
  }

  // VarDecl
  bool VisitVarDecl(VarDecl *Decl) {
    //VariableDeclation VD;
    Node *np;
    VariableDeclation *VD = new VariableDeclation();
    // 今までの出力
    if (Decl->getKind() == 50) {
      return true;
    }
    std::string kindname = "Var";
    std::string varname = Decl->getNameAsString();
    QualType vartype = Decl->getType();
    std::string arrsize;
    llvm::outs() << "{" << ":kind "<< "\"" << kindname << "\""  
		 << " :name " << "\"" << varname << "\"" 
		 << " :scope " << (Decl->isFileVarDecl() == 1? "\"global\"":"\"local\"");
    checkSpecifier(Decl->getStorageClass());
    PrintDisplayType(vartype);
    llvm::outs() << " :type [";
    PrintTypeInfo(vartype);
    checkCast();
    llvm::outs() << "]";
    PrintSourceRange(Decl->getSourceRange());
    if (Decl->hasInit()) {
      llvm::outs() << "\n :init ";
      linefeedflag = 0;
      TraverseStmt(Decl->getInit());
    }
    llvm::outs() << "}";

    // 修正版
    order.push_back(VD->kind);
    VD->name = Decl -> getNameAsString();
    VD->scope = (Decl -> isFileVarDecl() == 1 ? "global" : "local");
    //VD.autoBool = PrintAutoTypeInfo(vartype);
    VD->type = PrintTypeInfo(vartype);
    VD->displayType = PrintDisplayType(vartype);
    if (Decl->hasInit()) {
      //TraverseStmt(Decl->getInit());
      //VD->init = EpArray[EpArray.begin()];
      //EpArray.pop_back(EpArray.begin());
    }
    Node t = PrintSourceRange(Decl->getSourceRange());
    VD->beginFile = t.beginFile;
    VD->beginLine = t.beginLine;
    VD->beginColumn = t.beginColumn;
    VD->endFile = t.endFile;
    VD->endLine = t.endLine;
    VD->endColumn = t.endColumn;
    //VarDeclArray.push_back(VD);
    np = VD;
    prog.push_back(np);

    return false;
  }
  
  // 指定子:specifire::=(register, static, extern)
  void checkSpecifier(StorageClass SC) {
    if (labelflag != 0) {
      switch(SC) {
      case SC_Extern:
	os << " :Extern true";
	break;
      case SC_Static:
	os << " :Static true";
	break;
      case SC_Auto:
	os << " :Auto true";
	break;
      case SC_Register:
	os << " :Register true";
	break;
      case SC_None:
      case SC_PrivateExtern:
      case SC_OpenCLWorkGroupLocal:
      default:
	break;
      }
      caselabel += os.str();
      os.str("");
      os.clear();
    } else if (castflag != 0) {
      switch(SC) {
      case SC_Extern:
	cast << " :Extern true";
	break;
      case SC_Static:
	cast << " :Static true";
	break;
      case SC_Auto:
	cast << " :Auto true";
	break;
      case SC_Register:
	cast << " :Register true";
	break;
      case SC_None:
      case SC_PrivateExtern:
      case SC_OpenCLWorkGroupLocal:
      default:
	break;
      }
      castlabel += cast.str();
      cast.str("");
      cast.clear();
    } else {
      switch(SC) {
      case SC_Extern:
	llvm::outs() << " :Extern \"true\"";
	break;
      case SC_Static:
	llvm::outs() << " :Static \"true\"";
	break;
      case SC_Auto:
	llvm::outs() << " :Auto \"true\"";
	break;
      case SC_Register:
	llvm::outs() << " :Register \"true\"";
	break;
      case SC_None:
      case SC_PrivateExtern:
      case SC_OpenCLWorkGroupLocal:
      default:
	break;
      }
    }
  }
  
  // typeを文字列として出力
  //void PrintDisplayType(QualType typeInfo) {
  std::string PrintDisplayType(QualType typeInfo) {
    if (labelflag != 0 || castflag != 0) {
      os << " :DisplayType " << "\"" << typeInfo.getAsString() << "\"";
    } else {
      llvm::outs() << " :DisplayType " << "\"" << typeInfo.getAsString() << "\"";
    }

    return typeInfo.getAsString();
  }
 
  
  void PrintAutoTypeInfo(QualType typeInfo) {
  //bool PrintAutoTypeInfo(QualType typeInfo) {
    assert(labelflag == 0);
    if (castflag != 0) {
      //cast << " :Auto \"true\"";
      PrintQualifier(typeInfo);
      PrintTypeInfo(dyn_cast<AutoType>(typeInfo)->getDeducedType());
      //castlabel += cast.str();
      //cast.str("");
      //cast.clear();
     // return true;
    } else {
      //llvm::outs() << " :Auto \"true\"";
      PrintQualifier(typeInfo);
      PrintTypeInfo(dyn_cast<AutoType>(typeInfo)->getDeducedType());
     // return true;
    }
  }

  //void PrintTypedefTypeInfo(QualType typeInfo) {
  RenameType *PrintTypedefTypeInfo(QualType typeInfo) {
    RenameType *TDF = new RenameType();
    TypedefNameDecl *TDtype = dyn_cast<TypedefType>(typeInfo)->getDecl();
    assert(labelflag == 0);
    if (castflag != 0) {
      cast << "{:kind \"Typedef-type\""
	   << " :typename " << "\"" << (std::string)TDtype->getName() << "\""
	   << " :typedeftype ";
      PrintTypeInfo(TDtype->getUnderlyingType());
      PrintQualifier(typeInfo);
      cast << "}";
      castlabel += cast.str();
      cast.str("");
      cast.clear();
    } else {
      llvm::outs() << "{:kind \"Typedef-type\""
		   << " :typename " << "\"" << TDtype->getName() << "\""
		   << " :typedeftype ";
      PrintTypeInfo(TDtype->getUnderlyingType());
      PrintQualifier(typeInfo);
      llvm::outs() << "}";
    }

    TDF->typeName = (std::string)TDtype->getName();
    TDF->typedefType = PrintTypeInfo(TDtype->getUnderlyingType());

    return TDF;
  }

  //void PrintBuiltinTypeInfo(QualType typeInfo) {
  DataType *PrintBuiltinTypeInfo(QualType typeInfo) {
    std::string Typename;
    DataType *t;
    switch (dyn_cast<BuiltinType>(typeInfo)->getKind()) {
      case BuiltinType::Void:
      {
	Typename = "Void";
	t = new VoidType();
        return t;
      }
      case BuiltinType::Bool:
      {
	Typename = "Bool";
	t = new BoolType();
        return t;
      }
      case BuiltinType::UChar:
      {
	Typename = "UnsignedChar";
	t = new UnsignedCharType();
        return t;
      }
      case BuiltinType::UShort:
      {
	Typename = "UnsignedShort";
	t = new UnsignedShortType();
        return t;
      }
      case BuiltinType::UInt:
      {
	Typename = "UnsignedInt";
	t = new UnsignedIntType();
	return t;
      }
      case BuiltinType::ULong:
      {
	Typename = "UnsignedLong";
	t = new UnsignedLongType();
	return t;
      }
      case BuiltinType::ULongLong:
      {
	Typename = "UnsigndLongLong";
	t = new UnsignedLongLongType();
	return t;
      }
      case BuiltinType::Char_S:
      {
	Typename = "Char";
	t = new CharType();
	return t;
      }
      case BuiltinType::SChar:
      {
	Typename = "SignedChar";
	t = new CharType();
	return t;
      }
      case BuiltinType::Short:
      {
	Typename = "Short";
	t = new ShortType();
	return t;
      }
      case BuiltinType::Int:
      {
	Typename = "Int";
	t = new IntType();
	return t;
      }
      case BuiltinType::Long:
      {
	Typename = "Long";
	t = new LongType();
	return t;
      }
      case BuiltinType::LongLong:
      {
	Typename = "LongLong";
	t = new LongLongType();
	return t;
      }
      case BuiltinType::Float:
      {
	Typename = "Float";
	t = new FloatType();
	return t;
      }
      case BuiltinType::Double:
      {
	Typename = "Double";
	t = new DoubleType();
	return t;
      }
      case BuiltinType::LongDouble:
      {
	Typename = "LongDouble";
	t = new LongDoubleType();
	return t;
      }
	//以下c++に関するもの
      case BuiltinType::WChar_S:
      case BuiltinType::WChar_U:
      {
	Typename = "WChar_t";
	break;
      }
      case BuiltinType::Dependent:
      {
	Typename = "Dependent";
	break;
      }
      default:
      {
	Typename = "UnKnownError";
	llvm::outs() << "\n \""<< typeInfo.getAsString() << "\"は, 初出です."
		     << " astprint.cppのPrintTypeInfoにcaseを追加して下さい.";
	break;
      }
    }
    assert(labelflag == 0);
    if (castflag != 0) {
      cast << "{:kind \"" << Typename << "-type\"";
      PrintQualifier(typeInfo);
      cast << "}";
      castlabel += cast.str();
      cast.str("");
      cast.clear();
    } else {
      llvm::outs() << "{:kind \"" << Typename << "-type\"";
      PrintQualifier(typeInfo);
      llvm::outs() << "}";
    }

    return t;
  }

  //void PrintFunctionTypeInfo(QualType typeInfo) {
  FuncType *PrintFunctionTypeInfo(QualType typeInfo) {
    assert(labelflag == 0);
    FuncType *t = new FuncType();
    if (castflag != 0) {
      cast << "{:kind \"Func-type\""
	   << " :ParmsType [";
      if (dyn_cast<FunctionProtoType>(typeInfo)) {
	unsigned parms = dyn_cast<FunctionProtoType>(typeInfo)->getNumArgs();
	if (parms != 0) {
	  unsigned i = 0;
	  while (i < parms) {
	    if (i != 0) {
	      cast << " ";
	    }
	    t->parmType.push_back(PrintTypeInfo(dyn_cast<FunctionProtoType>(typeInfo)->getArgType(i)));
	    i++;
	  }
	}
      }
      cast << "]";
      if (dyn_cast<FunctionType>(typeInfo)) {
	cast << " :RetType ";
	QualType rettype = dyn_cast<FunctionType>(typeInfo)->getResultType();
	t->retType = PrintTypeInfo(rettype);
      } 
      cast << "}";
      castlabel += cast.str();
      cast.str("");
      cast.clear();
    } else {
      llvm::outs() << "{:kind \"Func-type\""
		   << " :ParmsType [";
      if (dyn_cast<FunctionProtoType>(typeInfo)) {
	unsigned parms = dyn_cast<FunctionProtoType>(typeInfo)->getNumArgs();
	if (parms != 0) {
	  unsigned i = 0;
	  while (i < parms) {
	    if (i != 0) {
	      llvm::outs() << " ";
	    }
	    t->parmType.push_back(PrintTypeInfo(dyn_cast<FunctionProtoType>(typeInfo)->getArgType(i)));
	    i++;
	  }
	}
      }
      llvm::outs() << "]";
      if (dyn_cast<FunctionType>(typeInfo)) {
	llvm::outs() << " :RetType ";
	QualType rettype = dyn_cast<FunctionType>(typeInfo)->getResultType();
	t->retType = PrintTypeInfo(rettype);
      } 
      llvm::outs() << "}";
    }

    return t;
  }

  //void PrintArrayTypeInfo(QualType typeInfo) {
  ArrayDataType *PrintArrayTypeInfo(QualType typeInfo) {
    QualType elmtype;
    ArrayDataType *t = new ArrayDataType();
    if (dyn_cast<ArrayType>(typeInfo)) {
      elmtype = dyn_cast<ArrayType>(typeInfo)->getElementType();
      assert(labelflag == 0);
      if (castflag != 0) {
	cast << "{:kind \"Array-type\"";
	if (dyn_cast<ConstantArrayType>(typeInfo)) {
	  cast << " :Arraysize \"" 
	       << dyn_cast<ConstantArrayType>(typeInfo)->getSize().toString(10, true) << "\"";
          //std::string index = dyn_cast<ConstantArrayType>(typeInfo)->getSize().toString(10, true);
          //t.arraySize = atoi(index);
	}
	if (dyn_cast<VariableArrayType>(typeInfo)) {
	  Expr *vaexpr = dyn_cast<VariableArrayType>(typeInfo)->getSizeExpr();
	  cast << " :ArraySizeExpression ";
	  linefeedflag = 0;
	  TraverseStmt(vaexpr);
	}
	PrintQualifier(typeInfo);
	cast << " :type [";
	t->type = PrintTypeInfo(elmtype);
	cast << "]}";
	castlabel += cast.str();
	cast.str("");
	cast.clear();
      } else {
	llvm::outs()  << "{:kind \"Array-type\"";
	if (dyn_cast<ConstantArrayType>(typeInfo)) {
	  llvm::outs() << " :ArraySize \"" 
		       << dyn_cast<ConstantArrayType>(typeInfo)->getSize() << "\"";
          //std::string index  = dyn_cast<ConstantArrayType>(typeInfo)->getSize().toString(10, true);
          //t.arraySize = std::stoi(index);
	}
	if (dyn_cast<VariableArrayType>(typeInfo)) {
	  Expr *vaexpr = dyn_cast<VariableArrayType>(typeInfo)->getSizeExpr();
	  llvm::outs() << " :ArraySizeExpression ";
	  linefeedflag = 0;
	  TraverseStmt(vaexpr);
	}
	PrintQualifier(typeInfo);
	llvm::outs() << " :type [";
	t->type = PrintTypeInfo(elmtype);
	llvm::outs() << "]}";
      }
    } else if (dyn_cast<ParenType>(typeInfo)) {
      elmtype = dyn_cast<ParenType>(typeInfo)->getInnerType();
      PrintTypeInfo(elmtype);
    }

    return t;
  }

  //void PrintPointerTypeInfo(QualType typeInfo) {
  PointeeType *PrintPointerTypeInfo(QualType typeInfo) {
    PointeeType *t = new PointeeType();
    if (dyn_cast<PointerType>(typeInfo)) {
      QualType elmtype = dyn_cast<PointerType>(typeInfo)->getPointeeType();
      assert(labelflag == 0);
      if (castflag != 0) {
	cast << "{:kind \"Pointer-type\""
	     << " :Pointee ";
	PrintTypeInfo(elmtype);
	PrintQualifier(typeInfo);
	cast << "}";
	castlabel += cast.str();
	cast.str("");
	cast.clear();
      } else {
	llvm::outs() << "{:kind \"Pointer-type\""
		     << " :Pointee ";
	PrintTypeInfo(elmtype);
	PrintQualifier(typeInfo);
	llvm::outs() << "}";
      }
    }
    QualType elmtype = dyn_cast<PointerType>(typeInfo)->getPointeeType();
    t->pointee = PrintTypeInfo(elmtype);

    return t;
  }

  //void PrintStructureTypeInfo(QualType typeInfo) {
  StructureType *PrintStructureTypeInfo(QualType typeInfo) {
    StructureType *t = new StructureType();
    assert(labelflag == 0);
    if (castflag != 0) {
      if (dyn_cast<ElaboratedType>(typeInfo)) {
	QualType etype = dyn_cast<ElaboratedType>(typeInfo)->getNamedType();
	PrintTypeInfo(etype);
      } 
      if (dyn_cast<RecordType>(typeInfo)) {
	RecordDecl *rdecl = dyn_cast<RecordType>(typeInfo)->getDecl();
	cast << "{:kind \"Structure-type\"";
	PrintQualifier(typeInfo);
	cast << " :name" << " \"" << (std::string)rdecl->getName() << "\"";
	/* FIXME: Output members of anonymous struct while taking
	 *        cast into account. Disable this function for the time being.
	 */
	if (false && rdecl->getName() == "") {
	  cast << "\n :member[";
	  linefeedflag = 0;
	  if (!(rdecl->field_empty())) {
	    RecordDecl::field_iterator itr = rdecl->field_begin();
	    while (itr != rdecl->field_end()) {
	      TraverseDecl(itr->getCanonicalDecl());
	      itr++;
	    } 
	  }
	  cast << "]";
	}
	cast << "}";
      }
      castlabel += cast.str();
      cast.str("");
      cast.clear();
    } else {
      if (dyn_cast<ElaboratedType>(typeInfo)) {
	QualType etype = dyn_cast<ElaboratedType>(typeInfo)->getNamedType();
	PrintTypeInfo(etype);
      } 
      if (dyn_cast<RecordType>(typeInfo)) {
	llvm::outs() << "{:kind \"Structure-type\"";
	PrintQualifier(typeInfo);
	RecordDecl *rdecl = dyn_cast<RecordType>(typeInfo)->getDecl();
	llvm::outs() << " :name" << " \"" << rdecl->getName() << "\"";  
	if (rdecl->getName() == "") {
	  llvm::outs() << "\n :member[";
	  linefeedflag = 0;
	  if (!(rdecl->field_empty())) {
	    RecordDecl::field_iterator itr = rdecl->field_begin();
	    while (itr != rdecl->field_end()) {
	      TraverseDecl(itr->getCanonicalDecl());
	      itr++;
	    } 
	  }
	  llvm::outs() << "]";
	}
	llvm::outs() << "}";
      }
    }

    return t;
  }

  //void PrintUnionTypeInfo(QualType typeInfo) {
  UnionType *PrintUnionTypeInfo(QualType typeInfo) {
    UnionType *t = new UnionType();
    if (castflag != 0) {
      if (dyn_cast<ElaboratedType>(typeInfo)) {
	QualType etype = dyn_cast<ElaboratedType>(typeInfo)->getNamedType();
	PrintTypeInfo(etype);
      } 
      if (dyn_cast<RecordType>(typeInfo)) {
	RecordDecl *rdecl = dyn_cast<RecordType>(typeInfo)->getDecl();
	cast << "{:kind \"Union-type\"";
	PrintQualifier(typeInfo);
	cast << " :name" << " \"" << (std::string)rdecl->getName() << "\"";
	if (rdecl->getName() == "") {
	  cast << "\n :member[";
	  linefeedflag = 0;
	  if (!(rdecl->field_empty())) {
	    RecordDecl::field_iterator itr = rdecl->field_begin();
	    while (itr != rdecl->field_end()) {
	      TraverseDecl(itr->getCanonicalDecl());
	      itr++;
	    } 
	  }
	  cast << "]";
	}
	cast << "}";
      }
      castlabel += cast.str();
      cast.str("");
      cast.clear();
    } else {
      if (dyn_cast<ElaboratedType>(typeInfo)) {
	QualType etype = dyn_cast<ElaboratedType>(typeInfo)->getNamedType();
	PrintTypeInfo(etype);
      } 
      if (dyn_cast<RecordType>(typeInfo)) {
	RecordDecl *rdecl = dyn_cast<RecordType>(typeInfo)->getDecl();
	llvm::outs() << "{:kind \"Union-type\"";
	PrintQualifier(typeInfo);
	llvm::outs() << " :name" << " \"" << rdecl->getName() << "\"";
	if (rdecl->getName() == "") {
	  llvm::outs() << "\n :member [";
	  linefeedflag = 0;
	  if (!(rdecl->field_empty())) {
	    RecordDecl::field_iterator itr = rdecl->field_begin();
	    while (itr != rdecl->field_end()) {
	      TraverseDecl(itr->getCanonicalDecl());
	      itr++;
	    } 
	  }
	  llvm::outs() << "]";
	}
	llvm::outs() << "}";
      }
    }
    return t;
  }

  // :typeの情報を出力
  //void PrintTypeInfo(QualType typeInfo) {
  DataType *PrintTypeInfo(QualType typeInfo) {
    //llvm::outs() << "(";
    DataType *t;
    //if (dyn_cast<AutoType>(typeInfo))
    //  t = PrintAutoTypeInfo(typeInfo);
    //else if (dyn_cast<TypedefType>(typeInfo))
    if (dyn_cast<TypedefType>(typeInfo))
      return PrintTypedefTypeInfo(typeInfo);
    else if (dyn_cast<BuiltinType>(typeInfo))
      return PrintBuiltinTypeInfo(typeInfo);
    else if (typeInfo->isFunctionType())
      return PrintFunctionTypeInfo(typeInfo);
    else if (typeInfo->isArrayType()) 
      return PrintArrayTypeInfo(typeInfo);
    else if (typeInfo->isPointerType())
      return PrintPointerTypeInfo(typeInfo);
    else if (typeInfo->isStructureType())
      return PrintStructureTypeInfo(typeInfo);
    else if (typeInfo->isUnionType()) 
      return PrintUnionTypeInfo(typeInfo);
    else {
      assert(labelflag == 0);
      if (castflag != 0) {
	cast << "{:kind \"Unknown-type\""
	     << " :typeString " << "\"" << typeInfo.getAsString() << "\"}";
	castlabel += cast.str();
	cast.str("");
	cast.clear();
      } else {
	llvm::outs() << "{:kind \"Unknown-type\""
		     << " :typeString " << "\"" << typeInfo.getAsString() << "\"}";
      }
    }

    return t;
    //llvm::outs() << "\"" << typeInfo.getAsString() << "\")";
    //llvm::outs()  << " " << typeInfo->getTypeClassName();
  }
  
  void PrintQualifier(QualType Qual) {
    if (labelflag != 0) {
      if (Qual.isConstQualified()) {
	os << " :const " << "\"true\"";
      }
      if (Qual.isVolatileQualified()) {
	os << " :volatile " << "\"true\"";
      }
      caselabel += os.str();
      os.str("");
      os.clear();
    } else if (castflag != 0) {
      if (Qual.isConstQualified()) {
	cast << " :const " << "\"true\"";
      }
      if (Qual.isVolatileQualified()) {
	cast << " :volatile " << "\"true\"";
      }
      castlabel += cast.str();
      cast.str("");
      cast.clear();
    } else {
      if (Qual.isConstQualified()) {
	llvm::outs() << " :const " << "\"true\"";
      }
      if (Qual.isVolatileQualified()) {
	llvm::outs() << " :volatile " << "\"true\"";
      }
    }
  }
    
  // Stmt : Binop, ReturnStmt, forstmt, ifstmt, ... etc
  // *Stmt, *Expr をカバーしていると思う
  bool TraverseStmt(Stmt *stmt) {
    if (skipcount > 0) {
      skipcount--;
      return true;
    }
    if (stmt != NULL){
      //llvm::outs() << "\nenter " << stmt->getStmtClassName() << "\n";
      switch (stmt->getStmtClass()) {
      case Stmt::BreakStmtClass:
	llvm::outs() << "\n";
        //llvm::outs() << "stmt:1";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::CaseStmtClass:
        //llvm::outs() << "stmt:2";
	caseflag = 1;
	casetoji = 1;
	labelflag++;
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::CompoundStmtClass: 
        //llvm::outs() << "stmt:3";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::ContinueStmtClass: 
        //llvm::outs() << "stmt:4";
	llvm::outs() << "\n";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::DeclStmtClass: 
        //llvm::outs() << "stmt:5";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::DefaultStmtClass: 
        //llvm::outs() << "stmt:6";
	caseflag = 1;
	labelflag++;
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::DoStmtClass: 
        //llvm::outs() << "stmt:7";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::ForStmtClass: 
        //llvm::outs() << "stmt:8";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::GotoStmtClass: 
        //llvm::outs() << "stmt:9";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::IfStmtClass: 
        //llvm::outs() << "stmt:10";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::LabelStmtClass: 
        //llvm::outs() << "stmt:11";
	caseflag = 1;
	labelflag++;
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::SwitchStmtClass: 
        //llvm::outs() << "stmt:12";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::WhileStmtClass: 
        //llvm::outs() << "stmt:13";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::ReturnStmtClass: 
        //llvm::outs() << "stmt:14";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::ArraySubscriptExprClass: 
        //llvm::outs() << "stmt:15";
	if (FuncCall == 1) {
	  FuncCall = 0;
	} else if (ArraySub == 1) {
	  RecursiveASTVisitor::TraverseStmt(stmt);
	  break;
	} else if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	ArraySub = 1;
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::CallExprClass: 
        //llvm::outs() << "stmt:16";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	FuncCall = 1;
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::CStyleCastExprClass: 
        //llvm::outs() << "stmt:17";
	castflag = 1;
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::DeclRefExprClass: 
        //llvm::outs() << "stmt:18";
	if (FuncCall == 1 || ArraySub == 1 || linefeedflag == 0 || linefeedbody == 0) {
	  FuncCall = 0;
	  ArraySub = 0;
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::ImplicitCastExprClass: 
        //llvm::outs() << "stmt:19";
	castflag = 1;
	if (dyn_cast<ImplicitCastExpr>(stmt)->getSubExpr()->getStmtClass() == 68) {
	  llvm::outs() << "{:kind \"NULL\" :value \"Null\"}";
	  break;
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::InitListExprClass: 
        //llvm::outs() << "stmt:20";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	llvm::outs() << "[";	
	RecursiveASTVisitor::TraverseStmt(stmt);
	llvm::outs() << "]";
	break;
      case Stmt::MemberExprClass: 
        //llvm::outs() << "stmt:21";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::ParenExprClass: 
        //llvm::outs() << "stmt:22";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::UnaryExprOrTypeTraitExprClass:
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::BinaryOperatorClass: 
        //llvm::outs() << "stmt:23";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::ConditionalOperatorClass: 
        //llvm::outs() << "stmt:24";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::CompoundAssignOperatorClass: 
        //llvm::outs() << "stmt:25";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::UnaryOperatorClass: 
        //llvm::outs() << "stmt:26";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::CharacterLiteralClass: 
        //llvm::outs() << "stmt:27";
	if (linefeedflag == 0) {
	  linefeedflag = 1;
	} else {
	  //llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::FloatingLiteralClass: 
        //llvm::outs() << "stmt:28";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::IntegerLiteralClass: 
        //llvm::outs() << "stmt:29";
	if (linefeedflag == 0) {
	  linefeedflag = 1;
	} else {
	  //llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::StringLiteralClass: 
        //llvm::outs() << "stmt:30";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	} else {
	  llvm::outs() << "\n";
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
	//以下C++に関係
      case Stmt::CXXNewExprClass: 
        //llvm::outs() << "stmt:31";
      case Stmt::CXXUnresolvedConstructExprClass: 
        //llvm::outs() << "stmt:32";
      case Stmt::ParenListExprClass: 
        //llvm::outs() << "stmt:33";
	llvm::outs() << "[";
	RecursiveASTVisitor::TraverseStmt(stmt);
	llvm::outs() << "]";
	break;
      case Stmt::CXXCatchStmtClass: 
        //llvm::outs() << "stmt:34";
      case Stmt::CXXTryStmtClass: 
        //llvm::outs() << "stmt:35";
      case Stmt::NullStmtClass: 
        //llvm::outs() << "stmt:36";
      case Stmt::AtomicExprClass: 
        //llvm::outs() << "stmt:37";
      case Stmt::CXXBindTemporaryExprClass: 
        //llvm::outs() << "stmt:38";
      case Stmt::CXXBoolLiteralExprClass: 
        //llvm::outs() << "stmt:39";
      case Stmt::CXXConstCastExprClass: 
        //llvm::outs() << "stmt:40";
      case Stmt::CXXConstructExprClass: 
        //llvm::outs() << "stmt:41";
      case Stmt::CXXDeleteExprClass: 
        //llvm::outs() << "stmt:42";
      case Stmt::CXXDependentScopeMemberExprClass: 
        //llvm::outs() << "stmt:43";
      case Stmt::CXXDynamicCastExprClass: 
        //llvm::outs() << "stmt:44";
      case Stmt::CXXFunctionalCastExprClass: 
        //llvm::outs() << "stmt:45";
      case Stmt::CXXMemberCallExprClass: 
        //llvm::outs() << "stmt:46";
      case Stmt::CXXOperatorCallExprClass: 
        //llvm::outs() << "stmt:47";
      case Stmt::CXXPseudoDestructorExprClass: 
        //llvm::outs() << "stmt:48";
      case Stmt::CXXReinterpretCastExprClass: 
        //llvm::outs() << "stmt:49";
      case Stmt::CXXScalarValueInitExprClass: 
        //llvm::outs() << "stmt:50";
      case Stmt::CXXStaticCastExprClass: 
        //llvm::outs() << "stmt:51";
      case Stmt::CXXThisExprClass: 
        //llvm::outs() << "stmt:52";
      case Stmt::CXXThrowExprClass: 
        //llvm::outs() << "stmt:53";
      case Stmt::DependentScopeDeclRefExprClass: 
        //llvm::outs() << "stmt:54";
      case Stmt::GNUNullExprClass: 
        //llvm::outs() << "stmt:55";
      case Stmt::MaterializeTemporaryExprClass: 
        //llvm::outs() << "stmt:56";
      case Stmt::UnaryTypeTraitExprClass: 
        //llvm::outs() << "stmt:57";
      case Stmt::UnresolvedLookupExprClass: 
        //llvm::outs() << "stmt:58";
      case Stmt::UnresolvedMemberExprClass: 
        //llvm::outs() << "stmt:59";
      case Stmt::ExprWithCleanupsClass: 
        //llvm::outs() << "stmt:60";
	//llvm::outs() << "?Stmt? " << stmt->getStmtClassName();	
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      default: 
        //llvm::outs() << "stmt:61";
	llvm::outs() << "?Stmt? " << stmt->getStmtClassName();	
	RecursiveASTVisitor::TraverseStmt(stmt);
      }
      //llvm::outs() << "\nleave " << stmt->getStmtClassName() << "\n";
    }
    return true;
  }
  bool shouldUseDataRecursionFor(Stmt* S) const {
    return false;
  }
  
  // CXXBoolLiteralExpr
  bool VisitCXXBoolLiteralExpr(CXXBoolLiteralExpr *cxxbool) {
    llvm::outs() << "\"" << cxxbool->getValue() << "\"";
    return true;
  }
  
  // CXXConstructExpr
  bool VisitCXXConstructExpr(CXXConstructExpr *cxxconst) {
    llvm::outs() << "{:kind \"Unknown\"}";
    return false;
  }

  // CXXDependentScopeMemberExpr
  bool VisitCXXDependentScopeMemberExpr(CXXDependentScopeMemberExpr *cxxdsm) {
    llvm::outs() << "{:kind \"Unknown\"}";
    return false;
  }

  // CXXNewExpr
  bool VisitCXXNewExpr(CXXNewExpr *cxxnew) {
    return false;
  }

  // CXXThisExpr
  bool VisitCXXThisExpr(CXXThisExpr *cxxthis) {
    llvm::outs() << "{:kind \"Unknown-Expr\"}";
    return true;
  }

  // DependentScopeDeclRefExpr
  bool VisitDependentScopeDeclRefExpr(DependentScopeDeclRefExpr *dsDRE) {
    llvm::outs() << "{:kind \"Unknown\""
		 << " :name \"" << dsDRE->getDeclName().getAsString() << "\"";
    PrintSourceRange(dsDRE->getSourceRange());
    llvm::outs() << "}";
    return false;
  }
  
  // UnaryTypeTraitExpr
  bool VisitUnaryTypeTraitExpr(UnaryTypeTraitExpr *utte) {
    llvm::outs() << "{:kind \"Unknown\"}";
    return true;
  }
  
  // UnresolvedLookupExpr
  bool VisitUnresolvedLookupExpr(UnresolvedLookupExpr *ulexpr) {
    llvm::outs() << "{:kind \"Unknown\"" 
		 << " :name \"" << ulexpr->getName() << "\"";
    PrintSourceRange(ulexpr->getSourceRange());
    llvm::outs() << "}";
    return false;
  }
  
  // UnresolvedMemberExpr
  bool VisitUnresolvedMemberExpr(UnresolvedMemberExpr *umemexpr) {
    TraverseStmt(umemexpr->getBase());
    return true;
  }

  //
  // BreakStmt
  bool VisitBreakStmt(BreakStmt *Break) {
    llvm::outs()  << "{:kind \"Break\"";
    checkLabel(); 
    PrintSourceRange(Break->getSourceRange());
    llvm::outs() << "}";
    return true;
  }
 
  // CaseStmt
  bool VisitCaseStmt(CaseStmt *Case) {
    os << "{:kind \"Case\"";
    PrintSourceRange(Case->getSourceRange());
    os  << " :value ";
    caselabel += os.str();
    os.str("");
    os.clear();

    /* FIXME: print case conditions other than literals
     */
    assert(Case->getLHS() != NULL);
    switch (Case->getLHS()->getStmtClass()) {
    case Stmt::IntegerLiteralClass:
    case Stmt::CharacterLiteralClass:
      return true;
    default:
      caselabel += "\"unsupported\"}";
      labelflag = 0;
      skipcount = 1;
      return true;
    }
  }
  
  // CompoundStmt
  bool VisitCompoundStmt(CompoundStmt *compound) {
    if (compound->body_empty()) {
      linefeedflag = 1;
      linefeedbody = 1;
    }
    return true;
  }
  // ContinueStmt
  bool VisitContinueStmt(ContinueStmt *Continue) {
    llvm::outs() << "{:kind \"Continue\"";
    checkLabel(); 
    PrintSourceRange(Continue->getSourceRange());
    llvm::outs() << "}";
    return true;
  } 
  
  // DefaultStmt
  bool VisitDefaultStmt(DefaultStmt *Default) {
    os << "{:kind \"Default\"";
    PrintSourceRange(Default->getSourceRange());
    os << "}";
    caselabel += os.str();
    os.str("");
    os.clear();
    labelflag = 0;
    return true;
  }

  // DoStmt
  bool VisitDoStmt(DoStmt *Do) {
    llvm::outs() << "{:kind \"Do\"";
    checkLabel(); 
    PrintSourceRange(Do->getSourceRange());
    llvm::outs() << "\n :condition ";
    linefeedflag = 0;
    TraverseStmt(Do->getCond());
    llvm::outs() << "\n :body [";
    linefeedflag = 0;
    linefeedbody = 0;
    TraverseStmt(Do->getBody());
    llvm::outs() << "]}";
    return false;
  }

  // ForStmt
  bool VisitForStmt(ForStmt *For) {
    llvm::outs() << "{:kind \"For\"";
    checkLabel(); 
    PrintSourceRange(For->getSourceRange());
    llvm::outs() << "\n :init [";
    linefeedflag = 0;
    TraverseStmt(For->getInit());
    llvm::outs() << "]\n :condition ";
    linefeedflag = 0;
    if (For->getCond() == NULL) {
      llvm::outs() << "[]";
    } else {
      TraverseStmt(For->getCond());
    }
    llvm::outs() << "\n :update [";
    linefeedflag = 0;
    TraverseStmt(For->getInc());
    llvm::outs() << "]\n :body [";
    linefeedflag = 0;
    linefeedbody = 0;
    TraverseStmt(For->getBody());
    llvm::outs() << "]}";
    return false;
  }

  // GotoStmt
  bool VisitGotoStmt(GotoStmt *Goto) {
    NamedDecl *label = dyn_cast<NamedDecl>(Goto->getLabel());
    llvm::outs() << "{:kind \"Goto\"";
    checkLabel(); 
    PrintSourceRange(Goto->getSourceRange());
    llvm::outs() << " :goto " << "\"" << label->getName() << "\"}";
    return true;
  }

  // IfStmt
  bool VisitIfStmt(IfStmt *If) {
    llvm::outs() << "{:kind \"If\"";
    checkLabel(); 
    PrintSourceRange(If->getSourceRange());
    llvm::outs() << "\n :condition ";
    linefeedflag = 0;
    TraverseStmt(If->getCond());
    llvm::outs() << "\n :then [";
    linefeedflag = 0;
    TraverseStmt(If->getThen());
    llvm::outs() << "]";
    if (If->getElse()) {
      llvm::outs() << "\n :else [";
      linefeedflag = 0;
      TraverseStmt(If->getElse());
      llvm::outs() << "]}";
    } else {
      llvm::outs() << "}";
    }
    return false;
  }

  // LabelStmt
  bool VisitLabelStmt(LabelStmt *Label) {
    os << "{:kind \"Label\"";
    PrintSourceRange(Label->getSourceRange());
    os  << " :name " << "\"" << Label->getName() << "\"}";
    caselabel += os.str();
    os.str("");
    os.clear();
    labelflag = 0;
    return true;
  }

  // SwitchStmt
  bool VisitSwitchStmt(SwitchStmt *Switch) {
    llvm::outs() << "{:kind \"Switch\"";
    checkLabel(); 
    PrintSourceRange(Switch->getSourceRange());
    llvm::outs() << "\n :condition ";
    linefeedflag = 0;
    TraverseStmt(Switch->getCond());
    llvm::outs() << "\n :body [";
    linefeedflag = 0;
    linefeedbody = 0;
    TraverseStmt(Switch->getBody());
    llvm::outs() << "]}";
    return false;
  }
  
  // WhileStmt
  bool VisitWhileStmt(WhileStmt *While) {
    llvm::outs() << "{:kind \"While\"";
    checkLabel(); 
    PrintSourceRange(While->getSourceRange());
    llvm::outs() << "\n :condition ";
    linefeedflag = 0;
    TraverseStmt(While->getCond());
    llvm::outs() << "\n :body [";
    linefeedflag = 0;
    linefeedbody = 0;
    TraverseStmt(While->getBody());
    llvm::outs() << "]}";
    return false;
  }
 
  // ReturnStmt
  bool VisitReturnStmt(ReturnStmt *Ret) {
    llvm::outs() << "{:kind \"Ret\"";
    checkLabel(); 
    PrintSourceRange(Ret->getSourceRange());
    if (Ret->getRetValue()) {
      llvm::outs() << " :value ";
      linefeedflag = 0;
      TraverseStmt(Ret->getRetValue());
    } else {
      
    }
    llvm::outs() << "}";
    return false;
  }

  //
  // ArraySubscriptExpr
  bool VisitArraySubscriptExpr(ArraySubscriptExpr *arrsub) {
    llvm::outs() << "{:kind \"ArrayRef\"";
    checkLabel(); 
    llvm::outs() << " :type [";
    PrintTypeInfo(arrsub->getType());
    checkCast();
    llvm::outs() << "]";
    PrintSourceRange(arrsub->getSourceRange());
    llvm::outs() << "\n :Array [";
    TraverseStmt(arrsub->getLHS());
    llvm::outs() << "]\n :Index ";
    linefeedflag = 0;
    TraverseStmt(arrsub->getRHS()); 
    llvm::outs() << "}";
    return false;
  }

  // CallExpr
  bool VisitCallExpr(CallExpr *call) {
    llvm::outs() << "{:kind \"FuncCall\"";
    checkLabel();
    llvm::outs() << " :type [";
    PrintTypeInfo(call->getType());
    checkCast();
    llvm::outs() << "]";
    PrintSourceRange(call->getSourceRange());
    llvm::outs() << "\n :Func [";
    linefeedflag = 0;
    TraverseStmt(call->getCallee());
    llvm::outs() << "]\n :Parm [";
    if (call->getNumArgs() != 0) {
      int argnum = 0;
      linefeedflag = 0;
      while (argnum != (int)call->getNumArgs()) {
	TraverseStmt(call->getArg(argnum));
	argnum++;
      }
    }
    llvm::outs() << "]}";
    return false;
  }

  // CStyleCastExpr
  bool VisitCStyleCastExpr(CStyleCastExpr *cscast) {
    if (cscast->getCastKind() != 3) {
      QualType casttype = cscast->getType();
      PrintTypeInfo(casttype); 
      ct.push(castlabel);
      castlabel = "";
      cast.str("");
      cast.clear();
    }
    castflag = 0;
    TraverseStmt(cscast->getSubExpr());
    return false;
  }

  // DeclRefExpr(DRE)
  bool VisitDeclRefExpr(DeclRefExpr *Declref) {
    if (Declref->hasTemplateKWAndArgsInfo()) {
      llvm::outs() << Declref->getNameInfo();
    }
    ValueDecl *valuedecl = Declref->getDecl();
    VarDecl *vardecl = dyn_cast<VarDecl>(valuedecl);
    FunctionDecl *funcdecl = dyn_cast<FunctionDecl>(valuedecl);
    std::string scope = "";
    if (labelflag != 0) {
      if (vardecl) {// 変数の場合
	QualType vartype = vardecl->getType();
	scope = (vardecl->isFileVarDecl() == 1? "global":"local");
	std::string Declreftype = Declref->getType().getAsString();
	os << "{:kind \"DRE\"" 
	   << " :name " << "\"" << Declref->getNameInfo().getAsString() << "\""
	   << " :scope " << "\"" << scope << "\"";
	checkSpecifier(vardecl->getStorageClass());
	PrintDisplayType(vartype);
	os << " :type [";
	PrintTypeInfo(vartype);
	checkCast();
	os << "]";
	PrintSourceRange(Declref->getSourceRange());
	os << "}";
	if (casetoji != 0) {
	  os << "}";
	  casetoji = 0;
	  labelflag = 0;
	}
	caselabel += os.str();
	os.str("");
	os.clear();
      } else if (funcdecl) {// 関数の場合
	QualType functype = funcdecl->getType();
	std::string Declreftype = funcdecl->getResultType().getAsString();
	os << "{:kind \"DRE\"" 
	   << " :name " << "\"" << Declref->getNameInfo().getAsString() << "\"";
	checkSpecifier(funcdecl->getStorageClass());
	PrintDisplayType(functype);
	os << " :type [";
	PrintTypeInfo(functype);
	checkCast();
	os << "]";
	PrintSourceRange(Declref->getSourceRange());
	os << "}";
	if (casetoji != 0) {
	  os << "}";
	  casetoji = 0;
	  labelflag = 0;
	}
	caselabel += os.str();
	os.str("");
	os.clear();
      } else {
	QualType Declreftype = Declref->getType();
	os << "{:kind \"DRE\"" 
	   << " :name " << "\"" << Declref->getNameInfo().getAsString() << "\"";
	PrintDisplayType(Declreftype);
	os << " :type [";
	PrintTypeInfo(Declreftype);
	checkCast();
	os << "]";
	PrintSourceRange(Declref->getSourceRange());
	os << "}";
      }
      if (casetoji != 0) {
	os << "}";
	casetoji = 0;
	labelflag = 0;
      }
      caselabel += os.str();
      os.str("");
      os.clear();
    } else {
      if (vardecl) {// 変数の場合
	QualType vartype = vardecl->getType();
	scope = (vardecl->isFileVarDecl() == 1? "global":"local");
	std::string Declreftype = Declref->getType().getAsString();
	llvm::outs() << "{:kind \"DRE\"" 
		     << " :name " << "\"" << Declref->getNameInfo() << "\""
		     << " :scope " << "\"" << scope << "\"";
	checkLabel(); 
	checkSpecifier(vardecl->getStorageClass());
	PrintDisplayType(vartype);
	llvm::outs() << " :type [";
	PrintTypeInfo(vartype);
	checkCast();
	llvm::outs() << "]";
	PrintSourceRange(Declref->getSourceRange());
	llvm::outs() << "}";
      } else if (funcdecl) {// 関数の場合
	QualType functype = funcdecl->getType();
	std::string Declreftype = funcdecl->getResultType().getAsString();
	llvm::outs() << "{:kind \"DRE\"" 
		     << " :name " << "\"" << Declref->getNameInfo() << "\"";
	checkLabel(); 
	checkSpecifier(funcdecl->getStorageClass());
	PrintDisplayType(functype);
	llvm::outs() << " :type [";
	PrintTypeInfo(functype);
	checkCast();
	llvm::outs() << "]";
	PrintSourceRange(Declref->getSourceRange());
	llvm::outs() << "}";
      } else {
	QualType Declreftype = Declref->getType();
	llvm::outs() << "{:kind \"DRE\"" 
		     << " :name " << "\"" << Declref->getNameInfo() << "\"";
	checkLabel(); 
	PrintDisplayType(Declreftype);
	llvm::outs() << " :type [";
	PrintTypeInfo(Declreftype);
	checkCast();
	llvm::outs() << "]";
	PrintSourceRange(Declref->getSourceRange());
	llvm::outs() << "}";
      }
    }
    return true;
  }

  // ImplicitCastExpr
  bool VisitImplicitCastExpr(ImplicitCastExpr *icast) {
    if (labelflag != 0) {
	  return true;
    }
    if (icast->getCastKind() != 3) {
      QualType casttype = icast->getType();
      PrintTypeInfo(casttype); 
      ct.push(castlabel);
      castlabel = "";
      cast.str("");
      cast.clear();
    }
    castflag = 0;
    return true;
  }

  // MemberExpr
  bool VisitMemberExpr(MemberExpr *mem) {
    Expr *base = mem->getBase();
    ValueDecl *vdecl = mem->getMemberDecl();
    QualType memtype = mem->getType();
    if (mem->isLValue() || mem->isRValue()) {
      llvm::outs() << "{:kind \"Struct\""
		   << " :struct ";
      linefeedflag = 0;
      TraverseStmt(base);
      llvm::outs() << " :structmember ";
      linefeedflag = 0;
      TraverseDecl(vdecl);
      llvm::outs() << " :type [";
      PrintTypeInfo(memtype);
      checkCast();
      llvm::outs() << "]";
      PrintSourceRange(mem->getSourceRange());
      llvm::outs() << "}";
      
      //linefeedflag = 0;
      //TraverseDecl(mem->getMemberDecl());
      return false; 
    }
    if(mem->isArrow()) {
      llvm::outs() << "{:kind \"Binop\" :op \"->\"";
      checkLabel(); 
      llvm::outs() << " :type [";
      PrintTypeInfo(mem->getType());
      checkCast();
      llvm::outs() << "]";
      PrintSourceRange(mem->getSourceRange());
      llvm::outs() << "\n :LHS ";
      getlhsArrow(base);
      llvm::outs() << "\n :RHS ";
      getrhsArrow(vdecl);
      llvm::outs() << "}"; 
      return false;
    } 
    return false;
  }
  bool getlhsArrow(Expr *base) {
    ImplicitCastExpr *imp = dyn_cast<ImplicitCastExpr>(base);
    if (imp) {
      Expr *sub = imp->getSubExpr();
      DeclRefExpr *def = dyn_cast<DeclRefExpr>(sub);
      if (def) {
	linefeedflag = 0;
	TraverseStmt(def);
      }
    }
    return true;
  }
  bool getrhsArrow(ValueDecl *vdecl) {
    QualType vartype = vdecl->getType();
    llvm::outs() << "{:kind \"DRE\""
		 << " :name " << "\"" << vdecl->getName() << "\"" 
		 << " :scope \"member\"";
    llvm::outs() << " :type [";
    PrintTypeInfo(vartype);
    checkCast();
    llvm::outs() << "]";
    PrintSourceRange(vdecl->getSourceRange());
    llvm::outs() << "}";
    return true;
  }
  
  // UnaryExprOrTypeTraitExpr
  bool VisitUnaryExprOrTypeTraitExpr(UnaryExprOrTypeTraitExpr *expr) {
    switch(expr->getKind()) {
    case UETT_SizeOf:
      llvm::outs() << "{:kind \"SizeOf\"";
      llvm::outs() << " :type [";
      PrintTypeInfo(expr->getType());
      checkCast();
      llvm::outs() << "]";
      if (expr->isArgumentType()) {
	llvm::outs() << " :ArgumentType [";
	PrintTypeInfo(expr->getArgumentType());
	llvm::outs() << "]";
      } else {
	llvm::outs() << " :Argument ";
	TraverseStmt(expr->getArgumentExpr());
      }
      PrintSourceRange(expr->getSourceRange());
      llvm::outs() << "}";
      return false;
    default: 
      break;
    }
    return true;
  }

  // UnaryOperator
  bool VisitUnaryOperator(UnaryOperator *Unop) {
    UnaryOperator::Opcode opcode = Unop->getOpcode();
    llvm::outs() << "{:kind \"Unop\""
		 << " :op " << "\"" << Unop->getOpcodeStr(opcode) << "\"";
    checkLabel(); 
    llvm::outs() << " :type [";
    PrintTypeInfo(Unop->getType());
    checkCast();
    llvm::outs() << "]";
    PrintSourceRange(Unop->getSourceRange());
    llvm::outs() << " :HS ";
    linefeedflag = 0;
    TraverseStmt(Unop->getSubExpr());
    llvm::outs() << "}";
    return false;
  }

  // BinaryOperator
  bool VisitBinaryOperator(BinaryOperator *Binop) {
    llvm::outs() << "{:kind \"Binop\"" 
		 << " :op " << "\"" << Binop->getOpcodeStr() << "\"";
    checkLabel(); 
    llvm::outs() << " :type [";
    PrintTypeInfo(Binop->getType());
    checkCast();
    llvm::outs() << "]";
    PrintSourceRange(Binop->getSourceRange());
    llvm::outs() << "\n :LHS ";
    linefeedflag = 0;
    TraverseStmt(Binop->getLHS());
    llvm::outs() << "\n :RHS ";
    if (Binop->getRHS()->getStmtClass() == 68) {
      llvm::outs() << "{:kind \"NULL\" :value \"Null\"}";
    } else {
      linefeedflag = 0;
      TraverseStmt(Binop->getRHS());
    }
    llvm::outs() << "}";
      
    return false;
  }
  
  // ConditionalOperator
  bool VisitConditionalOperator(ConditionalOperator *condop) {
    llvm::outs() << "{:kind \"Conditionalop\""; 
      //<< " :op " << "\"" << condop->getOpcodeStr() << "\"";
    checkLabel(); 
    llvm::outs() << " :type [";
    PrintTypeInfo(condop->getType());
    checkCast();
    llvm::outs() << "]";
    PrintSourceRange(condop->getSourceRange());
    llvm::outs() << "\n :operand1 ";
    linefeedflag = 0;
    TraverseStmt(condop->getCond());
    llvm::outs() << "\n :operand2 ";
    linefeedflag = 0;
    TraverseStmt(condop->getTrueExpr());
    llvm::outs() << "\n :operand3 ";
    if (condop->getRHS()->getStmtClass() == 68) {
      llvm::outs() << "{:kind \"NULL\" :value \"Null\"}";
    } else {
      linefeedflag = 0;
      TraverseStmt(condop->getFalseExpr());
    }
    llvm::outs() << "}";
      
    return false;
    
  }
    
  // LabelValue
  void getLabelValue(Expr *literal){
    std::string literalname;
    std::string literalvalue;
    //    QualType literaltype = literal->getType();
    assert(labelflag != 0);
    if (dyn_cast<IntegerLiteral>(literal)) {
      literalname = "IntegerLabel";
      IntegerLiteral* intL = dyn_cast<IntegerLiteral>(literal);
      os << "{:kind \"" << literalname << "\""
	 << " :value " << "\"" << intL->getValue().toString(10, true) << "\"";
    } else if (dyn_cast<CharacterLiteral>(literal)) {
      literalname = "CharacterLabel";
      CharacterLiteral* charL = dyn_cast<CharacterLiteral>(literal);
      os << "{:kind \"" << literalname << "\""
	 << " :value " << "\"" << charL->getValue() << "\""
	 << " :character " << "\"" << char(charL->getValue()) << "\"";
    }
    /* FIXME: print :type
       os << " :type [";
       PrintTypeInfo(literaltype);
       checkCast();
       os << "]";
    */
    if (dyn_cast<IntegerLiteral>(literal))
      os << " :type [{:kind \"Int-type\"}]";
    else if (dyn_cast<CharacterLiteral>(literal))
      os << " :type [{:kind \"Char-type\"}]";

    PrintSourceRange(literal->getSourceRange());
    os << "}}";
    casetoji = 0;
    caselabel += os.str();
    os.str("");
    os.clear();
    labelflag = 0;
    return;
  }

  // IntegerLiteral
  bool VisitIntegerLiteral(IntegerLiteral *Int) {
    QualType vartype = Int->getType();
    if (labelflag != 0) {
      getLabelValue(Int);
    } else {
      llvm::outs() << "{:kind \"IntegerLiteral\""
		   << " :value " << Int->getValue();
      llvm::outs() << " :type [";
      PrintTypeInfo(vartype);
      checkCast();
      llvm::outs() << "]";
      PrintSourceRange(Int->getSourceRange());
      llvm::outs() << "}";
    }
    return true;
  }

  // FloatingLiteral
  bool VisitFloatingLiteral(FloatingLiteral *Float) {
    QualType vartype = Float->getType();
    llvm::outs() << "{:kind \"FloatingLiteral\""
		 << " :value " << Float->getValueAsApproximateDouble();
    llvm::outs() << " :type [";
    PrintTypeInfo(vartype);
    checkCast();
    llvm::outs() << "]";
    PrintSourceRange(Float->getSourceRange());
    llvm::outs() << "}";
    return true;
  }

  std::string EscapeChar(char c) {
    std::string dest;
    switch (c) {
    case '\"':
    case '\\':
      dest.push_back('\\');
      break;
    default:
      break;
    }
    dest.push_back(c);
    return dest;
  }

  // CharacterLiteral
  bool VisitCharacterLiteral(CharacterLiteral *Char) {
    QualType vartype = Char->getType();
    if (labelflag != 0) {
      getLabelValue(Char);
    } else {
      llvm::outs() << "{:kind \"CharacterLiteral\""
		   << " :value " << "\"" << Char->getValue() << "\""
		   << " :character " << "\""
		   << EscapeChar(char(Char->getValue())) << "\"";
      llvm::outs() << " :type [";
      PrintTypeInfo(vartype);
      checkCast();
      llvm::outs() << "]";
      PrintSourceRange(Char->getSourceRange());
      llvm::outs() << "}";
    }
    return true;
  }

  std::string EscapeString(std::string src) {
    std::string dest;
    for (size_t i = 0; i < src.length();i++) {
      char c = src[i];
      switch (c) {
      case '\"':
      case '\\':
	dest.push_back('\\');
	break;
      default:
	break;
      }
      dest.push_back(c);
    }
    return dest;
  }

  // StringLiteral
  bool VisitStringLiteral(StringLiteral *String) {
    QualType vartype = String->getType();
    llvm::outs() << "{:kind \"StringLiteral\""
		 << " :value " << "\""
		 << EscapeString(String->getString().str()) << "\"";
    llvm::outs() << " :type [";
    PrintTypeInfo(vartype);
    checkCast();
    llvm::outs() << "]";
    PrintSourceRange(String->getSourceRange());
    llvm::outs() << "}";
    return true;
  }

  //// 出力関係
  // 行数 
  //void PrintSourceRange(SourceRange range) {
  Node PrintSourceRange(SourceRange range) {
    Node t;
    FullSourceLoc FullLocationBegin = Context->getFullLoc(range.getBegin());
    FullSourceLoc FullLocationEnd = Context->getFullLoc(range.getEnd());
    if (labelflag != 0) {
      os << " :line [" 
	 << Context->getFullLoc(range.getBegin()).getSpellingLineNumber() 
	 << " " 
	 << Context->getFullLoc(range.getEnd()).getSpellingLineNumber() 
	 << "]";
      caselabel += os.str();
      os.str("");
      os.clear();
    } else {
#if 0
      llvm::outs() << " :line [" 
		   << Context->getFullLoc(range.getBegin()).getSpellingLineNumber() 
		   << " " 
		   << Context->getFullLoc(range.getEnd()).getSpellingLineNumber() 
		   << "]";

      llvm::outs() << " :column " 
		   << Context->getFullLoc(range.getBegin()).getSpellingColumnNumber();
      // U>_<U
      llvm::outs() << " :Filename " 
		   << "\"" << FullLocation.getManager().getFilename(FullLocation) << "\""
		   << " :EndFile "
		   << "\"" << Context->getFullLoc(range.getEnd()).getManager().getFilename(FullLocation) << "\"";
#else
      
      llvm::outs() << " :loc-begin [" 
		   << "\"" << Context->getFullLoc(range.getBegin()).getManager().getFilename(FullLocationBegin) << "\""
		   << " " << Context->getFullLoc(range.getBegin()).getSpellingLineNumber()
		   << " " << Context->getFullLoc(range.getBegin()).getSpellingColumnNumber() << "]"
		   << " :loc-end [" 
		   << "\"" << Context->getFullLoc(range.getEnd()).getManager().getFilename(FullLocationEnd) << "\""
		   << " " << Context->getFullLoc(range.getEnd()).getSpellingLineNumber()
		   << " " << Context->getFullLoc(range.getEnd()).getSpellingColumnNumber() << "]";	
#endif
    }

    t.beginFile = Context->getFullLoc(range.getBegin()).getManager().getFilename(FullLocationBegin);
    t.beginLine = Context->getFullLoc(range.getBegin()).getSpellingLineNumber();
    t.beginColumn = Context->getFullLoc(range.getBegin()).getSpellingColumnNumber();
    t.endFile = Context->getFullLoc(range.getEnd()).getManager().getFilename(FullLocationEnd);
    t.endLine = Context->getFullLoc(range.getEnd()).getSpellingLineNumber();
    t.endColumn = Context->getFullLoc(range.getEnd()).getSpellingColumnNumber();

    return t;
  }

  // ラベルの有無
  void checkLabel() {
    if (caseflag != 0) {
      llvm::outs() << " :label [" << caselabel << "]";
      caselabel = "";
      caseflag = 0;
    }
  }

  // ImplicitCastの有無
  void checkCast() {
    if (labelflag != 0)
      llvm::outs() << "labelflag != 0 in checkCast";
    while (!ct.empty()) {
      llvm::outs() << " " << ct.top();
      ct.pop();
    }
  }
  
  // ASTの出力
  void printAST() {
    int i;
    for (i = 0; i != (int)prog.size(); i++) {
      prog[i]->printAST();
    }
  }

private:
  ASTContext *Context;
  std::string last_func;
  std::string source_file;
  std::string caselabel; // 出力したいラベルの属性
  std::string castlabel;
  std::ostringstream os;
  std::ostringstream cast;
  std::stack<std::string> ct; // cast_type (ImplicitCast, CStyleCastのみ)
  int ArraySub;
  int casetoji;
  int caseflag; // ラベルが付いている証
  int castflag; // ImplicitCastExpr が出現したか
  int FuncCall;
  int labelflag; // ラベル(case, default, label)が出現した印
  int linefeedbody;
  int linefeedflag;
  int skipcount; // skip processing a node while this count is positive
};


class MyAstConsumer : public clang::ASTConsumer {
public:
  explicit MyAstConsumer(ASTContext *Context, llvm::StringRef InFile) : Visitor(Context, InFile) {}
  virtual void HandleTranslationUnit(clang::ASTContext &Context) {
    llvm::outs() << "\n[";
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    llvm::outs() << "-----------------------------------------------";
    Visitor.printAST();
    llvm::outs() << "] \n\n";
  }

private:
  MyAstVisitor Visitor;
};

class MyAnalysisAction : public clang::ASTFrontendAction {
public:
  virtual clang::ASTConsumer *CreateASTConsumer(clang::CompilerInstance &Compiler, llvm::StringRef InFile)
  {
    return new MyAstConsumer(&Compiler.getASTContext(), InFile);
  }
};

int main(int argc, const char **argv) {
  CommonOptionsParser OptionsParser(argc, argv);
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  return Tool.run(newFrontendActionFactory<MyAnalysisAction>());
}
