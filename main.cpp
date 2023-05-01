#include <iostream>
#include <cstring>
#include <cstdio>
#include <fstream>
#include "Node.cpp"
#include "AVLTree.cpp"

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
    sscanf_s(cmd, "%s", command, 16);
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
    ofstream serialize_output = nullptr;
    ifstream deserialize_input = nullptr;

    while (gets(buffer)) {
        switch (resolve_cmd(buffer)) {
            case ADD:
                arg = (char*) calloc(256, sizeof(char));
                second_arg = (char*) calloc(1024, sizeof(char));
                sscanf_s(buffer, "%s %s %s", cmd, 16, arg, 256, second_arg, 1024);
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
                    sscanf_s(buffer, "%s %s", cmd, 16, arg, 256);
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
                    sscanf_s(buffer, "%s", arg, 256);
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
                sscanf_s(buffer, "%s %s %s", cmd, 16, arg, 256, second_arg, 1024);
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
                sscanf_s(buffer, "%s %s %s", cmd, 16, arg, 256, second_arg, 1024);
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
