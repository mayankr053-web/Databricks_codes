#include <bits/stdc++.h>
using namespace std;

struct Node {
    char val;
    int size;
    Node *left, *right;
    Node(char c) : val(c), size(1), left(nullptr), right(nullptr) {}
};

int getSize(Node* n) { return n ? n->size : 0; }
void update(Node* n) { if (n) n->size = 1 + getSize(n->left) + getSize(n->right); }

class NewString {
    Node* root = nullptr;

public:
    void insert(char c, int index) { root = insertNode(root, c, index); }
    void deleteAt(int index) { root = deleteNode(root, index); }
    char read(int index) { return readNode(root, index); }

    string getString() {
        string out;
        inorder(root, out);
        return out;
    }

private:
    Node* insertNode(Node* root, char c, int index) {
        if (!root) return new Node(c);
        int leftSize = getSize(root->left);
        if (index <= leftSize)
            root->left = insertNode(root->left, c, index);
        else
            root->right = insertNode(root->right, c, index - leftSize - 1);
        update(root);
        return root;
    }

    Node* deleteNode(Node* root, int index) {
        if (!root) return nullptr;
        int leftSize = getSize(root->left);
        if (index < leftSize)
            root->left = deleteNode(root->left, index);
        else if (index > leftSize)
            root->right = deleteNode(root->right, index - leftSize - 1);
        else {
            if (!root->left) { Node* r = root->right; delete root; return r; }
            if (!root->right) { Node* l = root->left; delete root; return l; }
            // replace with in-order successor
            Node* succ = root->right;
            while (succ->left) succ = succ->left;
            root->val = succ->val;
            root->right = deleteNode(root->right, 0);
        }
        update(root);
        return root;
    }

    char readNode(Node* root, int index) {
        int leftSize = getSize(root->left);
        if (index == leftSize) return root->val;
        if (index < leftSize) return readNode(root->left, index);
        return readNode(root->right, index - leftSize - 1);
    }

    void inorder(Node* root, string& out) {
        if (!root) return;
        inorder(root->left, out);
        out.push_back(root->val);
        inorder(root->right, out);
    }
};
