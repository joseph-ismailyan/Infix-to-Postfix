// Joseph Ismailyan
// infix to postfix converter and evaluator

#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

struct stack{
    char data;
    stack *next;
};

struct aStack{
    int data;
    aStack* next;
};

struct queue{
    char data;
    queue *next;
};

void aPop(aStack*&);
void aPush(int, aStack*&);
void pop(stack*&);
void push(char, stack*&);
void enqueue(char, queue*&, queue*&);
void dequeue(queue*&, queue*&);
void convertToPostfix(char*);
stack* createStack(char);
queue* createQueue(char);
queue* createRear(queue*&);
void traverseStack(stack*&);
void traverseQueue(queue*&);
void printOriginalEq(char*);
bool compPrecedence(char*, char);
bool isEmpty(stack*&);
bool isSymbol(char*);
bool symbol(char);

ifstream in("input.txt");

int main(){
    char A[20];

   if(!in) {
        cout << "File was not opened successfully" << endl;
        cout << "Please create or modify input.txt" << endl;
    return 0;
    }

    while(in)
        in >> A;
    printOriginalEq(A);
    convertToPostfix(A);
    in.close();
    cout << endl << endl << "Exiting.. " << endl;
return 0;
}

bool isEmpty(stack *&top){
    if(!top)
        return true;
    else
        return false;
}

void pop(stack *&top){

    stack *temp;

    if(top){
        temp = top;
        top = top->next;
        delete temp;
    }
}

void push(char data, stack *&top){

    stack *newstack = new stack;

    newstack->data = data;
    newstack->next = top;
    top = newstack;

}

void aPop(aStack *&top){

    aStack *temp;

    if(top){
        temp = top;
        top = top->next;
        delete temp;
    }
}

void aPush(int data, aStack *&top){

    aStack *newaStack = new aStack;

    newaStack->data = data;
    newaStack->next = top;
    top = newaStack;

}

void enqueue(char data, queue *&front, queue *&rear){

    queue *newqueue = new queue;

    newqueue->data = data;
    newqueue->next = NULL;

    if (rear){
        rear->next = newqueue;
        rear = newqueue;
    }
    else
        front = rear = newqueue;
}

void dequeue(queue *&front, queue *&rear){
    queue* temp;
    if(front){
        temp = front;
        front = front->next;
        delete temp;
    }
    if(!front)
        rear = NULL;
}

stack* createStack(char A){
    stack* top = new stack;
    top->data = A;
    top->next = NULL;
    return top;
}

queue* createQueue(char A){
    queue* front = new queue;
    queue* rear = front;
    front->data = A;
    front->next = NULL;
    return front;
}

queue* createRear(queue *&front){
    queue* rear = front;
    return rear;
}

void traverseStack(stack *&top){
    stack *temp;
    temp = top;
    while(top){
        cout << top->data;
        top = top->next;
    }
    top = temp;
}

void traverseQueue(queue *&front){
    queue* temp;
    temp = front;
    while(front){
        cout << front->data;
        front = front->next;
    }
    front = temp;
}
void printOriginalEq(char *A){
    int i = 0;
    cout << "The equation to process is: ";
        while(*(A+i) != '\0'){
            cout << *(A+i);
            i++;
    }
    cout << endl;
}

bool isSymbol(char *A){
    if((*A) == ('^' || '*' || '/' || '+' || '-' || '(' || '='))
        return true;
    else
        return false;
}
bool symbol(char A){
    if((A) == ('^' || '*' || '/' || '+' || '-' || '(' || '='))
        return true;
    else
        return false;
}
bool compPrecedence(char A, char B){
    int weightA, weightB;
    if (A == B)
        return false;
    if(A == '*' || A == '/')
        weightA = 4;
    if(A == '+' || A == '-')
        weightA = 3;
    if(A == '(')
        weightA = 2;
    if(A == '=')
        weightA = 1;
    if(B == '\0')
        return true;
    if(B == '*' || B == '/')
        weightB = 4;
    if(B == '+' || B == '-')
        weightB = 3;
    if(B == '(')
        weightB = 2;
    if(B == '=')
        weightB = 1;
    if(weightA > weightB)
        return true;
    else
        return false;
}

void convertToPostfix(char *A){


    stack* top = NULL;
    queue* front = NULL;
    queue* rear = NULL;
    int i = 0;
    while(*(A+i) != '\0'){
            if(isdigit(*(A+i))){
                enqueue(*(A+i), front, rear);

            }
            else if (*(A+i) == '(')
                    push(*(A+i), top);
            else if (*(A+i) == ')') {
                while (!isEmpty(top)) {
                    if (top->data == '(') {
                            pop(top);
                            break;
                        }
                    enqueue(top->data, front, rear);
                    pop(top);
                }
            }
            else{
                while((!isEmpty(top) && top->data != '(') && (compPrecedence(*(A+i), top->data) == false)){
                        enqueue(top->data, front, rear);
                        pop(top);
                   }
                    push(*(A+i), top);
                }
        i++;
    }
    while (!isEmpty(top)) {
        enqueue(top->data, front, rear);
        pop(top);
    }
    cout << "The processed equation is: ";
    traverseQueue(front);
    //delete top;
    //delete front;
    //delete rear;

    cout << endl << "Now evaluating the expression." << endl;
    aStack* aTop = NULL;
    char rhs[0];
    char lhs[0];
    int intRHS = 0;
    int intLHS = 0;
    char data2[20];
    int answer = 0;
    int toPush;
    char charAnswer[20];
    *data2 = front->data;
    toPush = atoi(data2);
    aPush(toPush, aTop);
    front = front->next;
    while(aTop){
        if(front){
            while(((front->data) != '*') && ((front->data) != '+') && ((front->data) != '-') && ((front->data) != '/')){
                *data2 = front->data;
                toPush = atoi(data2);
                aPush(toPush, aTop);
                dequeue(front, rear);
            }

            intRHS = aTop->data;
            aPop(aTop);
            intLHS = aTop->data;
            aPop(aTop);

            if(front->data == '+'){
                answer = (intLHS + intRHS);
                front = front->next;
            }
            else if(front->data == '-'){
                answer = (intLHS - intRHS);
                front = front->next;
            }
            else if(front->data == '*'){
                answer = (intLHS* intRHS);
                front = front->next;
            }
            else if(front->data == '/'){
                answer = (intLHS / intRHS);
                front = front->next;
            }
            aPush(answer, aTop);
        }
        else
            break;
    }
    cout << "The answer is: " << answer;
    aPop(aTop);
}
