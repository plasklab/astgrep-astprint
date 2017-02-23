# プログラム
## プログラム全体のASTの出力
```
[] of <Declaration>
```
- プログラム全体は上記のように出力する  

## ノード
```
<Node> ::= 
    {:kind String
     :loc-begin [String Int Int]
     :loc-end [String Int Int]
    }
```  
- kindのStringはノードの種類を出力する
- loc-begin，loc-endのStringはファイル名を，Intには行番号，列番号の順に出力する  

# 宣言
```
<Declaration> ::= extends <Node>
    {:name String}
```  
- nameのStringは，宣言によって指定されるオブジェクトの名前を出力する  

## メンバー変数宣言
```
<FieldDeclaration> ::= extends <Declaration>
    {:kind "FieldDecl"
     :scope "Member"
     :type <Type>
    }
```  
- typeはメンバー変数の型を出力する

## オブジェクトの宣言
```
<ObjectDeclaration> ::= extends <Declaration>
    {:type <Type>
     :display-type String
    }
```  
- typeは，オブジェクトの型を出力する
- display-typeのStringは，プログラムで記述されているオブジェクトの型を出力する　　

## 関数宣言
```
<FunctionDeclaration> ::= extends <ObjectDeclaration>
    {:kind "FuncDecl"
     :parm [] of <ParameterDeclaration>
     :body [] of <Declaration> | <Expression> | <Statement>
     opt:static "true"
     opt:auto "true"
     opt:extern "true"
     opt:register "true"
    }
```  
- parmは，関数宣言の際の引数を出力する
- bodyは，関数宣言の際の処理を出力する
- static，auto，extern，registerは，関数宣言の際に定義されていれば出力する

## 変数の宣言
```
<DeclarationOfVariables> ::= extends <ObjectDeclaration>
    {:scope "local" | "golbal"}
```    

## 引数宣言
```
<ParameterDeclaration> ::= extends <DeclarationOfVariables>
    {:kind "ParmDecl"}
```  

## 変数宣言
```
<VariableDeclaration> ::= extends <DeclarationOfVariables>
    {:kind "VarDecl"
     opt:init <Expression>
     opt:static "true"
     opt:auto "true"
     opt:extern "true"
     opt:register "true"

    }
```  
- initは初期値が代入された場合出力する  
- static，auto，extern，registerは，関数宣言の際に定義された場合出力する

## 型宣言
```
<TypeDeclaration> ::= extends <Declaration>
    {:member [] of <FieldDeclaration>}
```  
- memberは宣言したメンバー変数を出力する

## 構造体宣言
```
<StructDeclaration> ::= extends <TypeDeclaration>
    {:kind "StructDecl"}
```  

## 共用体宣言    
```
<UnionDeclaration> ::= extends <TypeDeclaration>
    {:kind "UnionDecl"}
```  

# 式
```
<Expression> ::= extends <Node>
    {:type [] of <Type>}
```  
- :typeは式で扱う型を出力する

## 関数呼び出し
```
<FunctionCall> ::= extends <Expression>
    {:kind "FuncCall
     :label [] of <LabelStatement> | <CaseStatement> | <DefaultStatement>
     :func <DeclarationReferenceExpression>
     :parm [] of <DeclarationReferenceExpression>
    }
```  
- funcは，関数呼び出しを行った関数の情報を出力する
- parmは，関数呼び出しを行った際の引数を出力する

## 参照
```
<Reference> ::= extends <Expression>
    {:name String
     :label [] of <LabelStatement> | <CaseStatement> | <DefaultStatement>
    }
```
- nameのStringは参照するオブジェクトの名前を出力する

## メンバ変数の参照
```
<StructReference> ::= extends <Reference>
    {:kind "Struct"
     opt:op "->"
     :struct <DeclarationReferenceExpression>
     :struct-member <DeclarationReferenceExpression>
    }
```  
- メンバ変数を参照する際に，アロー演算子を用いるとopを出力する
- structは，構造体，共用体変数の情報を出力する
- struct-memberは，参照するメンバー変数の情報を出力する

## 配列参照
```
<ArrayReference> ::= extends <Reference>
    {:kind "ArrayRef"
     :array <DeclarationReferenceExpression>
     :index <Expration>
    }
```  
- arrayは，配列の情報を出力する
- indexは，配列参照する際のインデックスを出力する

