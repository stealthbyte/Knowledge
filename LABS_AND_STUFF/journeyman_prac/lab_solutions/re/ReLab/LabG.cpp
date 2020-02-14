/********************************************************************
* Lab G
********************************************************************/
/**
 * @brief - The cmdline file will have the arguments in null terminated strings
 * In light of that you need to read the whole file and then replace the null
 * bytes with spaces to make it look like a normally formatted string
 *
 * @param str[in, out] - The string of data with possible null bytes
 * @param bytes_read[in] - The length of the string in bytes
 */
void replace_char(char *str, size_t len, char token, char replace)
{
    size_t i = 0;
    for(; i < len; i++)
    {
        if (str[i] == token)
        {
            str[i] = replace;
        }
    }
}

/**
 * @brief - Read in a file and strip out any unwanted characters
 *
 * @param path[in] - The path to the file
 * @param output[out] - The buffer that will hold the file data
 * @return - The number of bytes read
 */
size_t get_process_name(char *path, char *output)
{
    char name[1024] = { 0 };
    int i = 0;
    char *index = NULL;
    size_t bytes_read = 0;
    FILE *fp = NULL;

    fp = fopen(path, "rb");
    if (NULL == fp)
    {
        return bytes_read;
    }

    bytes_read = fread(name, sizeof(char), 1024, fp);
    if (bytes_read == 0)
    {
        goto CLEANUP;
    }
    else
    {
        index = name+6;
        while(*index != '\n') {
            *output = *index;
            index++;
            output++;
        }
        output[i] = '\0';

        //memcpy(output, name+8, bytes_read-8);
        //replace_char(output, bytes_read, '\0', ' ');
        //replace_char(output, bytes_read, '\n', '\0');
    }

CLEANUP:
    fclose(fp);
    return bytes_read;

}

char *getProcName(unsigned long pid)
{
    char path[1024] = { 0 };
    char *result = (char*)malloc(1024);
    size_t bytes_read = 0;

    // Check if cmd line file exists
    snprintf(path, sizeof(path), "/proc/%ld/status", pid);
    if (access(path, F_OK) == -1)
    {
        printf("Failed to access %s\n", path);
        return NULL;
    }

    // If get_cmdline returns a non-zero number then we are done
    bytes_read = get_process_name(path, result);
    if (0 != bytes_read)
    {
        return result;
    }
    else {
        return NULL;
    }
}

unsigned long *getProcList(unsigned long *pLength)
{
    DIR *dir = NULL;
    struct dirent *entry = { 0 };
    unsigned long cbAlloc = 10 * sizeof(unsigned long);
    unsigned long *pids = (unsigned long*)malloc(cbAlloc);
    *pLength = 0;

    printf("Opendir\n");
    if (!(dir = opendir("/proc")))
    {
        perror("opendir");
        return NULL;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        // Check to see if I need more memory
        if (*pLength == (cbAlloc/sizeof(unsigned long)))
        {
            cbAlloc *= 2;
            pids = (unsigned long*) realloc(pids, cbAlloc);
            if (!pids)
            {
                printf("Memory allocation failure for %ld bytes!\n", cbAlloc);
                return NULL;
            }
        }

        if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") == 0 ||
                strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }

            if (atoi(entry->d_name) == 0) {
                continue;
            }

            pids[*pLength] = (unsigned long)atoi(entry->d_name);
            *pLength = *pLength + 1;
            printf("%s - %d\n", entry->d_name, atoi(entry->d_name));
        }
    }
    closedir(dir);
    return pids;
}

//calls two subfuncs, to get list of pids and print names for each process
//good one for enums (maybe)
extern "C"
__attribute__((visibility("default")))
void ReLabG()
{
	unsigned long num_pids;
	unsigned long *pids = getProcList(&num_pids);

	if( pids )
	{
		for(unsigned long i=0; i < num_pids; i++)
		{
			if( !pids[i] )
				continue;

			char *sz = getProcName(pids[i]);
			if( sz )
			{
				printf("%4ld %s\n", pids[i], sz);
				free(sz);
			}
		}
		free(pids);
	}
}
