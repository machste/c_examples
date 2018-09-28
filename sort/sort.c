#include <masc.h>


int main(int argc, char *argv[])
{
    int ret = 0;
    // Setup argument parser
    Argparse *ap = new(Argparse, path_basename(argv[0]),
            "Sort all lines of a file");
    argparse_add_arg(ap, "file", "FILE", NULL, argparse_file, "file to sort");
    // Parse command line arguments
    Map *args = argparse_parse(ap, argc, argv);
    File *f = map_get(args, "file");
    // Read all lines in
    List *lines = file_readlines(f);
    list_sort(lines, cmp);
    // Reopen file and write the sorted lines
    if(file_reopen(f, "w")) {
        file_writelines(f, lines);
    } else {
        fprint(stderr, "File '%s': %s!\n", file_path(f), file_err_msg(f));
        ret = -1;
    }
    // Cleanup
    delete_objs(ap, args);
    return ret;
}
