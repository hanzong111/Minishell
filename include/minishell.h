/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojing-ha <ojing-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 12:37:05 by zah               #+#    #+#             */
/*   Updated: 2023/01/28 13:06:50 by ojing-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <termios.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "../libft_comb/includes/libft.h"
# include "../libft_comb/includes/ft_printf.h"
# include "../libft_comb/includes/get_next_line_bonus.h"

int		g_error;

typedef enum s_token_type
{
	TOKEN_WORD,
	TOKEN_IN,
	TOKEN_OUT,
	TOKEN_AIN,
	TOKEN_AOUT,
	TOKEN_PIPE,
	TOKEN_ERR
}	t_token_type;

typedef struct s_dlist
{
	void			*content;
	struct s_dlist	*prev;
	struct s_dlist	*next;
}	t_dlist;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
}	t_token;

/**
 * @brief The lexer struct. 
 * Contain the original string as input
 * and pointer to the unprocess string as current
 */
typedef struct s_lexer
{
	char	*input;
	char	*current;
}	t_lexer;

/**
 * @brief The expander struct.
 * Contain the original string and a int as tracker
 */
typedef struct s_expander
{
	char	*input;
	int		current;
}	t_expander;

/**
 * @brief The struct used to store encironment variable.
 * Contain two string, key the variable name, and the variable value.
 */
typedef struct s_env
{
	char	*key;
	char	*value;
}	t_env;

/**
 * @brief The struct used to store file information. 
 * File type is used to check ifthe file should be Append (A) or Trunc (T). 
 * Default file name is set as STDIN and STDOUT for infile and outfile. 
 */
typedef struct s_file
{
	char	file_type;
	char	*file_name;
}	t_file;

/**
* @brief The struct used to store info to allow program to be execute.
 * Have a double array storing all command including arguement. Two double
 * array of struct t_file storing infile and outfile information
 */
typedef struct s_command
{
	char	**full_command;
	t_file	**infile;
	t_file	**outfile;

	int		in_fd;
	int		out_fd;
	char	*temp_name;
}	t_command;

/**
 * @brief 
 * Store a linked list of environment struct which will be updated whenever
 * a variable has been exported
 */
typedef struct s_main
{
	t_dlist		*env_list;
	char		**envp;
	char		**built_in;

	pid_t		*pid;
	int			**pipe;
	int			last_pipe;
	int			counter;
}	t_main;

//Exit Function
void		ms_success_exit(t_main *main);
void		ms_error_exit(char *err_msg);

//String Utility Finction
int			ms_is_empty_string(char *str);
int			ms_char_match(char *str, char c);
int			ms_strcmp(char *s1, char *s2);
int			ms_check_enclosed_length(char *str);
char		*ms_strdup_length(char *str, int length);
char		*ms_create_empty_string(void);
char		*ms_strjoin_free(char *src, char *new);

//Array Utility Function
char		**ms_array_append(char **arr, char *new);
void		ms_del_array(char **arr);
char		**ms_dup_array(char **arr);

//Double Linked List Function
t_dlist		*ms_dlist_new(void *content);
void		ms_dlist_addback(t_dlist **list, t_dlist *node);
void		ms_dlist_clear(t_dlist **lst, void (*del)(void *));
void		ms_dlist_del_first(t_dlist **lst, void (*del)(void *));
void		ms_dlst_del_last(t_dlist **lst, void (*del)(void *));
void		ms_dlst_del_target(t_dlist **lst, t_dlist *target,
				void (*del)(void *));
int			ms_dlist_size(t_dlist *head);
t_dlist		*ms_dlst_last(t_dlist *head);
t_dlist		*ms_sort_list(t_dlist *lst, int (*cmp)(void *, void *));

//Signal Function
void		ms_init_sig_handler(void);

