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
  std::string getBeginFile();
  virtual void printAST();
  virtual ~Node() {};
};

void Node::setLocation(Node loc) {
  beginFile = loc.beginFile;
  beginLine = loc.beginLine;
  beginColumn = loc.beginColumn;
  endFile = loc.endFile;
  endLine = loc.endLine;
  endColumn = loc.endColumn;
}

std::string Node::getBeginFile() {
  return beginFile;
}

void Node::PrintLocation() {
  llvm::outs() << " :loc-begin [\"" << beginFile << "\" " << beginLine << " " << beginColumn 
  	   << "] :loc-end [\"" << endFile << "\" " << endLine << " " << endColumn << "]";
}

void Node::printAST() {
  llvm::outs() << "{:kind \"Unsupported\"}";
}


class DataType : public Node {
public:
  std::string typedefString;
  bool constBool;
  bool volatileBool;
  void setTypedefString(std::string ts);
  virtual void printType();
  virtual ~DataType() {};
};

void DataType::setTypedefString(std::string ts) {
  typedefString = ts;
}

void DataType::printType() {
  llvm::outs() << "{:type \"Unknown-type\" :typedef-string \"" << typedefString << "\"}";
}

class VoidType : public DataType {
public:
  VoidType();
  void printType();
};

VoidType::VoidType() {
  kind = "VoidType";
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
  kind = "BoolType";
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
  llvm::outs() << "{:kind \"" << kind << "\" :pointee ";
  pointee->printType();
  llvm::outs() << "}";
}

class FuncType : public DataType {
public:
  std::vector<DataType*> parmType;
  DataType *retType;
  FuncType(std::vector<DataType *> pt, DataType *rt);
  void printType();
};

FuncType::FuncType(std::vector<DataType *> pt, DataType *rt) {
  kind = "FuncType";
  parmType = pt;
  retType = rt;
}

void FuncType::printType() {
  int i;
  llvm::outs() << "{:kind \"" << kind << "\"";
  llvm::outs() << " :parms-type [";
  if (parmType.size() != 0 ) {
    for (i = 0; i < (int)parmType.size() - 1; i++) {
      parmType[i]->printType();
      llvm::outs() << " ";
    }
    parmType[(int)parmType.size() - 1]->printType();
  }
  llvm::outs() << "]";
  llvm::outs() << " :ret-type ";
  if (retType != NULL) {
    retType->printType();
  }
  llvm::outs() << "}";
}

class ArrayDataType : public DataType {
public:
  std::string ConstArraySize;
  Node *VarArraysize;
  DataType *type;
  ArrayDataType(std::string size, Node *VarSize, DataType *dt);
  void printType();
};

ArrayDataType::ArrayDataType(std::string ConstSize, Node *VarSize, DataType *dt) {
  kind = "ArrayType";
  ConstArraySize = ConstSize;
  VarArraysize = VarSize;
  type = dt;
}

void ArrayDataType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  llvm::outs() << " :array-size ";
  if (ConstArraySize != "") {
    llvm::outs() << "\"" << ConstArraySize << "\"";
  } else if (VarArraysize != NULL) {
    VarArraysize->printAST();
  } else {
    llvm::outs() << "[]";
  }
  llvm::outs() << " :type ";
  type->printType();
  llvm::outs() << "}";
}

class StructureType : public DataType {
public:
  std::string name;
  std::vector<Node *> member;
  StructureType(std::string n, std::vector<Node *> mem);
  void printType();
};

StructureType::StructureType(std::string n, std::vector<Node *> mem) {
  kind = "StructureType";
  name = n;
  member = mem;
}

void StructureType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name << "\"";
  if (member.size() != 0) {
    llvm::outs() << " :member [";
    for (int i = 0; i < (int)member.size(); i++) {
      member[i]->printAST();
    }
    llvm::outs() << "]";
  }
  llvm::outs() << "}";
}

class UnionType : public DataType {
public:
  std::string name;
  std::vector<Node *> member;
  UnionType(std::string n, std::vector<Node *> mem);
  void printType();
};

UnionType::UnionType(std::string n, std::vector<Node *> mem) {
  kind = "UnionType";
  name = n;
  member = mem;
}

void UnionType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name << "\"";
  if (member.size() != 0) {
    llvm::outs() << " :member [";
    for (int i = 0; i < (int)member.size(); i++) {
      member[i]->printAST();
    }
    llvm::outs() << "]";
  }
  llvm::outs() << "}";
}

// typedefType
class RenameType : public DataType {
public:
  std::string typeName;
  DataType *typedefType;
  RenameType(std::string tn, DataType *defType);
  void printType();
};

RenameType::RenameType(std::string tn, DataType *defType) {
  kind = "TypedefType";
  typeName = tn;
  typedefType = defType;
}

void RenameType::printType() {
  llvm::outs() << "{:kind \"" << kind << "\" :type-name \"" << typeName << "\" :typedef-type ";
  typedefType->printType();
  llvm::outs() << "}";
}

class Speci : public Node {
public:
  bool Static;
  bool Auto;
  bool Extern;
  bool Register;
  Speci(bool e, bool s, bool a, bool r);
  void printSpecifier();
};

Speci::Speci(bool e, bool s, bool a, bool r) {
  Extern = e;
  Static = s;
  Auto = a;
  Register = r;
}

void Speci::printSpecifier() {
  if (Static == true)
    llvm::outs() << " :static \"true\"";
  if (Extern == true)
    llvm::outs() << " :extern \"true\"";
  if (Register == true)
    llvm::outs() << " :register \"true\"";
  if (Auto == true)
    llvm::outs() << " :auto \"true\"";

}

class Expression : public Node {
public:
  std::vector<DataType *> type;
  virtual void printAST();
};

void Expression::printAST() {
  llvm::outs() << "{:kind \"No Expression\"}";
}

class Reference : public Expression {
public:
  std::string name;
  std::vector<Node *> label;
  void setLabel(Node *l);
  void printLabel();
};

void Reference::setLabel(Node *l) {
  label.push_back(l);
}

void Reference::printLabel() {
  llvm::outs() << " :label [";
  for (int i = 0; i < (int)label.size(); i++) {
    label[i]->printAST();
  }
  llvm::outs() << "]";
}

class DeclarationReferenceExpression : public Reference {
public:
  std::string scope;
  Speci *spe;
  DeclarationReferenceExpression(std::string n, std::string s, std::vector<DataType *> dts, Node loc, Speci *sp);
  void printAST();
};

