//#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "AVL_NODE.h"
//#include "utilities.h"

#define ILLEGAL_BF 2


template <class T>
class AVLtree{

    AVLnode<T>* root;
    int num_of_elements;
    AVLnode<T>* max_element;
    AVLnode<T>* min_element;


public:
    
    /* AVLtree: a constructor of the AVLtree class */
    explicit AVLtree();
    
    /* ~AVLtree: a destructor of the AVLtree class*/
    ~AVLtree();

    /** AVLtree: a copy constructor of the AVLtree class 
     * 
     * @param tree - the target tree to be copied
     */ 
    AVLtree(const AVLtree& tree);

    /** ClearTree: clears all the nodes of the tree
     * 
     * @param root - the root of the target tree to be cleared
     */
    void ClearTree(AVLnode<T>* root);

    /**
     * clear: clears the tree (deletes + sets ptrs to nullptr) 
     *        using ClearTree method
     */
    void clear();

    /** calcHeight: calculates the height of the given node
     * 
     * @param node - the target node to calculate its height
     * 
     * @return the new caculated height of the given node
     */  
    int calcHeight(AVLnode<T>* node) const;

    /** BalanceFactor: calculates the balance factor of the givent node
     * 
     * @param node - target node to calculate its balance factor
     * 
     * @return the new balance factor of the given node
     */ 
    int BalanceFactor(AVLnode<T>* node) const;

    /** NumOfElements: a numOfElements getter of the AVLtree class
     * 
     * @return the numOfElements of the local tree
     */ 
    int NumOfElements() const;

    /** setNumOfElements: a numOfElements setter of the AVLtree class
     * 
     * @param num_of_elements - the target numOfElements to be set with
     */
    void setNumOfElements(int num_of_elements);

    /** setMaxElement: a MaxElement ptr setter of the AVLtree class
     * 
     * @param max - the target MaxElement ptr to be set with
     */
    void setMaxElement(AVLnode<T>* max);
    
    /** setMinElement: a MinElement ptr setter of the AVLtree class
     * 
     * @param min - the target MinElement ptr to be set with
     */
    void setMinElement(AVLnode<T>* min);

    /** LL_Rotation: rotates the tree with the given root according to the LL case
     * 
     * @param root - the root of the target tree to be rotated
     * 
     * @return the new root of after the rotation
     */ 
    AVLnode<T>* LL_Rotation(AVLnode<T>* root);
    
    /** RR_Rotation: rotates the tree with the given root according to the RR case
     * 
     * @param root - the root of the target tree to be rotated
     * 
     * @return the new root of after the rotation
     */
    AVLnode<T>* RR_Rotation(AVLnode<T>* root);
    
    /** LR_Rotation: rotates the tree with the given root according to the LR case
     * 
     * @param root - the root of the target tree to be rotated
     * 
     * @return the new root of after the rotation
     */
    AVLnode<T>* LR_Rotation(AVLnode<T>* root);
    
    /** RL_Rotation: rotates the tree with the given root according to the RL case
     * 
     * @param root - the root of the target tree to be rotated
     * 
     * @return the new root of after the rotation
     */
    AVLnode<T>* RL_Rotation(AVLnode<T>* root);
    
    /** Insert: calls the wrapping function with the given info and the root of the local tree
     * 
     * @param info - the target info to be inserted to the local tree
     */ 
    void Insert(T* info);

    /** Insert: inserts a new node containing the given info to the tree rooted with the given root 
     * 
     * @param info - target info to be added to the tree
     *        root - the root of the target tree to insert the info into
     * 
     * @return the new root of the tree after the insertion
     */ 
    AVLnode<T>* Insert(T* info, AVLnode<T>* root);

    /** FindMaxElement: returns a ptr of the node that contains the max info in 
     *                  the tree rooted with the given node. 
     * 
     * @param node - the root of the target tree to search for the min element in
     * 
     * @return a ptr pointing at the node of the min element
     */ 
    AVLnode<T>* FindMaxElement(AVLnode<T>* node);

