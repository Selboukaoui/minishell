/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:42:54 by asebban           #+#    #+#             */
/*   Updated: 2025/05/14 22:11:07 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

/* Dynamic array of strings */
typedef struct {
    char **items;
    size_t  count, capacity;
} str_array;

static void arr_init(str_array *a) {
    a->count = 0;
    a->capacity = 8;
    a->items = malloc(sizeof(char*) * a->capacity);
}

static void arr_push(str_array *a, char *s) {
    if (a->count + 1 > a->capacity) {
        a->capacity *= 2;
        a->items = realloc(a->items, sizeof(char*) * a->capacity);
    }
    a->items[a->count++] = s;
}

static void arr_finish(str_array *a) {
    arr_push(a, NULL);
}

/*
 * Return 2 if s starts with "<<", else 0.
 */
static int heredoc_len(const char *s) {
    return (s[0]=='<' && s[1]=='<') ? 2 : 0;
}

/*
 * Split on whitespace, quote-groups, and heredoc only.
 */
char **split_heredoc(const char *s) {
    str_array toks;
    arr_init(&toks);

    size_t i = 0, n = strlen(s);
    while (i < n) {
        /* skip spaces */
        while (i < n && isspace((unsigned char)s[i])) i++;
        if (i >= n) break;

        /* heredoc operator? */
        int hlen = heredoc_len(s + i);
        if (hlen) {
            arr_push(&toks, strndup(s + i, hlen));
            i += hlen;
            continue;
        }

        /* quoted string */
        if (s[i]=='\'' || s[i]=='\"') {
            char q = s[i++];
            size_t start = i-1;
            while (i < n && s[i]!=q) {
                if (s[i]=='\\' && i+1<n && s[i+1]==q) i+=2;
                else i++;
            }
            if (i<n) i++;  /* include closing quote */
            arr_push(&toks, strndup(s + start, i - start));
            continue;
        }

        /* bare word */
        size_t start = i;
        while (i < n
           && !isspace((unsigned char)s[i])
           && !(s[i]=='<' && s[i+1]=='<')
           && s[i] != '\'' && s[i] != '\"')
        {
            i++;
        }
        arr_push(&toks, strndup(s + start, i - start));
    }

    arr_finish(&toks);
    return toks.items;
}