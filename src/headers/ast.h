//
// Created by David Yang on 2025-04-22.
//

#pragma once
#include <utility>
#include <vector>


class tc_Bitset {
public:
    explicit tc_Bitset(const char* bits);
    explicit tc_Bitset(std::vector<bool> bits);

    [[nodiscard]] std::string get_bits() const;

private:
    std::string bits{};
};

/**
 * @class Visitor
 * @brief Provides an interface for visiting various types of nodes in an abstract syntax tree (AST).
 *
 * The Visitor class defines a set of methods for visiting different node types
 * in an AST, enabling the implementation of operations on node structures
 * without modifying their classes. Each "visit" method corresponds to a specific
 * node type and facilitates extending the functionality of the AST through
 * the visitor pattern.
 */
class Visitor {
public:
    Visitor() = default;
    virtual ~Visitor() = default;

    virtual void visit(class ProgramNode* node);
    virtual void visit(class ExprNumberNode* node);
    virtual void visit(class ExprIdentifierNode* node);
    virtual void visit(class ExprAssignmentNode* node);
    virtual void visit(class ExprAddNode* node);
    virtual void visit(class ExprSubNode* node);
    virtual void visit(class ExprMultNode* node);
    virtual void visit(class ExprDivNode* node);
    virtual void visit(class ExprExpoNode* node);
    virtual void visit(class ExprVariableNode* node);
    virtual void visit(class ExprBinaryOp* node);
    virtual void visit(class ExprUnaryOp* node);
    virtual void visit(class StmtOutputNode* node);
    virtual void visit(class StmtInputNode* node);
    virtual void visit(class StmtLoopNode* node);
    virtual void visit(class StmtImportNode* node);
    virtual void visit(class StmtPackageNode* node);
    virtual void visit(class StmtAsciiNode* node);
    virtual void visit(class StmtArrayNode* node);
};

class WhichVisitor final : public Visitor {
public:
    WhichVisitor () = default;

    void visit(ProgramNode* node) override;
    void visit(ExprNumberNode* node) override;
    void visit(ExprIdentifierNode* node) override;
    void visit(ExprAssignmentNode* node) override;
    void visit(ExprAddNode* node) override;
    void visit(ExprSubNode* node) override;
    void visit(ExprMultNode* node) override;
    void visit(ExprDivNode* node) override;
    void visit(ExprExpoNode* node) override;
    void visit(ExprVariableNode* node) override;
    void visit(ExprBinaryOp* node) override;
    void visit(ExprUnaryOp* node) override;
    void visit(StmtOutputNode* node) override;
    void visit(StmtInputNode* node) override;
    void visit(StmtLoopNode* node) override;
    void visit(StmtImportNode* node) override;
    void visit(StmtPackageNode* node) override;
    void visit(StmtAsciiNode* node) override;
    void visit(StmtArrayNode* node) override;

    [[nodiscard]] std::string getVisitorTypeName() const { return visitor_type_name; }


    std::string visitor_type_name;
};

template <typename T>
class NameGetterVisitor final {
    NameGetterVisitor() = default;

    std::string nameGetter(T node);
};


class BitAssignVisitor final : public Visitor {
public:
    explicit BitAssignVisitor(const tc_Bitset& bits) : _bitsToAssign(bits) {}

    void visit(ExprVariableNode* node) override;

private:
    tc_Bitset _bitsToAssign;
};

class OutputNameAssignVisitor final : public Visitor {
public:
    explicit OutputNameAssignVisitor(std::string  name) : _outputName(std::move(name)) {}

    void visit(StmtOutputNode* node) override;

private:
    std::string _outputName;
};

class VariableNameAssignVisitor final : public Visitor {
public:
    explicit VariableNameAssignVisitor(std::string name) : _variableName(std::move(name)) {}

    void visit(ExprVariableNode* node) override;

private:
    std::string _variableName;
};

class VariableValueGetterVisitor final : public Visitor {
public:
    VariableValueGetterVisitor(std::string name, tc_Bitset value) : _value(std::move(value)), _name(std::move(name)) {}

