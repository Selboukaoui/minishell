/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:42:54 by asebban           #+#    #+#             */
/*   Updated: 2025/05/14 19:53:20 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// A simple malloc wrapper (replace with your own allocator if needed)

/*
 * Count how many tokens the input will split into:
 *  - Skips spaces outside quotes
 *  - Treats unquoted "<<" as a standalone token
 *  - Honors single- and double-quote pairing
 */
static int count_tokens1(const char *s)
{
    int  i = 0, count = 0;
    char quote = 0;

    while (s[i])
    {
        // Skip spaces outside quotes
        while (s[i] == ' ' && !quote)
            i++;
        if (!s[i])
            break;
        count++;  // one new token

        // If here-doc operator unquoted, consume it
        if (!quote && s[i] == '<' && s[i + 1] == '<')
        {
            i += 2;
        }
        else
        {
            // Otherwise, scan until next space or << outside quotes
            while (s[i])
            {
                if ((s[i] == '\'' || s[i] == '\"'))
                {
                    if (!quote)
                        quote = s[i];
                    else if (quote == s[i])
                        quote = 0;
                }
                if (!quote && (s[i] == ' ' || (s[i] == '<' && s[i + 1] == '<')))
                    break;
                i++;
            }
        }
    }
    return count;
}

/*
 * Fill the provided array with tokens, using the same logic as count_tokens().
 */
static char **lex_heredoc(const char *s, char **out)
{
    int  i = 0, tok = 0;
    char quote = 0;
    int  start, len;

    while (s[i])
    {
        // Skip spaces outside quotes
        while (s[i] == ' ' && !quote)
            i++;
        if (!s[i])
            break;
        start = i;

        // If here-doc operator unquoted, consume it
        if (!quote && s[i] == '<' && s[i + 1] == '<')
        {
            i += 2;
        }
        else
        {
            // Otherwise, scan until next space or << outside quotes
            while (s[i])
            {
                if ((s[i] == '\'' || s[i] == '\"'))
                {
                    if (!quote)
                        quote = s[i];
                    else if (quote == s[i])
                        quote = 0;
                }
                if (!quote && (s[i] == ' ' || (s[i] == '<' && s[i + 1] == '<')))
                    break;
                i++;
            }
        }

        // Duplicate the token substring
        len = i - start;
        out[tok++] = ft_strndup(s + start, len);
        if (!out[tok - 1])
            return NULL;
    }
    out[tok] = NULL;
    return out;
}

/*
 * Public API:
 *  - Splits on spaces and unquoted "<<" only.
 *  - Returns a NULL‑terminated array of malloc’d strings.
 */
char **ft_split2(const char *s, char c /* must be ' ' here */)
{
    char **array;
    int    n;

    (void)c;  // only spaces + here-doc supported
    if (!s)
        return NULL;

    n = count_tokens1(s);
    array = ft_malloc(sizeof(char *) * (n + 1), 1);
    if (!array)
        return NULL;

    return lex_heredoc(s, array);
}
