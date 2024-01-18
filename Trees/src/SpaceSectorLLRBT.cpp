#include "SpaceSectorLLRBT.h"

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

int isRed(Sector *node)
{
    if (node == nullptr)
        return 0;

    return node->color;
}

Sector* rotateLeft(Sector* node)
{
    Sector *child = node -> right;
    Sector *childLeft = child -> left;

    child -> left = node;
    node -> right = childLeft;

    return child;
}

Sector* rotateRight(Sector* node)
{
    Sector *child = node -> left;
    Sector *childRight =  child -> right;

    child -> right = node;
    node -> left = childRight;

    return child;
}

void swapColors(Sector *node1, Sector *node2)
{
    bool temp = node1 -> color;
    node1 -> color = node2 -> color;
    node2 -> color = temp;
}


Sector* insert(Sector* currentNode, int x, int y, int z)
{

    if (currentNode == nullptr)
        return new Sector(x, y, z);

    if (x < currentNode->x) {
        currentNode->left = insert(currentNode->left, x, y, z);
    }
    else if(x == currentNode->x){
        if(y < currentNode->y){
            currentNode->left = insert(currentNode->left, x, y, z);
        }
        else if(y == currentNode->y){
            if(z < currentNode->z){
                currentNode->left = insert(currentNode->left, x, y, z);
            }
            else if(z > currentNode->z){
                currentNode->right = insert(currentNode->right, x, y, z);;
            }
        }
        else if(y > currentNode->y){
            currentNode->right = insert(currentNode->right, x, y, z);;
        }
    }

    else if(x > currentNode->x){
        currentNode->right = insert(currentNode->right, x, y, z);
    }

    else{
        return currentNode;
    }

    if (isRed(currentNode -> right) &&
        !isRed(currentNode -> left))
    {
        currentNode = rotateLeft(currentNode);
        swapColors(currentNode, currentNode -> left);
    }
    if (isRed(currentNode -> left) &&
        isRed(currentNode -> left -> left))
    {
        currentNode = rotateRight(currentNode);
        swapColors(currentNode, currentNode -> right);
    }

    if (isRed(currentNode -> left) && isRed(currentNode -> right))
    {
        currentNode -> color = !currentNode -> color;
        currentNode -> left -> color = false;
        currentNode -> right -> color = false;
    }
    return currentNode;
}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
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
                insertSectorByCoordinates(stoi(x), stoi(y), stoi(z));
            }
        }
    }
}

void DestroyRecursiveLLRBT(Sector* node)
{
    if (node)
    {
        DestroyRecursiveLLRBT(node->left);
        DestroyRecursiveLLRBT(node->right);
        delete node;
    }
}


// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    // TODO: Free any dynamically allocated memory in this class.
    DestroyRecursiveLLRBT(root);
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map 
    // according to the coordinates-based comparison criteria.
    root = insert(root, x, y, z);
}

void Inorder(Sector *node)
{
    if(node == nullptr)
        return;
    Inorder(node -> left);
    if(!node->color)
        cout << "BLACK sector: " + node->sector_code << endl;
    else
        cout << "RED sector: " + node->sector_code << endl;
    Inorder(node -> right);
}


void SpaceSectorLLRBT::displaySectorsInOrder() {
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors 
    // to STDOUT in the given format.
    cout << "Space sectors inorder traversal:" << endl;
    Inorder(root);
    cout << endl;
}

void Preorder(Sector *node)
{
    if(node == nullptr)
        return;
    if(!node->color)
        cout << "BLACK sector: " + node->sector_code << endl;
    else
        cout << "RED sector: " + node->sector_code << endl;
    Preorder(node -> left);
    Preorder(node -> right);
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors preorder traversal:" << endl;
    Preorder(root);
    cout << endl;
}

void Postorder(Sector *node)
{
    if(node == nullptr)
        return;
    Postorder(node -> left);
    Postorder(node -> right);
    if(!node->color)
        cout << "BLACK sector: " + node->sector_code << endl;
    else
        cout << "RED sector: " + node->sector_code << endl;
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors postorder traversal:" << endl;
    Postorder(root);
    cout << endl;
}


bool yesPath(Sector *root, vector<Sector*>& arr, string code)
{
    if (!root)
        return false;

    arr.push_back(root);

    if (root->sector_code == code)
        return true;

    if (yesPath(root->left, arr, code) ||
        yesPath(root->right, arr, code))
        return true;

    arr.pop_back();
    return false;
}


std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    std::vector<Sector*> path_earth;
    std::vector<Sector*> path_node;
    yesPath(root, path_earth, "0SSS");
    yesPath(root, path_node, sector_code);
    int count = 0;

    if(path_node.size() == 0){}
    else{
        //cout << "Earth path" << endl;
        for(int i = 0; i < path_earth.size(); i++){
            for(int j = 0; j < path_node.size(); j++){
                if(path_earth[i] == path_node[j])
                    count++;
            }
            //cout << path_earth[i]->sector_code << endl;
        }
        int size = path_earth.size() - 1;
        for(; count - 1 < size; size--){
            path.push_back(path_earth[size]);
        }

        int size_node = path_node.size();
        for(; count <= size_node; count++){
            path.push_back(path_node[count - 1]);
        }
        /*
        cout << "Node path" << endl;
        for(int i = 0; i < path_node.size(); i++){
            cout << path_node[i]->sector_code << endl;
        }
         */
    }
    return path;
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
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
    }

}