    void visit(ExprVariableNode* node) override;

    [[nodiscard]] tc_Bitset getValue() const;
    [[nodiscard]] std::string getName() const;

private:
    tc_Bitset _value;
    std::string _name;
};

class OutputNameGetterVisitor final : public Visitor {
public:
    OutputNameGetterVisitor() = default;

    void visit(StmtOutputNode* node) override;

    [[nodiscard]] std::string getName() const;

private:
    std::string _name;
};

class IdentifierNameAssignVisitor final : public Visitor {
public:
    IdentifierNameAssignVisitor() = default;
    explicit IdentifierNameAssignVisitor(std::string name) : _name(std::move(name)) {}

    void visit(ExprIdentifierNode* node) override;

    [[nodiscard]] std::string getName() const;

    void setName(std::string name);

private:
    std::string _name;
};

class IdentifierNameGetterVisitor final : public Visitor {
public:
    IdentifierNameGetterVisitor() = default;
    explicit IdentifierNameGetterVisitor(std::string name) : _name(std::move(name)) {}

    void visit(ExprIdentifierNode* node) override;

    [[nodiscard]] std::string getName() const;

private:
    std::string _name;
};

class ArrayNameManagementVisitor final : public Visitor {
public:
    ArrayNameManagementVisitor() = default;
    explicit ArrayNameManagementVisitor(std::string name) : _name(std::move(name)) {}

    void visit(StmtArrayNode* node) override;

    [[nodiscard]] std::string getName() const;

private:
    std::string _name;
};

class LoopIterationCountIdentifierAssignAndGetterVisitor final : public Visitor {
public:
    LoopIterationCountIdentifierAssignAndGetterVisitor() = default;
    explicit LoopIterationCountIdentifierAssignAndGetterVisitor(std::string name) : _name(std::move(name)) {}

    void visit(StmtLoopNode *node) override;

    [[nodiscard]] std::string getName() const;

private:
    std::string _name;
};

class LoopIterationCountGetterVisitor final : public Visitor {
public:
    LoopIterationCountGetterVisitor() = default;

    void visit (StmtLoopNode *node) override;

    [[nodiscard]] std::string getName() const;

private:
    std::string _name;
};

/**
 * @class AST
 * @brief Represents a base class for an abstract syntax tree (AST) structure.
 *
 * The AST class serves as the base class for building hierarchical structures
 * for abstract syntax trees. It supports parent-child relationships and
 * provides functionality for manipulating the tree structure and visiting nodes.
 */
class AST : public std::enable_shared_from_this<AST> {
public:
    AST() : parent(nullptr) {}
    virtual ~AST() = default;

    virtual void accept(Visitor* visitor);

    virtual void addChild(std::shared_ptr<AST> child);
    virtual void removeChild(std::shared_ptr<AST> child);
    virtual void removeAllChildren();

    [[nodiscard]] std::shared_ptr<AST> getParent() const { return parent; }
    [[nodiscard]] const std::vector<std::shared_ptr<AST>>& getChildren() const { return children; }

    virtual void addParent(std::shared_ptr<AST> parent);

protected:
    std::shared_ptr<AST> parent;
    std::vector<std::shared_ptr<AST>> children;
};

/**
 * @class ExpressionNode
 * @brief Represents the base class for all expression nodes in an abstract syntax tree (AST).
 *
 * The ExpressionNode class serves as a foundational abstraction for nodes that
 * represent expressions in an AST. It extends the AST base class and provides
 * a common interface for different types of expression nodes.
 */
class ExpressionNode : public AST {
public:
    ExpressionNode() = default;
    ~ExpressionNode() override = default;
};

/**
 * @class StatementNode
 * @brief Represents a base class for all statement nodes within the abstract syntax tree (AST).
 *
 * The StatementNode class serves as a foundation for statement-related nodes in
 * the AST hierarchy. It provides a common structure for specific types of
 * statements, enabling easier management and extensibility of different statement types.
 */
