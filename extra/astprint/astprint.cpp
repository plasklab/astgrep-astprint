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
  bool constBool;
  bool volatileBool;
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
};

class UnsignedType : public DataType {
public:
};

class IntType : public SignedType {
public:
  IntType(bool cons, bool vol);
  void printType();
};

IntType::IntType(bool cons, bool vol) {
  kind = "IntType";
  constBool = cons;
  volatileBool = vol;
}

void IntType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << " :const \"true\"";
  if (volatileBool == true)
    llvm::outs() << " :volatileBool \"true\"";
  llvm::outs() << "}";
}

class UnsignedIntType : public UnsignedType {
public:
  UnsignedIntType(bool cons, bool vol);
  void printType();
};

UnsignedIntType::UnsignedIntType(bool cons, bool vol) {
  kind = "UnsignedIntType";
  constBool = cons;
  volatileBool = vol;
}

void UnsignedIntType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << " :const \"true\"";
  if (volatileBool == true)
    llvm::outs() << " :volatileBool \"true\"";
  llvm::outs() << "}";
}

class FloatType : public SignedType {
public:
  FloatType(bool cons, bool vol);
  void printType();
};

FloatType::FloatType(bool cons, bool vol) {
  kind = "FloatType";
  constBool = cons;
  volatileBool = vol;
}

void FloatType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << " :const \"true\"";
  if (volatileBool == true)
    llvm::outs() << " :volatileBool \"true\"";
  llvm::outs() << "}";
}

class UnsignedFloatType : public UnsignedType {
public:
  UnsignedFloatType(bool cons, bool vol);
  void printType();
};

UnsignedFloatType::UnsignedFloatType(bool cons, bool vol) {
  kind = "UnsignedFloatType";
  constBool = cons;
  volatileBool = vol;
}

void UnsignedFloatType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << " :const \"true\"";
  if (volatileBool == true)
    llvm::outs() << " :volatileBool \"true\"";
  llvm::outs() << "}";
}

class LongType : public SignedType {
public:
  LongType(bool cons, bool vol);
  void printType();
};

LongType::LongType(bool cons, bool vol) {
  kind = "LongType";
  constBool = cons;
  volatileBool = vol;
}

void LongType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << " :const \"true\"";
  if (volatileBool == true)
    llvm::outs() << " :volatileBool \"true\"";
  llvm::outs() << "}";
}

class UnsignedLongType : public UnsignedType {
public:
  UnsignedLongType(bool cons, bool vol);
  void printType();
};

UnsignedLongType::UnsignedLongType(bool cons, bool vol) {
  kind = "UnsignedLongType";
  constBool = cons;
  volatileBool = vol;
}

void UnsignedLongType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << " :const \"true\"";
  if (volatileBool == true)
    llvm::outs() << " :volatileBool \"true\"";
  llvm::outs() << "}";
}

class LongLongType : public SignedType {
public:
  LongLongType(bool cons, bool vol);
  void printType();
};

LongLongType::LongLongType(bool cons, bool vol) {
  kind = "LongLongType";
  constBool = cons;
  volatileBool = vol;
}

void LongLongType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << " :const \"true\"";
  if (volatileBool == true)
    llvm::outs() << " :volatileBool \"true\"";
  llvm::outs() << "}";
}

class UnsignedLongLongType : public UnsignedType {
public:
  UnsignedLongLongType(bool cons, bool vol);
  void printType();
};

UnsignedLongLongType::UnsignedLongLongType(bool cons, bool vol) {
  kind = "UnsignedLongLongType";
  constBool = cons;
  volatileBool = vol;
}

void UnsignedLongLongType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << " :const \"true\"";
  if (volatileBool == true)
    llvm::outs() << " :volatileBool \"true\"";
  llvm::outs() << "}";
}

class DoubleType : public SignedType {
public:
  DoubleType(bool cons, bool vol);
  void printType();
};

DoubleType::DoubleType(bool cons, bool vol) {
  kind = "DoubleType";
  constBool = cons;
  volatileBool = vol;
}

void DoubleType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << " :const \"true\"";
  if (volatileBool == true)
    llvm::outs() << " :volatileBool \"true\"";
  llvm::outs() << "}";
}

class UnsignedDoubleType : public UnsignedType {
public:
  UnsignedDoubleType(bool cons, bool vol);
  void printType();
};

UnsignedDoubleType::UnsignedDoubleType(bool cons, bool vol) {
  kind = "UnsingedDoubleType";
  constBool = cons;
  volatileBool = vol;
}

void UnsignedDoubleType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << " :const \"true\"";
  if (volatileBool == true)
    llvm::outs() << " :volatileBool \"true\"";
  llvm::outs() << "}";
}

class LongDoubleType : public SignedType {
public:
  LongDoubleType(bool cons, bool vol);
  void printType();
};

LongDoubleType::LongDoubleType(bool cons, bool vol) {
  kind = "LongDoubleType";
  constBool = cons;
  volatileBool = vol;
}

void LongDoubleType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << " :const \"true\"";
  if (volatileBool == true)
    llvm::outs() << " :volatileBool \"true\"";
  llvm::outs() << "}";
}

class ShortType : public SignedType {
public:
  ShortType(bool cons, bool vol);
  void printType();
};

ShortType::ShortType(bool cons, bool vol) {
  kind = "ShortType";
  constBool = cons;
  volatileBool = vol;
}

void ShortType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << " :const \"true\"";
  if (volatileBool == true)
    llvm::outs() << " :volatileBool \"true\"";
  llvm::outs() << "}";
}

class UnsignedShortType : public UnsignedType {
public:
  UnsignedShortType(bool cons, bool vol);
  void printType();
};

UnsignedShortType::UnsignedShortType(bool cons, bool vol) {
  kind = "UnsingedShortType";
  constBool = cons;
  volatileBool = vol;
}

void UnsignedShortType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << " :const \"true\"";
  if (volatileBool == true)
    llvm::outs() << " :volatileBool \"true\"";
  llvm::outs() << "}";
}

class CharType : public SignedType {
public:
  CharType(bool cons, bool vol);
  void printType();
};

CharType::CharType(bool cons, bool vol) {
  kind = "CharType";
  constBool = cons;
  volatileBool = vol;
}

void CharType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << " :const \"true\"";
  if (volatileBool == true)
    llvm::outs() << " :volatileBool \"true\"";
  llvm::outs() << "}";
}

class UnsignedCharType : public UnsignedType {
public:
  UnsignedCharType(bool cons, bool vol);
  void printType();
};

