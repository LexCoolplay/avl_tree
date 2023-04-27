//
// Created by Алексей on 27.04.2023.
//

#ifndef AVL_TREE_NODE_H
#define AVL_TREE_NODE_H

#include <string>

using namespace std;

template <typename T = string>
class Node {
private:
    Node* leftChild;
    Node* rightChild;
    int nodeId;
    T value;
    int height;
public:
    Node* get(int node_id);

    Node(int nodeId, T Value);
    ~Node();

    void addNode(Node<T>* addedNode);
    void balance();
    bool deleteNode(int node_id);

    T getValue();
    Node* right();
    Node* left();

    void serialize(ostream& os) const;
    Node* deserialize(istream& is);
};


#endif //AVL_TREE_NODE_H