    /** FindMinElement: returns a ptr of the node that contains the min info in 
     *                  the tree rooted with the given node. 
     * 
     * @param node - the root of the target tree to search for the min element in
     * 
     * @return a ptr pointing at the node of the min element
     */ 
    AVLnode<T>* FindMinElement(AVLnode<T>* node);

    /** Find: calls the wrapping function with the given value and the root of the local tree
     * 
     * @param value - the target info to be found in the local tree
     */
    T* Find(const T& value) const;

    /** Find: searches for the node with the given value in the tree rooted with the given root
     *        and returns the ptr pointing at it
     * 
     * @param root - the root of the target tree to search for the value in it
     *        value - the target vlue to be searched for
     * 
     * @return the ptr pointing at the node with the given value
     */ 
    AVLnode<T>* Find(AVLnode<T>*root, const T& value) const;    
    
    /** getRoot: a root getter in the AVLtree class
     * 
     * @return a ptr pointing at the root of the local tree
     */ 
    AVLnode<T>* getRoot() const;

    /** MaxElement: a maxElement getter in the AVLtree class
     * 
     * @return a ptr pointing at the maxElement in the local tree
     */
    AVLnode<T>* MaxElement() const;

    /** MaxElementInfo: a getter of maxElement's info in the AVLtree class
     * 
     * @return a ptr pointing at the info of maxElement in the local tree
     */
    T* MaxElementInfo() const;
    
    /** MinElement: a minElement getter in the AVLtree class
     * 
     * @return a ptr pointing at the minElement in the local tree
     */
    AVLnode<T>* MinElement() const;

    /** setRoot: a root setter in the AVLtree class
     * 
     * @param new_root - target root to be set with
     */ 
    void setRoot(AVLnode<T>* new_root);

    /** operator<: checks whether the local tree is smaller than the given tree according to 
     *             the instructions of the project 
     * 
     * @param tree - target tree for the local tree to be compared with
     * 
     * @return true, in case the local tree is smaller than the given tree.
     *         Otherwise, false.
     */ 
    bool operator<(const AVLtree<T>& tree) const; 

    /** operator==: checks the equality between the local tree and the given tree according to 
     *             the instructions of the project 
     * 
     * @param tree - target tree for the local tree to be compared with
     * 
     * @return true, in case the local tree is equal to the given tree.
     *         Otherwise, false.
     */ 
    bool operator==(const AVLtree<T>& tree) const; 

    /** operator>: checks whether the local tree is bigger than the given tree according to 
     *             the instructions of the project 
     * 
     * @param tree - target tree for the local tree to be compared with
     * 
     * @return true, in case the local tree is bigger than the given tree.
     *         Otherwise, false.
     */ 
    bool operator>(const AVLtree<T>& tree) const;


    /**
     * DeleteNode: calls the wrapping function with the given info and the root of the local tree
     * 
     * @param info - the target info to be deleted from the local tree
     */
    void DeleteNode(const T* info);

    /**
     * DeleteNode: deletes the node (and it's info) containing the given info of the tree 
     *             rooted with the given node
     * 
     * @param node - the root of the target tree to delete the info from
     * @param info - target info to be deleted from the tree
     * @param flag - flag helper to complete the deletion
     * @param counter - counter helper to complete the deletion
     * 
     * @return - the new root of the tree 
     */
    AVLnode<T>* DeleteNode(AVLnode<T>* node, const T* info, bool flag, int counter);

    /**
     * ClearTreeKeepHead: calls the wrapping function with the given root and then sets
     *                    the ptrs to nullptr
     * 
     * @param root - root of the target tree to be cleared 
     */
    void ClearTreeKeepHead(AVLnode<T>* root);

    /**
     * ClearTreeKeepHeadAux: clears the tree (without deleting the inner info of the nodes)
     * 
     * @param root - root of the target tree to be cleared 
     */
    void ClearTreeKeepHeadAux(AVLnode<T>* root);

    /**
     * DeleteActiveNode: calls the wrapping function with the given info and the local root
     * 
     * @param info - target info of the node to be deleted in the local tree
     */
    void DeleteActiveNode(const T* info);


