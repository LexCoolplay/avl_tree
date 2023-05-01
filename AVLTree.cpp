//
// Created by Алексей on 27.04.2023.
//

#include "AVLTree.h"
#include "Node.h"
#include <string>


template <typename T>
bool AVLTree<T>::addNode(Node<T>* node) {
    if (this->getNode(node->getHash())) {
        return false;
    }
    root = root->addNode(node);
    this->nodeCount++;
    return true;
}

template <typename T>
bool AVLTree<T>::addNode(int node_id, T value) {
    Node<T>* addedNode = new Node<T>(node_id, value);
    return addNode(addedNode);
}

template <typename T>
bool AVLTree<T>::deleteNode(ull hash) {
    if (!this->getNode(hash))
    {
        return false;
    }
    root->deleteNode(hash);
    this->nodeCount--;
    return true;
}

template <>
AVLTree<char*>* AVLTree<char*>::deserialize(std::istream &is){
    char* in_value = (char*) malloc(sizeof(char) * 256);
    is >> in_value;
    int node_count = stoi(in_value);
    auto* result_root = new Node<char*>(0, in_value);
    result_root = result_root->deserialize(is);
    auto* result_tree = new AVLTree<char*>(result_root);
    bool result;
    for (int i = 1; i < node_count; ++i) {
        auto* added_node = new Node<char*>(0, in_value);
        added_node = added_node->deserialize(is);
        result = result_tree->addNode(added_node);
        if (!result) {
            return nullptr;
        }
    }
    this->root = result_root;
    this->nodeCount = node_count;
    return this;
}

template <typename T>
Node<T>* AVLTree<T>::getNode(ull hash) {
    return this->root->find(hash);
}

template <typename T>
void AVLTree<T>::serialize(std::ostream &os) const {
    os << to_string(this->nodeCount);
    os << separator;
    this->root->serialize(os);
}

template <typename T>
AVLTree<T>::AVLTree(Node<T>* root) {
    this->root = root;
}

template <>
AVLTree<char*>::AVLTree(long long node_id, char* value) {
    this->root = new Node<char*>(node_id, value);
}

template <typename T>
AVLTree<T>::~AVLTree() {
    delete this->root;
}