/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouassi <abouassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 11:31:44 by abouassi          #+#    #+#             */
/*   Updated: 2023/04/17 02:23:22 by abouassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H

# define MINISHELL_H

# include <stdio.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>

# include <unistd.h>
# include <stdlib.h>
# include <termios.h>
# include <errno.h>
# include <dirent.h>
# include <string.h>
# include <limits.h>

int	g_last_exit;

enum e_seps{
	end=1,
	pipe_op=2,
	or_op=3,
	and_op=4,
	o_curly=5,
	c_curly=6,
};

enum e_redirect{
	dless=7, /// here_doc
	less=8,  // infile
	great=9,  // oufile
	dgreat=10  // oufile expand
};

typedef struct s_redirect
{
	int						my_redirect;
	char					*here_lim;
	int						here_doc_expand;
	char					*filename;
	struct s_redirect	*next;
}				t_redirect;

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	int				here_doc;
	int				out_fd;
	int				in_fd;
	int				cheak_in;
	t_redirect		*list_of_red;
	int				in_of_next;
	int				sep;
	int				level;
	struct s_cmd	*prev;
	struct s_cmd	*next;
}				t_cmd;

typedef struct s_env
{
	int				cheak;
	char			*key;
	char			*value;
	char			*content;
	struct s_env	*next;
}	t_env;
typedef struct s_val
{
	char *small;
	int fd;
}	t_val;

typedef struct s_global
{
	t_cmd	*all_cmd;
	t_env	*env_list;
	t_list	*tokens;
	int		first_cmd;
}				t_global;

typedef struct s_expand_data
{
	t_list		**tokens;
	t_global	*all;
	int			*ind;
	int			to_split;
	int			*nbr_split;
	char		**arr_words;
	char		*arr;
	char		*arr_next;
}				t_expand_data;

//////////// execution //////
void cmd_env(t_env *env,int fd);
void p_error(char *str);
t_env	*ft_lstnew_mini(char *content);
void	ft_lstadd_back_mini(t_env **lst, t_env *new);
//int	ft_strcmp(const char *str1, const char *str2);
char	*ft_strchr( const char *str, int c);
char **var_val(char *env);
void	del_node(t_env **head,char *key);
void cmd_unset(char** arg,t_env **env);
void cmd_export(t_env **env,char **arg,int fd);
void	cmd_cd(char **path,t_env **env);
char *cmd_pwd(int fd,int k);
int check_key(char *env);
void cmd_exit(char **str);
void cmd_echo(char **cmd,int fd);
char *get_env(t_env *env,char *s,char *s2);
void print_order(t_env *env, t_val *val ,t_env *ptr2 ,t_env *ptr3);
int	ft_lstsize_mini(t_cmd *lst);
//----------------------------------------
int her_doc(t_redirect *red);
void set_her_doc(t_cmd *cmd);
char	*my_path(char *cmd1, char **env);
void	free_all(char **str, char *str2);
void	check_error2(char *str);
void	ft_puterr(char *str);
void	check_error(char *str, int n);
void	set_red(t_cmd *cmd);
void cmd_excution(t_global *all, char **env);
int  cheackis_buil(char *cmd);
void exucute_builtine(t_global *all,t_cmd *cmds);
void execution_mini(t_global *all,char **env);



/* parsing */

/* tokenization */

int		is_delimiter(char *s, int index);
int		line_tokenization(char *line, t_global *all);
char	*tokendup(char **line);
char	*tokendup_simple(char **line);
char	*tokendup_dquote(char **line, char c);
char	*ft_strjoin_free(char *s1, char *s2);

/* grammar */
void	parse_tokens(t_global *all, t_list *tokens);
void	ft_cmds_add_back(t_cmd **lst, t_cmd *new);
void	ft_redi_add_back(t_redirect **lst, t_redirect *new);
void	ft_env_add_back(t_env **lst, t_env *new);
void	init_cmd_elem(t_cmd **cmd_elem, int sep);
void	set_level(int o_parenth, int c_parenth, int *lvl, int *cmd_lvl);
void	expand_tokens(t_global *all, t_list *tokens);
void	skip_parenth(t_list **tokens, int *o_parenth, int *c_parenth);
void	fill_redir(t_global *all, t_list **tokens, t_redirect **redir_list);
void	fill_args(t_list **tokens, t_cmd **cmd_elem);

/* debugging */
void	show_tokens(t_list *tokens);
void	show_cmds(t_cmd *cmds);
void	show_redis(t_redirect *list);

/* filename expansion */
int		gllobing(t_list *tokens, int *nbr_matchs);
int		get_index(char *token);
void	get_last_index(char *token, int *ind);
int		begin_match(char **haystack, char *needle, int ind);
int		tail_match(char *haystack, char *needle, int ind);
char	*sub_match(char **haystack, char *needle, int ref, int len);
char	*remove_quotes(char *token, int quotes_nbr);
int		count_quotes(char *s);

/* param expansion */
void	param_expansion(t_list *tokens, t_global *all, int *nbr_split);
char	*search_value(char *token, t_expand_data *data, int i);
int		handle_split(char *sub, char *value, int i, t_expand_data *data);
int		handle_variable(char **token, char *sub, char *value, int i);

/* check_syntax */
int		is_sep(char *s);
int		is_redir(char *s);
int		is_curly(char *s);
int		check_syntax(t_list *tokens);
int		check_extra_parenth(t_list *tokens);
#endif
