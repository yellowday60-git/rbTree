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

tree_pointer find_min(rbtree_pointer t, tree_pointer n)
{
    tree_pointer min = n;

    if(n->left == t->NIL)
        return min;

    min = find_min(t, n->left);
    return min;
} 
tree_pointer find_successor(rbtree_pointer t, tree_pointer n)
{
    if(n->right == t->NIL)
        return n;
    else
        return find_min(t, n->right);
    
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

void deletion_fixup(rbtree_pointer t, tree_pointer x, tree_pointer p, tree_pointer s, tree_pointer l, tree_pointer r)
{
    //root 체크
    if(p == t->root)
        return;

    //case 1-1
    if(p->color == RED && s->color == BLACK && l->color == BLACK && r->color == BLACK)
    {
        p->color = BLACK;
        s->color = RED;
        return;
    }

    //case *-2
    if(s->color == BLACK && r->color == RED)
    {
        s->color = p->color;
        p->color = BLACK;
        r->color = BLACK;
        left_rotation(t, p);
        return;
    }

    //case *-3
    if(s->color == BLACK && l->color == RED && r->color == BLACK)
    {
        l->color = BLACK;
        s->color = RED;
        right_rotation(t, s);

        //case *-2
        l->color = p->color;
        p->color = BLACK;
        s->color = BLACK;
        left_rotation(t, p);

        return;
    }

    //case 2-4
    if(p->color == BLACK && s->color == RED)
    {
        p->color = RED;
        s->color = BLACK;
        left_rotation(t, p);
        return;
    }

    //case 2-1
    if(p->color == BLACK && s->color == BLACK && l->color == BLACK && r->color == BLACK)
    {

        s->color = RED;

        //p에서 같은 현상 발생
        deletion_fixup(t, p, p->parent, p->parent->right, p->parent->left, p->parent->right);
        return;
    }
    return;
}
void deletion(rbtree_pointer t, tree_pointer z)
{
    //successor를 찾음
    tree_pointer m = find_successor(t, z);
    
    //successor의 값으로 바꿈
    z->data = m->data;
    
    //                  p
    //                 / \
    //                /   \
    //               /     \
    //              m       s
    //               \     / \
    //                \   /   \
    //                 x l      r

    //삭제 노드가 RED 일때
    if(m->color == RED){
        m->parent->left = m-> right;
        m->right->parent = m->parent;
        free(m);
        return;
    }
    
    //삭제 노드가 BLACK이고 유일한 자식이 RED 일때
    if(m->color == BLACK && m->right->color == RED)
    {
        m->parent->left = m-> right;
        m->right->parent = m->parent;
        m->right->color = BLACK;
        free(m);
        return;
    }

    tree_pointer x = m->right;
    tree_pointer p = m->parent;
    tree_pointer s = p->right;
    tree_pointer l = s->left;
    tree_pointer r = r->right;
    p->left = x;
    x->parent = p;

    //m이 BLACK이고 x도 BLACK 일때
    deletion_fixup(t, x, p, s, l, r);
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
    d = new_node(40);
    e = new_node(50);
    f = new_node(60);
    g = new_node(70);
    h = new_node(80);
    i = new_node(90);
    j = new_node(100);
    k = new_node(110);
    l = new_node(120);
    m = new_node(130);

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

    printf("### insertion test ###\n");
    test(t, t->root);

    deletion(t, d);
    deletion(t, a);
    deletion(t, c);
    deletion(t, e);
    deletion(t, g);
    deletion(t, i);
    deletion(t, k);
    deletion(t, l);
    deletion(t, m);

    printf("### deletion test ###\n");
    test(t, t->root);

    return 0;
}