UnsignedCharType::UnsignedCharType(bool cons, bool vol) {
  kind = "UnsignedChartType";
  constBool = cons;
  volatileBool = vol;
}

void UnsignedCharType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if (constBool == true)
    llvm::outs() << " :const \"true\"";
  if (volatileBool == true)
    llvm::outs() << " :volatileBool \"true\"";
  llvm::outs() << "}";
}

class PointeeType : public DataType {
public:
  DataType *pointee;
  PointeeType(DataType *point);
  void printType();
};

PointeeType::PointeeType(DataType *point) {
  kind = "PointerType";
  pointee = point;
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
  FuncType(std::vector<DataType *> parmType, DataType *retType);
  void printType();
};

FuncType::FuncType(std::vector<DataType *> parmType, DataType *retType) {
  kind = "FuncType";
  parmType(parmType);
  retType = retType;
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
  std::string arraySize;
  DataType *type;
  ArrayDataType(std::string size, DataType *type);
  void printType();
};

ArrayDataType::ArrayDataType(std::string size, DataType *type) {
  kind = "ArrayType";
  arraySize = size;
  type = type;
}

void ArrayDataType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\" :array-size \"" << arraySize << "\" :type ";
  type->printType();
  llvm::outs() << "}";
}

class StructureType : public DataType {
public:
  std::string name;
  StructureType(std::string name);
  void printType();
};

StructureType::StructureType(std::string name) {
  kind = "StructureType";
  name = name;
}

void StructureType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name << "\"}";
}

class UnionType : public DataType {
public:
  std::string name;
  UnionType(std::string name);
  void printType();
};

UnionType::UnionType(std::string name) {
  kind = "UnionType";
  name = name;
}

void UnionType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name << "\"}";
}

// typedefType
class RenameType : public DataType {
public:
  std::string typeName;
  DataType *typedefType;
  RenameType(std::string typeName, DataType *defType);
  void printType();
};

RenameType::RenameType(std::string typeName, DataType *defType) {
  kind = "TypedefType";
  typeName = typeName;
  typedefType = defType;
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
  void setLocation(Node loc);
  //Node(std::string kind, std::string beginFile, int beginLine, int beginColumn,
  //             std::string endFile, int endLine, int endColumn);
  virtual void printAST();
  virtual ~Node() {};
};
/*
Node::Node(std::string kind, std::string beginFile, int beginLine, int beginColumn,
             std::string endFile, int endLine, int endColumn) {
  kind = kind;
  beginFile = beginFile;
  beginLine = beginLine;
  beginColumn = beginColumn;
  endFile = endFile;
  endLine = endLine;
  endColumn = endColumn;
}
*/
void Node::setLocation(Node loc) {
  beginFile = loc.beginFile;
  beginLine = loc.beginLine;
  beginColumn = loc.beginColumn;
  endFile = loc.endFile;
  endLine = loc.endLine;
  endColumn = loc.endColumn;
}

void Node::PrintLocation() {
  llvm::outs() << " :loc-begin [\"" << beginFile << "\" " << beginLine << " " << beginColumn 
  	   << "] :loc-end [\"" << endFile << "\" " << endLine << " " << endColumn << "]";
}

void Node::printAST() {
  llvm::outs() << "{:No Kind}";
}

class Expression : public Node {
public:
  std::vector<DataType *> type;
  //Expression(DataType *dt, std::vector<DataType *> type);
  virtual void printAST();
};
/*
Expression::Expression(DataType *dt, std::vector<DataType *> type):Node(kind) {
  type.push_back(dt);
  if (type.size() != 0) {
    int i = type.size();
    for (int j = 0; j < i; j++) {
      type.push_back(type[0]);
      type.erase(type.begin() + i);
    }
  }
}
*/
void Expression::printAST() {
  llvm::outs() << "No Expression";
}

class Reference : public Expression {
public:
  std::string name;
  //Reference(std::string name);
};
/*
Reference::Reference(std::string name) {
  name = name;
}
*/
class DeclationReferenceExpression : public Reference {
public:
  std::string scope;
  DeclationReferenceExpression(std::string name, std::string scope, std::vector<DataType *> dts, Node loc);
  void printAST();
};

DeclationReferenceExpression::DeclationReferenceExpression(std::string name, std::string scope,
  std::vector<DataType *> dts, Node loc) {
  kind = "DRE";
  name = name;
  scope = scope;
  type(dts);
  setLocation(loc);
}

void DeclationReferenceExpression::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name 
               << "\" :scope \"" << scope << "\" :type [";
  for (int i = 0; i < (int)type.size(); i++) {
    type[i]->printType();
  }
  llvm::outs() << "] ";
  PrintLocation();
  llvm::outs() << "}";
}

class MemberReference : public Reference {
public:
  std::string scope;
  MemberReference(std::string name, DataType *dt, std::vector<DataType *> dts, Node loc);
  void printAST();  
};

MemberReference::MemberReference(std::string name, DataType *dt, std::vector<DataType *> dts, Node loc) {
  kind = "Field";
  scope = "Member";
  name = name;
  type.push_back(dt);
  while (0 != (int)dts.size()) {
    type.pusch_back(dts[0]);
    dts.erase(dts.begin());
  }
  setLocation(loc);
}

void MemberReference::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name 
               << "\" :scope \"" << scope << "\" :type [";
  for (int i = 0; i < (int)type.size(); i++) {
    type[i]->printType();
  }
  PrintLocation();
  llvm::outs() << "}";
}

class StructReference : public Reference {
public:
  Node *structs;
  Node *structMember;
  StructReference(Node *str, Node strMem, std::vector<DataType *> type, Node loc);
  void printAST();
};

StructReference::StructReference(Node *str, Node strMem, std::vector<DataType *> type, Node loc) {
  kind = "Struct";
  structs = str;
  structMember = strMem;
  type(type);
  setLocation(loc);
}

void StructReference::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :struct ";
  structs->printAST();
  llvm::outs() << " :struct-member ";
  structMember->printAST();
  PrintLocation();
  llvm::outs() << "}";
}

class Operator : public Expression {
};

class BinOp : public Operator {
public:
  std::string op;
  Node *left;
  Node *right;
  BinOp(std::string op, DataType *dt, std::vector<DataType *> dts, Node *left, Node *right, Node loc);
  void printAST();
};

BinOp::BinOp() {
  kind = "BinOp";
  op = op;
  type.push_back(dt);
  while(0 != (int)dts.size()) {
    type.push_back(dts[0]);
    dts.erase(dts.begin());
  }
  left = left;
  right = right;
  setLocation(loc);
}

