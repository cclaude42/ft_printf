/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cclaude <cclaude@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 17:18:32 by cclaude           #+#    #+#             */
/*   Updated: 2019/10/29 16:58:24 by cclaude          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		is_end_flag(const char c)
{
	if (c == 'c' || c == 's' || c == 'p' || c == 'd' || c == 'i')
		return (1);
	if (c == 'u' || c == 'x' || c == 'X' || c == '%')
		return (1);
	return (0);
}

int		is_valid_flag(const char c)
{
	if (c == 'c' || c == 's' || c == 'p' || c == 'd' || c == 'i')
		return (1);
	if (c == 'u' || c == 'x' || c == 'X' || c == '%' || c == ' ')
		return (1);
	if (c == '-' || c == '.' || c == '*')
		return (1);
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int		ft_atoi_star(const char *s, int *i, va_list args)
{
	int	num;

	num = 0;
	if (s[*i] == '.')
		(*i)++;
	if (s[*i] == '*')
	{
		num = va_arg(args, unsigned int);
		return (num);
	}
	while (s[*i] >= '0' && s[*i] <= '9')
	{
		num = num * 10 + (s[*i] - 48);
		(*i)++;
	}
	(*i)--;
	return (num);
}

void	ft_neg_star(struct fl_gs *flags)
{
	if (flags->precision < 0)
	{
		flags->dot = 0;
		flags->precision = 0;
	}
	else if (flags->width < 0)
	{
		flags->minus = 1;
		flags->width = -flags->width;
	}
	return ;
}

void	flagger(const char *s, int *i, struct fl_gs *flags, va_list args)
{
	flags->minus = 0;
	flags->zero = 0;
	flags->dot = 0;
	flags->precision = 0;
	flags->width = 0;
	while (is_end_flag(s[*i]) == 0 && is_valid_flag(s[*i]))
	{
		if (s[*i] == '-')
			flags->minus = 1;
		else if (s[*i] == '0')
			flags->zero = 1;
		else if (s[*i] == '.')
		{
			flags->dot = 1;
			flags->precision = ft_atoi_star(s, i, args);
		}
		else if (s[*i] == '*' || (s[*i] >= '1' && s[*i] <= '9'))
			flags->width = ft_atoi_star(s, i, args);
		if (flags->precision < 0 || flags->width < 0)
			ft_neg_star(flags);
		(*i)++;
	}
	// printf("Flag list\n");
	// printf("Minus : %d\n", flags->minus);
	// printf("Zero : %d\n", flags->zero);
	// printf("Dot : %d\n", flags->dot);
	// printf("Precision : %d\n", flags->precision);
	// printf("Width : %d\n", flags->width);
}

int		func_branch(const char *s, int *i, va_list args)
{
	struct fl_gs	flags;
	int				printed;

	(*i)++;
	printed = 0;
	flagger(s, i, &flags, args);
	if (s[*i] == 'c')
		printed = ft_print_char(va_arg(args, int), flags);
	else if (s[*i] == 's')
		printed = ft_print_str(va_arg(args, char *), flags);
	else if (s[*i] == 'p')
		printed = ft_print_mem(va_arg(args, unsigned long), 1, flags);
	else if (s[*i] == 'd' || s[*i] == 'i')
		printed = ft_print_nbr(va_arg(args, int), flags);
	else if (s[*i] == 'u')
		printed = ft_print_uns(va_arg(args, unsigned int), flags);
	else if (s[*i] == 'x')
		printed = ft_print_hex(va_arg(args, unsigned int), flags);
	else if (s[*i] == 'X')
		printed = ft_print_hexcap(va_arg(args, unsigned int), flags);
	(*i)++;
	return (printed);
}

int		ft_printf(const char *string, ...)
{
	va_list	args;
	int		i;
	int		printed;

	va_start(args, string);
	i = 0;
	printed = 0;
	while (string[i] != '\0')
	{
		if (string[i] == '%')
			printed += func_branch(string, &i, args);
		else
		{
			write(1, &string[i++], 1);
			printed++;
		}
	}
	va_end(args);
	return (printed);
}
