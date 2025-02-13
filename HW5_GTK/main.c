#include <glib.h>
#include <stdio.h>

void get_recursive_date(const gchar* a) {
    GDir *dir;
    GError *error = NULL;
    const gchar *filename;
    dir = g_dir_open(a, 0, &error);
    while (filename=g_dir_read_name(dir)){
        gchar *fullpath = g_build_filename(a, filename, NULL);
        printf("%s\n", filename);
        if (g_file_test(fullpath, G_FILE_TEST_IS_DIR)) {
            get_recursive_date(fullpath);
        }
    }
}

int main()
{   
    GDir *dir, *dir2;
    GError *error = NULL;
    const gchar *filename;

    gchar* a = g_get_current_dir();
    get_recursive_date(a);
    return 0;
}