void BinOp::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :op \"" << op << "\" :type [";
  for (int i = 0; i < (int)type.size(); i++) {
    type[i]->printType();
  }
  llvm::outs() << "]";
  PrintLocation();
  llvm::outs() << "\n :left ";
  left->printAST();
  llvm::outs() << "\n :right ";
  right->printAST();
  llvm::outs() << "}\n";
}

class UnOp : public Operator {
public:
  std::string op;
  Node *operand;
  UnOp(std::string op, std::vector<DataType *> dts, Node *operand, Node loc);
  void printAST();
};

UnOp::UnOp(std::string op, std::vector<DataType *> dts, Node *operand, Node loc) {
  kind = "UnOp";
  op = op;
  type(type);
  operand = operand;
  setLocation(loc);
}

void UnOp::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :op \"" << op << "\" :type [";
  for (int i = 0; i < (int)type.size(); i++) {
    type[i]->printType();
  }
  llvm::outs() << "]";
  PrintLocation();
  llvm::outs() << "\n :operand ";
  operand->printAST();
  llvm::outs() << "}";
}

class ConditionalOp : public Operator {
public:
  Node *condition;
  Node *then;
  Node *denial;
  ConditionalOp(DataType *dt, std::vector<DataType *> dts, Node *condition, Node *then, Node *denial, Node loc);
  void printAST();
};

ConditionalOp::ConditionalOp(DataType *dt, std::vector<DataType *> dts, Node *condition, Node *then, Node *denial, Node loc) {
  kind = "ConditionalOp";
  type.push_back(dt);
  while(0 != (int)dts.size()) {
    type.push_back(dts[0]);
    dts.erase(dts.begin());
  }
  condition = condtion;
  then = then;
  denial = denial;
  setLocation(loc);
}

void ConditionalOp::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :type ";
  for (int i = 0; i < (int)type.size(); i++) {
    type[i]->printType();
  }
  PrintLocation();
  llvm::outs() << "\n :condition ";
  condition->printAST();
  llvm::outs() << "\n :then ";
  then->printAST();
  llvm::outs() << "\n :else ";
  denial->printAST();
  llvm::outs() << "}";
}

class Literal : public Expression {
public:
  std::vector<DataType *> type;
  virtual void printAST();
};

void Literal::printAST() {
  llvm::outs() << "No Literal";
}

class IntLiteral : public Literal {
public:
  std::string value;
  IntLiteral(std::string value, DataType *dt, std::vector<DataType *> dts, Node loc);
  void printAST();
};

IntLiteral::IntLiteral(std::string value, DataType *dt, std::vector<DataType *> dts, Node loc) {
  kind = "IntegerLiteral";
  value = value;
  type.push_back(dt);
  while (0 != (int)dts.size()) {
    type.push_back(dts[0]);
    dts.erase(dts.begin());
  }
  setLocation(loc);
}

void IntLiteral::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :value " << value << " :type [";
  for (int i = 0; i < (int)type.size(); i++) {
    type[i]->printType();
  }
  llvm::outs() << "]";
  PrintLocation();
  llvm::outs() << "}";
}

class CharLiteral : public Literal {
public:
  char value;
  CharLiteral(char value, DataType *dt, std::vector<DataType *> dts, Node loc);
  void printAST();
};

CharLiteral::CharLiteral(char value, DataType *dt, std::vector<DataType *> dts, Node loc) {
  kind = "CharacterLiteral";
  value = value;
  type.push_back(dt);
  while (0 != (int)dts.size()) {
    type.push_back(dts[0]);
    dts.erase(dts.begin());
  }
  setLocation(loc);
}

void CharLiteral::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :value \"" << value << "\" :type [";
  for (int i = 0; i < (int)type.size(); i++) {
    type[i]->printType();
  }
  llvm::outs() << "]";
  PrintLocation();
  llvm::outs() << "}";
}

class FloatLiteral : public Literal {
public:
  float value;
  FloatLiteral(float value, DataType *dt, std::vector<DataType *> dts, Node loc);
  void printAST();
};

FloatLiteral::FloatLiteral(float value, DataType *dt, std::vector<DataType *> dts, Node loc) {
  kind = "FloatingLiteral";
  value = value;
  type.push_back(dt);
  while (0 != dts.size()) {
    type.push_back(dts[0]);
    dts.erase(dts.begin());
  }
  setLocation(loc);
}

void FloatLiteral::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :value " << value << " :type [";
  for (int i = 0; i < (int)type.size(); i++) {
    type[i]->printType();
  }
  llvm::outs() << "]";
  PrintLocation();
  llvm::outs() << "}";
}

class ArrayReference : public Reference {
public:
  Node *array;
  Node *index;
  ArrayReference(Node *array, Node *index, Node loc);
  void printAST();
};

ArrayReference::ArrayReference(Node *array, Node *index, Node loc) {
  kind = "ArrayRef";
  array = array;
  index = index;
  setLocation(loc);
}

void ArrayReference::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :type ";
  type[0]->printType();
  llvm::outs() << " :array ";
  array->printAST();
  llvm::outs() << " :index ";
  index->printAST();
  llvm::outs() << " ";
  PrintLocation();
  llvm::outs() <<"}";
}

class FunctionCall : public Expression {
public:
  Node *func;
  std::vector<Node *> parm;
  FunctionCall(DataType *dt, Node *func, std::vector<Node *> parm, Node loc);
  void printAST();
};

FunctionCall::FunctionCall(DataType *dt, Node *func, std::vector<Node *> parm, Node loc) {
  kind = "FuncCall";
  type.push_back(dt);
  func = func;
  parm(parm);
  setLocation();
}

void FunctionCall::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :type ";
  type[0]->printType();
  llvm::outs() << "\n :func ";
  func->printAST();
  llvm::outs() << "\n :parm [";
  for (int i = 0; i < (int)parm.size(); i++) {
    parm[i]->printAST();
  }
  llvm::outs() << "] ";
  PrintLocation();
  llvm::outs() << "}";
}

class Declation : public Node {
public:
  std::string name;
};

class FieldDeclation : public Declation {
public:
  std::string scope;
  DataType *type;
  FieldDeclation(std::string name, DataType *dt, Node loc);
  void printAST();
};

FieldDeclation::FieldDeclation(std::string name, DataType *dt, Node loc) {
  kind = "FieldDecl";
  scope = "Member";
  name = name;
  type = dt;
  setLocation(loc);
}

