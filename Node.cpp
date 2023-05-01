//
// Created by Алексей on 27.04.2023.
//

#include "Node.h"

const char separator = ' ';

template <typename T>
T Node<T>::getValue() {
    return this->value;
}

template <typename T>
Node<T>::Node(int nodeId, T value) {
    this->nodeId = nodeId;
    this->value = value;
    this->hash = nodeId;
}

template <>
Node<char*>::Node(int nodeId, char* value) {
    this->nodeId = nodeId;
    this->value = value;

    // polynomial rolling хэш-функция
    for (int index = 0; value[index] != 0; index++)
    {
        this->hash *= 31;
        this->hash += value[index];
    }
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
Node<T>* Node<T>::find(ull target_hash) {
    if (target_hash == this->hash) {
        return this;
    }
    if (target_hash < this->nodeId) {
        if (this->left() == nullptr) return nullptr;
        return this->left()->find(target_hash);
    }
    if (this->right() == nullptr) return nullptr;
    return this->right()->find(target_hash);
}

template<>
void Node<char*>::serialize(std::ostream &os) const {
    os << to_string(nodeId);
    os << separator;
    os << value;
    os << separator;
    if (leftChild != nullptr) {
        leftChild->serialize(os);
    }
    if (rightChild != nullptr) {
        rightChild->serialize(os);
    }
}

template<>
Node<char*>* Node<char*>::deserialize(std::istream &is){
    char* in_value = (char*) malloc(sizeof (char) * 256);
    is >> in_value;
    int node_id = std::atoi(in_value);
    is >> in_value;
    return new Node<char*>(node_id, in_value);
}

template<typename T>
Node<T>* Node<T>::addNode(Node<T> *addedNode) {
    if (addedNode->hash < this->hash) {
        this->left() == nullptr? this->leftChild = addedNode : this->leftChild = this->left()->addNode(addedNode);
    }
    else {
        this->right() == nullptr? this->rightChild = addedNode : this->rightChild = this->right()->addNode(addedNode);
    }
    Node<T>* result_node = this->balance();
    return result_node;
}

template<typename T>
ull Node<T>::getHash() {
    return this->hash;
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
    this->rightChild = child->leftChild;
    child->leftChild = this;
    this->recalculateHeight();
    child->recalculateHeight();
    return child;
}

template<typename T>
Node<T>* Node<T>::rightRotate() {
    Node<T>* child = this->left();
    this->leftChild = child->rightChild;
    child->rightChild = this;
    this->recalculateHeight();
    child->recalculateHeight();
    return child;
}

template <typename T>
Node<T>* Node<T>::deleteNode(ull target_hash) {
    if(target_hash < this->hash) {
        this->left()->deleteNode(target_hash);
        return this->balance();
    }
    if (target_hash > this->hash) {
        this->right()->deleteNode(target_hash);
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
           this->rightChild = this->right()->rightRotate();
       }
        return this->leftRotate();
    }

    if (this->diffHeight() == -2) {
        if (this->left()->diffHeight() > 0) {
            this->leftChild = this->left()->leftRotate();
        }
        return this->rightRotate();
    }

    // если в балансе
    return this;
}