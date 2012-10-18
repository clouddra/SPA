#include <vector>

class Node {

public:
    Node(int type, int v);
    int getNodeType();
    int getValue();
    int getParent();
    int getLeft();
    int getRight();
    std::vector<int> getChildren();
    void setParent(int p);
    void setLeft(int l);
    void setRight(int r);
    void addChild(int c);

private:
    int nodeType; // enum
    int value;    // index of some table or null depending on nodeType
    // Links are index of AST
    int parent;
    int leftSibling;
    int rightSibling;
    std::vector<int> children;
};