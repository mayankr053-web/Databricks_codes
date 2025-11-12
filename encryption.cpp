#include <bits/stdc++.h>
using namespace std;

class File {
public:
    string name;
    bool isEncrypted;

    File(const string& name, bool isEncrypted = true)
        : name(name), isEncrypted(isEncrypted) {}
};

class Directory {
public:
    string name;
    unordered_map<string, Directory*> directories;
    unordered_map<string, File*> files;

    Directory(const string& name) : name(name) {}

    void addChild(Directory* child) {
        directories[child->name] = child;
    }

    void addChild(File* file) {
        files[file->name] = file;
    }

    // Count encrypted & unencrypted files in the whole subtree
    pair<int, int> countFiles() {
        int encrypted = 0, unencrypted = 0;
        for (auto& [_, f] : files) {
            if (f->isEncrypted) encrypted++;
            else unencrypted++;
        }
        for (auto& [_, d] : directories) {
            auto [e, u] = d->countFiles();
            encrypted += e;
            unencrypted += u;
        }
        return {encrypted, unencrypted};
    }

private:
    // --- Helper 1: count all files in this directory recursively
    int countTotalFiles() {
        int total = (int)files.size();
        for (auto& [_, dir] : directories) {
            total += dir->countTotalFiles();
        }
        return total;
    }

    // --- Helper 2: compute cost of encrypting this directory as a whole
    long long getEncryptDirectoryCost(long long requestTime, long long fileEncryptTime) {
        int totalFiles = countTotalFiles();
        return requestTime + totalFiles * fileEncryptTime;
    }

    // --- Helper 3: compute cost of encrypting children (dirs + unencrypted files)
    long long getEncryptChildrenCost(long long requestTime, long long fileEncryptTime) {
        long long cost = 0;

        // Encrypt all unencrypted files individually
        for (auto& [_, f] : files) {
            if (!f->isEncrypted)
                cost += requestTime + fileEncryptTime;
        }

        // Recurse into subdirectories (each decides its own optimal way)
        for (auto& [_, d] : directories) {
            cost += d->getMinimumEncryptTime(requestTime, fileEncryptTime);
        }

        return cost;
    }

public:
    // --- Main Function: choose optimal strategy
    long long getMinimumEncryptTime(long long requestTime, long long fileEncryptTime) {
        long long encryptWholeDir = getEncryptDirectoryCost(requestTime, fileEncryptTime);
        long long encryptChildren = getEncryptChildrenCost(requestTime, fileEncryptTime);
        return min(encryptWholeDir, encryptChildren);
    }
};

int main() {
    // Example directory structure:
    // /
    // ├── dir1
    // │    ├── file1 => encrypted
    // │    └── file2 => unencrypted
    // └── dir2
    //      ├── file1 => unencrypted
    //      └── file2 => encrypted

    Directory* root = new Directory("/");
    Directory* dir1 = new Directory("dir1");
    Directory* dir2 = new Directory("dir2");

    dir1->addChild(new File("file1", true));
    dir1->addChild(new File("file2", false));

    dir2->addChild(new File("file1", false));
    dir2->addChild(new File("file2", true));

    root->addChild(dir1);
    root->addChild(dir2);

    auto [enc, unenc] = root->countFiles();
    cout << "Encrypted: " << enc << ", Unencrypted: " << unenc << "\n\n";

    // Test 1
    cout << "Test 1 => "
         << root->getMinimumEncryptTime(3, 1) << "\n"; // Expect 7

    // Test 2
    cout << "Test 2 => "
         << root->getMinimumEncryptTime(2, 2) << "\n"; // Expect 8

    // cleanup omitted for brevity
    return 0;
}
