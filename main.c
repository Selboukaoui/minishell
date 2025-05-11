/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:02:28 by asebban           #+#    #+#             */
/*   Updated: 2025/04/16 14:02:28 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

int g_signals;

void *ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
    void *new_ptr;

    if (new_size == 0)
    {
        // free(ptr);
        return NULL;
    }

    new_ptr = ft_malloc(new_size,  1);
    if (!new_ptr)
        return NULL;

    if (ptr)
    {
        // Copy the old contents to the new memory block
        size_t size_to_copy = old_size < new_size ? old_size : new_size;
        ft_memcpy(new_ptr, ptr, size_to_copy);
        // free(ptr);
    }

    return new_ptr;
}

static int ft_isspace(int c)
{
    return (c == ' ' || c == '\t' || c == '\n' ||
            c == '\v' || c == '\f' || c == '\r');
}

int is_cmdline_empty(const char *cmdline)
{
    if (!cmdline)
        return 1;

    int i = 0;
    while (cmdline[i])
    {
        // skip whitespace
        if (ft_isspace((unsigned char)cmdline[i])) {
            i++;
            continue;
        }

        // skip empty double quotes
        if (cmdline[i] == '"' && cmdline[i + 1] == '"' ) {
            i += 2;
            continue;
        }

        // skip empty single quotes
        if (cmdline[i] == '\'' && cmdline[i + 1] == '\'') {
            i += 2;
            continue;
        }

        // if any other non-space, non-empty-quote char is found, not empty
        return 0;
    }

    return 1;  // only whitespace or empty quotes
}



/* Expose replace_vars for external use */
// char *expand_variables(const char *input, t_shell *shell)
// {
//     return (replace_vars(input, shell));
// }
/* Expose replace_vars for external use */
// char *expand_variables(const char *input, t_shell *shell)
// {
//     return (replace_vars(input, shell));
// }

int main(int ac, char **av, char **env)
{
    t_shell *shell;

    if (ac != 1)
	{
		printf("\033[0;31mUsage: ./minishell\033[0m\n");
		return (1);
	}
    (void)ac;
    (void)av;
    g_signals = 1;
    rl_catch_signals = 0;
    shell = init_shell(env);
    if (!shell)
        return (1);
    while (1)
    {
        signal_setup(PARENT);
        if (!isatty(1) || !isatty(0))
            return (ft_malloc(0,0), 0);
        g_signals = 0;
        shell->rl_input = readline("\001\033[0;32m\002minishell-1.0$~ \001\033[0m\002");
        g_signals = 1;
        if (!shell->rl_input)
		{
            free (shell->rl_input);
            write(1, "exit\n", 5);
            ft_malloc(0, 0);
			exit(exit_status(0,0));
		}
        if (*shell->rl_input)
            add_history(shell->rl_input);
        if (!check_syntax(shell)) 
            continue ; 
        shell->rl_input = clean_rl_copy(shell->rl_input);
        shell->rl_input = process_line_expand_first_var(shell->rl_input, shell);
        // printf("the new str-->%s\n", shell->rl_input);
        char *str = handle_dollar_quotes(shell->rl_input);
        // free (shell->rl_input);
        shell->rl_input = str;
        shell->rl_input = replace_var_equals_var(shell->rl_input, shell);// handle echo $PATH=''
        // printf("str1-->%s\n", shell->rl_input );
        shell->rl_input = export_hard(shell->rl_input, shell);
        // printf("strrrr-->%s\n", shell->rl_input);
        if (!shell->rl_input)
            continue;
        shell->rl_copy = replace_vars1(shell->rl_input, shell);
        // printf("str-->%s\n", shell->rl_copy)

        if (parser(shell) == false)
            continue ;
        // t_lexer_list *lexr = shell->lex_head;
        // while(lexr)
        // {
            // printf("cmd----->%s\n", lexr->str);
            // printf("type----->%u\n",lexr->type);
        //     lexr = lexr->next;
        // }
        executor(shell);
    }
    ft_malloc(0,0);
    return(0);
}











// static char *handle_dollar_quotes(const char *input)
// {
//     int len = ft_strlen(input);
//     char *result = ft_malloc(len + 1);
//     if (!result)
//         return NULL;