void FieldDeclation::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \" " << name 
               << "\" :scope \"" << scope << "\" :type";
  type->printType();
  PrintLocation();
  llvm::outs() << "}\n";
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
  ParameterDeclation(std::string name, std::string dispType, DataType *dt, Node loc, bool au, bool sta);
  void printAST();
};

ParameterDeclation::ParameterDeclation(std::string name, std::string dispType, DataType *dt, Node loc, bool au, bool sta) {
  kind = "ParmDecl";
  name = name;
  displayType = dispType;
  type = dt;
  setLocation(loc);
  autoBool = au;
  staticBool = sta;
}

void ParameterDeclation::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name << "\" :display-type \"" 
               << displayType << "\" :type ";
  type->printType();
  PrintLocation();
  llvm::outs() << "}";
}

class VariableDeclation : public DeclationOfVariables {
public:
  Node *init;
  VariableDeclation(std::string name, std::string scope, std::string dispType, DataType *dt, Node *init, Node loc, bool au, bool sta);
  void printAST();
};

VariableDeclation::VariableDeclation(std::string name, std::string scope, std::string dispType, DataType *dt, Node *init, Node loc, bool au, bool sta) {
  kind = "VarDecl";
  name = name;
  scope = scope;
  displayType = dispType;
  type = dt;
  init = init;
  setLocation(loc);
  autoBool = au;
  staticBool = sta;
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
  if (init != NULL) {
    llvm::outs() << "\n :init ";
    init->printAST();
  }
  llvm::outs() << "}\n";
}

class FunctionDeclation : public Declation {
public:
  DataType *type;
  std::vector<Node *> parm;
  std::vector<Node *> body;
  FunctionDeclation(std::string name, DataType *dt, std::vector<Node *> parm, std::vector<Node *> body, Node loc);
  void printAST();
};

FunctionDeclation::FunctionDeclation(std::string name, DataType *dt, std::vector<Node *> parm, std::vector<Node *> body, Node loc) {
  kind = "FuncDecl";
  name = name;
  type = dt;
  parm(parm);
  body(body);
  setLocation(loc);
}

void FunctionDeclation::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name << "\" :type ";
  type->printType();
  PrintLocation();
  llvm::outs() << "\n :Parm [";
  if (parm.size() != 0) {
    for (int i = 0; i < (int)parm.size(); i++) {
      parm[i]->printAST();
    }
  }
  llvm::outs() << "]\n :body [";
  if (body.size() != 0) {
    for (int i = 0; i < (int)body.size(); i++) {
      body[i]->printAST();
    }
  }
  llvm::outs() << "]}\n";
}

class TypeDeclation : public Declation {
public:
  std::vector<Node *> member;
};

class StructDeclation : public TypeDeclation {
public:
  StructDeclation(std::string name, std::vector<Node *> member, Node loc);
  void printAST();
};

StructDeclation::StructDeclation(std::string name, std::vector<Node *> member, Node loc) {
  kind = "StructDecl";
  name = name;
  member(member);
  setLocation(loc);
}

void StructDeclation::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name << "\"";
  PrintLocation();
  llvm::outs() << "\n :member [";
  for (int i = 0; i < (int)member.size(); i++) {
    member[i]->printAST();
  }
  llvm::outs() << "]}\n";
}

class UnionDeclation : public TypeDeclation {
public:
  UnionDeclation(std::string name, std::vector<Node *> member, Node loc);
  void printAST();
};

UnionDeclation::UnionDeclation(std::string name, std::vector<Node *> member, Node loc) {
  kind = "UnionDecl";
  name = name;
  member(member);
  setLocation(loc);
}

void UnionDeclation::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name << "\""
  PrintLocation();
  llvm::outs() << "\n :member [";
  for (int i = 0; i < (int)member.size(); i++) {
    member[i]->printAST();
  }
  llvm::outs() << "]}\n";
}

class Statement : public Node {
};

class RepetitionStatement : public Statement {
public:
  Node *condition;
  std::vector<Node *> body;
};

class WhileStatement : public RepetitionStatement {
public:
  WhileStatement(Node *cond, std::vector<Node *> body, Node loc);
  void printAST();
};

WhileStatement::WhileStatement(Node *cond, std::vector<Node *> body, Node loc) {
  kind = "While";
  condition = cond;
  body(body);
  setLocation(loc);
}

void WhileStatement::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  PrintLocation();
  llvm::outs() << "\n :condition ";
  condition->printAST();
  llvm::outs() << "\n :body [";
  for (int i = 0; i < (int)body.size(); i++) {
    body[i]->printAST();
  }
  llvm::outs() << "]}\n";
}

class DoStatement : public RepetitionStatement {
public:
  DoStatement(Node *cond, std::vector<Node *> body, Node loc);
  void printAST();
};

DoStatement::DoStatement(Node *cond, std::vector<Node *> body, Node loc) {
  kind = "Do";
  condition = cond;
  body(body);
  setLocation(loc);
}

void DoStatement::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  PrintLocation();
  llvm::outs() << "\n :condition ";
  condition->printAST();
  llvm::outs() << "\n :body [";
  for (int i = 0; i < (int)body.size(); i++) {
    body[i]->printAST();
  }
  llvm::outs() << "]}\n";
}

class ForStatement : public RepetitionStatement {
public:
  Node *init;
  Node *update;
  ForStatement(Node *cond, std::vector<Node *> body, Node *init, Node *update, Node loc);
  void printAST();
};

ForStatement::ForStatement(Node *cond, std::vector<Node *> body, Node *init, Node *update, Node loc) {
  kind = "For";
  condition = cond;
  body(body);
  init = init;
  update = update;
  setLocation(loc);
}

void ForStatement::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  PrintLocation();
  llvm::outs() << "\n :init ";
  init->printAST();
  llvm::outs() << "\n :condition ";
  condition->printAST();
  llvm::outs() << "\n :update ";
  update->printAST();
  llvm::outs() << "\n :body [";
  for (int i = 0; i < (int)body.size(); i++) {
    body[i]->printAST();
  }
  llvm::outs() << "]}\n";
}

class BranchStatement : public Statement {
public:
  Node *condition;
};

class IfStatement : public BranchStatement {
public:
  std::vector<Node *> then;
  std::vector<Node *> denial;
  IfStatement(Node *cond, std::vector<Node *> then, std::vector<Node *> denial, Node loc);
  void printAST();
};

IfStatement::IfStatement(Node *cond, std::vector<Node *> then, std::vector<Node *> denial, Node loc) {
  kind = "If";
  condition = cond;
  then(then);
  denial(denial);
  setLocation(loc);
}

