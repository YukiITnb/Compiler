#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>

struct node{
    char word[20];
    int count;
    char lines[1000];
    struct node *next;
};


struct node *head = NULL;

void printList()
{
   struct node *ptr = head;
   printf("\n[ \n");
   
   while(ptr != NULL)
   {        
      printf("(%s %d %s) \n",ptr->word,ptr->count,ptr->lines);
      ptr = ptr->next;
   }
   
   printf(" ]");
}

void insertFirst(char word[], int line)
{
   struct node *link = (struct node*) malloc(sizeof(struct node));
   
   strcpy(link->word, word);
   link->count = 1;

   memset(link->lines,0,strlen(link->lines));
   char ln[10];
   itoa(line, ln, 10);
   strcat(link->lines,ln);
   strcat(link->lines," ");
   
   link->next = head;
   
   head = link;
}


bool isEmpty()
{
   return head == NULL;
}

int length()
{
   int length = 0;
   struct node *current;
   
   for(current = head; current != NULL; current = current->next)
   {
      length++;
   }
   
   return length;
}

struct node* find(char arr[]){
   struct node* current = head;
   if(head == NULL)
   {
      return NULL;
   }
   while(strcmp(current->word,arr) != 0){
   
      if(current->next == NULL){
         return NULL;
      }else {
         current = current->next;
      }
   }      
   
   return current;
}


void sort(){
    int i, j, k, tempCount;
    char tempWord[20], tempLines[20] ;
    struct node *tmp;
    struct node *next;
        
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

int inarr(char arr[],char word[]){
    char *kq = strstr(arr,word);
    if(kq == NULL)
        return 0;
    return 1;
}


void readstopw(FILE *fps,char arr[]){
	char c[100];
	while(fgets(c,100,fps) != NULL){
        xoaXuongDong(c);
        strcat(arr,c);
        strcat(arr," ");
	}
}

void readvanban(FILE *fp,char arr[]){
	char c;
	int countline = 1;
	char word[30];
	memset(word,0,strlen(word));
    while ((c = fgetc(fp)) != EOF)
    {
        // printf("%c", c);
      if (c == '\n')
        {
         countline++;
         // printf("%d",countline);
        }
      int key = isalpha(c);

    	if (key == 0 && c != '\n'){
         if (inarr(arr,word) == 0){

       		if (find(word) != NULL){
       			struct node *tmp = find(word);
       			tmp->count++;
               char ln[10];
               itoa(countline, ln, 10);
               if (inarr(tmp->lines,ln) == 0 )
               {
                  strcat(tmp->lines,ln);
                  strcat(tmp->lines," ");
               }
       		}
       		else{
       			insertFirst(word,countline);
       		}
         }
    		
    		// printf("%s ", word);
    		memset(word,0,strlen(word));

    	}

      if (key !=0 && c != '\n' && c != '\0'){
        	// printf("%d", key);
         char d = tolower(c);
        	sprintf (word, "%s%c", word,d);
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
	readstopw(fps,arr);
   printf("%s",arr);

    readvanban(fp,arr);

    sort();
    printList();

	fclose(fp);
	fclose(fps);
	return 0;
}