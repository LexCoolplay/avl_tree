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
    AVLTree(long long node_id, char* value);
    ~AVLTree();

    bool addNode(Node<T>* addedNode);
    bool addNode(int node_id, T value);

    Node<T>* getNode(ull hash);
    bool deleteNode(ull hash);

    void serialize(ostream& os) const;
    AVLTree* deserialize(istream& is);
private:
    Node<T>* root;
    int nodeCount = 1;
};


#endif //AVL_TREE_AVLTREE_H
