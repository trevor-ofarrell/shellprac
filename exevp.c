#include "holberton.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

extern char **__environ;

int __execvpe(const char *file, char *const argv[], char *const envp[])
{
	const char *p, *z, *path = pEnv("PATH");
	size_t l, k;
	char b;

	errno = ENOENT;
	if (!*file) return -1;

	if (_strchr(file, '/'))
		return execve(file, argv, envp);

	if (!path) path = "/usr/local/bin:/bin:/usr/bin";
	k = strnlen(file, NAME_MAX+1);
	if (k > NAME_MAX) {
		errno = ENAMETOOLONG;
		return -1;
	}
	l = strnlen(path, PATH_MAX-1)+1;

	for (p = path; ; p = z)
	{
		b[l + k + 1];
		z = __strchrnul(p, ':');
		if (z - p >= l)
		{
			if (!*z++)
				break;
			continue;
		}
		_memcpy(b, p, z-p);
		b[z - p] = '/';
		_memcpy(b + (z - p) + ( z > p), file, k + 1);
		execve(b, argv, envp);
	}
	return -1;
}

int execvp(const char *file, char *const argv[])
{
	return __execvpe(file, argv, __environ);
}
