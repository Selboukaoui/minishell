#include <stdio.h>
#include <string.h>

#include <stdio.h>
#include <string.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool is_redirection(const char *s)
{
    return strcmp(s, ">") == 0 || strcmp(s, "<") == 0 ||
           strcmp(s, ">>") == 0 || strcmp(s, "<<") == 0;
}

void fix_redirection_order(char **cmd) {
    int i = 0;

    while (cmd[i]) {
        // Work within a pipe segment
        int seg_start = i;
        int seg_end = i;

        // Find end of this pipe segment
        while (cmd[seg_end] && strcmp(cmd[seg_end], "|") != 0)
            seg_end++;

        // Process only the first redirection in this segment
        for (int j = seg_start; j < seg_end - 2; j++) {
            if (is_redirection(cmd[j]) &&
                !is_redirection(cmd[j + 1]) &&
                !is_redirection(cmd[j + 2]) &&
                strcmp(cmd[j + 1], "|") != 0 &&
                strcmp(cmd[j + 2], "|") != 0) {

                // Swap j+1 and j+2
                char *tmp = cmd[j + 1];
                cmd[j + 1] = cmd[j + 2];
                cmd[j + 2] = tmp;
                break; // Only fix first redirection per segment
            }
        }

        i = seg_end;
        if (cmd[i])
            i++; // Skip the '|'
    }
}


int main()
{
    char *cmd[] = {"ls", ">", "out", ">", "grep", "txt", NULL};
    fix_redirection_order(cmd);

// Should print: ls test > out | grep txt
    for (int i = 0; cmd[i]; i++) {
        printf("%s ", cmd[i]);
    }
}
























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
    char    *tmp;

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
        {
            free (shell->rl_input);
            continue ;
        }
        tmp = shell->rl_input;
        shell->rl_input = clean_rl_copy(shell->rl_input);
        shell->rl_input = process_line_expand_first_var(shell->rl_input, shell);
        free(tmp);
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
        ft_malloc(0,0);
    }
    return(0);
}



