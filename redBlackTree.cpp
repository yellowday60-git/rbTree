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
    n->color = RED; // μ λΈλλ RED

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
    // λΆλͺ¨κ° redμΌλ λ¬Έμ κ° λ°μ
    while (z->parent->color == RED)
    {
        // z->p == z->p->p->left_child μΌλ
        if (z->parent == z->parent->parent->left)
        {
            //yλ μΌμ΄ λΈλ
            tree_pointer y = z->parent->parent->right;

            //μΌμ΄ λΈλκ° RED μ΄λ©΄ z->p->pλ₯Ό REDλ‘, yμ pλ blackμΌλ‘
            if (y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;

                //recursive νκ² μ§ν
                z = z->parent->parent;
            }
            //μΌμ΄ λΈλκ° BLACKμ΄λ©΄ xκ° pμ left, rigthμ λ°λΌ λ‘νμ΄μ μ²λ¦¬
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
        // z->p == z->p->p->right_child μΌλ
        else
        {
            //yλ μΌμ΄ λΈλ
            tree_pointer y = z->parent->parent->left;

            //μΌμ΄ λΈλκ° RED μ΄λ©΄ z->p->pλ₯Ό REDλ‘, yμ pλ blackμΌλ‘
            if (y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;

                //recursive νκ² μ§ν
                z = z->parent->parent;
            }
            //μΌμ΄ λΈλκ° BLACKμ΄λ©΄ xκ° pμ left, rigthμ λ°λΌ λ‘νμ΄μ μ²λ¦¬
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
    // λ£¨νΈκΉμ§ REDλ‘ μ€μ  λλ κ²μ λ°©μ§!!
    t->root->color = BLACK;
}
void insertion(rbtree_pointer t, tree_pointer z)
{

    tree_pointer y = t->NIL;
    tree_pointer temp = t->root;

    //BST μ½μ
    while (temp != t->NIL)
    {
        y = temp;
        if (z->data < temp->data)
            temp = temp->left;
        else
            temp = temp->right;
    }
    z->parent = y;

    // zκ° root μΌλ μ²λ¦¬
    if (y == t->NIL)
        t->root = z;
    // λλ¨Έμ§ μ²λ¦¬
    else if (z->data < y->data)
        y->left = z;
    else
        y->right = z;

    // μ½μλ λΈλλ red
    z->left = t->NIL;
    z->right = t->NIL;
    z->color = RED;

    insert_fixup(t, z);
}

void deletion_fixup(rbtree_pointer t, tree_pointer x, tree_pointer p, tree_pointer s, tree_pointer l, tree_pointer r)
{
    //root μ²΄ν¬
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

        //pμμ κ°μ νμ λ°μ
        deletion_fixup(t, p, p->parent, p->parent->right, p->parent->left, p->parent->right);
        return;
    }
    return;
}
void deletion(rbtree_pointer t, tree_pointer z)
{
    //successorλ₯Ό μ°Ύμ
    tree_pointer m = find_successor(t, z);
    
    //successorμ κ°μΌλ‘ λ°κΏ
    z->data = m->data;
    
    //                  p
    //                 / \
    //                /   \
    //               /     \
    //              m       s
    //               \     / \
    //                \   /   \
    //                 x l      r

    //μ­μ  λΈλκ° RED μΌλ
    if(m->color == RED){
        m->parent->left = m-> right;
        m->right->parent = m->parent;
        free(m);
        return;
    }
    
    //μ­μ  λΈλκ° BLACKμ΄κ³  μ μΌν μμμ΄ RED μΌλ
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

    //mμ΄ BLACKμ΄κ³  xλ BLACK μΌλ
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
    // μλ‘μ΄ λ λ-λΈλ νΈλ¦¬ μμ±
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