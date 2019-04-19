# glib库函数：链表、排序表、二叉树、哈希 - Koma Hub - CSDN博客
2019年03月03日 16:14:25[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：19
个人分类：[Glib																[GTK](https://blog.csdn.net/Rong_Toa/article/category/7156203)](https://blog.csdn.net/Rong_Toa/article/category/8611689)
**Table of Contents**
[tree.c](#tree.c)
[sortlist.c](#sortlist.c)
[hash.c](#hash.c)
[message.c](#message.c)
[list.c](#list.c)
[mem.c](#mem.c)
# tree.c
```cpp
/*
 * File: tree.c
 */
#include <stdio.h>
#include <string.h>
#include <glib.h>
/*
 * CompareNames
 *
 * Compare function used so sort the tree
 */
gint CompareNames (gconstpointer name1, gconstpointer name2)
{
	return (strcmp (name1, name2));
}
/*
 * TraverseTree
 *
 * Function that displays the information at the
 * node.
 */
gint TraverseTree (gpointer key, gpointer value, gpointer data)
{
	char	*sKey = key;
	char	*sValue = value;
	g_print ("Key: %s,  Value: %s\n", sKey, sValue);
	return FALSE;
}
/*
 * main
 *
 * Program begins here.
 */
int main (int argc, char *argv[])
{
	GTree	*tree;
	tree = g_tree_new (CompareNames);
	g_tree_insert (tree, "Fred", "Loud");
	g_tree_insert (tree, "Mary", "Obnoxious");
	g_tree_insert (tree, "Sue", "Drunk");
	g_tree_insert (tree, "John", "Quiet");
	g_tree_insert (tree, "Shelley", "Civil");
	g_tree_insert (tree, "Mark", "Strange");
	g_tree_insert (tree, "Renato", "Mighty");
        g_print ("Looking up Fred - value %s\n", 
		 (gchar *) g_tree_lookup (tree, "Fred"));
	g_print ("Tree height: %d\n", g_tree_height (tree));
	g_print ("Tree nodes: %d\n", g_tree_nnodes (tree));
	g_print ("Tree:\n ");
	g_tree_traverse (tree, TraverseTree, G_IN_ORDER, NULL);
}
```
# sortlist.c
```cpp
/*
 * File: sortlist.c
 */
#include <stdio.h>
#include <string.h>
#include <glib.h>
/*
 * Compare Names
 *
 * A Comparison function for inserting names alphabetically.
 */
gint CompareNames (gconstpointer sName1, gconstpointer sName2)
{
    return ((gint) strcmp ((char *) sName1, (char *) sName2));
}
/*
 * PrintNames
 *
 * A print function to display the data elements 
 * in the list.
 */
PrintNames (gpointer data, gpointer user_data)
{
    gchar    *message;
    GSList   *list;
    /* --- Convert the data to a string --- */
    message = (gchar *) data;
    /* --- Display the string --- */
    g_print ("%s position is ...\n", message);
}
/*
 * main
 * 
 * program begins here.
 */
int main (int argc, char *argv[])
{
    int    nIndex;
    GSList    *list, *node;
    /* --- Must be set to NULL --- */
    list = NULL;
    /* --- Insert the names  --- */
    list = g_slist_insert_sorted (list, "Fred", CompareNames);
    list = g_slist_insert_sorted (list, "Joe", CompareNames);
    list = g_slist_insert_sorted (list, "Susie", CompareNames);
    list = g_slist_insert_sorted (list, "Frank", CompareNames);
    list = g_slist_insert_sorted (list, "Wilma", CompareNames);
    list = g_slist_insert_sorted (list, "Mary", CompareNames);
    node = g_slist_find (list, "Wilma");
    if (node) {
        g_print ("found..\n");
    } else {
        g_print ("not found..\n");
    }
    /* --- Loop through the list --- */
    for (nIndex = 0; node = g_slist_nth (list, nIndex); nIndex++) {
        /* --- Dump out the contents and look up the index --- */
        g_print ("%s position %d\n", 
                 (char *) node->data, 
                 g_slist_index (list, node->data));
    }
    g_print ("-----\n");
    /* --- Another way to print each of the data elements. --- */ 
    g_slist_foreach (list, (GFunc) PrintNames, list);
}
```
# hash.c
```cpp
#include <stdio.h>
#include <string.h>
#include <glib.h>
/*
 * Compare Names
 *
 * A Comparison function for inserting names alphabetically.
 */
gint HashCompare (gconstpointer sName1, gconstpointer sName2)
{
    return (!strcmp ((char *) sName1, (char *) sName2));
}
/*
 * Make a hash from the first two characters.
 */
guint HashFunction (gconstpointer key)
{
    return (((char *) key)[0] + ((char *) key)[1]);
}
/*
 * PrintNames
 *
 * A print function to display the data elements 
 * in the list.
 */
PrintNames (gpointer key, gpointer value, gpointer user_data)
{
    g_print ("Key: %s, Value: %s\n", 
		(gchar *) key, (gchar *) value);
}
int main (int argc, char *argv[])
{
    int         nIndex;
    char        buffer[88];
    GHashTable	*hTable;
    hTable = g_hash_table_new (HashFunction, HashCompare);
    /* --- Insert the names  --- */
    g_hash_table_insert (hTable, "Fred", "Boring");
    g_hash_table_insert (hTable, "Mary", "Shifty");
    g_hash_table_insert (hTable, "Sue", "Nice");
    g_hash_table_insert (hTable, "John", "Strange");
    g_hash_table_insert (hTable, "Shelley", "Abnormal");
    g_hash_table_insert (hTable, "Markus", "Absent minded");
    g_hash_table_insert (hTable, "Renato", "Paranoid");
    g_hash_table_insert (hTable, "Renato", "Smart");
    g_hash_table_insert (hTable, "Renato", "Intelligent");
    g_hash_table_insert (hTable, "Renato", "Stubby toes");
    g_print ("Renato is %s\n", 
	     (gchar *) g_hash_table_lookup (hTable, "Renato"));
    g_print ("-----\n");
    /* --- Another way to print each of the data elements. --- */
    g_hash_table_foreach (hTable, (GHFunc) PrintNames, NULL);
}
```
# message.c
```cpp
/* 
 * message.c
 *
 * Example showing message functions
 */
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
/*
 * Surfer Print
 *
 * Override function for the g_print function. 
 */
void SurferPrint (const gchar *buf)
{
    printf ("Dude, ");
    fputs (buf, stdout);
}
/*
 * Surfer Message
 *
 * Override for the g_message function
 */
void SurferMessage (const gchar *buf)
{
    printf ("Dude, ya got a message -> ");
    fputs (buf, stdout);
}
/*
 * SurferWarning
 *
 * Override for the g_warning function
 */
void SurferWarning (const gchar *buf)
{
    printf ("Bad news Dude. -> ");
    fputs (buf, stdout);
}
/*
 * SurferError
 *
 * Override for the g_error function
 */
void SurferError (const gchar *buf)
{
    printf ("Major wipe out, dude. -> ");
    fputs (buf, stdout);
}
/*
 * ShowParams
 *
 * Show the options available for running the program.
 */
void ShowParams ()
{
    printf ("Must pass in parameter.  Valid parameters are:\n");
    printf (" 'surfer' - use surfer message handling.\n");
    printf (" 'normal' - use normal message handling.\n ");
    exit (0);
}
/*
 * main
 *
 * Program begins here
 */
int main (int argc, char *argv[])
{
    /* --- Not enough args? --- */
    if (argc <= 1) {
        ShowParams ();
    }
    /* --- Normal speech? --- */
    if (strcmp (argv[1], "normal") == 0) {
        /* --- Do nothing - just verify that parameter is valid. --- */
    /* --- Surfer speech?  --- */
    } else if (strcmp (argv[1], "surfer") == 0) {
        /* --- Seems that they want surfer speech for the errors. --- */
        g_set_error_handler (SurferError);
        g_set_warning_handler (SurferWarning);
        g_set_message_handler (SurferMessage);
        g_set_print_handler (SurferPrint);
    } else {
        /* --- Can only pick 'normal' or 'surfer' --- */
        ShowParams ();
    }
    /* 
     * --- Show functions at work.  If we have custom handlers, 
     * --- the message will be intercepted.
     */
    g_print ("Here's a print\n");
    g_message ("Here's a message\n");
    g_warning ("Here's a warning\n");
    g_error ("Here's an error\n");
    
}
```
# list.c
```cpp
/*
 * list.c 
 *
 * Show linked lists in action
 */
#include <stdio.h>
#include <string.h>
#include <glib.h>
/*
 * Display List
 * 
 * Display the data items in the list.
 */
void DisplayList (GSList *list)
{
    int    nIndex;
    GSList *node;
    for (nIndex = 0; node = g_slist_nth (list, nIndex); nIndex++) {
        g_print ("%s\n", (char *) node->data);
    }
}
/*
 * ListLookup
 *
 * Look up the string in the linked list and display a message as
 * to whether the name was found.
 */
void ListLookup (GSList *list, char *szName)
{
    GSList *item;
    g_print ("Looking up: %s  ", szName);
    /* --- Run the search --- */
    item = g_slist_find (list, szName);
    /* --- If we found it. --- */
    if (item) {
        g_print ("--Found it--\n");
    } else {
        g_print ("**not found**\n");
    }
}
/*
 * main
 * 
 * Program begins here.
 */
int main (int argc, char *argv[])
{
    GSList      *list;
    char	szMary[] = "Mary";
    /* --- Empty list. --- */
    list = NULL;
    /* --- Insert items in this order --- */
    /* --- Fred, Susie, Frank, Wilma  --- */
    list = g_slist_append (list, "Fred");
    list = g_slist_append (list, "Susie");
    list = g_slist_append (list, "Frank");
    list = g_slist_append (list, "Wilma");
    /* --- Insert Joe at the front of the list. --- *.
    list = g_slist_prepend (list, "Joe");
    /* --- Insert Mary after the second position --- *.
    list = g_slist_insert (list, "Mary", 2);
    /* --- Show the list so far. --- */
    DisplayList (list);
    ListLookup (list, "Mike");
    ListLookup (list, szMary);
    ListLookup (list, "Mary");
    ListLookup (list, "Susan");
    ListLookup (list, "Tyler");
    g_print ("-------------------\n");
    g_print ("   Reversing\n");
    g_print ("-------------------\n");
    /* --- Reverse the list. --- */
    list = g_slist_reverse (list);
    DisplayList (list);
    /* --- Free data --- */
    g_slist_free (list);
}
```
# mem.c
```cpp
#include <stdio.h>
#include <string.h>
#include <glib.h>
/*
 * CompareNames
 *
 * Comparsion function used to sort names
 */
gint CompareNames (gconstpointer name1, gconstpointer name2)
{
	return ((gint) strcmp (name1, name2));
}
/*
 * TraverseTree
 *
 * Function called from tree to display the node's
 * information
 */
gint TraverseTree (gpointer key, gpointer value, gpointer data)
{
	char	*sKey = key;
	char	*sValue = value;
	g_print ("Key: %s,  Value: %s\n", sKey, sValue);
	return FALSE;
}
int main (int argc, char *argv[])
{
	GTree	*tree;
        char *bad_pointer;
	tree = g_tree_new (CompareNames);
	g_tree_insert (tree, "Fred", "Loud");
	g_tree_insert (tree, "Mary", "Obnoxious");
	g_tree_insert (tree, "Sue", "Drunk");
	g_tree_insert (tree, "John", "Quiet");
	g_tree_insert (tree, "Shelley", "Civil");
	g_tree_insert (tree, "Mark", "Strange");
	g_tree_insert (tree, "Renato", "Mighty");
        g_mem_profile ();
        bad_pointer = g_malloc (100 * sizeof (char));
        bad_pointer[101] = 'a';
        g_free (bad_pointer);
        g_free (bad_pointer);
}
```
