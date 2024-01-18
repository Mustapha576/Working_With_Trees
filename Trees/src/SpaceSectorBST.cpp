#include "SpaceSectorBST.h"

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}


void DestroyRecursive(Sector* node)
{
    if (node)
    {
        DestroyRecursive(node->left);
        DestroyRecursive(node->right);
        delete node;
    }
}

SpaceSectorBST::~SpaceSectorBST() {
    // Free any dynamically allocated memory in this class.
    DestroyRecursive(root);
}

Sector* insertRecursive(Sector* currentNode, int x, int y, int z) {
    // If the current node is nullptr, create a new node and return it as the new leaf.
    if (currentNode == nullptr) {
        return new Sector(x, y, z);
    }

    else{
        // Compare the coordinates to decide whether to go left or right.
        if (x < currentNode->x) {
            // Insert to the left subtree.
            currentNode->left = insertRecursive(currentNode->left, x, y, z);
        }
        else if(x == currentNode->x){
            if(y < currentNode->y){
                currentNode->left = insertRecursive(currentNode->left, x, y, z);
            }
            else if(y == currentNode->y){
                if(z < currentNode->z){
                    currentNode->left = insertRecursive(currentNode->left, x, y, z);
                }
                else if(z > currentNode->z){
                    currentNode->right = insertRecursive(currentNode->right, x, y, z);;
                }
            }
            else if(y > currentNode->y){
                currentNode->right = insertRecursive(currentNode->right, x, y, z);;
            }
        }

        else if(x > currentNode->x){
            // Insert to the right subtree.
            currentNode->right = insertRecursive(currentNode->right, x, y, z);
        }
    }
    // Return the current node after the insertion.
    return currentNode;
}

void inorder(Sector* t) {
    if(t == nullptr)
        return;
    inorder(t->left);
    cout << t->sector_code << endl;
    inorder(t->right);
}

void preorder(Sector* t){
    if(t == nullptr)
        return;
    cout << t->sector_code << endl;
    preorder(t->left);
    preorder(t->right);
}

void postorder(Sector* t){
    if(t == nullptr)
        return;
    postorder(t->left);
    postorder(t->right);
    cout << t->sector_code << endl;
}

void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the BST sector map
    // according to the given comparison critera based on the sector coordinates.
    fstream file;
    file.open(filename, ios::in);
    if(file.is_open()){
        string line;
        int count = 0;
        while(getline(file, line)){
            string x, y, z;
            int values = 0;

            if(count == 0){
                count++;
                continue;
            }
            else if(count == 1){
                for(char i : line){
                    if(i == ','){values++;}
                    else{
                        if(values == 0){x += i;}
                        else if(values == 1){y += i;}
                        else{z += i;}
                    }
                }
                //insertRecursive(root, stoi(x), stoi(y), stoi(z));
                insertSectorByCoordinates(stoi(x), stoi(y), stoi(z));
            }
        }
    }

}

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    // Instantiate and insert a new sector into the space sector BST map according to the
    // coordinates-based comparison criteria.
    root = insertRecursive(root, x, y, z);
}

Sector* findMin(Sector* t)
{
    if(t == nullptr)
        return nullptr;
    else if(t->left == nullptr)
        return t;
    else
        return findMin(t->left);
}

Sector* deleteNode(string sector_code, Sector* node){
    Sector* temp;
    if(sector_code != node->sector_code){
        if(node->left == nullptr && node->right == nullptr){
        }
        else if(node->left == nullptr && node->right != nullptr){
            node->right = deleteNode(sector_code, node->right);
        }
        else if(node->left != nullptr && node->right == nullptr){
            node->left = deleteNode(sector_code, node->left);
        }
        else{
            node->right = deleteNode(sector_code, node->right);
            node->left = deleteNode(sector_code, node->left);
        }

    }
    else if(node->left && node->right){
        temp = findMin(node->right);
        node->sector_code = temp->sector_code;
        node->distance_from_earth = temp->distance_from_earth;
        node->x = temp->x;
        node->y = temp->y;
        node->z = temp->z;
        node->right = deleteNode(node->sector_code, node->right);
    }
    else{
        temp = node;
        if(node->left == nullptr){
            node = node->right;
        }
        else if(node->right == nullptr){
            node = node->left;
        }
        delete temp;
    }
    return node;
}


void SpaceSectorBST::deleteSector(const std::string& sector_code) {
    // TODO: Delete the sector given by its sector_code from the BST.
    root = deleteNode(sector_code, root);
}

void SpaceSectorBST::displaySectorsInOrder() {
    // TODO: Traverse the space sector BST map in-order and print the sectors
    // to STDOUT in the given format.
    cout << "Space sectors inorder traversal:" << endl;
    inorder(root);
    cout << endl;
}

void SpaceSectorBST::displaySectorsPreOrder() {
    // TODO: Traverse the space sector BST map in pre-order traversal and print
    // the sectors to STDOUT in the given format.
    cout << "Space sectors preorder traversal:" << endl;
    preorder(root);
    cout << endl;
}

void SpaceSectorBST::displaySectorsPostOrder() {
    // TODO: Traverse the space sector BST map in post-order traversal and print
    // the sectors to STDOUT in the given format.
    cout << "Space sectors postorder traversal:" << endl;
    postorder(root);
    cout << endl;
}

bool hasPath(Sector *root, vector<Sector*>& arr, string code)
{
    if (!root)
        return false;

    arr.push_back(root);

    if (root->sector_code == code)
        return true;

    if (hasPath(root->left, arr, code) ||
        hasPath(root->right, arr, code))
        return true;

    arr.pop_back();
    return false;
}

std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    hasPath(root, path, sector_code);
    return path;
}

void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function
    // to STDOUT in the given format.
    if(path.size() != 0){
        cout << "The stellar path to Dr. Elara: ";
        for(int i = 0; i < path.size(); i++){
            if(i == path.size() - 1){
                cout << path[i]->sector_code;
            }
            else{
                cout << path[i]->sector_code + "->";
            }
        }
        cout << endl;
    }
    else{
        cout << "A path to Dr. Elara could not be found." << endl;
        cout << endl;
    }

}