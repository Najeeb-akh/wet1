#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <new>
#include "AvlNode.h"
#include <iostream>
//#include "DSW_CAR.h"



using std::cout;
using std::endl;

/**
 * Generic AVL Tree
 *
 * the following functions are available:
 * Constructor       - Creates a new empty AVL Tree.
 * Destructor        - Deletes an existing AVL Tree.
 *
 * max               - returns the maximum between two integers.
 *
 * setBalance        - updates the height balance of the received node based
 *                        on the equation height(leftSon)-height(RightSon)
 *
 * RightRotate       - executes an LL Rotation. (node is B)
 *
 * LeftRotate        - executes an RR Rotation. (node is B)
 *
 * LRRotate          - executes an RR Rotation and then an LL Rotation.
 *
 * RLRotate          - executes an LL Rotation and then an RR Rotation.
 *
 * reBalance         - rearranges the AVL tree to be balanced after adding
 *                     or removing a node.
 *
 * Find              - checks if the AVL tree contains a node with the received key.
 *
 * FindAndReturn     - checks if the AVL tree contains a node with the received
 *                      key and returns a pointer.
 *
 * Find_aux          - Finds a node in the AVL tree based on the matching data/value
 *                     and returns a pointer to the node.
 *
 * add               - creates a new node with the received parameters, adds
 *                     it to the AVL tree and reorganizes the tree to
 *                     keep it balanced. returns true if added successfully.
 *
 * updateParents     - given a root of an avl tree with only pointing down arrows,
 *                     it updates the parent of each node in the tree.
 *
 * Delete            - Deletes a node in the list that contains the same data/value as
 *                     the received one. returns true if the node is found and
 *                     deleted.
 *
 * DeleteByPointer   - Deletes a node in the list that matches the node pointed
 *                     at by the received pointer, and reorganizes the tree to
 *                     keep it balanced.
 *
 * Size              - returns the size of the AVL tree in the received pointer
 *                     and returns true.
 *
 * ClearTree         - Deletes all the left and right subroots of the given node
 *                     and then deletes the node itself.DOES NOT maintain the tree balanced
 *
 * inorder           - executes an inorder tour over the AVL tree nodes and returns
 *                     (god knows what , ask fadi)
 *
 * PrintTree         - prints the tree as how it should look graphically.
 */

template <class T,class K>
class Avl_Tree{
public:
    AvlNode<T,K>* root;
    AvlNode<T,K>* min;
    AvlNode<T,K>* maxN;
    int size;

