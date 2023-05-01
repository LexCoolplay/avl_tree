//
// Created by Алексей on 27.04.2023.
//

#include "AVLTree.h"
#include "Node.h"
#include <string>


template <typename T>
bool AVLTree<T>::addNode(Node<T>* node) {
    if (this->getNode(node->getId())) {
        return false;
    }
    root->addNode(node);
    this->nodeCount++;
    return true;
}

template <typename T>
bool AVLTree<T>::addNode(int node_id, T value) {
    Node<T>* addedNode = new Node<T>(node_id, value);
    return addNode(addedNode);
}

template <typename T>
bool AVLTree<T>::deleteNode(int node_id) {
    if (!this->getNode(node_id))
    {
        return false;
    }
    root->deleteNode(node_id);
    this->nodeCount--;
    return true;
}

template <>
AVLTree<string>* AVLTree<string>::deserialize(std::istream &is){
    string in_value;
    is >> in_value;
    int node_count = stoi(in_value);
    Node<string>* result_root;
    result_root->deserialize(is);
    AVLTree<string>* result_tree = new AVLTree<string>(result_root);
    for (int i = 0; i < node_count; ++i) {
        Node<string>* added_node;
        added_node->deserialize(is);
        result_tree->addNode(added_node);
    }
    delete this->root;
    this->root = result_root;
    return this;
}

template <typename T>
Node<T>* AVLTree<T>::getNode(int node_id) {
    return this->root->find(node_id);
}

template <typename T>
void AVLTree<T>::serialize(std::ostream &os) const {
    os >> this->nodeCount;
    this->root->serialize(os);
}

template <typename T>
AVLTree<T>::AVLTree(Node<T>* root) {
    this->root = root;
}

template <typename T>
AVLTree<T>::~AVLTree() {
    delete this->root;
}