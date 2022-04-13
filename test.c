#include <stdio.h>
#include <string.h>
 
int inarr(char arr[],char word[]){
    char *kq = strstr(arr,word);
    if(kq == NULL)
        return 0;
    return 1;
}

int main(){
    char word[20] = "abc";
    char c[20] = "ab";
    int k = inarr(word,c);
    printf("%d", k);
    return 0;
}

// #include<stdio.h>
// #include <string.h>
 
// int main() {
//   char str[100] = "Day la mot vi du ve ham strstr() trong c, vi du...";
//   char *sub;
//   sub = strstr(str, "Day");
//   printf("\nChuoi con la: %s", sub);
//   return 0;
// }