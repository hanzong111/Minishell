/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojing-ha <ojing-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 20:43:56 by ojing-ha          #+#    #+#             */
/*   Updated: 2023/01/28 12:58:17 by ojing-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execve(t_main *main, t_command *cmd)
{
	char	*final_path;
	char	*value;
	char	**current_envp;

	if (cmd->full_command[0] == NULL)
		exit (0);
	final_path = ft_pathsort(main, cmd);
	current_envp = ms_lst_to_env(main);
	if (access(final_path, F_OK) == 0)
		execve(final_path, cmd->full_command, current_envp);
	value = check_path(main->env_list);
	if (value == NULL)
		print_error(cmd->full_command[0], "No Such file or directory");
	else
		print_error(cmd->full_command[0], "Command not found");
	free(final_path);
}

int	execve_builtin(t_main *main, t_command *cmd)
{
	(void) main;
	if (cmd->full_command[0] == NULL)
		return (0);
	if (ms_get_built_in(cmd->full_command[0]) == 2
		|| ms_get_built_in(cmd->full_command[0]) == 4
		|| ms_get_built_in(cmd->full_command[0]) == 5
		|| ms_get_built_in(cmd->full_command[0]) == 7)
	{
		if (ms_get_built_in(cmd->full_command[0]) == 2)
			ms_cmd_cd(main, cmd);
		if (ms_get_built_in(cmd->full_command[0]) == 4)
			ms_cmd_export(main, cmd);
		if (ms_get_built_in(cmd->full_command[0]) == 5)
			ms_cmd_unset(main, cmd);
		if (ms_get_built_in(cmd->full_command[0]) == 7)
			ms_cmd_exit(cmd);
		return (1);
	}
	return (0);
}
