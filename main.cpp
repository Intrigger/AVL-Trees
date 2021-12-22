#include <iostream>

class AVLTree{

private:
    struct AVLNode{
        int key;
        AVLNode* left;
        AVLNode* right;
        AVLNode* parent;
        int balance;
    };

    AVLNode* root;

public:
    AVLTree(){
        root = nullptr;
    }

    void insert(int value){
        return;
    }

};




int main(){

    using std::cout;

    cout << "AVL Trees lab #4\n";



    return 0;
}