//
// Created by David Yang on 2025-04-22.
//

#include <bitset>

#include <algorithm>
#include <ranges>

#include "headers/ast.h"

tc_Bitset::tc_Bitset(const char *bits) {
    this->bits = bits;
}

tc_Bitset::tc_Bitset(std::vector<bool> bits) {
    for (auto bit : bits) {
        this->bits += bit ? '1' : '0';
    }
}

std::string tc_Bitset::get_bits() const {
    return this->bits;
}


// Visitor static method implementations
void Visitor::visit(ProgramNode* node) {
    (void)node;
}

void Visitor::visit(ExprNumberNode* node) {
    (void)node;
}

void Visitor::visit(ExprIdentifierNode* node) {
    (void)node;
}

void Visitor::visit(ExprAssignmentNode* node) {
    (void)node;
}

void Visitor::visit(ExprAddNode* node) {
    (void)node;
}

void Visitor::visit(ExprSubNode* node) {
    (void)node;
}

void Visitor::visit(ExprMultNode* node) {
    (void)node;
}

void Visitor::visit(ExprDivNode* node) {
    (void)node;
}

void Visitor::visit(ExprExpoNode* node) {
    (void)node;
}

void Visitor::visit(ExprVariableNode* node) {
    (void)node;
}

void Visitor::visit(ExprBinaryOp* node) {
    (void)node;
}

void Visitor::visit(ExprUnaryOp* node) {
    (void)node;
}

void Visitor::visit(StmtOutputNode* node) {
    (void)node;
}

void Visitor::visit(StmtInputNode* node) {
    (void)node;
}

void Visitor::visit(StmtLoopNode* node) {
    (void)node;
}

void Visitor::visit(StmtImportNode* node) {
    (void)node;
}

void Visitor::visit(StmtPackageNode* node) {
    (void)node;
}

void Visitor::visit(StmtAsciiNode* node) {
    (void)node;
}

void Visitor::visit(StmtArrayNode* node) {
    (void)node;
}


void WhichVisitor::visit(ProgramNode* node) {
    (void)node;
    this->visitor_type_name = "ProgramNode";
}

void WhichVisitor::visit(ExprNumberNode* node) {
    (void)node;
    this->visitor_type_name = "ExprNumberNode";
}

void WhichVisitor::visit(ExprIdentifierNode* node) {
    (void)node;
    this->visitor_type_name = "ExprIdentifierNode";
}

void WhichVisitor::visit(ExprAssignmentNode* node) {
    (void)node;
    this->visitor_type_name = "ExprAssignmentNode";
}

void WhichVisitor::visit(ExprAddNode* node) {
    (void)node;
    this->visitor_type_name = "ExprAddNode";
}

void WhichVisitor::visit(ExprSubNode* node) {
    (void)node;
    this->visitor_type_name = "ExprSubNode";
}

void WhichVisitor::visit(ExprMultNode* node) {
    (void)node;
    this->visitor_type_name = "ExprMultNode";
}

void WhichVisitor::visit(ExprDivNode* node) {
    (void)node;
    this->visitor_type_name = "ExprDivNode";
}

void WhichVisitor::visit(ExprExpoNode* node) {
    (void)node;
    this->visitor_type_name = "ExprExpoNode";
}

void WhichVisitor::visit(ExprVariableNode* node) {
    (void)node;
    this->visitor_type_name = "ExprVariableNode";
}

void WhichVisitor::visit(ExprBinaryOp* node) {
    (void)node;
    this->visitor_type_name = "ExprBinaryOp";
}

void WhichVisitor::visit(ExprUnaryOp* node) {
    (void)node;
    this->visitor_type_name = "ExprUnaryOp";
}

void WhichVisitor::visit(StmtOutputNode* node) {
    (void)node;
    this->visitor_type_name = "StmtOutputNode";
}

void WhichVisitor::visit(StmtInputNode* node) {
    (void)node;
    this->visitor_type_name = "StmtInputNode";
}

