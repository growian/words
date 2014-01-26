#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define FILENAME "cities.txt" 
#define MAXLEN_WORD 64
#define MAX_AL 26
#define ASCII_OFFSET 65

//

typedef struct word {
   char* val;
   struct word* next; 
} word;

void chomp(char* str) {
   size_t l = strlen(str);
   str[l-1] = '\0';
}

int index_first(char* str) {
   return toupper(str[0]) - ASCII_OFFSET;
}

int index_last(char *str) {
   int last_char = strlen(str) - 1;
   return toupper(str[last_char]) - ASCII_OFFSET;
}

word* create_word(char* str) {
   word* w = malloc(sizeof(word));
   w->val = str;
   return w;
}

word* append_word(word* w, char* str) {
   w->next = create_word(str);
   return w->next;
}

word* prefix_word(word* w, char* str) {
   word* tmp = create_word(str);
   tmp->next = w;
   return tmp; 
}

word* delete_word(word* w, char* str) {
   if (strcmp(str, w->val) == 0) {
      return w->next;
   } else {
      word *curr = w;
      word *pre;
      while (curr->next != NULL) {
         pre = curr;
         curr = curr->next;
         if (strcmp(str, curr->val) == 0) {
            if (curr->next != NULL) {
               pre->next = curr->next;
            } else {
               pre->next = NULL;
            } 
            break;
         }
      }
   } 
   return w;
}

void add_to_list(word* w, char* str) {
   if (w == NULL) {
      w = create_word(str);
   } else { 
      while (w->next != NULL)
         w = w->next;
      append_word(w, str);         
      printf("new  word added");
   }
}

int read_file(word* first[], word* last[]) {
   FILE* file = fopen(FILENAME, "r");
   char buffer[MAXLEN_WORD];
   char* str;
   int ifi, ila, num_words = 0;
   while (fgets(buffer, MAXLEN_WORD, file)) {
      chomp(buffer);
      str = strdup(buffer);
      ifi = index_first(str);
      ila = index_last(str);
      add_to_list(first[ifi], str);
      add_to_list(last[ila], str);
      num_words++;
   }
   return num_words;
}

char *find_word_in_list(word *w, char *str) {
   word *tmp = w;
   while (tmp != NULL) {
      if (strcmp(str, tmp->val) == 0)
         return tmp->val; 
      tmp = tmp->next;
   }
   return NULL;
}

void print_list(word *w) {
   word *tmp = w;
   while (tmp != NULL) {
      printf("%s, ", tmp->val);
      tmp = tmp->next;
   }
}

void print_array(word *w[]) {
   for (int i = 0; i < MAX_AL; i++) {
      print_list(w[i]);
   }
}

int main() {
   word* chainbegin = NULL;
   word* chainend = NULL; 
   word* first[MAX_AL] = {0};
   word* last[MAX_AL] = {0};
   int words_left = read_file(first, last);
   printf("%i words left\n", words_left);
   char input[MAXLEN_WORD];
   while (1) {
      int ifi, ila, len_chain = 0;
      char* ps;
      while (1) {
         printf("Next word: ");
         fgets(input, MAXLEN_WORD, stdin);
         chomp(input);
         if (strlen(input) == 0) { 		// Erste Eingabe 0
            print_array(first);
            printf("\n");
            continue;
         } else {   	
            ifi = index_first(input);
            ila = index_last(input);			
            ps = find_word_in_list(first[ifi], input);
            if (ps != NULL) {			// Neue Chain anlegen und spiel starten
               chainbegin = create_word(ps);
               len_chain++;
               chainend = chainbegin;
               first[ifi] = delete_word(first[ifi], ps);     
               last[ila] = delete_word(last[ila], ps);      
               words_left--;
               break;
            } else {				// Wort ist nicht in der Liste
               printf("Word not in index\n");
               continue;
            }
         }
      }   
      while (1) {
         ifi = index_first(chainbegin->val);			
         ila = index_last(chainend->val);
         printf("%i words left\n", words_left);
         printf("Current chain of %i words: %s ... %s\n",len_chain ,chainbegin->val, chainend->val);
         printf("Next word: ");
         fgets(input, MAXLEN_WORD, stdin);
         chomp(input);
         if (strlen(input) == 0) { 		// Erste Eingabe 0
            print_list(last[ifi]);
            print_list(first[ila]); 
            printf("\n");
            continue;
         } else {   				
            ps = find_word_in_list(last[ifi], input);	// entweder das wort passt vorne an die kette
            if (ps != NULL) {
               chainbegin = prefix_word(chainbegin, ps); 
            } else {
               ps = find_word_in_list(first[ila], input);	// oder hinten
               if (ps != NULL) {
                  chainend = append_word(chainend, ps);
               } else {
                  printf("Word not in index or not matching or already used\n");			// oder gar nicht
                  continue;
               }
            }
            ifi = index_first(ps);
            ila = index_last(ps); 
            last[ila] = delete_word(last[ila], ps);      
            first[ifi] = delete_word(first[ifi], ps);     
            words_left--;
            len_chain++;
         }
      }
   }
   return 0;
}