//     int i = 0, j = 0;
//     while (i < len)
//     {
//         if (input[i] == '$')
//         {
//             int dollar_count = 0;
//             int k = i;
//             while (k < len && input[k] == '$')
//                 dollar_count++, k++;

//             /* if next char is a quote, apply special rules */
//             if (k < len && (input[k] == '\'' || input[k] == '"'))
//             {
//                 if (dollar_count == 1)
//                 {
//                     /* single $ → preserve it */
//                     result[j++] = '$';
//                 }
//                 else if (dollar_count % 2 == 0)
//                 {
//                     /* even → copy them all */
//                     while (dollar_count--)
//                         result[j++] = '$';
//                 }
//                 else
//                 {
//                     /* odd >1 → drop one, copy the rest */
//                     dollar_count--;
//                     while (dollar_count--)
//                         result[j++] = '$';
//                 }
//             }
//             else
//             {
//                 /* no quote after → copy every $ */
//                 while (dollar_count--)
//                     result[j++] = '$';
//             }
//             i = k;
//         }
//         else
//         {
//             result[j++] = input[i++];
//         }
//     }
//     result[j] = '\0';
//     return result;
// }


// static char *handle_dollar_quotes(const char *input)
// {
//     int len = ft_strlen(input);
//     char *result = ft_malloc(len + 1); // Allocate enough (final string will be smaller or same)
//     if (!result)
//         return NULL;

//     int i = 0, j = 0;
//     while (i < len)
//     {
//         if (input[i] == '$')
//         {
//             int dollar_count = 0;
//             int k = i;

//             // Count how many dollars
//             while (k < len && input[k] == '$')
//             {
//                 dollar_count++;
//                 k++;
//             }

//             // Check if next character is a quote
//             if (k < len && (input[k] == '\'' || input[k] == '"'))
//             {
//                 if (dollar_count % 2 == 0)
//                 {
//                     // Even number of dollars, copy all
//                     while (dollar_count--)
//                         result[j++] = '$';
//                 }
//                 else
//                 {
//                     // Odd number, remove one dollar
//                     dollar_count--;
//                     while (dollar_count--)
//                         result[j++] = '$';
//                 }
//             }
//             else
//             {
//                 // No quote after, copy all dollars
//                 while (dollar_count--)
//                     result[j++] = '$';
//             }
//             i = k;
//         }
//         else
//         {
//             result[j++] = input[i++];
//         }
//     }

//     result[j] = '\0';
//     return result;
// }


// int main(int ac, char **av, char **env)
// {
//     t_shell *shell;

//     (void)ac;
//     (void)av;
//     g_signal = 0;
//     rl_catch_signals = 0;
//     shell = init_shell(env);
//     signal_setup(PARENT);
    
//     if (!shell)
//         return (1);
//     while (1)
//     {
//         g_signal = -1;
//         shell->rl_input = readline("minishell-1.0$~ ");
//         g_signal = 0;
//         if (!shell->rl_input) // CTRL+D (EOF)
// 		{
// 			write(1, "exit\n", 5);
//             ft_malloc(0, 0);
// 			break; // command not found in ctr + d handle it
// 		}
//         if (*shell->rl_input)
//             add_history(shell->rl_input);

//         // handle history cmd here
//         shell->rl_input = handle_dollar_quotes(shell->rl_input);
        // printf("str_handle_dolar ----> %s\n", shell->rl_input);
//         shell->rl_copy = clean_rl_copy(shell->rl_input);
        // printf ("rl->copy : %s\n",shell->rl_copy);
//         shell->rl_copy = replace_vars(shell->rl_input, shell);

        // printf("str ----> %s\n", shell->rl_copy);
//         // if (!ft_strcmp(shell->rl_input, "\\"))
//         // {
//         //     ft_putstr_fd("syntax error near unexpected token`(xx)'\n", 2);
//         //     //clean
//         // }

        // printf ("%s-->\n", shell->rl_copy);
//         if (parser(shell) == false)
//             continue ;

//         // t_shell *tst = shell;
//         // while (tst->lex_head)
//         // {
        //     printf ("str ----> %s\n type -----> %u\n", tst->lex_head->str, tst->lex_head->type);
//         //     tst->lex_head = tst->lex_head->next;
//         // }

//         executor(shell);
//         // and clean for next loop
//     }
//     // clean
//     return(0);// return exit_code
// }
