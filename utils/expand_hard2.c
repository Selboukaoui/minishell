/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_hard2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 17:21:28 by asebban           #+#    #+#             */
/*   Updated: 2025/05/11 18:55:28 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int ft_isspace(int c)
{
    return (c == ' ' || c == '\t' || c == '\n' ||
            c == '\v' || c == '\f' || c == '\r');
}

static const char *skip_leading_spaces(const char *s)
{
    while (*s && ft_isspace((unsigned char)*s))
        s++;
    return s;
}

static  char    *join_with_pipes(char **parts)
{
    char    *res;
    // char    *tmp;
    size_t  i;

    res = NULL;
    i = 0;
    while (parts[i])
    {
        if (!res)
            res = ft_strdup(parts[i]);
        else
            res = ft_strjoin(ft_strjoin(res, "|"), parts[i]);
        i++;
    }
    return (res);
}

static void *ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
    void *new_ptr;

    if (new_size == 0)
    {
        free(ptr);
        return NULL;
    }

    new_ptr = ft_malloc(new_size, 1);
    if (!new_ptr)
        return NULL;

    if (ptr)
    {
        size_t copy_size = old_size < new_size ? old_size : new_size;
        ft_memcpy(new_ptr, ptr, copy_size);
        // free(ptr);
    }

    return new_ptr;
}

void append_chunk(char **dst, size_t *dst_len, const char *src, size_t len)
{
    char *new_dst;

    new_dst = ft_realloc(*dst, *dst_len, *dst_len + len + 1);
    if (!new_dst)
    {
        // Handle allocation failure if necessary
        return;
    }

    *dst = new_dst;
    memcpy(*dst + *dst_len, src, len);
    *dst_len += len;
    (*dst)[*dst_len] = '\0';
}

static void app_result(char **pres)
{
    const char  *in;
    char        *out;
    size_t      out_len;
    size_t      i;
    
    i = 0;
    in = *pres;
    out = malloc(1);
    out_len = 0;
    out[0] = '\0';

    while (in[i])
    {
        if (in[i] == '>' && in[i+1] == '>')
        {
            append_chunk(&out, &out_len, "'>>'", 4);
            i += 2;
        }
        // Single-char tokens: <, >, |
        else if (in[i] == '<' || in[i] == '>' || in[i] == '|')
        {
            char quoted[4] = { '\'', in[i], '\'', '\0' };
            append_chunk(&out, &out_len, quoted, 3);
            i += 1;
        }
        // Everything else
        
        else {
            append_chunk(&out, &out_len, &in[i], 1);
            i += 1;
        }
    }

    *pres = out;
}

static char *expand_vars_in(const char *src, t_shell *shell)
{
    const char *trimmed = skip_leading_spaces(src);
    size_t j = 1;
    char *first_var;
    char *first_val;
    char *result;

    printf ("here\n");
    // Parse first var name
    while (trimmed[j] && (ft_isalnum((unsigned char)trimmed[j]) || trimmed[j] == '_'))
        j++;
    first_var = ft_strndup(trimmed + 1, j - 1);

    // Lookup in env
    first_val = get_env_value(shell->env, first_var);
    // free(first_var);

    if (first_val)
    {
        // Fully expand all vars
        result = ft_replace_var3(trimmed, shell);
        printf("the value before : %s\n", result);
        app_result(&result);
        return result;
    } 
    else
    { 
        // Skip this $var and check for next one
        const char *next = skip_leading_spaces(trimmed + j);
        if (*next == '$')
            return expand_vars_in(next, shell);
        // Nothing to expand
        return NULL;
    }
}



char    *process_line_expand_first_var(char *line, t_shell *shell)
{
    char    **parts;
    size_t  i;
    char    *trimmed;
    char    *expanded;
    char    *result;

    i = 0;
    parts = ft_split1(line, '|');
    if (!parts)
        return (NULL);
    while (parts[i])
    {
        trimmed = (char *)skip_leading_spaces(parts[i]);
        if (ft_strncmp(trimmed, "echo", 4) == 0)
        if (*trimmed == '$')// if NULL
        {
            expanded = expand_vars_in(trimmed, shell);
            if (expanded)
                parts[i] = expanded;
        }
        i++;
    }
    result = join_with_pipes(parts);
    return (result);
}