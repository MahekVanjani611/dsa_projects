#include<bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
using namespace std;

class MetroStop;
class MetroLine;
class AVLNode;

class MetroStop {
private:
    std::string stopName;
    MetroStop* nextStop;
    MetroStop* prevStop;
    MetroLine* line;
    int fare;

public:
    MetroStop(std::string name, MetroLine* metroLine, int fare);
    std::string getStopName() const;
    MetroStop* getNextStop() const;
    MetroStop* getPrevStop() const;
    MetroLine* getLine() const;
    int getFare() const;

    void setNextStop(MetroStop* next);
    void setPrevStop(MetroStop* prev);
};

MetroStop::MetroStop(std::string name, MetroLine* metroLine, int fare) {
    stopName = name;
    nextStop = nullptr;
    prevStop = nullptr;
    line = metroLine;
    this->fare = fare;
}

std::string MetroStop::getStopName() const {
    return stopName;
}

MetroStop* MetroStop::getNextStop() const {
    return nextStop;
}

MetroStop* MetroStop::getPrevStop() const {
    return prevStop;
}

MetroLine* MetroStop::getLine() const {
    return line;
}

int MetroStop::getFare() const {
    return fare;
}

void MetroStop::setNextStop(MetroStop* next) {
    nextStop = next;
}

void MetroStop::setPrevStop(MetroStop* prev) {
    prevStop = prev;
}

class MetroLine {
private:
    std::string lineName;
    MetroStop* node;

public:
    MetroLine(std::string name);

    std::string getLineName() const;
    MetroStop* getNode() const;

    void setNode(MetroStop* node);

    void populateLine(std::string filename);

    void printLine() const;
    int getTotalStops() const;
};

MetroLine::MetroLine(std::string name) {
    lineName = name;
    node = nullptr;
}

std::string MetroLine::getLineName() const {
    return lineName;
}

MetroStop* MetroLine::getNode() const {
    return node;
}

void MetroLine::setNode(MetroStop* node) {
    this->node = node;
}

void MetroLine::printLine() const {
    for (MetroStop* stop = node; stop != nullptr; stop = stop->getNextStop()) {
        cout << stop->getStopName() << endl;
    }
}

int MetroLine::getTotalStops() const {
    int ctr = 0;
    for (MetroStop* stop = node; stop != nullptr; stop = stop->getNextStop()) {
        ctr++;
    }
    return ctr;
}

void MetroLine::populateLine(std::string filename) {

     ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string stopName;
        int fare;
        ss >> stopName >> fare;
        // cout<<"stop name: "<<stopName<< " ,Fare: "<<fare<<endl;
        MetroStop* metroStop = new MetroStop(stopName, this, fare);

        if (node == nullptr) {
            node = metroStop;
        } else {
            MetroStop* current = node;
            while (current->getNextStop() != nullptr) {
                current = current->getNextStop();
            }
            current->setNextStop(metroStop);
            metroStop->setPrevStop(current);
        }
    }
file.close();
}

class AVLNode {
private:
    std::string stopName;
    std::vector<MetroStop*> stops;
    AVLNode* left;
    AVLNode* right;
    AVLNode* parent;

public:
    AVLNode(std::string name);

    std::string getStopName() const;
    const std::vector<MetroStop*>& getStops() const;
    AVLNode* getLeft() const;
    AVLNode* getRight() const;
    AVLNode* getParent() const;

    void addMetroStop(MetroStop* metroStop);
    void setLeft(AVLNode* left);
    void setRight(AVLNode* right);
    void setParent(AVLNode* parent);
};

AVLNode::AVLNode(std::string name) {
    stopName = name;
    left = nullptr;
    right = nullptr;
}

std::string AVLNode::getStopName() const {
    return stopName;
}

const std::vector<MetroStop*>& AVLNode::getStops() const {
    return stops;
}

AVLNode* AVLNode::getLeft() const {
    return left;
}

AVLNode* AVLNode::getRight() const {
    return right;
}

AVLNode* AVLNode::getParent() const {
    return parent;
}

void AVLNode::setLeft(AVLNode* left) {
    this->left = left;
}

void AVLNode::setRight(AVLNode* right) {
    this->right = right;
}

void AVLNode::setParent(AVLNode* parent) {
    this->parent = parent;
}

void AVLNode::addMetroStop(MetroStop* metroStop) {
    stops.push_back(metroStop);
}

class AVLTree {
private:
    AVLNode* root;

public:
AVLTree() {
        root = nullptr;
    }
 
    AVLNode* getRoot() const;

    void setRoot(AVLNode* root);

    int height(AVLNode* node);
    int balanceFactor(AVLNode* node);
    void rotateLeft(AVLNode* node);
    void rotateRight(AVLNode* node);
    void balance(AVLNode* node);
    int stringCompare(string s1, string s2);
    AVLNode* insert(AVLNode* node, MetroStop* metroStop);
    void populateTree(MetroLine* metroLine);
    void inOrderTraversal(AVLNode* node);
    int getTotalNodes(AVLNode* node);
    AVLNode* searchStop(string stopName);
};