    /**
     * deleteActiveNode: deletes the node containing the given info of the tree rooted with the given node
     * 
     * @param info - target info of the node to be deleted in the tree rooted with node
     */
    AVLnode<T>* deleteActiveNode(AVLnode<T>* node, const T* info);




};

 //----------------------------------    END OF CLASS SCOPE  -------------------------------------//







//----------------------------------  METHODS  IMPLEMENTAION  -----------------------------------//

template <class T>
AVLtree<T>::AVLtree()
{
    this->root=nullptr;
    this->num_of_elements=0;
    this->max_element=root;
    this->min_element=root;;
}

template <class T>
AVLtree<T>::~AVLtree()
{
    ClearTree(this->root);

    this->num_of_elements = 0;
    this->root = nullptr;
    this->max_element = nullptr;
    this->min_element = nullptr;
    
}

template <class T>
void AVLtree<T>::ClearTree(AVLnode<T>* root)
{
    if(root==nullptr)
    {
        return;
    }

    ClearTree(root->Left());
    ClearTree(root->Right());
    root->ClearNode();
    delete root;
    root = nullptr;
}

template <class T>
void AVLtree<T>::ClearTreeKeepHeadAux(AVLnode<T>* root)
{
    if(root==nullptr)
    {
        return;
    }

    ClearTreeKeepHeadAux(root->Left());
    ClearTreeKeepHeadAux(root->Right());

    delete root;
    root = nullptr;
}


template <class T>
void AVLtree<T>::ClearTreeKeepHead(AVLnode<T>* root)
{
    ClearTreeKeepHeadAux(root);

    this->num_of_elements = 0;
    this->root = nullptr;
    this->max_element = nullptr;
    this->min_element = nullptr;
}

template <class T>
void AVLtree<T>::clear()
{
    ClearTree(this->root);

    this->num_of_elements = 0;
    this->root = nullptr;
    this->max_element = nullptr;
    this->min_element = nullptr;
}


template <class T>
AVLtree<T>::AVLtree(const AVLtree& tree)
{
    this->root=tree.root;
    this->num_of_elements=tree.num_of_elements;
    this->max_element=tree.max_element;
    this->min_element=tree.min_element;

}

template <class T>
int AVLtree<T>::NumOfElements() const
{
    return num_of_elements;
}

template <class T>
void AVLtree<T>::setNumOfElements(int num_of_elements) 
{
    this->num_of_elements=num_of_elements;
}

template <class T>
void AVLtree<T>::setMaxElement(AVLnode<T>* max) 
{
    this->max_element=max;
}

template <class T>
void AVLtree<T>::setMinElement(AVLnode<T>* min) 
{
    this->min_element=min;
}
    

template <class T> 
int AVLtree<T>::calcHeight(AVLnode<T>* node) const
{
    if(node->Left() && node->Right())
    {
        return max(node->Left()->Height(), node->Right()->Height())+1;
    }

    else if(node->Left() && node->Right() == nullptr)
    {
        return node->Left()->Height() + 1;
    }

    else if(node->Left() ==nullptr && node->Right())
    {
        return node->Right()->Height() + 1;
    }
    return 0;
}


template <class T>
int AVLtree<T>::BalanceFactor(AVLnode<T>* root) const
{
    int balance = 0;
    if(root->Left()!=nullptr && root->Right()!=nullptr)
    {
        balance = root->Left()->Height() - root->Right()->Height();
    }

    else if(root->Left()!=nullptr && root->Right()==nullptr)
    {
        balance = root->Left()->Height() +1;
    }

    else if (root->Right()!=nullptr)
    {
        balance = -1 -(root->Right()->Height());
    }
    return balance;     
}

template <class T>
AVLnode<T>* AVLtree<T>::LL_Rotation(AVLnode<T>* root)
{
    if(root==nullptr)
    {
        return nullptr;  
    }

    AVLnode<T>* new_root = root->Left();  
    AVLnode<T>* LR_sub = new_root->Right();  
    
    new_root->setRight(root);
    new_root->setParent(nullptr);
    root->setParent(new_root);
    root->setLeft(LR_sub);
    
    if(LR_sub!=nullptr)
    {
        LR_sub->setParent(root);    
    }
   
    root->setHeight(calcHeight(root));
    new_root->setHeight(calcHeight(new_root));

    return new_root; 
}


