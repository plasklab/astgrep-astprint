add_clang_executable(astprint
astprint.cpp
)

target_link_libraries(astprint clangFrontendTool clangFrontend 
clangDriver clangSerialization clangCodeGen clangParse clangSema
clangStaticAnalyzerFrontend clangStaticAnalyzerCheckers
clangStaticAnalyzerCore clangAnalysis clangARCMigrate
clangRewriteCore clangRewriteFrontend clangEdit clangAST
clangASTMatchers clangLex clangBasic clangTooling)