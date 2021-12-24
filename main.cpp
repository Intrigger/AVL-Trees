#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cmath>

class AVLTree{

private:
    struct AVLNode{
        int key;
        AVLNode* left;
        AVLNode* right;
        AVLNode* parent;
        int balance;
        int height;
    };

    int getDepth(AVLNode* node){
        using namespace std;
        vector<std::pair<AVLNode*, int>> queue;

        if ((root == nullptr) or (node == nullptr)) return 0;
        queue.push_back(std::make_pair(node, 1));

        int depth = 0;

        while (queue.size() != 0){
            //this_thread::sleep_for(chrono::milliseconds(100));
            int length = queue.size();
            std::pair<AVLNode*, int> element = queue[length - 1];
            queue.pop_back();
            
            depth = max(depth, element.second);

            if (element.first->left != nullptr)
                queue.push_back(std::make_pair(element.first->left, element.second + 1));
            if (element.first->right != nullptr)
                queue.push_back(std::make_pair(element.first->right, element.second + 1));

        }
        
        return depth;
    }

    bool inTree(AVLNode* root, AVLNode* node){
        using namespace std;
        vector<AVLNode*> queue;

        if ((root == nullptr) or (node == nullptr)) return 0;
        queue.push_back(root);

        while (queue.size() != 0){
            int length = queue.size();
            AVLNode* element = queue[length - 1];
            queue.pop_back();
            
            if (element == node) return true;

            if (element->left != nullptr)
                queue.push_back(element->left);
            if (element->right != nullptr)
                queue.push_back(element->right);

        }

        return false;
    }

    AVLNode* root;

public:
    AVLTree(){
        root = nullptr;
    }



    void insert(int value){
        using namespace std;
        //если нет корня - создаем корень, ставим его значением аргументы функции
        if (root == nullptr){
            root = new AVLNode[1];
            root->balance = 0;
            root->left = nullptr;
            root->right = nullptr;
            root->parent = nullptr;
            root->key = value;
            root->height = 0;
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
                else return;
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

            cur->left = nullptr;
            cur->right = nullptr;

            //после добавления проводим балансировку
            //поднимаемся вверх от добавленного узла, пока не найдем узел с нарушением
            
            AVLNode* core = nullptr, *precore = nullptr;

            AVLNode* current = cur;

            bool bad_balance = false;

            while (current != nullptr){
                current->balance = getDepth(current->right) - getDepth(current->left);
                precore = core;
                core = current;

                if (abs(current->balance) > 1){
                    bad_balance = true;
                    break;
                }
                else{
                    current = current->parent;
                }
                //this_thread::sleep_for(chrono::milliseconds(500));
            }

            if (bad_balance){
                //1 добавление в левое поддерево левого сына опорного узла
                if ((precore == core->left) and inTree(precore->left, cur)){
                    cout << "case 1\n";
                    if (core == root){
                        root = precore;
                        precore->parent = nullptr;
                        core->left = precore->right;
                        if (precore->right != nullptr)
                            precore->right->parent = core;
                        precore->right = core;
                        core->parent = precore;
                    }
                    else{
                        if (core == core->parent->left){
                            core->parent->left = precore;
                        }
                        else{
                            core->parent->right = precore;
                        }

                        if (precore->right != nullptr)
                            precore->right->parent = core;

                        precore->parent = core->parent;
                        core->parent = precore;

                       
                        core->left = precore->right;
                        precore->right = core;
                    }
                }
                //2 добавление в правое поддерево правого сына опорного узла
                if ((precore == core->right) and (inTree(precore->right, cur))){
                    cout << "case 2\n";
                    if (core == root){
                        root = precore;
                        precore->parent = nullptr;
                        core->right = precore->left;
                        if (precore->left != nullptr)
                            precore->left->parent = core;
                        precore->left = core;
                        core->parent = precore;

                    }
                    else{
                        if (core == core->parent->left){
                            core->parent->left = precore;
                        }
                        else{
                            core->parent->right = precore;
                        }

                        if (precore->left != nullptr)
                            precore->left->parent = core;

                        precore->parent = core->parent;
                        core->parent = precore;

                        
                        core->right = precore->left;
                        precore->left = core;
                    }
                }

                //3 добавление в правое поддерево левого сына опорного узла
                if ((precore == core->left) and (inTree(precore->right, cur))){

                    AVLNode* C = core;
                    AVLNode* A = precore;
                    AVLNode* B = precore->right;

                    if (core == root){
                        root = B;
                        B->parent = nullptr;
                    }
                    else{
                        if (core->parent->left == core){
                            core->parent->left = B;
                            B->parent = core->parent;
                        }
                        else{
                            core->parent->right = B;
                            B->parent = core->parent;
                        }
                    }

                    if (B->left != nullptr)
                        B->left->parent = A;
                    A->right = B->left;
                    
                    B->left = A;
                    A->parent = B;

                    if (B->right != nullptr)
                        B->right->parent = C;
                    C->left = B->right;

                    B->right = C;
                    C->parent = B;


                    
                }

                //4 добавление в левое поддерево правого сына опорного узла
                if ((precore == core->right) and (inTree(precore->left, cur))){
                    AVLNode* A = core;
                    AVLNode* C = precore;
                    AVLNode* B = precore->left;

                    if (core == root){
                        root = B;
                        B->parent = nullptr;
                    }
                    else{
                        if (core->parent->left == core){
                            core->parent->left = B;
                            B->parent = core->parent;
                        }
                        else{
                            core->parent->right = B;
                            B->parent = core->parent;
                        }
                    }


                    if (B->left != nullptr)
                        B->left->parent = A;
                    A->right = B->left;
                    
                    B->left = A;
                    A->parent = B;

                    if (B->right != nullptr)
                        B->right->parent = C;
                    C->left = B->right;

                    B->right = C;
                    C->parent = B;

                    
                }
            }

        }
    }