    Avl_Tree():size(0){
        root= nullptr;
        min = nullptr;
    }
//***********************
/**
 * returns the maximum between two integers.
 * @param  a
 * @param  b
 * @return a - if a>b
 *         b - if a<=b
 */
    int max(int a, int b) {
        if (a > b) {
            return a;
        }
        return b;
    }
//***********************

//***********************
/*   int height(AvlNode<T> *node) {
//        if (node == nullptr) {
//            return -1;
//        }
//        return 1 + max(height(node->right_son), height(node->left_son));
//    }
*/
//***********************
//***********************
/**
 * calculates and updates the balance factor of the given node.
 * @param node - an avl node.
 */
    void setBalance(AvlNode<T,K> *node) {
//      node->balance_factor = height(node->left_son) - height(node->right_son);
        if(node->right_son==nullptr && node->left_son==nullptr){
            node->height=0;
            node->balance_factor=0;
            return;
        }

        if(node->left_son!=nullptr && node->right_son==nullptr){
            node->height=(node->left_son->height)+1;
            node->balance_factor=(node->left_son->height)+1;
            return;
        }

        if(node->left_son==nullptr && node->right_son!=nullptr){
            node->height=(node->right_son->height+1);
            node->balance_factor=((node->right_son->height)+1)*-1;
            return;
        }
        node->height=max(node->right_son->height,node->left_son->height)+1;
        node->balance_factor=(node->left_son->height)-(node->right_son->height);
    }
//***********************
/**
 * executes a rotation to the right as we learned in the slides.
 * @param  node - the "v" node shown in the tutorials.
 * @return a ptr to to an avl node.
 */
    AvlNode<T,K>* RightRotate(AvlNode<T,K> *node,bool flag) {
        AvlNode<T,K> *n = node->left_son;
        n->parent = node->parent;
        node->left_son = n->right_son;

        if (node->left_son != nullptr) {
            node->left_son->parent = node;
            n->right_son = node;
            node->parent = n;
        } else {
            n->right_son = node;
            node->parent = n;
        }
        if (n->parent != nullptr) {
            if (n->parent->right_son == node) {
                n->parent->right_son = n;

            } else {
                n->parent->left_son = n;
            }
        }
        setBalance(node);
        setBalance(n);
        if(!flag){
            int leftSub = 0;
            int rightSub = 0;
            if (n->left_son) leftSub = n->left_son->num_of_subNodes;
            if (node->left_son) rightSub = node->left_son->num_of_subNodes;
            n->num_of_subNodes = leftSub + rightSub + 1;
            int leftSub1 = 0;
            if (node->right_son) leftSub1 = node->right_son->num_of_subNodes;
            node->num_of_subNodes = leftSub1 + n->num_of_subNodes;
        }
        else{
            int rightSub = 0;
            int leftSub = 0;
            if(node->right_son){
                rightSub = node->right_son->num_of_subNodes;
            }
            if(node->left_son){
                leftSub = node->left_son->num_of_subNodes;
            }
            node->num_of_subNodes = rightSub + leftSub + 1;
            int leftSub1 =0;
            if(n->left_son){
                leftSub1 = n->left_son->num_of_subNodes;
            }
            n->num_of_subNodes = node->num_of_subNodes + leftSub1 +1;
        }

        return n;
    }

//***********************
/**
 * executes a rotation to the left as we learned in the slides.
 * @param  node - the "v" node shown in the tutorials.
 * @return a ptr to to an avl node.
 */
    AvlNode<T,K>* LeftRotate(AvlNode<T,K> *node,bool flag) {
        AvlNode<T,K> *n = node->right_son;
        n->parent = node->parent;
        node->right_son = n->left_son;

        if (node->right_son != nullptr) {
            node->right_son->parent = node;
            n->left_son = node;
            node->parent = n;
        } else {
            n->left_son = node;
            node->parent = n;
        }
        if (n->parent != nullptr) {
            if (n->parent->right_son == node) {
                n->parent->right_son = n;

            } else {
                n->parent->left_son = n;
            }
        }
        setBalance(node);
        setBalance(n);

        if (!flag){
            int rightSub = 0;
            int leftSub = 0;
            if(node->right_son){
                rightSub = node->right_son->num_of_subNodes;
            }
            if(node->left_son){
                leftSub = node->left_son->num_of_subNodes;
            }
            node->num_of_subNodes = rightSub + leftSub + 1;
            int leftSub1 =0;
            if(n->left_son){
                leftSub1 = n->left_son->num_of_subNodes;
            }
            n->num_of_subNodes = node->num_of_subNodes + leftSub1 +1;
        }
        else{
            int leftSub = 0;
            int rightSub = 0;
            if (n->left_son) leftSub = n->left_son->num_of_subNodes;
            if (node->left_son) rightSub = node->left_son->num_of_subNodes;
            n->num_of_subNodes = leftSub + rightSub + 1;
            int leftSub1 = 0;
            if (node->right_son) leftSub1 = node->right_son->num_of_subNodes;
            node->num_of_subNodes = leftSub1 + n->num_of_subNodes;
        }


        return n;
    }
//***********************
/**
 * executes a left rotation followed with right rotation. (LRRotation).
 * @param node
 * @return
 */
    AvlNode<T,K>* LRRotate(AvlNode<T,K> *node) {
        node->left_son = LeftRotate(node->left_son, false);
        return RightRotate(node, false);
    }
//***********************
/**
 * * executes a right rotation followed with left rotation. (RLRotation).
 * @param node
 * @return
 */
    AvlNode<T,K>* RLRotate(AvlNode<T,K> *node) {
        node->right_son = RightRotate(node->right_son, false);
        return LeftRotate(node, false);
    }
//***********************
/**
 * checks if there are any unbalanced nodes, excutes the appropriate rotations,
 * and updates the new balance factor for each node along the path.
 * @param node - an avl node
 */
    void reBalance(AvlNode<T,K> *node) {
        setBalance(node);
        if (node->balance_factor == 2) {
            if (node->left_son->balance_factor >= 0) {
                node = RightRotate(node, true);
            } else {
                node = LRRotate(node);
            }
        } else if (node->balance_factor == -2) {
            if (node->right_son->balance_factor <= 0)
                node = LeftRotate(node, true);
            else node = RLRotate(node);
        }
        if (node->parent != nullptr) {
            reBalance(node->parent);
        } else {
            root = node;
        }
    }
//***********************

