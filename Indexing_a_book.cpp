#include <bits/stdc++.h>
using namespace std;
class HybridNode {
public:
    string key;   
    string element;   
    HybridNode* parent;   
    HybridNode* left_child;  
    HybridNode* right_child;  
    HybridNode* next_node; 
    string color = "black"; 

    HybridNode(string key_val, string element_val) : key(key_val), element(element_val), parent(nullptr), left_child(nullptr), right_child(nullptr), next_node(nullptr) {}
};
class RedBlackTree {
private:
    HybridNode* root;   
    HybridNode* deleteNode(HybridNode* node, string key);
    

public:
     RedBlackTree() : root(nullptr) {}
     
     HybridNode* getRoot() {
        return root;  
    }

    void setRoot(HybridNode* node) {
        root = node; 
    }

    void rotateLeft(HybridNode* node) {
        HybridNode* right = node->right_child;
        node->right_child = right->left_child;

        if (right->left_child) right->left_child->parent = node;
        right->parent = node->parent;

        if (!node->parent) root = right;
        else if (node == node->parent->left_child) node->parent->left_child = right;
        else node->parent->right_child = right;

        right->left_child = node;
        node->parent = right;
    }

    void rotateRight(HybridNode* node) {
        HybridNode* left = node->left_child;
        node->left_child = left->right_child;

        if (left->right_child) left->right_child->parent = node;
        left->parent = node->parent;

        if (!node->parent) root = left;
        else if (node == node->parent->right_child) node->parent->right_child = left;
        else node->parent->left_child = left;

        left->right_child = node;
        node->parent = left;
    }
void fixViolations(HybridNode* &node) {
    HybridNode* parent = nullptr;
    HybridNode* grandparent = nullptr;

    for (; (node != root) && (node->color != "black") && (node->parent->color == "red");) {
        parent = node->parent;
        grandparent = parent->parent;

       
        switch (parent == grandparent->left_child) {
            case true: {
                HybridNode* uncle = grandparent->right_child;

                if (uncle && uncle->color == "red") {
                    grandparent->color = "red";
                    parent->color = "black";
                    uncle->color = "black";
                    node = grandparent;
                    break;  
                }
                
                if (node == parent->right_child) {
                    rotateLeft(parent);
                    node = parent;
                    parent = node->parent;
                }

                rotateRight(grandparent);
                swap(parent->color, grandparent->color);
                node = parent;
                break;
            }
            case false: {
                HybridNode* uncle = grandparent->left_child;

                if (uncle && uncle->color == "red") {
                    grandparent->color = "red";
                    parent->color = "black";
                    uncle->color = "black";
                    node = grandparent;
                    break;
                }

                if (node == parent->left_child) {
                    rotateRight(parent);
                    node = parent;
                    parent = node->parent;
                }

                rotateLeft(grandparent);
                swap(parent->color, grandparent->color);
                node = parent;
                break;
            }
        }
    }

    root->color = "black";
}

