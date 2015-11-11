/**********************************************************************
 * btree.cpp - CSCI124 - Ass5 - Contains BinaryTree class definition
 * <NAME> <STUDENT No.> <DATE>
 **********************************************************************/
#include <cstddef>
#include <cstring>
#include <iostream>
#include "btree.h"

using namespace std;

// Compare function for BinaryTree class - TreeDataType is defined in btree.h
int TreeDataCmp(const TreeDataType &D1,const TreeDataType &D2){ return strcmp(D1->Word,D2->Word);}

BinaryTree::BinaryTree()
{
	root = NULL;
}

BinaryTree::~BinaryTree()
{
	Kill();
}

void BinaryTree::Kill()
{
	PostOrderDelete(root);
	root = NULL;
}

void BinaryTree::PostOrderDelete(BinaryTree::nodePtr curr)
{
	// if tree isn't empty
	if (curr != 0)
	{
		// delete left branch
		PostOrderDelete(curr->left);
		
		// delete right branch
		PostOrderDelete(curr->right);

		delete curr;
	}
}

bool BinaryTree::Locate(const TreeDataType& data, TreeDataType& founddata)
{
	// find node
	nodePtr foundNode = FindNode(data, root);

	// return true and assign data if found	
	if (foundNode != 0)
	{
		founddata = foundNode->data;
		return true;
	}
	else
		return false;
}

BinaryTree::nodePtr BinaryTree::FindNode(const TreeDataType& data, BinaryTree::nodePtr root)
{
	// get comparison
	if (root == NULL) return 0;
	int Result = TreeDataCmp(data, root->data);
	
	// if the same return node
	if (Result == 0)
		return root;
	// else if smaller pursue the left tree
	else if (Result < 0 && root->left != 0)
		return FindNode(data, root->left);
	// else if larger pursue the right tree
	else if (Result > 0 && root->right != 0)
		return FindNode(data, root->right);
	return 0;
}


bool BinaryTree::Delete(const TreeDataType& data, TreeDataType& deldata)
{
	nodePtr parent, prev, curr = FindNode(data, root);

	if (curr == NULL)
		return false;
	deldata = curr->data;
	parent = curr->parent;
	if (curr->left == NULL) // no left subtree
	{
		if (curr->right == NULL) // no children
		{
			if (parent == NULL) // curr is root
				root = NULL;
			else if (parent->left == curr)
				parent->left = NULL;
			else
				parent->right = NULL;
		}
		else // only right subtree
		{
			if (parent == NULL)
			{
				root = curr->right;
				root->parent = NULL;
			}
			else if (parent->left == curr)
			{
				parent->left = curr->right;
				curr->right->parent = parent;
			}
			else
			{
				parent->right = curr->right;
				curr->right->parent = parent;
			}
		}
	}
	else if (curr->right == NULL) // only left subtree
	{
		if (parent == NULL)
		{
			root = curr->left;
			root->parent = NULL;
		}
		else if (parent->left == curr)
		{
			parent->left = curr->left;
			curr->left->parent = parent;
		}
		else
		{
			parent->right = curr->left;
			curr->left->parent = parent;
		}
	}
	else // has both subtrees
	{
		prev = curr->left;
		while (prev->right != NULL)
			prev = prev->right;
		prev->right = curr->right;
		curr->right->parent = prev;
		if (parent == NULL)
		{
			root = curr->left;
			root->parent = NULL;
		}
		else
		{
			parent->right = curr->left;
			curr->left->parent = parent;
		}
	}
	delete curr;
	return true;
}

void BinaryTree::Insert(const TreeDataType& data)
{
	InsertNode(data, root);
}

void BinaryTree::InsertNode(const TreeDataType& data, BinaryTree::nodePtr& root)
{
	if (root == NULL)
	{
		root = new node;
		root->data = data;
		root->left = NULL;
		root->right = NULL;
		root->parent = NULL;
	}
	else if (TreeDataCmp(data,root->data) <= 0)
	{
		if (root->left == NULL)
		{
			root->left = new node;
			root->left->data = data;
			root->left->left = NULL;
			root->left->right = NULL;
			root->left->parent = root;
		}
		else
			InsertNode(data,root->left);
	}
	else
	{
		if (root->right == NULL)
		{
			root->right = new node;
			root->right->data = data;
			root->right->left = NULL;
			root->right->right = NULL;
			root->right->parent = root;
		}
		else
 			InsertNode(data,root->right);
 	}
}

void BinaryTree::SetIterator()
{
	// set to root
	curr = root;

	// find left most leaf
	if (curr != 0)
		while (curr->left != 0)
			curr = curr->left;
}

TreeDataType BinaryTree::Next()
{
	TreeDataType rvalue = curr->data;

	
	// go into right subtree
	if (curr->right != 0)
	{
		curr = curr->right;
		while (curr->left != 0)
			curr = curr->left;
	}
	// if no parent then we are finished
	else if (curr->parent == 0)
		curr = 0;
	// else find ancestor
	else
	{
		while (curr->parent != 0 && (curr->parent)->left != curr)
			curr = curr->parent;
		curr = curr->parent;
	}
	return rvalue;
}

bool BinaryTree::More()
{
	return (curr != 0);
}
