/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmarkoun <hmarkoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 11:04:12 by hmarkoun          #+#    #+#             */
/*   Updated: 2025/11/11 17:36:58 by hmarkoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static void	*ft_free(char **to_free)
{
	if (to_free && *to_free)
	{
		free(*to_free);
		*to_free = NULL;
	}
	return (NULL);
}

static char	*ft_buffer_clean(char *saved_buffer)
{
	char	*new_line;
	size_t	line_pos;
	char	*new_buffer;

	new_line = ft_strchr(saved_buffer, '\n');
	if (!new_line)
		return (ft_free(&saved_buffer));
	new_line++;
	line_pos = ft_strlen(new_line);
	if (line_pos == 0)
		return (ft_free(&saved_buffer));
	new_buffer = malloc(line_pos + 1);
	if (!new_buffer)
		return (ft_free(&saved_buffer));
	ft_strlcpy(new_buffer, new_line, line_pos + 1);
	free(saved_buffer);
	return (new_buffer);
}

static char	*ft_line(char *saved_buffer)
{
	char	*line;
	size_t	i;
	size_t	end_line;

	if (!saved_buffer)
		return (NULL);
	end_line = 0;
	while (saved_buffer[end_line] && saved_buffer[end_line] != '\n')
		end_line++;
	if (saved_buffer[end_line] == '\n')
		end_line++;
	i = 0;
	line = malloc(end_line + 1);
	if (!line)
		return (NULL);
	while (i < end_line)
	{
		line[i] = saved_buffer[i];
		i++;
	}
	line[i] = 0;
	return (line);\
}

static char	*ft_read_and_join(int fd, char *saved_buffer, char *temp_buffer)
{
	ssize_t	bytes_read;
	char	*new_buffer;

	while (1)
	{
		bytes_read = read(fd, temp_buffer, (size_t)BUFFER_SIZE);
		if (bytes_read == -1)
			return (ft_free(&saved_buffer));
		if (bytes_read == 0)
			break ;
		temp_buffer[bytes_read] = 0;
		new_buffer = ft_strjoin(saved_buffer, temp_buffer);
		if (!new_buffer)
			return (ft_free(&saved_buffer));
		free(saved_buffer);
		saved_buffer = new_buffer;
		if (ft_strchr(saved_buffer, '\n'))
			break ;
	}
	return (saved_buffer);
}

char	*get_next_line(int fd)
{
	static char	*saved_buffer[1024];
	char		*temp_buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE >= INT_MAX)
		return (ft_free(&saved_buffer[fd]));
	if (read(fd, 0, 0) < 0)
		return (ft_free(&saved_buffer[fd]));
	if (!ft_strchr(saved_buffer[fd], '\n'))
	{
		temp_buffer = malloc((size_t)BUFFER_SIZE + 1);
		if (!temp_buffer)
			return (ft_free(&saved_buffer[fd]));
		saved_buffer[fd] = ft_read_and_join(fd, saved_buffer[fd], temp_buffer);
		free(temp_buffer);
	}
	line = ft_line(saved_buffer[fd]);
	if (!line)
		return (ft_free(&saved_buffer[fd]));
	saved_buffer[fd] = ft_buffer_clean(saved_buffer[fd]);
	return (line);
}