void IfStatement::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  PrintLocation();
  llvm::outs() << "\n :condition ";
  condition->printAST();
  llvm::outs() << "\n :then [";
  for (int i = 0; i < (int)then.size(); i++) {
    then[i]->printAST();
  }
  llvm::outs() << "]";
  if (denial.size() != 0) {
    llvm::outs() << "\n :else [";
    for (int i = 0; i < (int)denial.size(); i++) {
      denial[i]->printAST();
    }
    llvm::outs() << "] ";
  }
  llvm::outs() << "}\n";
}

class SwitchStatement : public BranchStatement {
public:
  std::vector<Node *> body;
  SwitchStatement(Node *cond, std::vector<Node *> body, Node loc);
  void printAST();
};

SwitchStatement::SwitchStatement(Node *cond, std::vector<Node *> body, Node loc) {
  kind = "Switch";
  condition = cond;
  body(body);
  setLocation(loc);
}

void SwitchStatement::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  PrintLocation();
  llvm::outs() << "\n :condition ";
  condition->printAST();
  llvm::outs() << "\n :body [";
  for (int i = 0; i < (int)body.size(); i ++) {
    body[i]->printAST();
  }
  llvm::outs() << "]}\n";
}

class LabelStatement : public Statement {
public:
  std::string name;
  LabelStatement(std::string name, Node loc);
  void printAST();
};

LabelStatement::LabelStatement(std::string name, Node loc) {
  kind = "Label";
  name = name;
  setLocation(loc);
}

void LabelStatement::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name << "\" ";
  PrintLocation();
  llvm::outs() << "}";
}

class CaseStatement : public Statement {
public:
  Node *value;
  CaseStatement(Node *val, Node loc);
  void printAST();
};

CaseStatement::CaseStatement(Node *val, Node loc) {
  kind = "Case";
  value = val;
  setLocation(loc);
}

void CaseStatement::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  PrintLocation();
  llvm::outs() << "\n :value ";
  value->printAST();
  llvm::outs() << "}\n";
}

class DefaultStatement : public Statement {
public:
  DefaultStatement(Node loc);
  void printAST();
};

DefaultStatement::DefaultStatement(Node loc) {
  kind = "Default";
  setLocation(loc);
}

void DefaultStatement::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  PrintLocation();
  llvm::outs() << "}\n";
}

class GotoStatement : public Statement {
public:
  std::string jump;
  GotoStatement(std::string jump, Node loc);
  void printAST();
};

GotoStatement::GotoStatement(std::string jump, Node loc) {
  kind = "Goto";
  jump = jump;
  setLocation(loc);
}

void GotoStatement::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :goto \"" << jump << "\" ";
  PrintLocation();
  llvm::outs() << "}\n";
}

class ContinueStatement : public Statement {
public:
  ContinueStatement(Node loc);
  void printAST();
};

ContinueStatement::ContinueStatement(Node loc) {
  kind = "Continue";
  setLocation(loc);
}

void ContinueStatement::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" ";
  PrintLocation();
  llvm::outs() << "}\n";
}

class BreakStatement : public Statement {
public:
  BreakStatement(Node loc);
  void printAST();
};

BreakStatement::BreakStatement(Node loc) {
  kind = "Break";
  setLocation(loc);
}

void BreakStatement::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" ";
  PrintLocation();
  llvm::outs() << "}\n";
}

class ReturnStatement : public Statement {
public:
  Node *value;
  ReturnStatement(Node *val, Node loc);
  void printAST();
};

ReturnStatement::ReturnStatement(Node *val, Node loc) {
  kind = "Ret";
  value = val;
  setLocation(loc);
}

void ReturnStatement::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  PrintLocation();
  llvm::outs() << "\n :value ";
  value->printAST();
  llvm::outs() << "}";
}

