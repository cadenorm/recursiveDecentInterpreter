
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include <ctype.h>
#define MAXSIZE 50

char * lex;
void get(char** lex){
    (*lex)++;
    printf("get() called. New token is: %c\n",**lex);
    if(*lex==(char*)' '){
        if ((*lex)-1 == (char*)'-' && (*lex)+1 == (char*)'>'){
            printf("Error: whitespace between '-' and '>' .");
        }else if((*lex)-1 == (char*)':' && (*lex) == (char*)'='){
            printf("Error: whitespace between ':' and '=' .");
        }else{
            get((char**)lex); //recursive get call to move past whitespace
        }

    }
};
int top=-1;
char item;
char stack[MAXSIZE];
int isFull(){
    if (top==MAXSIZE-1){
        return 1;
    }else{
        return 0;
    }
}
int isEmpty(){
    if(top==-1){
        return 1;
    }else{
        return 0;
    }
}
void push(char item){
    if(isFull()){
        printf("Stack is full.");
        return;
    }

    top++;
    stack[top] = item;
}
char pop(){
    if(isEmpty()){
        printf("Stack is empty");
        return 0;
    }
    item = stack[top];
    top--;
    return item;
}

struct varTable{
    char vars[MAXSIZE];
    char vals[MAXSIZE];
    int next;
};
struct varTable v;
void placeVar(char var, char val){
    if (v.next != MAXSIZE - 1){
        v.vars[v.next] = var;
        v.vals[v.next] = val;
        v.next++;
    }else{printf("Error: Not enough memory for variable assignment");}
}
int lookupVar(char var){
    for(int i=0;i<=v.next;i++){
        if(v.vars[i]==var){
            return v.vals[i];
        }
    }
    printf("Error: Variable not previously declared.");
    return -1;
}
bool VA();
bool IT();
bool CT();
bool IT_TAIL();
bool CT_TAIL();
bool L();
bool A();
bool B(){
    if (VA()){
        if(IT()){
            if(*lex == '.'){
                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }else{
        return false;
    }
}
bool VA(){
    char vartemp;
    if(*lex == '#'){
        get(&lex);
        if (islower(*lex)){
            vartemp = *lex;
            get(&lex);
            if(*lex == ':'){
                get(&lex);
                if(*lex == '='){
                    get(&lex);
                    if(IT()){
                        if(*lex == ';'){
                            char valtemp = pop();
                            placeVar(vartemp,valtemp);
                            get(&lex);
                            if(VA()){
                                return true;
                            }else{
                                return false;
                            }
                        }else{
                            return false;
                        }
                    }else{
                        return false;
                    }
                }else{
                    return false;
                }
            }else{
                return false;
            }
        }else{
            return false;
        }

    }else if(*lex == '~'){ //['~','T','F','('])
        return true;
    }else if(*lex == 'T'){
        return true;
    }else if(*lex == 'F'){
        return true;
    }else if(*lex == '('){
        return true;
    }else if(islower(*lex)){
        int varDef = lookupVar(*lex);
        if(varDef != -1){
            return true;
        }
        else{
            return false;
        }
    }else{
        return false;
    }
}
bool IT(){
    if(CT()){
        if(IT_TAIL()){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}
bool IT_TAIL(){
    if (*lex == '-'){
        get(&lex);
        if(*lex == '>'){
            get(&lex);
            char* right = (char*)lex;
            char left = pop();
            if(left == 'F'){
                push('T');
            }else{
                if(*right == 'T'){
                    push('T');
                }else{
                    push('F');
                }
            }
            if(CT()){
                char* right = (char*)lex;
                char left = pop();
                if(left == 'F'){
                    push('T');
                }else{
                    if(*right == 'T'){
                        push('T');
                    }else{
                        push('F');
                    }
                }
                if(IT_TAIL()){
                    return true;
                }else{
                    return false;
                }
            }else{
                return false;
            }
        }else{
            return false;
        }
    }else if (*lex == '.'){
        return true;
    }else if (*lex == ';'){
        return true;
    }else if (*lex == ')'){
        return true;
    }else{
        return false;
    }
}
bool CT(){
    if(L()){
        get(&lex);
        if(CT_TAIL()){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}
bool CT_TAIL(){
    if (*lex == 'V'){
        get(&lex);
        if(L()){
            char right = pop();
            char left = pop();
            if (right == 'T' || left == 'T'){
                push('T');
            }else{
                push('F');
            }
            get(&lex);
            if(CT_TAIL()){
                return true;
            }
            else{
                return false;
            }
        }else{
            return false;
        }
    }else if (*lex == '^'){
        get(&lex);
        if (L()){
            char right = pop();
            char left = pop();
            if (right == 'T' && left == 'T'){
                push('T');
            }else{
                push('F');
            }
            get(&lex);
            if(CT_TAIL()){
                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }else if (*lex == '.'){
        return true;
    }else if (*lex == ';'){
        return true;
    }else if (*lex == ')'){
        return true;
    }else{
        if(*lex == '-'){
            if(*lex+1=='>'){
               return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
        return false;
    }
}
bool L(){
    if(A()){
        return true;
    }
    else if (*lex == '~'){
        get(&lex);
        if(L()){
            char neg =pop();
            if(neg == 'F'){
                push('T');
            }else{
                push('F');
            }
            return true;
        }else{
            return false;
        }
    }
    else{
        return false;
    }
}
bool A(){
    if (*lex == 'T'){
        push('T');
        return true;
    }else if(*lex == 'F'){
        push('F');
        return true;
    }else if(*lex == '('){
             get(&lex);
             if(IT()){
                get(&lex);
                if(*lex == ')'){
                    return true;
                }else{
                    return false;
                }
             }
             else{
                return false;
             }

    }else{
        if(islower(*lex)){
            if(lookupVar(*lex)==-1){
                    return false;
            }else{
                push(lookupVar(*lex)); //if var is in the variable table push the value of var on to the stack
                return true;
            }
        }else{
            return false;
        }
    }

}
int main()
{
    char testStr[] = "#a:=T;#b:=F;b.";
    lex =testStr;
    printf("Lex initialized. Start token: %c",*lex);

    if(B()){
        char result = pop();
        printf("Valid Syntax and the value of the expression is: %c",result);
    }else{
       printf("Invalid Syntax");
    }
}
