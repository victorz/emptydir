/**************************************************************************
    emptydir - check if directories are empty or not.
    Copyright (C) 2010  Victor Zamanian

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **************************************************************************/

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>

/*
 * Returns 1 if the directory at `path' is empty.
 * Returns 0 if the directory at `path' is not empty.
 */
int emptydir(DIR* dp);

/*
 * Wrapper for fprintf that also prints out the error message
 * corresponding to the current value of `errno'. The error function
 * prints to stderr.
 */
int error(const char* format, ...);

/*
 * Prints usage information.
 */
void usage();

int main(int argc, char** argv)
{
	DIR* dp;
	int arg;
	char* prog_name;

	prog_name = argv[0];

	if (argc < 2) {
		usage();
		return 1;
	}

	for (arg = 1; arg < argc; arg++) {
		dp = opendir(argv[arg]);
		if (dp == NULL)
			error("%s: Error when opening `%s'", prog_name, argv[arg]);
		else
		{
			int result = emptydir(dp);
			/* No need to close the directory unless dp wasn't NULL. */
			closedir(dp);
			if (result)
				printf("`%s' is empty.\n", argv[arg]);
			else
				printf("`%s' is not empty.\n", argv[arg]);
		}
	}

	return 0;
}

int emptydir(DIR* dp)
{
	struct dirent* entry;
	size_t files = 0;
	
	do
	{
		/* Read next directory entry. */
		entry = readdir(dp);

		/* If there was an entry and it wasn't "." nor "..". */
		if (entry != NULL && strcmp(entry->d_name, ".") &&
		                      strcmp(entry->d_name, ".."))
			files++;
	}
	while (entry != NULL && files == 0);

	return !files;
}

int error(const char* format, ...)
{
	va_list arg;
	int printed;
	char* errmsg;

	/* Copy error message first. */
	{
		char* tmp;
		tmp = strerror(errno);
		errmsg = malloc((strlen(tmp) + 1)*sizeof(char));
		strcpy(errmsg, tmp);
	}

	va_start(arg, format);
	printed = vfprintf(stderr, format, arg);
	va_end(arg);

	printed += fprintf(stderr, ": %s\n", errmsg);

	free(errmsg);

	return printed;
}

void usage(const char* prog_name)
{
	fprintf(stderr, "%s DIRECTORY...", prog_name);
}