class MyAstVisitor : public RecursiveASTVisitor<MyAstVisitor> {
private:
  std::vector<Node *> prog;
  std::vector<DataType *> castType;

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
	}
	RecursiveASTVisitor::TraverseDecl(decl);
	break;
      case Decl::Function: 
        //llvm::outs() << "decl:2";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	}
	RecursiveASTVisitor::TraverseDecl(decl);
	break;
      case Decl::ParmVar: 
        //llvm::outs() << "decl:3";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
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
	}
	RecursiveASTVisitor::TraverseDecl(decl);
	break;
      case Decl::Var: 
        //llvm::outs() << "decl:7";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
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
    std::string name = (std::string)field->getName();
    DataType *type = PrintTypeInfo(fieldtype);
    Node t = PrintSourceRange(field->getSourceRange());

    FieldDeclation *FD = new FieldDeclation(name, type, t);
    Node *np = FD;
    prog.push_back(np);

    return false;
  }
  
  // FunctionDecl
  bool VisitFunctionDecl(FunctionDecl *Decl) {
    std::string name = Decl->getQualifiedNameAsString();
    DataType *type = PrintTypeInfo(Decl->getType());
    std::vector<Node *> parm;
    std::vector<Node *> body;

    if (Decl->param_size()) {
      int i = prog.size();
      int j = Decl->param_size();
      for (int k = 0; k < j; k++) {
        TraverseDecl(Decl->getParamDecl(k));
        parm.push_back(prog[i]);
        prog.erase(prog.begin() + i);
      }
    }
    int i = prog.size();
    TraverseStmt(Decl->getBody());
    int j = prog.size();
    if (i < j) {
      for (int k = i; k < j; k++) {
        body.push_back(prog[i]);
        prog.erase(prog.begin() + i);
      }
    }
    Node t = PrintSourceRange(Decl->getSourceRange());

    FunctionDeclation *FD = new FunctionDeclation(name, type, parm, body, t);
    Node *np = FD;
    prog.push_back(np);

    return false;
  }

  // ParmVarDecl
  bool VisitParmVarDecl(ParmVarDecl *Decl) {
    ParameterDeclation *PD = new ParameterDeclation();
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

    // 修正版
    PD->name = Decl->getNameAsString();
    PD->type = PrintTypeInfo(vartype);
    Node t = PrintSourceRange(Decl->getSourceRange());
    PD->beginFile = t.beginFile;
    PD->beginLine = t.beginLine;
    PD->beginColumn = t.beginColumn;
    PD->endFile = t.endFile;
    PD->endLine = t.endLine;
    PD->endColumn = t.endColumn;
    Node *np = PD;
    prog.push_back(np);

    return true;
  }

  // RecordDecl (Structure?)
  bool VisitRecordDecl(RecordDecl *record) {
    /*
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
    */
    if (record->isStruct()) {
      SD->name = (std::string)record->getName();
      std::vector<Node *> member;
      if (!(record->field_empty())) {
        RecordDecl::field_iterator itr = record->field_begin();
        int i = prog.size();
        while (itr != record->field_end()) {
          TraverseDecl(itr->getCanonicalDecl());
          member.push_back(prog[i]);
          prog.erase(prog.begin() + i);
          itr++;
        }
      }
      Node t = PrintSourceRange(record->getSourceRange());

      StructDeclation *SD = new StructDeclation(name, member, t);
      Node *np = SD;
      prog.push_back(np);
    } else if (record->isUnion()) {
    }
    return false;
  }

  // VarDecl
  bool VisitVarDecl(VarDecl *Decl) {
    if (Decl->getKind() == 50) {
      return true;
    }
    QualType vartype = Decl->getType();
    std::string name = Decl -> getNameAsString();
    std::string scope = (Decl -> isFileVarDecl() == 1 ? "global" : "local");
    DataType *type = PrintTypeInfo(vartype);
    std::stirng displayType = PrintDisplayType(vartype);
    Node *init;
    if (Decl->hasInit()) {
      int i = prog.size();
      TraverseStmt(Decl->getInit());
      if (i < (int)prog.size()) {
        init = prog[i];
        prog.pop_back();
      }
    }
    Node t = PrintSourceRange(Decl->getSourceRange());

    VariableDeclation *VD = new VariableDeclation(name, scope, displayType, type, init, t, false, false);
    Node *np = VD;
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
    /*
    if (labelflag != 0 || castflag != 0) {
      os << " :DisplayType " << "\"" << typeInfo.getAsString() << "\"";
    } else {
      llvm::outs() << " :DisplayType " << "\"" << typeInfo.getAsString() << "\"";
    }
    */
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

    DataType *tp = PrintQualifier(typeInfo);
    TDF->typeName = (std::string)TDtype->getName();
    TDF->constBool = tp->constBool;
    TDF->volatileBool = tp->volatileBool;
    TDF->typedefType = PrintTypeInfo(TDtype->getUnderlyingType());

    return TDF;
  }

  DataType *PrintBuiltinTypeInfo(QualType typeInfo) {
    DataType *tp = PrintQualifier(typeInfo);
    bool constBool = tp->constBool;
    bool volatileBool = tp->volatileBool;

    DataType *t;
    switch (dyn_cast<BuiltinType>(typeInfo)->getKind()) {
      case BuiltinType::Void:
      {
	t = new VoidType();
        break;
      }
      case BuiltinType::Bool:
      {
	t = new BoolType();
        break;
      }
      case BuiltinType::UChar:
      {
	t = new UnsignedCharType(constBool, volatileBool);
        break;
      }
      case BuiltinType::UShort:
      {
	t = new UnsignedShortType(constBool, volatileBool);
        break;
      }
      case BuiltinType::UInt:
      {
	t = new UnsignedIntType(constBool, volatileBool);
        break;
      }
      case BuiltinType::ULong:
      {
	t = new UnsignedLongType(constBool, volatileBool);
        break;
      }
      case BuiltinType::ULongLong:
      {
	t = new UnsignedLongLongType(constBool, volatileBool);
        break;
      }
      case BuiltinType::Char_S:
      {
	t = new CharType(constBool, volatileBool);
        break;
      }
      case BuiltinType::SChar:
      {
	t = new CharType(constBool, volatileBool);
        break;
      }
      case BuiltinType::Short:
      {
	t = new ShortType(constBool, volatileBool);
        break;
      }
      case BuiltinType::Int:
      {
	t = new IntType(constBool, volatileBool);
        break;
      }
      case BuiltinType::Long:
      {
	t = new LongType(constBool, volatileBool);
        break;
      }
      case BuiltinType::LongLong:
      {
	t = new LongLongType(constBool, volatileBool);
        break;
      }
      case BuiltinType::Float:
      {
	t = new FloatType(constBool, volatileBool);
        break;
      }
      case BuiltinType::Double:
      {
	t = new DoubleType(constBool, volatileBool);
        break;
      }
      case BuiltinType::LongDouble:
      {
	t = new LongDoubleType(constBool, volatileBool);
        break;
      }
	//以下c++に関するもの
      case BuiltinType::WChar_S:
      case BuiltinType::WChar_U:
      {
	//Typename = "WChar_t";
	break;
      }
      case BuiltinType::Dependent:
      {
	//Typename = "Dependent";
	break;
      }
      default:
      {
	llvm::outs() << "\n \""<< typeInfo.getAsString() << "\"は, 初出です."
		     << " astprint.cppのPrintTypeInfoにcaseを追加して下さい.";
	break;
      }
    }
    
    return t;
  }

  FuncType *PrintFunctionTypeInfo(QualType typeInfo) {
    std::vector<DataType *> parmType
    DataType *retType
    if (dyn_cast<FunctionProtoType>(typeInfo)) {
      unsigned parms = dyn_cast<FunctionProtoType>(typeInfo)->getNumArgs();
      if (parms != 0) {
       unsigned i = 0;
       while (i < parms) {
	  parmType.push_back(PrintTypeInfo(dyn_cast<FunctionProtoType>(typeInfo)->getArgType(i)));
	  i++;
       }
      }
    }
    if (dyn_cast<FunctionType>(typeInfo)) {
      QualType rettype = dyn_cast<FunctionType>(typeInfo)->getResultType();
      retType = PrintTypeInfo(rettype);
    } 

    FuncType *t = new FuncType(parmType, retType);
    return t;
  }

  //void PrintArrayTypeInfo(QualType typeInfo) {
  ArrayDataType *PrintArrayTypeInfo(QualType typeInfo) {
    QualType elmtype = dyn_cast<ArrayType>(typeInfo)->getElementType();
    std::string arraySize = dyn_cast<ConstantArrayType>(typeInfo)->getSize().toString(10, true);
    DataType *type = PrintTypeInfo(elmtype);

    ArrayDataType *t = new ArrayDataType(arraySize, type);
    return t;
  }

  //void PrintPointerTypeInfo(QualType typeInfo) {
  PointeeType *PrintPointerTypeInfo(QualType typeInfo) {
    QualType elmtype = dyn_cast<PointerType>(typeInfo)->getPointeeType();
    DataType *pointee = PrintTypeInfo(elmtype);

    PointeeType *t = new PointeeType(pintee);
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
  
  DataType *PrintQualifier(QualType Qual) {
    DataType *DT = new DataType();
    if (Qual.isConstQualified()) 
      DT->constBool = true;
    if (Qual.isVolatileQualified())
      DT->volatileBool = true;
    return DT;
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
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::ForStmtClass: 
        //llvm::outs() << "stmt:8";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::GotoStmtClass: 
        //llvm::outs() << "stmt:9";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::IfStmtClass: 
        //llvm::outs() << "stmt:10";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
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
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::WhileStmtClass: 
        //llvm::outs() << "stmt:13";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::ReturnStmtClass: 
        //llvm::outs() << "stmt:14";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
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
	}
	ArraySub = 1;
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::CallExprClass: 
        //llvm::outs() << "stmt:16";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
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
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::ConditionalOperatorClass: 
        //llvm::outs() << "stmt:24";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
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
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::CharacterLiteralClass: 
        //llvm::outs() << "stmt:27";
	if (linefeedflag == 0) {
	  linefeedflag = 1;
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::FloatingLiteralClass: 
        //llvm::outs() << "stmt:28";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::IntegerLiteralClass: 
        //llvm::outs() << "stmt:29";
	if (linefeedflag == 0) {
	  linefeedflag = 1;
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::StringLiteralClass: 
        //llvm::outs() << "stmt:30";
	if (linefeedflag == 0 || linefeedbody == 0) {
	  linefeedflag = 1;
	  linefeedbody = 1;
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

  // BreakStmt
  bool VisitBreakStmt(BreakStmt *Break) {
    Node t = PrintSourceRange(Break->getSourceRange());

    BreakStatement *BS = new BreakStatement(t);
    Node *np = BS;
    prog.push_back(np);

    return true;
  }
 
  // CaseStmt
  bool VisitCaseStmt(CaseStmt *Case) {
    int i = prog.size();
    Node *value;
    TraverseStmt(Case->getLHS());
    value = prog[i];
    prog.pop_back();
    Node t = PrintSourceRange(Case->getSourceRange());

    CaseStatement *CS = new CaseStatement(value, t);
    Node *np = CS;
    prog.push_back(np);
    skipcount = 1;

    return true;
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
    Node t = PrintSourceRange(Default->getSourceRange());

    DefaultStatement *DS = new DefaultStatement(t);
    Node *np = DS;
    prog.push_back(np);

    return true;
  }

  // DoStmt
  bool VisitDoStmt(DoStmt *Do) {
    int i = prog.size();
    TraverseStmt(Do->getCond());
    Node *condition = prog[i];
    prog.pop_back();
    i = prog.size();
    TraverseStmt(Do->getBody());
    int j = prog.size();
    std::vector<Node *> body;
    for (int k = i; k < j; k++) {
      body.push_back(prog[i]);
      prog.erase(prog.begin() + i);
    }
    Node t = PrintSourceRange(Do->getSourceRange());

    DoStatement *DS = new DoStatement(condition, body, t);
    Node *np = DS;
    prog.push_back(np);

    return false;
  }

  // ForStmt
  bool VisitForStmt(ForStmt *For) {
    int i = prog.size();
    Node *init;
    if (For->getInit() != NULL) {
      TraverseStmt(For->getInit());
      init = prog[i];
      prog.pop_back();
    }
    i = prog.size();
    Node *condition;
    if (For->getCond() != NULL) {
      TraverseStmt(For->getCond());
      condition = prog[i];
      prog.pop_back();
    }
    i = prog.size();
    Node *update;
    if (For->getInd() != NULL) {
      TraverseStmt(For->getInc());
      update = prog[i];
      prog.pop_back();
    }
    i = prog.size();
    TraverseStmt(For->getBody());
    int j = prog.size();
    std::vector<Node *> prog;
    for (int k = i; k < j; k++) {
      body.push_back(prog[i]);
      prog.erase(prog.begin() + i);
    }

    Node t = PrintSourceRange(For->getSourceRange());

    ForStatement *FS = new ForStatement(condition, body, init, update, t);
    Node *np = FS;
    prog.push_back(np);
      
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
    int i = prog.size();
    TraverseStmt(If->getCond());
    Node *condition = prog[i];
    prog.pop_back();
    i = prog.size();
    TraverseStmt(If->getThen());
    int j = prog.size();
    std::vector<Node *> then;
    for (int k = i; k < j; k++) {
      then.push_back(prog[i]);
      prog.erase(prog.begin() + i);
    }
    std::vector<Node *> denial;
    if (If->getElse()) {
      i = prog.size();
      TraverseStmt(If->getElse());
      j = prog.size();
      for (int k = i; k < j; k++) {
        denial.push_back(prog[i]);
        prog.erase(prog.begin() + i);
      }
    }
    Node t = PrintSourceRange(If->getSourceRange());

    IfStatement *IS = new IfStatement(condition, then, denial, t);
    Node *np = IS;
    prog.push_back(np);

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
    int i = prog.size();
    TraverseStmt(Switch->getCond());
    Node *condition = prog[i];
    prog.pop_back();
    i = prog.size();
    TraverseStmt(Switch->getBody());
    int j = prog.size();
    std::vector<Node *> body;
    for (int k = i; k < j; k++) {
      body.push_back(prog[i]);
      prog.erase(prog.begin() + i);
    }
    Node t = PrintSourceRange(Switch->getSourceRange());

    SwitchStatement *SS = new SwitchStatement(condition, body, t);
    Node *tp = SS;
    prog.push_back(tp);

    return false;
  }
  
  // WhileStmt
  bool VisitWhileStmt(WhileStmt *While) {
    int i = prog.size();
    TraverseStmt(While->getCond());
    Node *condition = prog[i];
    prog.pop_back();
    i = prog.size();
    TraverseStmt(While->getBody());
    int j = prog.size();
    std::vector<Node *> body;
    for (int k = i; k < j; k++) {
      body.push_back(prog[i]);
      prog.erase(prog.begin() + i);
    }
    Node t = PrintSourceRange(While->getSourceRange());

    WhileStatement *WS = new WhileStatement(condition, body, t);
    Node *np = WS;
    prog.push_back(np);

    return false;
  }
 
  // ReturnStmt
  bool VisitReturnStmt(ReturnStmt *Ret) {
    Node *value;
    if (Ret->getRetValue()) {
      int i = prog.size();
      TraverseStmt(Ret->getRetValue());
      value = prog[i];
      prog.pop_back();
    }
    Node t = PrintSourceRange(Ret->getSourceRange());

    ReturnStatement *RS = new ReturnStatement(value, t);
    Node *tp = RS;
    prog.push_back(tp);

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
      castType.push_back(PrintTypeInfo(casttype)); 
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

    // 修正版
    DeclationReferenceExpression *DRE;
    if (vardecl) {
      QualType vartype = vardecl->getType();
      std::string name = Declref->getNameInfo().getAsString();
      std::string scope = (vardecl->isFileVarDecl() == 1 ? "global" : "local");
      std::vector<DataType *> type;
      type.push_back(PrintTypeInfo(vartype));
      if (castType.size() != 0) {
        for (int i = 0; i < (int)castType.size(); i++) {
          type.push_back(castType[0]);
          castType.erase(castType.begin());
        }
      }
      Node t = PrintSourceRange(Declref->getSourceRange());

      DRE = new DeclationReferenceExpression(name, scope, type, t);
    } else if (funcdecl) {

    }
    // 既存版
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
      if (vardecl) {
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

    Node *np = DRE;
    prog.push_back(np);

    return true;
  }

  // ImplicitCastExpr
  bool VisitImplicitCastExpr(ImplicitCastExpr *icast) {
    if (icast->getCastKind() != 3) {
      QualType casttype = icast->getType();
      castType.push_back(PrintTypeInfo(casttype)); 
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
      int i = prog.size();
      TraverseStmt(base);
      Node *structs = prog[i];
      prog.pop_back();
      i = prog.size();
      TraverseDecl(vdecl);
      Node *structMember = prog[i];
      prog.pop_back();
      std::vector<DataType *> type;
      type.push_back(PrintTypeInfo(memtype));
      if (castType.size() != 0) {
        for (int i = 0; i < (int)castType.size(); i++) {
          type.push_back(castType[0]);
          castType.erase(castType.begin());
        }
      }
      Node t = PrintSourceRange(mem->getSourceRange());

      StructReference *SR = new StructReference(structs, structMember, type, t);
      Node *np = SR;
      prog.push_back(np);
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
    std::string op = Unop->getOpcodeStr(opcode);
    std::vector<DataType *> type;
    type.push_back(PrintTypeInfo(Unop->getType()));
    if (castType.size() != 0) {
      for (int i = 0; i < (int)castType.size(); i++) {
        type.push_back(castType[0]);
        castType.erase(castType.begin());
      }
    }
    int i = prog.size();
    Node *operand;
    TraverseStmt(Unop->getSubExpr());
    if (i < (int)prog.size()) {
      operand = prog[i];
      prog.pop_back();
    }
    Node t = PrintSourceRange(Unop->getSourceRange());

    UnOp *UO = new UnOp(op, type, operand, t);
    Node *np = UO;
    prog.push_back(np);

    return false;
  }

  // BinaryOperator
  bool VisitBinaryOperator(BinaryOperator *Binop) {
    BinOp *BO = new BinOp();
    BO->op = Binop->getOpcodeStr();
    BO->type.push_back(PrintTypeInfo(Binop->getType()));
    if (castType.size() != 0) {
      for (int i = 0; i < (int)castType.size(); i++) {
        BO->type.push_back(castType[0]);
        castType.erase(castType.begin());
      }
    }
    int i = prog.size();
    TraverseStmt(Binop->getLHS());
    BO->left = prog[i];
    prog.pop_back();
    i = prog.size();
    TraverseStmt(Binop->getRHS());
    BO->right = prog[i];
    prog.pop_back();

    Node t = PrintSourceRange(Binop->getSourceRange());
    BO->beginFile = t.beginFile;
    BO->beginLine = t.beginLine;
    BO->beginColumn = t.beginColumn;
    BO->endFile = t.endFile;
    BO->endLine = t.endLine;
    BO->endColumn = t.endColumn;

    Node *np = BO;
    prog.push_back(np);

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
    IntLiteral *IL = new IntLiteral();
    QualType vartype = Int->getType();
    IL->value = Int->getValue().toString(10, true);
    IL->type.push_back(PrintTypeInfo(vartype));
    if (castType.size() != 0) {
      for (int i = 0; i < (int)castType.size(); i++) {
        IL->type.push_back(castType[0]);
        castType.erase(castType.begin());
      }
    }
    Node t = PrintSourceRange(Int->getSourceRange());
    IL->beginFile = t.beginFile;
    IL->beginLine = t.beginLine;
    IL->beginColumn = t.beginColumn;
    IL->endFile = t.endFile;
    IL->endLine = t.endLine;
    IL->endColumn = t.endColumn;
    Node *np = IL;
    prog.push_back(np);

    return true;
  }

  // FloatingLiteral
  bool VisitFloatingLiteral(FloatingLiteral *Float) {
    FloatLiteral *FL = new FloatLiteral();
    QualType vartype = Float->getType();
    FL->value = Float->getValueAsApproximateDouble();
    FL->type.push_back(PrintTypeInfo(vartype));
    if (castType.size() != 0) {
      for (int i = 0; i < (int)castType.size(); i++) {
        FL->type.push_back(castType[0]);
        castType.erase(castType.begin());
      }
    }
    Node t = PrintSourceRange(Float->getSourceRange());
    FL->beginFile = t.beginFile;
    FL->beginLine = t.beginLine;
    FL->beginColumn = t.beginColumn;
    FL->endFile = t.endFile;
    FL->endLine = t.endLine;
    FL->endColumn = t.endColumn;
    Node *np = FL;
    prog.push_back(np);

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
    CharLiteral *CL = new CharLiteral();
    QualType vartype = Char->getType();
    CL->value = Char->getValue();
    CL->type.push_back(PrintTypeInfo(vartype));
    if (castType.size() != 0) {
      for (int i = 0; i < (int)castType.size(); i++) {
        CL->type.push_back(castType[0]);
        castType.erase(castType.begin());
      }
    }
    Node t = PrintSourceRange(Char->getSourceRange());
    CL->beginFile = t.beginFile;
    CL->beginLine = t.beginLine;
    CL->beginColumn = t.beginColumn;
    CL->endFile = t.endFile;
    CL->endLine = t.endLine;
    CL->endColumn = t.endColumn;
    Node *np = CL;
    prog.push_back(np);

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
  Node PrintSourceRange(SourceRange range) {
    Node t;
    FullSourceLoc FullLocationBegin = Context->getFullLoc(range.getBegin());
    FullSourceLoc FullLocationEnd = Context->getFullLoc(range.getEnd());
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
    //if (labelflag != 0)
    //  llvm::outs() << "labelflag != 0 in checkCast";
    while (!ct.empty()) {
      llvm::outs() << " " << ct.top();
      ct.pop();
    }
  }
  
  // ASTの出力
  void printAST() {
    for (int i = 0; i != (int)prog.size(); i++) {
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
    llvm::outs() << "\n----------------------------------------------------------------------\n";
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
