////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             Albert Pistorius                               //
//                                 (60603)                                    //
//                     ADSA: Assignment 2 - BST/AVL Tree                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

//Node Structure: each node contains data, it's height value and the address' to it's children nodes
struct Node {
    int data;
    int height;
    struct Node *left, *right;
};

//Function used to create a node and assign it the integer given as input
Node * CreateNode(int data) {
    Node * temp = new Node();
    temp->data = data;
    temp->left = nullptr;           //Has zero children when first initialized
    temp->right = nullptr;
    temp->height = 1;
    return temp;
}

//Prints the AVL tree in Preorder
void PrintPreOrder(Node *node) {
    cout << node->data;
    if (node->left != nullptr) {
        cout << " ";
        PrintPreOrder(node->left);
    }
    if (node->right != nullptr) {
        cout << " ";
        PrintPreOrder(node->right);
    }
}

//Prints the AVL tree in Postorder
void PrintPostOrder(Node *node) {
    if (node->left != nullptr) {
        PrintPostOrder(node->left);
        cout << " ";
    }
    if (node->right != nullptr) {
        PrintPostOrder(node->right);
        cout << " ";
    }
    cout << node->data;
}

//Prints the AVL tree in Order
void PrintInOrder(Node *node) {
    if (node->left != nullptr) {
        PrintInOrder(node->left);
    }
    cout << node->data << " ";
    if (node->right != nullptr) {
        PrintInOrder(node->right);
    }
}

//Helper function to retrieve a nodes height value
int Height(Node *node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

//Helper Function used to update a node and its parents height values
//Used after inserting/removing a node
int UpdateHeight(Node *node) {
    if ((node->left != nullptr) && (node->right != nullptr))                    //Node has two children nodes
        return 1 + max(Height(node->left), Height(node->right));
    else if ((node->left == nullptr) || (node->right == nullptr))               //Node only has one child node
        return 1 + ((node->left) ? Height(node->left) : Height(node->right));
    else
        return 1;                                                               //Node has no children nodes
}

//Function used to retrieve the balance of a node
//The balance is the difference between the nodes child nodes height values
int GetBalance(Node *node) {
    if (node == nullptr)
        return 0;
    return Height(node->left) - Height(node->right);
}

//Function to rotate the node given as input, and its child nodes, to the right
Node * RotateRight(Node *node) {
    Node *x = node->left;
    Node *y = x->right;
    x->right = node;
    node->left = y;

    node->height = UpdateHeight(node);
    x->height = UpdateHeight(x);

    return x;
}

//Function to rotate the node input and its child nodes left
Node * RotateLeft(Node *node) {
    Node *x = node->right;
    Node *y = x->left;
    x->left = node;
    node->right = y;

    node->height = UpdateHeight(node);
    x->height = UpdateHeight(x);

    return x;
}

//Function used to insert a node containing the data input
Node * InsertNode(Node *node, int data) {
    if (node == nullptr)                                                        //Node location has been found and is created
        return CreateNode(data);
    else {
        if (data < node->data)                                                  //Travel left if data is smaller than the current node being evaluated
            node->left = InsertNode(node->left, data);
        else if (data > node->data)                                             //Travel right if data is larger than the current node being evaluated
            node->right = InsertNode(node->right, data);
    }

    //Update height value and get balance
    node->height = UpdateHeight(node);
    int bal = GetBalance(node);

    //Right Options:
    if (bal < -1) {
        //Right-Right
        if (data > node->right->data)
            return RotateLeft(node);
        //Right-Left
        else if (data < node->right->data) {
            node->right = RotateRight(node->right);
            return RotateLeft(node);
        }
    }
    //Left Options
    else if (bal > 1) {
        //Left-Left
        if (data < node->left->data)
            return RotateRight(node);
        //Left-Right
        if (data > node->left->data) {
            node->left = RotateLeft(node->left);
            return RotateRight(node);
        }
    }
    return node;
}

//Function used to return the maximum node, starting from the node given as input
Node * FindMaxValue(Node *node) {
    if (node->right == nullptr)
        return node;
    return FindMaxValue(node->right);
}

//Function used to remove a node containing containing the data given as input
Node * RemoveNode(Node *node, int data) {
    if (node == nullptr)
        return node;

    if (data < node->data)                                                      //Travel left if data is smaller than the current node being evaluated
        node->left = RemoveNode(node->left, data);
    else if (data > node->data)                                                 //Travel right if data is larger than the current node being evaluated
        node->right = RemoveNode(node->right, data);
    else {                                                                      //Node has been found and will be deleted
        if ((node->left == nullptr) && (node->right == nullptr)) {                  //Case for if node has no children
            node = NULL;
            free(node);
        } else if ((node->left != nullptr) && (node->right != nullptr)) {           //Case for if node has two children
            Node *temp = FindMaxValue(node->left);
            node->data = temp->data;
            node->left = RemoveNode(node->left, temp->data);
        } else {                                                                    //Case for if node has one child
            Node *temp = node;
            if (node->left != nullptr)                                                  //If child is the left child
                node = (node->left);
            else if (node->right != nullptr)                                            //If child is the right child
                node = (node->right);
            free(temp);
        }
    }

    if (node == nullptr)
        return node;

    //Update height value and get balance
    node->height = UpdateHeight(node);
    int bal = GetBalance(node);

    //Left Options
    if (bal > 1) {
        //Left-Left
        if (GetBalance(node->left) >= 0)
            return RotateRight(node);
        //Left-Right
        else {
            node->left = RotateLeft(node->left);
            return RotateRight(node);
        }
    }
    //Right Options
    else if (bal < -1) {
        //Right-Right
        if (GetBalance(node->right) <= 0)
            return RotateLeft(node);
        //Left-Left
        else {
            node->right = RotateRight(node->right);
            return RotateLeft(node);
        }
    }
    return node;
}


int main() {

    string input, strdata;
    stringstream inputstream;
    getline(cin, input);
    inputstream << input;
    string command;
    int data;

    Node * root = nullptr;              //Initialize AVL tree root node

    while(!inputstream.eof()) {
        inputstream >> command;

        //Handles commands like A or D, for inserting/removing nodes
        if ((command[0] == 'A') || (command[0] == 'D')) {
            strdata = command.substr(1);
            if(stringstream(strdata) >> data) {
                root = (command[0] == 'A') ? InsertNode(root, data) : RemoveNode(root, data);
            }
        //Handles printing the tree
        } else {
            if (root == nullptr)
                cout << "EMPTY";
            //POST and PRE order prints
            else if (command[0] == 'P')
                command[1] == 'O' ? PrintPostOrder(root) : PrintPreOrder(root);
            //IN order print
            else if (command[0] == 'I')
                PrintInOrder(root);

            cout << "\n";
        }
        command = "";
    }

}
