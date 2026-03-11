// Copyright (c) Andrew Marin 2026

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>

void print_permissions(struct stat *sb) {
    printf("%c", (sb->st_mode & S_IRUSR) ? 'r' : '-');
    printf("%c", (sb->st_mode & S_IWUSR) ? 'w' : '-');
    printf("%c", (sb->st_mode & S_IXUSR) ? 'x' : '-');

    printf("%c", (sb->st_mode & S_IRGRP) ? 'r' : '-');
    printf("%c", (sb->st_mode & S_IWGRP) ? 'w' : '-');
    printf("%c", (sb->st_mode & S_IXGRP) ? 'x' : '-');

    printf("%c", (sb->st_mode & S_IROTH) ? 'r' : '-');
    printf("%c", (sb->st_mode & S_IWOTH) ? 'w' : '-');
    printf("%c", (sb->st_mode & S_IXOTH) ? 'x' : '-');

    printf("\n");
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: lab08 filename" << std::endl;
        return 0;
    }

    struct stat fileInfo;

    if (stat(argv[1], &fileInfo) == -1) {
        std::cerr << "Error: Unable to stat '" << argv[1] << "'" << std::endl;
        return 0;
    }

    std::cout << argv[1] << " permissions: ";
    print_permissions(&fileInfo);

    return 0;
}