template <class T>
AVLnode<T>* AVLtree<T>::RR_Rotation(AVLnode<T>* root)
{
    if(root==nullptr){
        return nullptr;  
    }
    AVLnode<T>* new_root = root->Right();  
    AVLnode<T>* RL_sub = new_root->Left();  
  
    new_root->setLeft(root);
    new_root->setParent(nullptr);
    root->setParent(new_root);
    root->setRight(RL_sub);
    if(RL_sub!=nullptr){
        RL_sub->setParent(root);    
    }


    root->setHeight(calcHeight(root));
    new_root->setHeight(calcHeight(new_root));

    return new_root; 
}

template <class T>
AVLnode<T>* AVLtree<T>::RL_Rotation(AVLnode<T>* root)
{
        
    AVLnode<T>* R_sub=root->Right();
    AVLnode<T>* RL_sub=R_sub->Left();
    root->setRight(RL_sub->Left());
    if((RL_sub->Left())!=nullptr){
        (RL_sub->Left())->setParent(root);    
    }
    R_sub->setLeft(RL_sub->Right());
    if(RL_sub->Right()!=nullptr){
        (RL_sub->Right())->setParent(R_sub);
    }

    AVLnode<T>* new_root=RL_sub;
    new_root->setLeft(root);
    new_root->setParent(nullptr);
    root->setParent(new_root);
    new_root->setRight(R_sub);
    if(R_sub!=nullptr){
        R_sub->setParent(new_root);  
        R_sub->setHeight(calcHeight(R_sub));  
    }
    root->setHeight(calcHeight(root));
    new_root->setHeight(calcHeight(new_root));
    

    return new_root; 
}

template <class T>
AVLnode<T>* AVLtree<T>::LR_Rotation(AVLnode<T>* root)
{
        
    AVLnode<T>* L_sub=root->Left();
    AVLnode<T>* LR_sub=L_sub->Right();
    root->setLeft(LR_sub->Right());
    if((LR_sub->Right())!=nullptr){
        (LR_sub->Right())->setParent(root);    
    }
    L_sub->setRight(LR_sub->Left());
    if((LR_sub->Left())!=nullptr){
        (LR_sub->Left())->setParent(L_sub);    
    }

    AVLnode<T>* new_root=LR_sub;
    new_root->setRight(root);
    new_root->setParent(nullptr);
    root->setParent(new_root);
    new_root->setLeft(L_sub);
    if(L_sub!=nullptr){
        L_sub->setParent(new_root);  
        L_sub->setHeight(calcHeight(L_sub));  
    }

    root->setHeight(calcHeight(root));
    new_root->setHeight(calcHeight(new_root));

        
    return new_root; 
}
template <class T>
void AVLtree<T>::Insert(T* info)
{
    this->root = Insert(info,root);
    num_of_elements++;
    max_element = FindMaxElement(root);
    min_element = FindMinElement(root);
}

template <class T>
AVLnode<T>* AVLtree<T>::Insert(T* info, AVLnode<T>* curr_root)
{
    if(curr_root == nullptr)
    {
        AVLnode<T>* ptr = new AVLnode<T>(info);
        curr_root = ptr;
        return curr_root;         
    }
    else if (*info < curr_root->Info()){
        AVLnode<T>* left_child=Insert(info, curr_root->Left());
        curr_root->setLeft(left_child); 
        if(left_child!=nullptr){
            left_child->setParent(curr_root);
        }

    }
    else if (*info > curr_root->Info()){ 
        AVLnode<T>* right_child=Insert(info, curr_root->Right());
        curr_root->setRight(right_child);
        if(right_child!=nullptr){
            right_child->setParent(curr_root);
        }  
    }

    curr_root->setHeight(calcHeight(curr_root));

    if (BalanceFactor(curr_root) ==ILLEGAL_BF && BalanceFactor(curr_root->Left())>=0){
        return LL_Rotation(curr_root);  
    }
  
    if (BalanceFactor(curr_root) ==-ILLEGAL_BF && BalanceFactor(curr_root->Right())<=0){  
        return RR_Rotation(curr_root);  
    }

    if (BalanceFactor(curr_root) ==ILLEGAL_BF && BalanceFactor(curr_root->Left())==-1){  
        return LR_Rotation(curr_root);  
    }  
  
    if (BalanceFactor(curr_root) == -ILLEGAL_BF && BalanceFactor(curr_root->Right())==1){  
        return RL_Rotation(curr_root);  
    }  

    return curr_root;

}