class StatementNode : public AST {
public:
    StatementNode() = default;
    ~StatementNode() override = default;
};

/**
 * @class ProgramNode
 * @brief Represents the root node of a program's abstract syntax tree (AST).
 *
 * The ProgramNode class serves as the top-level container for managing the structure of an AST.
 * It facilitates parent-child relationships and provides methods for adding and removing child nodes,
 * as well as managing the parent node. It additionally supports visitor-based traversal through its `accept` method.
 * This class is a concrete implementation of the AST base class and cannot be inherited further.
 */
class ProgramNode final : public AST {
public:
    ProgramNode() = default;
    ~ProgramNode() override = default;

    void accept(Visitor* visitor) override;

    void addChild(std::shared_ptr<AST> child) override;
    void removeChild(std::shared_ptr<AST> child) override;
    void removeAllChildren() override;

    void addParent(std::shared_ptr<AST> parent) override;
};

/**
 * @class ExprNumberNode
 * @brief Represents a node in the abstract syntax tree (AST) that holds a numeric value.
 *
 * The ExprNumberNode class is a specific implementation of ExpressionNode designed
 * to represent numeric literals in the AST. It stores a floating-point value and
 * supports tree manipulation methods for parent-child relationships as well as
 * the visitor pattern for traversing.
 */
class ExprNumberNode final : public ExpressionNode {
public:
    explicit ExprNumberNode(const float value) : value(value) {}
    ~ExprNumberNode() override = default;

    void accept(Visitor* visitor) override;

    void addChild(std::shared_ptr<AST> child) override;
    void removeChild(std::shared_ptr<AST> child) override;
    void removeAllChildren() override;

    void addParent(std::shared_ptr<AST> parent) override;

    float value;
};

/**
 * @class ExprIdentifierNode
 * @brief Represents an identifier expression node in the abstract syntax tree (AST).
 *
 * The ExprIdentifierNode class is a specialized type of ExpressionNode that represents
 * an identifier in the syntax tree. It provides functionality to interact with visitor
 * objects as part of a visitor pattern and supports parent-children relationships within
 * the AST structure.
 */
class ExprIdentifierNode final : public ExpressionNode {
public:
    ExprIdentifierNode() = default;
    ~ExprIdentifierNode() override = default;

    void accept(Visitor *visitor) override;

    void addChild(std::shared_ptr<AST> child) override;
    void removeChild(std::shared_ptr<AST> child) override;
    void removeAllChildren() override;

    void addParent(std::shared_ptr<AST> parent) override;

    std::string name;
};

/**
 * @class ExprAssignmentNode
 * @brief Represents an assignment expression node in the abstract syntax tree (AST).
 *
 * The ExprAssignmentNode class is a specific type of ExpressionNode that models
 * assignment operations in the AST. This class supports adding and removing child
 * nodes, managing parent-child relationships, and accepting visitors through the
 * visitor pattern for processing or transformation tasks.
 */
class ExprAssignmentNode final : public ExpressionNode {
public:
    ExprAssignmentNode() = default;
    ~ExprAssignmentNode() override = default;

    void accept(Visitor* visitor) override;

    void addChild(std::shared_ptr<AST> child) override;
    void removeChild(std::shared_ptr<AST> child) override;
    void removeAllChildren() override;

    void addParent(std::shared_ptr<AST> parent) override;
};

/**
 * @class ExprAddNode
 * @brief Represents a node in the abstract syntax tree (AST) for addition expressions.
 *
 * ExprAddNode is a part of the expression nodes hierarchy in the AST, specifically
 * designed to model binary addition operations. It inherits from ExpressionNode and
 * implements methods for managing its children and parent in the AST structure.
 */
class ExprAddNode final : public ExpressionNode {
public:
    ExprAddNode() = default;
    ~ExprAddNode() override = default;

    void accept(Visitor* visitor) override;