DeclarationReferenceExpression::DeclarationReferenceExpression(std::string n, std::string s,
  std::vector<DataType *> dts, Node loc, Speci *sp) {
  kind = "DeclRef";
  name = n;
  scope = s;
  type = dts;
  setLocation(loc);
  spe = sp;
}

void DeclarationReferenceExpression::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name << "\"";
  if (spe != NULL) {
    spe->printSpecifier();
  }
  if (scope != "") {
    llvm::outs() << " :scope \"" << scope << "\"";
  }
  if ((int)label.size() != 0) {
    printLabel();
  }
  llvm::outs() << " :type [";
  for (int i = 0; i < (int)type.size(); i++) {
    type[i]->printType();
  }
  llvm::outs() << "]";
  PrintLocation();
  llvm::outs() << "}";
}

class StructReference : public Reference {
public:
  std::string op;
  Node *structs;
  Node *structMember;
  StructReference(std::string o, Node *str, Node *strMem, std::vector<DataType *> dts, Node loc);
  void printAST();
};

StructReference::StructReference(std::string o, Node *str, Node *strMem, std::vector<DataType *> dts, Node loc) {
  kind = "Struct";
  op = o;
  structs = str;
  structMember = strMem;
  type = dts;
  setLocation(loc);
}

void StructReference::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if ((int)label.size() != 0) {
    printLabel();
  }
  if (op != "") {
  llvm::outs() << " :op \"" << op <<"\"";
  }
  llvm::outs() << " :struct ";
  structs->printAST();
  llvm::outs() << " :struct-member ";
  structMember->printAST();
  PrintLocation();
  llvm::outs() << "}";
}

class Operator : public Expression {
public:
  std::vector<Node *> label;
  void setLabel(Node *l);
  void printLabel();
};

void Operator::setLabel(Node *l) {
  label.push_back(l);
}

void Operator::printLabel() {
  llvm::outs() << " :label [";
  for (int i = 0; i < (int)label.size(); i++) {
    label[i]->printAST();
  }
  llvm::outs() << "]";
}

class BinOp : public Operator {
public:
  std::string op;
  Node *left;
  Node *right;
  BinOp(std::string o, std::vector<DataType *> dts, Node *l, Node *r, Node loc);
  void printAST();
};

BinOp::BinOp(std::string o, std::vector<DataType *> dts, Node *l, Node *r, Node loc) {
  kind = "BinOp";
  op = o;
  type = dts;
  left = l;
  right = r;
  setLocation(loc);
}

void BinOp::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if ((int)label.size() != 0) {
    printLabel();
  }
  llvm::outs() << " :op \"" << op << "\" :type [";
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
  // sizeOf用
  DataType *argumentType;
  UnOp(std::string o, std::vector<DataType *> dts, Node *oper, Node loc);
  // sizeOf用
  UnOp(std::string o, DataType *argument, std::vector<DataType *> dts, Node *oper, Node loc);
  void printAST();
};

UnOp::UnOp(std::string o, std::vector<DataType *> dts, Node *oper, Node loc) {
  kind = "UnOp";
  op = o;
  type = dts;
  operand = oper;
  setLocation(loc);
}

UnOp::UnOp(std::string o, DataType *argument, std::vector<DataType *> dts, Node *oper, Node loc) {
  kind = "UnOp";
  op = o;
  argumentType = argument;
  type = dts;
  operand = oper;
  setLocation(loc);
}

void UnOp::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if ((int)label.size() != 0) {
    printLabel();
  }
  llvm::outs() << " :op \"" << op << "\" :type [";
  for (int i = 0; i < (int)type.size(); i++) {
    type[i]->printType();
  }
  llvm::outs() << "]";
  PrintLocation();
  if (operand != NULL) {
    llvm::outs() << "\n :operand ";
    operand->printAST();
  } else if (argumentType != NULL) {
    llvm::outs() << "\n :argument-type ";
    argumentType->printType();
  }
  llvm::outs() << "}";
}

class ConditionalOp : public Operator {
public:
  Node *condition;
  Node *then;
  Node *denial;
  ConditionalOp(std::vector<DataType *> dts, Node *cond, Node *t, Node *d, Node loc);
  void printAST();
};

ConditionalOp::ConditionalOp(std::vector<DataType *> dts, Node *cond, Node *t, Node *d, Node loc) {
  kind = "ConditionalOp";
  type = dts;
  condition = cond;
  then = t;
  denial = d;
  setLocation(loc);
}

void ConditionalOp::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if ((int)label.size() != 0) {
    printLabel();
  }
  llvm::outs() << " :type [";
  for (int i = 0; i < (int)type.size(); i++) {
    type[i]->printType();
  }
  llvm::outs() << "]";
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
  llvm::outs() << "{:kind \"No Literal\"}";
}

class IntLiteral : public Literal {
public:
  std::string value;
  IntLiteral(std::string v, std::vector<DataType *> dts, Node loc);
  void printAST();
};

IntLiteral::IntLiteral(std::string v, std::vector<DataType *> dts, Node loc) {
  kind = "IntegerLiteral";
  value = v;
  type = dts;
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
  std::string value;
  CharLiteral(std::string v, std::vector<DataType *> dts, Node loc);
  void printAST();
};

CharLiteral::CharLiteral(std::string v, std::vector<DataType *> dts, Node loc) {
  kind = "CharacterLiteral";
  value = v;
  type = dts;
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
  FloatLiteral(float v, std::vector<DataType *> dts, Node loc);
  void printAST();
};

FloatLiteral::FloatLiteral(float v, std::vector<DataType *> dts, Node loc) {
  kind = "FloatingLiteral";
  value = v;
  type = dts;
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

class StringsLiteral : public Literal {
public:
  std::string value;
  StringsLiteral(std::string v, std::vector<DataType *> dts, Node loc);
  void printAST();
};

StringsLiteral::StringsLiteral(std::string v, std::vector<DataType *> dts, Node loc) {
  kind = "StringLiteral";
  value = v;
  type = dts;
  setLocation(loc);
}

void StringsLiteral::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :value \"" << value << "\" :type [";
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
  ArrayReference(Node *arr, Node *i, DataType *tp, Node loc);
  void printAST();
};

ArrayReference::ArrayReference(Node *arr, Node *i, DataType *dt, Node loc) {
  kind = "ArrayRef";
  array = arr;
  index = i;
  type.push_back(dt);
  setLocation(loc);
}