    void remove(int value){

        using namespace std;

        AVLNode* cur = root;

        bool found = false;

        while (cur != nullptr){
            if (value < cur->key){
                cur = cur->left;
            }
            else if (value > cur->key){
                cur = cur->right;
            }
            else{
                found = true;
                break;
            }
        }

        if (!found) return;
        
        //нет детей
        if ((cur->left == nullptr) and (cur->right == nullptr)){
            if (cur->parent == nullptr){
                delete[] cur;
                root = nullptr;
            }
            else{
                if (cur->parent->left == cur){
                    cur->parent->left = nullptr;
                }
                else{
                    cur->parent->right = nullptr;
                }
                AVLNode* current = cur->parent;
                delete[] cur;                

                while (current != nullptr){
                    cout << "Current->key: " << current->key << endl;
                    int leftDepth = getDepth(current->left);
                    int rightDepth = getDepth(current->right);
                    current->balance = rightDepth - leftDepth;
                    if (abs(current->balance) > 1){
                        cout << "current->key = " << current->key << endl;
                        if (leftDepth == rightDepth - 2){
                            cout << "case 1!\n";
                            AVLNode* A = current;
                            AVLNode* B = current->right;
                            if (getDepth(B->right) >= getDepth(B->left)){
                                cout << "case 1a!\n";
                                if (A == root){
                                    root = B;
                                    B->parent = nullptr;
                                    A->right = B->left;
                                    if (B->left != nullptr)
                                        B->left->parent = A;
                                    B->left = A;
                                    A->parent = B;

                                }
                                else{
                                    if (A == A->parent->left){
                                        A->parent->left = B;
                                    }
                                    else{
                                        A->parent->right = B;
                                    }

                                    if (B->left != nullptr)
                                        B->left->parent = A;

                                    B->parent = A->parent;
                                    A->parent = B;

                                    
                                    A->right = B->left;
                                    B->left = A;
                                }                            
                            }
                            else{
                                cout << "case 1b!";
                                AVLNode* C = current->right;
                                B = C->left;

                                if (getDepth(C->right) < getDepth(B)){
                                    
                                    

                                    if (A == root){
                                        root = B;
                                        B->parent = nullptr;
                                    }
                                    else{
                                        if (A->parent->left == A){
                                            A->parent->left = B;
                                            B->parent = A->parent;
                                        }
                                        else{
                                            A->parent->right = B;
                                            B->parent = A->parent;
                                        }
                                    }


                                    if (B->left != nullptr)
                                        B->left->parent = A;
                                    A->right = B->left;
                                    
                                    B->left = A;
                                    A->parent = B;

                                    if (B->right != nullptr)
                                        B->right->parent = C;
                                    C->left = B->right;

                                    B->right = C;
                                    C->parent = B;

                
                                }

                            }
                            
                        }
                        else if (leftDepth == rightDepth + 2){
                            

                            AVLNode* B = current;
                            AVLNode* A = B->left;

                            if (getDepth(A->left) >= getDepth(A->right)){

                                if (B == root){
                                    root = A;
                                    A->parent = nullptr;
                                    B->left = A->right;
                                    if (A->right != nullptr)
                                        A->right->parent = B;
                                    A->right = B;
                                    B->parent = A;
                                }
                                else{
                                    if (B == B->parent->left){
                                        B->parent->left = A;
                                    }
                                    else{
                                        B->parent->right = A;
                                    }

                                    if (A->right != nullptr)
                                        A->right->parent = B;

                                    A->parent = B->parent;
                                    B->parent = A;

                                
                                    B->left = A->right;
                                    A->right = B;
                                }
                            }
                            else{
                                AVLNode* C = current;
                                A = C->left;
                                B = A->right;

                                if (C == root){
                                    root = B;
                                    B->parent = nullptr;
                                }
                                else{
                                    if (C->parent->left == C){
                                        C->parent->left = B;
                                        B->parent = C->parent;
                                    }
                                    else{
                                        C->parent->right = B;
                                        B->parent = C->parent;
                                    }
                                }

                                if (B->left != nullptr)
                                    B->left->parent = A;
                                A->right = B->left;
                                
                                B->left = A;
                                A->parent = B;

                                if (B->right != nullptr)
                                    B->right->parent = C;
                                C->left = B->right;

                                B->right = C;
                                C->parent = B;
                            }
                        }
                    }
                    current = current->parent;
                }
            }
        }
        else{
            if ((cur->left != nullptr) ^ (cur->right != nullptr)){
                if (cur->left != nullptr){
                    if (cur == root){
                        root = cur->left;
                        root->parent = nullptr;
                    }
                    else{
                        if (cur == cur->parent->left){
                            cur->parent->left = cur->left;
                        }
                        else{
                            cur->parent->right = cur->left;
                        }

                        delete[] cur;

                    }
                }
                if (cur->right != nullptr){
                    if (cur == root){
                        root = cur->right;
                        root->parent = nullptr;
                    }
                    else{
                        if (cur == cur->parent->left){
                            cur->parent->left = cur->right;
                            delete[] cur;
                        }
                        else{
                            cur->parent->right = cur->right;
                            delete[] cur;
                        }
                    }
                }
            }
            else{
                if ((cur->left != nullptr) and (cur->right != nullptr)){
                    //ищем самый левый узел в правом поддереве

                    cout << "двое детей!\n";
                    this_thread::sleep_for(chrono::milliseconds(100));

                    AVLNode* current = cur->right;
                    while (current->left != nullptr){
                        current = current->left;
                    }

                    std::swap(cur->key, current->key);
                    cur = current;
                    if ((cur->left == nullptr) and (cur->right == nullptr)){
                        if (cur->parent == nullptr){
                            delete[] cur;
                            root = nullptr;
                        }
                        else{
                            if (cur->parent->left == cur){
                                cur->parent->left = nullptr;
                            }
                            else{
                                cur->parent->right = nullptr;
                            }
                            AVLNode* current = cur->parent;
                            delete[] cur;                

                            while (current != nullptr){
                                cout << "Current->key: " << current->key << endl;
                                int leftDepth = getDepth(current->left);
                                int rightDepth = getDepth(current->right);
                                current->balance = rightDepth - leftDepth;
                                if (abs(current->balance) > 1){
                                    cout << "current->key = " << current->key << endl;
                                    if (leftDepth == rightDepth - 2){
                                        cout << "case 1!\n";
                                        AVLNode* A = current;
                                        AVLNode* B = current->right;
                                        if (getDepth(B->right) >= getDepth(B->left)){
                                            cout << "case 1a!\n";
                                            if (A == root){
                                                root = B;
                                                B->parent = nullptr;
                                                A->right = B->left;
                                                if (B->left != nullptr)
                                                    B->left->parent = A;
                                                B->left = A;
                                                A->parent = B;

                                            }
                                            else{
                                                if (A == A->parent->left){
                                                    A->parent->left = B;
                                                }
                                                else{
                                                    A->parent->right = B;
                                                }

                                                if (B->left != nullptr)
                                                    B->left->parent = A;

                                                B->parent = A->parent;
                                                A->parent = B;

                                                
                                                A->right = B->left;
                                                B->left = A;
                                            }                            
                                        }
                                        else{
                                            cout << "case 1b!";
                                            AVLNode* C = current->right;
                                            B = C->left;

                                            if (getDepth(C->right) < getDepth(B)){
                                                
                                                

                                                if (A == root){
                                                    root = B;
                                                    B->parent = nullptr;
                                                }
                                                else{
                                                    if (A->parent->left == A){
                                                        A->parent->left = B;
                                                        B->parent = A->parent;
                                                    }
                                                    else{
                                                        A->parent->right = B;
                                                        B->parent = A->parent;
                                                    }
                                                }


                                                if (B->left != nullptr)
                                                    B->left->parent = A;
                                                A->right = B->left;
                                                
                                                B->left = A;
                                                A->parent = B;

                                                if (B->right != nullptr)
                                                    B->right->parent = C;
                                                C->left = B->right;

                                                B->right = C;
                                                C->parent = B;

                            
                                            }

                                        }
                                        
                                    }
                                    else if (leftDepth == rightDepth + 2){
                                        

                                        AVLNode* B = current;
                                        AVLNode* A = B->left;

                                        if (getDepth(A->left) >= getDepth(A->right)){

                                            if (B == root){
                                                root = A;
                                                A->parent = nullptr;
                                                B->left = A->right;
                                                if (A->right != nullptr)
                                                    A->right->parent = B;
                                                A->right = B;
                                                B->parent = A;
                                            }
                                            else{
                                                if (B == B->parent->left){
                                                    B->parent->left = A;
                                                }
                                                else{
                                                    B->parent->right = A;
                                                }

                                                if (A->right != nullptr)
                                                    A->right->parent = B;

                                                A->parent = B->parent;
                                                B->parent = A;

                                            
                                                B->left = A->right;
                                                A->right = B;
                                            }
                                        }
                                        else{
                                            AVLNode* C = current;
                                            A = C->left;
                                            B = A->right;

                                            if (C == root){
                                                root = B;
                                                B->parent = nullptr;
                                            }
                                            else{
                                                if (C->parent->left == C){
                                                    C->parent->left = B;
                                                    B->parent = C->parent;
                                                }
                                                else{
                                                    C->parent->right = B;
                                                    B->parent = C->parent;
                                                }
                                            }

                                            if (B->left != nullptr)
                                                B->left->parent = A;
                                            A->right = B->left;
                                            
                                            B->left = A;
                                            A->parent = B;

                                            if (B->right != nullptr)
                                                B->right->parent = C;
                                            C->left = B->right;

                                            B->right = C;
                                            C->parent = B;
                                        }
                                    }
                                }
                                current = current->parent;
                            }
                        }
                    }

                }

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
            std::cout << element.first->key << "\t" << "depth: " << getDepth(element.first) << "\t";
            if (element.first->parent != nullptr)
                std::cout << element.first->parent->key << std::endl;
            else std::cout << std::endl;
            if (element.first->left != nullptr)
                queue.push_back(std::make_pair(element.first->left, element.second + 1));
            if (element.first->right != nullptr)
                queue.push_back(std::make_pair(element.first->right, element.second + 1));

        }

    }

};




int main(){


    
    using namespace std;


    cout << "AVL Trees lab #4\n";

    AVLTree tree;

    const int tree_size = 7;

    int* values = new int[tree_size];


    for (int i = 0; i < tree_size; i++){
        values[i] = i;
    }

    // cout << "numbers generated!\n";

    // cout << "enter some character: ";
    // int x;
    // cin >> x;

    for (int i = 0; i < tree_size; i++){
        cout << "insert " << values[i] << endl;
        tree.insert(values[i]);
    }

    tree.remove(1);

    cout << endl; tree.print(); cout << endl;


    return 0;
}