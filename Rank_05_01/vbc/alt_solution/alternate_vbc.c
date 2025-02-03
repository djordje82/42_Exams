#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct Node{
	char		op;
	int			value;
	struct Node	*left, *right;
} Node;

Node *parseString(char **input);
Node *parseMultiplication(char **input);

void	freeAST(Node *node)
{
	if (node)
	{
		freeAST(node->left);
		freeAST(node->right);
		free(node);
	}
}
void	unexpected(char c)
{
	if (c)
		printf("unexpected token '%c'\n", c);
	else
		printf("unexpected end of input\n");
}

Node	*createNode(char op, int value, Node *left, Node *right)
{
	Node *node=malloc(sizeof(Node));
	if (!node)
	{
		freeAST(left);
		freeAST(right);
		return NULL;
	}
	node->op=op;
	node->value = value;
	node->left=left;
	node->right=right;
	return node;
}

int	evaluateAST(Node *node)
{
	if (!node->op)
		return node->value;
	int	leftVal = evaluateAST(node->left);
	int	rightVal = evaluateAST(node->right);
	if (node->op == '*')
		return leftVal * rightVal;
	return leftVal + rightVal;
}

Node *parseBrackets(char **input)
{
	Node *node = NULL;
	if (**input == '(')
	{
		(*input)++;
		node = parseString(input);
		if (!node)
			return NULL;
		if (**input != ')')
		{
//			printf("unbalanced brackets\n");
			unexpected(**input);
			freeAST(node);
			return NULL;
		}
		(*input)++;
	}
	else if (isdigit(**input))
	{
		node = createNode('\0', **input - '0', NULL, NULL);
		if (!node)
			return NULL;
		(*input)++;
	}
	else
	{
		unexpected(**input);
		return NULL;
	}
	return node;
}
Node *parseMultiplication(char **input)
{
	Node *node = parseBrackets(input);
	if (!node)
		return NULL;
	while (**input == '*')
	{
		char op = **input;
		(*input)++;
		Node *right = parseBrackets(input);
		if (!right)
		{
			freeAST(node);
			return NULL;
		}
		Node *new = createNode(op, 0, node, right);
		if (!new)
		{
			freeAST(right);
			freeAST(node);
			return NULL;
		}
		node = new;
	}
	return node;
}
Node *parseString(char **input)
{
	Node *node = parseMultiplication(input);
	if (!node)
		return NULL;
	while (**input == '+')
	{
		char op = **input;
		(*input)++;
		Node *right = parseMultiplication(input);
		if (!right)
		{
			freeAST(node);
			return NULL;
		}
		Node *new = createNode(op, 0, node, right);
		if (!new)
		{
			freeAST(right);
			freeAST(node);
			return NULL;
		}
		node = new;
	}
	return node;
}

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("Usage: ./vbc <expression>\n");
		return 1;
	}
	char *ptr = av[1];
	Node *ast = parseString(&ptr);
	if (!ast)
		return 1;
	if (*ptr != '\0' && *ptr != '\n')
	{
		unexpected(*ptr);
		freeAST(ast);
		return 1;
	}
	printf("%d\n", evaluateAST(ast));
	freeAST(ast);
	return 0;
}