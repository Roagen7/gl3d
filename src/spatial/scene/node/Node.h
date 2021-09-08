//
// Created by roagen on 08.09.2021.
//

#ifndef UNTITLED4_NODE_H
#define UNTITLED4_NODE_H


#include "../../../object/Mesh.h"

class Node {
public:
    std::vector<Node*> children;
    std::vector<Mesh*> meshes;
};


#endif //UNTITLED4_NODE_H