template <class T>
AVLnode<T>* AVLtree<T>::FindMaxElement(AVLnode<T>* node)
{
    AVLnode<T>* ptr=node;
    while (ptr->Right() !=nullptr)
    {
        ptr = ptr->Right();
    }
    return ptr;    
}

template <class T>
AVLnode<T>* AVLtree<T>::FindMinElement(AVLnode<T>* node)
{
    AVLnode<T>* ptr=node;

    while (ptr->Left() !=nullptr)
    {
        ptr = ptr->Left();
    }
    return ptr;  
}

template <class T>
AVLnode<T>* AVLtree<T>::getRoot() const
{
    return this->root;

}

template <class T>
AVLnode<T>* AVLtree<T>::MaxElement() const
{
    return max_element;

}

template <class T>
T* AVLtree<T>::MaxElementInfo() const
{
    if (max_element)
    {
        return max_element->InfoPtr();
    }
    return nullptr;
}


template <class T>
AVLnode<T>* AVLtree<T>::MinElement() const
{
    return min_element;
}

template <class T>
void AVLtree<T>::setRoot(AVLnode<T>* new_root)
{
    this->root=new_root;
}


template <class T>
AVLnode<T>* AVLtree<T>::Find(AVLnode<T>*root, const T& value) const
{
    if(root!=nullptr)
    {
        if( value> *root->InfoPtr())
        {
            return Find(root->Right(),value);
        }
        else if(value< *root->InfoPtr())
        {
            return Find(root->Left(),value);
        }
        else
        {
            return root;
        }
    }

    return nullptr;
}

template <class T>
T* AVLtree<T>::Find(const T& value) const
{
    AVLnode<T>* node = Find(this->root,value);
    if(node != nullptr)
    {
        return node->InfoPtr();
    }
    return nullptr;
}


template <class T>
bool AVLtree<T>::operator==(const AVLtree<T>& tree) const
{
    return this->root->Info() == tree.root->Info();
}


template <class T>
bool AVLtree<T>::operator<(const AVLtree<T>& tree) const
{
    return this->root->Info() < tree.root->Info();

}

template <class T>
bool AVLtree<T>::operator>(const AVLtree<T>& tree) const
{
    return this->root->Info() > tree.root->Info();
}


template <class T>
void AVLtree<T>::DeleteNode(const T* info)
{
    this->root = DeleteNode(this->root, info, false, 0);
    num_of_elements--;
    if(root == nullptr)
    {
        min_element = nullptr;
        max_element = nullptr;
    }
    else
    {
        max_element = FindMaxElement(root);
        min_element = FindMinElement(root);
    }
}


