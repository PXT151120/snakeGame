#include "stdlib.h"
#include "stdio.h"
#include "dirent.h"
#include "string.h"
#include "unistd.h"
#include "limits.h"
#include "libgen.h"

/* Return full path to symlink of gamepad */
void UdevHelpI_FindGamePadPath(char *retPath)
{
    const char *l_BasePath = "/dev/input/by-id/";
    DIR *l_dir = opendir(l_BasePath);
    struct dirent *l_dirEntry;

    if (!l_dir)
    {
        perror("can not open /dev/input/by-id/\n");
        return;
    }

    while ((l_dirEntry = readdir(l_dir)) != (void*)0)
    {
        char l_FullPath[PATH_MAX] = "";
        if (strstr(l_dirEntry->d_name, "event-joystick"))
        {
            snprintf(l_FullPath, sizeof(l_FullPath), "%s%s", l_BasePath, l_dirEntry->d_name);

            // Map symlink to event path
            char l_eventPath[PATH_MAX];
            ssize_t len = readlink(l_FullPath, l_eventPath, sizeof(l_eventPath) - 1);
            if (-1 != len)
            {
                l_eventPath[len] = '\0';
                snprintf(retPath, PATH_MAX, "/dev/input/%s", basename(l_eventPath));
            }

        }
    }

    closedir(l_dir);
}


