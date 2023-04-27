//
// Created by Алексей on 27.04.2023.
//

#ifndef AVL_TREE_AVLTREE_H
#define AVL_TREE_AVLTREE_H

#include <iostream>
#include "Node.h"

using namespace std;

template <typename T>
class AVLTree {
public:
    AVLTree(Node<T>* root);
    ~AVLTree();

    void addNode(Node<T>* addedNode);
    void addNode(int node_id, T value);

    Node<T>* getNode(int node_id);
    void deleteNode(int node_id);

    void serialize(ostream& os) const;
    AVLTree* deserialize(istream& is);
private:
    Node<T>* root;
    void rebalance();
    int nodeCount = 0;
};


#endif //AVL_TREE_AVLTREE_H