AVLNode* AVLTree::getRoot() const {
    return root;
}

void AVLTree::setRoot(AVLNode* root) {
    this->root = root;
}

int AVLTree::height(AVLNode* node) {
    if (node == nullptr) {
        return 0;
    }

    for (int leftHeight = height(node->getLeft()), rightHeight = height(node->getRight());;) {
        cout << "Node: " << node->getStopName() << ", Left Height: " << leftHeight << ", Right Height: " << rightHeight << endl;

        return 1 + max(leftHeight, rightHeight);
    }
}


int AVLTree::stringCompare(string s1, string s2) {
    char *c1 = new char[s1.length() + 1];
    strcpy(c1, s1.c_str());

    char *c2 = new char[s2.length() + 1];
    strcpy(c2, s2.c_str());

    int result = strcmp(c1, c2);
    return result;
}

int AVLTree::balanceFactor(AVLNode* node) {
    if (node == nullptr) {
        return 0;
    }

    return height(node->getLeft()) - height(node->getRight());
}

void AVLTree::rotateLeft(AVLNode* node) {
    AVLNode* NEWROOT = node->getRight();
    node->setRight(NEWROOT->getLeft());
    NEWROOT->setLeft(node);

    AVLNode* parent = node->getParent();
    if (parent != nullptr) {
        if (parent->getLeft() == node) {
            parent->setLeft(NEWROOT);
        } else {
            parent->setRight(NEWROOT);
        }
    }
    NEWROOT->setParent(parent);
    node->setParent(NEWROOT);
}


void AVLTree::rotateRight(AVLNode* node) {
    AVLNode* NEWROOT = node->getLeft();
    node->setLeft(NEWROOT->getRight());
    NEWROOT->setRight(node);

    AVLNode* parent = node->getParent();
    if (parent != nullptr) {
        if (parent->getLeft() == node) {
            parent->setLeft(NEWROOT);
        } else {
            parent->setRight(NEWROOT);
        }
    }
    NEWROOT->setParent(parent);
    node->setParent(NEWROOT);
}



void AVLTree::balance(AVLNode* node) {
     int balancefac = balanceFactor(node);

    if (balancefac > 1) {
        if (balanceFactor(node->getLeft()) < 0) {
            rotateLeft(node->getLeft());
        }
        rotateRight(node);
    }
    else if (balancefac < -1) {

        if (balanceFactor(node->getRight()) > 0) {
            rotateRight(node->getRight());
        }
        rotateLeft(node);
    }
}

AVLNode* AVLTree::insert(AVLNode* node, MetroStop* metroStop) {
    if (node == nullptr) {
        root = new AVLNode(metroStop->getStopName());
        root->addMetroStop(metroStop);
        return root;
    } else {

        int comparison = stringCompare(metroStop->getStopName(), node->getStopName());

        if (comparison < 0) {
            if (node->getLeft() == nullptr) {
        
                AVLNode* leftChild = new AVLNode(metroStop->getStopName());
                leftChild->addMetroStop(metroStop);
                node->setLeft(leftChild);
                return leftChild;
            } else {
             
                node->setLeft(insert(node->getLeft(), metroStop));
                return node;
            }
        } else if (comparison > 0) {
            if (node->getRight() == nullptr) {
       
                AVLNode* rightChild = new AVLNode(metroStop->getStopName());
                rightChild->addMetroStop(metroStop);
                node->setRight(rightChild);
                return rightChild;
            } else {
           
                node->setRight(insert(node->getRight(), metroStop));
                return node;
            }
        } else {
    
            node->addMetroStop(metroStop);
            return node;
        }

        balance(node);
    }
}

void AVLTree::populateTree(MetroLine* metroLine) {
    MetroStop* curr = metroLine->getNode();
    while (curr != nullptr) {
        if (root == nullptr) {
            root = new AVLNode(curr->getStopName());
            root->addMetroStop(curr);
        } else {
            insert(root, curr);
        }
        curr = curr->getNextStop();
    }
}

void AVLTree::inOrderTraversal(AVLNode* node) {
    if (node == nullptr) {
        return;
    }
    inOrderTraversal(node->getLeft());
    cout << node->getStopName() << endl;
    inOrderTraversal(node->getRight());
}

int AVLTree::getTotalNodes(AVLNode* node) {
    if (node == nullptr) {
        return 0;
    }

    int totalNodes = 1;
    for (int leftNodes = getTotalNodes(node->getLeft()), rightNodes = getTotalNodes(node->getRight());;) {
        totalNodes += leftNodes + rightNodes;

        cout << "Node: " << node->getStopName() << ", Total Nodes: " << totalNodes << endl;

        return totalNodes;
    }
}