    Avl_Tree(const Avl_Tree& copied)
    {
        root = nullptr;
        root = copy_avl_helper(copied.root);
        this->updateMin();
        this->updateMax();
        size = copied.size;
    }
    //---------------------------------------------------------------------------------------------------------------------
    AvlNode<T,K>* copy_avl_helper(AvlNode<T,K>* node)const
    {
        if (!node)
        {
            return nullptr;
        }
        AvlNode<T,K>* copy_root = new AvlNode<T,K>((node->key) , (node->value) ,node->height);
        copy_root->updateLeftSon(copy_avl_helper(node->left_son));
        copy_root->updateRightSon(copy_avl_helper(node->right_son));
        return copy_root;
    }

/**
 * searched for a certain node in the avl tree, based on it's key.
 * @param  key
 * @return true  - if a node with the same key exists.
 *         false - else.
 */
    T* Find(K key){
        if(this->root== nullptr){
            return nullptr;
        }
        AvlNode<T,K>* ptr=root;
        while (true){
            if(ptr->key==key){
                return &ptr->value;
            }
            bool left=ptr->key>key;
            if(left){
                ptr=ptr->left_son;
            } else{
                ptr=ptr->right_son;
            }
            if(ptr== nullptr) return nullptr;

        }
    }

    void updateRankPostOrder(AvlNode<T,K>* node){
        if(node== nullptr)
            return;
        updateRankPostOrder(node->left_son);
        updateRankPostOrder(node->right_son);
        int lefSub =0;
        int rightSub = 0;
        if(node->left_son) lefSub = node->left_son->num_of_subNodes;
        if (node->right_son) rightSub = node->right_son->num_of_subNodes;
        node->num_of_subNodes = lefSub + rightSub +1;
    }

    void updateHighPostOrder(AvlNode<T,K>* node){
        if (node == nullptr)return;
        updateHighPostOrder(node->left_son);
        updateHighPostOrder(node->right_son);
        if(node->right_son==nullptr && node->left_son==nullptr){
            node->height=0;
            return;
        }
        if(node->left_son!=nullptr && node->right_son==nullptr){
            node->height=(node->left_son->height)+1;
            return;
        }
        if(node->left_son==nullptr && node->right_son!=nullptr){
            node->height=(node->right_son->height+1);
            return;
        }
        node->height=max(node->right_son->height,node->left_son->height)+1;
    }

    void updateBalanceFactor(AvlNode<T,K>* node){
        if(node== nullptr)return;
        updateBalanceFactor(node->left_son);
        updateBalanceFactor(node->right_son);
        if(node->right_son==nullptr && node->left_son==nullptr){
            node->balance_factor=0;
            return;
        }

        if(node->left_son!=nullptr && node->right_son==nullptr){
            node->balance_factor=(node->left_son->height)+1;
            return;
        }

        if(node->left_son==nullptr && node->right_son!=nullptr){
            node->balance_factor=((node->right_son->height)+1)*-1;
            return;
        }
        node->balance_factor=(node->left_son->height)-(node->right_son->height);
    }