void WhichVisitor::visit(StmtLoopNode* node) {
    (void)node;
    this->visitor_type_name = "StmtLoopNode";
}

void WhichVisitor::visit(StmtImportNode* node) {
    (void)node;
    this->visitor_type_name = "StmtImportNode";
}

void WhichVisitor::visit(StmtPackageNode* node) {
    (void)node;
    this->visitor_type_name = "StmtPackageNode";
}

void WhichVisitor::visit(StmtAsciiNode* node) {
    (void)node;
    this->visitor_type_name = "StmtAsciiNode";
}

void WhichVisitor::visit(StmtArrayNode *node) {
    (void)node;
    this->visitor_type_name = "StmtArrayNode";
}


void BitAssignVisitor::visit(ExprVariableNode *node) {
    node->bits = this->_bitsToAssign;
}

void OutputNameAssignVisitor::visit(StmtOutputNode *node) {
    node->name = this->_outputName;
}

void VariableNameAssignVisitor::visit(ExprVariableNode *node) {
    node->name = this->_variableName;
}

void VariableValueGetterVisitor::visit(ExprVariableNode *node) {
    this->_value = node->bits;
    this->_name = node->name;
}

void OutputNameGetterVisitor::visit(StmtOutputNode *node) {
    this->_name = node->name;
}

void IdentifierNameAssignVisitor::visit(ExprIdentifierNode *node) {
    node->name = this->_name;
}

void ArrayNameManagementVisitor::visit(StmtArrayNode *node) {
    this->_name = node->name;
}

void IdentifierNameGetterVisitor::visit(ExprIdentifierNode *node) {
    this->_name = node->name;
}

void LoopIterationCountIdentifierAssignAndGetterVisitor::visit(StmtLoopNode *node) {
    node->iteration_count_identifier = this->_name;
}

void LoopIterationCountGetterVisitor::visit(StmtLoopNode *node) {
    this->_name = node->iteration_count_identifier;
}


tc_Bitset VariableValueGetterVisitor::getValue() const {
    return this->_value;
}

std::string VariableValueGetterVisitor::getName() const {
    return this->_name;
}

std::string OutputNameGetterVisitor::getName() const {
    return this->_name;
}

std::string IdentifierNameAssignVisitor::getName() const {
    return this->_name;
}

void IdentifierNameAssignVisitor::setName(std::string name) {
    this->_name = std::move(name);
}

std::string ArrayNameManagementVisitor::getName() const {
    return this->_name;
}

std::string IdentifierNameGetterVisitor::getName() const {
    return this->_name;
}

std::string LoopIterationCountIdentifierAssignAndGetterVisitor::getName() const {
    return this->_name;
}

std::string LoopIterationCountGetterVisitor::getName() const {
    return this->_name;
}



// Implement the AST base class methods
void AST::addChild(std::shared_ptr<AST> child) {
    children.push_back(child);
    child->addParent(shared_from_this());
}

void AST::removeChild(std::shared_ptr<AST> child) {
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        children.erase(it);
        child->addParent(nullptr);
    }
}

void AST::removeAllChildren() {
    for (const auto& child : children) {
        child->addParent(nullptr);
    }
    children.clear();
}

void AST::addParent(std::shared_ptr<AST> parent) {
    this->parent = parent;
}

void AST::accept(Visitor* visitor) {
    (void)visitor;
}

// TODO ACCEPT

void ProgramNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void ExprNumberNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void ExprIdentifierNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void ExprAssignmentNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void ExprAddNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void ExprSubNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void ExprMultNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void ExprDivNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void ExprExpoNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void ExprVariableNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void ExprBinaryOp::accept(Visitor* visitor) {
    visitor->visit(this);
}

void ExprUnaryOp::accept(Visitor* visitor) {
    visitor->visit(this);
}

void StmtLoopNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void StmtImportNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void StmtPackageNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void StmtAsciiNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void StmtOutputNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void StmtInputNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void StmtArrayNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

// TODO ADD CHILD