template <class T>
AVLnode<T>* AVLtree<T>::DeleteNode(AVLnode<T>* node, const T* info, bool flag, int counter) {

    if (node == nullptr) 
    {
        return nullptr;
    }

    if (node->right == nullptr && node->left == nullptr) 
    {
        if (node == this->root)
        {
            delete node->info;
            delete node;
            return nullptr;
        }

        if (*(node->info) == *info) 
        {
            if (!flag)
            {
                delete node->info;
            }
            delete node;
        }
        return nullptr;
    }

    if (*(node->info) > *info) 
    {
        node->left = DeleteNode(node->left, info, flag, counter);
        if (node->left != nullptr)
        {
            node->left->parent = node;
        }
    }

    else if (*(node->info) < *info) 
    {
        node->right = DeleteNode(node->right, info, flag, counter);
        if (node->right != nullptr)
        {
            node->right->parent = node;
        }
    }

    else 
    {
        flag = true;
        if (node->right != nullptr) 
        {
            AVLnode<T>* leaf = node->right;
            while (leaf->left != nullptr) 
            {
                leaf = leaf->left;
            }

            if(counter == 0)
            {
                delete node->info;
            }
            
            node->info = leaf->info;

            counter++;
            node->right = DeleteNode(node->right, leaf->info ,flag, counter);

            if(node->right != nullptr)
            {
                node->right->parent = node;
            }
        }

        else 
        {
            AVLnode<T>* leaf = node->left;
            while (leaf->right != nullptr) 
            {
                leaf = leaf->right;
            }
            
            if(counter == 0)
            {
                delete node->info;
            }

            node->info = leaf->info;

            counter++;
            node->left = DeleteNode(node->left, leaf->info, flag, counter);

            if(node->left != nullptr)
            {
                node->left->parent = node;
            }
        }
    }

    node->height = calcHeight(node);
    int bf = BalanceFactor(node);
    if (bf == 2 && BalanceFactor(node->left) >= 0) {
        node = LL_Rotation(node);
    }
    if (bf == 2 && BalanceFactor(node->left) == -1) {
        node = LR_Rotation(node);
    }
    if (bf == -2 && BalanceFactor(node->right) <= 0) {
        node = RR_Rotation(node);
    }
    if (bf == -2 && BalanceFactor(node->right) == 1) {
        node = RL_Rotation(node);
    }

    return node;
}



template <class T>
void AVLtree<T>::DeleteActiveNode(const T* info)
{
    this->root = deleteActiveNode(this->root, info);
    num_of_elements--;
    if(root == nullptr)
    {
        min_element = nullptr;
        max_element = nullptr;
    }
    else
    {
        max_element = FindMaxElement(root);
        min_element = FindMinElement(root);
    }
}


template <class T>
AVLnode<T>* AVLtree<T>::deleteActiveNode(AVLnode<T>* node, const T* info) {

    if (node == nullptr) 
    {
        return nullptr;
    }

    if (node->right == nullptr && node->left == nullptr) 
    {
        if (*(node->info) == *info) 
        {
            delete node;
        }
        return nullptr;
    }

    if (*(node->info) > *info) 
    {
        node->left = deleteActiveNode(node->left, info);
        if (node->left != nullptr)
        {
            node->left->parent = node;
        }
    }

    else if (*(node->info) < *info) 
    {
        node->right = deleteActiveNode(node->right, info);
        if (node->right != nullptr)
        {
            node->right->parent = node;
        }
    }

    else 
    {
        if (node->right != nullptr) 
        {
            AVLnode<T>* leaf = node->right;
            while (leaf->left != nullptr) 
            {
                leaf = leaf->left;
            }

            node->info = leaf->info;

            node->right = deleteActiveNode(node->right, leaf->info);

            if(node->right != nullptr)
            {
                node->right->parent = node;
            }
        }

        else 
        {
            AVLnode<T>* leaf = node->left;
            while (leaf->right != nullptr) 
            {
                leaf = leaf->right;
            }
            
            node->info = leaf->info;

            node->left = deleteActiveNode(node->left, leaf->info);

            if(node->left != nullptr)
            {
                node->left->parent = node;
            }
        }
    }

    node->height = calcHeight(node);
    int bf = BalanceFactor(node);
    if (bf == 2 && BalanceFactor(node->left) >= 0) {
        node = LL_Rotation(node);
    }
    if (bf == 2 && BalanceFactor(node->left) == -1) {
        node = LR_Rotation(node);
    }
    if (bf == -2 && BalanceFactor(node->right) <= 0) {
        node = RR_Rotation(node);
    }
    if (bf == -2 && BalanceFactor(node->right) == 1) {
        node = RL_Rotation(node);
    }

    return node;
}




//#endif /*AVLTREE_H_*/