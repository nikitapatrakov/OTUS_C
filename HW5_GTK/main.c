#include <gtk/gtk.h>
#include <glib.h>

enum {
    COLUMN_NAME,
    NUM_COLUMNS
};

void TreeModel(const gchar *pathdir, GtkTreeStore *tree_store, GtkTreeIter *current, GtkTreeIter *parent)
{
    const gchar *filename;
    GDir *dir = g_dir_open(pathdir, 0, NULL);
    if (!dir) {
        g_printerr("не существуюший адрес: %s\n", pathdir);
        return;
    }

    while ((filename = g_dir_read_name(dir)) != NULL) {
        GtkTreeIter iter;
        gchar *full_path = g_build_filename(pathdir, filename, NULL);
        gtk_tree_store_append(tree_store, current, parent);
        gtk_tree_store_set(tree_store, current, COLUMN_NAME, g_strdup(filename), -1);

        if (g_file_test(full_path, G_FILE_TEST_IS_DIR)) {
            buildTreeModel(full_path, tree_store, &iter, current);
        }

        g_free(full_path);
    }

    g_dir_close(dir);
}

void renderTreeViewWidget(GtkWidget *window)
{
    const gchar *currentDir = g_get_current_dir();
    GtkTreeStore *tree_store = gtk_tree_store_new(NUM_COLUMNS, G_TYPE_STRING, G_TYPE_INT);
    GtkTreeIter current;
    buildTreeModel(currentDir, tree_store, &current, NULL);
    
    GtkWidget *tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(tree_store));
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes("dir", renderer, "text", COLUMN_NAME, NULL);

    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
    gtk_container_add(GTK_CONTAINER(window), tree_view);
}

static void activate(GtkApplication* app, gpointer user_data)
{
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "TreeView");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

    renderTreeViewWidget(window);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
    GtkApplication *app = gtk_application_new("org.example.TreeModelExample", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    const int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}