    void updateTree(AvlNode<T,K>* node){
        updateRankPostOrder(node);
        updateHighPostOrder(node);
        updateBalanceFactor(node);
    }
//***********************
/**
 * the same as the "find" function above, with the addition of returning a pointer
 * to the found node, or null of not.
 * @param  key .
 * @return node ptr - if a node with the same key exists.
 *         null     - else.
 */
    AvlNode<T,K>* FindAndReturn(int key){
        if(this->root== nullptr){
            return nullptr;
        }
        AvlNode<T,K>* ptr=root;
        while (true){
            if(ptr->key==key){
                return ptr;
            }
            bool left=ptr->key > key;
            if(left){
                ptr=ptr->left_son;
            } else{
                ptr=ptr->right_son;
            }
            if(ptr== nullptr) return nullptr;
        }
    }
//***********************
/**
 * used to find the node based on the node's value.
 * @param DC            - a pointer value that we are searching for.
 * @return node pointer - a pointer to the node in case it was found in the avl tree.
 */
    AvlNode<T,K>* Find_aux(K key){
        if(! this) return nullptr;
        if(root== nullptr){
            return nullptr;
        }
        AvlNode<T,K>* ptr=root;
        while (true){
            if(key==(ptr->key)){
                return ptr;
            }
            bool left=(ptr->key)>key;
            if(left){
                ptr=ptr->left_son;
            } else{
                ptr=ptr->right_son;
            }
            if(ptr== nullptr) return nullptr;
        }
    }
//***********************
/**
 * creates a new node with the received parameters, adds it to the AVL tree and
 * reorganizes the tree to keep it balanced. returns true if added successfully.
 * @param DC     - a pointer to the value of the future node.
 * @param key    - a key of the future node.
 * @return true  - if the node was successfully created.
 *         false - else.
 */
        bool Add(T* DC,K key){
        if(this->root== nullptr){
            this->root=new AvlNode<T,K>(key,*DC,0);
            this->updateMin();
            this->updateMax();
            this->root->num_of_subNodes=1;
            size++;
        }else {
            AvlNode<T,K> *p, *n = root;
            while (true) {
                if (n->key == key) return false;
                p = n;
                bool left = key < n->key;
                if (left) {
                    n = n->left_son;

                } else {
                    n = n->right_son;
                }
                if (n == nullptr) {
                    if (left) {
                        p->left_son = new AvlNode<T,K>(key,*DC,0,p);
                        this->updateMin();
                        this->updateMax();
//                        this->updateRank();
                        size++;
                    } else {
                        p->right_son = new AvlNode<T,K>(key,*DC,0,p);
                        this->updateMin();
                        this->updateMax();
//                        this->updateRank();
                        size++;
                    }

                    reBalance(p);
                    break;
                }
            }
            AvlNode<T,K>* ptr=this->Find_aux(key);
            this->updateRank(ptr);
        }
        return true;
}
//***********************

void updateRank(AvlNode<T,K> * ptr)
{
            if (!ptr) return;
    while (ptr !=root && ptr!= nullptr)
    {
        int lefSub =0;
        int rightSub = 0;
        if(ptr->left_son) lefSub = ptr->left_son->num_of_subNodes;
        if (ptr->right_son) rightSub = ptr->right_son->num_of_subNodes;
        ptr->num_of_subNodes = lefSub + rightSub +1;
        ptr = ptr->parent;
    }
        int lefSub =0;
        int rightSub = 0;
        if(ptr->left_son) lefSub = ptr->left_son->num_of_subNodes;
        if (ptr->right_son) rightSub = ptr->right_son->num_of_subNodes;
        ptr->num_of_subNodes = lefSub + rightSub +1;
}
/**
 * updates the parent pointer for each node in the avl.
 * @param AVL_Root - the root of the avl tree which its' nodes has no parents provided.
 */
    void updateParents(AvlNode<T,K>* AVL_Root)
    {
        if(AVL_Root == nullptr)
            return;
        if(AVL_Root->right_son)
            AVL_Root->right_son->parent=AVL_Root;
        if(AVL_Root->left_son)
            AVL_Root->left_son->parent=AVL_Root;
        updateParents(AVL_Root->right_son);
        updateParents(AVL_Root->left_son);
    }
//***********************