    void addChild(std::shared_ptr<AST> child) override;
    void removeChild(std::shared_ptr<AST> child) override;
    void removeAllChildren() override;

    void addParent(std::shared_ptr<AST> parent) override;
};

/**
 * @class ExprSubNode
 * @brief Represents a subtraction operation node in the abstract syntax tree (AST).
 *
 * The ExprSubNode class models a binary subtraction operation in the AST.
 * It inherits from ExpressionNode and implements methods for managing
 * parent and child relationships within the tree structure. Additionally,
 * it supports the visitor pattern, allowing operations to be performed
 * on instances of ExprSubNode through a Visitor interface.
 */
class ExprSubNode final : public ExpressionNode {
public:
    ExprSubNode() = default;
    ~ExprSubNode() override = default;

    void accept(Visitor* visitor) override;

    void addChild(std::shared_ptr<AST> child) override;
    void removeChild(std::shared_ptr<AST> child) override;
    void removeAllChildren() override;

    void addParent(std::shared_ptr<AST> parent) override;
};

/**
 * @class ExprMultNode
 * @brief Represents a multiplication expression node in the abstract syntax tree (AST).
 *
 * The ExprMultNode class is a specialized type of ExpressionNode that models
 * a multiplication operation in an abstract syntax tree. It provides implementations
 * for managing parent and child relationships within the tree, as well as accepting
 * visitors for traversal or additional processing. This class adheres to the visitor
 * pattern to facilitate extensible operations on tree structures.
 */
class ExprMultNode final : public ExpressionNode {
public:
    ExprMultNode() = default;
    ~ExprMultNode() override = default;

    void accept(Visitor* visitor) override;

    void addChild(std::shared_ptr<AST> child) override;
    void removeChild(std::shared_ptr<AST> child) override;
    void removeAllChildren() override;

    void addParent(std::shared_ptr<AST> parent) override;
};

/**
 * @class ExprDivNode
 * @brief Represents a division operation node in the abstract syntax tree (AST).
 *
 * The ExprDivNode class models a division operation as part of an expression
 * in the AST. It provides mechanisms to accept a visitor for processing,
 * manage child nodes, and establish parent-child relationships within the tree.
 *
 * This class ensures that the corresponding visit logic in the visitor is invoked,
 * and it allows adding or removing children dynamically to form the AST structure.
 */
class ExprDivNode final : public ExpressionNode {
public:
    ExprDivNode() = default;
    ~ExprDivNode() override = default;

    void accept(Visitor* visitor) override;

    void addChild(std::shared_ptr<AST> child) override;
    void removeChild(std::shared_ptr<AST> child) override;
    void removeAllChildren() override;

    void addParent(std::shared_ptr<AST> parent) override;
};

/**
 * @class ExprExpoNode
 * @brief Represents an exponential expression node in the abstract syntax tree (AST).
 *
 * ExprExpoNode is a concrete implementation of the ExpressionNode class, used to
 * model exponential operations in the AST. This class overrides methods to handle
 * child and parent relationships and provides an accept method for the Visitor pattern.
 *
 * The primary purpose of this node is to support the representation of exponentiation
 * operations in expressions, facilitating tree traversal and manipulation.
 */
class ExprExpoNode final : public ExpressionNode {
public:
    ExprExpoNode() = default;
    ~ExprExpoNode() override = default;

    void accept(Visitor* visitor) override;

    void addChild(std::shared_ptr<AST> child) override;
    void removeChild(std::shared_ptr<AST> child) override;
    void removeAllChildren() override;

    void addParent(std::shared_ptr<AST> parent) override;
};

/**
 * @class ExprVariableNode
 * @brief Represents a variable node in the abstract syntax tree (AST).
 *
 * ExprVariableNode is a specialized type of ExpressionNode that encapsulates
 * the behavior of a variable in an abstract syntax tree. This class
 * provides functionality to accept visitors through the visitor apattern,
 * as well as manage parent and child node relationships in the AST hierarchy.
 */
