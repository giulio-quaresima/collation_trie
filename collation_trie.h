/* 
 * MIT License
 * 
 * Copyright (c) 2019 Giulio Quaresima
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file collation_trie.h
 * @author Giulio Quaresima
 * 
 * collation_trie provides a trie data structure with with access and manipulation methods.
 * 
 * The trie supports the current's locale collation, set with the 
 * setLocale function and the LC_COLLATE or the LC_ALL category,
 * which means, for example, that if the collation is UTF, the letter 'è'
 * is collated after the letter 'd' but BEFORE the letter 'e'
 * (with the default C collation, 'è' is collated after 'z', and
 * all the uppercase letters are collated before all the lowercase ones).
 * 
 * If you want to use the environment's collation, symply call
 * setlocale (LC_ALL, "");
 * before using this trie.
 * 
 * The behaviour of the tree when the collation changes between
 * insertions is not defined: the caller should set the collation
 * before the trie creation or, at least, before the insertion
 * of the first key, and then leave it unchanged until finish:
 * the only way to change collation of an already populated trie 
 * is to create a new collation_trie and copy all the items.
 * 
 * This trie allocates at each node an array of pointers to the children: 
 * as a trade-off between insertion speed and memory consumption, 
 * this implementation uses pointer arrays of minimum length required 
 * to index the former and the latter children node: when a new children
 * is inserted which is less than the former or greater than the
 * latter (in the sense defined by the current collation), the
 * pointer array grows to fit the new children's span. 
 */

#ifndef _COLLATION_TRIE_H
#define _COLLATION_TRIE_H

/**
 * A trie: all manipulation methods of this data structure
 * accept a pointer to this struct.
 */
typedef struct collation_trie collation_trie; 

/**
 * Create a new, empty Trie.
 */
collation_trie *collation_trie_new();

/**
 * Frees all the storage allocated by the trie internals,
 * except the values of each node (see collation_trie_put and collation_trie_get).
 * 
 * @param trie The trie.
 * 
 * @param frees_keys If true, this method frees also the memory allocated for each key.
 * 
 * @param frees_values If true, this method frees also the memory allocated for each value.
 */
void collation_trie_free(struct collation_trie *trie, _Bool frees_keys, _Bool frees_values);

/**
 * Associates the specified value with the specified key in this trie:
 * if the key is already present in the trie, the old value
 * is replaced with the specified value.
 * 
 * @return The old value if the key was already present present, otherwise NULL.
 */
void* collation_trie_put(struct collation_trie *trie, char *key, void *value);

/**
 * Look for a key in the trie.
 * 
 * @return 1 if the key is present, otherwise 0.
 */
_Bool collation_trie_contains(struct collation_trie *trie, char *key);

/**
 * Look for a key in the trie.
 * 
 * @return The vaule associated with the key, or NULL if the key
 * is not present: it is possible that this method returns NULL
 * when collation_trie_contains(t, key) returns 1, because it is legal to
 * associate a NULL value to a key.
 */
void* collation_trie_get(struct collation_trie *trie, char *key);

/**
 * @return The size of the trie, i.e. the number of distinct keys in it.
 */
unsigned int collation_trie_size(struct collation_trie *trie);

/**
 * Pre-order traversal which guarantees that the visitor function
 * will be called in the lexicographic order of the keys.
 * The visitor function will accept two parameters, the former
 * is the key, the latter is a pointer to the value associated
 * with the key.
 */
void collation_trie_traverse_pre_order(struct collation_trie *trie, void visitor(char*, void*));

#endif