## 変数，関数の参照
```
<DeclarationReferenceExpression> ::= exntends <Reference>
    {:kind "DeclRef"
     opt:scope "global" | "local"
     opt:static "true"
     opt:auto "true"
     opt:extern "true"
     opt:register "true"
    }
```  
- scopeが付くのは変数を参照する場合出力する
- scopeは変数のスコープを出力する
- static，auto，extern，registerは，関数宣言の際に定義された場合出力する

## 演算
```
<Operator> ::= extends <Expression>
    {:label [] of <LabelStatement> | <CaseStatement> | <DefaultStatement>}
```  

## 三項演算式
```
<ConditionalOperator> ::= extends <Operator>
    {:kind "ConditionalOp"
     :condition <Expression>
     :then <Expression>
     :else <Expression>
    }
```  
- conditionは，三項演算式の条件式を出力する
- thenは，三項演算式のthen式を出力する
- elseは，三項演算式のelse式を出力する

## 二項演算式
```
<BinaryOperator> ::= extends <Operator>
    {:kind "BinOp"
     :op string
     :left <Expression>
     :right <Expression>
    }
```  
- opは，二項演算子を出力する
- leftは，左辺の項を出力する
- rightは，右辺の項を出力する

## 単項演算式
```
<UnaryOperator> ::= extends <Operator>
    {:kind "UnOp"
     :op string
     opt:operand <Expression>
     opt:argumentType <DataType>
    }
```  
- opは，単項演算子を出力する
- operandは，演算する項を出力する
- sizeOf演算子のときargumentTypeを出力する場合がある
- sizeOf演算子のときargumentTypeを出力する場合operandは出力されない

## リテラル
```
<Literal> ::= extends <Expression>
    {:value value}
```  
- valueは，定義された値を出力する

## 整数リテラル
```
<IntegerLiteral> ::= extends <Literal>
    {:kind "IntegerLiteral"
     :value Int
    }
```  
-valueは，Int型で出力する

## 文字リテラル
```
<CharacterLiteral> ::= extends <Literal>
    {:kind "CharacterLiteral"
     :value Char
    }
```  
- valueは，char型で出力する
  
## 浮動小数点リテラル
```
<FloatingLiteral> ::= extends <Literal>
    {:kind "FloatingLiteral"
     :value Float
    }
```  
- valueは，Float型で出力する

## 文字列リテラル
```
<StringLiteral> ::= extends <Literal>
  {:kind "StringLiteral"
   :value String
  }
```
- valueは，String型で出力する

# 状態文
```
<Statement> ::= extends <Node>
    {:label [] of <LabelStatement> | <CaseStatement> | <DefaultStatement>}
```

## 繰り返し
```
<RepetitionStatement> ::= extends <Statement>
    {:condition <Expression>
　   :body [] of <Expression> | <Declaration> | <Statement>
    }
```  
- conditionは，繰り返しの条件式を出力する
- bodyは，繰り返す処理を出力する

## while
```
<WhileStatement> ::= extends <RepetitionStatement>
    {:kind "While"}
```  

## do
```
<DoStatement> ::= extends <RepetitionStatement>
    {:kind "Do"}
```  

## for
```
<ForStatement> ::= extends <RepetitionStatement>
    {:kind "For"
     :init <Declaration> | <Expression>
     :update <Expression>
    }
```  
- initは，初期設定を出力する
- updateは，更新式を出力する

## 分岐
```
<BranchStatement> ::= extends <Statement>
    {:condition <Expression>}
```  
- conditionは，条件分岐の条件式を出力する

## if
```
<IfStatement> ::= extends <BranchStatement>
    {:kind "If"
     :then [] of <Declaration> | <Expression>
     :else [] of <Declaration> | <Expression>
    }
```  
- thenは，条件分岐のthen処理を出力する
- elseは，条件分岐のelse処理を出力する

## switch
```
<SwitchStatement> ::= extends <BranchStatement>
    {:kind "Switch"
     :body []　of <Declaration> | <Expression> | <Statement>
    }
```  
- bodyは，処理を出力する

## label
```
<LabelStatement> ::= extends <Statement>
    {:kind "Label"
     :name String
    }
``` 
- nameは，labelに付けられた名前を出力する 

## case
```
<CaseStatement> ::= extends <Statement>
    {:kind "Case"
     :value <Literal>
    }
```  
- valueは，caseの後に続く値を出力する

