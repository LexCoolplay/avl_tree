//
// Created by Алексей on 27.04.2023.
//

#include "Node.h"

const char separator = ';';

template <typename T>
Node<T>* Node<T>::get(int node_id) {
    Node<T>* resultNode;
    if (this->id == node_id)
    {
        return this;
    }
    resultNode = rightChild->get(node_id);
    if (resultNode == nullptr)
    {
        return rightChild->get(node_id);
    }
}

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
void Node<T>::addNode(Node<T> *addedNode) {

}

template <typename T>
bool Node<T>::deleteNode(int node_id) {}

template <typename T>
void Node<T>::balance() {}