void ProgramNode::addChild(std::shared_ptr<AST> child) {
    children.push_back(child);
    child->addParent(shared_from_this());
}

void ExprNumberNode::addChild(std::shared_ptr<AST> child) {
    children.push_back(child);
    child->addParent(shared_from_this());
}

void ExprIdentifierNode::addChild(std::shared_ptr<AST> child) {
    children.push_back(child);
    child->addParent(shared_from_this());
}

void ExprAssignmentNode::addChild(std::shared_ptr<AST> child) {
    children.push_back(child);
    child->addParent(shared_from_this());
}

void ExprAddNode::addChild(std::shared_ptr<AST> child) {
    children.push_back(child);
    child->addParent(shared_from_this());
}

void ExprSubNode::addChild(std::shared_ptr<AST> child) {
    children.push_back(child);
    child->addParent(shared_from_this());
}

void ExprMultNode::addChild(std::shared_ptr<AST> child) {
    children.push_back(child);
    child->addParent(shared_from_this());
}

void ExprDivNode::addChild(std::shared_ptr<AST> child) {
    children.push_back(child);
    child->addParent(shared_from_this());
}

void ExprExpoNode::addChild(std::shared_ptr<AST> child) {
    children.push_back(child);
    child->addParent(shared_from_this());
}

void ExprVariableNode::addChild(std::shared_ptr<AST> child) {
    children.push_back(child);
    child->addParent(shared_from_this());
}

void ExprBinaryOp::addChild(std::shared_ptr<AST> child) {
    children.push_back(child);
    child->addParent(shared_from_this());
}

void ExprUnaryOp::addChild(std::shared_ptr<AST> child) {
    children.push_back(child);
    child->addParent(shared_from_this());
}

void StmtLoopNode::addChild(std::shared_ptr<AST> child) {
    children.push_back(child);
    child->addParent(shared_from_this());
}

void StmtImportNode::addChild(std::shared_ptr<AST> child) {
    children.push_back(child);
    child->addParent(shared_from_this());
}

void StmtPackageNode::addChild(std::shared_ptr<AST> child) {
    children.push_back(child);
    child->addParent(shared_from_this());
}

void StmtAsciiNode::addChild(std::shared_ptr<AST> child) {
    children.push_back(child);
    child->addParent(shared_from_this());
}

void StmtOutputNode::addChild(std::shared_ptr<AST> child) {
    children.push_back(child);
    child->addParent(shared_from_this());
}

void StmtInputNode::addChild(std::shared_ptr<AST> child) {
    children.push_back(child);
    child->addParent(shared_from_this());
}

void StmtArrayNode::addChild(std::shared_ptr<AST> child) {
    children.push_back(child);
    child->addParent(shared_from_this());
}

// TODO REMOVE CHILD

void ProgramNode::removeChild(std::shared_ptr<AST> child) {
    auto it = std::ranges::find(children, child);
    if (it != children.end()) {
        children.erase(it);
        child->addParent(nullptr);
    }
}

void ExprNumberNode::removeChild(std::shared_ptr<AST> child) {
    auto it = std::ranges::find(children, child);
    if (it != children.end()) {
        children.erase(it);
        child->addParent(nullptr);
    }
}

void ExprIdentifierNode::removeChild(std::shared_ptr<AST>child) {
    auto it = std::ranges::find(children, child);
    if (it != children.end()) {
        children.erase(it);
        child->addParent(nullptr);
    }
}


void ExprAssignmentNode::removeChild(std::shared_ptr<AST> child) {
    auto it = std::ranges::find(children, child);
    if (it != children.end()) {
        children.erase(it);
        child->addParent(nullptr);
    }
}

void ExprAddNode::removeChild(std::shared_ptr<AST> child) {
    auto it = std::ranges::find(children, child);
    if (it != children.end()) {
        children.erase(it);
        child->addParent(nullptr);
    }
}