    bool Delete(K key){
        AvlNode<T,K>* ptr=this->Find_aux(key);
        if(ptr== nullptr)
            return false;
        this->DeleteByPointer(ptr);
        return true;
    }
//***********************
/**
 * Deletes a node in the list that matches the node pointed at by the received
 * pointer, and reorganizes the tree to keep it balanced.
 * @param node - a node pointer that is used to searching for its' matching node
 */
    void  DeleteByPointer(AvlNode<T,K>* node){
      if ((node==root || node->parent == nullptr) && node->left_son == nullptr && node->right_son != nullptr) {
          root = node->right_son;
          node->right_son->parent = nullptr;
          delete node;
          this->updateMin();
          this->updateMax();
          this->updateRank(root);
          size--;
          return;
      }
      if ((node==root || node->parent == nullptr) && node->left_son != nullptr && node->right_son == nullptr) {
          root = node->left_son;
          node->left_son->parent = nullptr;
          AvlNode<T,K>* par = node->parent;
          delete node;
          this->updateMin();
          this->updateMax();
          this->updateRank(par);
        size--;
          return ;
      }
      if ((node==root || node->parent == nullptr) && node->left_son == nullptr && node->right_son == nullptr) {

          AvlNode<T,K>* par = node->parent;
          delete node;
          root = nullptr;
          this->updateMin();
          this->updateMax();
          this->updateRank(par);
          size--;
          return ;
      }

        AvlNode<T,K>* p=node->parent;
          if (node->left_son == nullptr && node->right_son == nullptr) {
              if (node->parent->right_son != nullptr) {
                  if (node->parent->right_son->key == node->key) {
                      p->right_son = nullptr;
                  }
              }
              if (node->parent->left_son != nullptr) {
                  if (node->parent->left_son->key == node->key) {
                      p->left_son = nullptr;
                  }

              }
              AvlNode<T,K>* par = node->parent;
              delete node;
              reBalance(p);
              this->updateMin();
              this->updateMax();
              this->updateRank(par);
              size--;
              return ;
          }

          if (node->left_son != nullptr && node->right_son == nullptr) {
              if (node->parent->right_son != nullptr) {
                  if (node->parent->right_son->key == node->key) {
                      node->parent->right_son = node->left_son;
                      node->left_son->parent = node->parent;
                  }
              }
              if (node->parent->left_son != nullptr) {
                  if (node->parent->left_son->key == node->key) {
                      node->parent->left_son = node->left_son;
                      node->left_son->parent = node->parent;
                  }
              }
              AvlNode<T,K>* par = node->parent;
              delete node;
              reBalance(p);
              this->updateMin();
              this->updateMax();
              this->updateRank(par);
              size--;
              return ;

          }
          if (node->left_son == nullptr && node->right_son != nullptr) {
              if (node->parent->right_son != nullptr) {
                  if (node->parent->right_son->key == node->key) {
                      node->parent->right_son = node->right_son;
                      node->right_son->parent = node->parent;
                  }
              }

              if (node->parent->left_son != nullptr) {
                  if (node->parent->left_son->key == node->key) {
                      node->parent->left_son = node->right_son;
                      node->right_son->parent = node->parent;
                  }
              }
              AvlNode<T,K>* par = node->parent;
              delete node;
              reBalance(p);
              this->updateMin();
              this->updateMax();
              this->updateRank(par);
              size--;
              return ;

          }
          if (node->left_son != nullptr && node->right_son != nullptr) {
              AvlNode<T,K>* after = node->right_son;
              while (after->left_son != nullptr) {
                  after = after->left_son;
              }
              K tmp=node->key;
              node->key = after->key;
              after->key=tmp;

              T tmp1=(node->value);
              node->value=after->value;
              after->value=tmp1;

              DeleteByPointer(after);
          }
    }

/**
 * deletes all the nodes of the tree.
 * @param node - the root of the avl tree to be cleared.
 */
    void clear_tree(AvlNode<T,K>* node){
        if(node== nullptr) return;
        clear_tree(node->left_son);
        clear_tree(node->right_son);
        delete node;
    }
//***********************
/**
 * executes an inorder traversal.
 * @param node - the root.
 * @param mmm
 * @return c   - the size of the avl tree, the number of its' nodes.
 */


