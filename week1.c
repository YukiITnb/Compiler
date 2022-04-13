#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

typedef struct WordList{
    char word[20];
    int count;
    char lines[1000];
    struct WordList *next;
} List;


List *head = NULL;

void printList() {
    List *ptr = head;
    printf("\n[");

    while(ptr != NULL)
	{        
      printf("(%s %d %s) ",ptr->word,ptr->count, ptr->lines);
      ptr = ptr->next;
    }
	
    printf(" ]");
}


void insertFirst (char word[]) {
    List *link = (List*) malloc(sizeof(List));
	
    strcpy(link->word, word);
    link->count = 0;
    // strcpy(link->lines, lines);
    memset(link->lines,0,strlen(link->lines));
    link->next = head;
	
    head = link;
}


int isEmpty() {
    if (head == NULL) {
        return 1;
    } else return 0;
}

int length () {
    int length = 0;
    List *tmp;
	
    for (tmp = head; tmp != NULL; tmp = tmp->next) {
        length++;
    }
	
    return length;
}


List* find(char word[]) {
    List* tmp = head;
    if (head == NULL) {
        return NULL;
    }
    while (strcmp(tmp->word, word) != 0) {
        if (tmp->next == NULL ) {
            return NULL;
        } else {
            tmp = tmp->next;
        }
    }      
    return tmp;
}


List* deleteKey(char word[]){

    List* tmp = head;
    List* previous = NULL;
        
    if (head == NULL) {
        return NULL;
    }

    while (strcmp(tmp->word, word) != 0) {
        if (tmp->next == NULL) {
            return NULL;
        } else {
            previous = tmp;
            tmp = tmp->next;             
        }
    }

    if (tmp == head) {
        head = head->next;
    } else {
        previous->next = tmp->next;
    }    
    return tmp;
}

void sort() {

    int i, j, k, tempCount;
    char tempWord[20], tempLines[20] ;
    List *tmp;
    List *next;
        
    int size = length();
    k = size ;
        
    for ( i = 0 ; i < size - 1 ; i++, k-- ) {
        tmp = head ;
        next = head->next ;
            
        for ( j = 1 ; j < k ; j++ ) {   
            
            if (strcmp(tmp->word, next->word) > 0) {
                strcpy(tempWord, tmp->word);
                strcpy(tmp->word, next->word);
                strcpy(next->word, tempWord);

                tempCount = tmp->count;
                tmp->count = next->count;
                next->count = tempCount;

                strcpy(tempLines, tmp->lines);
                strcpy(tmp->lines, next->lines);
                strcpy(next->lines, tempLines);
            }
                
            tmp = tmp->next;
            next = next->next;                        
        }
    }
}

void xoaXuongDong(char x[]){
    size_t len = strlen(x);
    if(x[len-1]=='\n'){
        x[len-1]='\0';
    }
}


void readstopw(FILE *fps,char arr[]){
	char c[100];
	while(fgets(c,100,fps) != NULL){
        xoaXuongDong(c);
        strcat(arr,c);
        strcat(arr," ");
	}
}

int inarr(char arr[],char word[]){
    char *kq = strstr(arr,word);
    if(kq==NULL)
        return 0;
    return 1;
}

void readvanban(FILE *fp,char arr[]){
    char c;
    int k =0;
    int line=0;
    char word[20];
    while( (c = fgetc(fp)) != EOF){
        memset(word,0,strlen(word));
        if( strcmp(c," ") == 0)
            continue;
        if(isalpha(c) != 0 ){
            strcat(word,c);
        }
        if(strcmp(c,"\n") == 0)
            line++;

    }
        if(inarr(arr,word) == 0){
            if(find(word) != NULL){
                List *tmp = find(word);
        }
        else{
            (insertFirst(word));
            printf("%s",word);
        }
        }

}

int main()
{
	char arr[100000];
	FILE *fp = NULL;
	fp = fopen("vanban.txt","r");

	FILE *fps = NULL;
	fps = fopen("stopw.txt","r");
	// readstopw(fps,arr);
    // printf("%s",arr);
    readvanban(fp,arr);
    printList();


	fclose(fp);
	fclose(fps);
	return 0;
}