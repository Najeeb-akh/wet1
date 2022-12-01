#ifndef AVL_NODE_H
#define AVL_NODE_H

#include <stdbool.h>
#include <string.h>
#include <iostream>
//#include <memory>


template <class T>
class AVLnode{

public:

    T* info;
    AVLnode<T>* left; 
    AVLnode<T>* right;  
    AVLnode<T>* parent; 
    bool visited; 
    int height;


    /** AVLnode: a constructor of the AVLnode class
     * 
     * @param info - the target info for the AVLnode to be initialized with  
     */ 
    explicit AVLnode(T* info);

    /* ~AVLnode: a destructor of the AVLnode class*/
    ~AVLnode() = default;

    /** AVLnode: a default copy constructor of the AVLnode class 
     * 
     * @param node - the target node to be copied
     */ 
    AVLnode(const AVLnode<T>& node)=default;
    
    /** Info: an info getter of the AVLnode class
     * 
     * @return the info dereference of the local AVLnode
     */ 
    T Info();

    /**
     * InfoPtr: an info ptr getter of the AVLnode class
     * 
     * @return the info ptr of the local AVLnode
     */
    T* InfoPtr();

    /**
     * ClearNode: clears the local node by deleting it and setting its
     *            right, left and parent pointers to nullptr
     */
    void ClearNode();

    /** Left: a left ptr getter of the AVLnode class
     * 
     * @return the left ptr of the local AVLnode
     */ 
    AVLnode<T>* Left() const;

    /** Right: a right ptr getter of the AVLnode class
     * 
     * @return the right ptr of the local AVLnode
     */ 
    AVLnode<T>* Right() const;

    /** Parent: a parent ptr getter of the AVLnode class
     * 
     * @return the parent ptr of the local AVLnode
     */  
    AVLnode<T>* Parent() const;

    /** Height: a height getter of the AVLnode class
     * 
     * @return the height of the local AVLnode
     */ 
    int Height() const;

    /** setInfo: an info setter of the AVLnode class
     * 
     * @param info - the target info ptr to be set with
     */ 
    void setInfo(T* info);

    /** setRight: a right ptr setter of the AVLnode class
     * 
     * @param new_right - the target right ptr to be set with
     */
    void setRight(AVLnode<T>* new_right);
    
    /** setLeft: a left ptr setter of the AVLnode class
     * 
     * @param new_left - the target left ptr to be set with
     */    
    void setLeft(AVLnode<T>* new_left);

    /** setParent: a parent ptr setter of the AVLnode class
     * 
     * @param new_parent - the target parent ptr to be set with
     */
    void setParent(AVLnode<T>* new_parent);
    
    /** setHeight: a height setter of the AVLnode class
     * 
     * @param new_height - the target height to be set with
     */ 
    void setHeight(int new_height);

    /** isVisited: a boolianic function to check whether the local AVLnide has been vosoted or not
     * 
     * @return true, in case the local AVLnode has been visited. Otherwise, false.
     */ 
    bool isVisited() const;

    /** setVisited:a Visited setter of the AVLnode class
     * 
     * @param flag - the target flag to be set with
     */ 
    void setVisited(bool flag);


};

 //----------------------------------    END OF CLASS SCOPE  -------------------------------------//








//----------------------------------  METHODS  IMPLEMENTAION  -----------------------------------//


template <class T>
AVLnode<T>::AVLnode(T* info): info(info)
{
   right=nullptr;
   left=nullptr;
   parent=nullptr;
   height=0;
   visited=false;
}


template <class T>
void AVLnode<T>::ClearNode()
{
    delete info;
    right=nullptr;
    left=nullptr;
    parent=nullptr;
}


template <class T>
T AVLnode<T>::Info() 
{
    return *info;
}

template <class T>
T* AVLnode<T>::InfoPtr()
{
    return info;
}

template <class T>
bool AVLnode<T>::isVisited() const
{
    return visited;
}

template <class T>
void AVLnode<T>::setVisited(bool flag) 
{
    this->visited=flag;
}

template <class T>
AVLnode<T>* AVLnode<T>::Left() const
{
    return this->left;
}

template <class T>
AVLnode<T>* AVLnode<T>::Right() const
{
    return this->right;
}

template <class T>
int AVLnode<T>::Height() const
{
    return this->height;
}


template <class T>
AVLnode<T>* AVLnode<T>::Parent() const
{
    return this->parent;
}

template <class T>
void AVLnode<T>::setInfo(T* info)
{
    this->info = info;
}

template <class T>
void AVLnode<T>::setLeft(AVLnode<T>* new_left)
{
    this->left=new_left;
}

template <class T>
void AVLnode<T>::setRight(AVLnode<T>* new_right)
{
    this->right=new_right;
}


template <class T>
void AVLnode<T>::setParent(AVLnode<T>* new_parent)
{
    this->parent=new_parent;
}

template <class T>
void AVLnode<T>::setHeight(int new_height)
{
    this->height=new_height;
}   


#endif /* AVLnode_H_ */