void ExprSubNode::removeChild(std::shared_ptr<AST> child) {
    auto it = std::ranges::find(children, child);
    if (it != children.end()) {
        children.erase(it);
        child->addParent(nullptr);
    }
}

void ExprMultNode::removeChild(std::shared_ptr<AST> child) {
    auto it = std::ranges::find(children, child);
    if (it != children.end()) {
        children.erase(it);
        child->addParent(nullptr);
    }
}

void ExprDivNode::removeChild(std::shared_ptr<AST> child) {
    auto it = std::ranges::find(children, child);
    if (it != children.end()) {
        children.erase(it);
        child->addParent(nullptr);
    }
}

void ExprExpoNode::removeChild(std::shared_ptr<AST> child) {
    auto it = std::ranges::find(children, child);
    if (it != children.end()) {
        children.erase(it);
        child->addParent(nullptr);
    }
}

void ExprVariableNode::removeChild(std::shared_ptr<AST> child) {
    auto it = std::ranges::find(children, child);
    if (it != children.end()) {
        children.erase(it);
        child->addParent(nullptr);
    }
}

void ExprBinaryOp::removeChild(std::shared_ptr<AST> child) {
    auto it = std::ranges::find(children, child);
    if (it != children.end()) {
        children.erase(it);
        child->addParent(nullptr);
    }
}

void ExprUnaryOp::removeChild(std::shared_ptr<AST> child) {
    auto it = std::ranges::find(children, child);
    if (it != children.end()) {
        children.erase(it);
        child->addParent(nullptr);
    }
}

void StmtLoopNode::removeChild(std::shared_ptr<AST> child) {
    auto it = std::ranges::find(children, child);
    if (it != children.end()) {
        children.erase(it);
        child->addParent(nullptr);
    }
}

void StmtImportNode::removeChild(std::shared_ptr<AST> child) {
    auto it = std::ranges::find(children, child);
    if (it != children.end()) {
        children.erase(it);
        child->addParent(nullptr);
    }
}

void StmtPackageNode::removeChild(std::shared_ptr<AST> child) {
    auto it = std::ranges::find(children, child);
    if (it != children.end()) {
        children.erase(it);
        child->addParent(nullptr);
    }
}

void StmtAsciiNode::removeChild(std::shared_ptr<AST> child) {
    auto it = std::ranges::find(children, child);
    if (it != children.end()) {
        children.erase(it);
        child->addParent(nullptr);
    }
}

void StmtOutputNode::removeChild(std::shared_ptr<AST> child) {
    auto it = std::ranges::find(children, child);
    if (it != children.end()) {
        children.erase(it);
        child->addParent(nullptr);
    }
}

void StmtInputNode::removeChild(std::shared_ptr<AST> child) {
    auto it = std::ranges::find(children, child);
    if (it != children.end()) {
        children.erase(it);
        child->addParent(nullptr);
    }
}

void StmtArrayNode::removeChild(std::shared_ptr<AST> child) {
    auto it = std::ranges::find(children, child);
    if (it != children.end()) {
        children.erase(it);
        child->addParent(nullptr);
    }
}

// TODO REMOVE ALL CHILDREN


void ProgramNode::removeAllChildren() {
    for (const auto& child : children) {
        child->addParent(nullptr);
    }
    children.clear();
}

void ExprNumberNode::removeAllChildren() {
    for (const auto& child : children) {
        child->addParent(nullptr);
    }
    children.clear();
}

void ExprIdentifierNode::removeAllChildren() {
    for (const auto& child : children) {
        child->addParent(nullptr);
    }
    children.clear();
}

void ExprAssignmentNode::removeAllChildren() {
    for (const auto& child : children) {
        child->addParent(nullptr);
    }
    children.clear();
}

void ExprAddNode::removeAllChildren() {
    for (const auto& child : children) {
        child->addParent(nullptr);
    }
    children.clear();
}

void ExprSubNode::removeAllChildren() {
    for (const auto& child : children) {
        child->addParent(nullptr);
    }
    children.clear();
}

