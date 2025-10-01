#include "minishell.h"


static int  read_sign(const char **ptr, int *sign)
{
    const char  *str;

    str = *ptr;
    *sign = 1;
    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            *sign = -1;
        str++;
        if (!*str)
            return (0);
    }
    *ptr = str;
    return (1);
}

static void init_limits(int sign, long long *cutoff, int *last_limit)
{
    *cutoff = LLONG_MAX / 10LL;
    if (sign > 0)
        *last_limit = (int)(LLONG_MAX % 10LL);
    else
        *last_limit = (int)(LLONG_MAX % 10LL) + 1;
}

static int  llmin_edge(int sign, long long result, int digit, char next_ch, long long *out)
{
    long long    cutoff;
    int          last_limit;

    if (sign >= 0)
        return (0);
    cutoff = LLONG_MAX / 10LL;
    last_limit = (int)(LLONG_MAX % 10LL) + 1;
    if (result == cutoff && digit == last_limit && next_ch == '\0')
    {
        *out = LLONG_MIN;
        return (1);
    }
    return (0);
}


static int  will_overflow(long long result, int digit, int sign,
                          long long cutoff, int last_limit)
{
    if (result > cutoff)
        return (1);
    if (result == cutoff)
    {
        if (digit > last_limit)
            return (1);
    }
    (void)sign;
    return (0);
}


int ft_strtoll(const char *str, long long *out)
{
    int         sign;
    long long   result;
    long long   cutoff;
    int         last_limit;
    int         digit;

    if (!str || !out)
        return (0);
    if (!read_sign(&str, &sign))
        return (0);
    if (*str < '0' || *str > '9')
        return (0);

    init_limits(sign, &cutoff, &last_limit);
    result = 0;
    while (*str >= '0' && *str <= '9')
    {
        digit = *str - '0';
        if (llmin_edge(sign, result, digit, *(str + 1), out))
            return (1);
        if (will_overflow(result, digit, sign, cutoff, last_limit))
            return (0);
        result = result * 10 + digit;
        str++;
    }
    if (*str != '\0')
        return (0);
    if (sign > 0)
        *out = result;
    else
        *out = -result;
    return (1);
}