## default
```
<DefaultStatement> ::= extends <Statement>
    {:kind "Default"}
```

## goto
```
<GotoStatement> ::= extends <Statement>
    {:kind "Goto"
     :goto String
    }
```  
- gotoのStringは，ジャンプ先のlabe名を出力する  

## continue
```
<ContinueStatement> ::= extends <Statement>
    {:kind "Continue"}
```  

## break
```
<BreakStatement> ::= extends <Statement>
    {:kind "Break"}
```  

## return
```
<ReturnStatement> ::= extends <Statement>
    {:kind "Ret"
     opt:value <Expression>
    }
```  
- 返り値が存在する場合valueは出力される
- valueは，返り値の値または，参照した変数の情報を出力する

# 型
```
<Type> ::= extends <Node>
    {opt:const "true"
     opt:volatile "true"
    }
```
- typeに出力される要素を定義している
- const，volatileがtrueの場合出力される

## Void型
```
<VoidType> ::= extends <Type>
  {:kind "VoidType"}
```

## Bool型
```
<BoolType> ::= extends <Type>
  {:kind "BoolType"}
```

# 符号あり型
```
<SignedType> :: extends <Type>
```

## Int型
```
<IntType> ::= extends <SignedType>
    {:kind "IntType"}
```

## Float型
```
<FloatType> ::= extends <SignedType>
    {:kind "FloatType"}
```

## Long型
```
<LongType> ::= extends <SignedType>
    {:kind "LongType"}
```

## Double型
```
<DoubleType> ::= extends <SignedType>
    {:kind "DoubleType"}
```

## Short型
```
<ShortType> ::= extends <SignedType>
    {:kind "ShortType"}
```

## Char型
```
<CharType> ::= extends <SignedType>
    {:kind "CharType"}
```

## LongLong型
```
<LongLongType> ::= extends <SignedType>
    {:kind "LongLongType"}
```

## LongDouble型
```
<LongDoubleType> ::= extends <SignedType>
   {:kind "LongDoubleType"}
```


# 符号なし型
```
<UnsignedType> :: extends <Type>
```

## Int型
```
<IntType> ::= extends <UnsignedType>
    {:kind "UnsignedIntType"}
```

## Float型
```
<FloatType> ::= extends <UnsignedType>
    {:kind "UnsignedFloatType"}
```

## Long型
```
<LongType> ::= extends <UnsignedType>
    {:kind "UnsignedLongType"}
```

## Double型
```
<DoubleType> ::= extends <UnsignedType>
    {:kind "UnsignedDoubleType"}
```

## Short型
```
<ShortType> ::= extends <UnsignedType>
    {:kind "UnsignedShortType"}
```

## Char型
```
<CharType> ::= extends <UnsignedType>
    {:kind "UnsignedCharType"}
```

## LongLong型
```
<LongLongType> ::= extends <UnsignedType>
    {:kind "UnsignedLongLongType"}
```

## ポインタ型
```
<PointerType> ::= extends <Type>
    {:kind "PointerType"
     :pointee <Type>
    }
```
- pointeeは，ポインタ型の型を出力する

## 関数型
```
<FunctionType> ::= extends <Type>
    {:kind "FuncType"
     :parm-type [] of <Type>
     :ret-type  <Type>
    }
```
- ret-typeは，返り値の型を出力する

## 配列型
```
<ArrayType> ::= extends <Type>
    {:kind "ArrayType"
     :array-size <Expression>
     :type <Type>
    }
```
- array-sizeのIntは，配列のサイズを出力する
- typeは，配列の型を出力する

## 構造体型
```
<StructureType> ::= extends <Type>
    {:kind "StructureType"
     :name String
     :member [] of <FieldDeclaration>
    }
```
- nameのStringは，構造体の名前を出力する
- memberは，構造体で宣言されたメンバ変数を出力する

## 共用体
```
<UnionType> ::= extends <Type>
    {:kind "UnionType"
     :name String
     :member [] of <FieldDeclaration>
    }
```
- nameのStringは，共用体の名前を出力する
- memberは，構造体で宣言されたメンバ変数を出力する

## typedef型
```
<TypedefType> ::= extends <Type>
    {:kind "TypedefType"
     :typedef-name String
     :typedef-type <Type>
    }
```
- typedef-nameには，typedefで宣言した型名を出力する
- typedef-typeは，typedefで宣言した型を出力する
