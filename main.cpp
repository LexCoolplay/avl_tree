#include <iostream>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <string>

// Node.h

//
// Created by Алексей on 27.04.2023.
//

#ifndef AVL_TREE_NODE_H
#define AVL_TREE_NODE_H

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

// end Node.h

// Node.cpp

//
// Created by Алексей on 27.04.2023.
//

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
    if (target_hash < this->hash) {
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
        this->leftChild = this->left()->deleteNode(target_hash);
        return this->balance();
    }
    if (target_hash > this->hash) {
        this->rightChild= this->right()->deleteNode(target_hash);
        return this->balance();
    }
    // Попали в ноду
    Node<T>* left_child = this->left();
    Node<T>* right_child = this->right();
    if (!right_child) {
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

// end Node.cpp

// AVLTree.h

//
// Created by Алексей on 27.04.2023.
//

#ifndef AVL_TREE_AVLTREE_H
#define AVL_TREE_AVLTREE_H

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

// end AVLTree.h

// AVLTree.cpp

//
// Created by Алексей on 27.04.2023.
//

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

// end AVLTree.cpp

// main

enum cmd_commands{
    ADD,
    DELETE,
    GET,
    SERIALIZE,
    DESERIALIZE,
    NONE
};

ull string_hash(char* buffer) {
    // polynomial rolling хэш-функция
    ull hash = 0;
    for (int index = 0; buffer[index] != 0; index++)
    {
        hash *= 31;
        hash += buffer[index];
    }
    return hash;
}

cmd_commands resolve_cmd(char* cmd) {
    char command[256] = "";
    char subcommand[16] = "";
    sscanf(cmd, "%s", command);
    if (strcmp("+", command) == 0) {
        return ADD;
    }
    if (strcmp("-", command) == 0) {
        return DELETE;
    }
    if (strcmp("!", command) == 0) {
        sscanf(cmd, "%s %s", command, subcommand, 16, 16);
        if (strcmp(subcommand, "Load") == 0) {
            return DESERIALIZE;
        }
        if (strcmp(subcommand, "Save") == 0) {
            return SERIALIZE;
        }
        else {
            return NONE;
        }
    }
    return GET;
}

int main() {
    char buffer[1024] = "";
    char* cmd = (char*) calloc(16, sizeof(char));
    char* arg = nullptr;
    char* second_arg = nullptr;
    AVLTree<char*>* working_tree = nullptr;
    AVLTree<char*>* working_tree_copy = nullptr;
    ofstream serialize_output;
    ifstream deserialize_input;

    while (gets(buffer)) {
        switch (resolve_cmd(buffer)) {
            case ADD:
                arg = (char*) calloc(256, sizeof(char));
                second_arg = (char*) calloc(1024, sizeof(char));
                sscanf(buffer, "%s %s %s", cmd, arg, second_arg);
                if (working_tree == nullptr) {
                    working_tree = new AVLTree<char *>(atoll(second_arg), arg);
                    cout << "OK" << endl;
                } else {
                    auto *addedNode = new Node<char *>(atoll(second_arg), arg);
                    bool result = working_tree->addNode(addedNode);
                    if (result) {
                        cout << "OK" << endl;
                    }
                    else {
                        cout << "Exists" << endl;
                    }
                }
                delete second_arg;
                break;
            case DELETE:
                if (working_tree == nullptr) {
                    cout << "NoSuchWord\n";
                } else {
                    arg = new char[256];
                    sscanf(buffer, "%s %s", cmd, arg);
                    bool result = working_tree->deleteNode(string_hash(arg));
                    delete arg;
                    if (result) {
                        cout << "OK" << endl;
                    } else {
                        cout << "NoSuchWord" << endl;
                    }
                    delete arg;
                }
                break;
            case GET:
                if (working_tree == nullptr) {
                    cout << "NoSuchWord\n";
                } else {
                    arg = new char[256];
                    sscanf(buffer, "%s", arg);
                    Node<char *> *result = working_tree->getNode(string_hash(arg));
                    if (result == nullptr) {
                        cout << "NoSuchWord" << endl;
                    } else {
                        cout << "OK: " << result->getId() << endl;
                    }
                    delete arg;
                }
                break;
            case SERIALIZE:
                arg = (char*) calloc(256, sizeof(char));
                second_arg = (char*) calloc(1024, sizeof(char));
                sscanf(buffer, "%s %s %s", cmd, arg, second_arg);
                serialize_output.open(second_arg, ios::out | ios::binary);
                if (working_tree == nullptr) {
                    serialize_output << 0;
                    serialize_output << separator;
                    serialize_output.close();
                }
                else {
                    working_tree->serialize(serialize_output);
                    serialize_output.flush();
                    serialize_output.close();
                }
                cout << "OK" << endl;
                delete arg;
                delete second_arg;
                break;
            case DESERIALIZE:
                if (working_tree == nullptr) {
                    char* dummy_string = (char*) "a";
                    working_tree = new AVLTree<char *>(0, dummy_string);
                    working_tree_copy = nullptr;
                }
                else {
                    working_tree_copy = working_tree;
                }
                arg = (char*) calloc(256, sizeof(char));
                second_arg = (char*) calloc(1024, sizeof(char));
                sscanf(buffer, "%s %s %s", cmd, arg, second_arg);
                deserialize_input.open(second_arg, ios::in | ios::binary);
                working_tree = working_tree->deserialize(deserialize_input);
                if (working_tree == nullptr) {
                    cout << "ERROR: Serialization Failed!" << endl;
                    working_tree = working_tree_copy;
                }
                deserialize_input.close();
                delete working_tree_copy;
                break;
            case NONE:
                cout << "ERROR: No such command!" << endl;
        }
    }
    delete working_tree;
    return 0;
}
