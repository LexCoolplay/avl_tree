//
// Created by Алексей on 27.04.2023.
//

#include "Node.h"

const char separator = ';';

template <typename T>
T Node<T>::getValue() {
    return this->value;
}

template <typename T>
Node<T>::Node(int nodeId, T value) {
    this->nodeId = nodeId;
    this->value  = value;
}
template <typename T>
Node<T>::~Node() {
    delete value;
}

template <typename T>
Node<T>* Node<T>::left() {
    return this->leftChild;
}

template <typename T>
Node<T>* Node<T>::right() {
    return this->rightChild;
}

template <typename T>
int Node<T>::getId() {
    return this->nodeId;
}

template <typename T>
Node<T>* Node<T>::find(int node_id) {
    if (node_id == this->nodeId) {
        return this;
    }
    if (node_id < this->nodeId) {
        if (!this->left()) return nullptr;
        return this->left()->find(node_id);
    }
    if (!this->right()) return nullptr;
    return this->right()->find(node_id);
}

template<>
void Node<string>::serialize(std::ostream &os) const {
    os << to_string(nodeId);
    os << value;
    if (leftChild != nullptr) {
        leftChild->serialize(os);
    }
    if (rightChild != nullptr) {
        leftChild->serialize(os);
    }
}

template<>
Node<string>* Node<string>::deserialize(std::istream &is){
    string in_value;
    is >> in_value;
    this->nodeId = std::stoi(in_value);
    is >> in_value;
    this->value = in_value;
    return this;
}

template<typename T>
Node<T>* Node<T>::addNode(Node<T> *addedNode) {
    if (addedNode->nodeId < this->nodeId) {
        this->left() == nullptr? this->leftChild = addedNode : this->left()->addNode(addedNode);
    }
    else {
        this->right() == nullptr? this->rightChild = addedNode : this->right()->addNode(addedNode);
    }
    return this->balance();
}

template<typename T>
Node<T>* Node<T>::deleteMin() {
    if (!this->left()) {
        this->right();
    }
    this->leftChild = this->left()->deleteMin();
    return this->balance();
}

template<typename T>
Node<T>* Node<T>::findMin() {
    if (this->left()) {
        return this->left();
    }
    return this;
}

template<typename T>
void Node<T>::recalculateHeight() {
    int left_height = 0, right_height = 0;
    if (this->left()) {
        left_height = this->left()->height;
    }
    if (this->right()) {
        right_height = this->right()->height;
    }
    this->height= right_height > left_height? right_height + 1 : left_height + 1;
}

template<typename T>
int Node<T>::diffHeight() {
    int left_height = 0, right_height = 0;
    if (this->left()) {
        left_height = this->left()->height;
    }
    if (this->right()) {
        right_height = this->right()->height;
    }
    return right_height - left_height;
}

template<typename T>
Node<T>* Node<T>::leftRotate() {
    Node<T>* child = this->right();
    this->leftChild = child->leftChild;
    child->leftChild = this;
    this->recalculateHeight();
    child->recalculateHeight();
    return child;
}

template<typename T>
Node<T>* Node<T>::rightRotate() {
    Node<T>* child = this->left();
    this->rightChild = child->leftChild;
    child->rightChild = this;
    this->recalculateHeight();
    child->recalculateHeight();
    return child;
}

template <typename T>
Node<T>* Node<T>::deleteNode(int node_id) {
    if(node_id < this->value) {
        this->left()->deleteNode(node_id);
        return this->balance();
    }
    if (node_id > this->value) {
        this->right()->deleteNode(node_id);
        return this->balance();
    }
    // Попали в ноду
    Node<T>* left_child = this->left();
    Node<T>* right_child = this->right();
    if (!right_child) {
        delete this;
        return left_child;
    }
    Node<T>* min = right_child->findMin();
    min->rightChild = right_child->deleteMin();
    min->leftChild = left_child;
    return min->balance();
}

template <typename T>
Node<T>* Node<T>::balance() {
    this->recalculateHeight();
    if (this->diffHeight() == 2) {
       if (this->right()->diffHeight() < 0) {
           this->right() = this->right()->rightRotate();
       }
        return this->leftRotate();
    }

    if (this->diffHeight() == -2) {
        if (this->left()->diffHeight() > 0) {
            this->left()= this->left()->leftRotate();
        }
        return this->rightRotate();
    }

    // если в балансе
    return this;
}