void ArrayReference::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if ((int)label.size() != 0) {
    printLabel();
  }
  llvm::outs() << " :type ";
  type[0]->printType();
  PrintLocation();
  llvm::outs() << "\n :array ";
  array->printAST();
  llvm::outs() << "\n :index ";
  index->printAST();
  llvm::outs() <<"}";
}

class FunctionCall : public Expression {
public:
  Node *func;
  std::vector<Node *> label;
  std::vector<Node *> parm;
  FunctionCall(DataType *dt, Node *f, std::vector<Node *> p, Node loc);
  void setLabel(Node *l);
  void printAST();
};

FunctionCall::FunctionCall(DataType *dt, Node *f, std::vector<Node *> p, Node loc) {
  kind = "FuncCall";
  type.push_back(dt);
  func = f;
  parm = p;
  setLocation(loc);
}

void FunctionCall::setLabel(Node *l) {
  label.push_back(l);
}

void FunctionCall::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :type ";
  type[0]->printType();
  if ((int)label.size() != 0) {
    llvm::outs() << " :label [";
    for (int i = 0; i < (int)label.size(); i++) {
      label[i]->printAST();
    }
    llvm::outs() << "]";
  }
  llvm::outs() << "\n :func ";
  func->printAST();
  llvm::outs() << "\n :parm [";
  for (int i = 0; i < (int)parm.size(); i++) {
    parm[i]->printAST();
  }
  llvm::outs() << "] ";
  PrintLocation();
  llvm::outs() << "}\n";
}

class Declaration : public Node {
public:
  std::string name;
};

class FieldDeclaration : public Declaration {
public:
  std::string scope;
  DataType *type;
  FieldDeclaration(std::string n, DataType *dt, Node loc);
  void printAST();
};

FieldDeclaration::FieldDeclaration(std::string n, DataType *dt, Node loc) {
  kind = "FieldDecl";
  scope = "Member";
  name = n;
  type = dt;
  setLocation(loc);
}

void FieldDeclaration::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name 
               << "\" :scope \"" << scope << "\" :type ";
  type->printType();
  PrintLocation();
  llvm::outs() << "}\n";
}

class ObjectDeclaration : public Declaration {
public:
  DataType *type;
  std::string displayType;
};

class DeclarationOfVariables : public ObjectDeclaration {
public:
  std::string scope;
};

class ParameterDeclaration : public DeclarationOfVariables {
public:
  ParameterDeclaration(std::string n, std::string display, DataType *dt, Node loc);
  void printAST();
};

ParameterDeclaration::ParameterDeclaration(std::string n, std::string display, DataType *dt, Node loc) {
  kind = "ParmDecl";
  name = n;
  displayType = display;
  type = dt;
  setLocation(loc);
}

void ParameterDeclaration::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name << "\" :display-type \"" 
               << displayType << "\" :type ";
  type->printType();
  PrintLocation();
  llvm::outs() << "}";
}

class VariableDeclaration : public DeclarationOfVariables {
public:
  std::vector<Node *> init;
  Speci *spe;
  VariableDeclaration(std::string n, std::string s, std::string display, DataType *dt, std::vector<Node *>i, Node loc, Speci *sp);
  void printAST();
};

VariableDeclaration::VariableDeclaration(std::string n, std::string s, std::string display, DataType *dt, std::vector<Node *>i, Node loc, Speci *sp) {
  kind = "VarDecl";
  name = n;
  scope = s;
  displayType = display;
  type = dt;
  init = i;
  setLocation(loc);
  spe = sp;
}

void VariableDeclaration::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name << "\" :scope \"" << scope << "\"";
  spe->printSpecifier();
  llvm::outs() << " :display-type \"" << displayType << "\" :type ";
  type->printType();
  PrintLocation();
  if (init.size() != 0) {
    llvm::outs() << "\n :init ";
    for (int i = 0; i < (int)init.size(); i++) {
      init[i]->printAST();
    }
    llvm::outs() << "";
  }
  llvm::outs() << "}\n";
}

class FunctionDeclaration : public ObjectDeclaration {
public:
  std::vector<Node *> parm;
  std::vector<Node *> body;
  Speci *spe;
  FunctionDeclaration(std::string n, DataType *dt, std::vector<Node *> p, std::vector<Node *> b, Node loc, Speci *sp);
  void printAST();
};

FunctionDeclaration::FunctionDeclaration(std::string n, DataType *dt, std::vector<Node *> p, std::vector<Node *> b, Node loc, Speci *sp) {
  kind = "FuncDecl";
  name = n;
  type = dt;
  parm = p;
  body = b;
  setLocation(loc);
  spe = sp;
}

void FunctionDeclaration::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name << "\"";
  spe->printSpecifier();
  llvm::outs() << " :type ";
  type->printType();
  PrintLocation();
  llvm::outs() << "\n :parm [";
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

class TypeDeclaration : public Declaration {
public:
  std::vector<Node *> member;
};

class StructDeclaration : public TypeDeclaration {
public:
  StructDeclaration(std::string n, std::vector<Node *> m, Node loc);
  void printAST();
};

StructDeclaration::StructDeclaration(std::string n, std::vector<Node *> m, Node loc) {
  kind = "StructDecl";
  name = n;
  member = m;
  setLocation(loc);
}

void StructDeclaration::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name << "\"";
  PrintLocation();
  llvm::outs() << "\n :member [";
  for (int i = 0; i < (int)member.size(); i++) {
    member[i]->printAST();
  }
  llvm::outs() << "]}\n";
}

class UnionDeclaration : public TypeDeclaration {
public:
  UnionDeclaration(std::string n, std::vector<Node *> m, Node loc);
  void printAST();
};

UnionDeclaration::UnionDeclaration(std::string n, std::vector<Node *> m, Node loc) {
  kind = "UnionDecl";
  name = n;
  member = m;
  setLocation(loc);
}

void UnionDeclaration::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :name \"" << name << "\"";
  PrintLocation();
  llvm::outs() << "\n :member [";
  for (int i = 0; i < (int)member.size(); i++) {
    member[i]->printAST();
  }
  llvm::outs() << "]}\n";
}

class TypedefDeclaration : public TypeDeclaration {
public:
  DataType *typedefType;
  TypedefDeclaration(std::string n, DataType *type, Node loc);
  void printAST();
};

TypedefDeclaration::TypedefDeclaration(std::string n, DataType *type, Node loc) {
  kind = "TypedefDecl";
  name = n;
  typedefType = type;
  setLocation(loc);
}

void TypedefDeclaration::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\" :typedef-name \"" << name << "\" :typedef-type ";
  typedefType->printType();
  PrintLocation();
  llvm::outs() << "}\n";
}

