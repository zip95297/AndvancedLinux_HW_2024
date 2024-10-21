#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

#define MAX_PATH 64

/*
struct stat {
    dev_t     st_dev;     // 文件所在设备的设备号
    ino_t     st_ino;     // 文件的索引节点号（inode）
    mode_t    st_mode;    // 文件的类型和权限
    nlink_t   st_nlink;   // 硬链接数
    uid_t     st_uid;     // 所有者的用户ID
    gid_t     st_gid;     // 所有者的组ID
    dev_t     st_rdev;    // 特殊设备文件的设备号
    off_t     st_size;    // 文件的总大小（字节）
    blksize_t st_blksize; // 文件系统的块大小
    blkcnt_t  st_blocks;  // 文件所占的块数
    time_t    st_atime;   // 最后访问时间
    time_t    st_mtime;   // 最后修改时间
    time_t    st_ctime;   // 最后状态更改时间
};
*/

char GetFileType(mode_t mode) {
    if (S_ISREG(mode)) {
        return '-';
    } else if (S_ISDIR(mode)) {
        return 'd';
    } else if (S_ISCHR(mode)) {
        return 'c';
    } else if (S_ISBLK(mode)) {
        return 'b';
    } else if (S_ISFIFO(mode)) {
        return 'p';
    } else if (S_ISLNK(mode)) {
        return 'l';
    } else if (S_ISSOCK(mode)) {
        return 's';
    } else {
        return '?';
    }
};

char *GetFileMode(mode_t mode) {
    static char mode_str[11];
    mode_str[0] = GetFileType(mode);
    mode_str[1] = (mode & S_IRUSR) ? 'r' : '-';
    mode_str[2] = (mode & S_IWUSR) ? 'w' : '-';
    mode_str[3] = (mode & S_IXUSR) ? 'x' : '-';
    mode_str[4] = (mode & S_IRGRP) ? 'r' : '-';
    mode_str[5] = (mode & S_IWGRP) ? 'w' : '-';
    mode_str[6] = (mode & S_IXGRP) ? 'x' : '-';
    mode_str[7] = (mode & S_IROTH) ? 'r' : '-';
    mode_str[8] = (mode & S_IWOTH) ? 'w' : '-';
    mode_str[9] = (mode & S_IXOTH) ? 'x' : '-';
    mode_str[10] = '\0';
    return mode_str;
};

int GetFileList(const char *path){
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    struct stat st;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.' || (entry->d_name[0] == '.' && entry->d_name[1] == '.')) {
            continue;
        }
        lstat(entry->d_name, &st);

        // file type & rights
        char *mode = GetFileMode(st.st_mode);
        // hard link count
        nlink_t nlink = st.st_nlink;
        // owner name
        uid_t uid = st.st_uid;
        char *owner = getpwuid(uid)->pw_name;
        // group name
        gid_t gid = st.st_gid;
        char *group = getgrgid(gid)->gr_name;
        // file size
        off_t size = st.st_size;
        // last modified time
        time_t mtime = st.st_mtime;
        char*  time = ctime(&mtime);
        time[strlen(time) - 1] = '\0';
        time = time + 4;
        // file name
        char *name = entry->d_name;

        printf("%s %2hu %s\t%s\t%5lld\t%s\t%s\n", mode, nlink, owner, group, size, time, name);
    }

    return 0;
};

int main(int argc, char *argv[]) {
    char cwd[MAX_PATH];
    if (getcwd(cwd, MAX_PATH) == NULL) {
        perror("getcwd");
        return 1;
    }
    GetFileList(cwd);

    return 0;
};