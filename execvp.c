
static void
DoExec(file, argv)
    char *file;			/* File to execute. */
    char **argv;		/* Arguments to the program. */
{
    execve(file, argv, environ);
    if (errno == ENOEXEC) {
	/*
	 * Attempt to execute the file as a shell script using
	 * the Bourne shell)
	 */
	register int i;
#define MAX_ARGS 1000
	char *newArgv[MAX_ARGS+1];

	for (i = 0; argv[i] != 0; i++) {
	    /* Empty loop body */
	}
	if (i >= MAX_ARGS) {
	    return;
	}
	newArgv[0] = "sh";
	newArgv[1] = file;
	for (i = 1; argv[i] != 0; i++) {
	    newArgv[i+1] = argv[i];
	}
	newArgv[i+1] = 0;
	execve("/sprite/cmds/sh", newArgv, environ);
    }
}
int
execvp(name, argv)
    char *name;			/* Name of file containing program to exec. */
    char **argv;		/* Array of arguments to pass to program. */
{
    char *path;
#define MAX_NAME_SIZE 1000
    char fullName[MAX_NAME_SIZE+1];
    register char *first, *last;
    int nameLength, size, noAccess;

    noAccess = 0;

    if (index(name, '/') != 0) {
	/*
	 * If the name specifies a path, don't search for it on the search path,
	 * just try and execute it.
	 */
	DoExec(name, argv);
	return -1;
    }

    path = getenv("PATH");
    if (path == 0) {
	path = "/sprite/cmds";
    }
    nameLength = strlen(name);
    for (first = path; ; first = last+1) {

	/*
	 * Generate the next file name to try.
	 */

	for (last = first; (*last != 0) && (*last != ':'); last++) {
	    /* Empty loop body. */
	}
	size = last-first;
	if ((size + nameLength + 2) >= MAX_NAME_SIZE) {
	    continue;
	}
	(void) strncpy(fullName, first, size);
	if (last[-1] != '/') {
	    fullName[size] = '/';
	    size++;
	}
	(void) strcpy(fullName + size, name);

	DoExec(fullName, argv);
	if (errno == EACCES) {
	    noAccess = 1;
	} else if (errno != ENOENT) {
	    break;
	}
	if (*last == 0) {
	    /*
	     * Hit the end of the path. We're done.
	     * If there existed a file by the right name along the search path,
	     * but its permissions were wrong, return FS_NO_ACCESS. Else return
	     * whatever we just got back.
	     */
	    if (noAccess) {
		errno = EACCES;
	    }
	    break;
	}
    }
    return -1;
}
