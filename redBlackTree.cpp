#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include<iostream>

using namespace std;

#define BLACK 0
#define RED 1

typedef struct tree_node
{
    struct tree_node *parent;
    struct tree_node *left;
    struct tree_node *right;
    int data;
    int color;
} *tree_pointer;

typedef struct rbtree
{
    tree_pointer root;
    tree_pointer NIL;
} *rbtree_pointer;

tree_pointer new_node(int data)
{
    tree_pointer n = (tree_pointer)malloc(sizeof(tree_node));

    n->parent = NULL;
    n->left = NULL;
    n->right = NULL;
    n->data = data;
    n->color = RED; // 새 노드느 RED

    return n;
}
rbtree_pointer new_rbtree()
{
    rbtree_pointer t = (rbtree_pointer)malloc(sizeof(rbtree));
    tree_pointer nil_node = (tree_pointer)malloc(sizeof(tree_node));

    nil_node->parent = NULL;
    nil_node->left = NULL;
    nil_node->right = NULL;
    nil_node->data = 0;
    nil_node->color = BLACK;
    t->NIL = nil_node;
    t->root = t->NIL;

    return t;
}

tree_pointer find_max(tree_pointer n)
{
    tree_pointer max = n;
    while(n->right != NULL)
    {
        max = find_max(n->right);
    }

    return max;
} 
tree_pointer find_successor(tree_pointer n)
{
    if(n->left == NULL)
        return NULL;
    else
        return find_max(n->left);
    
}

void left_rotation(rbtree_pointer t, tree_pointer x)
{
    tree_pointer y = x->right;
    x->right = y->left;

    if (y->left != t->NIL)
        y->left->parent = x;


    y->parent = x->parent;

    if (x->parent == t->NIL)
        t->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;


    y->left = x;
    x->parent = y;
}
void right_rotation(rbtree_pointer t, tree_pointer x)
{
    tree_pointer y = x->left;
    x->left = y->right;

    if (y->right != t->NIL)
        y->right->parent = x;

    y->parent = x->parent;

    if (x->parent == t->NIL)
        t->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;


    y->right = x;
    x->parent = y;
}

void insert_fixup(rbtree_pointer t, tree_pointer z)
{
    // 부모가 red일때 문제가 발생
    while (z->parent->color == RED)
    {
        // z->p == z->p->p->left_child 일때
        if (z->parent == z->parent->parent->left)
        {
            //y는 삼촌 노드
            tree_pointer y = z->parent->parent->right;

            //삼촌 노드가 RED 이면 z->p->p를 RED로, y와 p는 black으로
            if (y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;

                //recursive 하게 진행
                z = z->parent->parent;
            }
            //삼촌 노드가 BLACK이면 x가 p의 left, rigth에 따라 로테이션 처리
            else
            {
                
                if (z == z->parent->right)
                {
                    z = z->parent;
                    left_rotation(t, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotation(t, z->parent->parent);
            }
        }
        // z->p == z->p->p->right_child 일때
        else
        {
            //y는 삼촌 노드
            tree_pointer y = z->parent->parent->left;

            //삼촌 노드가 RED 이면 z->p->p를 RED로, y와 p는 black으로
            if (y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;

                //recursive 하게 진행
                z = z->parent->parent;
            }
            //삼촌 노드가 BLACK이면 x가 p의 left, rigth에 따라 로테이션 처리
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    right_rotation(t, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotation(t, z->parent->parent);
            }
        }
    }
    // 루트까지 RED로 설정 되는 것을 방지!!
    t->root->color = BLACK;
}
void insertion(rbtree_pointer t, tree_pointer z)
{

    tree_pointer y = t->NIL;
    tree_pointer temp = t->root;

    //BST 삽입
    while (temp != t->NIL)
    {
        y = temp;
        if (z->data < temp->data)
            temp = temp->left;
        else
            temp = temp->right;
    }
    z->parent = y;

    // z가 root 일때 처리
    if (y == t->NIL)
        t->root = z;
    // 나머지 처리
    else if (z->data < y->data)
        y->left = z;
    else
        y->right = z;

    // 삽입된 노드는 red
    z->left = t->NIL;
    z->right = t->NIL;
    z->color = RED;

    insert_fixup(t, z);
}

void deletion_fixup(rbtree_pointer t, tree_pointer z);
void deletion(rbtree_pointer t, tree_pointer z)
{
    //successor를 찾음
    tree_pointer successor = find_successor(z);

    //successor의 자리를 변환
    successor -> parent -> right = t->NIL;
    successor -> right = z -> right;
    successor -> left = z -> left;
    successor -> parent = z -> parent;



    if(z->color == RED){

    }

    free(z);


}

void preorder(rbtree_pointer t, tree_pointer n)
{
    if (n != t->NIL)
    {
        printf("%d ", n->data);
        preorder(t, n->left);
        preorder(t, n->right);
    }
}
void inorder(rbtree_pointer t, tree_pointer n)
{
    if (n != t->NIL)
    {
        inorder(t, n->left);
        printf("%d ", n->data);
        inorder(t, n->right);
    }
}
void postorder(rbtree_pointer t, tree_pointer n)
{
    if (n != t->NIL)
    {
        postorder(t, n->left);
        postorder(t, n->right);
        printf("%d ", n->data);
    }
}

void test(rbtree_pointer t, tree_pointer n)
{
    printf("preorder : ");
    preorder(t,n);
    printf("\n");
    
    printf("inorder : ");
    inorder(t,n);
    printf("\n");

    printf("postorder : ");
    postorder(t,n);
    printf("\n");
    return;
}

int main()
{
    // 새로운 레드-블랙 트리 생성
    rbtree_pointer t = new_rbtree();
    tree_pointer a, b, c, d, e, f, g, h, i, j, k, l, m;

    a = new_node(10);
    b = new_node(20);
    c = new_node(30);
    d = new_node(100);
    e = new_node(90);
    f = new_node(40);
    g = new_node(50);
    h = new_node(60);
    i = new_node(70);
    j = new_node(80);
    k = new_node(150);
    l = new_node(110);
    m = new_node(120);

    insertion(t, a);
    insertion(t, b);
    insertion(t, c);
    insertion(t, d);
    insertion(t, e);
    insertion(t, f);
    insertion(t, g);
    insertion(t, h);
    insertion(t, i);
    insertion(t, j);
    insertion(t, k);
    insertion(t, l);
    insertion(t, m);


    test(t, t->root);

    return 0;
}