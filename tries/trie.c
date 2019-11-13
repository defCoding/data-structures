#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#define ALPHABET_SIZE 26
#define MAX_STRING_SIZE 100
#define STRING_FORMAT_SPECIFIER "%100s"

int get_char_index(char c);
struct TrieNode * create_node();
void insert(struct TrieNode *node, const char *key);
bool search(struct TrieNode *node, const char *key);
bool is_empty(struct TrieNode *node);
struct TrieNode * delete(struct TrieNode *node, const char *key);
void display(struct TrieNode *node);
void display_helper(struct TrieNode *node, char *str, int index);
void autocomplete(struct TrieNode *node, char *prefix);
void autocomplete_helper(struct TrieNode *node, char *prefix, int end_index);

struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    bool is_word_end;
};

int main() {
    int count;

    printf("How many words do you want to insert? ");
    scanf("%d", &count);

    struct TrieNode * root = create_node();
    int i;
    for (i = 0; i < count; i++) {
        char str[MAX_STRING_SIZE];
        scanf(STRING_FORMAT_SPECIFIER, str);
        insert(root, str);
    }

    printf("How many searches do you want to do? ");
    int searches;
    scanf("%d", &searches);

    for (i = 0; i < searches; i++) {
        char str[MAX_STRING_SIZE];
        scanf(STRING_FORMAT_SPECIFIER, str);
        printf(search(root, str) ? "true\n" : "false\n");
    }

    printf("How many deletions do you want to do? ");
    int deletions;
    scanf("%d", &deletions);

    for (i = 0; i < deletions; i++) {
        char str[MAX_STRING_SIZE];
        scanf(STRING_FORMAT_SPECIFIER, str);
        root = delete(root, str);
    }

    printf("How many searches do you want to do? ");
    scanf("%d", &searches);

    for (i = 0; i < searches; i++) {
        char str[MAX_STRING_SIZE];
        scanf(STRING_FORMAT_SPECIFIER, str);
        printf(search(root, str) ? "true\n" : "false\n");
    }

    printf("Strings currently in trie:\n");
    display(root);

    int autorequests;

    printf("How many autocomplete requests do you want to do? ");
    scanf("%d", &autorequests);

    for (i = 0; i < autorequests; i++) {
        char str[MAX_STRING_SIZE];
        scanf(STRING_FORMAT_SPECIFIER, str);
        printf("Strings that match %s\n===================\n", str);
        autocomplete(root, str);
        printf("===================\n");
    }
    return 0;
}

/**
 * Creates a node.
 * @return Returns a pointer to the created node.
 * */
struct TrieNode * create_node() {
    struct TrieNode *node = (struct TrieNode *) malloc(sizeof(struct TrieNode));
    
    node->is_word_end = false;
    int i;
    for (i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }

    return node;
}

/**
 * Converts a character into an index.
 * @param c The character that we want to convert to an index.
 * @return The index of the character.
 * */
int get_char_index(char c) {
    return (int) c - (int) 'a';
}

/**
 * Inserts a string into a given trie.
 * @param node A pointer to the trie that we are inserting into.
 * @param key The string we are inserting into node.
 * */
void insert(struct TrieNode *node, const char *key) {
    int length = strlen(key);
    
    struct TrieNode *curr_node = node;
    int i, index;
    // Step into the trie and insert a trie node wherever needed into the child of the current trie node.
    for (i = 0; i < length; i++) {
        index = get_char_index(key[i]);
        if (!curr_node->children[index]) {
            curr_node->children[index] = create_node();
        }

        curr_node = curr_node->children[index];
    }
    // Mark the end of the word now that we have fully stepped through.
    curr_node->is_word_end = true;
}

/**
 * Checks if a given string exists in a given trie.
 * @param node A pointer to the trie we are searching in.
 * @param key The string we are searching for.
 * @return A boolean representing if the key exists in the trie.
 * */
bool search(struct TrieNode *node, const char *key) {
    int length = strlen(key);

    struct TrieNode *curr_node = node;
    int i, index;
    // Step into the trie and keep searching until we've reached the end of a string or reach a null value in a child.
    for (i = 0; i < length; i++) {
        index = get_char_index(key[i]);
        if (!curr_node->children[index]) {
            return false;
        }

        curr_node = curr_node->children[index];
    }

    return (curr_node && curr_node->is_word_end);
}