void ExprMultNode::removeAllChildren() {
    for (const auto& child : children) {
        child->addParent(nullptr);
    }
    children.clear();
}

void ExprDivNode::removeAllChildren() {
    for (const auto& child : children) {
        child->addParent(nullptr);
    }
    children.clear();
}

void ExprExpoNode::removeAllChildren() {
    for (const auto& child : children) {
        child->addParent(nullptr);
    }
    children.clear();
}

void ExprVariableNode::removeAllChildren() {
    for (const auto& child : children) {
        child->addParent(nullptr);
    }
    children.clear();
}

void ExprBinaryOp::removeAllChildren() {
    for (const auto& child : children) {
        child->addParent(nullptr);
    }
    children.clear();
}

void ExprUnaryOp::removeAllChildren() {
    for (const auto& child : children) {
        child->addParent(nullptr);
    }
    children.clear();
}

void StmtLoopNode::removeAllChildren() {
    for (const auto& child : children) {
        child->addParent(nullptr);
    }
    children.clear();
}

void StmtImportNode::removeAllChildren() {
    for (const auto& child : children) {
        child->addParent(nullptr);
    }
    children.clear();
}

void StmtPackageNode::removeAllChildren() {
    for (const auto& child : children) {
        child->addParent(nullptr);
    }
    children.clear();
}

void StmtAsciiNode::removeAllChildren() {
    for (const auto& child : children) {
        child->addParent(nullptr);
    }
    children.clear();
}

void StmtOutputNode::removeAllChildren() {
    for (const auto& child : children) {
        child->addParent(nullptr);
    }
    children.clear();
}

void StmtInputNode::removeAllChildren() {
    for (const auto& child : children) {
        child->addParent(nullptr);
    }
    children.clear();
}

void StmtArrayNode::removeAllChildren() {
    for (const auto& child : children) {
        child->addParent(nullptr);
    }
    children.clear();
}

// TODO ADD PARENT

void ProgramNode::addParent(std::shared_ptr<AST> parent) {
    this->parent = parent;
}

void ExprNumberNode::addParent(std::shared_ptr<AST> parent) {
    this->parent = parent;
}

void ExprIdentifierNode::addParent(std::shared_ptr<AST> parent) {
    this->parent = parent;
}

void ExprAssignmentNode::addParent(std::shared_ptr<AST> parent) {
    this->parent = parent;
}

void ExprAddNode::addParent(std::shared_ptr<AST> parent) {
    this->parent = parent;
}

void ExprSubNode::addParent(std::shared_ptr<AST> parent) {
    this->parent = parent;
}

void ExprMultNode::addParent(std::shared_ptr<AST> parent) {
    this->parent = parent;
}

void ExprDivNode::addParent(std::shared_ptr<AST> parent) {
    this->parent = parent;
}

void ExprExpoNode::addParent(std::shared_ptr<AST> parent) {
    this->parent = parent;
}

void ExprVariableNode::addParent(std::shared_ptr<AST> parent) {
    this->parent = parent;
}

void ExprBinaryOp::addParent(std::shared_ptr<AST> parent) {
    this->parent = parent;
}

void ExprUnaryOp::addParent(std::shared_ptr<AST> parent) {
    this->parent = parent;
}

void StmtLoopNode::addParent(std::shared_ptr<AST> parent) {
    this->parent = parent;
}

void StmtImportNode::addParent(std::shared_ptr<AST> parent) {
    this->parent = parent;
}

void StmtPackageNode::addParent(std::shared_ptr<AST> parent) {
    this->parent = parent;
}

void StmtAsciiNode::addParent(std::shared_ptr<AST> parent) {
    this->parent = parent;
}

void StmtOutputNode::addParent(std::shared_ptr<AST> parent) {
    this->parent = parent;
}

void StmtInputNode::addParent(std::shared_ptr<AST> parent) {
    this->parent = parent;
}

void StmtArrayNode::addParent(std::shared_ptr<AST> parent) {
    this->parent = parent;
}