class Statement : public Node {
public:
  std::vector<Node *> label;
  void setLabel(Node *l);
  void printLabel();
};

void Statement::setLabel(Node *l) {
  label.push_back(l);
}

void Statement::printLabel() {
  llvm::outs() << " :label [";
  for (int i = 0; i < (int)label.size(); i++) {
    label[i]->printAST();
  }
  llvm::outs() << "]";
}

class RepetitionStatement : public Statement {
public:
  Node *condition;
  std::vector<Node *> body;
};

class WhileStatement : public RepetitionStatement {
public:
  WhileStatement(Node *cond, std::vector<Node *> b, Node loc);
  void printAST();
};

WhileStatement::WhileStatement(Node *cond, std::vector<Node *> b, Node loc) {
  kind = "While";
  condition = cond;
  body = b;
  setLocation(loc);
}

void WhileStatement::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if ((int)label.size() != 0) {
    printLabel();
  }
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
  DoStatement(Node *cond, std::vector<Node *> b, Node loc);
  void printAST();
};

DoStatement::DoStatement(Node *cond, std::vector<Node *> b, Node loc) {
  kind = "Do";
  condition = cond;
  body = b;
  setLocation(loc);
}

void DoStatement::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if ((int)label.size() != 0) {
    printLabel();
  }
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
  ForStatement(Node *cond, std::vector<Node *> b, Node *ini, Node *up, Node loc);
  void printAST();
};

ForStatement::ForStatement(Node *cond, std::vector<Node *> b, Node *ini, Node *up, Node loc) {
  kind = "For";
  condition = cond;
  body = b;
  init = ini;
  update = up;
  setLocation(loc);
}

void ForStatement::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if ((int)label.size() != 0) {
    printLabel();
  }
  PrintLocation();
  llvm::outs() << "\n :init ";
  if (init != NULL) {
    init->printAST();
  } else {
    llvm::outs() << "[]";
  }
  llvm::outs() << "\n :condition ";
  if (condition != NULL) {
    condition->printAST();
  } else {
    llvm::outs() << "[]";
  }
  llvm::outs() << "\n :update ";
  if (update != NULL) {
    update->printAST();
  } else {
    llvm::outs() << "[]";
  }
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
  IfStatement(Node *cond, std::vector<Node *> t, std::vector<Node *> d, Node loc);
  void printAST();
};

IfStatement::IfStatement(Node *cond, std::vector<Node *> t, std::vector<Node *> d, Node loc) {
  kind = "If";
  condition = cond;
  then = t;
  denial = d;
  setLocation(loc);
}

void IfStatement::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if ((int)label.size() != 0) {
    printLabel();
  }
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
    llvm::outs() << "]";
  }
  llvm::outs() << "}\n";
}

class SwitchStatement : public BranchStatement {
public:
  std::vector<Node *> body;
  SwitchStatement(Node *cond, std::vector<Node *> b, Node loc);
  void printAST();
};

SwitchStatement::SwitchStatement(Node *cond, std::vector<Node *> b, Node loc) {
  kind = "Switch";
  condition = cond;
  body = b;
  setLocation(loc);
}

void SwitchStatement::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if ((int)label.size() != 0) {
    printLabel();
  }
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
  LabelStatement(std::string n, Node loc);
  void printAST();
};

LabelStatement::LabelStatement(std::string n, Node loc) {
  kind = "Label";
  name = n;
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
  llvm::outs() << "\n :value [";
  if (value != NULL) {
    value->printAST();
  }
  llvm::outs() << "]}\n";
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
  GotoStatement(std::string j, Node loc);
  void printAST();
};

GotoStatement::GotoStatement(std::string j, Node loc) {
  kind = "Goto";
  jump = j;
  setLocation(loc);
}

void GotoStatement::printAST() {
  llvm::outs() << "{:kind \"" << kind << "\"";
  if ((int)label.size() != 0) {
    printLabel();
  }
  llvm::outs() << " :goto \"" << jump << "\" ";
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
  llvm::outs() << "{:kind \"" << kind << "\"";
  if ((int)label.size() != 0) {
    printLabel();
  }
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
  llvm::outs() << "{:kind \"" << kind << "\"";
  if ((int)label.size() != 0) {
    printLabel();
  }
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
  if ((int)label.size() != 0) {
    printLabel();
  }
  PrintLocation();
  llvm::outs() << "\n :value [";
  if (value != NULL) {
    value->printAST();
  }
  llvm::outs() << "]}";
}

class MyAstVisitor : public RecursiveASTVisitor<MyAstVisitor> {
private:
  // 完成した木を保存するvector
  std::vector<Node *> prog;
  // castの遷移を保存するvector
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
	RecursiveASTVisitor::TraverseDecl(decl);
	break;
      case Decl::Function: 
        //llvm::outs() << "decl:2";
	RecursiveASTVisitor::TraverseDecl(decl);
	break;
      case Decl::ParmVar: 
        //llvm::outs() << "decl:3";
	RecursiveASTVisitor::TraverseDecl(decl);
	break;
      case Decl::TranslationUnit: 
        //llvm::outs() << "decl:4";
	skipcount = 0;
	RecursiveASTVisitor::TraverseDecl(decl);
	break;
      case Decl::Typedef: 
        //llvm::outs() << "decl:5";
	RecursiveASTVisitor::TraverseDecl(decl);
	break;
      case Decl::Record: 
        //llvm::outs() << "decl:6";
	RecursiveASTVisitor::TraverseDecl(decl);
	break;
      case Decl::Var: 
        //llvm::outs() << "decl:7";
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
	//llvm::outs() << " ?Decl? " << decl->getDeclKindName();
        Node *np = new Node();
        prog.push_back(np);
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

    FieldDeclaration *FD = new FieldDeclaration(name, type, t);
    Node *np = FD;
    prog.push_back(np);

