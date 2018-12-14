#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE 100
#define FALSE 0
#define TRUE 1

typedef struct Node{
    char* item;
    struct Node *next;
}Node;

Node *searchList(Node *head, char* item);
Node *predecessorNode(Node* head, char *s);
Node *get_tail(Node *cur);
Node *partition(Node *head, Node *end, Node **nHead, Node **nTail);
Node *rQuick(Node *head, Node *tail);
void deleteNode(Node **head, char *s);
void get_strings(Node **head);
void insertList(Node **head, char*s);
char *next_input();
int pop(void);
int peek(void);
void push(int);
int str_cmp(const char *s1, const char *s2);
int l_str_cmp(const char *s1, const char *s2);
void reverseShortened(char *s);
int str_len(char *s1);
void print_list(Node *head);
void sort_strings(Node **head);

int main(){
    Node *head = malloc(sizeof(Node));
    head->item = next_input();
    head->next = NULL;
    get_strings(&head);
    sort_strings(&head);
    print_list(head);
}

void sort_strings(Node **head){
    *head = rQuick(*head, get_tail(*head));
}

/* get_tail: returns a pointer to the last node in the list */
Node *get_tail(Node *cur){
    while(cur != NULL && cur->next != NULL){
        cur = cur->next;
    }
    return cur;
}

Node *partition(Node *head, Node *end, Node **nHead, Node **nTail){
    Node *pivot = end;
    Node *prev = NULL, *cur = head, *tail = pivot;

    while(cur != pivot){
        if(l_str_cmp(cur->item, pivot->item) < 0){
            //string in curr is less than pivot, thus we make cur the new head
            if(*nHead == NULL){
                *nHead = cur;
            }
            prev = cur;
            cur = cur->next;
        }
        else{

            //string in curr is greater than pivot
            if(prev != NULL){
                prev->next = cur->next;
            }
            //put curr after tail and update tail to reflect that
            Node *p = cur->next;
            cur->next = NULL;
            tail->next = cur;
            tail = cur;
            cur = p;
        }
    }

    if(*nHead == NULL){
        /*this means that the pivot was never greater than cur, and thus
        must be the smallest element in the current portion of the linked list.
        Thus we know nHead should be our pivot. */
        *nHead = pivot;
    }
    //We must updated the reference to the real tail node with our local tail variable
    *nTail = tail;

    return pivot;
}

Node *rQuick(Node *head, Node *tail){
    if(head == NULL || head == tail){
        return head;
    }

    Node *nHead = NULL;
    Node *nTail = NULL;

    Node *pivot = partition(head, tail, &nHead, &nTail);

    if(nHead != pivot){
        /*as we mentioned in partition, this means the pivot is not the smallest element
        in our list. If nHead was indeed equal to pivot it would be pointless to attempt
        to sort elements to the left of the pivot, and thus we skip this part */
        Node *p = nHead;
        while(p->next != pivot){
            p = p->next;
        }
        p->next = NULL;

        nHead = rQuick(nHead, p);

        //We make the pivot the new tail for this portion of the list
        p = get_tail(nHead);
        p->next = pivot;
    }

    //Now we have to sort the portion to the right of our partition
    pivot->next = rQuick(pivot->next, nTail);

    return nHead;
}

Node *searchList(Node *head, char* item){
    if (head == NULL) return NULL;

    while(head->next != NULL){
        head = head->next;
        if(str_cmp(head->item, item) == 0){
            return head;
        }
    }
    return NULL;
}

void print_list(Node *head){
    while(head->next != NULL){
        printf("%s\n", head->item);
        head = head->next;
    }
    printf("%s", head->item);
}

/* insertList: appends a node which contains string s to the front of the list */
void insertList(Node **head, char *s){
    Node *temp = malloc(sizeof(Node));
    int item_size = str_len(s);
    //printf("%d\n", item_size);
    //temp->item = malloc(sizeof(char) * item_size);
    temp->item = s;
    temp->next = *head;
    *head = temp;
}

/* insertEnd: appends a node which contains string s to the end of the list */
void insertEnd(Node **head, char *s){
    Node *to_insert = malloc(sizeof(Node));
    Node *temp = get_tail(*head);
    temp->next = to_insert;
    to_insert->item = s;
    to_insert->next = NULL;
}

/* predecessorNode: returns a pointer to the node before the node that contains string s */
Node *predecessorNode(Node *head, char *s){
    if((head == NULL) || (head->next == NULL)){
        printf("Error: predecessor sought on null list\n");
        return(NULL);
    }
    if(str_cmp((head->next)->item, s) == 0){
        return head;
    }
    else{
        return(predecessorNode((head->next), s));
    }
}

void deleteNode(Node **head, char *s){
    Node *temp;
    Node *pred;

    temp = searchList(*head, s);
    if(temp != NULL){
        pred = predecessorNode(*head, s);
        if (pred == NULL){
            *head = temp->next;
        }
        else{
            pred->next = temp->next;
        }
        free(temp);
    }

}

void get_strings(Node **head){
    char *input;
    while((input = next_input()) != NULL){
        insertEnd(head, input);
    }
}

/* reverse: reverses string s in place */
void reverseShortened(char *s){
    /* we use this to reverse the string that we pop off the stack to get it back into the
    correct form, otherwise it would be backwards */

    //printf("\n Pre-reverse: %s\n", s);
    for (int i = 0, c = 0, j = str_len(s)-1; i<j; i++, j--){
        c = s[i], s[i] = s[j], s[j] = c;
    }
    //printf("\nPost reverse %s\n", s);
}

/* str_len: return the length of the string */
int str_len(char *s1){
    int i;
    for(i = 0; s1[i] != '\0'; i++);
    return i;
}

int sp = 0; /* next free stack position */
int val[BUFSIZE]; /* value stack */

/* push: push x onto the stack */
void push(int x){
    if (sp < BUFSIZE){
        val[sp++] = x;
    }
    else{
        return;
    }
}

int pop(void){
    if (sp > 0){
        return val[--sp];
    }
    else{
        return NULL;
    }
}

int peek(void){
    if (sp > 0){
        return val[sp-1];
    }
    else{
        return NULL;
    }
}

int str_cmp(const char* s1, const char* s2){
    /* thought we were sorting by length AND alphabetically but I guess that is not the case
    so this is a waste */

    int l_s1 = str_len(s1), l_s2 = str_len(s2);
    if(l_s1 > l_s2){
        return 1;
    }
    else if(l_s1 < l_s2){
        return -1;
    }
    while(*s1 && (*s1==*s2)){
        s1++, s2++;
    }
    return *(const unsigned char*)s1-*(const unsigned char*)s2;
}

int l_str_cmp(const char *s1, const char *s2){
    int l_s1 = str_len(s1), l_s2 = str_len(s2);
    return ((l_s1 > l_s2) ? 1 : -1);
}

char *next_input(){
    int c, i;
    int end = FALSE;
    char *input;
    if(pop() == -TRUE){
        return NULL;
    }

    while((c = getchar()) != EOF){
        if(c == ';'){
            break;
        }
        push(c);
    }

    if(c == ';' && sp == 0){
        input = malloc(sizeof(char));
        input[0] = '\0';
        return input;
    }
    else if(c == EOF){
        end = TRUE;
    }
    if(sp <= 0){
        return NULL;
    }

    input = malloc(sizeof(char) * sp);

    for(i = 0; (c = pop()) != NULL; i++){
        input[i] = c;
    }
    input[i] = '\0';
    reverseShortened(input);

    if(end == TRUE){
        push(-end);
    }

    return input;
}