/**
 * Checks if a given trie node is empty (aka has no children).
 * @param node A pointer to the trie we are checking for emptiness.
 * @return A boolean representing if the tree is empty.
 * */
bool is_empty(struct TrieNode *node) {
    int i;
    for (i = 0; i < ALPHABET_SIZE; i++) {
        if (!node->children[i]) {
            return false;
        }
    }
    return true;
}

/**
 * Delete a given string from a given trie.
 * @param node A pointer to the trie we are deleting from.
 * @param key The string we are deleting.
 * @return Returns a pointer to the node after deletion.
 * */
struct TrieNode * delete(struct TrieNode *node, const char *key) {
    // If the node is NULL, there is nothing to delete so return NULL.
    if (!node) {
        return NULL;
    }

    // If we are at the end of the string, unmark is_word_end to false, as we've deleted that word. If the node is empty and does not lead to other strings, then delete the node. Return the node.
    if (strlen(key) == 0) {
        node->is_word_end = false;
        if (is_empty(node)) {
            free(node);
            node = NULL;
        }
        return node;
    }

    // Otherwise, go to the next node and delete with the rest of the string.
    int index = get_char_index(key[0]);
    node->children[index] = delete(node->children[index], &key[1]);

    // If after deletion this node becomes empty and this node is not the end of another string, then delete the node. Return the node.
    if (is_empty(node) && !node->is_word_end) {
        free(node);
        node = NULL;
    }

    return node;
}

/**
 * Displays what is currently stored inside of a given trie node.
 * @param node A pointer to the trie we are displaying.
 * */
void display(struct TrieNode *node) {
    char str[MAX_STRING_SIZE];
    display_helper(node, str, 0);
}

/**
 * Helper function that accumulates a given string as it displays a trie node.
 * @param node A pointer to the trie we are displaying.
 * @param str The string we have accumulated thus far.
 * @param index The next index in the string to be filled.
 * */
void display_helper(struct TrieNode *node, char *str, int index) {
    // If we've reached a word end, then print out the string so far.
    if (node->is_word_end) {
        str[index] = '\0';
        printf("%s\n", str);
    }

    int i;
    // Go through all of the children of the given node, and if there exists another child, add the letter representing that child
    // and then recursively call display on the child to fill the rest of the string.
    for (i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            str[index] = 'a' + i;
            display_helper(node->children[i], str, index + 1);
        }
    }
}

/**
 * Auto-completes a given prefix based on a given trie.
 * @param node A pointer to the trie we are searching through.
 * @param prefix The prefix the user provided.
 * */
void autocomplete(struct TrieNode *node, char *prefix) {
    int length = strlen(prefix);
    
    struct TrieNode *curr_node = node;
    int i, index;
    
    // Step through the trie and find where we should start beginning our autocomplete.
    for (i = 0; i < length; i++) {
        index = get_char_index(prefix[i]);

        // If the curr_node does not contain the next letter as a child, then this prefix does not exist so stop here.
        if (!curr_node->children[index]) {
            return;
        }

        curr_node = curr_node->children[index];
    }

    // Now that we have arrived at the node where we may begin autocomplete, call the helper starting at that node.
    autocomplete_helper(curr_node, prefix, length);
}

/**
 * Helper for autocomplete() that keeps track of current index of the prefix we are working on.
 * @param node A pointer to the trie we are searching through.
 * @param prefix The prefix we are working with.
 * @param end_index Index of the end of the prefix.
 * */
void autocomplete_helper(struct TrieNode *node, char *prefix, int end_index) {
    // If we're at the end of a word, print the word.
    if (node->is_word_end) {
        prefix[end_index] = '\0';
        printf("%s\n", prefix);
    }

    // Continue on if there are any children to this node. Add letter representing the child to the prefix and recursively call on the child.
    int i;
    for (i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            prefix[end_index] = 'a' + i;
            autocomplete_helper(node->children[i], prefix, end_index + 1);
        }
    }
}
