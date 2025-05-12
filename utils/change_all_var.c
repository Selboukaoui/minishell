/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_all_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 18:01:31 by asebban           #+#    #+#             */
/*   Updated: 2025/05/12 12:54:06 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static void	toggle_quote(t_varctx *ctx)
// {
// 	char	c;

// 	c = ctx->in[ctx->pos[0]];
// 	if (c == '\'' && !ctx->double_s)
// 	{
// 		ctx->out[ctx->pos[1]++] = c;
// 		ctx->single = !ctx->single;
// 		ctx->pos[0]++;
// 	}
// 	else if (c == '"' && !ctx->single)
// 	{
// 		ctx->out[ctx->pos[1]++] = c;
// 		ctx->double_s = !(ctx->double_s);
// 		ctx->pos[0]++;
// 	}
// }

// static void	write_char(t_varctx *ctx)
// {
// 	ctx->out[ctx->pos[1]++] = ctx->in[ctx->pos[0]++];
// }

// static void	handle_var(t_varctx *ctx)
// {
// 	char	name[PATH_MAX];
// 	int		k;
// 	size_t	*p;
// 	char	*val;

// 	k = 0;
// 	p = ctx->pos;
// 	p[0]++;
// 	while ((ft_isalnum(ctx->in[p[0]]) || ctx->in[p[0]] == '_')
// 		&& k < PATH_MAX)
// 		name[k++] = ctx->in[p[0]++];
// 	name[k] = '\0';
// 	val = get_env_value(ctx->sh->env, name);
// 	if (val)
// 		write_value(ctx, val);
// }

// static	void	process_change_all_var(t_varctx *ctx)
// {
// 	char	c;

// 	while (ctx->in[ctx->pos[0]])
// 	{
// 		toggle_quote(ctx);
// 		c = ctx->in[ctx->pos[0]];
// 		if ((c == '\'' && !ctx->double_s)
// 			|| (c == '"' && !ctx->single))
// 		{
// 			ctx->pos[0]++;
// 			continue ;
// 		}
// 		else if (c == '$' && !ctx->single
// 			&& ctx->in[ctx->pos[0] + 1]
// 			&& (ft_isalnum(ctx->in[ctx->pos[0] + 1])
// 				|| ctx->in[ctx->pos[0] + 1] == '_'))
// 			handle_var(ctx);
// 		else
// 			write_char(ctx);
// 	}
// }

// char	*change_all_var(const char *str, t_shell *sh)
// {
// 	t_varctx	ctx;
// 	int			len;

// 	if (!str)
// 		return (NULL);
// 	len = calculate_max_len(str, sh);
// 	ctx.out = ft_malloc(len + 1, 1);
// 	if (!ctx.out)
// 		return (NULL);
// 	ctx.in = str;
// 	ctx.sh = sh;
// 	ctx.single = false;
// 	ctx.double_s = false;
// 	ctx.pos[0] = 0;
// 	ctx.pos[1] = 0;
// 	process_change_all_var(&ctx);
// 	ctx.out[ctx.pos[1]] = '\0';
// 	return (ctx.out);
// }



static void    toggle_quote(t_varctx *ctx)
{
    char c = ctx->in[ctx->pos[0]];

    if (c == '\'' && !ctx->double_s)
    {
        ctx->single = !ctx->single;
        ctx->out[ctx->pos[1]++] = c;
        ctx->pos[0]++;
    }
    else if (c == '"' && !ctx->single)
    {
        ctx->double_s = !ctx->double_s;
        ctx->out[ctx->pos[1]++] = c;
        ctx->pos[0]++;
    }
}

static void    write_char(t_varctx *ctx)
{
    ctx->out[ctx->pos[1]++] = ctx->in[ctx->pos[0]++];
}

static void    handle_var(t_varctx *ctx)
{
    char    name[PATH_MAX];
    int     k = 0;
    int     i = ctx->pos[0] + 1;

    /* collect variable name */
    while ((ft_isalnum(ctx->in[i]) || ctx->in[i] == '_')
           && k < PATH_MAX - 1)
    {
        name[k++] = ctx->in[i++];
    }
    name[k] = '\0';

    /* advance input pointer past $ and name */
    ctx->pos[0] = i;

    /* write expanded value */
    write_value(ctx, get_env_value(ctx->sh->env, name));
}

char    *change_all_var(const char *str, t_shell *sh)
{
    t_varctx    ctx;
    int         maxlen;

    if (!str || !sh)
        return (NULL);

    /* pre-calc max buffer size */
    maxlen = calculate_max_len(str, sh) + 1;
    if (!(ctx.out = ft_malloc(maxlen, 1)))
        return (NULL);

    ctx.in = str;
    ctx.sh = sh;
    ctx.single = false;
    ctx.double_s = false;
    ctx.pos[0] = 0;
    ctx.pos[1] = 0;

    while (ctx.in[ctx.pos[0]])
    {
        /* handle quotes first: toggles state and copies quote char */
        if (ctx.in[ctx.pos[0]] == '\'' && !ctx.double_s)
        {
            toggle_quote(&ctx);
            continue;
        }
        if (ctx.in[ctx.pos[0]] == '"' && !ctx.single)
        {
            toggle_quote(&ctx);
            continue;
        }

        /* expand only when not inside single quotes */
        if (ctx.in[ctx.pos[0]] == '$' && !ctx.single
            && (ft_isalnum(ctx.in[ctx.pos[0] + 1]) || ctx.in[ctx.pos[0] + 1] == '_'))
        {
            handle_var(&ctx);
        }
        else
        {
            write_char(&ctx);
        }
    }

    ctx.out[ctx.pos[1]] = '\0';
    return (ctx.out);
}
