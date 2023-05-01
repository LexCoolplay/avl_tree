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
    int height = 1;

    Node<T>* deleteMin();
    Node<T>* findMin();


    Node<T>* balance();
    void recalculateHeight();
    int diffHeight();
    Node<T>* rightRotate();
    Node<T>* leftRotate();
public:
    Node<T>* find(int node_id);

    Node(int nodeId, T Value);
    ~Node();

    Node<T>* addNode(Node<T>* addedNode);

    Node<T>* deleteNode(int node_id);

    T getValue();
    int getId();
    Node* right();
    Node* left();

    void serialize(ostream& os) const;
    Node* deserialize(istream& is);
};


#endif //AVL_TREE_NODE_H