//Lexer and Expander
void		ms_process_input(char *input, t_main *main);
int			ms_get_split_length(t_lexer *lexer);
int			ms_check_enclosed_length(char *str);
char		*ms_expander(char *str, t_main *main);
int			expander_advanced(char *str);
int			get_expand_length(char *str);
char		*ms_intepret_string(t_expander *expander, char *str,
				int length, t_main *main);
char		*ms_intepret_quote(t_expander *expander, char *str,
				int length, t_main *main);
char		*ms_expand_string(char *str, int length, t_main *main);
char		*ms_append_quote(char *str);
int			ms_is_env_character(char c);
t_dlist		*ms_tokenized(char *str);
t_dlist		*ms_create_word_token(char *content);
char		*ms_token_trim(char *str);
char		*ms_tokenize_space(char *current, t_dlist **list);

//Parser
void		ms_parse_input(t_dlist *token_list, t_main *main);
t_file		*ms_init_tfile(void);
void		ms_set_tfile(t_file *target, char type, char *file_name);
t_file		**ms_append_tfile_array(t_file **files, char type, char *file_name);
t_command	*ms_init_command(void);
void		create_command(t_dlist *command_list, t_dlist *token, int length);
int			ms_get_command_length(t_dlist *token_list);
char		**ms_get_command(t_command *current, t_dlist *token_list,
				int length);
t_file		**ms_get_file_info(t_file **current, t_token *token);

//Built in command function
t_dlist		*ms_dup_env(char **envp);
void		ms_env_free(void *env);

//Free function
void		ms_free_token(void *token);
void		ms_free_command(void *content);
void		ms_free_tfile_array(t_file **files);
void		ms_env_free(void *env);

//Testing use Function, will be deleted
void		print_token_node(t_dlist *node);
void		print_token_list(t_dlist **list);
void		print_env_list(t_dlist **lst);
void		print_command_list(t_dlist **list);

//Path sort functions
char		**ft_path_extract(t_main *main);
char		*ft_path_check(char **paths, char *cmd);
char		**ft_pathcat(char **paths);
char		*ft_pathsort(t_main	*main, t_command *cmd);
char		*check_path(t_dlist *list);

//Execute functions
char		*ft_pathsort(t_main	*main, t_command *cmd);
char		**ms_lst_to_env(t_main *main);
void		ft_execve(t_main *main, t_command *cmd);
int			execve_builtin(t_main *main, t_command *cmd);

//Processes
void		free_temp(t_command *cmd);
int			lst_len(t_dlist *list);
void		get_error_value(void);
void		print_error(char *str, char *error_msg);
void		first_process(t_main *main, t_command *cmd);
void		middle_process(t_main *main, t_command *cmd);
void		last_process(t_main *main, t_command *cmd);
void		single_process(t_main *main, t_command *cmd);
void		sort_in_out(t_main *main, t_command *cmd, int in_fd, int out_fd);
void		ft_execute(t_main *main, t_command *cmd, int len);

//Here_doc functions
char		*ft_strcat(char *src, char *dst);
void		heredoc_execute(t_command *cmd, t_main *main);
void		here_doc(t_command *cmd, char *limiter, t_main *main);
void		get_temp_name(t_command *cmd, int temp_id);
int			compare(char *buf, char *limiter);
int			check(char *buf, char *limit, int len);
void		process(t_main *main, t_dlist **lst);
void		get_here_doc(t_dlist **list, t_main *main);

//Built in Commands
int			ms_get_built_in(char *command);
int			check_built_in(t_main *main, t_command *cmd);
void		ms_cmd_echo(t_command *cmd);
void		ms_cmd_cd(t_main *main, t_command *cmd);
void		ms_cmd_pwd(t_command *cmd);
void		ms_cmd_export(t_main *main, t_command *cmd);
void		ms_export_add(t_main *main, char **command);
void		ms_cmd_unset(t_main *main, t_command *cmd);
void		ms_cmd_env(t_main *main, t_command *cmd);
void		ms_cmd_exit(t_command *cmd);

#endif