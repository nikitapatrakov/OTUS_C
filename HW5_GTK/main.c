#include <gtk/gtk.h>


static GListModel *create_list_model_cb(gpointer item, gpointer user_data)
{
    // Создаём список для хранения элементов собственного типа GtkDbRow
    GListStore *list_store = g_list_store_new(G_TYPE_STRING);
    g_list_store_append(list_store, "nikita");
    g_list_store_append(list_store, "anna");
    return G_LIST_MODEL(list_store);
}

static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
  g_print ("Hello World\n");
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *box;
  GtkWidget *col;

  col = gtk_column_view_new(NULL);
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

  box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_halign (box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign (box, GTK_ALIGN_CENTER);
  GtkColumnViewColumn *column = gtk_column_view_column_new ("Name",
  NULL);
  gtk_column_view_append_column(NULL, column);
  gtk_window_set_child (GTK_WINDOW (window), col);

  button = gtk_button_new_with_label ("Hello World");

  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  

  gtk_box_append (GTK_BOX (box), button);

  gtk_window_present (GTK_WINDOW (window));
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}