class ExprVariableNode final : public ExpressionNode {
public:
    ExprVariableNode() : bits("0") {};
    ~ExprVariableNode() override = default;

    void accept(Visitor* visitor) override;

    void addChild(std::shared_ptr<AST> child) override;
    void removeChild(std::shared_ptr<AST> child) override;
    void removeAllChildren() override;

    void addParent(std::shared_ptr<AST> parent) override;

    tc_Bitset bits;
    std::string name;
};

/**
 * @class ExprBinaryOp
 * @brief Represents a binary operation expression node within an abstract syntax tree (AST).
 *
 * ExprBinaryOp is a final class that inherits from ExpressionNode and models
 * an expression with two operands joined by a binary operator. This class
 * implements functionality to manage its relationship with child and parent
 * nodes, and provides a method to accept a visitor for traversing or
 * applying operations following the visitor pattern.
 */
class ExprBinaryOp final : public ExpressionNode {
public:
    ExprBinaryOp() = default;
    ~ExprBinaryOp() override = default;

    void accept(Visitor* visitor) override;

    void addChild(std::shared_ptr<AST> child) override;
    void removeChild(std::shared_ptr<AST> child) override;
    void removeAllChildren() override;

    void addParent(std::shared_ptr<AST> parent) override;
};

/**
 * @class ExprUnaryOp
 * @brief Represents a unary operation expression node in the abstract syntax tree (AST).
 *
 * The ExprUnaryOp class inherits from the ExpressionNode class and provides functionality
 * specific to unary operations for expression trees. It supports visiting functionality
 * via the Visitor pattern and allows managing relationships between parent and child AST nodes.
 *
 * This class enables the addition, removal, and management of its child nodes, while
 * also supporting visitor-based traversal and operations.
 */
class ExprUnaryOp final : public ExpressionNode {
public:
    ExprUnaryOp() = default;
    ~ExprUnaryOp() override = default;

    void accept(Visitor* visitor) override;

    void addChild(std::shared_ptr<AST> child) override;
    void removeChild(std::shared_ptr<AST> child) override;
    void removeAllChildren() override;

    void addParent(std::shared_ptr<AST> parent) override;
};

/**
 * @class StmtLoopNode
 * @brief Represents a loop statement node in the abstract syntax tree (AST).
 *
 * The StmtLoopNode class is a specialized type of StatementNode that encapsulates
 * the structure and behavior of a loop statement within the AST. It supports operations
 * such as accepting a visitor, managing child nodes, and establishing parent-child
 * relationships within the tree structure.
 */
class StmtLoopNode final : public StatementNode {
public:
    StmtLoopNode() = default;
    ~StmtLoopNode() override = default;

    void accept(Visitor* visitor) override;

    void addChild(std::shared_ptr<AST> child) override;
    void removeChild(std::shared_ptr<AST> child) override;
    void removeAllChildren() override;

    void addParent(std::shared_ptr<AST> parent) override;

    std::string iteration_count_identifier;
};

/**
 * @class StmtImportNode
 * @brief Represents an import statement node in the abstract syntax tree (AST).
 *
 * The StmtImportNode class is a final implementation of a statement node designed
 * to handle import statements in the AST. It provides methods for interacting with
 * child and parent nodes, as well as accepting visitors for operations based on
 * the visitor design pattern.
 */
class StmtImportNode final : public StatementNode {
public:
    StmtImportNode() = default;
    ~StmtImportNode() override = default;

    void accept(Visitor* visitor) override;

    void addChild(std::shared_ptr<AST> child) override;
    void removeChild(std::shared_ptr<AST> child) override;
    void removeAllChildren() override;

    void addParent(std::shared_ptr<AST> parent) override;
};

/**
 * @class StmtPackageNode
 * @brief Represents a statement node for defining a package in the abstract syntax tree (AST).
 *
 * The StmtPackageNode class is a specialized type of statement node that is used
 * to represent package declarations in the AST. This class provides mechanisms
 * for managing relationships with parent and child nodes, as well as enabling
 * visitor-based traversal through the visitor pattern.
 */