AVLNode* AVLTree::searchStop(string stopName) {
    AVLNode* curr = root;
    while (curr != nullptr) {
        int comparison = stringCompare(stopName, curr->getStopName());
        if (comparison == 0) {
            return curr;
        } else if (comparison < 0) {
            curr = curr->getLeft();
        } else {
            curr = curr->getRight();
        }
    }
    return nullptr;
}

class Trip {
private:
    MetroStop* node;
    Trip* prev;

public:
    Trip(MetroStop* metroStop, Trip* previousTrip);

    MetroStop* getNode() const;
    Trip* getPrev() const;
};

Trip::Trip(MetroStop* metroStop, Trip* previousTrip) {
    node = metroStop;
    prev = previousTrip;
}

MetroStop* Trip::getNode() const {
    return node;
}

Trip* Trip::getPrev() const {
    return prev;
}

class Exploration {
private:

    std::queue<Trip*> trips;

public:
    Exploration();

    std::queue<Trip*> getTrips() const;

    void enqueue(Trip* trip);
    Trip* dequeue();
    bool isEmpty() const;
};

Exploration::Exploration() {
}

std::queue<Trip*> Exploration::getTrips() const {
    return trips;
}

void Exploration::enqueue(Trip* trip) {
    trips.push(trip);
}

Trip* Exploration::dequeue() {
    if (trips.empty()) {
        return nullptr;
    }
    Trip* trip = trips.front();
    trips.pop();
    cout << "Dequeued: " << trip->getNode()->getStopName() << endl;
    return trip;
}

bool Exploration::isEmpty() const {
    return trips.empty();
}

class Path {
private:
    std::vector<MetroStop*> stops;
    int totalFare;

public:
    Path();

    std::vector<MetroStop*> getStops() const;
    int getTotalFare() const;

    void addStop(MetroStop* stop);
    void setTotalFare(int fare);

    void printPath() const;
};

Path::Path() {
    totalFare = 0;
}

std::vector<MetroStop*> Path::getStops() const {
    return stops;
}

int Path::getTotalFare() const {
    return totalFare;
}

void Path::addStop(MetroStop* stop) {
    stops.push_back(stop);
}

void Path::setTotalFare(int fare) {
    totalFare = fare;
}

void Path::printPath() const {
    for (auto stop : stops) {
        cout << stop->getStopName() << endl;
    }
}

class PathFinder {
private:
    AVLTree* tree;
    std::vector<MetroLine*> lines;

public:
    PathFinder(AVLTree* avlTree, const std::vector<MetroLine*>& metroLines);
    void createAVLTree();
    Path* findPath(std::string origin, std::string destination);

    AVLTree* getTree() const;
    const std::vector<MetroLine*>& getLines() const;
};

PathFinder::PathFinder(AVLTree* avlTree, const std::vector<MetroLine*>& metroLines) {
    tree = avlTree;
    lines = metroLines;
}

AVLTree* PathFinder::getTree() const {
    return tree;
}

const std::vector<MetroLine*>& PathFinder::getLines() const {
    return lines;
}

void PathFinder::createAVLTree() {
     for (MetroLine* metroLine : lines) {
        tree->populateTree(metroLine);
    }
}

Path* PathFinder::findPath(std::string origin, std::string destination) {

    AVLNode* originNode = tree->searchStop(origin);
    AVLNode* destinationNode = tree->searchStop(destination);

    if (originNode == nullptr || destinationNode == nullptr) {

        return nullptr;
    }

    std::queue<AVLNode*> queue;
    std::unordered_map<AVLNode*, AVLNode*> parent; 

    queue.push(originNode);
    parent[originNode] = nullptr;

    while (!queue.empty()) {
        AVLNode* currentNode = queue.front();
        queue.pop();

        if (currentNode == destinationNode) {

            Path* path = new Path();
            int totalFare = 0;

            while (currentNode != nullptr) {
      
                for (MetroStop* metroStop : currentNode->getStops()) {
                    totalFare += metroStop->getFare(); 
                }

                for (MetroStop* metroStop : currentNode->getStops()) {
                    path->addStop(metroStop);
                }

                currentNode = parent[currentNode];
            }

            path->setTotalFare(totalFare);

            std::reverse(path->getStops().begin(), path->getStops().end());

            return path;
        }

  
        if (currentNode->getLeft() != nullptr && parent.find(currentNode->getLeft()) == parent.end()) {
            queue.push(currentNode->getLeft());
            parent[currentNode->getLeft()] = currentNode;
        }

        if (currentNode->getRight() != nullptr && parent.find(currentNode->getRight()) == parent.end()) {
            queue.push(currentNode->getRight());
            parent[currentNode->getRight()] = currentNode;
        }
    }

    return nullptr;

}

