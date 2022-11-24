#ifndef AVL_NODE_H
#define AVL_NODE_H
#include <iostream>
//#include "DSW_CAR.h"

using std::endl;
class Key {
public:
    int typeIde;
    int modelId;
    int score;
    Key(int typeID=0,int modelId=0,int score=0):typeIde(typeID),modelId(modelId),score(score) {}
    Key(const Key &key){
        typeIde = key.typeIde;
        modelId = key.modelId;
        score = key.score;
    }
    Key& operator=(const Key &key){
        if(*this==key)
            return *this;
        typeIde = key.typeIde;
        modelId = key.modelId;
        score = key.score;
        return *this;
    }
    bool operator>(const Key & k)
    {
        if(score > k.score){
            return true;
        }

        if (score == k.score){
            if(typeIde == k.typeIde){
                if (modelId > k.modelId){
                    return true;
                }
             }
            return typeIde > k.typeIde;
        }
        return false;
    }
    bool operator<(const Key & k)
    {
        return !operator>(k);
    }
    bool operator==(const Key & k)
    {
        return typeIde == k.typeIde && modelId==k.modelId && score==k.score;
    }
    bool operator!=(const Key & k)
    {
        return !operator==(k);
    }
    friend std::ostream& operator<<(std::ostream& os,const Key key){
        os << key.score << endl;
        return os;
    }

};


class Keys {
public:
    int typeIde;
    int modelId;
    int sells;
    Keys(int typeID=0,int modelId=0,int sells=0):typeIde(typeID),modelId(modelId),sells(sells) {}
    Keys(const Keys &key){
        typeIde = key.typeIde;
        modelId = key.modelId;
        sells = key.sells;
    }
    Keys& operator=(const Keys &key){
        if(*this==key)
            return *this;
        typeIde = key.typeIde;
        modelId = key.modelId;
        sells = key.sells;
        return *this;
    }
    bool operator>(const Keys & k)
    {
       if (sells > k.sells){
           return true;
       }
        if (sells == k.sells ){
            if(typeIde < k.typeIde){
                return true;
            }
            if(typeIde == k.typeIde ){
                if(modelId < k.modelId){
                    return true;
                }
                if(modelId > k.modelId){
                    return false;
                }
            }
            return false;
        }
        return false;
    }
    bool operator<(const Keys & k)
    {
        return !operator>(k);
    }
    bool operator==(const Keys & k)
    {
        return typeIde == k.typeIde && modelId==k.modelId && sells==k.sells;
    }
    bool operator!=(const Keys & k)
    {
        return !operator==(k);
    }
    friend std::ostream& operator<<(std::ostream& os,const Keys key){
        os << key.sells << endl;
        return os;
    }

};


template<class T,class K>
class AvlNode {
public:
    K  key;
    T value;
    int height;
    AvlNode *parent;
    AvlNode *right_son;
    AvlNode *left_son;
    int balance_factor;
    int num_of_subNodes;
    AvlNode() {

    }

    AvlNode(K key,T data, int height = 0, AvlNode *parent = nullptr,
                 AvlNode *right_son = nullptr, AvlNode *left_son = nullptr,
                 int balance_factor = 0, int num_of_subNodes=1) : key(key),value(data), height(height),
                                                                           parent(parent), right_son(right_son),
                                                                           left_son(left_son),
                                                                           balance_factor(balance_factor),num_of_subNodes(num_of_subNodes){

    }

//*********
    ~AvlNode() {}


//*********
    bool operator==(AvlNode<T,K>& n)
    {
        return (this->key) == (n.key);
    }

//*********
    bool operator<(AvlNode<T,K>& n)
    {
        return (this->key) < (n.key);
    }

//*********
    bool operator>(AvlNode<T,K>& n)
    {
        return (this->key) > (n.key);
    }
//*********
    void UpdateParent(AvlNode<T,K>* parent){
        if(!this){
            return;
        }
        this->parent = parent;
    }
    void updateRightSon(AvlNode<T,K>* right){
        if(!this){
            return;
        }
        this->right_son = right;
        if(right){
            right->UpdateParent(this);
        }
    }

    void updateLeftSon(AvlNode<T,K>* left){
        if(!this){
            return;
        }
        this->left_son = left;
        if(left){
            left->UpdateParent(this);
        }
    }
};
#endif //UNTITLED2_AVLNODE_H
