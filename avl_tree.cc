#include <iostream>
#include <algorithm>
#include <queue>
#include <cassert>

using namespace std;

// Node structure for the AVL Tree
struct Node {
    int data;
    Node *left;
    Node *right;
    int height;

    Node(int val) : data(val), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    Node *root;

    // Helper function to get the height of a node
    int height(Node *N) {
        if (N == nullptr)
            return 0;
        return N->height;
    }

    // Helper function to update the height of a node
    void updateHeight(Node* N) {
        if (N != nullptr) {
            N->height = 1 + max(height(N->left), height(N->right));
        }
    }

    // Get balance factor of a node N
    int getBalance(Node *N) {
        if (N == nullptr)
            return 0;
        return height(N->left) - height(N->right);
    }

    // Right rotate subtree rooted with y
    //       y                               x
    //      / \                             /   \
    //     x   T3  -> Right Rotate (y) ->  T1    y
    //    / \                                   / \
    //   T1  T2                                T2  T3
    // returns x, the new root of the subtree
    Node *rightRotate(Node *y) {
        Node *x = y->left;
        Node *T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    // Left rotate subtree rooted with x
    //     x                               y
    //    / \                             /   \
    //   T1  y   -> Left Rotate (x) ->   x     T3
    //      / \                         / \
    //     T2  T3                      T1  T2
    // returns y, the new root of the subtree
    Node *leftRotate(Node *x) {
        Node *y = x->right;
        Node *T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    // Recursive function to insert a key in the subtree rooted
    // with node and returns the new root of the subtree.
    Node* insertNode(Node* node, int key) {
        if (node == nullptr)
            return(new Node(key));

        if (key < node->data)
            node->left = insertNode(node->left, key);
        else if (key > node->data)
            node->right = insertNode(node->right, key);
        else
            return node;

        updateHeight(node);

        int balance = getBalance(node);

        if (balance > 1 && key < node->left->data)
            return rightRotate(node);

        if (balance < -1 && key > node->right->data)
            return leftRotate(node);

        if (balance > 1 && key > node->left->data) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && key < node->right->data) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // Helper function to find the node with minimum value
    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
    }

    // Recursive function to delete a node with given key
    // from subtree with given root. It returns root of
    // the modified subtree.
    Node* deleteNode(Node* root, int key) {
        if (root == nullptr)
            return root;

        if (key < root->data)
            root->left = deleteNode(root->left, key);
        else if (key > root->data)
            root->right = deleteNode(root->right, key);
        else {
            if ((root->left == nullptr) || (root->right == nullptr)) {
                Node *temp = root->left ? root->left : root->right;

                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                } else {
                    *root = *temp; // Copy the contents of the non-empty child
                }
                delete temp;
            } else {
                Node* temp = minValueNode(root->right);

                root->data = temp->data;

                root->right = deleteNode(root->right, temp->data);
            }
        }

        if (root == nullptr)
            return root;

        updateHeight(root);

        int balance = getBalance(root);

        if (balance > 1 && getBalance(root->left) >= 0)
            return rightRotate(root);

        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);

        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    // Recursive helper for destroying the tree (freeing memory)
    void destroyRecursive(Node* node) {
        if (node) {
            destroyRecursive(node->left);
            destroyRecursive(node->right);
            delete node;
        }
    }

    // Recursive helper for searching a key
    Node* searchNodeUtil(Node* node, int key) {
        if (node == nullptr || node->data == key) {
            return node;
        }
        if (key < node->data) {
            return searchNodeUtil(node->left, key);
        }
        return searchNodeUtil(node->right, key);
    }

public:
    /**
     * @brief Constructs an empty AVLTree.
     * @note Space Complexity: O(n), where n is the number of nodes in the tree.
     */
    AVLTree() : root(nullptr) {}

    /**
     * @brief Destroys the AVLTree, freeing all allocated memory.
     */
    ~AVLTree() {
        destroyRecursive(root);
        root = nullptr;
    }

    /**
     * @brief Inserts a new key into the AVL tree.
     * @param key The integer key to insert.
     * @note Time Complexity: O(log n) due to tree traversal and rebalancing operations.
     */
    void insert(int key) {
        root = insertNode(root, key);
    }

    /**
     * @brief Removes a key from the AVL tree.
     * @param key The integer key to remove.
     * @note Time Complexity: O(log n) due to tree traversal and rebalancing operations.
     */
    void remove(int key) {
        root = deleteNode(root, key);
    }

