#include <stdio.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1000
#define MAX_STACK_SIZE 1000
#define MAX_TOKEN_SIZE 5

char * getToken(char *, int *);
int isOperator(char *);
int precedence(char *);
int associativity(char *);
char * getDerivative(char *);
char * combine(char *, char, char *);
char * copy(char *, char *);
char * ncopy (char *, char *, int);
int compare(char *, char *);
int ncompare(char *,char *,int);
int length(char *);
void push(char **, int *, char *);
char * pop(char **, int *);
int isEmpty(int);
char * top(char **, int);



int main()
{
    char * expression,*input_long,c='a';
    int input_len=0,k=0,i = 0;
    int postfixStackTop = -1, infixStackTop = -1, derivativeStackTop = -1, pos = 0, itemCount = 0;
    char ** postfix = (char **)malloc(MAX_STACK_SIZE * sizeof(char *));
    char ** postfixStack = (char **)malloc(MAX_STACK_SIZE * sizeof(char *));
    char ** infixStack = (char **)malloc(MAX_STACK_SIZE * sizeof(char *));
    char ** derivativeStack = (char **)malloc(MAX_STACK_SIZE * sizeof(char *));

    /*input*/
    input_long=(char *)calloc(MAX_INPUT_SIZE,sizeof(char));

    while(c!=EOF)
    {
        c = getchar();
        if(c==32||c==10||c==EOF) continue;
        else input_long[input_len++]=c;
    }

    expression=(char *)calloc(input_len+1,sizeof(char));
    for(k=0; k<input_len; k++) expression[k]=input_long[k];
    free(input_long);
    expression[input_len]='\0';
    /*printf("--%s--\n",expression);*/


    while (pos < MAX_INPUT_SIZE && expression[pos] != '\0')
    {
        char * token;
        token = (char *)calloc(5, sizeof(char));

        if (expression[pos] == '(')
        {
            copy(token, "(");
            ++pos;
        }
        else if (expression[pos] == ')')
        {
            copy(token, ")");
            ++pos;
        }
        else if (expression[pos] == '^')
        {
            copy(token, "^");
            ++pos;
        }
        else if (expression[pos] == '*')
        {
            copy(token, "*");
            ++pos;
        }
        else if (expression[pos] == '/')
        {
            copy(token, "/");
            ++pos;
        }
        else if (expression[pos] == '+')
        {
            copy(token, "+");
            ++pos;

        }
        else if (expression[pos] == '-')
        {
            copy(token, "-");
            ++pos;
        }
        else if (expression[pos] == 'X')
        {
            copy(token, "X");
            ++pos;
        }
        else if (ncompare(expression + pos, "sin", 3) == 0)
        {
            copy(token, "sin");
            pos += 3;
        }
        else if (ncompare(expression + pos, "cos", 3) == 0)
        {
            copy(token, "cos");
            pos += 3;
        }
        else if (ncompare(expression + pos, "tan", 3) == 0)
        {
            copy(token, "tan");
            pos += 3;
        }
        else if (ncompare(expression + pos, "sh", 2) == 0)
        {
            copy(token, "sh");
            pos += 2;
        }
        else if (ncompare(expression + pos, "ch", 2) == 0)
        {
            copy(token, "ch");
            pos += 2;
        }
        else if (ncompare(expression + pos, "ln", 2) == 0)
        {
            copy(token, "ln");
            pos += 2;
        }
        else
        {
            int end = pos;
            while (expression[end] >= 48 && expression[end] <= 57) end++;
            ncopy(token, expression + pos, (end - pos));
            pos = end;
        }

        /*push(stack, &stackTop, token); */

        if (isOperator(token))
        {
            char *topToken = top(postfixStack, postfixStackTop);
            while (!isEmpty(postfixStackTop) && compare(topToken, "(") != 0 && precedence(token) < precedence(topToken))
            {
                postfix[itemCount++] = pop(postfixStack, &postfixStackTop);
                topToken = top(postfixStack, postfixStackTop);
            }
            if(!isEmpty(postfixStackTop) && compare(topToken, "(") != 0 && precedence(token) == precedence(topToken) && associativity(token)==0)
            {
                postfix[itemCount++] = pop(postfixStack, &postfixStackTop);
                topToken = top(postfixStack, postfixStackTop);
            }
            push(postfixStack, &postfixStackTop, token);
        }
        else if (compare(token, "(") == 0)
        {
            push(postfixStack, &postfixStackTop, token);
        }
        else if (compare(token, ")") == 0)
        {
            while (!isEmpty(postfixStackTop))
            {
                char * popped = pop(postfixStack, &postfixStackTop);
                /*printf("%s",popped);*/
                if (compare(popped, "(") == 0)
                {
                    break;
                }
                postfix[itemCount++] = popped;
            }
        }
        else
        {
            postfix[itemCount++] = token;
        }
    }

    while (!isEmpty(postfixStackTop))
    {
        postfix[itemCount++] = pop(postfixStack, &postfixStackTop);
    }

    /*for(i=0;i<itemCount;i++)printf("%s",*(postfix+i));*/

    for (i = 0; i < itemCount; i++)
    {
        char * token = postfix[i];
        if (isOperator(token))
        {
            char Operator = token[0];
            char * op1, * op2, * dop1, * dop2, * newToken, * newDerivativeToken, * newDerivativeToken1, * newDerivativeToken2, * newDerivativeTokentemp, * newDerivativeTokentemp2;
            op2 = pop(infixStack, &infixStackTop);
            op1 = pop(infixStack, &infixStackTop);

            newToken = combine(op1, Operator, op2);

            push(infixStack, &infixStackTop, newToken);

            dop2 = pop(derivativeStack, &derivativeStackTop);
            dop1 = pop(derivativeStack, &derivativeStackTop);

            if (Operator == '+' || Operator == '-')
            {
                newDerivativeToken = combine(dop1, Operator, dop2);
                push(derivativeStack, &derivativeStackTop, newDerivativeToken);
            }
            else if (Operator == '*')
            {
                newDerivativeToken1 = combine(dop1, Operator, op2);
                newDerivativeToken2 = combine(op1,Operator,dop2);
                newDerivativeToken = combine(newDerivativeToken1, '+',newDerivativeToken2);
                push(derivativeStack, &derivativeStackTop, newDerivativeToken);
            }
            else if (Operator == '^')
            {
                int N = atoi(op2);
                char * N2=(char * )calloc(3,sizeof(char));
                sprintf(N2, "%d", N-1);
                newDerivativeToken1 = combine(op1,Operator,N2);
                newDerivativeToken2 = combine(op2,'*',dop1);
                newDerivativeToken = combine(newDerivativeToken1,'*',newDerivativeToken2);
                push(derivativeStack, &derivativeStackTop, newDerivativeToken);
            }
            else if (Operator == '/')
            {
                newDerivativeToken1 = combine(dop1, '*', op2);
                newDerivativeToken2 = combine(op1,'*',dop2);
                newDerivativeTokentemp = combine(newDerivativeToken1, '-',newDerivativeToken2);
                newDerivativeTokentemp2 = combine(op2,'^',"2");
                newDerivativeToken = combine(newDerivativeTokentemp,Operator,newDerivativeTokentemp2);
                push(derivativeStack, &derivativeStackTop, newDerivativeToken);
            }
        }
        else
        {
            push(infixStack, &infixStackTop, token);
            push(derivativeStack, &derivativeStackTop, getDerivative(token));
        }
    }
    printf("%s\n",pop(derivativeStack,&derivativeStackTop));
    /*FREE*/
    free(postfix);
    free(postfixStack);
    free(infixStack);
    free(derivativeStack);

    return 0;
}