    HybridNode* insert(HybridNode* root, HybridNode* node) {
        if (!root) return node;

        if (node->key < root->key) {
            root->left_child = insert(root->left_child, node);
            root->left_child->parent = root;
        } else if (node->key > root->key) {
            root->right_child = insert(root->right_child, node);
            root->right_child->parent = root;
        }

        return root;
    }

public:
    void insertValue(string key, string element) {
        HybridNode* node = new HybridNode(key, element);
        node->color = "red";
        root = insert(root, node);
        fixViolations(node);
    }

vector<HybridNode*> traverseUp(HybridNode* node) {
    vector<HybridNode*> path;
    for(; node; node = node->parent) {
        path.push_back(node);
    }
    return path;
}

vector<HybridNode*> traverseDown(HybridNode* node, string bit_sequence) {
    vector<HybridNode*> path;
    for (char bit : bit_sequence) {
        if (!node) break;  
        path.push_back(node);
        if (bit == '0') {
            node = node->left_child;
        } else if (bit == '1') {
            node = node->right_child;
        }
    }
    return path;
}



void preOrderHelper(HybridNode* node, vector<HybridNode*>& nodes, int depth) {
    if (!node || depth < 0) return; 
    nodes.push_back(node);
    preOrderHelper(node->left_child, nodes, depth - 1);  
    preOrderHelper(node->right_child, nodes, depth - 1); 
}

vector<HybridNode*> preOrderTraversal(HybridNode* node, int depth) {
    vector<HybridNode*> nodes;
    preOrderHelper(node, nodes, depth);
    return nodes;
}




HybridNode* search(string key) {
    HybridNode* node = root;
    while (node) {
        if (key == node->key) return node;
        else if (key < node->key) node = node->left_child;
        else node = node->right_child;
    }
    return nullptr;
}

    
int blackheight(HybridNode* node) {
    int height = 0;
    while (node) {
        if (node->color == "black") height++;
        node = node->left_child;
    }
    return height;
}
void updateElement(string key, string element) {
    HybridNode* node = search(key);
    if (node) {
        node->element = element; 
    }
}
void inOrderHelper(HybridNode* node, vector<HybridNode*>& nodes) {
    if (!node) return;
    inOrderHelper(node->left_child, nodes);
    nodes.push_back(node);
    inOrderHelper(node->right_child, nodes);
}

vector<HybridNode*> inOrderTraversal() {
    vector<HybridNode*> nodes;
    inOrderHelper(root, nodes);
    return nodes;
}
bool isRed(HybridNode* node) {
    if (!node) return false;
    return (node->color == "red");
}

HybridNode* leftRotate(HybridNode* node) {
    HybridNode* rightChild = node->right_child;
    node->right_child = rightChild->left_child;
    rightChild->left_child = node;
    rightChild->color = node->color;
    node->color = "red";
    return rightChild;
}

HybridNode* rightRotate(HybridNode* node) {
    HybridNode* leftChild = node->left_child;
    node->left_child = leftChild->right_child;
    leftChild->right_child = node;
    leftChild->color = node->color;
    node->color = "red";
    return leftChild;
}

HybridNode* moveRedLeft(HybridNode* node) {
    node->color = "black";
    node->left_child->color = "red";
    node->right_child->color = "red";
    if (isRed(node->right_child->left_child)) {
        node->right_child = rightRotate(node->right_child);
        node = leftRotate(node);
        node->color = "red";
        node->left_child->color = "black";
    }
    return node;
}

HybridNode* moveRedRight(HybridNode* node) {
    node->color = "black";
    node->left_child->color = "red";
    node->right_child->color = "red";
    if (isRed(node->left_child->left_child)) {
        node = rightRotate(node);
        node->color = "red";
        node->right_child->color = "black";
    }
    return node;
}

HybridNode* fixUp(HybridNode* node) {
    if (isRed(node->right_child))
        node = leftRotate(node);
    if (isRed(node->left_child) && isRed(node->left_child->left_child))
        node = rightRotate(node);
    if (isRed(node->left_child) && isRed(node->right_child)) {
        node->left_child->color = "black";
        node->right_child->color = "black";
        node->color = "red";
    }
    return node;
}

HybridNode* deleteMin(HybridNode* node) {
    if (!node->left_child)
        return nullptr;
    if (!isRed(node->left_child) && !isRed(node->left_child->left_child))
        node = moveRedLeft(node);
    node->left_child = deleteMin(node->left_child);
    return fixUp(node);
}

HybridNode* getMin(HybridNode* node) {
    while (node->left_child)
        node = node->left_child;
    return node;
}

bool deleteNode(string key) {
        if (!root) return false;
        HybridNode* oldRoot = root; 
        root = deleteNode(root, key);
        if (root) root->color = "black";
        return oldRoot != root;  
    }



};
HybridNode* RedBlackTree::deleteNode(HybridNode* node, string key) {
    if (key < node->key) {
        if (!isRed(node->left_child) && !isRed(node->left_child->left_child))
            node = moveRedLeft(node);
        node->left_child = deleteNode(node->left_child, key);
    } else {
        if (isRed(node->left_child))
            node = rightRotate(node);
        if (key == node->key && !(node->right_child))
            return nullptr;
        if (!isRed(node->right_child) && !isRed(node->right_child->left_child))
            node = moveRedRight(node);
        if (key == node->key) {
            node->key = getMin(node->right_child)->key;
            node->right_child = deleteMin(node->right_child);
        } else
            node->right_child = deleteNode(node->right_child, key);
    }
    return fixUp(node);
}