class StmtPackageNode final : public StatementNode {
public:
    StmtPackageNode() = default;
    ~StmtPackageNode() override = default;

    void accept(Visitor* visitor) override;

    void addChild(std::shared_ptr<AST> child) override;
    void removeChild(std::shared_ptr<AST> child) override;
    void removeAllChildren() override;

    void addParent(std::shared_ptr<AST> parent) override;
};

/**
 * @class StmtAsciiNode
 * @brief Represents a specific type of statement node in an abstract syntax tree (AST) that handles ASCII-related statements.
 *
 * The StmtAsciiNode class is a derived class from StatementNode and provides functionality
 * for accepting visitor objects, managing child and parent relationships, and maintaining
 * the hierarchical structure of the AST.
 *
 * Responsibilities:
 * - Implements the visitor pattern by overriding the accept method.
 * - Supports adding, removing, and managing child nodes.
 * - Supports assigning and managing the parent node.
 */
class StmtAsciiNode final : public StatementNode {
public:
    StmtAsciiNode() = default;
    ~StmtAsciiNode() override = default;

    void accept(Visitor* visitor) override;

    void addChild(std::shared_ptr<AST> child) override;
    void removeChild(std::shared_ptr<AST> child) override;
    void removeAllChildren() override;

    void addParent(std::shared_ptr<AST> parent) override;
};

/**
 * @class StmtOutputNode
 * @brief Represents an output statement node in the abstract syntax tree (AST).
 *
 * The StmtOutputNode class is a specific type of statement node used in the AST to depict
 * output-related operations. It inherits from the StatementNode base class and provides
 * functionality for interacting with its hierarchical relationships in the AST, such as
 * managing parent and child nodes. Additionally, it implements the accept method
 * to support the visitor pattern.
 */
class StmtOutputNode final : public StatementNode {
public:
    StmtOutputNode() = default;
    ~StmtOutputNode() override = default;

    void accept(Visitor *visitor) override;

    void addChild(std::shared_ptr<AST> child) override;
    void removeChild(std::shared_ptr<AST> child) override;
    void removeAllChildren() override;

    void addParent(std::shared_ptr<AST> parent) override;

    std::string name;
};

/**
 * @class StmtInputNode
 * @brief Represents an input statement node in the abstract syntax tree (AST).
 *
 * The StmtInputNode class is a concrete implementation of the StatementNode,
 * specifically designed for input statement representations in the AST.
 * It provides functionality to handle children and parent nodes, as well as
 * support visitor traversal via the visitor pattern.
 *
 * Methods in this class allow managing hierarchical relationships between
 * this node and other nodes in the AST, ensuring proper structure and organization.
 */
class StmtInputNode final : public StatementNode {
public:
    StmtInputNode() = default;
    ~StmtInputNode() override = default;

    void accept(Visitor *visitor) override;

    void addChild(std::shared_ptr<AST> child) override;
    void removeChild(std::shared_ptr<AST> child) override;
    void removeAllChildren() override;

    void addParent(std::shared_ptr<AST> parent) override;

    std::string name;
};

/**
 * @class StmtArrayNode
 * @brief Represents a statement node specifically designed to handle array-related operations in the AST.
 *
 * The StmtArrayNode class serves as a specialized type of StatementNode for managing
 * array constructs within an abstract syntax tree (AST). It provides functionality
 * for accepting visitors, managing parent-child relationships, and maintaining specific
 * properties such as a name for the array being represented. This class effectively
 * encapsulates behavior relevant to arrays as part of the AST structure.
 */
class StmtArrayNode final : public StatementNode {
public:
    StmtArrayNode() = default;
    ~StmtArrayNode() override = default;

    void accept(Visitor *visitor) override;

    void addChild(std::shared_ptr<AST> child) override;
    void removeChild(std::shared_ptr<AST> child) override;
    void removeAllChildren() override;

    void addParent(std::shared_ptr<AST> parent) override;

    std::string name;
};