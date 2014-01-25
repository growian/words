#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define FILENAME "cities.txt" 
#define MAXLEN_WORD 64
#define MAX_AL 26

struct word {
   char *val;
   struct word *next; 
};

void chomp(char *str) {
   size_t p=strlen(str);
   str[p-1]='\0';
}

int index_first(char *str) {
   return (toupper(str[0]))-65;
}

int index_last(char *str) {
   int last_char = strlen(str)-1;
   return (toupper(str[last_char]))-65;
}

struct word *create_word(char *str) {
   struct word *w = malloc(sizeof(struct word));
   w->val = str;
   return w;
}

struct word *append_word(struct word *w, char *str) {
   w->next = malloc(sizeof(struct word));
   w = w->next;
   w->val = str;
   return w;
}

struct word *prefix_word(struct word *w, char *str) {
   struct word *tmp = malloc(sizeof(struct word));
   tmp->val = str;
   tmp->next = w;
   return tmp; 
}

struct word *delete_word(struct word *w, char *str) {
   if (strcmp(str, w->val) == 0) {
      return w->next;
   } else {
      struct word *tmp = w;
      struct word *pre;
      while (tmp->next != NULL) {
         pre = tmp;
         tmp = tmp->next;
         if (strcmp(str, tmp->val) == 0) {
            if (tmp->next != NULL) {
               pre->next = tmp->next;
            } else {
               pre->next = NULL;
            } 
            break;
         }
      }
   } 
   return w;
}

char *str_to_heap(char *str) {
   char *ptr = malloc(sizeof(strlen(str)));
   strcpy(ptr, str);
   return ptr;
}

void add_to_list(struct word **ppw, char *str) {
   if (*ppw == NULL) {
      *ppw = create_word(str);
   } else { 
      struct word *pw = *ppw;
      while (pw->next != NULL)
         pw = pw->next;
      append_word(pw, str);         
   }
}

int read_file(struct word *words_first[], struct word *words_last[]) {
   FILE* file = fopen(FILENAME, "r");
   char buffer[MAXLEN_WORD];
   char *str;
   int ifi, ila, num_words = 0;
   while (fgets(buffer, MAXLEN_WORD, file)) {
      chomp(buffer);
      str = str_to_heap(buffer);
      ifi = index_first(str);
      ila = index_last(str);
      add_to_list(words_first+ifi, str);
      add_to_list(words_last+ila, str);
      num_words++;
   }
   return num_words;
}

char *find_word_in_list(struct word *w, char *str) {
   struct word *tmp = w;
   while (tmp != NULL) {
      if (strcmp(str, tmp->val) == 0)
         return tmp->val; 
      tmp = tmp->next;
   }
   return NULL;
}

void print_list(struct word *w) {
   struct word *tmp = w;
   while (tmp != NULL) {
      printf("%s, ", tmp->val);
      tmp = tmp->next;
   }
}

void print_array(struct word *w[]) {
   for (int i = 0; i < MAX_AL; i++) {
      print_list(w[i]);
   }
}

int main() {
   struct word *xchain = NULL, *chainx = NULL; 
   struct word *words_first[MAX_AL] = {NULL}, *words_last[MAX_AL] = {NULL};
   int words_left = read_file(words_first, words_last);
   printf("%i words left\n", words_left);
   char input[MAXLEN_WORD];
   while (1) {
      int ifi, ila, len_chain = 0;
      char *ps;
      while (1) {
         printf("Next word: ");
         fgets(input, MAXLEN_WORD, stdin);
         chomp(input);
         if (strlen(input) == 0) { 		// Erste Eingabe 0
            print_array(words_first);
            printf("\n");
            continue;
         } else {   	
            ifi = index_first(input);
            ila = index_last(input);			
            ps = find_word_in_list(words_first[ifi], input);
            if (ps != NULL) {			// Neue Chain anlegen und spiel starten
               xchain = create_word(ps);
               len_chain++;
               chainx = xchain;
               words_first[ifi] = delete_word(words_first[ifi], ps);     
               words_last[ila] = delete_word(words_last[ila], ps);      
               words_left--;
               break;
            } else {				// Wort ist nicht in der Liste
               printf("Word not in index\n");
               continue;
            }
         }
      }   
      while (1) {
         ifi = index_first(xchain->val);			
         ila = index_last(chainx->val);
         printf("%i words left\n", words_left);
         printf("Current chain of %i words: %s ... %s\n",len_chain ,xchain->val, chainx->val);
         printf("Next word: ");
         fgets(input, MAXLEN_WORD, stdin);
         chomp(input);
         if (strlen(input) == 0) { 		// Erste Eingabe 0
            print_list(words_last[ifi]);
            print_list(words_first[ila]); 
            printf("\n");
            continue;
         } else {   				
            ps = find_word_in_list(words_last[ifi], input);	// entweder das wort passt vorne an die kette
            if (ps != NULL) {
               xchain = prefix_word(xchain, ps); 
            } else {
               ps = find_word_in_list(words_first[ila], input);	// oder hinten
               if (ps != NULL) {
                  chainx = append_word(chainx, ps);
               } else {
                  printf("Word not in index or not matching or already used\n");			// oder gar nicht
                  continue;
               }
            }
            ifi = index_first(ps);
            ila = index_last(ps); 
            words_last[ila] = delete_word(words_last[ila], ps);      
            words_first[ifi] = delete_word(words_first[ifi], ps);     
            words_left--;
            len_chain++;
         }
      }
   }
   return 0;
}