    /**
     * @brief Searches for a key in the AVL tree.
     * @param key The integer key to search for.
     * @return True if the key is found, false otherwise.
     * @note Time Complexity: O(log n) due to tree traversal.
     */
    bool search(int key) {
        return searchNodeUtil(root, key) != nullptr;
    }
};

void runAssertTests() {
    cout << "Running AVLTree Assert Tests..." << endl;

    // Test 1: Empty Tree
    AVLTree tree1;
    assert(!tree1.search(10));
    cout << "Test 1 (Empty Tree) PASSED" << endl;

    // Test 2: Single Insertion
    AVLTree tree2;
    tree2.insert(10);
    assert(tree2.search(10));
    assert(!tree2.search(20));
    cout << "Test 2 (Single Insertion) PASSED" << endl;

    // Test 3: Multiple Insertions (no initial rotation)
    AVLTree tree3;
    tree3.insert(10);
    tree3.insert(5);
    tree3.insert(15);
    assert(tree3.search(5) && tree3.search(10) && tree3.search(15));
    cout << "Test 3 (Multiple Insertions - No Rotation) PASSED" << endl;

    // Test 4: Insertions causing rotations
    // LL Case
    AVLTree ll_tree;
    ll_tree.insert(30);
    ll_tree.insert(20);
    ll_tree.insert(10);
    assert(ll_tree.search(10) && ll_tree.search(20) && ll_tree.search(30));

    // RR Case
    AVLTree rr_tree;
    rr_tree.insert(10);
    rr_tree.insert(20);
    rr_tree.insert(30);
    assert(rr_tree.search(10) && rr_tree.search(20) && rr_tree.search(30));

    // LR Case
    AVLTree lr_tree;
    lr_tree.insert(30);
    lr_tree.insert(10);
    lr_tree.insert(20);
    assert(lr_tree.search(10) && lr_tree.search(20) && lr_tree.search(30));

    // RL Case
    AVLTree rl_tree;
    rl_tree.insert(10);
    rl_tree.insert(30);
    rl_tree.insert(20);
    assert(rl_tree.search(10) && rl_tree.search(20) && rl_tree.search(30));
    cout << "Test 4 (Rotations on Insert) PASSED" << endl;

    // Test 5: Deletion
    AVLTree tree_del;
    tree_del.insert(10); tree_del.insert(5); tree_del.insert(15); tree_del.insert(3); tree_del.insert(7); tree_del.insert(12); tree_del.insert(17);
    
    tree_del.remove(3);
    assert(!tree_del.search(3) && tree_del.search(5));
    
    tree_del.remove(5);
    assert(!tree_del.search(5) && tree_del.search(7) && tree_del.search(10));

    tree_del.remove(10);
    assert(!tree_del.search(10) && tree_del.search(12) && tree_del.search(15));
    cout << "Test 5 (Deletions - Leaf, One Child, Two Children) PASSED" << endl;

    // Test 6: Deletion causing rebalance
    AVLTree tree_rebal_del;
    tree_rebal_del.insert(20); tree_rebal_del.insert(10); tree_rebal_del.insert(30); tree_rebal_del.insert(5);
    tree_rebal_del.remove(30);
    assert(!tree_rebal_del.search(30) && tree_rebal_del.search(5) && tree_rebal_del.search(10) && tree_rebal_del.search(20));
    cout << "Test 6 (Deletion Causing Rebalance) PASSED" << endl;

    // Test 7: Deleting non-existent element
    AVLTree tree_non_exist;
    tree_non_exist.insert(10);
    tree_non_exist.remove(100);
    assert(tree_non_exist.search(10));
    cout << "Test 7 (Deleting Non-Existent) PASSED" << endl;

    cout << "All AVLTree assert tests completed successfully!" << endl;
}

void runAVLTreeSample() {
    cout << "\n--- Manual Demonstration ---" << endl;
    AVLTree tree;

    cout << "Inserting elements..." << endl;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(25);

    cout << "\nSearching for 25: " << (tree.search(25) ? "Found" : "Not Found") << endl;
    cout << "Searching for 100: " << (tree.search(100) ? "Found" : "Not Found") << endl;


    cout << "\nDeleting 10 (leaf):" << endl;
    tree.remove(10);
}


int main() {
    runAssertTests();
    runAVLTreeSample();
    return 0;
}