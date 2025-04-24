#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Sema/Sema.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

namespace {

class ImplicitConversionVisitor : public RecursiveASTVisitor<ImplicitConversionVisitor> {
public:
    explicit ImplicitConversionVisitor(ASTContext &Context) : Context(Context) {}

    bool VisitFunctionDecl(FunctionDecl *FD) {
        if (FD->hasBody()) {
            currentFunction = FD->getNameAsString();
        } else {
            currentFunction.clear();
        }
        return true;
    }

    bool VisitImplicitCastExpr(ImplicitCastExpr *ICE) {
        if (currentFunction.empty()) {
            return true; // игнорировать вне функций
        }

        QualType FromType = ICE->getSubExpr()->getType();
        QualType ToType = ICE->getType();

        /* 
            LValue-to-RValue преобразования порождают приколы по типу int -> int
            Закоментить / Раскоментить следующие 3 строки, чтобы фильтровать 'странные' преобразования
        */ 
        // if (FromType.getAsString() == ToType.getAsString()) {
        //     return true;
        // }

        std::string key = FromType.getAsString() + " -> " + ToType.getAsString();
        functionConversions[currentFunction][key]++;

        return true;
    }

    void printStats() {
        for (const auto &funcEntry : functionConversions) {
            llvm::outs() << "Function `" << funcEntry.first << "`\n";
            for (const auto &conversion : funcEntry.second) {
                llvm::outs() << conversion.first << ": " << conversion.second << "\n";
            }
            llvm::outs() << "\n";
        }
    }

private:
    ASTContext &Context;
    std::string currentFunction;
    std::map<std::string, std::map<std::string, unsigned>> functionConversions;
};

class ImplicitConversionConsumer : public ASTConsumer {
public:
    explicit ImplicitConversionConsumer(CompilerInstance &Instance)
        : Visitor(Instance.getASTContext()) {}

    void HandleTranslationUnit(ASTContext &Context) override {
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
        Visitor.printStats();
    }

private:
    ImplicitConversionVisitor Visitor;
};

class ImplicitConversionAction : public PluginASTAction {
protected:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                  llvm::StringRef) override {
        return std::make_unique<ImplicitConversionConsumer>(CI);
    }

    bool ParseArgs(const CompilerInstance &,
                  const std::vector<std::string> &) override {
        return true;
    }

    PluginASTAction::ActionType getActionType() override {
        return AddBeforeMainAction;
    }
};

} // namespace

static FrontendPluginRegistry::Add<ImplicitConversionAction>
    X("count-implicit-conversions", "Count implicit type conversions in functions");
