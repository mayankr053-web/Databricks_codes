#include <iostream>
#include <cstdlib>

struct Node {
    char c;
    int totalCount;   // size of subtree
    Node* left;
    Node* right;

    Node(char ch)
        : c(ch), totalCount(1), left(nullptr), right(nullptr) {}
};

int size(Node* n) { return n ? n->totalCount : 0; }

void update(Node* n) {
    if (n) n->totalCount = 1 + size(n->left) + size(n->right);
}

// Insert character c at index pos
Node* insert(Node* root, char c, int pos) {
    if (!root) return new Node(c);

    int leftSize = size(root->left);
    if (pos <= leftSize) {
        root->left = insert(root->left, c, pos);
    } else {
        root->right = insert(root->right, c, pos - leftSize - 1);
    }
    update(root);
    return root;
}

// Find k-th character (0-indexed)
char read(Node* root, int k) {
    if (!root) return '\0';
    int leftSize = size(root->left);
    if (k < leftSize)
        return read(root->left, k);
    else if (k == leftSize)
        return root->c;
    else
        return read(root->right, k - leftSize - 1);
}

// Delete character at index pos
Node* deleteAt(Node* root, int pos) {
    if (!root) return nullptr;

    int leftSize = size(root->left);
    if (pos < leftSize)
        root->left = deleteAt(root->left, pos);
    else if (pos > leftSize)
        root->right = deleteAt(root->right, pos - leftSize - 1);
    else {
        // delete current node
        if (!root->left) {
            Node* r = root->right;
            delete root;
            return r;
        } else if (!root->right) {
            Node* l = root->left;
            delete root;
            return l;
        } else {
            // replace with inorder successor
            Node* cur = root->right;
            while (cur->left)
                cur = cur->left;
            root->c = cur->c;
            root->right = deleteAt(root->right, 0); // delete successor
        }
    }
    update(root);
    return root;
}

void inorder(Node* root) {
    if (!root) return;
    inorder(root->left);
    std::cout << root->c;
    inorder(root->right);
}

// ---------------- Example Usage ----------------
int main() {
    Node* root = nullptr;
    std::string s = "Hello";
    for (int i = 0; i < s.size(); i++)
        root = insert(root, s[i], i);

    inorder(root); // Hello
    std::cout << "\n";

    root = insert(root, 'X', 2);
    inorder(root); // HeXllo
    std::cout << "\n";

    root = deleteAt(root, 3);
    inorder(root); // HeXlo
    std::cout << "\n";

    std::cout << "Char at 2: " << read(root, 2) << "\n"; // X
}
