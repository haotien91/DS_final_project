#include <iostream>
#include <vector>

using namespace std;

struct Node {
    string sub = "";
    vector<int> child;

    Node() {};

    Node(const string& sub, initializer_list<int> children) : sub(sub) {
        child.insert(child.end(), children);
    }
};

struct SuffixTree {
    vector<Node> nodes;

    SuffixTree() {
        // Empty
    };

    void NewSuffix(const string& str) {
        nodes.push_back(Node{});
        for (int i = 0; i<str.length(); i++) {
            addSuffix(str.substr(i));
        }
    }

private:
    void addSuffix(const string& suf) {
        int n = 0;
        int i = 0;
        while (i < suf.length()) {
            char b = suf[i];
            int x2 = 0;
            int n2;
            while(1) {
                auto children = nodes[n].child;
                if (x2 == children.size()) {
                    // no matching child, remainder become new node
                    n2 = nodes.size();
                    nodes.push_back(Node(suf.substr(i), {}));
                    nodes[n].child.push_back(n2);
                    return;
                }
                n2 = children[x2];
                if (nodes[n2].sub[0] == b) {
                    break;
                }
                x2++;
            }
            auto sub2 = nodes[n2].sub;
            int j = 0;
            while (j < sub2.size()) {
                if (suf[i+j] != sub2[j]) {
                    // split n2
                    auto n3 = n2;
                    // new node for the part in common
                    n2 = nodes.size();
                    nodes.push_back(Node(sub2.substr(0, j), {n3}));
                    nodes[n3].sub = sub2.substr(j); // old node loses the part in common
                    nodes[n].child[x2] = n2;
                    break; // continue down
                }
                j++;
            }
            i += j; // advance past part in common
            n = n2; // continue down
        }

    }

};


int search_parse();