class IndexEntry {
private:
    string word;
    vector<pair<string, int>> chapter_word_counts;  
public:
    IndexEntry(string word_val) : word(word_val) {}

    void setWord(const string& word_val) {  
        word = word_val;
    }

    string getWord() const { 
        return word;
    }

    
    void addChapterWordCount(const string& chapter, int count) {
        chapter_word_counts.push_back({chapter, count});
    }

    void setChapterWordCounts(const vector<pair<string, int>>& chapter_word_counts_val) {  
        chapter_word_counts = chapter_word_counts_val;
    }

    vector<pair<string, int>> getChapterWordCounts() const {  
        return chapter_word_counts;
    }


};






class Lexicon {
private:
    RedBlackTree red_black_tree;
    vector<string> chapters_list;


public:
    Lexicon() {}

    void setRedBlackTree(const RedBlackTree& tree) {
        red_black_tree = tree;
    }

    RedBlackTree getRedBlackTree() const {
        return red_black_tree;
    }
    
    void getChapterList() {
        ifstream inputfile("chapterNames.txt");

        if (!inputfile.is_open()) {
            cerr << "Unable to open file" << endl;
            return;
        }

        string line;
        while (getline(inputfile, line)) {
            istringstream iss(line);
            string word;

            while (iss >> word) {
                chapters_list.push_back(word);
               
            }
        }
    }

    vector<string> getChapters() const {
        return chapters_list;
    }

vector<pair<string, int>> getWordsFromChapter(const string& chapter_name) {
   
    unordered_map<string, int> word_count;

 
    ifstream chapter_file(chapter_name);

   
    if (!chapter_file) {
        cerr << "Error opening file: " << chapter_name << endl;
        return {};
    }

    string word;
    while (chapter_file >> word) {
  
        transform(word.begin(), word.end(), word.begin(), ::tolower);

        word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());

        word_count[word]++;
    }

    chapter_file.close();

    vector<pair<string, int>> result(word_count.begin(), word_count.end());
    return result;
}


void readChapters(const vector<string>& chapter_names) {
    for (const string& chapter_name : chapter_names) {
        vector<pair<string, int>> wordCounts = getWordsFromChapter(chapter_name);
        
        for (const auto& [word, count] : wordCounts) {
            HybridNode* node = red_black_tree.search(word);
            if (node) {
       
                red_black_tree.updateElement(word, chapter_name);
        
            } else {
       
                red_black_tree.insertValue(word, chapter_name);
            }
        }
    }
}

    vector<IndexEntry> buildIndex() {
        vector<IndexEntry> index;
        HybridNode* root = red_black_tree.getRoot();

       
        if (root) {
            IndexEntry entry(root->key);
            index.push_back(entry);
        }

        return index;
    }
};
class MRUCache {
private:
    list<pair<string, IndexEntry>> Cche;  
    unordered_map<string, list<pair<string, IndexEntry>>::iterator> map;  
    int capacity;

public:
    MRUCache(int d) : capacity(d) {}

  
    IndexEntry get(string word) {
        if (map.find(word) == map.end()) return IndexEntry("");  

  
        Cche.splice(Cche.begin(), Cche, map[word]);
        return map[word]->second;
    }

   
    void put(string word, IndexEntry value) {
        if (map.find(word) != map.end()) {
        
            Cche.erase(map[word]);
        } else if (Cche.size() >= capacity) {
            
            string last = Cche.back().first;
            Cche.pop_back();
            map.erase(last);
        }

        Cche.push_front({word, value});
        map[word] = Cche.begin();
    }
};