    return false;
  }
  
  // FunctionDecl
  bool VisitFunctionDecl(FunctionDecl *Decl) {
    std::string name = Decl->getQualifiedNameAsString();
    DataType *type = PrintTypeInfo(Decl->getType());
    Speci *sp = checkSpecifier(Decl->getStorageClass());
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

    FunctionDeclaration *FD = new FunctionDeclaration(name, type, parm, body, t, sp);
    Node *np = FD;
    prog.push_back(np);
    return false;
  }

  // ParmVarDecl
  bool VisitParmVarDecl(ParmVarDecl *Decl) {
    QualType vartype = Decl->getType();
    std::string name = Decl->getNameAsString();
    DataType *type = PrintTypeInfo(vartype);
    std::string disp = PrintDisplayType(vartype);
    Node t = PrintSourceRange(Decl->getSourceRange());

    ParameterDeclaration *PD = new ParameterDeclaration(name, disp, type, t);
    Node *np = PD;
    prog.push_back(np);
    return true;
  }

  // TypedefDecl
  bool VisitTypedefDecl(TypedefDecl *Typedef) {
    //llvm::outs() << "VisitTypeDefType()";
    std::string name = Typedef->getNameAsString();
    DataType *typedefType = PrintTypeInfo(Typedef->getUnderlyingType());
    Node t = PrintSourceRange(Typedef->getSourceRange());

    TypedefDeclaration *TD = new TypedefDeclaration(name, typedefType, t);
    Node *np = TD;
    prog.push_back(np);
    return false;
  }

  // RecordDecl (Structure?)
  bool VisitRecordDecl(RecordDecl *record) {
    if (record->isStruct()) {
      std::string name = (std::string)record->getName();
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

      StructDeclaration *SD = new StructDeclaration(name, member, t);
      Node *np = SD;
      prog.push_back(np);
    } else if (record->isUnion()) {
      std::string name = (std::string)record->getName();
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

      UnionDeclaration *UD = new UnionDeclaration(name, member, t);
      Node *np = UD;
      prog.push_back(np);

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
    Speci *sp = checkSpecifier(Decl->getStorageClass());
    std::string displayType = PrintDisplayType(vartype);
    std::vector<Node *> init;
    if (Decl->hasInit()) {
      int i = prog.size();
      TraverseStmt(Decl->getInit());
      while (i != (int)prog.size()) {
        init.push_back(prog[i]);
        prog.erase(prog.begin() + i);
      }
    } 
    Node t = PrintSourceRange(Decl->getSourceRange());

    VariableDeclaration *VD = new VariableDeclaration(name, scope, displayType, type, init, t, sp);
    Node *np = VD;
    prog.push_back(np);

    return false;
  }
  
  // 指定子:specifire::=(register, static, extern)
  Speci *checkSpecifier(StorageClass SC) {
    bool Extern = false;
    bool Static = false;
    bool Auto = false;
    bool Register = false;
    switch (SC) {
      case SC_Extern:
        Extern = true;
        break;
      case SC_Static:
        Static = true;
        break;
      case SC_Auto:
        Auto = true;
        break;
      case SC_Register:
        Register = true;
        break;
      case SC_None:
      case SC_PrivateExtern:
      case SC_OpenCLWorkGroupLocal:
      default:
        break;
    }
    Speci *sp = new Speci(Extern, Static, Auto, Register);

    return sp;
  }
  
  // typeを文字列として出力
  std::string PrintDisplayType(QualType typeInfo) {
    return typeInfo.getAsString();
  }
 
  RenameType *PrintTypedefTypeInfo(QualType typeInfo) {
    TypedefNameDecl *TDtype = dyn_cast<TypedefType>(typeInfo)->getDecl();
    std::string typeName = (std::string)TDtype->getName();
    DataType *typedefType = PrintTypeInfo(TDtype->getUnderlyingType());

    RenameType *TDF = new RenameType(typeName, typedefType);
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
	//llvm::outs() << "\n \""<< typeInfo.getAsString() << "\"は, 初出です."
	//	     << " astprint.cppのPrintTypeInfoにcaseを追加して下さい.";
        t = new DataType();
        t->setTypedefString(typeInfo.getAsString());
	break;
      }
    }
    
    return t;
  }

  FuncType *PrintFunctionTypeInfo(QualType typeInfo) {
    std::vector<DataType *> parmType;
    DataType *retType;
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
    } else {
      retType = new VoidType();
    }

    FuncType *t = new FuncType(parmType, retType);
    return t;
  }

  DataType *PrintArrayTypeInfo(QualType typeInfo) {
    QualType elmtype;
    DataType *t;
    if (dyn_cast<ArrayType>(typeInfo)) {
      std::string ConstArraySize = "";
      Node *VarArraySize = NULL;
      elmtype = dyn_cast<ArrayType>(typeInfo)->getElementType();
      if (dyn_cast<ConstantArrayType>(typeInfo)) {
        ConstArraySize = dyn_cast<ConstantArrayType>(typeInfo)->getSize().toString(10, true);
      }
      if (dyn_cast<VariableArrayType>(typeInfo)) {
        Expr *vaexpr = dyn_cast<VariableArrayType>(typeInfo)->getSizeExpr();
        int i = prog.size();
        TraverseStmt(vaexpr);
        VarArraySize = prog[i];
        prog.pop_back();
      } 
      DataType *type = PrintTypeInfo(elmtype);

      t = new ArrayDataType(ConstArraySize, VarArraySize, type);
    } else if (dyn_cast<ParenType>(typeInfo)) {
      elmtype = dyn_cast<ParenType>(typeInfo)->getInnerType();
      t = PrintTypeInfo(elmtype);
    }
    return t;
  }

  PointeeType *PrintPointerTypeInfo(QualType typeInfo) {
    DataType *pointee;
    if (dyn_cast<PointerType>(typeInfo)) {
      QualType elmtype = dyn_cast<PointerType>(typeInfo)->getPointeeType();
      pointee = PrintTypeInfo(elmtype);
    } else {
      pointee = new DataType();
    }

    PointeeType *t = new PointeeType(pointee);
    return t;
  }

  StructureType *PrintStructureTypeInfo(QualType typeInfo) {
    std::string name = "";
    std::vector<Node *> member;
    if (dyn_cast<ElaboratedType>(typeInfo)) {
      QualType etype = dyn_cast<ElaboratedType>(typeInfo)->getNamedType();
      if (dyn_cast<RecordType>(etype)) {
        RecordDecl *rdecl = dyn_cast<RecordType>(etype)->getDecl();
        name = rdecl->getName();
        if (name == "") {
          if (!(rdecl->field_empty())) {
            RecordDecl::field_iterator itr = rdecl->field_begin();
            int i = prog.size();
            while (itr != rdecl->field_end()) {
              TraverseDecl(itr->getCanonicalDecl());
              member.push_back(prog[i]);
              prog.erase(prog.begin() + i);
              itr++;
            }
          }
        }
      } else {
        PrintTypeInfo(etype);
      }
    }

    StructureType *t = new StructureType(name, member);

    return t;
  }

  UnionType *PrintUnionTypeInfo(QualType typeInfo) {
    std::string name = "";
    std::vector<Node *> member;
    if (dyn_cast<ElaboratedType>(typeInfo)) {
        QualType etype = dyn_cast<ElaboratedType>(typeInfo)->getNamedType();
      if (dyn_cast<RecordType>(etype)) {
        RecordDecl *rdecl = dyn_cast<RecordType>(etype)->getDecl();
        name = rdecl->getName();
        if (name == "" && !(rdecl->field_empty())) {
          RecordDecl::field_iterator itr = rdecl->field_begin();
          int i = prog.size();
          while (itr != rdecl->field_end()) {
            TraverseDecl(itr->getCanonicalDecl());
            member.push_back(prog[i]);
            prog.erase(prog.begin() + i);
            itr++;
          }
        }
      } else {
        PrintTypeInfo(etype);
      }
    }

    UnionType *t = new UnionType(name, member);
    return t;
  }

  // :typeの情報を出力
  DataType *PrintTypeInfo(QualType typeInfo) {
    DataType *t;
    if (dyn_cast<TypedefType>(typeInfo)) {
      return PrintTypedefTypeInfo(typeInfo);
    } else if (dyn_cast<TypedefType>(typeInfo)) {
      return PrintTypedefTypeInfo(typeInfo);
    } else if (dyn_cast<BuiltinType>(typeInfo)) {
      return PrintBuiltinTypeInfo(typeInfo);
    } else if (typeInfo->isFunctionType()) {
      return PrintFunctionTypeInfo(typeInfo);
    } else if (typeInfo->isArrayType()) {
      return PrintArrayTypeInfo(typeInfo);
    } else if (typeInfo->isPointerType()) {
      return PrintPointerTypeInfo(typeInfo);
    } else if (typeInfo->isStructureType()) {
      return PrintStructureTypeInfo(typeInfo);
    } else if (typeInfo->isUnionType()) {
      return PrintUnionTypeInfo(typeInfo);
    } else {
      std::string typeString = typeInfo.getAsString();
      t = new DataType();
      t->setTypedefString(typeString);
    }

    return t;
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
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::DoStmtClass: 
        //llvm::outs() << "stmt:7";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::ForStmtClass: 
        //llvm::outs() << "stmt:8";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::GotoStmtClass: 
        //llvm::outs() << "stmt:9";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::IfStmtClass: 
        //llvm::outs() << "stmt:10";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::LabelStmtClass: 
        //llvm::outs() << "stmt:11";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::SwitchStmtClass: 
        //llvm::outs() << "stmt:12";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::WhileStmtClass: 
        //llvm::outs() << "stmt:13";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::ReturnStmtClass: 
        //llvm::outs() << "stmt:14";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::ArraySubscriptExprClass: 
        //llvm::outs() << "stmt:15";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::CallExprClass: 
        //llvm::outs() << "stmt:16";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::CStyleCastExprClass: 
        //llvm::outs() << "stmt:17";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::DeclRefExprClass: 
        //llvm::outs() << "stmt:18";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::ImplicitCastExprClass: 
        //llvm::outs() << "stmt:19";
	if (dyn_cast<ImplicitCastExpr>(stmt)->getSubExpr()->getStmtClass() == 68) {
	  llvm::outs() << "{:kind \"NULL\" :value \"Null\"}";
	  break;
	}
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::InitListExprClass: 
        //llvm::outs() << "stmt:20";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::MemberExprClass: 
        //llvm::outs() << "stmt:21";
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
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::ConditionalOperatorClass: 
        //llvm::outs() << "stmt:24";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::CompoundAssignOperatorClass: 
        //llvm::outs() << "stmt:25";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::UnaryOperatorClass: 
        //llvm::outs() << "stmt:26";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::CharacterLiteralClass: 
        //llvm::outs() << "stmt:27";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::FloatingLiteralClass: 
        //llvm::outs() << "stmt:28";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::IntegerLiteralClass: 
        //llvm::outs() << "stmt:29";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
      case Stmt::StringLiteralClass: 
        //llvm::outs() << "stmt:30";
	RecursiveASTVisitor::TraverseStmt(stmt);
	break;
	//以下C++に関係
      case Stmt::CXXNewExprClass: 
        //llvm::outs() << "stmt:31";
      case Stmt::CXXUnresolvedConstructExprClass: 
        //llvm::outs() << "stmt:32";
      case Stmt::ParenListExprClass: 
        //llvm::outs() << "stmt:33";
	RecursiveASTVisitor::TraverseStmt(stmt);
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
	//llvm::outs() << "?Stmt? " << stmt->getStmtClassName();	
        Node *np = new Node();
        prog.push_back(np);
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
    if (Case->getLHS()) {
      TraverseStmt(Case->getLHS());
      value = prog[i];
      prog.pop_back();
    } else {
      value = new Node();
    }
    Node t = PrintSourceRange(Case->getSourceRange());

    CaseStatement *CS = new CaseStatement(value, t);
    Node *np = CS;
    prog.push_back(np);
    skipcount = 1;

    return true;
  }
  
  // CompoundStmt
  bool VisitCompoundStmt(CompoundStmt *compound) {
    return true;
  }

  // ContinueStmt
  bool VisitContinueStmt(ContinueStmt *Continue) {
    Node t = PrintSourceRange(Continue->getSourceRange());

    ContinueStatement *CS = new ContinueStatement(t);
    Node *np = CS;
    prog.push_back(np);
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
    } else {
      init = NULL;
    }
    i = prog.size();
    Node *condition;
    if (For->getCond() != NULL) {
      TraverseStmt(For->getCond());
      condition = prog[i];
      prog.pop_back();
    } else {
      condition = NULL;
    }
    i = prog.size();
    Node *update;
    if (For->getInc() != NULL) {
      TraverseStmt(For->getInc());
      update = prog[i];
      prog.pop_back();
    } else {
      update = NULL;
    }
    i = prog.size();
    TraverseStmt(For->getBody());
    int j = prog.size();
    std::vector<Node *> body;
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
    std::string name = label->getName();
    Node t = PrintSourceRange(Goto->getSourceRange());

    GotoStatement *GS = new GotoStatement(name, t);
    Node *np = GS;
    prog.push_back(np);
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
    std::string name = Label->getName();
    Node t = PrintSourceRange(Label->getSourceRange());

    LabelStatement *LS = new LabelStatement(name, t);
    Node *np = LS;
    prog.push_back(np);
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
    } else {
      value = NULL;
    }
    Node t = PrintSourceRange(Ret->getSourceRange());

    ReturnStatement *RS = new ReturnStatement(value, t);
    Node *tp = RS;
    prog.push_back(tp);

    return false;
  }

  // ArraySubscriptExpr
  bool VisitArraySubscriptExpr(ArraySubscriptExpr *arrsub) {
    int i = prog.size();
    TraverseStmt(arrsub->getLHS());
    Node *array = prog[i];
    prog.pop_back();
    i = prog.size();
    TraverseStmt(arrsub->getRHS());
    Node *index = prog[i];
    prog.pop_back();
    DataType *type = PrintTypeInfo(arrsub->getType());
    Node t = PrintSourceRange(arrsub->getSourceRange());

    ArrayReference *AR = new ArrayReference(array, index, type, t);
    Node *np = AR;
    prog.push_back(np);
    return false;
  }

  // CallExpr
  bool VisitCallExpr(CallExpr *call) {
    DataType *type = PrintTypeInfo(call->getType());
    int i = prog.size();
    TraverseStmt(call->getCallee());
    Node *func = prog[i];
    prog.pop_back();
    std::vector<Node *> parm;
    if (call->getNumArgs() != 0) {
      i = prog.size();
      int argnum = 0;
      while (argnum != (int)call->getNumArgs()) {
        TraverseStmt(call->getArg(argnum));
        argnum++;
      }
      while (i != (int)prog.size()) {
        parm.push_back(prog[i]);
        prog.erase(prog.begin() + i);
      }
    }
    Node t = PrintSourceRange(call->getSourceRange());

    FunctionCall *FC = new FunctionCall(type, func, parm, t);
    Node *np = FC;
    prog.push_back(np);

    return false;
  }

  // CStyleCastExpr
  bool VisitCStyleCastExpr(CStyleCastExpr *cscast) {
    if (cscast->getCastKind() != 3) {
      QualType casttype = cscast->getType();
      castType.push_back(PrintTypeInfo(casttype)); 
    }
    TraverseStmt(cscast->getSubExpr());
    return false;
  }

  // DeclRefExpr(DRE)
  bool VisitDeclRefExpr(DeclRefExpr *Declref) {
    ValueDecl *valuedecl = Declref->getDecl();
    VarDecl *vardecl = dyn_cast<VarDecl>(valuedecl);
    FunctionDecl *funcdecl = dyn_cast<FunctionDecl>(valuedecl);
    std::string scope = "";
    DeclarationReferenceExpression *DRE;
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
      Speci *sp = checkSpecifier(vardecl->getStorageClass());
      Node t = PrintSourceRange(Declref->getSourceRange());

      DRE = new DeclarationReferenceExpression(name, scope, type, t, sp);
    } else if (funcdecl) {
      QualType funcType = funcdecl->getType();
      std::string name = Declref->getNameInfo().getAsString();
      Speci *sp = checkSpecifier(funcdecl->getStorageClass());
      std::vector<DataType *>type;
      type.push_back(PrintTypeInfo(funcType));
      if (castType.size() != 0) {
        while (castType.size() != 0) {
          type.push_back(castType[0]);
          castType.erase(castType.begin());
        }
      }
      Node t = PrintSourceRange(Declref->getSourceRange());

      DRE = new DeclarationReferenceExpression(name, scope, type, t, sp);
    } else {
      QualType Declreftype = Declref->getType();
      std::string name = Declref->getNameInfo().getAsString();
      std::string scope = "";
      std::vector<DataType *> type;
      type.push_back(PrintTypeInfo(Declreftype));
      if (castType.size() != 0) {
        for (int i = 0; i < (int)castType.size(); i++) {
          type.push_back(castType[0]);
          castType.erase(castType.begin());
        }
      }
      Node t = PrintSourceRange(Declref->getSourceRange());

      DRE = new DeclarationReferenceExpression(name, scope, type, t, NULL);
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

      StructReference *SR = new StructReference("", structs, structMember, type, t);
      Node *np = SR;
      prog.push_back(np);
      return false;
    }
    if(mem->isArrow()) {
      std::string op = "->";
      std::vector<DataType *> type;
      type.push_back(PrintTypeInfo(mem->getType()));
      if (castType.size() != 0) {
        while (castType.size() != 0) {
          type.push_back(castType[0]);
          castType.erase(castType.begin());
        }
      }
      Node *structs, *structMember;
      int i = prog.size();
      getlhsArrow(base);
      structs = prog[i];
      prog.pop_back();
      i = prog.size();
      getrhsArrow(vdecl);
      structMember = prog[i];
      prog.pop_back();
      Node t = PrintSourceRange(mem->getSourceRange());

      StructReference *SR = new StructReference(op, structs, structMember, type, t);
      Node *np = SR;
      prog.push_back(np);
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
	TraverseStmt(def);
      }
    }
    return true;
  }
  bool getrhsArrow(ValueDecl *vdecl) {
    QualType vartype = vdecl->getType();
    std::string name = vdecl->getName();
    std::string scope = "Member";
    std::vector<DataType *> type;
    type.push_back(PrintTypeInfo(vartype));
    if (castType.size() != 0) {
      while (castType.size() != 0) {
        type.push_back(castType[0]);
        castType.erase(castType.begin());
      }
    }
    Node t = PrintSourceRange(vdecl->getSourceRange());
    Speci *sp = new Speci(false, false, false, false);

    DeclarationReferenceExpression *DRE = new DeclarationReferenceExpression (name, scope, type, t, sp);
    Node *np = DRE;
    prog.push_back(np);
    return true;
  }
  
  // UnaryExprOrTypeTraitExpr
  bool VisitUnaryExprOrTypeTraitExpr(UnaryExprOrTypeTraitExpr *expr) {
    switch (expr->getKind()) {
    case UETT_SizeOf:
    {
      std::string op = "sizeOf";
      std::vector<DataType *> type;
      type.push_back(PrintTypeInfo(expr->getType()));
      if (castType.size() != 0) {
        while (castType.size() != 0) {
          type.push_back(castType[0]);
          castType.erase(castType.begin());
        }
      }
      int i = prog.size();
      Node *operand = NULL;
      DataType *argumentType = NULL;
      if (expr->isArgumentType()) {
        argumentType = PrintTypeInfo(expr->getArgumentType());
      } else {
        TraverseStmt(expr->getArgumentExpr());
        operand = prog[i];
        prog.erase(prog.begin() + i);
      }
      Node t = PrintSourceRange(expr->getSourceRange());

      UnOp *UO = new UnOp(op, argumentType, type, operand, t);
      Node *np = UO;
      prog.push_back(np);
      return false;
    }
    default:
    {
      break;
    }
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
    std::string op = Binop->getOpcodeStr();
    std::vector<DataType *> type;
    type.push_back(PrintTypeInfo(Binop->getType()));
    if (castType.size() != 0) {
      for (int i = 0; i < (int)castType.size(); i++) {
        type.push_back(castType[0]);
        castType.erase(castType.begin());
      }
    }
    int i = prog.size();
    TraverseStmt(Binop->getLHS());
    Node *left = prog[i];
    prog.pop_back();
    Node *right;
    i = prog.size();
    TraverseStmt(Binop->getRHS());
    right = prog[i];
    prog.pop_back();
    Node t = PrintSourceRange(Binop->getSourceRange());

    BinOp *BO = new BinOp(op, type, left, right, t);
    Node *np = BO;
    prog.push_back(np);

    return false;
  }
  
  // ConditionalOperator
  bool VisitConditionalOperator(ConditionalOperator *condop) {
    std::vector<DataType *> type;
    type.push_back(PrintTypeInfo(condop->getType()));
    if (castType.size() != 0) {
      while (castType.size() != 0) {
        type.push_back(castType[0]);
        castType.erase(castType.begin());
      }
    }
    int i = prog.size();
    TraverseStmt(condop->getCond());
    Node *cond = prog[i];
    prog.pop_back();
    i = prog.size();
    TraverseStmt(condop->getTrueExpr());
    Node *then = prog[i];
    prog.pop_back();
    i = prog.size();
    TraverseStmt(condop->getFalseExpr());
    Node *dinal = prog[i];
    prog.pop_back();
    Node t = PrintSourceRange(condop->getSourceRange());

    ConditionalOp *CO = new ConditionalOp(type, cond, then, dinal, t);
    Node *np = CO;
    prog.push_back(np);

    return false;
    
  }
    
  // IntegerLiteral
  bool VisitIntegerLiteral(IntegerLiteral *Int) {
    QualType vartype = Int->getType();
    std::string value = Int->getValue().toString(10, true);
    std::vector<DataType *> type;
    type.push_back(PrintTypeInfo(vartype));
    if (castType.size() != 0) {
      for (int i = 0; i < (int)castType.size(); i++) {
        type.push_back(castType[0]);
        castType.erase(castType.begin());
      }
    }
    Node t = PrintSourceRange(Int->getSourceRange());

    IntLiteral *IL = new IntLiteral(value, type, t);
    Node *np = IL;
    prog.push_back(np);

    return true;
  }

  // FloatingLiteral
  bool VisitFloatingLiteral(FloatingLiteral *Float) {
    QualType vartype = Float->getType();
    float value = Float->getValueAsApproximateDouble();
    std::vector<DataType *> type;
    type.push_back(PrintTypeInfo(vartype));
    if (castType.size() != 0) {
      for (int i = 0; i < (int)castType.size(); i++) {
        type.push_back(castType[0]);
        castType.erase(castType.begin());
      }
    }
    Node t = PrintSourceRange(Float->getSourceRange());

    FloatLiteral *FL = new FloatLiteral(value, type, t);
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
    QualType vartype = Char->getType();
    std::string value = EscapeChar(Char->getValue());
    std::vector<DataType *> type;
    type.push_back(PrintTypeInfo(vartype));
    if (castType.size() != 0) {
      for (int i = 0; i < (int)castType.size(); i++) {
        type.push_back(castType[0]);
        castType.erase(castType.begin());
      }
    }
    Node t = PrintSourceRange(Char->getSourceRange());

    CharLiteral *CL = new CharLiteral(value, type, t);
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
    std::string value = EscapeString(String->getString().str());
    std::vector<DataType *> type;
    type.push_back(PrintTypeInfo(vartype));
    if (castType.size() != 0) {
      while (castType.size() != 0) {
        type.push_back(castType[0]);
        castType.erase(castType.begin());
      }
    }
    Node t = PrintSourceRange(String->getSourceRange());

    StringsLiteral *SL = new StringsLiteral(value, type, t);
    Node *np = SL;
    prog.push_back(np);
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

  // ASTの出力
  void printAST() {
    for (int i = 0; i != (int)prog.size(); i++) {
      prog[i]->printAST();
    }
  }

  // includeファイルの削除(オプション)
  void clearIncludeFile(std::string analysisFile) {
    int i = 0;
    while (i != (int)prog.size()) {
      if (prog[i]->getBeginFile() != analysisFile) {
        prog.erase(prog.begin() + i);
      } else {
        i++;
      }
    }
  }

private:
  ASTContext *Context;
  std::string source_file;
  int skipcount; // skip processing a node while this count is positive
};


class MyAstConsumer : public clang::ASTConsumer {
public:
  explicit MyAstConsumer(ASTContext *Context, llvm::StringRef InFile) : Visitor(Context, InFile) {
    analysisFile = InFile;
}
  virtual void HandleTranslationUnit(clang::ASTContext &Context) {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    //llvm::outs() << "\n----------------------------------------------------------------------\n";
    if (dIncFile) {
      Visitor.clearIncludeFile(analysisFile);
    }
    llvm::outs() << "\n[";
    Visitor.printAST();
    llvm::outs() << "] \n\n";
  }

  static void setDIncFile(bool opt) {
    dIncFile = opt;
  }

private:
  MyAstVisitor Visitor;
  llvm::StringRef analysisFile;
  static bool dIncFile;
};

bool MyAstConsumer::dIncFile;

class MyAnalysisAction : public clang::ASTFrontendAction {
public:
  virtual clang::ASTConsumer *CreateASTConsumer(clang::CompilerInstance &Compiler, llvm::StringRef InFile)
  {
    return new MyAstConsumer(&Compiler.getASTContext(), InFile);
  }
};

static OwningPtr<OptTable> Options(createDriverOptTable());

static cl::opt<bool> Dincfile("d-incfile", cl::desc("Delete the include file"));
static cl::opt<bool> Clabel("change-label", cl::desc("Change label AST"));

int main(int argc, const char *argv[]) {
  CommonOptionsParser OptionsParser(argc, argv);
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  if (Dincfile) {
    MyAstConsumer::setDIncFile(Dincfile);
  } else if (Clabel) {
  }

  return Tool.run(newFrontendActionFactory<MyAnalysisAction>());
}
