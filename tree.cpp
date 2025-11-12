#include <bits/stdc++.h>
using namespace std;

class TreeNode {
public:
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr), parent(nullptr) {}
};

void printPreorder(TreeNode* root);
void printInorder(TreeNode* root);
void printPostorder(TreeNode* root);

int main() {
    // Example tree:
    //        1
    //       / \
    //      2   3
    //     / \
    //    4   5

    TreeNode* root = new TreeNode(1);
    TreeNode* n2 = new TreeNode(2);
    TreeNode* n3 = new TreeNode(3);
    TreeNode* n4 = new TreeNode(4);
    TreeNode* n5 = new TreeNode(5);

    root->left = n2; root->right = n3;
    n2->parent = root; n3->parent = root;
    n2->left = n4; n2->right = n5;
    n4->parent = n2; n5->parent = n2;

    cout << "Preorder: ";
    printPreorder(root);
    cout << "\nInorder: ";
    printInorder(root);
    cout << "\nPostorder: ";
    printPostorder(root);
    cout << endl;
}

void printPreorder(TreeNode* root) {
    if (!root) return;
    TreeNode* prev = nullptr;
    TreeNode* curr = root;
    TreeNode* next = nullptr;

    while (curr) {
        if (prev == curr->parent) {
            cout << curr->val << " ";
            if (curr->left)
                next = curr->left;
            else if (curr->right)
                next = curr->right;
            else
                next = curr->parent;
        } else if (prev == curr->left) {
            if (curr->right)
                next = curr->right;
            else
                next = curr->parent;
        } else {
            next = curr->parent;
        }
        prev = curr;
        curr = next;
    }
}

void printInorder(TreeNode* root) {
    if (!root) return;
    TreeNode* prev = nullptr;
    TreeNode* curr = root;
    TreeNode* next = nullptr;

    while (curr) {
        if (prev == curr->parent) {
            if (curr->left)
                next = curr->left;
            else {
                cout << curr->val << " ";
                next = curr->right ? curr->right : curr->parent;
            }
        } else if (prev == curr->left) {
            cout << curr->val << " ";
            next = curr->right ? curr->right : curr->parent;
        } else {
            next = curr->parent;
        }
        prev = curr;
        curr = next;
    }
}

void printPostorder(TreeNode* root) {
    if (!root) return;
    TreeNode* prev = nullptr;
    TreeNode* curr = root;
    TreeNode* next = nullptr;

    while (curr) {
        if (prev == curr->parent) {
            if (curr->left)
                next = curr->left;
            else if (curr->right)
                next = curr->right;
            else {
                cout << curr->val << " ";
                next = curr->parent;
            }
        } else if (prev == curr->left) {
            if (curr->right)
                next = curr->right;
            else {
                cout << curr->val << " ";
                next = curr->parent;
            }
        } else {
            cout << curr->val << " ";
            next = curr->parent;
        }
        prev = curr;
        curr = next;
    }
}
