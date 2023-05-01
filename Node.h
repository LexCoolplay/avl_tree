//
// Created by Алексей on 27.04.2023.
//

#ifndef AVL_TREE_NODE_H
#define AVL_TREE_NODE_H

#include <string>

using namespace std;

typedef unsigned long long ull;

template <typename T = string>
class Node {
private:
    Node* leftChild = nullptr;
    Node* rightChild = nullptr;
    long long nodeId;
    T value;
    int height = 1;

    Node<T>* deleteMin();
    Node<T>* findMin();

    ull hash = 0;

    Node<T>* balance();
    void recalculateHeight();
    int diffHeight();
    Node<T>* rightRotate();
    Node<T>* leftRotate();
public:
    Node<T>* find(ull hash);

    Node(int nodeId, T Value);
    ~Node();

    Node<T>* addNode(Node<T>* addedNode);

    Node<T>* deleteNode(ull hash);

    T getValue();
    int getId();
    Node* right();
    Node* left();
    ull getHash();

    void serialize(ostream& os) const;
    Node* deserialize(istream& is);
};


#endif //AVL_TREE_NODE_H