char *copy(char *where, char *from)
{
    char *flag = where;
    int i=0;
    for(i=0; from[i] != '\0'; i++)
    {
        where[i] = from[i];
    }
    where[i] = '\0';
    return flag;
}

char *ncopy (char *where, char *from, int num)
{
    char *flag = where;
    int i=0;

    for(i=0; from[i]!='\0'&&num!=0; i++,num--)
    {
        where[i] = from[i];
    }
    where[i] = '\0';
    return flag;
}


int compare(char *c1, char *c2)
{
    int a=0;
    while(*c1)
    {
        if(*c1!=*c2)
        {
            a=1;
            break;
        }
        c1++;
        c2++;
    }
    return a;
}

int ncompare(char *c1,char *c2,int n)
{
    int a=0;
    while(n!=0)
    {
        if (*c1=='\0') a=0;
        if (*c1 == *c2)
        {
            ++c1;
            ++c2;
        }
        else if (*c1!=*c2) a=1;
        n--;
    }
    return a;
}

int length(char *s)
{
    int len;
    for (len=0; *s!='\0'; len++)s++;
    return len;
}

int isOperator(char * token)
{
    if (compare(token, "+") == 0 || compare(token, "-") == 0 || compare(token, "^") == 0 || compare(token, "*") == 0
            || compare(token, "/") == 0)
    {
        return 1;
    }

    return 0;
}

int precedence(char * token)
{
    if (compare(token, "+") == 0 || compare(token, "-") == 0)
    {
        return 1;
    }
    else if (compare(token, "*") == 0 || compare(token, "/") == 0)
    {
        return 2;
    }
    else if (compare(token, "^") == 0)
    {
        return 3;
    }
    return 0;
}
/*X-2*X-3*X-4*X
(1-(((0*X)+(2*1))-(((0*X)+(3*1))-((0*X)+(4*1)))))*/
int associativity(char * token)
{
    int a=1;
    if (compare(token, "+") == 0 || compare(token, "-") == 0 || compare(token, "*") == 0 || compare(token, "/") == 0) a=0;
    else a=1;
    return a;
}

char *combine(char *op1, char Operator, char *op2)
{
    char *newToken;
    int op1Length, op2Length;
    op1Length = length(op1);
    op2Length = length(op2);
    newToken = (char *)calloc(op1Length + op2Length + 4, sizeof(char));
    newToken[0] = '(';
    copy(newToken + 1, op1);
    newToken[op1Length + 1] = Operator;
    copy(newToken + op1Length + 2, op2);
    newToken[op1Length + op2Length + 2] = ')';
    /*printf("%s\n",newToken);*/
    return newToken;
}


char *getDerivative(char *token)
{
    if (compare(token, "ln") == 0)
    {
        return "(1/X)";
    }
    else if (compare(token, "ch") == 0)
    {
        return "sh";
    }
    else if (compare(token, "sh") == 0)
    {
        return "ch";
    }
    else if (compare(token, "sin") == 0)
    {
        return "cos";
    }
    else if (compare(token, "X") == 0)
    {
        return "1";
    }
    else if (compare(token, "tan") == 0)
    {
        return "(1+tan^2)";
    }
    else
    {
        return "0";
    }
}

/* Stack */


void push(char ** stack, int * top, char * item)
{
    stack[++*top] = item;
}

char * pop(char ** stack, int * top)
{
    return stack[(*top)--];
}

int isEmpty(int top)
{
    if(top == -1) return 1;

    return 0;
}

char * top(char ** stack, int top)
{
    return stack[top];
}