    int inorderTest(AvlNode<T,K> *node) {
        if (node == nullptr) return 0;
        inorderTest(node->left_son);
        if(node->key>10 || node->key<1)
            return 1;
        inorderTest(node->right_son);
        return 0;
    }
//***********************

    void inorderGetVal2(AvlNode<T,K> node, int songs,int* i,int* count,int num)
    {
        if (node == nullptr || (*count)>=num)
            return;
        inorderGetVal2(node->left_son,songs,i,count,num);
        if((*count)<num) {
            songs[(*i)] = node->key;
            (*i)++;
            (*count)++;
        }
        inorderGetVal2(node->right_son,songs,i,count,num);

    }

    void inorderGetVal(AvlNode<T,K> node, T arr,int* i)
    {
        if (node == nullptr)
            return;
        inorderGetVal(node->left_son,arr,i);
            (arr[(*i)]) = node->value;
            (*i)++;
        inorderGetVal(node->right_son,arr,i);

    }

    void inorderGetValT(AvlNode<T,K> node, K arr,int* i)
    {
        if (node == nullptr)
            return;
        inorderGetValT(node->left_son,arr,i);
        (arr[(*i)]) = node->key;
        (*i)++;
        inorderGetValT(node->right_son,arr,i);

    }
//***********************
/**
 * prints the tree in a graphic way.
 * @param ptr   - the root of the avl tree.
 * @param level -
 */
    void print_tree(AvlNode<T,K> *ptr, const int &level) {
        if (ptr != nullptr) {
            print_tree(ptr->right_son, level + 1);
             std :: cout << std :: endl;
            if (ptr == this->root) {
              std::  cout << "Root->";
            }
            for (int i = 0; i < level && ptr != root; i++)
                std ::cout << "   ";
            std :: cout <<ptr->value.typeId << " "<< ptr->value.modelId << " "<<ptr->key ;
            print_tree(ptr->left_son, level + 1);
        }
    }
//***********************



    ///get the max vlaue
    AvlNode<T,K>* find_max(AvlNode<T,K>* root){
        AvlNode<T,K>* current = root;
        while (current!= nullptr && current->right_son != nullptr  )
            current = current->right_son;

        return current;
    }

    AvlNode<T,K>* find_min(AvlNode<T,K>* root){
        AvlNode<T,K>* current = root;
        while (current!= nullptr && current->left_son != nullptr  )
            current = current->left_son;

        return current;
    }

    AvlNode<T,K>* select(AvlNode<T,K>* node,int k){
        if(!node){
            return nullptr;
        }
        int leftSub = 0;
        if(node->left_son){
            leftSub = node->left_son->num_of_subNodes;
        }
    int rank = leftSub +1;
    if(rank==k){
        return node;
    } else if (rank > k){
        return select(node->left_son,k);
    } else{
        return select(node->right_son,k -rank);
    }
    }

    void updateMax(){
        maxN = find_max(root);
    }
    void updateMin(){
        min = find_min(root);
    }
//***********************
    ~Avl_Tree(){
        if(this->root)
            clear_tree(this->root);
        size=0;
        root=nullptr;
    }
//***********************

    Avl_Tree &operator = (const Avl_Tree& tree) {
        if (this == &tree) return *this;
        if(this->root)
            clear_tree(this->root);
        root = copy_avl_helper(tree.root);
        this->updateMin();
        this->updateMax();
        size = tree.size;
        return *this;
    }
//***********************
};

#endif //UNTITLED2_AVL_TREE_H
