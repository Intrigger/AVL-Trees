#include <iostream>
#include <vector>

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
        //если нет корня - создаем корень, ставим его значением аргументы функции
        if (root == nullptr){
            root = new AVLNode[1];
            root->balance = 0;
            root->left = nullptr;
            root->right = nullptr;
            root->parent = nullptr;
            root->key = value;
            return;
        }
        else{
            AVLNode* cur = root;
            AVLNode* preCurrent = cur;

            while (cur != nullptr){
                if (value < cur->key){
                    preCurrent = cur;
                    cur = cur->left;
                }
                else if (value > cur->key){
                    preCurrent = cur;
                    cur = cur->right;
                }
                else break;
            }

            cur = new AVLNode[1];
            cur->key = value;
            cur->parent = preCurrent;

            if (value < preCurrent->key){
                preCurrent->left = cur;
            }
            else{
                preCurrent->right = cur;
            }

        }
    }

    void print(){
        using std::vector;
        vector<std::pair<AVLNode*, int>> queue;

        if (root == nullptr) return;
        queue.push_back(std::make_pair(root, 0));

        int tabs_count = 0;

        while (queue.size() != 0){
            int length = queue.size();
            std::pair<AVLNode*, int> element = queue[length - 1];
            queue.pop_back();
            for (int t = 0; t < element.second; t++) std::cout << "\t";
            std::cout << element.first->key << "\n";
            if (element.first->left != nullptr)
                queue.push_back(std::make_pair(element.first->left, element.second + 1));
            if (element.first->right != nullptr)
                queue.push_back(std::make_pair(element.first->right, element.second + 1));
        }



    }

};




int main(){

    srand(time(0));

    using namespace std;

    cout << "AVL Trees lab #4\n";

    AVLTree tree;

    const int tree_size = 16;

    for (int i = 0; i < tree_size; i++){
        int v = rand() % 1000;
        cout << v << " ";
        tree.insert(v);
    }

    cout << endl;   
    tree.print();



    return 0;
}