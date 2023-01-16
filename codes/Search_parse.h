#include <iostream>
#include <vector>

using namespace std;

struct Node {
    string sub = "";
    vector<int> child;
    int type;

    Node() {
        type = 2;
    };

    Node(const string& sub, initializer_list<int> children, int type) : sub(sub), type(type) {
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
        // 對Exact設下標記
        ExactaddSuffix(str);

        for (int i = 1; i<str.length(); i++) {
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
                    // 一般的node標記為 type = 0
                    nodes.push_back(Node(suf.substr(i), {}, 0));
                    nodes[n].child.push_back(n2);
                    return;
                }
                n2 = children[x2];
                // find node[n2] has common initial alphabet
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
                    // 將兩者相同部分做為新的Node
                    n2 = nodes.size();

                    // 若該node可到達Exact的結尾, 則標記為type = 2
                    if (nodes[n3].type == 1 || nodes[n3].type == 2)
                        nodes.push_back(Node(sub2.substr(0, j), {n3}, 2));
                    else
                        nodes.push_back(Node(sub2.substr(0, j), {n3}, 0));
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

    void ExactaddSuffix(const string& suf) {
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
                    nodes.push_back(Node(suf.substr(i), {}, 1));
                    nodes[n].child.push_back(n2);
                    return;
                }
                n2 = children[x2];

                if (nodes[n2].sub == suf.substr(i)) {
                    nodes[n2].type = 1;
                    return;
                }

                // find node[n2] has common initial alphabet
                if (nodes[n2].sub[0] == b) {
                    // nodes[n2].type = 2;
                    break;
                }
                x2++;
            }
            auto sub2 = nodes[n2].sub;
            int j = 0;

            // nodes[n].type = 2;

            while (j < sub2.size()) {
                if (suf[i+j] != sub2[j]) {
                    // split n2
                    auto n3 = n2;
                    // 將兩者相同部分做為新的Node
                    n2 = nodes.size();
                    // 前往Exact_end的path標記為 type = 2
                    nodes.push_back(Node(sub2.substr(0, j), {n3}, 2));
                    nodes[n3].sub = sub2.substr(j); // old node loses the part in common
                    nodes[n].child[x2] = n2;
                    break; // continue down
                }
                j++;
            }
            nodes[n2].type = 2;
            i += j; // advance past part in common
            n = n2; // continue down
        }
    }



};